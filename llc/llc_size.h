#include "llc_typeint.h"

#ifndef LLC_SIZE_H_23627
#define LLC_SIZE_H_23627

namespace llc
{
	tplt<tpnm T>										sinx	uint32_t	size		(const T & sized)										noexcept	{ return sized.size();							}
	tplt<tpnm _t, size_t _arraySize>					sinx	uint32_t	size		(const _t (&/*staticArray*/)[_arraySize])				noexcept	{ return _arraySize;							}
	tplt<tpnm _t, size_t _sizeDepth, size_t _sizeWidth>	sinx	uint32_t	size		(const _t (&/*staticArray*/)[_sizeWidth][_sizeDepth])	noexcept	{ return _sizeDepth * _sizeWidth;				}
	tplt<tpnm _t, size_t _arraySize>					sinx	uint32_t	byte_count	(const _t (&/*staticArray*/)[_arraySize])				noexcept	{ return szof(_t) * (_arraySize);				}
	tplt<tpnm _t, size_t _sizeDepth, size_t _sizeWidth>	sinx	uint32_t	byte_count	(const _t (&/*staticArray*/)[_sizeWidth][_sizeDepth])	noexcept	{ return szof(_t) * (_sizeDepth*_sizeWidth);	}
}

#endif // LLC_SIZE_H_23627
