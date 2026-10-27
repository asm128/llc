#include "llc_file.h"
#include "llc_path.h"

#include "llc_string.h"
#include "llc_cstdio.h"
#include "llc_string_compose.h"

#include <new>
#ifndef LLC_ARDUINO
#	include <sys/stat.h>
#endif

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

llc::err_t		llc::with			(vcst_t filepath, vcst_t mode, const function<err_t(FILE*&)> & funcFile)	{
	FILE				* fp				= {};
	if_fail_fe(llc::fopen_s(fp, filepath, mode));
	if_null_ve(-1, fp);
	err_t				result;
	if_fail_ef(result = funcFile(fp), "filepath:'%.*s'.", (int)filepath.size(), filepath.begin());
	if(fp)
		fclose(fp);
	return result;
}

stxp llc::vcst_t	LLC_OPEN_MODE_READ		= "rb";
stxp llc::vcst_t	LLC_OPEN_MODE_WRITE		= "wb";
stxp llc::vcst_t	LLC_OPEN_MODE_APPEND	= "ab+";

#ifdef LLC_ARDUINO
#	if defined(LLC_ESP32) || defined(LLC_ESP8266)
stin	fs::FS&		getSoCFileSystem	()	{ rtrn LLC_SOC_FILESYSTEM_INSTANCE; }
sttc  	llc::err_t	socPath  			(llc::string & fixed, llc::vcst_t path) {
	llc::vcst_t			trimmd				= {};
	if_fail_fe(llc::ltrim(trimmd, path, "/ \t\n"));
	rtrn llc::append_strings(fixed, '/', trimmd);
}
#		define FIX_SOC_PATH(_pathView)                      {   \
	if_zero_fe(_pathView.size());                           	\
	string      _fixedPath;                            			\
	if('/' != _pathView[0]) {                               	\
		if_fail_fe(::socPath(_fixedPath, _pathView)); 			\
		_pathView = _fixedPath;                       			\
	}															\
}
#	endif
#endif // LLC_ARDUINO

#define llc_file_info_printf(...) do {} while(0) // info_printf

s3_t	llc::	fileSize			(llc::vcst_t fileName)			{
#ifdef LLC_ARDUINO
#	if defined(LLC_ESP32) || defined(LLC_ESP8266)
	FIX_SOC_PATH(fileName);
	File				fp						= getSoCFileSystem().open(fileName.begin(), LLC_OPEN_MODE_READ);
	if_true_fef(!fp, "Cannot open file: %.*s.", (int)fileName.size(), fileName.begin());
	u2_c				fileSize				= (::llc::u2_t)fp.size();
#	endif // LLC_ESP32
#else
	::FILE				* fp					= 0;
	if_fail_fef(llc::fopen_s(fp, fileName, ::LLC_OPEN_MODE_READ), "Failed to open '%.*s'.", (int)fileName.size(), fileName.begin());
	if_true_fef(0 == fp, "Failed to open '%.*s'.", (int)fileName.size(), fileName.begin());
	if(0 != llc::fseek(fp, 0, FSEEK_END)) {
		error_printf("Unknown error reading '%.*s'.", (int)fileName.size(), fileName.begin());
		fclose(fp);
		return -1;
	}
	const s3_t			fileSize				= llc::ftell(fp);
	if(0 > fileSize)
		error_printf("Unknown error reading '%.*s'.", (int)fileName.size(), fileName.begin());
	fclose(fp);
#endif // LLC_ARDUINO
	return fileSize;
}

llc::err_t	llc::fileLastWriteTime	(vcst_t fileName, s3_t & modified) {
#ifdef LLC_WINDOWS
	struct _stat64	fileStatus	= {};
	if(_stat64(fileName.begin(), &fileStatus))
		rtrn 1;
	modified = (s3_t)fileStatus.st_mtime;
#elif defined(LLC_ARDUINO)
	(void)fileName;
	(void)modified;
	rtrn 1;
#else
	struct stat		fileStatus	= {};
	if(stat(fileName.begin(), &fileStatus))
		rtrn 1;
	modified = (s3_t)fileStatus.st_mtime;
#endif
	rtrn 0;
}

