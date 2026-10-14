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

	GDEFINE_ENUM_TYPE(NORMAL_MODE, uint8_t);
	GDEFINE_ENUM_VALUE(NORMAL_MODE, Point			, 0);
	GDEFINE_ENUM_VALUE(NORMAL_MODE, Line			, 1);
	GDEFINE_ENUM_VALUE(NORMAL_MODE, Triangle		, 2);
	GDEFINE_ENUM_VALUE(NORMAL_MODE, Quad			, 3);

	GDEFINE_ENUM_TYPE(MESH_MODE, uint8_t);
	GDEFINE_ENUM_VALUE(MESH_MODE, List				, 0);
	GDEFINE_ENUM_VALUE(MESH_MODE, Strip				, 1);
	GDEFINE_ENUM_VALUE(MESH_MODE, Fan				, 2);

	GDEFINE_ENUM_TYPE(GEOMETRY_TYPE, uint8_t);
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

		::llc::error_t						Save			(::llc::au0_t & output) const { 
			llc_necs(llc::savePOD(output, Desc));
			llc_necs(llc::saveView(output, GeometryBuffers));
			llc_necs(llc::saveView(output, ConstantBuffers));
			llc_necs(llc::saveView(output, GeometrySlices));
			return 0;
		}
		::llc::error_t						Load			(::llc::vcu0_t & input) {
			llc_necs(llc::loadPOD(input, Desc));
			llc_necs(llc::loadView(input, GeometryBuffers));
			llc_necs(llc::loadView(input, ConstantBuffers));
			llc_necs(llc::loadView(input, GeometrySlices));
			return 0;
		}
	};	

	typedef	::llc::llc_linear_map_pobj<::llc::SGeometryMesh>	SMeshManager;
} // namespace

#endif // LLC_ENGINE_RENDERMESH_H_23627
