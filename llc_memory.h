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
	ndsx	uintptr_t					calc_align_address			(uintptr_t alignment, uintptr_t address)	noexcept	{ const uintptr_t amask = alignment - 1; return (alignment - (amask & address)) & amask; }
	nsix	uintptr_t					calc_align_address_4		(uintptr_t address)							noexcept	{ return calc_align_address( 4, address); }
	nsix	uintptr_t					calc_align_address_8		(uintptr_t address)							noexcept	{ return calc_align_address( 8, address); }
	nsix	uintptr_t					calc_align_address_16		(uintptr_t address)							noexcept	{ return calc_align_address(16, address); }
	nsix	uintptr_t					calc_align_address_32		(uintptr_t address)							noexcept	{ return calc_align_address(32, address); }
	nsix	uintptr_t					calc_align_address_64		(uintptr_t address)							noexcept	{ return calc_align_address(64, address); }

#if defined(LLC_WINDOWS)
	stin	void*						llc_free					(void* ptr)									noexcept	{ _aligned_free(ptr); rtrn 0; }
	stin	void*						llc_malloc					(size_t size)								noexcept	{ int8_t* p = (int8_t*)_aligned_malloc(size + 1, LLC_MALLOC_ALIGN); return p; }
#elif defined(LLC_NEWLIB)
	stin	void*						llc_free					(void* ptr)									noexcept	{
		::free(ptr);
	}
	stin	void*						llc_malloc					(size_t size)								noexcept	{
		int8_t* p = (int8_t*)::memalign(LLC_MALLOC_ALIGN, size + 1);
		return p;
	}
#elif defined(LLC_ATMEL) || defined(ESP8266)
	stin	void*						llc_free					(void* ptr)									noexcept	{ ::free(ptr); rtrn 0; }
	stin	void*						llc_malloc					(size_t size)								noexcept	{ int8_t* p = (int8_t*)::malloc(size + 1); return p; }
#else
	stin	void*						llc_free					(void* ptr)									noexcept	{ ::free(ptr); rtrn 0; }
	stin	void*						llc_malloc					(size_t size)								noexcept	{ int8_t* p = (int8_t*)::memalign(LLC_MALLOC_ALIGN, size + 1); return p; }
#endif

	tplT stin	T*&					malloc						(T*& newRef)								noexcept	{ return newRef = (T*)llc_malloc(szof(T)); }
	tplT stin	T*&					malloc						(T*& newRef, u2_t count)					noexcept	{ return newRef = (T*)llc_malloc(szof(T) * count); }

	tplt<tpnm _typePtr>
	stin	void						safe_llc_free				(_typePtr &p)								noexcept	{
		_typePtr								_pepe						= p;
		p									= 0;
		llc_free((void*)_pepe);
	}

	struct auto_llc_free : public ::llc::auto_handler<void*, nullptr>	{ using TWrapper::auto_handler; inline ~auto_llc_free() noexcept { close(); } inline void close() noexcept { safe_llc_free(Handle); } };

#define GREF_PAGE_SIZE_MAX (4096)
	tplt<tpnm _tBase>	sinx	uint32_t	get_page_size				()							noexcept	{ return (uint32_t)(szof(_tBase) <= GREF_PAGE_SIZE_MAX) ? GREF_PAGE_SIZE_MAX/szof(_tBase) : 1; };
	tplt<tpnm _tBase>	sinx	uint32_t	get_type_size				()							noexcept	{ return (uint32_t) szof(_tBase); }
	tplt<tpnm _tBase>	sinx	uint32_t	get_type_size				(const _tBase&)				noexcept	{ return (uint32_t) szof(_tBase); }
	tplt<tpnm _tBase>	sinx	uint32_t	get_type_size_padded		(uint32_t paddingInBytes)	noexcept	{ return paddingInBytes ? (uint32_t) ( (szof(_tBase) / paddingInBytes) + one_if(szof(_tBase) % paddingInBytes) ) * paddingInBytes : szof(_tBase); }
	tplt<tpnm _tBase>	sinx	uint32_t	get_type_align				()							noexcept	{
		return (uint32_t)
			(	(0 == (szof(_tBase) % 32))	? 32
			:	(0 == (szof(_tBase) % 16))	? 16
			:	(0 == (szof(_tBase) % 8 ))	? 8
			:	(0 == (szof(_tBase) % 4 ))	? 4
			:	(0 == (szof(_tBase) % 2 ))	? 2
			:	1
			);
	}

	tplt <tpnm _tBase>	sinx	uint32_t	get_type_align_multiplier	()							noexcept	{
		return (uint32_t)
			(	(0 == (szof(_tBase) % 32)) ? szof(_tBase) / 32
			:	(0 == (szof(_tBase) % 16)) ? szof(_tBase) / 16
			:	(0 == (szof(_tBase) %  8)) ? szof(_tBase) /  8
			:	(0 == (szof(_tBase) %  4)) ? szof(_tBase) /  4
			:	(0 == (szof(_tBase) %  2)) ? szof(_tBase) /  2
			:	szof(_tBase)
			);
	}

	tplt <tpnm _tBase>	stin	int32_t		podcmp		(const _tBase* pA, const _tBase* pB)							noexcept	{
		if(0 == pA)
			return (0 == pB) ? 0 : 0x7FFFFFFF;
		else if(0 == pB)
			return 0x7FFFFFFF;

		return memcmp(pA, pB, szof(_tBase));
	}

	tplt <tpnm _t, size_t _len>	_t*			memcpy_s					(_t (&dest)[_len], const _t (&src)[_len])				noexcept	{ ::memcpy(dest, src, szof(_t) * _len); return dest; }
}	// namespace

#endif // LLC_MEMORY_H__92836409283642038462309846
