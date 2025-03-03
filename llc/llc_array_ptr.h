#include "llc_aobj_pobj.h"
#include "llc_aobj_ppod.h"
#include "llc_array_pod.h"

#ifndef LLC_ARRAY_PTR_H_23627
#define LLC_ARRAY_PTR_H_23627

namespace llc
{
	tplt<tpnm T> using papod	= ::llc::pobj<::llc::apod<T>>;
	tplt<tpnm T> using apapod	= ::llc::aobj<::llc::papod<T>>;

	tydf	::llc::papod<uc_t	>	pauchar;
	tydf	::llc::papod<char		>	pachar;
	tydf	::llc::papod<float		>	paf32;
	tydf	::llc::papod<double		>	paf64;
	tydf	::llc::papod<uint8_t	>	pau8;
	tydf	::llc::papod<uint16_t	>	pau16;
	tydf	::llc::papod<uint32_t	>	pau32;
	tydf	::llc::papod<uint64_t	>	pau64;
	tydf	::llc::papod<int8_t		>	pai8;
	tydf	::llc::papod<int16_t	>	pai16;
	tydf	::llc::papod<int32_t	>	pai32;
	tydf	::llc::papod<int64_t	>	pai64;

	tydf	::llc::aobj<::llc::pauchar	>	apauchar;
	tydf	::llc::aobj<::llc::pachar	>	apachar;
	tydf	::llc::aobj<::llc::paf32	>	apaf32;
	tydf	::llc::aobj<::llc::paf64	>	apaf64;
	tydf	::llc::aobj<::llc::pau8		>	apau8;
	tydf	::llc::aobj<::llc::pau16	>	apau16;
	tydf	::llc::aobj<::llc::pau32	>	apau32;
	tydf	::llc::aobj<::llc::pau64	>	apau64;
	tydf	::llc::aobj<::llc::pai8		>	apai8;
	tydf	::llc::aobj<::llc::pai16	>	apai16;
	tydf	::llc::aobj<::llc::pai32	>	apai32;
	tydf	::llc::aobj<::llc::pai64	>	apai64;
} // namespace

#endif // LLC_ARRAY_PTR_H_23627
