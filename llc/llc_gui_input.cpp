#include "llc_gui.h"

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
	if(0 == gui.LastSize.LengthSquared())
		rtrn 0;

	(void)in_Events;
	gui.Controls.EventQueue.clear();
	if_fail_fe(::llc::guiUpdateMetrics(gui, gui.LastSize, false));
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
	rtrn 0;
}
