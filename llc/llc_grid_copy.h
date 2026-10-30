#include "llc_grid.h"
#include "llc_view_bit.h"
#include "llc_array_pod.h"
#include "llc_rect2.h"

#ifndef LLC_GRID_COPY_20983429834
#define LLC_GRID_COPY_20983429834

namespace llc
{
	tplt<tpnm T, tpnm _tCoord>
	::llc::err_t			grid_copy_ex					(::llc::grid<T> & dst, const ::llc::grid<T>& src, const ::llc::n2<_tCoord> & size_, const ::llc::n2<_tCoord> & offsetDst, const ::llc::n2<_tCoord> & offsetSrc)	{
		for(::llc::u2_t y = 0; y < (::llc::u2_t)size_.y; ++y) {
#if defined(LLC_DEBUG_GRID_COPY_EX) && (defined(DEBUG) || defined(_DEBUG))
			for(::llc::u2_t x = 0; x < (::llc::u2_t)size_.x; ++x)
				dst[y + offsetDst.y][x + offsetDst.x]						= src[y + offsetSrc.y][x + offsetSrc.x];
#else
			const ::llc::u2_t													sizeToCopy						= size_.x * sizeof(T);
			memcpy(&dst[y + offsetDst.y][offsetDst.x], &src[y + offsetSrc.y][offsetSrc.x], sizeToCopy);
#endif
		}
		return ::llc::max(s2_t(size_.x * size_.y), (s2_t)0);
	}

	tplt<tpnm T, tpnm _tCoord>
	::llc::err_t			grid_copy_alpha_ex				(::llc::grid<T> & dst, const ::llc::grid<T>& src, const ::llc::n2<_tCoord> & size_, const ::llc::n2<_tCoord> & offsetDst, const ::llc::n2<_tCoord> & offsetSrc, const T& comparand)	{
		for(::llc::u2_t y = 0; y < (::llc::u2_t)size_.y; ++y)
			for(::llc::u2_t x = 0; x < (::llc::u2_t)size_.x; ++x)
				if (src[y + offsetSrc.y][x + offsetSrc.x] != comparand)
					dst[y + offsetDst.y][x + offsetDst.x]						= src[y + offsetSrc.y][x + offsetSrc.x];
		return ::llc::max(s2_t(size_.x * size_.y), (s2_t)0);
	}

	tplt<tpnm T, tpnm _tCoord>
	::llc::err_t			grid_copy_alpha_ex				(::llc::grid<T> & dst, const ::llc::grid<T>& src, const ::llc::n2<_tCoord> & size_, const ::llc::n2<_tCoord> & offsetDst, const ::llc::n2<_tCoord> & offsetSrc, const T& comparand, const T& color)	{
		for(::llc::u2_t y = 0; y < (::llc::u2_t)size_.y; ++y)
			for(::llc::u2_t x = 0; x < (::llc::u2_t)size_.x; ++x)
				if (src[y + offsetSrc.y][x + offsetSrc.x] != comparand)
					dst[y + offsetDst.y][x + offsetDst.x]						= color;
		return ::llc::max(s2_t(size_.x * size_.y), (s2_t)0);
	}

	tplt<tpnm T, tpnm _tCoord>
	::llc::err_t			grid_copy_blend_ex				(::llc::grid<T> & dst, const ::llc::grid<T>& src, const ::llc::n2<_tCoord> & size_, const ::llc::n2<_tCoord> & offsetDst, const ::llc::n2<_tCoord> & offsetSrc)	{
		::llc::n2s2_t count = {s2_t(size_.x + ((offsetSrc.x < 0) ? offsetSrc.x : 0)), s2_t(size_.y + ((offsetSrc.y < 0) ? offsetSrc.y : 0))};
		for(s2_t y = 0; y < (s2_t)count.y; ++y)
			for(s2_t x = 0; x < (s2_t)count.x; ++x) {
				const ::llc::n2s2_t									coordSrcCell					= {s2_t(x + offsetSrc.x), s2_t(y + offsetSrc.y)};
				if(coordSrcCell.x < 0 || coordSrcCell.y < 0)
					continue;
				const T													& srcCell						= src[coordSrcCell.y][coordSrcCell.x];
				if(srcCell.a) {
					T															& dstCell						= dst[y + offsetDst.y][x + offsetDst.x];
					f3_t															factor							= srcCell.a / 255.0;
					dstCell														= ::llc::interpolate_linear(dstCell, srcCell, factor);
				}
			}
		return ::llc::max(s2_t(count.x * count.y), (s2_t)0);
	}

