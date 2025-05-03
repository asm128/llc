#include "llc_array.h"

#ifndef LLC_PATH_H_23627
#define LLC_PATH_H_23627

namespace llc
{
	stct SPathContents {
		::llc::aasc_t				Files					= {};
		::llc::aobj<SPathContents>	Folders					= {};
	};

	err_t						pathCreate				(::llc::vcst_c & folderName, sc_c separator = '/');	// Recursive
	err_t						pathList				(::llc::vcst_c & pathToList, ::llc::aasc_t & output, bool listFolders, ::llc::vcst_c extension = {});		// Not recursive
	err_t						pathList				(const ::llc::SPathContents & input, ::llc::aasc_t	& output, ::llc::vcst_c extension = {});	// recursively walk over a pathcontents hierarchy and store all the file names into "output"
	err_t						pathList				(const ::llc::SPathContents & input, ::llc::avcsc_t	& output, ::llc::vcst_c extension = {});	// recursively walk over a pathcontents hierarchy and store all the file names into "output"
	err_t						pathList				(::llc::vcst_c & pathToList, SPathContents & output, ::llc::vcst_c extension = {});		// Recursive
	stin	err_t				pathList				(::llc::vcst_c & pathToList, ::llc::aachar & output, ::llc::vcst_c extension = {}) {
		::llc::SPathContents			tree					= {};
		s2_t							error					= ::llc::pathList(pathToList, tree, extension);
		llc_necs(error |= ::llc::pathList(tree, output, extension));
		return 0;
	}	// Recursive
	// this function was ceated in order to work around the problem of the JSON system returning pointers to the original string, without having the opportunity of processing escaped path slashes.
	//err_t						pathNameCompose			(::llc::asc_t & out_composed, ::llc::vcst_t fileName, ::llc::vcst_t path = {}, ::llc::vcst_t extension = {});
	err_t						pathNameCompose			(::llc::vcsc_c & path, ::llc::vcsc_c & fileName, ::llc::asc_t & out_composed);
	err_t						findLastSlash			(::llc::vcsc_c & path);

	err_t						pathList				(::llc::vcst_c & pathToList, ::llc::SPathContents & outputTree, ::llc::function<err_t(bool, vcst_c&)> onItem, llc::vcst_c extension);
} // namespace

#endif // LLC_PATH_H_23627
