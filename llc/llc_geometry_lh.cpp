#ifndef LLC_ATMEL
#include "llc_geometry_lh.h"

::llc::error_t			llc::geometryBuildTileListFromImage		(::llc::gc8bgra image, ::llc::apod<::llc::STile> & out_tiles, ::llc::u2_t imagePitch)	{
	{
		::llc::STile											newTile;
		for(::llc::u2_t z = 0, maxZ = image.metrics().y; z < maxZ ; ++z)
		for(::llc::u2_t x = 0, maxX = image.metrics().x; x < maxX ; ++x) {
			::llc::rgbaf										currentPixel							= (0 == imagePitch) ? image[z][x] : image.begin()[z * imagePitch + x];
			float													pixelHeight								= float((currentPixel.r + (double)currentPixel.g + currentPixel.b) / 3.0);
			for(::llc::u2_t iCorner = 0; iCorner < 4; ++iCorner)
				newTile.Height[iCorner]								= pixelHeight;
			newTile.Top											= 1;
			newTile.Front										= -1;
			newTile.Right										= -1;
			newTile.Flags										= 0;
			out_tiles.push_back(newTile);
		}
	}
	for(::llc::u2_t z = 0; z < image.metrics().y; ++z)
	for(::llc::u2_t x = 0; x < image.metrics().x - 1; ++x) {
		::llc::STile											& currentTile							= out_tiles[z * image.metrics().y + x];
		::llc::STile											& frontTile								= out_tiles[z * image.metrics().y + x + 1];
		if(currentTile.Height[1] != frontTile.Height[0] || currentTile.Height[3] != frontTile.Height[2])
			currentTile.Front									= 1;
	}
	for(::llc::u2_t z = 0; z < image.metrics().y - 1; ++z)
	for(::llc::u2_t x = 0; x < image.metrics().x; ++x) {
		::llc::STile											& currentTile							= out_tiles[z		* image.metrics().y + x];
		::llc::STile											& rightTile								= out_tiles[(z + 1)	* image.metrics().y + x];
		if(currentTile.Height[2] != rightTile.Height[0] || currentTile.Height[3] != rightTile.Height[1])
			currentTile.Right									= 1;
	}
	return 0;
}