	tplt<tpnm _tCoord>
	::llc::err_t			grid_raster_alpha_bit_ex		(const ::llc::view_bit<u3_t> & src, const ::llc::n2u2_t & srcMetrics, const ::llc::n2u2_t & size_, const ::llc::n2<_tCoord> & offsetDst, const ::llc::n2<_tCoord> & offsetSrc, ::llc::apod<::llc::n2u1_t> & dstCoords) {
		for(::llc::u1_t y = 0; y < size_.y; ++y)
			for(::llc::u1_t x = 0; x < size_.x; ++x)
				if(src[(y + offsetSrc.y) * srcMetrics.x + (x + offsetSrc.x)])
					dstCoords.push_back({(::llc::u1_t)(x + offsetDst.x), (::llc::u1_t)(y + offsetDst.y)});
		return ::llc::max(size_.x * size_.y, (::llc::u2_t)0U);
	}
	tplt<tpnm _tCoord>
	::llc::err_t			grid_raster_alpha_bit			(const ::llc::n2u2_t & dstMetrics, const ::llc::view_bit<u3_t> & src, const ::llc::n2u2_t & srcMetrics, const ::llc::rect2<_tCoord>& dstRect, const ::llc::n2<_tCoord> & srcOffset, ::llc::apod<::llc::n2u1_t> & dstCoords) {
		const ::llc::n2s2_t			offsetSrc					= srcOffset.s2()  + ::llc::n2s2_t{-::llc::min((s2_t)0, (s2_t)dstRect.Offset.x), -::llc::min((s2_t)0, (s2_t)dstRect.Offset.y)};
		const ::llc::n2s2_t			offsetDst					= {::llc::max((s2_t)0, (s2_t)dstRect.Offset.x),  ::llc::max((s2_t)0, (s2_t)dstRect.Offset.y)};
		const s2_t				stopYDst					= s2_t(srcMetrics.y) + ::llc::min((s2_t)0, (s2_t)dstRect.Offset.y) - ::llc::min<_tCoord>(srcOffset.y, 0);
		const s2_t				stopXDst					= s2_t(srcMetrics.x) + ::llc::min((s2_t)0, (s2_t)dstRect.Offset.x) - ::llc::min<_tCoord>(srcOffset.x, 0);
		const s2_t				maxY						= ::llc::min(::llc::min(stopYDst - (s2_t)srcOffset.y, (s2_t)dstMetrics.y - offsetDst.y), s2_t(dstRect.Size.y/* - dstRect.Offset.y*/));
		const s2_t				maxX						= ::llc::min(::llc::min(stopXDst - (s2_t)srcOffset.x, (s2_t)dstMetrics.x - offsetDst.x), s2_t(dstRect.Size.x/* - dstRect.Offset.x*/));
		return (maxX > 0) ? ::llc::grid_raster_alpha_bit_ex(src, srcMetrics, ::llc::n2s2_t{maxX, maxY}.Clamp({}, dstMetrics.s2()).u2(), offsetDst, offsetSrc, dstCoords) : 0;
	}

	// -----------------------------------------------
	tplt<tpnm T, tpnm _tCoord>
	::llc::err_t			grid_copy_alpha					(::llc::grid<T> & dst, const ::llc::grid<T>& src, const ::llc::n2<_tCoord> & dstOffset, const ::llc::rect2<_tCoord>& srcRect, const T& comparand)		{
		const ::llc::n2s2_t			offsetSrc						= srcRect.Offset.s2()  + ::llc::n2s2_t{-::llc::min((s2_t)0, (s2_t)dstOffset.x), -::llc::min((s2_t)0, (s2_t)dstOffset.y)};
		const ::llc::n2s2_t			offsetDst						= {::llc::max((s2_t)0, (s2_t)dstOffset.x),  ::llc::max((s2_t)0, (s2_t)dstOffset.y)};
		const s2_t				stopYDst						= s2_t(src.metrics().y) + ::llc::min((s2_t)0, (s2_t)dstOffset.y);
		const s2_t				stopXDst						= s2_t(src.metrics().x) + ::llc::min((s2_t)0, (s2_t)dstOffset.x);
		const s2_t				maxY							= ::llc::min(::llc::min(stopYDst - (s2_t)srcRect.Offset.y, (s2_t)dst.metrics().y - offsetDst.y), (s2_t)srcRect.Size.y);
		const s2_t				maxX							= ::llc::min(::llc::min(stopXDst - (s2_t)srcRect.Offset.x, (s2_t)dst.metrics().x - offsetDst.x), (s2_t)srcRect.Size.x);
		return (maxX > 0) ? ::llc::grid_copy_alpha_ex(dst, src, ::llc::n2s2_t{maxX, maxY}, offsetDst, offsetSrc, comparand) : 0;
	}

