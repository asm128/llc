#include "llc_tcpip.h"
#include "llc_parse.h"
#include "llc_json.h"
#ifdef LLC_WINDOWS
#	include "llc_windows.h"
#endif
#ifndef LLC_ESP8266
#	include "llc_stdsocket.h"
#endif

::llc::error_t			llc::tcpipAddress		(::llc::vcst_t strIP, uint32_t & address, uint16_t & port) {
	uint32_t					iOffset					= ::llc::tcpipAddress(strIP, address);
	return (iOffset < strIP.size())
		? iOffset + ::llc::parseIntegerDecimal({&strIP[iOffset], strIP.size() - iOffset}, port)
		: iOffset
		;
}

::llc::error_t			jsonTcpipAddress		(llc::vcst_t strIP, uint32_t & ipv4) {
	::llc::SJSONReader			reader					= {};
	llc_necall(llc::jsonParse(reader, strIP), "Failed to parse ip from string: '%s'.", ::llc::toString(strIP).begin());
	ipv4					= 0;
	{ int32_t iToken; llc_necs(iToken = ::llc::jsonArrayValueGet(reader, 0, 0)); ipv4 |= ::llc::byte_to<uint32_t>((uint8_t)reader[iToken]->Token->Value, 0); }
	{ int32_t iToken; llc_necs(iToken = ::llc::jsonArrayValueGet(reader, 0, 1)); ipv4 |= ::llc::byte_to<uint32_t>((uint8_t)reader[iToken]->Token->Value, 1); }
	{ int32_t iToken; llc_necs(iToken = ::llc::jsonArrayValueGet(reader, 0, 2)); ipv4 |= ::llc::byte_to<uint32_t>((uint8_t)reader[iToken]->Token->Value, 2); }
	{ int32_t iToken; llc_necs(iToken = ::llc::jsonArrayValueGet(reader, 0, 3)); ipv4 |= ::llc::byte_to<uint32_t>((uint8_t)reader[iToken]->Token->Value, 3); }
	return 0;
}

::llc::error_t			llc::tcpipAddress		(llc::vcst_t strIP, uint32_t & ipv4)	{
	ipv4					= 0;
	::llc::ltrim(strIP);
	if(0 == strIP.size()) 
		return 0;

	uint32_t					iOffset					= 0;
	uint32_t					iEnd					= 0;
	for(uint8_t iVal = 0; iVal < 4 && iOffset < strIP.size(); ++iVal) {
		while(iEnd < strIP.size()) {
			char curChar = strIP[iEnd];
			if(0 == iVal && curChar == '[') 
				return jsonTcpipAddress(strIP, ipv4);

			if( curChar == ','
			 || curChar == '.'
			 || curChar == ':'
			 || curChar == '\0'
			 || (iEnd - iOffset) > 3	// 3 digit max
			)
				break;
			++iEnd;
		}
		uint8_t			valueRead	= 0;
		llc_necs(llc::parseIntegerDecimal({&strIP[iOffset], iEnd - iOffset}, valueRead));

		ipv4	|= ::llc::byte_to<uint32_t>(valueRead, iVal);

		iOffset		= iEnd + 1;
		iEnd		= iOffset;
	}
	return iOffset;
}

::llc::error_t			llc::tcpipAddress		(::llc::vcst_t strIP, ::llc::vcst_t strPort, ::llc::SIPv4End & ipv4) {
	if(0 == strPort.size()) 
		return ::llc::tcpipAddress(strIP, ipv4.IP, ipv4.Port);

	u2_c				iOffset					= ::llc::tcpipAddress(strIP, ipv4.IP);
	return iOffset + ::llc::parseIntegerDecimal(strPort, ipv4.Port);
}

#ifndef LLC_ATMEL
#ifndef LLC_ESP8266
#if defined(LLC_WINDOWS)
#	include <WS2tcpip.h>
#elif defined(LLC_ESP8266)
//#	include <lwip/sockets.h>
//#	include <lwip/netdb.h>
#	include <lwip/inet.h>
#else
#	include <netdb.h>
#	include <arpa/inet.h>
#endif

::llc::error_t			llc::tcpipInitialize						()							{
#if defined(LLC_WINDOWS)
	::WSADATA										w											= {};
	ree_if(::WSAStartup(0x0202, &w) != 0, "Could not open Windows sockets: 0x%X '%s'", WSAGetLastError(), ::llc::getWindowsErrorAsString(WSAGetLastError()).begin());
#endif
	info_printf("%s", "Network subsystem initialized.");
	return 0;
}

