#include "llc_array_pod.h"
#include "llc_range.h"

#ifndef LLC_APOD_RANGE_H
#define LLC_APOD_RANGE_H

namespace llc
{
	tplT using	arange	= ::llc::apod<::llc::range<T>>;

	tydf	::llc::arange<sc_t>	arangesc_t;	tdcs	arangesc_t	arangesc_c;
	tydf	::llc::arange<uc_t>	arangeuc_t;	tdcs	arangeuc_t	arangeuc_c;
	tydf	::llc::arange<u0_t>	arangeu0_t;	tdcs	arangeu0_t	arangeu0_c;
	tydf	::llc::arange<u1_t>	arangeu1_t;	tdcs	arangeu1_t	arangeu1_c;
	tydf	::llc::arange<u2_t>	arangeu2_t;	tdcs	arangeu2_t	arangeu2_c;
	tydf	::llc::arange<u3_t>	arangeu3_t;	tdcs	arangeu3_t	arangeu3_c;
	tydf	::llc::arange<s0_t>	aranges0_t;	tdcs	aranges0_t	aranges0_c;
	tydf	::llc::arange<s1_t>	aranges1_t;	tdcs	aranges1_t	aranges1_c;
	tydf	::llc::arange<s2_t>	aranges2_t;	tdcs	aranges2_t	aranges2_c;
	tydf	::llc::arange<s3_t>	aranges3_t;	tdcs	aranges3_t	aranges3_c;
	tydf	::llc::arange<f2_t>	arangef2_t;	tdcs	arangef2_t	arangef2_c;
	tydf	::llc::arange<f3_t>	arangef3_t;	tdcs	arangef3_t	arangef3_c;
}

#endif // LLC_APOD_RANGE_H