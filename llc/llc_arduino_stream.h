#include "llc_typeint.h"

#ifndef LLC_ARDUINO_STREAM_H
#define LLC_ARDUINO_STREAM_H

#ifdef LLC_ARDUINO
#include <Stream.h>

namespace llc
{
	//ndsi	vcsc_t		tovcc   (cnst Stream   & srcstr)							{ rtrn srcstr.length() ? vcsc_t{srcstr.begin(), srcstr.length()} : vcs{}; }
	//stin	err_t	tovcc   (vcsc_t		& output, cnst Stream	& srcstr)	{ rtrn (output = tovcc(srcstr)).size(); }
	//stin	err_t	tolabel (vcsc_t		& output, cnst Stream	& srcstr)	{ rtrn (output = label(tovcc(srcstr))).size(); }
	//stin	err_t	toachar (asc_t	& output, cnst Stream	& srcstr)	{ llc_necs(output.reserve(srcstr.length())); rtrn (output = tovcc(srcstr)).size(); }
	//stin	err_t	append 	(asc_t	& output, cnst Stream	& srcstr)	{ rtrn output.append(tovcc(srcstr)); }

	//ndsi	vcsc_t		tolabel (cnst Stream & srcstr)	{ rtrn label(tovcc(srcstr)); }
	//ndsi	asc_t	toachar (cnst Stream & srcstr)	{ rtrn tovcc(srcstr); }
	//ndsi	vcsc_t		str		(cnst Stream & srcstr)	{ rtrn tovcc(srcstr); }
	ndsi	u2_t 		size	(cnst Stream & srcstr)	{ rtrn (*((Stream*)&srcstr)).available(); }
	ndsi	u2_t 		size	(Stream & srcstr)		{ rtrn srcstr.available(); }
} // namespace
#endif // LLC_ARDUINO

#endif // LLC_ARDUINO_STREAM_H
