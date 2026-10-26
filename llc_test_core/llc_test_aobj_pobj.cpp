#include "llc_test_core_suites.h"
#include "llc_aobj_pobj.h"

namespace {
	struct SAObjPObjProbe {
		sttc ::llc::s2_t Alive;
		sttc ::llc::s2_t Constructed;
		sttc ::llc::s2_t Destroyed;
		::llc::u2_t Value = {};

		SAObjPObjProbe() { ++Alive; ++Constructed; }
		SAObjPObjProbe(::llc::u2_t value) : Value{value} { ++Alive; ++Constructed; }
		SAObjPObjProbe(cnst SAObjPObjProbe & other) : Value{other.Value} { ++Alive; ++Constructed; }
		~SAObjPObjProbe() { --Alive; ++Destroyed; }
	};

	::llc::s2_t SAObjPObjProbe::Alive = {};
	::llc::s2_t SAObjPObjProbe::Constructed = {};
	::llc::s2_t SAObjPObjProbe::Destroyed = {};

	sttc void aobjPobjReset() {
		SAObjPObjProbe::Alive = SAObjPObjProbe::Constructed = SAObjPObjProbe::Destroyed = {};
	}
}

GDEFINE_ENUM_TYPE(TEST_AOBJ_POBJ_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_AOBJ_POBJ_RESULT, ALIASES        , 0, "Object-pointer array aliases preserve their declared nested container shapes.");
GDEFINE_ENUM_VALUED(TEST_AOBJ_POBJ_RESULT, EMPTY          , 1, "Object-pointer arrays and owners begin empty and null.");
GDEFINE_ENUM_VALUED(TEST_AOBJ_POBJ_RESULT, OWNERSHIP      , 2, "An object-pointer array retains pointees after source handles are cleared.");
GDEFINE_ENUM_VALUED(TEST_AOBJ_POBJ_RESULT, COPY_MOVE      , 3, "Copying and moving an object-pointer array preserve shared pointee ownership.");
GDEFINE_ENUM_VALUED(TEST_AOBJ_POBJ_RESULT, SELF_ASSIGNMENT, 4, "Self-assignment preserves every object handle and pointee.");
GDEFINE_ENUM_VALUED(TEST_AOBJ_POBJ_RESULT, VIEW_STABILITY , 5, "Array storage growth does not move the separately allocated pointees.");
GDEFINE_ENUM_VALUED(TEST_AOBJ_POBJ_RESULT, BOUNDARIES     , 6, "Removing the first and final handles releases exactly their pointees.");

sttc ::llc::err_t testAObjPObjAliases(::llc::ATestError & results) {
	static_assert(szof(::llc::apobj<SAObjPObjProbe>) == szof(::llc::aobj<::llc::pobj<SAObjPObjProbe>>));
	static_assert(szof(::llc::paobj<SAObjPObjProbe>) == szof(::llc::pobj<::llc::aobj<SAObjPObjProbe>>));
	::llc::apo<SAObjPObjProbe> handles;
	::llc::paobj<SAObjPObjProbe> ownedArray;
	LLC_TEST_REQUIRE(results, TEST_AOBJ_POBJ_RESULT_ALIASES, 0 == ownedArray.create());
	::llc::apaobj<SAObjPObjProbe> arrayOwners;
	LLC_TEST_REQUIRE(results, TEST_AOBJ_POBJ_RESULT_ALIASES, 0 != arrayOwners.push_back(ownedArray));
	::llc::aapobj<SAObjPObjProbe> nestedHandleArrays;
	LLC_TEST_REQUIRE(results, TEST_AOBJ_POBJ_RESULT_ALIASES, 0 != nestedHandleArrays.push_back(handles));
	LLC_TEST_CHECK(results, TEST_AOBJ_POBJ_RESULT_ALIASES, arrayOwners.size() != 1 || nestedHandleArrays.size() != 1 || arrayOwners[0]->size() != 0 || nestedHandleArrays[0].size() != 0);
	rtrn 0;
}

