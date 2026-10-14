#include "llc_raster_lh.h"

#ifndef LLC_ATMEL

#include <algorithm>

::llc::error_t			llc::pixelBlend			(::llc::g8bgra pixels, ::llc::n2s1_t position, ::llc::bgra color)	{
	if( (position.x >= 0 && position.x < (int32_t)pixels.metrics().x)
	 && (position.y >= 0 && position.y < (int32_t)pixels.metrics().y)
	) {
		::llc::bgra					& targetPixel	= pixels[position.y][position.x];
		::llc::rgbaf				srcPixel		= targetPixel;
		targetPixel				= ::llc::interpolate_linear(srcPixel, ::llc::rgbaf(color), color.a / 255.0f);
		return 1;
	}
	return 0;
}

::llc::error_t			llc::setPixel			(::llc::g8bgra pixels, ::llc::n2s1_t position, ::llc::bgra color)	{
	if( (position.x >= 0 && position.x < (int32_t)pixels.metrics().x)
	 && (position.y >= 0 && position.y < (int32_t)pixels.metrics().y)
	) {
		::llc::bgra					& targetPixel = pixels[position.y][position.x];
		targetPixel				= {color.b, color.g, color.r, (uint8_t)::llc::clamped(targetPixel.a + (uint32_t)color.a, 0U, 255U)};
		return 1;
	}
	return 0;
}

::llc::error_t			llc::drawRectangle		(::llc::g8bgra pixels, ::llc::rect2<int16_t> rectangle, ::llc::bgra color)	{
	int32_t						countPixels				= 0;
	for(int16_t y = 0; y < (int16_t)rectangle.Size.y; ++y)
	for(int16_t x = 0; x < (int16_t)rectangle.Size.x; ++x)
		countPixels += ::llc::setPixel(pixels, {(int16_t)(rectangle.Offset.x + x), (int16_t)(rectangle.Offset.y + y)}, color);
	return countPixels;
}

::llc::error_t			llc::drawCircle			(::llc::g8bgra pixels, ::llc::circle<int16_t> circle, ::llc::bgra color)	{
	int32_t						countPixels				= 0;
	for(int16_t y = (int16_t)-circle.Radius; y < (int16_t)circle.Radius; ++y)
	for(int16_t x = (int16_t)-circle.Radius; x < (int16_t)circle.Radius; ++x) {
		::llc::n2s1_t				position			= {x, y};
		if(position.Length() <= circle.Radius)
			countPixels += ::llc::setPixel(pixels, {int16_t(circle.Center.x + x), int16_t(circle.Center.y + y)}, color);
	}
	return countPixels;
}

// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
::llc::error_t			llc::drawLine       	(::llc::g8bgra pixels, ::llc::line2s1_t line, ::llc::bgra color)	{
	int32_t						countPixels				= 0;
	int32_t						dx						= (int32_t)fabs(line.B.x - line.A.x);
	int32_t						sx						= (int32_t)line.A.x < line.B.x ? 1 : -1;
	int32_t						dy						= (int32_t)-fabs(line.B.y - line.A.y);
	int32_t						sy						= (int32_t)line.A.y < line.B.y ? 1 : -1;
	int32_t						err						= dx + dy;  /* error value e_xy */
	while (true) {   /* loop */
		if (line.A.x == line.B.x && line.A.y == line.B.y)
			break;
		int32_t						e2						= 2 * err;
		if (e2 >= dy) {
			err						+= dy; /* e_xy+e_x > 0 */
			line.A.x				+= (int16_t)sx;
			countPixels += setPixel(pixels, {line.A.x, line.A.y}, color);
		}
		if (e2 <= dx) { /* e_xy+e_y < 0 */
			err						+= dx;
			line.A.y				+= (int16_t)sy;
			countPixels += setPixel(pixels, {line.A.x, line.A.y}, color);
		}

	}
	return countPixels;
}


// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
::llc::error_t			llc::drawLine       	(const ::llc::n2u1_t offscreenMetrics, ::llc::line2s1_t line, ::llc::apod<::llc::n2s1_t> & pixelCoords)	{
	int32_t						dx						= (int32_t)fabs(line.B.x - line.A.x);
	int32_t						sx						= (int32_t)line.A.x < line.B.x ? 1 : -1;
	int32_t						dy						= (int32_t)-fabs(line.B.y - line.A.y );
	int32_t						sy						= (int32_t)line.A.y < line.B.y ? 1 : -1;
	int32_t						err						= dx + dy;  /* error value e_xy */
	pixelCoords.push_back({line.A.x, line.A.y});
	while (true) {   /* loop */
		if (line.A.x == line.B.x && line.A.y == line.B.y)
			break;
		int32_t						e2						= 2 * err;
		if (e2 >= dy) {
			err						+= dy; /* e_xy+e_x > 0 */
			line.A.x				+= (int16_t)sx;
			if( line.A.x >= 0 && line.A.x < (int32_t)offscreenMetrics.x
			 && line.A.y >= 0 && line.A.y < (int32_t)offscreenMetrics.y
			)
				pixelCoords.push_back({line.A.x, line.A.y});

		}
		if (e2 <= dx) { /* e_xy+e_y < 0 */
			err						+= dx;
			line.A.y				+= (int16_t)sy;
			if( line.A.x >= 0 && line.A.x < (int32_t)offscreenMetrics.x
			 && line.A.y >= 0 && line.A.y < (int32_t)offscreenMetrics.y
			)
				pixelCoords.push_back({line.A.x, line.A.y});
		}

	}
	return 0;
}


// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
::llc::error_t			llc::drawLine
	( const ::llc::n2u1_t		offscreenMetrics
	, const ::llc::line3f2_t		& lineFloat
	, ::llc::apod<::llc::n3f2_t>	& pixelCoords
	, ::llc::grid<uint32_t>	depthBuffer
	) {
	::llc::line2s2_t				line					= {{(int32_t)lineFloat.A.x, (int32_t)lineFloat.A.y}, {(int32_t)lineFloat.B.x, (int32_t)lineFloat.B.y}};
	int32_t						xDiff					= (int32_t)fabs(line.B.x - line.A.x);
	int32_t						yDiff					= (int32_t)-fabs(line.B.y - line.A.y);
	int32_t						sx						= (int32_t)line.A.x < line.B.x ? 1 : -1;
	int32_t						sy						= (int32_t)line.A.y < line.B.y ? 1 : -1;
	int32_t						err						= xDiff + yDiff;  /* error value e_xy */

	bool						yAxis					= fabs(yDiff) > xDiff;
	::llc::vu2_t					depthBufferRow			= {};
	if( line.A.x >= 0 && line.A.x < (int32_t)offscreenMetrics.x
	 && line.A.y >= 0 && line.A.y < (int32_t)offscreenMetrics.y
	 && lineFloat.A.z >= 0 && lineFloat.A.z <= 1
	) {
		depthBufferRow			= depthBuffer[line.A.y];
		uint32_t					& depthCell				= depthBufferRow[line.A.x];
		uint32_t					intZ					= uint32_t((double)0xFFFFFFFU * lineFloat.A.z);
		if( depthCell > intZ ) {
			depthCell				= intZ;
			//pixelCoords.push_back({(int32_t)line.A.x, (int32_t)line.A.y});
			pixelCoords.push_back(lineFloat.A);
		}
	}

	//bool								outside				= true;
	const double				factorUnit				= 1.0 / (yAxis ? fabs(yDiff) : xDiff);
	while (true) {   /* loop */
		if (line.A.x == line.B.x && line.A.y == line.B.y)
			break;
		int32_t						e2						= 2 * err;
		if (e2 >= yDiff) {
			err						+= yDiff; /* e_xy+e_x > 0 */
			line.A.x				+= sx;
			if( line.A.x >= 0 && line.A.x < (int32_t)offscreenMetrics.x
			 && line.A.y >= 0 && line.A.y < (int32_t)offscreenMetrics.y
			) {
				depthBufferRow			= depthBuffer[line.A.y];
				const double				factor					= 1.0 / (yAxis ? factorUnit * line.A.y : factorUnit * line.A.x);
				const double				finalZ					= ::llc::interpolate_linear(lineFloat.A.z, lineFloat.B.z, factor);// lineFloat.B.z * factor + (lineFloat.A.z * (1.0 - factor));
				if (finalZ <= 0 || finalZ >= 1)
					continue;
				const uint32_t				intZ					= uint32_t(0xFFFFFFFU * finalZ);
				uint32_t					& depthCell				= depthBufferRow[line.A.x];
				if(depthCell <= intZ)
					continue;

				depthCell				= intZ;
				pixelCoords.push_back({(float)line.A.x, (float)line.A.y, (float)finalZ});
			}
			//else
			//	outside							= true;
		}
		if (e2 <= xDiff) { /* e_xy+e_y < 0 */
			err						+= xDiff;
			line.A.y				+= sy;
			if( line.A.x >= 0 && line.A.x < (int32_t)offscreenMetrics.x
			 && line.A.y >= 0 && line.A.y < (int32_t)offscreenMetrics.y
			) {
				depthBufferRow					= depthBuffer[line.A.y];
				const double				factor					= 1.0 / (yAxis ? factorUnit * line.A.y : factorUnit * line.A.x);
				const double				finalZ					= ::llc::interpolate_linear(lineFloat.A.z, lineFloat.B.z, factor);// lineFloat.B.z * factor + (lineFloat.A.z * (1.0 - factor));
				if (finalZ <= 0 || finalZ >= 1)
					continue;
				const uint32_t				intZ					= uint32_t(0xFFFFFFFU * finalZ);
				uint32_t					& depthCell				= depthBufferRow[line.A.x];
				if(depthCell <= intZ)
					continue;

				depthCell				= intZ;
				pixelCoords.push_back({(float)line.A.x, (float)line.A.y, (float)finalZ});
			}
			//else
			//	outside					= true;
		}

	}
	return 0;
}

