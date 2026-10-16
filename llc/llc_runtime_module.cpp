#include "llc_runtime_module.h"

::llc::error_t			llc::loadRuntimeModule					(::llc::SRuntimeModule& loadedModule, const ::llc::vcs& moduleName)				{
	loadedModule.Handle								= LLC_LOAD_MODULE(moduleName.begin());
	ree_if(0 == loadedModule.Handle, "Cannot load module: %s.", moduleName.begin());
	typedef ::llc::SRuntimeModule::TRegistry TRegistry;
	const ::llc::view<const ::llc::STypeIdentifier>	& funcnames		= TRegistry::get_types();
	const ::llc::view<const ::llc::DATA_TYPE>			& members		= TRegistry::get_data_type_ids();
	for(uint32_t iMember = 0, countMembers = members.size(); iMember < countMembers; ++iMember) {
		if(members[iMember] == ::llc::DATA_TYPE_FUN) {
			(&loadedModule.Handle)[iMember + 1]				= LLC_LOAD_MODULE_FUNCTION(loadedModule.Handle, funcnames[iMember].Name.begin());
			if(0 == (&loadedModule.Handle)[iMember + 1])
				error_printf("Failed to load module function: %s.", funcnames[iMember].Name.begin());
		}
		else
			break;
	}
	return 0;
}

::llc::err_t llc::unloadRuntimeModule(SRuntimeModule & loadedModule) {
	if_true_ve(OS_BUSY, loadedModule.Application);
	if(0 == loadedModule.Handle)
		rtrn 0;
#if defined(LLC_WINDOWS)
	if_zero_ve(OS_ERROR, LLC_FREE_MODULE(loadedModule.Handle));
#elif defined(LLC_LINUX) || defined(LLC_ANDROID)
	if_true_ve(OS_ERROR, LLC_FREE_MODULE(loadedModule.Handle));
#else
	rtrn OS_NOT_AVAILABLE;
#endif
	loadedModule = {};
	rtrn 0;
}
