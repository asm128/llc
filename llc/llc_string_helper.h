#include "llc_string.h"

#ifndef LLC_STRING_HELPER_H_23627
#define LLC_STRING_HELPER_H_23627

namespace llc
{
	tplN2u
	inline	err_t	formatForSize(vcst_c text, sc_t (&output)[N], vcst_c pre, vcst_c post) {
		string			format			= {};
		u2_c			requiredSize	= pre.size() + post.size() + text.size() + 0xFF;
		if_fail_fe(format.resize(requiredSize, {}));
		sprintf_s(&format[0], format.size(), "%.*s%%.%us%.*s", LLC_VIEW_ARGS(pre), text.size(), LLC_VIEW_ARGS(post));
		return sprintf_s(output, N, format.begin(), text.begin());
	}
}

#endif // LLC_STRING_HELPER_H_23627