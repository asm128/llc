#include "llc_array.h"
#include "llc_grid.h"

#ifndef LLC_GRID_STATIC_H_23627
#define LLC_GRID_STATIC_H_23627

namespace llc
{
	tplt<tpnm T, size_t _sizeWidth, size_t _sizeDepth>
	struct grid_static : public grid<T> {
		typedef	T			TCell;
		typedef	grid<T>		TGridView;

		stacxpr	uint32_t	Width									= (uint32_t)_sizeWidth;
		stacxpr	uint32_t	Depth									= (uint32_t)_sizeDepth;

				TCell		Cells	[_sizeDepth][_sizeWidth]		= {};

		inlcxpr				grid_static								()																		: grid<T>(&Cells[0][0], _sizeWidth, _sizeDepth)	{}

		::llc::error_t		read					(cnst s0_t* input, u2_t* inout_bytesRead)							{
			ree_if(0 == input || 0 == inout_bytesRead, "%s", "Invalid input!");
			*inout_bytesRead												+= sizeof(T) * Width * Depth;
			memcpy(Cells, input, sizeof(Cells));
			return 0;
		}

		::llc::error_t		write									(s0_t* output, u2_t* inout_bytesWritten)			const				{
			ree_if(0 == output && 0 == inout_bytesWritten, "%s", "Invalid input!");
			if(0 != inout_bytesWritten)
				*inout_bytesWritten												+= sizeof(T) * Width * Depth;	// Just return the size required to store this.

			if(0 == output)
				return 0;

			memcpy(output, Cells, sizeof(Cells));
			return 0;
		}
	};
}

#endif // LLC_GRID_STATIC_H_23627
