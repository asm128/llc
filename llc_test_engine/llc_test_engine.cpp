// llc_test_core.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "llc_test_engine_suites.h"
#include "llc_log.h"
#include "llc_parse.h"
#include "llc_timer.h"
#include "llc_runtime.h"
#include "llc_test_report.h"

sttc ::llc::u3_t TestSeedOverride = {};
sttc bool TestSeedOverrideEnabled = false;
::llc::string TestRSMPath = {};

::llc::u3_t testSeed(::llc::u3_t fallback) { rtrn TestSeedOverrideEnabled ? TestSeedOverride : fallback; }

tplt<tpnm TFunction>
stct STestSuite {
	TFunction		& Function;
	::llc::vcst_t	Name, Folder;
};

tplt<tpnm TFunction>
sinx	STestSuite<TFunction>	testSuite			(TFunction & function, ::llc::vcst_t name, ::llc::vcst_t folder) nxpt { rtrn {function, name, folder}; }

tplt<tpnm TFunction>
sttc ::llc::err_t testSuiteRun(ATestError & results, STestRunReport & run, ::llc::view<cnst ::llc::vcst_t> selectedSuites, cnst STestSuite<TFunction> & suite) {
	if(selectedSuites.size() && -1 == selectedSuites.find(suite.Name))
		rtrn 0;
	STestSuiteReport report = {suite.Name, suite.Folder};
	::llc::string folder;
	if_fail_fe(::llc::pathNameCompose(run.Folder, suite.Folder, folder));
	if(::llc::failed(report.Status = ::llc::pathCreate(folder))) {
		report.Status = report.ReportStatus = -1;
		if_fail_fe(run.Suites.push_back(report));
		error_printf("Cannot create suite folder '%.*s'.", LLC_VIEW_ARGS(folder));
		rtrn -1;
	}
	ATestError suiteResults;
	STestLogBuffer log;
	{
		STestLogCapture capture(log);
		::llc::STimer timer;
		try { report.Status = suite.Function(suiteResults, folder); }
		catch(...) { report.Status = -1; error_printf("%s", "Unhandled suite exception."); }
		timer.Frame();
		report.Microseconds = timer.LastTimeMicroseconds;
		report.Checks = testCheckCount(suiteResults);
		report.Failures = testErrorCount(suiteResults);
		report.Groups = suiteResults.size();
		for(cnst auto & item : suiteResults) {
			if(run.SuccessDetails && item.SuccessCount)
				always_printf("PASS %.*s::%.*s(%" LLC_FMT_S3 ") x %u: %.*s", LLC_VIEW_ARGS(item.EnumName), LLC_VIEW_ARGS(item.ValueName), item.Value, item.SuccessCount, LLC_VIEW_ARGS(item.Description));
			if(item.Count)
				error_printf("FAIL %.*s::%.*s(%" LLC_FMT_S3 ") x %u: %.*s", LLC_VIEW_ARGS(item.EnumName), LLC_VIEW_ARGS(item.ValueName), item.Value, item.Count, LLC_VIEW_ARGS(item.Description));
		}
		if(::llc::failed(report.Status) || report.Failures)
			error_printf("%.*s() failed: result:%i, %u/%u checks passed.", LLC_VIEW_ARGS(suite.Name), report.Status, report.Checks - report.Failures, report.Checks);
		else
			always_printf("%.*s() result OK in %" LLC_FMT_U3 " us: %u checks passed.", LLC_VIEW_ARGS(suite.Name), report.Microseconds, report.Checks);
	}
	if(log.Failed) report.ReportStatus = -1;
	try { report.ReportStatus = ::llc::min(report.ReportStatus, testSuiteReportWrite(folder, report, suiteResults, log)); }
	catch(...) { report.ReportStatus = -1; error_printf("Could not serialize suite '%.*s'.", LLC_VIEW_ARGS(suite.Name)); }
	if_fail_fe(run.Suites.push_back(report));
	for(cnst auto & item : suiteResults)
		if_fail_fe(results.push_back(item));
	always_printf("Suite %.*s: %u checks, %u failures, status:%i, reporting:%i.", LLC_VIEW_ARGS(suite.Name), report.Checks, report.Failures, report.Status, report.ReportStatus);
	rtrn (::llc::failed(report.Status) || ::llc::failed(report.ReportStatus) || report.Failures) ? -1 : 0;
}

tplt<tpnm... TSuites>
sttc	::llc::err_t	testSuitesRun				(ATestError & results, STestRunReport & run, ::llc::view<cnst ::llc::vcst_t> selectedSuites, cnst TSuites &... suites) {
	for(cnst ::llc::vcst_t & selectedSuite : selectedSuites) {
		cnst bool found = ((suites.Name == selectedSuite) || ...);
		if_zero_fef(found, "Unknown test suite:'%.*s'.", (int)selectedSuite.size(), selectedSuite.begin());
	}
	::llc::err_t	result				= 0;
	((result = ::llc::min(result, ::testSuiteRun(results, run, selectedSuites, suites))), ...);
	rtrn result;
}

#define LLC_TEST_SUITE(funcSuite, folder) ::testSuite(funcSuite, #funcSuite, folder)

