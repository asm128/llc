#include "llc_sun.h"
#include <ctime>

LLC_USING_TYPEINT();
LLC_USING_N2();
LLC_USING_N3();

f3_t					llc::calcSunlightFactor(f3_t daylightRatioExtra, u2_t daylightOffsetMinutes)	{ 
	const f3_t				sunAngleFactor			= ::llc::calcSunPosition().Dot(::llc::n3f2_t{0, 1}.RotateZ(::llc::SCALE_DAY_MINUTES_TO_RADIANS * daylightOffsetMinutes));
	return ::llc::clamped(sunAngleFactor + daylightRatioExtra, 0.0, 1.0);
}

::llc::n3f2_t			llc::calcSunPosition	()	{ 
	const time_t				now						= time(0);
	tm							ltm						= {};
	localtime_s(&ltm, &now);

	const u2_t				hourInSeconds			= ltm.tm_hour * 60 * 60;
	const u2_t				minsInSeconds			= ltm.tm_min * 60;
	const u2_t				daySeconds				= hourInSeconds + minsInSeconds + ltm.tm_sec;

	return ::llc::n3f2_t{0, -1}.RotateZ(::llc::SCALE_DAY_SECONDS_TO_RADIANS * daySeconds);
}
