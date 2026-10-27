#include "llc_error.h"

#ifdef LLC_WINDOWS
#	include <WinSock2.h>
#	include <Windows.h>
#else
#	include <unistd.h>
#endif

#ifndef LLC_SAFE_H_23627
#define LLC_SAFE_H_23627

namespace llc 
{
	tplT	err_t	llc_safe_assign			(T* & handle, const T & value)	{ if(0			== handle) return -1; *handle = value; return 0; }
	tplT	err_t	llc_safe_free			(T* & handle)					{ if(0			== handle) return -1; ::free		(handle); handle = {}; return 0; }
	tplT	err_t	llc_safe_delete			(T* & handle)					{ if(0			== handle) return -1; delete		(handle); handle = {}; return 0; }
	tplT	err_t	llc_safe_fclose			(T  & handle)					{ if(0			== handle) return -1; ::fclose		(handle); handle = {}; return 0; }
#ifdef LLC_WINDOWS
	tplT	err_t	llc_safe_closesocket	(T  & handle)					{ if(0 == handle || INVALID_SOCKET == handle) return -1; ::closesocket(handle); handle = INVALID_SOCKET; return 0; }
	tplT	err_t	llc_safe_closehandle	(T  & handle)					{ if(T(-1LL)	== handle) return -1; ::CloseHandle	(handle); handle = T(-1LL); return 0; }
#else
	tplT	err_t	llc_safe_closesocket	(T  & handle)					{ if(0 == handle || T(-1) == handle) return -1; ::close(handle); handle = T(-1); return 0; }
#endif
} // namespace 

#endif // LLC_SAFE_H_23627
