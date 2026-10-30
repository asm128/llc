#ifndef LLC_ATMEL

#include "llc_geometry.h"
#include "llc_voxel.h"
#include "llc_view_n3.h"
#include "llc_sphere.h"

LLC_USING_APOD();
LLC_USING_VIEW();
LLC_USING_TYPEINT();
LLC_USING_N2();
LLC_USING_N3();

usng llc::one_if;
usng nmsp llc;

llc::err_t			llc::geometryBuildBox		(SGeometryBuffers & geometry, const SParamsBox & params) {
	const u2_t				offsetPositions				= geometry.Positions		.size(); geometry.Positions			.resize(offsetPositions			+ 24);
	const u2_t				offsetNormals				= geometry.Normals			.size(); geometry.Normals			.resize(offsetNormals			+ 24);
	const u2_t				offsetTextureCoords			= geometry.TextureCoords	.size(); geometry.TextureCoords		.resize(offsetTextureCoords		+ 24);
	const u2_t				offsetPositionIndices		= geometry.PositionIndices	.size(); geometry.PositionIndices	.resize(offsetPositionIndices	+ 36);
	const vcn3f2_t			vertices					= {&VOXEL_FACE_VERTICES[0].A, 24};
	for(u2_t iVertex = offsetPositions; iVertex < geometry.Positions.size(); ++iVertex)
		geometry.Positions[iVertex]	= params.Orientation.RotateVector(vertices[iVertex - offsetPositions].Scaled(params.HalfSizes * 2.0f) - params.Origin);

	memcpy(&geometry.Normals		[offsetNormals		], VOXEL_FACE_NORMALS.begin(), VOXEL_FACE_NORMALS	.byte_count());
	for(u2_t iVertex = offsetNormals; iVertex < geometry.Normals.size(); ++iVertex)
		geometry.Normals[iVertex]	= params.Orientation.RotateVector(geometry.Normals[iVertex]);
	memcpy(&geometry.TextureCoords	[offsetTextureCoords], VOXEL_FACE_UV.begin()		, VOXEL_FACE_UV		.byte_count());
	const vcu1_t			indices						= {VOXEL_FACE_INDICES_16[0], 36};
	for(u2_t iIndex = offsetPositionIndices; iIndex < geometry.PositionIndices.size(); ++iIndex) 
		geometry.PositionIndices[iIndex] = offsetPositions + indices[iIndex - offsetPositionIndices];

	return 0;
}

enum SQUARE_MODE 
	{ SQUARE_MODE_TOP_LEFT
	, SQUARE_MODE_TOP_RIGHT
	};

