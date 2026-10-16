#include "llc_process.h"
#include "llc_keyval.h"

#ifdef LLC_WINDOWS
#	include <Windows.h>
#else
#	include <unistd.h>
#endif

::llc::error_t			llc::environmentBlockViews		(vcs0_t environmentBlock, aobj<kv<vcst_t>> & out_Views)	{
	::llc::aobj<::llc::kv<::llc::vcst_t>>	views						= {};
	for(::llc::u2_t offsetBeginVar = 0; offsetBeginVar < environmentBlock.size(); ) {
		cnst ::llc::err_t			offsetEndVar					= ::llc::find('\0', environmentBlock, offsetBeginVar);
		if_true_fef(0 > offsetEndVar, "Unterminated environment variable at offset:%u.", offsetBeginVar);
		if((::llc::u2_t)offsetEndVar == offsetBeginVar)
			break;
		::llc::vcst_t					viewEnvironVar					= {};
		llc_necs(environmentBlock.slice(viewEnvironVar, offsetBeginVar, offsetEndVar - offsetBeginVar));
		::llc::TKeyValConstString	newKeyVal						= {};
		if(-1 != ::llc::keyval_split({viewEnvironVar.begin(), viewEnvironVar.size()}, newKeyVal))
			llc_necs(views.push_back(newKeyVal));
		else {
			newKeyVal.Key			= {viewEnvironVar.begin(), viewEnvironVar.size()};
			llc_necs(views.push_back(newKeyVal));
		}
		offsetBeginVar				= offsetEndVar + 1;
	}
	llc_necs(out_Views.append(views));
	return 0;
}

::llc::error_t			llc::environmentBlockFromEnviron(::llc::string & environmentBlock)	{
#ifndef LLC_ATMEL
    for (sc_t **env = environ; *env; ++env)
		environmentBlock.append(*env, (::llc::u2_t)strlen(*env) + 1);
#endif
	environmentBlock.push_back(0);

	// The following function retrieves the process's environment block using GetEnvironmentStrings and adds it to the environmentBlock parameter.
	//sc_t						* lpvEnv						= GetEnvironmentStringsA();	// Get a pointer to the environment block.
	//ree_if(lpvEnv == NULL, "GetEnvironmentStringsA failed (%x)\n", (::llc::u2_t)GetLastError());
	//int32_t						lengthEnvStrings				= ::llc::find_sequence_pod(::llc::view<const sc_t>{"\0", 2U}, ::llc::view<const sc_t>{(const sc_t*)lpvEnv, (::llc::u2_t)-1});
	//environmentBlock		= ::llc::view<const sc_t>{lpvEnv, (::llc::u2_t)lengthEnvStrings + 2U};
	//FreeEnvironmentStringsA(lpvEnv);
	return 0;
}
