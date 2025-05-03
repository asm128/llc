#include "llc_file.h"
#include "llc_cstdio.h"
#include "llc_cstring.h"
#include "llc_path.h"
#include "llc_string_compose.h"

#include <new>

#ifdef LLC_WINDOWS
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif
#	include <Windows.h>
#elif defined(LLC_ANDROID) || defined(LLC_LINUX)
#	include <dirent.h>
#	include <unistd.h>
#	include <fcntl.h> // Definition of AT_* constants
#elif defined(LLC_ARDUINO)
#	ifdef LLC_ESP8266
#		include <LittleFS.h>
#		include "umm_malloc/umm_malloc.h"
#		define 	LLC_SOC_FILESYSTEM_INSTANCE LittleFS
#	elif defined(LLC_ESP32) 
#		include <SPIFFS.h>
#		define 	LLC_SOC_FILESYSTEM_INSTANCE SPIFFS
#	endif // LLC_ESP32
#endif // LLC_ARDUINO

LLC_USING_APOD();
LLC_USING_VIEW();
LLC_USING_TYPEINT();

llc::err_t		llc::with			(vcst_t filepath, vcst_t mode, const llc::function<llc::err_t(FILE*&)> & funcFile)	{ 
	FILE					* fp				= {};
	llc_necs(llc::fopen_s(&fp, filepath, mode)); if_null_ve(-1, fp);
	llc::err_t	result;
	if_fail_ef(result = funcFile(fp), "filepath:'%s'.", filepath.begin());
	if(fp)
		fclose(fp);
	return result;
}

stxp llc::vcst_t	LLC_OPEN_MODE_READ		= LLC_CXS("rb");
stxp llc::vcst_t	LLC_OPEN_MODE_WRITE		= LLC_CXS("wb");
stxp llc::vcst_t	LLC_OPEN_MODE_APPEND	= LLC_CXS("ab+");

#ifdef LLC_ARDUINO
#	if defined(LLC_ESP32) || defined(LLC_ESP8266)
stin	fs::FS&		getSoCFileSystem	()	{ rtrn LLC_SOC_FILESYSTEM_INSTANCE; }
sttc  	llc::err_t	socPath  			(llc::asc_t & fixed, llc::vcsc_t path) {
	llc::vcsc_t			trimmd;
	if_fail_fe(llc::ltrim(trimmd, path, "/ \t\n"));
	rtrn llc::append_strings(fixed, '/', trimmd);
}
#		define FIX_SOC_PATH(_pathView)                      {   \
	if_zero_fe(_pathView.size());                           	\
	asc_t      _fixedPath;                            			\
	if('/' != _pathView[0]) {                               	\
		if_fail_fe(::socPath(_fixedPath, _pathView)); 			\
		_pathView = _fixedPath;                       			\
	}															\
}
#	endif
#endif // LLC_ARDUINO

#define llc_file_info_printf(...) do {} while(0) // info_printf

s3_t	llc::	fileSize			(llc::vcst_t usfileName)			{
	llc::asc_t			fileName					= llc::toString(usfileName);
#ifdef LLC_ARDUINO
#	if defined(LLC_ESP32) || defined(LLC_ESP8266)
	FIX_SOC_PATH(fileName);
	File				fp						= getSoCFileSystem().open(fileName.begin(), LLC_OPEN_MODE_READ);
	ree_if(!fp, "Cannot open file: %s.", fileName.begin());
	u2_c				fileSize				= (uint32_t)fp.size();
#	endif // LLC_ESP32
#else
	::FILE				* fp					= 0;
	llc_necall(llc::fopen_s(&fp, fileName, ::LLC_OPEN_MODE_READ), "Failed to open '%s'.", fileName.begin());
	ree_if(0 == fp, "Failed to open '%s'.", fileName.begin());
	if(0 != llc::fseek(fp, 0, FSEEK_END)) {
		error_printf("%s", "Unknown error reading '%s'.", fileName.begin());
		fclose(fp);
		return -1;
	}
	const s3_t			fileSize				= llc::ftell(fp);
	if(0 > fileSize)
		error_printf("%s", "Unknown error reading '%s'.", fileName.begin());
	fclose(fp);
#endif // LLC_ARDUINO
	return fileSize;
}

