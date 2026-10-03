// llc_test_core.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "llc_test_core.h"
#include "llc_log.h"
#include "llc_timer.h"
#include "llc_runtime.h"

tplt<tpnm TFunction>
stct STestSuite {
	TFunction		& Function;
	::llc::vcsc_t	Name;
};

tplt<tpnm TFunction>
sinx	STestSuite<TFunction>	testSuite			(TFunction & function, ::llc::vcsc_t name) nxpt { rtrn {function, name}; }

tplt<tpnm TFunction>
sttc	::llc::err_t	testSuiteRun				(ATestError & results, cnst STestSuite<TFunction> & testSuite) {
	::llc::u2_c		checkCountBefore	= testCheckCount(results);
	::llc::u2_c		failureCountBefore	= testErrorCount(results);
	::llc::STimer	suiteTimer;
	::llc::err_t	tempResult			= testSuite.Function(results);
	suiteTimer.Frame();

	::llc::u2_c		checkCount			= testCheckCount(results) - checkCountBefore;
	::llc::u2_c		failureCount		= testErrorCount(results) - failureCountBefore;
	if_true_fef(::llc::failed(tempResult), "%.*s() failed with result:%i after %" LLC_FMT_U3 " us.", (int)testSuite.Name.size(), testSuite.Name.begin(), tempResult, suiteTimer.LastTimeMicroseconds);
	if_true_fef(failureCount, "%.*s() completed in %" LLC_FMT_U3 " us: %u/%u checks passed, %u failed.", (int)testSuite.Name.size(), testSuite.Name.begin(), suiteTimer.LastTimeMicroseconds, checkCount - failureCount, checkCount, failureCount);
	always_printf("%.*s() result OK in %" LLC_FMT_U3 " us: %u checks passed.", (int)testSuite.Name.size(), testSuite.Name.begin(), suiteTimer.LastTimeMicroseconds, checkCount);
	rtrn 0;
}

tplt<tpnm... TSuites>
sttc	::llc::err_t	testSuitesRun				(ATestError & results, cnst TSuites &... suites) {
	::llc::err_t	result				= 0;
	((result = ::llc::min(result, ::testSuiteRun(results, suites))), ...);
	rtrn result;
}

#define LLC_TEST_SUITE(funcSuite) ::testSuite(funcSuite, LLC_CXS(#funcSuite))

sttc	::llc::err_t	test_core_entry_point		(::llc::SRuntimeValues & runtimeValues);
LLC_SYSTEM_OS_ENTRY_POINT(::test_core_entry_point);


sttc	::llc::err_t	test_core_entry_point		(::llc::SRuntimeValues & runtimeValues) {
	ATestError		results;
	llc::err_t		result				= 0;
	cnst bool		logSuccessDetails	= 0 <= ::llc::argsOptionIndex(runtimeValues.EntryPointArgs, "success-details");

	::llc::STimer	testTimer;
	result = testSuitesRun(results
		, LLC_TEST_SUITE(testSPRNG         )
		, LLC_TEST_SUITE(testBitField      )
		, LLC_TEST_SUITE(testCPow          )
		, LLC_TEST_SUITE(testCTTIParser    )
		, LLC_TEST_SUITE(testStr           )
		, LLC_TEST_SUITE(testArrayStatic   )
		, LLC_TEST_SUITE(testArrayPod      )
		, LLC_TEST_SUITE(testBase64       )
		, LLC_TEST_SUITE(testJSONReader    )
		, LLC_TEST_SUITE(testPath          )
		, LLC_TEST_SUITE(testView          )
		, LLC_TEST_SUITE(testViewBit       )
		, LLC_TEST_SUITE(testPackedUInt    )
		, LLC_TEST_SUITE(testViewSerialize )
		);
	testTimer.Frame();

	::llc::u2_c		successCount		= testSuccessCount(results);
	::llc::u2_c		failureCount		= testErrorCount(results);

	if_true_ef(failureCount, "Test run completed in %" LLC_FMT_U3 " us: %u/%u checks passed, %u failed across %u result groups.", testTimer.LastTimeMicroseconds, successCount, successCount + failureCount, failureCount, results.size())
	else { 
		always_printf("Test run completed in %" LLC_FMT_U3 " us: all %u checks passed across %u result groups.", testTimer.LastTimeMicroseconds, successCount, results.size());
	}
	for(::llc::u2_t iResult = 0; iResult < results.size(); ++iResult) {
		cnst STestError & testResult = results[iResult];
		if(logSuccessDetails && testResult.SuccessCount)
			always_printf("PASS %s::%s(%" LLC_FMT_S3 ") x %u - validated against failure: %s", testResult.EnumName, testResult.ValueName, testResult.Value, testResult.SuccessCount, testResult.Description);
		if_true_ef(testResult.Count, "FAIL %s::%s(%" LLC_FMT_S3 ") x %u: %s", testResult.EnumName, testResult.ValueName, testResult.Value, testResult.Count, testResult.Description);
	}
	return llc::failed(result) || failureCount ? EXIT_FAILURE : EXIT_SUCCESS;
}
