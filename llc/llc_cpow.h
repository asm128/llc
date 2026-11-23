#include "llc_typeint.h"

#ifndef LLC_CPOW_H
#define LLC_CPOW_H

namespace llc
{
	tplt<size_t exp, tpnm T>
	stxp T cpow_impl(T base) {
		if constexpr(0 == exp)
			rtrn T(1);
		else
			rtrn T(base * cpow_impl<exp - 1>(base));
	}

	tplt<size_t exp, tpnm T>
	nsix T cpow(T base) { rtrn cpow_impl<exp>(base); }

	stxp	u0_t		ASCII_ALPHABET_LENGTH	= 'Z' - 'A' + 1;
	stxp	u0_t		ASCII_LETTER_COUNT		= ASCII_ALPHABET_LENGTH * 2;
	stxp	u0_t		ASCII_DIGIT_COUNT		= ASCII_LETTER_COUNT + 10;

	nsix	char		digit_ascii				(char remainder)								nxpt	{ return remainder + ((remainder < 10) ? '0' : (remainder < 10 + ASCII_ALPHABET_LENGTH) ? 'A' - 10 : 'a' - 10 - ASCII_ALPHABET_LENGTH); }
	nsix	char		digit_ascii				(uint64_t value, u0_t base)					nxpt	{ return digit_ascii(char(value % base)); }
	tplt<u0_t exp, tpnm TValue>
	nsix	char		digit					(const TValue value, const TValue base = 10)	nxpt	{ return ::llc::digit_ascii(value / ::llc::cpow<exp>(base), (u0_t)base); }
} // namespace

#endif // LLC_CPOW_H
