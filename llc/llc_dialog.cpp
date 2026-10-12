#include "llc_dialog.h"

::llc::SControlReference llc::controlReference(cnst ::llc::SGUI & gui, ::llc::cid_t iControl) nxpt {
	if(::llc::controlInvalid(gui, iControl))
		rtrn {};
	rtrn {iControl, gui.Controls.Entities[iControl].Generation};
}

bool llc::controlReferenceValid(cnst ::llc::SGUI & gui, cnst ::llc::SControlReference & reference) nxpt {
	rtrn false == ::llc::controlInvalid(gui, reference.Id)
		&& gui.Controls.Entities[reference.Id].Generation == reference.Generation
		;
}

bool llc::dialogControlBelongs(cnst ::llc::SDialog & dialog, cnst ::llc::SGUI & gui, ::llc::cid_t iControl) nxpt {
	if(false == ::llc::controlReferenceValid(gui, dialog.Root) || ::llc::controlInvalid(gui, iControl))
		rtrn false;
	for(::llc::u2_t iDepth = 0; iControl != ::llc::CID_INVALID && iDepth < gui.Controls.Entities.size(); ++iDepth) {
		if(iControl == dialog.Root.Id)
			rtrn true;
		if(::llc::controlInvalid(gui, iControl))
			rtrn false;
		iControl = gui.Controls.Entities[iControl].Parent;
	}
	rtrn false;
}

sttc void dialogReferenceSanitize(::llc::SControlReference & reference, cnst ::llc::SGUI & gui) nxpt {
	if(false == ::llc::controlReferenceValid(gui, reference))
		reference = {};
}

sttc void dialogReferencesSanitize(::llc::SDialog & dialog, cnst ::llc::SGUI & gui) nxpt {
	::dialogReferenceSanitize(dialog.FocusedCurrent, gui);
	::dialogReferenceSanitize(dialog.FocusedPrevious, gui);
	::dialogReferenceSanitize(dialog.SelectedCurrent, gui);
	::dialogReferenceSanitize(dialog.SelectedPrevious, gui);
}

sttc void dialogReferenceSet(::llc::SControlReference & current, ::llc::SControlReference & previous, cnst ::llc::SControlReference & reference) nxpt {
	if(current == reference)
		rtrn;
	previous = current;
	current = reference;
}

::llc::err_t llc::dialogCreate(::llc::SDialog & dialog, ::llc::SGUI & gui, cnst ::llc::SDialogCreate & setup) {
	if_true_fef(dialog.Root.Id != ::llc::CID_INVALID, "Dialog root already assigned:%i.", dialog.Root.Id);
	if_true_fef(setup.Parent != ::llc::CID_INVALID && ::llc::controlInvalid(gui, setup.Parent), "Invalid dialog parent:%i.", setup.Parent);
	if_true_fef(setup.Size.x < 0 || setup.Size.y < 0, "Invalid dialog size:{%i, %i}.", setup.Size.x, setup.Size.y);

	cnst ::llc::cid_t idRoot = ::llc::controlCreate(gui);
	if_true_fef(idRoot < 0, "Failed to create dialog root:%i.", idRoot);
	if(setup.Parent != ::llc::CID_INVALID && ::llc::failed(::llc::controlSetParent(gui, idRoot, setup.Parent))) {
		::llc::controlDelete(gui, idRoot);
		rtrn -1;
	}

	::llc::SControlPlacement & placement = gui.Controls.Placement[idRoot];
	placement.Area.Size = setup.Size;
	placement.Margin = {};
	placement.Border = {};
	placement.Align = ::llc::ALIGN_TOP_LEFT;
	if(setup.Fill) {
		cnst ::llc::cid_t idSizeTarget = setup.Parent == ::llc::CID_INVALID ? idRoot : setup.Parent;
		gui.Controls.Constraints[idRoot].AttachSizeToControl = {idSizeTarget, idSizeTarget};
	}
	dialog = {};
	dialog.Root = ::llc::controlReference(gui, idRoot);
	rtrn idRoot;
}

::llc::err_t llc::dialogDelete(::llc::SDialog & dialog, ::llc::SGUI & gui) {
	if(::llc::controlReferenceValid(gui, dialog.Root))
		if_fail_fe(::llc::controlDelete(gui, dialog.Root.Id, true));
	dialog = {};
	rtrn 0;
}

::llc::cid_t llc::dialogControlCreate(::llc::SDialog & dialog, ::llc::SGUI & gui, ::llc::cid_t iParent) {
	if_true_fe(false == ::llc::controlReferenceValid(gui, dialog.Root));
	if(iParent == ::llc::CID_INVALID)
		iParent = dialog.Root.Id;
	if_true_fef(false == ::llc::dialogControlBelongs(dialog, gui, iParent), "Control parent:%i does not belong to dialog root:%i.", iParent, dialog.Root.Id);
	rtrn ::llc::controlCreateChild(gui, iParent);
}

