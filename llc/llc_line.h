#include "llc_typeint.h"

#ifndef LLC_LINE_H_23627
#define LLC_LINE_H_23627

namespace llc
{
#pragma pack(push, 1)
	tplt<tpnm T>
	struct line {
		T	A, B;

		cxpr	line	()							= default;
		cxpr	line	(cnst line<T> & other)		= default;
		cxpr	line	(cnst T & a, cnst T & b)	: A(a), B(b) {}

		LLC_DEFAULT_OPERATOR(line<T>, A == other.A && B == other.B);
	};
#pragma pack(pop)
}

#endif // LLC_LINE_H_23627
