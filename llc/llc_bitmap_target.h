#include "llc_image.h"
#include "llc_color.h"
#include "llc_camera.h"

#include "llc_tri2.h"
#include "llc_line2.h"
#include "llc_line3.h"
#include "llc_circle.h"

#include "llc_swap.h" // this is required for ::llc::swap()


#ifndef BITMAP_TARGET_H_23627
#define BITMAP_TARGET_H_23627

namespace llc
{
	tplt<tpnm _tCoord, tpnm T>
	::llc::error_t		drawPixelBrightness		(::llc::grid<T> & viewOffscreen, const ::llc::n2<_tCoord> & sourcePosition, const T & colorLight, float factor, double range)								{	// --- This function will draw some coloured symbols in each cell of the ASCII screen.
		::llc::n2f3_t			maxRange				= {range, range};
		double					rangeUnit				= 1.0 / maxRange.Length();
		for(int32_t y = -(int32_t)range - 1, blendCount = 1 + (int32_t)range + 1; y < blendCount; ++y)	// the + 1 - 1 is because we actually process more surrounding pixels in order to compensate for the flooring of the coordinates
		for(int32_t x = -(int32_t)range - 1; x < blendCount; ++x) {										// as it causes a visual effect of the light being cut to a rectangle and having sharp borders.
			if(x || y) {
				::llc::n2<_tCoord>		blendPos				= sourcePosition + ::llc::n2<_tCoord>{(_tCoord)x, (_tCoord)y};
				if( blendPos.x < (int32_t)viewOffscreen.metrics().x && blendPos.x >= 0
				 && blendPos.y < (int32_t)viewOffscreen.metrics().y && blendPos.y >= 0
				 ) {
					::llc::n2<_tCoord>		brightDistance			= blendPos - sourcePosition;
					double					brightDistanceLength	= brightDistance.Length();
					double					distanceFactor			= brightDistanceLength * rangeUnit;
					if(distanceFactor <= 1.0) {
						viewOffscreen[(uint32_t)blendPos.y][(uint32_t)blendPos.x]	= ::llc::interpolate_linear(viewOffscreen[(uint32_t)blendPos.y][(uint32_t)blendPos.x], colorLight, factor * (1.0f - distanceFactor));
					}
				}
			}
		}
		return 0;
	}

	tplt<tpnm _tCoord, tpnm T>
	::llc::error_t		drawPixelLight				(::llc::grid<T> & viewOffscreen, const ::llc::n2<_tCoord> & sourcePosition, const T & colorLight, float maxFactor, double range)								{	// --- This function will draw some coloured symbols in each cell of the ASCII screen.
		if( ((uint32_t)sourcePosition.x) < viewOffscreen.metrics().x
		 && ((uint32_t)sourcePosition.y) < viewOffscreen.metrics().y
		 )
			viewOffscreen[(uint32_t)sourcePosition.y][(uint32_t)sourcePosition.x]	= colorLight;
		return drawPixelBrightness(viewOffscreen, sourcePosition, colorLight, maxFactor, range);
	}

	tplt<tpnm _tColor>
	::llc::error_t			drawLineVertical		(::llc::grid<_tColor> & target, const _tColor& value, int32_t x, int32_t y1, int32_t y2)	{
		if(x < 0 || x >= (int32_t)target.metrics().x)
			return 0;
		const int32_t				yMin					= ::llc::min(y1, y2);
		const int32_t				yMax					= ::llc::max(y1, y2);
		const int32_t				yStop					= ::llc::min(yMax, (int32_t)target.metrics().y);
		int32_t						y						= ::llc::max(yMin, (int32_t)0);
		for(; y < yStop; ++y)
			target[y][x]			= value;
		return y;
	}

	tplt<tpnm _tColor>
	::llc::error_t			drawLineHorizontal		(::llc::grid<_tColor> & target, const _tColor& value, int32_t y, int32_t x1, int32_t x2)	{
		if(y < 0 || y >= (int32_t)target.metrics().y)
			return 0;
		const int32_t				xMin					= ::llc::min(x1, x2);
		const int32_t				xMax					= ::llc::max(x1, x2);
		const int32_t				xStop					= ::llc::min(xMax, (int32_t)target.metrics().x);
		int32_t						x						= ::llc::max(xMin, (int32_t)0);
		for(; x < xStop; ++x)
			target[y][x]			= value;
		return x;
	}

