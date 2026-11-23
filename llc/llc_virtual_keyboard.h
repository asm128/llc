#include "llc_enum.h"
#include "llc_gui_control_state.h"

#ifndef LLC_VIRTUAL_KEYBOARD_H_23627
#define LLC_VIRTUAL_KEYBOARD_H_23627

namespace llc
{
	GDEFINE_ENUM_TYPE (VK_EVENT, uint8_t);
	GDEFINE_ENUM_VALUE(VK_EVENT, PRESS		,  1);
	GDEFINE_ENUM_VALUE(VK_EVENT, RELEASE	,  2);
	GDEFINE_ENUM_VALUE(VK_EVENT, HOLD		,  3);
	GDEFINE_ENUM_VALUE(VK_EVENT, EDIT		,  4);
	GDEFINE_ENUM_VALUE(VK_EVENT, ARROW		,  5);

	GDEFINE_ENUM_TYPE (VK_SCANCODE, uint8_t);
	GDEFINE_ENUM_VALUE(VK_SCANCODE, Escape		, 0);
	GDEFINE_ENUM_VALUE(VK_SCANCODE, Enter		, 1);
	GDEFINE_ENUM_VALUE(VK_SCANCODE, Backspace	, 2);
	GDEFINE_ENUM_VALUE(VK_SCANCODE, Clear		, 3);
	GDEFINE_ENUM_VALUE(VK_SCANCODE, Left		, 4);
	GDEFINE_ENUM_VALUE(VK_SCANCODE, Right		, 5);
	GDEFINE_ENUM_VALUE(VK_SCANCODE, Up			, 6);
	GDEFINE_ENUM_VALUE(VK_SCANCODE, Down		, 7);
#pragma pack(push, 1)
	struct SVirtualKeyboardEvent {
		VK_EVENT		Type;
		VK_SCANCODE		ScanCode;
	};
#pragma pack(pop)

	struct SVirtualKeyboard {
		::llc::cid_t	IdRoot;
		::llc::acid		IdKeys;
		::llc::au1_t	Keys;

		::llc::apod<::llc::SVirtualKeyboardEvent>	Events;
	};

	
	::llc::err_t			virtualKeyboardHandleEvent	(const ::llc::SVirtualKeyboard & vk, ::llc::cid_t iControl, ::llc::apod<::llc::SVirtualKeyboardEvent> & events);

	//stin	::llc::err_t	virtualKeyboardHandleEvent	(::llc::SVirtualKeyboard & vk, cid_t iControl) { return ::llc::virtualKeyboardHandleEvent(vk, iControl, vk.Events); }
} // namespace 

#endif // LLC_VIRTUAL_KEYBOARD_H_23627
