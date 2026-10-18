#include "llc_n3.h"

#ifndef LLC_COORD_H_23627
#define LLC_COORD_H_23627

namespace llc
{
#pragma pack(push, 1)
	tplT struct SOrigin {
		n3<T>	Front, Up, Right;

		LLC_DEFAULT_OPERATOR(SOrigin<T>, Front == other.Front && Up == other.Up && Right == other.Right);
	};
#pragma pack(pop)
}

#endif // LLC_COORD_H_23627
