#include "llc_gui_checkbox.h"
#include "llc_gui_slider.h"
#include "llc_system_key.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(GUI_COMPONENT_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, CHECKBOX_DEFAULT		,  0, "A default check box has an assigned control.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, CHECKBOX_CREATE		,  1, "Check-box creation failed.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, CHECKBOX_RETURN		,  2, "Check-box creation returned the wrong control.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, CHECKBOX_PARENT		,  3, "The check box has the wrong parent.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, CHECKBOX_AREA		,  4, "The check box has the wrong placement.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, CHECKBOX_TEXT		,  5, "The check box lost its text.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, CHECKBOX_INITIAL		,  6, "The check box lost its initial checked state.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, CHECKBOX_SELECTION	,  7, "The check-box value and selected state disagree.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, CHECKBOX_IDLE		,  8, "An idle check box changed value.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, CHECKBOX_ACTION		,  9, "A check-box action did not toggle its value.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, CHECKBOX_SET			, 10, "Explicit check-box assignment failed.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, SLIDER_DEFAULT		, 11, "A default slider has assigned controls.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, SLIDER_CREATE			, 12, "Slider creation failed.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, SLIDER_RETURN			, 13, "Slider creation returned the wrong root.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, SLIDER_PARENT			, 14, "The slider has the wrong parent hierarchy.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, SLIDER_AREA			, 15, "The slider has the wrong placement.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, SLIDER_VALUE			, 16, "The slider has the wrong value.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, SLIDER_TEXT			, 17, "The slider has the wrong value text.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, SLIDER_OFFSET			, 18, "The slider thumb has the wrong offset.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, SLIDER_CLAMP			, 19, "The slider did not clamp a value to its limits.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, SLIDER_FORMAT			, 20, "The slider did not use its text formatter.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, SLIDER_METRICS		, 21, "Slider metric publication failed.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, SLIDER_KEYBOARD		, 22, "Keyboard input changed the slider incorrectly.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, SLIDER_PAGE			, 23, "Page input changed the slider incorrectly.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, SLIDER_HOME_END		, 24, "Home or End changed the slider incorrectly.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, SLIDER_MOUSE_PAGE	, 25, "Track input changed the slider incorrectly.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, SLIDER_MOUSE_DRAG	, 26, "Thumb dragging changed the slider incorrectly.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, SLIDER_VERTICAL		, 27, "A vertical slider has the wrong geometry.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, SLIDER_ACTION			, 28, "Slider activation changed the value incorrectly.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, SLIDER_RELEASE		, 29, "Releasing the slider changed its value.");
GDEFINE_ENUM_VALUED(GUI_COMPONENT_TEST_RESULT, SLIDER_PUBLISHED		, 30, "A slider value change did not publish the new thumb geometry.");

