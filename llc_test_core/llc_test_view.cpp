#include "llc_test_core_suites.h"
#include "llc_view.h"

GDEFINE_ENUM_TYPE(TEST_VIEW_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_VIEW_RESULT, METRICS , 0, "A view reports the extent of its referenced storage.");
GDEFINE_ENUM_VALUED(TEST_VIEW_RESULT, SLICE   , 1, "A slice keeps the requested range and aliases its source.");
GDEFINE_ENUM_VALUED(TEST_VIEW_RESULT, CONTENT , 2, "View mutation and search operate on the referenced elements.");
GDEFINE_ENUM_VALUED(TEST_VIEW_RESULT, REVERSE , 3, "Reverting a view reverses the referenced sequence in place.");

::llc::err_t testView(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::u2_t storage[] = {1, 2, 3, 2, 5};
	::llc::view<::llc::u2_t> values = storage;
	LLC_TEST_CHECK(results, TEST_VIEW_RESULT_METRICS, values.size() != 5 || values.byte_count() != sizeof(storage));

	::llc::view<::llc::u2_t> middle;
	LLC_TEST_REQUIRE(results, TEST_VIEW_RESULT_SLICE, 3 != values.slice(middle, 1, 3));
	middle[0] = 7;
	LLC_TEST_CHECK(results, TEST_VIEW_RESULT_SLICE, middle.size() != 3 || storage[1] != 7);

	LLC_TEST_CHECK(results, TEST_VIEW_RESULT_CONTENT, 2 != values.fill(9, 2, 4));
	LLC_TEST_CHECK(results, TEST_VIEW_RESULT_CONTENT, storage[2] != 9 || storage[3] != 9 || 2 != values.find(9) || 3 != values.rfind(9));

	LLC_TEST_REQUIRE(results, TEST_VIEW_RESULT_REVERSE, 0 != values.revert());
	LLC_TEST_CHECK(results, TEST_VIEW_RESULT_REVERSE, storage[0] != 5 || storage[1] != 9 || storage[2] != 9 || storage[3] != 7 || storage[4] != 1);
	rtrn 0;
}
