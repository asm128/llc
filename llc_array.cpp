#include "llc_array.h"
#include "llc_stdstring.h"

::llc::error_t			llc::filterPostfix	(::llc::view<::llc::vcsc_c> input, ::llc::vcsc_c postfix, ::llc::aobj<::llc::vcsc_t> & filtered, bool nullIncluded) { 
	for(uint32_t iInput = 0; iInput < input.size(); ++iInput) { 
		::llc::vcsc_c & currentInput = input[iInput]; 
		if((postfix.size() < currentInput.size() || (nullIncluded && postfix.size() == currentInput.size())) && 0 == strncmp(currentInput.end() - postfix.size(), postfix.begin(), postfix.size())) 
			filtered.push_back(currentInput); 
	} 
	return 0; 
}

::llc::error_t			llc::filterPrefix	(::llc::view<::llc::vcsc_c> input, ::llc::vcsc_c prefix, ::llc::aobj<::llc::vcsc_t> & filtered, bool nullIncluded) { 
	for(uint32_t iInput = 0; iInput < input.size(); ++iInput) { 
		::llc::vcsc_c & currentInput = input[iInput]; 
		if((prefix .size() < currentInput.size() || (nullIncluded && prefix .size() == currentInput.size())) && 0 == strncmp(currentInput.begin(), prefix .begin(), prefix .size())) 
			filtered.push_back(currentInput); 
	}
	return 0; 
}

::llc::error_t			llc::camelCase		(::llc::vcsc_t input, ::llc::asc_t & camelCased) {
	bool						capsNext			= true;
	for(uint32_t i = 0; i < input.size(); ++i) {
		char						current				= input[i];
		if(current == '_' || current == '-')
			capsNext	= true;
		else {
			if(capsNext) {
				::llc::toupper(current);
				capsNext	= false;
			}
			//else
			//	::llc::tolower(::llc::vc{&camelCased[1], camelCased.size() - 1});
			camelCased.push_back(current);
		}
	}
	return 0;
}