// This function is useful for splitting files smaller than 4gb very quick.
static	llc::err_t	fileSplitSmall	(llc::vcst_t fileNameSrc, u2_c sizePartMax) {
	ree_if(0 == sizePartMax, "Invalid part size: %" LLC_FMT_U2 ".", fileNameSrc.begin(), sizePartMax);
	llc::apod<int8_t>				fileInMemory;
	llc_necall(llc::fileToMemory(fileNameSrc, fileInMemory), "Failed to load '%s'.", fileNameSrc);

	// -- Write parts to disk.
	uint32_t			countParts			= fileInMemory.size() / sizePartMax + one_if(fileInMemory.size() % sizePartMax);
	char				fileNameDst	[1024]	= {};
	uint32_t			iPart				= 0;
	for(; iPart < countParts; ++iPart) {
		u2_c					offsetPart					= sizePartMax * iPart;
		llc_necall(snprintf(fileNameDst, llc::size(fileNameDst) - 2, "%s.%.2" LLC_FMT_U2, fileNameSrc.begin(), iPart), "File name too large: %s.", fileNameSrc.begin());
		info_printf("Creating part %" LLC_FMT_U2 ": '%s'.", iPart, fileNameDst);
		FILE				* fpDest					= 0;
		ree_if(0 != llc::fopen_s(&fpDest, fileNameDst, LLC_OPEN_MODE_WRITE), "Failed to open file: %s.", fileNameDst);
		ree_if(0 == fpDest, "Failed to create file: %s.", fileNameDst);
		uint32_t			countBytes					= (iPart == countParts - 1) ? fileInMemory.size() - offsetPart : sizePartMax;
		ree_if(countBytes != fwrite(&fileInMemory[offsetPart], 1, countBytes, fpDest), "Failed to write part %" LLC_FMT_U2 " of %" LLC_FMT_U2 " bytes to disk. Disk full?", iPart, countBytes);
		fclose(fpDest);
	}
	return countParts;
}

// This function is useful for splitting files smaller than 4gb very quick.
static	llc::err_t	fileSplitLarge				(llc::vcst_t fileNameSrc, u2_c sizePartMax) {
	ree_if(0 == sizePartMax, "Invalid part size: %" LLC_FMT_U2 ".", fileNameSrc.begin(), sizePartMax);
	s3_t				sizeFile;
	if_fail_vef(-1, sizeFile = llc::fileSize(fileNameSrc), "Failed to open file %s.", fileNameSrc.begin());
	FILE				* fp				= 0;
	ree_if(0 == fp, "%s", "Files larger than 3gb still not supported.");
	ree_if(0 != llc::fopen_s(&fp, {fileNameSrc}, LLC_OPEN_MODE_READ), "Failed to open '%s'.", fileNameSrc.begin());
	ree_if(0 == fp, "Failed to open '%s'.", fileNameSrc.begin());

	llc::as0_t					partInMemory;
	llc_necall(partInMemory.resize(sizePartMax), "Failed to allocate buffer for file part. Out of memory? File part size: %" LLC_FMT_U2 ".", sizePartMax);

	// -- Write parts to disk.
	uint32_t			countParts					= (uint32_t)(sizeFile / sizePartMax + one_if(sizeFile % sizePartMax));
	char				fileNameDst	[1024]			= {};
	uint32_t			iPart				= 0;
	for(; iPart < countParts; ++iPart) { //
		u2_c					offsetPart					= sizePartMax * iPart;
		llc_necall(snprintf(fileNameDst, llc::size(fileNameDst) - 2, "%s.%.2" LLC_FMT_U2, fileNameSrc.begin(), iPart), "File name too large: %s.", fileNameSrc.begin());
		info_printf("Creating part %" LLC_FMT_U2 ": '%s'.", iPart, fileNameDst);
		FILE				* fpDest					= 0;
		ree_if(0 != llc::fopen_s(&fpDest, fileNameDst, LLC_OPEN_MODE_WRITE), "Failed to open file: %s.", fileNameDst);
		ree_if(0 == fpDest, "Failed to create file: %s.", fileNameDst);
		s3_t				countBytes					= (iPart == countParts - 1) ? sizeFile - offsetPart : sizePartMax;
		ree_if(countBytes != (s3_t)fwrite(partInMemory.begin(), 1, (uint32_t)countBytes, fpDest), "Failed to write part %" LLC_FMT_U2 " of %" LLC_FMT_U2 " bytes to disk. Disk full?", iPart, countBytes);
		fclose(fpDest);
	}
	return countParts;
}

