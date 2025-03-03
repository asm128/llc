#include "llc_view.h"
#include "llc_safe.h"

#ifndef LLC_STDSTRING_H_23627
#define LLC_STDSTRING_H_23627
namespace llc
{
	//sinx	::llc::error_t	toupper		(char input)				{ return input (curByte >= 'A' && curByte <= 'Z') & ~0x20; }
	//sinx	::llc::error_t	tolower		(char input)				{ return input (curByte >= 'A' && curByte <= 'Z') | 0x20; }
	sinx	::llc::error_t	toupper		(char & input)				{ return (input >= 'a' && input <= 'z') ? input &= ~0x20U : 0; }
	sinx	::llc::error_t	tolower		(char & input)				{ return (input >= 'A' && input <= 'Z') ? input |= 0x20U  : 0; }

	::llc::error_t			tolower		(::llc::view<char> input);
	::llc::error_t			toupper		(::llc::view<char> input);
	::llc::error_t			stoull		(::llc::vcsc_c & input, uint64_t & output);
}
#endif // LLC_STDSTRING_H_23627
