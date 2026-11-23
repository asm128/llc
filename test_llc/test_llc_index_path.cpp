#include "llc_index_path.h"
#include "llc_file.h"

#include "test_llc_suites.h"

GDEFINE_ENUM_TYPE(INDEX_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(INDEX_TEST_RESULT, FILE_WRITE		, 0, "A real index fixture file could not be written.");
GDEFINE_ENUM_VALUED(INDEX_TEST_RESULT, SCAN_RESULT		, 1, "pathList() rejected the real fixture directory.");
GDEFINE_ENUM_VALUED(INDEX_TEST_RESULT, SCAN_COUNT		, 2, "The extension filter retained an unexpected number of files.");
GDEFINE_ENUM_VALUED(INDEX_TEST_RESULT, EXTENSION_SET		, 3, "The extension filter did not retain exactly the .cpp and .h files.");
GDEFINE_ENUM_VALUED(INDEX_TEST_RESULT, BUILD_RESULT		, 4, "getWordDict() rejected the filtered source files.");
GDEFINE_ENUM_VALUED(INDEX_TEST_RESULT, CONTENT_RESOLVE	, 5, "A file-content word did not resolve its source record.");
GDEFINE_ENUM_VALUED(INDEX_TEST_RESULT, RESOLVED_ITEM		, 6, "A content query resolved an unexpected source record.");
GDEFINE_ENUM_VALUED(INDEX_TEST_RESULT, FULL_PATH			, 7, "A resolved source record did not reconstruct its full path.");
GDEFINE_ENUM_VALUED(INDEX_TEST_RESULT, REOPEN_RESULT		, 8, "A resolved source file could not be reopened.");
GDEFINE_ENUM_VALUED(INDEX_TEST_RESULT, SOURCE_BYTES		, 9, "A reopened source file does not match its indexed fixture bytes.");
GDEFINE_ENUM_VALUED(INDEX_TEST_RESULT, EXCLUDED_CONTENT	, 10, "Content from the excluded extension entered the index.");
GDEFINE_ENUM_VALUED(INDEX_TEST_RESULT, OPTIONAL_PATH		, 11, "Optional path indexing did not honor its selected state.");

stxp ::llc::vcst_c	INDEX_FILE_NAMES[]		= {"index_fixture.cpp", "index_fixture.h", "index_fixture.txt"};
stxp ::llc::vcst_c	INDEX_FILE_CONTENTS[]	= {"cpp_content_probe alpha", "header_content_probe beta", "excluded_text_probe gamma"};
stxp ::llc::vcst_c	INDEX_CONTENT_PROBES[]	= {"cpp_content_probe", "header_content_probe"};

sttc ::llc::err_t indexFixtureWrite(::llc::vcst_t folder, ::llc::vcst_t name, ::llc::vcst_t contents, ::llc::string & fullPath) {
	if_fail_fe(fullPath.clear());
	if_fail_fe(::llc::pathNameCompose(folder, name, fullPath));
	rtrn ::llc::fileFromMemory(fullPath, contents);
}

sttc b8_t indexExtensionAccepted(::llc::vcst_t path) {
	::llc::vcst_t fileName = {};
	if(::llc::failed(::llc::pathFilename(path, fileName)))
		rtrn false;
	::llc::vcst_t extension = {};
	if(fileName.size() >= 4) {
		fileName.slice(extension, fileName.size() - 4);
		if(extension == ".cpp")
			rtrn true;
	}
	if(fileName.size() >= 2) {
		fileName.slice(extension, fileName.size() - 2);
		if(extension == ".h")
			rtrn true;
	}
	rtrn false;
}

::llc::err_t test_index_path(STestRunnerState &, ::llc::ATestError & errors, ::llc::vcst_t outputFolder) {
	::llc::string fixturePath[::llc::size(INDEX_FILE_NAMES)] = {};
	for(::llc::u2_t iFile = 0; iFile < ::llc::size(INDEX_FILE_NAMES); ++iFile) {
		cnst ::llc::err_t writeResult = ::indexFixtureWrite(outputFolder, INDEX_FILE_NAMES[iFile], INDEX_FILE_CONTENTS[iFile], fixturePath[iFile]);
		LLC_TEST_REQUIREF(errors, INDEX_TEST_RESULT_FILE_WRITE, ::llc::failed(writeResult), "File:%u, result:%i.", iFile, writeResult);
	}

	::llc::SPathTree tree = {};
	cnst ::llc::err_t scanResult = ::llc::pathList(tree, outputFolder, [](b8_t folder, ::llc::vcst_c & entry) -> ::llc::err_t {
		rtrn folder || false == ::indexExtensionAccepted(entry) ? 1 : 0;
	});
	LLC_TEST_REQUIREF(errors, INDEX_TEST_RESULT_SCAN_RESULT, ::llc::failed(scanResult), "Result:%i.", scanResult);
	LLC_TEST_REQUIREF(errors, INDEX_TEST_RESULT_SCAN_COUNT, 2 != tree.Tree.size() || 2 != tree.Name.size(), "Nodes:%u, names:%u.", tree.Tree.size(), tree.Name.size());
	b8_t foundCPP = false, foundH = false, foundTXT = false;
	for(::llc::u2_t iName = 0; iName < tree.Name.size(); ++iName) {
		foundCPP |= tree.Name[iName] == INDEX_FILE_NAMES[0];
		foundH   |= tree.Name[iName] == INDEX_FILE_NAMES[1];
		foundTXT |= tree.Name[iName] == INDEX_FILE_NAMES[2];
	}
	LLC_TEST_REQUIREF(errors, INDEX_TEST_RESULT_EXTENSION_SET, false == foundCPP || false == foundH || foundTXT, "CPP:%u, H:%u, TXT:%u.", foundCPP, foundH, foundTXT);

	::llc::aobj<::llc::SWordIndices> wordSet = {};
	::llc::aobj<::llc::vcst_t> reverseMap = {};
	::llc::SPathIndexData indexData = {};
	indexData.IndexPath = false;
	cnst ::llc::err_t buildResult = ::llc::getWordDict(indexData, wordSet, reverseMap, tree);
	LLC_TEST_REQUIREF(errors, INDEX_TEST_RESULT_BUILD_RESULT, ::llc::failed(buildResult), "Result:%i.", buildResult);

	for(::llc::u2_t iFile = 0; iFile < ::llc::size(INDEX_CONTENT_PROBES); ++iFile) {
		::llc::au2_t resolved = {};
		cnst ::llc::err_t resolveResult = ::llc::wordDictResolve(wordSet, INDEX_CONTENT_PROBES[iFile], tree.Name.size(), resolved);
		LLC_TEST_REQUIREF(errors, INDEX_TEST_RESULT_CONTENT_RESOLVE, 1 != resolveResult || 1 != resolved.size(), "File:%u, result:%i, count:%u.", iFile, resolveResult, resolved.size());
		cnst ::llc::u2_t itemIndex = resolved[0];
		LLC_TEST_REQUIREF(errors, INDEX_TEST_RESULT_RESOLVED_ITEM, itemIndex >= tree.Tree.size() || itemIndex >= reverseMap.size(), "File:%u, item:%u, nodes:%u, reverse:%u.", iFile, itemIndex, tree.Tree.size(), reverseMap.size());
		LLC_TEST_REQUIREF(errors, INDEX_TEST_RESULT_RESOLVED_ITEM, reverseMap[itemIndex] != INDEX_FILE_NAMES[iFile], "File:%u, item:%u, name:'%.*s'.", iFile, itemIndex, LLC_VIEW_ARGS(reverseMap[itemIndex]));
		::llc::string resolvedPath = {};
		cnst ::llc::err_t pathResult = ::llc::pathNodePath(resolvedPath, tree, *tree.Tree[itemIndex]);
		LLC_TEST_REQUIREF(errors, INDEX_TEST_RESULT_FULL_PATH, ::llc::failed(pathResult) || resolvedPath != fixturePath[iFile], "File:%u, result:%i, resolved:'%.*s', expected:'%.*s'.", iFile, pathResult, LLC_VIEW_ARGS(resolvedPath), LLC_VIEW_ARGS(fixturePath[iFile]));
		::llc::string reopened = {};
		cnst ::llc::err_t reopenResult = ::llc::fileToMemory(resolvedPath, reopened);
		LLC_TEST_REQUIREF(errors, INDEX_TEST_RESULT_REOPEN_RESULT, ::llc::failed(reopenResult), "File:%u, result:%i.", iFile, reopenResult);
		LLC_TEST_CHECKF(errors, INDEX_TEST_RESULT_SOURCE_BYTES, reopened != INDEX_FILE_CONTENTS[iFile], "File:%u, reopened:%u, expected:%u.", iFile, reopened.size(), INDEX_FILE_CONTENTS[iFile].size());
	}
	::llc::au2_t excluded = {};
	cnst ::llc::err_t excludedResult = ::llc::wordDictResolve(wordSet, "excluded_text_probe", tree.Name.size(), excluded);
	LLC_TEST_CHECKF(errors, INDEX_TEST_RESULT_EXCLUDED_CONTENT, excludedResult || excluded.size(), "Result:%i, count:%u.", excludedResult, excluded.size());
	::llc::au2_t pathMatches = {};
	cnst ::llc::err_t pathDisabledResult = ::llc::wordDictResolve(wordSet, INDEX_FILE_NAMES[0], tree.Name.size(), pathMatches);
	LLC_TEST_CHECKF(errors, INDEX_TEST_RESULT_OPTIONAL_PATH, pathDisabledResult || pathMatches.size(), "Result:%i, count:%u.", pathDisabledResult, pathMatches.size());

	if_fail_fe(wordSet.clear());
	indexData.IndexPath = true;
	cnst ::llc::err_t pathBuildResult = ::llc::getWordDict(indexData, wordSet, reverseMap, tree);
	LLC_TEST_REQUIREF(errors, INDEX_TEST_RESULT_BUILD_RESULT, ::llc::failed(pathBuildResult), "Result:%i.", pathBuildResult);
	cnst ::llc::err_t pathEnabledResult = ::llc::wordDictResolve(wordSet, INDEX_FILE_NAMES[0], tree.Name.size(), pathMatches);
	LLC_TEST_CHECKF(errors, INDEX_TEST_RESULT_OPTIONAL_PATH, 1 != pathEnabledResult || 1 != pathMatches.size(), "Result:%i, count:%u.", pathEnabledResult, pathMatches.size());
	rtrn 0;
}
