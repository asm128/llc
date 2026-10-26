#include "llc_test_core_suites.h"
#include "llc_array_static.h"

GDEFINE_ENUM_TYPE(TEST_ARRAY_STATIC_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_ARRAY_STATIC_RESULT, METRICS , 0, "A static array exposes its compile-time extent and byte count.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_STATIC_RESULT, SLICE   , 1, "A static-array slice aliases the selected storage.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_STATIC_RESULT, VIEW    , 2, "A static array converts to a const view of the same elements.");

sttc ::llc::err_t testArrayStaticMetrics(::llc::ATestError & results) {
	::llc::astatic<::llc::u2_t, 4> values = {{1, 2, 3, 4}};
	LLC_TEST_CHECK(results, TEST_ARRAY_STATIC_RESULT_METRICS, values.size() != 4 || values.byte_count() != sizeof(values.Storage));
	rtrn 0;
}

sttc ::llc::err_t testArrayStaticSlice(::llc::ATestError & results) {
	::llc::astatic<::llc::u2_t, 4> values = {{1, 2, 3, 4}};
	::llc::view<::llc::u2_t> middle;
	LLC_TEST_REQUIRE(results, TEST_ARRAY_STATIC_RESULT_SLICE, 2 != values.slice(middle, 1, 2));
	middle[1] = 9;
	LLC_TEST_CHECK(results, TEST_ARRAY_STATIC_RESULT_SLICE, values[2] != 9);
	rtrn 0;
}

sttc ::llc::err_t testArrayStaticView(::llc::ATestError & results) {
	::llc::astatic<::llc::u2_t, 4> values = {{1, 2, 3, 4}};
	cnst ::llc::view<cnst ::llc::u2_t> readOnly = values;
	LLC_TEST_CHECK(results, TEST_ARRAY_STATIC_RESULT_VIEW, readOnly.size() != values.size() || readOnly.begin() != values.begin() || readOnly[0] != 1 || readOnly[3] != 4);
	rtrn 0;
}

::llc::err_t testArrayStatic(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testArrayStaticMetrics(results));
	result = ::llc::min(result, ::testArrayStaticSlice(results));
	result = ::llc::min(result, ::testArrayStaticView(results));
	rtrn result;
}