sttc ::llc::err_t dialogStateSet(::llc::SDialog & dialog, ::llc::SGUI & gui, ::llc::cid_t iControl, bool focus) {
	if_true_fe(false == ::llc::controlReferenceValid(gui, dialog.Root));
	if_true_fef(iControl != ::llc::CID_INVALID && false == ::llc::dialogControlBelongs(dialog, gui, iControl), "Control:%i does not belong to dialog root:%i.", iControl, dialog.Root.Id);
	::dialogReferencesSanitize(dialog, gui);
	::llc::SControlReference & current = focus ? dialog.FocusedCurrent : dialog.SelectedCurrent;
	::llc::SControlReference & previous = focus ? dialog.FocusedPrevious : dialog.SelectedPrevious;
	cnst ::llc::SControlReference reference = ::llc::controlReference(gui, iControl);
	if(current == reference)
		rtrn 0;
	if(::llc::controlReferenceValid(gui, current)) {
		if(focus)
			gui.Controls.SetFocused(current.Id, false);
		else
			gui.Controls.SetSelected(current.Id, false);
	}
	::dialogReferenceSet(current, previous, reference);
	if(::llc::controlReferenceValid(gui, current)) {
		if(focus)
			gui.Controls.SetFocused(current.Id, true);
		else
			gui.Controls.SetSelected(current.Id, true);
	}
	rtrn 0;
}

::llc::err_t llc::dialogFocus(::llc::SDialog & dialog, ::llc::SGUI & gui, ::llc::cid_t iControl) {
	rtrn ::dialogStateSet(dialog, gui, iControl, true);
}

::llc::err_t llc::dialogSelect(::llc::SDialog & dialog, ::llc::SGUI & gui, ::llc::cid_t iControl) {
	rtrn ::dialogStateSet(dialog, gui, iControl, false);
}

sttc void dialogStateApply(::llc::SControlReference & current, ::llc::SControlReference & previous, cnst ::llc::SControlReference & reference, ::llc::GUI_CONTROL_FLAG clear, ::llc::GUI_CONTROL_FLAG set, ::llc::GUI_CONTROL_FLAG flag) nxpt {
	if(::llc::bit_true(set, flag))
		::dialogReferenceSet(current, previous, reference);
	else if(::llc::bit_true(clear, flag) && current == reference)
		::dialogReferenceSet(current, previous, {});
}

::llc::err_t llc::dialogUpdate(::llc::SDialog & dialog, cnst ::llc::SGUI & gui, ::llc::view<cnst ::llc::pobj<::llc::SEventSystem>> events) {
	if_true_fe(false == ::llc::controlReferenceValid(gui, dialog.Root));
	::llc::SDialog next = dialog;
	::dialogReferencesSanitize(next, gui);
	for(::llc::u2_t iEvent = 0; iEvent < events.size(); ++iEvent) {
		cnst ::llc::pobj<::llc::SEventSystem> & eventSystem = events[iEvent];
		if(0 == eventSystem.get_ref() || eventSystem->Type != ::llc::SYSTEM_EVENT_GUI)
			continue;
		::llc::SEView<::llc::EVENT_GUI_CONTROL> eventGUI = {};
		if_fail_fe(eventSystem->ExtractChild(eventGUI));
		if(eventGUI.Type != ::llc::EVENT_GUI_CONTROL_StateChange)
			continue;
		::llc::vcu0_t input = eventGUI.Data;
		::llc::SChangeControlState stateChange = {};
		if_fail_fe(::llc::loadPOD(input, stateChange));
		if_true_fef(input.size(), "Unexpected GUI state-change payload tail:%u.", input.size());
		if(false == ::llc::dialogControlBelongs(next, gui, stateChange.Id))
			continue;
		cnst ::llc::SControlReference reference = ::llc::controlReference(gui, stateChange.Id);
		::dialogStateApply(next.FocusedCurrent, next.FocusedPrevious, reference, stateChange.Clear, stateChange.Set, ::llc::GUI_CONTROL_FLAG_Focused);
		::dialogStateApply(next.SelectedCurrent, next.SelectedPrevious, reference, stateChange.Clear, stateChange.Set, ::llc::GUI_CONTROL_FLAG_Selected);
	}
	::dialogReferencesSanitize(next, gui);
	dialog = next;
	rtrn 0;
}

::llc::err_t llc::dialogUpdate(::llc::SDialog & dialog, cnst ::llc::SGUI & gui) {
	rtrn ::llc::dialogUpdate(dialog, gui, {gui.Controls.EventQueue.begin(), gui.Controls.EventQueue.size()});
}
