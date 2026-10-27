#include "llc_socket_server.h"

#include "llc_chrono.h"
#include "llc_sync.h"

#include <cstring>
#if defined(LLC_WINDOWS)
#	include <process.h>
typedef int llc_server_socklen_t;
#else
typedef socklen_t llc_server_socklen_t;
#endif

LLC_USING_TYPEINT();

static ::llc::err_t udpServerUpdateClients(::llc::SSocketServer & server) {
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
					if(client->Queue.Send.size() && ::llc::failed(::llc::connectionSendQueue(*client, cacheSent, cacheSend)))
						warning_printf("Failed to send queued data for client %u.", iClient);
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
					if(0 == client.get_ref() || INVALID_SOCKET == client->Socket.Handle || ::llc::CONNECTION_STATE_DISCONNECTED == client->State)
						continue;
					if(FD_ISSET(client->Socket.Handle, &sockets))
						if(::llc::failed(clientsToProcess.push_back(client)))
							warning_printf("Failed to queue readable UDP client %u.", iClient);
				}
			}
			for(u2_t iClient = 0; iClient < clientsToProcess.size(); ++iClient) {
				if(false == server.Listen)
					return 0;
				::llc::SSocketConnection & client = *clientsToProcess[iClient];
				for(;;) {
					sockaddr_in remote = {};
					llc_server_socklen_t remoteSize = (llc_server_socklen_t)sizeof(remote);
					::llc::SCommand command = {};
					const int peeked = ::recvfrom(client.Socket.Handle, (sc_t*)&command, (int)sizeof(command), MSG_PEEK, (sockaddr*)&remote, &remoteSize);
					if(peeked < 0) {
#if defined(LLC_WINDOWS)
						const int lastError = ::WSAGetLastError();
						if(lastError != WSAEMSGSIZE && lastError != WSAEWOULDBLOCK && lastError != WSAETIMEDOUT) {
							warning_printf("Could not receive client datagram. Error: %u.", (u2_t)lastError);
							::recvfrom(client.Socket.Handle, (sc_t*)&command, (int)sizeof(command), 0, 0, 0);
							if(lastError == WSAENETRESET || lastError == WSAENOTSOCK)
								client.State = ::llc::CONNECTION_STATE_DISCONNECTED;
							break;
						}
#else
						break;
#endif
					}
					::llc::SIPv4End address = {};
					::llc::tcpipAddressFromSockaddr(remote, address);
					if(client.Address != address) {
						warning_printf("Command received from an invalid address: %u.%u.%u.%u:%u.", LLC_IPV4_EXPAND(address));
						::recvfrom(client.Socket.Handle, (sc_t*)&command, (int)sizeof(command), 0, 0, 0);
					}
					else {
						if(::llc::failed(::llc::connectionHandleCommand(client, command, receiveBuffer)))
							warning_printf("Error processing command from: %u.%u.%u.%u:%u.", LLC_IPV4_EXPAND(address));
						if(INVALID_SOCKET != client.Socket.Handle)
							::recvfrom(client.Socket.Handle, (sc_t*)&command, (int)sizeof(command), 0, 0, 0);
					}
					if(INVALID_SOCKET == client.Socket.Handle)
						break;
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

static ::llc::err_t udpServerRecycleClient(::llc::SSocketServer & server, ::llc::pobj<::llc::SSocketConnection> & client) {
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

static ::llc::err_t udpServerAcceptClient(::llc::SSocketServer & server, ::llc::SCommand command, const sockaddr_in & remote) {
	::llc::pobj<::llc::SSocketConnection> client;
	{
		::llc::lock_guard lock(server.Mutex);
		const s2_t recycled = ::udpServerRecycleClient(server, client);
		::llc::tcpipAddressFromSockaddr(remote, client->Address);
		command.Type = ::llc::ENDPOINT_COMMAND_TYPE_RESPONSE;
		::llc::pobj<::llc::SMessage> response;
		response->Time = ::llc::timeCurrentInUs();
		response->Command = command;
		llc_necs(client->Queue.Send.push_back(response));
		client->LastPing = client->FirstPing = ::llc::timeCurrentInUs();
		if_true_fef(INVALID_SOCKET == (client->Socket.Handle = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)), "%s", "Could not create client socket.");
		client->State = ::llc::CONNECTION_STATE_HANDSHAKE;
		if(-1 == recycled)
			llc_necs(server.Clients.push_back(client));
	}
	return 0;
}

static ::llc::err_t udpServerListenTick(::llc::SSocketServer & server, const sockaddr_in & local) {
	server.Socket.close();
	if_true_fef(INVALID_SOCKET == (server.Socket.Handle = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)), "%s", "Could not create listener socket.");
	if_true_fef(0 != ::bind(server.Socket.Handle, (const sockaddr*)&local, sizeof(local)), "%s", "Could not bind listener socket.");
	llc_server_socklen_t remoteSize = (llc_server_socklen_t)sizeof(sockaddr_in);
	sockaddr_in remote = {};
	::llc::SCommand command = {};
	const int received = ::recvfrom(server.Socket.Handle, (sc_t*)&command, (int)sizeof(command), MSG_PEEK, (sockaddr*)&remote, &remoteSize);
	if_true_fef(received <= 0, "%s", "Could not receive listener command.");
	if_true_fef(::llc::ENDPOINT_COMMAND_TYPE_REQUEST != command.Type, "Invalid message type: %u.", (u2_t)command.Type);
	if_true_fef(::llc::ENDPOINT_COMMAND_CONNECT != command.Command && ::llc::ENDPOINT_COMMAND_DISCONNECT != command.Command, "Invalid command: %u.", (u2_t)command.Command);
	if(::llc::ENDPOINT_COMMAND_CONNECT == command.Command)
		return ::udpServerAcceptClient(server, command, remote);
	if(false == server.Listen)
		return 0;
	return -1;
}

static ::llc::err_t udpServerRun(::llc::SSocketServer & server) {
	server.Listen = true;
	llc_necs(::llc::tcpipAddress(server.Address.Port, server.AdapterIndex, ::llc::TRANSPORT_PROTOCOL_UDP, server.Address));
	sockaddr_in local = {};
	llc_necs(::llc::tcpipAddressToSockaddr(server.Address, local));
#if defined(LLC_WINDOWS)
	_beginthread([](void * instance) { ::udpServerUpdateClients(*(::llc::SSocketServer*)instance); }, 0, &server);
#else
#	error "Not implemented."
#endif
	while(server.Listen)
		if(::llc::failed(::udpServerListenTick(server, local)) && server.Listen)
			warning_printf("UDP listener tick failed on %u.%u.%u.%u:%u.", LLC_IPV4_EXPAND(server.Address));
	server.Socket.close();
	return 0;
}

static void udpServerThread(void * instance) {
	::llc::SSocketServer & server = *(::llc::SSocketServer*)instance;
	if(::llc::failed(::udpServerRun(server)))
		error_printf("%s", "UDP server stopped after an error.");
	server.Listen = false;
	server.Socket.close();
}

::llc::err_t llc::serverStart(SSocketServer & server, uint16_t port, int16_t adapterIndex) {
	server.Listen = true;
	server.Address.Port = port;
	server.AdapterIndex = adapterIndex;
#if defined(LLC_WINDOWS)
	_beginthread(::udpServerThread, 0, &server);
	return 0;
#else
	return -1;
#endif
}

::llc::err_t llc::serverStop(SSocketServer & server) {
	server.Listen = false;
	sockaddr_in local = {};
	llc_necs(::llc::tcpipAddressToSockaddr(server.Address, local));
	const SCommand command = {ENDPOINT_COMMAND_DISCONNECT, ENDPOINT_COMMAND_TYPE_REQUEST};
	if(INVALID_SOCKET != server.Socket.Handle)
		::sendto(server.Socket.Handle, (const sc_t*)&command, (int)sizeof(command), 0, (sockaddr*)&local, sizeof(local));
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
