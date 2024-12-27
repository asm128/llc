#include "llc_error.h"
#include "llc_sync.h"

#ifdef LLC_ATMEL
#	define LLC_LOG_ARDUINO_FLASHSTRINGHELPER
#endif

#ifdef LLC_ARDUINO
#	ifdef LLC_LOG_ARDUINO_FLASHSTRINGHELPER
#		include <WString.h>
#	endif
#endif

#ifndef LLC_LOG_CORE_H
#define LLC_LOG_CORE_H

namespace llc
{
	tydf	error_t	(*log_print_t)		(const char * text);
	error_t			_base_log_print		(const char * text);
	tydf	error_t	(*log_write_t)		(const char * text, uint32_t textLen);
	error_t			_base_log_write		(const char * text, uint32_t textLen);
#define base_log_write(text, textLen)	::llc::_base_log_write(text, textLen)
#define base_log_print(text)			::llc::_base_log_print(text)

#ifdef LLC_LOG_ARDUINO_FLASHSTRINGHELPER
	tydf	error_t	(*log_print_P_t)	(const __FlashStringHelper * text);
	error_t			_base_log_print_P	(const __FlashStringHelper * text);
#	define base_log_print_P(text)	::llc::_base_log_print_P(text)
#	define base_log_print_F(text)	base_log_print_P(F(text))
#else
#	define base_log_print_P(text)	base_log_print(text)
#	define base_log_print_F(text)	base_log_print(text)
#endif
	error_t			setupDefaultLogCallbacks();
	error_t			setupLogCallbacks
		( log_print_t	funcLogPrint
		, log_write_t	funcLogWrite
		);
	tpl_vtArgs
	sinx	void	dummy				(_tArgs&&...)		{}
} // namespace

#ifdef LLC_WINDOWS
#	define llc_throw(...)								throw(__VA_ARGS__)
#elif defined(LLC_ARDUINO) && defined(LLC_ESP32)
#	define llc_throw(...)								do { ::llc::sleep(500); throw(__VA_ARGS__); } while(0)
#else
#	define llc_throw(...)								do { ::llc::sleep(500); char * nulp = 0; unsigned int i = 0; while(++i) nulp[i] = (char)i; ::llc::dummy(__VA_ARGS__); } while(0)
#endif

#ifndef block_log
#	define block_log(llc_logFunction, statement, textToLog) { llc_logFunction(textToLog); statement; }	// Block with a statement preceded by a log call.
#	ifdef LLC_WINDOWS
#		define block_logf(llc_logFunction, statement, format, ...) { llc_logFunction(format, __VA_ARGS__); statement; }	// Block with a statement preceded by a formatted log call.
#	else
#		define block_logf(llc_logFunction, statement, format, ...) { llc_logFunction(format, ##__VA_ARGS__); statement; }	// Block with a statement preceded by a formatted log call.
#	endif
#endif

// - Conditional log of condition.
#ifndef if_true_log
#	define if_true_log(llc_logFunction, condition)									if(condition) { llc_logFunction(#condition); }											// - Log condition if (condition) == true.
#	define if_true_block_log(llc_logFunction, condition, statement)					if(condition) block_log(llc_logFunction, statement, #condition)							// - Conditional block with custom statement preceded by a log call.
#endif // if_true_log

// - Conditional log formatted.
#ifndef if_true_logf
#	ifdef LLC_WINDOWS					
#		define if_true_logf(llc_logFunction, condition, format, ...)						if(condition) { llc_logFunction(#condition "->" format, __VA_ARGS__); }					// - Log condition and custom formatted string args if (condition) == true.
#		define if_true_block_logf(llc_logFunction, condition, statement, format, ...)		if(condition) block_logf(llc_logFunction, statement, #condition "->" format, __VA_ARGS__)	// - Conditional block with custom statement preceded by a log call with custom formatted string args.
#	else
#		ifdef LLC_ATMEL																																																											
#			define if_true_logf(llc_logFunction, condition, format, ...)					if_true_log			(llc_logFunction, condition)				// - Log condition and custom formatted string args if (condition) == true.
#			define if_true_block_logf(llc_logFunction, condition, statement, format, ...)	if_true_block_log	(llc_logFunction, condition, statement) 	// - Conditional block with custom statement preceded by a log call with custom formatted string args.
#		else
#			define if_true_logf(llc_logFunction, condition, format, ...)					if(condition) { llc_logFunction(#condition "->" format, ##__VA_ARGS__); }						// - Log condition and custom formatted string args if (condition) == true.
#			define if_true_block_logf(llc_logFunction, condition, statement, format, ...)	if(condition) block_logf(llc_logFunction, statement, #condition "->" format, ##__VA_ARGS__)		// - Conditional block with custom statement preceded by a log call with custom formatted string args.
#		endif
#	endif
#endif // if_true_logf