::llc::error_t			llc::geometryBuildGridFromTileList		(::llc::SGeometryQuads & geometry, ::llc::grid<const ::llc::STile> tiles, ::llc::n2f2_t gridCenter, const ::llc::n3f2_t & scale) {
	::llc::n2f2_t									texCoordUnits						= {1.0f / tiles.metrics().x, 1.0f / tiles.metrics().y};
	for(::llc::u2_t z = 0; z < tiles.metrics().y; ++z)
	for(::llc::u2_t x = 0; x < tiles.metrics().x; ++x) {
		::llc::n2f2_t									texcoords	[4]				=
			{ {(x		) * texCoordUnits.x, (z		) * texCoordUnits.y}
			, {(x + 1	) * texCoordUnits.x, (z		) * texCoordUnits.y}
			, {(x		) * texCoordUnits.x, (z + 1	) * texCoordUnits.y}
			, {(x + 1	) * texCoordUnits.x, (z + 1	) * texCoordUnits.y}
			};
		const ::llc::STile										& currentTile							= tiles[z][x];
		{
			geometry.TextureCoords.push_back(
				{ texcoords[0]
				, texcoords[2]
				, texcoords[1]
				});
			geometry.TextureCoords.push_back(
				{ texcoords[1]
				, texcoords[2]
				, texcoords[3]
				});
		}
		{ // top
			::llc::n3f2_t									coords	[4]			=
				{ {x + 0.f, currentTile.Height[0], (z + 0) * -1.f}
				, {x + 0.f, currentTile.Height[1], (z + 1) * -1.f}
				, {x + 1.f, currentTile.Height[2], (z + 0) * -1.f}
				, {x + 1.f, currentTile.Height[3], (z + 1) * -1.f}
				};
			if(coords[0].y || coords[1].y || coords[2].y || coords[3].y) {
				::llc::tri3f2_t								triangleA			= {coords[0], coords[2], coords[1]};//
				::llc::tri3f2_t								triangleB			= {coords[1], coords[2], coords[3]};//
				triangleA.Scale(scale);
				triangleB.Scale(scale);
				triangleA.Translate({-gridCenter.x, 0, -gridCenter.y});
				triangleB.Translate({-gridCenter.x, 0, -gridCenter.y});
				geometry.Triangles	.push_back(triangleA);
				geometry.Triangles	.push_back(triangleB);
				geometry.Normals.push_back((triangleB.A - triangleB.B).Normalize().Cross((triangleA.A - triangleA.B).Normalize()).Normalize().f2());
				//geometry.Normals.push_back((triangleA.A - triangleA.B).Normalize().Cross((triangleB.A - triangleB.B).Normalize()).Normalize().f2());
			}
		}
		if(0 <= currentTile.Front || (x == (tiles.metrics().x - 1) && (0 != currentTile.Height[1] || 0 != currentTile.Height[3]))) {
			::llc::n3f2_t									coords[4];
			if(0 <= currentTile.Front) { // front
				const ::llc::STile										& frontTile			= tiles[z][x + 1];
				coords[0]											= {x + 1.f, currentTile.Height	[1], (z + 0) * -1.f};
				coords[1]											= {x + 1.f, currentTile.Height	[3], (z + 1) * -1.f};
				coords[2]											= {x + 1.f, frontTile.Height	[0], (z + 0) * -1.f};
				coords[3]											= {x + 1.f, frontTile.Height	[2], (z + 1) * -1.f};
			}
			else {
				coords[0]											= {x + 1.f, currentTile.Height	[1], (z + 0) * -1.f};
				coords[1]											= {x + 1.f, currentTile.Height	[3], (z + 1) * -1.f};
				coords[2]											= {x + 1.f, 0, (z + 0) * -1.f};
				coords[3]											= {x + 1.f, 0, (z + 1) * -1.f};
			}
			::llc::tri3f2_t								triangleA			= {coords[0], coords[2], coords[1]};//{coords[0], coords[2], coords[1]};
			::llc::tri3f2_t								triangleB			= {coords[1], coords[2], coords[3]};//{coords[1], coords[2], coords[3]};
			triangleA.Scale(scale);
			triangleB.Scale(scale);
			triangleA.Translate({-gridCenter.x, 0, -gridCenter.y});
			triangleB.Translate({-gridCenter.x, 0, -gridCenter.y});
			geometry.Triangles	.push_back(triangleA);
			geometry.Triangles	.push_back(triangleB);
			//geometry.Normals.push_back((triangleA.A - triangleA.B).Normalize().Cross((triangleB.A - triangleB.B).Normalize()).Normalize().f2());
			geometry.Normals.push_back((triangleB.A - triangleB.B).Normalize().Cross((triangleA.A - triangleA.B).Normalize()).Normalize().f2());
			geometry.TextureCoords.push_back(
				{ texcoords[0]
				, texcoords[2]
				, texcoords[1]
				});
			geometry.TextureCoords.push_back(
				{ texcoords[1]
				, texcoords[2]
				, texcoords[3]
				});
		}
		if(0 <= currentTile.Right || ((z == tiles.metrics().y - 1) && (0 != currentTile.Height[2] || 0 != currentTile.Height[3]))) {
			::llc::n3f2_t									coords[4];
			if(0 <= currentTile.Right) {
				const ::llc::STile										& rightTile			= tiles[z + 1][x];
				coords[0]											= {x + 0.f, currentTile.Height	[2], (z + 1) * -1.f};
				coords[1]											= {x + 1.f, currentTile.Height	[3], (z + 1) * -1.f};
				coords[2]											= {x + 0.f, rightTile.Height	[0], (z + 1) * -1.f};
				coords[3]											= {x + 1.f, rightTile.Height	[1], (z + 1) * -1.f};
			}
			else {
				coords[0]											= {x + 0.f, currentTile.Height	[2], (z + 1) * -1.f};
				coords[1]											= {x + 1.f, currentTile.Height	[3], (z + 1) * -1.f};
				coords[2]											= {x + 0.f, 0, (z + 1) * -1.f};
				coords[3]											= {x + 1.f, 0, (z + 1) * -1.f};
			}
			::llc::tri3f2_t								triangleA			= {coords[0], coords[1], coords[2]};
			::llc::tri3f2_t								triangleB			= {coords[1], coords[3], coords[2]};
			triangleA.Scale(scale);
			triangleB.Scale(scale);
			triangleA.Translate({-gridCenter.x, 0, -gridCenter.y});
			triangleB.Translate({-gridCenter.x, 0, -gridCenter.y});
			geometry.Triangles	.push_back(triangleA);
			geometry.Triangles	.push_back(triangleB);
			geometry.Normals.push_back((triangleA.A - triangleA.B).Normalize().Cross((triangleB.A - triangleB.B).Normalize()).Normalize().f2());
			//geometry.Normals.push_back((triangleB.A - triangleB.B).Normalize().Cross((triangleA.A - triangleA.B).Normalize()).Normalize().f2());
			geometry.TextureCoords.push_back(
				{ texcoords[0]
				, texcoords[2]
				, texcoords[1]
				});
			geometry.TextureCoords.push_back(
				{ texcoords[1]
				, texcoords[2]
				, texcoords[3]
				});
		}
	}
	return 0;
}




//stacxpr	::llc::n3s0_t	geometryIndexedSquarePositions		[]			= {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0}};
//stacxpr	int8_t		geometryIndexedSquareIndices		[]			= {0, 1, 2, 1, 3, 2};
//stacxpr	::llc::n3s0_t	geometryIndexedSquareNormal			[]			= {{0, 0, 1}};
//stacxpr	int8_t		geometryIndexedSquareNormalIndices	[]			= {0};
//stacxpr	::llc::n3s0_t	geometryIndexedSquareUV				[]			= {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0}};
//
//stacxpr	int8_t		geometryIndexedSquareUVIndices		[]			= {0, 1, 2, 1, 3, 2};
//
//// Vertex coordinates for cube faces
//stacxpr	::llc::n3s0_t	geometryIndexedCubePositions		[8]			=
//	{ {0, 0, 0}, {0, 1, 0}, {1, 0, 0}, {1, 1, 0}
//	, {0, 0, 1}, {0, 1, 1}, {1, 0, 1}, {1, 1, 1}
//	};
//
//stacxpr	int8_t		geometryIndexedCubeIndices			[12 * 3]	=
//	{ 0, 2, 1, 1, 2, 3 //
//	, 
//	};

