#include "llc_dialog.h"
#include "llc_gui_inputbox.h"
#include "llc_gui_viewport.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(DIALOG_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, DEFAULT_ROOT				,  0, "A default dialog has an assigned root.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, DEFAULT_FOCUS			,  1, "A default dialog has a focused control.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, DEFAULT_SELECTION		,  2, "A default dialog has a selected control.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, INVALID_PARENT			,  3, "Dialog creation accepted an invalid parent.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, INVALID_SIZE				,  4, "Dialog creation accepted a negative size.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, REJECTED_CONTROL_COUNT	,  5, "Rejected dialog creation changed the live-control count.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, CREATE_RESULT			,  6, "Dialog creation failed.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, CREATE_RETURN			,  7, "Dialog creation did not return its root control.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, ROOT_REFERENCE			,  8, "The dialog root reference is invalid.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, ROOT_PARENT				,  9, "The dialog root has the wrong parent.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, ROOT_SIZE				, 10, "The dialog root has the wrong initial size.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, ROOT_MARGIN				, 11, "The dialog root owns renderer-facing margin geometry.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, ROOT_BORDER				, 12, "The dialog root owns renderer-facing border geometry.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, ROOT_FILL_X				, 13, "The dialog root does not fill its parent horizontally.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, ROOT_FILL_Y				, 14, "The dialog root does not fill its parent vertically.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, REENTRY_RESULT			, 15, "Dialog creation accepted an assigned dialog.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, REENTRY_ROOT				, 16, "Rejected dialog creation changed the root reference.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, CHILD_CREATE				, 17, "Dialog child creation failed.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, CHILD_PARENT				, 18, "A dialog child has the wrong parent.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, NESTED_PARENT			, 19, "A nested dialog control has the wrong parent.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, ROOT_MEMBERSHIP			, 20, "The dialog root is outside its own hierarchy.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, CHILD_MEMBERSHIP			, 21, "A dialog child is outside the dialog hierarchy.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, NESTED_MEMBERSHIP		, 22, "A nested control is outside the dialog hierarchy.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, OUTSIDER_MEMBERSHIP		, 23, "An unrelated control belongs to the dialog hierarchy.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, OUTSIDER_PARENT			, 24, "Dialog child creation accepted an unrelated parent.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, FOCUS_RESULT				, 25, "Dialog focus assignment failed.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, FOCUS_CURRENT			, 26, "The dialog stored the wrong current focus.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, FOCUS_PREVIOUS			, 27, "The dialog stored the wrong previous focus.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, FOCUS_FLAG				, 28, "Dialog focus state is inconsistent with the control table.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, OUTSIDER_FOCUS			, 29, "Dialog focus accepted an unrelated control.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, SELECTION_CURRENT		, 30, "The dialog stored the wrong current selection.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, SELECTION_PREVIOUS		, 31, "The dialog stored the wrong previous selection.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, SELECTION_FLAG			, 32, "Dialog selection state is inconsistent with the control table.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, EVENT_UPDATE				, 33, "Dialog event synchronization failed.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, EVENT_FOCUS				, 34, "A GUI focus event did not update the dialog.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, EVENT_SELECTION			, 35, "A GUI selection event did not update the dialog.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, MALFORMED_RESULT			, 36, "Dialog event synchronization accepted a malformed event.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, MALFORMED_STATE			, 37, "A malformed event partially changed dialog state.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, DELETED_REFERENCE		, 38, "A deleted control reference remained valid.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, REUSED_REFERENCE			, 39, "A reused control slot preserved the old generation.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, REUSED_FOCUS				, 40, "A reused control slot inherited stale dialog focus.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, VIEWPORT_ROOT			, 41, "A viewport root is outside the dialog hierarchy.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, VIEWPORT_TITLE			, 42, "A viewport title is outside the dialog hierarchy.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, VIEWPORT_CLIENT			, 43, "A viewport client is outside the dialog hierarchy.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, INPUT_ROOT				, 44, "An input-box root is outside the dialog hierarchy.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, INPUT_TEXT				, 45, "An input-box text control is outside the dialog hierarchy.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, INPUT_KEYBOARD			, 46, "A virtual keyboard is outside the dialog hierarchy.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, DELETE_RESULT			, 47, "Dialog deletion failed.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, DELETE_ROOT				, 48, "Dialog deletion left its root control alive.");
GDEFINE_ENUM_VALUED(DIALOG_TEST_RESULT, DELETE_STATE				, 49, "Dialog deletion retained form state.");

