#include "llc_typeint.h"	// for s3_t

#if !(defined(LLC_WINDOWS) || defined(LLC_CMSIS) || defined(LLC_ARDUINO))
#	include <chrono>
#endif

#ifndef LLC_TIMER_H_23627
#define LLC_TIMER_H_23627

namespace llc	//
{
	stct STimer {
		f3_t			LastTimeSeconds			= 0;	// This variable is set by Frame() or Reset() and holds the time difference in seconds between a Frame() call and the next.
		u3_t		LastTimeMicroseconds	= 0;	// This variable is set by Frame() or Reset() and holds the time difference in seconds between a Frame() call and the next.

		inline			STimer					()		noexcept	{ Reset(); }	// STimer constructor. This function/method executes when we create an instance of STimer without us requiring to call the function directly. It's used to set the initial values of the struct.
		// --
		void			Reset					()		noexcept;	// Reset timer
		f3_t			Frame					()		noexcept;	// Calculate time elapsed since the last Frame() or Reset() call.

#if !(defined(LLC_WINDOWS) || defined(LLC_ARDUINO) || defined(LLC_CMSIS))
		::std::chrono::high_resolution_clock::time_point	PrevTimeStamp		= {};
#else // (LLC_WINDOWS || LLC_ARDUINO || LLC_CMSIS)
		s3_t			PrevTimeStamp			= 0;
#	ifndef LLC_ATMEL
		s3_t			CountsPerSecond			= 0;
		f3_t			SecondsPerCount			= 0;
		f3_t			MicrosecondsPerCount	= 0;
#	endif // LLC_ATMEL
#endif // !(LLC_WINDOWS || LLC_ARDUINO || LLC_CMSIS)
	}; // STimer
} // namespace

#endif // LLC_TIMER_H_23627
