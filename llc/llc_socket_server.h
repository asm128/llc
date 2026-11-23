#include "llc_connection_ip.h"

#ifndef LLC_SOCKET_SERVER_H_23627
#define LLC_SOCKET_SERVER_H_23627

nmsp llc
{
	struct SSocketServer {
		auto_socket_close			Socket			= {};
		apobj<SConnection>			Clients			= {};
		apobj<SSocketConnection>	Sockets			= {};
		mutex						Mutex			= {};
		SIPv4End					Address			= {};
		s1_t						AdapterIndex	= {};
		TRANSPORT_PROTOCOL			Protocol		= TRANSPORT_PROTOCOL_UDP;
		b8_t						Listen			= {};
		s2_t						Timeout			= 3000000;	// Kill clients timeout
	};

	err_t						serverStart							(SSocketServer & serverInstance, u1_t port, s1_t adapterIndex = 0, TRANSPORT_PROTOCOL protocol = TRANSPORT_PROTOCOL_UDP);
	err_t						serverStop							(SSocketServer & serverInstance);
	err_t						serverPayloadCollect				(SSocketServer & server, aobj<apobj<SMessage>> & receivedMessages);
} // nmsp

#endif // LLC_SOCKET_SERVER_H_23627
