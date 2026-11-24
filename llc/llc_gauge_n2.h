/// Copyright 2009-2023 - asm128
#include "llc_n2.h"
#include "llc_gauge.h"

#ifndef LLC_GAUGE_N2
#define LLC_GAUGE_N2

namespace llc
{
#pragma pack(push, 1)
	typedef	gaugemax<n2sc_t>	gauge3sc_t;
	typedef	gaugemax<n2u0_t>	gauge3u0_t;
	typedef	gaugemax<n2f2_t>	gauge3f2_t;
	typedef	gaugemax<n2f3_t>	gauge3f3_t;
	typedef	gaugemax<n2u0_t>	gauge3u0_t;
	typedef	gaugemax<n2u1_t>	gauge3u1_t;
	typedef	gaugemax<n2u2_t>	gauge3u2_t;
	typedef	gaugemax<n2u3_t>	gauge3u3_t;
	typedef	gaugemax<n2s0_t>	gauge3s0_t;
	typedef	gaugemax<n2s1_t>	gauge3s1_t;
	typedef	gaugemax<n2s2_t>	gauge3s2_t;
	typedef	gaugemax<n2s3_t>	gauge3s3_t;
#pragma pack(pop)
} // namespace

#endif // LLC_GAUGE_N2
