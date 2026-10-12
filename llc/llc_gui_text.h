#include "llc_array.h"
#include "llc_grid.h"
#include "llc_image.h"
#include "llc_view_bit.h"
#include "llc_grid_copy.h"
#include "llc_bitmap_target.h"

#ifndef LLC_GUI_TEXT_H_23627
#define LLC_GUI_TEXT_H_23627

namespace llc
{
	inln	::llc::err_t	textFontMetricsValidate			(::llc::n2u2_c & fontMetrics, ::llc::n2u0_c & sizeCharCell, ::llc::u2_t characterCellsX) {
		if_zero_fef(characterCellsX, "%s", "Character column count cannot be zero.");
		if_true_fef(0 == sizeCharCell.x || 0 == sizeCharCell.y, "Character size:{%u, %u}.", sizeCharCell.x, sizeCharCell.y);
		cnst ::llc::u2_t		usedColumns					= ::llc::min<::llc::u2_t>(characterCellsX, 256U);
		cnst ::llc::u2_t		requiredRows					= (256U + characterCellsX - 1U) / characterCellsX;
		cnst ::llc::n2u2_t		requiredMetrics					= {usedColumns * sizeCharCell.x, requiredRows * sizeCharCell.y};
		if_true_fef(requiredMetrics.x > fontMetrics.x || requiredMetrics.y > fontMetrics.y, "Font metrics:{%u, %u}, required:{%u, %u}.", fontMetrics.x, fontMetrics.y, requiredMetrics.x, requiredMetrics.y);
		rtrn 0;
	}

	stincxp	::llc::error_t	textLineCalcWidth				(::llc::n2u0_c & sizeCharCell, ::llc::vcst_c & text0)		{ return (::llc::error_t)(sizeCharCell.x * text0.size()); }

