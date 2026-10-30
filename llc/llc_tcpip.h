#include "llc_array_static.h"

#ifndef LLC_TCPIP_H_23627
#define LLC_TCPIP_H_23627

namespace llc
{
#pragma pack(push, 1)
	err_t		tcpipAddress		(vcst_t strIP, u2_t & ipv4);
	err_t		tcpipAddress		(vcst_t strIP, u2_t & ipv4, u1_t & port);

	stct SIPv4 {
		u2_t			IP					= {};

							SIPv4				(llc::vcst_t strIP)							{ tcpipAddress(strIP, IP); }

		inxp				SIPv4				()								noexcept	= default;
		inxp				SIPv4				(const SIPv4 &)					noexcept	= default;
		inxp				SIPv4				(u2_t little_endian)		noexcept	: IP(little_endian) {}
		cxpr				SIPv4				(u0_t _0, u0_t _1, u0_t _2, u0_t _3 = 0)
			: IP
			{ byte_to<u2_t>(_0, 0)
			| byte_to<u2_t>(_1, 1)
			| byte_to<u2_t>(_2, 2)
			| byte_to<u2_t>(_3, 3)
			} {}

		LLC_DEFAULT_OPERATOR(SIPv4, IP == other.IP);

		inxp	u0_t		oper[]			(u0_t index)		csnx	{ return byte_at(IP, index); }
		u0_t&				oper[]			(u0_t index)		nxpt	{ return *(((u0_t*)&IP) + index); }

		inxp	oper	u2_t			()					csnx	{ return IP; }
		cxpr	oper	astu0_t<4>		()					csnx	{
			return
				{ byte_at(IP, 0)
				, byte_at(IP, 1)
				, byte_at(IP, 2)
				, byte_at(IP, 3)
				};
		}

		template<size_t buflen>
		asttc<sc_t, (buflen > 16) ? buflen : 16>
					toString			(char separator = '.')						csnx	{
			asttc<sc_t, (buflen > 16) ? buflen : 16>	result	= {};
			print(result.Storage, separator);
			return result;
		}

		tplt<size_t bufferSize>
		err_t 		print				(char (&target)[bufferSize], char separator = '.')	csnx	{
			const char format[] = {'%', 'u', separator, '%', 'u', separator, '%', 'u', separator, '%', 'u', 0};
			return sprintf_s(target, format
				, byte_at(IP, 0)
				, byte_at(IP, 1)
				, byte_at(IP, 2)
				, byte_at(IP, 3)
				);
		}
	};

	stin	asttc<sc_t, 16>	str			(const SIPv4 ip)	{ return ip.toString<16>(); }

	tydf struct SIPv4End : SIPv4 {
		u1_t			Port				= {};

		LLC_DEFAULT_OPERATOR(SIPv4End, IP == other.IP && Port == other.Port);

		template<size_t buflen>
		oper			asttc<sc_t, min((size_t)22, buflen)>	()		csnx	{
			if(0 == Port)
				return *(const SIPv4*)this;

			asttc<sc_t, buflen>	result;
			sprintf_s(result.Storage, "%u.%u.%u.%u:%u"
				, byte_at(IP, 0)
				, byte_at(IP, 1)
				, byte_at(IP, 2)
				, byte_at(IP, 3)
				, Port
				);
			return result;
		}
	} SIPv4Endpoint;

	stct SIPv4Node {
		SIPv4	IPv4		= {};
		SIPv4	Gateway		= {};
		SIPv4	NetMask		= {};

		cxpr	SIPv4	NetworkID	()	csnx	{
			return SIPv4
				( NetMask[0] & IPv4[0]
				, NetMask[1] & IPv4[1]
				, NetMask[2] & IPv4[2]
				, NetMask[3] & IPv4[3]
				);
		}

		cxpr	SIPv4	BroadcastIP	()	csnx	{
			return SIPv4
				( ~NetMask[0] & IPv4[0]
				, ~NetMask[1] & IPv4[1]
				, ~NetMask[2] & IPv4[2]
				, ~NetMask[3] & IPv4[3]
				);
		}
	};

