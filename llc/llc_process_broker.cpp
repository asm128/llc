#include "llc_process_broker.h"

::llc::err_t llc::processBrokerRun(SProcessBrokerJob & job) {
	job.Response.clear();
	job.ErrorOutput.clear();
	job.ExitCode						= 0;
	job.ErrorDone						= 0;
	job.ErrorResult					= 0;
	job.Done							= 0;
	job.Result = [&job]() -> ::llc::err_t {
		cnst ::llc::vcu0_t request			= job.Request;
		stxp ::llc::u0_t environmentEnd[]	= {0, 0};
		cnst ::llc::err_t contentOffset		= request.find(::llc::vcu0_t{environmentEnd, ::llc::size(environmentEnd)});
		if_true_fef(0 > contentOffset, "%s", "Failed to find the process environment block terminator.");
		cnst ::llc::u2_t contentBegin		= (::llc::u2_t)contentOffset + ::llc::size(environmentEnd);
		cnst ::llc::vcs0_t environment		= {(cnst ::llc::s0_t*)request.begin(), contentBegin};
		::llc::vcu0_t input;
		if_fail_fe(request.slice(input, contentBegin, request.size() - contentBegin));
		::llc::SProcessManager<> manager;
		rtrn manager.Execute(job.CommandLineApp, job.CommandLine, input, job.Response, job.ErrorOutput, job.ExitCode, environment);
	}();
	job.ErrorResult						= ::llc::failed(job.Result) ? job.Result : 0;
	job.ErrorDone						= 1;
	job.Done							= 1;
	rtrn job.Result;
}