// Splits a file into file.## parts.
llc::err_t	llc::	fileSplit				(vcst_t fileNameSrc, u2_c sizePartMax) {
	// -- Get file size to determine which algorithm to use.
	// -- For files smaller than 3gb, we use a fast algorithm that loads the entire file in memory.
	// -- For files of, or larger than, 3gb, we use a fast algorithm that loads chunks of 1gb in memory for writing the parts.
	stxp	u2_c		gigabyte					= 1024U*1024U*1024U;
	stxp	u2_c		sizeSmallFileMax			= 3U * gigabyte;
	const s3_t				sizeFile					= llc::fileSize(fileNameSrc);
	ree_if(-1 == sizeFile, "Failed to get size for file: '%s'.", fileNameSrc.begin());
	return (sizeSmallFileMax > sizeFile) ? ::fileSplitSmall(fileNameSrc, sizePartMax) : ::fileSplitLarge(fileNameSrc, sizePartMax);
}
// Joins a file split into file.## parts.
llc::err_t	llc::	fileJoin				(vcst_t fileNameDst)	{
	char				fileNameSrc	[1024]			= {};
	uint32_t					iFile				= 0;
	llc_necall(snprintf(fileNameSrc, llc::size(fileNameSrc) - 2, "%s.%.2" LLC_FMT_U2, str(fileNameDst).begin(), ++iFile), "File name too large: %s.", fileNameDst.begin());
	::FILE				* fpDest					= 0;
	llc::apod<char>			finalPathName				= llc::toString(fileNameDst);
	llc_necall(llc::fopen_s(&fpDest, finalPathName, LLC_OPEN_MODE_WRITE), "%s", finalPathName.begin());
	ree_if(0 == fpDest, "Failed to create file: %s.", finalPathName.begin());
	llc::apod<int8_t>			fileInMemory				= {};
	// Load each .split part and write it to the destionation file.
	while(0 == llc::fileToMemory(fileNameSrc, fileInMemory)) {	// Load first part and write it to the joined file.
		ree_if(fileInMemory.size() != fwrite(fileInMemory.begin(), 1, fileInMemory.size(), fpDest), "Write operation failed. Disk full? File size: %" LLC_FMT_U2 ". File name: %s.", fileInMemory.size(), fileNameSrc);
		llc_necall(snprintf(fileNameSrc, llc::size(fileNameSrc) - 2, "%s.%.2" LLC_FMT_U2, finalPathName.begin(), ++iFile), "File name too large: %s.", finalPathName.begin());
		fileInMemory.clear();
	}
	fclose(fpDest);
	return iFile - 1;
}

#define LLC_DEBUG_FILE_CONTENTS

llc::err_t	llc::	fileToMemory			(vcst_t usfileName, llc::au0_t & fileInMemory, uint32_t maxSize, uint64_t offset)		{
	llc::asc_t			fileName					= llc::toString(usfileName);
	llc_file_info_printf("Loading '%s'.", fileName.begin());

#ifdef LLC_ARDUINO
#	if defined(LLC_ESP32) || defined(ESP8266)
	FIX_SOC_PATH(fileName);
	File				fp					= getSoCFileSystem().open(fileName.begin(), LLC_OPEN_MODE_READ);
	ree_if(!fp, "Cannot open file: %s.", fileName.begin());
	u2_c				fileSize					= (uint32_t)fp.size();
	fileInMemory.clear();
#		ifdef LLC_ESP32
	llc_necall(fileInMemory.resize(fileSize), "Too large to load in memory? File size: %" LLC_FMT_S2 ". Available memory: %" LLC_FMT_S2 ".", fp.size(), heap_caps_get_free_size(MALLOC_CAP_8BIT));
#		else
	llc_necall(fileInMemory.resize(fileSize), "Too large to load in memory? File size: %" LLC_FMT_S2 ". Available memory: %" LLC_FMT_S2 ".", fp.size(), umm_free_heap_size_lw());
#		endif // LLC_ESP32
#	endif // LLC_ESP32 || LLC_ESP8266
	rees_if(fileSize != (uint32_t)fp.readBytes((char*)fileInMemory.begin(), fileSize));
	fp.close();
	llc::err_t	result				= 0;
#else // !LLC_ARDUINO
	::FILE				* fp				= 0;
	const int32_t				fileErr				= llc::fopen_s(&fp, fileName, ::LLC_OPEN_MODE_READ);
	rvw_if((fileErr > 0) ? -fileErr : fileErr, 0 != fileErr || 0 == fp, "Cannot open file: %s.", fileName.begin());
	fseek(fp, 0, SEEK_END);
	const s3_t				fileSize					= llc::ftell(fp);
	fail_if_lt3s(fileSize, 0);
	fail_if_ge3u(offset, fileSize);
	u2_c				maxRead				= (uint32_t)min((s3_t)maxSize, fileSize - (s3_t)offset);
	fail_if_ne3u(0, llc::fseek(fp, offset, FSEEK_SET));
	llc::err_t	result				= 0;
	if_failed(fileInMemory.resize(maxRead)) {
		error_printf("File too large? : %llu.", (uint64_t)fileSize);
		result					= -1;
	}
	else {
		if(fileSize != (int32_t)fread(fileInMemory.begin(), szof(uint8_t), fileSize, fp)) {
			error_printf("fread() failed! file: '%s'.", fileName.begin());
			result					= -1;
		}
	}
	fclose(fp);
#endif // LLC_ARDUINO

#ifdef LLC_DEBUG_FILE_CONTENTS
	llc_file_info_printf("'%s' loaded successfully. Size: %" LLC_FMT_U2 ":\n%s\n", fileName.begin(), fileInMemory.size(), fileInMemory.size() ? fileInMemory.begin() : (const uint8_t*)"");
#else
	llc_file_info_printf("'%s' loaded successfully. Size: %" LLC_FMT_U2 ".", fileName.begin(), fileInMemory.size());
#endif // LLC_DEBUG_FILE_CONTENTS
	return result;
}

