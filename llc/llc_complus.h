#include "llc_array_obj.h"

#ifndef LLC_COMPLUS_H_23627
#define LLC_COMPLUS_H_23627

#ifdef LLC_WINDOWS

#include <Unknwn.h>

namespace llc
{
	tplt<tpnm TUnknown>
	u2_t	safe_release	(TUnknown** storage) nxpt {
		if(0 == storage)
			rtrn 0;
		TUnknown* acquired = *storage;
		*storage = {};
		rtrn acquired ? (u2_t)acquired->Release() : 0;
	}

	tplt<tpnm TUnknown>
	u2_t	safe_addref	(TUnknown** storage, TUnknown* toAcquire) nxpt {
		if(0 == storage)
			rtrn 0;
		TUnknown* released = *storage;
		u2_c references = toAcquire ? (u2_t)toAcquire->AddRef() : 0;
		*storage = toAcquire;
		if(released)
			released->Release();
		rtrn references;
	}

	tplt<tpnm TUnknown>
	class ptr_com {
	protected:
		TUnknown* Unknown = {};
	public:
		tydf ptr_com<TUnknown>	TNCOPtr;
		tydf TUnknown			TRef;

		inln			~ptr_com	() nxpt { clear(); }

		inxp			ptr_com		() nxpt = default;
		inln			ptr_com		(cnst TNCOPtr& other) nxpt : Unknown(other.Unknown) { if(Unknown) Unknown->AddRef(); }
		inxp			ptr_com		(TNCOPtr&& other) nxpt : Unknown(other.Unknown) { other.Unknown = {}; }
		// A raw-pointer construction adopts one existing COM reference. Assignment and set_ref() acquire an additional reference.
		inxp			ptr_com		(TRef* other) nxpt : Unknown(other) {}

		inxp oper		TRef*		() nxpt { rtrn Unknown; }
		inxp oper		cnst TRef*	() csnx { rtrn Unknown; }

		inxp bool		oper==		(cnst TNCOPtr& other) csnx { rtrn Unknown == other.Unknown; }
		inxp bool		oper!=		(cnst TNCOPtr& other) csnx { rtrn false == operator==(other); }

		inln TNCOPtr&	oper=		(cnst TNCOPtr& other) nxpt { safe_addref(&Unknown, other.Unknown); rtrn *this; }
		inln TNCOPtr&	oper=		(TNCOPtr&& other) nxpt {
			if(this != &other) {
				clear();
				Unknown = other.Unknown;
				other.Unknown = {};
			}
			rtrn *this;
		}
		inln TNCOPtr&	oper=		(TRef* other) nxpt { safe_addref(&Unknown, other); rtrn *this; }

		inln TRef*		oper->		() nxpt { rtrn Unknown; }
		inln cnst TRef*	oper->		() csnx { rtrn Unknown; }

		inln u2_t		clear		() nxpt { rtrn safe_release(&Unknown); }
		inxp cnst TRef*	get_ref		() csnx { rtrn Unknown; }
		inln cnst TRef*	set_ref		(TRef* reference) nxpt { safe_addref(&Unknown, reference); rtrn Unknown; }
		inxp TRef*		get			() csnx { rtrn Unknown; }

		inln TRef*		attach		(TRef* reference) nxpt {
			TRef* released = Unknown;
			Unknown = reference;
			if(released)
				released->Release();
			rtrn Unknown;
		}
		inln TRef*		detach		() nxpt { TRef* detached = Unknown; Unknown = {}; rtrn detached; }
		// COM output parameters replace, rather than silently leak, the currently owned reference.
		inln TRef**		put			() nxpt { clear(); rtrn &Unknown; }

		tplt<tpnm TOther>
		inln err_t		as			(TOther** other) nxpt {
			if(0 == other || 0 == Unknown)
				rtrn E_POINTER;
			TOther* acquired = {};
			HRESULT hr = Unknown->QueryInterface(__uuidof(TOther), (void**)&acquired);
			if(FAILED(hr)) {
				if(acquired)
					acquired->Release();
				rtrn (err_t)hr;
			}
			TOther* released = *other;
			*other = acquired;
			if(released)
				released->Release();
			rtrn (err_t)hr;
		}

		tplt<tpnm TOther>
		inln err_t		as			(ptr_com<TOther>& other) nxpt {
			if(0 == Unknown)
				rtrn E_POINTER;
			TOther* acquired = {};
			HRESULT hr = Unknown->QueryInterface(__uuidof(TOther), (void**)&acquired);
			if(FAILED(hr)) {
				if(acquired)
					acquired->Release();
				rtrn (err_t)hr;
			}
			other.attach(acquired);
			rtrn (err_t)hr;
		}
	};
	tplt<tpnm T> usng pcom		= ptr_com<T>;
	tplt<tpnm T> usng acom		= aobj<pcom<T>>;
	tplt<tpnm T> usng vcom		= view<pcom<T>>;
	tplt<tpnm T> usng array_com	= acom<T>;
	tplt<tpnm T> usng view_com	= vcom<T>;
}

#endif // LLC_WINDOWS

#endif // LLC_COMPLUS_H_23627
