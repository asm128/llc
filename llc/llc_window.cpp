#include "llc_window.h"
#include "llc_file.h"
#include "llc_path.h"
#include "llc_png.h"
#include "llc_chrono.h"
#include "llc_string_compose.h"
#include "llc_system_key.h"
#include <filesystem>

::llc::err_t			llc::windowScreenshotHotkey(::llc::SWindow & window, ::llc::vcst_t programName, ::llc::vcst_t screenshotDirectory) {
	cnst ::llc::SInput & input = *window.Input;
	cnst ::llc::u0_t ctrl = ::llc::SYSTEM_KEY_Control, shift = ::llc::SYSTEM_KEY_Shift, s = 'S';
	if(false == (input.KeyboardPrevious.KeyState[ctrl] && input.KeyboardPrevious.KeyState[shift] && input.KeyboardPrevious.KeyState[s]
	&& (input.KeyUp(ctrl) || input.KeyUp(shift) || input.KeyUp(s))))
		rtrn 0;
	::llc::vcst_t executable = {};
	if_fail_fe(::llc::pathStem(programName, executable));
	::llc::string filename = {};
	if_fail_fe(::llc::timeStringFormat("{yy}{MM}{dd}_{hh}{mm}{ss}", ::llc::timeCurrentInMs(), filename));
#if defined(LLC_DEBUG_ENABLED)
	stxp ::llc::vcst_c configuration = "Debug";
#else
	stxp ::llc::vcst_c configuration = "Release";
#endif
	if_fail_fe(::llc::append_strings(filename, "_", executable, "_", configuration, ".png"));
	if_fail_fe(window.ScreenshotFile.clear());
	if_fail_fe(::llc::pathNameCompose(screenshotDirectory, filename, window.ScreenshotFile));
	cnst ::llc::string baseName = filename;
	::std::error_code fileError = {};
	for(::llc::u2_t repeat = 1; ::std::filesystem::exists(window.ScreenshotFile.begin(), fileError); ++repeat) {
		filename = baseName;
		if_fail_fe(filename.resize(filename.size() - 4));
		if_fail_fe(::llc::append_strings(filename, "_", ::llc::str(repeat), ".png"));
		if_fail_fe(window.ScreenshotFile.clear());
		if_fail_fe(::llc::pathNameCompose(screenshotDirectory, filename, window.ScreenshotFile));
	}
	if(fileError) {
		warning_printf("Screenshot collision check failed: %s.", fileError.message().c_str());
		rtrn 0;
	}
	if_fail_fe(::llc::eventEnqueueScreenSaveFramebuffer(window.EventQueue));
	::llc::vcst_t screenshot_path = window.ScreenshotFile;
	info_printf("Screenshot expected at: \"%.*s\"", LLC_VIEW_ARGS(screenshot_path));
	rtrn 0;
}

::llc::err_t			llc::windowSaveFramebuffer(::llc::SWindow & window) {
	if_zero_fe(window.ScreenshotFile.size());
	if_null_fe(window.BackBuffer.get_ref());
	if_zero_fe(window.BackBuffer->Color.View.size());
	::llc::vcst_t			folder			= {};
	if_fail_fe(::llc::pathDirectory(window.ScreenshotFile, folder));
	if_fail_fe(::llc::pathCreate(folder));
	::llc::au0_t			pngBytes		= {};
	if_fail_fe(::llc::pngFileWrite(window.BackBuffer->Color.View, pngBytes));
	rtrn ::llc::fileFromMemory(window.ScreenshotFile, pngBytes);
}

#if defined(LLC_XCB)
#	include <xcb/xcb_image.h>
#	include <xkbcommon/xkbcommon-keysyms.h>
#elif defined(LLC_FRAMEBUFFER)
#	include <errno.h>
#	include <string.h>
#	include <unistd.h>
#	include <xkbcommon/xkbcommon-keysyms.h>
#endif

LLC_USING_TYPEINT();

