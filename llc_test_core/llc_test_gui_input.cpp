#include "llc_gui.h"

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

::llc::err_t testGUIInput(ATestError & errors) {
	rtrn ::testGUIInputProcess(errors);
}
