#include "llc_color.h"
#include "llc_n2.h"
#include "llc_linear_map_pobj.h"
#include "llc_color_type.h"

#ifndef LLC_ENGINE_RENDERIMAGE_H_23627
#define LLC_ENGINE_RENDERIMAGE_H_23627

namespace llc 
{
#pragma pack(push, 1)
	struct SSurfaceDescription {
		::llc::COLOR_TYPE			ColorType			= {};
		s0_t						BitDepth			= {};
		s0_t						MethodCompression	= {};
		s0_t						MethodFilter		= {};
		s0_t						MethodInterlace		= {};
		::llc::n2u1_t				Dimensions			= {};
	};
#pragma pack(pop)

	struct SSurface {
		::llc::SSurfaceDescription	Desc			= {};
		::llc::au0_t				Data			= {};

		::llc::err_t				Save			(::llc::au0_t & output)	const	{ if_fail_fe(llc::savePOD(output, Desc)); return ::llc::saveView(output, Data); }
		::llc::err_t				Load			(::llc::vcu0_t & input)			{ if_fail_fe(llc::loadPOD(input, Desc)); return ::llc::loadView(input, Data); }
	};

	typedef	::llc::llc_linear_map_pobj<::llc::SSurface>	SSurfaceManager;
} // namespace

#endif // LLC_ENGINE_RENDERIMAGE_H_23627
