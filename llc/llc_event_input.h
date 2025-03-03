#include "llc_system_event.h"
#include "llc_n2.h"

#ifndef LLC_EVENT_INPUT_H_23627
#define LLC_EVENT_INPUT_H_23627

namespace llc
{	
	GDEFINE_ENUM_TYPE(EVENT_TEXT, uint8_t);
	GDEFINE_ENUM_VALUE(EVENT_TEXT, Char		, 0);
	GDEFINE_ENUM_VALUE(EVENT_TEXT, Position	, 1);
	GDEFINE_ENUM_VALUE(EVENT_TEXT, Cursor	, 2);
	stin	::llc::error_t	eventEnqueueTextChar		(::llc::apobj<::llc::SEventSystem> & queue, uint32_t utf8)	{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Text, ::llc::EVENT_TEXT_Char, utf8); }

	GDEFINE_ENUM_TYPE (EVENT_KEYBOARD, uint8_t);
	GDEFINE_ENUM_VALUE(EVENT_KEYBOARD, Up		, 0);
	GDEFINE_ENUM_VALUE(EVENT_KEYBOARD, Down		, 1);
	GDEFINE_ENUM_VALUE(EVENT_KEYBOARD, SysUp	, 2);
	GDEFINE_ENUM_VALUE(EVENT_KEYBOARD, SysDown	, 3);
	stin	::llc::error_t	eventEnqueueKeyboardUp		(::llc::apobj<::llc::SEventSystem> & queue, uint8_t key)	{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Keyboard, ::llc::EVENT_KEYBOARD_Up		, key); }
	stin	::llc::error_t	eventEnqueueKeyboardDown	(::llc::apobj<::llc::SEventSystem> & queue, uint8_t key)	{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Keyboard, ::llc::EVENT_KEYBOARD_Down	, key); }
	stin	::llc::error_t	eventEnqueueKeyboardSysUp	(::llc::apobj<::llc::SEventSystem> & queue, uint8_t key)	{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Keyboard, ::llc::EVENT_KEYBOARD_SysUp	, key); }
	stin	::llc::error_t	eventEnqueueKeyboardSysDown	(::llc::apobj<::llc::SEventSystem> & queue, uint8_t key)	{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Keyboard, ::llc::EVENT_KEYBOARD_SysDown, key); }

	GDEFINE_ENUM_TYPE (EVENT_MOUSE, uint8_t);
	GDEFINE_ENUM_VALUE(EVENT_MOUSE, Position	, 0);
	GDEFINE_ENUM_VALUE(EVENT_MOUSE, Up			, 1);
	GDEFINE_ENUM_VALUE(EVENT_MOUSE, Down		, 2);
	GDEFINE_ENUM_VALUE(EVENT_MOUSE, DoubleClick	, 3);
	GDEFINE_ENUM_VALUE(EVENT_MOUSE, Wheel		, 4);
	stin	::llc::error_t	eventEnqueueMousePosition	(::llc::apobj<::llc::SEventSystem> & queue, ::llc::n2i16 coord)	{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Mouse, ::llc::EVENT_MOUSE_Position		, coord ); }
	stin	::llc::error_t	eventEnqueueMouseUp			(::llc::apobj<::llc::SEventSystem> & queue, uint8_t button)		{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Mouse, ::llc::EVENT_MOUSE_Up			, button); }
	stin	::llc::error_t	eventEnqueueMouseDown		(::llc::apobj<::llc::SEventSystem> & queue, uint8_t button)		{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Mouse, ::llc::EVENT_MOUSE_Down			, button); }
	stin	::llc::error_t	eventEnqueueMouseDoubleClick(::llc::apobj<::llc::SEventSystem> & queue, uint8_t button)		{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Mouse, ::llc::EVENT_MOUSE_DoubleClick	, button); }
	stin	::llc::error_t	eventEnqueueMouseWheel		(::llc::apobj<::llc::SEventSystem> & queue, int16_t delta )		{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Mouse, ::llc::EVENT_MOUSE_Wheel		, delta ); }

	GDEFINE_ENUM_TYPE (EVENT_TOUCH, uint8_t);
	GDEFINE_ENUM_VALUE(EVENT_TOUCH, Begin		, 0);
	GDEFINE_ENUM_VALUE(EVENT_TOUCH, End			, 1);
	GDEFINE_ENUM_VALUE(EVENT_TOUCH, Update		, 2);
	GDEFINE_ENUM_VALUE(EVENT_TOUCH, Ownership	, 3);
	stin	::llc::error_t	eventEnqueueTouchBegin		(::llc::apobj<::llc::SEventSystem> & queue, ::llc::n2u1_t coord)	{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Touch, ::llc::EVENT_TOUCH_Begin	, coord); }
	stin	::llc::error_t	eventEnqueueTouchEnd		(::llc::apobj<::llc::SEventSystem> & queue, ::llc::n2u1_t coord)	{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Touch, ::llc::EVENT_TOUCH_End		, coord); }
	stin	::llc::error_t	eventEnqueueTouchUpdate		(::llc::apobj<::llc::SEventSystem> & queue, ::llc::n2u1_t coord)	{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Touch, ::llc::EVENT_TOUCH_Update	, coord); }
	stin	::llc::error_t	eventEnqueueTouchOwnership	(::llc::apobj<::llc::SEventSystem> & queue, ::llc::n2u1_t coord)	{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Touch, ::llc::EVENT_TOUCH_Ownership, coord); }

	GDEFINE_ENUM_TYPE(EVENT_JOYPAD, uint8_t);
	GDEFINE_ENUM_VALUE(EVENT_JOYPAD, Move		, 0);
	GDEFINE_ENUM_VALUE(EVENT_JOYPAD, Button_up	, 1);
	GDEFINE_ENUM_VALUE(EVENT_JOYPAD, Button_down, 2);
	GDEFINE_ENUM_VALUE(EVENT_JOYPAD, ZMove		, 3);
	stin	::llc::error_t	eventEnqueueJoyPadMove		(::llc::apobj<::llc::SEventSystem> & queue, ::llc::n2i16 coord)	{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Joypad, ::llc::EVENT_JOYPAD_Move		, coord ); }
	stin	::llc::error_t	eventEnqueueJoyPadZMove		(::llc::apobj<::llc::SEventSystem> & queue, uint16_t value)		{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Joypad, ::llc::EVENT_JOYPAD_ZMove		, value ); }
	stin	::llc::error_t	eventEnqueueJoyPadUp		(::llc::apobj<::llc::SEventSystem> & queue, uint8_t button)		{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Joypad, ::llc::EVENT_JOYPAD_Button_up	, button); }
	stin	::llc::error_t	eventEnqueueJoyPadDown		(::llc::apobj<::llc::SEventSystem> & queue, uint8_t button)		{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Joypad, ::llc::EVENT_JOYPAD_Button_down, button); }
} // namespace

#endif // LLC_EVENT_INPUT_H_23627
