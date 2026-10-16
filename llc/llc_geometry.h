#include "llc_geometry_buffers.h"
#include "llc_enum.h"
#include "llc_minmax.h"
#include "llc_quat.h"

#ifndef LLC_GEOMETRY_H_23627
#define LLC_GEOMETRY_H_23627

namespace llc
{
#ifdef LLC_ATMEL
	enum SHAPE_TYPE : u0_t
		{ SHAPE_TYPE_Custom		= 0
		, SHAPE_TYPE_Rectangle	= 1
		, SHAPE_TYPE_Circle		= 2
		, SHAPE_TYPE_Ring		= 3
		, SHAPE_TYPE_Cube		= 4
		, SHAPE_TYPE_Sphere		= 5
		, SHAPE_TYPE_Cylinder	= 6
		, SHAPE_TYPE_Torus		= 7
		};
#else
	GDEFINE_ENUM_TYPE(SHAPE_TYPE, u0_t);
	GDEFINE_ENUM_VALUE(SHAPE_TYPE, Custom		, 0);
	GDEFINE_ENUM_VALUE(SHAPE_TYPE, Rectangle	, 1);
	GDEFINE_ENUM_VALUE(SHAPE_TYPE, Circle		, 2);
	GDEFINE_ENUM_VALUE(SHAPE_TYPE, Ring			, 3);
	GDEFINE_ENUM_VALUE(SHAPE_TYPE, Cube			, 4);
	GDEFINE_ENUM_VALUE(SHAPE_TYPE, Sphere		, 5);
	GDEFINE_ENUM_VALUE(SHAPE_TYPE, Cylinder		, 6);
	GDEFINE_ENUM_VALUE(SHAPE_TYPE, Torus		, 7);
#endif

#pragma pack(push, 1)
	struct SParamsBox {
		::llc::n3f2_t		Origin			= {.5f, .5f, .5f};
		::llc::n3f2_t		HalfSizes		= {.5f, .5f, .5f};
		::llc::quatf2_t		Orientation		= {0, 0, 0, 1};

		LLC_DEFAULT_OPERATOR_C(SParamsBox, Origin == other.Origin && HalfSizes == other.HalfSizes); 
	};

	struct SParamsSphere { 
		::llc::n3f2_t		Origin			= {};
		::llc::n2u1_t		CellCount		= {32, 32};
		bool				Reverse			= false;
		float				Circumference	= 1.0f;
		float				Radius			= .5f;
		::llc::quatf2_t		Orientation		= {0, 0, 0, 1};
	
		LLC_DEFAULT_OPERATOR_C(SParamsSphere, Origin == other.Origin && CellCount == other.CellCount && Reverse == other.Reverse && Circumference == other.Circumference && Radius == other.Radius);
	};

	struct SParamsDisc { 
		::llc::n3f2_t		Origin			= {};
		::llc::u1_t			Slices			= 16;
		float				Circumference	= 1.0f;
		float				Radius			= .5f;
		::llc::quatf2_t		Orientation		= {0, 0, 0, 1};
	
		LLC_DEFAULT_OPERATOR_C(SParamsDisc, Origin == other.Origin && Slices == other.Slices && Circumference == other.Circumference && Radius == other.Radius);
	};

	struct SParamsCircle : SParamsDisc {
		bool				Reverse			= false;

		constexpr			SParamsCircle	()							noexcept	= default;
		constexpr			SParamsCircle	(const SParamsCircle &)		noexcept	= default;
		constexpr			SParamsCircle	(const SParamsDisc & other)	noexcept	: SParamsDisc(other) {}

		LLC_DEFAULT_OPERATOR_C(SParamsCircle, SParamsDisc::operator==(other) && Reverse == other.Reverse);
	};

	struct SParamsRingSide { 
		::llc::n3f2_t		Origin			= {};
		::llc::u1_t			Slices			= 16;
		bool				Reverse			= false;
		float				Circumference	= 1.0f;
		::llc::minmaxf2_t	Radius			= {.45f, .5f};
		::llc::quatf2_t		Orientation		= {0, 0, 0, 1};
	
		LLC_DEFAULT_OPERATOR_C(SParamsRingSide, Origin == other.Origin && Slices == other.Slices && Reverse == other.Reverse && Circumference == other.Circumference && Radius == other.Radius);
	};

