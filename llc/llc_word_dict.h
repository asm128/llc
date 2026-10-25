#include "llc_array_obj.h"
#include "llc_string.h"
#include "llc_stdstring.h"
#include "llc_keyval.h"

#ifndef LLC_WORD_DICT_H_23627
#define LLC_WORD_DICT_H_23627

namespace llc {
	// Common stop words, pronouns and articles that are not convenient to consider for a search index
	stxp	vcst_t	WORDDICT_TEXT_SKIPWORDS[]	= {"the", "and", "it" , "to" , "an" , "by" , "was", "is" , "in" , "as" , "for", "of" , "up" , "a"  , "un" , "con", "del", "por", "que", "es" , "en" , "de" , "la" , "el" , "para", "los", "las"};

	stct SWordIndices {
		string			Text;
		au2_t			Indices;
	};

	tydf	aobj	<SWordIndices>				SWordDict;
			err_t	getWordDict					(SWordDict & wordIndex, view<vcst_c> item, u2_t iItem, aobj<string> & indexable, view<vcst_c> skipWords = WORDDICT_TEXT_SKIPWORDS);
}

#endif // LLC_WORD_DICT_H_23627
