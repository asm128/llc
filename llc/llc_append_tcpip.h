#include "llc_tcpip.h"
#include "llc_string.h"

#ifndef LLC_APOD_TCPIP_H
#define LLC_APOD_TCPIP_H

namespace llc
{
	::llc::error_t		appendString		(::llc::string & output, llc::SIPv4 ip, char separator = '.');
	::llc::error_t		appendBraced		(::llc::string & output, llc::SIPv4 ip, char separator = ',');
	::llc::error_t		appendQuoted		(::llc::string & output, llc::SIPv4 ip, char separator = '.');
	::llc::error_t		appendBracedPrefixed(::llc::string & output, llc::SIPv4 ip, bool usePrefix, char prefix = ',', char ip_separator = ',');
	::llc::error_t		appendQuotedPrefixed(::llc::string & output, llc::SIPv4 ip, bool usePrefix, char prefix = ',', char ip_separator = '.');
} // namespace

#endif // LLC_APOD_TCPIP_H
