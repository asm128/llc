#include "llc_string.h"
#include "llc_stdstring.h"

LLC_USING_VIEW();

stxp vcst_c	LLC_CAMELCASE_WORD_SEPARATORS	= "-_."; 
llc::err_t	llc::camelCase		(vcst_t input, asc_t & camelCased) {
	bool						capsNext			= true;
	for(::llc::u2_t i = 0; i < input.size(); ++i) {
		char						current				= input[i];
		if(is_any_of(current, LLC_CAMELCASE_WORD_SEPARATORS))
			capsNext	= true;
		else {
			if(capsNext) {
				::llc::toupper(current);
				capsNext	= false;
			}
			//else
			//	tolower(vc{&camelCased[1], camelCased.size() - 1});
			if_fail_fe(camelCased.push_back(current));
		}
	}
	return 0;
}
