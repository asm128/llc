#ifndef LLC_ATMEL

#include "llc_geometry.h"
#include "llc_voxel.h"
#include "llc_view_n3.h"
#include "llc_sphere.h"

::llc::error_t			llc::geometryBuildBox		(::llc::SGeometryBuffers & geometry, const ::llc::SParamsBox & params) {
	const ::llc::u2_t				offsetPositions				= geometry.Positions		.size(); geometry.Positions			.resize(offsetPositions			+ 24);
	const ::llc::u2_t				offsetNormals				= geometry.Normals			.size(); geometry.Normals			.resize(offsetNormals			+ 24);
	const ::llc::u2_t				offsetTextureCoords			= geometry.TextureCoords	.size(); geometry.TextureCoords		.resize(offsetTextureCoords		+ 24);
	const ::llc::u2_t				offsetPositionIndices		= geometry.PositionIndices	.size(); geometry.PositionIndices	.resize(offsetPositionIndices	+ 36);
	const ::llc::vcn3f2_t			vertices					= {&::llc::VOXEL_FACE_VERTICES[0].A, 24};
	for(::llc::u2_t iVertex = offsetPositions; iVertex < geometry.Positions.size(); ++iVertex)
		geometry.Positions[iVertex]	= params.Orientation.RotateVector(vertices[iVertex - offsetPositions].Scaled(params.HalfSizes * 2.0f) - params.Origin);

	memcpy(&geometry.Normals		[offsetNormals		], ::llc::VOXEL_FACE_NORMALS.begin(), ::llc::VOXEL_FACE_NORMALS	.byte_count());
	for(::llc::u2_t iVertex = offsetNormals; iVertex < geometry.Normals.size(); ++iVertex)
		geometry.Normals[iVertex]	= params.Orientation.RotateVector(geometry.Normals[iVertex]);
	memcpy(&geometry.TextureCoords	[offsetTextureCoords], ::llc::VOXEL_FACE_UV.begin()		, ::llc::VOXEL_FACE_UV		.byte_count());
	const ::llc::vcu1_t			indices						= {::llc::VOXEL_FACE_INDICES_16[0], 36};
	for(::llc::u2_t iIndex = offsetPositionIndices; iIndex < geometry.PositionIndices.size(); ++iIndex) 
		geometry.PositionIndices[iIndex] = offsetPositions + indices[iIndex - offsetPositionIndices];

	return 0;
}

enum SQUARE_MODE 
	{ SQUARE_MODE_TOP_LEFT
	, SQUARE_MODE_TOP_RIGHT
	};

