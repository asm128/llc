#include "llc_append_html.h"

#ifndef LLC_ATMEL
llc::err_t	llc::appendHtmlScripts	(::llc::asc_t & output, llc::vcvcs filenames) { 
	::llc::err_t 		result 				= 0;
	stxp	const char	FMT_ATTR_SRC_JS	[]	= "type=\"text/javascript\" src=\"/%s.js\"";
	for(uint32_t iFile = 0; iFile < filenames.size(); ++iFile) {
		char 				cooked	[128] 		= {};
		if_fail_e(::llc::sprintf_s(cooked, FMT_ATTR_SRC_JS, filenames[iFile].begin()));
		result 		+= ::llc::appendXmlTag(output, "script", cooked);
	}
	return result;
}
llc::err_t	llc::appendHtmlStyles	(::llc::asc_t & output, llc::vcvcs filenames) { 
	::llc::err_t 		result 				= 0;
	stxp	const char	FMT_ATTR_HREF_CSS	[]	= "rel=\"stylesheet\" href=\"/%s.css\"";
	for(uint32_t iFile = 0; iFile < filenames.size(); ++iFile) {
		char 				cooked	[128] 		= {};
		if_fail_e(::llc::sprintf_s(cooked, FMT_ATTR_HREF_CSS, filenames[iFile].begin()));
		result 		+= ::llc::appendXmlTag(output, "link", cooked);
	}
	return result;
}
llc::err_t	llc::appendHtmlHead	(::llc::asc_t & output, ::llc::vcst_t title, ::llc::vcvcs filesCSS, ::llc::vcvcs filesJS) {
	return ::llc::appendXmlTag(output, "head", vcs{}, [&output, title, filesCSS, filesJS]() { 
		return ::llc::appendXmlTag		(output, "title", vcs{}, title)
			+  ::llc::appendHtmlStyles	(output, filesCSS)
			+  ::llc::appendHtmlScripts	(output, filesJS)
			; 
	});
}
llc::err_t	llc::appendHtmlPage	(::llc::asc_t & output, const ::llc::FAppend & funcAppendHead, const ::llc::FAppend & funcAppendBody) {
	return ::llc::appendXmlTag(output, "html", vcs{}, [&output, funcAppendHead, funcAppendBody]() {
		return ::llc::appendXmlTag(output, "head", vcs{}, [&output, funcAppendHead]() { return funcAppendHead(output); })
			+  ::llc::appendXmlTag(output, "body", vcs{}, [&output, funcAppendBody]() { return funcAppendBody(output); })
			;
	});
}
llc::err_t	llc::appendHtmlPage	(::llc::asc_t & output, ::llc::vcst_t title, ::llc::vcvcs filesCSS, ::llc::vcvcs filesJS, const ::llc::FAppend & funcAppendBody, ::llc::vcst_t postScript) {
	return ::llc::appendXmlTag(output, "html", vcs{}, [&output, &title, &filesCSS, &filesJS, &funcAppendBody, &postScript]() {
		return ::llc::appendHtmlHead(output, title, filesCSS, filesJS)
			+  ::llc::appendXmlTag(output, "body", vcs{}, [&output, funcAppendBody]() { return funcAppendBody(output); })
			+  ((0 == postScript.size()) ? 0 : ::llc::appendXmlTag(output, "script", vcs{}, postScript))
			;
	});
}
llc::err_t	llc::appendHtmlPage	(::llc::asc_t & output, const ::llc::FAppend & funcAppendCSS, const ::llc::FAppend & funcAppendJS, const ::llc::FAppend & funcAppendBody) {
	return ::llc::appendXmlTag(output, "html", vcs{}, [&output, funcAppendBody, funcAppendCSS, funcAppendJS]() {
		return ::llc::appendXmlTag(output, "head", vcs{}, [&output, funcAppendCSS, funcAppendJS]() {
			return ::llc::appendXmlTag(output, "style" , vcs{}, [&output, funcAppendCSS]() { return funcAppendCSS ? funcAppendCSS(output) : 0; })
				+  ::llc::appendXmlTag(output, "script", vcs{}, [&output, funcAppendJS ]() { return funcAppendJS  ? funcAppendJS (output) : 0; })
				;
		}) + ::llc::appendXmlTag(output, "body", vcs{}, [&output, funcAppendBody]() { return funcAppendBody(output); });
	});
}
#endif
