/// Copyright 2017 - asm128
#include "llc_label.h"

#ifndef LLC_TYPE_IDENTIFIER_H_23627
#define LLC_TYPE_IDENTIFIER_H_23627

namespace llc
{
	struct STypeIdentifier {
		vcst_t					NameSpace				= {};
		vcst_t					Name					= {};

		inxp					STypeIdentifier			()			= default;
		inxp					STypeIdentifier			(vcst_c & nameSpace, vcst_c & name)	: NameSpace(nameSpace), Name(name)	{}

		inline	bool			operator==				(const STypeIdentifier & right)		const	noexcept	{ return NameSpace == right.NameSpace && Name == right.Name; }
	};	// struct
} // namespace


#endif // LLC_TYPE_IDENTIFIER_H_23627
