#include "llc_apod_color.h"
#include "llc_tri2.h"
#include "llc_tri3.h"
#include "llc_range.h"
#include "llc_color_type.h"
#include "llc_grid_color.h"

#ifndef CED_GEOMETRY_H_23627
#define CED_GEOMETRY_H_23627

namespace llc
{
#pragma pack(push, 1)
	tplt<tpnm _tAxis, tpnm _tColor> struct SLightPoint2 { ::llc::n2f2_t Position; _tColor Color; };
	tplt<tpnm _tAxis, tpnm _tColor> struct SLightPoint3 { ::llc::n3f2_t Position; _tColor Color; };

	struct SImageTag {
		 COLOR_TYPE	ColorType;
		 int32_t	Index;
		 int32_t	Palette;
	};
#pragma pack(pop)

	struct SGeometryQuads {
		::llc::apod<::llc::tri3f2_t>	Triangles;
		::llc::apod<::llc::n3f2_t>	Normals;
		::llc::apod<::llc::tri2f2_t>	TextureCoords;
	};

	struct SGeometryTriangles {
		::llc::apod<::llc::tri3f2_t>	Triangles;
		::llc::apod<::llc::tri3f2_t>	Normals;
		::llc::apod<::llc::tri2f2_t>	TextureCoords;
	};


	GDEFINE_ENUM_TYPE (MODEL_NODE_TYPE, uint8_t);
	GDEFINE_ENUM_VALUE(MODEL_NODE_TYPE, FILE		, 0);
	GDEFINE_ENUM_VALUE(MODEL_NODE_TYPE, FUNCTION	, 1);
	GDEFINE_ENUM_VALUE(MODEL_NODE_TYPE, COMMAND		, 2);
	GDEFINE_ENUM_VALUE(MODEL_NODE_TYPE, POINT		, 3);
	GDEFINE_ENUM_VALUE(MODEL_NODE_TYPE, SEGMENT		, 4);
	GDEFINE_ENUM_VALUE(MODEL_NODE_TYPE, RECTANGLE	, 5);
	GDEFINE_ENUM_VALUE(MODEL_NODE_TYPE, TRIANGLE	, 6);
	GDEFINE_ENUM_VALUE(MODEL_NODE_TYPE, CIRCLE		, 7);
	GDEFINE_ENUM_VALUE(MODEL_NODE_TYPE, BOX			, 8);
	GDEFINE_ENUM_VALUE(MODEL_NODE_TYPE, SPHERE		, 9);
	GDEFINE_ENUM_VALUE(MODEL_NODE_TYPE, CYLINDER	, 10);

	GDEFINE_ENUM_TYPE (PRIMITIVE_TYPE, uint8_t);
	GDEFINE_ENUM_VALUE(PRIMITIVE_TYPE, POINT		, 0);
	GDEFINE_ENUM_VALUE(PRIMITIVE_TYPE, SEGMENT		, 1);
	GDEFINE_ENUM_VALUE(PRIMITIVE_TYPE, TRIANGLE		, 2);
	GDEFINE_ENUM_VALUE(PRIMITIVE_TYPE, RECTANGLE	, 3);

	GDEFINE_ENUM_TYPE (INDEX_MODE, uint8_t);
	GDEFINE_ENUM_VALUE(INDEX_MODE, LIST			, 0);
	GDEFINE_ENUM_VALUE(INDEX_MODE, STRIP		, 1);
	GDEFINE_ENUM_VALUE(INDEX_MODE, FAN			, 2);

	GDEFINE_ENUM_TYPE (BUFFER_TYPE, uint8_t);
	GDEFINE_ENUM_VALUE(BUFFER_TYPE, VERTEX		, 0);
	GDEFINE_ENUM_VALUE(BUFFER_TYPE, POLYGON		, 1);
#pragma pack(push, 1)

	struct SGeometryGroupModes {
		PRIMITIVE_TYPE						PrimitiveType	;
		INDEX_MODE							IndexMode		;

		uint16_t							Transparent		: 1;
		uint16_t							SkipSpecular	: 1;
		uint16_t							SkipAmbient		: 1;
		uint16_t							SkipDiffuse		: 1;
	};

	struct SGeometryGroup {
		uint32_t							Geometry		= (uint32_t)-1;
		uint32_t							Image			= (uint32_t)-1;
		uint32_t							Material		= (uint32_t)-1;
		::llc::SGeometryGroupModes			Modes			= {};
		::llc::rangeu2_t					Slice			= {};
	};
#pragma pack(pop)