// Vertex coordinates for cube faces
stacxpr	::llc::tri3s0_t	geometryCube	[12]	=
	{ {{0, 0, 0}, {0, 1, 0}, {1, 0, 0}}	// Right	- first
	, {{0, 1, 0}, {1, 1, 0}, {1, 0, 0}}	// Right	- second

	, {{0, 1, 0}, {0, 0, 0}, {0, 0, 1}}	// Back		- first
	, {{0, 1, 1}, {0, 1, 0}, {0, 0, 1}}	// Back		- second

	, {{0, 0, 1}, {0, 0, 0}, {1, 0, 0}}	// Bottom	- first
	, {{1, 0, 1}, {0, 0, 1}, {1, 0, 0}}	// Bottom	- second

	, {{0, 1, 1}, {0, 0, 1}, {1, 0, 1}}	// Left		- first
	, {{1, 1, 1}, {0, 1, 1}, {1, 0, 1}}	// Left		- second

	, {{1, 1, 0}, {1, 1, 1}, {1, 0, 1}}	// Front	- first
	, {{1, 0, 0}, {1, 1, 0}, {1, 0, 1}}	// Front	- second

	, {{0, 1, 1}, {1, 1, 1}, {1, 1, 0}}	// Top		- first
	, {{0, 1, 0}, {0, 1, 1}, {1, 1, 0}}	// Top		- second
	};

// Vertex coordinates for cube faces
stacxpr	::llc::tri2s0_t	texCoordCubeYUp	[12]	=
	{ {{0, 1}, {0, 0}, {1, 1}}	// Right	- first
	, {{0, 0}, {1, 0}, {1, 1}}	// Right	- second
	//{} //
	, {{1, 0}, {1, 1}, {0, 1}}	// Back		- first
	, {{0, 0}, {1, 0}, {0, 1}}	// Back		- second
	//{} //
	, {{0, 0}, {1, 0}, {1, 1}}	// Bottom	- first
	, {{0, 1}, {0, 0}, {1, 1}}	// Bottom	- second
	//{} //
	, {{1, 0}, {1, 1}, {0, 1}}	// Left		- second
	, {{0, 0}, {1, 0}, {0, 1}}	// Left		- first
	//{} //
	, {{0, 0}, {1, 0}, {1, 1}}	// Front	- first
	, {{0, 1}, {0, 0}, {1, 1}}	// Front	- second
	//{} //
	, {{1, 1}, {0, 1}, {0, 0}}	// Top		- first
	, {{1, 0}, {1, 1}, {0, 0}}	// Top		- second
	};

stacxpr	::llc::n3s0_t		geometryNormals	[6]		=
	{ { 0, 0,-1} // Right
	, {-1, 0, 0} // Back
	, { 0,-1, 0} // Bottom
	, { 0, 0, 1} // Left
	, { 1, 0, 0} // Front
	, { 0, 1, 0} // Top
	};

::llc::error_t			llc::geometryBuildCube	(SGeometryQuads & geometry, const ::llc::n3f2_t & scale, const ::llc::n3f2_t & translation)	{
	const ::llc::u2_t											firstTriangle					= geometry.Triangles.size(); 
	llc_necs(llc::geometryBuildCube(geometry, scale));
	for(::llc::u2_t iTri = firstTriangle; iTri < geometry.Triangles.size(); ++iTri) 
		geometry.Triangles[iTri].Translate(translation);
	return 0;
}

::llc::error_t			llc::geometryBuildCube	(SGeometryQuads & geometry, const ::llc::n3f2_t & scale)	{
	const ::llc::u2_t				triangleOffset			= geometry.Triangles.size();
	geometry.Triangles		.resize(triangleOffset + (::llc::u2_t)::llc::size(geometryCube));
	geometry.Normals		.resize(triangleOffset + (::llc::u2_t)::llc::size(geometryNormals));
	geometry.TextureCoords	.resize(triangleOffset + (::llc::u2_t)::llc::size(texCoordCubeYUp));

	for(::llc::u2_t iTriangle = 0, countTriangles = (::llc::u2_t)::llc::size(geometryCube); iTriangle < countTriangles; ++iTriangle) {
		::llc::tri3f2_t				& newTriangle		= geometry.Triangles[triangleOffset + iTriangle];
		newTriangle				= geometryCube[iTriangle].f2();
		newTriangle.A			-= {.5, .5, .5};
		newTriangle.B			-= {.5, .5, .5};
		newTriangle.C			-= {.5, .5, .5};
		newTriangle.Scale(scale);

		::llc::tri2f2_t				& newTriangleTex	= geometry.TextureCoords[triangleOffset + iTriangle];
		newTriangleTex			= ::texCoordCubeYUp[iTriangle].f2();

		::llc::n3f2_t				& newNormal			= geometry.Normals[(triangleOffset + iTriangle) / 2];
		::llc::n3f2_t				normal				= (newTriangle.A - newTriangle.B).Normalize().Cross((newTriangle.A - newTriangle.C).Normalize());
		normal.Normalize();
		newNormal				= normal; //geometryNormals[iTriangle / 2].f2();
	}
	return 0;
}

