#include "llc_process_broker.h"
#include "llc_string_compose.h"
#include "llc_test_core.h"
#ifdef LLC_LINUX
#	include <errno.h>
#	include <signal.h>
#	include <unistd.h>
#endif

GDEFINE_ENUM_TYPE(PROCESS_BROKER_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(PROCESS_BROKER_TEST_RESULT, EXECUTE			, 1, "SProcessBroker failed to execute the configured child.");
GDEFINE_ENUM_VALUED(PROCESS_BROKER_TEST_RESULT, COMPLETION		, 2, "SProcessBroker reported an invalid completion state.");
GDEFINE_ENUM_VALUED(PROCESS_BROKER_TEST_RESULT, STDOUT_BYTES		, 3, "SProcessBroker lost or changed standard output.");
GDEFINE_ENUM_VALUED(PROCESS_BROKER_TEST_RESULT, STDERR_BYTES		, 4, "SProcessBroker lost or changed standard error.");
GDEFINE_ENUM_VALUED(PROCESS_BROKER_TEST_RESULT, EMPTY_INPUT		, 5, "SProcessBroker mishandled a request without standard input.");
GDEFINE_ENUM_VALUED(PROCESS_BROKER_TEST_RESULT, REQUEST			, 6, "SProcessBroker accepted a request without an environment terminator.");

#ifdef LLC_WINDOWS
sttc DWORD WINAPI processBrokerChildTimeout(void * completed) {
	if(WAIT_OBJECT_0 != WaitForSingleObject((HANDLE)completed, 15000))
		ExitProcess(124);
	rtrn 0;
}

::llc::err_t testProcessEchoChild() {
	HANDLE completed = CreateEventA(0, TRUE, FALSE, 0);
	if_null_fe(completed);
	HANDLE watchdog = CreateThread(0, 0, ::processBrokerChildTimeout, completed, 0, 0);
	if(0 == watchdog) {
		CloseHandle(completed);
		rtrn -1;
	}
	cnst ::llc::err_t result = []() -> ::llc::err_t {
		::llc::u0_t buffer[4096] = {};
		DWORD transferred = 0;
		for(::llc::u2_t i = 0; i < 16; ++i) {
			if(FALSE == WriteFile(GetStdHandle(STD_ERROR_HANDLE), buffer, sizeof(buffer), &transferred, 0) || transferred != sizeof(buffer))
				rtrn -1;
		}
		while(true) {
			if(FALSE == ReadFile(GetStdHandle(STD_INPUT_HANDLE), buffer, sizeof(buffer), &transferred, 0))
				rtrn ERROR_BROKEN_PIPE == GetLastError() ? 0 : -1;
			if(0 == transferred)
				rtrn 0;
			DWORD written = 0;
			if(FALSE == WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), buffer, transferred, &written, 0) || written != transferred)
				rtrn -1;
		}
	}();
	cnst BOOL signaled = SetEvent(completed);
	cnst DWORD waited = WaitForSingleObject(watchdog, INFINITE);
	cnst BOOL closedThread = CloseHandle(watchdog);
	cnst BOOL closedEvent = CloseHandle(completed);
	rtrn signaled && WAIT_OBJECT_0 == waited && closedThread && closedEvent ? result : -1;
}
#elif defined(LLC_LINUX)
sttc ::llc::err_t processBrokerTestWrite(int descriptor, ::llc::vcu0_t bytes) {
	for(::llc::u2_t offset = 0; offset < bytes.size(); ) {
		cnst ssize_t written = write(descriptor, bytes.begin() + offset, bytes.size() - offset);
		if(0 > written && EINTR == errno)
			continue;
		if(0 >= written)
			rtrn -1;
		offset += (::llc::u2_t)written;
	}
	rtrn 0;
}

::llc::err_t testProcessMarkerChild() {
	stxp ::llc::sc_t marker[] = "llc process child";
	if_fail_fe(::processBrokerTestWrite(STDERR_FILENO, {(cnst ::llc::u0_t*)marker, sizeof(marker) - 1}));
	rtrn -1;
}

::llc::err_t testProcessEchoChild() {
	if(SIG_ERR == signal(SIGALRM, SIG_DFL))
		rtrn -1;
	alarm(15);
	::llc::u0_t buffer[4096] = {};
	for(::llc::u2_t i = 0; i < 16; ++i)
		if_fail_fe(::processBrokerTestWrite(STDERR_FILENO, {buffer, sizeof(buffer)}));
	for(;;) {
		cnst ssize_t count = read(STDIN_FILENO, buffer, sizeof(buffer));
		if(0 > count && EINTR == errno)
			continue;
		if(0 > count)
			rtrn -1;
		if(0 == count)
			break;
		if_fail_fe(::processBrokerTestWrite(STDOUT_FILENO, {buffer, (::llc::u2_t)count}));
	}
	alarm(0);
	rtrn 0;
}
#endif

