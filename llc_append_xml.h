#include "llc_string_compose.h"

#ifndef LLC_APPEND_XML_H
#define LLC_APPEND_XML_H

namespace llc
{
	llc::err_t	appendXmlAttr		(::llc::asc_t & output, ::llc::vcst_t attrName, ::llc::vcst_t attrValue = {});
	llc::err_t	appendXmlOpenTag	(::llc::asc_t & output, ::llc::vcst_t tagName);
	llc::err_t	appendXmlTagVoid	(::llc::asc_t & output, ::llc::vcst_t tagName, ::llc::vcst_t tagAttributes = {});
	llc::err_t	appendXmlTagOpening	(::llc::asc_t & output, ::llc::vcst_t tagName, ::llc::vcst_t tagAttributes = {});
	llc::err_t	appendXmlTagClosing	(::llc::asc_t & output, ::llc::vcst_t tagName);
	llc::err_t	appendXmlTag		(::llc::asc_t & output, ::llc::vcst_t tagName, ::llc::vcst_t tagAttributes = {}, ::llc::vcst_t innerHtml = {});
	llc::err_t	appendXmlTag		(::llc::asc_t & output, ::llc::vcst_t tagName, ::llc::vcst_t tagAttributes, const FAppend & funcAppend);
	llc::err_t	appendXmlTag		(::llc::asc_t & output, ::llc::vcst_t tagName, ::llc::vcst_t tagAttributes, const ::llc::function<llc::err_t()> & funcAppend);
	llc::err_t	appendXmlTag		(::llc::asc_t & output, ::llc::vcst_t tagName, const ::llc::function<llc::err_t()> & funcAppendAttributes, const ::llc::function<llc::err_t()> & funcAppend);

} // namespace 

#endif // LLC_APPEND_XML_H