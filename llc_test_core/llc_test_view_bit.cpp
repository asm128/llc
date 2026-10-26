#include "llc_test_core_suites.h"
#include "llc_view_bit.h"

GDEFINE_ENUM_TYPE(TEST_VIEW_BIT_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_VIEW_BIT_RESULT, INDEX    , 0, "Bit indexing addresses individual bits across storage elements.");
GDEFINE_ENUM_VALUED(TEST_VIEW_BIT_RESULT, ITERATOR , 1, "Bit iterators traverse exactly the declared bit count.");
GDEFINE_ENUM_VALUED(TEST_VIEW_BIT_RESULT, REVERSE  , 2, "Bit reversal uses the declared bit count rather than the storage capacity.");

sttc ::llc::err_t testViewBitIndex(::llc::ATestError & results) {
	::llc::u0_t storage[2] = {};
	::llc::view_bit<::llc::u0_t> bits = {12, storage};
	bits[0] = true;
	bits[7] = true;
	bits[8] = true;
	bits[11] = true;
	LLC_TEST_CHECK(results, TEST_VIEW_BIT_RESULT_INDEX, bits.size() != 12 || storage[0] != 0x81 || storage[1] != 0x09);
	rtrn 0;
}

sttc ::llc::err_t testViewBitIterator(::llc::ATestError & results) {
	::llc::u0_t storage[2] = {0x81, 0x09};
	::llc::view_bit<::llc::u0_t> bits = {12, storage};
	::llc::u2_t visited = 0, enabled = 0;
	for(auto bit = bits.begin(), stop = bits.end(); bit != stop; ++bit) {
		++visited;
		enabled += (bool)*bit ? 1U : 0U;
	}
	LLC_TEST_CHECK(results, TEST_VIEW_BIT_RESULT_ITERATOR, visited != 12 || enabled != 4);
	rtrn 0;
}

sttc ::llc::err_t testViewBitReverse(::llc::ATestError & results) {
	::llc::u0_t storage[2] = {0x81, 0x09};
	::llc::view_bit<::llc::u0_t> bits = {12, storage};
	LLC_TEST_REQUIRE(results, TEST_VIEW_BIT_RESULT_REVERSE, 0 != ::llc::reverse_bits(bits));
	LLC_TEST_CHECK(results, TEST_VIEW_BIT_RESULT_REVERSE, storage[0] != 0x19 || storage[1] != 0x08);
	rtrn 0;
}

::llc::err_t testViewBit(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testViewBitIndex(results));
	result = ::llc::min(result, ::testViewBitIterator(results));
	result = ::llc::min(result, ::testViewBitReverse(results));
	rtrn result;
}
