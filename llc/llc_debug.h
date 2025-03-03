/// Copyright 2010-2022 - ogarnd
#include "llc_platform_globals.h"

#ifdef LLC_ATMEL
#   include <stddef.h>
#else
#   include <cstddef>
#endif

// Uncomment these to toggle behavior
//#define LLC_DISABLE_DEBUG_BREAK_ON_ERROR_LOG
//#define LLC_KEEP_SYSTEM_ERROR_ON_ERROR_LOG

#ifndef LLC_DEBUG_H
#define LLC_DEBUG_H

#ifndef LLC_DEBUG_ENABLED
namespace llc { stxp size_t DEBUG_BUILD = 0; } 
#	define LLC_PLATFORM_CRT_BREAKPOINT()		do {} while(0)
#	define LLC_PLATFORM_CRT_CHECK_MEMORY()		do {} while(0)
//#	ifndef ARDUINOJSON_ENABLE_COMMENTS
#		define LLC_ERROR_PRINTF_ENABLED
#		define LLC_WARNING_PRINTF_ENABLED	// Uncomment as needed
//#	endif
//#	define LLC_INFO_PRINTF_ENABLED		// Uncomment as needed
//#	define LLC_SUCCESS_PRINTF_ENABLED	// Uncomment as needed
//#	define LLC_VERBOSE_PRINTF_ENABLED	// Uncomment as needed
#else
namespace llc { stxp size_t DEBUG_BUILD = (size_t)-1; } 
#	define LLC_ERROR_PRINTF_ENABLED
#	define LLC_WARNING_PRINTF_ENABLED
#	define LLC_INFO_PRINTF_ENABLED
//#	define LLC_SUCCESS_PRINTF_ENABLED	// Uncomment as needed
//#	define LLC_VERBOSE_PRINTF_ENABLED
#	ifndef LLC_DISABLE_DEBUG_BREAK_ON_ERROR_LOG
#		define LLC_USE_DEBUG_BREAK_ON_ERROR_LOG
#	endif
#	ifndef LLC_KEEP_SYSTEM_ERROR_ON_ERROR_LOG
#		define LLC_CLEAR_SYSTEM_ERROR_ON_ERROR_LOG
#	endif
#	ifdef LLC_WINDOWS
#		include <crtdbg.h>
#		if defined LLC_USE_DEBUG_BREAK_ON_ERROR_LOG
#			define LLC_PLATFORM_CRT_BREAKPOINT		(void)_CrtDbgBreak
#		else
#			define LLC_PLATFORM_CRT_BREAKPOINT()	do {} while(0)
#		endif
#		define LLC_PLATFORM_CRT_CHECK_MEMORY()	do {} while(0) // (void)_CrtCheckMemory
#	else
#		define LLC_PLATFORM_CRT_BREAKPOINT()	do {} while(0)
#		ifndef LLC_ESP32
#			define LLC_PLATFORM_CRT_CHECK_MEMORY()	do {} while(0)
#		else
#			include <esp_heap_caps.h>
#			include <freertos/FreeRTOS.h>
#			include <freertos/task.h>
#			define LLC_PLATFORM_CRT_CHECK_MEMORY() do { info_printf("Available RAM - Heap: %" LLC_FMT_U2 " bytes, Stack: %" LLC_FMT_U2 " bytes.", heap_caps_get_free_size(MALLOC_CAP_8BIT), uxTaskGetStackHighWaterMark(NULL)); } while(0)
#		endif
#	endif
#endif // LLC_DEBUG_ENABLED

#endif // LLC_DEBUG_H
