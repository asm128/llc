#include "llc_view.h"

#ifndef LLC_KEYVAL_H_23627
#define LLC_KEYVAL_H_23627

namespace llc
{
	tydf	keyval<vcsc_t, vcsc_t>	TKeyValConstChar, TKeyValConstString;

			error_t					token_split			(char valueSeparator, vcst_c & input_string, TKeyValConstChar& output_views);
	inln	error_t					keyval_split		(const vcs& input_string, TKeyValConstString& out_keyval) { return token_split('=', input_string, out_keyval); }

			error_t					keyValVerify		(const view<TKeyValConstString> & environViews, vcsc_c & keyToVerify, vcsc_c & valueToVerify);
			error_t					keyvalNumeric		(vcst_t key, const view<const TKeyValConstString> keyVals, uint64_t & outputNumber);
	tplt <tpnm _tNumeric>
			error_t					keyvalNumeric		(const vcs & key, const view<const TKeyValConstString> keyVals, _tNumeric & outputNumber)	{
		uint64_t							value				= 0;
		error_t								indexKey			= keyvalNumeric(key, keyVals, value);
		if(-1 != indexKey)
			outputNumber				= value;
		return indexKey;
	}

	tplt <tpnm... _tArgs>
			error_t					keyValVerify		(const view<TKeyValConstString> & environViews, vcsc_c & keyToVerify, const view<vcsc_c>& valueToVerify)	{
		for(uint32_t iKey = 0; iKey < valueToVerify.size(); ++iKey) {
			err_c						val						= keyValVerify(environViews, keyToVerify, valueToVerify[iKey]);
			if(-1 != val)
				return val;
		}
		return -1;
	}


} // namespace

#endif // LLC_KEYVAL_H_23627
