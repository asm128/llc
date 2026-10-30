/// Copyright 2017-2018 - asm128
#include "llc_module.h"
#include "llc_ptr_obj.h"
#include "llc_process.h"
#include "llc_runtime_module.h"
#include "llc_timer.h"


#if defined LLC_AVOID_LOCAL_APPLICATION_MODEL_EXECUTABLE_RUNTIME
#		define LLC_DEFINE_APPLICATION_ENTRY_POINT(_mainClass, _moduleTitle)		LLC_DEFINE_APP_MODULE_INTERFACE(_mainClass, _moduleTitle)
#else
#	define LLC_DEFINE_APPLICATION_ENTRY_POINT(_mainClass, _moduleTitle)										\
namespace llc																								\
{																											\
	struct SRuntimeState {																					\
		refcount_t			RenderThreadUsers	= {};														\
		pobj<_mainClass>	Application			= {};														\
		b8_t				RequestedExit		= {};														\
	};																										\
}																											\
		LLC_DEFINE_APP_MODULE_INTERFACE(_mainClass, _moduleTitle);											\
		static	::llc::err_t	rtMain		(::llc::SRuntimeValues & runtimeValues);						\
		LLC_OS_ENTRY_POINT(rtMain);																			\
		::llc::err_t			rtMain		(::llc::SRuntimeValues & runtimeValues)	{						\
		{																									\
			LLC_SYSTEM_OS_DEBUG_INIT_FLAGS();																\
			{																								\
				::llc::pobj<_mainClass>	app;																\
				if_true_fe(0 == app.create(runtimeValues));													\
				info_printf("%s", "Initializing application instance.");									\
				if_fail_fe(::setup(*app));																	\
				info_printf("%s", "Application instance initialized successfully. Executing main loop...");	\
				while(true) {																				\
					::llc::err_t			updateResult		= ::update(*app, false);					\
					if_true_bi(::llc::APPLICATION_STATE_EXIT == updateResult);								\
					if_true_be(::llc::failed(updateResult));												\
					if_true_e(::draw(*app));																\
				}																							\
				info_printf("%s", "Cleaning up application instance...");									\
				::cleanup(*app);																			\
			}																								\
			info_printf("%s", "Application instance destroyed.");											\
		}																									\
		return 0;																							\
	} // 
#	define LLC_DEFINE_APPLICATION_ENTRY_POINT_MT(_mainClass, _moduleTitle)										\
	namespace llc																								\
	{																											\
		struct SRuntimeState {																					\
			refcount_t			RenderThreadUsers	= {};														\
			pobj<_mainClass>	Application			= {};														\
			b8_t				RequestedExit		= {};														\
		};																										\
	}																											\
 	LLC_DEFINE_APP_MODULE_INTERFACE(_mainClass, _moduleTitle);													\
	static	::llc::err_t	rtMain		(::llc::SRuntimeValues & runtimeValues);								\
	LLC_OS_ENTRY_POINT(rtMain);																				\
																												\
	static	void				threadRender		(void * pRuntimeState)	{									\
		::llc::SRuntimeState			& runtimeState		= *(::llc::SRuntimeState*)pRuntimeState;			\
		if_true_ref(!runtimeState.Application, "Main module is null:(%s)\"%s\"", #_mainClass, _moduleTitle);	\
		llc_sync_increment(runtimeState.RenderThreadUsers);														\
		::llc::s2_t						result				= 0;									\
		/*::llc::sleep(1);*/																					\
		while(result = llc_sync_compare_exchange(runtimeState.RenderThreadUsers, 0, 1) - 1 && result != -1) {	\
			::draw(*runtimeState.Application);																	\
		}																										\
		info_printf("%s", "Render thread done.");																\
	}																											\
																												\
	static	::llc::err_t	threadRenderStart	(::llc::SRuntimeState& runtimeState)	{						\
		_beginthread(threadRender, 0, &runtimeState);															\
		return 0;																								\
	}																											\
																												\
	static	::llc::err_t	lrt_Loop			(::llc::SRuntimeState & runtimeState)	{						\
		::llc::err_t				updateResult	= ::update(*runtimeState.Application, false);				\
		if(1 == updateResult || ::llc::failed(updateResult)) {													\
			if_true_i(::llc::APPLICATION_STATE_EXIT == updateResult);											\
			if_true_e(::llc::failed(updateResult));																\
		}																										\
		else {																									\
			llc_sync_increment(runtimeState.RenderThreadUsers);	/* Report we're alive	*/						\
			if_fail_fe(::threadRenderStart(runtimeState));														\
			info_printf("%s", "Application instance initialized successfully. Executing main loop...");			\
			while(true) {																						\
				updateResult	= ::update(*runtimeState.Application, false);									\
				if_true_bi(::llc::APPLICATION_STATE_EXIT == updateResult);										\
				if_true_be(::llc::failed(updateResult));														\
				/*if_true_ef(mainModule.Render(app), "Why would this ever happen?");	*/						\
				/*Sleep(1);		*/																				\
			}																									\
			llc_sync_decrement(runtimeState.RenderThreadUsers);	/* Report we're done	*/						\
		}																										\
		{																										\
			::llc::STimer	timer;																				\
			::llc::f3_t		elapsedTime		= 0;																\
			while(-1 != llc_sync_compare_exchange(runtimeState.RenderThreadUsers, -1, 0)) {	/* Wait until the render thread is closed.	*/	\
				if(elapsedTime > 5.0f)						\
					break;									\
				::llc::sleep(10);							\
				timer.Frame();								\
				/*elapsedTime += timer.LastTimeSeconds;	*/	\
			}												\
		}													\
		return updateResult;								\
	}														\
	sttc	::llc::err_t	rtMain			(::llc::SRuntimeValues & runtimeValues)	{	\
		::llc::SRuntimeState		runtimeState	= {};								\
		{																				\
			if_null_fe(runtimeState.Application.create(runtimeValues));					\
			info_printf("%s", "Initializing application instance.");					\
			if(::llc::failed(::setup(*runtimeState.Application))) {						\
				error_printf("%s", "Setup() Failed!");									\
				if_fail_fe(::cleanup(*runtimeState.Application));							\
				return -1;																\
			}																			\
			::lrt_Loop(runtimeState);													\
			info_printf("%s", "Cleaning up application instance...");					\
			if_fail_fe(::cleanup(*runtimeState.Application));								\
		}																				\
		info_printf("%s", "Application instance destroyed.");							\
		return 0;																		\
	} //


#endif // LLC_AVOID_LOCAL_APPLICATION_MODULE_MODEL_EXECUTABLE_RUNTIME
