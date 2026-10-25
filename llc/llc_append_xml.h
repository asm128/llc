#include "llc_string_compose.h"

#ifndef LLC_APPEND_XML_H
#define LLC_APPEND_XML_H

namespace llc
{
	err_t	appendXmlAttr		(asc_t & output, vcst_t attrName, vcst_t attrValue = {});
	err_t	appendXmlOpenTag	(asc_t & output, vcst_t tagName);
	err_t	appendXmlTagVoid	(asc_t & output, vcst_t tagName, vcst_t tagAttributes = {});
	err_t	appendXmlTagOpening	(asc_t & output, vcst_t tagName, vcst_t tagAttributes = {});
	err_t	appendXmlTagClosing	(asc_t & output, vcst_t tagName);
	err_t	appendXmlTag		(asc_t & output, vcst_t tagName, vcst_t tagAttributes = {}, vcst_t innerHtml = {});
	err_t	appendXmlTag		(asc_t & output, vcst_t tagName, vcst_t tagAttributes, const FAppend & funcAppend);
	err_t	appendXmlTag		(asc_t & output, vcst_t tagName, vcst_t tagAttributes, const function<err_t()> & funcAppend);
	err_t	appendXmlTag		(asc_t & output, vcst_t tagName, const function<err_t()> & funcAppendAttributes, const function<err_t()> & funcAppend);

} // namespace

#endif // LLC_APPEND_XML_H