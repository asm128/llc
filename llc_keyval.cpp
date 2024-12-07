#include "llc_keyval.h"
#include "llc_safe.h"
#include "llc_parse.h"
#include "llc_apod_serialize.h"

::llc::error_t			llc::join				(::llc::asc_t & output, char separator, ::llc::vcvsc_t fields)	{
	for(uint32_t iField = 0; iField < fields.size();) {
		llc_necall(output.append(fields[iField]), "%" LLC_FMT_U2 "", iField);
		if(++iField < fields.size())
			llc_necall(output.push_back(separator), "%" LLC_FMT_U2 "", iField);
	}
	return 0;
}

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

::llc::error_t			llc::token_split		(char token, const ::llc::vcst_t & input_string, TKeyValConstChar & output_views)	{
	int32_t						indexToken;
	llc_necall(indexToken = ::llc::find(token, input_string), "'%c' not found.", token);
	output_views.Key		= {input_string.begin(), (uint32_t)indexToken};
	output_views.Val		= (uint32_t(indexToken + 1) < input_string.size())
		? ::llc::vcst_t{&input_string[indexToken + 1U], input_string.size() - (indexToken + 1U)}
		: ::llc::vcst_t{}	// empty view if there's no data after the separator.
		;
	::llc::trim(output_views.Key, output_views.Key);
	::llc::trim(output_views.Val, output_views.Val);
	return 0;
}

::llc::error_t			llc::find				(const ::llc::vcst_t & keyToFind, const ::llc::view<const ::llc::TKeyValConstString> & keyvals, ::llc::vcsc_t & out_val)		{
	::llc::error_t				index					= ::llc::find(keyToFind, keyvals);
	out_val					= (-1 == index) ? ::llc::vcst_t{} : keyvals[index].Val;
	return index;
}

::llc::error_t			llc::keyvalNumeric		(const ::llc::vcst_t & key, const ::llc::view<const ::llc::TKeyValConstString> keyVals, uint64_t * outputNumber)	{
	ree_if(0 == outputNumber, "%s", "Output number cannot point to a null address.");
	::llc::error_t				indexKey				= ::llc::find(key, keyVals);
	if(-1 != indexKey)
		::llc::parseIntegerDecimal(keyVals[indexKey].Val, outputNumber);

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
