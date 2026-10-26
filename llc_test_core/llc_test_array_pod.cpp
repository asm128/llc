#include "llc_test_core_suites.h"
#include "llc_array_pod.h"

GDEFINE_ENUM_TYPE(TEST_ARRAY_POD_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_ARRAY_POD_RESULT, APPEND           , 0, "POD arrays append values and report the first inserted index.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_POD_RESULT, PUSH             , 1, "POD arrays append one value and report its index.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_POD_RESULT, INSERT           , 2, "POD-array insertion preserves the surrounding order.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_POD_RESULT, REMOVE           , 3, "Ordered removal preserves the remaining order.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_POD_RESULT, REMOVE_UNORDERED , 4, "Unordered removal fills the removed position with the last value.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_POD_RESULT, POP              , 5, "Popping removes and returns the final value.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_POD_RESULT, ALIAS            , 6, "Appending an aliased view survives storage reallocation.");

sttc ::llc::err_t testArrayPodAppend(::llc::ATestError & results) {
	::llc::au2_t values;
	cnst ::llc::u2_t source[] = {1, 2, 3};
	LLC_TEST_REQUIRE(results, TEST_ARRAY_POD_RESULT_APPEND, 0 != values.append(source));
	LLC_TEST_CHECK(results, TEST_ARRAY_POD_RESULT_APPEND, values.size() != 3 || values[0] != 1 || values[1] != 2 || values[2] != 3 || values.begin()[values.size()] != 0);
	rtrn 0;
}

sttc ::llc::err_t testArrayPodPush(::llc::ATestError & results) {
	::llc::au2_t values;
	LLC_TEST_REQUIRE(results, TEST_ARRAY_POD_RESULT_PUSH, 0 != values.push_back(1));
	LLC_TEST_REQUIRE(results, TEST_ARRAY_POD_RESULT_PUSH, 1 != values.push_back(2));
	LLC_TEST_CHECK(results, TEST_ARRAY_POD_RESULT_PUSH, values.size() != 2 || values[0] != 1 || values[1] != 2 || values.begin()[values.size()] != 0);
	rtrn 0;
}

sttc ::llc::err_t testArrayPodInsert(::llc::ATestError & results) {
	::llc::au2_t values = {1, 2, 3, 4};
	cnst ::llc::u2_t inserted[] = {7, 8};
	LLC_TEST_REQUIRE(results, TEST_ARRAY_POD_RESULT_INSERT, 6 != values.insert(1, inserted));
	LLC_TEST_CHECK(results, TEST_ARRAY_POD_RESULT_INSERT, values[0] != 1 || values[1] != 7 || values[2] != 8 || values[3] != 2 || values[4] != 3 || values[5] != 4);
	rtrn 0;
}

sttc ::llc::err_t testArrayPodRemove(::llc::ATestError & results) {
	::llc::au2_t values = {1, 2, 3, 4};
	LLC_TEST_REQUIRE(results, TEST_ARRAY_POD_RESULT_REMOVE, 3 != values.remove(1));
	LLC_TEST_CHECK(results, TEST_ARRAY_POD_RESULT_REMOVE, values.size() != 3 || values[0] != 1 || values[1] != 3 || values[2] != 4);
	rtrn 0;
}

sttc ::llc::err_t testArrayPodRemoveUnordered(::llc::ATestError & results) {
	::llc::au2_t values = {1, 2, 3, 4};
	LLC_TEST_REQUIRE(results, TEST_ARRAY_POD_RESULT_REMOVE_UNORDERED, 3 != values.remove_unordered(1));
	LLC_TEST_CHECK(results, TEST_ARRAY_POD_RESULT_REMOVE_UNORDERED, values.size() != 3 || values[0] != 1 || values[1] != 4 || values[2] != 3);
	rtrn 0;
}

sttc ::llc::err_t testArrayPodPop(::llc::ATestError & results) {
	::llc::au2_t values = {1, 2, 3};
	::llc::u2_t removed = {};
	LLC_TEST_REQUIRE(results, TEST_ARRAY_POD_RESULT_POP, 2 != values.pop_back(removed));
	LLC_TEST_CHECK(results, TEST_ARRAY_POD_RESULT_POP, removed != 3 || values.size() != 2 || values[0] != 1 || values[1] != 2);
	rtrn 0;
}

sttc ::llc::err_t testArrayPodAlias(::llc::ATestError & results) {
	::llc::au2_t values;
	cnst ::llc::u2_t source[] = {10, 20, 30, 40, 50};
	LLC_TEST_REQUIRE(results, TEST_ARRAY_POD_RESULT_ALIAS, 0 != values.append(source));
	cnst ::llc::view<cnst ::llc::u2_t> sourceView = {values.begin(), values.size()};
	LLC_TEST_REQUIRE(results, TEST_ARRAY_POD_RESULT_ALIAS, 5 != values.append(sourceView));
	LLC_TEST_CHECK(results, TEST_ARRAY_POD_RESULT_ALIAS, values.size() != 10 || values[0] != 10 || values[4] != 50 || values[5] != 10 || values[9] != 50 || values.begin()[values.size()] != 0);
	rtrn 0;
}

::llc::err_t testArrayPod(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testArrayPodAppend(results));
	result = ::llc::min(result, ::testArrayPodPush(results));
	result = ::llc::min(result, ::testArrayPodInsert(results));
	result = ::llc::min(result, ::testArrayPodRemove(results));
	result = ::llc::min(result, ::testArrayPodRemoveUnordered(results));
	result = ::llc::min(result, ::testArrayPodPop(results));
	result = ::llc::min(result, ::testArrayPodAlias(results));
	rtrn result;
}
