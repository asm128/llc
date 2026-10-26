#include "llc_test_core_suites.h"

#if __has_include("llc_app_impl.h")
#	define LLC_TEST_HAS_APP_IMPL_H 1
#else
#	define LLC_TEST_HAS_APP_IMPL_H 0
#endif
#if __has_include("llc_module.h")
#	define LLC_TEST_HAS_MODULE_H 1
#else
#	define LLC_TEST_HAS_MODULE_H 0
#endif
#if __has_include("llc_runtime_module.h")
#	define LLC_TEST_HAS_RUNTIME_MODULE_H 1
#else
#	define LLC_TEST_HAS_RUNTIME_MODULE_H 0
#endif
#if __has_include("llc_ptr_obj.h")
#	define LLC_TEST_HAS_PTR_OBJ_H 1
#else
#	define LLC_TEST_HAS_PTR_OBJ_H 0
#endif
#if __has_include("llc_process.h")
#	define LLC_TEST_HAS_PROCESS_H 1
#else
#	define LLC_TEST_HAS_PROCESS_H 0
#endif
#if __has_include("llc_timer.h")
#	define LLC_TEST_HAS_TIMER_H 1
#else
#	define LLC_TEST_HAS_TIMER_H 0
#endif

#if LLC_TEST_HAS_APP_IMPL_H && LLC_TEST_HAS_MODULE_H && LLC_TEST_HAS_RUNTIME_MODULE_H && LLC_TEST_HAS_PTR_OBJ_H && LLC_TEST_HAS_PROCESS_H && LLC_TEST_HAS_TIMER_H
#	define LLC_AVOID_LOCAL_APPLICATION_MODEL_EXECUTABLE_RUNTIME
#	include "llc_app_impl.h"
#	define LLC_TEST_APP_IMPL_READY 1
#else
#	define LLC_TEST_APP_IMPL_READY 0
#endif

#if LLC_TEST_APP_IMPL_READY && defined(LLC_DEFINE_APPLICATION_ENTRY_POINT) && defined(LLC_DEFINE_APPLICATION_ENTRY_POINT_MT)
#	define LLC_TEST_HAS_APP_IMPL_MACROS 1
#else
#	define LLC_TEST_HAS_APP_IMPL_MACROS 0
#endif

GDEFINE_ENUM_TYPE(TEST_APP_IMPL_H_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_APP_IMPL_H_RESULT, HEADER_AVAILABLE      ,  0, "The canonical LLC application implementation header is available.");
GDEFINE_ENUM_VALUED(TEST_APP_IMPL_H_RESULT, MODULE_DEPENDENCY     ,  1, "The application implementation module dependency is available.");
GDEFINE_ENUM_VALUED(TEST_APP_IMPL_H_RESULT, RUNTIME_DEPENDENCY    ,  2, "The application implementation runtime-module dependency is available.");
GDEFINE_ENUM_VALUED(TEST_APP_IMPL_H_RESULT, POINTER_DEPENDENCY    ,  3, "The application implementation object-pointer dependency is available.");
GDEFINE_ENUM_VALUED(TEST_APP_IMPL_H_RESULT, PROCESS_DEPENDENCY    ,  4, "The application implementation process dependency is available.");
GDEFINE_ENUM_VALUED(TEST_APP_IMPL_H_RESULT, TIMER_DEPENDENCY      ,  5, "The application implementation timer dependency is available.");
GDEFINE_ENUM_VALUED(TEST_APP_IMPL_H_RESULT, ENTRY_POINT_MACROS    ,  6, "Both application entry-point macro families are exposed.");
GDEFINE_ENUM_VALUED(TEST_APP_IMPL_H_RESULT, MODULE_TITLE          ,  7, "The generated module interface reports and copies its exact title.");
GDEFINE_ENUM_VALUED(TEST_APP_IMPL_H_RESULT, CREATE_DELETE         ,  8, "The generated module interface constructs and destroys one application instance.");
GDEFINE_ENUM_VALUED(TEST_APP_IMPL_H_RESULT, CALLBACK_ORDER        ,  9, "Setup, update, draw and cleanup callbacks retain lifecycle order.");
GDEFINE_ENUM_VALUED(TEST_APP_IMPL_H_RESULT, REQUESTED_EXIT        , 10, "The generated update wrapper forwards the requested-exit flag.");
GDEFINE_ENUM_VALUED(TEST_APP_IMPL_H_RESULT, RESULT_FORWARDING     , 11, "Generated callback wrappers preserve callback result codes.");
GDEFINE_ENUM_VALUED(TEST_APP_IMPL_H_RESULT, EXCEPTION_TRANSLATION , 12, "Generated callback wrappers translate callback exceptions to failure.");
GDEFINE_ENUM_VALUED(TEST_APP_IMPL_H_RESULT, CONSTRUCTOR_EXCEPTION , 13, "The generated create wrapper translates constructor exceptions to failure.");

