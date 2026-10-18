#include "llc_ptr_pod.h"
#include "llc_view.h"

#ifndef LLC_VIEW_PPOD_H_23627
#define LLC_VIEW_PPOD_H_23627

namespace llc
{
	tplT using	vppod	= ::llc::view<::llc::ppod<T>>;
	tplT using	vcppod	= ::llc::view<cnst ::llc::ppod<T>>;
} // namespace

#endif // LLC_VIEW_PPOD_H_23627
