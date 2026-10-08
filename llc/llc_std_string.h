#include "llc_label.h"
#include "llc_string.h"

#include <string>

#ifndef LLC_STD_STRING_H
#define LLC_STD_STRING_H

namespace llc
{
	ndsi	vcst_t	tovcc   (const std::string & srcstr)					{ return {srcstr.data(), (uint32_t)srcstr.size()}; }
	stin	error_t	tovcc   (vcst_t	& output, const std::string & srcstr)	{ return (output = tovcc(srcstr)).size(); }
	stin	error_t	tolabel (vcst_t	& output, const std::string & srcstr)	{ return (output = label(tovcc(srcstr))).size(); }
	stin	error_t	toachar (string	& output, const std::string & srcstr)	{ llc_necs(output.reserve((uint32_t)srcstr.length())); return (output = tovcc(srcstr)).size(); }
	stin	error_t	append 	(string	& output, const std::string & srcstr)	{ return output.append(tovcc(srcstr)); }

	ndsi	vcst_t	tolabel (const std::string & srcstr) 					{ return label(tovcc(srcstr)); }
	ndsi	string	toachar (const std::string & srcstr) 					{ return tovcc(srcstr); }
} // namespace llc

#endif // LLC_STD_STRING_H