#ifdef if_true_log
#	define if_true_log_and_break(llc_logFunction, condition)						if_true_block_log(llc_logFunction, condition, break)					// - Log condition and break if (condition) == true.
#	define if_true_log_and_continue(llc_logFunction, condition)						if_true_block_log(llc_logFunction, condition, continue)					// - Log condition and continue if (condition) == true.
#	define if_true_log_and_throw(llc_logFunction, condition)						if_true_block_log(llc_logFunction, condition, llc_throw(#condition))	// - Log condition and return if (condition) == true.
#	define if_true_log_and_return(llc_logFunction, condition)						if_true_block_log(llc_logFunction, condition, return)					// - Log condition and return if (condition) == true.
#	define if_true_log_and_return_value(llc_logFunction, value, condition)			if_true_block_log(llc_logFunction, condition, return (value))	// - Log condition and return some value if (condition) == true.
#	define if_true_log_and_fail(llc_logFunction, condition)							if_true_log_and_return_value(llc_logFunction, -1, condition)			// - Log condition and return some value if (condition) == true.
#endif // if_true_log
#ifdef if_true_logf
#	define if_true_logf_and_break(llc_logFunction, condition, format, ...)					if_true_block_logf(llc_logFunction, condition, break, format, __VA_ARGS__)			// - Log formatted string and break if (condition) == true.
#	define if_true_logf_and_continue(llc_logFunction, condition, format, ...)				if_true_block_logf(llc_logFunction, condition, continue, format, __VA_ARGS__)			// - Log formatted string and continue if (condition) == true.
#	define if_true_logf_and_throw(llc_logFunction, condition, format, ...)					if_true_block_logf(llc_logFunction, condition, llc_throw(#condition), format, __VA_ARGS__)			// - Log condition and return if (condition) == true.
#	define if_true_logf_and_return(llc_logFunction, condition, format, ...)					if_true_block_logf(llc_logFunction, condition, return, format, __VA_ARGS__)			// - Log formatted string and return if (condition) == true.
#	define if_true_logf_and_return_value(llc_logFunction, value, condition, format, ...)	if_true_block_logf(llc_logFunction, condition, return (value), format, __VA_ARGS__)	// - Log formatted string and return some value if (condition) == true.
#	define if_true_logf_and_fail(llc_logFunction, condition, format, ...)					if_true_logf_and_return_value(llc_logFunction, -1, condition, format, __VA_ARGS__)	// - Log formatted string and return -1 if (condition) == true.
#endif // if_true_logf

#ifdef if_true_log
#	ifndef if_fail_log
#		define if_fail_log(llc_logFunction, condition)									if_true_log					(llc_logFunction, ::llc::failed(condition))				// - if condition < 0 -> log
#		define if_fail_log_and_break(llc_logFunction, condition)						if_true_log_and_break		(llc_logFunction, ::llc::failed(condition))				// - if condition < 0 -> break
#		define if_fail_log_and_continue(llc_logFunction, condition)						if_true_log_and_continue	(llc_logFunction, ::llc::failed(condition))				// - if condition < 0 -> continue
#		define if_fail_log_and_throw(llc_logFunction, condition)						if_true_log_and_throw		(llc_logFunction, ::llc::failed(condition))				// - if condition < 0 -> return
#		define if_fail_log_and_return(llc_logFunction, condition)						if_true_log_and_return		(llc_logFunction, ::llc::failed(condition))				// - if condition < 0 -> return
#		define if_fail_log_and_return_value(llc_logFunction, value, condition)			if_true_log_and_return_value(llc_logFunction, (value), ::llc::failed(condition))	// - if condition < 0 -> return value
#		define if_fail_log_and_fail(llc_logFunction, condition)							if_true_log_and_fail		(llc_logFunction, ::llc::failed(condition))				// - if condition < 0 -> return -1
#		define if_zero_log(llc_logFunction, condition)									if_true_log					(llc_logFunction, 0 == (condition))						// - if condition == 0 -> log
#		define if_zero_log_and_break(llc_logFunction, condition)						if_true_log_and_break		(llc_logFunction, 0 == (condition))						// - if condition == 0 -> break
#		define if_zero_log_and_continue(llc_logFunction, condition)						if_true_log_and_continue	(llc_logFunction, 0 == (condition))						// - if condition == 0 -> continue
#		define if_zero_log_and_throw(llc_logFunction, condition)						if_true_log_and_throw		(llc_logFunction, 0 == (condition))						// - if condition == 0 -> return
#		define if_zero_log_and_return(llc_logFunction, condition)						if_true_log_and_return		(llc_logFunction, 0 == (condition))						// - if condition == 0 -> return
#		define if_zero_log_and_return_value(llc_logFunction, value, condition)			if_true_log_and_return_value(llc_logFunction, (value), 0 == (condition))			// - if condition == 0 -> return value
#		define if_zero_log_and_fail(llc_logFunction, condition)							if_true_log_and_fail		(llc_logFunction, 0 == (condition))						// - if condition == 0 -> return -1
#		define if_null_log																if_zero_log																			// - if condition == 0 -> log
#		define if_null_log_and_break													if_zero_log_and_break																// - if condition == 0 -> break
#		define if_null_log_and_continue													if_zero_log_and_continue															// - if condition == 0 -> continue
#		define if_null_log_and_throw													if_zero_log_and_throw																// - if condition == 0 -> return
#		define if_null_log_and_return													if_zero_log_and_return																// - if condition == 0 -> return
#		define if_null_log_and_return_value												if_zero_log_and_return_value														// - if condition == 0 -> return value
#		define if_null_log_and_fail														if_zero_log_and_fail																// - if condition == 0 -> return -1
#	endif // if_fail_log
#endif // if_true_log