	tplt<tpnm _tColor>
	static	::llc::error_t	textLineDrawFixedSize			(::llc::grid<_tColor> & target, const ::llc::grid<_tColor> & viewTextureFont, uint16_t characterCellsX, int16_t dstOffsetY, const ::llc::n2u0_t & sizeCharCell, ::llc::vcst_c & text0, const ::llc::n2s1_t dstTextOffset)	{	// --- This function will draw some coloured symbols in each cell of the ASCII screen.
		if(0 == text0.size())
			rtrn 0;
		if_fail_fe(::llc::textFontMetricsValidate(viewTextureFont.metrics(), sizeCharCell, characterCellsX));
		for(int32_t iChar = 0, charCount = (int32_t)text0.size(); iChar < charCount; ++iChar) {
			const uc_t					charToDraw						= (uc_t)text0[iChar];
			int16_t						coordTableX						= charToDraw % characterCellsX;
			int16_t						coordTableY						= charToDraw / characterCellsX;
			const ::llc::n2s1_t			coordCharTable					= {int16_t(coordTableX * sizeCharCell.x), int16_t(coordTableY * sizeCharCell.y)};
			const ::llc::rect2s1_t		dstRect0						= {{int16_t(sizeCharCell.x * iChar), dstOffsetY}, sizeCharCell.s1()};
			const ::llc::rect2s1_t		srcRect0						= {coordCharTable, sizeCharCell.s1()};
			es_if(failed(::llc::grid_copy_alpha(target, viewTextureFont, ::llc::rect2s1_t{dstTextOffset + dstRect0.Offset, dstRect0.Size}, srcRect0.Offset, {0xFF, 0x00, 0xFF, 0xFF})));
			//es_if(failed(::llc::grid_copy(target, viewTextureFont, dstTextOffset + dstOffset1, srcRect0)));
		}
		return 0;
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	tplt<tpnm _tColor>
	static	::llc::error_t	textLineDrawFixedSizeLit		(::llc::grid<_tColor> & target, const ::llc::view_bit<uint64_t> & viewTextureFont, const ::llc::n2u2_t & viewMetrics, uint16_t characterCellsX, int16_t dstOffsetY, const ::llc::n2u0_t & sizeCharCell, const ::llc::vcst_t & text0, const ::llc::n2s1_t dstTextOffset, const _tColor& color)	{	// --- This function will draw some coloured symbols in each cell of the ASCII screen.
		if(0 == text0.size())
			rtrn 0;
		if_fail_fe(::llc::textFontMetricsValidate(viewMetrics, sizeCharCell, characterCellsX));
		if_true_fef(::llc::u3_t(viewMetrics.x) * viewMetrics.y > viewTextureFont.size(), "Font bit count:%u, required:%" LLC_FMT_U3 ".", viewTextureFont.size(), ::llc::u3_t(viewMetrics.x) * viewMetrics.y);
		::llc::apod<::llc::n2u1_t>	dstCoords;
		for(int32_t iChar = 0, charCount = (int32_t)text0.size(); iChar < charCount; ++iChar) {
			const uc_t					charToDraw						= (uc_t)text0[iChar];
			const int16_t				coordTableX						= charToDraw % characterCellsX;
			const int16_t				coordTableY						= charToDraw / characterCellsX;
			const ::llc::n2s1_t			coordCharTable					= {int16_t(coordTableX * sizeCharCell.x), int16_t(coordTableY * sizeCharCell.y)};
			const ::llc::rect2s1_t		dstRect0						= {{int16_t(sizeCharCell.x * iChar), dstOffsetY}, sizeCharCell.s1()};
			const ::llc::rect2s1_t		srcRect0						= {coordCharTable, sizeCharCell.s1()};
			//es_if(failed(::llc::grid_copy_alpha_bit(target, viewTextureFont, dstTextOffset + dstOffset1, viewMetrics, color, srcRect0)));
			es_if(failed(::llc::grid_raster_alpha_bit(target.metrics(), viewTextureFont, viewMetrics, ::llc::rect2s1_t{dstTextOffset + dstRect0.Offset, sizeCharCell.s1()}, srcRect0.Offset, dstCoords)));
			//es_if(failed(::llc::grid_raster_alpha_bit(target, viewTextureFont, dstTextOffset + dstOffset1, viewMetrics, srcRect0, dstCoords)));
		}
		for(uint32_t iCoord = 0; iCoord < dstCoords.size(); ++iCoord)
			::llc::drawPixelLight(target, dstCoords[iCoord], color, 0.05f, 0.75);
		return 0;
	}

	tplt<tpnm _tColor>
	static	::llc::error_t	textLineDrawAlignedFixedSize	(::llc::grid<_tColor> & targetView, const ::llc::grid<_tColor> & fontAtlas, uint16_t lineOffset, const ::llc::n2u1_t & targetSize, const ::llc::n2u0_t & sizeCharCell, const ::llc::vcst_t & text0)	{	// --- This function will draw some coloured symbols in each cell of the ASCII screen.
		const ::llc::n2s1_t			dstTextOffset					= {int16_t((targetSize.x >> 1) - (::llc::textLineCalcWidth(sizeCharCell, text0) >> 1)), };
		uint16_t					dstOffsetY						= int16_t(lineOffset * sizeCharCell.y);
		return ::llc::textLineDrawFixedSize(targetView, fontAtlas, 32, dstOffsetY, sizeCharCell, text0, dstTextOffset);
	}

	tplt<tpnm _tColor>
	static	::llc::error_t	textLineDrawAlignedFixedSizeLit	(::llc::grid<_tColor> & targetView, const ::llc::view_bit<uint64_t> & fontAtlas, const ::llc::n2u2_t & viewMetrics, uint16_t lineOffset, const ::llc::n2u1_t & targetSize, const ::llc::n2u0_t & sizeCharCell, const ::llc::vcst_t & text0, const _tColor & color)	{	// --- This function will draw some coloured symbols in each cell of the ASCII screen.
		const ::llc::n2s1_t			dstTextOffset					= {int16_t((targetSize.x >> 1) - (::llc::textLineCalcWidth(sizeCharCell, text0) >> 1)), };
		uint16_t					dstOffsetY						= int16_t(lineOffset * sizeCharCell.y);
		return ::llc::textLineDrawFixedSizeLit(targetView, fontAtlas, viewMetrics, 32, dstOffsetY, sizeCharCell, text0, dstTextOffset, color);
	}

	tplt<tpnm _tColor>
	static	::llc::error_t	textLineRaster					(const ::llc::n2u2_t & targetMetrics, const ::llc::n2u0_t & fontCharSize, const ::llc::rect2<int16_t> & targetRect, const ::llc::imgmono<_tColor> & fontTexture, ::llc::vcst_c & textToRaster, ::llc::apod<::llc::n2u1_t> & dstCoords)		{
		if(0 == textToRaster.size())
			rtrn 0;
		cnst ::llc::n2u2_t		fontMetrics						= fontTexture.metrics();
		cnst ::llc::u2_t		charsPerRow						= fontCharSize.x ? fontTexture.Pitch / fontCharSize.x : 0;
		if_fail_fe(::llc::textFontMetricsValidate(fontMetrics, fontCharSize, charsPerRow));
		if_true_fef(targetMetrics.x > 0x10000U || targetMetrics.y > 0x10000U, "Target metrics:{%u, %u} exceed 16-bit coordinate output.", targetMetrics.x, targetMetrics.y);
		for(uint32_t iChar = 0, countChars = textToRaster.size(); iChar < countChars; ++iChar) {
			const uc_t					charToDraw						= (uc_t)textToRaster[iChar];
			const int32_t				coordTableX						= charToDraw % charsPerRow;
			const int32_t				coordTableY						= charToDraw / charsPerRow;
			const ::llc::n2s2_t			coordCharTable					= {coordTableX * fontCharSize.x, coordTableY * fontCharSize.y};
			const ::llc::rect2s2_t		srcRect0						= {coordCharTable, fontCharSize.s2()};
			::llc::rect2s2_t			dstRect1						= {{int32_t(targetRect.Offset.x + fontCharSize.x * iChar), targetRect.Offset.y}, fontCharSize.s2()};
			dstRect1.Size				+= {::llc::min(0, dstRect1.Offset.x), ::llc::min(0, dstRect1.Offset.y)};
			es_if(failed(::llc::grid_raster_alpha_bit(targetMetrics, fontTexture.View, fontMetrics, dstRect1, srcRect0.Offset, dstCoords)));
		}
		return 0;
	}
} // namespace

#endif // LLC_GUI_TEXT_H_23627
