#include "llc_path.h"
#include "llc_file.h"

#include "llc_test_core.h"

#include <filesystem>

GDEFINE_ENUM_TYPE(PATH_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, OK						, 0, "All path tests passed.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, LAST_SLASH_NONE			, 1, "findLastSlash() did not report that the path contains no separator.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, LAST_SLASH_POSITION		, 2, "findLastSlash() did not return the last mixed-separator position.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, LAST_SLASH_TRAILING		, 3, "findLastSlash() did not return a trailing separator position.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, COMPOSE_TEXT				, 4, "pathNameCompose() produced unexpected path text.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, COMPOSE_DUPLICATE_SEPARATOR, 5, "pathNameCompose() did not reject adjacent separators within an input component.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, COMPOSE_APPEND			, 6, "pathNameCompose() did not append to the caller output.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, COMPOSE_EMPTY				, 7, "pathNameCompose() changed an empty composition.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, COMPOSE_RETURN			, 8, "pathNameCompose() did not return the resulting output size.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, COMPOSE_TERMINATOR		, 9, "pathNameCompose() did not preserve the output terminator.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, COMPOSE_SEPARATOR			, 10, "pathNameCompose() did not normalize path separators.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, COMPOSE_BOUNDARY			, 11, "pathNameCompose() did not collapse separators at the component boundary.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, COMPOSE_PATH_ONLY			, 12, "pathNameCompose() changed a path composed without a filename.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, COMPOSE_ROOT				, 13, "pathNameCompose() did not preserve the path root.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, COMPOSE_FAILURE_PRESERVE	, 14, "pathNameCompose() changed caller output after rejecting invalid input.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_EMPTY			, 15, "pathNormalize() did not normalize an empty path.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_SEPARATOR		, 16, "pathNormalize() did not normalize path separators.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_DOT				, 17, "pathNormalize() did not remove current-directory segments.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_PARENT			, 18, "pathNormalize() did not resolve parent-directory segments.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_ROOT				, 19, "pathNormalize() did not preserve an absolute root.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_UNC				, 20, "pathNormalize() did not preserve a UNC root.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_DRIVE			, 21, "pathNormalize() did not preserve drive-path semantics.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_TRAILING		, 22, "pathNormalize() did not remove a non-root trailing separator.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_REPLACE			, 23, "pathNormalize() appended to output instead of replacing it.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_RETURN			, 24, "pathNormalize() did not return the normalized path size.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_TERMINATOR		, 25, "pathNormalize() did not preserve the output terminator.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_INVALID			, 26, "pathNormalize() did not reject invalid input.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_FAILURE_PRESERVE, 27, "pathNormalize() changed caller output after rejecting invalid input.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, ABSOLUTE_CURRENT			, 28, "pathAbsolute() did not resolve the current directory.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, ABSOLUTE_RELATIVE			, 29, "pathAbsolute() did not resolve and normalize a relative path.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, ABSOLUTE_NONEXISTENT		, 30, "pathAbsolute() required the target path to exist.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, ABSOLUTE_IDEMPOTENT		, 31, "pathAbsolute() changed an already absolute normalized path.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, ABSOLUTE_SEPARATOR		, 32, "pathAbsolute() did not honor the requested output separator.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, ABSOLUTE_RETURN			, 33, "pathAbsolute() did not return the resolved path size.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, ABSOLUTE_TERMINATOR		, 34, "pathAbsolute() did not preserve the output terminator.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, ABSOLUTE_INVALID			, 35, "pathAbsolute() did not reject invalid input.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, ABSOLUTE_FAILURE_PRESERVE	, 36, "pathAbsolute() changed caller output after rejecting invalid input.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, LIST_CALLBACK_RESULT		, 37, "Recursive pathList() with a callback failed.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, LIST_CALLBACK_RECURSIVE	, 38, "Recursive pathList() did not forward its callback to nested folders.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, LIST_CALLBACK_COUNTS		, 39, "Recursive pathList() callback reported unexpected file or folder counts.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, LIST_CALLBACK_TREE		, 40, "Recursive pathList() with a callback produced an unexpected tree.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, LIST_PLAIN_RESULT			, 41, "Non-callback pathList() traversal failed.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, BEGIN_TEXT					, 42, "pathBegin() produced an unexpected protected prefix.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, BEGIN_RETURN				, 43, "pathBegin() did not return the protected prefix size.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, DIRECTORY_TEXT				, 44, "pathDirectory() produced an unexpected directory slice.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, FILENAME_TEXT				, 45, "pathFilename() produced an unexpected filename slice.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, STEM_TEXT					, 46, "pathStem() produced an unexpected stem slice.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, LIST_EXTENSION_FILTER		, 47, "Recursive pathList() ignored its extension filter.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, ABSOLUTE_CURRENT_PATH		, 48, "pathAbsolute() did not produce an absolute current-directory path.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, ABSOLUTE_RELATIVE_RETURN	, 49, "pathAbsolute() returned the wrong relative-path result size.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, ABSOLUTE_IDEMPOTENT_RETURN, 50, "pathAbsolute() returned the wrong absolute-path result size.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, ABSOLUTE_RELATIVE_TERMINATOR, 51, "pathAbsolute() omitted the relative-path output terminator.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, ABSOLUTE_IDEMPOTENT_TERMINATOR, 52, "pathAbsolute() omitted the absolute-path output terminator.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, LIST_CALLBACK_FOLDER_COUNT, 53, "Recursive pathList() callback reported the wrong folder count.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, LIST_CALLBACK_TREE_FOLDER_COUNT, 54, "Recursive callback pathList() tree contained the wrong folder count.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, LIST_TREE_FILE_EQUIVALENT, 55, "Callback and non-callback pathList() trees disagreed on file count.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, LIST_TREE_FOLDER_EQUIVALENT, 56, "Callback and non-callback pathList() trees disagreed on folder count.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, LIST_EXTENSION_FOLDER_COUNT, 57, "Extension-filtered pathList() tree contained the wrong folder count.");

tydf ::llc::err_t (*TFileToString)(::llc::vcst_t, ::llc::string &, uint32_t, uint64_t);
tydf ::llc::err_t (*TFileFromBytes)(::llc::vcst_t, ::llc::vcs0_t, bool);
tydf ::llc::err_t (*TFileFromString)(::llc::vcst_t, ::llc::vcst_t, bool);
tydf ::llc::err_t (*TPathCreate)(::llc::vcst_t, ::llc::sc_c);
tydf ::llc::err_t (*TPathListOwned)(cnst ::llc::SPathContents &, ::llc::aobj<::llc::string> &, ::llc::vcst_t);
tydf ::llc::err_t (*TPathListViews)(cnst ::llc::SPathContents &, ::llc::aobj<::llc::vcst_t> &, ::llc::vcst_t);
tydf ::llc::err_t (*TPathListTree)(::llc::vcst_t, ::llc::SPathContents &, ::llc::vcst_t);
tydf ::llc::err_t (*TPathListRecursive)(::llc::vcst_t, ::llc::aobj<::llc::string> &, ::llc::vcst_t);
tydf ::llc::err_t (*TPathCompose)(::llc::vcst_t, ::llc::vcst_t, ::llc::string &);
tydf ::llc::err_t (*TPathTransform)(::llc::vcst_t, ::llc::string &, ::llc::sc_c);
tydf ::llc::err_t (*TPathSlice)(::llc::vcst_t, ::llc::vcst_t &);
tydf ::llc::err_t (*TPathFindSlash)(::llc::vcst_t);

static_assert(::llc::is_cnst<::llc::vcst_t::T>::Value, "Path string views must expose const characters.");
static_assert(::llc::is_cnst<::llc::vcs0_t::T>::Value, "File byte views must expose const bytes.");
static_assert(requires {
	static_cast<TFileToString>(&::llc::fileToMemory);
	static_cast<TFileFromBytes>(&::llc::fileFromMemory);
	static_cast<TFileFromString>(&::llc::fileFromMemory);
	}, "File memory operations must preserve their string and const-view contracts.");
static_assert(requires {
	static_cast<TPathCreate>(&::llc::pathCreate);
	static_cast<TPathListOwned>(&::llc::pathList);
	static_cast<TPathListViews>(&::llc::pathList);
	static_cast<TPathListTree>(&::llc::pathList);
	static_cast<TPathListRecursive>(&::llc::pathList);
	}, "Path listing must preserve its owned and borrowed output contracts.");
static_assert(requires {
	static_cast<TPathCompose>(&::llc::pathNameCompose);
	static_cast<TPathTransform>(&::llc::pathNormalize);
	static_cast<TPathTransform>(&::llc::pathAbsolute);
	static_cast<TPathSlice>(&::llc::pathBegin);
	static_cast<TPathSlice>(&::llc::pathDirectory);
	static_cast<TPathSlice>(&::llc::pathFilename);
	static_cast<TPathSlice>(&::llc::pathStem);
	static_cast<TPathFindSlash>(&::llc::findLastSlash);
	}, "Path operations must preserve their string-view contracts.");

stct SPathSlashCase {
	::llc::vcst_t		Path;
	::llc::err_t		Expected;
	PATH_TEST_RESULT	Result;
};

stct SPathComposeCase {
	::llc::vcst_t		Prefix;
	::llc::vcst_t		Path;
	::llc::vcst_t		FileName;
	::llc::vcst_t		Expected;
	PATH_TEST_RESULT	Result;
};

stct SPathNormalizeCase {
	::llc::vcst_t		Path;
	::llc::vcst_t		Expected;
	::llc::sc_t		Separator;
	PATH_TEST_RESULT	Result;
};

stct SPathBeginCase {
	::llc::vcst_t		Path;
	::llc::vcst_t		Expected;
};

stct SPathPartsCase {
	::llc::vcst_t		Path		= {};
	::llc::vcst_t		Directory	= {};
	::llc::vcst_t		Filename	= {};
	::llc::vcst_t		Stem		= {};
};

sttc bool pathTextMismatch(::llc::vcst_t actual, ::llc::vcst_t expected) {
	rtrn actual.size() != expected.size() || (actual.size() && memcmp(actual.begin(), expected.begin(), actual.size()));
}

stct SPathListCounts {
	::llc::u2_t		Files		= 0;
	::llc::u2_t		Folders		= 0;
};

stct SPathListFixture {
	::llc::vcst_t			Root		= LLC_CXS("./llc_test_path_list");
						~SPathListFixture	() { std::error_code error; std::filesystem::remove_all(Root.begin(), error); }
};

sttc SPathListCounts pathListCounts(const ::llc::SPathContents & pathContents) {
	SPathListCounts		counts		= {pathContents.Files.size(), pathContents.Folders.size()};
	for(cnst ::llc::SPathContents & child : pathContents.Folders) {
		cnst SPathListCounts	childCounts	= ::pathListCounts(child);
		counts.Files		+= childCounts.Files;
		counts.Folders		+= childCounts.Folders;
	}
	rtrn counts;
}

sttc ::llc::err_t testFindLastSlash(ATestError & errors) {
	cnst SPathSlashCase cases[] =
		{ {LLC_CXS("")						, -1, PATH_TEST_RESULT_LAST_SLASH_NONE}
		, {LLC_CXS("file.txt")				, -1, PATH_TEST_RESULT_LAST_SLASH_NONE}
		, {LLC_CXS("folder/file.txt")		,  6, PATH_TEST_RESULT_LAST_SLASH_POSITION}
		, {LLC_CXS("folder\\file.txt")		,  6, PATH_TEST_RESULT_LAST_SLASH_POSITION}
		, {LLC_CXS("root/child\\file.txt")	, 10, PATH_TEST_RESULT_LAST_SLASH_POSITION}
		, {LLC_CXS("/file.txt")				,  0, PATH_TEST_RESULT_LAST_SLASH_POSITION}
		, {LLC_CXS("folder/")				,  6, PATH_TEST_RESULT_LAST_SLASH_TRAILING}
		, {LLC_CXS("folder\\")				,  6, PATH_TEST_RESULT_LAST_SLASH_TRAILING}
		};
	for(cnst SPathSlashCase & testCase : cases) {
		cnst ::llc::err_t actual = ::llc::findLastSlash(testCase.Path);
		LLC_TEST_CHECK(errors, testCase.Result, actual != testCase.Expected
			, "Path:'%.*s' returned:%" LLC_FMT_S2 ", expected:%" LLC_FMT_S2 "."
			, (int)testCase.Path.size(), testCase.Path.begin(), actual, testCase.Expected
			);
	}
	cnst SPathPartsCase partCases[] =
		{ {LLC_CXS("file.txt")					, LLC_CXS("")			, LLC_CXS("file.txt")			, LLC_CXS("file")}
		, {LLC_CXS("folder/archive.test.cpp")	, LLC_CXS("folder/")	, LLC_CXS("archive.test.cpp")	, LLC_CXS("archive.test")}
		, {LLC_CXS("C:\\folder\\archive.test.cpp"), LLC_CXS("C:\\folder\\"), LLC_CXS("archive.test.cpp")	, LLC_CXS("archive.test")}
		, {LLC_CXS("/file")						, LLC_CXS("/")		, LLC_CXS("file")				, LLC_CXS("file")}
		, {LLC_CXS("README")						, LLC_CXS("")			, LLC_CXS("README")			, LLC_CXS("README")}
		, {LLC_CXS(".profile")					, LLC_CXS("")			, LLC_CXS(".profile")			, LLC_CXS(".profile")}
		};
	for(cnst SPathPartsCase & testCase : partCases) {
		::llc::vcst_t directory = {};
		::llc::vcst_t filename  = {};
		::llc::vcst_t stem      = {};
		if_fail_fe(::llc::pathDirectory(testCase.Path, directory));
		if_fail_fe(::llc::pathFilename(testCase.Path, filename));
		if_fail_fe(::llc::pathStem(testCase.Path, stem));
		LLC_TEST_CHECK(errors, PATH_TEST_RESULT_DIRECTORY_TEXT, pathTextMismatch(directory, testCase.Directory), "Path:'%.*s'.", (int)testCase.Path.size(), testCase.Path.begin());
		LLC_TEST_CHECK(errors, PATH_TEST_RESULT_FILENAME_TEXT , pathTextMismatch(filename , testCase.Filename ), "Path:'%.*s'.", (int)testCase.Path.size(), testCase.Path.begin());
		LLC_TEST_CHECK(errors, PATH_TEST_RESULT_STEM_TEXT     , pathTextMismatch(stem     , testCase.Stem     ), "Path:'%.*s'.", (int)testCase.Path.size(), testCase.Path.begin());
	}
	rtrn 0;
}

sttc ::llc::err_t testPathBegin(ATestError & errors) {
	cnst SPathBeginCase cases[] =
		{ {LLC_CXS("")						, LLC_CXS("")}
		, {LLC_CXS("folder/file")			, LLC_CXS("")}
		, {LLC_CXS("/a/b")					, LLC_CXS("/")}
		, {LLC_CXS("/folder/file")			, LLC_CXS("/")}
		, {LLC_CXS("\\folder\\file")			, LLC_CXS("\\")}
		, {LLC_CXS("C:a")					, LLC_CXS("C:")}
		, {LLC_CXS("C:folder\\file")			, LLC_CXS("C:")}
		, {LLC_CXS("C:/a")					, LLC_CXS("C:/")}
		, {LLC_CXS("D:/folder/file")			, LLC_CXS("D:/")}
		, {LLC_CXS("z:\\folder\\file")			, LLC_CXS("z:\\")}
		, {LLC_CXS("C:")						, LLC_CXS("C:")}
		, {LLC_CXS("C:/")					, LLC_CXS("C:/")}
		, {LLC_CXS("//server/share/a")		, LLC_CXS("//server/share/")}
		, {LLC_CXS("//server/share/folder")	, LLC_CXS("//server/share/")}
		, {LLC_CXS("\\\\server\\share\\folder")	, LLC_CXS("\\\\server\\share\\")}
		, {LLC_CXS("1:/folder")					, LLC_CXS("")}
		};
	for(cnst SPathBeginCase & testCase : cases) {
		::llc::vcst_t		actual;
		cnst ::llc::err_t result = ::llc::pathBegin(testCase.Path, actual);
		LLC_TEST_CHECK(errors, PATH_TEST_RESULT_BEGIN_TEXT, pathTextMismatch(actual, testCase.Expected)
			, "Path:'%.*s' produced prefix:'%.*s'/%u, expected:'%.*s'/%u."
			, (int)testCase.Path.size(), testCase.Path.begin(), (int)actual.size(), actual.begin(), actual.size(), (int)testCase.Expected.size(), testCase.Expected.begin(), testCase.Expected.size()
			);
		LLC_TEST_CHECK(errors, PATH_TEST_RESULT_BEGIN_RETURN, result != (::llc::err_t)actual.size()
			, "Path:'%.*s' returned:%" LLC_FMT_S2 ", prefix size:%u."
			, (int)testCase.Path.size(), testCase.Path.begin(), result, actual.size()
			);
	}
	rtrn 0;
}

sttc ::llc::err_t testPathNameCompose(ATestError & errors) {
	cnst SPathComposeCase cases[] =
		{ {LLC_CXS("")		, LLC_CXS("")				, LLC_CXS("")			, LLC_CXS("")					, PATH_TEST_RESULT_COMPOSE_EMPTY}
		, {LLC_CXS("")		, LLC_CXS("")				, LLC_CXS("file.txt")	, LLC_CXS("file.txt")			, PATH_TEST_RESULT_COMPOSE_TEXT}
		, {LLC_CXS("")		, LLC_CXS("folder")			, LLC_CXS("file.txt")	, LLC_CXS("folder/file.txt")		, PATH_TEST_RESULT_COMPOSE_TEXT}
		, {LLC_CXS("")		, LLC_CXS("folder/")		, LLC_CXS("file.txt")	, LLC_CXS("folder/file.txt")		, PATH_TEST_RESULT_COMPOSE_TEXT}
		, {LLC_CXS("")		, LLC_CXS("folder\\")		, LLC_CXS("file.txt")	, LLC_CXS("folder/file.txt")		, PATH_TEST_RESULT_COMPOSE_SEPARATOR}
		, {LLC_CXS("")		, LLC_CXS("folder/")		, LLC_CXS("/file.txt")	, LLC_CXS("folder/file.txt")		, PATH_TEST_RESULT_COMPOSE_BOUNDARY}
		, {LLC_CXS("")		, LLC_CXS("folder")			, LLC_CXS("\\file.txt")	, LLC_CXS("folder/file.txt")		, PATH_TEST_RESULT_COMPOSE_BOUNDARY}
		, {LLC_CXS("")		, LLC_CXS("folder")			, LLC_CXS("")			, LLC_CXS("folder")				, PATH_TEST_RESULT_COMPOSE_PATH_ONLY}
		, {LLC_CXS("")		, LLC_CXS("/")				, LLC_CXS("/file.txt")	, LLC_CXS("/file.txt")			, PATH_TEST_RESULT_COMPOSE_ROOT}
		, {LLC_CXS("")		, LLC_CXS("C:\\root")			, LLC_CXS("file.txt")	, LLC_CXS("C:/root/file.txt")		, PATH_TEST_RESULT_COMPOSE_SEPARATOR}
		, {LLC_CXS("")		, LLC_CXS("\\\\server\\share")	, LLC_CXS("file.txt")	, LLC_CXS("//server/share/file.txt")	, PATH_TEST_RESULT_COMPOSE_ROOT}
		, {LLC_CXS("prefix:")	, LLC_CXS("folder")			, LLC_CXS("file.txt")	, LLC_CXS("prefix:folder/file.txt"), PATH_TEST_RESULT_COMPOSE_APPEND}
		};
	for(cnst SPathComposeCase & testCase : cases) {
		::llc::string		output		= testCase.Prefix;
		cnst ::llc::err_t result		= ::llc::pathNameCompose(testCase.Path, testCase.FileName, output);
		LLC_TEST_CHECK(errors, testCase.Result, pathTextMismatch(output, testCase.Expected)
			, "Prefix:'%.*s', path:'%.*s', file:'%.*s' produced:'%.*s'/%u, expected:'%.*s'/%u."
			, (int)testCase.Prefix.size(), testCase.Prefix.begin(), (int)testCase.Path.size(), testCase.Path.begin(), (int)testCase.FileName.size(), testCase.FileName.begin()
			, (int)output.size(), output.begin(), output.size(), (int)testCase.Expected.size(), testCase.Expected.begin(), testCase.Expected.size()
			);
		LLC_TEST_CHECK(errors, PATH_TEST_RESULT_COMPOSE_RETURN, result != (::llc::err_t)output.size()
			, "Prefix:'%.*s', path:'%.*s', file:'%.*s' returned:%" LLC_FMT_S2 ", output size:%u."
			, (int)testCase.Prefix.size(), testCase.Prefix.begin(), (int)testCase.Path.size(), testCase.Path.begin(), (int)testCase.FileName.size(), testCase.FileName.begin(), result, output.size()
			);
		LLC_TEST_CHECK(errors, PATH_TEST_RESULT_COMPOSE_TERMINATOR, output.size() && output.begin()[output.size()]
			, "Prefix:'%.*s', path:'%.*s', file:'%.*s' output size:%u, terminator:%i."
			, (int)testCase.Prefix.size(), testCase.Prefix.begin(), (int)testCase.Path.size(), testCase.Path.begin(), (int)testCase.FileName.size(), testCase.FileName.begin(), output.size(), output.size() ? output.begin()[output.size()] : 0
			);
	}
	rtrn 0;
}

sttc ::llc::err_t testPathNameComposeInvalid(ATestError & errors) {
	cnst SPathComposeCase cases[] =
		{ {LLC_CXS("preserved:")	, LLC_CXS("root//folder")	, LLC_CXS("file.txt")		, LLC_CXS("preserved:")	, PATH_TEST_RESULT_COMPOSE_DUPLICATE_SEPARATOR}
		, {LLC_CXS("preserved:")	, LLC_CXS("root\\\\folder")	, LLC_CXS("file.txt")		, LLC_CXS("preserved:")	, PATH_TEST_RESULT_COMPOSE_DUPLICATE_SEPARATOR}
		, {LLC_CXS("preserved:")	, LLC_CXS("root\\/folder")	, LLC_CXS("file.txt")		, LLC_CXS("preserved:")	, PATH_TEST_RESULT_COMPOSE_DUPLICATE_SEPARATOR}
		, {LLC_CXS("preserved:")	, LLC_CXS("root")			, LLC_CXS("child//file")	, LLC_CXS("preserved:")	, PATH_TEST_RESULT_COMPOSE_DUPLICATE_SEPARATOR}
		, {LLC_CXS("preserved:")	, LLC_CXS("///server/share")	, LLC_CXS("file.txt")		, LLC_CXS("preserved:")	, PATH_TEST_RESULT_COMPOSE_DUPLICATE_SEPARATOR}
		};
	for(cnst SPathComposeCase & testCase : cases) {
		::llc::string		output		= testCase.Prefix;
		cnst ::llc::err_t result		= ::llc::pathNameCompose(testCase.Path, testCase.FileName, output);
		LLC_TEST_CHECK(errors, testCase.Result, false == ::llc::failed(result)
			, "Path:'%.*s', file:'%.*s' returned:%" LLC_FMT_S2 ", expected failure."
			, (int)testCase.Path.size(), testCase.Path.begin(), (int)testCase.FileName.size(), testCase.FileName.begin(), result
			);
		LLC_TEST_CHECK(errors, PATH_TEST_RESULT_COMPOSE_FAILURE_PRESERVE, pathTextMismatch(output, testCase.Expected)
			, "Path:'%.*s', file:'%.*s' changed output to:'%.*s'/%u, expected:'%.*s'/%u."
			, (int)testCase.Path.size(), testCase.Path.begin(), (int)testCase.FileName.size(), testCase.FileName.begin()
			, (int)output.size(), output.begin(), output.size(), (int)testCase.Expected.size(), testCase.Expected.begin(), testCase.Expected.size()
			);
	}
	rtrn 0;
}

sttc ::llc::err_t testPathNormalize(ATestError & errors) {
	cnst SPathNormalizeCase cases[] =
		{ {LLC_CXS("")							, LLC_CXS("")						, '/', PATH_TEST_RESULT_NORMALIZE_EMPTY}
		, {LLC_CXS(".")							, LLC_CXS(".")						, '/', PATH_TEST_RESULT_NORMALIZE_DOT}
		, {LLC_CXS("./folder/./file")			, LLC_CXS("folder/file")			, '/', PATH_TEST_RESULT_NORMALIZE_DOT}
		, {LLC_CXS("folder\\child/file")			, LLC_CXS("folder/child/file")		, '/', PATH_TEST_RESULT_NORMALIZE_SEPARATOR}
		, {LLC_CXS("folder/child/")				, LLC_CXS("folder/child")			, '/', PATH_TEST_RESULT_NORMALIZE_TRAILING}
		, {LLC_CXS("folder/child/../file")		, LLC_CXS("folder/file")			, '/', PATH_TEST_RESULT_NORMALIZE_PARENT}
		, {LLC_CXS("folder/../../file")			, LLC_CXS("../file")				, '/', PATH_TEST_RESULT_NORMALIZE_PARENT}
		, {LLC_CXS("folder/..")					, LLC_CXS(".")						, '/', PATH_TEST_RESULT_NORMALIZE_PARENT}
		, {LLC_CXS("/a/b")						, LLC_CXS("/a/b")					, '/', PATH_TEST_RESULT_NORMALIZE_ROOT}
		, {LLC_CXS("/folder/../")				, LLC_CXS("/")						, '/', PATH_TEST_RESULT_NORMALIZE_ROOT}
		, {LLC_CXS("C:/a")						, LLC_CXS("C:/a")					, '/', PATH_TEST_RESULT_NORMALIZE_DRIVE}
		, {LLC_CXS("C:a")						, LLC_CXS("C:a")					, '/', PATH_TEST_RESULT_NORMALIZE_DRIVE}
		, {LLC_CXS("C:\\folder\\..\\file")		, LLC_CXS("C:/file")				, '/', PATH_TEST_RESULT_NORMALIZE_DRIVE}
		, {LLC_CXS("C:folder\\..\\file")			, LLC_CXS("C:file")				, '/', PATH_TEST_RESULT_NORMALIZE_DRIVE}
		, {LLC_CXS("C:\\")						, LLC_CXS("C:/")					, '/', PATH_TEST_RESULT_NORMALIZE_ROOT}
		, {LLC_CXS("//server/share/a")			, LLC_CXS("//server/share/a")		, '/', PATH_TEST_RESULT_NORMALIZE_UNC}
		, {LLC_CXS("\\\\server\\share\\folder\\..\\file"), LLC_CXS("//server/share/file")	, '/', PATH_TEST_RESULT_NORMALIZE_UNC}
		, {LLC_CXS("\\\\server\\share\\")			, LLC_CXS("//server/share/")		, '/', PATH_TEST_RESULT_NORMALIZE_UNC}
		, {LLC_CXS("C:/folder/file")				, LLC_CXS("C:\\folder\\file")		, '\\', PATH_TEST_RESULT_NORMALIZE_SEPARATOR}
		};
	for(cnst SPathNormalizeCase & testCase : cases) {
		::llc::string		output		= LLC_CXS("stale");
		cnst ::llc::err_t result		= ::llc::pathNormalize(testCase.Path, output, testCase.Separator);
		LLC_TEST_CHECK(errors, testCase.Result, pathTextMismatch(output, testCase.Expected)
			, "Path:'%.*s', separator:'%c' produced:'%.*s'/%u, expected:'%.*s'/%u."
			, (int)testCase.Path.size(), testCase.Path.begin(), testCase.Separator, (int)output.size(), output.begin(), output.size(), (int)testCase.Expected.size(), testCase.Expected.begin(), testCase.Expected.size()
			);
		LLC_TEST_CHECK(errors, PATH_TEST_RESULT_NORMALIZE_REPLACE, output.size() >= 5 && 0 == memcmp(output.begin(), "stale", 5)
			, "Path:'%.*s' retained stale output:'%.*s'/%u."
			, (int)testCase.Path.size(), testCase.Path.begin(), (int)output.size(), output.begin(), output.size()
			);
		LLC_TEST_CHECK(errors, PATH_TEST_RESULT_NORMALIZE_RETURN, result != (::llc::err_t)output.size()
			, "Path:'%.*s' returned:%" LLC_FMT_S2 ", output size:%u."
			, (int)testCase.Path.size(), testCase.Path.begin(), result, output.size()
			);
		LLC_TEST_CHECK(errors, PATH_TEST_RESULT_NORMALIZE_TERMINATOR, output.size() && output.begin()[output.size()]
			, "Path:'%.*s' output size:%u, terminator:%i."
			, (int)testCase.Path.size(), testCase.Path.begin(), output.size(), output.size() ? output.begin()[output.size()] : 0
			);
	}
	rtrn 0;
}

sttc ::llc::err_t testPathNormalizeInvalid(ATestError & errors) {
	cnst SPathNormalizeCase cases[] =
		{ {LLC_CXS("a//b")			, LLC_CXS("preserved")	, '/', PATH_TEST_RESULT_NORMALIZE_INVALID}
		, {LLC_CXS("root//folder")	, LLC_CXS("preserved")	, '/', PATH_TEST_RESULT_NORMALIZE_INVALID}
		, {LLC_CXS("C://a")			, LLC_CXS("preserved")	, '/', PATH_TEST_RESULT_NORMALIZE_INVALID}
		, {LLC_CXS("//server/share//a"), LLC_CXS("preserved")	, '/', PATH_TEST_RESULT_NORMALIZE_INVALID}
		, {LLC_CXS("///server/share")	, LLC_CXS("preserved")	, '/', PATH_TEST_RESULT_NORMALIZE_INVALID}
		, {LLC_CXS("//server")		, LLC_CXS("preserved")	, '/', PATH_TEST_RESULT_NORMALIZE_INVALID}
		, {LLC_CXS("folder/file")		, LLC_CXS("preserved")	, ':', PATH_TEST_RESULT_NORMALIZE_INVALID}
		};
	for(cnst SPathNormalizeCase & testCase : cases) {
		::llc::string		output		= testCase.Expected;
		cnst ::llc::err_t result		= ::llc::pathNormalize(testCase.Path, output, testCase.Separator);
		LLC_TEST_CHECK(errors, testCase.Result, false == ::llc::failed(result)
			, "Path:'%.*s', separator:'%c' returned:%" LLC_FMT_S2 ", expected failure."
			, (int)testCase.Path.size(), testCase.Path.begin(), testCase.Separator, result
			);
		LLC_TEST_CHECK(errors, PATH_TEST_RESULT_NORMALIZE_FAILURE_PRESERVE, pathTextMismatch(output, testCase.Expected)
			, "Path:'%.*s', separator:'%c' changed output to:'%.*s'/%u."
			, (int)testCase.Path.size(), testCase.Path.begin(), testCase.Separator, (int)output.size(), output.begin(), output.size()
			);
	}
	rtrn 0;
}

sttc bool pathIsAbsolute(::llc::vcst_t path) {
	rtrn path.size() && ('/' == path[0] || '\\' == path[0] || (path.size() > 2 && ':' == path[1] && ('/' == path[2] || '\\' == path[2])));
}

sttc ::llc::err_t testPathAbsolute(ATestError & errors) {
	::llc::string		current;
	cnst ::llc::err_t currentResult	= ::llc::pathAbsolute(LLC_CXS("."), current);
	LLC_TEST_REQUIRE(errors, PATH_TEST_RESULT_ABSOLUTE_CURRENT, ::llc::failed(currentResult)
		, "Current directory resolution returned:%" LLC_FMT_S2 ", path:'%.*s'/%u."
		, currentResult, (int)current.size(), current.begin(), current.size()
		);
	LLC_TEST_REQUIRE(errors, PATH_TEST_RESULT_ABSOLUTE_CURRENT_PATH, false == ::pathIsAbsolute(current)
		, "Current directory resolution produced:'%.*s'/%u, expected an absolute path."
		, (int)current.size(), current.begin(), current.size()
		);

	::llc::string		expected;
	if_fail_fe(::llc::pathNameCompose(current, LLC_CXS("llc_test_path_nonexistent"), expected));
	::llc::string		resolved;
	cnst ::llc::err_t resolvedResult = ::llc::pathAbsolute(LLC_CXS("./llc/../llc_test_path_nonexistent"), resolved);
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_ABSOLUTE_RELATIVE, pathTextMismatch(resolved, expected)
		, "Relative resolution produced:'%.*s'/%u, expected:'%.*s'/%u."
		, (int)resolved.size(), resolved.begin(), resolved.size(), (int)expected.size(), expected.begin(), expected.size()
		);
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_ABSOLUTE_NONEXISTENT, ::llc::failed(resolvedResult)
		, "Nonexistent target resolution returned:%" LLC_FMT_S2 ", path:'%.*s'/%u."
		, resolvedResult, (int)resolved.size(), resolved.begin(), resolved.size()
		);

	::llc::string		idempotent;
	cnst ::llc::err_t idempotentResult = ::llc::pathAbsolute(current, idempotent);
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_ABSOLUTE_IDEMPOTENT, pathTextMismatch(idempotent, current)
		, "Absolute input:'%.*s'/%u produced:'%.*s'/%u."
		, (int)current.size(), current.begin(), current.size(), (int)idempotent.size(), idempotent.begin(), idempotent.size()
		);
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_ABSOLUTE_RETURN, currentResult != (::llc::err_t)current.size()
		, "Current path returned:%" LLC_FMT_S2 ", output size:%u."
		, currentResult, current.size()
		);
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_ABSOLUTE_RELATIVE_RETURN, resolvedResult != (::llc::err_t)resolved.size()
		, "Resolved path returned:%" LLC_FMT_S2 ", output size:%u."
		, resolvedResult, resolved.size()
		);
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_ABSOLUTE_IDEMPOTENT_RETURN, idempotentResult != (::llc::err_t)idempotent.size()
		, "Idempotent path returned:%" LLC_FMT_S2 ", output size:%u."
		, idempotentResult, idempotent.size()
		);
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_ABSOLUTE_TERMINATOR, current.size() && current.begin()[current.size()]
		, "Current path terminator:%i, expected:0."
		, current.size() ? current.begin()[current.size()] : 0
		);
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_ABSOLUTE_RELATIVE_TERMINATOR, resolved.size() && resolved.begin()[resolved.size()]
		, "Resolved path terminator:%i, expected:0."
		, resolved.size() ? resolved.begin()[resolved.size()] : 0
		);
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_ABSOLUTE_IDEMPOTENT_TERMINATOR, idempotent.size() && idempotent.begin()[idempotent.size()]
		, "Idempotent path terminator:%i, expected:0."
		, idempotent.size() ? idempotent.begin()[idempotent.size()] : 0
		);

	::llc::string		backslash;
	if_fail_fe(::llc::pathAbsolute(LLC_CXS("."), backslash, '\\'));
	::llc::string		backslashAsSlash;
	if_fail_fe(::llc::pathNormalize(backslash, backslashAsSlash));
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_ABSOLUTE_SEPARATOR, pathTextMismatch(backslashAsSlash, current)
		, "Backslash path:'%.*s' normalized to:'%.*s', expected:'%.*s'."
		, (int)backslash.size(), backslash.begin(), (int)backslashAsSlash.size(), backslashAsSlash.begin(), (int)current.size(), current.begin()
		);
	rtrn 0;
}

