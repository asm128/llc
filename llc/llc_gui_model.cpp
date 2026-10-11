#include "llc_gui.h"

::llc::error_t llc::controlInvalid(cnst ::llc::SGUI & gui, ::llc::cid_t iControl) {
	rtrn ::llc::controlInvalid(gui.Controls, iControl);
}

::llc::cid_t llc::controlCreate(::llc::SGUI & gui) {
	rtrn ::llc::controlCreate(gui.Controls, gui.DefaultColors);
}

::llc::cid_t llc::controlCreateChild(::llc::SGUI & gui, ::llc::cid_t iParent) {
	if_true_fe(::llc::controlInvalid(gui, iParent));
	cnst ::llc::cid_t iControl = ::llc::controlCreate(gui);
	if_true_fe(iControl < 0);
	if(::llc::failed(::llc::controlSetParent(gui, iControl, iParent))) {
		::llc::controlDelete(gui, iControl);
		rtrn -1;
	}
	rtrn iControl;
}

::llc::error_t llc::controlDelete(::llc::SGUI & gui, ::llc::cid_t iControl, bool recursive) {
	rtrn ::llc::controlDelete(gui.Controls, iControl, recursive);
}

::llc::error_t llc::controlSetParent(::llc::SGUI & gui, ::llc::cid_t iControl, ::llc::cid_t iParent) {
	rtrn ::llc::controlSetParent(gui.Controls, iControl, iParent);
}

::llc::error_t llc::controlTextSet(::llc::SGUI & gui, ::llc::cid_t iControl, cnst ::llc::vcst_t & text) {
	if_true_fe(::llc::controlInvalid(gui, iControl));
	::llc::vcst_t & oldText = gui.Controls.Text[iControl].Text;
	if(text == oldText)
		rtrn 0;
	oldText = text;
	gui.Controls.SetUpdated(iControl, false);
	rtrn 0;
}

::llc::error_t llc::controlFontSet(::llc::SGUI & gui, ::llc::cid_t iControl, ::llc::s1_t iFont) {
	if_true_fe(::llc::controlInvalid(gui, iControl));
	::llc::s1_t & oldFont = gui.Controls.Text[iControl].FontSelected;
	if(iFont == oldFont)
		rtrn 0;
	oldFont = iFont;
	gui.Controls.SetUpdated(iControl, false);
	rtrn 0;
}

sttc ::llc::error_t controlInheritedFlag(cnst ::llc::SGUI & gui, ::llc::cid_t iControl, ::llc::GUI_CONTROL_FLAG flag) {
	for(::llc::u2_t iDepth = 0; iControl != ::llc::CID_INVALID && iDepth < gui.Controls.Entities.size(); ++iDepth) {
		if(::llc::controlInvalid(gui, iControl))
			rtrn 1;
		cnst ::llc::SControlState & state = gui.Controls.States[iControl];
		if(::llc::bit_test_masked(state.Flag, state.Mask, flag))
			rtrn 1;
		iControl = gui.Controls.Entities[iControl].Parent;
	}
	rtrn 0;
}

::llc::error_t llc::controlHidden(cnst ::llc::SGUI & gui, ::llc::cid_t iControl) {
	rtrn ::controlInheritedFlag(gui, iControl, ::llc::GUI_CONTROL_FLAG_Hidden);
}

::llc::error_t llc::controlDisabled(cnst ::llc::SGUI & gui, ::llc::cid_t iControl) {
	rtrn ::controlInheritedFlag(gui, iControl, ::llc::GUI_CONTROL_FLAG_Disabled);
}

::llc::error_t llc::controlMetricsInvalidate(::llc::SGUI & gui, ::llc::cid_t iControl) {
	if_true_fe(::llc::controlInvalid(gui, iControl));
	gui.Controls.SetUpdated(iControl, false);
	cnst ::llc::vcid children = gui.Controls.Children[iControl];
	for(::llc::u2_t iChild = 0; iChild < children.size(); ++iChild)
		if_fail_fe(::llc::controlMetricsInvalidate(gui, children[iChild]));
	rtrn 0;
}

::llc::error_t llc::guiDeselect(::llc::SGUI & gui) {
	for(::llc::u2_t iControl = 0; iControl < gui.Controls.States.size(); ++iControl) {
		cnst ::llc::cid_t idControl = (::llc::cid_t)iControl;
		if(::llc::controlInvalid(gui, idControl))
			continue;
		gui.Controls.SetSelected(idControl, false);
	}
	rtrn 0;
}

::llc::error_t llc::guiSelect(::llc::SGUI & gui, ::llc::cid_t controlToSelect) {
	if_true_fe(controlToSelect != ::llc::CID_INVALID && ::llc::controlInvalid(gui, controlToSelect));
	for(::llc::u2_t iControl = 0; iControl < gui.Controls.States.size(); ++iControl) {
		cnst ::llc::cid_t idControl = (::llc::cid_t)iControl;
		if(::llc::controlInvalid(gui, idControl))
			continue;
		gui.Controls.SetSelected(idControl, controlToSelect == idControl);
	}
	rtrn 0;
}

::llc::error_t llc::guiGetProcessableControls(cnst ::llc::SGUI & gui, ::llc::acid & controlIndices) {
	for(::llc::u2_t iControl = 0; iControl < gui.Controls.States.size(); ++iControl) {
		cnst ::llc::cid_t idControl = (::llc::cid_t)iControl;
		if(::llc::controlInvalid(gui, idControl) || ::llc::controlDisabled(gui, idControl) || ::llc::controlHidden(gui, idControl))
			continue;
		if_fail_fe(controlIndices.push_back(idControl));
	}
	rtrn controlIndices.size();
}

::llc::error_t llc::guiProcessControls(cnst ::llc::SGUI & gui, cnst ::llc::function<::llc::error_t(::llc::cid_t iControl)> & funcOnExecute) {
	::llc::acid controlsToProcess = {};
	if_fail_fe(::llc::guiGetProcessableControls(gui, controlsToProcess));
	rtrn ::llc::guiProcessControls(gui, controlsToProcess, funcOnExecute);
}

::llc::error_t llc::guiProcessControls(cnst ::llc::SGUI & gui, ::llc::vcid controlsToProcess, cnst ::llc::function<::llc::error_t(::llc::cid_t iControl)> & funcOnExecute) {
	for(::llc::u2_t iControl = 0; iControl < controlsToProcess.size(); ++iControl) {
		cnst ::llc::cid_t idControl = controlsToProcess[iControl];
		if_true_fe(::llc::controlInvalid(gui, idControl));
		if(gui.Controls.States[idControl].IsAction())
			rtrn funcOnExecute(idControl);
	}
	rtrn 0;
}