sttc ::llc::err_t testAppImplHAvailability(::llc::ATestError & results) {
	LLC_TEST_CHECK(results, TEST_APP_IMPL_H_RESULT_HEADER_AVAILABLE  , 0 == LLC_TEST_HAS_APP_IMPL_H);
	LLC_TEST_CHECK(results, TEST_APP_IMPL_H_RESULT_MODULE_DEPENDENCY , 0 == LLC_TEST_HAS_MODULE_H);
	LLC_TEST_CHECK(results, TEST_APP_IMPL_H_RESULT_RUNTIME_DEPENDENCY, 0 == LLC_TEST_HAS_RUNTIME_MODULE_H);
	LLC_TEST_CHECK(results, TEST_APP_IMPL_H_RESULT_POINTER_DEPENDENCY, 0 == LLC_TEST_HAS_PTR_OBJ_H);
	LLC_TEST_CHECK(results, TEST_APP_IMPL_H_RESULT_PROCESS_DEPENDENCY, 0 == LLC_TEST_HAS_PROCESS_H);
	LLC_TEST_CHECK(results, TEST_APP_IMPL_H_RESULT_TIMER_DEPENDENCY  , 0 == LLC_TEST_HAS_TIMER_H);
	LLC_TEST_CHECK(results, TEST_APP_IMPL_H_RESULT_ENTRY_POINT_MACROS, 0 == LLC_TEST_HAS_APP_IMPL_MACROS);
	rtrn 0;
}

#if LLC_TEST_APP_IMPL_READY
namespace {
	enum APP_IMPL_EVENT : ::llc::u0_t {
		APP_IMPL_EVENT_CREATE,
		APP_IMPL_EVENT_SETUP,
		APP_IMPL_EVENT_UPDATE,
		APP_IMPL_EVENT_DRAW,
		APP_IMPL_EVENT_CLEANUP,
		APP_IMPL_EVENT_DELETE,
	};

	::llc::au0_t AppImplEvents;
	::llc::err_t AppImplSetupResult = 0;
	::llc::err_t AppImplUpdateResult = 0;
	::llc::err_t AppImplDrawResult = 0;
	::llc::err_t AppImplCleanupResult = 0;
	bool AppImplRequestedExit = false;
	bool AppImplThrowConstructor = false;
	bool AppImplThrowSetup = false;
	bool AppImplThrowUpdate = false;
	bool AppImplThrowDraw = false;
	bool AppImplThrowCleanup = false;

	struct SAppImplProbe {
		SAppImplProbe(::llc::SRuntimeValues &) {
			if(AppImplThrowConstructor)
				throw 1;
			AppImplEvents.push_back(APP_IMPL_EVENT_CREATE);
		}
		~SAppImplProbe() { AppImplEvents.push_back(APP_IMPL_EVENT_DELETE); }
	};

	sttc void appImplReset() {
		AppImplEvents.clear();
		AppImplSetupResult = AppImplUpdateResult = AppImplDrawResult = AppImplCleanupResult = 0;
		AppImplRequestedExit = false;
		AppImplThrowConstructor = AppImplThrowSetup = AppImplThrowUpdate = AppImplThrowDraw = AppImplThrowCleanup = false;
	}

	sttc ::llc::err_t appImplCreate(void * & instance) {
		::llc::SRuntimeValues runtimeValues = {};
		rtrn ::llc_moduleCreate(&instance, &runtimeValues);
	}
}

LLC_DEFINE_APPLICATION_ENTRY_POINT(::SAppImplProbe, "llc_test_app_impl");

::llc::err_t setup(::SAppImplProbe &) {
	::AppImplEvents.push_back(::APP_IMPL_EVENT_SETUP);
	if(::AppImplThrowSetup)
		throw 1;
	rtrn ::AppImplSetupResult;
}

::llc::err_t update(::SAppImplProbe &, bool requestedExit) {
	::AppImplEvents.push_back(::APP_IMPL_EVENT_UPDATE);
	::AppImplRequestedExit = requestedExit;
	if(::AppImplThrowUpdate)
		throw 1;
	rtrn ::AppImplUpdateResult;
}

::llc::err_t draw(::SAppImplProbe &) {
	::AppImplEvents.push_back(::APP_IMPL_EVENT_DRAW);
	if(::AppImplThrowDraw)
		throw 1;
	rtrn ::AppImplDrawResult;
}

