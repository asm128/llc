#include "llc_socket_server.h"

#include "llc_chrono.h"
#include "llc_sync.h"

#include <cstring>
#if defined(LLC_WINDOWS)
#	include <process.h>
typedef int llc_tcp_server_socklen_t;
#else
typedef socklen_t llc_tcp_server_socklen_t;
#endif

LLC_USING_TYPEINT();

static ::llc::err_t tcpServerSend(SOCKET socket, const ::llc::vcu0_t & bytes) {
	u2_t offset = 0;
	while(offset < bytes.size()) {
		const int sent = ::send(socket, (const sc_t*)bytes.begin() + offset, (int)(bytes.size() - offset), 0);
		if(sent <= 0)
			return -1;
		offset += (u2_t)sent;
	}
	return 0;
}

static ::llc::err_t tcpServerReceive(SOCKET socket, u0_t * bytes, u2_t size) {
	u2_t offset = 0;
	while(offset < size) {
		const int received = ::recv(socket, (sc_t*)bytes + offset, (int)(size - offset), 0);
		if(received <= 0)
			return -1;
		offset += (u2_t)received;
	}
	return 0;
}

static ::llc::err_t tcpServerReceiveFrame(SOCKET socket, ::llc::au0_t & frame) {
	::llc::SPayloadHeader header = {};
	llc_necs(::tcpServerReceive(socket, (u0_t*)&header, sizeof(header)));
	llc_necs(frame.resize((u2_t)sizeof(header) + header.Size));
	::memcpy(frame.begin(), &header, sizeof(header));
	return header.Size ? ::tcpServerReceive(socket, frame.begin() + sizeof(header), header.Size) : 0;
}

static ::llc::err_t tcpServerUpdateClients(::llc::SSocketServer & server) {
	::llc::apobj<::llc::SSocketConnection>	clientsToProcess;
	::llc::au0_t						receiveBuffer;
	::llc::apobj<::llc::SMessage>		cacheSent;
	::llc::apobj<::llc::SMessage>		cacheSend;
	while(server.Listen) {
		::llc::sleep(1);
		u2_t totalClientCount = 0;
		{
			::llc::lock_guard lock(server.Mutex);
			totalClientCount = server.Clients.size();
		}
		const u2_t remainder = totalClientCount % FD_SETSIZE;
		const u2_t stageCount = totalClientCount / FD_SETSIZE + (remainder ? 1U : 0U);
		for(u2_t iStage = 0; iStage < stageCount; ++iStage) {
			const u2_t offsetClient = iStage * FD_SETSIZE;
			const u2_t stageClientCount = (iStage == stageCount - 1 && remainder) ? remainder : FD_SETSIZE;
			fd_set sockets = {};
			FD_ZERO(&sockets);
			{
				::llc::lock_guard lock(server.Mutex);
				const u2_t endClient = ::llc::min(server.Clients.size(), offsetClient + stageClientCount);
				for(u2_t iClient = offsetClient; iClient < endClient; ++iClient) {
					::llc::pobj<::llc::SSocketConnection> client = server.Clients[iClient];
					if(0 == client.get_ref() || INVALID_SOCKET == client->Socket.Handle || ::llc::CONNECTION_STATE_DISCONNECTED == client->State)
						continue;
					if(client->Queue.Send.size()) {
						const ::llc::FConnectionSend send = [&client](const ::llc::vcu0_t & bytes) { return ::tcpServerSend(client->Socket.Handle, bytes); };
						if(::llc::failed(::llc::connectionSendQueue(*client, cacheSent, cacheSend, send)))
							warning_printf("Failed to send queued data for TCP client %u.", iClient);
					}
					FD_SET(client->Socket.Handle, &sockets);
				}
			}
			if(false == server.Listen)
				return 0;
			timeval waitTime = {0, 1000};
			const int ready = ::select(0, &sockets, 0, 0, &waitTime);
			if(ready <= 0)
				continue;
			clientsToProcess.clear();
			{
				::llc::lock_guard lock(server.Mutex);
				const u2_t endClient = ::llc::min(server.Clients.size(), offsetClient + stageClientCount);
				for(u2_t iClient = offsetClient; iClient < endClient; ++iClient) {
					::llc::pobj<::llc::SSocketConnection> client = server.Clients[iClient];
					if(client.get_ref() && INVALID_SOCKET != client->Socket.Handle && FD_ISSET(client->Socket.Handle, &sockets))
						if(::llc::failed(clientsToProcess.push_back(client)))
							warning_printf("Failed to queue readable TCP client %u.", iClient);
				}
			}
			for(u2_t iClient = 0; iClient < clientsToProcess.size(); ++iClient) {
				::llc::SSocketConnection & client = *clientsToProcess[iClient];
				for(;;) {
					::llc::SCommand command = {};
					const int peeked = ::recv(client.Socket.Handle, (sc_t*)&command, sizeof(command), MSG_PEEK);
					if(peeked <= 0) {
						client.State = ::llc::CONNECTION_STATE_DISCONNECTED;
						client.Socket.close();
						break;
					}
					const ::llc::FConnectionReceive receive = [&client](::llc::au0_t & frame) { return ::tcpServerReceiveFrame(client.Socket.Handle, frame); };
					const ::llc::err_t handled = ::llc::connectionHandleCommand(client, command, receive, receiveBuffer);
					if(::llc::ENDPOINT_COMMAND_PAYLOAD != command.Command) {
						::llc::SCommand consumed = {};
						if(::llc::failed(::tcpServerReceive(client.Socket.Handle, (u0_t*)&consumed, sizeof(consumed)))) {
							client.State = ::llc::CONNECTION_STATE_DISCONNECTED;
							client.Socket.close();
							break;
						}
					}
					if(::llc::failed(handled)) {
						warning_printf("Error processing TCP client command: %u.", (u2_t)command.Command);
						break;
					}
					if(::llc::ENDPOINT_COMMAND_DISCONNECT == command.Command) {
						client.Socket.close();
						break;
					}
					fd_set more = {};
					FD_ZERO(&more);
					FD_SET(client.Socket.Handle, &more);
					timeval noWait = {0, 0};
					if(::select(0, &more, 0, 0, &noWait) <= 0)
						break;
				}
			}
		}
	}
	return 0;
}

