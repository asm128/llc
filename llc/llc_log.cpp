#include "llc_log.h"
#include "llc_chrono.h"

LLC_USING_TYPEINT();

#ifdef LLC_ATMEL
#	include <stdio.h>
#else
#	include <cstdio>
#endif

#ifdef LLC_WINDOWS
#	include <Windows.h>
#elif defined(LLC_ARDUINO)
#	include <Arduino.h>
#else
#	include <errno.h>
#	ifdef LLC_ANDROID
#		include <android/log.h>
#		define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO , "llc_app", __VA_ARGS__))
#		define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "llc_app", __VA_ARGS__))
#		ifndef NDEBUG
#			define LOGV(...)  ((void)__android_log_print(ANDROID_LOG_VERBOSE, "llc_app", __VA_ARGS__))
#		else
#			define LOGV(...)  ((void)0)
#		endif
#	endif
#endif

#ifdef LLC_ATMEL
stxp		int		LOG_PREFIX_BUFFER_SIZE	= 64;
#else
stxp		int		LOG_PREFIX_BUFFER_SIZE	= 256;
#endif

::llc::error_t			llc::debug_print_prefix				(int8_t severity, const char * path, uint32_t line, const char * function) {
	stxp sc_c	STR_DEBUG_PREFIX[]					= "%i|%llu|%s(%" LLC_FMT_U2 "){%s}:";
	char			formatted[::LOG_PREFIX_BUFFER_SIZE]	= {};
	snprintf(formatted, llc::size(formatted), STR_DEBUG_PREFIX, severity, ::llc::timeCurrentInMs(), path, line, function);
	return base_log_print(formatted);
}

#if defined(LLC_WINDOWS)
static	::llc::error_t	default_base_log_write	(const char * text, uint32_t textLen) 	{ u2_t iChar = 0; for(; iChar < textLen; ++iChar) { sc_c buf[2] = {text[iChar], 0}; OutputDebugStringA(buf); } return iChar; }
static	::llc::error_t	default_base_log_print	(const char * text)						{ OutputDebugStringA(text); return (::llc::error_t)strlen(text); }
#elif defined(LLC_ANDROID)
static	::llc::error_t	default_base_log_write	(const char * text, uint32_t textLen)	{ LOGI("%s", text); return textLen; }
static	::llc::error_t	default_base_log_print	(const char * text)						{ LOGI("%s", text); return (::llc::error_t)strlen(text); }
#elif defined(LLC_ARDUINO)
static	::llc::error_t	default_base_log_write	(const char * text, uint32_t textLen)	{ return Serial ? Serial.write(text, textLen) : textLen; }
static	::llc::error_t	default_base_log_print	(const char * text)						{ return Serial ? Serial.print(text) : (::llc::error_t)strlen(text); }
#else
static	::llc::error_t	default_base_log_write	(const char * text, uint32_t textLen)	{ u2_t iChar = 0; for(; iChar < textLen; ++iChar) printf("%c", text[iChar]); return iChar; }
static	::llc::error_t	default_base_log_print	(const char * text)						{ return (::llc::error_t)printf("%s", text); }
#endif

::llc::log_write_t		llc_log_write					= default_base_log_write;
::llc::log_print_t		llc_log_print					= default_base_log_print;

::llc::error_t			llc::_base_log_print			(const char* text)						{ return (llc_log_print && text) ? ::llc_log_print(text) : 0; }
::llc::error_t			llc::_base_log_write			(const char* text, uint32_t textLen)	{ return (llc_log_write && text && textLen) ? ::llc_log_write(text, textLen) : 0; }
#ifdef LLC_LOG_ARDUINO_FLASHSTRINGHELPER
::llc::log_print_P_t	llc_log_print_P					= {};
::llc::error_t			llc::_base_log_print_P			(const __FlashStringHelper* text)		{ return (llc_log_print_P && text) ? ::llc_log_print_P(text) : 0; }
#endif

::llc::error_t			llc::setupDefaultLogCallbacks	()	{
	::llc_log_print	= default_base_log_print;
	::llc_log_write	= default_base_log_write;
	return 0;
}

