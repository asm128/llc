#include "llc_test_core_suites.h"
#include "llc_view.h"

GDEFINE_ENUM_TYPE(TEST_VIEW_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_VIEW_RESULT, METRICS , 0, "A view reports the extent of its referenced storage.");
GDEFINE_ENUM_VALUED(TEST_VIEW_RESULT, SLICE   , 1, "A slice keeps the requested range and aliases its source.");
GDEFINE_ENUM_VALUED(TEST_VIEW_RESULT, FILL    , 2, "View fill mutates only the selected interval.");
GDEFINE_ENUM_VALUED(TEST_VIEW_RESULT, FIND    , 3, "Forward and reverse search return the matching element positions.");
GDEFINE_ENUM_VALUED(TEST_VIEW_RESULT, REVERSE , 4, "Reverting a view reverses the referenced sequence in place.");

sttc ::llc::err_t testViewMetrics(::llc::ATestError & results) {
	::llc::u2_t storage[] = {1, 2, 3, 4, 5};
	cnst ::llc::view<::llc::u2_t> values = storage;
	LLC_TEST_CHECK(results, TEST_VIEW_RESULT_METRICS, values.size() != 5 || values.byte_count() != sizeof(storage));
	rtrn 0;
}

sttc ::llc::err_t testViewSlice(::llc::ATestError & results) {
	::llc::u2_t storage[] = {1, 2, 3, 4, 5};
	::llc::view<::llc::u2_t> values = storage;
	::llc::view<::llc::u2_t> middle;
	LLC_TEST_REQUIRE(results, TEST_VIEW_RESULT_SLICE, 3 != values.slice(middle, 1, 3));
	middle[0] = 7;
	LLC_TEST_CHECK(results, TEST_VIEW_RESULT_SLICE, middle.size() != 3 || storage[1] != 7);
	rtrn 0;
}

sttc ::llc::err_t testViewFill(::llc::ATestError & results) {
	::llc::u2_t storage[] = {1, 2, 3, 4, 5};
	::llc::view<::llc::u2_t> values = storage;
	LLC_TEST_REQUIRE(results, TEST_VIEW_RESULT_FILL, 2 != values.fill(9, 2, 4));
	LLC_TEST_CHECK(results, TEST_VIEW_RESULT_FILL, storage[0] != 1 || storage[1] != 2 || storage[2] != 9 || storage[3] != 9 || storage[4] != 5);
	rtrn 0;
}

sttc ::llc::err_t testViewFind(::llc::ATestError & results) {
	::llc::u2_t storage[] = {1, 2, 3, 2, 5};
	cnst ::llc::view<::llc::u2_t> values = storage;
	LLC_TEST_CHECK(results, TEST_VIEW_RESULT_FIND, 1 != values.find(2) || 3 != values.rfind(2) || -1 != values.find(9));
	rtrn 0;
}

sttc ::llc::err_t testViewReverse(::llc::ATestError & results) {
	::llc::u2_t storage[] = {1, 2, 3, 4, 5};
	::llc::view<::llc::u2_t> values = storage;
	LLC_TEST_REQUIRE(results, TEST_VIEW_RESULT_REVERSE, 0 != values.revert());
	LLC_TEST_CHECK(results, TEST_VIEW_RESULT_REVERSE, storage[0] != 5 || storage[1] != 4 || storage[2] != 3 || storage[3] != 2 || storage[4] != 1);
	rtrn 0;
}

::llc::err_t testView(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testViewMetrics(results));
	result = ::llc::min(result, ::testViewSlice(results));
	result = ::llc::min(result, ::testViewFill(results));
	result = ::llc::min(result, ::testViewFind(results));
	result = ::llc::min(result, ::testViewReverse(results));
	rtrn result;
}
