#include "llc_array.h"

#ifndef LLC_WINDOWS_H
#define LLC_WINDOWS_H

namespace llc
{
#ifdef LLC_WINDOWS
	::llc::apod<char>	getWindowsErrorAsString	(const int32_t lastError); // Get the error message, if any.
	::llc::error_t		wcstombs				(::llc::apod<char> & output, const ::llc::view<wchar_t> input);
	::llc::error_t		mbstowcs				(::llc::apod<wchar_t> & output, ::llc::vcsc_c input);
#endif
} // namespace

#endif // LLC_WINDOWS_H