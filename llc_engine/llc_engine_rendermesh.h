#include "llc_linear_map_pobj.h"

#include "llc_enum.h"

#include "llc_range_n2.h"

#ifndef LLC_ENGINE_RENDERMESH_H_23627
#define LLC_ENGINE_RENDERMESH_H_23627

namespace llc 
{
	// ------------------ Geometry Mesh
#pragma pack(push, 1)
	struct SGeometrySlice {
		::llc::rangeu2_t						Slice;
	};

	GDEFINE_ENUM_TYPE(NORMAL_MODE, u0_t);
	GDEFINE_ENUM_VALUE(NORMAL_MODE, Point			, 0);
	GDEFINE_ENUM_VALUE(NORMAL_MODE, Line			, 1);
	GDEFINE_ENUM_VALUE(NORMAL_MODE, Triangle		, 2);
	GDEFINE_ENUM_VALUE(NORMAL_MODE, Quad			, 3);

	GDEFINE_ENUM_TYPE(MESH_MODE, u0_t);
	GDEFINE_ENUM_VALUE(MESH_MODE, List				, 0);
	GDEFINE_ENUM_VALUE(MESH_MODE, Strip				, 1);
	GDEFINE_ENUM_VALUE(MESH_MODE, Fan				, 2);

	GDEFINE_ENUM_TYPE(GEOMETRY_TYPE, u0_t);
	GDEFINE_ENUM_VALUE(GEOMETRY_TYPE, Point			, 0);
	GDEFINE_ENUM_VALUE(GEOMETRY_TYPE, Line			, 1);
	GDEFINE_ENUM_VALUE(GEOMETRY_TYPE, Triangle		, 2);
	GDEFINE_ENUM_VALUE(GEOMETRY_TYPE, Quad			, 3);

	struct SMeshDescription {
		::llc::GEOMETRY_TYPE				Type;
		::llc::MESH_MODE					Mode;
		::llc::NORMAL_MODE					NormalMode;
	};
#pragma pack(pop)

	struct SGeometryMesh {
		::llc::SMeshDescription				Desc;
		::llc::au2_t						GeometryBuffers;
		::llc::au2_t						ConstantBuffers;
		::llc::apod<::llc::SGeometrySlice>	GeometrySlices;

		::llc::err_t						Save			(::llc::au0_t & output) const { 
			if_fail_fe(llc::savePOD(output, Desc));
			if_fail_fe(llc::saveView(output, GeometryBuffers));
			if_fail_fe(llc::saveView(output, ConstantBuffers));
			if_fail_fe(llc::saveView(output, GeometrySlices));
			return 0;
		}
		::llc::err_t						Load			(::llc::vcu0_t & input) {
			if_fail_fe(llc::loadPOD(input, Desc));
			if_fail_fe(llc::loadView(input, GeometryBuffers));
			if_fail_fe(llc::loadView(input, ConstantBuffers));
			if_fail_fe(llc::loadView(input, GeometrySlices));
			return 0;
		}
	};	

	typedef	::llc::llc_linear_map_pobj<::llc::SGeometryMesh>	SMeshManager;
} // namespace

#endif // LLC_ENGINE_RENDERMESH_H_23627