	tplt<tpnm T, tpnm _tCoord>
	::llc::err_t			grid_copy_blend					(::llc::grid<T> & dst, const ::llc::grid<T>& src, const ::llc::n2<_tCoord> & dstOffset, const ::llc::rect2<_tCoord>& srcRect)									{
		const ::llc::n2s2_t			offsetSrc						= srcRect.Offset.s2t()  + ::llc::n2s2_t{-::llc::min((s2_t)0, (s2_t)dstOffset.x), -::llc::min((s2_t)0, (s2_t)dstOffset.y)};
		const ::llc::n2s2_t			offsetDst						= {::llc::max((s2_t)0, (s2_t)dstOffset.x),  ::llc::max((s2_t)0, (s2_t)dstOffset.y)};
		const s2_t				stopYDst						= s2_t(src.metrics().y) + ::llc::min((s2_t)0, (s2_t)dstOffset.y);
		const s2_t				stopXDst						= s2_t(src.metrics().x) + ::llc::min((s2_t)0, (s2_t)dstOffset.x);
		const s2_t				maxY							= ::llc::min(::llc::min(stopYDst - (s2_t)srcRect.Offset.y, (s2_t)dst.metrics().y - offsetDst.y), (s2_t)srcRect.Size.y);
		const s2_t				maxX							= ::llc::min(::llc::min(stopXDst - (s2_t)srcRect.Offset.x, (s2_t)dst.metrics().x - offsetDst.x), (s2_t)srcRect.Size.x);
		return (maxX > 0) ? ::llc::grid_copy_blend_ex(dst, src, ::llc::n2s2_t{maxX, maxY}, offsetDst, offsetSrc) : 0;
	}

	tplt<tpnm T, tpnm _tCoord>
	::llc::err_t			grid_copy						(::llc::grid<T> & dst, const ::llc::grid<T>& src, const ::llc::n2<_tCoord> & dstOffset, const ::llc::rect2<_tCoord>& srcRect)									{
		const ::llc::n2s2_t			offsetSrc						= srcRect.Offset.s2t() + ::llc::n2s2_t{-::llc::min((s2_t)0, (s2_t)dstOffset.x), -::llc::min((s2_t)0, (s2_t)dstOffset.y)};
		const ::llc::n2s2_t			offsetDst						= {::llc::max((s2_t)0, (s2_t)dstOffset.x),  ::llc::max((s2_t)0, (s2_t)dstOffset.y)};
		const s2_t				stopYDst						= s2_t(src.metrics().y) + ::llc::min((s2_t)0, (s2_t)dstOffset.y);
		const s2_t				stopXDst						= s2_t(src.metrics().x) + ::llc::min((s2_t)0, (s2_t)dstOffset.x);
		const s2_t				maxY							= ::llc::min(::llc::min(stopYDst - (s2_t)srcRect.Offset.y, (s2_t)dst.metrics().y - offsetDst.y), (s2_t)srcRect.Size.y);
		const s2_t				maxX							= ::llc::min(::llc::min(stopXDst - (s2_t)srcRect.Offset.x, (s2_t)dst.metrics().x - offsetDst.x), (s2_t)srcRect.Size.x);
		return (maxX > 0) ? ::llc::grid_copy_ex(dst, src, ::llc::n2s2_t{maxX, maxY}, offsetDst, offsetSrc) : 0;
	}

