#include "llc_test_core_suites.h"
#include "llc_array_obj.h"

namespace {
	struct STrackedObject {
		sttc ::llc::s2_t Alive;
		sttc ::llc::s2_t Constructed;
		sttc ::llc::s2_t Destroyed;
		::llc::u2_t Value = {};

		STrackedObject() : Value{} { ++Alive; ++Constructed; }
		STrackedObject(::llc::u2_t value) : Value{value} { ++Alive; ++Constructed; }
		STrackedObject(cnst STrackedObject & other) : Value{other.Value} { ++Alive; ++Constructed; }
		STrackedObject(STrackedObject && other) nxpt : Value{other.Value} { ++Alive; ++Constructed; other.Value = {}; }
		~STrackedObject() { --Alive; ++Destroyed; }

		STrackedObject & operator=(cnst STrackedObject & other) { Value = other.Value; rtrn *this; }
		STrackedObject & operator=(STrackedObject && other) nxpt { Value = other.Value; other.Value = {}; rtrn *this; }
	};

	::llc::s2_t STrackedObject::Alive = {};
	::llc::s2_t STrackedObject::Constructed = {};
	::llc::s2_t STrackedObject::Destroyed = {};

	sttc void trackedReset() {
		STrackedObject::Alive = STrackedObject::Constructed = STrackedObject::Destroyed = {};
	}
}

GDEFINE_ENUM_TYPE(TEST_ARRAY_OBJ_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_ARRAY_OBJ_RESULT, LIFETIME         , 0, "Object arrays construct and destroy every owned element.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_OBJ_RESULT, INSERT           , 1, "Object-array insertion preserves element order and ownership.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_OBJ_RESULT, COPY             , 2, "Copying an object array creates independent elements.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_OBJ_RESULT, REMOVE           , 3, "Ordered object removal preserves the remaining order and ownership.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_OBJ_RESULT, REMOVE_UNORDERED , 4, "Unordered object removal fills the removed position and releases one element.");

sttc ::llc::err_t testArrayObjLifetime(::llc::ATestError & results) {
	::trackedReset();
	{
		::llc::aobj<STrackedObject> values;
		LLC_TEST_REQUIRE(results, TEST_ARRAY_OBJ_RESULT_LIFETIME, 0 != values.emplace_back(::llc::u2_t(1)));
		LLC_TEST_REQUIRE(results, TEST_ARRAY_OBJ_RESULT_LIFETIME, 1 != values.emplace_back(::llc::u2_t(2)));
		LLC_TEST_REQUIRE(results, TEST_ARRAY_OBJ_RESULT_LIFETIME, 2 != values.emplace_back(::llc::u2_t(3)));
		LLC_TEST_CHECK(results, TEST_ARRAY_OBJ_RESULT_LIFETIME, values.size() != 3 || STrackedObject::Alive != 3);
	}
	LLC_TEST_CHECKF(results, TEST_ARRAY_OBJ_RESULT_LIFETIME, 0 != STrackedObject::Alive || STrackedObject::Constructed != STrackedObject::Destroyed, "After array destruction: alive:%i, constructed:%i, destroyed:%i.", STrackedObject::Alive, STrackedObject::Constructed, STrackedObject::Destroyed);
	rtrn 0;
}

sttc ::llc::err_t testArrayObjInsert(::llc::ATestError & results) {
	::trackedReset();
	{
		::llc::aobj<STrackedObject> values;
		LLC_TEST_REQUIRE(results, TEST_ARRAY_OBJ_RESULT_INSERT, 0 != values.emplace_back(::llc::u2_t(1)));
		LLC_TEST_REQUIRE(results, TEST_ARRAY_OBJ_RESULT_INSERT, 1 != values.emplace_back(::llc::u2_t(2)));
		LLC_TEST_REQUIRE(results, TEST_ARRAY_OBJ_RESULT_INSERT, 2 != values.emplace_back(::llc::u2_t(3)));
		cnst STrackedObject inserted = {9};
		LLC_TEST_REQUIRE(results, TEST_ARRAY_OBJ_RESULT_INSERT, 4 != values.insert(1, inserted));
		LLC_TEST_CHECK(results, TEST_ARRAY_OBJ_RESULT_INSERT, values.size() != 4 || values[0].Value != 1 || values[1].Value != 9 || values[2].Value != 2 || values[3].Value != 3);
	}
	LLC_TEST_CHECKF(results, TEST_ARRAY_OBJ_RESULT_INSERT, 0 != STrackedObject::Alive || STrackedObject::Constructed != STrackedObject::Destroyed, "After insert fixture destruction: alive:%i, constructed:%i, destroyed:%i.", STrackedObject::Alive, STrackedObject::Constructed, STrackedObject::Destroyed);
	rtrn 0;
}

