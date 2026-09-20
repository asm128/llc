#include "llc_view.h"

#ifndef LLC_KEYVAL_H_26920
#define LLC_KEYVAL_H_26920

namespace llc
{
	tplt<tpnm _tKey, tpnm _tVal>
	stct keyval {
		tydf	_tKey			TKey;
		tydf	_tVal			TVal;
		tydf	keyval<TKey, TVal>	TKeyVal;

		TKey				Key	= {};
		TVal				Val	= {};

		LLC_DEFAULT_OPERATOR(TKeyVal, Key == other.Key && Val == other.Val);
	};

	tplt<tpnm _tVal> using kvvcst_t = keyval<vcst_t, _tVal>;
	tplt<tpnm _tVal> using kvu0_t   = keyval<u0_t  , _tVal>;
	tplt<tpnm _tVal> using kvu1_t   = keyval<u1_t  , _tVal>;
	tplt<tpnm _tVal> using kvu2_t   = keyval<u2_t  , _tVal>;
	tplt<tpnm _tVal> using kvu3_t   = keyval<u3_t  , _tVal>;
}

#endif // LLC_KEYVAL_H_26920
