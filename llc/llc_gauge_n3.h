/// Copyright 2009-2023 - asm128
#include "llc_n3.h"
#include "llc_gauge.h"

#ifndef LLC_GAUGE_N3
#define LLC_GAUGE_N3

namespace llc
{
#pragma pack(push, 1)
	typedef	gaugemax<n3sc_t>	gauge3sc_t;
	typedef	gaugemax<n3u0_t>	gauge3u0_t;
	typedef	gaugemax<n3f2_t>	gauge3f2_t;
	typedef	gaugemax<n3f3_t>	gauge3f3_t;
	typedef	gaugemax<n3u0_t>	gauge3u0_t;
	typedef	gaugemax<n3u1_t>	gauge3u1_t;
	typedef	gaugemax<n3u2_t>	gauge3u2_t;
	typedef	gaugemax<n3u3_t>	gauge3u3_t;
	typedef	gaugemax<n3s0_t>	gauge3s0_t;
	typedef	gaugemax<n3s1_t>	gauge3s1_t;
	typedef	gaugemax<n3s2_t>	gauge3s2_t;
	typedef	gaugemax<n3s3_t>	gauge3s3_t;
#pragma pack(pop)
} // namespace

#endif // LLC_GAUGE_N3
