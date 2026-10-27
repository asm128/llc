#include "llc_typeint.h"

#ifndef LLC_KEYVAL_H_26920
#define LLC_KEYVAL_H_26920

namespace llc
{
	tplt<tpnm _tKey, tpnm _tVal = _tKey>
	stct keyval {
		tydf	_tKey			TKey;
		tydf	_tVal			TVal;
		tydf	keyval<TKey, TVal>	TKeyVal;

		TKey				Key	= {};
		TVal				Val	= {};

		LLC_DEFAULT_OPERATOR(TKeyVal, Key == other.Key && Val == other.Val);
	};

	tplt<tpnm _tKey, tpnm _tVal = _tKey>	using kv		= keyval<_tKey, _tVal>;
	tplt<tpnm _tVal>						using kvu0_t	= kv<u0_t  , _tVal>;
	tplt<tpnm _tVal>						using kvu1_t	= kv<u1_t  , _tVal>;
	tplt<tpnm _tVal>						using kvu2_t	= kv<u2_t  , _tVal>;
	tplt<tpnm _tVal>						using kvu3_t	= kv<u3_t  , _tVal>;
}

#endif // LLC_KEYVAL_H_26920