llc::err_t	llc::	fileFromMemory			(vcst_t usfileName, vcu0_c & fileInMemory, bool append)	{
	llc::asc_t			fileName					= llc::toString(usfileName);
#ifdef LLC_DEBUG_FILE_CONTENTS
	llc_file_info_printf("%s '%s':\n%s\n", append ? "Appending to" : "Writing", fileName.begin(), fileInMemory.size() ? fileInMemory.begin() : (const uint8_t*)"");
#else
	llc_file_info_printf("%s '%s'.", append ? "Appending to" : "Writing", fileName.begin());
#endif // LLC_DEBUG_FILE_CONTENTS

	llc::err_t	result				= 0;
#ifdef LLC_ARDUINO
#	if defined(LLC_ESP32) || defined(ESP8266)
	FIX_SOC_PATH(fileName);
	File 				fp	 				= ::getSoCFileSystem().open(fileName.begin(), append ? LLC_OPEN_MODE_APPEND : LLC_OPEN_MODE_WRITE);
	ree_if(!fp, "Failed to open '%s'.", fileName.begin());
	ree_if(!fp.write(fileInMemory.begin(), fileInMemory.size()), "Failed to write to '%s'.", fileName.begin());
#	endif // LLC_ESP32 || LLC_ESP8266
#else // !LLC_ARDUINO
	::FILE				* fp				= 0;
	const int32_t				fileErr				= llc::fopen_s(&fp, fileName, append ? LLC_OPEN_MODE_APPEND : LLC_OPEN_MODE_WRITE);
	rvw_if((fileErr > 0) ? -fileErr : fileErr, 0 != fileErr || 0 == fp, "Failed to create '%s' for %s.", fileName.begin(), append ? "appending" : "writing");
	if(fileInMemory.size() != fwrite(fileInMemory.begin(), 1, fileInMemory.size(), fp)) {
		error_printf("Failed to write '%s'. Disk full? File size: %" LLC_FMT_U2 ".", fileName.begin(), fileInMemory.size());
		result				= -1;
	}
	fclose(fp);
#endif // LLC_ARDUINO
	llc_file_info_printf("'%s' written successfully.", fileName.begin());
	return result;
}

llc::err_t	llc::	fileDelete				(vcst_t usfileName)	{
	llc::asc_t	fileName					= llc::toString(usfileName);
	llc_file_info_printf("Deleting '%s'.", fileName.begin());
#ifdef LLC_WINDOWS
	ree_if(FALSE == DeleteFileA(fileName.begin()), "Failed to delete '%s'.", fileName.begin());
#elif defined(LLC_ANDROID)
#elif defined(LLC_ARDUINO)
#	if defined(LLC_ESP32) || defined(ESP8266)
	FIX_SOC_PATH(fileName);
	ree_if(0 == ::getSoCFileSystem().remove(fileName.begin()), "Failed to delete '%s'.", fileName.begin());
#	elif defined(LLC_ATMEL)
#	endif // LLC_ESP32 || LLC_ESP8266
#else
	llc_necall(unlink(fileName.begin()), "Failed to delete '%s'.", fileName.begin());
#endif
	llc_file_info_printf("'%s' deleted successfully.", fileName.begin());
	return 0;
}
llc::err_t	llc::	fileToMemory	(vcst_t folderPath, vcst_t fileName, llc::au0_t & fileBytes, uint32_t maxSize, uint64_t offset) {
	llc::asc_t			filePath			= {};
	llc_necall(llc::pathNameCompose(folderPath, fileName, filePath), "folderPath: '%s', fileName: '%s'.", folderPath.begin(), fileName.begin());
	llc_necall(llc::fileToMemory({filePath}, fileBytes, maxSize, offset), "folderPath: '%s', fileName: '%s'.", folderPath.begin(), fileName.begin());
	return 0;
}
llc::err_t	llc::	fileFromMemory	(vcst_t folderPath, vcst_t fileName, vcu0_c & fileInMemory, bool append) {
	llc::asc_t			filePath			= {}; 
	llc_necall(llc::pathNameCompose(folderPath, fileName, filePath), "folderPath: '%s', fileName: '%s'.", folderPath.begin(), fileName.begin());
	llc_necall(llc::fileFromMemory({filePath}, fileInMemory, append), "folderPath: '%s', fileName: '%s', append: %s.", folderPath.begin(), fileName.begin(), llc::bool2char(append));
	return 0;
}
