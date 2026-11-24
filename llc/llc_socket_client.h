#include "llc_connection_ip.h"

#ifndef LLC_UDP_CLIENT_H_23627
#define LLC_UDP_CLIENT_H_23627

nmsp llc
{
	struct SSocketClient {
		SConnection			Connection		= {};
		SSocketConnection	Socket			= {};
		SIPv4End			AddressConnect	= {};
		s1_t				AdapterIndex	= {};
		apobj<SMessage>		CacheSend		= {};	// This is only to speed up message processing.
		apobj<SMessage>		CacheSent		= {};	// This is only to speed up message processing.
	};

	err_t				clientDisconnect		(SSocketClient & client);
	err_t				clientConnect			(SSocketClient & client);
	err_t				clientUpdate			(SSocketClient & client);
}

#endif // LLC_UDP_CLIENT_H_23627
