#include "llc_view.h"

::llc::error_t			llc::trim				(::llc::vcsc_t & trimmed, ::llc::vcsc_c & original, ::llc::vcsc_c & characters) 	{
	u2_c				countChars				= ::llc::ltrim(trimmed, original, characters);
	return countChars + ::llc::rtrim(trimmed, trimmed, characters);
}

::llc::error_t			llc::ltrim				(::llc::vcsc_t & trimmed, ::llc::vcsc_c & original, ::llc::vcsc_c & characters)	{
	uint32_t					iChar					= 0;
	while(iChar < original.size() && original.size() >= (uint32_t)::llc::find(original[iChar], characters))
		++iChar;
	trimmed					= {original.begin() + iChar, original.size() - iChar};
	return iChar;
}

::llc::error_t			llc::rtrim				(::llc::vcsc_t & trimmed, ::llc::vcsc_c & original, ::llc::vcsc_c & characters)	{
	uint32_t					iChar					= original.size() - 1;
	while(iChar < original.size() && original.size() >= (uint32_t)::llc::find(original[iChar], characters))
		--iChar;
	u2_c				trimmedCount			= original.size() - iChar;
	trimmed					= {original.begin(), original.size() - trimmedCount + 1};
	return trimmedCount;
}
