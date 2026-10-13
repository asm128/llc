#include "llc_gui_slider.h"

#include "llc_cstring.h"
#include "llc_system_key.h"

sttc ::llc::err_t sliderValidate(cnst ::llc::SSlider & slider, cnst ::llc::SGUI & gui) {
	if_true_fef(::llc::controlInvalid(gui, slider.IdRoot), "Invalid slider root:%i.", slider.IdRoot);
	if_true_fef(::llc::controlInvalid(gui, slider.IdThumb), "Invalid slider thumb:%i.", slider.IdThumb);
	if_true_fef(gui.Controls.Entities[slider.IdThumb].Parent != slider.IdRoot, "Slider thumb:%i parent:%i, expected:%i.", slider.IdThumb, gui.Controls.Entities[slider.IdThumb].Parent, slider.IdRoot);
	if_true_fef(slider.ValueLimits.Max < slider.ValueLimits.Min, "Invalid slider limits:{%" LLC_FMT_S3 ", %" LLC_FMT_S3 "}.", slider.ValueLimits.Min, slider.ValueLimits.Max);
	if_true_fef(slider.ValueStep < 0 || slider.ValuePage < 0, "Invalid slider increments:{%" LLC_FMT_S3 ", %" LLC_FMT_S3 "}.", slider.ValueStep, slider.ValuePage);
	rtrn 0;
}

sttc ::llc::err_t sliderTextUpdate(::llc::SSlider & slider, ::llc::SGUI & gui) {
	::llc::astchar<64> text = {};
	cnst ::llc::err_t length = slider.TextFormat
		? slider.TextFormat(text, slider.ValueCurrent, slider.ValueLimits)
		: ::llc::sprintf_s(text.Storage, "%" LLC_FMT_S3, slider.ValueCurrent)
		;
	if_fail_fe(length);
	if_true_fef((::llc::u2_t)length >= text.size(), "Slider text length:%i exceeds capacity:%u.", length, text.size());
	cnst ::llc::vcst_t textView = {text.Storage, (::llc::u2_t)length};
	if(gui.Controls.Text[slider.IdRoot].Text == textView)
		rtrn 0;
	for(::llc::u2_t iCharacter = 0; iCharacter <= (::llc::u2_t)length; ++iCharacter)
		slider.ValueText[iCharacter] = text[iCharacter];
	gui.Controls.Text[slider.IdRoot].Text = {slider.ValueText.Storage, (::llc::u2_t)length};
	gui.Controls.SetUpdated(slider.IdRoot, false);
	rtrn 1;
}

sttc ::llc::s3_t sliderValueOffset(cnst ::llc::SSlider & slider, bool increase, ::llc::s3_t amount) nxpt {
	if(increase) {
		cnst ::llc::u3_t available = (::llc::u3_t)slider.ValueLimits.Max - (::llc::u3_t)slider.ValueCurrent;
		rtrn (::llc::u3_t)amount < available ? slider.ValueCurrent + amount : slider.ValueLimits.Max;
	}
	cnst ::llc::u3_t available = (::llc::u3_t)slider.ValueCurrent - (::llc::u3_t)slider.ValueLimits.Min;
	rtrn (::llc::u3_t)amount < available ? slider.ValueCurrent - amount : slider.ValueLimits.Min;
}

