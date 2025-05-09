#include "llc_arduino_stream.h"

#include "llc_label.h"
#include "llc_array_pod.h"

#ifndef LLC_ARDUINO_STRING_H
#define LLC_ARDUINO_STRING_H

#ifdef LLC_ARDUINO
#	include <WString.h>

namespace llc
{
	ndsi	vcsc_t	tovcc   (cnst String & srcstr)					{ rtrn srcstr.length() ? vcsc_t{srcstr.begin(), (u2_t)srcstr.length()} : vcs{}; }
	stin	err_t	tovcc   (vcsc_t	& output, cnst String & srcstr)	{ rtrn (output = tovcc(srcstr)).size(); }
	stin	err_t	tolabel (vcsc_t	& output, cnst String & srcstr)	{ rtrn (output = label(tovcc(srcstr))).size(); }
	stin	err_t	toachar (asc_t	& output, cnst String & srcstr)	{ llc_necs(output.reserve(srcstr.length())); rtrn (output = tovcc(srcstr)).size(); }
	stin	err_t	append 	(asc_t	& output, cnst String & srcstr)	{ rtrn output.append(tovcc(srcstr)); }

	ndsi	vcsc_t	tolabel (cnst String & srcstr)	{ rtrn label(tovcc(srcstr)); }
	ndsi	asc_t	toachar (cnst String & srcstr)	{ rtrn tovcc(srcstr); }
	ndsi	vcsc_t	str		(cnst String & srcstr)	{ rtrn tovcc(srcstr); }
	ndsi	u2_t 	size	(cnst String & srcstr)	{ rtrn srcstr.length(); }
			err_t	rtrim	(String & trimmed, const String & input);
} // namespace
#endif // LLC_ARDUINO

#endif // LLC_ARDUINO_STRING_H