::llc::error_t			llc::geometryBuildGrid	(SGeometryQuads & geometry, ::llc::n2u1_t gridSize, ::llc::n2f2_t gridCenter, const ::llc::n3f2_t & scale)	{
	::llc::n2f2_t				texCoordUnits			= {1.0f / gridSize.x, 1.0f / gridSize.y};
	for(::llc::u2_t z = 0; z < gridSize.y; ++z)
	for(::llc::u2_t x = 0; x < gridSize.x; ++x)  {
		::llc::n3f2_t				coords	[4]				=
			{ {0, 0, 0}
			, {0, 0, 1}
			, {1, 0, 0}
			, {1, 0, 1}
			};
		::llc::n2f2_t				texcoordsOffset			= {x * texCoordUnits.x, z * texCoordUnits.y};
		::llc::n2f2_t				texcoords	[4]			=
			{ {0.0f * texCoordUnits.x + texcoordsOffset.x, 0.0f * texCoordUnits.y + texcoordsOffset.y}
			, {0.0f * texCoordUnits.x + texcoordsOffset.x, 1.0f * texCoordUnits.y + texcoordsOffset.y}
			, {1.0f * texCoordUnits.x + texcoordsOffset.x, 0.0f * texCoordUnits.y + texcoordsOffset.y}
			, {1.0f * texCoordUnits.x + texcoordsOffset.x, 1.0f * texCoordUnits.y + texcoordsOffset.y}
			};
		::llc::tri2f2_t				triangleATex			= {texcoords[0], texcoords[1], texcoords[2]};
		::llc::tri2f2_t				triangleBTex			= {texcoords[1], texcoords[3], texcoords[2]};
		::llc::tri3f2_t				triangleA				= {coords[0], coords[1], coords[2]};
		::llc::tri3f2_t				triangleB				= {coords[1], coords[3], coords[2]};
		triangleA.A				+= {(float)x, 0, (float)z};
		triangleA.B				+= {(float)x, 0, (float)z};
		triangleA.C				+= {(float)x, 0, (float)z};
		triangleB.A				+= {(float)x, 0, (float)z};
		triangleB.B				+= {(float)x, 0, (float)z};
		triangleB.C				+= {(float)x, 0, (float)z};
		triangleA.Scale(scale);
		triangleB.Scale(scale);

		triangleA.Translate(::llc::n3f2_t{gridCenter.x, 0, gridCenter.y} * -1);
		triangleB.Translate(::llc::n3f2_t{gridCenter.x, 0, gridCenter.y} * -1);
		geometry.Triangles		.push_back(triangleA);
		geometry.Triangles		.push_back(triangleB);
		geometry.TextureCoords	.push_back(triangleATex);
		geometry.TextureCoords	.push_back(triangleBTex);
		::llc::n3f2_t				normal					= (triangleA.A - triangleA.B).Normalize().Cross((triangleB.A - triangleB.B).Normalize());
		normal.Normalize();
		geometry.Normals		.push_back(normal);
	}
	return 0;
}