::llc::error_t			llc::tcpipShutdown							()							{
#if defined(LLC_WINDOWS)
	ree_if(::WSACleanup() != 0, "Could not shut down Windows sockets: 0x%X '%s'", WSAGetLastError(), ::llc::getWindowsErrorAsString(WSAGetLastError()).begin());		// Open windows connection
#endif
	info_printf("%s", "Network subsystem shut down.");
	return 0;
}

::llc::error_t			llc::tcpipAddressFromSockaddr		(const sockaddr_in & sockaddr_ipv4, uint32_t & address, uint16_t & port) {
	port 	= (uint16_t)ntohs(sockaddr_ipv4.sin_port);
#ifndef LLC_WINDOWS
	address	= sockaddr_ipv4.sin_addr.s_addr;
#else
	address	= ::llc::SIPv4
		{ sockaddr_ipv4.sin_addr.S_un.S_un_b.s_b1 
		, sockaddr_ipv4.sin_addr.S_un.S_un_b.s_b2
		, sockaddr_ipv4.sin_addr.S_un.S_un_b.s_b3
		, sockaddr_ipv4.sin_addr.S_un.S_un_b.s_b4
		};
#endif
	return 0;
}

//::llc::error_t			llc::tcpipAddressFromSockaddr	(const sockaddr_in & sockaddr_ipv4, uint8_t * a1, uint8_t * a2, uint8_t * a3, uint8_t * a4, uint16_t * port)	{
//#if defined(LLC_WINDOWS)
//	llc_safe_assign(a1, (uint8_t)sockaddr_ipv4.sin_addr.S_un.S_un_b.s_b1);
//	llc_safe_assign(a2, (uint8_t)sockaddr_ipv4.sin_addr.S_un.S_un_b.s_b2);
//	llc_safe_assign(a3, (uint8_t)sockaddr_ipv4.sin_addr.S_un.S_un_b.s_b3);
//	llc_safe_assign(a4, (uint8_t)sockaddr_ipv4.sin_addr.S_un.S_un_b.s_b4);
//#else
//	llc_safe_assign(a1, ::llc::byte_at(sockaddr_ipv4.sin_addr.s_addr, 0));
//	llc_safe_assign(a2, ::llc::byte_at(sockaddr_ipv4.sin_addr.s_addr, 1));
//	llc_safe_assign(a3, ::llc::byte_at(sockaddr_ipv4.sin_addr.s_addr, 2));
//	llc_safe_assign(a4, ::llc::byte_at(sockaddr_ipv4.sin_addr.s_addr, 3));
//#endif
//	llc_safe_assign(port, (uint16_t)ntohs(sockaddr_ipv4.sin_port));
//	return 0;
//}
::llc::error_t			llc::tcpipAddressToSockaddr		(uint32_t address, uint16_t port, sockaddr_in & sockaddr_ipv4) {
	sockaddr_ipv4				= {};
	sockaddr_ipv4.sin_family	= AF_INET;
	sockaddr_ipv4.sin_port		= port ? htons(port) : (uint16_t)0;
#ifndef LLC_WINDOWS
	sockaddr_ipv4.sin_addr.s_addr				=  address;
#else
	sockaddr_ipv4.sin_addr.S_un.S_un_b.s_b1		= ::llc::byte_at(address, 0);
	sockaddr_ipv4.sin_addr.S_un.S_un_b.s_b2		= ::llc::byte_at(address, 1);
	sockaddr_ipv4.sin_addr.S_un.S_un_b.s_b3		= ::llc::byte_at(address, 2);
	sockaddr_ipv4.sin_addr.S_un.S_un_b.s_b4		= ::llc::byte_at(address, 3);
#endif
	return 0;
}

::llc::error_t			llc::tcpipAddressToSockaddr		(uint8_t a1, uint8_t a2, uint8_t a3, uint8_t a4, uint16_t port, sockaddr_in & sockaddr_ipv4)	{
	sockaddr_ipv4				= {};
	sockaddr_ipv4.sin_family	= AF_INET;
	sockaddr_ipv4.sin_port		= port ? htons(port) : (uint16_t)0;
#ifndef LLC_WINDOWS
	sockaddr_ipv4.sin_addr.s_addr				=  ::llc::SIPv4{a1, a2, a3, a4};
#else
	sockaddr_ipv4.sin_addr.S_un.S_un_b.s_b1		= a1;
	sockaddr_ipv4.sin_addr.S_un.S_un_b.s_b2		= a2;
	sockaddr_ipv4.sin_addr.S_un.S_un_b.s_b3		= a3;
	sockaddr_ipv4.sin_addr.S_un.S_un_b.s_b4		= a4;
#endif
	return 0;
}

