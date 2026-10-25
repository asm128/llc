#include "llc_append_html.h"

#ifndef LLC_ATMEL
llc::err_t	llc::appendHtmlScripts	(asc_t & output, llc::vcvcst_t filenames) {
	err_t 		result 				= 0;
	stxp	const char	FMT_ATTR_SRC_JS	[]	= "type=\"text/javascript\" src=\"/%s.js\"";
	for(uint32_t iFile = 0; iFile < filenames.size(); ++iFile) {
		char 				cooked	[128] 		= {};
		if_fail_e(sprintf_s(cooked, FMT_ATTR_SRC_JS, filenames[iFile].begin()));
		result 		+= appendXmlTag(output, "script", cooked);
	}
	return result;
}
llc::err_t	llc::appendHtmlStyles	(asc_t & output, llc::vcvcst_t filenames) {
	err_t 		result 				= 0;
	stxp	const char	FMT_ATTR_HREF_CSS	[]	= "rel=\"stylesheet\" href=\"/%s.css\"";
	for(uint32_t iFile = 0; iFile < filenames.size(); ++iFile) {
		char 				cooked	[128] 		= {};
		if_fail_e(sprintf_s(cooked, FMT_ATTR_HREF_CSS, filenames[iFile].begin()));
		result 		+= appendXmlTag(output, "link", cooked);
	}
	return result;
}
llc::err_t	llc::appendHtmlHead	(asc_t & output, vcst_t title, vcvcst_t filesCSS, vcvcst_t filesJS) {
	return appendXmlTag(output, "head", vcs{}, [&output, title, filesCSS, filesJS]() {
		return appendXmlTag		(output, "title", vcs{}, title)
			+  appendHtmlStyles	(output, filesCSS)
			+  appendHtmlScripts	(output, filesJS)
			;
	});
}
llc::err_t	llc::appendHtmlPage	(asc_t & output, const FAppend & funcAppendHead, const FAppend & funcAppendBody) {
	return appendXmlTag(output, "html", vcs{}, [&output, funcAppendHead, funcAppendBody]() {
		return appendXmlTag(output, "head", vcs{}, [&output, funcAppendHead]() { return funcAppendHead(output); })
			+  appendXmlTag(output, "body", vcs{}, [&output, funcAppendBody]() { return funcAppendBody(output); })
			;
	});
}
llc::err_t	llc::appendHtmlPage	(asc_t & output, vcst_t title, vcvcst_t filesCSS, vcvcst_t filesJS, const FAppend & funcAppendBody, vcst_t postScript) {
	return appendXmlTag(output, "html", vcs{}, [&output, &title, &filesCSS, &filesJS, &funcAppendBody, &postScript]() {
		return appendHtmlHead(output, title, filesCSS, filesJS)
			+  appendXmlTag(output, "body", vcs{}, [&output, funcAppendBody]() { return funcAppendBody(output); })
			+  ((0 == postScript.size()) ? 0 : appendXmlTag(output, "script", vcs{}, postScript))
			;
	});
}
llc::err_t	llc::appendHtmlPage	(asc_t & output, const FAppend & funcAppendCSS, const FAppend & funcAppendJS, const FAppend & funcAppendBody) {
	return appendXmlTag(output, "html", vcs{}, [&output, funcAppendBody, funcAppendCSS, funcAppendJS]() {
		return appendXmlTag(output, "head", vcs{}, [&output, funcAppendCSS, funcAppendJS]() {
			return appendXmlTag(output, "style" , vcs{}, [&output, funcAppendCSS]() { return funcAppendCSS ? funcAppendCSS(output) : 0; })
				+  appendXmlTag(output, "script", vcs{}, [&output, funcAppendJS ]() { return funcAppendJS  ? funcAppendJS (output) : 0; })
				;
		}) + appendXmlTag(output, "body", vcs{}, [&output, funcAppendBody]() { return funcAppendBody(output); });
	});
}
#endif
