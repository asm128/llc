#include "llc_platform_globals.h"

#ifndef LLC_ATMEL
#	include <memory> // this is required for ::std::swap()
#endif // LLC_ATMEL

#ifndef LLC_SWAP_H
#define LLC_SWAP_H

namespace llc
{
#ifndef LLC_ATMEL
	using ::std::swap;
#else // LLC_ATMEL
	tplt<class _Ty>
	inxp	void	swap(_Ty & _Left, _Ty & _Right) nxpt {
		_Ty _Tmp = _Left;
		_Left    = _Right;
		_Right   = _Tmp;
	}
#endif // LLC_ATMEL
} // namespace

#endif // LLC_SWAP_H
