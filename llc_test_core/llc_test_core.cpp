// llc_test_core.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "llc_test_core_suites.h"
#include "llc_log.h"
#include "llc_parse.h"
#include "llc_timer.h"
#include "llc_runtime.h"
#include "llc_test_report.h"

sttc ::llc::u3_t TestSeedOverride = {};
sttc bool TestSeedOverrideEnabled = false;

::llc::u3_t testSeed(::llc::u3_t fallback) { rtrn TestSeedOverrideEnabled ? TestSeedOverride : fallback; }

GDEFINE_ENUM_TYPE(TEST_REPORT_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_REPORT_RESULT, CAPTURE, 0, "Log capture must preserve print/write bytes and restore the enclosing buffer.");
GDEFINE_ENUM_VALUED(TEST_REPORT_RESULT, QUIET, 1, "File-only capture must not forward to the console writer.");
GDEFINE_ENUM_VALUED(TEST_REPORT_RESULT, CONSOLE, 2, "Opt-in console capture must forward each byte once.");
GDEFINE_ENUM_VALUED(TEST_REPORT_RESULT, ROOT_ERROR, 3, "Report-writing errors must survive in the root log.");

sttc ::llc::err_t testReporting(ATestError & errors, ::llc::vcst_t outputFolder) {
	STestLogBuffer captured;
	cnst STestLogWriters previousWriters = TestConsoleWriters;
	cnst bool previousConsole = TestConsoleEnabled;
	STestLogBuffer * previousBuffer = TestActiveLog;
	sttc ::llc::u2_t forwarded = 0;
	forwarded = 0;
	::llc::u2_t quietForwarded = 0;
	::llc::err_t captureResult = 0;
	TestConsoleWriters.Write = [](const char *, ::llc::u2_t length) -> ::llc::err_t { forwarded += length; rtrn 0; };
	{
		STestLogCapture capture(captured);
		TestConsoleEnabled = false;
		captureResult = ::llc::log_print("quiet");
		captureResult = ::llc::min(captureResult, ::llc::log_write(":raw", 4));
		quietForwarded = forwarded;
		TestConsoleEnabled = true;
		captureResult = ::llc::min(captureResult, ::llc::log_print("echo"));
		captureResult = ::llc::min(captureResult, ::llc::log_write(":raw", 4));
	}
	TestConsoleEnabled = previousConsole;
	TestConsoleWriters = previousWriters;
	STestLogWriters restored;
	if_fail_fe(::llc::getLogCallbacks(restored.Print, restored.Write));
	LLC_TEST_CHECK(errors, TEST_REPORT_RESULT_CAPTURE, ::llc::failed(captureResult) || captured.Failed || captured.Text != "quiet:rawecho:raw" || TestActiveLog != previousBuffer || restored.Print != testCapturePrint || restored.Write != testCaptureWrite);
	LLC_TEST_CHECK(errors, TEST_REPORT_RESULT_QUIET, quietForwarded != 0);
	LLC_TEST_CHECK(errors, TEST_REPORT_RESULT_CONSOLE, forwarded != 8);

	STestRunReport failedRun;
	::llc::string blockedFile;
	if_fail_fe(::llc::pathNameCompose(outputFolder, "report_write_failure", failedRun.Folder));
	if_fail_fe(::llc::pathCreate(failedRun.Folder));
	if_fail_fe(::llc::pathNameCompose(failedRun.Folder, "results.json", blockedFile));
	if_fail_fe(::llc::pathCreate(blockedFile)); // A directory cannot be replaced with the JSON file.
	STestLogBuffer failureLog;
	::llc::string expected;
	::llc::err_t reportResult, logResult;
	{
		STestLogCapture capture(failureLog);
		reportResult = testRunReportWrite(failedRun, -1, 0, false, 0);
		if_fail_fe(expected.append(failureLog.Text));
		logResult = testRootLogWrite(failedRun.Folder, failureLog);
	}
	::llc::au0_t saved;
	if_fail_fe(::llc::fileToMemory(failedRun.Folder, "console.log", saved));
	LLC_TEST_CHECK(errors, TEST_REPORT_RESULT_ROOT_ERROR, 0 <= reportResult || ::llc::failed(logResult) || failureLog.Failed || !expected.size() || saved != expected.cc().cu8());
	rtrn 0;
}

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

sttc	::llc::err_t	test_core_entry_point		(::llc::SRuntimeValues & runtimeValues);
LLC_OS_ENTRY_POINT(::test_core_entry_point);


