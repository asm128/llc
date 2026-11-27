#include "llc_string_compose.h"
#include "llc_append_css.h"

llc::err_t	llc::appendCSSBorderWidth			(asc_t & output, u0_t borderWidth, bool absoluteSize)	{
	char				cssBuffer	[32]		= {};
	stxp char		cssFormat	[]			= "border-width:%" LLC_FMT_U2 "%s;";
	const char			* unit					= absoluteSize ? "px" : "%";
	const int32_t		len						= sprintf_s(cssBuffer, cssFormat, borderWidth, unit);
	if_fail_fe(append_string(output, cssBuffer));
	return len;
}
llc::err_t	llc::appendCSSControlWidth			(asc_t & output, u1_t outerLimit, bool absoluteSize)	{
	char				cssBuffer	[24]		= {};
	stxp char		cssFormat	[]			= "width:%" LLC_FMT_U2 "%s;";
	const char			* unit					= absoluteSize ? "px" : "%";
	const int32_t 		len						= sprintf_s(cssBuffer, cssFormat, outerLimit, unit);
	if_fail_fe(append_string(output, cssBuffer));
	return len;
}
llc::err_t	llc::appendCSSControlHeight		(asc_t & output, u1_t outerLimit, bool absoluteSize)	{
	char				cssBuffer	[24]		= {};
	stxp char		cssFormat	[]			= "height:%" LLC_FMT_U2 "%s;";
	const char			* unit					= absoluteSize ? "px" : "%";
	const int32_t 		len						= sprintf_s(cssBuffer, cssFormat, outerLimit, unit);
	if_fail_fe(append_string(output, cssBuffer));
	return len;
}
llc::err_t	llc::appendCSSSizeWithBorderWidthRelative	(asc_t & output, n2u1_t outerLimit, u0_t borderWidth)	{
	//const n2u1_t	contentLimit 			= llc::n2s2_t{outerLimit.x - borderWidth * 2, outerLimit.y - borderWidth * 2}.u1();
	char				cssBuffer	[128]		= {};
	stxp char		cssFormat	[]			= "width:calc(%" LLC_FMT_U2 "%% - %" LLC_FMT_U2 "px);height:calc(%" LLC_FMT_U2 "%% - %" LLC_FMT_U2 "px);";
	const int32_t 		len						= sprintf_s(cssBuffer, cssFormat, outerLimit.x, borderWidth * 2, outerLimit.y, borderWidth * 2);
	if_fail_fe(append_string(output, cssBuffer));
	return len + appendCSSBorderWidth(output, borderWidth);
}
llc::err_t	llc::appendCSSSizeWithBorderWidth	(asc_t & output, n2u1_t outerLimit, u0_t borderWidth, bool absoluteSize)	{
	return absoluteSize 
		? appendCSSSizeWithBorderWidthAbsolute(output, outerLimit, borderWidth)
		: appendCSSSizeWithBorderWidthRelative(output, outerLimit, borderWidth)
		;
}
