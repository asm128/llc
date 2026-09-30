// llc_test_core.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "llc_test_core.h"
#include "llc_log.h"

#define LLC_TEST_SUITE_RUN(testSuiteFunction) {                                                                                         \
	::llc::u2_c failureCountBefore = testErrorCount(errors);                                                                             \
	::llc::err_t tempResult;                                                                                                             \
	if_true_block_log(error_printf, ::llc::failed(tempResult = testSuiteFunction(errors)), { result = -1; })                             \
	else if(failureCountBefore != testErrorCount(errors)) { result = -1; error_printf(#testSuiteFunction "() completed with %u failures.", testErrorCount(errors) - failureCountBefore); } \
	else always_printf(#testSuiteFunction "() result OK."); }

int main()
{
	ATestError errors;
	llc::err_t result = 0;
	LLC_TEST_SUITE_RUN(testSPRNG         );
	LLC_TEST_SUITE_RUN(testViewBit       );
	LLC_TEST_SUITE_RUN(testPackedUInt    );
	LLC_TEST_SUITE_RUN(testViewSerialize );
	if(errors.size()) {
		error_printf("Test run completed with %u failures in %u result groups.", testErrorCount(errors), errors.size());
		for(::llc::u2_t iError = 0; iError < errors.size(); ++iError) {
			cnst STestError & error = errors[iError];
			error_printf("%s::%s(%" LLC_FMT_S3 ") x %u: %s", error.EnumName, error.ValueName, error.Value, error.Count, error.Description);
		}
	}
	return llc::failed(result) || errors.size() ? EXIT_FAILURE : EXIT_SUCCESS;
}
