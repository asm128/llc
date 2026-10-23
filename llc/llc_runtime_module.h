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

LLC_DECLARE_MODULE_FUNCTION(llc_moduleTitle		, ::llc::sc_t* outputBuffer, ::llc::u0_t* outputBufferLength		);
LLC_DECLARE_MODULE_FUNCTION(llc_moduleVersion	, ::llc::u2_t* version												);
LLC_DECLARE_MODULE_FUNCTION(llc_moduleCreate	, void*	* customApplication, ::llc::SRuntimeValues* runtimeValues	);
LLC_DECLARE_MODULE_FUNCTION(llc_moduleDelete	, void*	* customApplication											);
LLC_DECLARE_MODULE_FUNCTION(llc_moduleSetup		, void	* customApplication											);
LLC_DECLARE_MODULE_FUNCTION(llc_moduleCleanup	, void	* customApplication											);
LLC_DECLARE_MODULE_FUNCTION(llc_moduleRender	, void	* customApplication											);
LLC_DECLARE_MODULE_FUNCTION(llc_moduleUpdate	, void	* customApplication, bool requestedExit						);

namespace llc
{
#pragma pack(push, 1)
	struct SRuntimeModule : public SModuleBase {
		LLCMFND(, llc_moduleTitle	, Title		)	= 0;
		LLCMFND(, llc_moduleVersion	, Version	)	= 0;
		LLCMFND(, llc_moduleCreate	, Create	)	= 0;
		LLCMFND(, llc_moduleDelete	, Delete	)	= 0;
		LLCMFND(, llc_moduleSetup	, Setup		)	= 0;
		LLCMFND(, llc_moduleCleanup	, Cleanup	)	= 0;
		LLCMFND(, llc_moduleRender	, Render	)	= 0;
		LLCMFND(, llc_moduleUpdate	, Update	)	= 0;

		void	* Application						= 0;
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

	GDEFINE_ENUM_TYPE (RUNTIME_CALLBACK_ID, u0_t);
	GDEFINE_ENUM_VALUE(RUNTIME_CALLBACK_ID, NONE	, 0		);
	GDEFINE_ENUM_VALUE(RUNTIME_CALLBACK_ID, TITLE	, 0x01	);
	GDEFINE_ENUM_VALUE(RUNTIME_CALLBACK_ID, VERSION	, 0x02	);
	GDEFINE_ENUM_VALUE(RUNTIME_CALLBACK_ID, CREATE	, 0x04	);
	GDEFINE_ENUM_VALUE(RUNTIME_CALLBACK_ID, DELETE	, 0x08	);
	GDEFINE_ENUM_VALUE(RUNTIME_CALLBACK_ID, SETUP	, 0x10	);
	GDEFINE_ENUM_VALUE(RUNTIME_CALLBACK_ID, CLEANUP	, 0x20	);
	GDEFINE_ENUM_VALUE(RUNTIME_CALLBACK_ID, RENDER	, 0x40	);
	GDEFINE_ENUM_VALUE(RUNTIME_CALLBACK_ID, UPDATE	, 0x80	);

	GDEFINE_ENUM_TYPE (APPLICATION_STATE, u0_t);
	GDEFINE_ENUM_VALUE(APPLICATION_STATE, NORMAL	, 0);
	GDEFINE_ENUM_VALUE(APPLICATION_STATE, EXIT		, 1);
	GDEFINE_ENUM_VALUE(APPLICATION_STATE, BUSY		, 2);
	GDEFINE_ENUM_VALUE(APPLICATION_STATE, STAY		, 3);
	GDEFINE_ENUM_VALUE(APPLICATION_STATE, RESTART	, 4);
}

#define LLC_DEFINE_APP_MODULE_INTERFACE(_mainClass, _moduleTitle)																	\
	::llc::err_t	setup		(_mainClass& app);																					\
	::llc::err_t	cleanup		(_mainClass& app);																					\
	::llc::err_t	update		(_mainClass& app, bool systemRequestedExit);														\
	::llc::err_t	draw		(_mainClass& app);																					\
	::llc::err_t	LLC_STDCALL	llc_moduleVersion	()																{ return 1; }	\
	::llc::err_t	LLC_STDCALL	llc_moduleCreate	(void*	* instanceApp, ::llc::SRuntimeValues* runtimeValues)	{ try { *instanceApp = new _mainClass{*runtimeValues};											return 0;		} catch(...) {} return -1; }	\
	::llc::err_t	LLC_STDCALL	llc_moduleDelete	(void*	* instanceApp)											{ try { delete ((_mainClass*)*instanceApp);	*instanceApp = 0;									return 0;		} catch(...) {} return -1; }	\
	::llc::err_t	LLC_STDCALL	llc_moduleSetup		(void	* instanceApp)											{ try { const ::llc::err_t result = setup		(*(_mainClass*)instanceApp);					return result;	} catch(...) {} return -1; }	\
	::llc::err_t	LLC_STDCALL	llc_moduleCleanup	(void	* instanceApp)											{ try { const ::llc::err_t result = cleanup	(*(_mainClass*)instanceApp);						return result;	} catch(...) {} return -1; }	\
	::llc::err_t	LLC_STDCALL	llc_moduleUpdate	(void	* instanceApp, bool systemRequestedExit)				{ try { const ::llc::err_t result = update	(*(_mainClass*)instanceApp, systemRequestedExit);	return result;	} catch(...) {} return -1; }	\
	::llc::err_t	LLC_STDCALL	llc_moduleRender	(void	* instanceApp)											{ try { const ::llc::err_t result = draw	(*(_mainClass*)instanceApp);						return result;	} catch(...) {} return -1; }	\
	::llc::err_t	LLC_STDCALL	llc_moduleTitle		(::llc::sc_t * out_title, ::llc::u2_t *maxCount)					{	\
		stxp	::llc::sc_c			mylmoduleTitle[] = _moduleTitle;													\
		if(0 == out_title) 																								\
			return maxCount ? (*maxCount = ::llc::size(mylmoduleTitle)) : ::llc::size(mylmoduleTitle);					\
		memcpy(out_title, mylmoduleTitle, ::llc::min(::llc::size(mylmoduleTitle), *maxCount));							\
		return 0;																										\
	} //

#endif // LLC_RUNTIME_MODULE_H_23627