::llc::err_t cleanup(::SAppImplProbe &) {
	::AppImplEvents.push_back(::APP_IMPL_EVENT_CLEANUP);
	if(::AppImplThrowCleanup)
		throw 1;
	rtrn ::AppImplCleanupResult;
}

sttc ::llc::err_t testAppImplHModuleTitle(::llc::ATestError & results) {
	::llc::u2_t count = 0;
	LLC_TEST_REQUIRE(results, TEST_APP_IMPL_H_RESULT_MODULE_TITLE, 0 >= ::llc_moduleTitle(0, &count));
	stxp ::llc::sc_c expected[] = "llc_test_app_impl";
	LLC_TEST_REQUIRE(results, TEST_APP_IMPL_H_RESULT_MODULE_TITLE, count != ::llc::size(expected));
	::llc::sc_t title[::llc::size(expected)] = {};
	::llc::u2_t capacity = ::llc::size(title);
	LLC_TEST_REQUIRE(results, TEST_APP_IMPL_H_RESULT_MODULE_TITLE, 0 != ::llc_moduleTitle(title, &capacity));
	LLC_TEST_CHECK(results, TEST_APP_IMPL_H_RESULT_MODULE_TITLE, 0 != memcmp(title, expected, szof(expected)));
	rtrn 0;
}

sttc ::llc::err_t testAppImplHCreateDelete(::llc::ATestError & results) {
	::appImplReset();
	void * instance = 0;
	LLC_TEST_REQUIRE(results, TEST_APP_IMPL_H_RESULT_CREATE_DELETE, 0 != ::appImplCreate(instance));
	LLC_TEST_REQUIRE(results, TEST_APP_IMPL_H_RESULT_CREATE_DELETE, 0 == instance);
	LLC_TEST_REQUIRE(results, TEST_APP_IMPL_H_RESULT_CREATE_DELETE, 0 != ::llc_moduleDelete(&instance));
	cnst ::llc::u0_t expected[] = {::APP_IMPL_EVENT_CREATE, ::APP_IMPL_EVENT_DELETE};
	LLC_TEST_CHECK(results, TEST_APP_IMPL_H_RESULT_CREATE_DELETE, instance || ::AppImplEvents.size() != ::llc::size(expected) || 0 != memcmp(::AppImplEvents.begin(), expected, szof(expected)));
	rtrn 0;
}

sttc ::llc::err_t testAppImplHCallbackOrder(::llc::ATestError & results) {
	::appImplReset();
	void * instance = 0;
	LLC_TEST_REQUIRE(results, TEST_APP_IMPL_H_RESULT_CALLBACK_ORDER, 0 != ::appImplCreate(instance));
	LLC_TEST_REQUIRE(results, TEST_APP_IMPL_H_RESULT_CALLBACK_ORDER, 0 != ::llc_moduleSetup(instance));
	LLC_TEST_REQUIRE(results, TEST_APP_IMPL_H_RESULT_CALLBACK_ORDER, 0 != ::llc_moduleUpdate(instance, false));
	LLC_TEST_REQUIRE(results, TEST_APP_IMPL_H_RESULT_CALLBACK_ORDER, 0 != ::llc_moduleRender(instance));
	LLC_TEST_REQUIRE(results, TEST_APP_IMPL_H_RESULT_CALLBACK_ORDER, 0 != ::llc_moduleCleanup(instance));
	LLC_TEST_REQUIRE(results, TEST_APP_IMPL_H_RESULT_CALLBACK_ORDER, 0 != ::llc_moduleDelete(&instance));
	cnst ::llc::u0_t expected[] = {::APP_IMPL_EVENT_CREATE, ::APP_IMPL_EVENT_SETUP, ::APP_IMPL_EVENT_UPDATE, ::APP_IMPL_EVENT_DRAW, ::APP_IMPL_EVENT_CLEANUP, ::APP_IMPL_EVENT_DELETE};
	LLC_TEST_CHECK(results, TEST_APP_IMPL_H_RESULT_CALLBACK_ORDER, ::AppImplEvents.size() != ::llc::size(expected) || 0 != memcmp(::AppImplEvents.begin(), expected, szof(expected)));
	rtrn 0;
}

