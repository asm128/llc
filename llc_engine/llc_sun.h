#include "llc_n3.h"

#ifndef LLC_SUN_H_23627
#define LLC_SUN_H_23627

namespace llc
{
	stxp	double	SCALE_DAY_SECONDS				= 1.0 / 86400;
	stxp	double	SCALE_DAY_SECONDS_TO_RADIANS	= ::llc::math_2pi * SCALE_DAY_SECONDS;
	stxp	double	SCALE_DAY_MINUTES_TO_RADIANS	= SCALE_DAY_SECONDS_TO_RADIANS * 60;

	::llc::n3f2_t	calcSunPosition					();
	double			calcSunlightFactor				(double daylightRatioExtra = .5, uint32_t daylightOffsetMinutes = 90);
} // namespace

#endif // LLC_SUN_H_23627
