/// Copyright 2016-2018 - asm128
#include "llc_auto_handler.h"
#include "llc_debug.h"
#include "llc_runtime.h"
#include "llc_ptr_obj.h"
#include "llc_sync.h"

#if defined(LLC_WINDOWS)
#	include <process.h>
#endif

#ifndef LLC_RUNTIME_APP_H_23627
#define LLC_RUNTIME_APP_H_23627

namespace llc
{
	enum APPLICATION_STATE : u0_t
		{	APPLICATION_STATE_NORMAL		= 0
		,	APPLICATION_STATE_EXIT			= 1
		,	APPLICATION_STATE_BUSY			= 2
		,	APPLICATION_STATE_STAY			= 3
		};

	stin	bool	runtimeMultithreaded	(SRuntimeValues & runtimeValues) noexcept					{ rtrn 0 != llc_sync_compare_exchange(runtimeValues.RuntimeMultithreaded, 0, 0); }
	stin	void	runtimeMultithreaded	(SRuntimeValues & runtimeValues, bool enabled) noexcept	{ llc_sync_exchange(runtimeValues.RuntimeMultithreaded, enabled); }
	stin	bool	runtimeDebugCRT			(SRuntimeValues & runtimeValues) noexcept					{ rtrn 0 != llc_sync_compare_exchange(runtimeValues.RuntimeDebugCRT, 0, 0); }
	stin	void	runtimeDebugCRT			(SRuntimeValues & runtimeValues, bool enabled) noexcept	{ llc_sync_exchange(runtimeValues.RuntimeDebugCRT, enabled); }

	struct auto_thread_user : public ::llc::auto_handler<refcount_t*, nullptr> {
		bool						MustRelease			= false;
		using						TWrapper::auto_handler;
		inline						~auto_thread_user		() noexcept	{ close(); }
		inline	void				close					() noexcept	{ if(MustRelease) { llc_sync_decrement(*Handle); MustRelease = false; } }
	};

	tplT
	struct SRuntimeState {
		refcount_t					DrawThreadUsers		= 0;
		refcount_t					DrawResult			= 0;
		T						* Application		= {};
	};

	tplT
	static	void	threadDraw			(void * pRuntimeState) {
		SRuntimeState<T>				& runtimeState		= *(SRuntimeState<T>*)pRuntimeState;
		auto_thread_user				threadUser			= {};
		threadUser.Handle				= &runtimeState.DrawThreadUsers;
		llc_sync_increment(*threadUser.Handle);
		threadUser.MustRelease			= true;
		err_t						result				= 0;
		while(1 < llc_sync_compare_exchange(runtimeState.DrawThreadUsers, 0, 1))
			if_fail_be(result = draw(*runtimeState.Application));
		threadUser.MustRelease			= failed(result);
		if(threadUser.MustRelease)
			llc_sync_exchange(runtimeState.DrawResult, result);
	}

	tplT
	static	err_t	threadDrawStop		(SRuntimeState<T> & runtimeState) {
		err_t						result					= (err_t)llc_sync_compare_exchange(runtimeState.DrawResult, 0, 0);
		if(0 != llc_sync_compare_exchange(runtimeState.DrawThreadUsers, 0, 0)) {
			llc_sync_decrement(runtimeState.DrawThreadUsers);
			while(-1 != llc_sync_compare_exchange(runtimeState.DrawThreadUsers, -1, 0))
				sleep(10);
			llc_sync_exchange(runtimeState.DrawThreadUsers, 0);
			result						= (err_t)llc_sync_compare_exchange(runtimeState.DrawResult, 0, 0);
		}
		rtrn result;
	}

	tplT
	static	err_t	threadDrawStart		(SRuntimeState<T> & runtimeState) {
#if defined(LLC_WINDOWS)
		auto_thread_user				threadUser			= {};
		threadUser.Handle				= &runtimeState.DrawThreadUsers;
		llc_sync_exchange(runtimeState.DrawResult, 0);
		llc_sync_increment(*threadUser.Handle);
		threadUser.MustRelease			= true;
		if_true_ve(OS_ERROR, -1L == (refcount_t)_beginthread(threadDraw<T>, 0, &runtimeState));
		threadUser.MustRelease			= false;
		err_t						result					= 0;
		while(1 == llc_sync_compare_exchange(runtimeState.DrawThreadUsers, 1, 1)
			&& not_failed(result = (err_t)llc_sync_compare_exchange(runtimeState.DrawResult, 0, 0)))
			sleep(1);
		rtrn failed(result) ? threadDrawStop(runtimeState) : result;
#else
		(void)runtimeState;
		rtrn OS_NOT_AVAILABLE;
#endif
	}

	tplT
	err_t			runtimeLoop			(T & application, SRuntimeValues & runtimeValues) {
		SRuntimeState<T>				runtimeState			= {0, 0, &application};
		err_t						result					= 0;
		while(1) {
			result						= update(application, false);
			debugCRTEnable(runtimeDebugCRT(runtimeValues));
			if_true_bi(APPLICATION_STATE_EXIT == result);
			if_fail_be(result);
			if_fail_be(result = (err_t)llc_sync_compare_exchange(runtimeState.DrawResult, 0, 0));
			if(runtimeMultithreaded(runtimeValues)) {
				if(0 == llc_sync_compare_exchange(runtimeState.DrawThreadUsers, 0, 0))
					if_fail_be(result = threadDrawStart(runtimeState));
			}
			else {
				if(0 != llc_sync_compare_exchange(runtimeState.DrawThreadUsers, 0, 0))
					if_fail_be(result = threadDrawStop(runtimeState));
				if_fail_be(result = draw(application));
			}
		}
		const err_t					resultDraw				= threadDrawStop(runtimeState);
		rtrn failed(result) ? result : failed(resultDraw) ? resultDraw : result;
	}

	tplT
	err_t			runtimeRun			(T & application, SRuntimeValues & runtimeValues) {
		debugCRTEnable(runtimeDebugCRT(runtimeValues));
		err_t						result					= setup(application);
		debugCRTEnable(runtimeDebugCRT(runtimeValues));
		if_fail_e(result);
		if(not_failed(result))
			result						= runtimeLoop(application, runtimeValues);
		const err_t					resultCleanup			= cleanup(application);
		if_fail_e(resultCleanup);
		rtrn failed(result) ? result : failed(resultCleanup) ? resultCleanup : result;
	}

	tplT
	err_t			runtimeEntryPoint	(SRuntimeValues & runtimeValues) {
		debugCRTEnable(runtimeDebugCRT(runtimeValues));
		pobj<T>						application				= {};
		if_null_ve(OS_NO_MEMORY, application.create(runtimeValues));
		rtrn runtimeRun(*application, runtimeValues);
	}
} // namespace

#define LLC_DEFINE_APPLICATION_ENTRY_POINT(_mainClass, _multithreaded, _debugCRT)\
	static ::llc::err_t llcRuntimeEntryPoint(::llc::SRuntimeValues & runtimeValues) {\
		::llc::runtimeMultithreaded(runtimeValues, _multithreaded);\
		::llc::runtimeDebugCRT(runtimeValues, _debugCRT);\
		return ::llc::runtimeEntryPoint<_mainClass>(runtimeValues);\
	}\
	LLC_SYSTEM_OS_ENTRY_POINT(::llcRuntimeEntryPoint)

#endif // LLC_RUNTIME_APP_H_23627