sttc ::llc::err_t testPathAbsoluteInvalid(ATestError & errors) {
	cnst SPathNormalizeCase cases[] =
		{ {LLC_CXS("")				, LLC_CXS("preserved")	, '/', PATH_TEST_RESULT_ABSOLUTE_INVALID}
		, {LLC_CXS("root//folder")	, LLC_CXS("preserved")	, '/', PATH_TEST_RESULT_ABSOLUTE_INVALID}
		, {LLC_CXS("folder/file")		, LLC_CXS("preserved")	, ':', PATH_TEST_RESULT_ABSOLUTE_INVALID}
		};
	for(cnst SPathNormalizeCase & testCase : cases) {
		::llc::string		output		= testCase.Expected;
		cnst ::llc::err_t result		= ::llc::pathAbsolute(testCase.Path, output, testCase.Separator);
		LLC_TEST_CHECK(errors, testCase.Result, false == ::llc::failed(result)
			, "Path:'%.*s', separator:'%c' returned:%" LLC_FMT_S2 ", expected failure."
			, (int)testCase.Path.size(), testCase.Path.begin(), testCase.Separator, result
			);
		LLC_TEST_CHECK(errors, PATH_TEST_RESULT_ABSOLUTE_FAILURE_PRESERVE, pathTextMismatch(output, testCase.Expected)
			, "Path:'%.*s', separator:'%c' changed output to:'%.*s'/%u."
			, (int)testCase.Path.size(), testCase.Path.begin(), testCase.Separator, (int)output.size(), output.begin(), output.size()
			);
	}
	rtrn 0;
}

