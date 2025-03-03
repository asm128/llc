#include "llc_typeint.h"
#include "llc_timer.h"

#ifndef LLC_FRAMEINFO_H_23627
#define LLC_FRAMEINFO_H_23627

namespace llc
{
#pragma pack(push, 1)
	stct SFrameSeconds {
		double					Total				;
		double					LastFrame			;
		double					LastFrameHalfSquared;
		// Helper
		double					UpdateFromTime		(double secondsToAdd)			noexcept	{
			Total					+= LastFrame = secondsToAdd;
			LastFrameHalfSquared	=  secondsToAdd * secondsToAdd * 0.5;
			return LastFrame;
		}
		inline	double			UpdateFromTimeUs	(uint64_t microsecondsToAdd)	noexcept	{
			return UpdateFromTime(microsecondsToAdd * .000001);
		}
	};

	stct SFrameMicroseconds {
		uint64_t				Total				;
		uint64_t				LastFrame			;
		// Helper
		inline	void			UpdateFromTime		(uint64_t microsecondsToAdd)	noexcept	{ Total += LastFrame = microsecondsToAdd; }
	};

	stct SFrameMeter {
		double					AverageFrameTime	= 0;
		uint64_t				FrameNumber			= 0;
		uint32_t				FramesPerSecond		= 0;
		uint32_t				FramesThisSecond	= 0;
		uint32_t				FrameStep			= 0;
		// Updates frame number and stats from delta time.
		uint64_t				Frame				(uint64_t deltaMicroseconds)	{
			++FrameNumber;
			++FramesThisSecond;
			FrameStep				+= (uint32_t)deltaMicroseconds;
			if(FrameStep > 1000000 || FramesThisSecond > 30) {
				FramesPerSecond			= uint32_t(FramesThisSecond / (FrameStep * .000001));
				FramesThisSecond		= 0;
				FrameStep				= 0;
				AverageFrameTime		= 1.0f / FramesPerSecond;
			}
			return FrameNumber;
		}
	};

	stct SFrameInfo {
		SFrameMicroseconds		Microseconds		= {};
		SFrameSeconds			Seconds				= {};
		SFrameMeter				FrameMeter			= {};
		// Sets last frame time and number. Returns delta seconds since last time.
		double					Frame				(uint64_t deltaMicroseconds)						{
			Microseconds	.UpdateFromTime(deltaMicroseconds);
			Seconds			.UpdateFromTimeUs(deltaMicroseconds);
			FrameMeter		.Frame(deltaMicroseconds);
			return Seconds.LastFrame;
		}
	};

	stct SFrameTimer {
		::llc::STimer			Timer				= {};
		::llc::SFrameInfo		FrameInfo			= {};

		// Returns delta seconds since the previous `Frame()` call. Gets delta from timer and use it to update FrameInfo.
		inline	double			Frame				() {
			Timer.Frame();
			return FrameInfo.Frame(Timer.LastTimeMicroseconds);
		}		
	};	
#pragma pack(pop)


} // namespace

#endif // LLC_FRAMEINFO_H_23627