tplt<tpnm _tIndex>
static	::llc::error_t	geometryBuildGridIndices	(::llc::apod<_tIndex> & positionIndices, ::llc::u2_t vertexOffset, const ::llc::n2u1_t cellCount, bool counterClockwise, bool outward) {
	const ::llc::u1_t				pitch						= cellCount.x + 1;								// 2 3 
	const ::llc::astu2_t<4>		indices						= counterClockwise ? ::llc::astu2_t<4>{{1U, 0U, ::llc::u2_t(pitch + 1U), pitch}} : ::llc::astu2_t<4>{{0U, 1U, pitch, ::llc::u2_t(pitch + 1U)}};	// 0 1 2 3
	const ::llc::u2_t				indices_modes[2][6]			=
		{ {indices[0], indices[2], indices[3], indices[0], indices[3], indices[1]}
		, {indices[0], indices[2], indices[1], indices[1], indices[2], indices[3]}
		};
	const ::llc::n2u1_t			halfCount					= {::llc::u1_t(cellCount.x >> 1U), ::llc::u1_t(cellCount.y >> 1U)};
	cellCount.enumerate([&positionIndices, halfCount, indices_modes, vertexOffset, outward](::llc::u2_t & vertexIndex, ::llc::n2u1_t & coord) {
		const bool					reverseQuad 
			= ( (coord.x >= halfCount.x && coord.y <  halfCount.y)
			 || (coord.x <  halfCount.x && coord.y >= halfCount.y)
			);
		const ::llc::vcu2_t			relative_indices			= indices_modes[((reverseQuad ? SQUARE_MODE_TOP_RIGHT : SQUARE_MODE_TOP_LEFT) + one_if(outward)) % 2];
		for(::llc::u2_t i = 0; i < 6; ++i) {
			positionIndices.push_back(vertexOffset + vertexIndex + relative_indices[i]);
		}
	}, [](::llc::u2_t & vertexIndex, ::llc::n2u1_t & ) { 
		++vertexIndex; 
	});
	return 0;
}
::llc::error_t			llc::geometryBuildGrid		(::llc::SGeometryBuffers & geometry, const ::llc::SParamsGrid & params) {
	const ::llc::u2_t				vertexOffset				= geometry.Positions.size();
	const ::llc::n2u1_t			vertexCount					= params.CellCount + ::llc::n2u1_t{1, 1};

	// -- Generate texture coordinates
	const ::llc::n2f3_t			cellUnits					= {1.0f / params.CellCount.x, 1.0f / params.CellCount.y};
	vertexCount.for_each([&geometry, &params, cellUnits](::llc::n2u1_t & coord) { geometry.TextureCoords.push_back(::llc::n2u2_t{coord.x, (::llc::u2_t)params.CellCount.y - coord.y}.f2().InPlaceScale(cellUnits).f2()); });

	// -- Generate normals
	geometry.Normals.resize(geometry.Normals.size() + vertexCount.Area(), params.Orientation.RotateVector({0, params.Reverse ? -1.0f : 1.0f, 0}));	// The normals are all the same for grids

	// -- Generate positions
	const ::llc::n2f3_t			scale						= params.Size.f3().InPlaceScale(cellUnits);
	vertexCount.for_each([&geometry, scale, params](::llc::n2u1_t & coord) {
		const ::llc::n3f3_t			position					= {coord.x * scale.x, 0, coord.y * scale.y};
		geometry.Positions.push_back(params.Orientation.RotateVector(position.f2() - params.Origin));
	});
	return ::geometryBuildGridIndices(geometry.PositionIndices, vertexOffset, params.CellCount, params.Reverse, params.Outward); 
}
::llc::error_t			llc::geometryBuildSphere	(::llc::SGeometryBuffers & geometry, const ::llc::SParamsSphere & params) {
	const ::llc::u2_t				 vertexOffset				= geometry.Positions.size();
	const ::llc::n2u1_t			vertexCount					= params.CellCount + ::llc::n2u1_t{1, 1};

	// -- Generate texture coordinates
	const ::llc::n2f3_t			cellUnits					= {1.0f / params.CellCount.x, 1.0f / params.CellCount.y};
	vertexCount.for_each([&geometry, &params, cellUnits](::llc::n2u1_t & coord) { geometry.TextureCoords.push_back(::llc::n2u2_t{coord.x, (::llc::u2_t)params.CellCount.y - coord.y}.f3().InPlaceScale(cellUnits).f2()); });

	// -- Generate normals and positions
	const double				reverseScale				= params.Reverse ? -1.0 : 1.0;
	const ::llc::n2f3_t			sliceScale					= ::llc::n2f3_t{::llc::math_2pi * cellUnits.x * reverseScale * params.Circumference, ::llc::math_pi * cellUnits.y};
	const double				sliceOffset					= 0; //::llc::math_pi * (1.0 - params.Circumference) * .5;
	for(::llc::u2_t y = 0; y < vertexCount.y; ++y) {
		const double				currentY					= sliceScale.y * y;
		const double				currentRadius				= sin(currentY);
		for(::llc::u2_t x = 0; x < vertexCount.x; ++x) {
			const double				currentX					= sliceScale.x * x + sliceOffset;
			const ::llc::n3f3_t			coord 						= ::llc::n3f3_t{currentRadius * cos(currentX), -cos(currentY), currentRadius * sin(currentX)};
			llc_necs(geometry.Normals	.push_back(params.Orientation.RotateVector((coord * reverseScale).f2())));
			llc_necs(geometry.Positions	.push_back(params.Orientation.RotateVector((coord * params.Radius).f2() - params.Origin)));
		}
	}
	return ::geometryBuildGridIndices(geometry.PositionIndices, vertexOffset, params.CellCount, false, false); 
}
::llc::error_t			llc::geometryBuildCircleSide(::llc::SGeometryBuffers & geometry, const ::llc::SParamsCircle & params) {
	const ::llc::u2_t				vertexOffset				= geometry.Positions.size();
	const ::llc::u2_t				vertexCount					= (::llc::u2_t)params.Slices + 1;
	const double				cellUnits					= 1.0 / params.Slices;
	const double				reverseScale				= params.Reverse ? -1.0 : 1.0;
	const double				sliceScale					= cellUnits * ::llc::math_2pi * params.Circumference * reverseScale;
	const double				sliceOffset					= 0; //::llc::math_2pi * (1.0 - params.Circumference);

	// -- Generate positions and texture coordinates
	llc_necs(geometry.Positions		.push_back(params.Orientation.RotateVector(-params.Origin)));	// The first vertex is the center of the circle
	llc_necs(geometry.TextureCoords	.push_back(::llc::n2f2_t{.5f, .5f}));					// The first vertex is the center of the circle
	for(::llc::u2_t iVertex = 0; iVertex < vertexCount; ++iVertex) {
		double						angle						= sliceScale * iVertex + sliceOffset;
		llc_necs(geometry.Positions		.push_back(params.Orientation.RotateVector(::llc::n3f3_t{params.Radius}.RotateY(angle).f2() - params.Origin)));
		llc_necs(geometry.TextureCoords	.push_back(::llc::n2f3_t{.5}.Rotate(angle).f2() + ::llc::n2f2_t{.5f, .5f}));
	}

	// -- Generate normals
	llc_necs(geometry.Normals.resize(geometry.Normals.size() + vertexCount + 1, params.Orientation.RotateVector({0, 1, 0})));	// The normals are all the same for a circle

	for(::llc::u2_t iSlice = 0, sliceCount = params.Slices + 1; iSlice < sliceCount; ++iSlice)
		llc_necs(geometry.PositionIndices.append({vertexOffset + 0, vertexOffset + iSlice + 1, vertexOffset + iSlice + 0}));

	return 0; 
}
::llc::error_t			llc::geometryBuildCylinderWall	(::llc::SGeometryBuffers & geometry, const ::llc::SParamsCylinderWall & params) {
	const ::llc::u2_t				vertexOffset				= geometry.Positions.size();
	const ::llc::n2u1_t			vertexCount					= params.CellCount + ::llc::n2u1_t{1, 1};

	// -- Generate texture coordinates
	const ::llc::n2f3_t			cellUnits					= {1.0f / params.CellCount.x, 1.0f / params.CellCount.y};
	vertexCount.for_each([&geometry, &params, cellUnits](::llc::n2u1_t & coord) { geometry.TextureCoords.push_back(::llc::n2u2_t{coord.x, (::llc::u2_t)params.CellCount.y - coord.y}.f3().InPlaceScale(cellUnits).f2()); });

	// -- Generate normals
	const double				reverseScale				= params.Reverse ? -1.0 : 1.0;
	const double				lengthScale					= cellUnits.y * params.Height;
	const ::llc::n3f3_t			normalBase					= llc::n3f3_t{params.Height, -(params.Radius.Max - params.Radius.Min)}.Normalize() * reverseScale;
	const double				sliceScale					= cellUnits.x * ::llc::math_2pi * params.Circumference * reverseScale;
	const double				sliceOffset					= 0; //::llc::math_pi * (1.0 - params.Circumference) * .5;
	vertexCount.for_each([&geometry, &normalBase, sliceScale, sliceOffset, &params](::llc::n2u1_t & coord) { geometry.Normals.push_back(params.Orientation.RotateVector((::llc::n3f3_t{normalBase}.RotateY(sliceScale * coord.x + sliceOffset)).f2())); });

	// -- Generate positions
	for(::llc::u2_t y = 0; y < vertexCount.y; ++y) {
		const double				radius						= ::llc::interpolate_linear(params.Radius.Min, params.Radius.Max, cellUnits.y * y);
		for(::llc::u2_t x = 0; x < vertexCount.x; ++x)
			geometry.Positions.push_back(params.Orientation.RotateVector(::llc::n3f3_t{radius, (double)y * lengthScale}.RotateY(sliceScale * x + sliceOffset).f2() - params.Origin));
	}
	return ::geometryBuildGridIndices(geometry.PositionIndices, vertexOffset, params.CellCount, false, false); 
}
//
::llc::error_t			llc::geometryBuildFigure0	(::llc::SGeometryBuffers & geometry, const ::llc::SParamsHelix & params) { //(::llc::SGeometryQuads & geometry, ::llc::u2_t stacks, ::llc::u2_t slices, float radius, const ::llc::n3f2_t & gridCenter)	{
	const ::llc::u2_t				vertexOffset				= geometry.Positions.size();
	const ::llc::n2u1_t			vertexCount					= params.CellCount + ::llc::n2u1_t{1, 1};

	// -- Generate texture coordinates
	const ::llc::n2f3_t			cellUnits					= {1.0f / params.CellCount.x, 1.0f / params.CellCount.y};
	geometry.TextureCoords	.reserve(vertexCount.Area());
	geometry.Positions		.reserve(vertexCount.Area());
	geometry.Normals		.reserve(vertexCount.Area());
	vertexCount.for_each([&geometry, &params, cellUnits](::llc::n2u1_t & coord) { geometry.TextureCoords.push_back(::llc::n2u2_t{coord.x, (::llc::u2_t)params.CellCount.y - coord.y}.f2().InPlaceScale(cellUnits).f2()); });

	//const double				lengthScale					= cellUnits.y * params.Length;
	const ::llc::n2f3_t			sliceScale					= ::llc::n2f3_t{::llc::math_2pi * cellUnits.x, ::llc::math_pi * cellUnits.y};
	for(::llc::u2_t y = 0; y < vertexCount.y; ++y) {
		const double				radius						= ::llc::interpolate_linear(params.Radius.Min, params.Radius.Max, cellUnits.y * y);
		for(::llc::u2_t x = 0; x < vertexCount.x; ++x) {
			::llc::n3f3_t				position					= {sin(sliceScale.y * y) * sin(sliceScale.x * x) * radius, sin(sliceScale.y * y) * sin(::llc::math_pi * x / params.CellCount.x) * params.Length, cos(sliceScale.x * x) * radius};
			geometry.Positions.push_back(params.Orientation.RotateVector(position.f2() - params.Origin));
		}
	}
	const ::llc::u2_t				indexOffset				= geometry.PositionIndices.size();
	llc_necs(::geometryBuildGridIndices(geometry.PositionIndices, vertexOffset, params.CellCount, false, false)); 
	for(::llc::u2_t iVertexIndex = indexOffset; iVertexIndex < geometry.PositionIndices.size(); iVertexIndex += 3) {
		const ::llc::n3f2_t	a	= geometry.Positions[geometry.PositionIndices[iVertexIndex + 0]];
		const ::llc::n3f2_t	b	= geometry.Positions[geometry.PositionIndices[iVertexIndex + 1]];
		const ::llc::n3f2_t	c	= geometry.Positions[geometry.PositionIndices[iVertexIndex + 2]];
		geometry.Normals.push_back(params.Orientation.RotateVector((a - b).Normalize().Cross((a - c).Normalize()).Normalize()));
	}
	return 0;
}
::llc::error_t			llc::geometryBuildHelixHalf	(::llc::SGeometryBuffers & geometry, const ::llc::SParamsHelix & params) {
	const ::llc::u2_t				vertexOffset				= geometry.Positions.size();
	const ::llc::n2u1_t			vertexCount					= params.CellCount + ::llc::n2u1_t{1, 1};

	// -- Generate texture coordinates
	const ::llc::n2f3_t			cellUnits					= {1.0f / params.CellCount.x, 1.0f / params.CellCount.y};
	geometry.TextureCoords	.reserve(vertexCount.Area());
	geometry.Positions		.reserve(vertexCount.Area());
	geometry.Normals		.reserve(vertexCount.Area());
	vertexCount.for_each([&geometry, &params, cellUnits](::llc::n2u1_t & coord) { geometry.TextureCoords.push_back(::llc::n2u2_t{coord.x, (::llc::u2_t)params.CellCount.y - coord.y}.f2().InPlaceScale(cellUnits).f2()); });

	//const double				lengthScale					= cellUnits.y * params.Length;
	const ::llc::n2f3_t			sliceScale					= ::llc::n2f3_t{::llc::math_2pi * cellUnits.x, ::llc::math_pi * cellUnits.y};
	for(::llc::u2_t y = 0; y < vertexCount.y; ++y) {
		const double				radius						= ::llc::interpolate_linear(params.Radius.Min, params.Radius.Max, cellUnits.y * y);
		for(::llc::u2_t x = 0; x < vertexCount.x; ++x) {
			::llc::n3f3_t				position					= {sin(sliceScale.y * y) * sin(sliceScale.x * x) * radius, sin(sliceScale.y * y) * cos(::llc::math_pi * x / params.CellCount.x) * params.Length, cos(sliceScale.x * x) * radius};
			geometry.Positions.push_back(params.Orientation.RotateVector(position.f2() - params.Origin));
		}
	}

	const ::llc::u2_t				indexOffset				= geometry.PositionIndices.size();
	llc_necs(::geometryBuildGridIndices(geometry.PositionIndices, vertexOffset, params.CellCount, false, false)); 
	for(::llc::u2_t iVertexIndex = indexOffset; iVertexIndex < geometry.PositionIndices.size(); iVertexIndex += 3) {
		const ::llc::n3f2_t	a	= geometry.Positions[geometry.PositionIndices[iVertexIndex + 0]];
		const ::llc::n3f2_t	b	= geometry.Positions[geometry.PositionIndices[iVertexIndex + 1]];
		const ::llc::n3f2_t	c	= geometry.Positions[geometry.PositionIndices[iVertexIndex + 2]];
		geometry.Normals.push_back(params.Orientation.RotateVector((a - b).Normalize().Cross((a - c).Normalize()).Normalize()));
	}
	return 0;
}
::llc::error_t			llc::geometryBuildHelix		(::llc::SGeometryBuffers & geometry, const ::llc::SParamsHelix & params)	{
	const ::llc::u2_t				vertexOffset				= geometry.Positions.size();
	const ::llc::u2_t				normalOffset				= geometry.Normals.size();
	const ::llc::u2_t				indexOffset				= geometry.PositionIndices.size();
	llc_necs(llc::geometryBuildHelixHalf(geometry, params));
	const ::llc::u2_t				vertexCount					= geometry.Positions.size() - vertexOffset;
	const ::llc::u2_t				normalCount					= geometry.Normals.size() - normalOffset;
	const ::llc::u2_t				indexCount					= geometry.PositionIndices.size() - indexOffset;
	llc_necs(geometry.TextureCoords	.append({&geometry.TextureCoords[vertexOffset], vertexCount}));
	llc_necs(geometry.Positions		.append({&geometry.Positions	[vertexOffset], vertexCount}));
	llc_necs(geometry.Normals		.append({&geometry.Normals		[normalOffset], normalCount}));
	for(::llc::u2_t iIndex = 0; iIndex < indexCount; ++iIndex)
		llc_necs(geometry.PositionIndices.push_back(geometry.PositionIndices[indexOffset + iIndex] + vertexCount));
	geometry.Positions	.for_each([](::llc::n3f2_t & coord){ coord.z *= -1; coord.y *= -1; return 0; }, vertexOffset + vertexCount);
	geometry.Normals	.for_each([](::llc::n3f2_t & coord){ coord.z *= -1; coord.y *= -1; return 0; }, normalOffset + normalCount);
	return 0;
}
::llc::error_t			llc::geometryBuildRingSide	(::llc::SGeometryBuffers & geometry, const ::llc::SParamsRingSide & params) {
	const ::llc::u2_t				vertexOffset				= geometry.Positions.size();
	::llc::SParamsGrid			paramsGrid					= {{}, {params.Slices, 1}, params.Reverse};
	llc_necall(llc::geometryBuildGrid(geometry, paramsGrid), "params.Slices: %i", params.Slices);
	const ::llc::u2_t				vertexCount					= geometry.Positions.size() - vertexOffset;
	const double				sliceScale					= ::llc::math_2pi * params.Circumference;
	geometry.Positions.enumerate([sliceScale, params, &geometry](const ::llc::u2_t & index, const ::llc::n3f2_t & position) {
			const double				weight						= position.x * sliceScale;
			const ::llc::SSinCos		sinCos						= {sin(weight), -cos(weight)};
			const ::llc::n3f3_t			relativePosOuter			= {sinCos.Sin * params.Radius.Max, 0, sinCos.Cos * params.Radius.Max};
			const ::llc::n3f3_t			relativePosInner			= {sinCos.Sin * params.Radius.Min, 0, sinCos.Cos * params.Radius.Min};
			::llc::n3f2_t				& posOuter					= geometry.Positions[index];
			::llc::n3f2_t				& posInner					= geometry.Positions[index + params.Slices + 1];
			posInner				= params.Orientation.RotateVector((relativePosInner.f2() * -1.f) - params.Origin);
			posOuter				= params.Orientation.RotateVector((relativePosOuter.f2() * -1.f) - params.Origin);
			return 0; 
		}, vertexOffset, vertexOffset + (vertexCount >> 1)
	);
	return 0;
}
::llc::error_t			llc::geometryBuildRingFlat	(::llc::SGeometryBuffers & geometry, const ::llc::SParamsRingSide & params) {
	llc_necs(llc::geometryBuildRingSide(geometry, params));
	::llc::SParamsRingSide		paramsSide					= params;
	paramsSide.Reverse		= true;
	llc_necs(llc::geometryBuildRingSide(geometry, paramsSide));
	return 0;
}
::llc::error_t			llc::geometryBuildDisc(::llc::SGeometryBuffers & geometry, const ::llc::SParamsDisc & params) {
	::llc::SParamsCircle		paramsSide					= params;
	llc_necs(llc::geometryBuildCircleSide(geometry, params));
	paramsSide.Reverse		= true;
	llc_necs(llc::geometryBuildCircleSide(geometry, paramsSide));
	return 0;
}
::llc::error_t			llc::geometryBuildRing		(::llc::SGeometryBuffers & geometry, const ::llc::SParamsTube & params) {
	::llc::SParamsRingSide		paramsSide					= {};
	paramsSide.Origin		= params.Origin;
	paramsSide.Slices		= params.CellCount.x;
	paramsSide.Radius		= params.RadiusYMin;
	paramsSide.Reverse		= !params.Reverse;	// looking down
	paramsSide.Circumference= params.Circumference;
	paramsSide.Orientation	= params.Orientation;
	llc_necs(llc::geometryBuildRingSide(geometry, paramsSide));
	{
		::llc::SParamsCylinderWall	paramsWall					= {};
		paramsWall.Origin		= params.Origin;
		paramsWall.CellCount	= params.CellCount;
		paramsWall.Radius		= {params.RadiusYMin.Max, params.RadiusYMax.Max};
		paramsWall.Circumference= params.Circumference;
		paramsWall.Height		= params.Height;
		paramsWall.Orientation	= params.Orientation;
		paramsWall.Reverse		= params.Reverse;
		llc_necs(llc::geometryBuildCylinderWall(geometry, paramsWall));
		////
		paramsWall.Radius		= {params.RadiusYMin.Min, params.RadiusYMax.Min};
		paramsWall.Reverse		= !paramsWall.Reverse;
		llc_necs(llc::geometryBuildCylinderWall(geometry, paramsWall));
	}
	//
	paramsSide.Origin.y		-= params.Height;
	paramsSide.Radius		= params.RadiusYMax;
	paramsSide.Reverse		= !paramsSide.Reverse;	// looking up
	llc_necs(llc::geometryBuildRingSide(geometry, paramsSide));
	return 0;
}
::llc::error_t			llc::geometryBuildCylinder		(::llc::SGeometryBuffers & geometry, const ::llc::SParamsCylinderWall & params) {
	::llc::SParamsCircle		cap							= {};
	cap.Origin				= params.Origin;
	cap.Radius				= params.Radius.Min;
	cap.Reverse				= !params.Reverse;
	cap.Orientation			= params.Orientation;
	llc_necs(llc::geometryBuildCircleSide(geometry, cap));

	llc_necs(llc::geometryBuildCylinderWall(geometry, params));

	cap.Reverse				= !cap.Reverse;
	cap.Radius				= params.Radius.Max;
	cap.Origin.y			-= params.Height;
	llc_necs(llc::geometryBuildCircleSide(geometry, cap));
	return 0;
}
#endif
