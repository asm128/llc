#include "llc_log.h"
#include "llc_runtime.h"
#include "llc_module.h"
#include "llc_view.h"
#include "llc_member_registry.h"

#ifndef LLC_RUNTIME_MODULE_H_23627
#define LLC_RUNTIME_MODULE_H_23627

#if defined(LLC_LINUX) || defined(LLC_ANDROID)
#	include <dlfcn.h>
#endif

LLC_DECLARE_MODULE_FUNCTION(llc_moduleTitle		, ::llc::sc_t	* outputBuffer, ::llc::u0_t * outputBufferLength			);
LLC_DECLARE_MODULE_FUNCTION(llc_moduleVersion	, ::llc::u2_t	* version													);
LLC_DECLARE_MODULE_FUNCTION(llc_moduleTitle		, ::llc::sc_t	* outputBuffer, ::llc::u0_t * outputBufferLength			);
LLC_DECLARE_MODULE_FUNCTION(llc_moduleVersion	, ::llc::u2_t	* version													);
LLC_DECLARE_MODULE_FUNCTION(llc_moduleCreate	, void*			* customApplication, ::llc::SRuntimeValues * runtimeValues	);
LLC_DECLARE_MODULE_FUNCTION(llc_moduleDelete	, void*			* customApplication											);
LLC_DECLARE_MODULE_FUNCTION(llc_moduleSetup		, void			* customApplication											);
LLC_DECLARE_MODULE_FUNCTION(llc_moduleCleanup	, void			* customApplication											);
LLC_DECLARE_MODULE_FUNCTION(llc_moduleRender	, void			* customApplication											);
LLC_DECLARE_MODULE_FUNCTION(llc_moduleUpdate	, void			* customApplication, bool requestedExit						);

namespace llc
{
#pragma pack(push, 1)
	struct SRuntimeModule : public SModuleBase {
		LLCMFND(, llc_moduleTitle	, Title		)				= 0;
		LLCMFND(, llc_moduleVersion	, Version	)				= 0;
		LLCMFND(, llc_moduleCreate	, Create	)				= 0;
		LLCMFND(, llc_moduleDelete	, Delete	)				= 0;
		LLCMFND(, llc_moduleSetup	, Setup		)				= 0;
		LLCMFND(, llc_moduleCleanup	, Cleanup	)				= 0;
		LLCMFND(, llc_moduleRender	, Render	)				= 0;
		LLCMFND(, llc_moduleUpdate	, Update	)				= 0;

		void													* Application					= 0;
		LLCM_REGISTRY
			(	LLCM_NAME(Title		)
			,	LLCM_NAME(Version	)
			,	LLCM_NAME(Create	)
			,	LLCM_NAME(Delete	)
			,	LLCM_NAME(Setup		)
			,	LLCM_NAME(Cleanup	)
			,	LLCM_NAME(Render	)
			,	LLCM_NAME(Update	)
			);
	};
#pragma pack(pop)

	err_t	loadRuntimeModule	(SRuntimeModule & loadedModule, cnst vcst_t & moduleName);
	err_t	unloadRuntimeModule	(SRuntimeModule & loadedModule);
}

#endif // LLC_RUNTIME_MODULE_H_23627