::llc::error_t			llc::tcpipAddress	(SOCKET socket, uint32_t & ip, uint16_t & port) {
	sockaddr_in					sockaddr_ipv4		= {};
	socklen_t					len					= szof(sockaddr_in);
	ree_if(getsockname(socket, (sockaddr*)&sockaddr_ipv4, &len) != 0, "%s", "getpeername failed.");
	return ::llc::tcpipAddressFromSockaddr(sockaddr_ipv4, ip, port);
}

::llc::error_t			llc::tcpipAddress	(uint16_t portRequested, uint32_t adapterIndex, TRANSPORT_PROTOCOL mode, uint8_t * a1, uint8_t * a2, uint8_t * a3, uint8_t * a4)										{
	char						host_name[257]								= {};
#if defined(LLC_ESP8266)
	sprintf(host_name, "%s", wifi_station_get_hostname());
#else	
	gethostname(host_name, 256);
#endif
	return ::llc::tcpipAddress(host_name, portRequested, adapterIndex, mode, a1, a2, a3, a4);
}

::llc::error_t			llc::tcpipAddress	(uint16_t portRequested, uint32_t adapterIndex, TRANSPORT_PROTOCOL mode, uint32_t & address)										{
	char						host_name[257]								= {};
#if defined(LLC_ESP8266)
	sprintf(host_name, "%s", wifi_station_get_hostname());
#else	
	gethostname(host_name, 256);
#endif
	return ::llc::tcpipAddress(host_name, portRequested, adapterIndex, mode, address);
}

::llc::error_t			llc::tcpipAddress	(const char* szHostName, uint16_t portRequested, uint32_t adapterIndex, TRANSPORT_PROTOCOL mode, uint8_t * a1, uint8_t * a2, uint8_t * a3, uint8_t * a4, uint16_t * port)				{
	::llc::SIPv4End 		addr; 
	llc_necs(llc::tcpipAddress(szHostName, portRequested, adapterIndex, mode, addr.IP, addr.Port));
	llc_safe_assign(a1, addr[0]);
	llc_safe_assign(a2, addr[1]);
	llc_safe_assign(a3, addr[2]);
	llc_safe_assign(a4, addr[3]);	
	llc_safe_assign(port, addr.Port);		
	return 0;
}

::llc::error_t		llc::tcpipAddress		(::llc::vcst_t hostName, uint16_t portRequested, uint32_t adapterIndex, TRANSPORT_PROTOCOL mode, uint8_t * a1, uint8_t * a2, uint8_t * a3, uint8_t * a4, uint16_t* port) { 
	return ::llc::tcpipAddress(::llc::toString(hostName).begin(), portRequested, adapterIndex, mode, a1, a2, a3, a4, port); 
}

