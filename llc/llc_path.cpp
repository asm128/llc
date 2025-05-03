#include "llc_path.h"

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

stxp	uint32_t LLC_MAX_PATH = 256;
//
::llc::err_t			llc::pathCreate				(::llc::vcst_c & pathName, sc_c separator) {
	if_zero_fw(pathName.size());
#ifndef LLC_ATMEL
	char						folder[LLC_MAX_PATH]		= {};
	s2_t						offsetBar					= -1;
	do {
		++offsetBar;
		offsetBar				= ::llc::find(separator, pathName, offsetBar);
		if(0 == offsetBar) {
			if(offsetBar < 0 || offsetBar == (int32_t)pathName.size() - 1)
				break;
			continue;
		}
		ree_if(0 == strncpy_s(folder, pathName.begin(), (offsetBar < 0) ? pathName.size() : offsetBar), "String buffer overflow? Path size: %" LLC_FMT_U2 ".", pathName.size());
		if(0 == strcmp(".", folder))
			continue;
#if defined(LLC_WINDOWS)
		if(!CreateDirectoryA(folder, NULL)) {
			DWORD						err							= GetLastError();
			ree_if(err != ERROR_ALREADY_EXISTS, "Failed to create directory: %s. hr: (%" LLC_FMT_U2 ")", folder, err);
		}
#else
		struct stat st = {0};
		if (stat(folder, &st) == -1) {
			mkdir(folder, 0700);
		}
#endif
		if(offsetBar < 0 || offsetBar == (s2_t)pathName.size() - 1)
			break;
	} while(true);
#endif
	return 0;
}

::llc::err_t			llc::findLastSlash			(::llc::vcsc_c & path)		{
	int32_t						indexOfStartOfFileName0		= ::llc::rfind('\\', path);
	int32_t						indexOfStartOfFileName1		= ::llc::rfind('/', path);
	return
		(-1 == indexOfStartOfFileName1) ? indexOfStartOfFileName0 :
		(-1 == indexOfStartOfFileName0) ? indexOfStartOfFileName1 :
		::llc::max(indexOfStartOfFileName0, indexOfStartOfFileName1)
		;
}
//
::llc::err_t			llc::pathNameCompose		(::llc::vcsc_c & path, ::llc::vcsc_c & fileName, ::llc::asc_t & out_composed)		{
	if(path.size()) {
		for(uint32_t iChar = 0; iChar < path.size(); ++iChar) {
			const char					curChar						= path[iChar];
			if(iChar < (path.size() - 1)) {
				const char					nxtChar						= path[iChar + 1];
				if('\\' == curChar && '\\' == nxtChar)
					continue;//++iChar;
			}
			out_composed.push_back(curChar);
		}
		if('\\' != path[path.size() - 1] && '/' != path[path.size() - 1])
			out_composed.push_back('/');
	}
	if(fileName.size()) {
		for(uint32_t iChar = ('\\' == fileName[0]) ? 1 : 0; iChar < fileName.size(); ++iChar) {
			const char					curChar						= fileName[iChar];
			if(iChar < (fileName.size() - 1)) {
				const char					nxtChar						= fileName[iChar + 1];
				if('\\' == curChar && '\\' == nxtChar)
					continue; //++iChar;
			}
			out_composed.push_back(curChar);
		}
	}
	return out_composed.size();
}

::llc::err_t			llc::pathList				(const ::llc::SPathContents & input, ::llc::avcsc_t & output, ::llc::vcst_c extension)					{
	for(uint32_t iFile = 0; iFile < input.Files.size(); ++iFile) {
		::llc::vcsc_c			& fileName					= input.Files[iFile];
		if(0 == extension.size() || (extension.size() < fileName.size() && 0 == strncmp(fileName.end() - extension.size(), extension.begin(), ::llc::min(extension.size(), fileName.size()))))
			llc_necs(output.push_back(fileName));
	}
	for(uint32_t iFolder = 0; iFolder < input.Folders.size(); ++iFolder)
		llc_necall(llc::pathList(input.Folders[iFolder], output, extension), "%s", "Unknown error!");
	return 0;
}