	// -----------------------------------------------
	tplt<tpnm T, tpnm _tCoord>
	::llc::err_t			grid_copy_alpha					(::llc::grid<T> & dst, const ::llc::grid<T>& src, const ::llc::rect2<_tCoord>& dstRect, const ::llc::n2<_tCoord> & srcOffset, const T& comparand)		{
		const ::llc::n2s2_t			offsetSrc						= srcOffset.s2()  + ::llc::n2s2_t{-::llc::min((s2_t)0, (s2_t)dstRect.Offset.x), -::llc::min((s2_t)0, (s2_t)dstRect.Offset.y)};
		const ::llc::n2s2_t			offsetDst						= {::llc::max((s2_t)0, (s2_t)dstRect.Offset.x),  ::llc::max((s2_t)0, (s2_t)dstRect.Offset.y)};
		const s2_t				stopYDst						= s2_t(src.metrics().y) + ::llc::min((s2_t)0, (s2_t)dstRect.Offset.y);
		const s2_t				stopXDst						= s2_t(src.metrics().x) + ::llc::min((s2_t)0, (s2_t)dstRect.Offset.x);
		const s2_t				maxY							= ::llc::min(::llc::min(stopYDst - (s2_t)srcOffset.y, (s2_t)dst.metrics().y - offsetDst.y), s2_t(dstRect.Size.y/* - dstRect.Offset.y*/));
		const s2_t				maxX							= ::llc::min(::llc::min(stopXDst - (s2_t)srcOffset.x, (s2_t)dst.metrics().x - offsetDst.x), s2_t(dstRect.Size.x/* - dstRect.Offset.x*/));
		return (maxX > 0) ? ::llc::grid_copy_alpha_ex(dst, src, ::llc::n2s2_t{maxX, maxY}, offsetDst, offsetSrc, comparand) : 0;
	}

	tplt<tpnm T, tpnm _tCoord>
	::llc::err_t			grid_copy_blend					(::llc::grid<T> & dst, const ::llc::grid<T>& src, const ::llc::rect2<_tCoord>& dstRect, const ::llc::n2<_tCoord> & srcOffset)									{
		const ::llc::n2s2_t			offsetSrc						= srcOffset.s2() + ::llc::n2s2_t{-::llc::min((s2_t)0, (s2_t)dstRect.Offset.x), -::llc::min((s2_t)0, (s2_t)dstRect.Offset.y)};
		const ::llc::n2s2_t			offsetDst						= {::llc::max((s2_t)0, (s2_t)dstRect.Offset.x),  ::llc::max((s2_t)0, (s2_t)dstRect.Offset.y)};
		const s2_t				stopYDst						= ((s2_t)src.metrics().y) + ::llc::min((s2_t)0, (s2_t)dstRect.Offset.y);
		const s2_t				stopXDst						= ((s2_t)src.metrics().x) + ::llc::min((s2_t)0, (s2_t)dstRect.Offset.x);
		const s2_t				maxY							= ::llc::min(::llc::min(stopYDst - (s2_t)srcOffset.y, (s2_t)dst.metrics().y - offsetDst.y), s2_t(dstRect.Size.y/* - dstRect.Offset.y*/));
		const s2_t				maxX							= ::llc::min(::llc::min(stopXDst - (s2_t)srcOffset.x, (s2_t)dst.metrics().x - offsetDst.x), s2_t(dstRect.Size.x/* - dstRect.Offset.x*/));
		return (maxX > 0) ? ::llc::grid_copy_blend_ex(dst, src, ::llc::n2s2_t{maxX, maxY}, offsetDst, offsetSrc) : 0;
	}

	tplt<tpnm T, tpnm _tCoord>
	::llc::err_t			grid_copy						(::llc::grid<T> & dst, const ::llc::grid<T>& src, const ::llc::rect2<_tCoord>& dstRect, const ::llc::n2<_tCoord> & srcOffset)									{
		const ::llc::n2s2_t			offsetSrc						= srcOffset.s2() + ::llc::n2s2_t{-::llc::min((s2_t)0, (s2_t)dstRect.Offset.x), -::llc::min((s2_t)0, (s2_t)dstRect.Offset.y)};
		const ::llc::n2s2_t			offsetDst						= {::llc::max((s2_t)0, (s2_t)dstRect.Offset.x),  ::llc::max((s2_t)0, (s2_t)dstRect.Offset.y)};
		const s2_t				stopYDst						= ((s2_t)src.metrics().y) + ::llc::min((s2_t)0, (s2_t)dstRect.Offset.y);
		const s2_t				stopXDst						= ((s2_t)src.metrics().x) + ::llc::min((s2_t)0, (s2_t)dstRect.Offset.x);
		const s2_t				maxY							= ::llc::min(::llc::min(stopYDst - (s2_t)srcOffset.y, (s2_t)dst.metrics().y - offsetDst.y), s2_t(dstRect.Size.y/* - dstRect.Offset.y*/));
		const s2_t				maxX							= ::llc::min(::llc::min(stopXDst - (s2_t)srcOffset.x, (s2_t)dst.metrics().x - offsetDst.x), s2_t(dstRect.Size.x/* - dstRect.Offset.x*/));
		return (maxX > 0) ? ::llc::grid_copy_ex(dst, src, ::llc::n2s2_t{maxX, maxY}, offsetDst, offsetSrc) : 0;
	}