::llc::error_t			llc::tcpipAddress	(const char* szHostName, uint16_t portRequested, uint32_t adapterIndex, TRANSPORT_PROTOCOL mode, uint32_t & address, uint16_t & port) {
	char						portString			[6]					= {};
	snprintf(portString, ::llc::size(portString), "%u", portRequested);

	// Setup the hints address info structure which is passed to the getaddrinfo() function
	::addrinfo					hints											= {};
	hints.ai_family			= AF_INET;
	hints.ai_socktype		= (TRANSPORT_PROTOCOL_TCP == mode) ? SOCK_STREAM	: SOCK_DGRAM	;
	hints.ai_protocol		= (TRANSPORT_PROTOCOL_TCP == mode) ? IPPROTO_TCP	: IPPROTO_UDP	;

	const ::addrinfo			* createdAddrInfo								= 0;
	if_fail_vef(-1, ::getaddrinfo(szHostName, portString, &hints, (::addrinfo**)&createdAddrInfo), "gettaddrinfo failed for host_name: %s, port: %s", szHostName, portString);
	//sockaddr_in6									* sockaddr_ipv6									= 0;

	uint32_t					iAddress										= 0;
	bool						addressFound									= false;
	for(const addrinfo* ptr = createdAddrInfo; ptr != NULL; ptr = ptr->ai_next)  {	// Retrieve each address and print out the hex bytes
		verbose_printf("getaddrinfo response at index %" LLC_FMT_U2 ".", iAddress);
		verbose_printf("Flags: 0x%x.", ptr->ai_flags);
		verbose_printf("%s", "Family: ");
		char						ipstringbuffer	[46]							= {};
#if defined(LLC_WINDOWS)
		wchar_t						ipwstringbuffer	[46]							= {};
#endif
		::sockaddr					* sockaddr_ip									=  0;
		::sockaddr_in				* sockaddr_ipv4									=  0;
#ifndef LLC_ESP8266
		::sockaddr_in6				* sockaddr_ipv6									=  0;
		(void)sockaddr_ipv6	;
#endif
		(void)sockaddr_ip	;
		(void)sockaddr_ipv4	;
		//DWORD dwRetval;
#if defined(LLC_WINDOWS)
		int32_t						iRetval;
		DWORD						ipbufferlength									= 46;
#else
		uint32_t					ipbufferlength									= 46;
#endif
		switch (ptr->ai_family)  {
		default			:	verbose_printf("Other %li.", ptr->ai_family	); break;
		//case AF_NETBIOS	:	info_printf("%s", "AF_NETBIOS (NetBIOS)"	); break;
		case AF_UNSPEC	:	verbose_printf("%s", "Unspecified"				); break;
		case AF_INET	:
			verbose_printf("%s", "AF_INET (IPv4)");
			sockaddr_ipv4								= (::sockaddr_in *) ptr->ai_addr;
			ipbufferlength								= 46;
 			::inet_ntop(AF_INET, &sockaddr_ipv4->sin_addr, ipstringbuffer, ipbufferlength);
			verbose_printf("IPv4 address %s", ipstringbuffer);
			// Copy address
			if(adapterIndex == iAddress) {
				::llc::tcpipAddressFromSockaddr(*sockaddr_ipv4, address, port);
				//printf("\tIPv4 address %s\n", inet_ntoa(sockaddr_ipv4->sin_addr) );
				addressFound								= true;
			}
			break;
#ifndef LLC_ESP8266
		case AF_INET6	:
			verbose_printf("%s", "AF_INET6 (IPv6)");
			// the InetNtop function is available on Windows Vista and later
			sockaddr_ipv6								= (struct ::sockaddr_in6 *) ptr->ai_addr;
			//info_printf("IPv6 address %s", InetNtop(AF_INET6, &sockaddr_ipv6->sin6_addr, ipwstringbuffer, 46) );

#	if defined(LLC_WINDOWS)
			sockaddr_ip									= (LPSOCKADDR)ptr->ai_addr;
			ipbufferlength								= 46;	// The buffer length is changed by each call to WSAAddresstoString, so we need to set it for each iteration through the loop for safety
			iRetval										= WSAAddressToStringW(sockaddr_ip, (DWORD) ptr->ai_addrlen, NULL, ipwstringbuffer, &ipbufferlength );	// We use WSAAddressToString since it is supported on Windows XP and later
			if (iRetval) {
				warning_printf("WSAAddressToString failed with code 0x%X: '%s'.", ::WSAGetLastError(), ::llc::getWindowsErrorAsString(::WSAGetLastError()).begin() );
			}
			else
				::wprintf(L"IPv6 address '%s'.\n", ipwstringbuffer);
#	endif
			break;
#endif
		}

		//verbose_printf("%s", "Socket type: ");
		//switch (ptr->ai_socktype) {
		//case 0				: verbose_printf("%s", "Unspecified"							);	break;
		//case SOCK_STREAM	: verbose_printf("%s", "SOCK_STREAM (stream)"					);	break;
		//case SOCK_DGRAM		: verbose_printf("%s", "SOCK_DGRAM (datagram)"					);	break;
		//case SOCK_RAW		: verbose_printf("%s", "SOCK_RAW (raw)"							);	break;
		//case SOCK_RDM		: verbose_printf("%s", "SOCK_RDM (reliable message datagram)"	);	break;
		//case SOCK_SEQPACKET	: verbose_printf("%s", "SOCK_SEQPACKET (pseudo-stream packet)"	);	break;
		//default:
		//	verbose_printf("Other %ld", ptr->ai_socktype);
		//	break;
		//}
		//verbose_printf("%s", "Protocol: ");
		//switch (ptr->ai_protocol) {
		//case 0				: verbose_printf("%s", "Unspecified"							);	break;
		//case IPPROTO_TCP	: verbose_printf("%s", "IPPROTO_TCP (TCP)"						);	break;
		//case IPPROTO_UDP	: verbose_printf("%s", "IPPROTO_UDP (UDP)"						);	break;
		//default:
		//	verbose_printf("Other %ld", ptr->ai_protocol);
		//	break;
		//}
		//++iAddress;
		verbose_printf("Length of this sockaddr: %" LLC_FMT_U2 "", (uint32_t)ptr->ai_addrlen);
		verbose_printf("Canonical name: %s", ptr->ai_canonname);
		if(addressFound)
			break;
	}
	::freeaddrinfo((::addrinfo*)createdAddrInfo);
//#else
//	struct hostent									* hp		= gethostbyname(host_name);
//
//	ree_if(hp == NULL, "Could not get host by name.");
//
//	b1											= hp->h_addr_list[0][0];
//	b2											= hp->h_addr_list[0][1];
//	b3											= hp->h_addr_list[0][2];
//	b4											= hp->h_addr_list[0][3];
//#endif
	return iAddress;
}
#endif // LLC_ESP8266
#endif // LLC_ATMEL
