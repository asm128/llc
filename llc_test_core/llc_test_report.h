#ifndef LLC_TEST_REPORT_H
#define LLC_TEST_REPORT_H

#include "llc_file.h"
#include <chrono>
#include <ctime>
#include <filesystem>
#include <mutex>
#include <string>
#include <vector>

struct STestLogWriters {
	::llc::log_print_t Print = {};
	::llc::log_write_t Write = {};
};

struct STestLogBuffer {
	::std::string Text;
	::std::mutex Mutex;
	bool Failed = false;
};
sttc STestLogBuffer * TestActiveLog = {};
sttc STestLogWriters TestConsoleWriters;

sttc ::llc::err_t testCaptureWrite(const char * text, ::llc::u2_t length) {
	if(TestActiveLog) {
		::std::lock_guard<::std::mutex> lock(TestActiveLog->Mutex);
		try { TestActiveLog->Text.append(text, length); }
		catch(...) { TestActiveLog->Failed = true; }
	}
	rtrn TestConsoleWriters.Write ? TestConsoleWriters.Write(text, length) : 0;
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

sttc ::llc::vcst_t testText(const ::std::string & text) { rtrn {text.c_str(), (::llc::u2_t)text.size()}; }
sttc ::llc::err_t testJSONString(::llc::vcst_t text, ::std::string & result) {
	result = "\"";
	stxp char hex[] = "0123456789abcdef";
	for(::llc::u2_t i = 0; i < text.size(); ++i) {
		cnst auto c = (::llc::u0_t)text[i];
		if(c == '"' || c == '\\') { result += '\\'; result += (char)c; }
		else if(c < 32) { result += "\\u00"; result += hex[c >> 4]; result += hex[c & 15]; }
		else result += (char)c;
	}
	result += '"';
	rtrn 0;
}

struct STestSuiteReport {
	::llc::vcst_t Name, Folder;
	::llc::err_t Status = 0, ReportStatus = 0;
	::llc::u3_t Microseconds = 0;
	::llc::u2_t Checks = 0, Failures = 0, Groups = 0;
};
struct STestRunReport {
	::std::string Folder;
	::std::vector<STestSuiteReport> Suites;
	bool SuccessDetails = false;
};

sttc ::llc::err_t testReportWrite(const ::std::string & folder, const char * filename, const ::std::string & contents) {
	cnst ::std::string path = folder + "/" + filename;
	if_true_fe(contents.size() > ::llc::u2_t(-1) || path.size() > ::llc::u2_t(-1));
	rtrn ::llc::fileFromMemory(testText(path), testText(contents));
}
sttc ::llc::err_t testSummaryJSON(cnst STestSuiteReport & report, ::std::string & output) {
	::std::string name, folder;
	if_fail_fe(testJSONString(report.Name, name));
	if_fail_fe(testJSONString(report.Folder, folder));
	output = "{\"name\":" + name
		+ ",\"folder\":" + folder
		+ ",\"status\":" + ::std::to_string(report.Status)
		+ ",\"report_status\":" + ::std::to_string(report.ReportStatus)
		+ ",\"duration_us\":" + ::std::to_string(report.Microseconds)
		+ ",\"checks\":" + ::std::to_string(report.Checks)
		+ ",\"passed\":" + ::std::to_string(report.Checks - report.Failures)
		+ ",\"failed\":" + ::std::to_string(report.Failures)
		+ ",\"result_groups\":" + ::std::to_string(report.Groups);
	rtrn 0;
}
sttc ::llc::err_t testSuiteReportWrite(const ::std::string & folder, cnst STestSuiteReport & report, cnst ATestError & results, cnst STestLogBuffer & log) {
	::std::string json;
	if_fail_fe(testSummaryJSON(report, json));
	json += ",\"schema_version\":1,\"results\":[";
	for(::llc::u2_t i = 0; i < results.size(); ++i) {
		cnst auto & item = results[i];
		::std::string enumName, valueName, description;
		if_fail_fe(testJSONString(item.EnumName, enumName));
		if_fail_fe(testJSONString(item.ValueName, valueName));
		if_fail_fe(testJSONString(item.Description, description));
		if(i) json += ',';
		json += "{\"enum\":" + enumName
			+ ",\"name\":" + valueName
			+ ",\"value\":" + ::std::to_string(item.Value)
			+ ",\"description\":" + description
			+ ",\"passed\":" + ::std::to_string(item.SuccessCount)
			+ ",\"failed\":" + ::std::to_string(item.Count) + "}";
	}
	json += "]}\n";
	cnst ::llc::err_t logResult = testReportWrite(folder, "console.log", log.Text);
	cnst ::llc::err_t jsonResult = testReportWrite(folder, "results.json", json);
	rtrn ::llc::min(logResult, jsonResult);
}

sttc ::llc::err_t testRunFolder(::llc::vcst_t parent, ::std::string & folder) {
	namespace fs = ::std::filesystem;
	::std::error_code error;
	cnst fs::path base = fs::absolute(fs::path(::std::string(parent.begin(), parent.size())), error);
	if_true_fef(error, "Cannot resolve output directory: %s.", error.message().c_str());
	fs::create_directories(base, error);
	if_true_fef(error, "Cannot create output directory: %s.", error.message().c_str());
	cnst auto now = ::std::chrono::system_clock::now();
	cnst auto time = ::std::chrono::system_clock::to_time_t(now);
	::std::tm local = {};
#ifdef LLC_WINDOWS
	if_true_fe(localtime_s(&local, &time));
#else
	if_null_fe(localtime_r(&time, &local));
#endif
	char stamp[32] = {};
	if_zero_fe(::std::strftime(stamp, sizeof(stamp), "%y%m%d_%H%M%S", &local));
	cnst auto hundredths = ::std::chrono::duration_cast<::std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000 / 10;
	cnst ::std::string prefix = "llc_test_core_" + ::std::string(stamp) + (hundredths < 10 ? "0" : "") + ::std::to_string(hundredths);
	for(::llc::u2_t collision = 0; collision < 10000; ++collision) {
		cnst fs::path candidate = base / (prefix + (collision ? "_" + ::std::to_string(collision) : ""));
		if(fs::create_directory(candidate, error)) { folder = candidate.generic_string(); rtrn 0; }
		if_true_fef(error, "Cannot create run directory: %s.", error.message().c_str());
	}
	rtrn -1;
}

sttc ::llc::err_t testRunReportWrite(cnst STestRunReport & run, ::llc::err_t status, ::llc::u3_t duration, bool seedEnabled, ::llc::u3_t seed, cnst STestLogBuffer & log) {
	::llc::u3_t checks = 0, failures = 0;
	::std::string suites;
	for(cnst auto & suite : run.Suites) {
		checks += suite.Checks; failures += suite.Failures;
		if(!suites.empty()) suites += ',';
		::std::string summary, relativePath;
		if_fail_fe(testSummaryJSON(suite, summary));
		if_fail_fe(testJSONString(testText(::std::string(suite.Folder.begin(), suite.Folder.size()) + "/results.json"), relativePath));
		suites += summary + ",\"results\":" + relativePath + "}";
	}
#ifdef _DEBUG
	stxp ::llc::vcst_t configuration = "Debug";
#else
	stxp ::llc::vcst_t configuration = "Release";
#endif
	::std::string config;
	if_fail_fe(testJSONString(configuration, config));
	cnst ::std::string json = "{\"schema_version\":1,\"name\":\"llc_test_core\",\"configuration\":" + config
		+ ",\"pointer_bits\":" + ::std::to_string(sizeof(void*) * 8)
		+ ",\"status\":" + ::std::to_string(status) + ",\"duration_us\":" + ::std::to_string(duration)
		+ ",\"seed_override\":" + (seedEnabled ? "\"" + ::std::to_string(seed) + "\"" : "null")
		+ ",\"checks\":" + ::std::to_string(checks) + ",\"passed\":" + ::std::to_string(checks - failures)
		+ ",\"failed\":" + ::std::to_string(failures) + ",\"suites\":[" + suites + "]}\n";
	cnst ::llc::err_t jsonResult = testReportWrite(run.Folder, "results.json", json);
	cnst ::llc::err_t logResult = testReportWrite(run.Folder, "console.log", log.Text);
	rtrn ::llc::min(jsonResult, logResult);
}
#endif // LLC_TEST_REPORT_H
