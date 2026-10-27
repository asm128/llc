#include "llc_connection_ip.h"

#ifndef LLC_UDP_SERVER_H_23627
#define LLC_UDP_SERVER_H_23627

namespace llc
{
	struct SSocketServer {
		auto_socket_close			Socket		;
		apobj<SSocketConnection>	Clients		;
		mutex						Mutex		;
		SIPv4End					Address		= {};
		int16_t						AdapterIndex	= 0;
		bool						Listen		= false;
		int32_t						Timeout		= 3000000;
	};

	err_t	serverStart				(SSocketServer & server, uint16_t port, int16_t adapterIndex = 0);
	err_t	serverStop				(SSocketServer & server);
	err_t	serverPayloadCollect	(SSocketServer & server, aobj<apobj<SMessage>> & receivedMessages);
} // namespace

#endif // LLC_UDP_SERVER_H_23627
