#include "llc_array_static.h"

#ifndef LLC_TCPIP_H_23627
#define LLC_TCPIP_H_23627

namespace llc
{
#pragma pack(push, 1)
	::llc::error_t		tcpipAddress		(::llc::vcst_t strIP, uint32_t & ipv4);
	::llc::error_t		tcpipAddress		(::llc::vcst_t strIP, uint32_t & ipv4, uint16_t & port);

	stct SIPv4 {
		uint32_t			IP					= {};
		
							SIPv4				(llc::vcst_t strIP)							{ ::llc::tcpipAddress(strIP, IP); }
							
		inxp				SIPv4				()								noexcept	= default;
		inxp				SIPv4				(const SIPv4 &)					noexcept	= default;
		inxp				SIPv4				(uint32_t little_endian)		noexcept	: IP(little_endian) {}
		cxpr				SIPv4				(uint8_t _0, uint8_t _1, uint8_t _2, uint8_t _3 = 0)
			: IP
			{ ::llc::byte_to<uint32_t>(_0, 0)
			| ::llc::byte_to<uint32_t>(_1, 1)
			| ::llc::byte_to<uint32_t>(_2, 2)
			| ::llc::byte_to<uint32_t>(_3, 3)
			} {}

		LLC_DEFAULT_OPERATOR(SIPv4, IP == other.IP);

		inxp	uint8_t		oper[]			(uint8_t index)		const	noexcept	{ return ::llc::byte_at(IP, index); }
		uint8_t&			oper[]			(uint8_t index)				noexcept	{ return *(((uint8_t*)&IP) + index); }

		inxp	oper	uint32_t			()					const	noexcept	{ return IP; }
		cxpr	oper	llc::astu8<4>		()					const	noexcept	{
			return 
				{ ::llc::byte_at(IP, 0)
				, ::llc::byte_at(IP, 1) 
				, ::llc::byte_at(IP, 2) 
				, ::llc::byte_at(IP, 3) 
				};
		}

		template<size_t buflen>
		llc::astchar<(buflen > 16) ? buflen : 16>	toString			(char separator = '.')						const	noexcept	{
			llc::astchar<(buflen > 16) ? buflen : 16>	result	= {};
			print(result.Storage, separator);
			return result; 
		}

		tplt<size_t bufferSize>
		::llc::error_t 		print				(char (&target)[bufferSize], char separator = '.')	const	noexcept	{
			const char format[] = {'%', 'u', separator, '%', 'u', separator, '%', 'u', separator, '%', 'u', 0};
			return sprintf_s(target, format
				, ::llc::byte_at(IP, 0)
				, ::llc::byte_at(IP, 1)
				, ::llc::byte_at(IP, 2)
				, ::llc::byte_at(IP, 3)
				);
		}
	};

	stin	::llc::astchar<16>	str			(const llc::SIPv4 ip)	{ return ip.toString<16>(); }

	tydf struct SIPv4End : SIPv4 {
		uint16_t			Port				= {};

		LLC_DEFAULT_OPERATOR(SIPv4End, IP == other.IP && Port == other.Port);

		template<size_t buflen>
		oper			llc::astchar<min((size_t)22, buflen)>	()		const	noexcept	{
			if(0 == Port)
				return *(const SIPv4*)this;

			llc::astchar<buflen>	result;
			sprintf_s(result.Storage, "%u.%u.%u.%u:%u"
				, ::llc::byte_at(IP, 0)
				, ::llc::byte_at(IP, 1)
				, ::llc::byte_at(IP, 2)
				, ::llc::byte_at(IP, 3)
				, Port
				);
			return result; 
		}
	} SIPv4Endpoint;

	stct SIPv4Node {
		::llc::SIPv4	IPv4		= {};
		::llc::SIPv4	Gateway		= {};
		::llc::SIPv4	NetMask		= {};

		cxpr	::llc::SIPv4	NetworkID	()	const	noexcept	{
			return ::llc::SIPv4
				( NetMask[0] & IPv4[0]
				, NetMask[1] & IPv4[1]
				, NetMask[2] & IPv4[2]
				, NetMask[3] & IPv4[3]
				);
		}

		cxpr	::llc::SIPv4	BroadcastIP	()	const	noexcept	{
			return ::llc::SIPv4
				( ~NetMask[0] & IPv4[0]
				, ~NetMask[1] & IPv4[1]
				, ~NetMask[2] & IPv4[2]
				, ~NetMask[3] & IPv4[3]
				);
		}
	};
	