tplt<tpnm _tIndex>
sttc	llc::err_t	geometryBuildGridIndices	(apod<_tIndex> & positionIndices, u2_t vertexOffset, const n2u1_t cellCount, bool counterClockwise, bool outward) {
	const u1_t				pitch						= cellCount.x + 1;								// 2 3 
	const astu2_t<4>		indices						= counterClockwise ? astu2_t<4>{{1U, 0U, u2_t(pitch + 1U), pitch}} : astu2_t<4>{{0U, 1U, pitch, u2_t(pitch + 1U)}};	// 0 1 2 3
	const u2_t				indices_modes[2][6]			=
		{ {indices[0], indices[2], indices[3], indices[0], indices[3], indices[1]}
		, {indices[0], indices[2], indices[1], indices[1], indices[2], indices[3]}
		};
	const n2u1_t			halfCount					= {u1_t(cellCount.x >> 1U), u1_t(cellCount.y >> 1U)};
	cellCount.enumerate([&positionIndices, halfCount, indices_modes, vertexOffset, outward](u2_t & vertexIndex, n2u1_t & coord) {
		const bool					reverseQuad 
			= ( (coord.x >= halfCount.x && coord.y <  halfCount.y)
			 || (coord.x <  halfCount.x && coord.y >= halfCount.y)
			);
		const vcu2_t			relative_indices			= indices_modes[((reverseQuad ? SQUARE_MODE_TOP_RIGHT : SQUARE_MODE_TOP_LEFT) + one_if(outward)) % 2];
		for(u2_t i = 0; i < 6; ++i) {
			positionIndices.push_back(vertexOffset + vertexIndex + relative_indices[i]);
		}
	}, [](u2_t & vertexIndex, n2u1_t & ) { 
		++vertexIndex; 
	});
	return 0;
}
llc::err_t			llc::geometryBuildGrid		(SGeometryBuffers & geometry, const SParamsGrid & params) {
	const u2_t				vertexOffset				= geometry.Positions.size();
	const n2u1_t			vertexCount					= params.CellCount + n2u1_t{1, 1};

	// -- Generate texture coordinates
	const n2f3_t			cellUnits					= {1.0f / params.CellCount.x, 1.0f / params.CellCount.y};
	vertexCount.for_each([&geometry, &params, cellUnits](n2u1_t & coord) { geometry.TextureCoords.push_back(n2u2_t{coord.x, (u2_t)params.CellCount.y - coord.y}.f2().InPlaceScale(cellUnits).f2()); });

	// -- Generate normals
	geometry.Normals.resize(geometry.Normals.size() + vertexCount.Area(), params.Orientation.RotateVector({0, params.Reverse ? -1.0f : 1.0f, 0}));	// The normals are all the same for grids

	// -- Generate positions
	const n2f3_t			scale						= params.Size.f3().InPlaceScale(cellUnits);
	vertexCount.for_each([&geometry, scale, params](n2u1_t & coord) {
		const n3f3_t			position					= {coord.x * scale.x, 0, coord.y * scale.y};
		geometry.Positions.push_back(params.Orientation.RotateVector(position.f2() - params.Origin));
	});
	return ::geometryBuildGridIndices(geometry.PositionIndices, vertexOffset, params.CellCount, params.Reverse, params.Outward); 
}
llc::err_t			llc::geometryBuildSphere	(SGeometryBuffers & geometry, const SParamsSphere & params) {
	const u2_t				 vertexOffset				= geometry.Positions.size();
	const n2u1_t			vertexCount					= params.CellCount + n2u1_t{1, 1};

	// -- Generate texture coordinates
	const n2f3_t			cellUnits					= {1.0f / params.CellCount.x, 1.0f / params.CellCount.y};
	vertexCount.for_each([&geometry, &params, cellUnits](n2u1_t & coord) { geometry.TextureCoords.push_back(n2u2_t{coord.x, (u2_t)params.CellCount.y - coord.y}.f3().InPlaceScale(cellUnits).f2()); });

	// -- Generate normals and positions
	const f3_t				reverseScale				= params.Reverse ? -1.0 : 1.0;
	const n2f3_t			sliceScale					= n2f3_t{math_2pi * cellUnits.x * reverseScale * params.Circumference, math_pi * cellUnits.y};
	const f3_t				sliceOffset					= 0; //math_pi * (1.0 - params.Circumference) * .5;
	for(u2_t y = 0; y < vertexCount.y; ++y) {
		const f3_t				currentY					= sliceScale.y * y;
		const f3_t				currentRadius				= sin(currentY);
		for(u2_t x = 0; x < vertexCount.x; ++x) {
			const f3_t				currentX					= sliceScale.x * x + sliceOffset;
			const n3f3_t			coord 						= n3f3_t{currentRadius * cos(currentX), -cos(currentY), currentRadius * sin(currentX)};
			if_fail_fe(geometry.Normals	.push_back(params.Orientation.RotateVector((coord * reverseScale).f2())));
			if_fail_fe(geometry.Positions	.push_back(params.Orientation.RotateVector((coord * params.Radius).f2() - params.Origin)));
		}
	}
	return ::geometryBuildGridIndices(geometry.PositionIndices, vertexOffset, params.CellCount, false, false); 
}
llc::err_t			llc::geometryBuildCircleSide(SGeometryBuffers & geometry, const SParamsCircle & params) {
	const u2_t				vertexOffset				= geometry.Positions.size();
	const u2_t				vertexCount					= (u2_t)params.Slices + 1;
	const f3_t				cellUnits					= 1.0 / params.Slices;
	const f3_t				reverseScale				= params.Reverse ? -1.0 : 1.0;
	const f3_t				sliceScale					= cellUnits * math_2pi * params.Circumference * reverseScale;
	const f3_t				sliceOffset					= 0; //math_2pi * (1.0 - params.Circumference);

	// -- Generate positions and texture coordinates
	if_fail_fe(geometry.Positions		.push_back(params.Orientation.RotateVector(-params.Origin)));	// The first vertex is the center of the circle
	if_fail_fe(geometry.TextureCoords	.push_back(n2f2_t{.5f, .5f}));					// The first vertex is the center of the circle
	for(u2_t iVertex = 0; iVertex < vertexCount; ++iVertex) {
		f3_t						angle						= sliceScale * iVertex + sliceOffset;
		if_fail_fe(geometry.Positions		.push_back(params.Orientation.RotateVector(n3f3_t{params.Radius}.RotateY(angle).f2() - params.Origin)));
		if_fail_fe(geometry.TextureCoords	.push_back(n2f3_t{.5}.Rotate(angle).f2() + n2f2_t{.5f, .5f}));
	}

	// -- Generate normals
	if_fail_fe(geometry.Normals.resize(geometry.Normals.size() + vertexCount + 1, params.Orientation.RotateVector({0, 1, 0})));	// The normals are all the same for a circle

	for(u2_t iSlice = 0, sliceCount = params.Slices + 1; iSlice < sliceCount; ++iSlice)
		if_fail_fe(geometry.PositionIndices.append({vertexOffset + 0, vertexOffset + iSlice + 1, vertexOffset + iSlice + 0}));

	return 0; 
}
llc::err_t			llc::geometryBuildCylinderWall	(SGeometryBuffers & geometry, const SParamsCylinderWall & params) {
	const u2_t					vertexOffset				= geometry.Positions.size();
	const n2u1_t				vertexCount					= params.CellCount + n2u1_t{1, 1};

	// -- Generate texture coordinates
	const n2f3_t				cellUnits					= {1.0f / params.CellCount.x, 1.0f / params.CellCount.y};
	vertexCount.for_each([&geometry, &params, cellUnits](n2u1_t & coord) { geometry.TextureCoords.push_back(n2u2_t{coord.x, (u2_t)params.CellCount.y - coord.y}.f3().InPlaceScale(cellUnits).f2()); });

	// -- Generate normals
	const f3_t					reverseScale				= params.Reverse ? -1.0 : 1.0;
	const f3_t					lengthScale					= cellUnits.y * params.Height;
	const n3f3_t				normalBase					= llc::n3f3_t{params.Height, -(params.Radius.Max - params.Radius.Min)}.Normalize() * reverseScale;
	const f3_t					sliceScale					= cellUnits.x * math_2pi * params.Circumference * reverseScale;
	const f3_t					sliceOffset					= 0; //math_pi * (1.0 - params.Circumference) * .5;
	vertexCount.for_each([&geometry, &normalBase, sliceScale, sliceOffset, &params](n2u1_t & coord) { geometry.Normals.push_back(params.Orientation.RotateVector((n3f3_t{normalBase}.RotateY(sliceScale * coord.x + sliceOffset)).f2())); });

	// -- Generate positions
	for(u2_t y = 0; y < vertexCount.y; ++y) {
		f3_c					radius						= interpolate_linear((f3_t)params.Radius.Min, (f3_t)params.Radius.Max, cellUnits.y * y);
		for(u2_t x = 0; x < vertexCount.x; ++x)
			geometry.Positions.push_back(params.Orientation.RotateVector(n3f3_t{radius, (f3_t)y * lengthScale}.RotateY(sliceScale * x + sliceOffset).f2() - params.Origin));
	}
	return ::geometryBuildGridIndices(geometry.PositionIndices, vertexOffset, params.CellCount, false, false); 
}
//
llc::err_t			llc::geometryBuildFigure0	(SGeometryBuffers & geometry, const SParamsHelix & params) { //(SGeometryQuads & geometry, u2_t stacks, u2_t slices, f2_t radius, const n3f2_t & gridCenter)	{
	const u2_t					vertexOffset				= geometry.Positions.size();
	const n2u1_t				vertexCount					= params.CellCount + n2u1_t{1, 1};

	// -- Generate texture coordinates
	const n2f3_t				cellUnits					= {1.0f / params.CellCount.x, 1.0f / params.CellCount.y};
	geometry.TextureCoords	.reserve(vertexCount.Area());
	geometry.Positions		.reserve(vertexCount.Area());
	geometry.Normals		.reserve(vertexCount.Area());
	vertexCount.for_each([&geometry, &params, cellUnits](n2u1_t & coord) { geometry.TextureCoords.push_back(n2u2_t{coord.x, (u2_t)params.CellCount.y - coord.y}.f2().InPlaceScale(cellUnits).f2()); });

	//const f3_t				lengthScale					= cellUnits.y * params.Length;
	const n2f3_t			sliceScale					= n2f3_t{math_2pi * cellUnits.x, math_pi * cellUnits.y};
	for(u2_t y = 0; y < vertexCount.y; ++y) {
		const f3_t					radius						= interpolate_linear((f3_t)params.Radius.Min, (f3_t)params.Radius.Max, cellUnits.y * y);
		for(u2_t x = 0; x < vertexCount.x; ++x) {
			n3f3_t				position					= {sin(sliceScale.y * y) * sin(sliceScale.x * x) * radius, sin(sliceScale.y * y) * sin(math_pi * x / params.CellCount.x) * params.Length, cos(sliceScale.x * x) * radius};
			geometry.Positions.push_back(params.Orientation.RotateVector(position.f2() - params.Origin));
		}
	}
	const u2_t				indexOffset				= geometry.PositionIndices.size();
	if_fail_fe(::geometryBuildGridIndices(geometry.PositionIndices, vertexOffset, params.CellCount, false, false)); 
	for(u2_t iVertexIndex = indexOffset; iVertexIndex < geometry.PositionIndices.size(); iVertexIndex += 3) {
		const n3f2_t	a	= geometry.Positions[geometry.PositionIndices[iVertexIndex + 0]];
		const n3f2_t	b	= geometry.Positions[geometry.PositionIndices[iVertexIndex + 1]];
		const n3f2_t	c	= geometry.Positions[geometry.PositionIndices[iVertexIndex + 2]];
		geometry.Normals.push_back(params.Orientation.RotateVector((a - b).Normalize().Cross((a - c).Normalize()).Normalize()));
	}
	return 0;
}
llc::err_t			llc::geometryBuildHelixHalf	(SGeometryBuffers & geometry, const SParamsHelix & params) {
	const u2_t				vertexOffset				= geometry.Positions.size();
	const n2u1_t			vertexCount					= params.CellCount + n2u1_t{1, 1};

	// -- Generate texture coordinates
	const n2f3_t			cellUnits					= {1.0f / params.CellCount.x, 1.0f / params.CellCount.y};
	geometry.TextureCoords	.reserve(vertexCount.Area());
	geometry.Positions		.reserve(vertexCount.Area());
	geometry.Normals		.reserve(vertexCount.Area());
	vertexCount.for_each([&geometry, &params, cellUnits](n2u1_t & coord) { geometry.TextureCoords.push_back(n2u2_t{coord.x, (u2_t)params.CellCount.y - coord.y}.f2().InPlaceScale(cellUnits).f2()); });

	//const f3_t				lengthScale					= cellUnits.y * params.Length;
	const n2f3_t			sliceScale					= n2f3_t{math_2pi * cellUnits.x, math_pi * cellUnits.y};
	for(u2_t y = 0; y < vertexCount.y; ++y) {
		const f3_t				radius						= ::llc::interpolate_linear((f3_t)params.Radius.Min, (f3_t)params.Radius.Max, cellUnits.y * y);
		for(u2_t x = 0; x < vertexCount.x; ++x) {
			n3f3_t				position					= {sin(sliceScale.y * y) * sin(sliceScale.x * x) * radius, sin(sliceScale.y * y) * cos(math_pi * x / params.CellCount.x) * params.Length, cos(sliceScale.x * x) * radius};
			geometry.Positions.push_back(params.Orientation.RotateVector(position.f2() - params.Origin));
		}
	}

	const u2_t				indexOffset				= geometry.PositionIndices.size();
	if_fail_fe(::geometryBuildGridIndices(geometry.PositionIndices, vertexOffset, params.CellCount, false, false)); 
	for(u2_t iVertexIndex = indexOffset; iVertexIndex < geometry.PositionIndices.size(); iVertexIndex += 3) {
		const n3f2_t	a	= geometry.Positions[geometry.PositionIndices[iVertexIndex + 0]];
		const n3f2_t	b	= geometry.Positions[geometry.PositionIndices[iVertexIndex + 1]];
		const n3f2_t	c	= geometry.Positions[geometry.PositionIndices[iVertexIndex + 2]];
		geometry.Normals.push_back(params.Orientation.RotateVector((a - b).Normalize().Cross((a - c).Normalize()).Normalize()));
	}
	return 0;
}
llc::err_t			llc::geometryBuildHelix		(SGeometryBuffers & geometry, const SParamsHelix & params)	{
	const u2_t				vertexOffset				= geometry.Positions.size();
	const u2_t				normalOffset				= geometry.Normals.size();
	const u2_t				indexOffset				= geometry.PositionIndices.size();
	if_fail_fe(llc::geometryBuildHelixHalf(geometry, params));
	const u2_t				vertexCount					= geometry.Positions.size() - vertexOffset;
	const u2_t				normalCount					= geometry.Normals.size() - normalOffset;
	const u2_t				indexCount					= geometry.PositionIndices.size() - indexOffset;
	if_fail_fe(geometry.TextureCoords	.append({&geometry.TextureCoords[vertexOffset], vertexCount}));
	if_fail_fe(geometry.Positions		.append({&geometry.Positions	[vertexOffset], vertexCount}));
	if_fail_fe(geometry.Normals		.append({&geometry.Normals		[normalOffset], normalCount}));
	for(u2_t iIndex = 0; iIndex < indexCount; ++iIndex)
		if_fail_fe(geometry.PositionIndices.push_back(geometry.PositionIndices[indexOffset + iIndex] + vertexCount));
	geometry.Positions	.for_each([](n3f2_t & coord){ coord.z *= -1; coord.y *= -1; return 0; }, vertexOffset + vertexCount);
	geometry.Normals	.for_each([](n3f2_t & coord){ coord.z *= -1; coord.y *= -1; return 0; }, normalOffset + normalCount);
	return 0;
}
llc::err_t			llc::geometryBuildRingSide	(SGeometryBuffers & geometry, const SParamsRingSide & params) {
	const u2_t				vertexOffset				= geometry.Positions.size();
	SParamsGrid			paramsGrid					= {{}, {params.Slices, 1}, params.Reverse};
	if_fail_fef(llc::geometryBuildGrid(geometry, paramsGrid), "params.Slices: %i", params.Slices);
	const u2_t				vertexCount					= geometry.Positions.size() - vertexOffset;
	const f3_t				sliceScale					= math_2pi * params.Circumference;
	geometry.Positions.enumerate([sliceScale, params, &geometry](const u2_t & index, const n3f2_t & position) {
			const f3_t				weight						= position.x * sliceScale;
			const SSinCos		sinCos						= {sin(weight), -cos(weight)};
			const n3f3_t			relativePosOuter			= {sinCos.Sin * params.Radius.Max, 0, sinCos.Cos * params.Radius.Max};
			const n3f3_t			relativePosInner			= {sinCos.Sin * params.Radius.Min, 0, sinCos.Cos * params.Radius.Min};
			n3f2_t				& posOuter					= geometry.Positions[index];
			n3f2_t				& posInner					= geometry.Positions[index + params.Slices + 1];
			posInner				= params.Orientation.RotateVector((relativePosInner.f2() * -1.f) - params.Origin);
			posOuter				= params.Orientation.RotateVector((relativePosOuter.f2() * -1.f) - params.Origin);
			return 0; 
		}, vertexOffset, vertexOffset + (vertexCount >> 1)
	);
	return 0;
}
llc::err_t			llc::geometryBuildRingFlat	(SGeometryBuffers & geometry, const SParamsRingSide & params) {
	if_fail_fe(llc::geometryBuildRingSide(geometry, params));
	SParamsRingSide			paramsSide					= params;
	paramsSide.Reverse	= true;
	if_fail_fe(llc::geometryBuildRingSide(geometry, paramsSide));
	return 0;
}
llc::err_t			llc::geometryBuildDisc(SGeometryBuffers & geometry, const SParamsDisc & params) {
	SParamsCircle			paramsSide					= params;
	if_fail_fe(llc::geometryBuildCircleSide(geometry, params));
	paramsSide.Reverse	= true;
	if_fail_fe(llc::geometryBuildCircleSide(geometry, paramsSide));
	return 0;
}
llc::err_t			llc::geometryBuildRing		(SGeometryBuffers & geometry, const SParamsTube & params) {
	SParamsRingSide			paramsSide					= {};
	paramsSide.Origin		= params.Origin;
	paramsSide.Slices		= params.CellCount.x;
	paramsSide.Radius		= params.RadiusYMin;
	paramsSide.Reverse		= !params.Reverse;	// looking down
	paramsSide.Circumference= params.Circumference;
	paramsSide.Orientation	= params.Orientation;
	if_fail_fe(llc::geometryBuildRingSide(geometry, paramsSide));
	{
		SParamsCylinderWall		paramsWall					= {};
		paramsWall.Origin		= params.Origin;
		paramsWall.CellCount	= params.CellCount;
		paramsWall.Radius		= {params.RadiusYMin.Max, params.RadiusYMax.Max};
		paramsWall.Circumference= params.Circumference;
		paramsWall.Height		= params.Height;
		paramsWall.Orientation	= params.Orientation;
		paramsWall.Reverse		= params.Reverse;
		if_fail_fe(llc::geometryBuildCylinderWall(geometry, paramsWall));
		////
		paramsWall.Radius		= {params.RadiusYMin.Min, params.RadiusYMax.Min};
		paramsWall.Reverse		= !paramsWall.Reverse;
		if_fail_fe(llc::geometryBuildCylinderWall(geometry, paramsWall));
	}
	//
	paramsSide.Origin.y		-= params.Height;
	paramsSide.Radius		= params.RadiusYMax;
	paramsSide.Reverse		= !paramsSide.Reverse;	// looking up
	if_fail_fe(llc::geometryBuildRingSide(geometry, paramsSide));
	return 0;
}
llc::err_t			llc::geometryBuildCylinder		(SGeometryBuffers & geometry, const SParamsCylinderWall & params) {
	SParamsCircle		cap							= {};
	cap.Origin				= params.Origin;
	cap.Radius				= params.Radius.Min;
	cap.Reverse				= !params.Reverse;
	cap.Orientation			= params.Orientation;
	if_fail_fe(llc::geometryBuildCircleSide(geometry, cap));

	if_fail_fe(llc::geometryBuildCylinderWall(geometry, params));

	cap.Reverse				= !cap.Reverse;
	cap.Radius				= params.Radius.Max;
	cap.Origin.y			-= params.Height;
	if_fail_fe(llc::geometryBuildCircleSide(geometry, cap));
	return 0;
}
#endif
