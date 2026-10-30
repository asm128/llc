#include "llc_array.h"

#ifndef LLC_TYPE_REGISTRY_H_23627
#define LLC_TYPE_REGISTRY_H_23627

namespace llc
{
	struct STypeRegistry {
		aobj<vcst_t>			Names;
		au2_t					MemberCount;

		sttc	STypeRegistry&	GetGlobalInstance	()	nxpt	{
			sttc STypeRegistry			instance;
			rtrn instance;
		}
	};

	tplt<tpnm _t>
	struct STypeRegistrator {
		oper					s2_t				()	{ return 0; }

								STypeRegistrator	()	{
			STypeRegistry				& typeRegistry		= ::llc::STypeRegistry::GetGlobalInstance();
			typeRegistry.Names		.push_back({_t::llc_type_name(), _t::llc_type_name_len()});
			typeRegistry.MemberCount.push_back(0);
		}
	};

#define LLC_BEGIN_POD(_structName)	struct _structName {

#define LLC_END_POD(_structName)																							\
	public:																													\
		sinx	::llc::view<sc_t>	llc_type_name			()	nxpt	{ rtrn {::llc::size(#_structName) - 1, #_structName}; }	\
		sinx	u2_t		llc_type_name_len		()	nxpt	{ rtrn ::llc::size(#_structName) - 1; }					\
	};																														\
	static	const int	llc_tr_##_structName	= ::llc::STypeRegistrator<_structName>();

#define LLC_STRUCT_MEMBER_DESC_NAME(_name)	_llc_dsc_##_name

#define LLC_STRUCT_MEMBER(_type, _name, ...)																	\
	struct LLC_STRUCT_MEMBER_DESC_NAME(_name) {																	\
		sinx	::llc::view<sc_t>	llc_member_type_name	()	nxpt	{ rtrn {::llc::size(#_type) - 1, #_type}; }	\
		sinx	::llc::view<sc_t>	llc_member_name			()	nxpt	{ rtrn {::llc::size(#_name) - 1, #_name}; }	\
	};																											\
	_type				_name
}

#endif // LLC_TYPE_REGISTRY_H_23627