// Retain the whole-file fast path for files below the streaming threshold.
sttc llc::err_t fileSplitSmall(llc::vcst_t fileNameSrc, u2_c sizePartMax) {
	llc::as0_t			fileInMemory;
	if_fail_fe(llc::fileToMemory(fileNameSrc, fileInMemory));
	cnst u2_t			countParts = fileInMemory.size() / sizePartMax + one_if(fileInMemory.size() % sizePartMax);
	if_true_fe(countParts > 0x7FFFFFFFU);
	llc::string			fileNameDst;
	for(u2_t iPart = 0; iPart < countParts; ++iPart) {
		cnst u2_t		offsetPart = sizePartMax * iPart;
		fileNameDst.clear();
		if_fail_fe(llc::append_strings(fileNameDst, fileNameSrc, '.'));
		if(iPart < 10)
			if_fail_fe(fileNameDst.push_back('0'));
		if_fail_fe(llc::append_strings(fileNameDst, llc::str(iPart)));
		info_printf("Creating part %u: '%s'.", iPart, fileNameDst.begin());
		cnst u2_t		countBytes = llc::min(sizePartMax, fileInMemory.size() - offsetPart);
		if_fail_fe(llc::fileFromMemory(fileNameDst, llc::vcs0_t{fileInMemory.begin() + offsetPart, countBytes}));
	}
	return (llc::err_t)countParts;
}

// Stream large files without retaining a complete part in memory.
sttc llc::err_t fileSplitLarge(llc::vcst_t fileNameSrc, u2_c sizePartMax, u3_c sizeFile) {
	llc::au0_t			buffer;
	cnst u3_t			countParts = sizeFile / sizePartMax + one_if(sizeFile % sizePartMax);
	llc::string			fileNameDst;
	u3_t				offset = 0;
	for(u2_t iPart = 0; iPart < countParts; ++iPart) {
		fileNameDst.clear();
		if_fail_fe(llc::append_strings(fileNameDst, fileNameSrc, '.'));
		if(iPart < 10)
			if_fail_fe(fileNameDst.push_back('0'));
		if_fail_fe(llc::append_strings(fileNameDst, llc::str(iPart)));
		cnst u3_t		partSize = llc::min(sizeFile - offset, (u3_t)sizePartMax);
		u3_t			partOffset = 0;
		while(partOffset < partSize) {
			cnst u2_t	count = (u2_t)llc::min(partSize - partOffset, (u3_t)(1024U * 1024U));
			if_fail_fe(llc::fileToMemory(fileNameSrc, buffer, count, offset));
			if_true_fef(buffer.size() != count, "Short read at offset %" LLC_FMT_U3 ": '%.*s'.", offset, LLC_VIEW_ARGS(fileNameSrc));
			if_fail_fe(llc::fileFromMemory(fileNameDst, buffer, partOffset != 0));
			partOffset += count;
			offset += count;
		}
	}
	return (llc::err_t)countParts;
}

// Splits a file into file.## parts; failure retains completed/partial output.
llc::err_t llc::fileSplit(vcst_t fileNameSrc, u2_c sizePartMax) {
	if_zero_fef(sizePartMax, "Invalid part size for '%.*s'.", LLC_VIEW_ARGS(fileNameSrc));
	stxp u2_c			sizeSmallFileMax = 3U * 1024U * 1024U * 1024U;
	cnst llc::string		fileName = fileNameSrc;
	cnst s3_t			sizeFile = llc::fileSize(fileName);
	if_fail_fef(sizeFile, "Failed to get size for '%.*s'.", LLC_VIEW_ARGS(fileNameSrc));
	if(0 == sizeFile)
		return 0;
	if_true_fef((u3_t)sizeFile / sizePartMax + one_if(sizeFile % sizePartMax) > 0x7FFFFFFFU, "Too many parts for '%.*s'.", LLC_VIEW_ARGS(fileNameSrc));
	return (sizeSmallFileMax > sizeFile) ? ::fileSplitSmall(fileName, sizePartMax) : ::fileSplitLarge(fileName, sizePartMax, (u3_t)sizeFile);
}
// Joins a file split into file.## parts.
llc::err_t	llc::	fileJoin				(vcst_t fileNameDst)	{
	string					fileNameSrc				= {};
	::llc::u2_t				iPart					= 0;
	if_fail_fe(llc::append_strings(fileNameSrc, fileNameDst, '.'));
	if_fail_fe(fileNameSrc.push_back('0'));
	if_fail_fe(llc::append_strings(fileNameSrc, llc::str(iPart)));
	::FILE					* fpDest					= 0;
	llc::string				finalPathName				= fileNameDst;
	if_fail_fef(llc::fopen_s(fpDest, finalPathName, LLC_OPEN_MODE_WRITE), "%s", finalPathName.begin());
	if_true_fef(0 == fpDest, "Failed to create file: %s.", finalPathName.begin());
	llc::apod<s0_t>			fileInMemory				= {};
	// Load each .split part and write it to the destionation file.
	while(0 == llc::fileToMemory(fileNameSrc, fileInMemory)) {	// Load first part and write it to the joined file.
		if_true_fef(fileInMemory.size() != fwrite(fileInMemory.begin(), 1, fileInMemory.size(), fpDest), "Write operation failed. Disk full? File size: %" LLC_FMT_U2 ". File name: %s.", fileInMemory.size(), fileNameSrc.begin());
		fileNameSrc.clear();
		++iPart;
		if_fail_fe(llc::append_strings(fileNameSrc, fileNameDst, '.'));
		if(iPart < 10)
			if_fail_fe(fileNameSrc.push_back('0'));
		if_fail_fe(llc::append_strings(fileNameSrc, llc::str(iPart)));
		fileInMemory.clear();
	}
	fclose(fpDest);
	return iPart;
}

