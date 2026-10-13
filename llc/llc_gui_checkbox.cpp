#include "llc_gui_checkbox.h"

sttc ::llc::err_t checkBoxValidate(cnst ::llc::SCheckBox & checkBox, cnst ::llc::SGUI & gui) {
	if_true_fef(::llc::controlInvalid(gui, checkBox.IdControl), "Invalid check-box control:%i.", checkBox.IdControl);
	rtrn 0;
}

::llc::err_t llc::checkBoxSetChecked(::llc::SCheckBox & checkBox, ::llc::SGUI & gui, bool checked) {
	if_fail_fe(::checkBoxValidate(checkBox, gui));
	cnst bool changed = checkBox.Checked != checked;
	if(false == changed)
		rtrn 0;
	cnst ::llc::vcst_t mark = checked ? ::llc::vcst_t{checkBox.Mark.Storage, checkBox.MarkLength} : ::llc::vcst_t{};
	if_fail_fe(::llc::controlTextSet(gui, checkBox.IdControl, mark));
	checkBox.Checked = checked;
	gui.Controls.SetSelected(checkBox.IdControl, checked);
	rtrn 1;
}

::llc::err_t llc::checkBoxCreate(::llc::SCheckBox & checkBox, ::llc::SGUI & gui, cnst ::llc::SCheckBoxCreate & setup) {
	if_true_fef(checkBox.IdControl != ::llc::CID_INVALID, "Check-box control already assigned:%i.", checkBox.IdControl);
	if_true_fef(setup.Parent != ::llc::CID_INVALID && ::llc::controlInvalid(gui, setup.Parent), "Invalid check-box parent:%i.", setup.Parent);
	if_true_fef(setup.Area.Size.x <= 0 || setup.Area.Size.y <= 0, "Invalid check-box size:{%i, %i}.", setup.Area.Size.x, setup.Area.Size.y);
	if_true_fef(setup.Text.size() >= checkBox.Mark.size(), "Check-box mark length:%u exceeds capacity:%u.", setup.Text.size(), checkBox.Mark.size());

	checkBox.IdControl = ::llc::controlCreate(gui);
	if_true_fef(checkBox.IdControl < 0, "Failed to create check-box control:%i.", checkBox.IdControl);
	if(setup.Parent != ::llc::CID_INVALID && ::llc::failed(::llc::controlSetParent(gui, checkBox.IdControl, setup.Parent))) {
		::llc::controlDelete(gui, checkBox.IdControl);
		checkBox = {};
		rtrn -1;
	}

	::llc::SControlPlacement & placement = gui.Controls.Placement[checkBox.IdControl];
	placement.Area = setup.Area;
	placement.Margin = {1, 1, 1, 1};
	placement.Border = {2, 2, 2, 2};
	gui.Controls.Text[checkBox.IdControl].Align = ::llc::ALIGN_CENTER;
	for(::llc::u0_t iCharacter = 0; iCharacter < setup.Text.size(); ++iCharacter)
		checkBox.Mark[iCharacter] = setup.Text[iCharacter];
	checkBox.MarkLength = (::llc::u0_t)setup.Text.size();
	cnst ::llc::err_t checkedResult = ::llc::checkBoxSetChecked(checkBox, gui, setup.Checked);
	if(::llc::failed(checkedResult)) {
		::llc::controlDelete(gui, checkBox.IdControl);
		checkBox = {};
		rtrn checkedResult;
	}
	rtrn checkBox.IdControl;
}

::llc::err_t llc::checkBoxUpdate(::llc::SCheckBox & checkBox, ::llc::SGUI & gui) {
	if_fail_fe(::checkBoxValidate(checkBox, gui));
	if(false == gui.Controls.IsAction(checkBox.IdControl))
		rtrn 0;
	rtrn ::llc::checkBoxSetChecked(checkBox, gui, false == checkBox.Checked);
}
