#include "llc_path.h"
#include "llc_string.h"

#if defined(LLC_WINDOWS)
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif
#	include <Windows.h>
#elif defined(LLC_ANDROID) || defined(LLC_LINUX)
#	include <dirent.h>
#	include <sys/stat.h>
#	include <sys/types.h>
#	include <unistd.h>
#elif !defined(LLC_ATMEL)
//#	include <dirent.h>
#	include <sys/stat.h>
////#	include <sys/types.h>
////#	include <unistd.h>
#endif

#if !defined(LLC_ARDUINO) && !defined(LLC_ATMEL)
#	include <filesystem>
#	include <string>
#endif

stxp	uint32_t LLC_MAX_PATH = 256;

#define llc_path_debug info_printf


//
::llc::err_t			llc::pathCreate				(::llc::vcst_t pathName, sc_c separator) {
	if_zero_fw(pathName.size());
#ifndef LLC_ATMEL
	char						folder[LLC_MAX_PATH]		= {};
	s2_t						offsetBar					= -1;
	do {
		++offsetBar;
		offsetBar				= ::llc::find(separator, pathName, offsetBar);
		if(0 == offsetBar) {
			if(offsetBar == (int32_t)pathName.size() - 1)
				break;
			continue;
		}
		ree_if(0 != strncpy_s(folder, pathName.begin(), (offsetBar < 0) ? pathName.size() : offsetBar), "String buffer overflow? Path size: %" LLC_FMT_U2 ".", pathName.size());
		if(0 == strcmp(".", folder))
			continue;
#if defined(LLC_WINDOWS)
		llc_path_debug("Creating folder \"%s\".", folder);
		if(FALSE == CreateDirectoryA(folder, NULL)) {
			ci_if(strlen(folder) == 2 && folder[1] == ':');
			DWORD						err							= GetLastError();
			ree_if(err != ERROR_ALREADY_EXISTS, "Failed to create directory: %s. hr: (%" LLC_FMT_U2 ")", folder, err);
		}
#else
		struct stat st = {0};
		if (stat(folder, &st) == -1) {
			mkdir(folder, 0700);
		}
#endif
	} while(offsetBar >= 0 && offsetBar != (s2_t)pathName.size() - 1);
#endif
	return 0;
}

::llc::err_t			llc::findLastSlash			(::llc::vcst_t path)		{
	int32_t						indexOfStartOfFileName0		= ::llc::rfind('\\', path);
	int32_t						indexOfStartOfFileName1		= ::llc::rfind('/', path);
	return
		(-1 == indexOfStartOfFileName1) ? indexOfStartOfFileName0 :
		(-1 == indexOfStartOfFileName0) ? indexOfStartOfFileName1 :
		::llc::max(indexOfStartOfFileName0, indexOfStartOfFileName1)
		;
}
::llc::err_t			llc::pathStem				(::llc::vcst_t path, ::llc::vcst_t & stem) {
	::llc::vcst_t filename = {};
	if_fail_fe(::llc::pathFilename(path, filename));
	if_zero_fef(filename.size(), "Path has no filename:'%.*s'.", (int)path.size(), path.begin());
	::llc::u2_t iEnd = filename.size();
	while(1 < iEnd)
		if('.' == filename[--iEnd])
			rtrn filename.slice(stem, 0, iEnd);
	rtrn filename.slice(stem, 0, filename.size());
}

stxp	bool				pathSeparator			(::llc::sc_c value) { rtrn '/' == value || '\\' == value; }
stxp	bool				pathDriveLetter			(::llc::sc_c value) { rtrn ('A' <= value && value <= 'Z') || ('a' <= value && value <= 'z'); }

::llc::err_t			llc::pathBegin				(::llc::vcst_t path, ::llc::vcst_t & output) {
	::llc::u2_t				prefixSize				= 0;
	if(path.size() && ::pathSeparator(path[0])) {
		prefixSize							= 1;
		if(path.size() > 1 && ::pathSeparator(path[1])) {
			prefixSize						= 2;
			while(prefixSize < path.size() && false == ::pathSeparator(path[prefixSize]))
				++prefixSize;
			if(prefixSize < path.size())
				++prefixSize;
			while(prefixSize < path.size() && false == ::pathSeparator(path[prefixSize]))
				++prefixSize;
			if(prefixSize < path.size())
				++prefixSize;
		}
	}
	else {
		::llc::vcst_t			drive					= path;
		if_fail_fe(::llc::split(':', drive));
		if(1 == drive.size() && drive.size() < path.size() && ::pathDriveLetter(drive[0])) {
			prefixSize						= 2;
			if(prefixSize < path.size() && ::pathSeparator(path[prefixSize]))
				++prefixSize;
		}
	}
	if_fail_fe(path.slice(output, 0, prefixSize));
	rtrn output.size();
}

