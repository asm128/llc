#include "llc_font_defaults.h"

::llc::err_t			llc::rasterFontDefaults	(::llc::SRasterFontManager & manager) {
	for(::llc::u2_t iFont = 0; iFont < ::llc::size(::llc::RASTER_FONT_DEFAULTS); ++iFont) {
		llc::po<::llc::SRasterFont>	& loadedFont			= manager.Fonts[manager.Fonts.push_back({})];
		loadedFont->CharSize	= ::llc::RASTER_FONT_DEFAULTS[iFont].CharSize;
		if_fail_fe(llc::rasterFontLoadB64(loadedFont->Texture, loadedFont->CharSize, ::llc::RASTER_FONT_DEFAULTS[iFont].Base64String));
	}
	rtrn 0;
}
