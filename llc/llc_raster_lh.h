#include "llc_grid.h"
#include "llc_matrix3.h"
#include "llc_color.h"
#include "llc_geometry_lh.h"
#include "llc_grid_color.h"

#include "llc_array_pod.h"
#include "llc_line2.h"
#include "llc_line3.h"
#include "llc_circle.h"
#include "llc_rect2.h"

#include "llc_functional.h"

#ifndef CED_DRAW_H_23627
#define CED_DRAW_H_23627

namespace llc
{
	struct SLight3 {
		::llc::n3f2_t	Position;
		f2_t			Range;
	};

	int				pixelBlend			(::llc::g8bgra pixels, ::llc::n2s1_t position, ::llc::bgra color);
	int				setPixel			(::llc::g8bgra pixels, ::llc::n2s1_t position, ::llc::bgra color);
	int				drawRectangle		(::llc::g8bgra pixels, ::llc::rect2s1_t  rectangle	, ::llc::bgra color);
	int				drawCircle			(::llc::g8bgra pixels, ::llc::circles1_t circle		, ::llc::bgra color);
	int				drawLine			(::llc::g8bgra pixels, ::llc::line2s1_t  line		, ::llc::bgra color);
	int				drawLine			(::llc::n2u1_c offscreenMetrics, ::llc::line2s1_t line, ::llc::apod<::llc::n2s1_t> & pixelCoords);
	int				drawLine
		( const ::llc::n2<::llc::u1_t>	offscreenMetrics
		, const ::llc::line3f2_t		& lineFloat
		, ::llc::apod<::llc::n3f2_t>	& pixelCoords
		, ::llc::grid<::llc::u2_t>	depthBuffer
		);
	int				drawTriangle		(::llc::g8bgra pixels, const ::llc::tri2s1_t & triangle, ::llc::bgra color);
	int				drawTriangle		(::llc::n2u2_t targetSize, const ::llc::tri3f2_t & triangle, ::llc::apod<::llc::n2s1_t> & pixelCoords, ::llc::apod<::llc::trif2_t> & proportions, ::llc::grid<::llc::u2_t> depthBuffer);
	int				drawTriangle		(::llc::n2u2_t targetSize, const ::llc::tri2f2_t & triangle, ::llc::apod<::llc::n2s1_t> & pixelCoords, ::llc::apod<::llc::trif2_t> & proportions);
	int				drawQuadTriangle
		( ::llc::g8bgra					targetPixels
		, const ::llc::SGeometryQuads	& geometry
		, const int						iTriangle
		, const ::llc::m4f2_t			& matrixTransform
		, const ::llc::m4f2_t			& matrixTransformView
		, const ::llc::n3f2_t			& lightVector
		, const ::llc::bgra				color
		, ::llc::apod<::llc::n2s1_t>	& pixelCoords
		, ::llc::apod<::llc::trif2_t>	& pixelVertexWeights
		, ::llc::grid<::llc::u2_t>			depthBuffer
		);
	int				drawQuadTriangle
		( ::llc::g8bgra						targetPixels
		, const ::llc::SGeometryQuads		& geometry
		, const int							iTriangle
		, const ::llc::m4f2_t				& matrixTransform
		, const ::llc::m4f2_t				& matrixTransformView
		, const ::llc::n3f2_t				& lightVector
		, ::llc::apod<::llc::n2s1_t>		& pixelCoords
		, ::llc::apod<::llc::trif2_t>		& pixelVertexWeights
		, ::llc::gc8bgra					textureImage
		, ::llc::apod<::llc::n3f2_t>		& lightPoints
		, ::llc::a8bgra						& lightColors
		, ::llc::grid<::llc::u2_t>				depthBuffer
		);

	int				drawQuadTriangle
		( ::llc::g8bgra						targetPixels
		, const ::llc::SGeometryQuads		& geometry
		, const int							iTriangle
		, const ::llc::m4f2_t				& matrixTransform
		, const ::llc::m4f2_t				& matrixTransformView
		, const ::llc::n3f2_t				& lightVector
		, ::llc::apod<::llc::n2s1_t>		& pixelCoords
		, ::llc::apod<::llc::trif2_t>		& pixelVertexWeights
		, ::llc::gc8bgra					textureImage
		, ::llc::apod<::llc::n3f2_t>		& lightPoints
		, ::llc::a8bgra						& lightColors
		, ::llc::grid<::llc::u2_t>				depthBuffer
		, const ::llc::function<::llc::err_t(::llc::g8bgra targetPixels, const ::llc::n2s1_t & pixelCoord, const ::llc::bgra & color)> & funcSetPixel
		);

