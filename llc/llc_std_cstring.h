#include "llc_label.h"
#include "llc_string.h"

#include <cstring>

#ifndef LLC_STD_CSTRING_H
#define LLC_STD_CSTRING_H

namespace llc
{
	ndsi	vcst_t	tovcc   (sc_c * srcstr)						{ rtrn {srcstr, (uint32_t)-1}; }
	stin	err_t	tovcc   (vcst_t	& output, sc_c * srcstr)	{ rtrn (output = tovcc(srcstr)).size(); }
	stin	err_t	tolabel (vcst_t	& output, sc_c * srcstr)	{ rtrn (output = label(tovcc(srcstr))).size(); }
	stin	err_t	toachar (string	& output, sc_c * srcstr)	{
		cnst vcst_t			vsrc	= tovcc(srcstr);
		llc_necs(output.reserve(vsrc.size())); 
		rtrn (output = vsrc).size(); 
	}
	stin	err_t	append 	(string	& output, sc_c * srcstr)	{ rtrn output.append(tovcc(srcstr)); }

	ndsi	vcst_t	tolabel (sc_c * srcstr)	{ rtrn label(tovcc(srcstr)); }
	ndsi	string	toachar (sc_c * srcstr)	{ rtrn tovcc(srcstr); }
} // namespace llc

#endif // LLC_STD_CSTRING_H
