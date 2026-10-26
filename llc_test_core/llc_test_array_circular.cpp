#include "llc_test_core_suites.h"
#include "llc_array_circular.h"

GDEFINE_ENUM_TYPE(TEST_ARRAY_CIRCULAR_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_ARRAY_CIRCULAR_RESULT, CAPACITY , 0, "A circular array reports used and free capacity.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_CIRCULAR_RESULT, FIFO     , 1, "A circular array removes values in insertion order.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_CIRCULAR_RESULT, WRAP     , 2, "Wrapped contents are exposed as ordered left and right views.");

::llc::err_t testArrayCircular(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::aciru32<4> values;
	LLC_TEST_CHECK(results, TEST_ARRAY_CIRCULAR_RESULT_CAPACITY, values.used() != 0 || values.free() != 4 || values.byte_count() != 0);
	LLC_TEST_REQUIRE(results, TEST_ARRAY_CIRCULAR_RESULT_FIFO, 1 != values.push_back(1) || 2 != values.push_back(2) || 3 != values.push_back(3));

	::llc::u2_t value = {};
	LLC_TEST_REQUIRE(results, TEST_ARRAY_CIRCULAR_RESULT_FIFO, 2 != values.next(value));
	LLC_TEST_CHECK(results, TEST_ARRAY_CIRCULAR_RESULT_FIFO, value != 1 || values.used() != 2 || values.free() != 2);
	LLC_TEST_REQUIRE(results, TEST_ARRAY_CIRCULAR_RESULT_WRAP, 3 != values.push_back(4) || 4 != values.push_back(5));

	::llc::view<cnst ::llc::u2_t> left, right;
	LLC_TEST_REQUIRE(results, TEST_ARRAY_CIRCULAR_RESULT_WRAP, 4 != values.pair(left, right));
	LLC_TEST_CHECK(results, TEST_ARRAY_CIRCULAR_RESULT_WRAP, left.size() != 3 || right.size() != 1 || left[0] != 2 || left[1] != 3 || left[2] != 4 || right[0] != 5);

	cnst ::llc::u2_t expected[] = {2, 3, 4, 5};
	for(::llc::u2_t i = 0; i < ::llc::size(expected); ++i) {
		cnst ::llc::err_t remaining = values.next(value);
		LLC_TEST_CHECK(results, TEST_ARRAY_CIRCULAR_RESULT_FIFO, value != expected[i] || remaining != (::llc::err_t)(3 - i));
	}
	LLC_TEST_CHECK(results, TEST_ARRAY_CIRCULAR_RESULT_CAPACITY, values.used() != 0 || values.free() != 4);
	rtrn 0;
}
