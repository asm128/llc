#include "llc_input.h"
#include "llc_event_input.h"
#include "llc_event_screen.h"

#include "llc_array_static.h"
#include "llc_ptr_obj.h"
#include "llc_image.h"
#include "llc_rect.h"
#include "llc_grid_color.h"

#if defined(LLC_WINDOWS)
#	include <Windows.h>
#elif defined(LLC_XCB)
#	include <xcb/xcb.h>
#endif

#ifndef LLC_WINDOW_H_23627
#define LLC_WINDOW_H_23627

namespace llc
{

	struct SWindowPlatformDetail {
#if defined(LLC_WINDOWS)
		typedef	::llc::bgra			TOSWindowNativeTexel;
		stacxpr	::llc::astatic<TCHAR, 256>	DefaultRootWindowClassName	= {TEXT("LLC_WINDOW")};

		::HWND								WindowHandle				= {};
		::WNDCLASSEX						WindowClass					= {sizeof(::WNDCLASSEX), CS_DBLCLKS, 0, 0, 0, 0, 0, 0, 0, 0, DefaultRootWindowClassName.begin(), 0};
		uint32_t							BitmapInfoSize				= 0;
		::BITMAPINFO						BitmapInfo					= {sizeof(::BITMAPINFO)};
		::HDC								IntermediateDeviceContext	= 0;    // <- note, we're creating, so it needs to be destroyed
		::HBITMAP							IntermediateBitmap			= 0;
		::llc::view<TOSWindowNativeTexel>	PixelBits					= {};

											~SWindowPlatformDetail		()	{
			if(IntermediateBitmap			) ::DeleteObject	(IntermediateBitmap			);
			if(IntermediateDeviceContext	) ::DeleteDC		(IntermediateDeviceContext	);
			if(WindowHandle					) ::DestroyWindow	(WindowHandle);
		}
#elif defined(LLC_XCB)
		typedef	::llc::bgra					TOSWindowNativeTexel;
		xcb_connection_t					* Connection				= {};
		xcb_gcontext_t						GC							= {};
		xcb_drawable_t						IdDrawable					= {};
		xcb_drawable_t						IdDrawableBackPixmap		= {};

											~SWindowPlatformDetail		()	{
			if(uint32_t(-1) != GC) {
				xcb_free_gc(Connection, GC);
				info_printf("%s.", "Window graphics context freed");
			}
		}
#endif // LLC_WINDOWS
	};

	struct SWindow {
		typedef	::llc::bgra					TTexel;
		typedef	::llc::rt<TTexel, uint32_t>	TOffscreen;

		::llc::SWindowPlatformDetail		PlatformDetail			= {};
		::llc::apobj<::llc::SEventSystem>	EventQueue				= {};
		::llc::pobj<TOffscreen>				BackBuffer				= {};
		::llc::pobj<::llc::SInput>			Input					= {};
		::llc::n2u1_t						PreviousSize			= {};
		::llc::n2u1_t						Size					= {320, 200};
		::llc::rectu1_t						WindowedWindowRect		= {0, 0, 320, 200};

		bool								Resized					: 1;
		bool								Repaint					: 1;
		bool								NoDraw					: 1;
		bool								MinOrMaxed				: 1;
		bool								Closed					: 1;
		bool								FullScreen				: 1;
	};

	::llc::error_t						windowPresentTarget		(::llc::SWindow & windowInstance, const ::llc::g8bgra & targetToPresent);
	::llc::error_t						windowUpdate			(::llc::SWindow & windowInstance);
	::llc::error_t						windowUpdateTick		(::llc::SWindow & windowInstance);
	::llc::error_t						fullScreenExit			(::llc::SWindow & windowInstance);
	::llc::error_t						fullScreenEnter			(::llc::SWindow & windowInstance);

	stainli ::llc::error_t				fullScreenToggle		(::llc::SWindow & windowInstance) { 
		return windowInstance.FullScreen ? ::llc::fullScreenExit(windowInstance) : ::llc::fullScreenEnter(windowInstance); 
	}

} // namespace

#endif // LLC_WINDOW_H_23627
