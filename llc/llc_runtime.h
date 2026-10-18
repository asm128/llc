/// Copyright 2016-2018 - asm128
#include "llc_args.h"
#include "llc_enum.h"

#if defined(LLC_WINDOWS)
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#elif defined(LLC_ANDROID)
#	include <android/native_activity.h>
#endif

#ifndef LLC_RUNTIME_H_23627
#define LLC_RUNTIME_H_23627

namespace llc
{
	struct SRuntimeValuesDetail {
#ifdef LLC_ANDROID
		ANativeActivity			* Activity		= nullptr;
		void					* SavedState	= nullptr;
		size_t					SavedStateSize	= 0;
#elif defined(LLC_WINDOWS)
		HINSTANCE				hInstance		;
		HINSTANCE				hPrevInstance	;
		LPSTR					lpCmdLine		;
		int						nShowCmd		;
#endif
	};
	struct SRuntimeValues {
		SRuntimeValuesDetail	PlatformDetail	;
		SCommandLineArgs		EntryPointArgs	;
	};

} // namespace

#if defined(LLC_WINDOWS)
#	define LLC_SYSTEM_OS_DEBUG_INIT_FLAGS()	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF)
#else
#	define LLC_SYSTEM_OS_DEBUG_INIT_FLAGS() do {} while(0)
#endif

#if defined(LLC_WINRT)
#	include <wrl.h>
#	define LLC_RO_INIT_MULTITHREADED() do { Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED); } while(0)
#else
#	define LLC_RO_INIT_MULTITHREADED() do {} while(0)
#endif

#define LLC_OS_ENTRY_POINT_COMMON_INIT(llc_app_entry_point, main_argc, main_argv, main_envp)	\
	LLC_SYSTEM_OS_DEBUG_INIT_FLAGS();															\
	if_true_fef(65535 < main_argc, "Invalid parameter count: %" LLC_FMT_U2 ".", main_argc);		\
	LLC_RO_INIT_MULTITHREADED();																\
	::llc::SRuntimeValues		runtimeValues		= {};										\
	if_fail_fe(::llc::argsParse(runtimeValues.EntryPointArgs, main_argc, main_argv, main_envp))	//

#define LLC_OS_ENTRY_POINT_ANY(llc_app_entry_point)												\
	int						main				(int argc, char *argv[]) {						\
		LLC_OS_ENTRY_POINT_COMMON_INIT(llc_app_entry_point, argc, argv, 0);						\
		return ::llc::failed(llc_app_entry_point(runtimeValues)) ? EXIT_FAILURE : EXIT_SUCCESS;	\
	} // main
#define LLC_OS_ENTRY_POINT_NOENVP(llc_app_entry_point)											\
	int						main				(int argc, char *argv[], char *envp[]) {		\
		LLC_OS_ENTRY_POINT_COMMON_INIT(llc_app_entry_point, argc, argv, envp);					\
		return ::llc::failed(llc_app_entry_point(runtimeValues)) ? EXIT_FAILURE : EXIT_SUCCESS;	\
	} // main 

#ifndef LLC_WINDOWS
#	define LLC_OS_ENTRY_POINT	LLC_OS_ENTRY_POINT_ANY
#else // LLC_WINDOWS
#	define LLC_OS_ENTRY_POINT(llc_app_entry_point)												\
	LLC_OS_ENTRY_POINT_ANY(llc_app_entry_point);												\
	int	WINAPI				WinMain																\
		(	_In_		::HINSTANCE		hInstance												\
		,	_In_opt_	::HINSTANCE		hPrevInstance											\
		,	_In_		::LPSTR			lpCmdLine												\
		,	_In_		::INT			nShowCmd												\
		) {																						\
		LLC_OS_ENTRY_POINT_COMMON_INIT(llc_app_entry_point, (__argc), (__argv), (environ));		\
		runtimeValues.PlatformDetail	= {hInstance, hPrevInstance, lpCmdLine, nShowCmd};		\
		return ::llc::failed(llc_app_entry_point(runtimeValues)) ? EXIT_FAILURE : EXIT_SUCCESS;	\
	} //
#endif // LLC_WINDOWS

#endif // LLC_RUNTIME_H_23627
