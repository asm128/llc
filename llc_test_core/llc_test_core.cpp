// llc_test_core.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "llc_test_core.h"
#include "llc_log.h"
#include "llc_timer.h"
#include "llc_runtime.h"

using FTestSuite = ::llc::err_t(*)(ATestError &);

sttc	::llc::err_t	testSuiteRun				(ATestError & results, FTestSuite testSuite, ::llc::sc_c * suiteName) {
	::llc::u2_c		checkCountBefore	= testCheckCount(results);
	::llc::u2_c		failureCountBefore	= testErrorCount(results);
	::llc::STimer	suiteTimer;
	::llc::err_t	tempResult			= testSuite(results);
	suiteTimer.Frame();

	::llc::u2_c		checkCount			= testCheckCount(results) - checkCountBefore;
	::llc::u2_c		failureCount		= testErrorCount(results) - failureCountBefore;
	if_true_fef(::llc::failed(tempResult), "%s() failed with result:%i after %" LLC_FMT_U3 " us.", suiteName, tempResult, suiteTimer.LastTimeMicroseconds);
	if_true_fef(failureCount, "%s() completed in %" LLC_FMT_U3 " us: %u/%u checks passed, %u failed.", suiteName, suiteTimer.LastTimeMicroseconds, checkCount - failureCount, checkCount, failureCount);
	always_printf("%s() result OK in %" LLC_FMT_U3 " us: %u checks passed.", suiteName, suiteTimer.LastTimeMicroseconds, checkCount);
	rtrn 0;
}

#define LLC_TEST_SUITE_RUN(funcSuite) ::llc::min(result, ::testSuiteRun(results, funcSuite, #funcSuite))

sttc	::llc::err_t	test_core_entry_point		(::llc::SRuntimeValues & runtimeValues);
LLC_SYSTEM_OS_ENTRY_POINT(::test_core_entry_point);


sttc	::llc::err_t	test_core_entry_point		(::llc::SRuntimeValues & runtimeValues) {
	ATestError		results;
	llc::err_t		result				= 0;
	cnst bool		logSuccessDetails	= 0 <= ::llc::argsOptionIndex(runtimeValues.EntryPointArgs, "success-details");

	::llc::STimer	testTimer;
	result = LLC_TEST_SUITE_RUN(testSPRNG         );
	result = LLC_TEST_SUITE_RUN(testCPow          );
	result = LLC_TEST_SUITE_RUN(testStr           );
	result = LLC_TEST_SUITE_RUN(testArrayStatic   );
	result = LLC_TEST_SUITE_RUN(testView          );
	result = LLC_TEST_SUITE_RUN(testViewBit       );
	result = LLC_TEST_SUITE_RUN(testPackedUInt    );
	result = LLC_TEST_SUITE_RUN(testViewSerialize );
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
