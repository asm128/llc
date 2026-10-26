#include "llc_test_core_suites.h"
#include "llc_array_pod.h"

GDEFINE_ENUM_TYPE(TEST_ARRAY_POD_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_ARRAY_POD_RESULT, APPEND , 0, "POD arrays append values and report the first inserted index.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_POD_RESULT, INSERT , 1, "POD-array insertion preserves the surrounding order.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_POD_RESULT, REMOVE , 2, "Ordered and unordered removal follow their distinct contracts.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_POD_RESULT, ALIAS  , 3, "Appending an aliased view survives storage reallocation.");

::llc::err_t testArrayPod(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::au2_t values;
	cnst ::llc::u2_t initial[] = {1, 2, 3};
	LLC_TEST_REQUIRE(results, TEST_ARRAY_POD_RESULT_APPEND, 0 != values.append(initial));
	LLC_TEST_REQUIRE(results, TEST_ARRAY_POD_RESULT_APPEND, 3 != values.push_back(4));
	LLC_TEST_CHECK(results, TEST_ARRAY_POD_RESULT_APPEND, values.size() != 4 || values[3] != 4);

	cnst ::llc::u2_t inserted[] = {7, 8};
	LLC_TEST_REQUIRE(results, TEST_ARRAY_POD_RESULT_INSERT, 6 != values.insert(1, inserted));
	LLC_TEST_CHECK(results, TEST_ARRAY_POD_RESULT_INSERT, values[0] != 1 || values[1] != 7 || values[2] != 8 || values[3] != 2 || values[4] != 3 || values[5] != 4);

	LLC_TEST_REQUIRE(results, TEST_ARRAY_POD_RESULT_REMOVE, 5 != values.remove(3));
	LLC_TEST_REQUIRE(results, TEST_ARRAY_POD_RESULT_REMOVE, 4 != values.remove_unordered(1));
	::llc::u2_t removed = {};
	LLC_TEST_REQUIRE(results, TEST_ARRAY_POD_RESULT_REMOVE, 3 != values.pop_back(removed));
	LLC_TEST_CHECK(results, TEST_ARRAY_POD_RESULT_REMOVE, removed != 3 || values[0] != 1 || values[1] != 4 || values[2] != 8);

	::llc::au2_t aliased;
	cnst ::llc::u2_t source[] = {10, 20, 30, 40, 50};
	LLC_TEST_REQUIRE(results, TEST_ARRAY_POD_RESULT_ALIAS, 0 != aliased.append(source));
	cnst ::llc::view<cnst ::llc::u2_t> sourceView = {aliased.begin(), aliased.size()};
	LLC_TEST_REQUIRE(results, TEST_ARRAY_POD_RESULT_ALIAS, 5 != aliased.append(sourceView));
	LLC_TEST_CHECK(results, TEST_ARRAY_POD_RESULT_ALIAS, aliased.size() != 10 || aliased[0] != 10 || aliased[4] != 50 || aliased[5] != 10 || aliased[9] != 50 || aliased.begin()[aliased.size()] != 0);
	rtrn 0;
}
