// llc_test_core.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "llc_test_core_suites.h"
#include "llc_log.h"
#include "llc_parse.h"
#include "llc_timer.h"
#include "llc_runtime.h"

sttc ::llc::u3_t TestSeedOverride = {};
sttc bool TestSeedOverrideEnabled = false;

::llc::u3_t testSeed(::llc::u3_t fallback) { rtrn TestSeedOverrideEnabled ? TestSeedOverride : fallback; }

tplt<tpnm TFunction>
stct STestSuite {
	TFunction		& Function;
	::llc::vcst_t	Name;
};

tplt<tpnm TFunction>
sinx	STestSuite<TFunction>	testSuite			(TFunction & function, ::llc::vcst_t name) nxpt { rtrn {function, name}; }

tplt<tpnm TFunction>
sttc	::llc::err_t	testSuiteRun				(ATestError & results, ::llc::view<cnst ::llc::vcst_t> selectedSuites, cnst STestSuite<TFunction> & testSuite) {
	if(selectedSuites.size() && -1 == selectedSuites.find(testSuite.Name))
		rtrn 0;
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
sttc	::llc::err_t	testSuitesRun				(ATestError & results, ::llc::view<cnst ::llc::vcst_t> selectedSuites, cnst TSuites &... suites) {
	for(cnst ::llc::vcst_t & selectedSuite : selectedSuites) {
		cnst bool found = ((suites.Name == selectedSuite) || ...);
		if_zero_fef(found, "Unknown test suite:'%.*s'.", (int)selectedSuite.size(), selectedSuite.begin());
	}
	::llc::err_t	result				= 0;
	((result = ::llc::min(result, ::testSuiteRun(results, selectedSuites, suites))), ...);
	rtrn result;
}

#define LLC_TEST_SUITE(funcSuite) ::testSuite(funcSuite, LLC_CXS(#funcSuite))

sttc	::llc::err_t	test_core_entry_point		(::llc::SRuntimeValues & runtimeValues);
LLC_SYSTEM_OS_ENTRY_POINT(::test_core_entry_point);


sttc	::llc::err_t	test_core_entry_point		(::llc::SRuntimeValues & runtimeValues) {
	ATestError		results;
	llc::err_t		result				= 0;
	cnst bool		logSuccessDetails	= 0 <= ::llc::argsOptionIndex(runtimeValues.EntryPointArgs, "success-details");
	::llc::aobj<::llc::vcst_t> selectedSuites;
	if_fail_fe(::llc::argsOptionValues(runtimeValues.EntryPointArgs, LLC_CXS("test"), selectedSuites));
	if(0 <= ::llc::argsOptionIndex(runtimeValues.EntryPointArgs, LLC_CXS("seed"))) {
		::llc::vcst_t seedText = {};
		if_fail_fe(::llc::argsOptionValue(runtimeValues.EntryPointArgs, LLC_CXS("seed"), seedText));
		::llc::u3_t seed = {};
		cnst ::llc::err_t digits = ::llc::parseIntegerDecimal(seedText, seed);
		stxp ::llc::vcst_t seedMaximum = LLC_CXS("18446744073709551615");
		::llc::u2_t significantOffset = 0;
		while(significantOffset < seedText.size() && '0' == seedText[significantOffset])
			++significantOffset;
		cnst ::llc::u2_t significantLength = seedText.size() - significantOffset;
		bool overflow = significantLength > seedMaximum.size();
		for(::llc::u2_t iDigit = 0; false == overflow && significantLength == seedMaximum.size() && iDigit < significantLength; ++iDigit) {
			if(seedText[significantOffset + iDigit] == seedMaximum[iDigit])
				continue;
			overflow = seedText[significantOffset + iDigit] > seedMaximum[iDigit];
			break;
		}
		if_true_fef(0 == seedText.size() || ::llc::failed(digits) || (::llc::u2_t)digits != seedText.size() || overflow, "Invalid --seed value:'%.*s'.", (int)seedText.size(), seedText.begin());
		TestSeedOverride = seed;
		TestSeedOverrideEnabled = true;
		always_printf("Using test seed:%" LLC_FMT_U3 ".", TestSeedOverride);
	}

	::llc::STimer	testTimer;
	result = testSuitesRun(results, selectedSuites
		, LLC_TEST_SUITE(testSPRNG         )
		, LLC_TEST_SUITE(testSTL           )
		, LLC_TEST_SUITE(testAdam7        )
		, LLC_TEST_SUITE(testApp          )
		, LLC_TEST_SUITE(testASCIIColor   )
		, LLC_TEST_SUITE(testASCIITarget  )
		, LLC_TEST_SUITE(testArgs          )
		, LLC_TEST_SUITE(testBitField      )
		, LLC_TEST_SUITE(testBitmapTarget  )
		, LLC_TEST_SUITE(testBlock         )
		, LLC_TEST_SUITE(testBlockContainer)
		, LLC_TEST_SUITE(testCamera        )
		, LLC_TEST_SUITE(testCollision     )
		, LLC_TEST_SUITE(testCOMPlus       )
		, LLC_TEST_SUITE(testCPow          )
		, LLC_TEST_SUITE(testCTTIParser    )
		, LLC_TEST_SUITE(testEnum          )
		, LLC_TEST_SUITE(testExpression    )
		, LLC_TEST_SUITE(testGeometry      )
		, LLC_TEST_SUITE(testGridImage     )
		, LLC_TEST_SUITE(testGridOperations)
		, LLC_TEST_SUITE(testGUI           )
		, LLC_TEST_SUITE(testInput         )
		, LLC_TEST_SUITE(testStr           )
		, LLC_TEST_SUITE(testSystemKey     )
		, LLC_TEST_SUITE(testTypeMetadata  )
		, LLC_TEST_SUITE(testArrayStatic   )
		, LLC_TEST_SUITE(testArrayBit      )
		, LLC_TEST_SUITE(testArrayMinMax   )
		, LLC_TEST_SUITE(testArrayObj      )
		, LLC_TEST_SUITE(testArrayPod      )
		, LLC_TEST_SUITE(testBase64       )
		, LLC_TEST_SUITE(testJSONExpression)
		, LLC_TEST_SUITE(testJSONReader    )
		, LLC_TEST_SUITE(testKeyVal        )
		, LLC_TEST_SUITE(testLabel         )
		, LLC_TEST_SUITE(testLinearMap     )
		, LLC_TEST_SUITE(testXMLReader     )
		, LLC_TEST_SUITE(testMSBuildXML    )
		, LLC_TEST_SUITE(testPath          )
		, LLC_TEST_SUITE(testPointers      )
		, LLC_TEST_SUITE(testPNG           )
		, LLC_TEST_SUITE(testProcess       )
		, LLC_TEST_SUITE(testRuntimeModule )
		, LLC_TEST_SUITE(testView          )
		, LLC_TEST_SUITE(testViewAliases   )
		, LLC_TEST_SUITE(testViewBit       )
		, LLC_TEST_SUITE(testPackedUInt    )
		, LLC_TEST_SUITE(testViewSerialize )
		, LLC_TEST_SUITE(testViewStream    )
		, LLC_TEST_SUITE(testVOX           )
		, LLC_TEST_SUITE(testVoxel         )
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
			always_printf("PASS %.*s::%.*s(%" LLC_FMT_S3 ") x %u - validated against failure: %.*s"
				, (int)testResult.EnumName.size(), testResult.EnumName.begin(), (int)testResult.ValueName.size(), testResult.ValueName.begin()
				, testResult.Value, testResult.SuccessCount, (int)testResult.Description.size(), testResult.Description.begin()
				);
		if_true_ef(testResult.Count, "FAIL %.*s::%.*s(%" LLC_FMT_S3 ") x %u: %.*s"
			, (int)testResult.EnumName.size(), testResult.EnumName.begin(), (int)testResult.ValueName.size(), testResult.ValueName.begin()
			, testResult.Value, testResult.Count, (int)testResult.Description.size(), testResult.Description.begin()
			);
	}
	rtrn ::llc::failed(result) ? result : failureCount ? ::llc::OS_ERROR : 0;
}