//
::llc::error_t			llc::geometryBuildFigure0	(::llc::SGeometryQuads & geometry, ::llc::u2_t stacks, ::llc::u2_t slices, float radius, const ::llc::n3f2_t & gridCenter)	{
	(void)radius;
	for(::llc::u2_t z = 0; z < stacks; ++z)
	for(::llc::u2_t x = 0; x < slices; ++x)  {
		::llc::n2f2_t				texcoords	[4]			=
			{ {0, 0}
			, {0, 1}
			, {1, 0}
			, {1, 1}
			};
		::llc::n3f3_t				coords	[4]				=
			{ {sin(::llc::math_pi * z		/ stacks	) * cos(::llc::math_2pi * x			/ slices), sin(::llc::math_pi * z		/ stacks) * sin(::llc::math_2pi * x			/ slices), cos(::llc::math_pi * x		/slices)}
			, {sin(::llc::math_pi * z		/ stacks	) * cos(::llc::math_2pi * (x + 1)	/ slices), sin(::llc::math_pi * z		/ stacks) * sin(::llc::math_2pi * (x + 1)	/ slices), cos(::llc::math_pi * (x + 1) /slices)}
			, {sin(::llc::math_pi * (z + 1)	/ stacks	) * cos(::llc::math_2pi * x			/ slices), sin(::llc::math_pi * (z + 1)	/ stacks) * sin(::llc::math_2pi * x			/ slices), cos(::llc::math_pi * x		/slices)}
			, {sin(::llc::math_pi * (z + 1)	/ stacks	) * cos(::llc::math_2pi * (x + 1)	/ slices), sin(::llc::math_pi * (z + 1)	/ stacks) * sin(::llc::math_2pi * (x + 1)	/ slices), cos(::llc::math_pi * (x + 1)	/slices)}
			};
		::llc::tri3f2_t				triangleA				= {coords[0].f2() * radius, coords[1].f2() * radius, coords[2].f2() * radius};
		::llc::tri3f2_t				triangleB				= {coords[1].f2() * radius, coords[3].f2() * radius, coords[2].f2() * radius};
		::llc::tri2f2_t				triangleATex			= {texcoords[0], texcoords[1], texcoords[2]};
		::llc::tri2f2_t				triangleBTex			= {texcoords[1], texcoords[3], texcoords[2]};
		triangleA.Translate(gridCenter * -1);
		triangleB.Translate(gridCenter * -1);
		geometry.Triangles		.push_back(triangleA);
		geometry.Triangles		.push_back(triangleB);
		geometry.Normals		.push_back((triangleA.A - triangleA.B).Normalize().Cross((triangleB.A - triangleB.B).Normalize()).Normalize().f2());
		//geometry.Normals		.push_back((coords[0] - coords[1]).Normalize().Cross((coords[1] - coords[3]).Normalize()).Normalize().f2());
		geometry.TextureCoords	.push_back(triangleATex);
		geometry.TextureCoords	.push_back(triangleBTex);
	}
	return 0;
}
//
//::llc::error_t			llc::geometryBuildFigure1	(::llc::SGeometryQuads & geometry, ::llc::u2_t stacks, ::llc::u2_t slices, float radius, const ::llc::n3f2_t & gridCenter)	{
//	(void)radius;
//	for(::llc::u2_t z = 0; z < stacks; ++z)
//	for(::llc::u2_t x = 0; x < slices; ++x)  {
//		::llc::n2f2_t									texcoords	[4]			=
//			{ {0, 0}
//			, {0, 1}
//			, {1, 0}
//			, {1, 1}
//			};
//		::llc::n3f3_t									coords	[4]				=
//			{ {sin(::llc::math_pi * z		/ stacks) * sin(::llc::math_2pi * x			/ slices), sin(::llc::math_pi * z		/ stacks	) * cos(::llc::math_2pi * x			/ slices), cos(::llc::math_pi * x		/slices)}
//			, {sin(::llc::math_pi * z		/ stacks) * sin(::llc::math_2pi * (x + 1)	/ slices), sin(::llc::math_pi * z		/ stacks	) * cos(::llc::math_2pi * (x + 1)	/ slices), cos(::llc::math_pi * (x + 1) /slices)}
//			, {sin(::llc::math_pi * (z + 1)	/ stacks) * sin(::llc::math_2pi * x			/ slices), sin(::llc::math_pi * (z + 1)	/ stacks	) * cos(::llc::math_2pi * x			/ slices), cos(::llc::math_pi * x		/slices)}
//			, {sin(::llc::math_pi * (z + 1)	/ stacks) * sin(::llc::math_2pi * (x + 1)	/ slices), sin(::llc::math_pi * (z + 1)	/ stacks	) * cos(::llc::math_2pi * (x + 1)	/ slices), cos(::llc::math_pi * (x + 1)	/slices)}
//			};
//		::llc::tri3f2_t								triangleA				= {coords[0].f2() * radius, coords[1].f2() * radius, coords[2].f2() * radius};
//		::llc::tri3f2_t								triangleB				= {coords[1].f2() * radius, coords[3].f2() * radius, coords[2].f2() * radius};
//		::llc::tri2f2_t								triangleATex			= {texcoords[0], texcoords[1], texcoords[2]};
//		::llc::tri2f2_t								triangleBTex			= {texcoords[1], texcoords[3], texcoords[2]};
//		triangleA.Translate(gridCenter * -1);
//		triangleB.Translate(gridCenter * -1);
//		geometry.Triangles		.push_back(triangleA);
//		geometry.Triangles		.push_back(triangleB);
//		geometry.Normals		.push_back((triangleA.A - triangleA.B).Normalize().Cross((triangleB.A - triangleB.B).Normalize()).Normalize().f2());
//		//geometry.Normals		.push_back((coords[0] - coords[1]).Normalize().Cross((coords[1] - coords[3]).Normalize()).Normalize().f2());
//		geometry.TextureCoords	.push_back(triangleATex);
//		geometry.TextureCoords	.push_back(triangleBTex);
//	}
//	return 0;
//}