	tplt<size_t count> using	astipv4		= ::llc::astatic<::llc::SIPv4, count>;
	tplt<size_t count> using	astipv4end	= ::llc::astatic<::llc::SIPv4End, count>;
#pragma pack(pop)

	::llc::error_t			tcpipInitialize						();
	::llc::error_t			tcpipShutdown						();

	enum TRANSPORT_PROTOCOL
		{ TRANSPORT_PROTOCOL_UDP			= 0
		, TRANSPORT_PROTOCOL_TCP
		};


#define LLC_IPV4_EXPAND_IP(addr)  ::llc::byte_at(addr.IP, 0)	\
								, ::llc::byte_at(addr.IP, 1)	\
								, ::llc::byte_at(addr.IP, 2)	\
								, ::llc::byte_at(addr.IP, 3)	\

#define LLC_IPV4_EXPAND(addr) (uint32_t)::llc::byte_at(addr.IP, 0)	\
							, (uint32_t)::llc::byte_at(addr.IP, 1)	\
							, (uint32_t)::llc::byte_at(addr.IP, 2)	\
							, (uint32_t)::llc::byte_at(addr.IP, 3)	\
							, (uint32_t)addr.Port

	stin	::llc::error_t	tcpipAddress		(::llc::vcst_t strIP, ::llc::SIPv4 & ipv4) { return ::llc::tcpipAddress(strIP, ipv4.IP); }


			::llc::error_t	tcpipAddress		(const char* szHostName, uint16_t portRequested, uint32_t adapterIndex, TRANSPORT_PROTOCOL mode, uint32_t & address, uint16_t & port);
			::llc::error_t	tcpipAddress		(const char* szHostName, uint16_t portRequested, uint32_t adapterIndex, TRANSPORT_PROTOCOL mode, uint8_t * a1, uint8_t * a2, uint8_t * a3, uint8_t * a4, uint16_t* port = 0);

			::llc::error_t	tcpipAddress		(::llc::vcst_t hostName, uint16_t portRequested, uint32_t adapterIndex, TRANSPORT_PROTOCOL mode, uint8_t * a1, uint8_t * a2, uint8_t * a3, uint8_t * a4, uint16_t* port = 0);
	stin	::llc::error_t	tcpipAddress		(const char* szHostName, uint16_t portRequested, uint32_t adapterIndex, TRANSPORT_PROTOCOL mode, uint32_t & address)				{
		uint16_t port = 0;
		return llc::tcpipAddress(szHostName, portRequested, adapterIndex, mode, address, port);
	}

			::llc::error_t	tcpipAddress		(uint16_t portRequested, uint32_t adapterIndex, TRANSPORT_PROTOCOL mode, uint32_t & address);
			::llc::error_t	tcpipAddress		(uint16_t portRequested, uint32_t adapterIndex, TRANSPORT_PROTOCOL mode, uint8_t * a1, uint8_t * a2, uint8_t * a3, uint8_t * a4);
	stin	::llc::error_t	tcpipAddress		(uint16_t portRequested, uint32_t adapterIndex, TRANSPORT_PROTOCOL mode, SIPv4End & address)							{
		address.Port			= portRequested;
		llc_necs(llc::tcpipAddress(portRequested, adapterIndex, mode, address.IP));
		return 0;
	}
	stin	::llc::error_t	tcpipAddress		(const char * hostName, uint16_t portRequested, uint32_t adapterIndex, TRANSPORT_PROTOCOL mode, SIPv4End & address)	{
		return ::llc::tcpipAddress(hostName, portRequested, adapterIndex, mode, address.IP, (address.Port = portRequested));
	}
	stin	::llc::error_t	tcpipAddress		(const ::llc::vcst_t & hostName, uint16_t portRequested, uint32_t adapterIndex, TRANSPORT_PROTOCOL mode, SIPv4End & address)	{
		return ::llc::tcpipAddress(hostName.begin(), portRequested, adapterIndex, mode, address.IP, (address.Port = portRequested));
	}

	::llc::error_t			tcpipAddress		(::llc::vcst_t strAddress, ::llc::vcst_t strPort, ::llc::SIPv4End & ipv4end);
	
} // namespace

#endif // LLC_TCPIP_H_23627

// 50y-5p4C3.41