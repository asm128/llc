#include  "llc_word_dict.h"
#include  "llc_string_compose.h"

namespace llc {
	sttc llc::err_t  getWordList	(aobj<string> & indexable, view<vcst_c> item, view<vcst_c> skipWords) {
		aobj<vcst_t>		upper			= {};
		for(vcst_t text : item)
			if_fail_fe(split(text, ' ', upper));

		if_fail_fe(indexable.resize(upper.size()));
		cnst kv<vcst_t>		replaceDict	[]	= {{"\xE1\xE9\xED\xF3\xFA\xE0\xE8\xEC\xF2\xF9\xE4\xEB\xEF\xF6\xFC\xE3\xF5\xF1\xE7\xBA\xAA", "aeiouaeiouaeiouaoncoa"}};
		string				fixedWord;
		for(u2_t iWord = 0, countWords = upper.size(); iWord < countWords; ++iWord) {
			vcst_t				originalWord	= upper[iWord];
			trim(originalWord, originalWord);
			string				& finalWord		= indexable[iWord];
			finalWord.clear();
			if_fail_fe(append_strings(finalWord, originalWord));
			tolower(finalWord);
			if_true_block_log(info_printf, 0 <= skipWords.find(finalWord), { finalWord.clear(); continue; });

			info_printf("finalWord: %.*s.", (int)finalWord.size(), finalWord.begin());
			fixedWord		= finalWord;
			bool				replaced		= false;
			for(u2_t iChar = 0; iChar < fixedWord.size(); ++iChar) {
				char				& wordChar		= fixedWord[iChar];
				for(u2_t iTable = 0; iTable < size(replaceDict); ++iTable) {
					cnst kv<vcst_t>		& table			= replaceDict[iTable];
					for(u2_t iItem = 0; iItem < table.Key.size(); ++iItem)
						if(wordChar == table.Key[iItem]) {
							wordChar	= table.Val[iItem];
							replaced	= true;
						}
				}
			}
			if(replaced) {
				if_fail_fe(indexable.push_back(fixedWord));
				info_printf("fixedWord: %.*s.", (int)fixedWord.size(), fixedWord.begin());
			}
		}
		rtrn 0;
	}
} // namespace

llc::err_t		llc::getWordDict		(SWordDict & wordIndex, view<vcst_c> item, u2_t iItem, aobj<string> & indexable, view<vcst_c> skipWords) {
	if_fail_fe(getWordList(indexable, item, skipWords));
	for(u2_t iItemWord = 0; iItemWord < indexable.size(); ++iItemWord) {
		vcst_t	itemWord	= indexable[iItemWord];
		info_printf("itemWord: %.*s.", (int)itemWord.size(), itemWord.begin());
		if(1 >= itemWord.size()) {
			continue;
		}
		bool	bFound	= false;
		if(',' == itemWord[itemWord.size() - 1] || '.' == itemWord[itemWord.size() - 1]) {
			itemWord	= {itemWord.begin(), itemWord.size() - 1};
			if(0 >= itemWord.size())
				continue;
		}
		for(u2_t iWordInSet = 0; iWordInSet < wordIndex.size(); ++iWordInSet) {
			SWordIndices	& dictWord	= wordIndex[iWordInSet];
			if(itemWord != dictWord.Text) {
				continue;
			}
			bFound	= true;
			if(0 > find(iItem, {dictWord.Indices.begin(), dictWord.Indices.size()}))
				if_fail_fe(dictWord.Indices.push_back(iItem));
		}
		if(false == bFound) {
			cnst err_t indexWord = wordIndex.push_back({itemWord});
			if_fail_fe(indexWord);
			if_fail_fe(wordIndex[indexWord].Indices.push_back(iItem));
		}
	}
	rtrn 0;
}