//https://fgiesen.wordpress.com/2013/02/08/triangle-rasterization-in-practice/
static	double		orient2d		(const ::llc::line2s1_t & segment, const ::llc::n2s1_t & point)	{ return (segment.B.x - segment.A.x) * (point.y - (double)segment.A.y) - (segment.B.y - segment.A.y) * (point.x - (double)segment.A.x); }
//static	double		orient2d		(const ::llc::line3s1_t & segment, const ::llc::n2s1_t & point)	{ return (segment.B.x - segment.A.x) * (point.y - (double)segment.A.y) - (segment.B.y - segment.A.y) * (point.x - (double)segment.A.x); }
//static	double		orient2d		(const ::llc::line3f2_t & segment, const ::llc::n2s1_t & point)	{ return (segment.B.x - segment.A.x) * (point.y - (double)segment.A.y) - (segment.B.y - segment.A.y) * (point.x - (double)segment.A.x); }
static	double		orient2d		(const ::llc::line2f2_t & segment, const ::llc::n2s1_t & point)	{ return (segment.B.x - segment.A.x) * (point.y - (double)segment.A.y) - (segment.B.y - segment.A.y) * (point.x - (double)segment.A.x); }
static	double		orient2d		(const ::llc::n3f2_t & start, const ::llc::n2f2_t & edge, const ::llc::n2s1_t & point)	{ return edge.x * (point.y - (double)start.y) - edge.y * (point.x - (double)start.x); }

tplt <tpnm _tValue>	_tValue	max3		(_tValue & a, _tValue & b, _tValue & c)			{ return ::std::max(::std::max(a, b), c); }
tplt <tpnm _tValue>	_tValue	min3		(_tValue & a, _tValue & b, _tValue & c)			{ return ::std::min(::std::min(a, b), c); }

::llc::error_t			llc::drawTriangle		(::llc::g8bgra pixels, const ::llc::tri2<int16_t> & triangle, ::llc::bgra color){
	// Compute triangle bounding box
	int16_t						minX					= ::min3(triangle.A.x, triangle.B.x, triangle.C.x);
	int16_t						minY					= ::min3(triangle.A.y, triangle.B.y, triangle.C.y);
	int16_t						maxX					= ::max3(triangle.A.x, triangle.B.x, triangle.C.x);
	int16_t						maxY					= ::max3(triangle.A.y, triangle.B.y, triangle.C.y);

	// Clip against screen bounds
	minX							= ::std::max(minX, (int16_t)0);
	minY							= ::std::max(minY, (int16_t)0);
	maxX							= ::std::min(maxX, int16_t(int16_t(pixels.metrics().x)	- 1));
	maxY							= ::std::min(maxY, int16_t(int16_t(pixels.metrics().y)	- 1));

	// Rasterize
	int32_t						countPixels				= 0;
	::llc::n2s1_t				p;
	for (p.y = minY; p.y <= maxY; ++p.y) 
	for (p.x = minX; p.x <= maxX; ++p.x) {
		// Determine barycentric coordinates
		double						w0						= ::orient2d({triangle.B, triangle.C}, p);
		double						w1						= ::orient2d({triangle.C, triangle.A}, p);
		double						w2						= ::orient2d({triangle.A, triangle.B}, p);
		// If p is on or inside all edges, render pixel.
		if (w0 < 0 || w1 < 0 || w2 < 0)
			continue;
		countPixels += ::llc::setPixel(pixels, p, color);
	}
	return countPixels;
}

