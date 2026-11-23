#include "llc_test_engine_suites.h"

sttc ::llc::err_t test_engine_entry_point(::llc::SRuntimeValues & runtimeValues);
LLC_OS_ENTRY_POINT(::test_engine_entry_point);

sttc ::llc::err_t test_engine_entry_point(::llc::SRuntimeValues & runtimeValues) {
	rtrn ::llc::runSuites(runtimeValues
		, LLC_TEST_SUITE(d3d)
		, LLC_TEST_SUITE(engine_managers)
		, LLC_TEST_SUITE(engine_serialization)
		//, LLC_TEST_SUITE(ro_rsm)
		, LLC_TEST_SUITE(ragnarok_gat)
		, LLC_TEST_SUITE(ragnarok_gnd)
		);
}