	int				drawQuadTriangle
		( ::llc::n2u2_c						& targetSize
		, ::llc::tri3f2_t					triangle
		, const ::llc::m4f2_t				& matrixTransformView
		, ::llc::apod<::llc::n2s1_t>		& pixelCoords
		, ::llc::apod<::llc::trif2_t>		& pixelVertexWeights
		, ::llc::grid<::llc::u2_t>				depthBuffer
		);
	int				drawQuadTriangle
		( ::llc::n2u2_c						& targetSize
		, const ::llc::tri3f2_t				& triangleScreen
		, ::llc::apod<::llc::n2s1_t>		& pixelCoords
		, ::llc::apod<::llc::trif2_t>		& pixelVertexWeights
		, ::llc::grid<::llc::u2_t>				depthBuffer
		);
	int				drawTriangle
		( ::llc::g8bgra						targetPixels
		, const ::llc::SGeometryTriangles	& geometry
		, const int							iTriangle
		, const ::llc::m4f2_t				& matrixTransform
		, const ::llc::m4f2_t				& matrixTransformView
		, const ::llc::n3f2_t				& lightVector
		, ::llc::bgra						color
		, ::llc::apod<::llc::n2s1_t>		& pixelCoords
		, ::llc::apod<::llc::trif2_t>		& pixelVertexWeights
		, ::llc::grid<::llc::u2_t>				depthBuffer
		);
	int				drawTriangle
		( ::llc::g8bgra						targetPixels
		, const ::llc::SGeometryTriangles	& geometry
		, const int							iTriangle
		, const ::llc::m4f2_t				& matrixTransform
		, const ::llc::m4f2_t				& matrixTransformView
		, const ::llc::n3f2_t				& lightVector
		, ::llc::apod<::llc::n2s1_t>		& pixelCoords
		, ::llc::apod<::llc::trif2_t>		& pixelVertexWeights
		, ::llc::gc8bgra					textureImage
		, ::llc::grid<::llc::u2_t>				depthBuffer
		);
	int				drawTriangle
		( ::llc::g8bgra						targetPixels
		, const ::llc::SGeometryTriangles	& geometry
		, const int							iTriangle
		, const ::llc::m4f2_t				& matrixTransform
		, const ::llc::m4f2_t				& matrixTransformView
		, const ::llc::n3f2_t				& lightVector
		, const ::llc::rgbaf				& lightColor
		, ::llc::apod<::llc::n2s1_t>		& pixelCoords
		, ::llc::apod<::llc::trif2_t>		& pixelVertexWeights
		, ::llc::gc8bgra					textureImage
		, ::llc::apod<::llc::SLight3>		& lightPoints
		, ::llc::a8bgra						& lightColors
		, ::llc::grid<::llc::u2_t>				depthBuffer
		);

	int				drawPixels
		( ::llc::g8bgra						targetPixels
		, const ::llc::tri3f2_t				& triangleWorld
		, const ::llc::n3f2_t				& normal
		, const ::llc::tri2f2_t				& triangleTexCoords
		, const ::llc::n3f2_t				& lightVector
		, ::llc::apod<::llc::n2s1_t>		& pixelCoords
		, ::llc::apod<::llc::trif2_t>		& pixelVertexWeights
		, ::llc::gc8bgra					textureImage
		, ::llc::apod<::llc::n3f2_t>		& lightPoints
		, ::llc::a8bgra						& lightColors
		, const ::llc::function<::llc::err_t(::llc::g8bgra targetPixels, ::llc::n2s1_c & pixelCoord, const ::llc::bgra & color)> & funcSetPixel
		);

	int				drawPixels
		( ::llc::g8bgra						targetPixels
		, const ::llc::tri3f2_t				& triangleWorld
		, const ::llc::n3f2_t				& normal
		, const ::llc::tri2f2_t				& triangleTexCoords
		, const ::llc::n3f2_t				& lightVector
		, ::llc::apod<::llc::n2s1_t>		& pixelCoords
		, ::llc::apod<::llc::trif2_t>		& pixelVertexWeights
		, ::llc::gc8bgra					textureImage
		, ::llc::apod<::llc::n3f2_t>		& lightPoints
		, ::llc::a8bgra						& lightColors
		);
} // namespace

#endif // CED_DRAW_H_23627
