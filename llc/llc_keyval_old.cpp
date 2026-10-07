#include "llc_keyval_old.h"
#include "llc_safe.h"
#include "llc_parse.h"
#include "llc_apod_serialize.h"

llc::err_t			llc::token_split		(char valueSeparator, vcst_t input_string, TKeyValConstChar & output_views)	{
	int32_t						indexSeparator;
	if_fail_wf(indexSeparator = find(valueSeparator, input_string), "'%c' character not found.", valueSeparator);
	output_views.Key		= {input_string.begin(), (uint32_t)indexSeparator};

	llc::b8_c					hasValue			= indexSeparator > 0 && uint32_t(indexSeparator + 1) < input_string.size();
	output_views.Val		= hasValue
		? vcst_t{&input_string[indexSeparator + 1U], input_string.size() - (indexSeparator + 1U)}
		: vcst_t{}	// empty view if there's no data after the separator.
		;
	trim(output_views.Key, output_views.Key);
	trim(output_views.Val, output_views.Val);
	return output_views.Val.size();
}

llc::err_t			llc::keyvalNumeric		(vcst_t key, cnst view<cnst TKeyValConstString> keyVals, uint64_t & outputNumber)	{
	llc::err_t				indexKey;
	if_fail_fwf(indexKey = find(key, keyVals), "key:\"%.*s\"", key.size(), key.begin());
	if_fail_vef(-2, parseIntegerDecimal(keyVals[indexKey].Val, outputNumber), "%.*s", keyVals[indexKey].Val.size(), keyVals[indexKey].Val.begin());
	return indexKey;
}

llc::err_t			llc::keyValVerify		(view<cnst TKeyValConstString> environViews, vcst_t keyToVerify, vcst_t valueToVerify)	{
	for(uint32_t iKey = 0; iKey < environViews.size(); ++iKey) {
		if(environViews[iKey].Key == keyToVerify)
			return (environViews[iKey].Val == valueToVerify) ? iKey : -1;
	}
	return -1;
}

llc::err_t			llc::keyValConstStringSerialize		(view<cnst TKeyValConstString> keyVals, view<cnst vcst_t> keysToSave, au0_t & output)	{
	apod<TKeyValConstString>	keyValsToSave					= {};
	for(uint32_t iKey = 0; iKey < keyVals.size(); ++iKey) {
		for(uint32_t iRef = 0; iRef < keysToSave.size(); ++iRef) {
			cnst TKeyValConstString	& kvToCheck						= keyVals[iKey];
			vcsc_c				& keyToSave						= keysToSave[iRef];
			if(kvToCheck.Key == keyToSave)
				keyValsToSave.push_back(kvToCheck);
		}
	}
	output.append((cnst uint8_t*)&keyValsToSave.size(), szof(uint32_t));
	uint32_t					iOffset								= 0;
	for(uint32_t iKey = 0; iKey < keyValsToSave.size(); ++iKey) {
		iOffset					+= saveView(output, keyValsToSave[iKey].Key);
		iOffset					+= saveView(output, keyValsToSave[iKey].Val);
	}
	return 0;
}

llc::err_t			llc::keyValConstStringDeserialize	(vcu0_t input, aobj<TKeyValConstString> & output)	{
	uint32_t					offset								= 0;
	u2_c				keysToRead							= *(u2_c*)input.begin();
	offset					+= (uint32_t)szof(uint32_t);
	output.resize(keysToRead);
	for(uint32_t iKey = 0; iKey < keysToRead; ++iKey) {
		offset					+= viewRead(output[iKey].Key, {&input[offset], input.size() - offset});
		offset					+= viewRead(output[iKey].Val, {&input[offset], input.size() - offset});
	}
	return 0;
}