sttc ::llc::err_t testAObjPObjEmpty(::llc::ATestError & results) {
	::llc::apobj<SAObjPObjProbe> handles;
	::llc::paobj<SAObjPObjProbe> ownedArray;
	LLC_TEST_CHECK(results, TEST_AOBJ_POBJ_RESULT_EMPTY, handles.size() != 0 || 0 != (::llc::aobj<SAObjPObjProbe>*)ownedArray);
	rtrn 0;
}

sttc ::llc::err_t testAObjPObjOwnership(::llc::ATestError & results) {
	::aobjPobjReset();
	{
		::llc::pobj<SAObjPObjProbe> first, second;
		LLC_TEST_REQUIRE(results, TEST_AOBJ_POBJ_RESULT_OWNERSHIP, 0 == first.create(::llc::u2_t(11)));
		LLC_TEST_REQUIRE(results, TEST_AOBJ_POBJ_RESULT_OWNERSHIP, 0 == second.create(::llc::u2_t(22)));
		::llc::apobj<SAObjPObjProbe> handles;
		LLC_TEST_REQUIRE(results, TEST_AOBJ_POBJ_RESULT_OWNERSHIP, 0 != handles.push_back(first));
		LLC_TEST_REQUIRE(results, TEST_AOBJ_POBJ_RESULT_OWNERSHIP, 1 != handles.push_back(second));
		first.clear();
		second.clear();
		LLC_TEST_CHECK(results, TEST_AOBJ_POBJ_RESULT_OWNERSHIP, SAObjPObjProbe::Alive != 2 || handles[0]->Value != 11 || handles[1]->Value != 22);
		handles.clear();
		LLC_TEST_CHECK(results, TEST_AOBJ_POBJ_RESULT_OWNERSHIP, SAObjPObjProbe::Alive != 0);
	}
	LLC_TEST_CHECK(results, TEST_AOBJ_POBJ_RESULT_OWNERSHIP, SAObjPObjProbe::Constructed != 2 || SAObjPObjProbe::Destroyed != 2);
	rtrn 0;
}

sttc ::llc::err_t testAObjPObjCopyMove(::llc::ATestError & results) {
	::aobjPobjReset();
	{
		::llc::pobj<SAObjPObjProbe> owner;
		LLC_TEST_REQUIRE(results, TEST_AOBJ_POBJ_RESULT_COPY_MOVE, 0 == owner.create(::llc::u2_t(31)));
		::llc::apobj<SAObjPObjProbe> source;
		LLC_TEST_REQUIRE(results, TEST_AOBJ_POBJ_RESULT_COPY_MOVE, 0 != source.push_back(owner));
		::llc::apobj<SAObjPObjProbe> copied = source;
		::llc::apobj<SAObjPObjProbe> moved = (::llc::apobj<SAObjPObjProbe>&&)copied;
		LLC_TEST_CHECK(results, TEST_AOBJ_POBJ_RESULT_COPY_MOVE, copied.size() != 0 || moved.size() != 1 || moved[0] != source[0] || moved[0] != owner || SAObjPObjProbe::Alive != 1);
	}
	LLC_TEST_CHECK(results, TEST_AOBJ_POBJ_RESULT_COPY_MOVE, SAObjPObjProbe::Alive != 0 || SAObjPObjProbe::Constructed != SAObjPObjProbe::Destroyed);
	rtrn 0;
}

sttc ::llc::err_t testAObjPObjSelfAssignment(::llc::ATestError & results) {
	::aobjPobjReset();
	{
		::llc::pobj<SAObjPObjProbe> owner;
		LLC_TEST_REQUIRE(results, TEST_AOBJ_POBJ_RESULT_SELF_ASSIGNMENT, 0 == owner.create(::llc::u2_t(41)));
		::llc::apobj<SAObjPObjProbe> handles;
		LLC_TEST_REQUIRE(results, TEST_AOBJ_POBJ_RESULT_SELF_ASSIGNMENT, 0 != handles.push_back(owner));
		cnst ::llc::gref<SAObjPObjProbe> * reference = handles[0].get_ref();
		handles = handles;
		LLC_TEST_CHECK(results, TEST_AOBJ_POBJ_RESULT_SELF_ASSIGNMENT, handles.size() != 1 || handles[0].get_ref() != reference || handles[0]->Value != 41 || SAObjPObjProbe::Alive != 1);
	}
	LLC_TEST_CHECK(results, TEST_AOBJ_POBJ_RESULT_SELF_ASSIGNMENT, SAObjPObjProbe::Alive != 0 || SAObjPObjProbe::Constructed != SAObjPObjProbe::Destroyed);
	rtrn 0;
}

