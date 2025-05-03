#include "llc_auto_handler.h"
#include "llc_safe.h"
#include "llc_tcpip.h"

#ifndef LLC_ATMEL		

#ifdef LLC_WINDOWS
#	include <WinSock2.h>
//#elif defined(LLC_ESP8266)
//#	include <lwip/sockets.h>
#else
#	if defined(LLC_ESP8266)
//#		include <lwip/sockets.h>
#		include <lwip/inet.h>
#	else
#		include <sys/types.h>
#		include <sys/socket.h>
#		include <netinet/in.h>
#	endif
#	include <unistd.h>
#endif

#ifndef LLC_STDSOCKET_H_23627
#define LLC_STDSOCKET_H_23627

namespace llc
{
#ifndef LLC_WINDOWS
tydf int SOCKET;
#	ifndef INVALID_SOCKET
#		define INVALID_SOCKET -1
#	endif
#endif

	struct auto_socket_close : public ::llc::auto_handler<SOCKET, INVALID_SOCKET>					{
		using			TWrapper						::auto_handler;
		inline											~auto_socket_close					()	noexcept	{ close(); }
		inline			void							close								()	noexcept	{ if(INVALID_SOCKET != Handle) { closesocket(Handle); Handle = INVALID_SOCKET; } }
	};

	::llc::error_t			tcpipAddressToSockaddr		(uint8_t a1, uint8_t a2, uint8_t a3, uint8_t a4, uint16_t port, sockaddr_in & sockaddr);
	::llc::error_t			tcpipAddressToSockaddr		(uint32_t address, uint16_t port, sockaddr_in & sockaddr);

	//::llc::error_t			tcpipAddressFromSockaddr	(const sockaddr_in & sockaddr, uint8_t * a1, uint8_t * a2, uint8_t * a3, uint8_t * a4, uint16_t * port);
	::llc::error_t			tcpipAddressFromSockaddr	(const sockaddr_in & sockaddr, uint32_t & address, uint16_t & port);
	::llc::error_t			tcpipAddress				(SOCKET socket, uint32_t & ip, uint16_t & port);

	stin	::llc::error_t	tcpipAddressToSockaddr		(const ::llc::SIPv4End & addr, sockaddr_in & sockaddr)	{ return tcpipAddressToSockaddr		
		( ::llc::byte_at(addr.IP, 0)
		, ::llc::byte_at(addr.IP, 1)
		, ::llc::byte_at(addr.IP, 2)
		, ::llc::byte_at(addr.IP, 3)
		, addr.Port, sockaddr
		); 
	}
	stin	::llc::error_t	tcpipAddressFromSockaddr	(const sockaddr_in & sockaddr, ::llc::SIPv4End & address)	{ return tcpipAddressFromSockaddr	(sockaddr	, address.IP, address.Port); }
	stin	::llc::error_t	tcpipAddress				(SOCKET socket, ::llc::SIPv4End & address)					{ return tcpipAddress				(socket		, address.IP, address.Port); }
} // namespace


#endif // LLC_STDSOCKET_H_23627

#endif // LLC_ATMEL
