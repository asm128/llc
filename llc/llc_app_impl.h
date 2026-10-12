/// Copyright 2017-2018 - asm128
#include "llc_ptr_obj.h"
#include "llc_runtime_module.h"
#include "llc_sync.h"

#if defined(LLC_WINDOWS)
#	include <process.h>
#endif

#ifndef LLC_APP_IMPL_H_23627
#define LLC_APP_IMPL_H_23627

namespace llc
{
	tydf void (*FAppThread)(void*);

	stin err_t appThreadStart(FAppThread threadFunction, void* runtimeState) {
#if defined(LLC_WINDOWS)
		rtrn -1L == (refcount_t)_beginthread(threadFunction, 0, runtimeState) ? OS_ERROR : 0;
#elif !defined(LLC_ARDUINO) && !defined(LLC_CMSIS)
		try { ::std::thread{threadFunction, runtimeState}.detach(); }
		catch(...) { rtrn OS_ERROR; }
		rtrn 0;
#else
		(void)threadFunction;
		(void)runtimeState;
		rtrn OS_NOT_AVAILABLE;
#endif
	}
}

#define LLC_DEFINE_APP_MODULE_INTERFACE(_mainClass, _moduleTitle)																			\
	::llc::err_t	setup		(_mainClass& app);																							\
	::llc::err_t	cleanup		(_mainClass& app);																							\
	::llc::err_t	update		(_mainClass& app, bool systemRequestedExit);																\
	::llc::err_t	draw		(_mainClass& app);																							\
	::llc::err_t	LLC_STDCALL	llc_moduleVersion	(::llc::u2_t* version) {																\
		if_null_ve(::llc::OS_INVALID_PARAMETER, version);																				\
		*version = 1;																													\
		rtrn 0;																															\
	}																																	\
	::llc::err_t	LLC_STDCALL	llc_moduleCreate		(void** instanceApp, ::llc::SRuntimeValues* runtimeValues) {								\
		if_null_ve(::llc::OS_INVALID_PARAMETER, instanceApp);																			\
		if_null_ve(::llc::OS_INVALID_PARAMETER, runtimeValues);																		\
		try { *instanceApp = new _mainClass{*runtimeValues}; rtrn 0; }																	\
		catch(...) { rtrn ::llc::OS_NO_MEMORY; }																						\
	}																																	\
	::llc::err_t	LLC_STDCALL	llc_moduleDelete		(void** instanceApp) {																		\
		if_null_ve(::llc::OS_INVALID_PARAMETER, instanceApp);																			\
		try { delete ((_mainClass*)*instanceApp); *instanceApp = 0; rtrn 0; }															\
		catch(...) { rtrn ::llc::OS_ERROR; }																							\
	}																																	\
	::llc::err_t	LLC_STDCALL	llc_moduleSetup		(void* instanceApp) {																		\
		if_null_ve(::llc::OS_INVALID_PARAMETER, instanceApp);																			\
		try { rtrn setup(*(_mainClass*)instanceApp); }																					\
		catch(...) { rtrn ::llc::OS_ERROR; }																							\
	}																																	\
	::llc::err_t	LLC_STDCALL	llc_moduleCleanup	(void* instanceApp) {																		\
		if_null_ve(::llc::OS_INVALID_PARAMETER, instanceApp);																			\
		try { rtrn cleanup(*(_mainClass*)instanceApp); }																				\
		catch(...) { rtrn ::llc::OS_ERROR; }																							\
	}																																	\
	::llc::err_t	LLC_STDCALL	llc_moduleUpdate		(void* instanceApp, bool systemRequestedExit) {											\
		if_null_ve(::llc::OS_INVALID_PARAMETER, instanceApp);																			\
		try { rtrn update(*(_mainClass*)instanceApp, systemRequestedExit); }															\
		catch(...) { rtrn ::llc::OS_ERROR; }																							\
	}																																	\
	::llc::err_t	LLC_STDCALL	llc_moduleRender		(void* instanceApp) {																		\
		if_null_ve(::llc::OS_INVALID_PARAMETER, instanceApp);																			\
		try { rtrn draw(*(_mainClass*)instanceApp); }																					\
		catch(...) { rtrn ::llc::OS_ERROR; }																							\
	}																																	\
	::llc::err_t	LLC_STDCALL	llc_moduleTitle		(::llc::sc_t* outputBuffer, ::llc::u0_t* outputBufferLength) {							\
		stxp ::llc::sc_t moduleTitle[] = _moduleTitle;																					\
		static_assert(::llc::size(moduleTitle) <= 0xFFU, "Module titles are limited to 255 bytes including the terminator.");			\
		if_null_ve(::llc::OS_INVALID_PARAMETER, outputBufferLength);																	\
		cnst ::llc::u0_t bufferLength = *outputBufferLength;																			\
		*outputBufferLength = (::llc::u0_t)::llc::size(moduleTitle);																	\
		if(0 == outputBuffer) rtrn 0;																									\
		if_true_ve(::llc::OS_OVERRUN, bufferLength < ::llc::size(moduleTitle));														\
		::memcpy(outputBuffer, moduleTitle, ::llc::size(moduleTitle));																	\
		rtrn 0;																															\
	}

