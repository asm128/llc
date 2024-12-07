#include "llc_append_css.h"

llc::err_t	llc::appendCSSBorderWidth			(::llc::asc_t & output, uint8_t borderWidth, bool absoluteSize)	{
	char				cssBuffer	[32]		= {};
	stxp char		cssFormat	[]			= "border-width:%" LLC_FMT_U2 "%s;";
	const char			* unit					= absoluteSize ? "px" : "%";
	const int32_t		len						= ::llc::sprintf_s(cssBuffer, cssFormat, borderWidth, unit);
	llc_necs(output.append_string(cssBuffer));
	return len;
}
llc::err_t	llc::appendCSSControlWidth			(::llc::asc_t & output, uint16_t outerLimit, bool absoluteSize)	{
	char				cssBuffer	[24]		= {};
	stxp char		cssFormat	[]			= "width:%" LLC_FMT_U2 "%s;";
	const char			* unit					= absoluteSize ? "px" : "%";
	const int32_t 		len						= ::llc::sprintf_s(cssBuffer, cssFormat, outerLimit, unit);
	llc_necs(output.append_string(cssBuffer));
	return len;
}
llc::err_t	llc::appendCSSControlHeight		(::llc::asc_t & output, uint16_t outerLimit, bool absoluteSize)	{
	char				cssBuffer	[24]		= {};
	stxp char		cssFormat	[]			= "height:%" LLC_FMT_U2 "%s;";
	const char			* unit					= absoluteSize ? "px" : "%";
	const int32_t 		len						= ::llc::sprintf_s(cssBuffer, cssFormat, outerLimit, unit);
	llc_necs(output.append_string(cssBuffer));
	return len;
}
llc::err_t	llc::appendCSSControlSize			(::llc::asc_t & output, ::llc::n2u1_t outerLimit, bool absoluteSize)	{
	return appendCSSControlWidth(output, outerLimit.x, absoluteSize) + appendCSSControlHeight(output, outerLimit.y, absoluteSize);
}
llc::err_t	llc::appendCSSSizeWithBorderWidthAbsolute	(::llc::asc_t & output, ::llc::n2u1_t outerLimit, uint8_t borderWidth)	{
	return appendCSSControlSize(output, ::llc::n2s2_t{outerLimit.x - borderWidth * 2, outerLimit.y - borderWidth * 2}.u1(), true) + appendCSSBorderWidth(output, borderWidth);
}
llc::err_t	llc::appendCSSSizeWithBorderWidthRelative	(::llc::asc_t & output, ::llc::n2u1_t outerLimit, uint8_t borderWidth)	{
	//const ::llc::n2u1_t	contentLimit 			= llc::n2s2_t{outerLimit.x - borderWidth * 2, outerLimit.y - borderWidth * 2}.u1();
	char				cssBuffer	[128]		= {};
	stxp char		cssFormat	[]			= "width:calc(%" LLC_FMT_U2 "%% - %" LLC_FMT_U2 "px);height:calc(%" LLC_FMT_U2 "%% - %" LLC_FMT_U2 "px);";
	const int32_t 		len						= ::llc::sprintf_s(cssBuffer, cssFormat, outerLimit.x, borderWidth * 2, outerLimit.y, borderWidth * 2);
	llc_necs(output.append_string(cssBuffer));
	return len + appendCSSBorderWidth(output, borderWidth);
}
llc::err_t	llc::appendCSSSizeWithBorderWidth	(::llc::asc_t & output, ::llc::n2u1_t outerLimit, uint8_t borderWidth, bool absoluteSize)	{
	return absoluteSize 
		? appendCSSSizeWithBorderWidthAbsolute(output, outerLimit, borderWidth)
		: appendCSSSizeWithBorderWidthRelative(output, outerLimit, borderWidth)
		;
}
