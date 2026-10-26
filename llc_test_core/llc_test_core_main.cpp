#include "llc_test_core_suites.h"

sttc llc::err_t		test_core_entry_point	(::llc::SRuntimeValues & runtimeValues);
LLC_OS_ENTRY_POINT(::test_core_entry_point);

LLC_USING_TYPEINT();

sttc llc::err_t		test_core_entry_point	(::llc::SRuntimeValues & runtimeValues) {
	rtrn ::llc::runSuites(runtimeValues
		, LLC_TEST_SUITE(testAdam7CPP         , "adam7_cpp")
		, LLC_TEST_SUITE(testAdam7H           , "adam7_h")
		, LLC_TEST_SUITE(testAESCPP           , "aes_cpp")
		, LLC_TEST_SUITE(testAESH             , "aes_h")
		, LLC_TEST_SUITE(testAlignH           , "align_h")
		);
}
