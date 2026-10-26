#include "llc_cstdio.h"
#include "llc_string.h"

#ifndef LLC_FILE_H_23627
#define LLC_FILE_H_23627

namespace llc
{
	err_t			with				(vcst_t filepath, vcst_t mode, const function<err_t(FILE*&)> & funcFile);	//

	err_t			fileJoin			(vcst_t fileNameDst);								// Joins a file split into file.split.## parts.
	err_t			fileSplit			(vcst_t fileNameSrc, u2_c partSize);		// Writes file.0, file.1, ...; returns part count. Failure retains completed/partial parts.
	s3_t			fileSize			(vcst_t fileName);
	err_t			fileLastWriteTime	(vcst_t fileName, s3_t & modified);
	err_t			fileDelete			(vcst_t fileName);

	err_t			fileFromMemory		(vcst_t fileName, vcu0_t fileInMemory, bool append = false);
	err_t			fileToMemory		(vcst_t fileName, au0_t & fileInMemory, ::llc::u2_t maxSize = 0xFFFFFFFFU, uint64_t offset = 0);
	err_t			fileToMemory		(vcst_t folderPath, vcst_t fileName, au0_t & fileBytes, ::llc::u2_t maxSize = 0xFFFFFFFFU, uint64_t offset = 0);
	err_t			fileFromMemory		(vcst_t folderPath, vcst_t fileName, vcu0_t fileInMemory, bool append = false);

	stin	err_t	fileToMemory		(vcst_t fileName, as0_t & fileInMemory, ::llc::u2_t maxSize = 0xFFFFFFFFU, uint64_t offset = 0)	{ rtrn fileToMemory		(fileName, *(au0_t*)&fileInMemory, maxSize, offset); }
	stin	err_t	fileToMemory		(vcst_t fileName, string & fileInMemory, ::llc::u2_t maxSize = 0xFFFFFFFFU, uint64_t offset = 0)	{ rtrn fileToMemory		(fileName, *(au0_t*)&fileInMemory, maxSize, offset); }
	stin	err_t	fileFromMemory		(vcst_t fileName, vcs0_t fileInMemory, bool append = false)									{ rtrn fileFromMemory	(fileName, fileInMemory.cu8(), append); }
	stin	err_t	fileFromMemory		(vcst_t fileName, vcst_t fileInMemory, bool append = false)									{ rtrn fileFromMemory	(fileName, fileInMemory.cu8(), append); }

	GDEFINE_ENUM_TYPE(OPEN_MODE, i2u_t);
	GDEFINE_ENUM_VALUE(OPEN_MODE, CLOSE		, 0);
	GDEFINE_ENUM_VALUE(OPEN_MODE, READ		, 'r' + (i1u_t('b') << 8));							//  1
	GDEFINE_ENUM_VALUE(OPEN_MODE, WRITE		, 'w' + (i1u_t('b') << 8));							//  2
	GDEFINE_ENUM_VALUE(OPEN_MODE, APPEND	, 'a' + (i2u_t('b') << 8) + (i2u_t('+') << 16));	//  3
} // namespace

#endif // LLC_FILE_H_23627
