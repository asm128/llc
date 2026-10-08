#include "llc_append_html.h"

#ifndef LLC_ATMEL
llc::err_t	llc::appendHtmlScripts	(::llc::string & output, llc::vcvcs filenames) {
	::llc::err_t 		result 				= 0;
	for(uint32_t iFile = 0; iFile < filenames.size(); ++iFile) {
		::llc::string		cooked			= {};
		if_fail_fe(::llc::append_strings(cooked, "type=\"text/javascript\" src=\"/", filenames[iFile], ".js\""));
		result 		+= ::llc::appendXmlTag(output, "script", cooked);
	}
	return result;
}
llc::err_t	llc::appendHtmlStyles	(::llc::string & output, llc::vcvcs filenames) {
	::llc::err_t 		result 				= 0;
	for(uint32_t iFile = 0; iFile < filenames.size(); ++iFile) {
		::llc::string		cooked			= {};
		if_fail_fe(::llc::append_strings(cooked, "rel=\"stylesheet\" href=\"/", filenames[iFile], ".css\""));
		result 		+= ::llc::appendXmlTag(output, "link", cooked);
	}
	return result;
}
llc::err_t	llc::appendHtmlHead	(::llc::string & output, ::llc::vcst_t title, ::llc::vcvcs filesCSS, ::llc::vcvcs filesJS) {
	return ::llc::appendXmlTag(output, "head", vcs{}, [&output, title, filesCSS, filesJS]() { 
		return ::llc::appendXmlTag		(output, "title", vcs{}, title)
			+  ::llc::appendHtmlStyles	(output, filesCSS)
			+  ::llc::appendHtmlScripts	(output, filesJS)
			; 
	});
}
llc::err_t	llc::appendHtmlPage	(::llc::string & output, const ::llc::FAppend & funcAppendHead, const ::llc::FAppend & funcAppendBody) {
	return ::llc::appendXmlTag(output, "html", vcs{}, [&output, funcAppendHead, funcAppendBody]() {
		return ::llc::appendXmlTag(output, "head", vcs{}, [&output, funcAppendHead]() { return funcAppendHead(output); })
			+  ::llc::appendXmlTag(output, "body", vcs{}, [&output, funcAppendBody]() { return funcAppendBody(output); })
			;
	});
}
llc::err_t	llc::appendHtmlPage	(::llc::string & output, ::llc::vcst_t title, ::llc::vcvcs filesCSS, ::llc::vcvcs filesJS, const ::llc::FAppend & funcAppendBody, ::llc::vcst_t postScript) {
	return ::llc::appendXmlTag(output, "html", vcs{}, [&output, &title, &filesCSS, &filesJS, &funcAppendBody, &postScript]() {
		return ::llc::appendHtmlHead(output, title, filesCSS, filesJS)
			+  ::llc::appendXmlTag(output, "body", vcs{}, [&output, funcAppendBody]() { return funcAppendBody(output); })
			+  ((0 == postScript.size()) ? 0 : ::llc::appendXmlTag(output, "script", vcs{}, postScript))
			;
	});
}
llc::err_t	llc::appendHtmlPage	(::llc::string & output, const ::llc::FAppend & funcAppendCSS, const ::llc::FAppend & funcAppendJS, const ::llc::FAppend & funcAppendBody) {
	return ::llc::appendXmlTag(output, "html", vcs{}, [&output, funcAppendBody, funcAppendCSS, funcAppendJS]() {
		return ::llc::appendXmlTag(output, "head", vcs{}, [&output, funcAppendCSS, funcAppendJS]() {
			return ::llc::appendXmlTag(output, "style" , vcs{}, [&output, funcAppendCSS]() { return funcAppendCSS ? funcAppendCSS(output) : 0; })
				+  ::llc::appendXmlTag(output, "script", vcs{}, [&output, funcAppendJS ]() { return funcAppendJS  ? funcAppendJS (output) : 0; })
				;
		}) + ::llc::appendXmlTag(output, "body", vcs{}, [&output, funcAppendBody]() { return funcAppendBody(output); });
	});
}
#endif
