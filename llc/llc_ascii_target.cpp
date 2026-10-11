#include "llc_ascii_target.h"

::llc::err_t llc::asciiTargetClear(::llc::SASCIITarget& target, ::llc::u0_t character, ::llc::u1_t color) {
	if_true_fef(false == target.valid(), "Cannot clear mismatched ASCII target planes. Character metrics:%u x %u, cells:%u. Color metrics:%u x %u, cells:%u."
		, target.Characters.metrics().x, target.Characters.metrics().y, target.Characters.size()
		, target.Colors.metrics().x, target.Colors.metrics().y, target.Colors.size()
		);
	if_fail_fe(target.Characters.View.fill(character));
	if_fail_fe(target.Colors.View.fill(color));
	rtrn 0;
}
