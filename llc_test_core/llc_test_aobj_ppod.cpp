#include "llc_test_core_suites.h"
#include "llc_aobj_ppod.h"

GDEFINE_ENUM_TYPE(TEST_AOBJ_PPOD_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_AOBJ_PPOD_RESULT, ALIAS          , 0, "The appod alias has the exact aobj<ppod<T>> container shape.");
GDEFINE_ENUM_VALUED(TEST_AOBJ_PPOD_RESULT, EMPTY          , 1, "A POD-pointer array begins empty.");
GDEFINE_ENUM_VALUED(TEST_AOBJ_PPOD_RESULT, OWNERSHIP      , 2, "A POD-pointer array retains values after source handles are cleared.");
GDEFINE_ENUM_VALUED(TEST_AOBJ_PPOD_RESULT, COPY_MOVE      , 3, "Copying and moving a POD-pointer array preserve shared storage.");
GDEFINE_ENUM_VALUED(TEST_AOBJ_PPOD_RESULT, SELF_ASSIGNMENT, 4, "Self-assignment preserves each POD handle and value.");
GDEFINE_ENUM_VALUED(TEST_AOBJ_PPOD_RESULT, VIEW_STABILITY , 5, "Array growth does not move separately allocated POD values.");
GDEFINE_ENUM_VALUED(TEST_AOBJ_PPOD_RESULT, BOUNDARIES     , 6, "First and final handle removal preserve the remaining POD values.");

sttc ::llc::err_t testAObjPPodAlias(::llc::ATestError & results) {
	static_assert(szof(::llc::appod<::llc::u2_t>) == szof(::llc::aobj<::llc::ppod<::llc::u2_t>>));
	::llc::appod<::llc::u2_t> values;
	::llc::ppod<::llc::u2_t> value;
	LLC_TEST_REQUIRE(results, TEST_AOBJ_PPOD_RESULT_ALIAS, 0 == value.create(::llc::u2_t(7)));
	LLC_TEST_REQUIRE(results, TEST_AOBJ_PPOD_RESULT_ALIAS, 0 != values.push_back(value));
	LLC_TEST_CHECK(results, TEST_AOBJ_PPOD_RESULT_ALIAS, values.size() != 1 || *values[0] != 7);
	rtrn 0;
}

sttc ::llc::err_t testAObjPPodEmpty(::llc::ATestError & results) {
	::llc::appod<::llc::u2_t> values;
	LLC_TEST_CHECK(results, TEST_AOBJ_PPOD_RESULT_EMPTY, values.size() != 0 || values.begin() != 0);
	rtrn 0;
}

sttc ::llc::err_t testAObjPPodOwnership(::llc::ATestError & results) {
	::llc::ppod<::llc::u2_t> first, second;
	LLC_TEST_REQUIRE(results, TEST_AOBJ_PPOD_RESULT_OWNERSHIP, 0 == first.create(::llc::u2_t(11)));
	LLC_TEST_REQUIRE(results, TEST_AOBJ_PPOD_RESULT_OWNERSHIP, 0 == second.create(::llc::u2_t(22)));
	::llc::appod<::llc::u2_t> values;
	LLC_TEST_REQUIRE(results, TEST_AOBJ_PPOD_RESULT_OWNERSHIP, 0 != values.push_back(first));
	LLC_TEST_REQUIRE(results, TEST_AOBJ_PPOD_RESULT_OWNERSHIP, 1 != values.push_back(second));
	first.clear();
	second.clear();
	LLC_TEST_CHECK(results, TEST_AOBJ_PPOD_RESULT_OWNERSHIP, *values[0] != 11 || *values[1] != 22 || values[0].get_ref()->References != 1 || values[1].get_ref()->References != 1);
	rtrn 0;
}

sttc ::llc::err_t testAObjPPodCopyMove(::llc::ATestError & results) {
	::llc::ppod<::llc::u2_t> owner;
	LLC_TEST_REQUIRE(results, TEST_AOBJ_PPOD_RESULT_COPY_MOVE, 0 == owner.create(::llc::u2_t(31)));
	::llc::appod<::llc::u2_t> source;
	LLC_TEST_REQUIRE(results, TEST_AOBJ_PPOD_RESULT_COPY_MOVE, 0 != source.push_back(owner));
	::llc::appod<::llc::u2_t> copied = source;
	::llc::appod<::llc::u2_t> moved = (::llc::appod<::llc::u2_t>&&)copied;
	LLC_TEST_CHECK(results, TEST_AOBJ_PPOD_RESULT_COPY_MOVE, copied.size() != 0 || moved.size() != 1 || moved[0] != source[0] || moved[0] != owner || moved[0].get_ref()->References != 3);
	rtrn 0;
}

