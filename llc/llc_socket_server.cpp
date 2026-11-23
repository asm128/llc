// http://www.gomorgan89.com
#include "llc_noise.h"
#include "llc_connection_ip.h"
#include "llc_array.h"
#include "llc_view_stream.h"
#include "llc_chrono.h"
#include "llc_sync.h"
#include "llc_socket_server.h"

#include <cstdlib>
#if defined(LLC_WINDOWS)
#	include <process.h>
	tydf	int			socklen_t;
#else
#	include <cerrno>
#	define WSAEMSGSIZE		EMSGSIZE
#	define WSAENETRESET		ENETRESET
#	define WSAENOTSOCK		ENOTSOCK
#	define _beginthread(callback, stackSize, argument)	::std::thread((callback), (argument)).detach()
	sttc inln int WSAGetLastError() { return errno; }
#endif

LLC_USING_TYPEINT();

sttc	::llc::err_t	updateClients			(llc::SSocketServer & serverInstance)		{
	::llc::apobj<::llc::SSocketConnection>	clientsToProcess;
	::llc::au0_t						receiveBuffer;
	::llc::apobj<::llc::SMessage>		cacheSent						= {};
	::llc::apobj<::llc::SMessage>		cacheSend						= {};
	while(serverInstance.Listen) {
		::llc::sleep(1);
		u2_t							totalClientCount				= serverInstance.Sockets.size();
		{
			::llc::lock_guard					lock								(serverInstance.Mutex);
			totalClientCount				= serverInstance.Sockets.size();
		}
		timeval								wait_time							= {0, 1};
		u2_t							remainder							= totalClientCount % 64;
		u2_t							stageCount							= ::llc::round_up(totalClientCount, 64U);
		fd_set								sockets								= {};
		for(u2_t iStage = 0; iStage < stageCount; ++iStage) {
			u2_c						offsetClient						= iStage * 64;
#if defined(LLC_WINDOWS)
			memset(&sockets, -1, szof(fd_set));
			sockets.fd_count				= 0;
#else
			FD_ZERO(&sockets);
#endif
			u2_t							stageClientCount					= (iStage == (stageCount - 1) && remainder) ? remainder : 64;
			{
				::llc::lock_guard					lock								(serverInstance.Mutex);
				for(u2_t iClient = 0, actualCount = ::llc::min(serverInstance.Sockets.size(), stageClientCount); iClient < actualCount; ++iClient) {
					::llc::pobj<::llc::SSocketConnection>	pclient							= serverInstance.Sockets[offsetClient + iClient];
					if(0 == pclient.get_ref() || pclient->IdConnection >= serverInstance.Clients.size() || 0 == serverInstance.Clients[pclient->IdConnection].get_ref()) {
						if(pclient.get_ref()) {
							pclient->IdConnection = ::llc::u2_t(-1);
							pclient->Socket.close();
						}
						continue;
					}
					::llc::SConnection					& connection						= *serverInstance.Clients[pclient->IdConnection];
					if(pclient->Socket == INVALID_SOCKET || connection.State == ::llc::CONNECTION_STATE_DISCONNECTED)
						continue;
					if(connection.Queue.Send.size())
						if_fail_e(::llc::connectionSendQueue(connection, *pclient, cacheSent, cacheSend));
#if defined(LLC_WINDOWS)
					sockets.fd_array[sockets.fd_count]		= pclient->Socket;
					if(sockets.fd_array[sockets.fd_count] != INVALID_SOCKET)
						++sockets.fd_count;
#else
					if(pclient->Socket != INVALID_SOCKET)
						FD_SET(pclient->Socket, &sockets);
#endif
				}
			}
			if(false == serverInstance.Listen)
				return 0;
			{
				clientsToProcess.clear();
				{
					::llc::lock_guard				lock								(serverInstance.Mutex);
					if_true_bi(0 == serverInstance.Sockets.size());
					for(u2_t sd = 0
#if defined(LLC_WINDOWS)
						; sd < sockets.fd_count
#else
						; sd < stageClientCount
#endif
						; ++sd) {
						for(u2_t iClient = 0, countCli = ::llc::min(serverInstance.Sockets.size(), stageClientCount); iClient < countCli; ++iClient) {
							::llc::pobj<::llc::SSocketConnection>		pclient								= serverInstance.Sockets[offsetClient + iClient];
							if(0 == pclient.get_ref() || pclient->IdConnection >= serverInstance.Clients.size() || 0 == serverInstance.Clients[pclient->IdConnection].get_ref() || pclient->Socket == INVALID_SOCKET || serverInstance.Clients[pclient->IdConnection]->State == ::llc::CONNECTION_STATE_DISCONNECTED)
								continue;
#if defined(LLC_WINDOWS)
							if(sockets.fd_array[sd] == pclient->Socket) {
#else
							if(sd == iClient && FD_ISSET(pclient->Socket, &sockets)) {
#endif
								if_fail_fe(clientsToProcess.push_back(pclient));
								break;
							}
						}
					}
				}
				for(u2_t iClient = 0; iClient < clientsToProcess.size(); ++iClient) {
					if(false == serverInstance.Listen)
						return 0;
					::llc::SSocketConnection					& clientSocket						= *clientsToProcess[iClient];
					if(clientSocket.IdConnection >= serverInstance.Clients.size() || 0 == serverInstance.Clients[clientSocket.IdConnection].get_ref()) {
						clientSocket.IdConnection = ::llc::u2_t(-1);
						clientSocket.Socket.close();
						continue;
					}
					::llc::SConnection						& client							= *serverInstance.Clients[clientSocket.IdConnection];
#if defined(LLC_WINDOWS)
					FD_ZERO(sockets.fd_array);
					sockets.fd_array[0]						= clientSocket.Socket;
					sockets.fd_count						= 1;
					::select(0, &sockets, 0, 0, &wait_time);
					while(sockets.fd_count > 0) {
#else
					FD_ZERO(&sockets);
					FD_SET(clientSocket.Socket, &sockets);
					wait_time = {0, 1};
					int socketsReady = ::select(clientSocket.Socket + 1, &sockets, 0, 0, &wait_time);
					while(0 < socketsReady && FD_ISSET(clientSocket.Socket, &sockets)) {
#endif
						sockaddr_in									sa_client							= {};						// Information about the client
						socklen_t								sa_length							= sizeof(sockaddr_in);	// Length of client struct
						::llc::SCommand							command								= {};						// Where to store received data
						int											bytes_received						= 0;
						if (::llc::failed(bytes_received = ::recvfrom(clientSocket.Socket, (sc_t*)&command, (int)sizeof(::llc::SCommand), MSG_PEEK, (sockaddr*)&sa_client, &sa_length))) {
							u2_t									lastError							= ::WSAGetLastError();
							if(lastError != WSAEMSGSIZE) {
								warning_printf("%s", "'lastError != WSAEMSGSIZE' - Could not receive datagram.");
								::recvfrom(clientSocket.Socket, (sc_t*)&command, (int)szof(::llc::SCommand), 0, 0, 0);
								if(lastError == WSAENETRESET || lastError == WSAENOTSOCK)
									client.State = ::llc::CONNECTION_STATE_DISCONNECTED;
								break;
							}
						}
						::llc::SIPv4End								address								= clientSocket.Address;
						if(::llc::TRANSPORT_PROTOCOL_UDP == serverInstance.Protocol)
							::llc::tcpipAddressFromSockaddr(sa_client, address);
						if(clientSocket.Address != address) {
							warning_printf("Command received from an invalid address: %u.%u.%u.%u:%u.", LLC_IPV4_EXPAND(address));
							::recvfrom(clientSocket.Socket, (sc_t*)&command, (int)szof(::llc::SCommand), 0, 0, 0);
							continue;
						}
						if_fail_ef(::llc::connectionHandleCommand(client, clientSocket, command, receiveBuffer), "Error processing command from: %u.%u.%u.%u:%u.", LLC_IPV4_EXPAND(address));
						if(INVALID_SOCKET != clientSocket.Socket) {
							if(::llc::TRANSPORT_PROTOCOL_TCP == serverInstance.Protocol && ::llc::ENDPOINT_COMMAND_PAYLOAD == command.Command)
								::recv(clientSocket.Socket, (sc_t*)receiveBuffer.begin(), (int)receiveBuffer.size(), MSG_WAITALL);
							else
								::recvfrom(clientSocket.Socket, (sc_t*)&command, (int)szof(::llc::SCommand), 0, 0, 0);
						}
#if defined(LLC_WINDOWS)
						::select(0, &sockets, 0, 0, &wait_time);
#else
						FD_ZERO(&sockets);
						FD_SET(clientSocket.Socket, &sockets);
						wait_time = {0, 1};
						socketsReady = ::select(clientSocket.Socket + 1, &sockets, 0, 0, &wait_time);
#endif
					}
				}
			}
		}
	}
	return 0;
}

sttc	::llc::err_t	recycleClient					(::llc::SSocketServer& serverInstance, ::llc::pobj<::llc::SConnection> & pClient, ::llc::pobj<::llc::SSocketConnection> & pSocket)		{
	for(u2_t iClient = 0, countClients = serverInstance.Clients.size(); iClient < countClients; ++iClient) {
		pClient										= serverInstance.Clients[iClient];
		pSocket										= serverInstance.Sockets[iClient];
		::llc::SConnection							& client						= *pClient;
		s3_t											limitTime						= (::llc::timeCurrentInUs() - client.LastPing);
		info_printf("LimitTime : %lli.", limitTime);
		if(limitTime > serverInstance.Timeout || pSocket->Socket == INVALID_SOCKET || client.State == ::llc::CONNECTION_STATE_DISCONNECTED) {
			client.State								= ::llc::CONNECTION_STATE_DISCONNECTED;
			pSocket->Socket.close();
			pSocket->IdConnection					= ::llc::u2_t(-1);
			client.KeyPing								=
			client.LastPing								=
			client.FirstPing							= 0;
			{
				::llc::lock_guard								lockRecv						(client.Queue.MutexReceive);
				client.Queue.Received	.clear();
			}
			{
				::llc::lock_guard								lockSend						(client.Queue.MutexSend);
				client.Queue.Send		.clear();
				client.Queue.Sent		.clear();
			}
			return iClient;
		}
		else {
			pClient										= {};
			pSocket										= {};
		}
	}
	return -1;
}

sttc	::llc::err_t	serverAcceptClient		(::llc::SSocketServer& serverInstance, ::llc::SCommand command, cnst sockaddr_in & sa_client, SOCKET acceptedSocket = INVALID_SOCKET)		{
	llc::pobj<llc::SConnection>		pClient				= {};
	llc::pobj<llc::SSocketConnection>	pSocket				= {};
	{ // accept
		::llc::lock_guard			lock					(serverInstance.Mutex);
		s2_t						found					= ::recycleClient(serverInstance, pClient, pSocket);
		u2_c				idConnection			= (-1 == found) ? serverInstance.Clients.size() : (u2_t)found;
		::llc::tcpipAddressFromSockaddr(sa_client, pSocket->Address);
		pSocket->IdConnection	= idConnection;
		command.Type			= ::llc::ENDPOINT_COMMAND_TYPE_RESPONSE;
		::llc::pobj<::llc::SMessage>	connectResponse		= {};
		connectResponse.create(::llc::SMessage{{}, (u3_t)::llc::timeCurrentInUs(), command});
		if_fail_fe(pClient->Queue.Send.push_back(connectResponse));
		pClient->LastPing		=
		pClient->FirstPing		= ::llc::timeCurrentInUs();
		if(INVALID_SOCKET == acceptedSocket) {
			if_true_vef(-1, INVALID_SOCKET == (pSocket->Socket.Handle = socket(AF_INET, SOCK_DGRAM, 0)), "%s", "Could not create socket.");
		}
		else
			pSocket->Socket.Handle	= acceptedSocket;
		pClient->State			= ::llc::CONNECTION_STATE_HANDSHAKE;
		if(found == -1) {
			if_fail_fe(serverInstance.Clients.push_back(pClient));
			if_fail_fe(serverInstance.Sockets.push_back(pSocket));
		}
	}
	info_printf("Current client count: %u", serverInstance.Clients.size());
	return 0;
}

sttc	::llc::err_t	serverListenTick		(::llc::SSocketServer& serverInstance, cnst sockaddr_in & server)		{
	socklen_t				client_length			= sizeof(sockaddr_in);		// Length of client struct
	::llc::SCommand			command					= {};	// Where to store received data
	int							bytes_received			= 0;
	sockaddr_in					sa_client				= {};	// Information about the client
	::llc::auto_socket_close	acceptedSocket			= {};
	if(::llc::TRANSPORT_PROTOCOL_TCP == serverInstance.Protocol) {
		if(INVALID_SOCKET == serverInstance.Socket) {
			if_true_vef(-1, INVALID_SOCKET == (serverInstance.Socket.Handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)), "%s", "Could not create socket.");
			if_fail_fef(::bind(serverInstance.Socket, (sockaddr *)&server, sizeof(sockaddr_in)), "%s", "Could not bind name to socket.");
			if_fail_fef(::listen(serverInstance.Socket, SOMAXCONN), "%s", "Could not listen on socket.");
		}
		if_fail_ve(-1, acceptedSocket.Handle = ::accept(serverInstance.Socket, (sockaddr*)&sa_client, &client_length));
		if_fail_ve(-1, bytes_received = ::recv(acceptedSocket, (sc_t*)&command, (int)sizeof(::llc::SCommand), 0));
	}
	else {
		serverInstance.Socket.close();
		if_true_vef(-1, INVALID_SOCKET == (serverInstance.Socket.Handle = socket(AF_INET, SOCK_DGRAM, 0)), "%s", "Could not create socket.");
		if_fail_fef(::bind(serverInstance.Socket, (sockaddr *)&server, sizeof(sockaddr_in)), "%s", "Could not bind name to socket.");	/* Bind address to socket */
		if_fail_ve(-1, bytes_received = recvfrom(serverInstance.Socket, (sc_t*)&command, (int)sizeof(::llc::SCommand), MSG_PEEK, (sockaddr*)&sa_client, &client_length));
	}
	if_true_vif(0, 0 == bytes_received, "%s", "Offline?");

	if_true_vef(-1, command.Type		!= ::llc::ENDPOINT_COMMAND_TYPE_REQUEST, "Invalid message type: 0x%x '%s'", command.Type, ::llc::get_value_label(command.Type).begin()); 
	if_true_vef(-1, command.Command	!= ::llc::ENDPOINT_COMMAND_DISCONNECT
		&& command.Command	!= ::llc::ENDPOINT_COMMAND_CONNECT
		, "Invalid message type: 0x%x '%s'", command.Type, ::llc::get_value_label(command.Type).begin()); 

	switch(command.Command) {
	case ::llc::ENDPOINT_COMMAND_CONNECT	: 
		if(::llc::TRANSPORT_PROTOCOL_TCP == serverInstance.Protocol) {
			cnst SOCKET			clientSocket			= acceptedSocket.Handle;
			acceptedSocket.Handle	= INVALID_SOCKET;
			if_fail_e(::serverAcceptClient(serverInstance, command, sa_client, clientSocket));
		}
		else
			if_fail_e(::serverAcceptClient(serverInstance, command, sa_client));
		::llc::sleep(1);
		break;
	case ::llc::ENDPOINT_COMMAND_DISCONNECT	: {
		if(false == serverInstance.Listen)
			break;

		sockaddr_in					sa_srv				= {};	// Information about the client
		::llc::tcpipAddressToSockaddr(serverInstance.Address, sa_srv);
		::llc::SIPv4End				address				= {{}, serverInstance.Address.Port};
		::llc::tcpipAddressFromSockaddr(sa_client, address);
		if_true_vef(-1, sa_client.sin_port != sa_srv.sin_port
			|| memcmp(&sa_client.sin_addr	, &sa_srv.sin_addr	, sizeof(sa_srv.sin_addr))
			|| memcmp(&sa_client.sin_family	, &sa_srv.sin_family, sizeof(sa_srv.sin_family))
			, "Invalid server address: %u.%u.%u.%u:%u", LLC_IPV4_EXPAND(address)
		);
		serverInstance.Listen	= false;	// Stop server (this breaks the listen loop).
		break;
	}
	}
	return 0;
}

sttc	void			threadUpdateClients		(void* serverInstance)					{ ::updateClients(*(::llc::SSocketServer*)serverInstance); }
sttc	::llc::err_t	server					(::llc::SSocketServer& serverInstance)		{
	serverInstance.Listen	= true;
	if_fail_fe(llc::tcpipAddress(serverInstance.Address.Port, serverInstance.AdapterIndex, serverInstance.Protocol, serverInstance.Address));
	sockaddr_in					server					= {};
	if_fail_fe(llc::tcpipAddressToSockaddr(serverInstance.Address, server));
#if defined(LLC_WINDOWS)
	_beginthread(::threadUpdateClients, 0, &serverInstance);
#else
	::std::thread(::threadUpdateClients, &serverInstance).detach();
#endif
	info_printf("Server running on %u.%u.%u.%u:%u", LLC_IPV4_EXPAND(serverInstance.Address));
	while(serverInstance.Listen) {	// Loop and get data from clients
		if_fail_w(::serverListenTick(serverInstance, server));
	}
	serverInstance.Socket.close();
	return 0;
}

sttc	void			threadServer			(void* pServerInstance)				{
	::llc::SSocketServer			& serverInstance		= *(::llc::SSocketServer*)pServerInstance;
	if_fail_e(server(serverInstance))
	else
		info_printf("Server gracefully closed.");

	serverInstance.Listen	= false;
	serverInstance.Socket.close();
	{
		::llc::lock_guard			lock					(serverInstance.Mutex);
		::llc::clear(serverInstance.Clients, serverInstance.Sockets);
	}
}

::llc::err_t			llc::serverStart		(::llc::SSocketServer& serverInstance, u1_t port, s1_t adapterIndex, ::llc::TRANSPORT_PROTOCOL protocol)		{
	serverInstance.Listen		= true;
	serverInstance.Address.Port	= port;
	serverInstance.AdapterIndex	= adapterIndex;
	serverInstance.Protocol		= protocol;
	_beginthread(::threadServer, 0, &serverInstance);
	return 0;
}

::llc::err_t			llc::serverStop			(::llc::SSocketServer& serverInstance)		{
	serverInstance.Listen	= false;
	if(::llc::TRANSPORT_PROTOCOL_TCP == serverInstance.Protocol) {
		serverInstance.Socket.close();
		return 0;
	}
	sockaddr_in					sa_srv					= {};							// Information about the client
	int							sa_length				= (int)sizeof(sockaddr_in);		// Length of client struct
	::llc::SCommand			command					= {::llc::ENDPOINT_COMMAND_DISCONNECT, ::llc::ENDPOINT_COMMAND_TYPE_REQUEST, };							// Where to store received data
	::llc::tcpipAddressToSockaddr(serverInstance.Address, sa_srv);
	u2_t					attempt					= 0;
	do{
		::sendto(serverInstance.Socket, (sc_c*)&command, (int)sizeof(::llc::SCommand), 0, (sockaddr*)&sa_srv, sa_length);
		::llc::sleep(100);
	} while(10 > ++attempt && serverInstance.Clients.size());

	if_true_if(attempt < 10, "Listening socket closed with %u disconnect attempt%s.", attempt, (attempt > 1) ? "s":"");
	return 0;
}

::llc::err_t			llc::serverPayloadCollect(::llc::SSocketServer & server, ::llc::aobj<::llc::apobj<::llc::SMessage>> & receivedMessages)		{
	lock_guard			lock					(server.Mutex);
	if_fail_fe(receivedMessages.resize(server.Clients.size()));
	for(u2_t iClient = 0, countClients = server.Clients.size(); iClient < countClients; ++iClient) {
		::llc::pnco<::llc::SConnection>	client			= server.Clients[iClient];
		if(!client)
			continue;

		if_fail_fe(llc::connectionPayloadCollect(*client, receivedMessages[iClient]));
	}
	return 0;
}