::llc::err_t			llc::pathList				(const ::llc::SPathContents & input, ::llc::aasc_t & output, ::llc::vcst_c extension)					{
	for(uint32_t iFile = 0; iFile < input.Files.size(); ++iFile) {
		::llc::vcsc_c			& fileName					= input.Files[iFile];
		if(0 == extension.size() || (extension.size() < fileName.size() && 0 == strncmp(fileName.end() - extension.size(), extension.begin(), ::llc::min(extension.size(), fileName.size()))))
			llc_necs(output.push_back(fileName));
	}
	for(uint32_t iFolder = 0; iFolder < input.Folders.size(); ++iFolder	)
		llc_necall(llc::pathList(input.Folders[iFolder], output, extension), "%s", "Unknown error!");
	return 0;
}

//llc::err_t		listDirContents		(llc::vcsc_t targetWildcard, llc::aasc_t & filenames, llc::aasc_t & dirnames) {
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

::llc::err_t			llc::pathList				(const ::llc::vcst_t & pathToList, ::llc::aasc_t & output, bool listFolders, ::llc::vcst_c extension)	{
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
			llc_necall(output.push_back(::llc::vcsc_t{sPath, (uint32_t)lenPath}), "%s", "Failed to push path to output list.");
		}
	}
#endif
	return 0;
}


::llc::err_t			llc::pathList				(::llc::vcst_c & pathToList, ::llc::SPathContents & pathContents, ::llc::vcst_c extension)						{
	::llc::asc_t				withoutTrailingSlash		= (pathToList.size() - 1 > (uint32_t)::llc::findLastSlash(pathToList)) ? pathToList : ::llc::vcst_t{pathToList.begin(), pathToList.size() - 1};
	char						bufferFormat[36]			= {};
	snprintf(bufferFormat, ::llc::size(bufferFormat) - 2, "%%.%" LLC_FMT_U2 "s/*.*", withoutTrailingSlash.size());
	char						sPath[LLC_MAX_PATH]			= {};
	llc_necall(snprintf(sPath, ::llc::size(sPath) - 2, bufferFormat, withoutTrailingSlash.begin()), "%s", "Path too long?");
#ifdef LLC_WINDOWS
	WIN32_FIND_DATAA			fdFile						= {};
	HANDLE						hFind						= NULL;
	hFind					= FindFirstFile(sPath, &fdFile);
	ree_if(hFind == INVALID_HANDLE_VALUE, "Path not found: [%s].", withoutTrailingSlash.begin());
	do if(	0 != strcmp(fdFile.cFileName, curDir)
		 &&	0 != strcmp(fdFile.cFileName, parDir)
		) {
		//_CrtCheckMemory();
		snprintf(bufferFormat, ::llc::size(bufferFormat) - 2, "%%.%" LLC_FMT_U2 "s/%%s", withoutTrailingSlash.size());
		int32_t						lenPath						= snprintf(sPath, ::llc::size(sPath) - 2, bufferFormat, withoutTrailingSlash.begin(), fdFile.cFileName);
		if(fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			::llc::err_t				newFolderIndex				= pathContents.Folders.push_back({});
			llc_necs(newFolderIndex);
			llc_necall(llc::pathList(sPath, pathContents.Folders[newFolderIndex], extension), "%s", "Unknown error!");
			verbose_printf("Directory: %s.", sPath);
		}
		else {
			int32_t						indexFile;
			llc_necall(indexFile = pathContents.Files.push_back(::llc::vcst_t{sPath, (uint32_t)lenPath}), "%s", "Failed to push path to output list");
			//pathContents.Files[indexFile].push_back(0);
			verbose_printf("File %" LLC_FMT_U2 ": %s.", indexFile, sPath);
		}
		//_CrtCheckMemory();
	}
	while(FindNextFile(hFind, &fdFile));

	FindClose(hFind);
#elif defined(LLC_ANDROID) || defined(LLC_LINUX)
	DIR							* dir						= nullptr;
	struct dirent				* drnt						= nullptr;
	dir						= opendir(withoutTrailingSlash.begin());
	while ((drnt = readdir(dir)) != NULL) {
		::llc::apod<char>			name						= ::llc::vcst_t{drnt->d_name, (uint32_t)-1};
		if (name != curDir && name != parDir) {
			int32_t						lenPath						= snprintf(sPath, ::llc::size(sPath) - 2, "%s/%s", withoutTrailingSlash.begin(), drnt->d_name);
			if(drnt->d_type == DT_DIR) {
				::llc::err_t				newFolderIndex				= pathContents.Folders.push_back({});
				llc_necs(newFolderIndex);
				llc_necall(llc::pathList(sPath, pathContents.Folders[newFolderIndex], extension), "%s", "Unkown error!");
				info_printf("Directory: %s.", sPath);
			}
			else {
				llc_necall(pathContents.Files.push_back(::llc::vcsc_t{sPath, (uint32_t)lenPath}), "%s", "Failed to push path to output list");
				info_printf("File: %s.", sPath);
			}
		}
	}
#endif
	return 0;
}

