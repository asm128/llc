/// Copyright 2010-2017 - ogarnd
#include "llc_eval.h"
#if defined(LLC_ATMEL)
#	include <stdlib.h>
#else
#	include <malloc.h>
#	include <memory.h>
#endif
#include "llc_auto_handler.h"

#ifndef LLC_MEMORY_H__92836409283642038462309846
#define LLC_MEMORY_H__92836409283642038462309846

#ifndef LLC_MALLOC_CUSTOM_ALIGN
#	define LLC_MALLOC_CUSTOM_ALIGN	16
#endif
#ifdef LLC_MALLOC_CUSTOM_ALIGN
#	define LLC_MALLOC_ALIGN			LLC_MALLOC_CUSTOM_ALIGN
#else
#	define LLC_MALLOC_ALIGN			szof(void*)
#endif

//#define	llc_calc_align_address		(alignment, address)	((alignment - ((alignment - 1) & (uintptr_t)address)) & (alignment - 1))	// returns the difference between an origin address and the next aligned addres. The alignment must always be a power of two.
//#define	llc_calc_align_address_4	(address)				llc_calc_align_address( 4, address)	// (0x04 - (0x03 & (uintptr_t)address) & 0x03)
//#define	llc_calc_align_address_8	(address)				llc_calc_align_address( 8, address)	// (0x08 - (0x07 & (uintptr_t)address) & 0x07)
//#define	llc_calc_align_address_16	(address)				llc_calc_align_address(16, address)	// (0x10 - (0x0F & (uintptr_t)address) & 0x0F)
//#define	llc_calc_align_address_32	(address)				llc_calc_align_address(32, address)	// (0x20 - (0x1F & (uintptr_t)address) & 0x1F)
//#define	llc_calc_align_address_64	(address)				llc_calc_align_address(64, address)	// (0x40 - (0x3F & (uintptr_t)address) & 0x3F)

namespace llc
{
	ndsx	uintptr_t	calc_align_address			(uintptr_t alignment, uintptr_t address)	nxpt	{ const uintptr_t amask = alignment - 1; return (alignment - (amask & address)) & amask; }
	nsix	uintptr_t	calc_align_address_4		(uintptr_t address)							nxpt	{ return calc_align_address( 4, address); }
	nsix	uintptr_t	calc_align_address_8		(uintptr_t address)							nxpt	{ return calc_align_address( 8, address); }
	nsix	uintptr_t	calc_align_address_16		(uintptr_t address)							nxpt	{ return calc_align_address(16, address); }
	nsix	uintptr_t	calc_align_address_32		(uintptr_t address)							nxpt	{ return calc_align_address(32, address); }
	nsix	uintptr_t	calc_align_address_64		(uintptr_t address)							nxpt	{ return calc_align_address(64, address); }

#ifdef LLC_WINDOWS
	stin	void*		llc_free		(void * ptr)			nxpt	{ _aligned_free(ptr); rtrn 0; }
	ndsi	void*		llc_malloc		(size_t size)			nxpt	{ return (u0_t*)_aligned_malloc(size + 1, LLC_MALLOC_ALIGN);  }
#elif defined(LLC_NEWLIB)
	stin	void*		llc_free		(void * ptr)			nxpt	{ ::free(ptr); return 0;	}
	ndsi	void*		llc_malloc		(size_t size)			nxpt	{ return (u0_t*)::memalign(LLC_MALLOC_ALIGN, size + 1); }
#elif defined(LLC_ATMEL) || defined(ESP8266)
	stin	void*		llc_free		(void * ptr)			nxpt	{ ::free(ptr); rtrn 0; }
	ndsi	void*		llc_malloc		(size_t size)			nxpt	{ return  (u0_t*)::malloc(size + 1); }
#else
	stin	void*		llc_free		(void * ptr)			nxpt	{ ::free(ptr); rtrn 0; }
	ndsi	void*		llc_malloc		(size_t size)			nxpt	{ return  (u0_t*)::memalign(LLC_MALLOC_ALIGN, size + 1); }
#endif
	tplT stin	T*&		malloc			(T*& newRef)			nxpt	{ return newRef = (T*)llc_malloc(szof(T)); }
	tplT stin	T*&		malloc			(T*& newRef, u2_t count)nxpt	{ return newRef = (T*)llc_malloc(szof(T) * count); }

	tplT
	stin	void		safe_llc_free	(T & p)					nxpt	{
		T						_pepe			= p;
		p					= 0;
		llc_free((void*)_pepe);
	}

	struct auto_free : public ::llc::auto_handler<void*, nullptr> {
		using	TWrapper	::auto_handler;

		inln				~auto_free				()	nxpt	{ close(); }
		inln	void		close					()	nxpt	{ safe_llc_free(Handle); }
	};
#define GREF_PAGE_SIZE_MAX (4096)
	tpl_t	nsix	u2_t	get_page_size			()						nxpt	{ return (u2_t)(szof(_t) <= GREF_PAGE_SIZE_MAX) ? GREF_PAGE_SIZE_MAX/szof(_t) : 1; };
	tpl_t	nsix	u2_t	get_type_size			()						nxpt	{ return (u2_t) szof(_t); }
	tpl_t	nsix	u2_t	get_type_size			(const _t&)				nxpt	{ return (u2_t) szof(_t); }
	tpl_t	nsix	u2_t	get_type_size_padded	(u2_t paddingInBytes)	nxpt	{ return paddingInBytes ? (u2_t) ( (szof(_t) / paddingInBytes) + one_if(szof(_t) % paddingInBytes) ) * paddingInBytes : szof(_t); }
	tpl_t	nsix	u2_t	get_type_align			()						nxpt	{
		return (u2_t)
			(	(0 == (szof(_t) % 32))	? 32
			:	(0 == (szof(_t) % 16))	? 16
			:	(0 == (szof(_t) % 8 ))	? 8
			:	(0 == (szof(_t) % 4 ))	? 4
			:	(0 == (szof(_t) % 2 ))	? 2
			:	1
			);
	}

	tpl_t	nsix	u2_t	get_type_align_multiplier	()		nxpt	{
		return (u2_t)
			(	(0 == (szof(_t) % 32)) ? szof(_t) / 32
			:	(0 == (szof(_t) % 16)) ? szof(_t) / 16
			:	(0 == (szof(_t) %  8)) ? szof(_t) /  8
			:	(0 == (szof(_t) %  4)) ? szof(_t) /  4
			:	(0 == (szof(_t) %  2)) ? szof(_t) /  2
			:	szof(_t)
			);
	}

	tpl_t	nsix	s2_t	podcmp						(const _t * pA, const _t * pB)							nxpt	{
		if(0 == pA)
			return (0 == pB) ? 0 : 0x7FFFFFFF;
		else if(0 == pB)
			return 0x7FFFFFFF;

		return memcmp(pA, pB, szof(_t));
	}

	tplTN2	stin	T*			memcpy_s					(T (&dest)[N], const T (&src)[N])		nxpt	{ ::memcpy(dest, src, szof(T) * N); return dest; }
}	// namespace

#endif // LLC_MEMORY_H__92836409283642038462309846
