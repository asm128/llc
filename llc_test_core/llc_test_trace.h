#ifndef LLC_TEST_TRACE_H
#define LLC_TEST_TRACE_H

#include "llc_test_core.h"

// Bounded previews keep generated binary cases readable; always disclose omitted bytes.
sttc inline void testTraceBytes(::llc::vcst_t label, ::llc::vcu0_t bytes) {
	stxp char digits[] = "0123456789ABCDEF";
	char text[64 * 3 + 1] = {};
	cnst ::llc::u2_t count = bytes.size() < 64 ? bytes.size() : 64;
	for(::llc::u2_t iByte = 0; iByte < count; ++iByte) {
		text[iByte * 3 + 0] = digits[bytes[iByte] >> 4];
		text[iByte * 3 + 1] = digits[bytes[iByte] & 15];
		text[iByte * 3 + 2] = ' ';
	}
	always_printf("%.*s: %u bytes; showing %u%s\n%s", LLC_VIEW_ARGS(label), bytes.size(), count, count < bytes.size() ? " (prefix only)" : "", text);
}

#endif
