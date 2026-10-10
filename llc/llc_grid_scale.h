#include "llc_grid.h"

#ifndef LLC_GRID_SCALE_20654834
#define LLC_GRID_SCALE_20654834

namespace llc
{
	tplt<tpnm T>
	::llc::error_t					grid_scale_fast				(::llc::grid<T> & dst, const ::llc::grid<T> & src, const float xFactor, const float yFactor) {
		for(uint32_t y = 0; y < dst.metrics().y; ++y)
		for(uint32_t x = 0; x < dst.metrics().x; ++x)
			dst[y][x]						= src[(uint32_t)(y * yFactor)][(uint32_t)(x * xFactor)];
		return 0;
	}

	tplt<tpnm T>
	::llc::error_t					grid_scale					(::llc::grid<T> & dst, const ::llc::grid<T> & src) {
		const float							xFactor						= src.metrics().x / (float)dst.metrics().x;
		const float							yFactor						= src.metrics().y / (float)dst.metrics().y;
		return grid_scale_fast(dst, src, xFactor, yFactor);
	}

	tplt<tpnm T>
	::llc::error_t					grid_scale					(::llc::grid<T> & dst, const ::llc::grid<T> & src, const float xFactor, const float yFactor) {
		for(uint32_t y = 0; y < dst.metrics().y; ++y)
		for(uint32_t x = 0; x < dst.metrics().x; ++x) {
			const u2_t						xSrc						= (uint32_t)(x * xFactor);
			const u2_t						ySrc						= (uint32_t)(y * yFactor);
			if( ::llc::in_range(ySrc, 0U, src.metrics().y)
			 && ::llc::in_range(xSrc, 0U, src.metrics().x)
			 )
				dst[y][x]						= src[ySrc][xSrc];
		}
		return 0;
	}

	tplt<tpnm T>
	::llc::error_t					grid_scale_alpha			(::llc::grid<T> & dst, const ::llc::grid<T> & src, const ::llc::n2<int32_t> & dstPos, const ::llc::n2<int32_t> & dstSize) {
		const ::llc::n2<int32_t>			srcSize						= {(int)src.metrics().x	, (int)src.metrics().y	};
		for(int y = dstPos.y, yStop = ::llc::min(dstPos.y + dstSize.y, (int32_t)dst.metrics().y); y < yStop; ++y) {
			::llc::n2<int32_t>					sourcePixelCoord			= {0, (int)((y - (double)dstPos.y) / dstSize.y * (double)srcSize.y)};
			for(int x = dstPos.x, xStop = ::llc::min(dstPos.x + dstSize.x, (int32_t)dst.metrics().x); x < xStop; ++x) {
				sourcePixelCoord.x				= (int)((x - (double)dstPos.x) / dstSize.x * (double)srcSize.x);
				T									& colorDst					= dst[y][x];
				const T								& colorSrc					= src[sourcePixelCoord.y][sourcePixelCoord.x];
				dst[y][x]						= ::llc::interpolate_linear(colorDst, colorSrc, colorSrc.a / 255.0);
			}
		}
		return 0;
	}

	tplt<tpnm T>
	::llc::error_t					grid_scale					(::llc::grid<T> & dst, const ::llc::grid<T> & src, const ::llc::n2<int32_t> & dstPos, const ::llc::n2<int32_t> & dstSize) {
		const ::llc::n2<int32_t>			srcSize						= {(int)src.metrics().x	, (int)src.metrics().y	};
		for(int y = dstPos.y, yStop = ::llc::min(dstPos.y + dstSize.y, (int32_t)dst.metrics().y); y < yStop; ++y) {
			::llc::n2<int32_t>					sourcePixelCoord			= {0, (int)((y - (double)dstPos.y) / dstSize.y * (double)srcSize.y)};
			for(int x = dstPos.x, xStop = ::llc::min(dstPos.x + dstSize.x, (int32_t)dst.metrics().x); x < xStop; ++x) {
				sourcePixelCoord.x				= (int)((x - (double)dstPos.x) / dstSize.x * (double)srcSize.x);
				dst[y][x]						= src[sourcePixelCoord.y][sourcePixelCoord.x];
			}
		}
		return 0;
	}
} // namespace

#endif // LLC_GRID_SCALE_20983429834