::llc::error_t			llc::geometryBuildHelixHalf	(SGeometryQuads & geometry, ::llc::u2_t stacks, ::llc::u2_t slices, float radius, const ::llc::n3f2_t & gridCenter, const ::llc::n3f2_t & scale)	{
	::llc::n2f2_t									texCoordUnits				= {1.0f / slices, 1.0f / stacks};
	for(::llc::u2_t z = 0; z < stacks; ++z)
	for(::llc::u2_t x = 0; x < slices; ++x)  {
		::llc::n2f2_t									texcoords	[4]			=
			{ {(x		) * texCoordUnits.x, (z		) * texCoordUnits.y}
			, {(x		) * texCoordUnits.x, (z + 1	) * texCoordUnits.y}
			, {(x + 1	) * texCoordUnits.x, (z		) * texCoordUnits.y}
			, {(x + 1	) * texCoordUnits.x, (z + 1	) * texCoordUnits.y}
			};
		::llc::n3f3_t									coords	[4]				=
			{ {sin(::llc::math_pi * z		/ stacks) * sin(::llc::math_2pi * x			/ slices), sin(::llc::math_pi * z		/ stacks	) * cos(::llc::math_pi * x			/slices), cos(::llc::math_2pi * x		/ slices)}
			, {sin(::llc::math_pi * z		/ stacks) * sin(::llc::math_2pi * (x + 1)	/ slices), sin(::llc::math_pi * z		/ stacks	) * cos(::llc::math_pi * (x + 1)	/slices), cos(::llc::math_2pi * (x + 1)	/ slices)}
			, {sin(::llc::math_pi * (z + 1)	/ stacks) * sin(::llc::math_2pi * x			/ slices), sin(::llc::math_pi * (z + 1)	/ stacks	) * cos(::llc::math_pi * x			/slices), cos(::llc::math_2pi * x		/ slices)}
			, {sin(::llc::math_pi * (z + 1)	/ stacks) * sin(::llc::math_2pi * (x + 1)	/ slices), sin(::llc::math_pi * (z + 1)	/ stacks	) * cos(::llc::math_pi * (x + 1)	/slices), cos(::llc::math_2pi * (x + 1)	/ slices)}
			};
		::llc::tri3f2_t								triangleA				= {coords[0].f2() * radius, coords[1].f2() * radius, coords[2].f2() * radius};
		::llc::tri3f2_t								triangleB				= {coords[1].f2() * radius, coords[3].f2() * radius, coords[2].f2() * radius};
		::llc::tri2f2_t								triangleATex			= {texcoords[0], texcoords[1], texcoords[2]};
		::llc::tri2f2_t								triangleBTex			= {texcoords[1], texcoords[3], texcoords[2]};
		triangleA.Scale(scale);
		triangleB.Scale(scale);
		triangleA.Translate(gridCenter * -1);
		triangleB.Translate(gridCenter * -1);
		geometry.Triangles		.push_back(triangleA);
		geometry.Triangles		.push_back(triangleB);
		geometry.Normals		.push_back((triangleA.A - triangleA.B).Normalize().Cross((triangleB.A - triangleB.B).Normalize()).Normalize().f2());
		geometry.TextureCoords	.push_back(triangleATex);																					  
		geometry.TextureCoords	.push_back(triangleBTex);
	}
	return 0;
}

::llc::error_t			llc::geometryBuildHelix		(SGeometryQuads & geometry, ::llc::u2_t stacks, ::llc::u2_t slices, float radius, const ::llc::n3f2_t & gridCenter, const ::llc::n3f2_t & scale)	{
	::llc::n2f2_t									texCoordUnits				= {1.0f / slices, 1.0f / stacks};
	for(::llc::u2_t z = 0; z < stacks; ++z)
	for(::llc::u2_t x = 0; x < slices; ++x)  {
		::llc::n2f2_t									texcoords	[4]			=
			{ {(x		) * texCoordUnits.x, (z		) * texCoordUnits.y}
			, {(x		) * texCoordUnits.x, (z + 1	) * texCoordUnits.y}
			, {(x + 1	) * texCoordUnits.x, (z		) * texCoordUnits.y}
			, {(x + 1	) * texCoordUnits.x, (z + 1	) * texCoordUnits.y}
			};
		::llc::n3f3_t									coords	[4]				=
			{ {sin(::llc::math_pi * z		/ stacks) * sin(::llc::math_2pi * x			/ slices), sin(::llc::math_pi * z		/ stacks	) * cos(::llc::math_pi * x			/slices), cos(::llc::math_2pi * x		/ slices)}
			, {sin(::llc::math_pi * z		/ stacks) * sin(::llc::math_2pi * (x + 1)	/ slices), sin(::llc::math_pi * z		/ stacks	) * cos(::llc::math_pi * (x + 1)	/slices), cos(::llc::math_2pi * (x + 1)	/ slices)}
			, {sin(::llc::math_pi * (z + 1)	/ stacks) * sin(::llc::math_2pi * x			/ slices), sin(::llc::math_pi * (z + 1)	/ stacks	) * cos(::llc::math_pi * x			/slices), cos(::llc::math_2pi * x		/ slices)}
			, {sin(::llc::math_pi * (z + 1)	/ stacks) * sin(::llc::math_2pi * (x + 1)	/ slices), sin(::llc::math_pi * (z + 1)	/ stacks	) * cos(::llc::math_pi * (x + 1)	/slices), cos(::llc::math_2pi * (x + 1)	/ slices)}
			};
		::llc::tri3f2_t								triangleA				= {coords[0].f2() * radius, coords[1].f2() * radius, coords[2].f2() * radius};
		::llc::tri3f2_t								triangleB				= {coords[1].f2() * radius, coords[3].f2() * radius, coords[2].f2() * radius};
		::llc::tri2f2_t								triangleATex			= {texcoords[0], texcoords[1], texcoords[2]};
		::llc::tri2f2_t								triangleBTex			= {texcoords[1], texcoords[3], texcoords[2]};
		::llc::tri3f2_t								triangleC				= triangleA;
		::llc::tri3f2_t								triangleD				= triangleB;
		triangleA.Translate({0, 0, -radius});
		triangleB.Translate({0, 0, -radius});
		triangleC.Translate({0, 0, -radius});
		triangleD.Translate({0, 0, -radius});
		triangleC.Scale({-1, 1, -1});
		triangleD.Scale({-1, 1, -1});

		triangleA.Scale(scale);
		triangleB.Scale(scale);
		triangleC.Scale(scale);
		triangleD.Scale(scale);

		triangleA.Translate(gridCenter * -1);
		triangleB.Translate(gridCenter * -1);
		triangleC.Translate(gridCenter * -1);
		triangleD.Translate(gridCenter * -1);
		::llc::n3f2_t									normal					= (triangleA.A - triangleA.B).Normalize().Cross((triangleB.A - triangleB.B).Normalize()).Normalize().f2();
		geometry.Triangles		.push_back(triangleA);
		geometry.Triangles		.push_back(triangleB);
		geometry.Triangles		.push_back(triangleC);
		geometry.Triangles		.push_back(triangleD);
		geometry.Normals		.push_back(normal);
		normal.Scale({-1, -1, 0});
		geometry.Normals		.push_back(normal);
		geometry.TextureCoords	.push_back(triangleATex);
		geometry.TextureCoords	.push_back(triangleBTex);
		geometry.TextureCoords	.push_back(triangleATex);
		geometry.TextureCoords	.push_back(triangleBTex);
	}
	return 0;
}

