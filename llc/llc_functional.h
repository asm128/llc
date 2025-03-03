#include "llc_error.h"

#ifdef LLC_ATMEL
#	include <ustd_functional.h>
#else
#	include <functional>
#endif

#ifndef LLC_FUNCTIONAL_H_23627
#define LLC_FUNCTIONAL_H_23627

namespace llc
{
#ifdef LLC_ATMEL
	tplt<tpnm _tFunction>	
	using	function	= ::ustd::function<_tFunction>;
#else
	tplt<tpnm _tFunction>	
	using	function	= ::std::function<_tFunction>;
#endif

	tplt<tpnm ..._tArgs>					using	FVoid				= ::llc::function<void(_tArgs&&...)>;
	tplt<tpnm ..._tArgs>					using	FBool				= ::llc::function<bool(_tArgs&&...)>;
	tplt<tpnm T, tpnm ..._tArgs>			using	FTransform			= ::llc::function<T(_tArgs&&...)>;

	tplt<tpnm T>							using	TFuncForEach		= FVoid<T&>;
	tplt<tpnm T>							using	TFuncForEachConst	= FVoid<const T&>;
	tplt<tpnm T, tpnm tCount = uint32_t>	using	TFuncEnumerate		= FVoid<tCount&, T&>;
	tplt<tpnm T, tpnm tCount = uint32_t>	using	TFuncEnumerateConst	= FVoid<tCount&, const T&>;

	tplt<tpnm TSource, tpnm TTarget>		using	TFuncAppend 		= function<::llc::error_t(TTarget 	& output, const TSource & origin)>;
	tplt<tpnm TIO>							using	TFuncSize			= function<::llc::error_t(const TIO & origin)>;
} // namespace

#endif // LLC_FUNCTIONAL_H_23627
