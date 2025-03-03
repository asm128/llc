#include "llc_error.h"

#ifndef LLC_SYNC_H_23627
#define LLC_SYNC_H_23627

#ifdef LLC_WINDOWS
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#	include <thread>
#	include <chrono>
#elif defined(LLC_ARDUINO)
#	include <Arduino.h>
//#elif defined(LLC_ANDROID) || defined(LLC_LINUX) || defined(LLC_ESP32)
#else
#	include <thread>
#	include <chrono>
#endif
namespace llc
{
#ifdef LLC_WINDOWS
	stin err_t	sleep	(u2_t milliseconds)		noexcept	{ Sleep(milliseconds); return (err_t)milliseconds; }
	stin u3_t	sleepUs	(u3_t microseconds)		noexcept	{ std::this_thread::sleep_for(std::chrono::microseconds(microseconds)); return (s3_t)microseconds; }
#elif defined(LLC_ARDUINO)
	stin err_t	sleep	(u2_t milliseconds)		noexcept	{ ::delay(milliseconds); return (err_t)milliseconds; }
	stin u3_t	sleepUs	(u3_t microseconds)		noexcept	{ ::delayMicroseconds(microseconds); return microseconds; }
#else
	stin err_t	sleep	(u2_t milliseconds)		noexcept	{ std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds)); return (err_t)milliseconds; }
	stin u3_t	sleepUs	(u3_t microseconds)		noexcept	{ std::this_thread::sleep_for(std::chrono::microseconds(microseconds)); return microseconds; }
#endif

#ifndef LLC_MTSUPPORT
#	define llc_sync_increment(nCount)							(++(nCount))
#	define llc_sync_decrement(nCount)							(--(nCount))
#	define llc_sync_exchange(Target, Value)						((Target) = (Value))
#	define llc_sync_compare_exchange(target, value, comparand)	((target) = ((target) == (comparand)) ? (value) : (target))
#else
#	if defined(LLC_ANDROID) || defined(LLC_LINUX)
/// <Atomic Builtins> http://gcc.gnu.org/onlinedocs/gcc-4.4.3/gcc/Atomic-Builtins.html#Atomic-Builtins
#		define llc_sync_increment(nCount)							(__sync_add_and_fetch(&(nCount), 1))
#		define llc_sync_decrement(nCount)							(__sync_sub_and_fetch(&(nCount), 1))
#		define llc_sync_exchange(target, value)						(__sync_lock_test_and_set(&(target), (value)))
#		define llc_sync_compare_exchange(nCount, value, comparand)	(__sync_val_compare_and_swap(&(nCount), (comparand), (value)))
#	elif !defined(LLC_WINDOWS)
#		define llc_sync_increment(nCount)							(++(nCount))													//	include <atomic>
#		define llc_sync_decrement(nCount)							(--(nCount))													//	define llc__sync_increment(nCount)			::std::atomic_fetch_add(&nCount, 1)
#		define llc_sync_exchange(Target, Value)						((Target) = (Value))											//	define llc__sync_decrement(nCount)			::std::atomic_fetch_sub(&nCount, 1)
#		define llc_sync_compare_exchange(target, value, comparand)	((target) = ((target) == (comparand)) ? (value) : (target))		//	define llc__sync_exchange(Target, Value)	((Target) = (Value))
#	elif defined(_WIN64) || defined(WIN64)
#		define llc_sync_increment(nCount)							InterlockedIncrement64			(&(nCount))
#		define llc_sync_decrement(nCount)							InterlockedDecrement64			(&(nCount))
#		define llc_sync_exchange(target, value)						InterlockedExchange64			(&(target), (value))
#		define llc_sync_compare_exchange(target, value, comparand)	InterlockedCompareExchange64	(&(target), (value),(comparand))
#	elif defined(_WIN32) || defined(WIN32)
#		define llc_sync_increment(nCount)							InterlockedIncrement		(&(nCount))
#		define llc_sync_decrement(nCount)							InterlockedDecrement		(&(nCount))
#		define llc_sync_exchange(target, value)						InterlockedExchange			(&(target), (value))
#		define llc_sync_compare_exchange(target, value, comparand)	InterlockedCompareExchange	(&(target), (value),(comparand))
#	endif
#endif
}

#endif // LLC_SYNC_H_23627