sttc ::llc::err_t testGUICheckBox(ATestError & errors) {
	cnst ::llc::SCheckBox defaults = {};
	LLC_TEST_CHECK(errors, GUI_COMPONENT_TEST_RESULT_CHECKBOX_DEFAULT, defaults.IdControl != ::llc::CID_INVALID);
	LLC_TEST_CHECK(errors, GUI_COMPONENT_TEST_RESULT_CHECKBOX_DEFAULT, defaults.Checked);

	::llc::SGUI gui = {};
	cnst ::llc::cid_t idParent = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_COMPONENT_TEST_RESULT_CHECKBOX_CREATE, idParent < 0, "Parent creation result:%i.", idParent);
	::llc::SCheckBox checkBox = {};
	cnst ::llc::SCheckBoxCreate setup = {idParent, {{12, 14}, {30, 30}}, LLC_CXS("X"), true};
	cnst ::llc::err_t createResult = ::llc::checkBoxCreate(checkBox, gui, setup);
	LLC_TEST_REQUIREF(errors, GUI_COMPONENT_TEST_RESULT_CHECKBOX_CREATE, ::llc::failed(createResult), "Create result:%i.", createResult);
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_CHECKBOX_RETURN, createResult != checkBox.IdControl, "Result:%i, control:%i.", createResult, checkBox.IdControl);
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_CHECKBOX_PARENT, gui.Controls.Entities[checkBox.IdControl].Parent != idParent, "Parent:%i, expected:%i.", gui.Controls.Entities[checkBox.IdControl].Parent, idParent);
	LLC_TEST_CHECK(errors, GUI_COMPONENT_TEST_RESULT_CHECKBOX_AREA, gui.Controls.Placement[checkBox.IdControl].Area != setup.Area);
	LLC_TEST_CHECK(errors, GUI_COMPONENT_TEST_RESULT_CHECKBOX_TEXT, gui.Controls.Text[checkBox.IdControl].Text != setup.Text);
	LLC_TEST_CHECK(errors, GUI_COMPONENT_TEST_RESULT_CHECKBOX_INITIAL, false == checkBox.Checked);
	LLC_TEST_CHECK(errors, GUI_COMPONENT_TEST_RESULT_CHECKBOX_SELECTION, false == gui.Controls.IsSelected(checkBox.IdControl));

	cnst ::llc::err_t idleResult = ::llc::checkBoxUpdate(checkBox, gui);
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_CHECKBOX_IDLE, idleResult || false == checkBox.Checked, "Result:%i, checked:%u.", idleResult, (::llc::u2_t)checkBox.Checked);
	gui.Controls.SetAction(checkBox.IdControl, true);
	cnst ::llc::err_t actionResult = ::llc::checkBoxUpdate(checkBox, gui);
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_CHECKBOX_ACTION, actionResult != 1 || checkBox.Checked, "Result:%i, checked:%u.", actionResult, (::llc::u2_t)checkBox.Checked);
	LLC_TEST_CHECK(errors, GUI_COMPONENT_TEST_RESULT_CHECKBOX_SELECTION, gui.Controls.IsSelected(checkBox.IdControl));
	LLC_TEST_CHECK(errors, GUI_COMPONENT_TEST_RESULT_CHECKBOX_TEXT, gui.Controls.Text[checkBox.IdControl].Text.size());
	cnst ::llc::err_t setResult = ::llc::checkBoxSetChecked(checkBox, gui, true);
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_CHECKBOX_SET, setResult != 1 || false == checkBox.Checked, "Result:%i, checked:%u.", setResult, (::llc::u2_t)checkBox.Checked);
	LLC_TEST_CHECK(errors, GUI_COMPONENT_TEST_RESULT_CHECKBOX_SELECTION, false == gui.Controls.IsSelected(checkBox.IdControl));
	LLC_TEST_CHECK(errors, GUI_COMPONENT_TEST_RESULT_CHECKBOX_TEXT, gui.Controls.Text[checkBox.IdControl].Text != setup.Text);
	rtrn 0;
}

sttc void inputKeyDown(::llc::SInput & input, ::llc::SYSTEM_KEY key) nxpt {
	input.KeyboardPrevious = input.KeyboardCurrent;
	input.KeyboardCurrent.KeyState[key] = 1;
}

sttc void inputKeyUp(::llc::SInput & input, ::llc::SYSTEM_KEY key) nxpt {
	input.KeyboardPrevious = input.KeyboardCurrent;
	input.KeyboardCurrent.KeyState[key] = 0;
}

