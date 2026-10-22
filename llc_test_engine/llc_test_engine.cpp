#include "llc_test_engine_suites.h"

sttc ::llc::err_t test_engine_entry_point(::llc::SRuntimeValues & runtimeValues);
LLC_OS_ENTRY_POINT(::test_engine_entry_point);

sttc ::llc::err_t test_engine_entry_point(::llc::SRuntimeValues & runtimeValues) {
	rtrn ::llc::runSuites(runtimeValues
		, LLC_TEST_SUITE(testD3D, "d3d")
		, LLC_TEST_SUITE(testEngineManagers, "engine_managers")
		, LLC_TEST_SUITE(testEngineSerialization, "engine_serialization")
		//, LLC_TEST_SUITE(testRORSM, "ro_rsm")
		, LLC_TEST_SUITE(testRagnarokGAT, "ragnarok_gat")
		, LLC_TEST_SUITE(testRagnarokGND, "ragnarok_gnd")
		);
}
