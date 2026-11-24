#include "llc_event_input.h"
#include "llc_gui_control_state.h"
#include "llc_system_key.h"

#ifndef LLC_VIRTUAL_KEYBOARD_H_23627
#define LLC_VIRTUAL_KEYBOARD_H_23627

namespace llc
{
	stct SVirtualKeyboardCharacter {
		::llc::cid_t	IdControl	= ::llc::CID_INVALID;
		::llc::u2_t	Character	= {};
	};
	stct SVirtualKeyboardCommand {
		::llc::cid_t		IdControl	= ::llc::CID_INVALID;
		::llc::SYSTEM_KEY	Key			= {};
	};

	stct SVirtualKeyboard {
		::llc::cid_t						IdRoot		= ::llc::CID_INVALID;
		::llc::apod<::llc::SVirtualKeyboardCharacter>	Characters	= {};
		::llc::apod<::llc::SVirtualKeyboardCommand>	Commands	= {};
	};

	::llc::err_t	virtualKeyboardHandleEvent	(cnst ::llc::SVirtualKeyboard & keyboard, ::llc::cid_t iControl, ::llc::TQueueSystemEvent & events);
} // namespace llc

#endif // LLC_VIRTUAL_KEYBOARD_H_23627
