#include "llc_connection.h"
#include "llc_stdsocket.h"
#include "llc_tcpip.h"

#ifndef LLC_CONNECTION_IP_H_261025
#define LLC_CONNECTION_IP_H_261025

nmsp llc
{
	struct SSocketConnection {
		auto_socket_close	Socket			= {};
		SIPv4End			Address			= {};
		u2_t				IdConnection	= u2_t(-1);
	};

	err_t	connectionSendQueue		(SConnection & connection, SSocketConnection & client, apobj<SMessage> & messageCacheSent, apobj<SMessage> & messageCacheSend);
	err_t	connectionHandleCommand	(SConnection & connection, SSocketConnection & client, SCommand & command, au0_t & receiveBuffer);
} // nmsp

#endif // LLC_CONNECTION_IP_H_261025
