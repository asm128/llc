#include "llc_log.h"
#include "llc_eval.h"

#ifndef LLC_ARRAY_VIEW_BIT_H_23627
#define LLC_ARRAY_VIEW_BIT_H_23627

namespace llc
{
#pragma pack(push, 1)
	tplt <tpnm _tInt>
	struct bit_proxy {
		tydf	_tInt			T;

		T						& Element;
		uint8_t					Offset;

		oper				bool			()				cnst	{ return Element & (1ULL << Offset); }
		bit_proxy&				oper=		(bool value)			{ value ? Element |= (1ULL << Offset) : Element &= ~(1ULL << Offset); return *this; }
	};

	tplt <tpnm _tInt>
	struct bit_iterator {
		tydf	_tInt			T;

		stxp	u2_t		ELEMENT_BITS	= szof(T) * 8;

		cnst T					& Begin;
		cnst T					& End;
		T						* Element;
		uint8_t					Offset	: 4;
		uint8_t					Stop	: 4;

		bit_proxy<T>			oper*		()				{
			gthrow_if(Element == &End, "Invalid index: %" LLC_FMT_U2 ".", u2_t(&Element - &Begin) + Offset);
			return {Element, (uint8_t)Offset};
		}
		
		bool					oper*		()		cnst	{
			gthrow_if(Element == &End, "Invalid index: %" LLC_FMT_U2 ".", u2_t(&Element - &Begin) + Offset);
			return (*Element) & (1ULL << Offset);
		}

		inline	oper		bool			()									cnst				{ gthrow_if(Element == &End, "Invalid index: %" LLC_FMT_U2 ".", u2_t(&Element - &Begin) + Offset); return (*Element) & (1ULL << Offset); }
		inxp	bool			oper==		(cnst bit_iterator & other)		cnst	nxpt	{ return (((1ULL << Offset)) & *Element) == (((1ULL << other.Offset)) & *other.Element); }
		inxp	bool			oper!=		(cnst bit_iterator & other)		cnst	nxpt	{ return (((1ULL << Offset)) & *Element) != (((1ULL << other.Offset)) & *other.Element); }

		inline	bit_iterator&	oper=		(bool value)	{ value ? *Element |= (1ULL << Offset)	: *Element &= ~(1ULL << Offset); return *this; }
		bit_iterator&			oper++		()				{ ++Offset; if(Offset >= ELEMENT_BITS)	{ ++Element; Offset = 0;				gthrow_if(Element >= (End   + 1), "Out of range: %" LLC_FMT_U2 ". End: %" LLC_FMT_U2 ".", Element, End  ); } return *this; }
		bit_iterator&			oper--		()				{ --Offset; if(Offset < 0)				{ --Element; Offset = ELEMENT_BITS - 1; gthrow_if(Element <  (Begin - 1), "Out of range: %" LLC_FMT_U2 ". End: %" LLC_FMT_U2 ".", Element, Begin); } return *this; }
		bit_iterator			oper++		(int)			{
			bit_iterator				result			(*this);	// Make a copy.
			++(*this);					// Use the prefix version to do the work.
			return result;				// Return the old value.
		}
		bit_iterator			oper--		(int)			{
			bit_iterator				result			(*this);	// Make a copy.
			--(*this);					// Use the prefix version to do the work.
			return result;				// Return the old value.
		}
	};

	tplt<tpnm _tInt>
	class view_bit {
	protected:
		// Properties / Member Variables
		_tInt					* Data			= 0;
		u2_t					Count			= 0;
	public:
		tydf	_tInt					T;
		tydf	bit_iterator<T>			TIter;
		tydf	bit_iterator<cnst T>	TIterConst;
		tydf	TIter					iterator;

		stxp	uint8_t			ELEMENT_BITS	= szof(T) * 8;
		//stxp	uint8_t			SHIFT_VALUE		= ;

		// Constructors
		inxp					view_bit		()								nxpt	= default;
		inline					view_bit		(T * data, u2_t bitCount)				: Data(data), Count(bitCount) { gthrow_if(bitCount && 0 == data, "Invalid parameters. Element count: %" LLC_FMT_U2 ".", bitCount); }
		tplN0u	inxp			view_bit		(T (&data)[N])					nxpt	: Data(data), Count(N * ELEMENT_BITS)								{}
		tplN0u	inline			view_bit		(T (&data)[N], u2_t bitCount)			: Data(data), Count(::llc::min(u2_t(N * ELEMENT_BITS), bitCount))	{ gthrow_if(bitCount > (N * ELEMENT_BITS), "Out of range count. Max count: %" LLC_FMT_U2 ". Requested: %" LLC_FMT_U2 ".", N * ELEMENT_BITS, bitCount); }
		// Operators
		bit_proxy<T>			oper[]		(u2_t index)			{
			gthrow_if(index >= Count, LLC_FMT_GE_U2, index, Count);
			u2_c				offsetRow		= index / ELEMENT_BITS;
			u2_c				offsetBit		= index % ELEMENT_BITS;
			return {Data[offsetRow], (uint8_t)offsetBit};
		}
		bool					oper[]		(u2_t index)	cnst	{
			gthrow_if(index >= Count, LLC_FMT_GE_U2, index, Count);
			u2_c				offsetRow		= index / ELEMENT_BITS;
			u2_c				offsetBit		= index % ELEMENT_BITS;
			return Data[offsetRow] & (1ULL << offsetBit);
		}
		// Methods
		inline	TIter			begin			()			nxpt	{ return {*Data, *(Data + round_up(Count, ELEMENT_BITS)), Data, 0, Count % ELEMENT_BITS}; }
		inline	TIter			end				()			nxpt	{ return {*Data, *(Data + round_up(Count, ELEMENT_BITS)), Data + round_up(Count, ELEMENT_BITS), 0, Count % ELEMENT_BITS}; }
		//																		  *		 *(									   )  *
		inxp	TIterConst		begin			()	cnst	nxpt	{ return {*Data, *(Data + round_up(Count, ELEMENT_BITS)), Data, 0, Count % ELEMENT_BITS}; }
		inxp	TIterConst		end				()	cnst	nxpt	{ return {*Data, *(Data + round_up(Count, ELEMENT_BITS)), Data + round_up(Count, ELEMENT_BITS), 0, Count % ELEMENT_BITS}; }

		inxp	u2_c&			size			()	cnst	nxpt	{ return Count; }
	};
#pragma pack(pop)

	tplT	using					vbit		= ::llc::view_bit<T>;
	tydf	::llc::view_bit<u0_t>	vbitu0_t, vbitu8	;
	tydf	::llc::view_bit<u1_t>	vbitu1_t, vbitu16	;
	tydf	::llc::view_bit<u2_t>	vbitu2_t, vbitu32	;
	tydf	::llc::view_bit<u3_t>	vbitu3_t, vbitu64	;
	tplT	err_t		reverse_bits		(::llc::view_bit<T> toReverse)													{
		u2_c				countBits			= toReverse.size() / 2;
		u2_c				lastBitIndex		= toReverse.size() - 1;
		for(u2_t iBit = 0; iBit < countBits; ++iBit) {
			u2_c				iRev				= lastBitIndex - iBit;
			cnst bool					current				= toReverse[iBit];
			toReverse[iBit]			= (bool)toReverse[iRev];
			toReverse[iRev]			= current;
		}
		return 0;
	}

} // namespace

#endif // LLC_ARRAY_VIEW_BIT_H_23627
