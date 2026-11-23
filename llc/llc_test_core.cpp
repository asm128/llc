#include "llc_test_core.h"
#include "llc_file.h"
#include "llc_chrono.h"
#include "llc_parse.h"
#include "llc_string_compose.h"

#include <cstdio>

#if defined(LLC_WINDOWS)
#	include <io.h>
#elif defined(LLC_LINUX)
#	include <unistd.h>
#endif

#if defined(LLC_WINDOWS)
#	define llc_test_dup		_dup
#	define llc_test_dup2		_dup2
#	define llc_test_close	_close
#	define llc_test_fileno	_fileno
#elif defined(LLC_LINUX)
#	define llc_test_dup		dup
#	define llc_test_dup2		dup2
#	define llc_test_close	close
#	define llc_test_fileno	fileno
#endif

namespace llc {
sttc u3_t TestSeedOverride = {};
sttc bool TestSeedOverrideEnabled = false;
u3_t testSeed(u3_t fallback) { rtrn TestSeedOverrideEnabled ? TestSeedOverride : fallback; }

STestLogCapture::~STestLogCapture() { close(); }

::llc::err_t STestLogCapture::open(::llc::vcst_t fileName) {
	if_true_fe(Active);
	if_true_fe(0 == fileName.size());
	if(0 != ::fflush(stdout) || 0 != ::fflush(stderr))
		rtrn -1;

	cnst int stdOut = llc_test_fileno(stdout);
	cnst int stdErr = llc_test_fileno(stderr);
	if(0 > stdOut || 0 > stdErr)
		rtrn -1;
	SavedStdOut = (::llc::s2_t)llc_test_dup(stdOut);
	SavedStdErr = (::llc::s2_t)llc_test_dup(stdErr);
	if(0 > SavedStdOut || 0 > SavedStdErr) {
		if(0 <= SavedStdOut) llc_test_close(SavedStdOut);
		if(0 <= SavedStdErr) llc_test_close(SavedStdErr);
		SavedStdOut = SavedStdErr = -1;
		rtrn -1;
	}
	FILE * outputFile = {};
#if defined(LLC_WINDOWS)
	cnst int openResult = ::fopen_s(&outputFile, fileName.begin(), "wb");
#elif defined(LLC_LINUX)
	outputFile = ::fopen(fileName.begin(), "wb");
	cnst int openResult = outputFile ? 0 : -1;
#endif
	if(0 != openResult || 0 == outputFile) {
		llc_test_close(SavedStdOut);
		llc_test_close(SavedStdErr);
		SavedStdOut = SavedStdErr = -1;
		rtrn -1;
	}
	cnst int output = llc_test_fileno(outputFile);
	cnst int stdoutResult = llc_test_dup2(output, stdOut);
	cnst int stderrResult = llc_test_dup2(output, stdErr);
	::fclose(outputFile);
	if(0 > stdoutResult || 0 > stderrResult) {
		llc_test_dup2(SavedStdOut, stdOut);
		llc_test_dup2(SavedStdErr, stdErr);
		llc_test_close(SavedStdOut);
		llc_test_close(SavedStdErr);
		SavedStdOut = SavedStdErr = -1;
		rtrn -1;
	}
	Active = true;
	rtrn 0;
}

::llc::err_t STestLogCapture::close() {
	if(false == Active)
		rtrn 0;
	::llc::err_t result = 0;
	if(0 != ::fflush(stdout) || 0 != ::fflush(stderr))
		result = -1;
	cnst int stdOut = llc_test_fileno(stdout);
	cnst int stdErr = llc_test_fileno(stderr);
	if(0 > llc_test_dup2(SavedStdOut, stdOut)) result = -1;
	if(0 > llc_test_dup2(SavedStdErr, stdErr)) result = -1;
	if(0 != llc_test_close(SavedStdOut)) result = -1;
	if(0 != llc_test_close(SavedStdErr)) result = -1;
	SavedStdOut = SavedStdErr = -1;
	Active = false;
	rtrn result;
}

sttc ::llc::err_t testJSONString(::llc::vcst_t text, ::llc::string & result) {
	result.clear();
	if_fail_fe(result.push_back('"'));
	stxp char hex[] = "0123456789abcdef";
	for(::llc::u2_t i = 0; i < text.size(); ++i) {
		cnst auto c = (::llc::u0_t)text[i];
		if(c == '"' || c == '\\') { if_fail_fe(::llc::append_strings(result, '\\', (::llc::sc_t)c)); }
		else if(c < 32) { if_fail_fe(::llc::append_strings(result, "\\u00", hex[c >> 4], hex[c & 15])); }
		else { if_fail_fe(result.push_back((::llc::sc_t)c)); }
	}
	rtrn result.push_back('"');
}

sttc ::llc::err_t testReportWrite(::llc::vcst_t folder, ::llc::vcst_t filename, ::llc::vcst_t contents) {
	::llc::string path;
	if_fail_fe(::llc::pathNameCompose(folder, filename, path));
	rtrn ::llc::fileFromMemory(path, contents);
}
sttc ::llc::err_t testSummaryJSON(cnst STestSuiteReport & report, ::llc::string & output) {
	::llc::string name, folder;
	if_fail_fe(testJSONString(report.Name, name));
	if_fail_fe(testJSONString(report.Folder, folder));
	output.clear();
	rtrn ::llc::append_strings(output, "{\"name\":", name
		, ",\"folder\":", folder
		, ",\"status\":", ::llc::str(report.Status)
		, ",\"report_status\":", ::llc::str(report.ReportStatus)
		, ",\"duration_us\":", ::llc::str(report.Microseconds)
		, ",\"checks\":", ::llc::str(report.Checks)
		, ",\"passed\":", ::llc::str(report.Checks - report.Failures)
		, ",\"failed\":", ::llc::str(report.Failures)
		, ",\"result_groups\":", ::llc::str(report.Groups));
}
::llc::err_t testSuiteReportWrite(::llc::vcst_t folder, cnst STestSuiteReport & report, cnst ATestError & results) {
	::llc::string json;
	if_fail_fe(testSummaryJSON(report, json));
	if_fail_fe(json.append_string(",\"schema_version\":1,\"results\":["));
	for(::llc::u2_t i = 0; i < results.size(); ++i) {
		cnst auto & item = results[i];
		::llc::string enumName, valueName, description;
		if_fail_fe(testJSONString(item.EnumName, enumName));
		if_fail_fe(testJSONString(item.ValueName, valueName));
		if_fail_fe(testJSONString(item.Description, description));
		if(i) { if_fail_fe(json.push_back(',')); }
		if_fail_fe(::llc::append_strings(json, "{\"enum\":", enumName
			, ",\"name\":", valueName
			, ",\"value\":", ::llc::str(item.Value)
			, ",\"description\":", description
			, ",\"passed\":", ::llc::str(item.SuccessCount)
			, ",\"failed\":", ::llc::str(item.Count), '}'));
	}
	if_fail_fe(json.append_string("]}\n"));
	rtrn testReportWrite(folder, "results.json", json);
}

sttc ::llc::err_t testRunFolder(::llc::vcst_t parent, ::llc::string & folder) {
	::llc::string name;
	if_fail_fe(::llc::timeStringFormat("llc_test_core_{yy}{MM}{dd}_{hh}{mm}{ss}{ff}", ::llc::timeCurrentInMs(), name));
	folder.clear();
	if_fail_fe(::llc::pathNameCompose(parent, name, folder));
	rtrn ::llc::pathCreate(folder);
}

sttc ::llc::err_t testRunReportWrite(cnst STestRunReport & run, ::llc::err_t status, ::llc::u3_t duration, bool seedEnabled, ::llc::u3_t seed) {
	::llc::u3_t checks = 0, failures = 0;
	::llc::string suites;
	for(cnst auto & suite : run.Suites) {
		checks += suite.Checks; failures += suite.Failures;
		if(suites.size()) { if_fail_fe(suites.push_back(',')); }
		::llc::string summary, relativePath, relativeName;
		if_fail_fe(testSummaryJSON(suite, summary));
		if_fail_fe(::llc::append_strings(relativeName, suite.Folder, "/results.json"));
		if_fail_fe(testJSONString(relativeName, relativePath));
		if_fail_fe(::llc::append_strings(suites, summary, ",\"results\":", relativePath, '}'));
	}
#ifdef _DEBUG
	stxp ::llc::vcst_t configuration = "Debug";
#else
	stxp ::llc::vcst_t configuration = "Release";
#endif
	::llc::string config, json;
	if_fail_fe(testJSONString(configuration, config));
	if_fail_fe(::llc::append_strings(json, "{\"schema_version\":1,\"name\":\"llc_test_core\",\"configuration\":", config
		, ",\"pointer_bits\":", ::llc::str((::llc::u2_t)(sizeof(void*) * 8))
		, ",\"status\":", ::llc::str(status), ",\"duration_us\":", ::llc::str(duration), ",\"seed_override\":"));
	if(seedEnabled) { if_fail_fe(::llc::append_strings(json, '"', ::llc::str(seed), '"')); }
	else { if_fail_fe(json.append_string("null")); }
	if_fail_fe(::llc::append_strings(json, ",\"checks\":", ::llc::str(checks), ",\"passed\":", ::llc::str(checks - failures)
		, ",\"failed\":", ::llc::str(failures), ",\"suites\":[", suites, "]}\n"));
	rtrn testReportWrite(run.Folder, "results.json", json);
}

err_t testRunPrepare(SRuntimeValues & runtimeValues, STestRunReport & run, aobj<vcst_t> & selectedSuites) {
	TestSeedOverride = {};
	TestSeedOverrideEnabled = false;
	vcst_t parent = ".";
	if(0 <= argsOptionIndex(runtimeValues.EntryPointArgs, "output")) {
		if_fail_fe(argsOptionValue(runtimeValues.EntryPointArgs, "output", parent));
	}
	if_fail_fe(testRunFolder(parent, run.Folder));
	always_printf("Test output: %.*s", LLC_VIEW_ARGS(run.Folder));
	run.SuccessDetails	= 0 <= ::llc::argsOptionIndex(runtimeValues.EntryPointArgs, "success-details");
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

	rtrn 0;
}
err_t testRunSummary(cnst ATestError & results, err_t result, u3_t microseconds) {
	::llc::u2_c		successCount		= testSuccessCount(results);
	::llc::u2_c		failureCount		= testErrorCount(results);

	if_true_ef(::llc::failed(result), "Test run has suite execution or reporting failures. Result:%i.", result);
	if_true_ef(failureCount, "Test run completed in %" LLC_FMT_U3 " us: %u/%u checks passed, %u failed across %u result groups.", microseconds, successCount, successCount + failureCount, failureCount, results.size())
	else {
		always_printf("Test run completed in %" LLC_FMT_U3 " us: all %u checks passed across %u result groups.", microseconds, successCount, results.size());
	}
	rtrn ::llc::failed(result) ? result : failureCount ? ::llc::OS_ERROR : 0;
}

err_t testRunFinish(cnst STestRunReport & run, err_t status, u3_t microseconds) {
	if(run.Folder.size()) {
		try { status = ::llc::min(status, testRunReportWrite(run, status, microseconds, TestSeedOverrideEnabled, TestSeedOverride)); }
		catch(...) { status = -1; error_printf("%s", "Cannot write run reports."); }
	}
	rtrn status;
}
} // namespace llc

