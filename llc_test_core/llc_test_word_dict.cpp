#include "llc_word_dict.h"
#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(WORD_DICT_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(WORD_DICT_TEST_RESULT, OK				, 0, "All word dictionary tests passed.");
GDEFINE_ENUM_VALUED(WORD_DICT_TEST_RESULT, WORDS			, 1, "Word insertion or normalization differs from the Obelisco contract.");
GDEFINE_ENUM_VALUED(WORD_DICT_TEST_RESULT, STOP_WORDS		, 2, "A configured stop word was indexed.");
GDEFINE_ENUM_VALUED(WORD_DICT_TEST_RESULT, REPLACEMENTS		, 3, "The legacy character replacement alias was not indexed.");
GDEFINE_ENUM_VALUED(WORD_DICT_TEST_RESULT, POSTINGS			, 4, "A word posting list contains missing or duplicate record identifiers.");
GDEFINE_ENUM_VALUED(WORD_DICT_TEST_RESULT, CUSTOM_SKIP_WORDS, 5, "The caller-supplied stop-word list was not honored.");

sttc ::llc::err_t wordIndex(cnst ::llc::SWordDict & dictionary, ::llc::vcst_t word) {
	for(::llc::u2_t iWord = 0; iWord < dictionary.size(); ++iWord)
		if(dictionary[iWord].Text == word)
			rtrn iWord;
	rtrn -1;
}

sttc ::llc::err_t testCatalogDemo(::llc::ATestError & errors) {
	cnst ::llc::vcst_t catalogItem0[] = {"The ALPHA alpha beta,", "\xE1rea"};
	cnst ::llc::vcst_t catalogItem1[] = {"alpha gamma area"};
	::llc::SWordDict dictionary;
	::llc::aobj<::llc::string> scratch;
	if_fail_fe(::llc::getWordDict(dictionary, catalogItem0, 7, scratch));
	if_fail_fe(::llc::getWordDict(dictionary, catalogItem1, 11, scratch));

	LLC_TEST_CHECK(errors, WORD_DICT_TEST_RESULT_STOP_WORDS, 0 <= ::wordIndex(dictionary, "the"));
	cnst ::llc::err_t iAlpha = ::wordIndex(dictionary, "alpha");
	cnst ::llc::err_t iBeta = ::wordIndex(dictionary, "beta");
	cnst ::llc::err_t iAccentedArea = ::wordIndex(dictionary, "\xE1rea");
	cnst ::llc::err_t iArea = ::wordIndex(dictionary, "area");
	cnst ::llc::err_t iGamma = ::wordIndex(dictionary, "gamma");
	LLC_TEST_REQUIRE(errors, WORD_DICT_TEST_RESULT_WORDS, 0 > iAlpha || 0 > iBeta || 0 > iAccentedArea || 0 > iArea || 0 > iGamma);

	LLC_TEST_REQUIREF(errors, WORD_DICT_TEST_RESULT_POSTINGS, dictionary[iAlpha].Indices.size() != 2, "Alpha posting count:%u, expected:2.", dictionary[iAlpha].Indices.size());
	if(2 == dictionary[iAlpha].Indices.size()) {
		LLC_TEST_CHECKF(errors, WORD_DICT_TEST_RESULT_POSTINGS, dictionary[iAlpha].Indices[0] != 7, "Alpha first record:%u, expected:7.", dictionary[iAlpha].Indices[0]);
		LLC_TEST_CHECKF(errors, WORD_DICT_TEST_RESULT_POSTINGS, dictionary[iAlpha].Indices[1] != 11, "Alpha second record:%u, expected:11.", dictionary[iAlpha].Indices[1]);
	}
	LLC_TEST_REQUIREF(errors, WORD_DICT_TEST_RESULT_REPLACEMENTS, dictionary[iArea].Indices.size() != 2, "Area posting count:%u, expected:2.", dictionary[iArea].Indices.size());
	if(2 == dictionary[iArea].Indices.size()) {
		LLC_TEST_CHECKF(errors, WORD_DICT_TEST_RESULT_REPLACEMENTS, dictionary[iArea].Indices[0] != 7, "Area alias record:%u, expected:7.", dictionary[iArea].Indices[0]);
		LLC_TEST_CHECKF(errors, WORD_DICT_TEST_RESULT_REPLACEMENTS, dictionary[iArea].Indices[1] != 11, "Area literal record:%u, expected:11.", dictionary[iArea].Indices[1]);
	}
	LLC_TEST_CHECKF(errors, WORD_DICT_TEST_RESULT_POSTINGS, dictionary[iBeta].Indices.size() != 1 || dictionary[iBeta].Indices[0] != 7, "Beta posting count:%u.", dictionary[iBeta].Indices.size());
	LLC_TEST_CHECKF(errors, WORD_DICT_TEST_RESULT_POSTINGS, dictionary[iGamma].Indices.size() != 1 || dictionary[iGamma].Indices[0] != 11, "Gamma posting count:%u.", dictionary[iGamma].Indices.size());
	LLC_TEST_CHECKF(errors, WORD_DICT_TEST_RESULT_REPLACEMENTS, dictionary[iAccentedArea].Indices.size() != 1 || dictionary[iAccentedArea].Indices[0] != 7, "Accented area posting count:%u.", dictionary[iAccentedArea].Indices.size());
	rtrn 0;
}

sttc ::llc::err_t testCustomSkipWordsDemo(::llc::ATestError & errors) {
	cnst ::llc::vcst_t skipWords[] = {"request", "status"};
	cnst ::llc::vcst_t fields[] = {"request build status failed"};
	::llc::SWordDict dictionary;
	::llc::aobj<::llc::string> scratch;
	if_fail_fe(::llc::getWordDict(dictionary, fields, 42, scratch, skipWords));

	LLC_TEST_CHECK(errors, WORD_DICT_TEST_RESULT_CUSTOM_SKIP_WORDS, 0 <= ::wordIndex(dictionary, "request") || 0 <= ::wordIndex(dictionary, "status"));
	LLC_TEST_CHECK(errors, WORD_DICT_TEST_RESULT_CUSTOM_SKIP_WORDS, 0 > ::wordIndex(dictionary, "build") || 0 > ::wordIndex(dictionary, "failed"));
	rtrn 0;
}

::llc::err_t testWordDict(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	if_fail_fe(::testCatalogDemo(errors));
	rtrn ::testCustomSkipWordsDemo(errors);
}
