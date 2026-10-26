#include "llc_test_core_suites.h"
#include "llc_array_obj.h"
#include "llc_array_pod.h"

GDEFINE_ENUM_TYPE(TEST_ARRAY_BASE_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_ARRAY_BASE_RESULT, RESIZE     , 0, "The shared resize helper resizes every supplied container.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_BASE_RESULT, RESIZE_OBJ , 1, "The object resize helper value-initializes new object elements.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_BASE_RESULT, CLEAR      , 2, "The shared clear helper clears every supplied container.");

sttc ::llc::err_t testArrayBaseResize(::llc::ATestError & results) {
	::llc::au2_t pod;
	::llc::aobj<::llc::u2_t> objects;
	LLC_TEST_REQUIRE(results, TEST_ARRAY_BASE_RESULT_RESIZE, 3 != ::llc::resize(3, pod, objects));
	LLC_TEST_CHECK(results, TEST_ARRAY_BASE_RESULT_RESIZE, pod.size() != 3 || objects.size() != 3);
	rtrn 0;
}

sttc ::llc::err_t testArrayBaseResizeObj(::llc::ATestError & results) {
	::llc::aobj<::llc::u2_t> objects;
	LLC_TEST_REQUIRE(results, TEST_ARRAY_BASE_RESULT_RESIZE_OBJ, 2 != objects.resize(2, ::llc::u2_t(7)));
	LLC_TEST_REQUIRE(results, TEST_ARRAY_BASE_RESULT_RESIZE_OBJ, 5 != ::llc::resize_obj(5, objects));
	LLC_TEST_CHECK(results, TEST_ARRAY_BASE_RESULT_RESIZE_OBJ, objects.size() != 5 || objects[0] != 7 || objects[1] != 7 || objects[2] != 0 || objects[3] != 0 || objects[4] != 0);
	rtrn 0;
}

sttc ::llc::err_t testArrayBaseClear(::llc::ATestError & results) {
	::llc::au2_t pod = {1, 2};
	::llc::aobj<::llc::u2_t> objects = {3, 4};
	LLC_TEST_REQUIRE(results, TEST_ARRAY_BASE_RESULT_CLEAR, 0 != ::llc::clear(pod, objects));
	LLC_TEST_CHECK(results, TEST_ARRAY_BASE_RESULT_CLEAR, pod.size() || objects.size());
	rtrn 0;
}

::llc::err_t testArrayBase(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testArrayBaseResize(results));
	result = ::llc::min(result, ::testArrayBaseResizeObj(results));
	result = ::llc::min(result, ::testArrayBaseClear(results));
	rtrn result;
}
