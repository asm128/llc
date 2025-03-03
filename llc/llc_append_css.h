#include "llc_string_compose.h"
#include "llc_n2.h"

#ifndef LLC_APPEND_CSS_H
#define LLC_APPEND_CSS_H

namespace llc
{
	tpl_vtArgs	err_t	appendCSSTag							(::llc::asc_t & output, llc::vcst_t tagName, _tArgs&&... cssAttributes)					{ return llc::append_strings(output, tagName, '{', cssAttributes..., '}', '\n'); }	
				err_t	appendCSSBorderWidth					(::llc::asc_t & output, uint8_t borderWidth, bool absoluteSize = true);
				err_t	appendCSSControlWidth					(::llc::asc_t & output, uint16_t outerLimit, bool absoluteSize = false);
				err_t	appendCSSControlHeight					(::llc::asc_t & output, uint16_t outerLimit, bool absoluteSize = false);
				err_t	appendCSSControlSize					(::llc::asc_t & output, ::llc::n2u1_t outerLimit, bool absoluteSize = false);
				err_t	appendCSSSizeWithBorderWidthAbsolute	(::llc::asc_t & output, ::llc::n2u1_t outerLimit, uint8_t borderWidth);
				err_t	appendCSSSizeWithBorderWidthRelative	(::llc::asc_t & output, ::llc::n2u1_t outerLimit, uint8_t borderWidth);
				err_t	appendCSSSizeWithBorderWidth			(::llc::asc_t & output, ::llc::n2u1_t outerLimit, uint8_t borderWidth, bool absoluteSize = false);
} // namespace 

#endif // LLC_APPEND_CSS_H