sttc	::llc::err_t	test_engine_entry_point	(::llc::SRuntimeValues & runtimeValues);
LLC_OS_ENTRY_POINT(::test_engine_entry_point);


sttc ::llc::err_t testEngineRun(::llc::SRuntimeValues & runtimeValues, STestRunReport & run) {
	//_CrtSetBreakAlloc(12390);
	ATestError		results;
	llc::err_t		result				= 0;
	run.SuccessDetails	= 0 <= ::llc::argsOptionIndex(runtimeValues.EntryPointArgs, "success-details");
	if(0 <= ::llc::argsOptionIndex(runtimeValues.EntryPointArgs, "rsm-file")) {
		::llc::vcst_t rsmPath = {};
		if_fail_fe(::llc::argsOptionValue(runtimeValues.EntryPointArgs, "rsm-file", rsmPath));
		if_fail_fe(TestRSMPath.append(rsmPath));
	}
	::llc::aobj<::llc::vcst_t> selectedSuites;
	if_fail_fe(::llc::argsOptionValues(runtimeValues.EntryPointArgs, "test", selectedSuites));
	if(0 <= ::llc::argsOptionIndex(runtimeValues.EntryPointArgs, "seed")) {
		::llc::vcst_t seedText = {};
		if_fail_fe(::llc::argsOptionValue(runtimeValues.EntryPointArgs, "seed", seedText));
		::llc::u3_t seed = {};
		cnst ::llc::err_t digits = ::llc::parseIntegerDecimal(seedText, seed);
		stxp ::llc::vcst_t seedMaximum = "18446744073709551615";
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
	result = testSuitesRun(results, run, selectedSuites
<<<<<<< Updated upstream
		, LLC_TEST_SUITE(testD3D, "d3d")
		, LLC_TEST_SUITE(testEngineManagers, "engine_managers")
		, LLC_TEST_SUITE(testEngineSerialization, "engine_serialization")
		//, LLC_TEST_SUITE(testRORSM, "ro_rsm")
=======
		, LLC_TEST_SUITE(testEngineManagers, "engine_managers")
		, LLC_TEST_SUITE(testEngineSerialization, "engine_serialization")
		, LLC_TEST_SUITE(testRagnarokGAT, "ragnarok_gat")
		, LLC_TEST_SUITE(testRagnarokGND, "ragnarok_gnd")
>>>>>>> Stashed changes
		);
	testTimer.Frame();

	::llc::u2_c		successCount		= testSuccessCount(results);
	::llc::u2_c		failureCount		= testErrorCount(results);

	if_true_ef(::llc::failed(result), "Test run has suite execution or reporting failures. Result:%i.", result);
	if_true_ef(failureCount, "Test run completed in %" LLC_FMT_U3 " us: %u/%u checks passed, %u failed across %u result groups.", testTimer.LastTimeMicroseconds, successCount, successCount + failureCount, failureCount, results.size())
	else { 
		always_printf("Test run completed in %" LLC_FMT_U3 " us: all %u checks passed across %u result groups.", testTimer.LastTimeMicroseconds, successCount, results.size());
	}
	rtrn ::llc::failed(result) ? result : failureCount ? ::llc::OS_ERROR : 0;
}

sttc ::llc::err_t test_engine_entry_point(::llc::SRuntimeValues & runtimeValues) {
	TestConsoleEnabled = 0 <= ::llc::argsOptionIndex(runtimeValues.EntryPointArgs, "console");
	STestLogBuffer log;
	STestLogCapture capture(log);
	if(!TestConsoleEnabled) capture.Previous = {};
	STestRunReport run;
	::llc::err_t status = 0;
	::llc::STimer timer;
	try {
		status = [&]() -> ::llc::err_t {
			::llc::vcst_t parent = ".";
			if(0 <= ::llc::argsOptionIndex(runtimeValues.EntryPointArgs, "output"))
				if_fail_fe(::llc::argsOptionValue(runtimeValues.EntryPointArgs, "output", parent));
			if_fail_fe(testRunFolder(parent, run.Folder));
			always_printf("Test output: %.*s", LLC_VIEW_ARGS(run.Folder));
			rtrn testEngineRun(runtimeValues, run);
		}();
	}
<<<<<<< Updated upstream
	try { status = ::llc::min(status, testRunReportWrite(run, status, timer.LastTimeMicroseconds, TestSeedOverrideEnabled, TestSeedOverride)); }
	catch(...) { status = -1; error_printf("%s", "Cannot write run reports."); }
	try { status = ::llc::min(status, testRootLogWrite(run.Folder, log)); }
	catch(...) { status = -1; }
	rtrn status;
=======
	catch(...) { status = -1; error_printf("%s", "Runner setup or execution exception."); }
	timer.Frame();
	if(log.Failed) status = -1;
	if(run.Folder.size()) {
		try { status = ::llc::min(status, testRunReportWrite(run, status, timer.LastTimeMicroseconds, TestSeedOverrideEnabled, TestSeedOverride)); }
		catch(...) { status = -1; error_printf("%s", "Cannot write run reports."); }
	}
	try { status = ::llc::min(status, testRootLogWrite(run.Folder, log)); }
	catch(...) { status = -1; }
	rtrn log.Failed ? -1 : status;
>>>>>>> Stashed changes
}
