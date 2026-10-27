#include "llc_test_core_suites.h"

 /// QUE CARAJO LA PUTA QUE TE PARIO #include <cstring>

LLC_USING_TYPEINT();
LLC_USING_VIEW();
LLC_USING_APOD();


	GDEFINE_ENUM_TYPE(TEST_CONNECTION, u0_t);
	
llc::err_t test_connection(::llc::ATestError & results, vcst_t) {


	if_fail_fe(::llc::tcpipInitialize());

	if_fail_fe(::llc::tcpipShutdown());

	return 0;
}

sttc ::llc::err_t	test_core_entry_point	(::llc::SRuntimeValues & runtimeValues);
LLC_OS_ENTRY_POINT(::test_core_entry_point);

sttc ::llc::err_t	test_core_entry_point	(::llc::SRuntimeValues & runtimeValues) {
	rtrn ::llc::runSuites(runtimeValues
		, LLC_TEST_SUITE(connection)
		, LLC_TEST_SUITE(connection_ip)
		, LLC_TEST_SUITE(udp_server)
		, LLC_TEST_SUITE(tcp_server)
		);
}
