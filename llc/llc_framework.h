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

namespace llc
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
				gthrow_if(0 == XCBConnection, "%s", "Failed to connect to X Server!");
				XCBScreen					= xcb_setup_roots_iterator(xcb_get_setup(XCBConnection)).data;
			}
		}

									
#endif
	};

	struct SFrameworkSettings {
		bool						GUIZoom					: 1;
	};

	struct SFramework : llc::SFrameTimer {
		::llc::SRuntimeValues		& RuntimeValues			;
		::llc::vcs					FileNameJSONConfig		= "llc_config.json";
		::llc::pobj<::llc::SGUI>	GUI						= {};
		::llc::SFrameworkDetail		PlatformDetail			= {};
		::llc::SWindow				RootWindow				= {};
		::llc::SFrameworkSettings	Settings				= {};
		::llc::SJSONFile			JSONConfig				= {};

#ifndef LLC_ATMEL
		::std::mutex				LockGUI;
#endif

		inline						SFramework				(::llc::SRuntimeValues & runtimeValues, ::llc::vcs fileNameJSONConfig = "llc_config.json")
			: RuntimeValues(runtimeValues), FileNameJSONConfig(fileNameJSONConfig) {
			RootWindow.Input.create();
			GUI.create();
			if(fileNameJSONConfig.size())	// Attempt to load config file.
				if_if(failed(::llc::jsonFileRead(JSONConfig, fileNameJSONConfig)), "File name: '%s'.", fileNameJSONConfig.begin());
		}
	}; // struct

	::llc::error_t				updateFramework			(::llc::SFramework & framework);
	::llc::error_t				mainWindowCreate		(::llc::SWindow & mainWindow, ::llc::SRuntimeValuesDetail & runtimeValues, ::llc::pobj<SInput> & displayInput);
	::llc::error_t				mainWindowDestroy		(::llc::SWindow & mainWindow);
} // namespace

#endif // LLC_FRAMEWORK_H_23627
