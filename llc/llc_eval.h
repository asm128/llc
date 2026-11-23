#include "llc_typeint.h"

#ifdef LLC_ATMEL
#	include <string.h>
#else
#	include <memory.h>
#endif

#ifndef LLC_EVAL_H_23627
#define LLC_EVAL_H_23627

#if defined(LLC_WINDOWS) || defined(LLC_ARDUINO) || defined(LLC_ATMEL)
#	ifdef min
#		undef min
#	endif
#	ifdef max
#		undef max
#	endif
#	if !defined(NOMINMAX)
#		define NOMINMAX
#	endif
#endif

namespace llc
{
#pragma pack(push, 1)
	tplt<tpnm T>	nsix	T&			min			(T & a, T & b)							noexcept	{ return (a < b) ? a : b; }
	tplt<tpnm T>	nsix	T&			max			(T & a, T & b)							noexcept	{ return (a > b) ? a : b; }
	tplt<tpnm T>	nsix	const T&	min			(const T & a, const T & b)				noexcept	{ return (a < b) ? a : b; }
	tplt<tpnm T>	nsix	const T&	max			(const T & a, const T & b)				noexcept	{ return (a > b) ? a : b; }
	tplt<tpnm T>	nsix	T			clamped		(T _value, const T _min, const T _max)	noexcept	{ return ::llc::min(_max, ::llc::max(_min, _value)); }

	// excluding the stop value
	tplt<tpnm T>	ndsx	bool		in_range	(const T & valueToTest, const T & rangeStart, const T & rangeStop)	noexcept	{ 
		return (valueToTest >= rangeStart) && (valueToTest < rangeStop);
	}
	tplt<tpnm T>	ndsx	bool		equal		(const T * other, const T * local, ::llc::u2_t count) noexcept {
		for(::llc::u2_t iElement = 0; iElement < count; ++iElement)
			if(other[iElement] != local[iElement])
				return false;
		return true;
	}

#pragma pack(pop)
#ifndef truif_true_e 
#	define truif_true_e(expression)				((expression) ? true : false)	// returns true  if the parameter evaluates to true
#	define falsif_true_e(expression)				((expression) ? false : true)	// returns false if the parameter evaluates to true
#	define one_if(expression)				((expression) ? 1 : 0)			// returns one   if the parameter evaluates to true
#	define zero_if(expression)				((expression) ? 0 : 1)			// returns zero  if the parameter evaluates to true
#	define is_one(your_int)					(1 == (your_int))				// returns true if the parameter is equal to one
#	define is_zero(your_int)				(0 == (your_int))				// returns true if the parameter is equal to zero
#	define onif_true_e_false(expression)			one_if(false == (expression))
//
#	define round_up(count, denominator)		(((count) + (denominator) - 1) / (denominator))	// 
#endif

} // namespace

#endif // LLC_EVAL_H_23627
