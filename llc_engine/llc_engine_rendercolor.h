#include "llc_color.h"

#ifndef LLC_ENGINE_RENDERCOLOR_H_23627
#define LLC_ENGINE_RENDERCOLOR_H_23627

namespace llc 
{
#pragma pack(push, 1)
	struct SRenderColor {
		::llc::rgbaf							Diffuse		= ::llc::RED;
		::llc::rgbaf							Ambient		= ::llc::GREEN;
		::llc::rgbaf							Specular	= ::llc::LIGHTGRAY;
	};
#pragma pack(pop)
} // namespace

#endif // LLC_ENGINE_RENDERCOLOR_H_23627
