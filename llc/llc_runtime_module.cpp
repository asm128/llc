#include "llc_runtime_module.h"

llc::err_t llc::unloadRuntimeModule(SRuntimeModule & loadedModule) {
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
llc::err_t llc::loadRuntimeModule(SRuntimeModule & loadedModule, cnst vcst_t & moduleName) {
	if_true_ve(OS_BUSY, loadedModule.Handle);
	loadedModule.Handle = LLC_LOAD_MODULE(moduleName.begin());
	if_null_vef(OS_NOT_FOUND, loadedModule.Handle, "Cannot load module:%.*s.", LLC_VIEW_ARGS(moduleName));

	tydf SRuntimeModule::TRegistry TRegistry;
	cnst view<cnst STypeIdentifier> & functionNames	= TRegistry::get_types();
	cnst view<cnst DATA_TYPE> & memberTypes			= TRegistry::get_data_type_ids();
	for(u2_t iMember = 0; iMember < memberTypes.size(); ++iMember) {
		if(DATA_TYPE_FUN != memberTypes[iMember])
			break;
		(&loadedModule.Handle)[iMember + 1] = LLC_LOAD_MODULE_FUNCTION(loadedModule.Handle, functionNames[iMember].Name.begin());
		if(0 == (&loadedModule.Handle)[iMember + 1]) {
			error_printf("Failed to load module function:%s.", functionNames[iMember].Name.begin());
			LLC_FREE_MODULE(loadedModule.Handle);
			loadedModule = {};
			rtrn OS_NOT_FOUND;
		}
	}
	rtrn 0;
}
