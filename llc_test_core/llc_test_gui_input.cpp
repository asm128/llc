#include "llc_gui.h"
#include "llc_system_key.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(GUI_INPUT_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, CREATE_RESULT			,  0, "The GUI input test could not create a control.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, METRICS_RESULT		,  1, "The GUI input test could not update control metrics.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, PROCESS_RESULT		,  2, "GUI input processing reported failure for valid input.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, TOPMOST_HOVER		,  3, "GUI input did not hover the topmost overlapping control.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, COVERED_HOVER		,  4, "GUI input hovered a control covered by another control.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, DISABLED_HOVER		,  5, "GUI input hovered a disabled control.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, FALLTHROUGH_HOVER	,  6, "A disabled control prevented an eligible control behind it from being hovered.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, PRESS_STATE			,  7, "A primary-button press did not set the hovered control's pressed state.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, PRESS_ACTION		,  8, "A primary-button press produced an action before release.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, RELEASE_STATE		,  9, "A primary-button release retained the control's pressed state.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, RELEASE_ACTION		, 10, "A primary-button release over the pressed control did not produce an action.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, ACTION_CLEAR			, 11, "GUI input retained an action beyond the frame that produced it.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, DRAG_HOVER			, 12, "Dragging outside retained the control's hovered state.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, DRAG_PRESS			, 13, "Dragging outside prematurely cleared the control's pressed state.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, OUTSIDE_ACTION		, 14, "Releasing outside the pressed control produced an action.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, DISPLAYED_METRICS		, 15, "GUI input replaced the displayed control metrics with unpublished placement data.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, DISPLAYED_HOVER		, 16, "GUI input did not use the displayed control metrics for hit testing.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, PENDING_METRICS		, 17, "GUI input consumed a pending control metric update.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, INPUT_MEASURE_COUNT	, 18, "GUI input requested text measurement for unpublished control data.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, PUBLISHED_METRICS		, 19, "The font-aware layout pass did not publish pending control metrics.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, PUBLISHED_MEASURE_COUNT, 20, "The font-aware layout pass did not measure the pending control text exactly once.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, TAB_FOCUS				, 21, "Tab did not focus the first keyboard control.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, TAB_CYCLE				, 22, "Tab did not cycle keyboard focus.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, TAB_SKIP_DISABLED		, 23, "Tab focused a disabled keyboard control.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, SHIFT_TAB_CYCLE		, 24, "Shift+Tab did not cycle keyboard focus backwards.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, KEYBOARD_ACTION		, 25, "Enter did not activate the focused control.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, KEYBOARD_ACTION_CLEAR	, 26, "Keyboard activation remained set after its input frame.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, SPACE_ACTION			, 27, "Space did not activate the focused control.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, MOUSE_FOCUS			, 28, "Mouse press did not synchronize keyboard focus.");
GDEFINE_ENUM_VALUED(GUI_INPUT_TEST_RESULT, MOUSE_CHILD_FOCUS		, 29, "Mouse press on a composite child did not focus its keyboard-listed owner.");