sttc	::llc::err_t	validatePathSeparators	(::llc::vcst_t path, bool allowUNC) {
	for(::llc::u2_t iChar = 1; iChar < path.size(); ++iChar)
		if_true_fef(::pathSeparator(path[iChar - 1]) && ::pathSeparator(path[iChar]) && false == (allowUNC && 1 == iChar)
			, "Adjacent separators at offset %" LLC_FMT_U2 ": '%.*s'."
			, iChar - 1, (int)path.size(), path.begin()
			);
	rtrn 0;
}

sttc	::llc::err_t 	appendPathNormalized		(::llc::vcst_t path, ::llc::string & output) {
	::llc::u2_t			written				= 0;
	for(::llc::u2_t iChar = 0; iChar < path.size(); ++iChar) {
		::llc::sc_t			curChar				= path[iChar];
		if(::pathSeparator(curChar))
			curChar					= '/';
		if_fail_fef(output.push_back(curChar), "output.size()=%" LLC_FMT_U2 ".", output.size());
		++written;
	}
	rtrn written;
}

::llc::err_t			llc::pathNormalize			(::llc::vcst_t path, ::llc::string & output, sc_c separator) {
	if_true_fef(false == ::pathSeparator(separator), "Invalid separator: '%c'.", separator);
	if_fail_fe(::validatePathSeparators(path, true));
	if(0 == path.size())
		rtrn output.clear();

	cnst bool				isUNC					= path.size() > 1 && ::pathSeparator(path[0]) && ::pathSeparator(path[1]);
	cnst bool				hasDrive				= path.size() > 1 && ::pathDriveLetter(path[0]) && ':' == path[1];
	cnst bool				isAbsolute				= isUNC || (!hasDrive && ::pathSeparator(path[0])) || (hasDrive && path.size() > 2 && ::pathSeparator(path[2]));
	::llc::u2_t				iChar					= isUNC ? 2U : hasDrive ? 2U : isAbsolute ? 1U : 0U;
	if(hasDrive && isAbsolute)
		++iChar;

	::llc::aobj<vcst_t>			segments;
	cnst ::llc::u2_t		protectedSegments		= isUNC ? 2U : 0U;
	while(iChar < path.size()) {
		cnst ::llc::u2_t		segmentStart			= iChar;
		while(iChar < path.size() && false == ::pathSeparator(path[iChar]))
			++iChar;
		::llc::vcsc_c			segment					= {path.begin() + segmentStart, iChar - segmentStart};
		if(segment == LLC_CXS(".")) {}
		else if(segment == LLC_CXS("..")) {
			if(segments.size() > protectedSegments && segments[segments.size() - 1] != LLC_CXS("..")) {
				if_fail_fe(segments.pop_back());
			}
			else if(false == isAbsolute) {
				if_fail_fe(segments.push_back(segment));
			}
		}
		else if(segment.size())
			if_fail_fe(segments.push_back(segment));
		if(iChar < path.size())
			++iChar;
	}
	if_true_fef(isUNC && segments.size() < 2, "Incomplete UNC root: '%.*s'.", (int)path.size(), path.begin());

	::llc::asc_t			normalized;
	if(hasDrive)
		if_fail_fe(normalized.append(path.begin(), 2));
	if(isUNC) {
		if_fail_fe(normalized.push_back(separator));
		if_fail_fe(normalized.push_back(separator));
	}
	else if(isAbsolute)
		if_fail_fe(normalized.push_back(separator));
	for(::llc::u2_t iSegment = 0; iSegment < segments.size(); ++iSegment) {
		cnst bool				driveRelativeFirst		= hasDrive && false == isAbsolute && 0 == iSegment;
		if(normalized.size() && normalized[normalized.size() - 1] != separator && false == driveRelativeFirst)
			if_fail_fe(normalized.push_back(separator));
		if_fail_fe(normalized.append(segments[iSegment]));
	}
	if(isUNC && segments.size() == protectedSegments && normalized[normalized.size() - 1] != separator) {
		if_fail_fe(normalized.push_back(separator));
	}
	else if(false == normalized.size() && path.size()) {
		if_fail_fe(normalized.push_back('.'));
	}
	output						= normalized;
	rtrn output.size();
}

