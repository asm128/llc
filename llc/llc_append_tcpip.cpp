#include "llc_append_tcpip.h"
#include "llc_string_compose.h"

::llc::error_t	llc::appendString			(::llc::asc_t & output, llc::SIPv4 ip, char separator) { 
	char				buffer[16]	= {};
	::llc::error_t		len;
	llc_necs(len = ip.print(buffer, separator));
	llc_necs(output.append_string(buffer));
	return len;
}

::llc::error_t	llc::appendBraced			(::llc::asc_t & output, llc::SIPv4 ip, char separator) { 
	char				buffer[16]	= {};
	llc_necs(ip.print(buffer, separator));
	return ::llc::appendBraced(output, buffer);
}

::llc::error_t	llc::appendQuoted			(::llc::asc_t & output, llc::SIPv4 ip, char separator) { 
	char				buffer[16]	= {};
	llc_necs(ip.print(buffer, separator));
	return ::llc::appendQuoted(output, buffer);
}

::llc::error_t	llc::appendBracedPrefixed	(::llc::asc_t & output, llc::SIPv4 ip, bool usePrefix, char prefix, char ip_separator) { 
	if(false == usePrefix) 
		return ::llc::appendBraced(output, ip, ip_separator); 
	llc_necs(output.push_back(prefix)); 
	return 1 + ::llc::appendBraced(output, ip, ip_separator); 
}

::llc::error_t	llc::appendQuotedPrefixed	(::llc::asc_t & output, llc::SIPv4 ip, bool usePrefix, char prefix, char ip_separator) { 
	if(false == usePrefix) 
		return ::llc::appendQuoted(output, ip, ip_separator); 
	llc_necs(output.push_back(prefix)); 
	return 1 + ::llc::appendQuoted(output, ip, ip_separator); 
}
