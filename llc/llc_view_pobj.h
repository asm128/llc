#include "llc_ptr_obj.h"
#include "llc_view.h"

#ifndef LLC_VIEW_POBJ_H_23627
#define LLC_VIEW_POBJ_H_23627

namespace llc
{
	tplT using	vpobj	= ::llc::view<::llc::pobj<T>>;
	tplT using	vcpobj	= ::llc::view<cnst ::llc::pobj<T>>;
} // namespace

#endif // LLC_VIEW_POBJ_H_23627