::llc::err_t			llc::pathAbsolute			(::llc::vcst_t path, ::llc::string & output, sc_c separator) {
	if_zero_fef(path.size(), "%s", "Empty path.");
	if_true_fef(false == ::pathSeparator(separator), "Invalid separator: '%c'.", separator);
	if_fail_fe(::validatePathSeparators(path, true));
#if !defined(LLC_ARDUINO) && !defined(LLC_ATMEL)
	try {
		cnst ::std::string		input					= {path.begin(), path.size()};
		cnst ::std::string		absolute				= ::std::filesystem::absolute(input).generic_string();
		::llc::vcsc_c			absoluteView			= {absolute.data(), (::llc::u2_t)absolute.size()};
		rtrn ::llc::pathNormalize(absoluteView, output, separator);
	}
	catch(cnst ::std::filesystem::filesystem_error & exception) {
		error_printf("Failed to resolve absolute path '%.*s': %s.", (int)path.size(), path.begin(), exception.what());
		rtrn -1;
	}
#else
	(void)output;
	(void)separator;
	error_printf("Absolute path resolution is unavailable on this platform: '%.*s'.", (int)path.size(), path.begin());
	rtrn -1;
#endif
}
//
::llc::err_t			llc::pathNameCompose		(::llc::vcst_t path, ::llc::vcst_t fileName, ::llc::string & out_composed)		{
	if_fail_fe(::validatePathSeparators(path, true));
	if_fail_fe(::validatePathSeparators(fileName, 0 == path.size()));
	::llc::u2_t				pathLength				= 0;
	if(path.size())
		if_fail_fe(pathLength = ::appendPathNormalized(path, out_composed));
	if(fileName.size()) {
		::llc::u2_t				fileOffset				= 0;
		if(pathLength) {
			while(fileOffset < fileName.size() && ::pathSeparator(fileName[fileOffset]))
				++fileOffset;
			if('/' != out_composed[out_composed.size() - 1] && fileOffset < fileName.size())
				if_fail_fef(out_composed.push_back('/'), "out_composed.size()=%" LLC_FMT_U2 ".", out_composed.size());
		}
		::llc::vcsc_c			fileToAppend				= {fileName.begin() + fileOffset, fileName.size() - fileOffset};
		if_fail_fe(::appendPathNormalized(fileToAppend, out_composed));
	}
	rtrn out_composed.size();
}

::llc::err_t			llc::pathList				(const ::llc::SPathContents & input, ::llc::aobj<vcst_t> & output, ::llc::vcst_c extension)					{
	llc_path_debug("extension=\"%s\"", extension.begin());
	for(uint32_t iFile = 0; iFile < input.Files.size(); ++iFile) {
		::llc::vcsc_c			& fileName					= input.Files[iFile];
		b8_t 					extensionMatch 				= 0 == strncmp(fileName.end() - extension.size(), extension.begin(), ::llc::min(extension.size(), fileName.size()));
		if(0 == extension.size() || (extension.size() < fileName.size() && extensionMatch)) {
			llc_path_debug("fileName=\"%s\"", fileName.begin());
			llc_necs(output.push_back(fileName));
		}
	}
	for(uint32_t iFolder = 0; iFolder < input.Folders.size(); ++iFolder)
		if_fail_fef(llc::pathList(input.Folders[iFolder], output, extension), "%s", "Unknown error!");
	return 0;
}

::llc::err_t			llc::pathList				(const ::llc::SPathContents & input, ::llc::aobj<string> & output, ::llc::vcst_c extension)					{
	llc_path_debug("extension=\"%s\"", extension.begin());
	for(uint32_t iFile = 0; iFile < input.Files.size(); ++iFile) {
		::llc::vcsc_c			& fileName					= input.Files[iFile];
		llc_path_debug("fileName=\"%s\"", fileName.begin());
		b8_t 					extensionMatch 				= 0 == strncmp(fileName.end() - extension.size(), extension.begin(), ::llc::min(extension.size(), fileName.size()));
		if(0 == extension.size() || (extension.size() < fileName.size() && extensionMatch)) {
			llc_path_debug("fileName=\"%s\"", fileName.begin());
			if_fail_fef(output.push_back(fileName), "fileName=%s, output.size()=%" LLC_FMT_U2, fileName.begin(), output.size());
		}
	}
	for(uint32_t iFolder = 0; iFolder < input.Folders.size(); ++iFolder	) {
		const ::llc::SPathContents	& childPath	= input.Folders[iFolder];
		if_fail_fef(llc::pathList(childPath, output, extension), "output=%s, extension=%s", output.begin(), extension.begin());
	}
	return 0;
}

