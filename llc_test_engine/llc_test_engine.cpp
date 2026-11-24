#include "llc_test_engine_suites.h"

sttc ::llc::err_t test_engine_entry_point(::llc::SRuntimeValues & runtimeValues);
LLC_OS_ENTRY_POINT(::test_engine_entry_point);

sttc ::llc::err_t test_engine_entry_point(::llc::SRuntimeValues & runtimeValues) {
	rtrn ::llc::runSuites(runtimeValues
		, ::llc::testSuite(testD3D, "d3d", "d3d")
		, ::llc::testSuite(testEngineManagers, "engine_managers", "engine_managers")
		, ::llc::testSuite(testEngineSerialization, "engine_serialization", "engine_serialization")
		//, LLC_TEST_SUITE(testRORSM, "ro_rsm")
		, ::llc::testSuite(testRagnarokGAT, "ragnarok_gat", "ragnarok_gat")
		, ::llc::testSuite(testRagnarokGND, "ragnarok_gnd", "ragnarok_gnd")
		);
}
