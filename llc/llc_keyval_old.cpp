#include "llc_keyval_old.h"
#include "llc_safe.h"
#include "llc_parse.h"
#include "llc_apod_serialize.h"

::llc::asc_t		llc::toString			(::llc::vcsc_c & strToLog)	{
	::llc::asc_t			sprintfable				= strToLog;
	if(sprintfable.size() && sprintfable[sprintfable.size() - 1] == 0) { // it already contains a null, so resize it to avoid counting it as part of the array.
		sprintfable.resize(sprintfable.size() - 1);
		return sprintfable;
	}

	es_if(::llc::failed(sprintfable.push_back(0)))
	else
		es_if(::llc::failed(sprintfable.resize(sprintfable.size()-1)));

	return sprintfable;
}

::llc::error_t			llc::token_split		(char valueSeparator, const ::llc::vcst_t & input_string, TKeyValConstChar & output_views)	{
	int32_t						indexSeparator;
	if_fail_wf(indexSeparator = ::llc::find(valueSeparator, input_string), "'%c' character not found.", valueSeparator);
	output_views.Key		= {input_string.begin(), (uint32_t)indexSeparator};

	llc::b8_c					hasValue			= indexSeparator > 0 && uint32_t(indexSeparator + 1) < input_string.size();
	output_views.Val		= hasValue
		? ::llc::vcst_t{&input_string[indexSeparator + 1U], input_string.size() - (indexSeparator + 1U)}
		: ::llc::vcst_t{}	// empty view if there's no data after the separator.
		;
	::llc::trim(output_views.Key, output_views.Key);
	::llc::trim(output_views.Val, output_views.Val);
	return output_views.Val.size();
}

::llc::error_t			llc::keyvalNumeric		(::llc::vcst_t key, const ::llc::view<const ::llc::TKeyValConstString> keyVals, uint64_t & outputNumber)	{
	::llc::error_t				indexKey;
	if_fail_fwf(indexKey = ::llc::find(key, keyVals), "key:\"%.*s\"", key.size(), key.begin());
	if_fail_vef(-2, ::llc::parseIntegerDecimal(keyVals[indexKey].Val, outputNumber), "%.*s", keyVals[indexKey].Val.size(), keyVals[indexKey].Val.begin());
	return indexKey;
}

::llc::error_t			llc::keyValVerify		(const ::llc::view<::llc::TKeyValConstString> & environViews, ::llc::vcsc_c & keyToVerify, ::llc::vcsc_c & valueToVerify)	{
	for(uint32_t iKey = 0; iKey < environViews.size(); ++iKey) {
		if(environViews[iKey].Key == keyToVerify)
			return (environViews[iKey].Val == valueToVerify) ? iKey : -1;
	}
	return -1;
}

::llc::error_t			llc::keyValConstStringSerialize		(const ::llc::view<const ::llc::TKeyValConstString> & keyVals, ::llc::vcvsc_c & keysToSave, ::llc::au0_t & output)	{
	::llc::apod<::llc::TKeyValConstString>	keyValsToSave					= {};
	for(uint32_t iKey = 0; iKey < keyVals.size(); ++iKey) {
		for(uint32_t iRef = 0; iRef < keysToSave.size(); ++iRef) {
			const ::llc::TKeyValConstString	& kvToCheck						= keyVals[iKey];
			::llc::vcsc_c				& keyToSave						= keysToSave[iRef];
			if(kvToCheck.Key == keyToSave)
				keyValsToSave.push_back(kvToCheck);
		}
	}
	output.append((const uint8_t*)&keyValsToSave.size(), szof(uint32_t));
	uint32_t					iOffset								= 0;
	for(uint32_t iKey = 0; iKey < keyValsToSave.size(); ++iKey) {
		iOffset					+= ::llc::saveView(output, keyValsToSave[iKey].Key);
		iOffset					+= ::llc::saveView(output, keyValsToSave[iKey].Val);
	}
	return 0;
}

::llc::error_t			llc::keyValConstStringDeserialize	(vcu0_c & input, ::llc::aobj<::llc::TKeyValConstString> & output)	{
	uint32_t					offset								= 0;
	u2_c				keysToRead							= *(u2_c*)input.begin();
	offset					+= (uint32_t)szof(uint32_t);
	output.resize(keysToRead);
	for(uint32_t iKey = 0; iKey < keysToRead; ++iKey) {
		offset					+= ::llc::viewRead(output[iKey].Key, {&input[offset], input.size() - offset});
		offset					+= ::llc::viewRead(output[iKey].Val, {&input[offset], input.size() - offset});
	}
	return 0;
}
