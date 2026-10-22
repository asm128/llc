#include "llc_runtime_module.h"
#include "llc_type_registry.h"
#include "llc_sync.h"
#include "llc_timer.h"
#include "llc_process.h"

#if defined(LLC_WINDOWS)
#	include <Windows.h>
#	include <crtdbg.h>
#	include <process.h>
#endif
#include <cstdlib>		// for EXIT_SUCCESS

namespace llc
{
	struct SRuntimeState {
		::llc::refcount_t		RenderThreadUsers	= 0; 
		::llc::SRuntimeModule	* MainModule		= 0; 
		bool					RequestedExit		= false; 
	};
} // namespace

static	void			threadRender		(void* pRuntimeState)												{
	::llc::SRuntimeState		& runtimeState		= *(::llc::SRuntimeState*)pRuntimeState;
	::llc::SRuntimeModule		* runtimeModule		= runtimeState.MainModule;
	if_null_re(runtimeModule);
	llc_sync_increment(runtimeState.RenderThreadUsers);
	int32_t						result				= 0;
	while(result = llc_sync_compare_exchange(runtimeState.RenderThreadUsers, 0, 1) - 1 && result != -1) {
		runtimeModule->Render(runtimeModule->Application);
	}
	info_printf("Render thread done.");
}

static	int				threadRenderStart	(::llc::SRuntimeState & runtimeState)										{
	_beginthread(threadRender, 0, &runtimeState);
	return 0;
}

static	int				lrt_Loop			(::llc::SRuntimeState & runtimeState, ::llc::SRuntimeModule & mainModule)	{
//#if defined(LLC_WINDOWS)
//	::llc::error_t				updateResult		= mainModule.Update(mainModule.Application, ::GetAsyncKeyState(VK_ESCAPE) != 0);
//#else
	::llc::error_t				updateResult		= mainModule.Update(mainModule.Application, false);
//#endif
	if(1 == updateResult || ::llc::failed(updateResult)) {
		if_true_i(::llc::APPLICATION_STATE_EXIT == updateResult);
		if_fail_ef(updateResult, "%i", updateResult);
	}
	else {
		llc_sync_increment(runtimeState.RenderThreadUsers);	// Report we're alive
		llc_necall(::threadRenderStart(runtimeState), "%s", "Unknown error");
		info_printf("%s", "Application instance initialized successfully. Executing main loop...");
		while(true) {
//#if defined(LLC_WINDOWS)
//			updateResult			= mainModule.Update(mainModule.Application, ::GetAsyncKeyState(VK_ESCAPE) != 0);
//#else
			updateResult			= mainModule.Update(mainModule.Application, false);
//#endif
			if_true_bi(::llc::APPLICATION_STATE_EXIT == updateResult);
			if_fail_be(updateResult);
			//ef_if(mainModule.Render(app), "Why would this ever happen?");
		}
		llc_sync_decrement(runtimeState.RenderThreadUsers);	// Report we're done
	}
	{
		::llc::STimer				timer;
		double						elapsedTime						= 0;
		while(-1 != llc_sync_compare_exchange(runtimeState.RenderThreadUsers, -1, 0)) {		// Wait until the render thread is closed.
			if(elapsedTime > 5.0f)
				break;
			::llc::sleep(10);
			timer.Frame();
			//elapsedTime			+= timer.LastTimeSeconds;
		}
	}
	return updateResult;
}

static	int					lrt_Main						(::llc::SRuntimeValues& globalRuntimeValues)						{
	globalRuntimeValues;
#if defined(LLC_WINDOWS)
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif
	::llc::SRuntimeState			runtimeState					= {};
	{
		::llc::vcst_t					fileName						= "gme";
		if(globalRuntimeValues.EntryPointArgs.Positionals.size())
			fileName					= globalRuntimeValues.EntryPointArgs.Positionals[0];
		char							mainModuleName	[512]			= {};
		snprintf(mainModuleName, ::llc::size(mainModuleName) - 2, "%s.%s", fileName.begin(), LLC_MODULE_EXTENSION);
		::llc::SRuntimeModule			mainModule						= {};
		llc_necall(llc::loadRuntimeModule(mainModule, mainModuleName), "Failed to create main module. %s.", mainModuleName);
		info_printf("Created main module: '%s'.", mainModuleName);
		void							* app;
		llc_necall(mainModule.Create(&app, &globalRuntimeValues), "Failed to instantiate main module class. %s.", mainModuleName);
		mainModule.Application		= app;
		info_printf("%s", "Initializing application instance.");
		if(::llc::failed(mainModule.Setup(app))) {
			error_printf("%s", "Setup() Failed!");
			if_fail_fef(mainModule.Delete(&app), "Failed to create main module. %s.", mainModuleName);
			return -1;
		}
		runtimeState.MainModule		= &mainModule;
		::lrt_Loop(runtimeState, mainModule);
		info_printf("%s", "Cleaning up application instance...");
		if_fail_fef(mainModule.Cleanup(app), "%s", mainModuleName);
		info_printf("%s", "Application instance destroyed.");
		if_fail_fef(mainModule.Delete(&app), "%s", mainModuleName);
	}
	return 0;
}

LLC_OS_ENTRY_POINT(::lrt_Main);