static ::llc::err_t tcpServerRecycleClient(::llc::SSocketServer & server, ::llc::pobj<::llc::SSocketConnection> & client) {
	for(u2_t iClient = 0; iClient < server.Clients.size(); ++iClient) {
		client = server.Clients[iClient];
		if(0 == client.get_ref())
			return iClient;
		const s3_t elapsed = ::llc::timeCurrentInUs() - client->LastPing;
		if(elapsed > server.Timeout || INVALID_SOCKET == client->Socket.Handle || ::llc::CONNECTION_STATE_DISCONNECTED == client->State) {
			client->State = ::llc::CONNECTION_STATE_DISCONNECTED;
			client->Socket.close();
			client->KeyPing = client->LastPing = client->FirstPing = 0;
			{
				::llc::lock_guard lock(client->Queue.MutexReceive);
				client->Queue.Received.clear();
			}
			{
				::llc::lock_guard lock(client->Queue.MutexSend);
				client->Queue.Send.clear();
				client->Queue.Sent.clear();
			}
			return iClient;
		}
		client = {};
	}
	return -1;
}

static ::llc::err_t tcpServerAcceptClient(::llc::SSocketServer & server, SOCKET accepted, const sockaddr_in & remote) {
	::llc::SCommand command = {};
	if(::llc::failed(::tcpServerReceive(accepted, (u0_t*)&command, sizeof(command))))
		return -1;
	if_true_fef(::llc::ENDPOINT_COMMAND_TYPE_REQUEST != command.Type || ::llc::ENDPOINT_COMMAND_CONNECT != command.Command || command.Packed, "%s", "Invalid TCP connect request.");
	::llc::pobj<::llc::SSocketConnection> client;
	{
		::llc::lock_guard lock(server.Mutex);
		const s2_t recycled = ::tcpServerRecycleClient(server, client);
		client->Socket.Handle = accepted;
		::llc::tcpipAddressFromSockaddr(remote, client->Address);
		command.Type = ::llc::ENDPOINT_COMMAND_TYPE_RESPONSE;
		::llc::pobj<::llc::SMessage> response;
		response->Time = ::llc::timeCurrentInUs();
		response->Command = command;
		llc_necs(client->Queue.Send.push_back(response));
		client->LastPing = client->FirstPing = ::llc::timeCurrentInUs();
		client->State = ::llc::CONNECTION_STATE_HANDSHAKE;
		if(-1 == recycled)
			llc_necs(server.Clients.push_back(client));
	}
	return 0;
}

