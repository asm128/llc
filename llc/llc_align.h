#include "llc_n2.h"
#include "llc_bit.h"
#include "llc_axis.h"

#ifndef LLC_ALIGN_H_23627
#define LLC_ALIGN_H_23627

namespace llc
{
	tplt<tpnm TCoord, tpnm TTarget>
	n2<TCoord> &	realignCoord
		( cnst n2<TTarget>	& targetSize
		, cnst n2<TCoord>	& coordToRealign
		, n2<TCoord>		& coordRealigned
		, ALIGN				align
		) nxpt {
		coordRealigned = coordToRealign;
			 if(bit_true(align, ALIGN_HCENTER	)) coordRealigned.x += targetSize.x >> 1;
		else if(bit_true(align, ALIGN_RIGHT		)) coordRealigned.x = targetSize.x - 1 - coordToRealign.x;

			 if(bit_true(align, ALIGN_VCENTER	)) coordRealigned.y += targetSize.y >> 1;
		else if(bit_true(align, ALIGN_BOTTOM		)) coordRealigned.y = targetSize.y - 1 - coordToRealign.y;
		return coordRealigned;
	}
}

#endif // LLC_ALIGN_H_23627