::llc::error_t			llc::drawTriangle
	( const ::llc::n2u2_t			targetSize
	, const ::llc::tri3f2_t			& triangle
	, ::llc::apod<::llc::n2s1_t>		& pixelCoords
	, ::llc::apod<::llc::trif2_t>	& proportions
	, ::llc::grid<uint32_t>		depthBuffer
	)	{
	// Compute triangle bounding box
	int16_t						minX					= (int16_t)::min3(triangle.A.x, triangle.B.x, triangle.C.x);
	int16_t						minY					= (int16_t)::min3(triangle.A.y, triangle.B.y, triangle.C.y);
	int16_t						maxX					= (int16_t)::max3(triangle.A.x, triangle.B.x, triangle.C.x);
	int16_t						maxY					= (int16_t)::max3(triangle.A.y, triangle.B.y, triangle.C.y);

	// Clip against screen bounds
	minX					= ::std::max(minX, (int16_t)0);
	minY					= ::std::max(minY, (int16_t)0);
	maxX					= ::std::min(maxX, (int16_t)((int32_t)targetSize.x - 1));
	maxY					= ::std::min(maxY, (int16_t)((int32_t)targetSize.y - 1));

	// Rasterize
	const ::llc::n2f2_t			edgeBC					= {triangle.C.x - triangle.B.x, triangle.C.y - triangle.B.y};
	const ::llc::n2f2_t			edgeCA					= {triangle.A.x - triangle.C.x, triangle.A.y - triangle.C.y};
	const ::llc::n2f2_t			edgeAB					= {triangle.B.x - triangle.A.x, triangle.B.y - triangle.A.y};
	::llc::n2s1_t				p;
	for (p.y = minY; p.y <= maxY; ++p.y)
	for (p.x = minX; p.x <= maxX; ++p.x) {
		// Determine barycentric coordinates
		double						w0						= ::orient2d(triangle.B, edgeBC, p);
		double						w1						= ::orient2d(triangle.C, edgeCA, p);
		double						w2						= ::orient2d(triangle.A, edgeAB, p);
		// If p is on or inside all edges, render pixel.
		if (w0 < 0 || w1 < 0 || w2 < 0)
			continue;

		double						proportionABC			= w0 + w1 + w2;
		if(proportionABC == 0)
			continue;
		double						proportionA				= w0 / proportionABC;
		double						proportionB				= w1 / proportionABC;
		double						proportionC				= 1.0 - (proportionA + proportionB);

		// Calculate interpolated depth
		double						finalZ					= triangle.A.z * proportionA;
		finalZ					+= triangle.B.z * proportionB;
		finalZ					+= triangle.C.z * proportionC;
		if(finalZ >= 1.0 || finalZ <= 0)
			continue;

		uint32_t					intZ					= uint32_t(0xFFFFFFFU * finalZ);
		uint32_t					& currentDepth			= depthBuffer[p.y][p.x];
		if(currentDepth <= intZ)
			continue;

		currentDepth			= intZ;
		pixelCoords.push_back(p.s1());
		proportions.push_back({(float)proportionA, (float)proportionB, (float)proportionC});

	}
	return 0;
}

::llc::error_t			llc::drawTriangle
	( const ::llc::n2u2_t			targetSize
	, const ::llc::tri2f2_t			& triangle
	, ::llc::apod<::llc::n2s1_t>		& pixelCoords
	, ::llc::apod<::llc::trif2_t>	& proportions
	)	{
	// Compute triangle bounding box
	int16_t						minX					= (int16_t)::min3(triangle.A.x, triangle.B.x, triangle.C.x);
	int16_t						minY					= (int16_t)::min3(triangle.A.y, triangle.B.y, triangle.C.y);
	int16_t						maxX					= (int16_t)::max3(triangle.A.x, triangle.B.x, triangle.C.x);
	int16_t						maxY					= (int16_t)::max3(triangle.A.y, triangle.B.y, triangle.C.y);

	// Clip against screen bounds
	minX					= ::std::max(minX, (int16_t)0);
	minY					= ::std::max(minY, (int16_t)0);
	maxX					= ::std::min(maxX, (int16_t)((int32_t)targetSize.x - 1));
	maxY					= ::std::min(maxY, (int16_t)((int32_t)targetSize.y - 1));

	// Rasterize
	::llc::n2s1_t p;
	for (p.y = minY; p.y <= maxY; ++p.y)
	for (p.x = minX; p.x <= maxX; ++p.x) {
		// Determine barycentric coordinates
		double						w0						= ::orient2d({triangle.B, triangle.C}, p);
		double						w1						= ::orient2d({triangle.C, triangle.A}, p);
		double						w2						= ::orient2d({triangle.A, triangle.B}, p);
		// If p is on or inside all edges, render pixel.
		if (w0 < 0 || w1 < 0 || w2 < 0)
			continue;

		double						proportionABC			= w0 + w1 + w2;
		if(proportionABC == 0)
			continue;
		double						proportionA				= w0 / proportionABC;
		double						proportionB				= w1 / proportionABC;
		double						proportionC				= 1.0 - (proportionA + proportionB);

		pixelCoords.push_back(p.s1());
		proportions.push_back({(float)proportionA, (float)proportionB, (float)proportionC});

	}
	return 0;
}

