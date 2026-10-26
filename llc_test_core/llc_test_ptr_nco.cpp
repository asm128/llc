#include "llc_test_core_suites.h"
#include "llc_ptr_nco.h"

namespace {
	struct SPncoProbe {
		sttc ::llc::s2_t Alive;
		sttc ::llc::s2_t Destroyed;
		::llc::u2_t Value = {};

		SPncoProbe(::llc::u2_t value) : Value{value} { ++Alive; }
		~SPncoProbe() { --Alive; ++Destroyed; }
	};

	::llc::s2_t SPncoProbe::Alive = {};
	::llc::s2_t SPncoProbe::Destroyed = {};

	sttc void pncoReset() {
		SPncoProbe::Alive = SPncoProbe::Destroyed = {};
	}
}

GDEFINE_ENUM_TYPE(TEST_PTR_NCO_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_PTR_NCO_RESULT, ADOPT   , 0, "A non-constructing pointer adopts an existing reference.");
GDEFINE_ENUM_VALUED(TEST_PTR_NCO_RESULT, COPY    , 1, "Copying a non-constructing pointer shares and counts its reference.");
GDEFINE_ENUM_VALUED(TEST_PTR_NCO_RESULT, MOVE    , 2, "Moving a non-constructing pointer transfers its handle without changing ownership count.");
GDEFINE_ENUM_VALUED(TEST_PTR_NCO_RESULT, REPLACE , 3, "Replacing an adopted reference releases the previous object.");
GDEFINE_ENUM_VALUED(TEST_PTR_NCO_RESULT, CLEAR   , 4, "Clearing the final handle destroys the referenced object.");

sttc ::llc::err_t testPtrNCOAdopt(::llc::ATestError & results) {
	::pncoReset();
	{
		::llc::gref<SPncoProbe> * reference = {};
		LLC_TEST_REQUIRE(results, TEST_PTR_NCO_RESULT_ADOPT, 0 == ::llc::ref_create(&reference, ::llc::u2_t(7)));
		::llc::pnco<SPncoProbe> owner = reference;
		reference = {};
		LLC_TEST_CHECK(results, TEST_PTR_NCO_RESULT_ADOPT, 0 == (SPncoProbe*)owner || owner->Value != 7 || owner.get_ref()->References != 1 || SPncoProbe::Alive != 1);
	}
	LLC_TEST_CHECK(results, TEST_PTR_NCO_RESULT_ADOPT, SPncoProbe::Alive != 0 || SPncoProbe::Destroyed != 1);
	rtrn 0;
}

sttc ::llc::err_t testPtrNCOCopy(::llc::ATestError & results) {
	::pncoReset();
	{
		::llc::gref<SPncoProbe> * reference = {};
		LLC_TEST_REQUIRE(results, TEST_PTR_NCO_RESULT_COPY, 0 == ::llc::ref_create(&reference, ::llc::u2_t(7)));
		::llc::pnco<SPncoProbe> owner = reference;
		reference = {};
		{
			::llc::pnco<SPncoProbe> copied = owner;
			LLC_TEST_CHECK(results, TEST_PTR_NCO_RESULT_COPY, copied != owner || owner.get_ref()->References != 2 || SPncoProbe::Alive != 1);
		}
		LLC_TEST_CHECK(results, TEST_PTR_NCO_RESULT_COPY, owner.get_ref()->References != 1 || SPncoProbe::Alive != 1);
	}
	LLC_TEST_CHECK(results, TEST_PTR_NCO_RESULT_COPY, SPncoProbe::Alive != 0 || SPncoProbe::Destroyed != 1);
	rtrn 0;
}

sttc ::llc::err_t testPtrNCOMove(::llc::ATestError & results) {
	::pncoReset();
	{
		::llc::gref<SPncoProbe> * reference = {};
		LLC_TEST_REQUIRE(results, TEST_PTR_NCO_RESULT_MOVE, 0 == ::llc::ref_create(&reference, ::llc::u2_t(7)));
		::llc::pnco<SPncoProbe> source = reference;
		reference = {};
		::llc::pnco<SPncoProbe> moved = (::llc::pnco<SPncoProbe>&&)source;
		LLC_TEST_CHECK(results, TEST_PTR_NCO_RESULT_MOVE, 0 != (SPncoProbe*)source || moved->Value != 7 || moved.get_ref()->References != 1 || SPncoProbe::Alive != 1);
	}
	LLC_TEST_CHECK(results, TEST_PTR_NCO_RESULT_MOVE, SPncoProbe::Alive != 0 || SPncoProbe::Destroyed != 1);
	rtrn 0;
}

sttc ::llc::err_t testPtrNCOReplace(::llc::ATestError & results) {
	::pncoReset();
	{
		::llc::gref<SPncoProbe> * reference = {};
		LLC_TEST_REQUIRE(results, TEST_PTR_NCO_RESULT_REPLACE, 0 == ::llc::ref_create(&reference, ::llc::u2_t(7)));
		::llc::pnco<SPncoProbe> owner = reference;
		reference = {};
		::llc::gref<SPncoProbe> * replacement = {};
		LLC_TEST_REQUIRE(results, TEST_PTR_NCO_RESULT_REPLACE, 0 == ::llc::ref_create(&replacement, ::llc::u2_t(9)));
		cnst ::llc::gref<SPncoProbe> * replacementIdentity = replacement;
		LLC_TEST_CHECK(results, TEST_PTR_NCO_RESULT_REPLACE, owner.set_ref(replacement) != replacementIdentity);
		replacement = {};
		LLC_TEST_CHECK(results, TEST_PTR_NCO_RESULT_REPLACE, owner->Value != 9 || owner.get_ref()->References != 1 || SPncoProbe::Alive != 1 || SPncoProbe::Destroyed != 1);
	}
	LLC_TEST_CHECK(results, TEST_PTR_NCO_RESULT_REPLACE, SPncoProbe::Alive != 0 || SPncoProbe::Destroyed != 2);
	rtrn 0;
}

sttc ::llc::err_t testPtrNCOClear(::llc::ATestError & results) {
	::pncoReset();
	::llc::gref<SPncoProbe> * reference = {};
	LLC_TEST_REQUIRE(results, TEST_PTR_NCO_RESULT_CLEAR, 0 == ::llc::ref_create(&reference, ::llc::u2_t(7)));
	::llc::pnco<SPncoProbe> owner = reference;
	reference = {};
	LLC_TEST_REQUIRE(results, TEST_PTR_NCO_RESULT_CLEAR, 0 != owner.clear());
	LLC_TEST_CHECK(results, TEST_PTR_NCO_RESULT_CLEAR, 0 != (SPncoProbe*)owner || SPncoProbe::Alive != 0 || SPncoProbe::Destroyed != 1);
	rtrn 0;
}

::llc::err_t testPtrNCO(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testPtrNCOAdopt(results));
	result = ::llc::min(result, ::testPtrNCOCopy(results));
	result = ::llc::min(result, ::testPtrNCOMove(results));
	result = ::llc::min(result, ::testPtrNCOReplace(results));
	result = ::llc::min(result, ::testPtrNCOClear(results));
	rtrn result;
}
