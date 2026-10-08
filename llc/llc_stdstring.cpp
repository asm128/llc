#include "llc_stdstring.h"
#include "llc_parse.h"

::llc::error_t			llc::stoull			(vcst_t input, uint64_t & outputNumber)	{
	return ::llc::parseIntegerDecimal(input, outputNumber);
}