	// -----------------------------------------------
	tplt<tpnm T, tpnm _tCoord>
	::llc::err_t			grid_copy_alpha					(::llc::grid<T> & dst, const ::llc::grid<T>& src, const ::llc::n2<_tCoord> & dstOffset, const ::llc::n2<_tCoord> & srcOffset, const T& comparand)			{
		const ::llc::n2s2_t			offsetSrc						= srcOffset.s2()  + ::llc::n2s2_t{-::llc::min((s2_t)0, (s2_t)dstOffset.x), -::llc::min((s2_t)0, (s2_t)dstOffset.y)};
		const ::llc::n2s2_t			offsetDst						= {::llc::max((s2_t)0, (s2_t)dstOffset.x),  ::llc::max((s2_t)0, (s2_t)dstOffset.y)};
		const s2_t				stopYDst						= s2_t(src.metrics().y) + ::llc::min((s2_t)0, (s2_t)dstOffset.y);
		const s2_t				stopXDst						= s2_t(src.metrics().x) + ::llc::min((s2_t)0, (s2_t)dstOffset.x);
		const s2_t				maxY							= ::llc::min(stopYDst - (s2_t)srcOffset.y, (s2_t)dst.metrics().y - offsetDst.y);
		const s2_t				maxX							= ::llc::min(stopXDst - (s2_t)srcOffset.x, (s2_t)dst.metrics().x - offsetDst.x);
		return (maxX > 0) ? ::llc::grid_copy_alpha_ex(dst, src, ::llc::n2s2_t{maxX, maxY}, offsetDst, offsetSrc, comparand) : 0;
	}

	tplt<tpnm T, tpnm _tCoord>
	::llc::err_t			grid_copy_blend					(::llc::grid<T> & dst, const ::llc::grid<T>& src, const ::llc::n2<_tCoord> & dstOffset, const ::llc::n2<_tCoord> & srcOffset)									{
		const ::llc::n2s2_t			offsetSrc						= srcOffset.s2()  + ::llc::n2s2_t{-::llc::min((s2_t)0, (s2_t)dstOffset.x), -::llc::min((s2_t)0, (s2_t)dstOffset.y)};
		const ::llc::n2s2_t			offsetDst						= {::llc::max((s2_t)0, (s2_t)dstOffset.x),  ::llc::max((s2_t)0, (s2_t)dstOffset.y)};
		const s2_t				stopYDst						= s2_t(src.metrics().y) + ::llc::min((s2_t)0, (s2_t)dstOffset.y);
		const s2_t				stopXDst						= s2_t(src.metrics().x) + ::llc::min((s2_t)0, (s2_t)dstOffset.x);
		const s2_t				maxY							= ::llc::min(stopYDst - (s2_t)srcOffset.y, (s2_t)dst.metrics().y - offsetDst.y);
		const s2_t				maxX							= ::llc::min(stopXDst - (s2_t)srcOffset.x, (s2_t)dst.metrics().x - offsetDst.x);
		return (maxX > 0) ? ::llc::grid_copy_blend_ex(dst, src, ::llc::n2s2_t{maxX, maxY}, offsetDst, offsetSrc) : 0;
	}

	tplt<tpnm T, tpnm _tCoord>
	::llc::err_t			grid_copy						(::llc::grid<T> & dst, const ::llc::grid<T>& src, const ::llc::n2<_tCoord> & dstOffset, const ::llc::n2<_tCoord> & srcOffset)									{
		const ::llc::n2s2_t			offsetSrc						= srcOffset.s2()  + ::llc::n2s2_t{-::llc::min((s2_t)0, (s2_t)dstOffset.x), -::llc::min((s2_t)0, (s2_t)dstOffset.y)};
		const ::llc::n2s2_t			offsetDst						= {::llc::max((s2_t)0, (s2_t)dstOffset.x),  ::llc::max((s2_t)0, (s2_t)dstOffset.y)};
		const s2_t				stopYDst						= s2_t(src.metrics().y) + ::llc::min((s2_t)0, (s2_t)dstOffset.y);
		const s2_t				stopXDst						= s2_t(src.metrics().x) + ::llc::min((s2_t)0, (s2_t)dstOffset.x);
		const s2_t				maxY							= ::llc::min(stopYDst - (s2_t)srcOffset.y, (s2_t)dst.metrics().y - offsetDst.y);
		const s2_t				maxX							= ::llc::min(stopXDst - (s2_t)srcOffset.x, (s2_t)dst.metrics().x - offsetDst.x);
		return (maxX > 0) ? ::llc::grid_copy_ex(dst, src, ::llc::n2s2_t{maxX, maxY}, offsetDst, offsetSrc) : 0;
	}

