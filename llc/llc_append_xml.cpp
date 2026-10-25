#include "llc_append_xml.h"

llc::err_t	llc::appendXmlOpenTag		(asc_t & output, vcst_t tagName) 							{ return append_strings(output, '<', tagName); }
llc::err_t	llc::appendXmlTagClosing	(asc_t & output, vcst_t tagName) 							{ return append_strings(output, "</", tagName, '>'); }
llc::err_t	llc::appendXmlTagOpening	(asc_t & output, vcst_t tagName, vcst_t tagAttributes)	{
	return tagAttributes.size()
		? append_strings(output, '<', tagName, ' ', tagAttributes, " >")
		: append_strings(output, '<', tagName, '>')
		;
}
llc::err_t	llc::appendXmlTagVoid		(asc_t & output, vcst_t tagName, vcst_t tagAttributes) {
	return tagAttributes.size()
		? append_strings(output, '<', tagName, ' ', tagAttributes, " >")
		: append_strings(output, '<', tagName, " />")
		;
}
llc::err_t	llc::appendXmlTag			(asc_t & output, vcst_t tagName, vcst_t tagAttributes, vcst_t innerHtml) {
	return tagAttributes.size()
		? append_strings(output, '<', tagName, ' ', tagAttributes, " >", innerHtml, "</", tagName, '>')
		: append_strings(output, '<', tagName, '>', innerHtml, "</", tagName, '>')
		;
}
llc::err_t	llc::appendXmlAttr			(asc_t & output, vcst_t attrName, vcst_t attrValue)	{
	return attrValue.size()
		? append_strings(output, " ", attrName, "=\"", attrValue, '"')
		: append_strings(output, " ", attrName)
		;
}
llc::err_t	llc::appendXmlTag			(asc_t & output, vcst_t tagName, vcst_t tagAttributes, const FAppend & funcAppend) {
	int32_t			result						= 0;

					{ int32_t _result; if_fail_fe(_result = appendXmlTagOpening(output, tagName, tagAttributes)); 	result += _result; }
	if(funcAppend)	{ int32_t _result; if_fail_fe(_result = funcAppend(output));									result += _result; }
					{ int32_t _result; if_fail_fe(_result = appendXmlTagClosing(output, tagName)); 					result += _result; }
	return result;
}
llc::err_t	llc::appendXmlTag			(asc_t & output, vcst_t tagName, vcst_t tagAttributes, const function<err_t()> & funcAppend) {
	int32_t			result						= 0;

					{ int32_t _result; if_fail_fe(_result = appendXmlTagOpening(output, tagName, tagAttributes)); 	result += _result; }
	if(funcAppend)	{ int32_t _result; if_fail_fe(_result = funcAppend());											result += _result; }
					{ int32_t _result; if_fail_fe(_result = appendXmlTagClosing(output, tagName)); 					result += _result; }
	return result;
}
llc::err_t	llc::appendXmlTag			(asc_t & output, vcst_t tagName, const function<err_t()> & funcAppendAttrs, const function<err_t()> & funcAppend) {
	int32_t			result						= 0;
	if(!funcAppendAttrs)
		return appendXmlTag(output, tagName, vcst_t{}, funcAppend);

					{ int32_t _result; if_fail_fe(_result = append_strings(output, '<', tagName, ' '));	result += _result; }
					{ int32_t _result; if_fail_fe(_result = funcAppendAttrs());									result += _result; }
					{ int32_t _result; if_fail_fe(_result = output.append_string(" >"));						result += _result; }
	if(funcAppend)	{ int32_t _result; if_fail_fe(_result = funcAppend());										result += _result; }
					{ int32_t _result; if_fail_fe(_result = appendXmlTagClosing(output, tagName));			result += _result; }
	return result;
}
