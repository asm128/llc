#include "llc_append_xml.h"

#ifndef LLC_APPEND_HTML_H
#define LLC_APPEND_HTML_H

namespace llc
{
	stin	llc::err_t	appendHtmlHead		(::llc::asc_t & output, ::llc::vcst_t tagAttributes, ::llc::vcst_t innerHtml)	{ return ::llc::appendXmlTag(output, "head", tagAttributes, innerHtml); }
	stin	llc::err_t	appendHtmlBody		(::llc::asc_t & output, ::llc::vcst_t tagAttributes, ::llc::vcst_t innerHtml)	{ return ::llc::appendXmlTag(output, "body", tagAttributes, innerHtml); }
	stin	llc::err_t	appendHtmlScript	(::llc::asc_t & output, ::llc::vcst_t tagAttributes, ::llc::vcst_t innerHtml)	{ return ::llc::appendXmlTag(output, "script", tagAttributes, innerHtml); }
	stin	llc::err_t	appendHtmlTable		(::llc::asc_t & output, ::llc::vcst_t tagAttributes, ::llc::vcst_t innerHtml)	{ return ::llc::appendXmlTag(output, "table", tagAttributes, innerHtml); }
	stin	llc::err_t	appendHtmlTableRow	(::llc::asc_t & output, ::llc::vcst_t tagAttributes, ::llc::vcst_t innerHtml)	{ return ::llc::appendXmlTag(output, "tr", tagAttributes, innerHtml); }
	stin	llc::err_t	appendHtmlTableCol	(::llc::asc_t & output, ::llc::vcst_t tagAttributes, ::llc::vcst_t innerHtml)	{ return ::llc::appendXmlTag(output, "td", tagAttributes, innerHtml); }
	
	llc::err_t			appendHtmlStyles	(::llc::asc_t & output, llc::vcvcs filenames);
	llc::err_t			appendHtmlScripts	(::llc::asc_t & output, llc::vcvcs filenames);
	llc::err_t			appendHtmlHead		(::llc::asc_t & output, ::llc::vcst_t title, ::llc::vcvcs filesCSS, ::llc::vcvcs filesJS);
	llc::err_t			appendHtmlPage		(::llc::asc_t & output, const ::llc::FAppend & funcAppendHead, const ::llc::FAppend & funcAppendBody);
	llc::err_t			appendHtmlPage		(::llc::asc_t & output, const ::llc::FAppend & funcAppendCSS, const ::llc::FAppend & funcAppendJS, const ::llc::FAppend & funcAppendBody);
	llc::err_t			appendHtmlPage		(::llc::asc_t & output, ::llc::vcst_t title, ::llc::vcvcs filesCSS, ::llc::vcvcs filesJS, const ::llc::FAppend & funcAppendBody, ::llc::vcst_t postScript = {});
} // namespace 

#endif // LLC_APPEND_HTML_H