	struct SParamsTube { 
		::llc::n3f2_t		Origin			= {};
		::llc::n2u1_t		CellCount		= {16, 1};
		bool				Reverse			= false;
		float				Circumference	= 1.0f;
		::llc::minmaxf2_t	RadiusYMin		= {.45f, .5f};
		::llc::minmaxf2_t	RadiusYMax		= {.45f, .5f};
		float				Height			= 1;
		::llc::quatf2_t		Orientation		= {0, 0, 0, 1};

		LLC_DEFAULT_OPERATOR_C(SParamsTube, Origin == other.Origin && CellCount == other.CellCount && Reverse == other.Reverse && Circumference == other.Circumference && RadiusYMin == other.RadiusYMin && RadiusYMax == other.RadiusYMax && Height == other.Height);
	};

	struct SParamsCylinderWall { 
		::llc::n3f2_t		Origin			= {};
		::llc::n2u1_t		CellCount		= {16, 2};
		bool				Reverse			= false;
		float				Circumference	= 1.0f;
		::llc::minmaxf2_t	Radius			= {.5f, .5f};
		float				Height			= 1;
		::llc::quatf2_t		Orientation		= {0, 0, 0, 1};

		LLC_DEFAULT_OPERATOR_C(SParamsCylinderWall, Origin == other.Origin && CellCount == other.CellCount && Reverse == other.Reverse && Circumference == other.Circumference && Radius == other.Radius && Height == other.Height); 
	};

	struct SParamsGrid { 
		::llc::n3f2_t		Origin			= {.5f, 0, .5f};
		::llc::n2u1_t		CellCount		= {9, 9};
		bool				Reverse			= false;
		bool				Outward			= false;
		::llc::n2f2_t		Size			= {1, 1};
		::llc::quatf2_t		Orientation		= {0, 0, 0, 1};

		LLC_DEFAULT_OPERATOR_C(SParamsGrid, Origin == other.Origin && CellCount == other.CellCount && Reverse == other.Reverse && Outward == other.Outward && Size == other.Size); 
	};

	struct SParamsHelix { 
		::llc::n3f2_t		Origin			= {0, .5f};
		::llc::n2u1_t		CellCount		= {9, 9};
		::llc::minmaxf2_t	Radius			= {.5f, .5f};	// TODO: Not working
		float				Length			= 1;
		::llc::quatf2_t		Orientation		= {0, 0, 0, 1};

		LLC_DEFAULT_OPERATOR_C(SParamsHelix, Origin == other.Origin && CellCount == other.CellCount && Radius == other.Radius && Length == other.Length); 
	};

#pragma pack(pop)

#ifndef LLC_ATMEL
	::llc::error_t		geometryBuildCircleSide		(::llc::SGeometryBuffers & geometry, const ::llc::SParamsCircle			& params);
	::llc::error_t		geometryBuildDisc			(::llc::SGeometryBuffers & geometry, const ::llc::SParamsDisc			& params);
	::llc::error_t		geometryBuildBox			(::llc::SGeometryBuffers & geometry, const ::llc::SParamsBox			& params);
	::llc::error_t		geometryBuildSphere			(::llc::SGeometryBuffers & geometry, const ::llc::SParamsSphere			& params);
	::llc::error_t		geometryBuildCylinderWall	(::llc::SGeometryBuffers & geometry, const ::llc::SParamsCylinderWall	& params);
	::llc::error_t		geometryBuildCylinder		(::llc::SGeometryBuffers & geometry, const ::llc::SParamsCylinderWall	& params);
	::llc::error_t		geometryBuildGrid			(::llc::SGeometryBuffers & geometry, const ::llc::SParamsGrid			& params);
	::llc::error_t		geometryBuildHelixHalf		(::llc::SGeometryBuffers & geometry, const ::llc::SParamsHelix			& params);	// TODO: Rewrite
	::llc::error_t		geometryBuildHelix			(::llc::SGeometryBuffers & geometry, const ::llc::SParamsHelix			& params);
	::llc::error_t		geometryBuildFigure0		(::llc::SGeometryBuffers & geometry, const ::llc::SParamsHelix			& params);
	::llc::error_t		geometryBuildRingSide		(::llc::SGeometryBuffers & geometry, const ::llc::SParamsRingSide		& params);
	::llc::error_t		geometryBuildRingFlat		(::llc::SGeometryBuffers & geometry, const ::llc::SParamsRingSide		& params);
	::llc::error_t		geometryBuildRing			(::llc::SGeometryBuffers & geometry, const ::llc::SParamsTube			& params);
#endif
} // namespace

#endif // LLC_GEOMETRY_H_23627
