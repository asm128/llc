#include "llc_array_ptr.h"
#include "llc_image.h"
#include "llc_base64.h"

#ifndef LLC_FONT_H_23627
#define LLC_FONT_H_23627

namespace llc
{
	struct SRasterFont {
		::llc::n2u0_t		CharSize;
		::llc::imgmonou3_t	Texture;
	};

	struct SRasterFontBase64 {
		::llc::n2u0_t		CharSize;
		::llc::vcst_t		Base64String;
	};

	struct SRasterFontManager {
		::llc::apobj<::llc::SRasterFont>	Fonts						= {};
	};

	::llc::err_t	rasterFontDefaults	(::llc::SRasterFontManager & manager);

	tplt<tpnm TCell>
	::llc::err_t	rasterFontLoad		(::llc::imgmono<TCell> & image, ::llc::n2u0_c & charSize, ::llc::vcu0_c decoded) {
		cnst ::llc::u2_t	imageHeight	= charSize.y * 256U;
		if_fail_fef(image.resize(charSize.x, imageHeight), "Character size: {%u, %u}.", charSize.x, charSize.y);
		memcpy(image.Texels.begin(), decoded.begin(), ::llc::min(image.Texels.byte_count(), decoded.byte_count()));
		rtrn 0;
	}
	tplt<tpnm TCell>
	::llc::err_t	rasterFontLoadB64	(::llc::imgmono<TCell> & image, ::llc::n2u0_c & charSize, ::llc::vcu0_c & base64String) {
		::llc::au0_t		decoded;
		if_fail_fe(::llc::base64Decode(base64String, decoded));
		rtrn rasterFontLoad(image, charSize, decoded);
	}
	tplt<tpnm TCell>
	::llc::err_t	rasterFontLoadB64	(::llc::imgmono<TCell> & image, ::llc::n2u0_c & charSize, ::llc::vcst_t base64String) {
		::llc::au0_t		decoded;
		if_fail_fe(::llc::base64Decode(base64String, decoded));
		rtrn rasterFontLoad(image, charSize, decoded);
	}
};

#endif // LLC_FONT_H_2903874982374