::llc::err_t llc::sliderSetValue(::llc::SSlider & slider, ::llc::SGUI & gui, ::llc::s3_t value) {
	if_fail_fe(::sliderValidate(slider, gui));
	cnst ::llc::s3_t valueNext = ::llc::max(slider.ValueLimits.Min, ::llc::min(slider.ValueLimits.Max, value));
	cnst bool valueChanged = slider.ValueCurrent != valueNext;
	slider.ValueCurrent = valueNext;

	cnst ::llc::SControlPlacement & rootPlacement = gui.Controls.Placement[slider.IdRoot];
	::llc::SControlPlacement & thumbPlacement = gui.Controls.Placement[slider.IdThumb];
	cnst ::llc::n2u1_t ncSpacing = ::llc::controlNCSpacing(rootPlacement);
	cnst ::llc::s2_t rootLength = slider.Vertical ? rootPlacement.Area.Size.y : rootPlacement.Area.Size.x;
	cnst ::llc::s2_t nonClientLength = slider.Vertical ? ncSpacing.y : ncSpacing.x;
	cnst ::llc::s2_t thumbLength = slider.Vertical ? thumbPlacement.Area.Size.y : thumbPlacement.Area.Size.x;
	cnst ::llc::s2_t trackLength = ::llc::max((::llc::s2_t)0, rootLength - nonClientLength - thumbLength);
	cnst long double range = (long double)slider.ValueLimits.Max - (long double)slider.ValueLimits.Min;
	cnst long double proportion = range ? ((long double)slider.ValueCurrent - (long double)slider.ValueLimits.Min) / range : 0;
	cnst ::llc::s1_t offsetNext = (::llc::s1_t)(trackLength * proportion);
	cnst bool offsetChanged = slider.Vertical ? thumbPlacement.Area.Offset.y != offsetNext : thumbPlacement.Area.Offset.x != offsetNext;
	if(slider.Vertical)
		thumbPlacement.Area.Offset.y = offsetNext;
	else
		thumbPlacement.Area.Offset.x = offsetNext;
	cnst ::llc::err_t textChanged = ::sliderTextUpdate(slider, gui);
	if_fail_fe(textChanged);
	if(offsetChanged)
		if_fail_fe(::llc::controlMetricsInvalidate(gui, slider.IdRoot));
	rtrn one_if(valueChanged || offsetChanged || textChanged);
}

::llc::err_t llc::sliderCreate(::llc::SSlider & slider, ::llc::SGUI & gui, cnst ::llc::SSliderCreate & setup) {
	if_true_fef(slider.IdRoot != ::llc::CID_INVALID || slider.IdThumb != ::llc::CID_INVALID, "Slider controls already assigned:{%i, %i}.", slider.IdRoot, slider.IdThumb);
	if_true_fef(setup.Parent != ::llc::CID_INVALID && ::llc::controlInvalid(gui, setup.Parent), "Invalid slider parent:%i.", setup.Parent);
	if_true_fef(setup.Area.Size.x <= 0 || setup.Area.Size.y <= 0, "Invalid slider size:{%i, %i}.", setup.Area.Size.x, setup.Area.Size.y);
	if_true_fef(setup.ValueLimits.Max < setup.ValueLimits.Min, "Invalid slider limits:{%" LLC_FMT_S3 ", %" LLC_FMT_S3 "}.", setup.ValueLimits.Min, setup.ValueLimits.Max);

	slider.IdRoot = ::llc::controlCreate(gui);
	if_true_fef(slider.IdRoot < 0, "Failed to create slider root:%i.", slider.IdRoot);
	if(setup.Parent != ::llc::CID_INVALID && ::llc::failed(::llc::controlSetParent(gui, slider.IdRoot, setup.Parent))) {
		::llc::controlDelete(gui, slider.IdRoot);
		slider = {};
		rtrn -1;
	}
	slider.IdThumb = ::llc::controlCreateChild(gui, slider.IdRoot);
	if(slider.IdThumb < 0) {
		cnst ::llc::err_t error = slider.IdThumb;
		::llc::controlDelete(gui, slider.IdRoot);
		slider = {};
		rtrn error;
	}

	slider.ValueLimits = setup.ValueLimits;
	slider.Vertical = setup.Vertical;
	::llc::SControlPlacement & rootPlacement = gui.Controls.Placement[slider.IdRoot];
	rootPlacement.Area = setup.Area;
	rootPlacement.Margin = {1, 1, 1, 1};
	rootPlacement.Border = {2, 2, 2, 2};
	gui.Controls.Text[slider.IdRoot].Align = ::llc::ALIGN_CENTER;
	::llc::SControlPlacement & thumbPlacement = gui.Controls.Placement[slider.IdThumb];
	thumbPlacement.Margin = {};
	thumbPlacement.Border = {1, 1, 1, 1};
	if(slider.Vertical) {
		thumbPlacement.Area.Size = {16, 20};
		gui.Controls.Constraints[slider.IdThumb].AttachSizeToControl.x = slider.IdThumb;
	}
	else {
		thumbPlacement.Area.Size = {20, 16};
		gui.Controls.Constraints[slider.IdThumb].AttachSizeToControl.y = slider.IdThumb;
	}
	cnst ::llc::err_t valueResult = ::llc::sliderSetValue(slider, gui, setup.Value);
	if(::llc::failed(valueResult)) {
		::llc::controlDelete(gui, slider.IdRoot);
		slider = {};
		rtrn valueResult;
	}
	rtrn slider.IdRoot;
}

