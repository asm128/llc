#include "llc_timer.h"

#ifdef LLC_WINDOWS
#	include <Windows.h>
#elif defined(LLC_ARDUINO)
#	include <Arduino.h>
#elif defined(LLC_CMSIS)
#endif

LLC_USING_TYPEINT();

stxp	f3_t			MICROSECOND_SCALE		= 0.000001;

void					llc::STimer::Reset		()				noexcept				{
	LastTimeSeconds			= f3_t(LastTimeMicroseconds = 0);
#if !(defined(LLC_WINDOWS) || defined(LLC_CMSIS))
#	ifdef LLC_ARDUINO
	PrevTimeStamp			= micros();
#	else // !LLC_ARDUINO
	PrevTimeStamp			= ::std::chrono::high_resolution_clock::now();
#	endif // LLC_ARDUINO
#else // (LLC_WINDOWS || LLC_CMSIS)
#	ifdef LLC_WINDOWS
	QueryPerformanceFrequency((::LARGE_INTEGER*)&CountsPerSecond);
	QueryPerformanceCounter((::LARGE_INTEGER*)&PrevTimeStamp);
#	elif defined(LLC_CMSIS)
	CountsPerSecond			= osKernelGetTickFreq();
	PrevTimeStamp			= osKernelGetTickCount();
#	endif // LLC_WINDOWS
	SecondsPerCount			= 1.0 / CountsPerSecond;
	MicrosecondsPerCount	= 1.0 / (CountsPerSecond * MICROSECOND_SCALE);
#endif // !(LLC_WINDOWS || LLC_CMSIS)
}

f3_t			llc::STimer::Frame		()				noexcept				{
#if defined(LLC_WINDOWS) || defined(LLC_CMSIS) 
#	ifdef LLC_WINDOWS
	s3_t				currentTimeStamp		= 0;
	QueryPerformanceCounter((::LARGE_INTEGER*)&currentTimeStamp);
#	elif defined(LLC_CMSIS)
	s3_t				currentTimeStamp		= osKernelGetTickCount();
#	endif // LLC_WINDOWS
	LastTimeSeconds			= (currentTimeStamp - PrevTimeStamp) * SecondsPerCount;
	LastTimeMicroseconds	= u3_t((currentTimeStamp - PrevTimeStamp) * MicrosecondsPerCount);
	PrevTimeStamp			= currentTimeStamp;
	return LastTimeSeconds;
#else // !(LLC_WINDOWS || LLC_CMSIS) 
#	ifdef LLC_ARDUINO
	s3_t		currentTimeStamp	= (s3_t)micros();
	LastTimeMicroseconds	= currentTimeStamp - PrevTimeStamp;
#	else // !LLC_ARDUINO
	auto		currentTimeStamp		= ::std::chrono::high_resolution_clock::now();
	LastTimeMicroseconds	= (s3_t)::std::chrono::duration_cast<std::chrono::microseconds>(currentTimeStamp - PrevTimeStamp).count();
#	endif // LLC_ARDUINO
	PrevTimeStamp			= currentTimeStamp;
	return LastTimeSeconds	= LastTimeMicroseconds * MICROSECOND_SCALE;
#endif // LLC_WINDOWS || LLC_CMSIS
}
