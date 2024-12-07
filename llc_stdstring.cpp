#include "llc_stdstring.h"
#include "llc_parse.h"

::llc::error_t			llc::toupper		(::llc::view<char> input)											{
	for(uint32_t iByte = 0, sizeHeader = input.size(); iByte < sizeHeader; ++iByte)
		::llc::toupper(input[iByte]);
	return 0;
}

::llc::error_t			llc::tolower		(::llc::view<char> input)											{
	for(uint32_t iByte = 0, sizeHeader = input.size(); iByte < sizeHeader; ++iByte)
		::llc::tolower(input[iByte]);
	return 0;
}

::llc::error_t			llc::stoull			(::llc::vcsc_c & input, uint64_t & outputNumber)	{
	return ::llc::parseIntegerDecimal(input, outputNumber);
}
