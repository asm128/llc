#include "llc_path.h"
#include "llc_file.h"
#include "llc_string_compose.h"
#include "llc_string.h"
#include "llc_timer.h"


LLC_USING_TYPEINT();
LLC_USING_APOD();
LLC_USING_VIEW();

#pragma pack(push, 1)
struct FileAttributes {
    uint32_t ReadOnly           : 1;  // 0x00000001  Prevents modification through normal file operations
    uint32_t Hidden             : 1;  // 0x00000002  Normally omitted from directory listings
    uint32_t System             : 1;  // 0x00000004  Identifies a file used by the operating system
    uint32_t Reserved0          : 1;  // 0x00000008  Reserved by Windows
    uint32_t Directory          : 1;  // 0x00000010  Entry represents a directory rather than a file
    uint32_t Archive            : 1;  // 0x00000020  Marks the file as changed since last backup
    uint32_t Device             : 1;  // 0x00000040  Reserved; historically used to identify device files
    uint32_t Normal             : 1;  // 0x00000080  No other attributes are set
    uint32_t Temporary          : 1;  // 0x00000100  Indicates temporary data; filesystem may avoid writing it to permanent storage
    uint32_t SparseFile         : 1;  // 0x00000200  File may contain large zero-filled regions that consume little/no disk space
    uint32_t ReparsePoint       : 1;  // 0x00000400  File has filesystem-specific metadata that changes how Windows accesses it
    uint32_t Compressed         : 1;  // 0x00000800  File data is transparently compressed by the filesystem
    uint32_t Offline            : 1;  // 0x00001000  File data is not immediately available locally and may need retrieval
    uint32_t NotContentIndexed  : 1;  // 0x00002000  Windows Search should not index the file's contents
    uint32_t Encrypted          : 1;  // 0x00004000  File data is transparently encrypted by the filesystem
    uint32_t IntegrityStream    : 1;  // 0x00008000  Filesystem maintains integrity information to detect/correct corruption
    uint32_t Virtual            : 1;  // 0x00010000  Reserved for system use
    uint32_t NoScrubData        : 1;  // 0x00020000  Storage integrity scrubber should not validate this file's data
    uint32_t ExtendedAttribute  : 1;  // 0x00040000  File has extended attributes associated with it
    uint32_t Pinned             : 1;  // 0x00080000  Cloud/storage provider should keep the file locally available
    uint32_t Unpinned           : 1;  // 0x00100000  Cloud/storage provider should not guarantee local availability
    uint32_t Reserved1          : 1;  // 0x00200000  Reserved by Windows
    uint32_t RecallOnOpen       : 1;  // 0x00400000  File data may be recalled from remote storage when the file is opened
    uint32_t RecallOnDataAccess : 1;  // 0x00800000  File data may be recalled from remote storage when its contents are accessed
    uint32_t Padding            : 8;  // Remaining unused bits
};
#pragma pack(pop)

// FILE_ATTRIBUTE_READONLY (0x1): The file is read-only. Applications can read the file but cannot write to it or delete it.
// FILE_ATTRIBUTE_HIDDEN (0x2): The file is hidden and not included in an ordinary directory listing.
// FILE_ATTRIBUTE_SYSTEM (0x4): The file is part of, or is used exclusively by, the operating system.
// FILE_ATTRIBUTE_DIRECTORY (0x10): The item is a directory (folder) rather than a file.
// FILE_ATTRIBUTE_ARCHIVE (0x20): The file should be archived. Windows sets this flag whenever a file is created or modified.
// FILE_ATTRIBUTE_NORMAL (0x80): The file has no other attributes set. This flag is only valid if it is used alone.
// FILE_ATTRIBUTE_REPARSE_POINT (0x400): The file or directory has an associated reparse point, or is a symbolic link/junction point.