#ifdef if_true_logf
#	ifndef if_fail_logf
#		define if_fail_logf(llc_logFunction, condition, format, ...)							if_true_logf					(llc_logFunction, ::llc::failed(condition), format, __VA_ARGS__)			// - if condition < 0 -> log
#		define if_fail_logf_and_break(llc_logFunction, condition, format, ...)					if_true_logf_and_break			(llc_logFunction, ::llc::failed(condition), format, __VA_ARGS__)			// - if condition < 0 -> break
#		define if_fail_logf_and_continue(llc_logFunction, condition, format, ...)				if_true_logf_and_continue		(llc_logFunction, ::llc::failed(condition), format, __VA_ARGS__)			// - if condition < 0 -> continue
#		define if_fail_logf_and_throw(llc_logFunction, condition, format, ...)					if_true_logf_and_throw			(llc_logFunction, ::llc::failed(condition), format, __VA_ARGS__)			// - if condition < 0 -> return
#		define if_fail_logf_and_return(llc_logFunction, condition, format, ...)					if_true_logf_and_return			(llc_logFunction, ::llc::failed(condition), format, __VA_ARGS__)			// - if condition < 0 -> return
#		define if_fail_logf_and_return_value(llc_logFunction, value, condition, format, ...)	if_true_logf_and_return_value	(llc_logFunction, (value), ::llc::failed(condition), format, __VA_ARGS__)	// - if condition < 0 -> return value
#		define if_fail_logf_and_fail(llc_logFunction, condition, format, ...)					if_true_logf_and_fail			(llc_logFunction, ::llc::failed(condition), format, __VA_ARGS__)			// - if condition < 0 -> return -1
#		define if_zero_logf(llc_logFunction, condition, format, ...)							if_true_logf					(llc_logFunction, 0 == (condition), format, __VA_ARGS__)					// - if condition == 0 -> log
#		define if_zero_logf_and_break(llc_logFunction, condition, format, ...)					if_true_logf_and_break			(llc_logFunction, 0 == (condition), format, __VA_ARGS__)					// - if condition == 0 -> break
#		define if_zero_logf_and_continue(llc_logFunction, condition, format, ...)				if_true_logf_and_continue		(llc_logFunction, 0 == (condition), format, __VA_ARGS__)					// - if condition == 0 -> continue
#		define if_zero_logf_and_throw(llc_logFunction, condition, format, ...)					if_true_logf_and_throw			(llc_logFunction, 0 == (condition), format, __VA_ARGS__)					// - if condition == 0 -> return
#		define if_zero_logf_and_return(llc_logFunction, condition, format, ...)					if_true_logf_and_return			(llc_logFunction, 0 == (condition), format, __VA_ARGS__)					// - if condition == 0 -> return
#		define if_zero_logf_and_return_value(llc_logFunction, value, condition, format, ...)	if_true_logf_and_return_value	(llc_logFunction, (value), 0 == (condition), format, __VA_ARGS__)			// - if condition == 0 -> return value
#		define if_zero_logf_and_fail(llc_logFunction, condition, format, ...)					if_true_logf_and_fail			(llc_logFunction, 0 == (condition), format, __VA_ARGS__)					// - if condition == 0 -> return -1
#		define if_null_logf						if_zero_logf					// - if condition == 0 -> log
#		define if_null_logf_and_break			if_zero_logf_and_break			// - if condition == 0 -> break
#		define if_null_logf_and_continue		if_zero_logf_and_continue		// - if condition == 0 -> continue
#		define if_null_logf_and_throw			if_zero_logf_and_throw			// - if condition == 0 -> return
#		define if_null_logf_and_return			if_zero_logf_and_return			// - if condition == 0 -> return
#		define if_null_logf_and_return_value	if_zero_logf_and_return_value	// - if condition == 0 -> return value
#		define if_null_logf_and_fail			if_zero_logf_and_fail			// - if condition == 0 -> return -1
#	endif // if_fail_logf
#endif // if_true_logf

#endif // LLC_LOG_CORE_H