	tplt<size_t count> using	astipv4		= astatic<SIPv4, count>;
	tplt<size_t count> using	astipv4end	= astatic<SIPv4End, count>;
#pragma pack(pop)

	err_t			tcpipInitialize						();
	err_t			tcpipShutdown						();

	enum TRANSPORT_PROTOCOL
		{ TRANSPORT_PROTOCOL_UDP			= 0
		, TRANSPORT_PROTOCOL_TCP
		};


#define LLC_IPV4_EXPAND_IP(addr)  ::llc::byte_at(addr.IP, 0)	\
								, ::llc::byte_at(addr.IP, 1)	\
								, ::llc::byte_at(addr.IP, 2)	\
								, ::llc::byte_at(addr.IP, 3)	\

#define LLC_IPV4_EXPAND(addr) (::llc::u2_t)::llc::byte_at(addr.IP, 0)	\
							, (::llc::u2_t)::llc::byte_at(addr.IP, 1)	\
							, (::llc::u2_t)::llc::byte_at(addr.IP, 2)	\
							, (::llc::u2_t)::llc::byte_at(addr.IP, 3)	\
							, (::llc::u2_t)addr.Port

	stin	err_t	tcpipAddress		(vcst_t strIP, SIPv4 & ipv4) { return tcpipAddress(strIP, ipv4.IP); }


			err_t	tcpipAddress		(const char* szHostName, u1_t portRequested, u2_t adapterIndex, TRANSPORT_PROTOCOL mode, u2_t & address, u1_t & port);
			err_t	tcpipAddress		(const char* szHostName, u1_t portRequested, u2_t adapterIndex, TRANSPORT_PROTOCOL mode, u0_t * a1, u0_t * a2, u0_t * a3, u0_t * a4, u1_t* port = 0);

			err_t	tcpipAddress		(vcst_t hostName, u1_t portRequested, u2_t adapterIndex, TRANSPORT_PROTOCOL mode, u0_t * a1, u0_t * a2, u0_t * a3, u0_t * a4, u1_t* port = 0);
	stin	err_t	tcpipAddress		(const char* szHostName, u1_t portRequested, u2_t adapterIndex, TRANSPORT_PROTOCOL mode, u2_t & address)				{
		u1_t port = 0;
		return llc::tcpipAddress(szHostName, portRequested, adapterIndex, mode, address, port);
	}

			err_t	tcpipAddress		(u1_t portRequested, u2_t adapterIndex, TRANSPORT_PROTOCOL mode, u2_t & address);
			err_t	tcpipAddress		(u1_t portRequested, u2_t adapterIndex, TRANSPORT_PROTOCOL mode, u0_t * a1, u0_t * a2, u0_t * a3, u0_t * a4);
	stin	err_t	tcpipAddress		(u1_t portRequested, u2_t adapterIndex, TRANSPORT_PROTOCOL mode, SIPv4End & address)							{
		address.Port			= portRequested;
		if_fail_fe(tcpipAddress(portRequested, adapterIndex, mode, address.IP));
		return 0;
	}
	stin	err_t	tcpipAddress		(const char * hostName, u1_t portRequested, u2_t adapterIndex, TRANSPORT_PROTOCOL mode, SIPv4End & address)	{
		return tcpipAddress(hostName, portRequested, adapterIndex, mode, address.IP, (address.Port = portRequested));
	}
	stin	err_t	tcpipAddress		(const vcst_t & hostName, u1_t portRequested, u2_t adapterIndex, TRANSPORT_PROTOCOL mode, SIPv4End & address)	{
		return tcpipAddress(hostName.begin(), portRequested, adapterIndex, mode, address.IP, (address.Port = portRequested));
	}

	err_t			tcpipAddress		(vcst_t strAddress, vcst_t strPort, SIPv4End & ipv4end);

} // namespace

#endif // LLC_TCPIP_H_23627

// 50y-5p4C3.41