//
::llc::error_t			llc::geometryBuildSphere	(SGeometryQuads & geometry, ::llc::u2_t stacks, ::llc::u2_t slices, float radius, const ::llc::n3f2_t & gridCenter)	{
	::llc::n2f2_t									texCoordUnits				= {1.0f / slices, 1.0f / stacks};
	for(::llc::u2_t z = 0; z < stacks; ++z)
	for(::llc::u2_t x = 0; x < slices; ++x)  {
		{
			::llc::n2f2_t									texcoords	[4]				=
				{ {(z		) * texCoordUnits.y, (x		) * texCoordUnits.x}
				, {(z		) * texCoordUnits.y, (x + 1	) * texCoordUnits.x}
				, {(z + 1	) * texCoordUnits.y, (x		) * texCoordUnits.x}
				, {(z + 1	) * texCoordUnits.y, (x + 1	) * texCoordUnits.x}
				};
			geometry.TextureCoords.push_back(
				{ texcoords[0]
				, texcoords[2]
				, texcoords[1]
				});
			geometry.TextureCoords.push_back(
				{ texcoords[1]
				, texcoords[2]
				, texcoords[3]
				});
		}
		::llc::n3f3_t									coords	[4]				=
			{ {sin(::llc::math_pi * x		/slices) * cos(::llc::math_2pi * z			/ stacks), cos(::llc::math_pi * x		/slices), sin(::llc::math_pi * x		/ slices) * sin(::llc::math_2pi * z			/ stacks)}
			, {sin(::llc::math_pi * (x + 1)	/slices) * cos(::llc::math_2pi * z			/ stacks), cos(::llc::math_pi * (x + 1)	/slices), sin(::llc::math_pi * (x + 1)	/ slices) * sin(::llc::math_2pi * z			/ stacks)}
			, {sin(::llc::math_pi * x		/slices) * cos(::llc::math_2pi * (z + 1)	/ stacks), cos(::llc::math_pi * x		/slices), sin(::llc::math_pi * x		/ slices) * sin(::llc::math_2pi * (z + 1)	/ stacks)}
			, {sin(::llc::math_pi * (x + 1)	/slices) * cos(::llc::math_2pi * (z + 1)	/ stacks), cos(::llc::math_pi * (x + 1)	/slices), sin(::llc::math_pi * (x + 1)	/ slices) * sin(::llc::math_2pi * (z + 1)	/ stacks)}
			};
		{
			::llc::tri3f2_t								triangleA			= {coords[0].f2() * radius, coords[2].f2() * radius, coords[1].f2() * radius};
			::llc::tri3f2_t								triangleB			= {coords[1].f2() * radius, coords[2].f2() * radius, coords[3].f2() * radius};
			triangleA.Translate(gridCenter * -1);
			triangleB.Translate(gridCenter * -1);
			geometry.Triangles	.push_back(triangleA);
			geometry.Triangles	.push_back(triangleB);
		}
		{
			::llc::n3f2_t									normal				= coords[0].f2();
			normal												+= coords[1].f2();		
			normal												+= coords[2].f2();
			normal												+= coords[3].f2();
			normal												/= 4;
			normal.Normalize();
			geometry.Normals.push_back(normal);
		}
	}
	return 0;
}