sttc ::llc::err_t testAObjPObjViewStability(::llc::ATestError & results) {
	::aobjPobjReset();
	{
		::llc::pobj<SAObjPObjProbe> first;
		LLC_TEST_REQUIRE(results, TEST_AOBJ_POBJ_RESULT_VIEW_STABILITY, 0 == first.create(::llc::u2_t(51)));
		SAObjPObjProbe * firstAddress = first;
		::llc::apobj<SAObjPObjProbe> handles;
		LLC_TEST_REQUIRE(results, TEST_AOBJ_POBJ_RESULT_VIEW_STABILITY, 0 != handles.push_back(first));
		for(::llc::u2_t i = 0; i < 12; ++i) {
			::llc::pobj<SAObjPObjProbe> next;
			LLC_TEST_REQUIRE(results, TEST_AOBJ_POBJ_RESULT_VIEW_STABILITY, 0 == next.create(::llc::u2_t(100 + i)));
			LLC_TEST_REQUIRE(results, TEST_AOBJ_POBJ_RESULT_VIEW_STABILITY, (::llc::s2_t)(i + 1) != handles.push_back(next));
		}
		LLC_TEST_CHECK(results, TEST_AOBJ_POBJ_RESULT_VIEW_STABILITY, (SAObjPObjProbe*)handles[0] != firstAddress || handles[0]->Value != 51 || SAObjPObjProbe::Alive != 13);
	}
	LLC_TEST_CHECK(results, TEST_AOBJ_POBJ_RESULT_VIEW_STABILITY, SAObjPObjProbe::Alive != 0 || SAObjPObjProbe::Constructed != SAObjPObjProbe::Destroyed);
	rtrn 0;
}

sttc ::llc::err_t testAObjPObjBoundaries(::llc::ATestError & results) {
	::aobjPobjReset();
	{
		::llc::apobj<SAObjPObjProbe> handles;
		for(::llc::u2_t i = 0; i < 3; ++i) {
			::llc::pobj<SAObjPObjProbe> owner;
			LLC_TEST_REQUIRE(results, TEST_AOBJ_POBJ_RESULT_BOUNDARIES, 0 == owner.create(::llc::u2_t(i + 1)));
			LLC_TEST_REQUIRE(results, TEST_AOBJ_POBJ_RESULT_BOUNDARIES, (::llc::s2_t)i != handles.push_back(owner));
		}
		LLC_TEST_REQUIRE(results, TEST_AOBJ_POBJ_RESULT_BOUNDARIES, 2 != handles.remove(0));
		LLC_TEST_CHECK(results, TEST_AOBJ_POBJ_RESULT_BOUNDARIES, SAObjPObjProbe::Alive != 2 || handles[0]->Value != 2 || handles[1]->Value != 3);
		LLC_TEST_REQUIRE(results, TEST_AOBJ_POBJ_RESULT_BOUNDARIES, 1 != handles.pop_back());
		LLC_TEST_CHECK(results, TEST_AOBJ_POBJ_RESULT_BOUNDARIES, SAObjPObjProbe::Alive != 1 || handles[0]->Value != 2);
	}
	LLC_TEST_CHECK(results, TEST_AOBJ_POBJ_RESULT_BOUNDARIES, SAObjPObjProbe::Alive != 0 || SAObjPObjProbe::Constructed != SAObjPObjProbe::Destroyed);
	rtrn 0;
}

::llc::err_t testAObjPObj(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testAObjPObjAliases(results));
	result = ::llc::min(result, ::testAObjPObjEmpty(results));
	result = ::llc::min(result, ::testAObjPObjOwnership(results));
	result = ::llc::min(result, ::testAObjPObjCopyMove(results));
	result = ::llc::min(result, ::testAObjPObjSelfAssignment(results));
	result = ::llc::min(result, ::testAObjPObjViewStability(results));
	result = ::llc::min(result, ::testAObjPObjBoundaries(results));
	rtrn result;
}