sttc ::llc::err_t testGUIInputProcess(ATestError & errors) {
	::llc::SGUI gui = {};
	cnst ::llc::cid_t idBack = ::llc::controlCreate(gui);
	cnst ::llc::cid_t idFront = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_CREATE_RESULT, idBack < 0, "Back control creation result:%i.", idBack);
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_CREATE_RESULT, idFront < 0, "Front control creation result:%i.", idFront);
	gui.Controls.Placement[idBack].Area = {{10, 10}, {60, 40}};
	gui.Controls.Placement[idFront].Area = {{10, 10}, {60, 40}};
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_METRICS_RESULT, 0 > ::llc::guiUpdateMetrics(gui, {100, 80}, true), "%s", "Initial metric update failed.");

	::llc::SInput input = {};
	input.MouseCurrent.Position = {20, 20};
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_PROCESS_RESULT, 0 > ::llc::guiProcessInput(gui, input), "%s", "Initial hover processing failed.");
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_TOPMOST_HOVER, false == gui.Controls.IsHovered(idFront));
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_COVERED_HOVER, gui.Controls.IsHovered(idBack));

	gui.Controls.SetDisabled(idFront, true);
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_PROCESS_RESULT, 0 > ::llc::guiProcessInput(gui, input), "%s", "Disabled fallthrough processing failed.");
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_DISABLED_HOVER, gui.Controls.IsHovered(idFront));
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_FALLTHROUGH_HOVER, false == gui.Controls.IsHovered(idBack));
	gui.Controls.SetDisabled(idFront, false);
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_PROCESS_RESULT, 0 > ::llc::guiProcessInput(gui, input), "%s", "Re-enabled hover processing failed.");

	input.MouseCurrent.ButtonState[0] = 1;
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_PROCESS_RESULT, 0 > ::llc::guiProcessInput(gui, input), "%s", "Press processing failed.");
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_PRESS_STATE, false == gui.Controls.IsPressed(idFront));
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_PRESS_ACTION, gui.Controls.IsAction(idFront));

	input.MousePrevious = input.MouseCurrent;
	input.MouseCurrent.ButtonState[0] = 0;
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_PROCESS_RESULT, 0 > ::llc::guiProcessInput(gui, input), "%s", "Release processing failed.");
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_RELEASE_STATE, gui.Controls.IsPressed(idFront));
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_RELEASE_ACTION, false == gui.Controls.IsAction(idFront));

	input.MousePrevious = input.MouseCurrent;
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_PROCESS_RESULT, 0 > ::llc::guiProcessInput(gui, input), "%s", "Action-clear processing failed.");
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_ACTION_CLEAR, gui.Controls.IsAction(idFront));

	input.MouseCurrent.ButtonState[0] = 1;
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_PROCESS_RESULT, 0 > ::llc::guiProcessInput(gui, input), "%s", "Drag press processing failed.");
	input.MousePrevious = input.MouseCurrent;
	input.MouseCurrent.Position = {90, 70};
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_PROCESS_RESULT, 0 > ::llc::guiProcessInput(gui, input), "%s", "Drag-outside processing failed.");
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_DRAG_HOVER, gui.Controls.IsHovered(idFront));
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_DRAG_PRESS, false == gui.Controls.IsPressed(idFront));
	input.MousePrevious = input.MouseCurrent;
	input.MouseCurrent.ButtonState[0] = 0;
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_PROCESS_RESULT, 0 > ::llc::guiProcessInput(gui, input), "%s", "Outside release processing failed.");
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_RELEASE_STATE, gui.Controls.IsPressed(idFront));
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_OUTSIDE_ACTION, gui.Controls.IsAction(idFront));
	rtrn 0;
}