::llc::error_t			llc::drawPixels
	( ::llc::g8bgra		targetPixels
	, const ::llc::tri3f2_t				& triangleWorld
	, const ::llc::n3f2_t				& normal
	, const ::llc::tri2f2_t				& triangleTexCoords
	, const ::llc::n3f2_t				& lightVector
	, ::llc::apod<::llc::n2s1_t>			& pixelCoords
	, ::llc::apod<::llc::trif2_t>		& pixelVertexWeights
	, ::llc::gc8bgra					textureImage
	, ::llc::apod<::llc::n3f2_t>			& lightPoints
	, ::llc::a8bgra			& lightColors
	, const ::llc::function<::llc::error_t(::llc::g8bgra targetPixels, const ::llc::n2s1_t & pixelCoord, const ::llc::bgra & color)> & funcSetPixel
	) {
	const ::llc::n2f2_t			imageUnit				= {textureImage.metrics().x - 0.000001f, textureImage.metrics().y - 0.000001f};
	double						lightFactorDirectional	= normal.Dot(lightVector);
	(void)lightFactorDirectional;
	int32_t						countPixels				= 0;
	for(uint32_t iPixelCoord = 0; iPixelCoord < pixelCoords.size(); ++iPixelCoord) {
		::llc::n2s1_t				pixelCoord				= pixelCoords		[iPixelCoord];
		const ::llc::trif2_t			& vertexWeights			= pixelVertexWeights[iPixelCoord];
		const ::llc::n2f2_t			texCoord				= ::llc::triangleWeight(vertexWeights, triangleTexCoords);
		const ::llc::n3f2_t			position				= ::llc::triangleWeight(vertexWeights, triangleWorld);
		const ::llc::rgbaf			texelColor				= textureImage.size() ? textureImage[(uint32_t)(texCoord.y * imageUnit.y) % textureImage.metrics().y][(uint32_t)(texCoord.x * imageUnit.x) % textureImage.metrics().x] : ::llc::bgra{::llc::GRAY};
		const uint32_t				countLights				= lightPoints.size();
		const ::llc::rgbaf			ambientPerLight			= countLights ? texelColor / (10.0 * countLights) : ::llc::rgbaf{};
		::llc::rgbaf				fragmentColor			= {};
		stacxpr	double				rangeLight				= 10.0;
		stacxpr	double				rangeLightSquared		= rangeLight * rangeLight;
		stacxpr	double				rangeUnit				= 1.0 / rangeLightSquared;
		for(uint32_t iLight = 0; iLight < countLights; ++iLight) {
			const ::llc::n3f2_t			lightToPoint			= lightPoints[iLight] - position;
			const double				distanceToLight			= lightToPoint.LengthSquared();
			if(distanceToLight > rangeLightSquared)
				continue;
			fragmentColor			+= ambientPerLight;
			const double				lightFactor				= lightToPoint.Dot(normal);
			if(lightFactor <= 0)
				continue;
			const double				invAttenuation			= ::std::max(0.0, 1.0 - (distanceToLight * rangeUnit));
			fragmentColor			+= (texelColor * lightColors[iLight]) * (invAttenuation);
		}
		countPixels += funcSetPixel(targetPixels, pixelCoord, (texelColor * .2 + (texelColor * lightFactorDirectional).Clamp() + fragmentColor.Clamp()).Clamp());
	}
	return countPixels;
}

::llc::error_t			llc::drawPixels
	( ::llc::g8bgra	targetPixels
	, const ::llc::tri3f2_t			& triangleWorld
	, const ::llc::n3f2_t			& normal
	, const ::llc::tri2f2_t			& triangleTexCoords
	, const ::llc::n3f2_t			& lightVector
	, ::llc::apod<::llc::n2s1_t>		& pixelCoords
	, ::llc::apod<::llc::trif2_t>	& pixelVertexWeights
	, ::llc::gc8bgra	textureImage
	, ::llc::apod<::llc::n3f2_t>		& lightPoints
	, ::llc::a8bgra		& lightColors
	) {

	return llc::drawPixels(targetPixels, triangleWorld, normal, triangleTexCoords, lightVector, pixelCoords, pixelVertexWeights, textureImage, lightPoints, lightColors, setPixel);
}

::llc::error_t			llc::drawQuadTriangle
	( const ::llc::n2u2_t			& targetSize
	, const ::llc::tri3f2_t		& triangleScreen
	, ::llc::apod<::llc::n2s1_t>		& pixelCoords
	, ::llc::apod<::llc::trif2_t>	& pixelVertexWeights
	, ::llc::grid<uint32_t>		depthBuffer
	) {
	return ::llc::drawTriangle(targetSize, triangleScreen, pixelCoords, pixelVertexWeights, depthBuffer);
}