	// This implementation is incorrect. The problem is that it draws borders even if it shuoldn't. I never tested it but I believe that's what the code says.
	tplt<tpnm _tCoord, tpnm _tColor>
	static	::llc::error_t	drawRectangleBorder		(::llc::grid<_tColor> & bitmapTarget, const _tColor & value, const ::llc::rect2<_tCoord> & rectangle)		{
		int32_t						yStart					= (int32_t)::llc::max((int32_t)0, (int32_t)rectangle.Offset.y);
		int32_t						yStop					= ::llc::min((int32_t)rectangle.Offset.y + (int32_t)rectangle.Size.y, (int32_t)bitmapTarget.metrics().y);
		int32_t						xStart					= (int32_t)::llc::max((int32_t)0, (int32_t)rectangle.Offset.x);
		int32_t						xStop					= ::llc::min((int32_t)rectangle.Offset.x + (int32_t)rectangle.Size.x, (int32_t)bitmapTarget.metrics().x);
		if(yStart >= yStop || xStart >= xStop)
			return 0;
		for(int32_t x = xStart; x < xStop; ++x)
			bitmapTarget[yStart][x]	= value;
		memcpy(&bitmapTarget[yStop - 1][xStart], &bitmapTarget[yStart][xStart], sizeof(_tColor) * xStop - xStart);
		for(int32_t y = yStart + 1, yMax = (yStop - 1); y < yMax; ++y) {
			bitmapTarget[y][0]			= value;
			bitmapTarget[y][xStop - 1]	= value;
		}
		return (yStop - yStart) * 2 + (xStop - xStart) * 2;
	}

	tplt<tpnm _tCoord, tpnm _tColor>
	static	::llc::error_t	drawCircle			(::llc::grid<_tColor> & bitmapTarget, const _tColor & value, const ::llc::circle<_tCoord> & circle)			{
		int32_t						xStop				= ::llc::min((int32_t)(circle.Center.x + circle.Radius + 2), (int32_t)bitmapTarget.metrics().x);
		double						radiusSquared		= circle.Radius * circle.Radius;
		int32_t						pixelsDrawn			= 0;
		const int32_t				startY				= ::llc::max((int32_t)0, (int32_t)(circle.Center.y - circle.Radius));
		const int32_t				startX				= ::llc::max((int32_t)0, (int32_t)(circle.Center.x - circle.Radius));
		for(int32_t y = startY, yStop = ::llc::min((int32_t)(circle.Center.y + circle.Radius + 2), (int32_t)bitmapTarget.metrics().y); y < yStop; ++y)
		for(int32_t x = startX; x < xStop; ++x) {
			::llc::n2s2_t				cellcurrent			= {x, y};
			double						distanceSquared		= (cellcurrent - circle.Center).LengthSquared();
			if(distanceSquared < radiusSquared) {
				if(circle.Center.x - circle.Radius < 0)
					bitmapTarget[y][x]		= value;
				else {
					for(const int32_t xLimit = ::llc::min((int32_t)circle.Center.x + (int32_t)(circle.Center.x - x), (int32_t)bitmapTarget.metrics().x); x < xLimit; ++x) {
						bitmapTarget[y][x]		= value;
						++pixelsDrawn;
					}
					break;
				}
			}
		}
		return pixelsDrawn;
	}

	tplt<tpnm _tCoord, tpnm _tColor>
	static	::llc::error_t	drawCircle			(const ::llc::n2u2_t & targetMetrics, const ::llc::circle<_tCoord> & circle, ::llc::apod<::llc::n2s2_t> & out_Points)			{
		int32_t						xStop				= ::llc::min((int32_t)(circle.Center.x + circle.Radius), (int32_t)targetMetrics.x);
		double						radiusSquared		= circle.Radius * circle.Radius;
		int32_t						pixelsDrawn			= 0;
		for(int32_t y = ::llc::max((int32_t)0, (int32_t)(circle.Center.y - circle.Radius)), yStop = ::llc::min((int32_t)(circle.Center.y + circle.Radius), (int32_t)targetMetrics.y); y < yStop; ++y)
		for(int32_t x = ::llc::max((int32_t)0, (int32_t)(circle.Center.x - circle.Radius)); x < xStop; ++x) {
			::llc::n2s2_t				cellcurrent			= {x, y};
			double						distanceSquared		= (cellcurrent - circle.Center).LengthSquared();
			if(distanceSquared < radiusSquared) {
				if(circle.Center.x - circle.Radius < 0)
					out_Points.push_back({x, y});
				else {
					for(const int32_t xLimit = ::llc::min((int32_t)circle.Center.x + ((int32_t)circle.Center.x - x), (int32_t)targetMetrics.x); x < xLimit; ++x) {
						out_Points.push_back({x, y});
						++pixelsDrawn;
					}
					break;
				}
			}
		}
		return pixelsDrawn;
	}

