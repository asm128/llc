#include "llc_file_pack.h"
#include "llc_deflate.h"
#include "llc_path.h"
#include "llc_file.h"
#include "llc_aes.h"
#include "llc_apod_serialize.h"
#include "llc_string_compose.h"
//#include "llc_cstdio.h"

LLC_USING_TYPEINT();

llc::err_t			llc::folderUnpack			(SFolderInMemory & out_loaded, vcst_t nameFileSrc)					{
	au0_t				rawFileInMemory				= {};
	if_fail_fef(fileToMemory(nameFileSrc, rawFileInMemory), "Failed to load pak file: %.*s.", (int)nameFileSrc.size(), nameFileSrc.begin());
	if_fail_fef(folderUnpack(out_loaded, rawFileInMemory), "Failed to unpack pak file: %.*s.", (int)nameFileSrc.size(), nameFileSrc.begin());
	return 0;
}
llc::err_t			llc::folderToDisk			(const SFolderPackage & folderPackage, const vcst_t nameFileDst)			{
	const SPackHeader 	& fileHeader				= folderPackage.PackageInfo;
	const au0_t			& compressedTableFiles		= folderPackage.CompressedTableFiles		;
	const au0_t			& compressedContentsPacked	= folderPackage.CompressedContentsPacked	;
	{
		FILE						* fp						= 0;
		if_fail_fef(fopen_s(fp, nameFileDst, "wb"), "\"%.*s\"", (int)nameFileDst.size(), nameFileDst.begin());
		if_null_fef(fp, "Failed to create file: \"%.*s\"", (int)nameFileDst.size(), nameFileDst.begin());

		if(szof(SPackHeader) != fwrite(&fileHeader, 1, szof(SPackHeader), fp)) {
			error_printf("Failed to write package header: \"%.*s\".", (int)nameFileDst.size(), nameFileDst.begin());
			fclose(fp);
			return -1;
		}
		if(compressedTableFiles.size() != fwrite(compressedTableFiles.begin(), 1, compressedTableFiles.size(), fp)) {
			error_printf("Failed to write package file table: \"%.*s\".", (int)nameFileDst.size(), nameFileDst.begin());
			fclose(fp);
			return -1;
		}
		if(compressedContentsPacked.size() != fwrite(compressedContentsPacked.begin(), 1, compressedContentsPacked.size(), fp)) {
			error_printf("Failed to write package contents: \"%.*s\".", (int)nameFileDst.size(), nameFileDst.begin());
			fclose(fp);
			return -1;
		}
		if_true_fef(fclose(fp), "Failed to close package file: \"%.*s\".", (int)nameFileDst.size(), nameFileDst.begin());
	}
	return 0;
}