sttc ::llc::err_t testGUIInputDisplayedMetrics(ATestError & errors) {
	::llc::SGUI gui = {};
	cnst ::llc::cid_t idControl = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_CREATE_RESULT, idControl < 0, "Control creation result:%i.", idControl);
	gui.Controls.Placement[idControl].Area = {{10, 10}, {30, 20}};
	gui.Controls.Text[idControl].Text = LLC_CXS("OLD");
	gui.Controls.Text[idControl].FontSelected = 2;
	::llc::u2_t measureCount = 0;
	cnst ::llc::FGUITextMeasure measure = [&measureCount](::llc::s1_t, ::llc::vcst_t text, ::llc::n2u1_t & size) {
		++measureCount;
		size = {(::llc::u1_t)(text.size() * 8U), 16};
		rtrn 0;
	};
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_METRICS_RESULT, 0 > ::llc::guiUpdateMetrics(gui, {100, 80}, measure, true), "%s", "Initial metric update failed.");
	LLC_TEST_CHECKF(errors, GUI_INPUT_TEST_RESULT_PUBLISHED_MEASURE_COUNT, measureCount != 1, "Initial measurement count:%u, expected:1.", measureCount);
	cnst ::llc::SControlArea displayedArea = gui.Controls.Area[idControl];

	gui.Controls.Placement[idControl].Area.Offset = {60, 10};
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_METRICS_RESULT, 0 > ::llc::controlTextSet(gui, idControl, LLC_CXS("FUTURE TEXT")), "%s", "Pending text update failed.");
	::llc::SInput input = {};
	input.MouseCurrent.Position = {20, 15};
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_PROCESS_RESULT, 0 > ::llc::guiProcessInput(gui, input), "%s", "Displayed metric processing failed.");
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_DISPLAYED_METRICS, gui.Controls.Area[idControl].Total.Global != displayedArea.Total.Global);
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_DISPLAYED_HOVER, false == gui.Controls.IsHovered(idControl));
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_PENDING_METRICS, gui.Controls.IsUpdated(idControl));
	LLC_TEST_CHECKF(errors, GUI_INPUT_TEST_RESULT_INPUT_MEASURE_COUNT, measureCount != 1, "Measurement count after input:%u, expected:1.", measureCount);

	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_METRICS_RESULT, 0 > ::llc::guiUpdateMetrics(gui, {100, 80}, measure, false), "%s", "Pending metric publication failed.");
	cnst ::llc::n2s1_t publishedOffset = {60, 10};
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_PUBLISHED_METRICS, gui.Controls.Area[idControl].Total.Global.Offset != publishedOffset);
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_PUBLISHED_METRICS, false == gui.Controls.IsUpdated(idControl));
	LLC_TEST_CHECKF(errors, GUI_INPUT_TEST_RESULT_PUBLISHED_MEASURE_COUNT, measureCount != 2, "Published measurement count:%u, expected:2.", measureCount);
	rtrn 0;
}

