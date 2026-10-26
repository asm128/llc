#include "llc_test_core_suites.h"
#include "llc_array_ptr.h"

GDEFINE_ENUM_TYPE(TEST_ARRAY_PTR_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_ARRAY_PTR_RESULT, CREATE  , 0, "Pointer arrays create and expose their referenced array.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_PTR_RESULT, SHARE   , 1, "Pointer-array copies share the same reference-counted array.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_PTR_RESULT, COLLECT , 2, "Arrays of array pointers retain their referenced arrays.");

sttc ::llc::err_t testArrayPtrCreate(::llc::ATestError & results) {
	::llc::pau2_t numbers;
	LLC_TEST_REQUIRE(results, TEST_ARRAY_PTR_RESULT_CREATE, 0 == numbers.create());
	LLC_TEST_REQUIRE(results, TEST_ARRAY_PTR_RESULT_CREATE, 0 != numbers->push_back(11));
	LLC_TEST_REQUIRE(results, TEST_ARRAY_PTR_RESULT_CREATE, 1 != numbers->push_back(22));
	LLC_TEST_CHECK(results, TEST_ARRAY_PTR_RESULT_CREATE, numbers->size() != 2 || numbers->oper[](0) != 11 || numbers->oper[](1) != 22);
	rtrn 0;
}

sttc ::llc::err_t testArrayPtrShare(::llc::ATestError & results) {
	::llc::pau2_t numbers;
	LLC_TEST_REQUIRE(results, TEST_ARRAY_PTR_RESULT_SHARE, 0 == numbers.create());
	LLC_TEST_REQUIRE(results, TEST_ARRAY_PTR_RESULT_SHARE, 0 != numbers->push_back(11));
	LLC_TEST_REQUIRE(results, TEST_ARRAY_PTR_RESULT_SHARE, 1 != numbers->push_back(22));
	::llc::pau2_t shared = numbers;
	LLC_TEST_CHECK(results, TEST_ARRAY_PTR_RESULT_SHARE, shared.get_ref() != numbers.get_ref());
	LLC_TEST_REQUIRE(results, TEST_ARRAY_PTR_RESULT_SHARE, 0 != numbers.clear());
	LLC_TEST_CHECK(results, TEST_ARRAY_PTR_RESULT_SHARE, (::llc::apod<::llc::u2_t>*)numbers != 0 || shared->size() != 2 || shared->oper[](1) != 22);
	rtrn 0;
}

sttc ::llc::err_t testArrayPtrCollect(::llc::ATestError & results) {
	::llc::pau2_t numbers;
	LLC_TEST_REQUIRE(results, TEST_ARRAY_PTR_RESULT_COLLECT, 0 == numbers.create());
	LLC_TEST_REQUIRE(results, TEST_ARRAY_PTR_RESULT_COLLECT, 0 != numbers->push_back(11));
	LLC_TEST_REQUIRE(results, TEST_ARRAY_PTR_RESULT_COLLECT, 1 != numbers->push_back(22));
	::llc::apau2_t arrays;
	LLC_TEST_REQUIRE(results, TEST_ARRAY_PTR_RESULT_COLLECT, 0 != arrays.push_back(numbers));
	LLC_TEST_REQUIRE(results, TEST_ARRAY_PTR_RESULT_COLLECT, 0 != numbers.clear());
	LLC_TEST_CHECK(results, TEST_ARRAY_PTR_RESULT_COLLECT, arrays.size() != 1 || arrays[0]->size() != 2 || arrays[0]->oper[](0) != 11);
	rtrn 0;
}

::llc::err_t testArrayPtr(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testArrayPtrCreate(results));
	result = ::llc::min(result, ::testArrayPtrShare(results));
	result = ::llc::min(result, ::testArrayPtrCollect(results));
	rtrn result;
}