sttc ::llc::u2_t dialogLiveControlCount(cnst ::llc::SGUI & gui) {
	::llc::u2_t count = 0;
	for(::llc::u2_t iControl = 0; iControl < gui.Controls.States.size(); ++iControl)
		count += false == ::llc::controlInvalid(gui, (::llc::cid_t)iControl);
	rtrn count;
}

sttc ::llc::err_t testDialogCreation(ATestError & errors) {
	::llc::SDialog dialog = {};
	LLC_TEST_CHECK(errors, DIALOG_TEST_RESULT_DEFAULT_ROOT, dialog.Root.Id != ::llc::CID_INVALID);
	LLC_TEST_CHECK(errors, DIALOG_TEST_RESULT_DEFAULT_FOCUS, dialog.FocusedCurrent.Id != ::llc::CID_INVALID || dialog.FocusedPrevious.Id != ::llc::CID_INVALID);
	LLC_TEST_CHECK(errors, DIALOG_TEST_RESULT_DEFAULT_SELECTION, dialog.SelectedCurrent.Id != ::llc::CID_INVALID || dialog.SelectedPrevious.Id != ::llc::CID_INVALID);

	::llc::SGUI gui = {};
	cnst ::llc::cid_t idParent = ::llc::controlCreate(gui);
	cnst ::llc::cid_t idHole = ::llc::controlCreate(gui);
	cnst ::llc::cid_t idOutsider = ::llc::controlCreate(gui);
	if_fail_fe(::llc::controlDelete(gui, idHole));
	cnst ::llc::u2_t liveBeforeRejected = ::dialogLiveControlCount(gui);
	::llc::setupLogCallbacks(0, 0);
	cnst ::llc::err_t invalidParentResult = ::llc::dialogCreate(dialog, gui, {(::llc::cid_t)1234, {10, 10}, true});
	cnst ::llc::err_t invalidSizeResult = ::llc::dialogCreate(dialog, gui, {idParent, {-1, 10}, true});
	::llc::setupDefaultLogCallbacks();
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_INVALID_PARENT, false == ::llc::failed(invalidParentResult), "Result:%i.", invalidParentResult);
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_INVALID_SIZE, false == ::llc::failed(invalidSizeResult), "Result:%i.", invalidSizeResult);
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_REJECTED_CONTROL_COUNT, ::dialogLiveControlCount(gui) != liveBeforeRejected, "Live controls:%u, expected:%u.", ::dialogLiveControlCount(gui), liveBeforeRejected);

	cnst ::llc::SDialogCreate setup = {idParent, {320, 200}, true};
	cnst ::llc::err_t createResult = ::llc::dialogCreate(dialog, gui, setup);
	LLC_TEST_REQUIREF(errors, DIALOG_TEST_RESULT_CREATE_RESULT, ::llc::failed(createResult), "Result:%i.", createResult);
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_CREATE_RETURN, createResult != dialog.Root.Id, "Result:%i, root:%i.", createResult, dialog.Root.Id);
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_CREATE_RETURN, dialog.Root.Id != idHole, "Root:%i, expected reused slot:%i.", dialog.Root.Id, idHole);
	LLC_TEST_CHECK(errors, DIALOG_TEST_RESULT_ROOT_REFERENCE, false == ::llc::controlReferenceValid(gui, dialog.Root));
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_ROOT_PARENT, gui.Controls.Entities[dialog.Root.Id].Parent != idParent, "Parent:%i, expected:%i.", gui.Controls.Entities[dialog.Root.Id].Parent, idParent);
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_ROOT_SIZE, gui.Controls.Placement[dialog.Root.Id].Area.Size != setup.Size, "Size:{%i, %i}, expected:{%i, %i}.", gui.Controls.Placement[dialog.Root.Id].Area.Size.x, gui.Controls.Placement[dialog.Root.Id].Area.Size.y, setup.Size.x, setup.Size.y);
	LLC_TEST_CHECK(errors, DIALOG_TEST_RESULT_ROOT_MARGIN, gui.Controls.Placement[dialog.Root.Id].Margin != ::llc::rectu1_t{});
	LLC_TEST_CHECK(errors, DIALOG_TEST_RESULT_ROOT_BORDER, gui.Controls.Placement[dialog.Root.Id].Border != ::llc::rectu1_t{});
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_ROOT_FILL_X, gui.Controls.Constraints[dialog.Root.Id].AttachSizeToControl.x != idParent, "Target:%i, expected:%i.", gui.Controls.Constraints[dialog.Root.Id].AttachSizeToControl.x, idParent);
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_ROOT_FILL_Y, gui.Controls.Constraints[dialog.Root.Id].AttachSizeToControl.y != idParent, "Target:%i, expected:%i.", gui.Controls.Constraints[dialog.Root.Id].AttachSizeToControl.y, idParent);
	cnst ::llc::SControlReference rootBeforeReentry = dialog.Root;
	::llc::setupLogCallbacks(0, 0);
	cnst ::llc::err_t reentryResult = ::llc::dialogCreate(dialog, gui, setup);
	::llc::setupDefaultLogCallbacks();
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_REENTRY_RESULT, false == ::llc::failed(reentryResult), "Result:%i.", reentryResult);
	LLC_TEST_CHECK(errors, DIALOG_TEST_RESULT_REENTRY_ROOT, dialog.Root != rootBeforeReentry);

	cnst ::llc::cid_t idChild = ::llc::dialogControlCreate(dialog, gui);
	cnst ::llc::cid_t idNested = ::llc::dialogControlCreate(dialog, gui, idChild);
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_CHILD_CREATE, idChild < 0 || idNested < 0, "Child:%i, nested:%i.", idChild, idNested);
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_CHILD_PARENT, gui.Controls.Entities[idChild].Parent != dialog.Root.Id, "Parent:%i, expected:%i.", gui.Controls.Entities[idChild].Parent, dialog.Root.Id);
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_NESTED_PARENT, gui.Controls.Entities[idNested].Parent != idChild, "Parent:%i, expected:%i.", gui.Controls.Entities[idNested].Parent, idChild);
	LLC_TEST_CHECK(errors, DIALOG_TEST_RESULT_ROOT_MEMBERSHIP, false == ::llc::dialogControlBelongs(dialog, gui, dialog.Root.Id));
	LLC_TEST_CHECK(errors, DIALOG_TEST_RESULT_CHILD_MEMBERSHIP, false == ::llc::dialogControlBelongs(dialog, gui, idChild));
	LLC_TEST_CHECK(errors, DIALOG_TEST_RESULT_NESTED_MEMBERSHIP, false == ::llc::dialogControlBelongs(dialog, gui, idNested));
	LLC_TEST_CHECK(errors, DIALOG_TEST_RESULT_OUTSIDER_MEMBERSHIP, ::llc::dialogControlBelongs(dialog, gui, idOutsider));
	::llc::setupLogCallbacks(0, 0);
	cnst ::llc::cid_t invalidChild = ::llc::dialogControlCreate(dialog, gui, idOutsider);
	::llc::setupDefaultLogCallbacks();
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_OUTSIDER_PARENT, 0 <= invalidChild, "Control:%i.", invalidChild);
	rtrn 0;
}