::llc::err_t test_process_broker(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
#if defined(LLC_WINDOWS) || defined(LLC_LINUX)
	::llc::SProcessBrokerJob job;
#ifdef LLC_WINDOWS
	::llc::sc_t modulePath[MAX_PATH] = {};
	cnst DWORD modulePathLength = GetModuleFileNameA(0, modulePath, ::llc::size(modulePath));
	LLC_TEST_REQUIREF(errors, PROCESS_BROKER_TEST_RESULT_EXECUTE, 0 == modulePathLength || ::llc::size(modulePath) == modulePathLength, "Executable path length:%u.", modulePathLength);
	job.CommandLineApp = ::llc::vcst_t{modulePath, modulePathLength};
	llc_necs(::llc::append_strings(job.CommandLine, '"', ::llc::vcst_t{modulePath, modulePathLength}, "\" --process-echo"));
#else
	job.CommandLineApp = "/proc/self/exe";
	job.CommandLine = "llc_test_core --process-echo";
#endif
	llc_necs(job.Request.push_back(0));
	llc_necs(job.Request.push_back(0));
	::llc::au0_t input;
	llc_necs(input.resize(4096));
	for(::llc::u2_t i = 0; i < input.size(); ++i)
		input[i] = (::llc::u0_t)i;
	llc_necs(job.Request.append(input));
	cnst ::llc::err_t result = ::llc::processBrokerRun(job);
	LLC_TEST_REQUIREF(errors, PROCESS_BROKER_TEST_RESULT_EXECUTE, ::llc::failed(result), "Result:%i.", result);
	LLC_TEST_CHECKF(errors, PROCESS_BROKER_TEST_RESULT_COMPLETION, 1 != job.Done || 1 != job.ErrorDone || job.ErrorResult, "Done:%i, error done:%i, error result:%i.", (int)job.Done, (int)job.ErrorDone, job.ErrorResult);
	LLC_TEST_REQUIREF(errors, PROCESS_BROKER_TEST_RESULT_STDOUT_BYTES, job.Response.size() != input.size(), "Output:%u, expected:%u.", job.Response.size(), input.size());
	LLC_TEST_CHECK(errors, PROCESS_BROKER_TEST_RESULT_STDOUT_BYTES, memcmp(job.Response.begin(), input.begin(), input.size()));
	stxp ::llc::u2_t expectedErrorSize = 16 * 4096;
	LLC_TEST_REQUIREF(errors, PROCESS_BROKER_TEST_RESULT_STDERR_BYTES, job.ErrorOutput.size() != expectedErrorSize, "Error output:%u, expected:%u.", job.ErrorOutput.size(), expectedErrorSize);
	bool errorChanged = false;
	for(::llc::u2_t i = 0; i < job.ErrorOutput.size(); ++i)
		errorChanged |= 0 != job.ErrorOutput[i];
	LLC_TEST_CHECK(errors, PROCESS_BROKER_TEST_RESULT_STDERR_BYTES, errorChanged);

	::llc::SProcessBrokerJob emptyJob;
	emptyJob.CommandLineApp = job.CommandLineApp;
#ifdef LLC_WINDOWS
	llc_necs(::llc::append_strings(emptyJob.CommandLine, '"', ::llc::vcst_t{modulePath, modulePathLength}, "\" --process-child"));
#else
	emptyJob.CommandLine = "llc_test_core --process-child";
#endif
	llc_necs(emptyJob.Request.push_back(0));
	llc_necs(emptyJob.Request.push_back(0));
	cnst ::llc::err_t emptyResult = ::llc::processBrokerRun(emptyJob);
	LLC_TEST_REQUIREF(errors, PROCESS_BROKER_TEST_RESULT_EMPTY_INPUT, ::llc::failed(emptyResult), "Result:%i.", emptyResult);
	stxp ::llc::sc_t marker[] = "llc process child";
	LLC_TEST_CHECKF(errors, PROCESS_BROKER_TEST_RESULT_EMPTY_INPUT, emptyJob.Response.size() || emptyJob.ErrorOutput.size() != ::llc::size(marker) - 1 || memcmp(emptyJob.ErrorOutput.begin(), marker, ::llc::size(marker) - 1), "Output:%u, error:%u, expected error:%u.", emptyJob.Response.size(), emptyJob.ErrorOutput.size(), ::llc::size(marker) - 1);
	LLC_TEST_CHECKF(errors, PROCESS_BROKER_TEST_RESULT_COMPLETION, 1 != emptyJob.Done || 1 != emptyJob.ErrorDone || emptyJob.ErrorResult, "Done:%i, error done:%i, error result:%i.", (int)emptyJob.Done, (int)emptyJob.ErrorDone, emptyJob.ErrorResult);

	::llc::SProcessBrokerJob invalidJob;
	invalidJob.CommandLineApp = job.CommandLineApp;
	invalidJob.CommandLine = job.CommandLine;
	llc_necs(invalidJob.Request.push_back(0));
	cnst ::llc::err_t invalidResult = ::llc::processBrokerRun(invalidJob);
	LLC_TEST_CHECKF(errors, PROCESS_BROKER_TEST_RESULT_REQUEST, ::llc::succeeded(invalidResult) || 1 != invalidJob.Done, "Result:%i, done:%i.", invalidResult, (int)invalidJob.Done);
#endif
	rtrn 0;
}