//llc::err_t		listDirContents		(llc::vcst_t targetWildcard, llc::aobj<string> & filenames, llc::aobj<string> & dirnames) {
//	WIN32_FIND_DATA data = {}; 
//	HANDLE hFind;
//
//	while (INVALID_HANDLE_VALUE !=  (hFind = FindFirstFile(targetWildcard.begin(), &data))) 
//		if(0 == (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) { 
//			 if_fail_fe(filenames.push_back(data.cFileName)); 
//		}
//		else if(0 != memcmp(data.cFileName,TEXT( "."), sizeof(TCHAR) * 2) && 0 != memcmp(data.cFileName, TEXT(".."), sizeof(TCHAR) * 3)) 
//			if_fail_fe(dirnames.push_back(data.cFileName));
//
//	FindClose(hFind);
//	return 0;
//}


#if !defined(LLC_ESP8266) // && !defined(LLC_ESP32) && !defined(LLC_ARDUINO) 
stxp	const char		curDir	[]					= ".";
stxp	const char		parDir	[]					= "..";
#endif

sttc ::llc::err_t		pathListEntry			(::llc::vcst_t pathToList, ::llc::SPathContents & pathContents, const ::llc::function<::llc::err_t(bool, ::llc::vcst_c&)> & onItem, ::llc::vcst_c extension, ::llc::vcst_t entryName, bool isFolder) {
	if(0 == strcmp(entryName.begin(), curDir) || 0 == strcmp(entryName.begin(), parDir))
		rtrn 0;
	char						bufferFormat[36]			= {};
	snprintf(bufferFormat, ::llc::size(bufferFormat) - 2, "%%.%" LLC_FMT_U2 "s/%%s", pathToList.size());
	char						sPath[LLC_MAX_PATH]			= {};
	int32_t						lenPath						= snprintf(sPath, ::llc::size(sPath) - 2, bufferFormat, pathToList.begin(), entryName.begin());
	::llc::err_t				action						= 0;
	if_fail_fef(action = onItem(isFolder, sPath), "'%s'", sPath);
	if(action & 1)
		rtrn 0;
	if(isFolder) {
		::llc::err_t				newFolderIndex				= pathContents.Folders.push_back({});
		llc_necs(newFolderIndex);
		llc_necall(llc::pathList(sPath, pathContents.Folders[newFolderIndex], onItem, extension), "'%s'", sPath);
		verbose_printf("Directory: %s.", sPath);
	}
	else {
		int32_t						indexFile;
		llc_necall(indexFile = pathContents.Files.push_back(::llc::vcst_t{sPath, (uint32_t)lenPath}), "%s", "Failed to push path to output list");
		verbose_printf("File %" LLC_FMT_U2 ": %s.", indexFile, sPath);
	}
	rtrn 0;
}

sttc ::llc::err_t		pathListNative			(::llc::vcst_t pathToList, ::llc::SPathContents & pathContents, const ::llc::function<::llc::err_t(bool, ::llc::vcst_c&)> & onItem, ::llc::vcst_c extension) {
	::llc::err_t				result						= 0;
#ifdef LLC_WINDOWS
	char						sPath[LLC_MAX_PATH]			= {};
	if_fail_fef(snprintf(sPath, ::llc::size(sPath) - 2, "%.*s/*.*", (int)pathToList.size(), pathToList.begin()), "Path too long: '%.*s'.", pathToList.size(), pathToList.begin());
	WIN32_FIND_DATAA			fdFile						= {};
	HANDLE						hFind						= FindFirstFile(sPath, &fdFile);
	ree_if(hFind == INVALID_HANDLE_VALUE, "Path not found: [%.*s].", pathToList.size(), pathToList.begin());
	do {
		cnst ::llc::vcst_t		entryName						= {fdFile.cFileName, (::llc::u2_t)-1};
		if_fail_bef(result = ::pathListEntry(pathToList, pathContents, onItem, extension, entryName, 0 != (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)), "'%s'", entryName.begin());
	}
	while(FindNextFile(hFind, &fdFile));
	FindClose(hFind);
#elif defined(LLC_ANDROID) || defined(LLC_LINUX)
	DIR							* dir							= opendir(pathToList.begin());
	ree_if(0 == dir, "Path not found: [%.*s].", pathToList.size(), pathToList.begin());
	struct dirent				* drnt						= nullptr;
	while((drnt = readdir(dir))) {
		cnst ::llc::vcst_t		entryName						= {drnt->d_name, (::llc::u2_t)-1};
		if_fail_bef(result = ::pathListEntry(pathToList, pathContents, onItem, extension, entryName, drnt->d_type == DT_DIR), "'%.*s'", entryName.size(), entryName.begin());
	}
	closedir(dir);
#else
	(void)pathToList;
	(void)pathContents;
	(void)onItem;
	(void)extension;
#endif
	if_fail_fe(result);
	rtrn 0;
}

