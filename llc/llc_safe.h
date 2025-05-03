#include "llc_typeint.h"

#ifndef LLC_SAFE_H_23627
#define LLC_SAFE_H_23627

namespace llc 
{
	tplT	::llc::err_t	llc_safe_assign			(T* & handle, const T & value)	{ if(0			== handle) return -1; *handle = value; return 0; }
	tplT	::llc::err_t	llc_safe_free			(T* & handle)					{ if(0			== handle) return -1; free			(handle); handle = {}; return 0; }
	tplT	::llc::err_t	llc_safe_fclose			(T  & handle)					{ if(0			== handle) return -1; fclose		(handle); handle = {}; return 0; }
#ifdef LLC_WINDOWS
	tplT	::llc::err_t	llc_safe_closehandle	(T  & handle)					{ if(T(-1LL)	== handle) return -1; CloseHandle	(handle); handle = T(-1LL); return 0; }
#endif
} // namespace 

#endif // LLC_SAFE_H_23627
