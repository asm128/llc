/// Copyright 2010-2024 - ogarnd
//#include "llc_ptr_obj.h"
#include "llc_ptr_pod.h"
#include "llc_array_obj.h"

#ifndef LLC_ARRAY_PPOD_H_23627
#define LLC_ARRAY_PPOD_H_23627

namespace llc
{
	tplt <tpnm T> using appod		= ::llc::aobj<::llc::ppod<T>>;
} // namespace

#endif // LLC_ARRAY_PPOD_H_23627