::llc::err_t			llc::pathList				(::llc::vcst_t pathToList, ::llc::aobj<string> & output, bool listFolders, ::llc::vcst_c extension)	{
	::llc::asc_t				withoutTrailingSlash		= (pathToList.size() - 1 > (uint32_t)::llc::findLastSlash(pathToList)) ? pathToList : ::llc::vcst_t{pathToList.begin(), pathToList.size() - 1};
	char						bufferFormat[16]			=  {};
	snprintf(bufferFormat, ::llc::size(bufferFormat) - 2, "%%.%" LLC_FMT_U2 "s/*.*", withoutTrailingSlash.size());
	char						sPath	[LLC_MAX_PATH]		= {};
	llc_necall(snprintf(sPath, ::llc::size(sPath) - 2, bufferFormat, withoutTrailingSlash.begin()), "bufferFormat: '%s'. withoutTrailingSlash: '%s'", bufferFormat, withoutTrailingSlash.begin());

#ifdef LLC_WINDOWS
	WIN32_FIND_DATAA			fdFile						= {};
	HANDLE						hFind						= NULL;
	hFind					= FindFirstFile(sPath, &fdFile);
	ree_if(hFind == INVALID_HANDLE_VALUE, "Path not found: [%s].", withoutTrailingSlash.begin());
	do if(	0 != strcmp(fdFile.cFileName, curDir)
		 &&	0 != strcmp(fdFile.cFileName, parDir)
		) {
		snprintf(bufferFormat, ::llc::size(bufferFormat) - 2, "%%.%" LLC_FMT_U2 "s/%%s", withoutTrailingSlash.size());
		int32_t						lenPath						= snprintf(sPath, ::llc::size(sPath) - 2, bufferFormat, withoutTrailingSlash.begin(), fdFile.cFileName);
		if((fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && false == listFolders)
			continue;

		::llc::vcst_c				viewPath					= sPath;
		if(0 == extension.size() || (extension.size() < viewPath.size() && 0 == strncmp(viewPath.end() - extension.size(), extension.begin(), ::llc::min(extension.size(), viewPath.size())))) {
			verbose_printf("Path: %s.", sPath);
			llc_necall(output.push_back(::llc::vcst_t{sPath, (uint32_t)lenPath}), "%s", "Failed to push path to output list.");
		}
		//output.push_back(0)
	}
	while(FindNextFile(hFind, &fdFile));
	FindClose(hFind);
#elif defined(LLC_ANDROID) || defined(LLC_LINUX)
	DIR							* dir						= 0;
	dirent						* drnt						= 0;
	dir						= opendir(withoutTrailingSlash.begin());
	while ((drnt = readdir(dir)) != NULL) {
		::llc::asc_t				name						= ::llc::vcst_t{drnt->d_name, (uint32_t)-1};
		if (name != curDir && name != parDir) {
			if(drnt->d_type == DT_DIR && false == listFolders)
				continue;
			int32_t						lenPath						= snprintf(sPath, ::llc::size(sPath) - 2, "%s/%s", withoutTrailingSlash.begin(), drnt->d_name);
			info_printf("Path: %s.", sPath);
			llc_necall(output.push_back(::llc::vcst_t{sPath, (uint32_t)lenPath}), "%s", "Failed to push path to output list.");
		}
	}
#endif
	return 0;
}


::llc::err_t			llc::pathList				(::llc::vcst_t pathToList, ::llc::SPathContents & pathContents, ::llc::vcst_c extension)						{
	rtrn ::llc::pathList(pathToList, pathContents, [](::llc::b8_t, ::llc::vcst_c &) { rtrn 0; }, extension);
}

::llc::err_t			llc::pathList				(::llc::vcst_t pathToList, ::llc::SPathContents & pathContents, ::llc::function<err_t(bool, vcst_c&)> onItem, ::llc::vcst_c extension)						{
	::llc::vcst_t				pathBegin;
	if_fail_fe(::llc::pathBegin(pathToList, pathBegin));
	cnst bool					removeTrailingSlash		= pathToList.size() > pathBegin.size() && ::pathSeparator(pathToList[pathToList.size() - 1]);
	::llc::string				withoutTrailingSlash		= removeTrailingSlash ? ::llc::vcst_t{pathToList.begin(), pathToList.size() - 1} : pathToList;
	rtrn ::pathListNative(withoutTrailingSlash, pathContents, onItem, extension);
}
