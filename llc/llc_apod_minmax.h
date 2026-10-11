#include "llc_array_pod.h"
#include "llc_minmax.h"

#ifndef LLC_APOD_MINMAX_H
#define LLC_APOD_MINMAX_H

namespace llc
{
	tplT using	aminmax	= ::llc::apod<::llc::minmax<T>>;

	tydf	::llc::aminmax<sc_t>	aminmaxsc_t;	tdcs	aminmaxsc_t	aminmaxsc_c;
	tydf	::llc::aminmax<uc_t>	aminmaxuc_t;	tdcs	aminmaxuc_t	aminmaxuc_c;
	tydf	::llc::aminmax<u0_t>	aminmaxu0_t;	tdcs	aminmaxu0_t	aminmaxu0_c;
	tydf	::llc::aminmax<u1_t>	aminmaxu1_t;	tdcs	aminmaxu1_t	aminmaxu1_c;
	tydf	::llc::aminmax<u2_t>	aminmaxu2_t;	tdcs	aminmaxu2_t	aminmaxu2_c;
	tydf	::llc::aminmax<u3_t>	aminmaxu3_t;	tdcs	aminmaxu3_t	aminmaxu3_c;
	tydf	::llc::aminmax<s0_t>	aminmaxs0_t;	tdcs	aminmaxs0_t	aminmaxs0_c;
	tydf	::llc::aminmax<s1_t>	aminmaxs1_t;	tdcs	aminmaxs1_t	aminmaxs1_c;
	tydf	::llc::aminmax<s2_t>	aminmaxs2_t;	tdcs	aminmaxs2_t	aminmaxs2_c;
	tydf	::llc::aminmax<s3_t>	aminmaxs3_t;	tdcs	aminmaxs3_t	aminmaxs3_c;
	tydf	::llc::aminmax<f2_t>	aminmaxf2_t;	tdcs	aminmaxf2_t	aminmaxf2_c;
	tydf	::llc::aminmax<f3_t>	aminmaxf3_t;	tdcs	aminmaxf3_t	aminmaxf3_c;
} // namespace llc

#endif // LLC_APOD_MINMAX_H
