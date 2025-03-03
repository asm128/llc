#include "llc_typeint.h"

#ifndef LLC_CPOW_H
#define LLC_CPOW_H

namespace llc
{
	tplt<size_t exp>	nsix	i0u_t			cpow	(i0u_t base)	{ return base * cpow<exp - 1>(base); }
	tplt<size_t exp>	nsix	i1u_t			cpow	(i1u_t base)	{ return base * cpow<exp - 1>(base); }
	tplt<size_t exp>	nsix	i2u_t			cpow	(i2u_t base)	{ return base * cpow<exp - 1>(base); }
	tplt<size_t exp>	nsix	i3u_t			cpow	(i3u_t base)	{ return base * cpow<exp - 1>(base); }
	tplt<size_t exp>	nsix	i0s_t			cpow	(i0s_t base)	{ return base * cpow<exp - 1>(base); }
	tplt<size_t exp>	nsix	i1s_t			cpow	(i1s_t base)	{ return base * cpow<exp - 1>(base); }
	tplt<size_t exp>	nsix	i2s_t			cpow	(i2s_t base)	{ return base * cpow<exp - 1>(base); }
	tplt<size_t exp>	nsix	i3s_t			cpow	(i3s_t base)	{ return base * cpow<exp - 1>(base); }
	tplt<size_t exp>	nsix	f2s_t			cpow	(f2s_t base)	{ return base * cpow<exp - 1>(base); }
	tplt<size_t exp>	nsix	f3s_t			cpow	(f3s_t base)	{ return base * cpow<exp - 1>(base); }
	tplt<>				ndix	i0u_t			cpow<0>	(i0u_t/*base*/)	{ return 1; }
	tplt<>				ndix	i1u_t			cpow<0>	(i1u_t/*base*/)	{ return 1; }
	tplt<>				ndix	i2u_t			cpow<0>	(i2u_t/*base*/)	{ return 1; }
	tplt<>				ndix	i3u_t			cpow<0>	(i3u_t/*base*/)	{ return 1; }
	tplt<>				ndix	i0s_t			cpow<0>	(i0s_t/*base*/)	{ return 1; }
	tplt<>				ndix	i1s_t			cpow<0>	(i1s_t/*base*/)	{ return 1; }
	tplt<>				ndix	i2s_t			cpow<0>	(i2s_t/*base*/)	{ return 1; }
	tplt<>				ndix	i3s_t			cpow<0>	(i3s_t/*base*/)	{ return 1; }
	tplt<>				ndix	f2s_t			cpow<0>	(f2s_t/*base*/)	{ return 1; }
	tplt<>				ndix	f3s_t			cpow<0>	(f3s_t/*base*/)	{ return 1; }

	stxp	uint8_t		ASCII_ALPHABET_LENGTH	= 'Z' - 'A';
	stxp	uint8_t		ASCII_LETTER_COUNT		= ASCII_ALPHABET_LENGTH * 2;
	stxp	uint8_t		ASCII_DIGIT_COUNT		= ASCII_LETTER_COUNT + 10;

	nsix char		digit_ascii				(char remainder)								nxpt	{ return remainder + ((remainder < 10) ? '0' : '7'); }
	nsix char		digit_ascii				(uint64_t value, uint8_t base)					nxpt	{ return digit_ascii(char(value % base)); }
	tplt<uint8_t exp, tpnm TValue>
	nsix	char		digit					(const TValue value, const TValue base = 10)	nxpt	{ return ::llc::digit_ascii(value / ::llc::cpow<exp>(base), (uint8_t)base); }
} // namespace

#endif // LLC_CPOW_H