sttc ::llc::err_t testAppImplHRequestedExit(::llc::ATestError & results) {
	::appImplReset();
	void * instance = 0;
	LLC_TEST_REQUIRE(results, TEST_APP_IMPL_H_RESULT_REQUESTED_EXIT, 0 != ::appImplCreate(instance));
	LLC_TEST_REQUIRE(results, TEST_APP_IMPL_H_RESULT_REQUESTED_EXIT, 0 != ::llc_moduleUpdate(instance, true));
	LLC_TEST_CHECK(results, TEST_APP_IMPL_H_RESULT_REQUESTED_EXIT, false == ::AppImplRequestedExit);
	LLC_TEST_REQUIRE(results, TEST_APP_IMPL_H_RESULT_REQUESTED_EXIT, 0 != ::llc_moduleDelete(&instance));
	rtrn 0;
}

sttc ::llc::err_t testAppImplHResultForwarding(::llc::ATestError & results) {
	::appImplReset();
	void * instance = 0;
	LLC_TEST_REQUIRE(results, TEST_APP_IMPL_H_RESULT_RESULT_FORWARDING, 0 != ::appImplCreate(instance));
	::AppImplSetupResult = -2;
	::AppImplUpdateResult = 3;
	::AppImplDrawResult = -4;
	::AppImplCleanupResult = 5;
	LLC_TEST_CHECK(results, TEST_APP_IMPL_H_RESULT_RESULT_FORWARDING, ::llc_moduleSetup(instance) != -2);
	LLC_TEST_CHECK(results, TEST_APP_IMPL_H_RESULT_RESULT_FORWARDING, ::llc_moduleUpdate(instance, false) != 3);
	LLC_TEST_CHECK(results, TEST_APP_IMPL_H_RESULT_RESULT_FORWARDING, ::llc_moduleRender(instance) != -4);
	LLC_TEST_CHECK(results, TEST_APP_IMPL_H_RESULT_RESULT_FORWARDING, ::llc_moduleCleanup(instance) != 5);
	LLC_TEST_REQUIRE(results, TEST_APP_IMPL_H_RESULT_RESULT_FORWARDING, 0 != ::llc_moduleDelete(&instance));
	rtrn 0;
}

sttc ::llc::err_t testAppImplHExceptionTranslation(::llc::ATestError & results) {
	::appImplReset();
	void * instance = 0;
	LLC_TEST_REQUIRE(results, TEST_APP_IMPL_H_RESULT_EXCEPTION_TRANSLATION, 0 != ::appImplCreate(instance));
	::AppImplThrowSetup = true;
	LLC_TEST_CHECK(results, TEST_APP_IMPL_H_RESULT_EXCEPTION_TRANSLATION, 0 <= ::llc_moduleSetup(instance));
	::AppImplThrowUpdate = true;
	LLC_TEST_CHECK(results, TEST_APP_IMPL_H_RESULT_EXCEPTION_TRANSLATION, 0 <= ::llc_moduleUpdate(instance, false));
	::AppImplThrowDraw = true;
	LLC_TEST_CHECK(results, TEST_APP_IMPL_H_RESULT_EXCEPTION_TRANSLATION, 0 <= ::llc_moduleRender(instance));
	::AppImplThrowCleanup = true;
	LLC_TEST_CHECK(results, TEST_APP_IMPL_H_RESULT_EXCEPTION_TRANSLATION, 0 <= ::llc_moduleCleanup(instance));
	LLC_TEST_REQUIRE(results, TEST_APP_IMPL_H_RESULT_EXCEPTION_TRANSLATION, 0 != ::llc_moduleDelete(&instance));
	rtrn 0;
}

sttc ::llc::err_t testAppImplHConstructorException(::llc::ATestError & results) {
	::appImplReset();
	::AppImplThrowConstructor = true;
	void * instance = 0;
	LLC_TEST_CHECK(results, TEST_APP_IMPL_H_RESULT_CONSTRUCTOR_EXCEPTION, 0 <= ::appImplCreate(instance));
	LLC_TEST_CHECK(results, TEST_APP_IMPL_H_RESULT_CONSTRUCTOR_EXCEPTION, instance || ::AppImplEvents.size());
	rtrn 0;
}
#endif

::llc::err_t testAppImplH(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = ::testAppImplHAvailability(results);
#if LLC_TEST_APP_IMPL_READY
	result = ::llc::min(result, ::testAppImplHModuleTitle(results));
	result = ::llc::min(result, ::testAppImplHCreateDelete(results));
	result = ::llc::min(result, ::testAppImplHCallbackOrder(results));
	result = ::llc::min(result, ::testAppImplHRequestedExit(results));
	result = ::llc::min(result, ::testAppImplHResultForwarding(results));
	result = ::llc::min(result, ::testAppImplHExceptionTranslation(results));
	result = ::llc::min(result, ::testAppImplHConstructorException(results));
#endif
	rtrn result;
}
