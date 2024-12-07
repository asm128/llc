/// Copyright 2010-2024 - ogarnd
#include "llc_ptr_obj.h"
#include "llc_array_obj.h"

#ifndef LLC_APOBJ_H_23627
#define LLC_APOBJ_H_23627

namespace llc
{
	tplT using apobj		= ::llc::aobj<::llc::pobj<T>>; 
	tplT using paobj		= ::llc::pobj<::llc::aobj<T>>; 

	tplT using vpobj		= ::llc::view<::llc::pobj<T>>; 
	tplT using vcpobj		= ::llc::view<const ::llc::pobj<T>>; 

	tplT using apo			= ::llc::apobj<T>; 

	tplT using apaobj		= ::llc::aobj<::llc::paobj<T>>; 
	tplT using aapobj		= ::llc::aobj<::llc::apobj<T>>; 
} // namespace

#endif // LLC_APOBJ_H_23627
