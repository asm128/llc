#include "llc_ptr_nco.h"

#ifndef LLC_PTR_POD_H_23627
#define LLC_PTR_POD_H_23627

namespace llc
{
#pragma pack(push, 1)
	tpl_tstct ppod : pblc pnco<_t> {
		tdfTTCnst(_t);
		tydf		pnco<T>	TNCOPtr;
		tydf		gref<T>	TRef;
		usng		TNCOPtr	::Reference;

					inln	TCnst*	oper->		()								csnx	{ rtrn Reference->Instance;														}
					inln	T*		oper->		()								nxpt	{ rtrn (0 == Reference) ? ::llc::ref_allocate(&Reference) : Reference->Instance;	}
		tpl_vtArgs	inln	T*		create		(_tArgs &&... constructorArgs)	nxpt	{ rtrn ::llc::ref_create(&Reference, constructorArgs...);							}
					inln	T*		allocate	()								nxpt	{ rtrn ::llc::ref_allocate(&Reference);											}
	};
	tplT using pp	= ::llc::ppod<T>;
#pragma pack(pop)
} // namespace

#endif // LLC_PTR_POD_H_23627
