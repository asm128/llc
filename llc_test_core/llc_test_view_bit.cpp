#include "llc_test_core_suites.h"
#include "llc_view_bit.h"

GDEFINE_ENUM_TYPE(TEST_VIEW_BIT_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_VIEW_BIT_RESULT, INDEX    , 0, "Bit indexing addresses individual bits across storage elements.");
GDEFINE_ENUM_VALUED(TEST_VIEW_BIT_RESULT, ITERATOR , 1, "Bit iterators traverse exactly the declared bit count.");
GDEFINE_ENUM_VALUED(TEST_VIEW_BIT_RESULT, REVERSE  , 2, "Bit reversal uses the declared bit count rather than the storage capacity.");

::llc::err_t testViewBit(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::u0_t storage[2] = {};
	::llc::view_bit<::llc::u0_t> bits = {12, storage};
	bits[0] = true;
	bits[7] = true;
	bits[8] = true;
	bits[11] = true;
	LLC_TEST_CHECK(results, TEST_VIEW_BIT_RESULT_INDEX, bits.size() != 12 || storage[0] != 0x81 || storage[1] != 0x09);

	::llc::u2_t enabled = 0;
	for(auto bit = bits.begin(), stop = bits.end(); bit != stop; ++bit)
		enabled += (bool)*bit ? 1U : 0U;
	LLC_TEST_CHECK(results, TEST_VIEW_BIT_RESULT_ITERATOR, enabled != 4);

	LLC_TEST_REQUIRE(results, TEST_VIEW_BIT_RESULT_REVERSE, 0 != ::llc::reverse_bits(bits));
	LLC_TEST_CHECK(results, TEST_VIEW_BIT_RESULT_REVERSE, storage[0] != 0x19 || storage[1] != 0x08);
	rtrn 0;
}
