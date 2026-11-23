#include "llc_index.h"

#include "test_llc_suites.h"

GDEFINE_ENUM_TYPE(INDEX_CORE_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(INDEX_CORE_TEST_RESULT, NORMALIZE_RESULT	, 0, "wordListNormalize() rejected valid input.");
GDEFINE_ENUM_VALUED(INDEX_CORE_TEST_RESULT, NORMALIZE_COUNT	, 1, "wordListNormalize() returned an unexpected word count.");
GDEFINE_ENUM_VALUED(INDEX_CORE_TEST_RESULT, NORMALIZE_VALUE	, 2, "wordListNormalize() returned an unexpected normalized word.");
GDEFINE_ENUM_VALUED(INDEX_CORE_TEST_RESULT, APPEND_RESULT		, 3, "wordDictAppend() rejected valid input.");
GDEFINE_ENUM_VALUED(INDEX_CORE_TEST_RESULT, APPEND_WORD		, 4, "wordDictAppend() did not create an expected dictionary word.");
GDEFINE_ENUM_VALUED(INDEX_CORE_TEST_RESULT, APPEND_POSTING	, 5, "wordDictAppend() did not create the expected unique posting.");
GDEFINE_ENUM_VALUED(INDEX_CORE_TEST_RESULT, LIST_RESULT		, 6, "getWordList() rejected a valid item.");
GDEFINE_ENUM_VALUED(INDEX_CORE_TEST_RESULT, LIST_WORD		, 7, "getWordList() did not expose an expected item word.");
GDEFINE_ENUM_VALUED(INDEX_CORE_TEST_RESULT, BUILD_RESULT		, 8, "getWordDict() rejected a valid item.");
GDEFINE_ENUM_VALUED(INDEX_CORE_TEST_RESULT, RESOLVE_RESULT	, 9, "wordDictResolve() returned an unexpected result count.");
GDEFINE_ENUM_VALUED(INDEX_CORE_TEST_RESULT, RESOLVE_ITEM		, 10, "wordDictResolve() returned an unexpected item order.");
GDEFINE_ENUM_VALUED(INDEX_CORE_TEST_RESULT, PROJECT_COUNT	, 13, "getWordDict() did not project every source item exactly once.");
GDEFINE_ENUM_VALUED(INDEX_CORE_TEST_RESULT, PROJECT_CACHE	, 14, "getWordDict() did not clear the raw-view cache before projection.");
GDEFINE_ENUM_VALUED(INDEX_CORE_TEST_RESULT, REBUILD_RESULT	, 15, "getWordDict() rejected a rebuild with retained word data.");
GDEFINE_ENUM_VALUED(INDEX_CORE_TEST_RESULT, REBUILD_STORAGE	, 16, "getWordDict() replaced retained workspace storage during an identical rebuild.");

sttc cnst ::llc::SWordIndices * indexWord(cnst ::llc::aobj<::llc::SWordIndices> & wordSet, ::llc::vcst_t word) {
	for(::llc::u2_t iWord = 0; iWord < wordSet.size(); ++iWord)
		if(wordSet[iWord].Text == word)
			rtrn &wordSet[iWord];
	rtrn {};
}

sttc b8_t indexWordHas(cnst ::llc::aobj<::llc::SWordIndices> & wordSet, ::llc::vcst_t word, ::llc::u2_t item) {
	cnst ::llc::SWordIndices * indexed = ::indexWord(wordSet, word);
	rtrn indexed && 0 <= ::llc::find(item, {indexed->Indices.begin(), indexed->Indices.size()});
}

sttc ::llc::err_t testIndexNormalizeAndAppend(::llc::ATestError & errors) {
	stxp ::llc::vcst_c inputWords[] = {"ALPHA", "the", "Beta"};
	::llc::aobj<::llc::string> normalized = {};
	cnst ::llc::err_t normalizeResult = ::llc::getWordList(normalized, inputWords);
	LLC_TEST_REQUIREF(errors, INDEX_CORE_TEST_RESULT_NORMALIZE_RESULT, ::llc::failed(normalizeResult), "Result:%i.", normalizeResult);
	LLC_TEST_REQUIREF(errors, INDEX_CORE_TEST_RESULT_NORMALIZE_COUNT, normalized.size() != ::llc::size(inputWords), "Count:%u.", normalized.size());
	LLC_TEST_CHECK(errors, INDEX_CORE_TEST_RESULT_NORMALIZE_VALUE, normalized[0] != "alpha");
	LLC_TEST_CHECK(errors, INDEX_CORE_TEST_RESULT_NORMALIZE_VALUE, normalized[1].size());
	LLC_TEST_CHECK(errors, INDEX_CORE_TEST_RESULT_NORMALIZE_VALUE, normalized[2] != "beta");

	if_fail_fe(normalized.push_back("alpha"));
	::llc::aobj<::llc::SWordIndices> wordSet = {};
	cnst ::llc::err_t appendResult = ::llc::wordDictAppend(wordSet, 7, normalized);
	LLC_TEST_REQUIREF(errors, INDEX_CORE_TEST_RESULT_APPEND_RESULT, ::llc::failed(appendResult), "Result:%i.", appendResult);
	cnst ::llc::SWordIndices * alpha = ::indexWord(wordSet, "alpha");
	cnst ::llc::SWordIndices * beta = ::indexWord(wordSet, "beta");
	LLC_TEST_REQUIREF(errors, INDEX_CORE_TEST_RESULT_APPEND_WORD, !alpha || !beta, "Words:%u.", wordSet.size());
	LLC_TEST_CHECKF(errors, INDEX_CORE_TEST_RESULT_APPEND_POSTING, 1 != alpha->Indices.size() || 7 != alpha->Indices[0], "Alpha postings:%u.", alpha->Indices.size());
	LLC_TEST_CHECKF(errors, INDEX_CORE_TEST_RESULT_APPEND_POSTING, 1 != beta->Indices.size() || 7 != beta->Indices[0], "Beta postings:%u.", beta->Indices.size());
	rtrn 0;
}

sttc ::llc::err_t testIndexBuildAndResolve(::llc::ATestError & errors) {
	stxp ::llc::vcst_c items[] = {"Alpha orchard common", "Betamax relay common", "Alphabet soup common"};
	::llc::aobj<::llc::vcst_t> itemViews = {};
	::llc::aobj<::llc::string> itemWords = {};
	if_fail_fe(::llc::split(items[0], ' ', itemViews));
	cnst ::llc::err_t listResult = ::llc::getWordList(itemWords, itemViews);
	LLC_TEST_REQUIREF(errors, INDEX_CORE_TEST_RESULT_LIST_RESULT, ::llc::failed(listResult), "Result:%i.", listResult);
	b8_t foundAlpha = false;
	b8_t foundOrchard = false;
	for(::llc::u2_t iWord = 0; iWord < itemWords.size(); ++iWord) {
		foundAlpha |= itemWords[iWord] == "alpha";
		foundOrchard |= itemWords[iWord] == "orchard";
	}
	LLC_TEST_CHECK(errors, INDEX_CORE_TEST_RESULT_LIST_WORD, false == foundAlpha);
	LLC_TEST_CHECK(errors, INDEX_CORE_TEST_RESULT_LIST_WORD, false == foundOrchard);

	::llc::aobj<::llc::SWordIndices> wordSet = {};
	::llc::SWordDictData wordData = {};
	::llc::u2_t projectedCount = 0;
	b8_t rawCacheWasDirty = false;
	auto project = [&projectedCount, &rawCacheWasDirty](::llc::u2_t iItem, ::llc::u2_t & itemIndex, ::llc::aobj<::llc::vcst_t> & rawWords) -> ::llc::err_t {
			rawCacheWasDirty |= 0 != rawWords.size();
			++projectedCount;
			itemIndex = (iItem + 2) % ::llc::size(items);
			rtrn ::llc::split(items[iItem], ' ', rawWords);
		};
	cnst ::llc::err_t buildResult = ::llc::getWordDict(wordData, wordSet, ::llc::size(items), project);
	LLC_TEST_REQUIREF(errors, INDEX_CORE_TEST_RESULT_BUILD_RESULT, ::llc::failed(buildResult), "Result:%i.", buildResult);
	LLC_TEST_CHECKF(errors, INDEX_CORE_TEST_RESULT_PROJECT_COUNT, projectedCount != ::llc::size(items), "Projected:%u, expected:%u.", projectedCount, ::llc::size(items));
	LLC_TEST_CHECK(errors, INDEX_CORE_TEST_RESULT_PROJECT_CACHE, rawCacheWasDirty);
	LLC_TEST_CHECK(errors, INDEX_CORE_TEST_RESULT_APPEND_WORD, false == ::indexWordHas(wordSet, "alpha", 2));
	LLC_TEST_CHECK(errors, INDEX_CORE_TEST_RESULT_APPEND_POSTING, false == ::indexWordHas(wordSet, "relay", 0));
	cnst ::llc::vcst_t * itemViewStorage = wordData.ItemViews.begin();
	cnst ::llc::string * itemWordStorage = wordData.ItemWords.begin();
	cnst ::llc::sc_t * firstWordStorage = wordData.ItemWords.size() ? wordData.ItemWords[0].begin() : 0;
	projectedCount = 0;
	rawCacheWasDirty = false;
	::llc::aobj<::llc::SWordIndices> rebuiltWordSet = {};
	cnst ::llc::err_t rebuildResult = ::llc::getWordDict(wordData, rebuiltWordSet, ::llc::size(items), project);
	LLC_TEST_REQUIREF(errors, INDEX_CORE_TEST_RESULT_REBUILD_RESULT, ::llc::failed(rebuildResult), "Result:%i.", rebuildResult);
	LLC_TEST_CHECKF(errors, INDEX_CORE_TEST_RESULT_PROJECT_COUNT, projectedCount != ::llc::size(items), "Projected:%u, expected:%u.", projectedCount, ::llc::size(items));
	LLC_TEST_CHECK(errors, INDEX_CORE_TEST_RESULT_PROJECT_CACHE, rawCacheWasDirty);
	LLC_TEST_CHECK(errors, INDEX_CORE_TEST_RESULT_REBUILD_STORAGE, itemViewStorage != wordData.ItemViews.begin() || itemWordStorage != wordData.ItemWords.begin() || firstWordStorage != wordData.ItemWords[0].begin());

	::llc::au2_t resolved = {};
	cnst ::llc::err_t sharedResult = ::llc::wordDictResolve(wordSet, "COMMON", ::llc::size(items), resolved);
	LLC_TEST_REQUIREF(errors, INDEX_CORE_TEST_RESULT_RESOLVE_RESULT, sharedResult != 3 || resolved.size() != 3, "Result:%i, count:%u.", sharedResult, resolved.size());
	for(::llc::u2_t iItem = 0; iItem < resolved.size(); ++iItem)
		LLC_TEST_CHECKF(errors, INDEX_CORE_TEST_RESULT_RESOLVE_ITEM, resolved[iItem] != iItem, "Position:%u, item:%u.", iItem, resolved[iItem]);
	cnst ::llc::err_t resolveResult = ::llc::wordDictResolve(wordSet, "PHA MAX", ::llc::size(items), resolved);
	LLC_TEST_REQUIREF(errors, INDEX_CORE_TEST_RESULT_RESOLVE_RESULT, resolveResult != 3 || resolved.size() != 3, "Result:%i, count:%u.", resolveResult, resolved.size());
	for(::llc::u2_t iItem = 0; iItem < resolved.size(); ++iItem)
		LLC_TEST_CHECKF(errors, INDEX_CORE_TEST_RESULT_RESOLVE_ITEM, resolved[iItem] != iItem, "Position:%u, item:%u.", iItem, resolved[iItem]);
	cnst ::llc::err_t emptyResult = ::llc::wordDictResolve(wordSet, "", ::llc::size(items), resolved);
	LLC_TEST_CHECKF(errors, INDEX_CORE_TEST_RESULT_RESOLVE_RESULT, emptyResult || resolved.size(), "Result:%i, count:%u.", emptyResult, resolved.size());

	rtrn 0;
}

::llc::err_t test_index(STestRunnerState &, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	if_fail_fe(::testIndexNormalizeAndAppend(errors));
	if_fail_fe(::testIndexBuildAndResolve(errors));
	rtrn 0;
}
