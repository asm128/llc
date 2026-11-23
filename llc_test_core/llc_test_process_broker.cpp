#include "llc_process_broker.h"
#include "llc_string_compose.h"
#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(PROCESS_BROKER_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(PROCESS_BROKER_TEST_RESULT, EXECUTE			, 1, "SProcessBroker failed to execute the configured child.");
GDEFINE_ENUM_VALUED(PROCESS_BROKER_TEST_RESULT, COMPLETION		, 2, "SProcessBroker reported an invalid completion state.");
GDEFINE_ENUM_VALUED(PROCESS_BROKER_TEST_RESULT, STDOUT_BYTES		, 3, "SProcessBroker lost or changed standard output.");
GDEFINE_ENUM_VALUED(PROCESS_BROKER_TEST_RESULT, STDERR_BYTES		, 4, "SProcessBroker lost or changed standard error.");

::llc::err_t testProcessBroker(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
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
	llc_necs(input.resize(1024 * 1024));
	for(::llc::u2_t i = 0; i < input.size(); ++i)
		input[i] = (::llc::u0_t)i;
	llc_necs(job.Request.append(input));
	cnst ::llc::err_t result = ::llc::processBrokerRun(job);
	LLC_TEST_REQUIREF(errors, PROCESS_BROKER_TEST_RESULT_EXECUTE, ::llc::failed(result) || job.ExitCode, "Result:%i, exit:%u.", result, job.ExitCode);
	LLC_TEST_CHECKF(errors, PROCESS_BROKER_TEST_RESULT_COMPLETION, 1 != job.Done || 1 != job.ErrorDone || job.ErrorResult, "Done:%i, error done:%i, error result:%i.", (int)job.Done, (int)job.ErrorDone, job.ErrorResult);
	LLC_TEST_REQUIREF(errors, PROCESS_BROKER_TEST_RESULT_STDOUT_BYTES, job.Response.size() != input.size(), "Output:%u, expected:%u.", job.Response.size(), input.size());
	LLC_TEST_CHECK(errors, PROCESS_BROKER_TEST_RESULT_STDOUT_BYTES, memcmp(job.Response.begin(), input.begin(), input.size()));
	stxp ::llc::u2_t expectedErrorSize = 16 * 4096;
	LLC_TEST_REQUIREF(errors, PROCESS_BROKER_TEST_RESULT_STDERR_BYTES, job.ErrorOutput.size() != expectedErrorSize, "Error output:%u, expected:%u.", job.ErrorOutput.size(), expectedErrorSize);
	bool errorChanged = false;
	for(::llc::u2_t i = 0; i < job.ErrorOutput.size(); ++i)
		errorChanged |= 0 != job.ErrorOutput[i];
	LLC_TEST_CHECK(errors, PROCESS_BROKER_TEST_RESULT_STDERR_BYTES, errorChanged);
#endif
	rtrn 0;
}
