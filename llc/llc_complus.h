#include "llc_array_obj.h"

#ifdef LLC_WINDOWS

#include "llc_windows.h"

#include <Unknwn.h>

#ifndef LLC_COMPLUS_H_23627
#define LLC_COMPLUS_H_23627

namespace llc
{
	tplt<tpnm TUnknown> 
	::llc::error_t		safe_release	(TUnknown ** storage) { 
		IUnknown*				p				= *storage;
		if(p) { 
			(*storage)			= 0; 
			return (::llc::error_t)p->Release(); 
		} 
		return 0;
	}

	tplt<tpnm TUnknown> 
	::llc::error_t		safe_addref		(TUnknown ** storage, TUnknown * toAcquire) { 
		::llc::error_t			result			= 0;
		IUnknown*				p				= *storage;
		*storage			= toAcquire;
		if(toAcquire)
			result = (::llc::error_t)toAcquire->AddRef();

		if(p)
			p->Release(); 

		return result; 
	}

	tplt<tpnm _tNCO>
	class ptr_com {
	protected:
		_tNCO				* Unknown			= 0;
	public:
		typedef	::llc::ptr_com<_tNCO>	TNCOPtr;
		typedef	_tNCO					TRef;

		inline					~ptr_com		()									noexcept	{ clear(); }

		inlcxpr					ptr_com			()									noexcept	= default;
		inline					ptr_com			(const TNCOPtr& other)				noexcept	{ Unknown = other.Unknown; if(Unknown) Unknown->AddRef(); }
		inlcxpr					ptr_com			(TNCOPtr&& other)					noexcept	{ Unknown = other.Unknown; other.Unknown = 0; }
		inline					ptr_com			(TRef* other)						noexcept	{ Unknown = other; }

		inlcxpr	operator		TRef		*	()									noexcept	{ return Unknown;	}
		inlcxpr	operator		const TRef	*	()							const	noexcept	{ return Unknown;	}

		inlcxpr	bool			operator==		(const TNCOPtr& other)		const	noexcept	{ return Unknown == other.Unknown; }
		inlcxpr	bool			operator!=		(const TNCOPtr& other)		const	noexcept	{ return !operator==(other); }

		inlcxpr	TNCOPtr			operator =		(const TNCOPtr& other)				noexcept	{ TRef* oldInstance = Unknown; Unknown = other.Unknown; if(Unknown) Unknown->AddRef();	if(oldInstance) oldInstance->Release(); return *this; }
		inlcxpr	TNCOPtr			operator =		(TNCOPtr&& other)					noexcept	{ TRef* oldInstance = Unknown; Unknown = other.Unknown; other.Unknown = 0;					if(oldInstance) oldInstance->Release(); return *this; }
		inlcxpr	TNCOPtr			operator =		(TRef* other)						noexcept	{ TRef* oldInstance = Unknown; Unknown = other; if(Unknown) Unknown->AddRef();				if(oldInstance) oldInstance->Release(); return *this; }

		inline	TRef*			operator->		()									noexcept	{ return Unknown; }
		inline	const TRef*		operator->		()							const	noexcept	{ return Unknown; }

		inline	TRef**			operator &		()									noexcept	{ return &Unknown; }

		inline	::llc::u2_t		clear			()									noexcept	{ return ::llc::safe_release(&Unknown); }
		inlcxpr	const TRef*		get_ref			()							const	noexcept	{ return Unknown; }
		inlcxpr	const TRef*		set_ref			(TRef* ref)							noexcept	{ ::llc::safe_addref(&Unknown, ref); return Unknown; }
		inlcxpr	TRef*			get				()							const	noexcept	{ return Unknown; }

		tplt<tpnm _tNCOOther>
		inline	::llc::error_t	as				(_tNCOOther** other)				noexcept	{ 
			_tNCOOther*					old				= *other;
			*other					= {};
			HRESULT						hr				= Unknown ? Unknown->QueryInterface(__uuidof(_tNCOOther), (void**)other) : 0;
			if(old)
				old->Release();
			return hr;
		}

		tplt<tpnm _tNCOOther>
		inline	::llc::error_t	as				(::llc::ptr_com<_tNCOOther>& other)	noexcept	{ 
			::llc::ptr_com<_tNCOOther>	old				= other;
			other					= {};
			HRESULT						hr				= Unknown ? Unknown->QueryInterface(__uuidof(_tNCOOther), (void**)&other) : 0;
			return hr;
		}
	};
	tplt <tpnm T> using pcom		= ::llc::ptr_com<T>;
	tplt <tpnm T> using acom		= ::llc::aobj<::llc::pcom<T>>;
	tplt <tpnm T> using vcom		= ::llc::view<::llc::pcom<T>>;
	tplt <tpnm T> using array_com	= ::llc::acom<T>;
	tplt <tpnm T> using view_com	= ::llc::vcom<T>;
}

#endif // LLC_COMPLUS_H_23627

#endif // LLC_WINDOWS - ignore the whole thing if not a Windows build
