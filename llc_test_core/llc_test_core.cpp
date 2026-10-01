// llc_test_core.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "llc_test_core.h"
#include "llc_log.h"
#include "llc_timer.h"
#include "llc_runtime.h"

#define LLC_TEST_SUITE_RUN(testSuiteFunction) {							\
	::llc::u2_c		checkCountBefore	= testCheckCount(results);		\
	::llc::u2_c		failureCountBefore	= testErrorCount(results);		\
	::llc::STimer	suiteTimer;											\
	::llc::err_t	tempResult			= testSuiteFunction(results);	\
	suiteTimer.Frame();													\
	if_true_block_logf(error_printf, ::llc::failed(tempResult), { result = -1; }, #testSuiteFunction "() failed with result:%i after %" LLC_FMT_U3 " us.", tempResult, suiteTimer.LastTimeMicroseconds) \
	else if(failureCountBefore != testErrorCount(results)) {			\
		result	= -1;													\
		error_printf(#testSuiteFunction "() completed in %" LLC_FMT_U3 " us: %u/%u checks passed, %u failed.", suiteTimer.LastTimeMicroseconds, (testCheckCount(results) - checkCountBefore) - (testErrorCount(results) - failureCountBefore), testCheckCount(results) - checkCountBefore, testErrorCount(results) - failureCountBefore); \
	}																	\
	else always_printf(#testSuiteFunction "() result OK in %" LLC_FMT_U3 " us: %u checks passed.", suiteTimer.LastTimeMicroseconds, testCheckCount(results) - checkCountBefore); \
}

sttc	::llc::err_t	test_core_entry_point		(::llc::SRuntimeValues & runtimeValues);
LLC_SYSTEM_OS_ENTRY_POINT(::test_core_entry_point);


sttc	::llc::err_t	test_core_entry_point		(::llc::SRuntimeValues & runtimeValues) {
	ATestError		results;
	llc::err_t		result				= 0;
	cnst bool		logSuccessDetails	= 0 <= ::llc::argsOptionIndex(runtimeValues.EntryPointArgs, "success-details");

	::llc::STimer	testTimer;
	LLC_TEST_SUITE_RUN(testSPRNG         );
	LLC_TEST_SUITE_RUN(testView          );
	LLC_TEST_SUITE_RUN(testViewBit       );
	LLC_TEST_SUITE_RUN(testPackedUInt    );
	LLC_TEST_SUITE_RUN(testViewSerialize );
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
