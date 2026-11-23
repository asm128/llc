#include "llc_aobj_pobj.h"
#include "llc_process.h"
#include "llc_sync.h"

#ifndef LLC_PROCESS_BROKER_H_23627
#define LLC_PROCESS_BROKER_H_23627

namespace llc
{
	stct SProcessBrokerJob {
		au0_t					Request;
		au0_t					Response;
		au0_t					ErrorOutput;
		string					CommandLineApp;
		string					CommandLine;
		u2_t					ExitCode			= {};
		refcount_t				ErrorDone			= 0;
		err_t					ErrorResult		= 0;
		refcount_t				Done				= 0;
		err_t					Result				= 0;
	};

	stct SProcessBroker {
		string					CommandLine;
		string					CommandLineApp;
		apobj<SProcessBrokerJob>	Jobs;
	};

	err_t		processBrokerRun		(SProcessBrokerJob & job);
}

#endif // LLC_PROCESS_BROKER_H_23627