sttc ::llc::err_t testGUISlider(ATestError & errors) {
	cnst ::llc::SSlider defaults = {};
	LLC_TEST_CHECK(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_DEFAULT, defaults.IdRoot != ::llc::CID_INVALID || defaults.IdThumb != ::llc::CID_INVALID);

	::llc::SGUI gui = {};
	cnst ::llc::cid_t idParent = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_CREATE, idParent < 0, "Parent creation result:%i.", idParent);
	::llc::SSlider slider = {};
	cnst ::llc::SSliderCreate setup = {idParent, {{10, 20}, {206, 30}}, {-50, 50}, 0, false};
	cnst ::llc::err_t createResult = ::llc::sliderCreate(slider, gui, setup);
	LLC_TEST_REQUIREF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_CREATE, ::llc::failed(createResult), "Create result:%i.", createResult);
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_RETURN, createResult != slider.IdRoot, "Result:%i, root:%i.", createResult, slider.IdRoot);
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_PARENT, gui.Controls.Entities[slider.IdRoot].Parent != idParent, "Root parent:%i, expected:%i.", gui.Controls.Entities[slider.IdRoot].Parent, idParent);
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_PARENT, gui.Controls.Entities[slider.IdThumb].Parent != slider.IdRoot, "Thumb parent:%i, expected:%i.", gui.Controls.Entities[slider.IdThumb].Parent, slider.IdRoot);
	LLC_TEST_CHECK(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_AREA, gui.Controls.Placement[slider.IdRoot].Area != setup.Area);
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_VALUE, slider.ValueCurrent != 0, "Value:%" LLC_FMT_S3 ", expected:0.", slider.ValueCurrent);
	LLC_TEST_CHECK(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_TEXT, gui.Controls.Text[slider.IdRoot].Text != LLC_CXS("0"));
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_OFFSET, gui.Controls.Placement[slider.IdThumb].Area.Offset.x != 90, "Offset:%i, expected:90.", gui.Controls.Placement[slider.IdThumb].Area.Offset.x);

	if_fail_fe(::llc::sliderSetValue(slider, gui, 100));
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_CLAMP, slider.ValueCurrent != 50, "Value:%" LLC_FMT_S3 ", expected:50.", slider.ValueCurrent);
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_OFFSET, gui.Controls.Placement[slider.IdThumb].Area.Offset.x != 180, "Offset:%i, expected:180.", gui.Controls.Placement[slider.IdThumb].Area.Offset.x);
	LLC_TEST_CHECK(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_TEXT, gui.Controls.Text[slider.IdRoot].Text != LLC_CXS("50"));
	if_fail_fe(::llc::sliderSetValue(slider, gui, -100));
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_CLAMP, slider.ValueCurrent != -50, "Value:%" LLC_FMT_S3 ", expected:-50.", slider.ValueCurrent);
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_OFFSET, gui.Controls.Placement[slider.IdThumb].Area.Offset.x, "Offset:%i, expected:0.", gui.Controls.Placement[slider.IdThumb].Area.Offset.x);

	slider.TextFormat = [](::llc::astchar<64> & output, ::llc::s3_t value, ::llc::minmaxs3_c &) { rtrn ::llc::sprintf_s(output.Storage, "VALUE:%" LLC_FMT_S3, value); };
	if_fail_fe(::llc::sliderSetValue(slider, gui, 25));
	LLC_TEST_CHECK(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_FORMAT, gui.Controls.Text[slider.IdRoot].Text != LLC_CXS("VALUE:25"));
	cnst ::llc::FGUITextMeasure textMeasure = [](::llc::s1_t, ::llc::vcst_t text, ::llc::n2u1_t & size) {
		size = {(::llc::u1_t)(text.size() * 8U), 16};
		rtrn 0;
	};
	LLC_TEST_REQUIREF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_METRICS, ::llc::failed(::llc::guiUpdateMetrics(gui, {400, 200}, textMeasure, true)), "%s", "Slider metric publication failed.");
	cnst ::llc::s1_t thumbOffsetBefore = gui.Controls.Area[slider.IdThumb].Total.Global.Offset.x;

	::llc::SInput input = {};
	gui.Controls.SetFocused(slider.IdRoot, true);
	::inputKeyDown(input, ::llc::SYSTEM_KEY_Right);
	if_fail_fe(::llc::sliderUpdate(slider, gui, input));
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_KEYBOARD, slider.ValueCurrent != 26, "Value:%" LLC_FMT_S3 ", expected:26.", slider.ValueCurrent);
	LLC_TEST_REQUIREF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_METRICS, ::llc::failed(::llc::guiUpdateMetrics(gui, {400, 200}, textMeasure, false)), "%s", "Slider metric refresh failed.");
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_PUBLISHED, gui.Controls.Area[slider.IdThumb].Total.Global.Offset.x == thumbOffsetBefore, "Published thumb offset:%i, previous:%i.", gui.Controls.Area[slider.IdThumb].Total.Global.Offset.x, thumbOffsetBefore);
	::inputKeyUp(input, ::llc::SYSTEM_KEY_Right);
	::inputKeyDown(input, ::llc::SYSTEM_KEY_Prior);
	if_fail_fe(::llc::sliderUpdate(slider, gui, input));
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_PAGE, slider.ValueCurrent != 36, "Value:%" LLC_FMT_S3 ", expected:36.", slider.ValueCurrent);
	::inputKeyUp(input, ::llc::SYSTEM_KEY_Prior);
	::inputKeyDown(input, ::llc::SYSTEM_KEY_Home);
	if_fail_fe(::llc::sliderUpdate(slider, gui, input));
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_HOME_END, slider.ValueCurrent != -50, "Home value:%" LLC_FMT_S3 ", expected:-50.", slider.ValueCurrent);
	::inputKeyUp(input, ::llc::SYSTEM_KEY_Home);
	::inputKeyDown(input, ::llc::SYSTEM_KEY_End);
	if_fail_fe(::llc::sliderUpdate(slider, gui, input));
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_HOME_END, slider.ValueCurrent != 50, "End value:%" LLC_FMT_S3 ", expected:50.", slider.ValueCurrent);
	::inputKeyUp(input, ::llc::SYSTEM_KEY_End);

	if_fail_fe(::llc::sliderSetValue(slider, gui, 0));
	gui.Controls.SetAction(slider.IdRoot, true);
	::inputKeyDown(input, ::llc::SYSTEM_KEY_Enter);
	if_fail_fe(::llc::sliderUpdate(slider, gui, input));
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_ACTION, slider.ValueCurrent != 1, "Value:%" LLC_FMT_S3 ", expected:1.", slider.ValueCurrent);
	gui.Controls.SetAction(slider.IdRoot, false);
	::inputKeyUp(input, ::llc::SYSTEM_KEY_Enter);

	if_fail_fe(::llc::sliderSetValue(slider, gui, 0));
	if_fail_fe(::llc::guiUpdateMetrics(gui, {400, 200}, textMeasure, true));
	input.MousePrevious = input.MouseCurrent;
	input.MouseCurrent.Position = {210, 32};
	input.MouseCurrent.ButtonState[0] = 1;
	gui.Controls.SetPressed(slider.IdRoot, true);
	if_fail_fe(::llc::sliderUpdate(slider, gui, input));
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_MOUSE_PAGE, slider.ValueCurrent != 10, "Value:%" LLC_FMT_S3 ", expected:10.", slider.ValueCurrent);
	gui.Controls.SetPressed(slider.IdRoot, false);
	gui.Controls.SetAction(slider.IdRoot, true);
	input.MousePrevious = input.MouseCurrent;
	input.MouseCurrent.ButtonState[0] = 0;
	if_fail_fe(::llc::sliderUpdate(slider, gui, input));
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_RELEASE, slider.ValueCurrent != 10, "Value:%" LLC_FMT_S3 ", expected:10.", slider.ValueCurrent);
	gui.Controls.SetAction(slider.IdRoot, false);

	if_fail_fe(::llc::sliderSetValue(slider, gui, 0));
	if_fail_fe(::llc::guiUpdateMetrics(gui, {400, 200}, textMeasure, true));
	gui.Controls.SetPressed(slider.IdRoot, false);
	gui.Controls.SetPressed(slider.IdThumb, true);
	input.MousePrevious.Position = {100, 32};
	input.MouseCurrent.Position = {215, 32};
	input.MousePrevious.ButtonState[0] = input.MouseCurrent.ButtonState[0] = 1;
	if_fail_fe(::llc::sliderUpdate(slider, gui, input));
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_MOUSE_DRAG, slider.ValueCurrent != 50, "Value:%" LLC_FMT_S3 ", expected:50.", slider.ValueCurrent);

	::llc::SSlider vertical = {};
	cnst ::llc::SSliderCreate verticalSetup = {idParent, {{240, 10}, {30, 206}}, {0, 100}, 100, true};
	LLC_TEST_REQUIREF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_CREATE, ::llc::failed(::llc::sliderCreate(vertical, gui, verticalSetup)), "%s", "Vertical slider creation failed.");
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_VERTICAL, gui.Controls.Placement[vertical.IdThumb].Area.Offset.y != 180, "Offset:%i, expected:180.", gui.Controls.Placement[vertical.IdThumb].Area.Offset.y);
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_VERTICAL, gui.Controls.Constraints[vertical.IdThumb].AttachSizeToControl.x != vertical.IdThumb, "Width target:%i, expected:%i.", gui.Controls.Constraints[vertical.IdThumb].AttachSizeToControl.x, vertical.IdThumb);
	LLC_TEST_REQUIREF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_METRICS, ::llc::failed(::llc::guiUpdateMetrics(gui, {400, 260}, textMeasure, true)), "%s", "Vertical slider metric publication failed.");
	cnst ::llc::s1_t verticalOffsetBefore = gui.Controls.Area[vertical.IdThumb].Total.Global.Offset.y;
	if_fail_fe(::llc::sliderSetValue(vertical, gui, 50));
	LLC_TEST_REQUIREF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_METRICS, ::llc::failed(::llc::guiUpdateMetrics(gui, {400, 260}, textMeasure, false)), "%s", "Vertical slider metric refresh failed.");
	LLC_TEST_CHECKF(errors, GUI_COMPONENT_TEST_RESULT_SLIDER_PUBLISHED, gui.Controls.Area[vertical.IdThumb].Total.Global.Offset.y == verticalOffsetBefore, "Published vertical thumb offset:%i, previous:%i.", gui.Controls.Area[vertical.IdThumb].Total.Global.Offset.y, verticalOffsetBefore);
	rtrn 0;
}

::llc::err_t testGUIComponents(ATestError & errors) {
	if_fail_fe(::testGUICheckBox(errors));
	rtrn ::testGUISlider(errors);
}
