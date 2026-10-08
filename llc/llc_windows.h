#include "llc_array.h"
#include "llc_string.h"

#ifndef LLC_WINDOWS_H
#define LLC_WINDOWS_H

namespace llc
{
#ifdef LLC_WINDOWS
	string		getWindowsErrorAsString	(cnst s2_t lastError); // Get the error message, if any.
	error_t		wcstombs				(string & output, const view<wchar_t> input);
	error_t		mbstowcs				(apod<wchar_t> & output, vcst_t input);
#endif
} // namespace

#endif // LLC_WINDOWS_H