#if defined LLC_AVOID_LOCAL_APPLICATION_MODEL_EXECUTABLE_RUNTIME
#	define LLC_DEFINE_APPLICATION_ENTRY_POINT(_mainClass, _moduleTitle)	\
		LLC_DEFINE_APP_MODULE_INTERFACE(_mainClass, _moduleTitle)
#	define LLC_DEFINE_APPLICATION_ENTRY_POINT_MT(_mainClass, _moduleTitle)	\
		LLC_DEFINE_APPLICATION_ENTRY_POINT(_mainClass, _moduleTitle)
#else
#	define LLC_DEFINE_APPLICATION_ENTRY_POINT(_mainClass, _moduleTitle)												\
		static ::llc::err_t rtMain(::llc::SRuntimeValues & runtimeValues);											\
		LLC_DEFINE_APP_MODULE_INTERFACE(_mainClass, _moduleTitle)														\
		LLC_SYSTEM_OS_ENTRY_POINT(::rtMain)																			\
		static ::llc::err_t rtMain(::llc::SRuntimeValues & runtimeValues) {											\
			::llc::pobj<_mainClass> app;																				\
			if_null_ve(::llc::OS_NO_MEMORY, app.create(runtimeValues));												\
			info_printf("%s", "Initializing application instance.");													\
			::llc::err_t result = ::setup(*app);																		\
			if(::llc::not_failed(result)) {																			\
				info_printf("%s", "Application instance initialized successfully. Executing main loop...");			\
				while(true) {																							\
					result = ::update(*app, false);																	\
					if_true_bi(::llc::APPLICATION_STATE_EXIT == result);												\
					if_fail_be(result);																				\
					if_fail_be(result = ::draw(*app));																\
				}																										\
			}																											\
			else																										\
				error_printf("Setup() failed with result:%i.", result);												\
			info_printf("%s", "Cleaning up application instance...");													\
			cnst ::llc::err_t resultCleanup = ::cleanup(*app);														\
			if_fail_ef(resultCleanup, "Cleanup() failed with result:%i.", resultCleanup);								\
			info_printf("%s", "Application instance destroyed.");														\
			rtrn ::llc::failed(result) ? result : ::llc::failed(resultCleanup) ? resultCleanup : 0;					\
		}