sttc ::llc::err_t testDialogState(ATestError & errors) {
	::llc::SGUI gui = {};
	::llc::SDialog dialog = {};
	if_fail_fe(::llc::dialogCreate(dialog, gui));
	cnst ::llc::cid_t idFirst = ::llc::dialogControlCreate(dialog, gui);
	cnst ::llc::cid_t idSecond = ::llc::dialogControlCreate(dialog, gui);
	cnst ::llc::cid_t idOutsider = ::llc::controlCreate(gui);
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_FOCUS_RESULT, ::llc::failed(::llc::dialogFocus(dialog, gui, idFirst)), "Control:%i.", idFirst);
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_FOCUS_CURRENT, dialog.FocusedCurrent.Id != idFirst, "Current:%i, expected:%i.", dialog.FocusedCurrent.Id, idFirst);
	LLC_TEST_CHECK(errors, DIALOG_TEST_RESULT_FOCUS_FLAG, false == gui.Controls.IsFocused(idFirst));
	if_fail_fe(::llc::dialogFocus(dialog, gui, idSecond));
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_FOCUS_CURRENT, dialog.FocusedCurrent.Id != idSecond, "Current:%i, expected:%i.", dialog.FocusedCurrent.Id, idSecond);
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_FOCUS_PREVIOUS, dialog.FocusedPrevious.Id != idFirst, "Previous:%i, expected:%i.", dialog.FocusedPrevious.Id, idFirst);
	LLC_TEST_CHECK(errors, DIALOG_TEST_RESULT_FOCUS_FLAG, gui.Controls.IsFocused(idFirst) || false == gui.Controls.IsFocused(idSecond));
	cnst ::llc::SDialog beforeOutsider = dialog;
	::llc::setupLogCallbacks(0, 0);
	cnst ::llc::err_t outsiderFocusResult = ::llc::dialogFocus(dialog, gui, idOutsider);
	::llc::setupDefaultLogCallbacks();
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_OUTSIDER_FOCUS, false == ::llc::failed(outsiderFocusResult), "Result:%i.", outsiderFocusResult);
	LLC_TEST_CHECK(errors, DIALOG_TEST_RESULT_OUTSIDER_FOCUS, dialog.FocusedCurrent != beforeOutsider.FocusedCurrent || dialog.FocusedPrevious != beforeOutsider.FocusedPrevious);
	if_fail_fe(::llc::dialogSelect(dialog, gui, idFirst));
	if_fail_fe(::llc::dialogSelect(dialog, gui, idSecond));
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_SELECTION_CURRENT, dialog.SelectedCurrent.Id != idSecond, "Current:%i, expected:%i.", dialog.SelectedCurrent.Id, idSecond);
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_SELECTION_PREVIOUS, dialog.SelectedPrevious.Id != idFirst, "Previous:%i, expected:%i.", dialog.SelectedPrevious.Id, idFirst);
	LLC_TEST_CHECK(errors, DIALOG_TEST_RESULT_SELECTION_FLAG, gui.Controls.IsSelected(idFirst) || false == gui.Controls.IsSelected(idSecond));
	if_fail_fe(::llc::dialogFocus(dialog, gui, ::llc::CID_INVALID));
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_FOCUS_CURRENT, dialog.FocusedCurrent.Id != ::llc::CID_INVALID, "Current:%i, expected invalid.", dialog.FocusedCurrent.Id);
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_FOCUS_PREVIOUS, dialog.FocusedPrevious.Id != idSecond, "Previous:%i, expected:%i.", dialog.FocusedPrevious.Id, idSecond);
	rtrn 0;
}