#define LLC_DEBUG_FILE_CONTENTS

llc::err_t	llc::	fileToMemory			(vcst_t usfileName, llc::au0_t & fileInMemory, ::llc::u2_t maxSize, uint64_t offset)		{
	llc::string			fileName					= usfileName;
	llc_file_info_printf("Loading '%s'.", fileName.begin());

#ifdef LLC_ARDUINO
#	if defined(LLC_ESP32) || defined(ESP8266)
	FIX_SOC_PATH(fileName);
	File				fp					= getSoCFileSystem().open(fileName.begin(), LLC_OPEN_MODE_READ);
	if_true_fef(!fp, "Cannot open file: %s.", fileName.begin());
	u2_c				fileSize					= (::llc::u2_t)fp.size();
	fileInMemory.clear();
#		ifdef LLC_ESP32
	if_fail_fef(fileInMemory.resize(fileSize), "Too large to load in memory? File size: %" LLC_FMT_S2 ". Available memory: %" LLC_FMT_S2 ".", fp.size(), heap_caps_get_free_size(MALLOC_CAP_8BIT));
#		else
	if_fail_fef(fileInMemory.resize(fileSize), "Too large to load in memory? File size: %" LLC_FMT_S2 ". Available memory: %" LLC_FMT_S2 ".", fp.size(), umm_free_heap_size_lw());
#		endif // LLC_ESP32
	if_true_fe(fileSize != (::llc::u2_t)fp.readBytes((char*)fileInMemory.begin(), fileSize));
	fp.close();
#	endif // LLC_ESP32 || LLC_ESP8266
	llc::err_t	result				= 0;
#else // !LLC_ARDUINO
	::FILE				* fp				= 0;
	const int32_t				fileErr				= llc::fopen_s(fp, fileName, ::LLC_OPEN_MODE_READ);
	if_true_vef((fileErr > 0) ? -fileErr : fileErr, 0 != fileErr || 0 == fp, "Cannot open file: %s.", fileName.begin());
	fseek(fp, 0, SEEK_END);
	const s3_t				fileSize					= llc::ftell(fp);
	llc_fail_if_lt(LLC_FMT_LT_S3, fileSize, 0);
	llc_fail_if_ge(LLC_FMT_GE_U3, offset, (u3_t)fileSize);
	u2_c				maxRead				= (::llc::u2_t)min((s3_t)maxSize, fileSize - (s3_t)offset);
	llc_fail_if_ne(LLC_FMT_NE_S3, 0, llc::fseek(fp, offset, FSEEK_SET));
	llc::err_t	result				= 0;
	if_failed(fileInMemory.resize(maxRead)) {
		error_printf("File too large? : %llu.", (uint64_t)fileSize);
		result					= -1;
	}
	else {
		if(maxRead != fread(fileInMemory.begin(), szof(u0_t), maxRead, fp)) {
			error_printf("fread() failed! file: '%s'.", fileName.begin());
			result					= -1;
		}
	}
	fclose(fp);
#endif // LLC_ARDUINO

#ifdef LLC_DEBUG_FILE_CONTENTS
	llc_file_info_printf("'%s' loaded successfully. Size: %" LLC_FMT_U2 ":\n%.*s\n", fileName.begin(), fileInMemory.size(), (int)fileInMemory.size(), (cnst char*)fileInMemory.begin());
#else
	llc_file_info_printf("'%s' loaded successfully. Size: %" LLC_FMT_U2 ".", fileName.begin(), fileInMemory.size());
#endif // LLC_DEBUG_FILE_CONTENTS
	return result;
}

