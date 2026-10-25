#include "llc_append_xml.h"

#ifndef LLC_APPEND_HTML_H
#define LLC_APPEND_HTML_H

namespace llc
{
	stin	err_t	appendHtmlHead		(asc_t & output, vcst_t tagAttributes, vcst_t innerHtml)	{ return appendXmlTag(output, "head", tagAttributes, innerHtml); }
	stin	err_t	appendHtmlBody		(asc_t & output, vcst_t tagAttributes, vcst_t innerHtml)	{ return appendXmlTag(output, "body", tagAttributes, innerHtml); }
	stin	err_t	appendHtmlScript	(asc_t & output, vcst_t tagAttributes, vcst_t innerHtml)	{ return appendXmlTag(output, "script", tagAttributes, innerHtml); }
	stin	err_t	appendHtmlTable		(asc_t & output, vcst_t tagAttributes, vcst_t innerHtml)	{ return appendXmlTag(output, "table", tagAttributes, innerHtml); }
	stin	err_t	appendHtmlTableRow	(asc_t & output, vcst_t tagAttributes, vcst_t innerHtml)	{ return appendXmlTag(output, "tr", tagAttributes, innerHtml); }
	stin	err_t	appendHtmlTableCol	(asc_t & output, vcst_t tagAttributes, vcst_t innerHtml)	{ return appendXmlTag(output, "td", tagAttributes, innerHtml); }

	err_t			appendHtmlStyles	(asc_t & output, vcvcst_t filenames);
	err_t			appendHtmlScripts	(asc_t & output, vcvcst_t filenames);
	err_t			appendHtmlHead		(asc_t & output, vcst_t title, vcvcst_t filesCSS, vcvcst_t filesJS);
	err_t			appendHtmlPage		(asc_t & output, const FAppend & funcAppendHead, const FAppend & funcAppendBody);
	err_t			appendHtmlPage		(asc_t & output, const FAppend & funcAppendCSS, const FAppend & funcAppendJS, const FAppend & funcAppendBody);
	err_t			appendHtmlPage		(asc_t & output, vcst_t title, vcvcst_t filesCSS, vcvcst_t filesJS, const FAppend & funcAppendBody, vcst_t postScript = {});
} // namespace

#endif // LLC_APPEND_HTML_H
