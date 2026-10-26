#include "llc_test_core_suites.h"

sttc ::llc::err_t test_core_entry_point(::llc::SRuntimeValues & runtimeValues);
LLC_OS_ENTRY_POINT(::test_core_entry_point);

sttc ::llc::err_t test_core_entry_point(::llc::SRuntimeValues & runtimeValues) {
	rtrn ::llc::runSuites(runtimeValues
		, LLC_TEST_SUITE(testView             , "view")
		, LLC_TEST_SUITE(testViewBit          , "view_bit")
		, LLC_TEST_SUITE(testArrayBase        , "array_base")
		, LLC_TEST_SUITE(testArrayStatic      , "array_static")
		, LLC_TEST_SUITE(testArrayPod         , "array_pod")
		, LLC_TEST_SUITE(testArrayObj         , "array_obj")
		, LLC_TEST_SUITE(testArrayPtr         , "array_ptr")
		, LLC_TEST_SUITE(testArrayCircular    , "array_circular")
		, LLC_TEST_SUITE(testGrid             , "grid")
		, LLC_TEST_SUITE(testBlockContainer   , "block_container")
		, LLC_TEST_SUITE(testBlockContainerNTS, "block_container_nts")
		, LLC_TEST_SUITE(testQueueAsync       , "queue_async")
		, LLC_TEST_SUITE(testString           , "string")
		);
}
