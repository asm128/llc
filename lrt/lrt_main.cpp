#include "llc_runtime_module.h"
#include "llc_sync.h"
#include "llc_string_compose.h"

#if defined(LLC_WINDOWS)
#	include <crtdbg.h>
#	include <process.h>
#endif

namespace {

struct SRuntimeState {
	llc::refcount_t		RenderThreadUsers	= 0;
	llc::refcount_t		RenderFinished		= 0;
	llc::SRuntimeModule	* MainModule		= 0;
	llc::err_t			RenderResult		= 0;
};

static unsigned __stdcall threadRender(void * state) {
	SRuntimeState & runtime = *(SRuntimeState*)state;
	llc::SRuntimeModule & module = *runtime.MainModule;
	// GPK's two-user handoff: the update loop releases its user, then render exits.
	while(llc_sync_compare_exchange(runtime.RenderThreadUsers, 0, 1) != 1) {
		runtime.RenderResult = module.Render(module.Application);
		if_fail_be(runtime.RenderResult);
	}
	llc_sync_exchange(runtime.RenderFinished, 1);
	rtrn 0;
}

static llc::err_t lrt_Loop(llc::SRuntimeModule & module) {
	llc::err_t result = module.Update(module.Application, false);
	if(llc::failed(result) || result == llc::APPLICATION_STATE_EXIT || result == llc::APPLICATION_STATE_RESTART)
		rtrn result;

	SRuntimeState runtime;
	runtime.MainModule = &module;
	// Reserve both users before starting: update may finish before the worker runs.
	runtime.RenderThreadUsers = 2;
	HANDLE thread = (HANDLE)_beginthreadex(0, 0, threadRender, &runtime, 0, 0);
	if_null_ve(llc::OS_ERROR, thread);
	info_printf("Application instance initialized successfully. Executing main loop...");
	for(;;) {
		result = module.Update(module.Application, false);
		if_fail_be(result);
		if(result == llc::APPLICATION_STATE_EXIT || result == llc::APPLICATION_STATE_RESTART)
			break;
		if(llc_sync_compare_exchange(runtime.RenderFinished, 0, 0)) {
			result = runtime.RenderResult;
			break;
		}
	}
	llc_sync_decrement(runtime.RenderThreadUsers);
	// Like the selector, exclude all rendering before Cleanup/Delete. A real join also
	// covers the worker's return path before its DLL code can be unmapped.
	cnst DWORD waited = WaitForSingleObject(thread, INFINITE);
	cnst BOOL closed = CloseHandle(thread);
	if_true_ve(llc::OS_ERROR, waited != WAIT_OBJECT_0);
	if_zero_ve(llc::OS_ERROR, closed);
	if_fail_ve(runtime.RenderResult, runtime.RenderResult);
	rtrn result;
}

static llc::err_t lrt_Main(llc::SRuntimeValues & runtime) {
#if defined(LLC_WINDOWS)
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif
	llc::vcst_t fileName = "gme";
	if(runtime.EntryPointArgs.Positionals.size())
		fileName = runtime.EntryPointArgs.Positionals[0];
	llc::string moduleName;
	if_fail_fe(llc::append_strings(moduleName, fileName, '.', LLC_MODULE_EXTENSION));
	for(;;) {
		llc::SRuntimeModule module;
		if_fail_fe(llc::loadRuntimeModule(module, moduleName));
		llc::err_t result = module.Create(&module.Application, &runtime);
		if(llc::failed(result)) {
			if(module.Application) { if_fail_fe(module.Delete(&module.Application)); }
			if_fail_fe(llc::unloadRuntimeModule(module));
			rtrn result;
		}
		result = module.Setup(module.Application);
		if(llc::succeeded(result))
			result = lrt_Loop(module);
		// NWOL selector order: stop callbacks, clean up, delete the instance.
		// A failed cleanup/delete stops recovery; do not unload possibly live code.
		if_fail_fe(module.Cleanup(module.Application));
		if_fail_fe(module.Delete(&module.Application));
		if_fail_fe(llc::unloadRuntimeModule(module));
		if(result != llc::APPLICATION_STATE_RESTART)
			rtrn result;
		info_printf("Reloading module from disk: %s.", moduleName.begin());
	}
}
}

LLC_OS_ENTRY_POINT(lrt_Main);
