#include "llc_view.h"
#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(VIEW_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SIZE		, 0, "view<> does not expose its element and byte counts.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FIND		, 1, "view<>::find() returned an unexpected index.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE		, 2, "view<>::slice() did not expose the requested subrange.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, MUTATION	, 3, "A mutable view did not update its source storage.");

::llc::err_t testView(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	::llc::u2_t values[] = {10, 20, 30, 40};
	::llc::vu2_t all = {values};
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SIZE, all.size() != 4 || all.byte_count() != szof(values));
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FIND, all.find(30U) != 2 || all.find(99U) != -1);
	::llc::vu2_t middle;
	cnst ::llc::err_t sliceResult = all.slice(middle, 1, 2);
	LLC_TEST_REQUIRE(errors, VIEW_TEST_RESULT_SLICE, sliceResult != 2 || middle.size() != 2 || middle[0] != 20 || middle[1] != 30);
	middle[0] = 25;
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_MUTATION, values[1] != 25);
	rtrn 0;
}
