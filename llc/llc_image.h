#include "llc_array_pod.h"
#include "llc_grid.h"

#ifndef LLC_IMAGE_H_23627
#define LLC_IMAGE_H_23627

namespace llc
{
	tplt<tpnm TCell>
	stct img {
		tydf TCell			T;
		tydf cnst TCell		TConst;
		tydf view<T>			TView;
		tydf view<TConst>		TViewConst;
		tydf grid<T>			TGrid;

		apod<T>				Texels	= {};
		grid<T>				View	= {};

		inxp				img		() = default;
						img		(cnst grid<T> & other) : Texels(view<cnst T>{other.begin(), other.size()}) {
			View = {Texels.begin(), other.metrics()};
		}
						img		(cnst img<T> & other) : Texels(other.Texels) {
			View = {Texels.begin(), other.View.metrics()};
		}

		inxp	oper		grid<cnst T>	() cnst nxpt { rtrn View; }
		inln	oper		grid<T>		() nxpt { rtrn View; }

		img&				oper=		(cnst grid<T> & other) {
			Texels = view<cnst T>{other.begin(), other.size()};
			View = {Texels.begin(), other.metrics()};
			rtrn *this;
		}
		img&				oper=		(cnst img<T> & other) {
			Texels = other.Texels;
			View = {Texels.begin(), other.View.metrics()};
			rtrn *this;
		}

		inln	TView		oper[]		(u2_t row) { rtrn View[row]; }
		inln	TViewConst	oper[]		(u2_t row) cnst { rtrn View[row]; }
		inxp	cnst T*		begin		() cnst nxpt { rtrn View.begin(); }
		inxp	cnst T*		end			() cnst nxpt { rtrn View.end(); }
		inxp	T*			begin		() nxpt { rtrn View.begin(); }
		inxp	T*			end			() nxpt { rtrn View.end(); }
		inxp	n2u2_c&		metrics		() cnst nxpt { rtrn View.metrics(); }
		inxp	u2_c&		size		() cnst nxpt { rtrn Texels.size(); }
		inxp	u2_t			area		() cnst nxpt { rtrn View.area(); }
		inxp	u2_t			byte_count	() cnst nxpt { rtrn Texels.byte_count(); }

		inln	err_t		resize		(n2s0_c & newSize) nxpt { rtrn resize(newSize.u2()); }
		inln	err_t		resize		(n2s1_c & newSize) nxpt { rtrn resize(newSize.u2()); }
		inln	err_t		resize		(n2s2_c & newSize) nxpt { rtrn resize(newSize.u2()); }
		inln	err_t		resize		(n2u0_c & newSize) nxpt { rtrn resize(newSize.u2()); }
		inln	err_t		resize		(n2u1_c & newSize) nxpt { rtrn resize(newSize.u2()); }
		err_t				resize		(n2u2_c & newSize) nxpt {
			if_fail_fef(Texels.resize(newSize.x * newSize.y), "Cannot resize image to %u x %u.", newSize.x, newSize.y);
			View = {Texels.begin(), newSize.x, newSize.y};
			rtrn 0;
		}

		inln	err_t		resize		(n2s0_c & newSize, cnst T & value) nxpt { rtrn resize(newSize.u2(), value); }
		inln	err_t		resize		(n2s1_c & newSize, cnst T & value) nxpt { rtrn resize(newSize.u2(), value); }
		inln	err_t		resize		(n2s2_c & newSize, cnst T & value) nxpt { rtrn resize(newSize.u2(), value); }
		inln	err_t		resize		(n2u0_c & newSize, cnst T & value) nxpt { rtrn resize(newSize.u2(), value); }
		inln	err_t		resize		(n2u1_c & newSize, cnst T & value) nxpt { rtrn resize(newSize.u2(), value); }
		err_t				resize		(n2u2_c & newSize, cnst T & value) nxpt {
			if_fail_fef(Texels.resize(newSize.x * newSize.y), "Cannot resize image to %u x %u.", newSize.x, newSize.y);
			View = {Texels.begin(), newSize.x, newSize.y};
			if_fail_fe(View.fill(value));
			rtrn Texels.size();
		}
	};

	tydf img<u0_t>	imgu0_t;
	tydf img<u1_t>	imgu1_t;
	tydf img<u2_t>	imgu2_t;
	tydf img<u3_t>	imgu3_t;
	tydf img<s0_t>	imgs0_t;
	tydf img<s1_t>	imgs1_t;
	tydf img<s2_t>	imgs2_t;
	tydf img<s3_t>	imgs3_t;
	tydf img<f2_t>	imgf2_t;
	tydf img<f3_t>	imgf3_t;
} // namespace llc

#endif // LLC_IMAGE_H_23627
