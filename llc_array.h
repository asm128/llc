#include "llc_array_pod.h"
#include "llc_array_obj.h"

#ifndef LLC_ARRAY_H_23627
#define LLC_ARRAY_H_23627

namespace llc
{
	tplT	using	aaobj		= ::llc::aobj<::llc::aobj<T>>; 
	tplT	using	aapod		= ::llc::aobj<::llc::apod<T>>; 

	tydf	::llc::aapod<uc_t>	aauc_t, aauchar		;
	tydf	::llc::aapod<sc_t>	aasc_t, aachar		;
	tydf	::llc::aapod<u0_t>	aau0_t, aauint8		, aau8;
	tydf	::llc::aapod<u1_t>	aau1_t, aauint16	, aau16;
	tydf	::llc::aapod<u2_t>	aau2_t, aauint32	, aau32;
	tydf	::llc::aapod<u3_t>	aau3_t, aauint64	, aau64;
	tydf	::llc::aapod<s0_t>	aas0_t, aaint8		, aai8;
	tydf	::llc::aapod<s1_t>	aas1_t, aaint16		, aai16;
	tydf	::llc::aapod<s2_t>	aas2_t, aaint32		, aai32;
	tydf	::llc::aapod<s3_t>	aas3_t, aaint64		, aai64;
	tydf	::llc::aapod<f2_t>	aaf2_t, aafloat		, aaf32;
	tydf	::llc::aapod<f3_t>	aaf3_t, aadouble	, aaf64;
} // namespace 

#endif // LLC_ARRAY_H_23627