	struct SModelNode {
		::llc::as2_t						VertexBuffers	= {};
		::llc::as2_t						Images			= {};
		int32_t								IndexBuffer		= -1;
		::llc::SGeometryGroupModes			Modes			= {};
		::llc::rangeu1_t					RangeIndex		= {};
		::llc::rangeu1_t					RangeVertex		= {};
	};

	struct SModelQuads {
		::llc::apod<::llc::tri3f2_t>			Triangles;
		::llc::apod<::llc::n3f2_t>			Normals;
		::llc::apod<::llc::tri2f2_t>			TextureCoords;
		::llc::apod<::llc::SGeometryGroup>	GeometryGroups;
	};

	struct SModelTriangles {
		::llc::apod<::llc::tri3f2_t>			Triangles;
		::llc::apod<::llc::tri3f2_t>			Normals;
		::llc::apod<::llc::tri2f2_t>			TextureCoords;
		::llc::apod<::llc::SGeometryGroup>	GeometryGroups;
	};

#pragma pack(push, 1)
	struct STile {
		float			Height	[4]	= {};
		int16_t			Top			= -1;
		int16_t			Front		= -1;
		int16_t			Right		= -1;
		int16_t			Flags		= -1;
	};

	tplt<tpnm _tIndex>
	struct STileIndices {
		_tIndex			Top		[4]	= {};
		_tIndex			Front	[4]	= {};
		_tIndex			Right	[4]	= {};
	};

#pragma pack(pop)

#ifndef LLC_ATMEL

	::llc::error_t	geometryBuildCube		(::llc::SGeometryQuads & geometry, ::llc::n3f2_c & scale);
	::llc::error_t	geometryBuildGrid		(::llc::SGeometryQuads & geometry, ::llc::n2u1_t gridSize, ::llc::n2f2_t gridCenter, ::llc::n3f2_c & scale);

	::llc::error_t	geometryBuildSphere		(::llc::SGeometryQuads & geometry, uint32_t stacks, uint32_t slices, float radius, ::llc::n3f2_c & gridCenter);
	::llc::error_t	geometryBuildCylinder	(::llc::SGeometryQuads & geometry, uint32_t stacks, uint32_t slices, float radiusYMin, float radiusYMax, ::llc::n3f2_c & gridCenter, ::llc::n3f2_c & scale);
	::llc::error_t	geometryBuildHelixHalf	(::llc::SGeometryQuads & geometry, uint32_t stacks, uint32_t slices, float radius, ::llc::n3f2_c & gridCenter, ::llc::n3f2_c & scale);
	::llc::error_t	geometryBuildHelix		(::llc::SGeometryQuads & geometry, uint32_t stacks, uint32_t slices, float radius, ::llc::n3f2_c & gridCenter, ::llc::n3f2_c & scale);
	::llc::error_t	geometryBuildTender		(::llc::SGeometryQuads & geometry, uint32_t stacks, uint32_t slices, float radius, ::llc::n3f2_c & gridCenter, ::llc::n3f2_c & scale);
	::llc::error_t	geometryBuildFigure0	(::llc::SGeometryQuads & geometry, uint32_t stacks, uint32_t slices, float radius, ::llc::n3f2_c & gridCenter);
	::llc::error_t	geometryBuildFigure1	(::llc::SGeometryQuads & geometry, uint32_t stacks, uint32_t slices, float radius, ::llc::n3f2_c & gridCenter);

	::llc::error_t	geometryBuildSphere		(::llc::SGeometryTriangles & geometry, uint32_t stacks, uint32_t slices, float radius, ::llc::n3f2_c & gridCenter);
	::llc::error_t	geometryBuildCube		(::llc::SGeometryQuads & geometry, const ::llc::n3f2_t & scale, const ::llc::n3f2_t & translation);

	::llc::error_t	geometryBuildTileListFromImage	(::llc::gc8bgra image, ::llc::apod<STile> & out_tiles, uint32_t imagePitch = 0);
	::llc::error_t	geometryBuildGridFromTileList	(::llc::SGeometryQuads & geometry, ::llc::grid<const ::llc::STile> image, ::llc::n2f2_t gridCenter, const ::llc::n3f2_t & scale);

#endif

#pragma pack(push, 1)
	struct SRenderMaterialPaletted {
		::llc::rgbaf	Diffuse		;
		uint16_t		Color		;
		uint16_t		Palette		;
	};

	struct SAABBGeometry {
		::llc::n3f2_t	Vertices	[8]			= {};
	};
#pragma pack(pop)

} // namespace

#endif // CED_GEOMETRY_H_23627
