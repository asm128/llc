#include "llc_log.h"
#include "llc_functional.h"
#include "llc_array_static.h"

#ifndef LLC_ARRAY_CIRCULAR_H
#define LLC_ARRAY_CIRCULAR_H

namespace llc
{

	tplt<tpnm _tPOD, uint32_t mAx>
	struct array_circular : private astatic<_tPOD, mAx> {
	protected:
		tydf	_tPOD			_t;
		uint32_t				Offset	= 0;
		uint32_t				Count	= 0;

		using astatic<_t, mAx>	::Storage;
		using astatic<_t, mAx>	::size;
		using astatic<_t, mAx>	::slice;
	public:
		tydf	_tPOD			T;
		tydf	astatic<T, mAx>	TAStatic;

		using TAStatic			::array_static;

		::llc::error_t			next			(T & output)	{
			rews_if(0 == Count);
			output = Storage[Offset % mAx];
			if(size() == ++Offset)
				Offset = 0;
			return Count -= 1;
		}

		::llc::error_t			push_back		(const T & output)	{
			rees_if(0 == free());
			Storage[(Offset + Count) % mAx] = output;
			return Count += 1;
		}

		template<uint32_t _nBatch>
		::llc::error_t			append			(const T (&output)[_nBatch])	{
			rees_if(Count + _nBatch > free());
			Storage[(Offset + Count) % mAx] = output;
			return Count += 1;
		}

		::llc::error_t			left			()	const	noexcept	{ return min(Count, size() - Offset); } // left side, first part
		::llc::error_t			right			()	const	noexcept	{ return max((::llc::error_t)0, (::llc::error_t)Count - left()); } // right side, second part

		inline	::llc::error_t	left			(::llc::view<const T> & _left)	const	noexcept	{ return slice(_left , Offset, left()); }
		inline	::llc::error_t	right			(::llc::view<const T> & _right)	const	noexcept	{ return slice(_right, 0, right()); }

		::llc::error_t			pair			(::llc::view<const T> & _left, ::llc::view<const T> & _right)	const	noexcept	{
			llc_necs(left(_left));
			llc_necs(right(_right));
			return _left.size() + _right.size(); 
		} 

		inxp	::llc::error_t	free			()	const	noexcept	{ return size() - Count; }
		inxp	::llc::error_t	used			()	const	noexcept	{ return Count; }
		inxp	::llc::error_t	byte_count		()	const	noexcept	{ return Count * szof(T); }

//		::llc::error_t			read			(const ::llc::function<int()> & funcRemaining, const ::llc::function<int(T *, uint32_t)> & funcRead) {
//			return 0;	
//		}	
		::llc::error_t			read			(const ::llc::function<int()> & funcRemaining, const ::llc::function<T()> & funcNext) {
			u2_c			stop			= mAx - Count;
			int32_t					readCount		= 0; 
			while(readCount < stop)
			switch(const error_t funcRemainingResult = funcRemaining()) {
			case 0:
				return readCount;
			default:
				rve_if_failed(-readCount, funcRemainingResult, "%" LLC_FMT_S2 ":%" LLC_FMT_S2, readCount, funcRemainingResult);
				rve_if_failed(-readCount, push_back(funcNext()), "%" LLC_FMT_S2, readCount);
				++readCount;
			}
			return readCount;
		}

		::llc::error_t			read		(uint32_t countToRead, const ::llc::function<int()> & funcRemaining, const ::llc::function<T()> & funcNext) {
			u2_c			stop			= ::llc::min(countToRead, mAx - Count);
			for(int32_t readCount = 0; readCount < stop; ++readCount) {
				rve_if(-readCount, 0 >= funcRemaining(), "%" LLC_FMT_S2 " < %" LLC_FMT_S2, readCount, countToRead);
				rve_if_failed(-readCount, push_back(funcNext()), "%" LLC_FMT_S2, readCount);
			}
			return stop;
		}
	};

	tplt<tpnm T, uint32_t _maxCount> using acir			= ::llc::array_circular<T, _maxCount>;
	tplt<tpnm T, uint32_t _maxCount> using acircular	= ::llc::acir<T, _maxCount>;
	
	tplt<uint32_t _maxCount>	using acirc		= ::llc::acir<char     , _maxCount>; tplt<uint32_t _maxCount>	using acirchar	= ::llc::acir<char    , _maxCount>;
	tplt<uint32_t _maxCount>	using aciruc 	= ::llc::acir<uc_t  , _maxCount>; tplt<uint32_t _maxCount>	using aciruchar	= ::llc::acir<uc_t , _maxCount>;
	tplt<uint32_t _maxCount>	using aciru8 	= ::llc::acir<uint8_t  , _maxCount>; 
	tplt<uint32_t _maxCount>	using aciru16	= ::llc::acir<uint16_t , _maxCount>; 
	tplt<uint32_t _maxCount>	using aciru32	= ::llc::acir<uint32_t , _maxCount>; 
	tplt<uint32_t _maxCount>	using aciru64	= ::llc::acir<uint64_t , _maxCount>; 
	tplt<uint32_t _maxCount>	using aciri8 	= ::llc::acir<int8_t   , _maxCount>; 
	tplt<uint32_t _maxCount>	using aciri16	= ::llc::acir<int16_t  , _maxCount>; 
	tplt<uint32_t _maxCount>	using aciri32	= ::llc::acir<int32_t  , _maxCount>; 
	tplt<uint32_t _maxCount>	using aciri64	= ::llc::acir<int64_t  , _maxCount>; 
	tplt<uint32_t _maxCount>	using acirf32	= ::llc::acir<f2s_t, _maxCount>; 
	tplt<uint32_t _maxCount>	using acirf64	= ::llc::acir<f3s_t, _maxCount>; 
} // namespace 

#endif // LLC_ARRAY_CIRCULAR_H
