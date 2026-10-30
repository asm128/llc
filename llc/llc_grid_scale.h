#include "llc_grid.h"

#ifndef LLC_GRID_SCALE_20654834
#define LLC_GRID_SCALE_20654834

namespace llc
{
	tplt<tpnm T>
	::llc::err_t					grid_scale_fast				(::llc::grid<T> & dst, const ::llc::grid<T> & src, const f2_t xFactor, const f2_t yFactor) {
		for(::llc::u2_t y = 0; y < dst.metrics().y; ++y)
		for(::llc::u2_t x = 0; x < dst.metrics().x; ++x)
			dst[y][x]						= src[(::llc::u2_t)(y * yFactor)][(::llc::u2_t)(x * xFactor)];
		return 0;
	}

	tplt<tpnm T>
	::llc::err_t					grid_scale					(::llc::grid<T> & dst, const ::llc::grid<T> & src) {
		const f2_t							xFactor						= src.metrics().x / (f2_t)dst.metrics().x;
		const f2_t							yFactor						= src.metrics().y / (f2_t)dst.metrics().y;
		return grid_scale_fast(dst, src, xFactor, yFactor);
	}

	tplt<tpnm T>
	::llc::err_t					grid_scale					(::llc::grid<T> & dst, const ::llc::grid<T> & src, const f2_t xFactor, const f2_t yFactor) {
		for(::llc::u2_t y = 0; y < dst.metrics().y; ++y)
		for(::llc::u2_t x = 0; x < dst.metrics().x; ++x) {
			const u2_t						xSrc						= (::llc::u2_t)(x * xFactor);
			const u2_t						ySrc						= (::llc::u2_t)(y * yFactor);
			if( ::llc::in_range(ySrc, 0U, src.metrics().y)
			 && ::llc::in_range(xSrc, 0U, src.metrics().x)
			 )
				dst[y][x]						= src[ySrc][xSrc];
		}
		return 0;
	}

	tplt<tpnm T>
	::llc::err_t					grid_scale_alpha			(::llc::grid<T> & dst, const ::llc::grid<T> & src, const ::llc::n2<s2_t> & dstPos, const ::llc::n2<s2_t> & dstSize) {
		const ::llc::n2<s2_t>			srcSize						= {(int)src.metrics().x	, (int)src.metrics().y	};
		for(int y = dstPos.y, yStop = ::llc::min(dstPos.y + dstSize.y, (s2_t)dst.metrics().y); y < yStop; ++y) {
			::llc::n2<s2_t>					sourcePixelCoord			= {0, (int)((y - (f3_t)dstPos.y) / dstSize.y * (f3_t)srcSize.y)};
			for(int x = dstPos.x, xStop = ::llc::min(dstPos.x + dstSize.x, (s2_t)dst.metrics().x); x < xStop; ++x) {
				sourcePixelCoord.x				= (int)((x - (f3_t)dstPos.x) / dstSize.x * (f3_t)srcSize.x);
				T									& colorDst					= dst[y][x];
				const T								& colorSrc					= src[sourcePixelCoord.y][sourcePixelCoord.x];
				dst[y][x]						= ::llc::interpolate_linear(colorDst, colorSrc, colorSrc.a / 255.0);
			}
		}
		return 0;
	}

	// Clip destination coordinates without changing the original sampling rectangle.
	// Empty rectangles are no-ops; negative extents and a sampled empty source fail.
	struct SGridScaleAxis {
		u2_t	Index			= 0;
		u2_t	Step			= 0;
		u2_t	Remainder		= 0;
		u2_t	RemainderStep	= 0;
		u2_t	Denominator		= 1;

		inln	void	advance		() nxpt {
			Index		+= Step;
			Remainder	+= RemainderStep;
			if(Remainder >= Denominator) {
				++Index;
				Remainder -= Denominator;
			}
		}
	};

	stxp SGridScaleAxis gridScaleAxis(s3_t dstStart, s2_t dstOrigin, u2_t srcSize, s2_t dstSize) nxpt {
		cnst u3_t	numerator	= u3_t(dstStart - dstOrigin) * srcSize;
		cnst u2_t	denominator	= (u2_t)dstSize;
		rtrn
			{ (u2_t)(numerator / denominator)
			, srcSize / denominator
			, (u2_t)(numerator % denominator)
			, srcSize % denominator
			, denominator
			};
	}

	tplt<tpnm T, tpnm TSource>
	::llc::err_t					grid_scale					(::llc::grid<T> & dst, const ::llc::grid<TSource> & src, const ::llc::n2<s2_t> & dstPos, const ::llc::n2<s2_t> & dstSize) {
		if_true_fef(dstSize.x < 0 || dstSize.y < 0, "Negative scale extent: %i x %i.", dstSize.x, dstSize.y);
		if(!dstSize.x || !dstSize.y) rtrn 0;
		cnst ::llc::n2s3_t start = {::llc::max(::llc::s3_t(0), ::llc::s3_t(dstPos.x)), ::llc::max(::llc::s3_t(0), ::llc::s3_t(dstPos.y))};
		cnst ::llc::n2s3_t stop = {::llc::min(::llc::s3_t(dstPos.x) + dstSize.x, ::llc::s3_t(dst.metrics().x)), ::llc::min(::llc::s3_t(dstPos.y) + dstSize.y, ::llc::s3_t(dst.metrics().y))};
		if(start.x >= stop.x || start.y >= stop.y) rtrn 0;
		cnst auto srcSize = src.metrics();
		if_true_fef(!srcSize.x || !srcSize.y, "Cannot sample an empty source: %u x %u.", srcSize.x, srcSize.y);
		cnst SGridScaleAxis sourceXStart = gridScaleAxis(start.x, dstPos.x, srcSize.x, dstSize.x);
		SGridScaleAxis sourceY = gridScaleAxis(start.y, dstPos.y, srcSize.y, dstSize.y);
		for(::llc::s3_t y = start.y; y < stop.y; ++y) {
			SGridScaleAxis sourceX = sourceXStart;
			for(::llc::s3_t x = start.x; x < stop.x; ++x) {
				dst[(::llc::u2_t)y][(::llc::u2_t)x] = src[sourceY.Index][sourceX.Index];
				sourceX.advance();
			}
			sourceY.advance();
		}
		rtrn 0;
	}
} // namespace

#endif // LLC_GRID_SCALE_20983429834
