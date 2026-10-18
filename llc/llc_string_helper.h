#include "llc_string.h"

#ifndef LLC_STRING_HELPER_H_23627
#define LLC_STRING_HELPER_H_23627

namespace llc
{
	tplN2u
	inline	err_t	formatForSize			(vcst_c text, sc_t (&output)[N], vcst_c pre, vcst_c post){
		string				format					= {};
		u2_c				requiredSize			= pre.size() + post.size() + text.size() + 0xFF;
		if_fail_fe(format.resize(formatBufferSize, {}));
		sprintf_s(&format[0], format.size(), "%.*s" "%%" ".%u" "s" "%.*s", pre.size(), pre.begin(), text.size(), post.size(), post.begin());
		return sprintf_s(output, maxlen, format.begin(), text.begin());
	}
}
#endif // LLC_STRING_HELPER_H_23627