sttc ::llc::err_t testPathListRecursive(ATestError & errors) {
	SPathListFixture		fixture;
	std::error_code		filesystemError;
	std::filesystem::remove_all(fixture.Root.begin(), filesystemError);
	if_true_fef(filesystemError.value(), "Failed to clear pathList() test fixture:'%s'. Error:%i.", fixture.Root.begin(), filesystemError.value());
	std::filesystem::create_directories("./llc_test_path_list/child/grandchild", filesystemError);
	if_true_fef(filesystemError.value(), "Failed to create pathList() test fixture:'%s'. Error:%i.", fixture.Root.begin(), filesystemError.value());
	if_fail_fe(::llc::fileFromMemory(LLC_CXS("./llc_test_path_list/root.bin"), ::llc::vcu0_c{}));
	if_fail_fe(::llc::fileFromMemory(LLC_CXS("./llc_test_path_list/child/child.bin"), ::llc::vcu0_c{}));
	if_fail_fe(::llc::fileFromMemory(LLC_CXS("./llc_test_path_list/child/grandchild/deep.bin"), ::llc::vcu0_c{}));

	cnst ::llc::vcst_t	rootPath		= fixture.Root;
	::llc::u2_t			callbackFiles	= 0;
	::llc::u2_t			callbackFolders	= 0;
	bool					deepFileSeen	= false;
	stxp ::llc::vcst_t	deepFileName	= LLC_CXS("deep.bin");
	::llc::SPathContents	callbackTree;
	cnst ::llc::err_t	callbackResult	= ::llc::pathList(rootPath, callbackTree
		, [&](::llc::b8_t isFolder, ::llc::vcst_c & path) -> ::llc::err_t {
			isFolder ? ++callbackFolders : ++callbackFiles;
			cnst ::llc::err_t iDeepFile = path.rfind(deepFileName);
			deepFileSeen		|= false == isFolder && 0 <= iDeepFile && (::llc::u2_t)iDeepFile + deepFileName.size() == path.size();
			rtrn 0;
		}
		, {}
		);
	LLC_TEST_REQUIRE(errors, PATH_TEST_RESULT_LIST_CALLBACK_RESULT, ::llc::failed(callbackResult)
		, "Root:'%.*s' returned:%" LLC_FMT_S2 "."
		, (int)rootPath.size(), rootPath.begin(), callbackResult
		);
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_LIST_CALLBACK_RECURSIVE, false == deepFileSeen
		, "Root:'%.*s' did not report the grandchild file. Callback files:%u, folders:%u."
		, (int)rootPath.size(), rootPath.begin(), callbackFiles, callbackFolders
		);
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_LIST_CALLBACK_COUNTS, 3 != callbackFiles
		, "Root:'%.*s' callback files:%u, expected:3."
		, (int)rootPath.size(), rootPath.begin(), callbackFiles
		);
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_LIST_CALLBACK_FOLDER_COUNT, 2 != callbackFolders
		, "Root:'%.*s' callback folders:%u, expected:2."
		, (int)rootPath.size(), rootPath.begin(), callbackFolders
		);

	cnst SPathListCounts	callbackCounts	= ::pathListCounts(callbackTree);
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_LIST_CALLBACK_TREE, 3 != callbackCounts.Files
		, "Root:'%.*s' callback tree files:%u, expected:3."
		, (int)rootPath.size(), rootPath.begin(), callbackCounts.Files
		);
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_LIST_CALLBACK_TREE_FOLDER_COUNT, 2 != callbackCounts.Folders
		, "Root:'%.*s' callback tree folders:%u, expected:2."
		, (int)rootPath.size(), rootPath.begin(), callbackCounts.Folders
		);

	::llc::SPathContents	plainTree;
	cnst ::llc::err_t	plainResult		= ::llc::pathList(rootPath, plainTree, {});
	LLC_TEST_REQUIRE(errors, PATH_TEST_RESULT_LIST_PLAIN_RESULT, ::llc::failed(plainResult)
		, "Root:'%.*s' non-callback traversal returned:%" LLC_FMT_S2 "."
		, (int)rootPath.size(), rootPath.begin(), plainResult
		);
	cnst SPathListCounts	plainCounts		= ::pathListCounts(plainTree);
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_LIST_TREE_FILE_EQUIVALENT, callbackCounts.Files != plainCounts.Files
		, "Root:'%.*s' callback files:%u, plain files:%u."
		, (int)rootPath.size(), rootPath.begin(), callbackCounts.Files, plainCounts.Files
		);
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_LIST_TREE_FOLDER_EQUIVALENT, callbackCounts.Folders != plainCounts.Folders
		, "Root:'%.*s' callback folders:%u, plain folders:%u."
		, (int)rootPath.size(), rootPath.begin(), callbackCounts.Folders, plainCounts.Folders
		);

	::llc::SPathContents	filteredTree;
	if_fail_fe(::llc::pathList(rootPath, filteredTree, LLC_CXS(".txt")));
	cnst SPathListCounts	filteredCounts	= ::pathListCounts(filteredTree);
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_LIST_EXTENSION_FILTER, filteredCounts.Files
		, "Root:'%.*s' filtered files:%u, expected:0."
		, (int)rootPath.size(), rootPath.begin(), filteredCounts.Files
		);
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_LIST_EXTENSION_FOLDER_COUNT, 2 != filteredCounts.Folders
		, "Root:'%.*s' filtered folders:%u, expected:2."
		, (int)rootPath.size(), rootPath.begin(), filteredCounts.Folders
		);
	rtrn 0;
}

::llc::err_t testPath(ATestError & errors) {
	if_fail_fe(testFindLastSlash(errors));
	if_fail_fe(testPathBegin(errors));
	if_fail_fe(testPathNameCompose(errors));
	if_fail_fe(testPathNameComposeInvalid(errors));
	if_fail_fe(testPathNormalize(errors));
	if_fail_fe(testPathNormalizeInvalid(errors));
	if_fail_fe(testPathAbsolute(errors));
	if_fail_fe(testPathAbsoluteInvalid(errors));
	if_fail_fe(testPathListRecursive(errors));
	rtrn 0;
}
