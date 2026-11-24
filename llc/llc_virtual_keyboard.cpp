#include "llc_virtual_keyboard.h"

::llc::err_t	llc::virtualKeyboardHandleEvent	(cnst ::llc::SVirtualKeyboard & keyboard, ::llc::cid_t iControl, ::llc::TQueueSystemEvent & events) {
	for(::llc::u2_t iCharacter = 0; iCharacter < keyboard.Characters.size(); ++iCharacter) {
		if(iControl == keyboard.Characters[iCharacter].IdControl) {
			if_fail_fe(::llc::eventEnqueueTextChar(events, keyboard.Characters[iCharacter].Character));
			rtrn 1;
		}
	}
	for(::llc::u2_t iCommand = 0; iCommand < keyboard.Commands.size(); ++iCommand) {
		if(iControl == keyboard.Commands[iCommand].IdControl) {
			if_fail_fe(::llc::eventEnqueueKeyboardDown(events, (::llc::u0_t)keyboard.Commands[iCommand].Key));
			rtrn 1;
		}
	}
	rtrn 0;
}