	// A good article on this kind of triangle rasterization: https://fgiesen.wordpress.com/2013/02/08/triangle-rasterization-in-practice/
	tplt<tpnm _tCoord, tpnm _tColor>
	static	::llc::error_t	drawTriangle		(::llc::grid<_tColor>& bitmapTarget, const _tColor& value, const ::llc::tri2<_tCoord>& triangle)										{
		::llc::n2s2_t				areaMin				= {(int32_t)::llc::min(::llc::min(triangle.A.x, triangle.B.x), triangle.C.x), (int32_t)::llc::min(::llc::min(triangle.A.y, triangle.B.y), triangle.C.y)};
		::llc::n2s2_t				areaMax				= {(int32_t)::llc::max(::llc::max(triangle.A.x, triangle.B.x), triangle.C.x), (int32_t)::llc::max(::llc::max(triangle.A.y, triangle.B.y), triangle.C.y)};
		const int32_t				xStop				= ::llc::min(areaMax.x, (int32_t)bitmapTarget.metrics().x);
		int32_t						pixelsDrawn			= 0;
		for(int32_t y = ::llc::max(areaMin.y, (int32_t)0), yStop = ::llc::min(areaMax.y, (int32_t)bitmapTarget.metrics().y); y < yStop; ++y)
		for(int32_t x = ::llc::max(areaMin.x, (int32_t)0); x < xStop; ++x) {
			const ::llc::n2s1_t			cellcurrent			= {(int16_t)x, (int16_t)y};
			// Determine barycentric coordinates
			int							w0					= ::llc::orient2d({triangle.B.s1(), triangle.A.s1()}, cellcurrent);	// ::llc::orient2d({triangle.A, triangle.B}, cellcurrent);
			int							w1					= ::llc::orient2d({triangle.C.s1(), triangle.B.s1()}, cellcurrent);	// ::llc::orient2d({triangle.B, triangle.C}, cellcurrent);
			int							w2					= ::llc::orient2d({triangle.A.s1(), triangle.C.s1()}, cellcurrent);	// ::llc::orient2d({triangle.C, triangle.A}, cellcurrent);
			if(w0 < 0)
				continue;
			if(w1 < 0)
				continue;
			if(w2 < 0)
				continue;
			//if (w0 >= 0 && w1 >= 0 && w2 >= 0) { // If p is on or inside all edges, render pixel.
				bitmapTarget[y][x]		= value;
				++pixelsDrawn;
			//}
		}
		return pixelsDrawn;
	}

	tplt<tpnm _tCoord>
	static	::llc::error_t	drawTriangle		(const ::llc::n2u2_t & targetMetrics, const ::llc::tri2<_tCoord> & triangle, ::llc::apod<::llc::n2s1_t> & out_Points)		{
		::llc::n2s2_t				areaMin				= {(int32_t)::llc::min(::llc::min(triangle.A.x, triangle.B.x), triangle.C.x), (int32_t)::llc::min(::llc::min(triangle.A.y, triangle.B.y), triangle.C.y)};
		::llc::n2s2_t				areaMax				= {(int32_t)::llc::max(::llc::max(triangle.A.x, triangle.B.x), triangle.C.x), (int32_t)::llc::max(::llc::max(triangle.A.y, triangle.B.y), triangle.C.y)};
		const int32_t				xStop				= ::llc::min(areaMax.x, (int32_t)targetMetrics.x);
		int32_t						pixelsDrawn			= 0;
		for(int32_t y = ::llc::max(areaMin.y, (int32_t)0), yStop = ::llc::min(areaMax.y, (int32_t)targetMetrics.y); y < yStop; ++y)
		for(int32_t x = ::llc::max(areaMin.x, (int32_t)0); x < xStop; ++x) {
			const ::llc::n2s2_t			cellcurrent			= {x, y};
			// Determine barycentric coordinates
			int32_t						w0					= ::llc::orient2d({triangle.B, triangle.A}, cellcurrent);	// ::llc::orient2d({triangle.A, triangle.B}, cellcurrent);
			int32_t						w1					= ::llc::orient2d({triangle.C, triangle.B}, cellcurrent);	// ::llc::orient2d({triangle.B, triangle.C}, cellcurrent);
			int32_t						w2					= ::llc::orient2d({triangle.A, triangle.C}, cellcurrent);	// ::llc::orient2d({triangle.C, triangle.A}, cellcurrent);
			if(w0 < 0)
				continue;
			if(w1 < 0)
				continue;
			if(w2 < 0)
				continue;
			//if (w0 >= 0 && w1 >= 0 && w2 >= 0) { // If p is on or inside all edges, render pixel.
			out_Points.push_back({(int16_t)x, (int16_t)y});
			++pixelsDrawn;
			//}
		}
		return pixelsDrawn;
	}

