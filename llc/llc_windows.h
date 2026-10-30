#include "llc_array.h"

#ifndef LLC_WINDOWS_H
#define LLC_WINDOWS_H

namespace llc
{
#ifdef LLC_WINDOWS
	apod<char>	getWindowsErrorAsString	(const s2_t lastError); // Get the error message, if any.
	err_t		wcstombs				(apod<char> & output, const view<wchar_t> input);
	err_t		mbstowcs				(apod<wchar_t> & output, vcs0_t input);
#endif
} // namespace

#endif // LLC_WINDOWS_H