::llc::error_t			llc::setupLogCallbacks
	( llc::log_print_t	funcLogPrint
	, llc::log_write_t	funcLogWrite
	) {
	::llc_log_print	= funcLogPrint;
	::llc_log_write	= funcLogWrite;

	struct llc_debug_checker_struct {
		llc_debug_checker_struct() {
			info_printf("DYNAMIC_LIBRARY_EXTENSION:\"" LLC_DYNAMIC_LIBRARY_EXTENSION "\".");
#	ifdef LLC_ARDUINO
			info_printf("ARDUINO"       );
#	endif // LLC_ARDUINO
#	ifdef LLC_DEBUG_ENABLED
			info_printf("DEBUG_ENABLED" );
#	endif // LLC_DEBUG_ENABLED
#	ifdef LLC_ESP32
			info_printf("ESP32"         );
#	endif // LLC_ESP32
#	ifdef LLC_ESP8266
			info_printf("ESP8266"       );
#	endif // LLC_ESP8266
#	ifdef LLC_ATMEL
			info_printf("ATMEL"         );
#	endif // LLC_ATMEL
#	ifdef LLC_ESPIDF
			info_printf("ESPIDF"        );
#	endif // LLC_ESPIDF
#	ifdef LLC_WINDOWS
			info_printf("WINDOWS"       );
#	endif // LLC_WINDOWS
#	ifdef LLC_LINUX
			info_printf("LINUX"         );
#	endif // LLC_LINUX
#	ifdef LLC_ANDROID
			info_printf("ANDROID"       );
#	endif // LLC_ANDROID
#	ifdef USE_DEBUG_BREAK_ON_ERROR_LOG
			info_printf("USE_DEBUG_BREAK_ON_ERROR_LOG");
#	endif // USE_DEBUG_BREAK_ON_ERROR_LOG
#	ifdef LLC_MTSUPPORT
			info_printf("MTSUPPORT"     );
#	endif // LLC_MTSUPPORT
#	ifdef LLC_DISABLE_CPP_EXCEPTIONS
			info_printf("DISABLE_CPP_EXCEPTIONS");
#	endif // LLC_DISABLE_CPP_EXCEPTIONS
#	ifdef NOMINMAX
			info_printf("NOMINMAX");
#	endif // NOMINMAX
#	ifdef WIN32_LEAN_AND_MEAN
			info_printf("WIN32_LEAN_AND_MEAN");
#	endif // WIN32_LEAN_AND_MEAN
#	ifdef __GNUC__
			info_printf("__GNUC__");
#	endif // __GNUC__
		}
	} llc_debug_checker_instance = {};
	(void)llc_debug_checker_instance;
	return 0;
}

#ifndef LLC_ARDUINO

static	::llc::error_t	getSystemErrorAsString			(const uint64_t lastError, char* buffer, uint32_t bufferSize)			{	// Get the error message, if any.
#ifdef LLC_WINDOWS
	rees_if(0 == buffer);
	return lastError
		? ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, (DWORD)(lastError & 0xFFFFFFFF), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, bufferSize, NULL)
		: 0
		;
#else
	(void) bufferSize;
	sprintf(buffer, "%" LLC_FMT_U2 ".", (uint32_t)lastError);
	return 0;
#endif
}

void					llc::_llc_print_system_errors	(const char* prefix, uint32_t prefixLen)								{
	char								bufferError[256]				= {};
#ifdef LLC_WINDOWS
	int64_t								lastSystemError					= ::GetLastError() & 0xFFFFFFFFFFFFFFFFLL;
#else
	int64_t								lastSystemError					= -1;
#endif
	if(lastSystemError) {
		base_log_write("\n", 1);
		::llc::error_t						stringLength					= ::getSystemErrorAsString((uint64_t)lastSystemError, bufferError, ::llc::size(bufferError));
		base_log_write(prefix, prefixLen);
		base_log_write(bufferError, (uint32_t)stringLength);
		base_log_write("\n", 1);
	}
	lastSystemError					= errno;
	if(lastSystemError) {
		base_log_write("\n", 1);
#ifdef LLC_WINDOWS
		::strerror_s(bufferError, (int32_t)lastSystemError);
		{
#else
		const char * serr = ::strerror((int32_t)lastSystemError);
		if(serr) {
			strcpy_s(bufferError, serr);
#endif
			char		bufferError2[256]	= {};
#ifdef LLC_WINDOWS
			size_t		stringLength		= ::snprintf(bufferError2, ::llc::size(bufferError2) - 2, "Last system error: 0x%llX '%s'.", lastSystemError, bufferError);
#else
			size_t		stringLength		= ::snprintf(bufferError2, ::llc::size(bufferError2) - 2, "Last system error: 0x%llX '%s'.", (unsigned long long)lastSystemError, bufferError);
#endif
			base_log_write(prefix, prefixLen);
			base_log_write(bufferError2, (uint32_t)stringLength);
			base_log_write("\n", 1);
		}
	}
#ifdef LLC_CLEAR_SYSTEM_ERROR_ON_ERROR_LOG
#	ifdef LLC_WINDOWS
#		define llcClearSystemError() SetLastError(0)
#	else
#		define llcClearSystemError() do{} while(0)
#	endif
#else
#	define llcClearSystemError() do{} while(0)
#endif // LLC_CLEAR_SYSTEM_ERROR_ON_ERROR_LOG

	llcClearSystemError();
}

#endif // LLC_ARDUINO