sttc ::llc::err_t testGUIInputKeyboard(ATestError & errors) {
	::llc::SGUI gui = {};
	::llc::astcid<3> controlsKeyboard = {};
	for(::llc::u2_t iControl = 0; iControl < controlsKeyboard.size(); ++iControl) {
		controlsKeyboard[iControl] = ::llc::controlCreate(gui);
		LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_CREATE_RESULT, controlsKeyboard[iControl] < 0, "Control:%u creation result:%i.", iControl, controlsKeyboard[iControl]);
		gui.Controls.Placement[controlsKeyboard[iControl]].Area = {{(::llc::s1_t)(10 + 30 * iControl), 10}, {20, 20}};
	}
	cnst ::llc::cid_t idChild = ::llc::controlCreateChild(gui, controlsKeyboard[2]);
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_CREATE_RESULT, idChild < 0, "Child creation result:%i.", idChild);
	gui.Controls.Placement[idChild].Area = {{0, 0}, {12, 12}};
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_METRICS_RESULT, 0 > ::llc::guiUpdateMetrics(gui, {100, 50}, true), "%s", "Keyboard-control metric update failed.");
	::llc::SInput input = {};
	input.MouseCurrent.Position = {99, 49};

	input.KeyboardCurrent.KeyState[::llc::SYSTEM_KEY_Tab] = 1;
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_PROCESS_RESULT, 0 > ::llc::guiProcessInput(gui, input, controlsKeyboard), "%s", "Initial Tab processing failed.");
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_TAB_FOCUS, false == gui.Controls.IsFocused(controlsKeyboard[0]));
	gui.Controls.SetDisabled(controlsKeyboard[1], true);

	input.KeyboardPrevious = input.KeyboardCurrent;
	input.KeyboardCurrent.KeyState[::llc::SYSTEM_KEY_Tab] = 0;
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_PROCESS_RESULT, 0 > ::llc::guiProcessInput(gui, input, controlsKeyboard), "%s", "Tab release processing failed.");
	input.KeyboardPrevious = input.KeyboardCurrent;
	input.KeyboardCurrent.KeyState[::llc::SYSTEM_KEY_Tab] = 1;
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_PROCESS_RESULT, 0 > ::llc::guiProcessInput(gui, input, controlsKeyboard), "%s", "Forward Tab processing failed.");
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_TAB_CYCLE, false == gui.Controls.IsFocused(controlsKeyboard[2]));
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_TAB_SKIP_DISABLED, gui.Controls.IsFocused(controlsKeyboard[1]));

	input.KeyboardPrevious = input.KeyboardCurrent;
	input.KeyboardCurrent.KeyState[::llc::SYSTEM_KEY_Tab] = 0;
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_PROCESS_RESULT, 0 > ::llc::guiProcessInput(gui, input, controlsKeyboard), "%s", "Second Tab release processing failed.");
	input.KeyboardPrevious = input.KeyboardCurrent;
	input.KeyboardCurrent.KeyState[::llc::SYSTEM_KEY_Shift] = 1;
	input.KeyboardCurrent.KeyState[::llc::SYSTEM_KEY_Tab] = 1;
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_PROCESS_RESULT, 0 > ::llc::guiProcessInput(gui, input, controlsKeyboard), "%s", "Shift+Tab processing failed.");
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_SHIFT_TAB_CYCLE, false == gui.Controls.IsFocused(controlsKeyboard[0]));

	input.KeyboardPrevious = input.KeyboardCurrent;
	input.KeyboardCurrent.KeyState[::llc::SYSTEM_KEY_Shift] = 0;
	input.KeyboardCurrent.KeyState[::llc::SYSTEM_KEY_Tab] = 0;
	input.KeyboardCurrent.KeyState[::llc::SYSTEM_KEY_Enter] = 1;
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_PROCESS_RESULT, 0 > ::llc::guiProcessInput(gui, input, controlsKeyboard), "%s", "Enter processing failed.");
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_KEYBOARD_ACTION, false == gui.Controls.IsAction(controlsKeyboard[0]));

	input.KeyboardPrevious = input.KeyboardCurrent;
	input.KeyboardCurrent.KeyState[::llc::SYSTEM_KEY_Enter] = 0;
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_PROCESS_RESULT, 0 > ::llc::guiProcessInput(gui, input, controlsKeyboard), "%s", "Enter release processing failed.");
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_KEYBOARD_ACTION_CLEAR, gui.Controls.IsAction(controlsKeyboard[0]));
	input.KeyboardPrevious = input.KeyboardCurrent;
	input.KeyboardCurrent.KeyState[::llc::SYSTEM_KEY_Space] = 1;
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_PROCESS_RESULT, 0 > ::llc::guiProcessInput(gui, input, controlsKeyboard), "%s", "Space processing failed.");
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_SPACE_ACTION, false == gui.Controls.IsAction(controlsKeyboard[0]));

	input.KeyboardPrevious = input.KeyboardCurrent;
	input.KeyboardCurrent.KeyState[::llc::SYSTEM_KEY_Space] = 0;
	input.MousePrevious = input.MouseCurrent;
	input.MouseCurrent.Position = {75, 15};
	input.MouseCurrent.ButtonState[0] = 1;
	LLC_TEST_REQUIREF(errors, GUI_INPUT_TEST_RESULT_PROCESS_RESULT, 0 > ::llc::guiProcessInput(gui, input, controlsKeyboard), "%s", "Mouse-focus processing failed.");
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_MOUSE_FOCUS, false == gui.Controls.IsFocused(controlsKeyboard[2]));
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_MOUSE_CHILD_FOCUS, false == gui.Controls.IsHovered(idChild));
	LLC_TEST_CHECK(errors, GUI_INPUT_TEST_RESULT_MOUSE_CHILD_FOCUS, gui.Controls.IsFocused(idChild));
	rtrn 0;
}

::llc::err_t testGUIInput(ATestError & errors) {
	if_fail_fe(::testGUIInputProcess(errors));
	if_fail_fe(::testGUIInputDisplayedMetrics(errors));
	rtrn ::testGUIInputKeyboard(errors);
}