#	define LLC_DEFINE_APPLICATION_ENTRY_POINT_MT(_mainClass, _moduleTitle)											\
		static ::llc::err_t rtMain(::llc::SRuntimeValues & runtimeValues);											\
		LLC_DEFINE_APP_MODULE_INTERFACE(_mainClass, _moduleTitle)													\
		LLC_SYSTEM_OS_ENTRY_POINT(::rtMain)																					\
																													\
		struct SRuntimeState {																						\
			::llc::refcount_t		DrawThreadUsers		= 0;														\
			::llc::refcount_t		DrawResult			= 0;														\
			::llc::pobj<_mainClass>	Application			= {};														\
		};																											\
																													\
		static	void				threadRender					(void* pRuntimeState)	{						\
			::SRuntimeState					& runtimeState					= *(::SRuntimeState*)pRuntimeState;		\
			if(!runtimeState.Application) {																			\
				llc_sync_exchange(runtimeState.DrawResult, ::llc::OS_INVALID_PARAMETER);							\
				rtrn;																								\
			}																										\
			llc_sync_increment(runtimeState.DrawThreadUsers);														\
			::llc::err_t						result							= 0;												\
			while(1 < llc_sync_compare_exchange(runtimeState.DrawThreadUsers, 0, 1))								\
				if_fail_be(result = ::draw(*runtimeState.Application));											\
			if(::llc::failed(result)) {																				\
				llc_sync_exchange(runtimeState.DrawResult, result);												\
				llc_sync_decrement(runtimeState.DrawThreadUsers);													\
			}																										\
			info_printf("%s", "Draw thread done.");																	\
		}																											\
																													\
		static ::llc::err_t threadRenderStop(::SRuntimeState & runtimeState) {									\
			if(0 != llc_sync_compare_exchange(runtimeState.DrawThreadUsers, 0, 0)) {								\
				llc_sync_decrement(runtimeState.DrawThreadUsers);													\
				while(-1 != llc_sync_compare_exchange(runtimeState.DrawThreadUsers, -1, 0))						\
					::llc::sleep(1);																				\
				llc_sync_exchange(runtimeState.DrawThreadUsers, 0);												\
			}																										\
			rtrn (::llc::err_t)llc_sync_compare_exchange(runtimeState.DrawResult, 0, 0);							\
		}																											\
																													\
		static ::llc::err_t threadRenderStart(::SRuntimeState & runtimeState) {									\
			llc_sync_exchange(runtimeState.DrawResult, 0);															\
			llc_sync_increment(runtimeState.DrawThreadUsers);														\
			cnst ::llc::err_t resultStart = ::llc::appThreadStart(::threadRender, &runtimeState);					\
			if(::llc::failed(resultStart)) {																		\
				llc_sync_decrement(runtimeState.DrawThreadUsers);													\
				rtrn resultStart;																					\
			}																										\
			::llc::err_t resultDraw = 0;																			\
			while(1 == llc_sync_compare_exchange(runtimeState.DrawThreadUsers, 0, 0)								\
				&& ::llc::not_failed(resultDraw = (::llc::err_t)llc_sync_compare_exchange(runtimeState.DrawResult, 0, 0)))	\
				::llc::sleep(1);																					\
			rtrn ::llc::failed(resultDraw) ? ::threadRenderStop(runtimeState) : 0;									\
		}																											\
																													\
		static ::llc::err_t grt_Loop(::SRuntimeState & runtimeState) {												\
			::llc::err_t resultUpdate = ::update(*runtimeState.Application, false);									\
			if(::llc::APPLICATION_STATE_EXIT != resultUpdate && ::llc::not_failed(resultUpdate)) {					\
				resultUpdate = ::threadRenderStart(runtimeState);													\
				if(::llc::not_failed(resultUpdate)) {																\
					info_printf("%s", "Application instance initialized successfully. Executing main loop...");		\
					while(true) {																					\
						resultUpdate = ::update(*runtimeState.Application, false);									\
						if_true_bi(::llc::APPLICATION_STATE_EXIT == resultUpdate);									\
						if_fail_be(resultUpdate);																	\
						if_fail_be((::llc::err_t)llc_sync_compare_exchange(runtimeState.DrawResult, 0, 0));			\
					}																								\
				}																									\
			}																										\
			cnst ::llc::err_t resultDraw = ::threadRenderStop(runtimeState);										\
			rtrn ::llc::failed(resultUpdate) ? resultUpdate : ::llc::failed(resultDraw) ? resultDraw : resultUpdate;	\
		}																											\
																													\
		static ::llc::err_t rtMain(::llc::SRuntimeValues & runtimeValues) {										\
			::SRuntimeState runtimeState = {};																		\
			if_null_ve(::llc::OS_NO_MEMORY, runtimeState.Application.create(runtimeValues));							\
			info_printf("%s", "Initializing application instance.");													\
			::llc::err_t result = ::setup(*runtimeState.Application);												\
			if(::llc::not_failed(result))																			\
				result = ::grt_Loop(runtimeState);																	\
			else																									\
				error_printf("Setup() failed with result:%i.", result);												\
			info_printf("%s", "Cleaning up application instance...");													\
			cnst ::llc::err_t resultCleanup = ::cleanup(*runtimeState.Application);									\
			if_fail_ef(resultCleanup, "Cleanup() failed with result:%i.", resultCleanup);								\
			info_printf("%s", "Application instance destroyed.");														\
			rtrn ::llc::failed(result) ? result : ::llc::failed(resultCleanup) ? resultCleanup : 0;					\
		}																											\


#endif // LLC_AVOID_LOCAL_APPLICATION_MODEL_EXECUTABLE_RUNTIME

#endif // LLC_APP_IMPL_H_23627
