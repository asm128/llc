#include "llc_view_bit.h"
#include "llc_array.h"

#ifndef LLC_ARRAY_BIT_H_23627
#define LLC_ARRAY_BIT_H_23627

namespace llc
{
	tplt<tpnm _tType>
	struct array_bit {
		typedef	_tType			T;
		typedef	vbit<T>	TView;

		apod<T>			Data			;
		view_bit<T>		View			;

		cxpr					array_bit		()					noexcept	= default;
								array_bit		(const view_bit<T> & other)
			:	Data(view<cnst T>{other.begin().Begin, round_up(other.size(), (u2_t)TView::ELEMENT_BITS)})
			,	View(Data.begin(), other.size())
			{}
								array_bit		(const array_bit<T> & other)
			: Data(other.Data)
			, View(Data.begin(), other.View.size())
			{}

		array_bit<T>&	operator=			(const view_bit<T> & other) {
			Data					= view<cnst T>{other.begin().Begin, round_up(other.size(), (u2_t)TView::ELEMENT_BITS)};
			View					= {Data.begin(), other.size()};
			return *this;
		}

		array_bit<T>&	operator=			(const array_bit<T> & other) {
			Data					= other.Data;
			View					= {Data.begin(), other.View.size()};
			return *this;
		}

		inxp	const u2_t&		size			()				const	noexcept	{ return View.size(); }

		err_t			resize			(u2_t newSize)			noexcept	{
			if_fail_fe(Data.resize(round_up(newSize, (u2_t)TView::ELEMENT_BITS)));
			View					= {Data.begin(), newSize};
			return 0;
		}
	}; // struct

	tplt<tpnm T>
	using	abit	= array_bit<T>;

	tydf	abit<u0_t>	abitu0_t;
	tydf	abit<u1_t>	abitu1_t;
	tydf	abit<u2_t>	abitu2_t;
	tydf	abit<u3_t>	abitu3_t;
} // namespace llc

#endif // LLC_ARRAY_BIT_H_23627
