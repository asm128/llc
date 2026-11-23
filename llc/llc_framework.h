#include "llc_gui.h"
#include "llc_runtime.h"
#include "llc_window.h"
#include "llc_timer.h"
#include "llc_frameinfo.h"
#include "llc_json_expression.h"
#include "llc_file.h"
#include "llc_parse.h"

#ifndef LLC_ATMEL
#	include <mutex>
#endif

#ifndef LLC_FRAMEWORK_H_23627
#define LLC_FRAMEWORK_H_23627

nmsp llc
{
	struct SFrameworkDetail {
#if defined(LLC_WINDOWS)
#elif defined(LLC_XCB)
		xcb_connection_t			* XCBConnection			= {};
		xcb_screen_t				* XCBScreen				= {};

									~SFrameworkDetail		() {
			if(XCBConnection) {
				xcb_disconnect(XCBConnection);
				info_printf("%s.", "XCB disconnected");
			}
		}

									SFrameworkDetail		() {
			if(0 == XCBConnection) {
				XCBConnection				= xcb_connect(NULL, NULL);
				if_true_tef(0 == XCBConnection, "%s", "Failed to connect to X Server!");
				XCBScreen					= xcb_setup_roots_iterator(xcb_get_setup(XCBConnection)).data;
			}
		}


#endif
	};

	struct SFrameworkSettings {
		b8_t						GUIZoom					: 1;
	};

	struct SFramework : SFrameTimer {
		SRuntimeValues		& RuntimeValues			;
		vcst_t				FileNameJSONConfig		= "llc_config.json";
		pobj<SGUI>			GUI						= {};
		SFrameworkDetail	PlatformDetail			= {};
		SWindow				RootWindow				= {};
		SFrameworkSettings	Settings				= {};
		SJSONFile			JSONConfig				= {};

#ifndef LLC_ATMEL
		::std::mutex			LockGUI;
#endif

		inln						SFramework				(SRuntimeValues & runtimeValues, vcst_t fileNameJSONConfig = "llc_config.json")
			: RuntimeValues(runtimeValues), FileNameJSONConfig(fileNameJSONConfig) {
			if_null_e(RootWindow.Input.create());
			if_null_e(GUI.create());
			if(fileNameJSONConfig.size())	// Attempt to load config file.
				if_fail_w(jsonFileRead(JSONConfig, fileNameJSONConfig));
		}
	}; // struct

	err_t 			updateFramework			(SFramework & framework);
	err_t 			mainWindowCreate		(SWindow & mainWindow, SRuntimeValuesDetail & runtimeValues, pobj<SInput> & displayInput);
	err_t 			mainWindowDestroy		(SWindow & mainWindow);
} // nmsp

#endif // LLC_FRAMEWORK_H_23627