sttc ::llc::err_t testCoreRun(::llc::SRuntimeValues & runtimeValues, STestRunReport & run) {
	//_CrtSetBreakAlloc(12390);
	ATestError		results;
	llc::err_t		result				= 0;
	run.SuccessDetails	= 0 <= ::llc::argsOptionIndex(runtimeValues.EntryPointArgs, "success-details");
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
		, LLC_TEST_SUITE(testSPRNG, "noise")
		, LLC_TEST_SUITE(testSTL, "stl")
		, LLC_TEST_SUITE(testAdam7, "adam7")
		//, LLC_TEST_SUITE(testApp, "app")
		, LLC_TEST_SUITE(testASCIIColor, "ascii_color")
		, LLC_TEST_SUITE(testASCIITarget, "ascii_target")
		, LLC_TEST_SUITE(testArgs, "args")
		, LLC_TEST_SUITE(testBitField, "bit")
		, LLC_TEST_SUITE(testBitmapTarget, "bitmap_target")
		, LLC_TEST_SUITE(testBlock, "block")
		, LLC_TEST_SUITE(testBlockContainer, "block_container")
		, LLC_TEST_SUITE(testCamera, "camera")
		, LLC_TEST_SUITE(testCollision, "collision")
		, LLC_TEST_SUITE(testCOMPlus, "complus")
		, LLC_TEST_SUITE(testCPow, "cpow")
		, LLC_TEST_SUITE(testCTTIParser, "ctti")
		, LLC_TEST_SUITE(testD3D, "d3d")
		, LLC_TEST_SUITE(testDeflate, "deflate")
		, LLC_TEST_SUITE(testEngineManagers, "engine_managers")
		, LLC_TEST_SUITE(testEngineSerialization, "engine_serialization")
		, LLC_TEST_SUITE(testEnum, "enum")
		, LLC_TEST_SUITE(testExpression, "expression")
		, LLC_TEST_SUITE(testGeometry, "geometry")
		, LLC_TEST_SUITE(testGridImage, "grid_image")
		, LLC_TEST_SUITE(testGridOperations, "grid_operations")
		, LLC_TEST_SUITE(testGUI, "gui")
		, LLC_TEST_SUITE(testInput, "input")
		, LLC_TEST_SUITE(testStr, "str")
		, LLC_TEST_SUITE(testSystemKey, "system_key")
		, LLC_TEST_SUITE(testTypeMetadata, "type_metadata")
		, LLC_TEST_SUITE(testArrayStatic, "array_static")
		, LLC_TEST_SUITE(testArrayBit, "array_bit")
		, LLC_TEST_SUITE(testArrayMinMax, "apod_minmax")
		, LLC_TEST_SUITE(testArrayObj, "array_obj")
		, LLC_TEST_SUITE(testArrayPod, "array_pod")
		, LLC_TEST_SUITE(testBase64, "base64")
		, LLC_TEST_SUITE(testJSONExpression, "json_expression")
		, LLC_TEST_SUITE(testJSONReader, "json")
		, LLC_TEST_SUITE(testKeyVal, "keyval")
		, LLC_TEST_SUITE(testLabel, "label")
		, LLC_TEST_SUITE(testLinearMap, "linear_map")
		, LLC_TEST_SUITE(testXMLReader, "xml")
		, LLC_TEST_SUITE(testMSBuildXML, "msbuild_xml")
		, LLC_TEST_SUITE(testPath, "path")
		, LLC_TEST_SUITE(testPointers, "ptr")
		, LLC_TEST_SUITE(testPNG, "png")
		, LLC_TEST_SUITE(testProcess, "process")
		, LLC_TEST_SUITE(testReporting, "reporting")
		//, LLC_TEST_SUITE(testRuntimeModule, "runtime_module")
		, LLC_TEST_SUITE(testView, "view")
		, LLC_TEST_SUITE(testViewAliases, "view_aliases")
		, LLC_TEST_SUITE(testViewBit, "view_bit")
		, LLC_TEST_SUITE(testPackedUInt, "packed_int")
		, LLC_TEST_SUITE(testViewSerialize, "view_serialize")
		, LLC_TEST_SUITE(testViewStream, "view_stream")
		, LLC_TEST_SUITE(testVOX, "vox")
		, LLC_TEST_SUITE(testVoxel, "voxel")
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

sttc ::llc::err_t test_core_entry_point(::llc::SRuntimeValues & runtimeValues) {
	TestConsoleEnabled = 0 <= ::llc::argsOptionIndex(runtimeValues.EntryPointArgs, "console");
	STestLogBuffer log;
	STestLogCapture capture(log);
	if(!TestConsoleEnabled) capture.Previous = {}; // Keep callback teardown silent when this runner exits.
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
			rtrn testCoreRun(runtimeValues, run);
		}();
	}
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
}
