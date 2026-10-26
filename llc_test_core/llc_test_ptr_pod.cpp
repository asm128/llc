#include "llc_test_core_suites.h"
#include "llc_ptr_pod.h"

GDEFINE_ENUM_TYPE(TEST_PTR_POD_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_PTR_POD_RESULT, ALLOCATE , 0, "A POD pointer allocates writable storage and owns one reference.");
GDEFINE_ENUM_VALUED(TEST_PTR_POD_RESULT, COPY     , 1, "Copying a POD pointer shares its storage and increments the reference count.");
GDEFINE_ENUM_VALUED(TEST_PTR_POD_RESULT, MOVE     , 2, "Moving a POD pointer empties the source without changing the reference count.");
GDEFINE_ENUM_VALUED(TEST_PTR_POD_RESULT, CLEAR    , 3, "Clearing one POD handle leaves the shared value alive for the other handle.");
GDEFINE_ENUM_VALUED(TEST_PTR_POD_RESULT, CREATE   , 4, "Creating a POD value initializes newly owned storage.");

sttc ::llc::err_t testPtrPODAllocate(::llc::ATestError & results) {
	::llc::ppod<::llc::u2_t> value;
	::llc::u2_t * allocated = value.allocate();
	LLC_TEST_REQUIRE(results, TEST_PTR_POD_RESULT_ALLOCATE, 0 == allocated);
	*allocated = 37;
	LLC_TEST_CHECK(results, TEST_PTR_POD_RESULT_ALLOCATE, (::llc::u2_t*)value != allocated || *value != 37 || value.get_ref()->References != 1);
	rtrn 0;
}

sttc ::llc::err_t testPtrPODCopy(::llc::ATestError & results) {
	::llc::ppod<::llc::u2_t> value;
	LLC_TEST_REQUIRE(results, TEST_PTR_POD_RESULT_COPY, 0 == value.create(::llc::u2_t(37)));
	::llc::ppod<::llc::u2_t> copied = value;
	LLC_TEST_CHECK(results, TEST_PTR_POD_RESULT_COPY, copied != value || copied.get_ref()->References != 2 || *copied != 37);
	rtrn 0;
}

sttc ::llc::err_t testPtrPODMove(::llc::ATestError & results) {
	::llc::ppod<::llc::u2_t> source;
	LLC_TEST_REQUIRE(results, TEST_PTR_POD_RESULT_MOVE, 0 == source.create(::llc::u2_t(37)));
	::llc::ppod<::llc::u2_t> moved = (::llc::ppod<::llc::u2_t>&&)source;
	LLC_TEST_CHECK(results, TEST_PTR_POD_RESULT_MOVE, 0 != (::llc::u2_t*)source || *moved != 37 || moved.get_ref()->References != 1);
	rtrn 0;
}

sttc ::llc::err_t testPtrPODClear(::llc::ATestError & results) {
	::llc::ppod<::llc::u2_t> value;
	LLC_TEST_REQUIRE(results, TEST_PTR_POD_RESULT_CLEAR, 0 == value.create(::llc::u2_t(37)));
	::llc::ppod<::llc::u2_t> retained = value;
	LLC_TEST_REQUIRE(results, TEST_PTR_POD_RESULT_CLEAR, 0 != value.clear());
	LLC_TEST_CHECK(results, TEST_PTR_POD_RESULT_CLEAR, 0 != (::llc::u2_t*)value || *retained != 37 || retained.get_ref()->References != 1);
	rtrn 0;
}

sttc ::llc::err_t testPtrPODCreate(::llc::ATestError & results) {
	::llc::ppod<::llc::u2_t> value;
	LLC_TEST_REQUIRE(results, TEST_PTR_POD_RESULT_CREATE, 0 == value.create(::llc::u2_t(81)));
	LLC_TEST_CHECK(results, TEST_PTR_POD_RESULT_CREATE, *value != 81 || value.get_ref()->References != 1);
	rtrn 0;
}

::llc::err_t testPtrPOD(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testPtrPODAllocate(results));
	result = ::llc::min(result, ::testPtrPODCopy(results));
	result = ::llc::min(result, ::testPtrPODMove(results));
	result = ::llc::min(result, ::testPtrPODClear(results));
	result = ::llc::min(result, ::testPtrPODCreate(results));
	rtrn result;
}
