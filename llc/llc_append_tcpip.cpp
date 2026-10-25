#include "llc_append_tcpip.h"
#include "llc_string_compose.h"

llc::error_t	llc::appendString			(asc_t & output, llc::SIPv4 ip, char separator) {
	char				buffer[16]	= {};
	error_t		len;
	if_fail_fe(len = ip.print(buffer, separator));
	if_fail_fe(output.append_string(buffer));
	return len;
}

llc::error_t	llc::appendBraced			(asc_t & output, llc::SIPv4 ip, char separator) {
	char				buffer[16]	= {};
	if_fail_fe(ip.print(buffer, separator));
	return appendBraced(output, buffer);
}

llc::error_t	llc::appendQuoted			(asc_t & output, llc::SIPv4 ip, char separator) {
	char				buffer[16]	= {};
	if_fail_fe(ip.print(buffer, separator));
	return appendQuoted(output, buffer);
}

llc::error_t	llc::appendBracedPrefixed	(asc_t & output, llc::SIPv4 ip, bool usePrefix, char prefix, char ip_separator) {
	if(false == usePrefix)
		return appendBraced(output, ip, ip_separator);
	if_fail_fe(output.push_back(prefix));
	return 1 + appendBraced(output, ip, ip_separator);
}

llc::error_t	llc::appendQuotedPrefixed	(asc_t & output, llc::SIPv4 ip, bool usePrefix, char prefix, char ip_separator) {
	if(false == usePrefix)
		return appendQuoted(output, ip, ip_separator);
	if_fail_fe(output.push_back(prefix));
	return 1 + appendQuoted(output, ip, ip_separator);
}
