// llc_test_core.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "llc_test_core.h"
#include "llc_log.h"

#define LLC_TEST_SUITE_RUN(testSuiteFunction) {                                                         \
    llc::err_t tempResult;                                                                              \
    if_true_block_log(error_printf, llc::failed(tempResult = testSuiteFunction()), { result = -1; })    \
    else always_printf(#testSuiteFunction "() result OK."); }

int main()
{
    llc::err_t result = 0; 
    LLC_TEST_SUITE_RUN(testViewBit       );
    LLC_TEST_SUITE_RUN(testPackedUInt    );
    LLC_TEST_SUITE_RUN(testViewSerialize );
    return llc::failed(result) ? EXIT_FAILURE : EXIT_SUCCESS;
}
