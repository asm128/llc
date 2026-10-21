#include "llc_file.h"
#include "llc_path.h"
#include "llc_test_core.h"
#include "llc_string_compose.h"

#include "llc_chrono.h"
#include "llc_mutex.h"

#ifndef LLC_TEST_REPORT_H
#define LLC_TEST_REPORT_H

struct STestLogWriters {
	::llc::log_print_t Print = {};
	::llc::log_write_t Write = {};
};

struct STestLogBuffer {
	::llc::string Text;
	::llc::mutex Mutex;
	bool Failed = false;
};
sttc STestLogBuffer * TestActiveLog = {};
sttc STestLogWriters TestConsoleWriters;

sttc ::llc::err_t testCaptureWrite(const char * text, ::llc::u2_t length) {
	// LLC allocation failures can log through this same callback.
	sttc thread_local bool writing = false;
	if(TestActiveLog && !writing) {
		writing = true;
		try {
			::llc::lock_guard lock(TestActiveLog->Mutex);
			if(::llc::failed(TestActiveLog->Text.append({text, length}))) TestActiveLog->Failed = true;
		}
		catch(...) { TestActiveLog->Failed = true; }
		writing = false;
	}
	rtrn 0; // TestConsoleWriters.Write ? TestConsoleWriters.Write(text, length) : 0;
}
sttc ::llc::err_t testCapturePrint(const char * text) { rtrn testCaptureWrite(text, (::llc::u2_t)strlen(text)); }

struct STestLogCapture {
	STestLogWriters Previous;
	STestLogBuffer * PreviousBuffer = TestActiveLog;
	STestLogCapture(STestLogBuffer & buffer) {
		::llc::getLogCallbacks(Previous.Print, Previous.Write);
		if(!PreviousBuffer)
			TestConsoleWriters = Previous;
		TestActiveLog = &buffer;
		::llc::setupLogCallbacks(testCapturePrint, testCaptureWrite);
	}
	~STestLogCapture() {
		TestActiveLog = PreviousBuffer;
		::llc::setupLogCallbacks(Previous.Print, Previous.Write);
	}
};

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

struct STestSuiteReport {
	::llc::vcst_t Name, Folder;
	::llc::err_t Status = 0, ReportStatus = 0;
	::llc::u3_t Microseconds = 0;
	::llc::u2_t Checks = 0, Failures = 0, Groups = 0;
};
struct STestRunReport {
	::llc::string Folder;
	::llc::aobj<STestSuiteReport> Suites;
	bool SuccessDetails = false;
};

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
sttc ::llc::err_t testSuiteReportWrite(::llc::vcst_t folder, cnst STestSuiteReport & report, cnst ATestError & results, cnst STestLogBuffer & log) {
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
	cnst ::llc::err_t logResult = testReportWrite(folder, "console.log", log.Text);
	cnst ::llc::err_t jsonResult = testReportWrite(folder, "results.json", json);
	rtrn ::llc::min(logResult, jsonResult);
}

sttc ::llc::err_t testRunFolder(::llc::vcst_t parent, ::llc::string & folder) {
	::llc::string name;
	if_fail_fe(::llc::timeStringFormat("llc_test_core_{yy}{MM}{dd}_{hh}{mm}{ss}{ff}", ::llc::timeCurrentInMs(), name));
	folder.clear();
	if_fail_fe(::llc::pathNameCompose(parent, name, folder));
	rtrn ::llc::pathCreate(folder);
}

sttc ::llc::err_t testRunReportWrite(cnst STestRunReport & run, ::llc::err_t status, ::llc::u3_t duration, bool seedEnabled, ::llc::u3_t seed, cnst STestLogBuffer & log) {
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
	cnst ::llc::err_t jsonResult = testReportWrite(run.Folder, "results.json", json);
	cnst ::llc::err_t logResult = testReportWrite(run.Folder, "console.log", log.Text);
	rtrn ::llc::min(jsonResult, logResult);
}
#endif // LLC_TEST_REPORT_H
