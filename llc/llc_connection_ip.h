#include "llc_connection.h"
#include "llc_stdsocket.h"
#include "llc_tcpip.h"

#ifndef LLC_CONNECTION_IP_H_261025
#define LLC_CONNECTION_IP_H_261025

namespace llc
{
	struct SSocketConnection {
		auto_socket_close	Socket			= {};
		SIPv4End			Address			= {};
		u2_t				IdConnection	= {};
	};

	err_t	connectionSendQueue		(SSocketConnection & client, apobj<SMessage> & messageCacheSent, apobj<SMessage> & messageCacheSend);
	err_t	connectionHandleCommand	(SSocketConnection & client, SCommand & command, au0_t & receiveBuffer);
} // namespace

#endif // LLC_CONNECTION_IP_H_261025
