#include "llc_test_core_suites.h"
#include "llc_array_obj.h"

namespace {
	struct STrackedObject {
		sttc ::llc::s2_t Alive;
		::llc::u2_t Value = {};

		STrackedObject() : Value{} { ++Alive; }
		STrackedObject(::llc::u2_t value) : Value{value} { ++Alive; }
		STrackedObject(cnst STrackedObject & other) : Value{other.Value} { ++Alive; }
		STrackedObject(STrackedObject && other) nxpt : Value{other.Value} { ++Alive; other.Value = {}; }
		~STrackedObject() { --Alive; }

		STrackedObject & operator=(cnst STrackedObject & other) { Value = other.Value; rtrn *this; }
		STrackedObject & operator=(STrackedObject && other) nxpt { Value = other.Value; other.Value = {}; rtrn *this; }
	};

	::llc::s2_t STrackedObject::Alive = {};
}

GDEFINE_ENUM_TYPE(TEST_ARRAY_OBJ_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_ARRAY_OBJ_RESULT, LIFETIME , 0, "Object arrays construct and destroy every owned element.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_OBJ_RESULT, INSERT   , 1, "Object-array insertion preserves element order.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_OBJ_RESULT, COPY     , 2, "Copying an object array creates independent elements.");
GDEFINE_ENUM_VALUED(TEST_ARRAY_OBJ_RESULT, REMOVE   , 3, "Object arrays support ordered and unordered removal.");

::llc::err_t testArrayObj(::llc::ATestError & results, ::llc::vcst_t) {
	LLC_TEST_REQUIRE(results, TEST_ARRAY_OBJ_RESULT_LIFETIME, 0 != STrackedObject::Alive);
	{
		::llc::aobj<STrackedObject> values;
		LLC_TEST_REQUIRE(results, TEST_ARRAY_OBJ_RESULT_LIFETIME, 0 != values.emplace_back(1U) || 1 != values.emplace_back(2U) || 2 != values.emplace_back(3U));
		LLC_TEST_CHECK(results, TEST_ARRAY_OBJ_RESULT_LIFETIME, values.size() != 3 || STrackedObject::Alive != 3);

		cnst STrackedObject inserted = {9};
		LLC_TEST_REQUIRE(results, TEST_ARRAY_OBJ_RESULT_INSERT, 4 != values.insert(1, inserted));
		LLC_TEST_CHECK(results, TEST_ARRAY_OBJ_RESULT_INSERT, values[0].Value != 1 || values[1].Value != 9 || values[2].Value != 2 || values[3].Value != 3);

		::llc::aobj<STrackedObject> copied = values;
		copied[0].Value = 44;
		LLC_TEST_CHECK(results, TEST_ARRAY_OBJ_RESULT_COPY, copied.size() != values.size() || copied[0].Value != 44 || values[0].Value != 1);

		LLC_TEST_REQUIRE(results, TEST_ARRAY_OBJ_RESULT_REMOVE, 3 != copied.remove(1));
		LLC_TEST_REQUIRE(results, TEST_ARRAY_OBJ_RESULT_REMOVE, 2 != copied.remove_unordered(0));
		LLC_TEST_CHECK(results, TEST_ARRAY_OBJ_RESULT_REMOVE, copied.size() != 2);
	}
	LLC_TEST_CHECK(results, TEST_ARRAY_OBJ_RESULT_LIFETIME, 0 != STrackedObject::Alive);
	rtrn 0;
}
