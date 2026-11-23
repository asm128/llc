#include "test_llc_suites.h"

sttc ::llc::err_t test_llc_entry_point(::llc::SRuntimeValues & runtimeValues);
LLC_OS_ENTRY_POINT(::test_llc_entry_point);

#if defined(LLC_WINDOWS) || defined(LLC_LINUX)
extern ::llc::err_t testProcessEchoChild();
#endif
#ifdef LLC_LINUX
extern ::llc::err_t testProcessMarkerChild();
extern ::llc::err_t testProcessSignalChild();
#endif


sttc ::llc::err_t test_llc_entry_point(::llc::SRuntimeValues & runtimeValues) {
	STestRunnerState runnerState	= {};
	rtrn ::llc::runSuites(runnerState, runtimeValues
		, LLC_TEST_SUITE(adam7)
		, LLC_TEST_SUITE(args)
		, LLC_TEST_SUITE(array_obj)
		, LLC_TEST_SUITE(array_pod)
		, LLC_TEST_SUITE(base64)
		, LLC_TEST_SUITE(ctti_enum)
		, LLC_TEST_SUITE(ctti_enum_json)
		, LLC_TEST_SUITE(ctti_stct)
		, LLC_TEST_SUITE(ctti_stct_json)
		, LLC_TEST_SUITE(deflate)
		, LLC_TEST_SUITE(enum)
		, LLC_TEST_SUITE(event)
		, LLC_TEST_SUITE(gui_control_serialization)
		, LLC_TEST_SUITE(expression_core)
		, LLC_TEST_SUITE(expression_ctti)
		, LLC_TEST_SUITE(expression)
		, LLC_TEST_SUITE(gui_json)
		, LLC_TEST_SUITE(index)
		, LLC_TEST_SUITE(index_catalog)
		, LLC_TEST_SUITE(index_path)
		, LLC_TEST_SUITE(json_core)
		, LLC_TEST_SUITE(json_ctti)
		, LLC_TEST_SUITE(json)
		, LLC_TEST_SUITE(json_expression_ctti)
		, LLC_TEST_SUITE(json_expression)
		, LLC_TEST_SUITE(log)
		, LLC_TEST_SUITE(png)
		, LLC_TEST_SUITE(array_static)
		, LLC_TEST_SUITE(string)
		, LLC_TEST_SUITE(view_serialize)
		);
}