::llc::err_t			llc::pathList				(::llc::vcst_c & pathToList, ::llc::SPathContents & pathContents, ::llc::function<err_t(bool, vcst_c&)> onItem, ::llc::vcst_c extension)						{
	::llc::asc_t				withoutTrailingSlash		= (pathToList.size() - 1 > (uint32_t)::llc::findLastSlash(pathToList)) ? pathToList : ::llc::vcst_t{pathToList.begin(), pathToList.size() - 1};
	char						bufferFormat[36]			= {};
	snprintf(bufferFormat, ::llc::size(bufferFormat) - 2, "%%.%" LLC_FMT_U2 "s/*.*", withoutTrailingSlash.size());
	char						sPath[LLC_MAX_PATH]			= {};
	llc_necall(snprintf(sPath, ::llc::size(sPath) - 2, bufferFormat, withoutTrailingSlash.begin()), "%s", "Path too long?");
#ifdef LLC_WINDOWS
	WIN32_FIND_DATAA			fdFile						= {};
	HANDLE						hFind						= NULL;
	hFind					= FindFirstFile(sPath, &fdFile);
	ree_if(hFind == INVALID_HANDLE_VALUE, "Path not found: [%s].", withoutTrailingSlash.begin());
	do if(	strcmp(fdFile.cFileName, curDir)
		 &&	strcmp(fdFile.cFileName, parDir)
		) {
		//_CrtCheckMemory();
		snprintf(bufferFormat, ::llc::size(bufferFormat) - 2, "%%.%" LLC_FMT_U2 "s/%%s", withoutTrailingSlash.size());
		int32_t						lenPath						= snprintf(sPath, ::llc::size(sPath) - 2, bufferFormat, withoutTrailingSlash.begin(), fdFile.cFileName);
		const bool					isFolder					= fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
		err_t						action						= 0;
		if_fail_fef(action = onItem(isFolder, sPath), "'%s'", sPath);
		if(action & 1)
			continue;
		if(isFolder) {
			::llc::err_t				newFolderIndex				= pathContents.Folders.push_back({});
			llc_necs(newFolderIndex);
			llc_necall(llc::pathList(sPath, pathContents.Folders[newFolderIndex], extension), "%s", "Unknown error!");
			verbose_printf("Directory: %s.", sPath);
		}
		else {
			int32_t						indexFile;
			llc_necall(indexFile = pathContents.Files.push_back(::llc::vcst_t{sPath, (uint32_t)lenPath}), "%s", "Failed to push path to output list");
			//pathContents.Files[indexFile].push_back(0);
			verbose_printf("File %" LLC_FMT_U2 ": %s.", indexFile, sPath);
		}
		//_CrtCheckMemory();
	}
	while(FindNextFile(hFind, &fdFile));

	FindClose(hFind);
#elif defined(LLC_ANDROID) || defined(LLC_LINUX)
	DIR							* dir						= nullptr;
	struct dirent				* drnt						= nullptr;
	dir						= opendir(withoutTrailingSlash.begin());
	while ((drnt = readdir(dir))) {
		::llc::apod<char>			name						= ::llc::vcst_t{drnt->d_name, (uint32_t)-1};
		if (name != curDir && name != parDir) {
			int32_t						lenPath						= snprintf(sPath, ::llc::size(sPath) - 2, "%s/%s", withoutTrailingSlash.begin(), drnt->d_name);
			const bool					isFolder					= drnt->d_type == DT_DIR;
			err_t						action						= 0;
			if_fail_fef(action = onItem(isFolder, sPath), "'%s'", sPath);
			if(action & 1)
				continue;
			if(isFolder) {
				::llc::err_t				newFolderIndex				= pathContents.Folders.push_back({});
				llc_necs(newFolderIndex);
				llc_necall(llc::pathList(sPath, pathContents.Folders[newFolderIndex], onItem, extension), "'%s'", sPath);
				info_printf("Directory: %s.", sPath);
			}
			else {
				llc_necall(pathContents.Files.push_back(::llc::vcsc_t{sPath, (uint32_t)lenPath}), "%s", sPath);
				info_printf("File: %s.", sPath);
			}
		}
	}
#endif
	return 0;
}