::llc::error_t			llc::drawQuadTriangle
	( const ::llc::n2u2_t			& targetSize
	, ::llc::tri3f2_t				triangle
	, const ::llc::m4f2_t			& matrixTransformVP
	, ::llc::apod<::llc::n2s1_t>		& pixelCoords
	, ::llc::apod<::llc::trif2_t>	& pixelVertexWeights
	, ::llc::grid<uint32_t>		depthBuffer
	) {
	::llc::transform(triangle, matrixTransformVP);
	if( triangle.A.z <= 0 || triangle.A.z >= 1
	 || triangle.B.z <= 0 || triangle.B.z >= 1
	 || triangle.C.z <= 0 || triangle.C.z >= 1
	)
		return 0;
	return ::llc::drawQuadTriangle(targetSize, triangle, pixelCoords, pixelVertexWeights, depthBuffer);
}

::llc::error_t			llc::drawQuadTriangle
	( ::llc::g8bgra	targetPixels
	, const ::llc::SGeometryQuads	& geometry
	, const int						iTriangle
	, const ::llc::m4f2_t			& matrixTransform
	, const ::llc::m4f2_t			& matrixTransformVP
	, const ::llc::n3f2_t			& lightVector
	, ::llc::apod<::llc::n2s1_t>		& pixelCoords
	, ::llc::apod<::llc::trif2_t>	& pixelVertexWeights
	, ::llc::gc8bgra	textureImage
	, ::llc::apod<::llc::n3f2_t>		& lightPoints
	, ::llc::a8bgra		& lightColors
	, ::llc::grid<uint32_t>		depthBuffer
	) {
	return ::llc::drawQuadTriangle(targetPixels, geometry, iTriangle, matrixTransform, matrixTransformVP, lightVector, pixelCoords, pixelVertexWeights, textureImage, lightPoints, lightColors, depthBuffer, setPixel);
}

::llc::error_t			llc::drawQuadTriangle
	( ::llc::g8bgra	targetPixels
	, const ::llc::SGeometryQuads	& geometry
	, const int						iTriangle
	, const ::llc::m4f2_t			& matrixTransform
	, const ::llc::m4f2_t			& matrixTransformVP
	, const ::llc::n3f2_t			& lightVector
	, ::llc::apod<::llc::n2s1_t>		& pixelCoords
	, ::llc::apod<::llc::trif2_t>	& pixelVertexWeights
	, ::llc::gc8bgra	textureImage
	, ::llc::apod<::llc::n3f2_t>		& lightPoints
	, ::llc::a8bgra		& lightColors
	, ::llc::grid<uint32_t>		depthBuffer
	, const ::llc::function<::llc::error_t(::llc::g8bgra targetPixels, const ::llc::n2s1_t & pixelCoord, const ::llc::bgra & color)> & funcSetPixel
	) {
	const ::llc::tri3f2_t		& triangle			= geometry.Triangles	[iTriangle];;
	const ::llc::tri2f2_t		& triangleTexCoords	= geometry.TextureCoords[iTriangle];
	const ::llc::n3f2_t			& normal			= geometry.Normals		[iTriangle / 2];
	::llc::drawQuadTriangle(targetPixels.metrics(), triangle, matrixTransformVP, pixelCoords, pixelVertexWeights, depthBuffer);
	const ::llc::n3f2_t			xnormal				= matrixTransform.TransformDirection(normal).Normalize();
	::llc::tri3f2_t				triangleWorld		= triangle;
	::llc::transform(triangleWorld, matrixTransform);
	return ::llc::drawPixels(targetPixels, triangleWorld, xnormal, triangleTexCoords, lightVector, pixelCoords, pixelVertexWeights, textureImage, lightPoints, lightColors, funcSetPixel);
}

::llc::error_t			llc::drawQuadTriangle
	( ::llc::g8bgra	targetPixels
	, const ::llc::SGeometryQuads	& geometry
	, const int						iTriangle
	, const ::llc::m4f2_t			& matrixTransform
	, const ::llc::m4f2_t			& matrixTransformView
	, const ::llc::n3f2_t			& lightVector
	, const ::llc::bgra				color
	, ::llc::apod<::llc::n2s1_t>		& pixelCoords
	, ::llc::apod<::llc::trif2_t>	& pixelVertexWeights
	, ::llc::grid<uint32_t>		depthBuffer
	) {
	::llc::tri3f2_t				triangle			= geometry.Triangles	[iTriangle];
	::llc::n3f2_t				normal				= geometry.Normals		[iTriangle / 2];
	normal					= matrixTransform.TransformDirection(normal).Normalize();
	::llc::transform(triangle, matrixTransformView);
	if( triangle.A.z <= 0 || triangle.A.z >= 1
	 || triangle.B.z <= 0 || triangle.B.z >= 1
	 || triangle.C.z <= 0 || triangle.C.z >= 1
	)
		return 0;

	double						lightFactor			= normal.Dot(lightVector);

	int32_t						countPixels			= 0;
	::llc::drawTriangle(targetPixels.metrics(), triangle, pixelCoords, pixelVertexWeights, depthBuffer);
	for(uint32_t iPixelCoord = 0; iPixelCoord < pixelCoords.size(); ++iPixelCoord) {
		::llc::n2s1_t				pixelCoord			= pixelCoords[iPixelCoord];
		countPixels += ::llc::setPixel(targetPixels, pixelCoord, (color * 0.1) + color * lightFactor);
	}
	return countPixels;
}