sttc ::llc::err_t testAObjPPodSelfAssignment(::llc::ATestError & results) {
	::llc::ppod<::llc::u2_t> owner;
	LLC_TEST_REQUIRE(results, TEST_AOBJ_PPOD_RESULT_SELF_ASSIGNMENT, 0 == owner.create(::llc::u2_t(41)));
	::llc::appod<::llc::u2_t> values;
	LLC_TEST_REQUIRE(results, TEST_AOBJ_PPOD_RESULT_SELF_ASSIGNMENT, 0 != values.push_back(owner));
	cnst ::llc::gref<::llc::u2_t> * reference = values[0].get_ref();
	values = values;
	LLC_TEST_CHECK(results, TEST_AOBJ_PPOD_RESULT_SELF_ASSIGNMENT, values.size() != 1 || values[0].get_ref() != reference || *values[0] != 41 || values[0].get_ref()->References != 2);
	rtrn 0;
}

sttc ::llc::err_t testAObjPPodViewStability(::llc::ATestError & results) {
	::llc::ppod<::llc::u2_t> first;
	LLC_TEST_REQUIRE(results, TEST_AOBJ_PPOD_RESULT_VIEW_STABILITY, 0 == first.create(::llc::u2_t(51)));
	::llc::u2_t * firstAddress = first;
	::llc::appod<::llc::u2_t> values;
	LLC_TEST_REQUIRE(results, TEST_AOBJ_PPOD_RESULT_VIEW_STABILITY, 0 != values.push_back(first));
	for(::llc::u2_t i = 0; i < 12; ++i) {
		::llc::ppod<::llc::u2_t> next;
		LLC_TEST_REQUIRE(results, TEST_AOBJ_PPOD_RESULT_VIEW_STABILITY, 0 == next.create(::llc::u2_t(100 + i)));
		LLC_TEST_REQUIRE(results, TEST_AOBJ_PPOD_RESULT_VIEW_STABILITY, (::llc::s2_t)(i + 1) != values.push_back(next));
	}
	LLC_TEST_CHECK(results, TEST_AOBJ_PPOD_RESULT_VIEW_STABILITY, (::llc::u2_t*)values[0] != firstAddress || *values[0] != 51);
	rtrn 0;
}

sttc ::llc::err_t testAObjPPodBoundaries(::llc::ATestError & results) {
	::llc::appod<::llc::u2_t> values;
	for(::llc::u2_t i = 0; i < 3; ++i) {
		::llc::ppod<::llc::u2_t> owner;
		LLC_TEST_REQUIRE(results, TEST_AOBJ_PPOD_RESULT_BOUNDARIES, 0 == owner.create(::llc::u2_t(i + 1)));
		LLC_TEST_REQUIRE(results, TEST_AOBJ_PPOD_RESULT_BOUNDARIES, (::llc::s2_t)i != values.push_back(owner));
	}
	LLC_TEST_REQUIRE(results, TEST_AOBJ_PPOD_RESULT_BOUNDARIES, 2 != values.remove(0));
	LLC_TEST_CHECK(results, TEST_AOBJ_PPOD_RESULT_BOUNDARIES, *values[0] != 2 || *values[1] != 3);
	LLC_TEST_REQUIRE(results, TEST_AOBJ_PPOD_RESULT_BOUNDARIES, 1 != values.pop_back());
	LLC_TEST_CHECK(results, TEST_AOBJ_PPOD_RESULT_BOUNDARIES, values.size() != 1 || *values[0] != 2 || values[0].get_ref()->References != 1);
	rtrn 0;
}

::llc::err_t testAObjPPod(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testAObjPPodAlias(results));
	result = ::llc::min(result, ::testAObjPPodEmpty(results));
	result = ::llc::min(result, ::testAObjPPodOwnership(results));
	result = ::llc::min(result, ::testAObjPPodCopyMove(results));
	result = ::llc::min(result, ::testAObjPPodSelfAssignment(results));
	result = ::llc::min(result, ::testAObjPPodViewStability(results));
	result = ::llc::min(result, ::testAObjPPodBoundaries(results));
	rtrn result;
}