	// -----------------------------------------------
	tplt<tpnm T, tpnm _tCoord>
	::llc::err_t			grid_copy_alpha					(::llc::grid<T> & dst, const ::llc::grid<T>& src, const ::llc::rect2<_tCoord>& dstRect, const T& comparand)													{
		const ::llc::n2s2_t			offsetSrc						= {-::llc::min((s2_t)0, (s2_t)dstRect.Offset.x), -::llc::min((s2_t)0, (s2_t)dstRect.Offset.y)};
		const ::llc::n2s2_t			offsetDst						= { ::llc::max((s2_t)0, (s2_t)dstRect.Offset.x),  ::llc::max((s2_t)0, (s2_t)dstRect.Offset.y)};
		const s2_t				stopYDst						= (s2_t)src.metrics().y + ::llc::min((s2_t)0, (s2_t)dstRect.Offset.y);
		const s2_t				stopXDst						= (s2_t)src.metrics().x + ::llc::min((s2_t)0, (s2_t)dstRect.Offset.x);
		const s2_t				maxY							= ::llc::max((s2_t)0, ::llc::min(::llc::min(stopYDst, (s2_t)dst.metrics().y - offsetDst.y), s2_t(dstRect.Size.y)));
		const s2_t				maxX							= ::llc::min(::llc::min(stopXDst, (s2_t)dst.metrics().x - offsetDst.x), s2_t(dstRect.Size.x));
		return (maxX > 0) ? ::llc::grid_copy_alpha_ex(dst, src, ::llc::n2s2_t{maxX, maxY}, offsetDst, offsetSrc, comparand) : 0;
	}

	tplt<tpnm T, tpnm _tCoord>
	::llc::err_t			grid_copy_blend					(::llc::grid<T> & dst, const ::llc::grid<T>& src, const ::llc::rect2<_tCoord>& dstRect)																			{
		const ::llc::n2s2_t			offsetSrc						= {-::llc::min((s2_t)0, (s2_t)dstRect.Offset.x), -::llc::min((s2_t)0, (s2_t)dstRect.Offset.y)};
		const ::llc::n2s2_t			offsetDst						= { ::llc::max((s2_t)0, (s2_t)dstRect.Offset.x),  ::llc::max((s2_t)0, (s2_t)dstRect.Offset.y)};
		const s2_t				stopYDst						= (s2_t)src.metrics().y + ::llc::min((s2_t)0, (s2_t)dstRect.Offset.y);
		const s2_t				stopXDst						= (s2_t)src.metrics().x + ::llc::min((s2_t)0, (s2_t)dstRect.Offset.x);
		const s2_t				maxY							= ::llc::max((s2_t)0, ::llc::min(::llc::min(stopYDst, (s2_t)dst.metrics().y - offsetDst.y), s2_t(dstRect.Size.y)));
		const s2_t				maxX							= ::llc::min(::llc::min(stopXDst, (s2_t)dst.metrics().x - offsetDst.x), s2_t(dstRect.Size.x));
		return (maxX > 0) ? ::llc::grid_copy_blend_ex(dst, src, ::llc::n2s2_t{maxX, maxY}, offsetDst, offsetSrc) : 0;
	}

