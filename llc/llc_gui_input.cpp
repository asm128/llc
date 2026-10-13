#include "llc_gui.h"
#include "llc_system_key.h"

sttc bool guiControlKeyboardEnabled(cnst ::llc::SGUI & gui, ::llc::cid_t idControl) nxpt {
	rtrn false == ::llc::controlInvalid(gui, idControl)
		&& 0 == ::llc::controlHidden(gui, idControl)
		&& 0 == ::llc::controlDisabled(gui, idControl)
		;
}

sttc ::llc::u2_t guiControlKeyboardIndex(::llc::vcid controlsKeyboard, ::llc::cid_t idControl) nxpt {
	for(::llc::u2_t iControl = 0; iControl < controlsKeyboard.size(); ++iControl) {
		if(controlsKeyboard[iControl] == idControl)
			rtrn iControl;
	}
	rtrn controlsKeyboard.size();
}

sttc void guiControlFocus(::llc::SGUI & gui, ::llc::cid_t idControl) nxpt {
	for(::llc::u2_t iControl = 0; iControl < gui.Controls.States.size(); ++iControl) {
		cnst ::llc::cid_t idCurrent = (::llc::cid_t)iControl;
		if(false == ::llc::controlInvalid(gui, idCurrent))
			gui.Controls.SetFocused(idCurrent, idCurrent == idControl);
	}
}

sttc ::llc::cid_t guiControlFocused(cnst ::llc::SGUI & gui, ::llc::vcid controlsKeyboard) nxpt {
	for(::llc::u2_t iControl = 0; iControl < controlsKeyboard.size(); ++iControl) {
		cnst ::llc::cid_t idControl = controlsKeyboard[iControl];
		if(::guiControlKeyboardEnabled(gui, idControl) && gui.Controls.IsFocused(idControl))
			rtrn idControl;
	}
	rtrn ::llc::CID_INVALID;
}

sttc ::llc::cid_t guiControlFocusMove(::llc::SGUI & gui, ::llc::vcid controlsKeyboard, bool reverse) nxpt {
	if(0 == controlsKeyboard.size())
		rtrn ::llc::CID_INVALID;
	cnst ::llc::cid_t idFocused = ::guiControlFocused(gui, controlsKeyboard);
	cnst ::llc::u2_t iFocused = ::guiControlKeyboardIndex(controlsKeyboard, idFocused);
	cnst ::llc::u2_t iOrigin = iFocused < controlsKeyboard.size() ? iFocused : (reverse ? 0 : controlsKeyboard.size() - 1);
	for(::llc::u2_t iOffset = 1; iOffset <= controlsKeyboard.size(); ++iOffset) {
		cnst ::llc::u2_t iControl = reverse
			? (iOrigin + controlsKeyboard.size() - iOffset) % controlsKeyboard.size()
			: (iOrigin + iOffset) % controlsKeyboard.size()
			;
		cnst ::llc::cid_t idControl = controlsKeyboard[iControl];
		if(::guiControlKeyboardEnabled(gui, idControl)) {
			::guiControlFocus(gui, idControl);
			rtrn idControl;
		}
	}
	::guiControlFocus(gui, ::llc::CID_INVALID);
	rtrn ::llc::CID_INVALID;
}

sttc ::llc::cid_t guiControlHit(::llc::SGUI & gui, cnst ::llc::SInput & input, ::llc::cid_t iControl) {
	if(::llc::controlInvalid(gui, iControl) || ::llc::controlHidden(gui, iControl))
		rtrn ::llc::CID_INVALID;

	cnst ::llc::vcid children = gui.Controls.Children[iControl];
	for(::llc::u2_t iChild = children.size(); iChild; --iChild) {
		cnst ::llc::cid_t idHovered = ::guiControlHit(gui, input, children[iChild - 1]);
		if(idHovered != ::llc::CID_INVALID)
			rtrn idHovered;
	}

	if(::llc::controlDisabled(gui, iControl) || false == ::llc::in_range(gui.CursorPos.s1(), gui.Controls.Area[iControl].Total.Global))
		rtrn ::llc::CID_INVALID;

	::llc::SControlState & state = gui.Controls.States[iControl];
	gui.Controls.SetHovered(iControl, true);
	if(input.ButtonDown(0) && false == state.IsPressed())
		gui.Controls.SetPressed(iControl, true);
	else if(input.ButtonUp(0))
		gui.Controls.SetPressed(iControl, false);
	rtrn iControl;
}

::llc::error_t llc::guiProcessInput(::llc::SGUI & gui, cnst ::llc::SInput & input, ::llc::vpobj<::llc::SEventSystem> in_Events) {
	rtrn ::llc::guiProcessInput(gui, input, ::llc::vcid{}, in_Events);
}

::llc::error_t llc::guiProcessInput(::llc::SGUI & gui, cnst ::llc::SInput & input, ::llc::vcid controlsKeyboard, ::llc::vpobj<::llc::SEventSystem> in_Events) {
	if(0 == gui.LastSize.LengthSquared())
		rtrn 0;

	(void)in_Events;
	gui.Controls.EventQueue.clear();
	gui.CursorPos = input.MouseCurrent.Position.f2();

	for(::llc::u2_t iControl = 0; iControl < gui.Controls.States.size(); ++iControl) {
		cnst ::llc::cid_t idControl = (::llc::cid_t)iControl;
		if(false == ::llc::controlInvalid(gui, idControl))
			gui.Controls.SetAction(idControl, false);
	}

	::llc::cid_t idHovered = ::llc::CID_INVALID;
	for(::llc::u2_t iControl = gui.Controls.Entities.size(); iControl && idHovered == ::llc::CID_INVALID; --iControl) {
		cnst ::llc::cid_t idRoot = (::llc::cid_t)(iControl - 1);
		if(::llc::controlInvalid(gui, idRoot))
			continue;
		cnst ::llc::cid_t idParent = gui.Controls.Entities[idRoot].Parent;
		if(idParent == ::llc::CID_INVALID || ::llc::controlInvalid(gui, idParent))
			idHovered = ::guiControlHit(gui, input, idRoot);
	}

	for(::llc::u2_t iControl = 0; iControl < gui.Controls.States.size(); ++iControl) {
		cnst ::llc::cid_t idControl = (::llc::cid_t)iControl;
		if(::llc::controlInvalid(gui, idControl) || idControl == idHovered)
			continue;
		gui.Controls.SetHovered(idControl, false);
		if(input.ButtonUp(0))
			gui.Controls.SetPressed(idControl, false);
	}
	if(input.ButtonDown(0) && ::guiControlKeyboardIndex(controlsKeyboard, idHovered) < controlsKeyboard.size())
		::guiControlFocus(gui, idHovered);
	if(input.KeyDown(::llc::SYSTEM_KEY_Tab)) {
		cnst bool reverse = 0 != input.KeyboardCurrent.KeyState[::llc::SYSTEM_KEY_Shift];
		::guiControlFocusMove(gui, controlsKeyboard, reverse);
	}
	if(input.KeyDown(::llc::SYSTEM_KEY_Enter) || input.KeyDown(::llc::SYSTEM_KEY_Space)) {
		cnst ::llc::cid_t idFocused = ::guiControlFocused(gui, controlsKeyboard);
		if(idFocused != ::llc::CID_INVALID)
			gui.Controls.SetAction(idFocused, true);
	}
	rtrn 0;
}
