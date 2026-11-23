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

	tplt<tpnm _sizeChunk>
	::llc::err_t	rasterFontLoad		(::llc::imgmono<_sizeChunk> & image, const ::llc::n2u0_t & charSize, ::llc::vcu0_c decoded) {
		const uint32_t		imageHeight			= charSize.y * 256;
		llc_necall(image.resize(charSize.x, imageHeight), "charSize: {%i, %i}", charSize.x, charSize.y);
		memcpy(image.Texels.begin(), decoded.begin(), ::llc::min(image.Texels.byte_count(), decoded.byte_count()));
		return 0;
	}
	tplt<tpnm _sizeChunk>
	::llc::err_t	rasterFontLoadB64	(::llc::imgmono<_sizeChunk> & image, const ::llc::n2u0_t & charSize, ::llc::vcu0_c & base64String) {
		::llc::au0_t		decoded;
		llc_necs(llc::base64Decode(base64String, decoded));
		return rasterFontLoad(image, charSize, decoded);
	}
	tplt<tpnm _sizeChunk>
	::llc::err_t	rasterFontLoadB64	(::llc::imgmono<_sizeChunk> & image, const ::llc::n2u0_t & charSize, ::llc::vcst_c & base64String) {
		return ::llc::rasterFontLoadB64(image, charSize, *(::llc::vcu0_c*)&base64String);
	}
};

#endif // LLC_FONT_H_2903874982374