//
::llc::error_t			llc::geometryBuildCylinder	(SGeometryQuads & geometry, ::llc::u2_t stacks, ::llc::u2_t slices, float radiusYMin, float radiusYMax, const ::llc::n3f2_t & gridCenter, const ::llc::n3f2_t & scale)	{
	::llc::n2f2_t				texCoordUnits				= {1.0f / slices, 1.0f / stacks};
	const double				radiusUnit					= 1.0 / stacks;
	for(::llc::u2_t y = 0; y < stacks; ++y) {
		double						radius						= ::llc::interpolate_linear(radiusYMin, radiusYMax, radiusUnit * y);
		double						radiusNext					= ::llc::interpolate_linear(radiusYMin, radiusYMax, radiusUnit * (y + 1));
		for(::llc::u2_t z = 0; z < slices; ++z) {
			{
				::llc::n2f2_t				texcoords	[4]				=
					{ {(z		) * texCoordUnits.x, (y		) * texCoordUnits.y}
					, {(z + 1	) * texCoordUnits.x, (y		) * texCoordUnits.y}
					, {(z		) * texCoordUnits.x, (y	+ 1	) * texCoordUnits.y}
					, {(z + 1	) * texCoordUnits.x, (y + 1	) * texCoordUnits.y}
					};
				geometry.TextureCoords.push_back(
					{ texcoords[0]
					, texcoords[2]
					, texcoords[1]
					});
				geometry.TextureCoords.push_back(
					{ texcoords[1]
					, texcoords[2]
					, texcoords[3]
					});
			}
			::llc::n3f3_t				coords	[4]					=
				{ {1 * radius, (double)(y		)}
				, {1 * radius, (double)(y		)}
				, {1 * radiusNext, (double)(y + 1	)}
				, {1 * radiusNext, (double)(y + 1	)}
				};
			coords[0].RotateY(::llc::math_2pi / slices * (z + 0));
			coords[1].RotateY(::llc::math_2pi / slices * (z + 1));
			coords[2].RotateY(::llc::math_2pi / slices * (z + 0));
			coords[3].RotateY(::llc::math_2pi / slices * (z + 1));

			{
				::llc::tri3f2_t				triangleA					= {coords[0].f2(), coords[2].f2(), coords[1].f2()};
				::llc::tri3f2_t				triangleB					= {coords[1].f2(), coords[2].f2(), coords[3].f2()};
				triangleA.Scale(scale);
				triangleB.Scale(scale);
				triangleA.Translate(gridCenter * -1);
				triangleB.Translate(gridCenter * -1);

				geometry.Triangles	.push_back(triangleA);
				geometry.Triangles	.push_back(triangleB);
				geometry.Normals	.push_back((triangleB.A - triangleB.B).Normalize().Cross((triangleA.A - triangleA.B).Normalize()).Normalize().f2());
			}
		}
	}
	return 0;
}


::llc::error_t			llc::geometryBuildTender	(SGeometryQuads & geometry, ::llc::u2_t stacks, ::llc::u2_t slices, float radius, const ::llc::n3f2_t & gridCenter, const ::llc::n3f2_t & scale)	{
	::llc::n2f2_t				texCoordUnits				= {1.0f / slices, 1.0f / stacks};
	for(::llc::u2_t z = 0; z < stacks; ++z)
	for(::llc::u2_t x = 0; x < slices; ++x)  {
		{
			::llc::n2f2_t				texcoords	[4]				=
				{ {(z		) * texCoordUnits.y, (x		) * texCoordUnits.x}
				, {(z		) * texCoordUnits.y, (x + 1	) * texCoordUnits.x}
				, {(z + 1	) * texCoordUnits.y, (x		) * texCoordUnits.x}
				, {(z + 1	) * texCoordUnits.y, (x + 1	) * texCoordUnits.x}
				};
			geometry.TextureCoords.push_back(
				{ texcoords[0]
				, texcoords[2]
				, texcoords[1]
				});
			geometry.TextureCoords.push_back(
				{ texcoords[1]
				, texcoords[2]
				, texcoords[3]
				});
		}
		//const ::llc::SSinCos		pairSinCos				= ::llc::getSinCos(::llc::math_2pi * z / slices);
		//const double					px						= x * pairSinCos.Cos - z * pairSinCos.Sin;
		//z							= (x * pairSinCos.Sin + z * pairSinCos.Cos);
		//x							= px;
		(void)radius;
		::llc::n3f3_t				coords	[4]					=
			{ {1 * radius, (double)(z		)}
			, {1 * radius, (double)(z		)}
			, {1 * radius, (double)(z + 1	)}
			, {1 * radius, (double)(z + 1	)}
			};
		coords[0].RotateY(::llc::math_2pi / slices * (x + 0));
		coords[1].RotateY(::llc::math_2pi / slices * (x + 1));
		coords[2].RotateY(::llc::math_2pi / slices * (x + 0));
		coords[3].RotateY(::llc::math_2pi / slices * (x + 1));
		coords[0].Normalize();
		coords[1].Normalize();
		coords[2].Normalize();
		coords[3].Normalize();

		{
			::llc::tri3f2_t				triangleA					= {coords[0].f2(), coords[2].f2(), coords[1].f2()};
			::llc::tri3f2_t				triangleB					= {coords[1].f2(), coords[2].f2(), coords[3].f2()};
			triangleA.Scale(scale);
			triangleB.Scale(scale);
			triangleA.Translate(gridCenter * -1);
			triangleB.Translate(gridCenter * -1);
			geometry.Triangles	.push_back(triangleA);
			geometry.Triangles	.push_back(triangleB);
			geometry.Normals	.push_back((triangleA.A - triangleA.B).Normalize().Cross((triangleB.A - triangleB.B).Normalize()).Normalize().f2());
		}																																	  
	}
	return 0;
}

#endif