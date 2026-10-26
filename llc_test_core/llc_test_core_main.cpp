#include "llc_test_core_suites.h"

sttc ::llc::err_t test_core_entry_point(::llc::SRuntimeValues & runtimeValues);
LLC_OS_ENTRY_POINT(::test_core_entry_point);

sttc ::llc::err_t test_core_entry_point(::llc::SRuntimeValues & runtimeValues) {
	::llc::vcst_t pngWorker;
	if(0 <= ::llc::argsOptionIndex(runtimeValues.EntryPointArgs, "png-worker")) {
		if_fail_fe(::llc::argsOptionValue(runtimeValues.EntryPointArgs, "png-worker", pngWorker));
		rtrn ::testPNGWorker(pngWorker);
	}
	rtrn ::llc::runSuites(runtimeValues
		, LLC_TEST_SUITE(testAdam7            , "adam7")
		, LLC_TEST_SUITE(testAES              , "aes")
		, LLC_TEST_SUITE(testAlign            , "align")
		, LLC_TEST_SUITE(testAObjPObj         , "aobj_pobj")
		, LLC_TEST_SUITE(testAObjPPod         , "aobj_ppod")
		, LLC_TEST_SUITE(testAPodColor        , "apod_color")
		, LLC_TEST_SUITE(testAPodGauge        , "apod_gauge")
		, LLC_TEST_SUITE(testAPodMinMax       , "apod_minmax")
		, LLC_TEST_SUITE(testView             , "view")
		, LLC_TEST_SUITE(testViewBit          , "view_bit")
		, LLC_TEST_SUITE(testArrayBase        , "array_base")
		, LLC_TEST_SUITE(testArrayStatic      , "array_static")
		, LLC_TEST_SUITE(testArrayPod         , "array_pod")
		, LLC_TEST_SUITE(testArrayObj         , "array_obj")
		, LLC_TEST_SUITE(testArrayPtr         , "array_ptr")
		, LLC_TEST_SUITE(testArrayCircular    , "array_circular")
		, LLC_TEST_SUITE(testGrid             , "grid")
		, LLC_TEST_SUITE(testGridCopy         , "grid_copy")
		, LLC_TEST_SUITE(testDeflate          , "deflate")
		, LLC_TEST_SUITE(testEncoding         , "encoding")
		, LLC_TEST_SUITE(testFile             , "file")
		, LLC_TEST_SUITE(testPNG              , "png")
		, LLC_TEST_SUITE(testBlockContainer   , "block_container")
		, LLC_TEST_SUITE(testBlockContainerNTS, "block_container_nts")
		, LLC_TEST_SUITE(testQueueAsync       , "queue_async")
		, LLC_TEST_SUITE(testString           , "string")
		, LLC_TEST_SUITE(testPtrNCO           , "ptr_nco")
		, LLC_TEST_SUITE(testPtrPOD           , "ptr_pod")
		, LLC_TEST_SUITE(testPtrObj           , "ptr_obj")
		);
}
