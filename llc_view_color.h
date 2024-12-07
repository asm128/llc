#include "llc_color.h"
#include "llc_view.h"

#ifndef LLC_VIEW_COLOR_H_23627
#define LLC_VIEW_COLOR_H_23627

namespace llc
{
	tydf	::llc::view<::llc::frgba	>		vrgbaf	, vfrgba	;
	tydf	::llc::view<::llc::u8bgra	>		vbgra	, v8bgra	;
	tydf	::llc::view<::llc::u8bgr	>		vbgr	, v8bgr		;
	tydf	::llc::view<::llc::u8rgba	>		vrgba	, v8rgba	;
	tydf	::llc::view<::llc::u8rgb	>		vrgb	, v8rgb		;

	tydf	::llc::view<const ::llc::frgba	>	vcrgbaf	, vcfrgba	;
	tydf	::llc::view<const ::llc::u8bgra	>	vcbgra	, vc8bgra	;
	tydf	::llc::view<const ::llc::u8bgr	>	vcbgr	, vc8bgr		;
	tydf	::llc::view<const ::llc::u8rgba	>	vcrgba	, vc8rgba	;
	tydf	::llc::view<const ::llc::u8rgb	>	vcrgb	, vc8rgb		;

	::llc::error_t 		vcu82bgra8	(vcu0_c & bytes, ::llc::bgra8 & colorToSet);
	::llc::error_t 		vcu82bgr8	(vcu0_c & bytes, ::llc::bgr8 & colorToSet);
} // namespace

#endif // LLC_VIEW_COLOR_H_23627
