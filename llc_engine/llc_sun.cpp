#include "llc_sun.h"
#include <ctime>

double					llc::calcSunlightFactor(double daylightRatioExtra, uint32_t daylightOffsetMinutes)	{ 
	const double				sunAngleFactor			= ::llc::calcSunPosition().Dot(::llc::n3f2_t{0, 1}.RotateZ(::llc::SCALE_DAY_MINUTES_TO_RADIANS * daylightOffsetMinutes));
	return ::llc::clamped(sunAngleFactor + daylightRatioExtra, 0.0, 1.0);
}

::llc::n3f2_t			llc::calcSunPosition	()	{ 
	const time_t				now						= time(0);
	tm							ltm						= {};
	localtime_s(&ltm, &now);

	const uint32_t				hourInSeconds			= ltm.tm_hour * 60 * 60;
	const uint32_t				minsInSeconds			= ltm.tm_min * 60;
	const uint32_t				daySeconds				= hourInSeconds + minsInSeconds + ltm.tm_sec;

	return ::llc::n3f2_t{0, -1}.RotateZ(::llc::SCALE_DAY_SECONDS_TO_RADIANS * daySeconds);
}
