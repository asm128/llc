#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(TEST_SMOKE_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_SMOKE_RESULT, RUNNER, 0, "The empty core-test runner executes one smoke check.");

sttc ::llc::err_t testSmoke(::llc::ATestError & results, ::llc::vcst_t) {
	LLC_TEST_CHECK(results, TEST_SMOKE_RESULT_RUNNER, false);
	rtrn 0;
}

sttc ::llc::err_t test_core_entry_point(::llc::SRuntimeValues & runtimeValues);
LLC_OS_ENTRY_POINT(::test_core_entry_point);

sttc ::llc::err_t test_core_entry_point(::llc::SRuntimeValues & runtimeValues) {
	rtrn ::llc::runSuites(runtimeValues, LLC_TEST_SUITE(testSmoke, "smoke"));
}