sttc ::llc::err_t testDialogEventsAndGenerations(ATestError & errors) {
	::llc::SGUI gui = {};
	::llc::SDialog dialog = {};
	if_fail_fe(::llc::dialogCreate(dialog, gui));
	::llc::cid_t idControl = ::llc::dialogControlCreate(dialog, gui);
	gui.Controls.EventQueue.clear();
	gui.Controls.SetFocused(idControl, true);
	gui.Controls.SetSelected(idControl, true);
	cnst ::llc::err_t updateResult = ::llc::dialogUpdate(dialog, gui);
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_EVENT_UPDATE, ::llc::failed(updateResult), "Result:%i.", updateResult);
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_EVENT_FOCUS, dialog.FocusedCurrent.Id != idControl, "Current:%i, expected:%i.", dialog.FocusedCurrent.Id, idControl);
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_EVENT_SELECTION, dialog.SelectedCurrent.Id != idControl, "Current:%i, expected:%i.", dialog.SelectedCurrent.Id, idControl);

	::llc::TQueueSystemEvent malformedEvents = {};
	::llc::PEventSystem malformed = {};
	malformed->Type = ::llc::SYSTEM_EVENT_GUI;
	if_fail_fe(::llc::eventWrapChild(*malformed, ::llc::EVENT_GUI_CONTROL_StateChange, ::llc::vcu0_t{}));
	if_fail_fe(malformedEvents.push_back(malformed));
	cnst ::llc::SDialog beforeMalformed = dialog;
	::llc::setupLogCallbacks(0, 0);
	cnst ::llc::err_t malformedResult = ::llc::dialogUpdate(dialog, gui, malformedEvents);
	::llc::setupDefaultLogCallbacks();
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_MALFORMED_RESULT, false == ::llc::failed(malformedResult), "Result:%i.", malformedResult);
	LLC_TEST_CHECK(errors, DIALOG_TEST_RESULT_MALFORMED_STATE, dialog.Root != beforeMalformed.Root || dialog.FocusedCurrent != beforeMalformed.FocusedCurrent || dialog.FocusedPrevious != beforeMalformed.FocusedPrevious || dialog.SelectedCurrent != beforeMalformed.SelectedCurrent || dialog.SelectedPrevious != beforeMalformed.SelectedPrevious);

	gui.Controls.EventQueue.clear();
	for(::llc::u2_t iPass = 0; iPass < 4096; ++iPass) {
		cnst ::llc::SControlReference oldReference = ::llc::controlReference(gui, idControl);
		if_fail_fe(::llc::dialogFocus(dialog, gui, idControl));
		if_fail_fe(::llc::controlDelete(gui, idControl));
		idControl = ::llc::dialogControlCreate(dialog, gui);
		LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_DELETED_REFERENCE, ::llc::controlReferenceValid(gui, oldReference), "Pass:%u, id:%i, generation:%u.", iPass, oldReference.Id, oldReference.Generation);
		LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_REUSED_REFERENCE, idControl != oldReference.Id || gui.Controls.Entities[idControl].Generation == oldReference.Generation, "Pass:%u, old:{%i, %u}, new:{%i, %u}.", iPass, oldReference.Id, oldReference.Generation, idControl, gui.Controls.Entities[idControl].Generation);
		gui.Controls.EventQueue.clear();
		if_fail_fe(::llc::dialogUpdate(dialog, gui, {}));
		LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_REUSED_FOCUS, dialog.FocusedCurrent.Id != ::llc::CID_INVALID, "Pass:%u, focus:{%i, %u}.", iPass, dialog.FocusedCurrent.Id, dialog.FocusedCurrent.Generation);
	}
	rtrn 0;
}

