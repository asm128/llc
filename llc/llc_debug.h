/// Copyright 2010-2022 - ogarnd
#include "llc_platform_globals.h"

#ifdef LLC_ATMEL
#   include <stddef.h>
#else
#   include <cstddef>
#endif

#ifndef LLC_DEBUG_H
#define LLC_DEBUG_H

#ifndef LLC_DEBUG_ENABLED
namespace llc { stxp size_t DEBUG_BUILD = 0; }
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
#	ifndef LLC_KEEP_SYSTEM_ERROR_ON_ERROR_LOG
#		define LLC_CLEAR_SYSTEM_ERROR_ON_ERROR_LOG
#	endif
#	ifdef LLC_WINDOWS
#		define LLC_PLATFORM_CRT_CHECK_MEMORY()	do {} while(0) // (void)_CrtCheckMemory
#	else // LLC_WINDOWS
#		ifndef LLC_ESP32
#		    ifdef LLC_ESP8266
#			    include <user_interface.h>
#			    define LLC_PLATFORM_CRT_CHECK_MEMORY() do { info_printf("Available RAM - Heap: %" LLC_FMT_U2 " bytes.", system_get_free_heap_size()); } while(0)
#		    else // !LLC_ESP8266
#			    define LLC_PLATFORM_CRT_CHECK_MEMORY()	do {} while(0)
#		    endif // LLC_ESP8266
#		else // LLC_ESP32
#			include <esp_heap_caps.h>
#			include <freertos/FreeRTOS.h>
#			include <freertos/task.h>
#			define LLC_PLATFORM_CRT_CHECK_MEMORY() do { info_printf("Available RAM - Heap: %" LLC_FMT_U2 " bytes, Stack: %" LLC_FMT_U2 " bytes.", heap_caps_get_free_size(MALLOC_CAP_8BIT), uxTaskGetStackHighWaterMark(NULL)); } while(0)
#		endif // LLC_ESP32
#	endif // LLC_WINDOWS
#endif // LLC_DEBUG_ENABLED

namespace llc
{
	int	debugBreakEnabled	()						nxpt;
	int	debugBreak			()						nxpt;
	int	debugBreakEnable	(int enabled = true)	nxpt;
	int	debugBreakDisable	()						nxpt;
} // namespace

#endif // LLC_DEBUG_H