	tplt<tpnm T, tpnm _tCoord>
	::llc::err_t			grid_copy						(::llc::grid<T> & dst, const ::llc::grid<T>& src, const ::llc::rect2<_tCoord>& dstRect)																			{
		const ::llc::n2s2_t			offsetSrc						= {-::llc::min((s2_t)0, (s2_t)dstRect.Offset.x), -::llc::min((s2_t)0, (s2_t)dstRect.Offset.y)};
		::llc::n2s2_t				offsetDst						= { ::llc::max((s2_t)0, (s2_t)dstRect.Offset.x),  ::llc::max((s2_t)0, (s2_t)dstRect.Offset.y)};
		const s2_t				stopYDst						= (s2_t)src.metrics().y + ::llc::min((s2_t)0, (s2_t)dstRect.Offset.y);
		const s2_t				stopXDst						= (s2_t)src.metrics().x + ::llc::min((s2_t)0, (s2_t)dstRect.Offset.x);
		const s2_t				maxY							= ::llc::max((s2_t)0, ::llc::min(::llc::min(stopYDst, (s2_t)dst.metrics().y - offsetDst.y), s2_t(dstRect.Size.y)));
		const s2_t				maxX							= ::llc::min(::llc::min(stopXDst, (s2_t)dst.metrics().x - offsetDst.x), s2_t(dstRect.Size.x));
		return (maxX > 0) ? ::llc::grid_copy_ex(dst, src, ::llc::n2s2_t{maxX, maxY}, offsetDst, offsetSrc) : 0;
	}
	// -----------------------------------------------
	tplt<tpnm T, tpnm _tCoord>
	::llc::err_t			grid_copy_alpha					(::llc::grid<T> & dst, const ::llc::grid<T>& src, const ::llc::n2<_tCoord> & dstOffset, const T& comparand)														{
		const ::llc::n2s2_t			offsetSrc						= {-::llc::min((s2_t)0, (s2_t)dstOffset.x), -::llc::min((s2_t)0, (s2_t)dstOffset.y)};
		const ::llc::n2s2_t			offsetDst						= { ::llc::max((s2_t)0, (s2_t)dstOffset.x),  ::llc::max((s2_t)0, (s2_t)dstOffset.y)};
		::llc::n2s2_t				stopDst							= src.metrics().s2() + ::llc::n2s2_t{::llc::min((s2_t)0, (s2_t)dstOffset.x), ::llc::min((s2_t)0, (s2_t)dstOffset.y)};
		stopDst.y				= ::llc::min(stopDst.y, (s2_t)dst.metrics().y - offsetDst.y);
		stopDst.x				= ::llc::min(stopDst.x, (s2_t)dst.metrics().x - offsetDst.x);
		return (stopDst.x > 0) ? ::llc::grid_copy_alpha_ex(dst, src, stopDst, offsetDst, offsetSrc, comparand) : 0;
	}

	tplt<tpnm T, tpnm _tCoord>
	::llc::err_t			grid_copy_alpha					(::llc::grid<T> & dst, const ::llc::grid<T>& src, const ::llc::n2<_tCoord> & dstOffset, const T& comparand, const T& color)														{
		const ::llc::n2s2_t			offsetSrc						= {-::llc::min((s2_t)0, (s2_t)dstOffset.x), -::llc::min((s2_t)0, (s2_t)dstOffset.y)};
		const ::llc::n2s2_t			offsetDst						= { ::llc::max((s2_t)0, (s2_t)dstOffset.x),  ::llc::max((s2_t)0, (s2_t)dstOffset.y)};
		::llc::n2s2_t				stopDst							= src.metrics().s2() + ::llc::n2s2_t{::llc::min((s2_t)0, (s2_t)dstOffset.x), ::llc::min((s2_t)0, (s2_t)dstOffset.y)};
		stopDst.y				= ::llc::min(stopDst.y, (s2_t)dst.metrics().y - offsetDst.y);
		stopDst.x				= ::llc::min(stopDst.x, (s2_t)dst.metrics().x - offsetDst.x);
		return (stopDst.x > 0) ? ::llc::grid_copy_alpha_ex(dst, src, stopDst, offsetDst, offsetSrc, comparand, color) : 0;
	}

	tplt<tpnm T, tpnm _tCoord>
	::llc::err_t			grid_copy_blend					(::llc::grid<T> & dst, const ::llc::grid<T>& src, const ::llc::n2<_tCoord> & dstOffset)			{
		const ::llc::n2s2_t			offsetSrc						= {-::llc::min((s2_t)0, (s2_t)dstOffset.x), -::llc::min((s2_t)0, (s2_t)dstOffset.y)};
		const ::llc::n2s2_t			offsetDst						= { ::llc::max((s2_t)0, (s2_t)dstOffset.x),  ::llc::max((s2_t)0, (s2_t)dstOffset.y)};
		::llc::n2s2_t				stopDst							= src.metrics().s2() + ::llc::n2s2_t{::llc::min((s2_t)0, (s2_t)dstOffset.x), ::llc::min((s2_t)0, (s2_t)dstOffset.y)};
		stopDst.y				= ::llc::min(stopDst.y, (s2_t)dst.metrics().y - offsetDst.y);
		stopDst.x				= ::llc::min(stopDst.x, (s2_t)dst.metrics().x - offsetDst.x);
		return (stopDst.x > 0) ? ::llc::grid_copy_blend_ex(dst, src, stopDst, offsetDst, offsetSrc) : 0;
	}

