#include "llc_error.h"

#ifndef LLC_MODULE_H_23627
#define LLC_MODULE_H_23627

#if defined(LLC_WINDOWS)
#	define LLC_STDCALL				//__stdcall
#	define LLC_MODULE_API_EXPORT	__declspec(dllexport)
#	define LLC_MODULE_API_IMPORT	__declspec(dllimport)
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#elif defined(LLC_LINUX) || defined(LLC_ANDROID) || defined(LLC_ESP32) || defined(LLC_ARDUINO)
#	define LLC_STDCALL
#	define LLC_MODULE_API_EXPORT	__attribute__((visibility("default")))
#	define LLC_MODULE_API_IMPORT
#else
//  do nothing and hope for the best?
#	define LLC_STDCALL
#	define LLC_MODULE_API_EXPORT
#	define LLC_MODULE_API_IMPORT
#	error "Unknown dynamic link import/export semantics."
#endif

#define LLC_MODULE_FUNCTION_NAME(functionName)	LLC_func_##functionName

#define LLC_DECLARE_MODULE_FUNCTION(functionName, ...) 																	\
	extern "C"	::llc::err_t		 LLC_STDCALL			functionName									(__VA_ARGS__);	\
	typedef		::llc::err_t		(LLC_STDCALL			* LLC_MODULE_FUNCTION_NAME(functionName))		(__VA_ARGS__);	\

#if defined(LLC_WINDOWS)
#	define	LLC_LOAD_MODULE(moduleName)								LoadLibraryA	(moduleName)
#	define	LLC_FREE_MODULE(moduleAddr)								FreeLibrary		((HMODULE)(moduleAddr))
#	define	LLC_LOAD_MODULE_FUNCTION(moduleHandle, symbolName)		GetProcAddress((HMODULE)moduleHandle, symbolName)
#elif defined(LLC_LINUX) || defined(LLC_ANDROID)
#	define	LLC_LOAD_MODULE(moduleName)								dlopen	(moduleName, RTLD_GLOBAL)
#	define	LLC_FREE_MODULE(moduleAddr)								dlclose	(moduleAddr)
#	define	LLC_LOAD_MODULE_FUNCTION(moduleHandle, symbolName)		dlsym	(moduleHandle, symbolName)
#else
#	define	LLC_LOAD_MODULE(moduleName)								::llc::nully(moduleName)
#	define	LLC_FREE_MODULE(moduleAddr)								::llc::nully(moduleAddr)
#	define	LLC_LOAD_MODULE_FUNCTION(moduleHandle, symbolName)		::llc::nully(moduleHandle, symbolName)
#endif

#if defined LLC_WINDOWS
#	define LLC_MODULE_EXTENSION "dll"
#else
#	define LLC_MODULE_EXTENSION "so"
#endif

/*#define LLC_DECLARE_MODULE_EXPORT_FUNCTION(functionName, ...) 															\
//	extern "C"	::llc::err_t		 LLC_MODULE_API_EXPORT	functionName									(__VA_ARGS__);	\
//	typedef		::llc::err_t		(LLC_MODULE_API_EXPORT	* LLC_MODULE_FUNCTION_NAME(functionName))		(__VA_ARGS__);
*/
namespace llc
{
#pragma pack(push, 1)
	struct SModuleBase {
						void				* Handle				= 0;
	};
#pragma pack(pop)
}


#endif // LLC_MODULE_H_23627