sttc ::llc::err_t testDialogComposition(ATestError & errors) {
	::llc::SGUI gui = {};
	::llc::SDialog dialog = {};
	if_fail_fe(::llc::dialogCreate(dialog, gui));
	::llc::SViewport viewport = {};
	::llc::SViewportCreate viewportSetup = {};
	viewportSetup.Parent = dialog.Root.Id;
	if_fail_fe(::llc::viewportCreate(viewport, gui, viewportSetup));
	LLC_TEST_CHECK(errors, DIALOG_TEST_RESULT_VIEWPORT_ROOT, false == ::llc::dialogControlBelongs(dialog, gui, viewport.IdRoot));
	LLC_TEST_CHECK(errors, DIALOG_TEST_RESULT_VIEWPORT_TITLE, false == ::llc::dialogControlBelongs(dialog, gui, viewport.IdTitle));
	LLC_TEST_CHECK(errors, DIALOG_TEST_RESULT_VIEWPORT_CLIENT, false == ::llc::dialogControlBelongs(dialog, gui, viewport.IdClient));
	::llc::SInputBox inputBox = {};
	if_fail_fe(::llc::inputBoxCreate(inputBox, gui, viewport.IdClient));
	LLC_TEST_CHECK(errors, DIALOG_TEST_RESULT_INPUT_ROOT, false == ::llc::dialogControlBelongs(dialog, gui, inputBox.IdRoot));
	LLC_TEST_CHECK(errors, DIALOG_TEST_RESULT_INPUT_TEXT, false == ::llc::dialogControlBelongs(dialog, gui, inputBox.IdText));
	LLC_TEST_CHECK(errors, DIALOG_TEST_RESULT_INPUT_KEYBOARD, false == ::llc::dialogControlBelongs(dialog, gui, inputBox.VirtualKeyboard.IdRoot));
	cnst ::llc::cid_t idRoot = dialog.Root.Id;
	cnst ::llc::err_t deleteResult = ::llc::dialogDelete(dialog, gui);
	LLC_TEST_CHECKF(errors, DIALOG_TEST_RESULT_DELETE_RESULT, ::llc::failed(deleteResult), "Result:%i.", deleteResult);
	LLC_TEST_CHECK(errors, DIALOG_TEST_RESULT_DELETE_ROOT, false == ::llc::controlInvalid(gui, idRoot));
	LLC_TEST_CHECK(errors, DIALOG_TEST_RESULT_DELETE_STATE, dialog.Root.Id != ::llc::CID_INVALID || dialog.FocusedCurrent.Id != ::llc::CID_INVALID || dialog.SelectedCurrent.Id != ::llc::CID_INVALID);
	rtrn 0;
}

::llc::err_t testDialog(ATestError & errors) {
	if_fail_fe(::testDialogCreation(errors));
	if_fail_fe(::testDialogState(errors));
	if_fail_fe(::testDialogEventsAndGenerations(errors));
	rtrn ::testDialogComposition(errors);
}
