#include "llc_string_compose.h"
#include "llc_n2.h"

#ifndef LLC_APPEND_CSS_H
#define LLC_APPEND_CSS_H

namespace llc
{
	tpl_vtArgs	err_t	appendCSSTag							(asc_t & output, llc::vcst_t tagName, _tArgs&&... cssAttributes)	{ return append_strings(output, tagName, '{', cssAttributes..., '}', '\n'); }
				err_t	appendCSSBorderWidth					(asc_t & output, uint8_t borderWidth, bool absoluteSize = true);
				err_t	appendCSSControlWidth					(asc_t & output, uint16_t outerLimit, bool absoluteSize = false);
				err_t	appendCSSControlHeight					(asc_t & output, uint16_t outerLimit, bool absoluteSize = false);
				err_t	appendCSSControlSize					(asc_t & output, n2u1_t outerLimit, bool absoluteSize = false);
				err_t	appendCSSSizeWithBorderWidthAbsolute	(asc_t & output, n2u1_t outerLimit, uint8_t borderWidth);
				err_t	appendCSSSizeWithBorderWidthRelative	(asc_t & output, n2u1_t outerLimit, uint8_t borderWidth);
				err_t	appendCSSSizeWithBorderWidth			(asc_t & output, n2u1_t outerLimit, uint8_t borderWidth, bool absoluteSize = false);
} // namespace

#endif // LLC_APPEND_CSS_H