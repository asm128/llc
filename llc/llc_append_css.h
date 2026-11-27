#include "llc_string_compose.h"
#include "llc_n2.h"

#ifndef LLC_APPEND_CSS_H
#define LLC_APPEND_CSS_H

namespace llc
{
	tpl_vtArgs
			err_t	appendCSSTag							(asc_t & output, vcst_t tagName, _tArgs&&... cssAttributes)					{ return append_strings(output, tagName, '{', cssAttributes..., '}', '\n'); }
			err_t	appendCSSBorderWidth					(asc_t & output, u0_t borderWidth, b8_t absoluteSize = true);
			err_t	appendCSSControlWidth					(asc_t & output, u1_t outerLimit, b8_t absoluteSize = false);
			err_t	appendCSSControlHeight					(asc_t & output, u1_t outerLimit, b8_t absoluteSize = false);
			err_t	appendCSSSizeWithBorderWidthRelative	(asc_t & output, n2u1_t outerLimit, u0_t borderWidth);
			err_t	appendCSSSizeWithBorderWidth			(asc_t & output, n2u1_t outerLimit, u0_t borderWidth, b8_t absoluteSize = false);
	stin	err_t	appendCSSControlSize					(asc_t & output, n2u1_t outerLimit, b8_t absoluteSize)	{
		return appendCSSControlWidth(output, outerLimit.x, absoluteSize) + appendCSSControlHeight(output, outerLimit.y, absoluteSize);
	}
	stin	err_t	appendCSSSizeWithBorderWidthAbsolute	(asc_t & output, n2u1_t outerLimit, u0_t borderWidth)	{
		return appendCSSControlSize(output, n2s2_t{outerLimit.x - borderWidth * 2, outerLimit.y - borderWidth * 2}.u1(), true) + appendCSSBorderWidth(output, borderWidth);
	}
} // namespace 

#endif // LLC_APPEND_CSS_H