llc::err_t listFolder   (llc::vcst_t path, bool recursiveWalk, llc::function<llc::err_t(const WIN32_FIND_DATAA&, llc::vcst_c)> callback) {
    WIN32_FIND_DATAA    data    = {};
	llc::rtrim(path, path, "/\\");
	llc::string 		 pathStr;
    if_fail_fe(llc::append_strings(pathStr, path, "/*"));
    HANDLE              hFind   = FindFirstFileExA(pathStr, FindExInfoBasic, &data, FindExSearchNameMatch, 0, 0);
    if_true_vif(0, hFind == INVALID_HANDLE_VALUE, "No files found in \"%s\"", path.begin());

    uint32_t            filesFound = 0;
    do {
        verbose_printf(
            "\ndwFileAttributes   (DWORD)    : %u"
            "\nftCreationTime     (FILETIME) : %llu"
            "\nftLastAccessTime   (FILETIME) : %llu"
            "\nftLastWriteTime    (FILETIME) : %llu"
            "\nnFileSizeHigh      (DWORD)    : %u"
            "\nnFileSizeLow       (DWORD)    : %u"
            "\ndwReserved0        (DWORD)    : %u"
            "\ndwReserved1        (DWORD)    : %u"
            "\ncFileName          (CHAR[%u]) : \"%s\""
            "\ncAlternateFileName (CHAR[14]) : \"%s\""
            , data.dwFileAttributes
            , *(const uint64_t*)&data.ftCreationTime
            , *(const uint64_t*)&data.ftLastAccessTime
            , *(const uint64_t*)&data.ftLastWriteTime
            , data.nFileSizeHigh
            , data.nFileSizeLow
            , data.dwReserved0
            , data.dwReserved1
            , MAX_PATH, data.cFileName
            , data.cAlternateFileName
            );
        SYSTEMTIME st = {};
        FileTimeToSystemTime(&data.ftCreationTime   , &st); verbose_printf("ftCreationTime   : %04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
        FileTimeToSystemTime(&data.ftLastAccessTime , &st); verbose_printf("ftLastAccessTime : %04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
        FileTimeToSystemTime(&data.ftLastWriteTime  , &st); verbose_printf("ftLastWriteTime  : %04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		if(callback) {
			if_fail_ef(callback(data, path), "Callback failed for file \"%s\".", data.cFileName);
		}
        if(recursiveWalk && (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && '.' != data.cFileName[0]) {
			pathStr.clear();
			if_fail_fe(llc::append_strings(pathStr, path, "/", data.cFileName));
			if_fail_bef(listFolder(pathStr, recursiveWalk, callback), "Failed to list folder \"%s\".", pathStr.begin());
        }
        ++filesFound;
    }
    while(FindNextFileA(hFind, &data));
    FindClose(hFind);

    rtrn filesFound;
}

struct SFileInfo {
	llc::string    path;
	llc::string    name;
	uint64_t       size;
	uint64_t       creationTime;
};
    struct SFileInfoPair {
        SFileInfo fileA;
        SFileInfo fileB;
    };

int    compareFileContents(const SFileInfoPair& pair) {
	struct SAutoCloseFile {
		FILE* Handle = nullptr;
		~SAutoCloseFile() { if(Handle) fclose(Handle); }
	};
	stxp u2_c       COMPARISON_CHUNK_SIZE = 1024 * 1024 * 256; 
	info_printf("\nComparing file contents for: " 
        "\n%s/%s" 
        "\n%s/%s"
        "\nFile  size: %llu bytes."
        "\nChunk size: %llu bytes."
        , pair.fileA.path.begin(), pair.fileA.name.begin(), pair.fileB.path.begin(), pair.fileB.name.begin()
        , pair.fileA.size, (uint64_t)COMPARISON_CHUNK_SIZE);
	static llc::au0_t      bufferA, bufferB;
    llc::resize(COMPARISON_CHUNK_SIZE, bufferA, bufferB);
    SAutoCloseFile  fileA = {}, fileB = {};
    llc::string     filePathA, filePathB;
    llc::append_strings(filePathA, pair.fileA.path, "/", pair.fileA.name);
    llc::append_strings(filePathB, pair.fileB.path, "/", pair.fileB.name);
	if_true_fwf(0 != fopen_s(&fileA.Handle, filePathA, "rb"), "Failed to open file: \"%s\"", filePathA.begin());
	if_true_fwf(0 != fopen_s(&fileB.Handle, filePathB, "rb"), "Failed to open file: \"%s\"", filePathB.begin());
    llc::STimer     timer           = {};
	u3_t            remainingBytes  = pair.fileA.size;
	while(remainingBytes) {
		u3_c            bytesToRead = (remainingBytes > COMPARISON_CHUNK_SIZE) ? COMPARISON_CHUNK_SIZE : remainingBytes;
		if_true_fe(bytesToRead != fread(bufferA.begin(), 1, bytesToRead, fileA.Handle));
		if_true_fe(bytesToRead != fread(bufferB.begin(), 1, bytesToRead, fileB.Handle));
		if_true_vi(1, memcmp(bufferA.begin(), bufferB.begin(), bytesToRead)); // Files are different
		remainingBytes -= bytesToRead;
        timer.Frame();
        info_printf("Chunk comparison execution time: %f seconds.", timer.LastTimeMicroseconds * 0.000001);
	}
    return 0; // Files are identical
}

llc::err_t moveFile(const SFileInfo & fileToMove, llc::vcst_c & targetFolder) {
    llc::string         oldPath         = {}
        ,               newPath         = {};
	if_fail_fe(llc::append_strings(oldPath, fileToMove.path, "/", fileToMove.name));
    llc::vstr_t         oldPathView     = fileToMove.path; 
	llc::ltrim(oldPathView, oldPathView, "\\/");
    s2_t                colonOffset     = oldPathView.find(':', 0);
    if(colonOffset >= 0)
		oldPathView[colonOffset] = '_';
	if_fail_fe(llc::append_strings(newPath, targetFolder, '/', oldPathView.cc(), '/'));
	if_fail_wf(llc::pathCreate(newPath, '/'), "Failed to create directory \"%s\"", newPath.begin());
	if_fail_fe(llc::append_strings(newPath, fileToMove.name));
    info_printf("Moving duplicated file: \"%s\" to \"%s\"", oldPath.begin(), newPath.begin());
	if_zero_fwf(MoveFileExA(oldPath.begin(), newPath.begin(), MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH)
        , "Failed to move duplicated file \"%s\" to \"%s\"", oldPath.begin(), newPath.begin());
    return 0;
}

int main() {
    
    static_assert(sizeof(FileAttributes) == 4, "Must be exactly 4 bytes");

    llc::vcst_t		        pathToProcess	= LLC_CXS("D:/");
    llc::aobj<SFileInfo>    largeFiles;
    ::listFolder(pathToProcess, true, [&largeFiles](const WIN32_FIND_DATAA & entryData, llc::vcst_t folderPath){ 
        if(entryData.nFileSizeLow < 0xFFFFF) 
            return 0; 
        SFileInfo           newInfo         = {};
        newInfo.size            = ((uint64_t)entryData.nFileSizeHigh << 32) | entryData.nFileSizeLow;
        newInfo.creationTime    = *(const uint64_t*)&entryData.ftCreationTime;
        verbose_printf("Large file found: %s. Size: %llu. Creation Time: %llu", entryData.cFileName, newInfo.size, newInfo.creationTime);
        if_fail_fe(llc::append_strings(newInfo.path, folderPath));
        if_fail_fe(llc::append_strings(newInfo.name, entryData.cFileName));
        if_fail_fe(largeFiles.push_back(newInfo));
        return 1;
        });
    info_printf("Total large files found: %u", largeFiles.size());
    llc::aobj<SFileInfoPair> potentiallyDuplicatedFiles;
	if(largeFiles.size() > 1) {
		info_printf("Comparing large files...");
        for(uint32_t iFile0 = 0; iFile0 < largeFiles.size() - 1; ++iFile0) {
            for(uint32_t iFile1 = iFile0 + 1; iFile1 < largeFiles.size(); ++iFile1) {
                if(largeFiles[iFile0].size == largeFiles[iFile1].size) {
                    verbose_printf("Found possibly duplicated large files: %s/%s and %s/%s"
                        , largeFiles[iFile0].path.begin(), largeFiles[iFile0].name.begin()
                        , largeFiles[iFile1].path.begin(), largeFiles[iFile1].name.begin()
                    );
                    SFileInfoPair pair = { largeFiles[iFile0], largeFiles[iFile1] };
                    potentiallyDuplicatedFiles.push_back(pair);
                }
            }
        }
	}
    info_printf("Total potentially duplicated large files found: %u", potentiallyDuplicatedFiles.size());
    stxp llc::vcst_t    targetFolder    = LLC_CXS("E:/Duplicated");
    llc::STimer         timer;
    for(const auto& pair : potentiallyDuplicatedFiles) {
        verbose_printf("Processing potentially duplicated files with size %llu: %s/%s and %s/%s"
            , pair.fileA.size
            , pair.fileA.path.begin(), pair.fileA.name.begin()
            , pair.fileB.path.begin(), pair.fileB.name.begin()
        );
		if(0 == compareFileContents(pair))
			verbose_printf("Files are different: %s/%s and %s/%s"
				, pair.fileA.path.begin(), pair.fileA.name.begin()
				, pair.fileB.path.begin(), pair.fileB.name.begin()
			);
        else {
			info_printf("Files are identical: %s/%s and %s/%s"
				, pair.fileA.path.begin(), pair.fileA.name.begin()
				, pair.fileB.path.begin(), pair.fileB.name.begin()
			);
            const b8_t          fileToMoveIsFileB 
                = (pair.fileA.name.size() < pair.fileB.name.size())
                //|| ((pair.fileB.name.size() == 13) && (0 == memcmp(pair.fileB.name.begin(), "177", 3))) 
                ;
			const SFileInfo     & fileToMove    = fileToMoveIsFileB ? pair.fileB : pair.fileA;
			if_fail_wf(moveFile(fileToMove, targetFolder), "Failed to move file \"%s\".", fileToMove.name.begin());

		}
        timer.Frame();
        info_printf("compareFileContents execution time: %f seconds.", timer.LastTimeMicroseconds * 0.000001);
    }
    //if_fail_ef(::codeProcessPath(pathToProcess), "Failed to process \"%s\".", pathToProcess.begin());

    return 0;
}

