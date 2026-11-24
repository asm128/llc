#include "llc_array_ptr.h"
#include "llc_label.h"
#include "llc_apod_serialize.h"
#include "llc_enum.h"

#ifndef LLC_LINEAR_MAP_POD_H_23627
#define LLC_LINEAR_MAP_POD_H_23627

namespace llc
{
	tplt <tpnm _tKey, tpnm _tValue>
	struct linear_map_pod {
		typedef _tValue		T;
		typedef _tKey		K;

		::llc::apod<T>		Values			= {};
		::llc::apod<K>		Keys			= {};

		const T&			operator[]		(const K & key)	const	{ return Values[Keys.find(key)]; }
		T&					operator[]		(const K & key)			{ return Values[Keys.find(key)]; }

		::llc::u2_t			size			()				const	{ return Keys.size(); }

		::llc::err_t		push_back		(const K & key, const T & instance)	{
			if_true_fef(0 <= Keys.find(key), "%s", "Duplicate key.");
			::llc::err_t				index			= 0;
			if_fail_fe(index = Keys.push_back(key));
			cnst ::llc::err_t		valueIndex		= Values.push_back(instance);
			if(::llc::failed(valueIndex)) {
				Keys.pop_back();
				return valueIndex;
			}
			return index;
		}

		::llc::err_t		Create			(const K & key)							{
			return push_back(key, T{});
		}

		::llc::err_t		Delete			(const K & key)					{
			cnst ::llc::err_t		index			= Keys.find(key);
			if_fail_fe(index);
			if_fail_fe(Keys.remove_unordered(index));
			return Values.remove_unordered(index);
		}

		::llc::err_t		Clone			(const K & key, ::llc::u2_t index)	{
			if_true_fef(index >= Values.size(), "Invalid source index:%u. Value count:%u.", index, Values.size());
			return push_back(key, Values[index]);
		}

		::llc::err_t		Save			(::llc::au0_t & output) const {
			if_true_fef(Keys.size() != Values.size(), "Key count:%u. Value count:%u.", Keys.size(), Values.size());
			if_fail_fe(llc::saveView(output, Keys));
			if_fail_fe(llc::saveView(output, Values));
			return 0;
		}

		::llc::err_t		Load			(::llc::vcu0_t & input) {
			::llc::vcu0_t			inputToRead		= input;
			::llc::apod<K>			loadedKeys		= {};
			::llc::apod<T>			loadedValues	= {};
			if_fail_fe(llc::loadView(inputToRead, loadedKeys));
			if_fail_fe(llc::loadView(inputToRead, loadedValues));
			if_true_fef(loadedKeys.size() != loadedValues.size(), "Key count:%u. Value count:%u.", loadedKeys.size(), loadedValues.size());
			for(::llc::u2_t iKey = 0; iKey < loadedKeys.size(); ++iKey)
				if_true_fef(0 <= loadedKeys.find(loadedKeys[iKey], iKey + 1), "Duplicate key at index:%u.", iKey);
			Keys		= loadedKeys;
			Values		= loadedValues;
			input		= inputToRead;
			return 0;
		}
	};
	tplt <tpnm _tKey, tpnm _tValue>
	using	lmpod	= linear_map_pod<_tKey, _tValue>;
} // namespace

#endif // LLC_LINEAR_MAP_POD_H_23627
