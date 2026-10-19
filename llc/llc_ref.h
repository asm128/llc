#include "llc_sync.h"
#include "llc_log.h"
#include "llc_memory.h"

#ifndef LLC_REF_H_23627
#define LLC_REF_H_23627

namespace llc
{
	tplTstct gref {
		T						* Instance = 0;
		refcount_vl				References = 0;
	};

	tplT	gref<T>*	ref_acquire				(gref<T> * llc_reference)	nxpt	{ if(llc_reference) llc_sync_increment(llc_reference->References); return llc_reference; }
	tplT	err_t		ref_release				(gref<T>* * llc_reference) {
		tydf	gref<T>			TRef;
		TRef					* oldRef				= *llc_reference;
		*llc_reference		= 0;
		if(oldRef)
			switch(refcount_c referenceCount = llc_sync_decrement(oldRef->References)) {
			default: if_fail_fef(referenceCount, "Reference count error! (%i).", referenceCount); break;
			case  0:
				if_null_e(oldRef->Instance)
				else {
					oldRef->Instance->~T(); 
					llc_free(oldRef->Instance);
				}
				llc_free(oldRef);
				break;
			}
		return 0;
	}
	tplT	T*			ref_allocate			(gref<T>* * llc_reference)	noexcept	{
		tydf	gref<T>			TRef;
		TRef					* newRef;
		if_null_ve(0, newRef = malloc(newRef));
		if_true_block_log(error_printf, 0 == (newRef->References = one_if(malloc(newRef->Instance))), llc_free(newRef); return 0;);
		TRef					* oldRef				= *llc_reference;
		*llc_reference		= newRef;
		ref_release(&oldRef);
		return (*llc_reference)->Instance;
	}
	tplT_vtArgs	T*		ref_create				(gref<T>* * llc_reference, _tArgs&&... argsConstructor)	{
		tydf	gref<T>			TRef;
		TRef					* newRef				= 0;
		if_true_ve(0, 0 == ref_allocate(&newRef));
		new (newRef->Instance) T{argsConstructor...};
		TRef					* oldRef				= *llc_reference;
		*llc_reference		= newRef;
		ref_release(&oldRef);
		return (*llc_reference)->Instance;
	}
} // namespace

#endif // LLC_REF_H_23627