	typedef	::llc::error_t	(*llc_raster_callback)	(void* bitmapTarget, const ::llc::n2u2_t & bitmapMetrics, const ::llc::n2u2_t & cellPos, const void* value);


	// Bresenham's line algorithm
	tplt<tpnm _tCoord, tpnm _tColor>
	static	::llc::error_t	rasterLine			(::llc::grid<_tColor> & bitmapTarget, const _tColor & value, const ::llc::line2<_tCoord> & line, llc_raster_callback callback)				{
		::llc::n2f2_t				A					= line.A.f2_t();
		::llc::n2f2_t				B					= line.B.f2_t();
		const bool					steep				= (fabs(B.y - A.y) > fabs(B.x - A.x));
		if(steep){
			::llc::swap(A.x, A.y);
			::llc::swap(B.x, B.y);
		}
		if(A.x > B.x) {
			::llc::swap(A.x, B.x);
			::llc::swap(A.y, B.y);
		}
		const ::llc::n2f2_t			d					= {B.x - A.x, (float)fabs(B.y - A.y)};
		float						error				= d.x / 2.0f;
		const int32_t				ystep				= (A.y < B.y) ? 1 : -1;
		int32_t						y					= (int32_t)A.y;
		int32_t						pixelsDrawn			= 0;
		if(steep) {
			for(int32_t x = (int32_t)A.x, xStop = (int32_t)B.x; x < xStop; ++x) {
				if(::llc::in_range(x, (int32_t)0, (int32_t)bitmapTarget.metrics().y) && ::llc::in_range(y, (int32_t)0, (int32_t)bitmapTarget.metrics().x)) {
					callback(bitmapTarget.begin(), bitmapTarget.metrics(), {(uint32_t)x, (uint32_t)y}, &value);
					++pixelsDrawn;
				}
				error					-= d.y;
				if(error < 0) {
					y						+= ystep;
					error					+= d.x;
				}
			}
		}
		else {
			for(int32_t x = (int32_t)A.x, xStop = (int32_t)B.x; x < xStop; ++x) {
				if(::llc::in_range(y, (int32_t)0, (int32_t)bitmapTarget.metrics().y) && ::llc::in_range(x, (int32_t)0, (int32_t)bitmapTarget.metrics().x)) {
					callback(bitmapTarget.begin(), bitmapTarget.metrics(), {(uint32_t)x, (uint32_t)y}, &value);
					++pixelsDrawn;
				}
				error					-= d.y;
				if(error < 0) {
					y						+= ystep;
					error					+= d.x;
				}
			}
		}
		return pixelsDrawn;
	}