::llc::error_t			llc::drawTriangle
	( ::llc::g8bgra	targetPixels
	, const ::llc::SGeometryTriangles	& geometry
	, const int						iTriangle
	, const ::llc::m4f2_t			& matrixTransform
	, const ::llc::m4f2_t			& matrixTransformView
	, const ::llc::n3f2_t			& lightVector
	, const ::llc::bgra				color
	, ::llc::apod<::llc::n2s1_t>		& pixelCoords
	, ::llc::apod<::llc::trif2_t>	& pixelVertexWeights
	, ::llc::grid<uint32_t>		depthBuffer
	) {
	::llc::tri3f2_t				triangle			= geometry.Triangles	[iTriangle];
	const ::llc::tri3f2_t		& triangleNormals	= geometry.Normals		[iTriangle];
	::llc::transform(triangle, matrixTransformView);
	if( triangle.A.z <= 0 || triangle.A.z >= 1
	 || triangle.B.z <= 0 || triangle.B.z >= 1
	 || triangle.C.z <= 0 || triangle.C.z >= 1
	)
		return 0;

	::llc::drawTriangle(targetPixels.metrics(), triangle, pixelCoords, pixelVertexWeights, depthBuffer);
	int32_t						countPixels				= 0;
	for(uint32_t iPixelCoord = 0; iPixelCoord < pixelCoords.size(); ++iPixelCoord) {
		const ::llc::n2s1_t			pixelCoord				= pixelCoords		[iPixelCoord];
		const ::llc::trif2_t			& vertexWeights			= pixelVertexWeights[iPixelCoord];
		::llc::n3f2_t				normal					= ::llc::triangleWeight(vertexWeights, triangleNormals);
		normal					= matrixTransform.TransformDirection(normal).Normalize();
		double						lightFactor				= normal.Dot(lightVector);
		countPixels += ::llc::setPixel(targetPixels, pixelCoord, color * lightFactor);
	}
	return countPixels;
}

::llc::error_t			llc::drawTriangle
	( const ::llc::g8bgra	targetPixels
	, const ::llc::SGeometryTriangles	& geometry
	, const int							iTriangle
	, const ::llc::m4f2_t				& matrixTransform
	, const ::llc::m4f2_t				& matrixTransformView
	, const ::llc::n3f2_t				& lightVector
	, ::llc::apod<::llc::n2s1_t>			& pixelCoords
	, ::llc::apod<::llc::trif2_t>		& pixelVertexWeights
	, ::llc::gc8bgra	textureImage
	, ::llc::grid<uint32_t>			depthBuffer
	) {
	::llc::tri3f2_t				triangle			= geometry.Triangles		[iTriangle];
	const ::llc::tri3f2_t		& triangleNormals	= geometry.Normals			[iTriangle];
	const ::llc::tri2f2_t		& triangleTexCoords	= geometry.TextureCoords	[iTriangle];
	::llc::transform(triangle, matrixTransformView);
	if( triangle.A.z <= 0 || triangle.A.z >= 1
	 || triangle.B.z <= 0 || triangle.B.z >= 1
	 || triangle.C.z <= 0 || triangle.C.z >= 1
	)
		return 0;

	int32_t						countPixels				= 0;
	::llc::drawTriangle(targetPixels.metrics(), triangle, pixelCoords, pixelVertexWeights, depthBuffer);
	const ::llc::n2f2_t			imageUnit				= {textureImage.metrics().x - 0.000001f, textureImage.metrics().y - 0.000001f};
	for(uint32_t iPixelCoord = 0; iPixelCoord < pixelCoords.size(); ++iPixelCoord) {
		const ::llc::n2s1_t			pixelCoord				= pixelCoords		[iPixelCoord];
		const ::llc::trif2_t			& vertexWeights			= pixelVertexWeights[iPixelCoord];
		::llc::n3f2_t				normal					= ::llc::triangleWeight(vertexWeights, triangleNormals);
		normal					= matrixTransform.TransformDirection(normal).Normalize();

		double						lightFactor				= normal.Dot(lightVector);

		::llc::n2f2_t				texCoord				= triangleTexCoords.A * vertexWeights.A;
		texCoord				+= triangleTexCoords.B * vertexWeights.B;
		texCoord				+= triangleTexCoords.C * vertexWeights.C;
		::llc::bgra					texelColor				= textureImage[(uint32_t)(texCoord.y * imageUnit.y)][(uint32_t)(texCoord.x * imageUnit.x)];
		countPixels += ::llc::setPixel(targetPixels, pixelCoord, (::llc::rgbaf(texelColor) * .3 + ::llc::rgbaf(texelColor) * lightFactor).Clamp());
	}
	return countPixels;
}