static ::llc::err_t tcpServerRun(::llc::SSocketServer & server) {
	server.Listen = true;
	llc_necs(::llc::tcpipAddress(server.Address.Port, server.AdapterIndex, ::llc::TRANSPORT_PROTOCOL_TCP, server.Address));
	sockaddr_in local = {};
	llc_necs(::llc::tcpipAddressToSockaddr(server.Address, local));
	if_true_fef(INVALID_SOCKET == (server.Socket.Handle = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)), "%s", "Could not create TCP listener socket.");
	if_true_fef(0 != ::bind(server.Socket.Handle, (const sockaddr*)&local, sizeof(local)), "%s", "Could not bind TCP listener socket.");
	if_true_fef(0 != ::listen(server.Socket.Handle, SOMAXCONN), "%s", "Could not listen on TCP socket.");
#if defined(LLC_WINDOWS)
	_beginthread([](void * instance) { ::tcpServerUpdateClients(*(::llc::SSocketServer*)instance); }, 0, &server);
#else
#	error "Not implemented."
#endif
	while(server.Listen) {
		sockaddr_in remote = {};
		llc_tcp_server_socklen_t remoteSize = (llc_tcp_server_socklen_t)sizeof(remote);
		const SOCKET accepted = ::accept(server.Socket.Handle, (sockaddr*)&remote, &remoteSize);
		if(INVALID_SOCKET == accepted) {
			if(false == server.Listen)
				break;
			warning_printf("%s", "Could not accept TCP client.");
			continue;
		}
		if(::llc::failed(::tcpServerAcceptClient(server, accepted, remote))) {
			::closesocket(accepted);
			warning_printf("%s", "Rejected invalid TCP client connection.");
		}
	}
	server.Socket.close();
	return 0;
}

static void tcpServerThread(void * instance) {
	::llc::SSocketServer & server = *(::llc::SSocketServer*)instance;
	if(::llc::failed(::tcpServerRun(server)))
		error_printf("%s", "TCP server stopped after an error.");
	server.Listen = false;
	server.Socket.close();
}

::llc::err_t llc::serverStart(SSocketServer & server, uint16_t port, int16_t adapterIndex) {
	server.Listen = true;
	server.Address.Port = port;
	server.AdapterIndex = adapterIndex;
#if defined(LLC_WINDOWS)
	_beginthread(::tcpServerThread, 0, &server);
	return 0;
#else
	return -1;
#endif
}

::llc::err_t llc::serverStop(SSocketServer & server) {
	server.Listen = false;
	server.Socket.close();
	::llc::lock_guard lock(server.Mutex);
	for(u2_t iClient = 0; iClient < server.Clients.size(); ++iClient)
		if(server.Clients[iClient].get_ref())
			server.Clients[iClient]->Socket.close();
	return 0;
}

::llc::err_t llc::serverPayloadCollect(SSocketServer & server, aobj<apobj<SMessage>> & receivedMessages) {
	::llc::lock_guard lock(server.Mutex);
	llc_necs(receivedMessages.resize(server.Clients.size()));
	for(u2_t iClient = 0; iClient < server.Clients.size(); ++iClient) {
		::llc::pobj<::llc::SSocketConnection> client = server.Clients[iClient];
		if(client.get_ref())
			llc_necs(::llc::connectionPayloadCollect(*client, receivedMessages[iClient]));
	}
	return 0;
}
