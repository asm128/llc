#include "llc_test_core_suites.h"

#if __has_include("llc_app_impl.cpp")
#	include "llc_app_impl.cpp"
#	define LLC_TEST_HAS_APP_IMPL_CPP 1
#else
#	define LLC_TEST_HAS_APP_IMPL_CPP 0
#endif

#if LLC_TEST_HAS_APP_IMPL_CPP && defined(LLC_DEFINE_APPLICATION_ENTRY_POINT)
#	define LLC_TEST_HAS_APP_ENTRY_POINT 1
#else
#	define LLC_TEST_HAS_APP_ENTRY_POINT 0
#endif

#if LLC_TEST_HAS_APP_IMPL_CPP && defined(LLC_DEFINE_APPLICATION_ENTRY_POINT_MT)
#	define LLC_TEST_HAS_APP_ENTRY_POINT_MT 1
#else
#	define LLC_TEST_HAS_APP_ENTRY_POINT_MT 0
#endif

GDEFINE_ENUM_TYPE(TEST_APP_IMPL_CPP_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_APP_IMPL_CPP_RESULT, SOURCE_AVAILABLE, 0, "The canonical LLC application implementation source is available to the test target.");
GDEFINE_ENUM_VALUED(TEST_APP_IMPL_CPP_RESULT, ENTRY_POINT     , 1, "The application implementation source exposes the single-thread entry-point macro.");
GDEFINE_ENUM_VALUED(TEST_APP_IMPL_CPP_RESULT, ENTRY_POINT_MT  , 2, "The application implementation source exposes the multi-thread entry-point macro.");

sttc ::llc::err_t testAppImplCPPSourceAvailable(::llc::ATestError & results) {
	LLC_TEST_CHECK(results, TEST_APP_IMPL_CPP_RESULT_SOURCE_AVAILABLE, 0 == LLC_TEST_HAS_APP_IMPL_CPP);
	rtrn 0;
}

sttc ::llc::err_t testAppImplCPPEntryPoint(::llc::ATestError & results) {
	LLC_TEST_CHECK(results, TEST_APP_IMPL_CPP_RESULT_ENTRY_POINT, 0 == LLC_TEST_HAS_APP_ENTRY_POINT);
	rtrn 0;
}

sttc ::llc::err_t testAppImplCPPEntryPointMT(::llc::ATestError & results) {
	LLC_TEST_CHECK(results, TEST_APP_IMPL_CPP_RESULT_ENTRY_POINT_MT, 0 == LLC_TEST_HAS_APP_ENTRY_POINT_MT);
	rtrn 0;
}

::llc::err_t testAppImplCPP(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testAppImplCPPSourceAvailable(results));
	result = ::llc::min(result, ::testAppImplCPPEntryPoint(results));
	result = ::llc::min(result, ::testAppImplCPPEntryPointMT(results));
	rtrn result;
}
