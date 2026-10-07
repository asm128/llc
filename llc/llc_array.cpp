#include "llc_array.h"
#include "llc_stdstring.h"

llc::err_t	llc::join			(asc_t & output, sc_t separator, view<cnst vcst_t> fields) {
	err_t			appended			= 0;
	err_t			result				= {};
	for(u2_t iField = 0; iField < fields.size(); ++iField) {
		if(iField) {
			if_fail_fef(result = output.append_string(separator), "Failed to append separator before field:%" LLC_FMT_U2 ".", iField);
			appended			+= result;
		}
		if_fail_fef(result = output.append_string(fields[iField]), "Failed to append field:%" LLC_FMT_U2 ".", iField);
		appended				+= result;
	}
	rtrn appended;
}

llc::err_t	llc::filterPostfix	(view<cnst vcst_t> input, vcst_t postfix, aobj<vcst_t> & filtered, bool nullIncluded) { 
	for(uint32_t iInput = 0; iInput < input.size(); ++iInput) { 
		vcst_t	currentInput = input[iInput]; 
		if((postfix.size() < currentInput.size() || (nullIncluded && postfix.size() == currentInput.size())) && 0 == strncmp(currentInput.end() - postfix.size(), postfix.begin(), postfix.size())) 
			filtered.push_back(currentInput); 
	} 
	return 0; 
}

llc::err_t	llc::filterPrefix	(view<cnst vcst_t> input, vcst_t prefix, aobj<vcst_t> & filtered, bool nullIncluded) { 
	for(uint32_t iInput = 0; iInput < input.size(); ++iInput) { 
		vcst_t	currentInput = input[iInput]; 
		if((prefix .size() < currentInput.size() || (nullIncluded && prefix .size() == currentInput.size())) && 0 == strncmp(currentInput.begin(), prefix .begin(), prefix .size())) 
			if_fail_fe(filtered.push_back(currentInput)); 
	}
	return 0; 
}
