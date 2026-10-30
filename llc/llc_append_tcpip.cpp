#include "llc_append_tcpip.h"
#include "llc_string_compose.h"

::llc::err_t	llc::appendString			(::llc::string & output, llc::SIPv4 ip, char separator) {
	char				buffer[16]	= {};
	::llc::err_t		len;
	if_fail_fe(len = ip.print(buffer, separator));
	if_fail_fe(output.append_string(buffer));
	return len;
}

::llc::err_t	llc::appendBraced			(::llc::string & output, llc::SIPv4 ip, char separator) {
	char				buffer[16]	= {};
	if_fail_fe(ip.print(buffer, separator));
	return ::llc::appendBraced(output, buffer);
}

::llc::err_t	llc::appendQuoted			(::llc::string & output, llc::SIPv4 ip, char separator) {
	char				buffer[16]	= {};
	if_fail_fe(ip.print(buffer, separator));
	return ::llc::appendQuoted(output, buffer);
}

::llc::err_t	llc::appendBracedPrefixed	(::llc::string & output, llc::SIPv4 ip, bool usePrefix, char prefix, char ip_separator) {
	if(false == usePrefix) 
		return ::llc::appendBraced(output, ip, ip_separator); 
	if_fail_fe(output.push_back(prefix)); 
	return 1 + ::llc::appendBraced(output, ip, ip_separator); 
}

::llc::err_t	llc::appendQuotedPrefixed	(::llc::string & output, llc::SIPv4 ip, bool usePrefix, char prefix, char ip_separator) {
	if(false == usePrefix) 
		return ::llc::appendQuoted(output, ip, ip_separator); 
	if_fail_fe(output.push_back(prefix)); 
	return 1 + ::llc::appendQuoted(output, ip, ip_separator); 
}