::llc::error_t			llc::drawTriangle
	( const ::llc::g8bgra	targetPixels
	, const ::llc::SGeometryTriangles	& geometry
	, const int							iTriangle
	, const ::llc::m4f2_t				& matrixTransform
	, const ::llc::m4f2_t				& matrixTransformView
	, const ::llc::n3f2_t				& lightVector
	, const ::llc::rgbaf				& lightColor
	, ::llc::apod<::llc::n2s1_t>			& pixelCoords
	, ::llc::apod<::llc::trif2_t>		& pixelVertexWeights
	, ::llc::gc8bgra	textureImage
	, ::llc::apod<::llc::SLight3>		& lightPoints
	, ::llc::a8bgra			& lightColors
	, ::llc::grid<uint32_t>			depthBuffer
	) {
	::llc::tri3f2_t				triangleWorld			= geometry.Triangles		[iTriangle];
	::llc::tri3f2_t				triangle				= triangleWorld;
	::llc::transform(triangle, matrixTransformView);
	if( triangle.A.z < 0 || triangle.A.z >= 1
	 || triangle.B.z < 0 || triangle.B.z >= 1
	 || triangle.C.z < 0 || triangle.C.z >= 1
	)
		return 0;

	int32_t						countPixels				= 0;
	::llc::transform(triangleWorld, matrixTransform);
	::llc::drawTriangle(targetPixels.metrics(), triangle, pixelCoords, pixelVertexWeights, depthBuffer);

	const ::llc::tri3f2_t		& triangleNormals		= geometry.Normals			[iTriangle];
	const ::llc::tri2f2_t		& triangleTexCoords		= geometry.TextureCoords	[iTriangle];
	const ::llc::n2f2_t			imageUnit				= {textureImage.metrics().x - 0.000001f, textureImage.metrics().y - 0.000001f};
	for(uint32_t iPixelCoord = 0; iPixelCoord < pixelCoords.size(); ++iPixelCoord) {
		const ::llc::n2s1_t			pixelCoord				= pixelCoords		[iPixelCoord];
		const ::llc::trif2_t			& vertexWeights			= pixelVertexWeights[iPixelCoord];

		::llc::n3f2_t				normal					= ::llc::triangleWeight(vertexWeights, triangleNormals);
		normal					= matrixTransform.TransformDirection(normal).Normalize();
		double						lightFactorDirectional	= normal.Dot(lightVector);
		::llc::n3f2_t				position				= ::llc::triangleWeight(vertexWeights, triangleWorld);
		::llc::n2f2_t				texCoord				= ::llc::triangleWeight(vertexWeights, triangleTexCoords);
		::llc::rgbaf				texelColor				= textureImage[(uint32_t)(texCoord.y * imageUnit.y) % textureImage.metrics().y][(uint32_t)(texCoord.x * imageUnit.x) % textureImage.metrics().x];
		::llc::rgbaf				fragmentColor			= {};

		for(uint32_t iLight = 0; iLight < lightPoints.size(); ++iLight) {
			const ::llc::SLight3		& light					= lightPoints[iLight];

			::llc::n3f2_t				lightToPoint			= light.Position - position;
			::llc::n3f2_t				vectorToLight			= lightToPoint;
			vectorToLight.Normalize();
			double						lightFactorPoint		= vectorToLight.Dot(normal);
			if(lightToPoint.Length() > light.Range || lightFactorPoint <= 0)
				continue;
			double						invAttenuation			= ::std::max(0.0, 1.0 - (lightToPoint.Length() / light.Range));
			fragmentColor			+= ::llc::rgbaf{texelColor * lightColors[iLight] * invAttenuation * .5}.Clamp();
		}
		 (void)lightColor;
		 (void)lightFactorDirectional;

		countPixels				+= ::llc::setPixel(targetPixels, pixelCoord, ((texelColor * .1) + texelColor * lightColor * lightFactorDirectional + fragmentColor).Clamp());
	}
	return countPixels;
}

#endif
