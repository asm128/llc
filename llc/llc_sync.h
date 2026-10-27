#include "llc_typeint.h"
#include "llc_error.h"

#ifndef LLC_SYNC_H_23627
#define LLC_SYNC_H_23627

#if defined(LLC_WINDOWS)
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#	include <thread>
#	include <chrono>
#elif defined(LLC_ARDUINO)
#	include <Arduino.h>
#else
#	include <thread>
#	include <chrono>
#endif
namespace llc
{
#if defined(LLC_ANDROID) || defined(LLC_LINUX)
/// <Atomic Builtins> http://gcc.gnu.org/onlinedocs/gcc-4.4.3/gcc/Atomic-Builtins.html#Atomic-Builtins
#	define llc__sync_increment(nCount)							(__sync_add_and_fetch(&nCount, 1))
#	define llc__sync_decrement(nCount)							(__sync_sub_and_fetch(&nCount, 1))
#	define llc__sync_exchange(target, value)					(__sync_lock_test_and_set(&(target), (value)))
#	define llc__sync_compare_exchange(nCount, value, comparand)	(__sync_val_compare_and_swap(&(nCount), (comparand), (value)))
#	define DECLARE_SHARED_SECTION(Name)							::std::mutex Name //(true)
#	define INIT_SHARED_SECTION(Name)							(1)
#	define ENTER_SHARED_SECTION(Name)							(Name).lock()
#	define LEAVE_SHARED_SECTION(Name)							(Name).unlock()
#	define DELETE_SHARED_SECTION(Name)							(0)

	stin ::llc::err_t	sleep	(u2_t milliseconds)		noexcept	{ std::this_thread::sleep_for (std::chrono::milliseconds(milliseconds)); return 0; }
#elif defined(LLC_WINDOWS)
#	if (defined( _WIN64 ) || defined( WIN64 ))
#		define llc__sync_increment(nCount)							( InterlockedIncrement64		( &(nCount) ) )
#		define llc__sync_decrement(nCount)							( InterlockedDecrement64		( &(nCount) ) )
#		define llc__sync_exchange(target, value)					( InterlockedExchange64			( &(target), (value) ) )
#		define llc__sync_compare_exchange(target, value, comparand)	( InterlockedCompareExchange64	( &(target), (value), (comparand) ) )
#	elif (defined( _WIN32 ) || defined( WIN32 ))
#		define llc__sync_increment(nCount)							( InterlockedIncrement			( &(nCount) ) )
#		define llc__sync_decrement(nCount)							( InterlockedDecrement			( &(nCount) ) )
#		define llc__sync_exchange(target, value)					( InterlockedExchange			( &(target), (value) ) )
#		define llc__sync_compare_exchange(target, value, comparand)	( InterlockedCompareExchange	( &(target), (value), (comparand) ) )
#	endif
#	define DECLARE_SHARED_SECTION(Name)			CRITICAL_SECTION Name
#	define INIT_SHARED_SECTION(Name)			((int32_t)InitializeCriticalSectionAndSpinCount(&Name, 400))
#	define ENTER_SHARED_SECTION(Name)			EnterCriticalSection (&Name)
#	define LEAVE_SHARED_SECTION(Name)			LeaveCriticalSection (&Name)
#	define DELETE_SHARED_SECTION(Name)			DeleteCriticalSection(&Name)

	stin	::llc::err_t	sleep	(u2_t milliseconds)		noexcept	{ Sleep(milliseconds); return 0; }
#else
#	define llc__sync_increment(nCount)			(++(nCount))
#	define llc__sync_decrement(nCount)			(--(nCount))
#	define llc__sync_exchange(Target, Value)	((Target) = (Value))
#	define DECLARE_SHARED_SECTION(Name)			::std::mutex Name //(true)
#	define INIT_SHARED_SECTION(Name)			(1)
#	define ENTER_SHARED_SECTION(Name)			(Name).lock()
#	define LEAVE_SHARED_SECTION(Name)			(Name).unlock()
#	define DELETE_SHARED_SECTION(Name)			(0)

#	ifdef LLC_ARDUINO
	stin ::llc::err_t	sleep	(u2_t milliseconds)		noexcept	{ ::delay(milliseconds); return 0; }
#	else
	stin ::llc::err_t	sleep	(u2_t milliseconds)		noexcept	{ std::this_thread::sleep_for (std::chrono::milliseconds(milliseconds)); return 0; }
#	endif
#endif

#if defined(LLC_ARDUINO)

	stin u3_t	 sleepUs	(u3_t microseconds)		nxpt	{ ::delayMicroseconds(microseconds); return microseconds; }
#else

	stin u3_t	 sleepUs	(u3_t microseconds)		nxpt	{ ::std::this_thread::sleep_for(::std::chrono::microseconds(microseconds)); return microseconds; }
#endif

#if defined(LLC_MTSUPPORT)
#	define DECLARE_CRITICAL_SECTION					DECLARE_SHARED_SECTION
#	define INIT_CRITICAL_SECTION					INIT_SHARED_SECTION
#	define ENTER_CRITICAL_SECTION					ENTER_SHARED_SECTION
#	define LEAVE_CRITICAL_SECTION					LEAVE_SHARED_SECTION
#	define DELETE_CRITICAL_SECTION					DELETE_SHARED_SECTION
#	if defined(LLC_ANDROID) || defined(LLC_LINUX)
#		define llc_sync_increment						llc__sync_increment
#		define llc_sync_decrement						llc__sync_decrement
#		define llc_sync_exchange						llc__sync_exchange
#		define llc_sync_compare_exchange				llc__sync_compare_exchange
#	elif defined(LLC_WINDOWS)
#		if (defined( _WIN64 ) || defined( WIN64 ))
#			define llc_sync_increment						llc__sync_increment
#			define llc_sync_decrement						llc__sync_decrement
#			define llc_sync_exchange						llc__sync_exchange
#			define llc_sync_compare_exchange				llc__sync_compare_exchange
#		elif (defined( _WIN32 ) || defined( WIN32 ))
#			define llc_sync_increment						llc__sync_increment
#			define llc_sync_decrement						llc__sync_decrement
#			define llc_sync_exchange						llc__sync_exchange
#			define llc_sync_compare_exchange				llc__sync_compare_exchange
#		endif
#	else
#		define llc_sync_increment						llc__sync_increment
#		define llc_sync_decrement						llc__sync_decrement
#		define llc_sync_exchange						llc__sync_exchange
#		define llc_sync_compare_exchange				llc__sync_compare_exchange
#	endif
#else
#	define llc_sync_increment(nCount)					(++(nCount))
#	define llc_sync_decrement(nCount)					(--(nCount))
#	define llc_sync_exchange(target, value)				((target) = (value))
#	define llc_sync_compare_exchange(target, value, comparand)	((target) = ((target) == (comparand)) ? (value) : (target))

#	define DECLARE_CRITICAL_SECTION(...)
#	define INIT_CRITICAL_SECTION(...)					(1)
#	define ENTER_CRITICAL_SECTION(...)					(0)
#	define LEAVE_CRITICAL_SECTION(...)					(0)
#	define DELETE_CRITICAL_SECTION(...)					do {} while(false)
#endif
}

#endif // LLC_SYNC_H_23627
