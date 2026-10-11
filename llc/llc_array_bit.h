#include "llc_view_bit.h"
#include "llc_array.h"

#ifndef LLC_ARRAY_BIT_H_23627
#define LLC_ARRAY_BIT_H_23627

namespace llc
{
	tplt<tpnm _tType>
	struct array_bit {
		typedef	_tType			T;
		typedef	::llc::vbit<T>	TView;

		::llc::apod<T>			Data			;
		::llc::view_bit<T>		View			;

		cnstxpr					array_bit		()					noexcept	= default;
								array_bit		(const ::llc::view_bit<T> & other)
			:	Data(::llc::view<cnst T>{other.begin().Begin, round_up(other.size(), TView::ELEMENT_BITS)})
			,	View(Data.begin(), other.size())
			{}
								array_bit		(const ::llc::array_bit<T> & other)
			: Data(other.Data)
			, View(Data.begin(), other.View.size())
			{}

		::llc::array_bit<T>&	operator=			(const ::llc::view_bit<T> & other) {
			Data					= ::llc::view<cnst T>{other.begin().Begin, round_up(other.size(), TView::ELEMENT_BITS)};
			View					= {Data.begin(), other.size()};
			return *this;
		}

		::llc::array_bit<T>&	operator=			(const ::llc::array_bit<T> & other) {
			Data					= other.Data;
			View					= {Data.begin(), other.View.size()};
			return *this;
		}

		inlcxpr	const u2_t&		size			()				const	noexcept	{ return View.size(); }

		::llc::error_t			resize			(u2_t newSize)			noexcept	{
			llc_necs(Data.resize(round_up(newSize, TView::ELEMENT_BITS)));
			View					= {Data.begin(), newSize};
			return 0;
		}
	}; // struct

	tplt<tpnm T>
	using	abit	= ::llc::array_bit<T>;

	tydf	abit<u0_t>	abitu0_t;
	tydf	abit<u1_t>	abitu1_t;
	tydf	abit<u2_t>	abitu2_t;
	tydf	abit<u3_t>	abitu3_t;
} // namespace llc

#endif // LLC_ARRAY_BIT_H_23627
