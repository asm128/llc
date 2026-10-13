#include "llc_app_impl.h"
#include "llc_array.h"

struct SRuntimeState {
	::llc::refcount_t		DrawThreadUsers	= 0;
	::llc::refcount_t		DrawResult		= 0;
	::llc::SRuntimeModule	* MainModule		= 0;
};

static void threadRender(void* pRuntimeState) {
	::SRuntimeState			& runtimeState	= *(::SRuntimeState*)pRuntimeState;
	::llc::SRuntimeModule	* runtimeModule	= runtimeState.MainModule;
	if(0 == runtimeModule || 0 == runtimeModule->Application) {
		llc_sync_exchange(runtimeState.DrawResult, ::llc::OS_INVALID_PARAMETER);
		rtrn;
	}

	llc_sync_increment(runtimeState.DrawThreadUsers);
	::llc::err_t			result			= 0;
	while(1 < llc_sync_compare_exchange(runtimeState.DrawThreadUsers, 0, 1))
		if_fail_be(result = runtimeModule->Render(runtimeModule->Application));
	if(::llc::failed(result)) {
		llc_sync_exchange(runtimeState.DrawResult, result);
		llc_sync_decrement(runtimeState.DrawThreadUsers);
	}
	info_printf("%s", "Draw thread done.");
}

static ::llc::err_t threadRenderStop(::SRuntimeState & runtimeState) {
	if(0 != llc_sync_compare_exchange(runtimeState.DrawThreadUsers, 0, 0)) {
		llc_sync_decrement(runtimeState.DrawThreadUsers);
		while(-1 != llc_sync_compare_exchange(runtimeState.DrawThreadUsers, -1, 0))
			::llc::sleep(1);
		llc_sync_exchange(runtimeState.DrawThreadUsers, 0);
	}
	rtrn (::llc::err_t)llc_sync_compare_exchange(runtimeState.DrawResult, 0, 0);
}

static ::llc::err_t threadRenderStart(::SRuntimeState & runtimeState) {
	llc_sync_exchange(runtimeState.DrawResult, 0);
	llc_sync_increment(runtimeState.DrawThreadUsers);
	cnst ::llc::err_t resultStart = ::llc::appThreadStart(::threadRender, &runtimeState);
	if(::llc::failed(resultStart)) {
		llc_sync_decrement(runtimeState.DrawThreadUsers);
		rtrn resultStart;
	}
	::llc::err_t resultDraw = 0;
	while(1 == llc_sync_compare_exchange(runtimeState.DrawThreadUsers, 0, 0)
		&& ::llc::not_failed(resultDraw = (::llc::err_t)llc_sync_compare_exchange(runtimeState.DrawResult, 0, 0)))
		::llc::sleep(1);
	rtrn ::llc::failed(resultDraw) ? ::threadRenderStop(runtimeState) : 0;
}

static ::llc::err_t lrtLoop(::SRuntimeState & runtimeState) {
	::llc::SRuntimeModule	& runtimeModule	= *runtimeState.MainModule;
	::llc::err_t			resultUpdate	= runtimeModule.Update(runtimeModule.Application, false);
	if(::llc::APPLICATION_STATE_EXIT != resultUpdate && ::llc::not_failed(resultUpdate)) {
		resultUpdate = ::threadRenderStart(runtimeState);
		if(::llc::not_failed(resultUpdate)) {
			info_printf("%s", "Application instance initialized successfully. Executing main loop...");
			while(true) {
				resultUpdate = runtimeModule.Update(runtimeModule.Application, false);
				if_true_bi(::llc::APPLICATION_STATE_EXIT == resultUpdate);
				if_fail_be(resultUpdate);
				if_fail_be((::llc::err_t)llc_sync_compare_exchange(runtimeState.DrawResult, 0, 0));
			}
		}
	}
	cnst ::llc::err_t resultDraw = ::threadRenderStop(runtimeState);
	rtrn ::llc::failed(resultUpdate) ? resultUpdate : ::llc::failed(resultDraw) ? resultDraw : resultUpdate;
}

static ::llc::err_t lrtModuleRun(::llc::SRuntimeModule & runtimeModule, ::llc::SRuntimeValues & runtimeValues) {
	if_fail_fe(runtimeModule.Create(&runtimeModule.Application, &runtimeValues));
	info_printf("%s", "Initializing application instance.");
	::llc::err_t result = runtimeModule.Setup(runtimeModule.Application);
	if(::llc::not_failed(result)) {
		::SRuntimeState runtimeState = {};
		runtimeState.MainModule = &runtimeModule;
		result = ::lrtLoop(runtimeState);
	}
	else
		error_printf("Setup() failed with result:%i.", result);

	info_printf("%s", "Cleaning up application instance...");
	cnst ::llc::err_t resultCleanup = runtimeModule.Cleanup(runtimeModule.Application);
	if_fail_ef(resultCleanup, "Cleanup() failed with result:%i.", resultCleanup);
	cnst ::llc::err_t resultDelete = runtimeModule.Delete(&runtimeModule.Application);
	if_fail_ef(resultDelete, "Application deletion failed with result:%i.", resultDelete);
	info_printf("%s", "Application instance destroyed.");
	rtrn ::llc::failed(result) ? result : ::llc::failed(resultCleanup) ? resultCleanup : resultDelete;
}

static ::llc::err_t lrtMain(::llc::SRuntimeValues & runtimeValues) {
	if_true_fef(1 != runtimeValues.EntryPointArgs.Positionals.size(), "%s", "Usage: lrt <module-name>.");
	::llc::asc_t moduleName = {};
	if_fail_fe(moduleName.append_string(runtimeValues.EntryPointArgs.Positionals[0]));
	if_fail_fe(moduleName.append_string("." LLC_MODULE_EXTENSION));

	::llc::SRuntimeModule runtimeModule = {};
	cnst ::llc::vcst_t moduleNameView = {moduleName.begin(), moduleName.size()};
	::llc::err_t result = ::llc::loadRuntimeModule(runtimeModule, moduleNameView);
	if_fail_fef(result, "Failed to load runtime module:'%s'.", moduleName.begin());
	info_printf("Loaded runtime module:'%s'.", moduleName.begin());
	result = ::lrtModuleRun(runtimeModule, runtimeValues);
	cnst ::llc::err_t resultUnload = ::llc::unloadRuntimeModule(runtimeModule);
	if_fail_ef(resultUnload, "Failed to unload runtime module:'%s'.", moduleName.begin());
	rtrn ::llc::failed(result) ? result : resultUnload;
}

LLC_SYSTEM_OS_ENTRY_POINT(::lrtMain)
