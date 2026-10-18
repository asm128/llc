//static void appTestPlatformDebugInit();
//#define LLC_PLATFORM_DEBUG_INIT() ::appTestPlatformDebugInit()
//
//#include "llc_test_core.h"
//#include "llc_runtime.h"
//#include "llc_runtime_module.h"
//
//sttc ::llc::err_t appTestPlatformEntry(::llc::SRuntimeValues &);
//#define main		appTestOSMain
//#define WinMain	appTestOSWinMain
//LLC_OS_ENTRY_POINT(::appTestPlatformEntry)
//#undef WinMain
//#undef main
//
//#undef LLC_OS_ENTRY_POINT
//#define LLC_OS_ENTRY_POINT(entryPoint)
//
//#define llc_moduleTitle		appTestSingleModuleTitle
//#define llc_moduleVersion	appTestSingleModuleVersion
//#define llc_moduleCreate	appTestSingleModuleCreate
//#define llc_moduleDelete	appTestSingleModuleDelete
//#define llc_moduleSetup		appTestSingleModuleSetup
//#define llc_moduleCleanup	appTestSingleModuleCleanup
//#define llc_moduleRender	appTestSingleModuleRender
//#define llc_moduleUpdate	appTestSingleModuleUpdate
//#define rtMain				appTestSingleMain
//#include "llc_app_impl.h"
//
//enum APP_TEST_MODE : ::llc::u0_t
//	{ APP_TEST_MODE_NORMAL
//	, APP_TEST_MODE_EARLY_EXIT
//	, APP_TEST_MODE_SETUP_FAILURE
//	, APP_TEST_MODE_UPDATE_FAILURE
//	, APP_TEST_MODE_DRAW_FAILURE
//	, APP_TEST_MODE_CLEANUP_FAILURE
//	, APP_TEST_MODE_DIRECT
//	, APP_TEST_MODE_CONSTRUCTOR_THROW
//	, APP_TEST_MODE_SETUP_THROW
//	, APP_TEST_MODE_UPDATE_THROW
//	, APP_TEST_MODE_DRAW_THROW
//	, APP_TEST_MODE_CLEANUP_THROW
//	};
//
//stxp ::llc::err_t APP_TEST_SETUP_FAILURE	= -101;
//stxp ::llc::err_t APP_TEST_UPDATE_FAILURE	= -102;
//stxp ::llc::err_t APP_TEST_DRAW_FAILURE		= -103;
//stxp ::llc::err_t APP_TEST_CLEANUP_FAILURE	= -104;
//
//struct SAppTestControl {
//	APP_TEST_MODE		Mode				= APP_TEST_MODE_NORMAL;
//	::llc::refcount_t	DebugInitCount		= {};
//	::llc::refcount_t	EntryPointCount		= {};
//	::llc::refcount_t	ConstructCount		= {};
//	::llc::refcount_t	DestructCount		= {};
//	::llc::refcount_t	SetupCount			= {};
//	::llc::refcount_t	UpdateCount			= {};
//	::llc::refcount_t	DrawCount			= {};
//	::llc::refcount_t	CleanupCount		= {};
//	::llc::refcount_t	UpdateActive		= {};
//	::llc::refcount_t	DrawActive			= {};
//	::llc::refcount_t	UpdateObservedDraw	= {};
//	::llc::refcount_t	DrawObservedUpdate	= {};
//	::llc::refcount_t	OverlapObserved		= {};
//	::llc::refcount_t	CleanupDuringDraw	= {};
//	::llc::refcount_t	WaitTimedOut		= {};
//};
//
//sttc SAppTestControl AppTestControl = {};
//
//stin ::llc::refcount_t appTestRead(::llc::refcount_t & value) { rtrn llc_sync_compare_exchange(value, 0, 0); }
//
//sttc void appTestPlatformDebugInit() { llc_sync_increment(AppTestControl.DebugInitCount); }
//
//sttc ::llc::err_t appTestPlatformEntry(::llc::SRuntimeValues &) {
//	llc_sync_increment(AppTestControl.EntryPointCount);
//	rtrn 0;
//}
//
//sttc bool appTestWaitNonzero(::llc::refcount_t & value) {
//	for(::llc::u2_t iWait = 0; iWait < 2000; ++iWait) {
//		if(appTestRead(value))
//			rtrn true;
//		::llc::sleep(1);
//	}
//	llc_sync_exchange(AppTestControl.WaitTimedOut, 1);
//	rtrn false;
//}
//
//sttc void appTestReset(APP_TEST_MODE mode) {
//	AppTestControl = {};
//	AppTestControl.Mode = mode;
//}
//
//struct SAppTestSingle {
//	SAppTestSingle(::llc::SRuntimeValues &) {
//		llc_sync_increment(AppTestControl.ConstructCount);
//		if(APP_TEST_MODE_CONSTRUCTOR_THROW == AppTestControl.Mode)
//			throw 0;
//	}
//	~SAppTestSingle() { llc_sync_increment(AppTestControl.DestructCount); }
//};
//
//struct SAppTestMT {
//	SAppTestMT(::llc::SRuntimeValues &) {
//		llc_sync_increment(AppTestControl.ConstructCount);
//		if(APP_TEST_MODE_CONSTRUCTOR_THROW == AppTestControl.Mode)
//			throw 0;
//	}
//	~SAppTestMT() { llc_sync_increment(AppTestControl.DestructCount); }
//};
//
//tplt<tpnm TApp> sttc ::llc::err_t appTestSetup(TApp &) {
//	llc_sync_increment(AppTestControl.SetupCount);
//	if(APP_TEST_MODE_SETUP_THROW == AppTestControl.Mode)
//		throw 0;
//	rtrn APP_TEST_MODE_SETUP_FAILURE == AppTestControl.Mode ? APP_TEST_SETUP_FAILURE : 0;
//}
//
//tplt<tpnm TApp> sttc ::llc::err_t appTestCleanup(TApp &) {
//	llc_sync_increment(AppTestControl.CleanupCount);
//	if(appTestRead(AppTestControl.DrawActive))
//		llc_sync_exchange(AppTestControl.CleanupDuringDraw, 1);
//	if(APP_TEST_MODE_CLEANUP_THROW == AppTestControl.Mode)
//		throw 0;
//	rtrn APP_TEST_MODE_CLEANUP_FAILURE == AppTestControl.Mode ? APP_TEST_CLEANUP_FAILURE : 0;
//}
//
//sttc ::llc::err_t appTestSingleUpdate() {
//	cnst ::llc::refcount_t updateCount = llc_sync_increment(AppTestControl.UpdateCount);
//	if(APP_TEST_MODE_UPDATE_THROW == AppTestControl.Mode)
//		throw 0;
//	if(APP_TEST_MODE_EARLY_EXIT == AppTestControl.Mode)
//		rtrn ::llc::APPLICATION_STATE_EXIT;
//	if(APP_TEST_MODE_UPDATE_FAILURE == AppTestControl.Mode && 1 < updateCount)
//		rtrn APP_TEST_UPDATE_FAILURE;
//	rtrn 1 < updateCount ? ::llc::APPLICATION_STATE_EXIT : ::llc::APPLICATION_STATE_NORMAL;
//}
//
//sttc ::llc::err_t appTestSingleDraw() {
//	llc_sync_increment(AppTestControl.DrawCount);
//	if(APP_TEST_MODE_DRAW_THROW == AppTestControl.Mode)
//		throw 0;
//	rtrn APP_TEST_MODE_DRAW_FAILURE == AppTestControl.Mode ? APP_TEST_DRAW_FAILURE : 0;
//}
//
//sttc ::llc::err_t appTestMTUpdate() {
//	cnst ::llc::refcount_t updateCount = llc_sync_increment(AppTestControl.UpdateCount);
//	if(APP_TEST_MODE_UPDATE_THROW == AppTestControl.Mode)
//		throw 0;
//	if(APP_TEST_MODE_EARLY_EXIT == AppTestControl.Mode)
//		rtrn ::llc::APPLICATION_STATE_EXIT;
//	if(APP_TEST_MODE_DIRECT == AppTestControl.Mode)
//		rtrn ::llc::APPLICATION_STATE_NORMAL;
//	if(1 == updateCount)
//		rtrn ::llc::APPLICATION_STATE_NORMAL;
//	if(APP_TEST_MODE_DRAW_FAILURE == AppTestControl.Mode && 2 < updateCount)
//		rtrn ::llc::APPLICATION_STATE_NORMAL;
//	llc_sync_exchange(AppTestControl.UpdateActive, 1);
//	if(appTestWaitNonzero(AppTestControl.DrawActive)) {
//		llc_sync_exchange(AppTestControl.UpdateObservedDraw, 1);
//		appTestWaitNonzero(AppTestControl.DrawObservedUpdate);
//		llc_sync_exchange(AppTestControl.OverlapObserved, 1);
//	}
//	llc_sync_exchange(AppTestControl.UpdateActive, 0);
//	if(APP_TEST_MODE_UPDATE_FAILURE == AppTestControl.Mode)
//		rtrn APP_TEST_UPDATE_FAILURE;
//	rtrn APP_TEST_MODE_DRAW_FAILURE == AppTestControl.Mode ? ::llc::APPLICATION_STATE_NORMAL : ::llc::APPLICATION_STATE_EXIT;
//}
//
//sttc ::llc::err_t appTestMTDraw() {
//	cnst ::llc::refcount_t drawCount = llc_sync_increment(AppTestControl.DrawCount);
//	if(APP_TEST_MODE_DRAW_THROW == AppTestControl.Mode)
//		throw 0;
//	if(APP_TEST_MODE_DIRECT == AppTestControl.Mode)
//		rtrn 0;
//	llc_sync_exchange(AppTestControl.DrawActive, 1);
//	if(1 == drawCount && appTestWaitNonzero(AppTestControl.UpdateActive)) {
//		llc_sync_exchange(AppTestControl.DrawObservedUpdate, 1);
//		appTestWaitNonzero(AppTestControl.UpdateObservedDraw);
//		llc_sync_exchange(AppTestControl.OverlapObserved, 1);
//	}
//	llc_sync_exchange(AppTestControl.DrawActive, 0);
//	rtrn APP_TEST_MODE_DRAW_FAILURE == AppTestControl.Mode ? APP_TEST_DRAW_FAILURE : 0;
//}
//
//::llc::err_t setup	(SAppTestSingle & app) { rtrn appTestSetup(app); }
//::llc::err_t cleanup	(SAppTestSingle & app) { rtrn appTestCleanup(app); }
//::llc::err_t update	(SAppTestSingle &, bool) { rtrn appTestSingleUpdate(); }
//::llc::err_t draw	(SAppTestSingle &) { rtrn appTestSingleDraw(); }
//
//LLC_DEFINE_APPLICATION_ENTRY_POINT(SAppTestSingle, "LLC application test");
//
//#undef llc_moduleTitle
//#undef llc_moduleVersion
//#undef llc_moduleCreate
//#undef llc_moduleDelete
//#undef llc_moduleSetup
//#undef llc_moduleCleanup
//#undef llc_moduleRender
//#undef llc_moduleUpdate
//#undef rtMain
//
//#define llc_moduleTitle		appTestMTModuleTitle
//#define llc_moduleVersion	appTestMTModuleVersion
//#define llc_moduleCreate	appTestMTModuleCreate
//#define llc_moduleDelete	appTestMTModuleDelete
//#define llc_moduleSetup		appTestMTModuleSetup
//#define llc_moduleCleanup	appTestMTModuleCleanup
//#define llc_moduleRender	appTestMTModuleRender
//#define llc_moduleUpdate	appTestMTModuleUpdate
//#define rtMain				appTestMTMain
//#define SRuntimeState		SAppTestRuntimeState
//#define threadRender		appTestThreadRender
//#define threadRenderStop	appTestThreadRenderStop
//#define threadRenderStart	appTestThreadRenderStart
//#define lrt_Loop			appTestLoop
//
//::llc::err_t setup	(SAppTestMT & app) { rtrn appTestSetup(app); }
//::llc::err_t cleanup	(SAppTestMT & app) { rtrn appTestCleanup(app); }
//::llc::err_t update	(SAppTestMT &, bool) { rtrn appTestMTUpdate(); }
//::llc::err_t draw	(SAppTestMT &) { rtrn appTestMTDraw(); }
//
//LLC_DEFINE_APPLICATION_ENTRY_POINT_MT(SAppTestMT, "LLC application test");
//
//#undef llc_moduleTitle
//#undef llc_moduleVersion
//#undef llc_moduleCreate
//#undef llc_moduleDelete
//#undef llc_moduleSetup
//#undef llc_moduleCleanup
//#undef llc_moduleRender
//#undef llc_moduleUpdate
//#undef rtMain
//#undef SRuntimeState
//#undef threadRender
//#undef threadRenderStop
//#undef threadRenderStart
//#undef lrt_Loop
//
//GDEFINE_ENUM_TYPE(APP_TEST_RESULT, ::llc::u0_t);
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_VERSION_NULL			,  1, "Module version accepted a null output pointer.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_VERSION_RESULT			,  2, "Module version rejected a valid output pointer.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_VERSION_VALUE			,  3, "Module version returned the wrong value.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_TITLE_LENGTH_NULL		,  4, "Module title accepted a null length pointer.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_TITLE_QUERY_RESULT		,  5, "Module title rejected a size query.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_TITLE_QUERY_LENGTH		,  6, "Module title returned the wrong required length.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_TITLE_SMALL_RESULT		,  7, "Module title accepted an undersized buffer.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_TITLE_SMALL_LENGTH		,  8, "Module title did not report the required length after an undersized buffer.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_TITLE_SMALL_OUTPUT		,  9, "Module title modified an undersized buffer.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_TITLE_COPY_RESULT		, 10, "Module title rejected an exactly-sized buffer.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_TITLE_COPY_LENGTH		, 11, "Module title changed the required length after a successful copy.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_TITLE_COPY_CONTENT		, 12, "Module title copied the wrong text.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_CREATE_INSTANCE_NULL	, 13, "Module creation accepted a null instance output pointer.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_CREATE_RUNTIME_NULL		, 14, "Module creation accepted a null runtime pointer.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_CREATE_THROW_RESULT		, 15, "Module creation did not contain a constructor exception.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_CREATE_THROW_OUTPUT		, 16, "Module creation produced an instance after a constructor exception.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_CREATE_RESULT			, 17, "Module creation rejected valid inputs.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_CREATE_OUTPUT			, 18, "Module creation did not produce an instance.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_CREATE_COUNT			, 19, "Module creation constructed the wrong number of instances.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_SETUP_NULL				, 20, "Module setup accepted a null instance.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_UPDATE_NULL				, 21, "Module update accepted a null instance.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_RENDER_NULL				, 22, "Module render accepted a null instance.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_CLEANUP_NULL			, 23, "Module cleanup accepted a null instance.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_DELETE_NULL				, 24, "Module delete accepted a null instance pointer.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_SETUP_RESULT			, 25, "Module setup rejected a valid instance.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_UPDATE_RESULT			, 26, "Module update rejected a valid instance.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_RENDER_RESULT			, 27, "Module render rejected a valid instance.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_CLEANUP_RESULT			, 28, "Module cleanup rejected a valid instance.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_SETUP_THROW				, 29, "Module setup did not contain a callback exception.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_UPDATE_THROW			, 30, "Module update did not contain a callback exception.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_RENDER_THROW			, 31, "Module render did not contain a callback exception.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_CLEANUP_THROW			, 32, "Module cleanup did not contain a callback exception.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_DELETE_RESULT			, 33, "Module deletion rejected a valid instance.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_DELETE_OUTPUT			, 34, "Module deletion did not clear the instance pointer.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MODULE_DELETE_COUNT			, 35, "Module deletion destroyed the wrong number of instances.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, SINGLE_NORMAL_RESULT			, 36, "Single-thread runtime rejected a normal lifecycle.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, SINGLE_NORMAL_CONSTRUCT		, 37, "Single-thread runtime constructed the wrong number of instances.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, SINGLE_NORMAL_SETUP			, 38, "Single-thread runtime called setup the wrong number of times.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, SINGLE_NORMAL_UPDATE			, 39, "Single-thread runtime called update the wrong number of times.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, SINGLE_NORMAL_DRAW			, 40, "Single-thread runtime called draw the wrong number of times.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, SINGLE_NORMAL_CLEANUP			, 41, "Single-thread runtime called cleanup the wrong number of times.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, SINGLE_NORMAL_DESTRUCT		, 42, "Single-thread runtime destroyed the wrong number of instances.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, SINGLE_SETUP_RESULT			, 43, "Single-thread runtime did not propagate setup failure.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, SINGLE_SETUP_UPDATE			, 44, "Single-thread runtime updated after setup failure.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, SINGLE_SETUP_DRAW				, 45, "Single-thread runtime drew after setup failure.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, SINGLE_SETUP_CLEANUP			, 46, "Single-thread runtime skipped cleanup after setup failure.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, SINGLE_UPDATE_RESULT			, 47, "Single-thread runtime did not propagate update failure.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, SINGLE_UPDATE_DRAW			, 48, "Single-thread runtime drew after update failure.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, SINGLE_UPDATE_CLEANUP			, 49, "Single-thread runtime skipped cleanup after update failure.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, SINGLE_DRAW_RESULT			, 50, "Single-thread runtime did not propagate draw failure.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, SINGLE_DRAW_CLEANUP			, 51, "Single-thread runtime skipped cleanup after draw failure.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, SINGLE_CLEANUP_RESULT			, 52, "Single-thread runtime did not propagate cleanup failure.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_NORMAL_RESULT				, 53, "Multithread runtime rejected a normal lifecycle.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_NORMAL_CONSTRUCT			, 54, "Multithread runtime constructed the wrong number of instances.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_NORMAL_SETUP				, 55, "Multithread runtime called setup the wrong number of times.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_NORMAL_UPDATE				, 56, "Multithread runtime did not complete both update phases.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_NORMAL_DRAW				, 57, "Multithread runtime did not execute draw.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_NORMAL_OVERLAP			, 58, "Multithread runtime did not overlap update and draw.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_NORMAL_TIMEOUT			, 59, "Multithread runtime stalled while establishing update/draw overlap.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_NORMAL_CLEANUP			, 60, "Multithread runtime called cleanup the wrong number of times.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_NORMAL_CLEANUP_OVERLAP	, 61, "Multithread runtime cleaned up while draw was active.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_NORMAL_DESTRUCT			, 62, "Multithread runtime destroyed the wrong number of instances.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_EARLY_RESULT				, 63, "Multithread runtime rejected an early application exit.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_EARLY_UPDATE				, 64, "Multithread runtime called update after an early exit.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_EARLY_DRAW				, 65, "Multithread runtime started draw after an early exit.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_EARLY_CLEANUP			, 66, "Multithread runtime skipped cleanup after an early exit.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_SETUP_RESULT				, 67, "Multithread runtime did not propagate setup failure.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_SETUP_UPDATE				, 68, "Multithread runtime updated after setup failure.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_SETUP_DRAW				, 69, "Multithread runtime drew after setup failure.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_SETUP_CLEANUP			, 70, "Multithread runtime skipped cleanup after setup failure.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_UPDATE_RESULT			, 71, "Multithread runtime did not propagate update failure.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_UPDATE_DRAW				, 72, "Multithread runtime did not draw before the update failure.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_UPDATE_CLEANUP			, 73, "Multithread runtime skipped cleanup after update failure.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_UPDATE_CLEANUP_OVERLAP	, 74, "Multithread runtime cleaned up while draw was active after update failure.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_DRAW_RESULT				, 75, "Multithread runtime did not propagate draw failure.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_DRAW_COUNT				, 76, "Multithread runtime did not execute the failing draw.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_DRAW_CLEANUP				, 77, "Multithread runtime skipped cleanup after draw failure.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_DRAW_CLEANUP_OVERLAP		, 78, "Multithread runtime cleaned up while the failing draw was active.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_CLEANUP_RESULT			, 79, "Multithread runtime did not propagate cleanup failure.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_CLEANUP_OVERLAP			, 80, "Multithread runtime cleaned up before draw stopped.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, ENTRY_RESULT				, 81, "Platform entry point rejected a successful application entry function.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, ENTRY_DEBUG_INIT			, 82, "Platform entry point did not invoke the overridden debug initialization hook exactly once.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, ENTRY_CALL					, 83, "Platform entry point did not invoke the application entry function exactly once.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_EARLY_TIMEOUT			, 84, "Multithread runtime stalled during early exit.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_SETUP_TIMEOUT			, 85, "Multithread runtime stalled after setup failure.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_UPDATE_TIMEOUT			, 86, "Multithread runtime stalled after update failure.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_DRAW_TIMEOUT				, 87, "Multithread runtime stalled after draw failure.");
//GDEFINE_ENUM_VALUED(APP_TEST_RESULT, MT_CLEANUP_TIMEOUT			, 88, "Multithread runtime stalled before cleanup failure.");
//
//sttc ::llc::err_t appTestRunSilenced(::llc::err_t (*runner)(::llc::SRuntimeValues &), ::llc::SRuntimeValues & runtimeValues) {
//	::llc::setupLogCallbacks(0, 0);
//	cnst ::llc::err_t result = runner(runtimeValues);
//	::llc::setupDefaultLogCallbacks();
//	rtrn result;
//}
//
//sttc ::llc::err_t testAppEntryPoint(ATestError & errors) {
//	appTestReset(APP_TEST_MODE_DIRECT);
//	::llc::sc_t executable[] = "llc_test_core";
//	::llc::sc_t* arguments[] = {executable, 0};
//	::llc::sc_t* environment[] = {0};
//	cnst int result = appTestOSMain(1, arguments, environment);
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_ENTRY_RESULT, result, "Result:%i, expected:0.", result);
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_ENTRY_DEBUG_INIT, 1 != appTestRead(AppTestControl.DebugInitCount), "Count:%" LLC_FMT_S3 ", expected:1.", (::llc::s3_t)appTestRead(AppTestControl.DebugInitCount));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_ENTRY_CALL, 1 != appTestRead(AppTestControl.EntryPointCount), "Count:%" LLC_FMT_S3 ", expected:1.", (::llc::s3_t)appTestRead(AppTestControl.EntryPointCount));
//	rtrn 0;
//}
//
//sttc ::llc::err_t testAppModule(ATestError & errors, ::llc::SRuntimeValues & runtimeValues) {
//	::llc::u2_t version = {};
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_VERSION_NULL, ::llc::OS_INVALID_PARAMETER != appTestMTModuleVersion(0));
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_VERSION_RESULT, ::llc::failed(appTestMTModuleVersion(&version)));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MODULE_VERSION_VALUE, 1 != version, "Version:%u, expected:1.", version);
//
//	stxp ::llc::sc_t moduleTitle[] = "LLC application test";
//	::llc::u0_t titleLength = {};
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_TITLE_LENGTH_NULL, ::llc::OS_INVALID_PARAMETER != appTestMTModuleTitle(0, 0));
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_TITLE_QUERY_RESULT, ::llc::failed(appTestMTModuleTitle(0, &titleLength)));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MODULE_TITLE_QUERY_LENGTH, ::llc::size(moduleTitle) != titleLength, "Length:%u, expected:%u.", titleLength, ::llc::size(moduleTitle));
//
//	::llc::sc_t output[::llc::size(moduleTitle)] = {};
//	output[0] = '#';
//	::llc::u0_t smallLength = (::llc::u0_t)(::llc::size(moduleTitle) - 1);
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_TITLE_SMALL_RESULT, ::llc::OS_OVERRUN != appTestMTModuleTitle(output, &smallLength));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MODULE_TITLE_SMALL_LENGTH, ::llc::size(moduleTitle) != smallLength, "Length:%u, expected:%u.", smallLength, ::llc::size(moduleTitle));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MODULE_TITLE_SMALL_OUTPUT, '#' != output[0], "First byte:0x%X, expected:0x%X.", output[0], '#');
//
//	::llc::u0_t exactLength = (::llc::u0_t)::llc::size(moduleTitle);
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_TITLE_COPY_RESULT, ::llc::failed(appTestMTModuleTitle(output, &exactLength)));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MODULE_TITLE_COPY_LENGTH, ::llc::size(moduleTitle) != exactLength, "Length:%u, expected:%u.", exactLength, ::llc::size(moduleTitle));
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_TITLE_COPY_CONTENT, 0 != ::memcmp(output, moduleTitle, ::llc::size(moduleTitle)));
//
//	void* instance = 0;
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_CREATE_INSTANCE_NULL, ::llc::OS_INVALID_PARAMETER != appTestMTModuleCreate(0, &runtimeValues));
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_CREATE_RUNTIME_NULL, ::llc::OS_INVALID_PARAMETER != appTestMTModuleCreate(&instance, 0));
//	appTestReset(APP_TEST_MODE_CONSTRUCTOR_THROW);
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_CREATE_THROW_RESULT, ::llc::OS_NO_MEMORY != appTestMTModuleCreate(&instance, &runtimeValues));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MODULE_CREATE_THROW_OUTPUT, 0 != instance, "Instance:%p, expected:null.", instance);
//
//	appTestReset(APP_TEST_MODE_DIRECT);
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_CREATE_RESULT, ::llc::failed(appTestMTModuleCreate(&instance, &runtimeValues)));
//	LLC_TEST_REQUIRE(errors, APP_TEST_RESULT_MODULE_CREATE_OUTPUT, 0 == instance);
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MODULE_CREATE_COUNT, 1 != appTestRead(AppTestControl.ConstructCount), "Count:%" LLC_FMT_S3 ", expected:1.", (::llc::s3_t)appTestRead(AppTestControl.ConstructCount));
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_SETUP_NULL, ::llc::OS_INVALID_PARAMETER != appTestMTModuleSetup(0));
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_UPDATE_NULL, ::llc::OS_INVALID_PARAMETER != appTestMTModuleUpdate(0, false));
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_RENDER_NULL, ::llc::OS_INVALID_PARAMETER != appTestMTModuleRender(0));
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_CLEANUP_NULL, ::llc::OS_INVALID_PARAMETER != appTestMTModuleCleanup(0));
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_DELETE_NULL, ::llc::OS_INVALID_PARAMETER != appTestMTModuleDelete(0));
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_SETUP_RESULT, ::llc::failed(appTestMTModuleSetup(instance)));
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_UPDATE_RESULT, ::llc::failed(appTestMTModuleUpdate(instance, false)));
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_RENDER_RESULT, ::llc::failed(appTestMTModuleRender(instance)));
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_CLEANUP_RESULT, ::llc::failed(appTestMTModuleCleanup(instance)));
//
//	AppTestControl.Mode = APP_TEST_MODE_SETUP_THROW;
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_SETUP_THROW, ::llc::OS_ERROR != appTestMTModuleSetup(instance));
//	AppTestControl.Mode = APP_TEST_MODE_UPDATE_THROW;
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_UPDATE_THROW, ::llc::OS_ERROR != appTestMTModuleUpdate(instance, false));
//	AppTestControl.Mode = APP_TEST_MODE_DRAW_THROW;
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_RENDER_THROW, ::llc::OS_ERROR != appTestMTModuleRender(instance));
//	AppTestControl.Mode = APP_TEST_MODE_CLEANUP_THROW;
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_CLEANUP_THROW, ::llc::OS_ERROR != appTestMTModuleCleanup(instance));
//	AppTestControl.Mode = APP_TEST_MODE_DIRECT;
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MODULE_DELETE_RESULT, ::llc::failed(appTestMTModuleDelete(&instance)));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MODULE_DELETE_OUTPUT, 0 != instance, "Instance:%p, expected:null.", instance);
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MODULE_DELETE_COUNT, 1 != appTestRead(AppTestControl.DestructCount), "Count:%" LLC_FMT_S3 ", expected:1.", (::llc::s3_t)appTestRead(AppTestControl.DestructCount));
//	rtrn 0;
//}
//
//sttc ::llc::err_t testAppSingleThread(ATestError & errors, ::llc::SRuntimeValues & runtimeValues) {
//	appTestReset(APP_TEST_MODE_NORMAL);
//	::llc::err_t result = appTestRunSilenced(appTestSingleMain, runtimeValues);
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_SINGLE_NORMAL_RESULT, ::llc::failed(result), "Result:%i.", result);
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_SINGLE_NORMAL_CONSTRUCT, 1 != appTestRead(AppTestControl.ConstructCount), "Count:%" LLC_FMT_S3 ", expected:1.", (::llc::s3_t)appTestRead(AppTestControl.ConstructCount));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_SINGLE_NORMAL_SETUP, 1 != appTestRead(AppTestControl.SetupCount), "Count:%" LLC_FMT_S3 ", expected:1.", (::llc::s3_t)appTestRead(AppTestControl.SetupCount));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_SINGLE_NORMAL_UPDATE, 2 != appTestRead(AppTestControl.UpdateCount), "Count:%" LLC_FMT_S3 ", expected:2.", (::llc::s3_t)appTestRead(AppTestControl.UpdateCount));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_SINGLE_NORMAL_DRAW, 1 != appTestRead(AppTestControl.DrawCount), "Count:%" LLC_FMT_S3 ", expected:1.", (::llc::s3_t)appTestRead(AppTestControl.DrawCount));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_SINGLE_NORMAL_CLEANUP, 1 != appTestRead(AppTestControl.CleanupCount), "Count:%" LLC_FMT_S3 ", expected:1.", (::llc::s3_t)appTestRead(AppTestControl.CleanupCount));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_SINGLE_NORMAL_DESTRUCT, 1 != appTestRead(AppTestControl.DestructCount), "Count:%" LLC_FMT_S3 ", expected:1.", (::llc::s3_t)appTestRead(AppTestControl.DestructCount));
//
//	appTestReset(APP_TEST_MODE_SETUP_FAILURE);
//	result = appTestRunSilenced(appTestSingleMain, runtimeValues);
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_SINGLE_SETUP_RESULT, APP_TEST_SETUP_FAILURE != result, "Result:%i, expected:%i.", result, APP_TEST_SETUP_FAILURE);
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_SINGLE_SETUP_UPDATE, appTestRead(AppTestControl.UpdateCount), "Count:%" LLC_FMT_S3 ", expected:0.", (::llc::s3_t)appTestRead(AppTestControl.UpdateCount));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_SINGLE_SETUP_DRAW, appTestRead(AppTestControl.DrawCount), "Count:%" LLC_FMT_S3 ", expected:0.", (::llc::s3_t)appTestRead(AppTestControl.DrawCount));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_SINGLE_SETUP_CLEANUP, 1 != appTestRead(AppTestControl.CleanupCount), "Count:%" LLC_FMT_S3 ", expected:1.", (::llc::s3_t)appTestRead(AppTestControl.CleanupCount));
//
//	appTestReset(APP_TEST_MODE_UPDATE_FAILURE);
//	result = appTestRunSilenced(appTestSingleMain, runtimeValues);
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_SINGLE_UPDATE_RESULT, APP_TEST_UPDATE_FAILURE != result, "Result:%i, expected:%i.", result, APP_TEST_UPDATE_FAILURE);
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_SINGLE_UPDATE_DRAW, 1 != appTestRead(AppTestControl.DrawCount), "Count:%" LLC_FMT_S3 ", expected:1.", (::llc::s3_t)appTestRead(AppTestControl.DrawCount));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_SINGLE_UPDATE_CLEANUP, 1 != appTestRead(AppTestControl.CleanupCount), "Count:%" LLC_FMT_S3 ", expected:1.", (::llc::s3_t)appTestRead(AppTestControl.CleanupCount));
//
//	appTestReset(APP_TEST_MODE_DRAW_FAILURE);
//	result = appTestRunSilenced(appTestSingleMain, runtimeValues);
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_SINGLE_DRAW_RESULT, APP_TEST_DRAW_FAILURE != result, "Result:%i, expected:%i.", result, APP_TEST_DRAW_FAILURE);
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_SINGLE_DRAW_CLEANUP, 1 != appTestRead(AppTestControl.CleanupCount), "Count:%" LLC_FMT_S3 ", expected:1.", (::llc::s3_t)appTestRead(AppTestControl.CleanupCount));
//
//	appTestReset(APP_TEST_MODE_CLEANUP_FAILURE);
//	result = appTestRunSilenced(appTestSingleMain, runtimeValues);
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_SINGLE_CLEANUP_RESULT, APP_TEST_CLEANUP_FAILURE != result, "Result:%i, expected:%i.", result, APP_TEST_CLEANUP_FAILURE);
//	rtrn 0;
//}
//
//sttc ::llc::err_t testAppMultiThread(ATestError & errors, ::llc::SRuntimeValues & runtimeValues) {
//	for(::llc::u0_t iRun = 0; iRun < 64; ++iRun) {
//		appTestReset(APP_TEST_MODE_NORMAL);
//		cnst ::llc::err_t result = appTestRunSilenced(appTestMTMain, runtimeValues);
//		LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_NORMAL_RESULT, ::llc::failed(result), "Run:%u, result:%i.", iRun, result);
//		LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_NORMAL_CONSTRUCT, 1 != appTestRead(AppTestControl.ConstructCount), "Run:%u, count:%" LLC_FMT_S3 ", expected:1.", iRun, (::llc::s3_t)appTestRead(AppTestControl.ConstructCount));
//		LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_NORMAL_SETUP, 1 != appTestRead(AppTestControl.SetupCount), "Run:%u, count:%" LLC_FMT_S3 ", expected:1.", iRun, (::llc::s3_t)appTestRead(AppTestControl.SetupCount));
//		LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_NORMAL_UPDATE, 2 != appTestRead(AppTestControl.UpdateCount), "Run:%u, count:%" LLC_FMT_S3 ", expected:2.", iRun, (::llc::s3_t)appTestRead(AppTestControl.UpdateCount));
//		LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_NORMAL_DRAW, 1 > appTestRead(AppTestControl.DrawCount), "Run:%u, count:%" LLC_FMT_S3 ", expected at least:1.", iRun, (::llc::s3_t)appTestRead(AppTestControl.DrawCount));
//		LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_NORMAL_OVERLAP, 0 == appTestRead(AppTestControl.OverlapObserved), "Run:%u.", iRun);
//		LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_NORMAL_TIMEOUT, appTestRead(AppTestControl.WaitTimedOut), "Run:%u.", iRun);
//		LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_NORMAL_CLEANUP, 1 != appTestRead(AppTestControl.CleanupCount), "Run:%u, count:%" LLC_FMT_S3 ", expected:1.", iRun, (::llc::s3_t)appTestRead(AppTestControl.CleanupCount));
//		LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_NORMAL_CLEANUP_OVERLAP, appTestRead(AppTestControl.CleanupDuringDraw), "Run:%u.", iRun);
//		LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_NORMAL_DESTRUCT, 1 != appTestRead(AppTestControl.DestructCount), "Run:%u, count:%" LLC_FMT_S3 ", expected:1.", iRun, (::llc::s3_t)appTestRead(AppTestControl.DestructCount));
//	}
//
//	appTestReset(APP_TEST_MODE_EARLY_EXIT);
//	::llc::err_t result = appTestRunSilenced(appTestMTMain, runtimeValues);
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_EARLY_RESULT, ::llc::failed(result), "Result:%i.", result);
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_EARLY_UPDATE, 1 != appTestRead(AppTestControl.UpdateCount), "Count:%" LLC_FMT_S3 ", expected:1.", (::llc::s3_t)appTestRead(AppTestControl.UpdateCount));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_EARLY_DRAW, appTestRead(AppTestControl.DrawCount), "Count:%" LLC_FMT_S3 ", expected:0.", (::llc::s3_t)appTestRead(AppTestControl.DrawCount));
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MT_EARLY_TIMEOUT, appTestRead(AppTestControl.WaitTimedOut));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_EARLY_CLEANUP, 1 != appTestRead(AppTestControl.CleanupCount), "Count:%" LLC_FMT_S3 ", expected:1.", (::llc::s3_t)appTestRead(AppTestControl.CleanupCount));
//
//	appTestReset(APP_TEST_MODE_SETUP_FAILURE);
//	result = appTestRunSilenced(appTestMTMain, runtimeValues);
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_SETUP_RESULT, APP_TEST_SETUP_FAILURE != result, "Result:%i, expected:%i.", result, APP_TEST_SETUP_FAILURE);
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_SETUP_UPDATE, appTestRead(AppTestControl.UpdateCount), "Count:%" LLC_FMT_S3 ", expected:0.", (::llc::s3_t)appTestRead(AppTestControl.UpdateCount));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_SETUP_DRAW, appTestRead(AppTestControl.DrawCount), "Count:%" LLC_FMT_S3 ", expected:0.", (::llc::s3_t)appTestRead(AppTestControl.DrawCount));
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MT_SETUP_TIMEOUT, appTestRead(AppTestControl.WaitTimedOut));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_SETUP_CLEANUP, 1 != appTestRead(AppTestControl.CleanupCount), "Count:%" LLC_FMT_S3 ", expected:1.", (::llc::s3_t)appTestRead(AppTestControl.CleanupCount));
//
//	appTestReset(APP_TEST_MODE_UPDATE_FAILURE);
//	result = appTestRunSilenced(appTestMTMain, runtimeValues);
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_UPDATE_RESULT, APP_TEST_UPDATE_FAILURE != result, "Result:%i, expected:%i.", result, APP_TEST_UPDATE_FAILURE);
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_UPDATE_DRAW, 1 > appTestRead(AppTestControl.DrawCount), "Count:%" LLC_FMT_S3 ", expected at least:1.", (::llc::s3_t)appTestRead(AppTestControl.DrawCount));
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MT_UPDATE_TIMEOUT, appTestRead(AppTestControl.WaitTimedOut));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_UPDATE_CLEANUP, 1 != appTestRead(AppTestControl.CleanupCount), "Count:%" LLC_FMT_S3 ", expected:1.", (::llc::s3_t)appTestRead(AppTestControl.CleanupCount));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_UPDATE_CLEANUP_OVERLAP, appTestRead(AppTestControl.CleanupDuringDraw), "Active:%" LLC_FMT_S3 ".", (::llc::s3_t)appTestRead(AppTestControl.DrawActive));
//
//	appTestReset(APP_TEST_MODE_DRAW_FAILURE);
//	result = appTestRunSilenced(appTestMTMain, runtimeValues);
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_DRAW_RESULT, APP_TEST_DRAW_FAILURE != result, "Result:%i, expected:%i.", result, APP_TEST_DRAW_FAILURE);
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_DRAW_COUNT, 1 > appTestRead(AppTestControl.DrawCount), "Count:%" LLC_FMT_S3 ", expected at least:1.", (::llc::s3_t)appTestRead(AppTestControl.DrawCount));
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MT_DRAW_TIMEOUT, appTestRead(AppTestControl.WaitTimedOut));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_DRAW_CLEANUP, 1 != appTestRead(AppTestControl.CleanupCount), "Count:%" LLC_FMT_S3 ", expected:1.", (::llc::s3_t)appTestRead(AppTestControl.CleanupCount));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_DRAW_CLEANUP_OVERLAP, appTestRead(AppTestControl.CleanupDuringDraw), "Active:%" LLC_FMT_S3 ".", (::llc::s3_t)appTestRead(AppTestControl.DrawActive));
//
//	appTestReset(APP_TEST_MODE_CLEANUP_FAILURE);
//	result = appTestRunSilenced(appTestMTMain, runtimeValues);
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_CLEANUP_RESULT, APP_TEST_CLEANUP_FAILURE != result, "Result:%i, expected:%i.", result, APP_TEST_CLEANUP_FAILURE);
//	LLC_TEST_CHECK(errors, APP_TEST_RESULT_MT_CLEANUP_TIMEOUT, appTestRead(AppTestControl.WaitTimedOut));
//	LLC_TEST_CHECKF(errors, APP_TEST_RESULT_MT_CLEANUP_OVERLAP, appTestRead(AppTestControl.CleanupDuringDraw), "Active:%" LLC_FMT_S3 ".", (::llc::s3_t)appTestRead(AppTestControl.DrawActive));
//	rtrn 0;
//}
//
//::llc::err_t testApp(ATestError & errors) {
//	::llc::SRuntimeValues runtimeValues = {};
//	if_fail_fe(testAppEntryPoint(errors));
//	if_fail_fe(testAppModule(errors, runtimeValues));
//	if_fail_fe(testAppSingleThread(errors, runtimeValues));
//	rtrn testAppMultiThread(errors, runtimeValues);
//}
