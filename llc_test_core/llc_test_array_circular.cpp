#include "llc_test_core_suites.h"
#include "llc_array_circular.h"

GDEFINE_ENUM_TYPE(TEST_ARRAY_CIRCULAR_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_ARRAY_CIRCULAR_RESULT, CAPACITY , 0, "A circular array reports used and free capacity.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_CIRCULAR_RESULT, FIFO     , 1, "A circular array removes values in insertion order.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_CIRCULAR_RESULT, WRAP     , 2, "Wrapped contents are exposed as ordered left and right views.");

sttc ::llc::err_t testArrayCircularCapacity(::llc::ATestError & results) {
	::llc::aciru32<4> values;
	LLC_TEST_CHECK(results, TEST_ARRAY_CIRCULAR_RESULT_CAPACITY, values.used() != 0 || values.free() != 4 || values.byte_count() != 0);
	rtrn 0;
}

sttc ::llc::err_t testArrayCircularFIFO(::llc::ATestError & results) {
	::llc::aciru32<4> values;
	LLC_TEST_REQUIRE(results, TEST_ARRAY_CIRCULAR_RESULT_FIFO, 1 != values.push_back(1));
	LLC_TEST_REQUIRE(results, TEST_ARRAY_CIRCULAR_RESULT_FIFO, 2 != values.push_back(2));
	LLC_TEST_REQUIRE(results, TEST_ARRAY_CIRCULAR_RESULT_FIFO, 3 != values.push_back(3));
	::llc::u2_t value = {};
	LLC_TEST_REQUIRE(results, TEST_ARRAY_CIRCULAR_RESULT_FIFO, 2 != values.next(value));
	LLC_TEST_CHECK(results, TEST_ARRAY_CIRCULAR_RESULT_FIFO, value != 1);
	LLC_TEST_REQUIRE(results, TEST_ARRAY_CIRCULAR_RESULT_FIFO, 1 != values.next(value));
	LLC_TEST_CHECK(results, TEST_ARRAY_CIRCULAR_RESULT_FIFO, value != 2);
	LLC_TEST_REQUIRE(results, TEST_ARRAY_CIRCULAR_RESULT_FIFO, 0 != values.next(value));
	LLC_TEST_CHECK(results, TEST_ARRAY_CIRCULAR_RESULT_FIFO, value != 3 || values.used() != 0 || values.free() != 4);
	rtrn 0;
}

sttc ::llc::err_t testArrayCircularWrap(::llc::ATestError & results) {
	::llc::aciru32<4> values;
	LLC_TEST_REQUIRE(results, TEST_ARRAY_CIRCULAR_RESULT_WRAP, 1 != values.push_back(1));
	LLC_TEST_REQUIRE(results, TEST_ARRAY_CIRCULAR_RESULT_WRAP, 2 != values.push_back(2));
	LLC_TEST_REQUIRE(results, TEST_ARRAY_CIRCULAR_RESULT_WRAP, 3 != values.push_back(3));
	::llc::u2_t value = {};
	LLC_TEST_REQUIRE(results, TEST_ARRAY_CIRCULAR_RESULT_WRAP, 2 != values.next(value));
	LLC_TEST_REQUIRE(results, TEST_ARRAY_CIRCULAR_RESULT_WRAP, 3 != values.push_back(4));
	LLC_TEST_REQUIRE(results, TEST_ARRAY_CIRCULAR_RESULT_WRAP, 4 != values.push_back(5));
	::llc::view<cnst ::llc::u2_t> left, right;
	LLC_TEST_REQUIRE(results, TEST_ARRAY_CIRCULAR_RESULT_WRAP, 4 != values.pair(left, right));
	LLC_TEST_CHECK(results, TEST_ARRAY_CIRCULAR_RESULT_WRAP, left.size() != 3 || right.size() != 1 || left[0] != 2 || left[1] != 3 || left[2] != 4 || right[0] != 5);
	rtrn 0;
}

::llc::err_t testArrayCircular(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testArrayCircularCapacity(results));
	result = ::llc::min(result, ::testArrayCircularFIFO(results));
	result = ::llc::min(result, ::testArrayCircularWrap(results));
	rtrn result;
}