GDEFINE_ENUM_TYPE(TEST_REPORT_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_REPORT_RESULT, OUTER_TEXT, 0, "The outer descriptor capture must receive stdout, stderr, LLC diagnostics and restored output exactly once.");
GDEFINE_ENUM_VALUED(TEST_REPORT_RESULT, INNER_TEXT, 1, "The nested descriptor capture must isolate its stdout and stderr bytes.");
GDEFINE_ENUM_VALUED(TEST_REPORT_RESULT, ROOT_ERROR, 2, "Report-writing errors must survive in a descriptor log.");

::llc::err_t llc::test_reporting(ATestError & errors, ::llc::vcst_t outputFolder) {
	::llc::string outerPath, innerPath;
	if_fail_fe(::llc::pathNameCompose(outputFolder, "capture_outer.log", outerPath));
	if_fail_fe(::llc::pathNameCompose(outputFolder, "capture_inner.log", innerPath));

	STestLogCapture outer;
	if_fail_fe(outer.open(outerPath));
	::fprintf(stdout, "%s", "stdout\n");
	::fflush(stdout);
	::fprintf(stderr, "%s", "stderr\n");
	::fflush(stderr);
	if_fail_fe(::llc::log_print("framework\n"));

	STestLogCapture inner;
	if_fail_fe(inner.open(innerPath));
	::fprintf(stdout, "%s", "inner-out\n");
	::fflush(stdout);
	::fprintf(stderr, "%s", "inner-err\n");
	::fflush(stderr);
	if_fail_fe(inner.close());
	::fprintf(stderr, "%s", "restored\n");
	::fflush(stderr);
	if_fail_fe(outer.close());

	::llc::au0_t outerBytes, innerBytes;
	if_fail_fe(::llc::fileToMemory(outerPath, outerBytes));
	if_fail_fe(::llc::fileToMemory(innerPath, innerBytes));
	stxp ::llc::sc_c expectedOuter[] = "stdout\nstderr\nframework\nrestored\n";
	stxp ::llc::sc_c expectedInner[] = "inner-out\ninner-err\n";
	cnst ::llc::vcu0_c outerExpected = {(cnst ::llc::u0_t*)expectedOuter, ::llc::size(expectedOuter) - 1};
	cnst ::llc::vcu0_c innerExpected = {(cnst ::llc::u0_t*)expectedInner, ::llc::size(expectedInner) - 1};
	LLC_TEST_CHECK(errors, TEST_REPORT_RESULT_OUTER_TEXT, outerBytes != outerExpected);
	LLC_TEST_CHECK(errors, TEST_REPORT_RESULT_INNER_TEXT, innerBytes != innerExpected);

	STestRunReport failedRun;
	::llc::string blockedFile;
	if_fail_fe(::llc::pathNameCompose(outputFolder, "report_write_failure", failedRun.Folder));
	if_fail_fe(::llc::pathCreate(failedRun.Folder));
	if_fail_fe(::llc::pathNameCompose(failedRun.Folder, "results.json", blockedFile));
	if_fail_fe(::llc::pathCreate(blockedFile)); // A directory cannot be replaced with the JSON file.
	::llc::string failurePath;
	if_fail_fe(::llc::pathNameCompose(outputFolder, "report_write_failure.log", failurePath));
	STestLogCapture failureCapture;
	if_fail_fe(failureCapture.open(failurePath));
	cnst ::llc::err_t reportResult = testRunReportWrite(failedRun, -1, 0, false, 0);
	cnst ::llc::err_t closeResult = failureCapture.close();
	::llc::au0_t saved;
	if_fail_fe(::llc::fileToMemory(failurePath, saved));
	LLC_TEST_CHECK(errors, TEST_REPORT_RESULT_ROOT_ERROR, 0 <= reportResult || ::llc::failed(closeResult) || !saved.size());
	rtrn 0;
}