llc::err_t	llc::	fileFromMemory			(vcst_t usfileName, vcu0_t fileInMemory, bool append)	{
	llc::string			fileName					= usfileName;
#ifdef LLC_DEBUG_FILE_CONTENTS
	llc_file_info_printf("%s '%s':\n%.*s\n", append ? "Appending to" : "Writing", fileName.begin(), (int)fileInMemory.size(), (cnst char*)fileInMemory.begin());
#else
	llc_file_info_printf("%s '%s'.", append ? "Appending to" : "Writing", fileName.begin());
#endif // LLC_DEBUG_FILE_CONTENTS

	llc::err_t	result				= 0;
#ifdef LLC_ARDUINO
#	if defined(LLC_ESP32) || defined(ESP8266)
	FIX_SOC_PATH(fileName);
	File 				fp	 				= ::getSoCFileSystem().open(fileName.begin(), append ? LLC_OPEN_MODE_APPEND : LLC_OPEN_MODE_WRITE);
	if_true_fef(!fp, "Failed to open '%s'.", fileName.begin());
	if_true_fef(!fp.write(fileInMemory.begin(), fileInMemory.size()), "Failed to write to '%s'.", fileName.begin());
#	endif // LLC_ESP32 || LLC_ESP8266
#else // !LLC_ARDUINO
	::FILE				* fp				= 0;
	const int32_t				fileErr				= llc::fopen_s(fp, fileName, append ? LLC_OPEN_MODE_APPEND : LLC_OPEN_MODE_WRITE);
	if_true_vwf((fileErr > 0) ? -fileErr : fileErr, 0 != fileErr || 0 == fp, "Failed to create '%s' for %s.", fileName.begin(), append ? "appending" : "writing");
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
	llc::string 			fileName				= usfileName;
	llc_file_info_printf("Deleting '%s'.", fileName.begin());
#ifdef LLC_WINDOWS
	if_true_fef(FALSE == DeleteFileA(fileName.begin()), "Failed to delete '%s'.", fileName.begin());
#elif defined(LLC_ANDROID)
#elif defined(LLC_ARDUINO)
#	if defined(LLC_ESP32) || defined(ESP8266)
	FIX_SOC_PATH(fileName);
	if_true_fef(0 == ::getSoCFileSystem().remove(fileName.begin()), "Failed to delete '%s'.", fileName.begin());
#	elif defined(LLC_ATMEL)
#	endif // LLC_ESP32 || LLC_ESP8266
#else
	if_fail_fef(unlink(fileName.begin()), "Failed to delete '%s'.", fileName.begin());
#endif
	llc_file_info_printf("'%s' deleted successfully.", fileName.begin());
	return 0;
}
llc::err_t	llc::fileToMemory		(vcst_t folderPath, vcst_t fileName, llc::au0_t & fileBytes, ::llc::u2_t maxSize, uint64_t offset) {
	llc::string			filePath			= {};
	if_fail_fef(llc::pathNameCompose(folderPath, fileName, filePath), "folderPath: '%.*s', fileName: '%.*s'.", (int)folderPath.size(), folderPath.begin(), (int)fileName.size(), fileName.begin());
	if_fail_fef(llc::fileToMemory(filePath, fileBytes, maxSize, offset), "folderPath: '%.*s', fileName: '%.*s'.", (int)folderPath.size(), folderPath.begin(), (int)fileName.size(), fileName.begin());
	return 0;
}
llc::err_t	llc::fileFromMemory		(vcst_t folderPath, vcst_t fileName, vcu0_c fileInMemory, bool append) {
	llc::string			filePath			= {};
	if_fail_fef(llc::pathNameCompose(folderPath, fileName, filePath), "folderPath: '%.*s', fileName: '%.*s'.", (int)folderPath.size(), folderPath.begin(), (int)fileName.size(), fileName.begin());
	if_fail_fef(llc::fileFromMemory(filePath, fileInMemory, append), "folderPath: '%.*s', fileName: '%.*s', append: %s.", (int)folderPath.size(), folderPath.begin(), (int)fileName.size(), fileName.begin(), llc::b82sc(append));
	return 0;
}
