#include "llc_input.h"
#include "llc_event_input.h"
#include "llc_event_screen.h"

#include "llc_array_static.h"
#include "llc_ptr_obj.h"
#include "llc_image.h"
#include "llc_rect.h"
#include "llc_grid_color.h"
#include "llc_string.h"

#if defined(LLC_WINDOWS)
#	include <Windows.h>
#elif defined(LLC_XCB)
#	include <xcb/xcb.h>
#	include <xkbcommon/xkbcommon.h>
#	include <xkbcommon/xkbcommon-compose.h>
#elif defined(LLC_FRAMEBUFFER)
#	include <linux/fb.h>
#	include <linux/input.h>
#	include <xkbcommon/xkbcommon.h>
#endif

#ifndef LLC_WINDOW_H_23627
#define LLC_WINDOW_H_23627

nmsp llc
{
	struct SWindowPlatformDetail {
#if defined(LLC_WINDOWS)
		tydf	::llc::bgra			TOSWindowNativeTexel;
		stxp	::llc::astatic<WCHAR, 256>	DefaultRootWindowClassName	= {L"LLC_WINDOW"};

		::HWND								WindowHandle				= {};
		::WNDCLASSEXW						WindowClass					= {sizeof(::WNDCLASSEXW), CS_DBLCLKS, 0, 0, 0, 0, 0, 0, 0, 0, DefaultRootWindowClassName.begin(), 0};
		u2_t								BitmapInfoSize				= 0;
		::BITMAPINFO						BitmapInfo					= {sizeof(::BITMAPINFO)};
		::HDC								IntermediateDeviceContext	= 0;    // <- note, we're creating, so it needs to be destroyed
		::HBITMAP							IntermediateBitmap			= 0;
		::llc::view<TOSWindowNativeTexel>	PixelBits					= {};
		u1_t								PendingHighSurrogate		= {};

											~SWindowPlatformDetail		()	{
			if(IntermediateBitmap			) ::DeleteObject	(IntermediateBitmap			);
			if(IntermediateDeviceContext	) ::DeleteDC		(IntermediateDeviceContext	);
			if(WindowHandle					) ::DestroyWindow	(WindowHandle);
		}
#elif defined(LLC_XCB)
		tydf	::llc::bgra					TOSWindowNativeTexel;
		xcb_connection_t					* Connection				= {};
		xcb_gcontext_t						GC							= {};
		xcb_drawable_t						IdDrawable					= {};
		xcb_drawable_t						IdDrawableBackPixmap		= {};
		xcb_atom_t							WMDelete					= {};
		::xkb_context						* XKBContext				= {};
		::xkb_keymap						* XKBKeymap				= {};
		::xkb_state						* XKBState					= {};
		::xkb_compose_table				* XKBComposeTable			= {};
		::xkb_compose_state				* XKBComposeState			= {};
		s2_t								XKBDeviceId				= -1;

											~SWindowPlatformDetail		()	{
			if(GC) {
				xcb_free_gc(Connection, GC);
				info_printf("%s.", "Window graphics context freed");
			}
		}
#elif defined(LLC_FRAMEBUFFER)
		tydf	::llc::bgra					TOSWindowNativeTexel;
		stxp	u2_c						InputDeviceLimit			= 32;
		stxp	u2_c						KeyboardLayoutCount		= 3;

		int								Framebuffer				= -1;
		u0_t							* FramebufferMemory		= {};
		size_t							FramebufferBytes		= {};
		::fb_var_screeninfo				FramebufferVariable		= {};
		::fb_fix_screeninfo				FramebufferFixed			= {};
		::llc::astatic<int, InputDeviceLimit>	InputDevices			= {};
		u2_t							InputDeviceCount		= {};
		::xkb_context					* XKBContext				= {};
		::xkb_keymap					* XKBKeymaps[KeyboardLayoutCount]	= {};
		::xkb_state						* XKBStates [KeyboardLayoutCount]	= {};
		u2_t							XKBActiveLayout			= {};
		s2_t							PointerX				= {};
		s2_t							PointerY				= {};
		b8_t							ControlDown			= {};
		b8_t							AltDown					= {};
		b8_t							LayoutSwitchHeld		= {};
#endif // LLC_WINDOWS
	};

	struct SWindow {
		tydf	::llc::bgra					TTexel;
		tydf	::llc::rt<TTexel, u2_t>	TOffscreen;

		::llc::SWindowPlatformDetail		PlatformDetail			= {};
		::llc::apobj<::llc::SEventSystem>	EventQueue				= {};
		::llc::pobj<TOffscreen>				BackBuffer				= {};
		::llc::pobj<::llc::SInput>			Input					= {};
		::llc::string						ScreenshotFile			= {};
		::llc::n2u1_t						PreviousSize			= {};
		::llc::n2u1_t						Size					= {320, 200};
		::llc::rectu1_t						WindowedWindowRect		= {0, 0, 320, 200};

		b8_t								Resized					: 1;
		b8_t								Repaint					: 1;
		b8_t								NoDraw					: 1;
		b8_t								MinOrMaxed				: 1;
		b8_t								Closed					: 1;
		b8_t								FullScreen				: 1;
	};

	llc::err_t 					windowPresentTarget		(::llc::SWindow & windowInstance, cnst ::llc::g8bgra & targetToPresent);
	llc::err_t 					windowSaveFramebuffer	(::llc::SWindow & windowInstance);
	llc::err_t 					windowScreenshotHotkey	(::llc::SWindow & windowInstance, ::llc::vcst_t programName, ::llc::vcst_t screenshotDirectory);
	llc::err_t 					windowUpdate			(::llc::SWindow & windowInstance);
	llc::err_t 					windowUpdateTick		(::llc::SWindow & windowInstance);
	llc::err_t 					fullScreenExit			(::llc::SWindow & windowInstance);
	llc::err_t 					fullScreenEnter			(::llc::SWindow & windowInstance);

	stin llc::err_t 			fullScreenToggle		(::llc::SWindow & windowInstance) {
		return windowInstance.FullScreen ? ::llc::fullScreenExit(windowInstance) : ::llc::fullScreenEnter(windowInstance);
	}

} // nmsp

#endif // LLC_WINDOW_H_23627
