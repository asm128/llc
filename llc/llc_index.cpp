#include "llc_index.h"
#include "llc_parse.h"
#include "llc_keyval_view.h"
#include "llc_stdstring.h"
#include "llc_string_compose.h"

LLC_USING_TYPEINT();
LLC_USING_VIEW();
LLC_USING_APOD();

usng nmsp llc;

llc::err_t	llc::wordListNormalize			(aobj<string> & itemWords, view<vcst_c> upper, view<vcst_c> exceptions) {
	if_fail_fe(itemWords.resize(upper.size()));
	cnst kv<vcst_t>			replaceDict	[]						=
		{	{ "áéíóúàèìòùäëïöüãõñçºª"
			, "aeiouaeiouaeiouaoncoa"
			}
		};
	string					fixedWord;
	for(u2_t iWord = 0, countWords = upper.size(); iWord < countWords; ++iWord) {
		vcst_t					originalWord						= upper[iWord];
		trim(originalWord, originalWord);
		string					& finalWord							= itemWords[iWord];
		finalWord.clear();
		if_fail_fe(append_strings(finalWord, originalWord));
		tolower(finalWord);
		if(0 <= exceptions.find(finalWord)) {
			finalWord.clear();
			continue;
		}
		info_printf("finalWord: %.*s.", LLC_VIEW_ARGS(finalWord));
		fixedWord.clear();
		b8_t					replaced		= false;
		for(u2_t iChar = 0; iChar < finalWord.size(); ++iChar) {
			b8_t found = false;
			for(u2_t iTable = 0; iTable < size(replaceDict); ++iTable) {
				cnst kv<vcst_t> &	table = replaceDict[iTable];
				if(iChar + 1 >= finalWord.size())
					continue;
				cnst err_t iItem = table.Key.find(vcst_t{&finalWord[iChar], 2});
				if(0 <= iItem && 0 == (iItem % 2)) {
					if_fail_fe(fixedWord.push_back(table.Val[iItem / 2]));
					++iChar;
					found = replaced = true;
					break;
				}
			}
			if(false == found)
				if_fail_fe(fixedWord.push_back(finalWord[iChar]));
		}
		if(replaced) {
			if_fail_fe(itemWords.push_back(fixedWord));
			info_printf("fixedWord: %.*s.", LLC_VIEW_ARGS(fixedWord));
		}
	}
	return 0;
}

llc::err_t	llc::getWordList				(aobj<string> & itemWords, view<vcst_c> upper, view<vcst_c> exceptions) {
	rtrn wordListNormalize(itemWords, upper, exceptions);
}

llc::err_t	llc::wordDictAppend				(aobj<SWordIndices> & wordSet, u2_t iItem, view<cnst string> itemWords) {
	for(u2_t iItemWord = 0; iItemWord < itemWords.size(); ++iItemWord) {
		vcst_t			itemWord			= itemWords[iItemWord];
		info_printf("itemWord: %.*s.", LLC_VIEW_ARGS(itemWord));
		if(1 >= itemWord.size())
			continue;
		b8_t			bFound				= false;
		if(',' == itemWord[itemWord.size() - 1] || '.' == itemWord[itemWord.size() - 1]) {
			if_fail_fe(itemWord.slice(itemWord, 0, itemWord.size()-1));
			if(0 >= itemWord.size())
				continue;
		}
		for(u2_t iWordInSet = 0; iWordInSet < wordSet.size(); ++iWordInSet) {
			SWordIndices	& dictWord			= wordSet[iWordInSet];
			if(itemWord != dictWord.Text)
				continue;
			bFound		= true;
			if(0 > find(iItem, {dictWord.Indices.begin(), dictWord.Indices.size()}))
				if_fail_fe(dictWord.Indices.push_back(iItem));
		}
		if(false == bFound)
			if_fail_fe(wordSet[wordSet.push_back({itemWord})].Indices.push_back(iItem));
	}
	return 0;
}

llc::err_t	llc::wordDictResolve	(view<cnst SWordIndices> wordSet, vcst_t input, u2_t itemCount, au2_t & itemIndices, view<vcst_c> exceptions) {
	if_fail_fe(itemIndices.clear());
	aobj<vcst_t>		inputWords						= {};
	aobj<string>		normalizedWords					= {};
	au0_t				matchedItems						= {};
	if_fail_fe(split(input, ' ', inputWords));
	if_fail_fe(wordListNormalize(normalizedWords, inputWords, exceptions));
	if_fail_fe(matchedItems.resize(itemCount));
	matchedItems.fill(0);
	for(u2_t iInputWord = 0; iInputWord < normalizedWords.size(); ++iInputWord) {
		vcst_c		inputWord						= normalizedWords[iInputWord];
		if(0 == inputWord.size())
			continue;
		for(u2_t iWord = 0; iWord < wordSet.size(); ++iWord) {
			cnst SWordIndices & indexedWord					= wordSet[iWord];
			if(0 > indexedWord.Text.find(inputWord))
				continue;
			for(u2_t iPosting = 0; iPosting < indexedWord.Indices.size(); ++iPosting) {
				u2_c		itemIndex						= indexedWord.Indices[iPosting];
				if_true_fef(itemIndex >= itemCount, "Invalid item index: %u. Item count: %u.", itemIndex, itemCount);
				matchedItems[itemIndex] = 1;
			}
		}
	}
	for(u2_t iItem = 0; iItem < matchedItems.size(); ++iItem)
		if(matchedItems[iItem])
			if_fail_fe(itemIndices.push_back(iItem));
	rtrn (err_t)itemIndices.size();
}