llc::err_t 		llc::fullScreenExit			(::llc::SWindow & window) {
	if(false == window.FullScreen)
		return 0;

	window.PreviousSize		= window.Size;
	window.Size				= window.WindowedWindowRect.Dimensions();
#if defined(LLC_WINDOWS)
	HWND						windowHandle				= window.PlatformDetail.WindowHandle;
	DWORD						style						= GetWindowLong(windowHandle, GWL_STYLE);
	SetWindowLong(windowHandle, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
	SetWindowPos(windowHandle, NULL, window.WindowedWindowRect.Left, window.WindowedWindowRect.Top
		, window.WindowedWindowRect.Right  - window.WindowedWindowRect.Left
		, window.WindowedWindowRect.Bottom - window.WindowedWindowRect.Top
		, SWP_NOOWNERZORDER | SWP_FRAMECHANGED
	);
#elif defined(LLC_XCB)
	u2_c				values[]					= {window.WindowedWindowRect.Left, window.WindowedWindowRect.Top, window.Size.x, window.Size.y};
	xcb_configure_window(window.PlatformDetail.Connection, window.PlatformDetail.IdDrawable, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y | XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, values);
	xcb_flush(window.PlatformDetail.Connection);
#endif // LLC_WINDOWS
	window.Resized = window.Repaint = true;
	if_true_e(failed(::llc::eventEnqueueScreenResize(window.EventQueue, window.Size)));
	window.FullScreen		= false;
	return 0;
}

llc::err_t 		llc::fullScreenEnter		(::llc::SWindow & window) {
	if(window.FullScreen)
		return 1;

	window.PreviousSize		= window.Size;
#if defined(LLC_WINDOWS)
	HWND						windowHandle				= window.PlatformDetail.WindowHandle;
	MONITORINFO					monitor_info				= {sizeof(monitor_info)};
	RECT						windowsRect					= {};
	if_true_fef(FALSE == GetWindowRect(windowHandle, &windowsRect), "Cannot get window rect for hWnd(0x%x)", windowHandle);
	window.WindowedWindowRect.Top		= (u1_t)windowsRect.top		;
	window.WindowedWindowRect.Left		= (u1_t)windowsRect.left	;
	window.WindowedWindowRect.Right		= (u1_t)windowsRect.right	;
	window.WindowedWindowRect.Bottom	= (u1_t)windowsRect.bottom	;
	if_true_fef(FALSE == GetMonitorInfoA(MonitorFromWindow(windowHandle, MONITOR_DEFAULTTOPRIMARY), &monitor_info), "Cannot get MONITORINFO for hWnd(0x%x)", windowHandle);

	cnst DWORD					style						= GetWindowLong(windowHandle, GWL_STYLE);
	SetWindowLong(windowHandle, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
	SetWindowPos(windowHandle, HWND_TOP, monitor_info.rcMonitor.left, monitor_info.rcMonitor.top
		, monitor_info.rcMonitor.right - monitor_info.rcMonitor.left
		, monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top
		, SWP_NOOWNERZORDER | SWP_FRAMECHANGED
	);

	window.Size				=
		{ u1_t(monitor_info.rcMonitor.right - monitor_info.rcMonitor.left)
		, u1_t(monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top)
		};
#elif defined(LLC_XCB)
	xcb_get_geometry_reply_t	* geometry					= xcb_get_geometry_reply(window.PlatformDetail.Connection, xcb_get_geometry(window.PlatformDetail.Connection, window.PlatformDetail.IdDrawable), 0);
	if_null_fe(geometry);
	window.WindowedWindowRect.Left	= geometry->x;
	window.WindowedWindowRect.Top	= geometry->y;
	window.WindowedWindowRect.Right	= geometry->x + geometry->width;
	window.WindowedWindowRect.Bottom	= geometry->y + geometry->height;
	free(geometry);
	xcb_screen_t				* screen					= xcb_setup_roots_iterator(xcb_get_setup(window.PlatformDetail.Connection)).data;
	window.Size				= {screen->width_in_pixels, screen->height_in_pixels};
	u2_c				values[]					= {0, 0, window.Size.x, window.Size.y};
	xcb_configure_window(window.PlatformDetail.Connection, window.PlatformDetail.IdDrawable, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y | XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, values);
	xcb_flush(window.PlatformDetail.Connection);
#endif // LLC_WINDOWS
	window.Resized = window.Repaint = true;
	if_true_e(failed(::llc::eventEnqueueScreenResize(window.EventQueue, window.Size)));
	window.FullScreen		= true;
	return 0;
}

#if defined(LLC_XCB) || defined(LLC_FRAMEBUFFER)
sttc	u0_t		xkbSystemKey			(xkb_keysym_t keySym) {
	if(keySym >= XKB_KEY_a && keySym <= XKB_KEY_z)
		return (u0_t)('A' + keySym - XKB_KEY_a);
	if(keySym >= XKB_KEY_A && keySym <= XKB_KEY_Z)
		return (u0_t)keySym;
	if(keySym >= XKB_KEY_0 && keySym <= XKB_KEY_9)
		return (u0_t)keySym;
	if(keySym >= XKB_KEY_KP_0 && keySym <= XKB_KEY_KP_9)
		return (u0_t)(::llc::SYSTEM_KEY_Numpad0 + keySym - XKB_KEY_KP_0);
	if(keySym >= XKB_KEY_F1 && keySym <= XKB_KEY_F24)
		return (u0_t)(::llc::SYSTEM_KEY_F1 + keySym - XKB_KEY_F1);

	switch(keySym) {
	default					: return 0;
	case XKB_KEY_BackSpace		: return ::llc::SYSTEM_KEY_Backspace;
	case XKB_KEY_Tab			:
	case XKB_KEY_ISO_Left_Tab	: return ::llc::SYSTEM_KEY_Tab;
	case XKB_KEY_Clear			: return ::llc::SYSTEM_KEY_Clear;
	case XKB_KEY_Return			:
	case XKB_KEY_KP_Enter		: return ::llc::SYSTEM_KEY_Enter;
	case XKB_KEY_Shift_L		: return ::llc::SYSTEM_KEY_LShift;
	case XKB_KEY_Shift_R		: return ::llc::SYSTEM_KEY_RShift;
	case XKB_KEY_Control_L		: return ::llc::SYSTEM_KEY_LControl;
	case XKB_KEY_Control_R		: return ::llc::SYSTEM_KEY_RControl;
	case XKB_KEY_Alt_L			:
	case XKB_KEY_Meta_L			: return ::llc::SYSTEM_KEY_LMenu;
	case XKB_KEY_Alt_R			:
	case XKB_KEY_Meta_R			:
	case XKB_KEY_ISO_Level3_Shift
							: return ::llc::SYSTEM_KEY_RMenu;
	case XKB_KEY_Pause			:
	case XKB_KEY_Break			: return ::llc::SYSTEM_KEY_Pause;
	case XKB_KEY_Caps_Lock		: return ::llc::SYSTEM_KEY_Capital;
	case XKB_KEY_Escape			: return ::llc::SYSTEM_KEY_Escape;
	case XKB_KEY_space			:
	case XKB_KEY_KP_Space		: return ::llc::SYSTEM_KEY_Space;
	case XKB_KEY_Page_Up		:
	case XKB_KEY_KP_Prior		: return ::llc::SYSTEM_KEY_Prior;
	case XKB_KEY_Page_Down		:
	case XKB_KEY_KP_Next		: return ::llc::SYSTEM_KEY_Next;
	case XKB_KEY_End			:
	case XKB_KEY_KP_End			: return ::llc::SYSTEM_KEY_End;
	case XKB_KEY_Home			:
	case XKB_KEY_KP_Home		: return ::llc::SYSTEM_KEY_Home;
	case XKB_KEY_Left			:
	case XKB_KEY_KP_Left		: return ::llc::SYSTEM_KEY_Left;
	case XKB_KEY_Up				:
	case XKB_KEY_KP_Up			: return ::llc::SYSTEM_KEY_Up;
	case XKB_KEY_Right			:
	case XKB_KEY_KP_Right		: return ::llc::SYSTEM_KEY_Right;
	case XKB_KEY_Down			:
	case XKB_KEY_KP_Down		: return ::llc::SYSTEM_KEY_Down;
	case XKB_KEY_Select			: return ::llc::SYSTEM_KEY_Select;
	case XKB_KEY_Print			:
	case XKB_KEY_Sys_Req		: return ::llc::SYSTEM_KEY_Snapshot;
	case XKB_KEY_Execute		: return ::llc::SYSTEM_KEY_Execute;
	case XKB_KEY_Insert			:
	case XKB_KEY_KP_Insert		: return ::llc::SYSTEM_KEY_Insert;
	case XKB_KEY_Delete			:
	case XKB_KEY_KP_Delete		: return ::llc::SYSTEM_KEY_Delete;
	case XKB_KEY_Help			: return ::llc::SYSTEM_KEY_Help;
	case XKB_KEY_Super_L		:
	case XKB_KEY_Hyper_L		: return ::llc::SYSTEM_KEY_Lwin;
	case XKB_KEY_Super_R		:
	case XKB_KEY_Hyper_R		: return ::llc::SYSTEM_KEY_Rwin;
	case XKB_KEY_Menu			: return ::llc::SYSTEM_KEY_Apps;
	case XKB_KEY_KP_Multiply	: return ::llc::SYSTEM_KEY_Multiply;
	case XKB_KEY_KP_Add			: return ::llc::SYSTEM_KEY_Add;
	case XKB_KEY_KP_Separator	: return ::llc::SYSTEM_KEY_Separator;
	case XKB_KEY_KP_Subtract	: return ::llc::SYSTEM_KEY_Subtract;
	case XKB_KEY_KP_Decimal		: return ::llc::SYSTEM_KEY_Decimal;
	case XKB_KEY_KP_Divide		: return ::llc::SYSTEM_KEY_Divide;
	case XKB_KEY_Num_Lock		: return ::llc::SYSTEM_KEY_Numlock;
	case XKB_KEY_Scroll_Lock	: return ::llc::SYSTEM_KEY_Scroll;
	case XKB_KEY_semicolon		:
	case XKB_KEY_colon			: return ::llc::SYSTEM_KEY_OEM_1;
	case XKB_KEY_equal			:
	case XKB_KEY_plus			: return ::llc::SYSTEM_KEY_OEM_plus;
	case XKB_KEY_comma			:
	case XKB_KEY_less			: return ::llc::SYSTEM_KEY_OEM_comma;
	case XKB_KEY_minus			:
	case XKB_KEY_underscore		: return ::llc::SYSTEM_KEY_OEM_minus;
	case XKB_KEY_period			:
	case XKB_KEY_greater		: return ::llc::SYSTEM_KEY_OEM_period;
	case XKB_KEY_slash			:
	case XKB_KEY_question		: return ::llc::SYSTEM_KEY_OEM_2;
	case XKB_KEY_grave			:
	case XKB_KEY_asciitilde		: return ::llc::SYSTEM_KEY_OEM_3;
	case XKB_KEY_bracketleft	:
	case XKB_KEY_braceleft		: return ::llc::SYSTEM_KEY_OEM_4;
	case XKB_KEY_backslash		:
	case XKB_KEY_bar			: return ::llc::SYSTEM_KEY_OEM_5;
	case XKB_KEY_bracketright	:
	case XKB_KEY_braceright		: return ::llc::SYSTEM_KEY_OEM_6;
	case XKB_KEY_apostrophe		:
	case XKB_KEY_quotedbl		: return ::llc::SYSTEM_KEY_OEM_7;
	}
}
#endif

#if defined(LLC_XCB)
sttc	xkb_keysym_t	xcbBaseKeySym			(cnst ::llc::SWindowPlatformDetail & detail, xkb_keycode_t keyCode, xkb_keysym_t currentKeySym) {
	xkb_layout_index_t	layout					= xkb_state_key_get_layout(detail.XKBState, keyCode);
	if(XKB_LAYOUT_INVALID == layout)
		layout = 0;
	cnst xkb_keysym_t	* keySyms				= {};
	return 0 < xkb_keymap_key_get_syms_by_level(detail.XKBKeymap, keyCode, layout, 0, &keySyms) ? keySyms[0] : currentKeySym;
}

sttc	u2_t		xcbTextCodePoint		(cnst ::llc::SWindowPlatformDetail & detail, xkb_keycode_t keyCode, xkb_keysym_t keySym) {
	u2_c		codePoint					= xkb_state_key_get_utf32(detail.XKBState, keyCode);
	if(0 == detail.XKBComposeState)
		return codePoint;

	if(XKB_COMPOSE_FEED_ACCEPTED != xkb_compose_state_feed(detail.XKBComposeState, keySym))
		return codePoint;

	switch(xkb_compose_state_get_status(detail.XKBComposeState)) {
	default:
	case XKB_COMPOSE_NOTHING	: return codePoint;
	case XKB_COMPOSE_COMPOSING	: return 0;
	case XKB_COMPOSE_CANCELLED	:
		xkb_compose_state_reset(detail.XKBComposeState);
		return 0;
	case XKB_COMPOSE_COMPOSED	: {
		u2_c	composed					= xkb_keysym_to_utf32(xkb_compose_state_get_one_sym(detail.XKBComposeState));
		xkb_compose_state_reset(detail.XKBComposeState);
		return composed;
	}	}
}
//
//sttc	u2_t		xcbInputModifiers		(cnst ::llc::SWindowPlatformDetail & detail, cnst ::llc::SInputKeyboard & keyboard) {
//	u2_t				modifiers					= {};
//	if(0 < xkb_state_mod_name_is_active(detail.XKBState, XKB_MOD_NAME_SHIFT, XKB_STATE_MODS_EFFECTIVE)) modifiers |= ::llc::INPUT_MODIFIER_Shift;
//	if(0 < xkb_state_mod_name_is_active(detail.XKBState, XKB_MOD_NAME_CAPS , XKB_STATE_MODS_EFFECTIVE)) modifiers |= ::llc::INPUT_MODIFIER_CapsLock;
//	if(0 < xkb_state_mod_name_is_active(detail.XKBState, XKB_MOD_NAME_CTRL , XKB_STATE_MODS_EFFECTIVE)) modifiers |= ::llc::INPUT_MODIFIER_Control;
//	if(0 < xkb_state_mod_name_is_active(detail.XKBState, XKB_MOD_NAME_ALT  , XKB_STATE_MODS_EFFECTIVE)) modifiers |= ::llc::INPUT_MODIFIER_Alt;
//	if(0 < xkb_state_mod_name_is_active(detail.XKBState, XKB_MOD_NAME_NUM  , XKB_STATE_MODS_EFFECTIVE)) modifiers |= ::llc::INPUT_MODIFIER_NumLock;
//	if(0 < xkb_state_mod_name_is_active(detail.XKBState, XKB_MOD_NAME_LOGO , XKB_STATE_MODS_EFFECTIVE)) modifiers |= ::llc::INPUT_MODIFIER_Super;
//	if(keyboard.KeyState[::llc::SYSTEM_KEY_RMenu]) modifiers |= ::llc::INPUT_MODIFIER_AltGr;
//	return modifiers;
//}
#elif defined(LLC_FRAMEBUFFER)
sttc	xkb_keysym_t	framebufferBaseKeySym	(cnst ::llc::SWindowPlatformDetail & detail, xkb_keycode_t keyCode, xkb_keysym_t currentKeySym) {
	::xkb_state			* state					= detail.XKBStates [detail.XKBActiveLayout];
	::xkb_keymap		* keymap				= detail.XKBKeymaps[detail.XKBActiveLayout];
	xkb_layout_index_t	layout					= ::xkb_state_key_get_layout(state, keyCode);
	if(XKB_LAYOUT_INVALID == layout)
		layout = 0;
	cnst xkb_keysym_t	* keySyms				= {};
	return 0 < ::xkb_keymap_key_get_syms_by_level(keymap, keyCode, layout, 0, &keySyms) ? keySyms[0] : currentKeySym;
}

sttc	void		framebufferKeyboardStateUpdate	(::llc::SWindowPlatformDetail & detail, xkb_keycode_t keyCode, b8_t pressed) {
	for(u2_t iLayout = 0; iLayout < detail.KeyboardLayoutCount; ++iLayout)
		::xkb_state_update_key(detail.XKBStates[iLayout], keyCode, pressed ? XKB_KEY_DOWN : XKB_KEY_UP);
}

sttc	llc::err_t framebufferInputUpdate		(::llc::SWindow & window, int inputDevice, u2_t deviceId) {
	::llc::SWindowPlatformDetail	& detail			= window.PlatformDetail;
	::llc::SInput					& input				= *window.Input;
	::input_event					event				= {};
	while(sizeof(event) == ::read(inputDevice, &event, sizeof(event))) {
		if(EV_KEY == event.type) {
			s2_t		buttonIndex				= -1;
			switch(event.code) {
			default			: break;
			case BTN_LEFT	: buttonIndex = 0; break;
			case BTN_MIDDLE	: buttonIndex = 1; break;
			case BTN_RIGHT	: buttonIndex = 2; break;
			}
			if(0 <= buttonIndex) {
				if(2 == event.value)
					continue;
				cnst b8_t pressed = 0 != event.value;
				input.MouseCurrent.ButtonState[buttonIndex] = pressed;
				if_fail_fe(pressed
					? ::llc::eventEnqueueMouseDown(window.EventQueue, (u0_t)buttonIndex)
					: ::llc::eventEnqueueMouseUp  (window.EventQueue, (u0_t)buttonIndex)
					);
				window.Repaint = true;
				continue;
			}

			cnst b8_t		pressed				= 0 != event.value;
			cnst b8_t		repeated				= 2 == event.value;
			if(!repeated) {
				if(KEY_LEFTCTRL == event.code || KEY_RIGHTCTRL == event.code) detail.ControlDown = pressed;
				if(KEY_LEFTALT  == event.code || KEY_RIGHTALT  == event.code) detail.AltDown = pressed;
			}
			cnst xkb_keycode_t	xkbKeyCode				= (xkb_keycode_t)event.code + 8;
			::xkb_state			* activeState				= detail.XKBStates[detail.XKBActiveLayout];
			cnst xkb_keysym_t	keySym					= ::xkb_state_key_get_one_sym(activeState, xkbKeyCode);
			cnst xkb_keysym_t	baseKeySym				= ::framebufferBaseKeySym(detail, xkbKeyCode, keySym);
			cnst u0_t			keyCode					= ::xkbSystemKey(baseKeySym);
			cnst u2_t			codePoint				= pressed ? ::xkb_state_key_get_utf32(activeState, xkbKeyCode) : 0;
			if(!repeated)
				::framebufferKeyboardStateUpdate(detail, xkbKeyCode, pressed);

			if(KEY_SPACE == event.code && pressed && detail.ControlDown && detail.AltDown) {
				detail.XKBActiveLayout = (detail.XKBActiveLayout + 1) % detail.KeyboardLayoutCount;
				detail.LayoutSwitchHeld = true;
				info_printf("Keyboard layout: %s.", 0 == detail.XKBActiveLayout ? "US" : 1 == detail.XKBActiveLayout ? "Spanish International" : "Latin American Spanish");
				continue;
			}
			if(KEY_SPACE == event.code && detail.LayoutSwitchHeld) {
				if(!pressed)
					detail.LayoutSwitchHeld = false;
				continue;
			}

			cnst f2_t timeStamp = (f2_t)event.time.tv_sec + (f2_t)event.time.tv_usec * .000001f;
			if(keyCode) {
				if_fail_fe(::llc::inputKeyboardStateSet(input, keyCode, pressed, deviceId, timeStamp));
				if_fail_fe(detail.AltDown
					? pressed ? ::llc::eventEnqueueKeyboardSysDown(window.EventQueue, keyCode) : ::llc::eventEnqueueKeyboardSysUp(window.EventQueue, keyCode)
					: pressed ? ::llc::eventEnqueueKeyboardDown   (window.EventQueue, keyCode) : ::llc::eventEnqueueKeyboardUp   (window.EventQueue, keyCode)
					);
			}
			if(codePoint)
				if_fail_fe(::llc::eventEnqueueTextChar(window.EventQueue, codePoint));
			window.Repaint = true;
		}
		else if(EV_REL == event.type) {
			switch(event.code) {
			default			: break;
			case REL_X		: detail.PointerX += event.value; break;
			case REL_Y		: detail.PointerY += event.value; break;
			case REL_WHEEL	: {
				input.MouseCurrent.Deltas.z += event.value;
				if_fail_fe(::llc::eventEnqueueMouseWheel(window.EventQueue, (s1_t)event.value));
				break;
			}	}
		}
		else if(EV_SYN == event.type && SYN_REPORT == event.code) {
			detail.PointerX = ::llc::clamped(detail.PointerX, 0, (s2_t)window.Size.x - 1);
			detail.PointerY = ::llc::clamped(detail.PointerY, 0, (s2_t)window.Size.y - 1);
			::llc::n2s1_t	newPosition				= {(s1_t)detail.PointerX, (s1_t)detail.PointerY};
			if(newPosition != input.MouseCurrent.Position) {
				input.MouseCurrent.Deltas.x += newPosition.x - input.MouseCurrent.Position.x;
				input.MouseCurrent.Deltas.y += newPosition.y - input.MouseCurrent.Position.y;
				input.MouseCurrent.Position = newPosition;
				if_fail_fe(::llc::eventEnqueueMousePosition(window.EventQueue, newPosition));
				window.Repaint = true;
			}
		}
	}
	return EAGAIN == errno || EWOULDBLOCK == errno ? 0 : -1;
}
#endif

llc::err_t 		llc::windowUpdateTick		(::llc::SWindow& displayInstance)											{
	b8_t						quit						= false;
#if defined(LLC_WINDOWS)
	::MSG						msg							= {};
	int							counter						= 0;
	while(::PeekMessage(&msg, displayInstance.PlatformDetail.WindowHandle, 0, 0, PM_REMOVE) && (100 > counter++)) {
		::TranslateMessage	(&msg);
		::DispatchMessage	(&msg);
		if(msg.message == WM_QUIT)
			quit = true;
	}
#elif defined(LLC_XCB)
	SInput				& input						= *displayInstance.Input;
	int							counter						= 0;
	while(xcb_generic_event_t * event = xcb_poll_for_event(displayInstance.PlatformDetail.Connection)) {
		switch(event->response_type & ~0x80) {
		default					: break;
		case XCB_EXPOSE			: displayInstance.Repaint = true; if_fail_e(eventEnqueueScreenRedraw(displayInstance.EventQueue, 0)); break;
		case XCB_FOCUS_IN		: if_fail_e(eventEnqueueScreenActivate(displayInstance.EventQueue, 0)); break;
		case XCB_FOCUS_OUT		: if_fail_e(eventEnqueueScreenDeactivate(displayInstance.EventQueue, 0)); break;
		case XCB_MAP_NOTIFY		: displayInstance.Repaint = true; if_fail_e(eventEnqueueScreenShow(displayInstance.EventQueue, 0)); break;
		case XCB_UNMAP_NOTIFY	: if_fail_e(eventEnqueueScreenHide(displayInstance.EventQueue, 0)); break;
		case XCB_KEY_PRESS		:
		case XCB_KEY_RELEASE	: {
			b8_c				pressed					= XCB_KEY_PRESS == (event->response_type & ~0x80);
			cnst xcb_key_press_event_t & key					= *(xcb_key_press_event_t*)event;
			SWindowPlatformDetail & detail				= displayInstance.PlatformDetail;
			cnst xkb_keysym_t	keySym						= xkb_state_key_get_one_sym(detail.XKBState, key.detail);
			cnst xkb_keysym_t	baseKeySym					= ::xcbBaseKeySym(detail, key.detail, keySym);
			u0_c				keyCode						= ::xkbSystemKey(baseKeySym);
			u2_c				codePoint					= pressed ? ::xcbTextCodePoint(detail, key.detail, keySym) : 0;
			xkb_state_update_key(detail.XKBState, key.detail, pressed ? XKB_KEY_DOWN : XKB_KEY_UP);
			if_fail_e(inputKeyboardStateSet(input, keyCode, pressed, detail.XKBDeviceId, key.time * .001f));

			b8_c			menuKey						= input.KeyboardCurrent.KeyState[SYSTEM_KEY_Menu]
				|| input.KeyboardCurrent.KeyState[SYSTEM_KEY_LMenu] || input.KeyboardCurrent.KeyState[SYSTEM_KEY_RMenu];
			b8_c			systemKey					= SYSTEM_KEY_Enter == keyCode && menuKey;
			if_fail_e(pressed
				? systemKey ? eventEnqueueKeyboardSysDown(displayInstance.EventQueue, keyCode) : eventEnqueueKeyboardDown(displayInstance.EventQueue, keyCode)
				: systemKey ? eventEnqueueKeyboardSysUp  (displayInstance.EventQueue, keyCode) : eventEnqueueKeyboardUp  (displayInstance.EventQueue, keyCode)
				);
			if(codePoint)
				if_fail_e(eventEnqueueTextChar(displayInstance.EventQueue, codePoint));
			//
			displayInstance.Repaint = true;
			break;
		}
		case XCB_BUTTON_PRESS	: {
			cnst xcb_button_press_event_t & button = *(xcb_button_press_event_t*)event;
			displayInstance.Repaint = true;
			if(button.detail == 4 || button.detail == 5) {
				input.MouseCurrent.Deltas.z = button.detail == 4 ? 1 : -1;
				if_fail_e(eventEnqueueMouseWheel(displayInstance.EventQueue, input.MouseCurrent.Deltas.z));
			}
			else if(button.detail && button.detail <= input.MouseCurrent.ButtonState.size()) {
				input.MouseCurrent.ButtonState[button.detail - 1] = 1;
				if_fail_e(eventEnqueueMouseDown(displayInstance.EventQueue, button.detail - 1));
			}
			break;
		}
		case XCB_BUTTON_RELEASE	: {
			cnst xcb_button_release_event_t & button = *(xcb_button_release_event_t*)event;
			displayInstance.Repaint = true;
			if(button.detail && button.detail <= input.MouseCurrent.ButtonState.size() && button.detail != 4 && button.detail != 5) {
				input.MouseCurrent.ButtonState[button.detail - 1] = 0;
				if_fail_e(eventEnqueueMouseUp(displayInstance.EventQueue, button.detail - 1));
			}
			break;
		}
		case XCB_MOTION_NOTIFY	: {
			cnst xcb_motion_notify_event_t & motion = *(xcb_motion_notify_event_t*)event;
			cnst s2_t x = motion.same_screen ? motion.event_x : motion.root_x;
			cnst s2_t y = motion.same_screen ? motion.event_y : motion.root_y;
			input.MouseCurrent.Position = n2s2_t{x, y}.Clamp({}, displayInstance.Size.s2()).s1();
			input.MouseCurrent.Deltas.x = input.MouseCurrent.Position.x - input.MousePrevious.Position.x;
			input.MouseCurrent.Deltas.y = input.MouseCurrent.Position.y - input.MousePrevious.Position.y;
			displayInstance.Repaint = true;
			if_fail_e(eventEnqueueMousePosition(displayInstance.EventQueue, input.MouseCurrent.Position));
			break;
		}
		case XCB_CONFIGURE_NOTIFY: {
			cnst xcb_configure_notify_event_t & configure = *(xcb_configure_notify_event_t*)event;
			n2u1_c size = {configure.width, configure.height};
			n2u1_c position = {(u1_t)configure.x, (u1_t)configure.y};
			if(false == displayInstance.FullScreen && (position.x != displayInstance.WindowedWindowRect.Left || position.y != displayInstance.WindowedWindowRect.Top)) {
				displayInstance.WindowedWindowRect.Left = position.x;
				displayInstance.WindowedWindowRect.Top = position.y;
				displayInstance.WindowedWindowRect.Right = position.x + size.x;
				displayInstance.WindowedWindowRect.Bottom = position.y + size.y;
				if_fail_e(eventEnqueueScreenMove(displayInstance.EventQueue, position));
			}
			if(size != displayInstance.Size) {
				displayInstance.PreviousSize = displayInstance.Size;
				displayInstance.Size = size;
				if(false == displayInstance.FullScreen) {
					displayInstance.WindowedWindowRect.Right = displayInstance.WindowedWindowRect.Left + size.x;
					displayInstance.WindowedWindowRect.Bottom = displayInstance.WindowedWindowRect.Top + size.y;
				}
				displayInstance.Resized = displayInstance.Repaint = true;
				if_fail_e(eventEnqueueScreenResize(displayInstance.EventQueue, size));
			}
			break;
		}
		case XCB_CLIENT_MESSAGE	: {
			cnst xcb_client_message_event_t & message = *(xcb_client_message_event_t*)event;
			if(message.data.data32[0] == displayInstance.PlatformDetail.WMDelete) {
				displayInstance.Closed = quit = true;
				if_fail_e(eventEnqueueScreenClose(displayInstance.EventQueue));
			}
			break;
		}
		case XCB_DESTROY_NOTIFY	: displayInstance.Closed = quit = true; if_fail_e(eventEnqueueScreenDestroy(displayInstance.EventQueue)); break;
		}
		free(event);
		if(100 <= ++counter)
			break;
	}
#elif defined(LLC_FRAMEBUFFER)
	for(u2_t iDevice = 0; iDevice < displayInstance.PlatformDetail.InputDeviceCount; ++iDevice)
		if_fail_fe(::framebufferInputUpdate(displayInstance, displayInstance.PlatformDetail.InputDevices[iDevice], iDevice));
#else
	(void)displayInstance;
#endif // LLC_WINDOWS
	return quit ? 1 : 0;
}

llc::err_t 		llc::windowUpdate		(SWindow & displayInstance)											{
	::llc::view<::llc::pobj<::llc::SEventSystem>> events = displayInstance.EventQueue;
	if_fail_fe(events.for_each([&displayInstance](cnst ::llc::pobj<::llc::SEventSystem> & event) -> ::llc::err_t {
		if_null_fe(event.get_ref());
		if(event->Type != ::llc::SYSTEM_EVENT_Screen)
			rtrn 0;
		rtrn ::llc::eventExtractAndHandle<::llc::EVENT_SCREEN>(*event, [&displayInstance](cnst ::llc::SScreenEventView & eventScreen) -> ::llc::err_t {
			if(eventScreen.Type == ::llc::EVENT_SCREEN_SaveFramebuffer) {
				cnst ::llc::err_t result = ::llc::windowSaveFramebuffer(displayInstance);
				if(::llc::failed(result)) error_printf("Screenshot save failed: %.*s.", LLC_VIEW_ARGS(displayInstance.ScreenshotFile));
			}
			rtrn 0;
		});
	}));
	displayInstance.EventQueue.clear();
	displayInstance.Resized	= false;
	displayInstance.Repaint	= false;
	b8_t						quit					= false;
#if defined(LLC_WINDOWS)
	::MSG						msg						= {};
	while(::PeekMessage(&msg, displayInstance.PlatformDetail.WindowHandle, 0, 0, PM_NOREMOVE))
		if(1 == windowUpdateTick(displayInstance))
			quit					= true;
#elif defined(LLC_XCB)
	err_t updateResult;
	if_fail_vef(updateResult, updateResult = windowUpdateTick(displayInstance), "updateResult:%" LLC_FMT_U2, updateResult);
	quit = 1 == updateResult;
#elif defined(LLC_FRAMEBUFFER)
	err_t updateResult;
	if_fail_vef(updateResult, updateResult = windowUpdateTick(displayInstance), "updateResult:%" LLC_FMT_U2, updateResult);
	quit = 1 == updateResult;
#endif // LLC_WINDOWS
	return quit ? 1 : 0;
}

#if defined(LLC_WINDOWS)
llc::err_t 		drawBuffer				(::HDC hdc, ::llc::SWindowPlatformDetail & offscreenDetail, int width, int height, cnst ::llc::g8bgra& colorArray)				{
	u2_c				bytesToCopy				= sizeof(::RGBQUAD) * colorArray.size();
	::llc::n2u1_c				metricsSource			= colorArray.metrics().u1();
	::llc::n2u1_c				prevSize				= {(u1_t)offscreenDetail.BitmapInfo.bmiHeader.biWidth, (u1_t)offscreenDetail.BitmapInfo.bmiHeader.biHeight};
	if( metricsSource.x != offscreenDetail.BitmapInfo.bmiHeader.biWidth
	 || metricsSource.y != offscreenDetail.BitmapInfo.bmiHeader.biHeight
	 || width  != offscreenDetail.BitmapInfo.bmiHeader.biWidth
	 || height != offscreenDetail.BitmapInfo.bmiHeader.biHeight
	 || 0 == offscreenDetail.IntermediateDeviceContext
	 || 0 == offscreenDetail.IntermediateBitmap
	) {
		offscreenDetail.BitmapInfoSize			= sizeof(::BITMAPINFO);
		//memcpy(&offscreenDetail.BitmapInfo->bmiColors[1], colorArray[0].begin(), metricsSource.x * metricsSource.y * sizeof(::llc::bgra));

		offscreenDetail.BitmapInfo.bmiHeader				= {sizeof(::BITMAPINFO)};
		offscreenDetail.BitmapInfo.bmiHeader.biWidth		= colorArray.metrics().x;
		offscreenDetail.BitmapInfo.bmiHeader.biHeight		= colorArray.metrics().y;
		offscreenDetail.BitmapInfo.bmiHeader.biPlanes		= 1;
		offscreenDetail.BitmapInfo.bmiHeader.biBitCount		= 32;
		offscreenDetail.BitmapInfo.bmiHeader.biCompression	= BI_RGB;
		offscreenDetail.BitmapInfo.bmiHeader.biSizeImage	= bytesToCopy;

		if(offscreenDetail.IntermediateBitmap)
			DeleteObject(offscreenDetail.IntermediateBitmap);

		if(offscreenDetail.IntermediateDeviceContext)
			DeleteDC(offscreenDetail.IntermediateDeviceContext);

		::llc::SWindowPlatformDetail::TOSWindowNativeTexel		* pixelBits						= 0;
		offscreenDetail.IntermediateDeviceContext			= ::CreateCompatibleDC(hdc);    // <- note, we're creating, so it needs to be destroyed
		if_true_fef(0 == (offscreenDetail.IntermediateBitmap		= ::CreateDIBSection(offscreenDetail.IntermediateDeviceContext, &offscreenDetail.BitmapInfo, DIB_RGB_COLORS, (void**) &pixelBits, NULL, 0)), "%s", "Failed to create intermediate dib section.");
		offscreenDetail.PixelBits							= {pixelBits, colorArray.area()};
	}
	u2_c lineWidth	= metricsSource.x * sizeof(::llc::bgra);
	u2_c lastLine		= metricsSource.y - 1;
	cnst ::llc::bgra									* dest										= colorArray.begin();
	for(u2_t y = 0, yMax = metricsSource.y; y < yMax; ++y)
		memcpy(&offscreenDetail.PixelBits[y * metricsSource.x], &dest[(lastLine - y) * metricsSource.x], lineWidth);

	::HBITMAP									hBmpOld										= (::HBITMAP)::SelectObject(offscreenDetail.IntermediateDeviceContext, offscreenDetail.IntermediateBitmap);    // <- altering state
	if_true_ef(FALSE == ::BitBlt(hdc, 0, 0
		, ::llc::min(metricsSource.x, (u1_t)offscreenDetail.BitmapInfo.bmiHeader.biWidth)
		, ::llc::min(metricsSource.y, (u1_t)offscreenDetail.BitmapInfo.bmiHeader.biHeight)
		, offscreenDetail.IntermediateDeviceContext, 0, 0, SRCCOPY)
		, "%s", "Not sure why would this happen but probably due to mismanagement of the target size or the system resources. I've had it failing when I acquired the device too much and never released it."
	);
	//::SetStretchBltMode(hdc, COLORONCOLOR);
	//if_true_e(FALSE == ::StretchBlt(hdc, 0, 0, width, height, offscreenDetail.IntermediateDeviceContext, 0, 0, colorArray.metrics().x, colorArray.metrics().y, SRCCOPY), "%s", "Not sure why would this happen but probably due to mismanagement of the target size or the system resources. I've had it failing when I acquired the device too much and never released it.");
	::SelectObject(hdc, hBmpOld);	// put the old bitmap back in the DC (restore state)
	return 0;
}
#endif // LLC_WINDOWS

llc::err_t 		llc::windowPresentTarget					(::llc::SWindow& displayInstance, cnst ::llc::g8bgra& targetToPresent)		{
	(void)displayInstance; (void)targetToPresent;
#if defined(LLC_WINDOWS)
	::HWND					windowHandle								= displayInstance.PlatformDetail.WindowHandle;
	if_true_vwf(1, 0 == windowHandle, "%s", "presentTarget called without a valid window handle set for the main window.");
	::HDC					dc											= ::GetDC(windowHandle);
	if_true_fef(0 == dc, "%s", "Failed to retrieve device context from the provided window handle.");
	if_true_ef(failed(::drawBuffer(dc, displayInstance.PlatformDetail, displayInstance.Size.x, displayInstance.Size.y, targetToPresent)), "%s", "Not sure why this would happen.");
	::ReleaseDC(windowHandle, dc);
#elif defined(LLC_XCB)
	if(false == displayInstance.Repaint)
		return 0;
	xcb_screen_t				* screen							= xcb_setup_roots_iterator(xcb_get_setup(displayInstance.PlatformDetail.Connection)).data;
	cnst ::llc::n2u2_t		& metrics						= targetToPresent.metrics();
	xcb_image_t				* image							= xcb_image_create_native
		( displayInstance.PlatformDetail.Connection
		, metrics.x
		, metrics.y
		, XCB_IMAGE_FORMAT_Z_PIXMAP
		, screen->root_depth
		, (u0_t*)targetToPresent.begin()
		, targetToPresent.size() * sizeof(::llc::bgra)
		, (u0_t*)targetToPresent.begin()
		);
	if_null_fe(image);
	xcb_image_put(displayInstance.PlatformDetail.Connection, displayInstance.PlatformDetail.IdDrawable, displayInstance.PlatformDetail.GC, image, 0, 0, 0);
	image->base = 0;
	xcb_image_destroy(image);
	xcb_flush(displayInstance.PlatformDetail.Connection);
	displayInstance.Repaint = false;
#elif defined(LLC_FRAMEBUFFER)
	::llc::SWindowPlatformDetail	& detail					= displayInstance.PlatformDetail;
	if(0 == detail.FramebufferMemory)
		return -1;
	cnst ::llc::n2<u2_t>	& metrics					= targetToPresent.metrics();
	if(0 == metrics.x || 0 == metrics.y)
		return 0;
	cnst u2_t			bytesPerPixel				= (detail.FramebufferVariable.bits_per_pixel + 7) / 8;
	if(0 == bytesPerPixel || 4 < bytesPerPixel)
		return -1;
	cnst b8_t			directCopy					=
		   4 == bytesPerPixel
		&& metrics.x == detail.FramebufferVariable.xres
		&& metrics.y == detail.FramebufferVariable.yres
		&&  0 == detail.FramebufferVariable.blue.offset && 8 == detail.FramebufferVariable.blue.length
		&&  8 == detail.FramebufferVariable.green.offset && 8 == detail.FramebufferVariable.green.length
		&& 16 == detail.FramebufferVariable.red.offset && 8 == detail.FramebufferVariable.red.length;
	for(u2_t y = 0; y < detail.FramebufferVariable.yres; ++y) {
		u0_t				* targetRow					= detail.FramebufferMemory
			+ (size_t)(y + detail.FramebufferVariable.yoffset) * detail.FramebufferFixed.line_length
			+ (size_t)detail.FramebufferVariable.xoffset * bytesPerPixel;
		cnst u2_t			sourceY					= directCopy ? y : (u2_t)((u3_t)y * metrics.y / detail.FramebufferVariable.yres);
		if(directCopy) {
			::memcpy(targetRow, targetToPresent.begin() + (size_t)sourceY * metrics.x, (size_t)metrics.x * sizeof(::llc::bgra));
			continue;
		}
		for(u2_t x = 0; x < detail.FramebufferVariable.xres; ++x) {
			cnst u2_t			sourceX					= (u2_t)((u3_t)x * metrics.x / detail.FramebufferVariable.xres);
			cnst ::llc::bgra	& source					= targetToPresent.begin()[(size_t)sourceY * metrics.x + sourceX];
			auto scaleChannel = [](u0_t value, cnst ::fb_bitfield & field) -> u2_t {
				if(0 == field.length)
					return 0;
				u2_c maximum = 32 <= field.length ? 0xFFFFFFFFU : ((1U << field.length) - 1U);
				return ((u2_t)value * maximum / 255U) << field.offset;
			};
			u2_c output = scaleChannel(source.r, detail.FramebufferVariable.red)
				| scaleChannel(source.g, detail.FramebufferVariable.green)
				| scaleChannel(source.b, detail.FramebufferVariable.blue)
				| scaleChannel(source.a, detail.FramebufferVariable.transp);
			::memcpy(targetRow + (size_t)x * bytesPerPixel, &output, bytesPerPixel);
		}
	}
	displayInstance.Repaint = false;
#else // ??
	(void)displayInstance; (void)targetToPresent;
#endif // LLC_WINDOWS
	return 0;
}
