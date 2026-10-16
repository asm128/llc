#include "llc_typeint.h"

#ifndef LLC_SIZE_H_23627
#define LLC_SIZE_H_23627

namespace llc
{
	tplt<tpnm T>										sinx	::llc::u2_t	size		(const T & sized)										noexcept	{ return sized.size();							}
	tplt<tpnm _t, size_t _arraySize>					sinx	::llc::u2_t	size		(const _t (&/*staticArray*/)[_arraySize])				noexcept	{ return _arraySize;							}
	tplt<tpnm _t, size_t _arraySize>					sinx	::llc::u2_t	byte_count	(const _t (&/*staticArray*/)[_arraySize])				noexcept	{ return szof(_t) * (_arraySize);				}
}

#endif // LLC_SIZE_H_23627
