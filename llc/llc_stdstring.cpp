#include "llc_stdstring.h"
#include "llc_parse.h"

::llc::err_t			llc::stoull			(vcst_t input, u3_t & outputNumber)	{
	return ::llc::parseIntegerDecimal(input, outputNumber);
}