::llc::err_t llc::sliderUpdate(::llc::SSlider & slider, ::llc::SGUI & gui, cnst ::llc::SInput & input) {
	if_fail_fe(::sliderValidate(slider, gui));
	::llc::s3_t valueNext = slider.ValueCurrent;
	cnst bool focused = gui.Controls.IsFocused(slider.IdRoot) || gui.Controls.IsFocused(slider.IdThumb);
	if(focused) {
		if(input.KeyDown(::llc::SYSTEM_KEY_Home))
			valueNext = slider.ValueLimits.Min;
		else if(input.KeyDown(::llc::SYSTEM_KEY_End))
			valueNext = slider.ValueLimits.Max;
		else if(input.KeyDown(::llc::SYSTEM_KEY_Prior))
			valueNext = ::sliderValueOffset(slider, true, slider.ValuePage);
		else if(input.KeyDown(::llc::SYSTEM_KEY_Next))
			valueNext = ::sliderValueOffset(slider, false, slider.ValuePage);
		else if(input.KeyDown(slider.Vertical ? ::llc::SYSTEM_KEY_Up : ::llc::SYSTEM_KEY_Right))
			valueNext = ::sliderValueOffset(slider, true, slider.ValueStep);
		else if(input.KeyDown(slider.Vertical ? ::llc::SYSTEM_KEY_Down : ::llc::SYSTEM_KEY_Left))
			valueNext = ::sliderValueOffset(slider, false, slider.ValueStep);
	}
	if(gui.Controls.IsAction(slider.IdRoot) && (input.KeyDown(::llc::SYSTEM_KEY_Enter) || input.KeyDown(::llc::SYSTEM_KEY_Space)))
		valueNext = ::sliderValueOffset(slider, true, slider.ValueStep);

	cnst ::llc::SControlState & thumbState = gui.Controls.States[slider.IdThumb];
	if(thumbState.IsPressed() && input.MouseCurrent.Position != input.MousePrevious.Position) {
		cnst ::llc::SControlArea & rootArea = gui.Controls.Area[slider.IdRoot];
		cnst ::llc::SControlArea & thumbArea = gui.Controls.Area[slider.IdThumb];
		cnst ::llc::s2_t rootLength = slider.Vertical ? rootArea.Client.Global.Size.y : rootArea.Client.Global.Size.x;
		cnst ::llc::s2_t thumbSize = slider.Vertical ? thumbArea.Total.Global.Size.y : thumbArea.Total.Global.Size.x;
		cnst ::llc::s2_t trackLength = ::llc::max((::llc::s2_t)0, rootLength - thumbSize);
		if(trackLength) {
			cnst ::llc::s2_t cursorPosition = slider.Vertical ? input.MouseCurrent.Position.y : input.MouseCurrent.Position.x;
			cnst ::llc::s2_t rootOffset = slider.Vertical ? rootArea.Client.Global.Offset.y : rootArea.Client.Global.Offset.x;
			cnst ::llc::s2_t cursor = cursorPosition - rootOffset - thumbSize / 2;
			cnst long double proportion = (long double)::llc::max<::llc::s2_t>(0, ::llc::min(trackLength, cursor)) / trackLength;
			cnst long double range = (long double)slider.ValueLimits.Max - (long double)slider.ValueLimits.Min;
			valueNext = (::llc::s3_t)((long double)slider.ValueLimits.Min + range * proportion);
		}
	}
	else if(input.ButtonDown(0) && gui.Controls.IsPressed(slider.IdRoot)) {
		cnst ::llc::SControlArea & thumbArea = gui.Controls.Area[slider.IdThumb];
		cnst ::llc::s2_t thumbOffset = slider.Vertical ? thumbArea.Total.Global.Offset.y : thumbArea.Total.Global.Offset.x;
		cnst ::llc::s2_t thumbSize = slider.Vertical ? thumbArea.Total.Global.Size.y : thumbArea.Total.Global.Size.x;
		cnst ::llc::s2_t cursorPosition = slider.Vertical ? input.MouseCurrent.Position.y : input.MouseCurrent.Position.x;
		valueNext = ::sliderValueOffset(slider, cursorPosition > thumbOffset + thumbSize / 2, slider.ValuePage);
	}
	rtrn ::llc::sliderSetValue(slider, gui, valueNext);
}
