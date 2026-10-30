#include "llc_log.h"
#include "llc_functional.h"
#include "llc_array_static.h"

#ifndef LLC_ARRAY_CIRCULAR_H
#define LLC_ARRAY_CIRCULAR_H

namespace llc
{

	tplt<tpnm _tPOD, ::llc::u2_t mAx>
	struct array_circular : private astatic<_tPOD, mAx> {
	protected:
		tydf	_tPOD			_t;
		::llc::u2_t				Offset	= 0;
		::llc::u2_t				Count	= 0;

		using astatic<_t, mAx>	::Storage;
		using astatic<_t, mAx>	::size;
		using astatic<_t, mAx>	::slice;
	public:
		tydf	_tPOD			T;
		tydf	astatic<T, mAx>	TAStatic;

		using TAStatic			::array_static;

		::llc::err_t			next			(T & output)	{
			if_true_vw(-1, 0 == Count);
			output = Storage[Offset % mAx];
			if(size() == ++Offset)
				Offset = 0;
			return Count -= 1;
		}

		::llc::err_t			push_back		(const T & output)	{
			if_true_fe(0 == free());
			Storage[(Offset + Count) % mAx] = output;
			return Count += 1;
		}

		template<::llc::u2_t _nBatch>
		::llc::err_t			append			(const T (&output)[_nBatch])	{
			if_true_fe(Count + _nBatch > free());
			Storage[(Offset + Count) % mAx] = output;
			return Count += 1;
		}

		::llc::err_t			left			()	const	noexcept	{ return min(Count, size() - Offset); } // left side, first part
		::llc::err_t			right			()	const	noexcept	{ return max((::llc::err_t)0, (::llc::err_t)Count - left()); } // right side, second part

		inline	::llc::err_t	left			(::llc::view<const T> & _left)	const	noexcept	{ return slice(_left , Offset, left()); }
		inline	::llc::err_t	right			(::llc::view<const T> & _right)	const	noexcept	{ return slice(_right, 0, right()); }

		::llc::err_t			pair			(::llc::view<const T> & _left, ::llc::view<const T> & _right)	const	noexcept	{
			if_fail_fe(left(_left));
			if_fail_fe(right(_right));
			return _left.size() + _right.size(); 
		} 

		inxp	::llc::err_t	free			()	const	noexcept	{ return size() - Count; }
		inxp	::llc::err_t	used			()	const	noexcept	{ return Count; }
		inxp	::llc::err_t	byte_count		()	const	noexcept	{ return Count * szof(T); }

//		::llc::err_t			read			(const ::llc::function<int()> & funcRemaining, const ::llc::function<int(T *, ::llc::u2_t)> & funcRead) {
//			return 0;	
//		}	
		::llc::err_t			read			(const ::llc::function<int()> & funcRemaining, const ::llc::function<T()> & funcNext) {
			u2_c			stop			= mAx - Count;
			s2_t					readCount		= 0; 
			while(readCount < stop)
			switch(const err_t funcRemainingResult = funcRemaining()) {
			case 0:
				return readCount;
			default:
				if_fail_vef(-readCount, funcRemainingResult, "%" LLC_FMT_S2 ":%" LLC_FMT_S2, readCount, funcRemainingResult);
				if_fail_vef(-readCount, push_back(funcNext()), "%" LLC_FMT_S2, readCount);
				++readCount;
			}
			return readCount;
		}

		::llc::err_t			read		(::llc::u2_t countToRead, const ::llc::function<int()> & funcRemaining, const ::llc::function<T()> & funcNext) {
			u2_c			stop			= ::llc::min(countToRead, mAx - Count);
			for(s2_t readCount = 0; readCount < stop; ++readCount) {
				if_true_vef(-readCount, 0 >= funcRemaining(), "%" LLC_FMT_S2 " < %" LLC_FMT_S2, readCount, countToRead);
				if_fail_vef(-readCount, push_back(funcNext()), "%" LLC_FMT_S2, readCount);
			}
			return stop;
		}
	};

	tplt<tpnm T, ::llc::u2_t _maxCount> using acir			= ::llc::array_circular<T, _maxCount>;
	tplt<tpnm T, ::llc::u2_t _maxCount> using acircular	= ::llc::acir<T, _maxCount>;
	
	tplt<::llc::u2_t _maxCount>	using acirc		= ::llc::acir<char     , _maxCount>; tplt<::llc::u2_t _maxCount>	using acirchar	= ::llc::acir<char    , _maxCount>;
	tplt<::llc::u2_t _maxCount>	using aciruc 	= ::llc::acir<uc_t  , _maxCount>; tplt<::llc::u2_t _maxCount>	using aciruchar	= ::llc::acir<uc_t , _maxCount>;
	tplt<::llc::u2_t _maxCount>	using aciru8 	= ::llc::acir<u0_t  , _maxCount>; 
	tplt<::llc::u2_t _maxCount>	using aciru16	= ::llc::acir<::llc::u1_t , _maxCount>; 
	tplt<::llc::u2_t _maxCount>	using aciru32	= ::llc::acir<::llc::u2_t , _maxCount>; 
	tplt<::llc::u2_t _maxCount>	using aciru64	= ::llc::acir<u3_t , _maxCount>; 
	tplt<::llc::u2_t _maxCount>	using aciri8 	= ::llc::acir<s0_t   , _maxCount>; 
	tplt<::llc::u2_t _maxCount>	using aciri16	= ::llc::acir<int16_t  , _maxCount>; 
	tplt<::llc::u2_t _maxCount>	using aciri32	= ::llc::acir<s2_t  , _maxCount>; 
	tplt<::llc::u2_t _maxCount>	using aciri64	= ::llc::acir<s3_t  , _maxCount>; 
	tplt<::llc::u2_t _maxCount>	using acirf32	= ::llc::acir<f2s_t, _maxCount>; 
	tplt<::llc::u2_t _maxCount>	using acirf64	= ::llc::acir<f3s_t, _maxCount>; 
} // namespace 

#endif // LLC_ARRAY_CIRCULAR_H
