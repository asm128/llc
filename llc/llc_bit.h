/// Copyright 2010-2024 - ogarnd
#include "llc_typeint.h"

#ifndef LLC_BIT_H
#define LLC_BIT_H

namespace llc
{
	tplTndsx	T		bit_clear		(cnst T state, cnst T bitsToClear)								nxpt	{ rtrn (state & bitsToClear)				? state & ~bitsToClear	: state; }
	tplTndsx	T		bit_set			(cnst T state, cnst T bitsToSet  )								nxpt	{ rtrn ((state & bitsToSet) != bitsToSet)	? state | bitsToSet	: state; }
	tplTndsx	T		bit_set			(cnst T state, cnst T bitsToSet, cnst bool value)				nxpt	{ rtrn value ? bit_set(state, bitsToSet) : bit_clear(state, bitsToSet); }
	tplTstxp	T		bit_test		(cnst T state, cnst T bitsToTest)								nxpt	{ rtrn ((state & bitsToTest) == bitsToTest) ? state : (T)0; }
	tplTndsx	T		bit_set_masked	(cnst T state, cnst T mask, cnst T bitsToSet, cnst bool value)	nxpt	{ rtrn (mask & bitsToSet) ? bit_set(state, mask & bitsToSet, value) : state; }
	tplTsinx	T		bit_test_masked	(cnst T state, cnst T mask, cnst T value)						nxpt	{ rtrn (mask & value) ? bit_test(state, mask & value) : (T)0; }
	tplTsinx	T		bit_true		(cnst T state, cnst T bitsToTest)								nxpt	{ rtrn bit_test(state, bitsToTest); }
	tplTstxp	T		bit_false		(cnst T state, cnst T bitsToTest)								nxpt	{
		cnst T		bitsThatMatch		= (state & bitsToTest);
		rtrn T((bitsThatMatch == bitsToTest) ? 0 : (state | bitsToTest) ? state | bitsToTest : -1);
	}
	tplTnsix	u2_t	bitsof			(cnst T&)		nxpt	{ rtrn szof(T); }
	tplTnsix	u2_t	bitsof			()				nxpt	{ rtrn szof(T); }
#ifdef LLC_DEBUG_ENABLED
	tplT 		T		bit_make		(u0_t bitIndex)	nxpt	{
		rve_if((T)-1LL, bitIndex >= (szof(T) * BYTE_SIZE), "Invalid bit index: %i", bitIndex);
#else // !LLC_DEBUG_ENABLED
	tplT ndxp	T		bit_make		(u0_t bitIndex)	nxpt	{
#endif // LLC_DEBUG_ENABLED
		rtrn (T)(((T)1) << bitIndex);
	}
//#ifdef LLC_BIGENDIAN
//	sinx	u0_t		le_byte_at		(uint32_t bytes, u0_t byteIndex)	nxpt	{ return ((0xFF000000 >> (byteIndex * 8)) & bytes) >> ((szof(bytes) - byteIndex) * 8); }
//#else
//	sinx	u0_t		le_byte_at		(uint32_t bytes, u0_t byteIndex)	nxpt	{ return ((0xFF << (byteIndex * 8)) & bytes) >> (byteIndex * 8); }
//#endif
	tplT	sttc	T	reverse_bitfield	(T input, s2_c bitDepth)	{
		u2_c					sizeType			= u2_t(szof(T) * 8);
		cnst T					mask				= ((T)(-1)) >> (sizeType - bitDepth);
		T						result				= 0;
		for(uint32_t iBit = 0; iBit < sizeType; iBit += bitDepth) {
			result				|= (input & mask) << (sizeType - bitDepth - iBit);
			input				>>= bitDepth;
		}
		rtrn result;
	}
}

#endif // LLC_BIT_H