sttc ::llc::err_t testArrayObjCopy(::llc::ATestError & results) {
	::trackedReset();
	{
		::llc::aobj<STrackedObject> source;
		LLC_TEST_REQUIRE(results, TEST_ARRAY_OBJ_RESULT_COPY, 0 != source.emplace_back(::llc::u2_t(1)));
		LLC_TEST_REQUIRE(results, TEST_ARRAY_OBJ_RESULT_COPY, 1 != source.emplace_back(::llc::u2_t(2)));
		::llc::aobj<STrackedObject> copied = source;
		copied[0].Value = 44;
		LLC_TEST_CHECK(results, TEST_ARRAY_OBJ_RESULT_COPY, copied.size() != source.size() || copied[0].Value != 44 || source[0].Value != 1);
	}
	LLC_TEST_CHECKF(results, TEST_ARRAY_OBJ_RESULT_COPY, 0 != STrackedObject::Alive || STrackedObject::Constructed != STrackedObject::Destroyed, "After copy fixture destruction: alive:%i, constructed:%i, destroyed:%i.", STrackedObject::Alive, STrackedObject::Constructed, STrackedObject::Destroyed);
	rtrn 0;
}

sttc ::llc::err_t testArrayObjRemove(::llc::ATestError & results) {
	::trackedReset();
	{
		::llc::aobj<STrackedObject> values;
		LLC_TEST_REQUIRE(results, TEST_ARRAY_OBJ_RESULT_REMOVE, 0 != values.emplace_back(::llc::u2_t(1)));
		LLC_TEST_REQUIRE(results, TEST_ARRAY_OBJ_RESULT_REMOVE, 1 != values.emplace_back(::llc::u2_t(2)));
		LLC_TEST_REQUIRE(results, TEST_ARRAY_OBJ_RESULT_REMOVE, 2 != values.emplace_back(::llc::u2_t(3)));
		LLC_TEST_REQUIRE(results, TEST_ARRAY_OBJ_RESULT_REMOVE, 3 != values.emplace_back(::llc::u2_t(4)));
		LLC_TEST_REQUIRE(results, TEST_ARRAY_OBJ_RESULT_REMOVE, 3 != values.remove(1));
		LLC_TEST_CHECK(results, TEST_ARRAY_OBJ_RESULT_REMOVE, values.size() != 3 || values[0].Value != 1 || values[1].Value != 3 || values[2].Value != 4 || STrackedObject::Alive != 3);
	}
	LLC_TEST_CHECKF(results, TEST_ARRAY_OBJ_RESULT_REMOVE, 0 != STrackedObject::Alive || STrackedObject::Constructed != STrackedObject::Destroyed, "After ordered-remove fixture destruction: alive:%i, constructed:%i, destroyed:%i.", STrackedObject::Alive, STrackedObject::Constructed, STrackedObject::Destroyed);
	rtrn 0;
}

sttc ::llc::err_t testArrayObjRemoveUnordered(::llc::ATestError & results) {
	::trackedReset();
	{
		::llc::aobj<STrackedObject> values;
		LLC_TEST_REQUIRE(results, TEST_ARRAY_OBJ_RESULT_REMOVE_UNORDERED, 0 != values.emplace_back(::llc::u2_t(1)));
		LLC_TEST_REQUIRE(results, TEST_ARRAY_OBJ_RESULT_REMOVE_UNORDERED, 1 != values.emplace_back(::llc::u2_t(2)));
		LLC_TEST_REQUIRE(results, TEST_ARRAY_OBJ_RESULT_REMOVE_UNORDERED, 2 != values.emplace_back(::llc::u2_t(3)));
		LLC_TEST_REQUIRE(results, TEST_ARRAY_OBJ_RESULT_REMOVE_UNORDERED, 3 != values.emplace_back(::llc::u2_t(4)));
		LLC_TEST_REQUIRE(results, TEST_ARRAY_OBJ_RESULT_REMOVE_UNORDERED, 3 != values.remove_unordered(1));
		LLC_TEST_CHECK(results, TEST_ARRAY_OBJ_RESULT_REMOVE_UNORDERED, values.size() != 3 || values[0].Value != 1 || values[1].Value != 4 || values[2].Value != 3 || STrackedObject::Alive != 3);
	}
	LLC_TEST_CHECKF(results, TEST_ARRAY_OBJ_RESULT_REMOVE_UNORDERED, 0 != STrackedObject::Alive || STrackedObject::Constructed != STrackedObject::Destroyed, "After unordered-remove fixture destruction: alive:%i, constructed:%i, destroyed:%i.", STrackedObject::Alive, STrackedObject::Constructed, STrackedObject::Destroyed);
	rtrn 0;
}

::llc::err_t testArrayObj(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testArrayObjLifetime(results));
	result = ::llc::min(result, ::testArrayObjInsert(results));
	result = ::llc::min(result, ::testArrayObjCopy(results));
	result = ::llc::min(result, ::testArrayObjRemove(results));
	result = ::llc::min(result, ::testArrayObjRemoveUnordered(results));
	rtrn result;
}