	tplt<tpnm T, tpnm _tCoord>
	::llc::err_t			grid_copy						(::llc::grid<T> & dst, const ::llc::grid<T>& src, const ::llc::n2<_tCoord> & dstOffset)			{
		const ::llc::n2s2_t			offsetSrc						= {-::llc::min((s2_t)0, (s2_t)dstOffset.x), -::llc::min((s2_t)0, (s2_t)dstOffset.y)};
		const ::llc::n2s2_t			offsetDst						= { ::llc::max((s2_t)0, (s2_t)dstOffset.x),  ::llc::max((s2_t)0, (s2_t)dstOffset.y)};
		::llc::n2s2_t				stopDst							= src.metrics().s2() + ::llc::n2s2_t{::llc::min((s2_t)0, (s2_t)dstOffset.x), ::llc::min((s2_t)0, (s2_t)dstOffset.y)};
		stopDst.y				= ::llc::min(stopDst.y, (s2_t)dst.metrics().y - offsetDst.y);
		stopDst.x				= ::llc::min(stopDst.x, (s2_t)dst.metrics().x - offsetDst.x);
		return (stopDst.x > 0) ? ::llc::grid_copy_ex(dst, src, stopDst, offsetDst, offsetSrc) : 0;
	}

	tplt<tpnm T> ::llc::err_t		grid_copy_alpha					(::llc::grid<T> & dst, const ::llc::grid<T> & src, const T & comparand)	{ const ::llc::n2u2_t metrics = {(::llc::u2_t)::llc::min(dst.metrics().x, src.metrics().x), (::llc::u2_t)::llc::min(dst.metrics().y, src.metrics().y)}; return ::llc::grid_copy_alpha_ex	(dst, src, metrics, {}, {}, comparand); }
	tplt<tpnm T> ::llc::err_t		grid_copy_blend					(::llc::grid<T> & dst, const ::llc::grid<T> & src)						{ const ::llc::n2u2_t metrics = {(::llc::u2_t)::llc::min(dst.metrics().x, src.metrics().x), (::llc::u2_t)::llc::min(dst.metrics().y, src.metrics().y)}; return ::llc::grid_copy_blend_ex	(dst, src, metrics, {}, {}); }
	tplt<tpnm T> ::llc::err_t		grid_copy						(::llc::grid<T> & dst, const ::llc::grid<T> & src)						{ const ::llc::n2u2_t metrics = {(::llc::u2_t)::llc::min(dst.metrics().x, src.metrics().x), (::llc::u2_t)::llc::min(dst.metrics().y, src.metrics().y)}; return ::llc::grid_copy_ex		(dst, src, metrics, {}, {}); }

	tplt<tpnm T>
	::llc::err_t			grid_mirror_x					(::llc::grid<T> & dst, const ::llc::grid<T>& src)											{
		const ::llc::n2s2_t			metrics							=
			{ ::llc::min((s2_t)dst.metrics().x, (s2_t)src.metrics().x)
			, ::llc::min((s2_t)dst.metrics().y, (s2_t)src.metrics().y)
			};
		::llc::u2_t					srcMaxX							= src.metrics().x - 1;
		for(s2_t y = 0; y < metrics.y; ++y)
		for(s2_t x = 0; x < metrics.x; ++x)
			dst[y][x]				= src[y][srcMaxX - x];
		return metrics.y * metrics.x;
	}

	tplt<tpnm T>
	::llc::err_t			grid_mirror_y					(::llc::grid<T> & dst, const ::llc::grid<T>& src)		{
		const ::llc::n2s2_t			metrics							=
			{ ::llc::min((s2_t)dst.metrics().x, (s2_t)src.metrics().x)
			, ::llc::min((s2_t)dst.metrics().y, (s2_t)src.metrics().y)
			};
		::llc::u2_t					srcMaxY							= src.metrics().y - 1;
		if(metrics.x > 0)
			for(::llc::u2_t y = 0; y < (::llc::u2_t)metrics.y; ++y)
				memcpy(&dst[y][0], &src[srcMaxY - y][0], sizeof(T) * metrics.x);
		return metrics.y * metrics.x;
	}
} // namespace

#endif // LLC_GRID_COPY_20983429834