stxp u2_t			DEFLATE_CHUNK_SIZE			= u2_t(1024) * 1024 * 4;
stxp u2_t			INFLATE_CHUNK_SIZE			= u2_t(1024) * 1024 * 4;
llc::err_t			llc::folderPack				(SFolderPackage & output, const vcst_t nameFolderSrc) {
	SPackHeader 			& fileHeader			= output.PackageInfo = {};
	// -- The following two arrays store the file table and the file contents that are going to be compressed and stored on disk
	au0_t					tableFiles				;
	au0_t					contentsPacked			;

	cnst err_t				totalFileCount			= llc::folderLoad(nameFolderSrc, tableFiles, contentsPacked);
	if_fail_fe(totalFileCount);
	fileHeader.TotalFileCount					= (u2_t)totalFileCount;

	fileHeader.SizeUncompressedTableFiles		= tableFiles		.size();
	fileHeader.SizeUncompressedContentsPacked	= contentsPacked	.size();
	au0_t					& compressedTableFiles		= output.CompressedTableFiles		;
	au0_t					& compressedContentsPacked	= output.CompressedContentsPacked	;
	{	// compress
		if_fail_fef(llc::arrayDeflate(tableFiles		, compressedTableFiles		, ::DEFLATE_CHUNK_SIZE), "%s", "Unknown error.");
		if_fail_fef(llc::arrayDeflate(contentsPacked	, compressedContentsPacked	, ::DEFLATE_CHUNK_SIZE), "%s", "Unknown error.");
		fileHeader.SizeCompressedTableFiles		= compressedTableFiles		.size();
		fileHeader.SizeCompressedContentsPacked	= compressedContentsPacked	.size();
	}

	return 0;
}
llc::err_t			llc::folderUnpack			(SFolderInMemory & output, vcu0_t rawFileInMemory)		{
	if_true_fef(rawFileInMemory.size() < szof(SPackHeader), "Package header is truncated. Package size: %u.", rawFileInMemory.size());
	const SPackHeader	& header					= *(SPackHeader*)&rawFileInMemory[0];
	u2_t					offsetCompressed			= szof(SPackHeader);
	if_true_fef(header.SizeCompressedTableFiles > rawFileInMemory.size() - offsetCompressed, "Compressed file table exceeds package bounds. Table size: %u. Remaining: %u.", header.SizeCompressedTableFiles, rawFileInMemory.size() - offsetCompressed);
	const vcu0_t			compressedTableFiles		= {rawFileInMemory.begin() + offsetCompressed, header.SizeCompressedTableFiles};
	offsetCompressed							+= header.SizeCompressedTableFiles;
	if_true_fef(header.SizeCompressedContentsPacked > rawFileInMemory.size() - offsetCompressed, "Compressed contents exceed package bounds. Contents size: %u. Remaining: %u.", header.SizeCompressedContentsPacked, rawFileInMemory.size() - offsetCompressed);
	const vcu0_t			compressedContentsPacked	= {rawFileInMemory.begin() + offsetCompressed, header.SizeCompressedContentsPacked};
	if_fail_fe(output.Names	.resize(header.TotalFileCount));
	if_fail_fe(output.Contents	.resize(header.TotalFileCount));
	output.DataInfo		.clear();
	output.DataContents	.clear();
	if_fail_fe(llc::arrayInflate(compressedTableFiles		, output.DataInfo		, ::INFLATE_CHUNK_SIZE));
	if_fail_fe(llc::arrayInflate(compressedContentsPacked	, output.DataContents	, ::INFLATE_CHUNK_SIZE));
	if_true_fef(output.DataInfo.size() != header.SizeUncompressedTableFiles, "Unexpected file table size. Expected: %u. Actual: %u.", header.SizeUncompressedTableFiles, output.DataInfo.size());
	if_true_fef(output.DataContents.size() != header.SizeUncompressedContentsPacked, "Unexpected package contents size. Expected: %u. Actual: %u.", header.SizeUncompressedContentsPacked, output.DataContents.size());
	{ // Build access tables.
		u2_t					offsetInfo					= 0;
		for(u2_t iFile = 0; iFile < output.Names.size(); ++iFile) {
			if_true_fef(offsetInfo > output.DataInfo.size() || szof(rangeu2_t) > output.DataInfo.size() - offsetInfo, "Truncated file location at entry %u.", iFile);
			const rangeu2_t		& fileLocation				= *(const rangeu2_t*)&output.DataInfo[offsetInfo];
			offsetInfo				+= szof(rangeu2_t);
			if_true_fef(offsetInfo > output.DataInfo.size() || szof(u2_t) > output.DataInfo.size() - offsetInfo, "Truncated file name length at entry %u.", iFile);
			u2_c				lenName						= *(u2_t*)&output.DataInfo[offsetInfo];
			offsetInfo				+= szof(u2_t);
			if_true_fef(offsetInfo > output.DataInfo.size() || lenName > output.DataInfo.size() - offsetInfo, "File name exceeds table bounds at entry %u.", iFile);
			output.Names[iFile]	= {lenName ? (const char*)&output.DataInfo[offsetInfo] : 0, lenName};
			offsetInfo				+= lenName;
			if_true_fef(fileLocation.Offset > output.DataContents.size() || fileLocation.Count > output.DataContents.size() - fileLocation.Offset, "File contents exceed package bounds at entry %u.", iFile);
			output.Contents[iFile]	= {fileLocation.Count ? &output.DataContents[fileLocation.Offset] : 0, fileLocation.Count};
		}
		if_true_fef(offsetInfo != output.DataInfo.size(), "Unexpected bytes at the end of the file table. Parsed: %u. Table size: %u.", offsetInfo, output.DataInfo.size());
	}
	return 0;
}
llc::err_t			llc::folderLoad				(vcst_t nameFolderSrc, au0_t & tableFiles, au0_t & contentsPacked) {
	// -- The following two arrays store the file table and the file contents that are going to be compressed and stored on disk
	u2_t					totalFileCount			= 0;
	{
		aobj<string>	listFiles				= {};
		if_fail_fef(llc::pathList(nameFolderSrc, listFiles), "Failed to list folder: %.*s.", (int)nameFolderSrc.size(), nameFolderSrc.begin());

		au0_t				contentsTemp			= {};
		rangeu2_t			fileLocation			= {0, 0};
		for(u2_t iFile = 0; iFile < listFiles.size(); ++iFile) {
			fileLocation.Offset		= contentsPacked.size();
			cnst vcst_t			pathToLoad				= {listFiles[iFile].begin(), listFiles[iFile].size()};
			if(0 == pathToLoad.size())
				continue;

			info_printf("pathToLoad (%u): '%.*s'.", iFile, (int)pathToLoad.size(), pathToLoad.begin());
			if_fail_fef(llc::fileToMemory(pathToLoad, contentsTemp), "Failed to load file: %.*s.", (int)pathToLoad.size(), pathToLoad.begin());

			fileLocation.Count		= contentsTemp.size();
			if_fail_fe(llc::savePOD(tableFiles, fileLocation));
			if_fail_fe(llc::saveView(tableFiles, pathToLoad));
			if_fail_fef(contentsPacked.append(contentsTemp.begin(), contentsTemp.size())	, "Failed to append data bytes. Buffer sizes:\ntableFiles     : %u.\ncontentsPacked : %u.", tableFiles.size(), contentsPacked.size());
			contentsTemp.clear();

			++totalFileCount;
		}
	}
	return totalFileCount;
}
llc::err_t			llc::folderToDisk			(const SFolderInMemory & virtualFolder, vcst_t destinationPath)				{
	string				finalPathName				= {};
	FILE						* fp						= 0;
	for(u2_t iFile = 0, countFiles = virtualFolder.Names.size(); iFile < countFiles; ++iFile) {
		llc_safe_fclose(fp);
		const vcst_t			& fileName					= virtualFolder.Names		[iFile];
		vcu0_c			& fileContent				= virtualFolder.Contents	[iFile];
		finalPathName.clear();
		 if_fail_fe(append_strings(finalPathName, destinationPath, fileName));
		info_printf("File found (%u):'%s'. Size: %u.", iFile, finalPathName.begin(), fileContent.size());
		error_t				indexSlash					= findLastSlash(finalPathName);
		if(-1 != indexSlash) { // Create path if any specified.
			vcst_t				folderName					= {};
			if_fail_fe(finalPathName.slice(folderName, 0, (u2_t)indexSlash));
			if_fail_cef(pathCreate(folderName), "Failed to create folder: %.*s.", (int)folderName.size(), folderName.begin());
		}
		if_fail_fef(fopen_s(fp, finalPathName, "wb"), "%s", finalPathName.begin());
		if_null_cef(fp, "Failed to create file: %s.", finalPathName.begin());
		if_true_cef(fileContent.size() != fwrite(fileContent.begin(), 1, fileContent.size(), fp), "Failed to write file: %s. Disk full?", finalPathName.begin());
	}
	llc_safe_fclose(fp);
	return 0;
}
llc::err_t			llc::folderPackToDisk		(vcst_t nameFileDst	 , vcst_t nameFolderSrc)		{
	SFolderPackage		folderPackage;
	if_fail_fef(llc::folderPack(folderPackage, nameFolderSrc), "Failed to pack folder: %.*s.", (int)nameFolderSrc.size(), nameFolderSrc.begin());
	if_fail_fef(llc::folderToDisk(folderPackage, nameFileDst), "Failed to pack folder: %.*s.", (int)nameFolderSrc.size(), nameFolderSrc.begin());
	return 0;
}
llc::err_t			llc::folderUnpackToDisk		(vcst_t namePathDst	 , vcst_t nameFileSrc)		{
	SFolderInMemory		virtualFolder				= {};
	if_fail_fef(llc::folderUnpack(virtualFolder, nameFileSrc), "Failed to unpack file: %.*s.", (int)nameFileSrc.size(), nameFileSrc.begin());
	if_fail_fef(llc::folderToDisk(virtualFolder, namePathDst), "Failed to write folder to disk. Disk full or insufficient permissions. File name: %.*s. Destination path: %.*s.", (int)nameFileSrc.size(), nameFileSrc.begin(), (int)namePathDst.size(), namePathDst.begin());
	return 0;
}
llc::err_t			llc::inflateToMemory  		(au0_t & tempCache	 , vcst_t fileName, au0_t & output) {
	if_fail_fe(llc::fileToMemory(fileName, tempCache));
	info_printf("File size: %u.", tempCache.size());

	if_fail_fe(llc::arrayInflate(tempCache, output));
	info_printf("Inflated size: %u.", output.size());
	return 0;
}
llc::err_t			llc::deflateFromMemory		(au0_t & tempCache	 , vcst_t fileName, vcu0_t input) {
	info_printf("Input size: %u.", input.size());
	if_fail_fe(llc::arrayDeflate(input, tempCache));

	info_printf("Deflated size: %u.", tempCache.size());
	return fileFromMemory(fileName, tempCache);
}
llc::err_t			llc::fileFromMemorySecure	(SLoadCache & recycle, vcst_t fileName, vcu0_t key, b8_t deflate, vcu0_t blockBytes) {
	if(false == deflate && 0 == key.size())
		recycle.Encrypted		= blockBytes;
	else {
		if(false == deflate) {
			if_fail_fef(llc::aesEncode(blockBytes, key, AES_LEVEL_256, recycle.Encrypted), "Failed to encrypt file: %.*s.", (int)fileName.size(), fileName.begin());
		}
		else if(0 == key.size()) {
			if_fail_fef(llc::arrayDeflate(blockBytes, recycle.Encrypted), "Failed to deflate file: %.*s.", (int)fileName.size(), fileName.begin());
		}
		else {
			if_fail_fef(llc::arrayDeflate(blockBytes, recycle.Deflated), "Failed to deflate file: %.*s.", (int)fileName.size(), fileName.begin());
			if_fail_fef(llc::aesEncode(recycle.Deflated, key, AES_LEVEL_256, recycle.Encrypted), "Failed to encrypt file: %.*s.", (int)fileName.size(), fileName.begin());
		}
	}
	if_fail_fe(llc::crcGenerateAndAppend(recycle.Encrypted));
	if_fail_fef(llc::fileFromMemory(fileName, recycle.Encrypted), "Failed to save file: %.*s.", (int)fileName.size(), fileName.begin());
	return 0;
}
llc::err_t			llc::fileToMemorySecure		(SLoadCache & recycle, vcst_t fileName, vcu0_t key, const b8_t deflate, au0_t & loadedBytes)								{
	if(false == deflate && 0 == key.size()) {
		if_fail_fef(llc::fileToMemory(fileName, loadedBytes), "Failed to read file: %.*s.", (int)fileName.size(), fileName.begin());
		if_fail_fe(llc::crcVerifyAndRemove(loadedBytes));
	}
	else {
		if_fail_fef(llc::fileToMemory(fileName, recycle.Encrypted), "Failed to read file: %.*s.", (int)fileName.size(), fileName.begin());
		if_fail_fe(llc::crcVerifyAndRemove(recycle.Encrypted));
		if(false == deflate) {
			if_fail_fef(llc::aesDecode(recycle.Encrypted, key, AES_LEVEL_256, loadedBytes), "Failed to decrypt file: %.*s.", (int)fileName.size(), fileName.begin());
		}
		else if(0 == key.size()) {
			if_fail_fef(llc::arrayInflate(recycle.Encrypted, loadedBytes), "Failed to inflate file: %.*s.", (int)fileName.size(), fileName.begin());
		}
		else {
			if_fail_fef(llc::aesDecode(recycle.Encrypted, key, AES_LEVEL_256, recycle.Deflated), "Failed to decrypt file: %.*s.", (int)fileName.size(), fileName.begin());
			if_fail_fef(llc::arrayInflate(recycle.Deflated, loadedBytes), "Failed to inflate file: %.*s.", (int)fileName.size(), fileName.begin());
		}
	}
	return 0;
}
