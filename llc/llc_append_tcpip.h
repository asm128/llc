#include "llc_tcpip.h"
#include "llc_array_pod.h"

#ifndef LLC_APOD_TCPIP_H
#define LLC_APOD_TCPIP_H

namespace llc
{
	::llc::error_t		appendString		(::llc::asc_t & output, llc::SIPv4 ip, char separator = '.');
	::llc::error_t		appendBraced		(::llc::asc_t & output, llc::SIPv4 ip, char separator = ',');
	::llc::error_t		appendQuoted		(::llc::asc_t & output, llc::SIPv4 ip, char separator = '.');
	::llc::error_t		appendBracedPrefixed(::llc::asc_t & output, llc::SIPv4 ip, bool usePrefix, char prefix = ',', char ip_separator = ',');
	::llc::error_t		appendQuotedPrefixed(::llc::asc_t & output, llc::SIPv4 ip, bool usePrefix, char prefix = ',', char ip_separator = '.');
} // namespace

#endif // LLC_APOD_TCPIP_H