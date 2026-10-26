#include "llc_test_core_suites.h"
#include "llc_ptr_obj.h"

namespace {
	struct SPobjProbe {
		sttc ::llc::s2_t Alive;
		sttc ::llc::s2_t Constructed;
		sttc ::llc::s2_t Destroyed;
		::llc::u2_t Value = {};

		SPobjProbe() { ++Alive; ++Constructed; }
		SPobjProbe(::llc::u2_t value) : Value{value} { ++Alive; ++Constructed; }
		SPobjProbe(cnst SPobjProbe & other) : Value{other.Value} { ++Alive; ++Constructed; }
		~SPobjProbe() { --Alive; ++Destroyed; }
	};

	::llc::s2_t SPobjProbe::Alive = {};
	::llc::s2_t SPobjProbe::Constructed = {};
	::llc::s2_t SPobjProbe::Destroyed = {};

	sttc void pobjReset() {
		SPobjProbe::Alive = SPobjProbe::Constructed = SPobjProbe::Destroyed = {};
	}
}

GDEFINE_ENUM_TYPE(TEST_PTR_OBJ_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_PTR_OBJ_RESULT, CREATE  , 0, "An object pointer constructs its referenced object.");
GDEFINE_ENUM_VALUED(TEST_PTR_OBJ_RESULT, SHARE   , 1, "Copying and moving object pointers share one constructed object.");
GDEFINE_ENUM_VALUED(TEST_PTR_OBJ_RESULT, CLEAR   , 2, "Clearing a non-final object handle does not destroy the shared object.");
GDEFINE_ENUM_VALUED(TEST_PTR_OBJ_RESULT, REPLACE , 3, "Replacing sole-owned object storage destroys the previous object exactly once.");
GDEFINE_ENUM_VALUED(TEST_PTR_OBJ_RESULT, LAZY    , 4, "Dereferencing an empty object pointer default-constructs its object.");

sttc ::llc::err_t testPtrObjCreate(::llc::ATestError & results) {
	::pobjReset();
	{
		::llc::pobj<SPobjProbe> owner;
		LLC_TEST_REQUIRE(results, TEST_PTR_OBJ_RESULT_CREATE, 0 == owner.create(::llc::u2_t(11)));
		LLC_TEST_CHECK(results, TEST_PTR_OBJ_RESULT_CREATE, owner->Value != 11 || owner.get_ref()->References != 1 || SPobjProbe::Alive != 1 || SPobjProbe::Constructed != 1 || SPobjProbe::Destroyed != 0);
	}
	LLC_TEST_CHECK(results, TEST_PTR_OBJ_RESULT_CREATE, SPobjProbe::Alive != 0 || SPobjProbe::Constructed != 1 || SPobjProbe::Destroyed != 1);
	rtrn 0;
}

sttc ::llc::err_t testPtrObjShare(::llc::ATestError & results) {
	::pobjReset();
	{
		::llc::pobj<SPobjProbe> owner;
		LLC_TEST_REQUIRE(results, TEST_PTR_OBJ_RESULT_SHARE, 0 == owner.create(::llc::u2_t(11)));
		::llc::pobj<SPobjProbe> copied = owner;
		::llc::pobj<SPobjProbe> moved = (::llc::pobj<SPobjProbe>&&)copied;
		LLC_TEST_CHECK(results, TEST_PTR_OBJ_RESULT_SHARE, 0 != (SPobjProbe*)copied || moved != owner || owner.get_ref()->References != 2 || SPobjProbe::Alive != 1 || SPobjProbe::Constructed != 1);
	}
	LLC_TEST_CHECK(results, TEST_PTR_OBJ_RESULT_SHARE, SPobjProbe::Alive != 0 || SPobjProbe::Constructed != 1 || SPobjProbe::Destroyed != 1);
	rtrn 0;
}

sttc ::llc::err_t testPtrObjClear(::llc::ATestError & results) {
	::pobjReset();
	{
		::llc::pobj<SPobjProbe> owner;
		LLC_TEST_REQUIRE(results, TEST_PTR_OBJ_RESULT_CLEAR, 0 == owner.create(::llc::u2_t(11)));
		::llc::pobj<SPobjProbe> retained = owner;
		LLC_TEST_REQUIRE(results, TEST_PTR_OBJ_RESULT_CLEAR, 0 != owner.clear());
		LLC_TEST_CHECK(results, TEST_PTR_OBJ_RESULT_CLEAR, 0 != (SPobjProbe*)owner || retained->Value != 11 || retained.get_ref()->References != 1 || SPobjProbe::Alive != 1 || SPobjProbe::Destroyed != 0);
	}
	LLC_TEST_CHECK(results, TEST_PTR_OBJ_RESULT_CLEAR, SPobjProbe::Alive != 0 || SPobjProbe::Constructed != 1 || SPobjProbe::Destroyed != 1);
	rtrn 0;
}

sttc ::llc::err_t testPtrObjReplace(::llc::ATestError & results) {
	::pobjReset();
	{
		::llc::pobj<SPobjProbe> owner;
		LLC_TEST_REQUIRE(results, TEST_PTR_OBJ_RESULT_REPLACE, 0 == owner.create(::llc::u2_t(11)));
		LLC_TEST_REQUIRE(results, TEST_PTR_OBJ_RESULT_REPLACE, 0 == owner.create(::llc::u2_t(22)));
		LLC_TEST_CHECK(results, TEST_PTR_OBJ_RESULT_REPLACE, owner->Value != 22 || owner.get_ref()->References != 1 || SPobjProbe::Alive != 1 || SPobjProbe::Constructed != 2 || SPobjProbe::Destroyed != 1);
	}
	LLC_TEST_CHECK(results, TEST_PTR_OBJ_RESULT_REPLACE, SPobjProbe::Alive != 0 || SPobjProbe::Constructed != 2 || SPobjProbe::Destroyed != 2);
	rtrn 0;
}

sttc ::llc::err_t testPtrObjLazy(::llc::ATestError & results) {
	::pobjReset();
	{
		::llc::pobj<SPobjProbe> lazy;
		lazy->Value = 33;
		LLC_TEST_CHECK(results, TEST_PTR_OBJ_RESULT_LAZY, lazy->Value != 33 || lazy.get_ref()->References != 1 || SPobjProbe::Alive != 1 || SPobjProbe::Constructed != 1 || SPobjProbe::Destroyed != 0);
	}
	LLC_TEST_CHECK(results, TEST_PTR_OBJ_RESULT_LAZY, SPobjProbe::Alive != 0 || SPobjProbe::Constructed != 1 || SPobjProbe::Destroyed != 1);
	rtrn 0;
}

::llc::err_t testPtrObj(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testPtrObjCreate(results));
	result = ::llc::min(result, ::testPtrObjShare(results));
	result = ::llc::min(result, ::testPtrObjClear(results));
	result = ::llc::min(result, ::testPtrObjReplace(results));
	result = ::llc::min(result, ::testPtrObjLazy(results));
	rtrn result;
}
