#include "llc_view.h"
#include "llc_grid.h"

#include "llc_packed_int.h"

#ifndef LLC_GRID_SERIALIZE_H_23627
#define LLC_GRID_SERIALIZE_H_23627

namespace llc
{
	// A Pascal grid stores its 32-bit width and height followed by tightly packed cells.
	tplTInTOut requires(szof(TIn) == 1 && (false == is_cnst<TIn>::Value || is_cnst<TOut>::Value))
				err_t	gridPascal	(grid<TOut> & gridToRead, view<TIn> input) {
		stxp u2_c			metricsWidth				= szof(n2u2_t);
		if_true_fef(input.size() < metricsWidth, "Invalid input size: %" LLC_FMT_U2 ".", input.size());
		n2u2_t				metrics					= {};
		memcpy(&metrics, input.begin(), metricsWidth);
		u3_c				cellCount					= (u3_t)metrics.x * metrics.y;
		if_true_fef(cellCount > (0x7FFFFFFFU - metricsWidth) / szof(TOut), "Grid dimensions overflow the readable byte range: %" LLC_FMT_U2 " x %" LLC_FMT_U2 ".", metrics.x, metrics.y);
		u2_c				dataSize					= (u2_t)cellCount * szof(TOut);
		if_true_fef(dataSize > input.size() - metricsWidth, "Invalid input size: %" LLC_FMT_U2 ". Grid dimensions: %" LLC_FMT_U2 " x %" LLC_FMT_U2 ".", input.size(), metrics.x, metrics.y);
		gridToRead								= {cellCount ? reinterpret_cast<TOut*>(&input[metricsWidth]) : 0, metrics};
		rtrn metricsWidth + dataSize;
	}
	tplTstin	err_t	gridPascal	(grid<cnst T> & gridToRead, vcu0_c input)	{ rtrn gridPascal<u0_c, cnst T>(gridToRead, input); }
	tplTstin	err_t	gridPascal	(grid<cnst T> & gridToRead, vcs0_t input)	{ rtrn gridPascal<s0_c, cnst T>(gridToRead, input); }
	tplTstin	err_t	gridPascal	(grid<cnst T> & gridToRead, vcst_t input)	{ rtrn gridPascal<sc_c, cnst T>(gridToRead, input); }
	tplTstin	err_t	gridPascal	(grid<T> & gridToRead, vu0_t input)			{ rtrn gridPascal<u0_t, T>(gridToRead, input); }
	tplTstin	err_t	gridPascal	(grid<T> & gridToRead, vs0_t input)			{ rtrn gridPascal<s0_t, T>(gridToRead, input); }
} // namespace

#endif // LLC_GRID_SERIALIZE_H_23627