	// Bresenham's line algorithm
	tplt<tpnm _tCoord, tpnm _tColor>
	static	::llc::error_t	drawLine			(::llc::grid<_tColor> & target, const _tColor & value, const ::llc::line2<_tCoord> & line)				{
		::llc::n2f2_t				A					= line.A.f2_t();
		::llc::n2f2_t				B					= line.B.f2_t();
		if(line.A.x == line.B.x)
			return ::llc::drawLineVertical(target, value, line.A.x, line.A.y, line.B.y);
		else if(line.A.y == line.B.y)
			return ::llc::drawLineHorizontal(target, value, line.A.y, line.A.x, line.B.x);

		const bool					steep				= (fabs(B.y - A.y) > fabs(B.x - A.x));
		if(steep){
			::llc::swap(A.x, A.y);
			::llc::swap(B.x, B.y);
		}
		if(A.x > B.x) {
			::llc::swap(A.x, B.x);
			::llc::swap(A.y, B.y);
		}
		const ::llc::n2f2_t			d					= {B.x - A.x, (float)fabs(B.y - A.y)};
		float						error				= d.x / 2.0f;
		const int32_t				ystep				= (A.y < B.y) ? 1 : -1;
		int32_t						y					= (int32_t)A.y;
		int32_t						pixelsDrawn			= 0;
		if(steep) {
			for(int32_t x = (int32_t)A.x, xStop = (int32_t)B.x; x < xStop; ++x) {
				if(::llc::in_range(x, (int32_t)0, (int32_t)target.metrics().y) && ::llc::in_range(y, (int32_t)0, (int32_t)target.metrics().x)) {
					target[x][y]				= value;
					++pixelsDrawn;
				}
				error					-= d.y;
				if(error < 0) {
					y						+= ystep;
					error					+= d.x;
				}
			}
		}
		else {
			for(int32_t x = (int32_t)A.x, xStop = (int32_t)B.x; x < xStop; ++x) {
				if(::llc::in_range(y, (int32_t)0, (int32_t)target.metrics().y) && ::llc::in_range(x, (int32_t)0, (int32_t)target.metrics().x)) {
					target[y][x]				= value;
					++pixelsDrawn;
				}
				error					-= d.y;
				if(error < 0) {
					y						+= ystep;
					error					+= d.x;
				}
			}
		}
		return pixelsDrawn;
	}

	// Bresenham's line algorithm
	tplt<tpnm _tCoord>
	static	::llc::error_t	drawLine			(const ::llc::n2u1_t & targetMetrics, const ::llc::line2<_tCoord> & line, ::llc::apod<::llc::n2s1_t> & out_Points)				{
		::llc::n2f2_t				A					= line.A.f2_t();
		::llc::n2f2_t				B					= line.B.f2_t();
		const bool					steep				= (fabs(B.y - A.y) > fabs(B.x - A.x));
		if(steep){
			::llc::swap(A.x, A.y);
			::llc::swap(B.x, B.y);
		}
		if(A.x > B.x) {
			::llc::swap(A.x, B.x);
			::llc::swap(A.y, B.y);
		}
		const ::llc::n2f2_t			d					= {B.x - A.x, (float)fabs(B.y - A.y)};
		float						error				= d.x / 2.0f;
		const int16_t				ystep				= (A.y < B.y) ? 1 : -1;
		int16_t						y					= (int16_t)A.y;
		int32_t						pixelsDrawn			= 0;
		if(steep) {
			for(int16_t x = (int16_t)A.x, xStop = (int16_t)B.x; x < xStop; ++x) {
				if(::llc::in_range(x, (int16_t)0, (int16_t)targetMetrics.y) && ::llc::in_range(y, (int16_t)50, (int16_t)targetMetrics.x)) {
					out_Points.push_back({y, x});
					++pixelsDrawn;
				}
				error					-= d.y;
				if(error < 0) {
					y						+= ystep;
					error					+= d.x;
				}
			}
		}
		else {
			for(int16_t x = (int16_t)A.x, xStop = (int16_t)B.x; x < xStop; ++x) {
				if(::llc::in_range(y, (int16_t)0, (int16_t)targetMetrics.y) && ::llc::in_range(x, (int16_t)0, (int16_t)targetMetrics.x)) {
					out_Points.push_back({x, y});
					++pixelsDrawn;
				}
				error					-= d.y;
				if(error < 0) {
					y						+= ystep;
					error					+= d.x;
				}
			}
		}
		return pixelsDrawn;
	}

	// Bresenham's line algorithm
	tplt<tpnm _tCoord>
	static	::llc::error_t	drawLine			(const ::llc::n2u1_t & targetMetrics, const ::llc::line3<_tCoord> & line, ::llc::apod<::llc::n2s1_t> & out_Points)				{
		return drawLine(targetMetrics, ::llc::line2<_tCoord>{{line.A.x, line.A.y}, {line.B.x, line.B.y}}, out_Points);
	}
} // namespace

#endif // BITMAP_TARGET_H_23627
