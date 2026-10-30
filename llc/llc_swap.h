#include "llc_platform_globals.h"

#ifndef LLC_SWAP_H_23627
#define LLC_SWAP_H_23627

namespace llc
{
	tplT
	inxp	void	swap(T & left, T & right) nxpt {
		T	temporary	= left;
		left			= right;
		right			= temporary;
	}
} // namespace

#endif // LLC_SWAP_H_23627
