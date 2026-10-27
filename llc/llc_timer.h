#include "llc_typeint.h"	// for int64_t
#if !defined(LLC_WINDOWS) && !defined(LLC_ATMEL) && !defined(LLC_CMSIS) && !defined(LLC_ARDUINO) && !defined(LLC_ESP32)
#	include <chrono>
#endif


#ifndef LLC_TIMER_H_23627
#define LLC_TIMER_H_23627

namespace llc	//
{
	struct STimer {
		double			LastTimeSeconds			= 0;	// This variable is set by Frame() or Reset() and holds the time difference in seconds between a Frame() call and the next.
		uint64_t		LastTimeMicroseconds	= 0;	// This variable is set by Frame() or Reset() and holds the time difference in seconds between a Frame() call and the next.

		inline			STimer					()		noexcept	{ Reset(); }	// STimer constructor. This function/method executes when we create an instance of STimer without us requiring to call the function directly. It's used to set the initial values of the struct.
		// --
		void			Reset					()		noexcept;	// Reset timer
		double			Frame					()		noexcept;	// Calculate time elapsed since the last Frame() or Reset() call.

	private:
#if defined(LLC_WINDOWS) || defined(LLC_ARDUINO) || defined(LLC_CMSIS) || defined(LLC_ESP32)
		int64_t			PrevTimeStamp			= 0;
#	ifndef LLC_ATMEL
		int64_t			CountsPerSecond			= 0;
		double			SecondsPerCount			= 0;
		double			MicrosecondsPerCount	= 0;
#	endif
#else
		::std::chrono::high_resolution_clock::time_point	PrevTimeStamp		= {};
#endif
	};
}

#endif // LLC_TIMER_H_23627
