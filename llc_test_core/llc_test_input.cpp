#include "llc_input.h"

#include "llc_test_core.h"

#include <type_traits>

GDEFINE_ENUM_TYPE(INPUT_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, JOYSTICK_BUTTON_TYPE			,  0, "SInputJoystick::ButtonState has the wrong scalar type.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, MOUSE_BUTTON_TYPE			,  1, "SInputMouse::ButtonState has the wrong array type.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, MOUSE_TIME_TYPE				,  2, "SInputMouse::ButtonTime has the wrong array type.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, MOUSE_STRENGTH_TYPE			,  3, "SInputMouse::ButtonStrength has the wrong array type.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, KEYBOARD_STATE_TYPE			,  4, "SInputKeyboard::KeyState has the wrong array type.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, KEYBOARD_TIME_TYPE			,  5, "SInputKeyboard::KeyTime has the wrong array type.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, KEYBOARD_STRENGTH_TYPE		,  6, "SInputKeyboard::KeyStrength has the wrong array type.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, JOYSTICK_PACKED_SIZE			,  7, "SInputJoystick has the wrong packed size.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, MOUSE_PACKED_SIZE			,  8, "SInputMouse has the wrong packed size.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, KEYBOARD_PACKED_SIZE			,  9, "SInputKeyboard has the wrong packed size.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, DEFAULT_KEY_STATE			, 10, "A default keyboard key is not released.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, DEFAULT_KEY_TIME				, 11, "A default keyboard key has nonzero time.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, DEFAULT_KEY_STRENGTH			, 12, "A default keyboard key has nonzero strength.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, DEFAULT_BUTTON_STATE			, 13, "A default mouse button is not released.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, DEFAULT_BUTTON_TIME			, 14, "A default mouse button has nonzero time.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, DEFAULT_BUTTON_STRENGTH		, 15, "A default mouse button has nonzero strength.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, KEY_IDLE_DOWN					, 16, "KeyDown() reported an idle key.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, KEY_IDLE_UP					, 17, "KeyUp() reported an idle key.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, KEY_PRESS_DOWN				, 18, "KeyDown() missed a key press.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, KEY_PRESS_UP					, 19, "KeyUp() reported a key press.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, KEY_HELD_DOWN					, 20, "KeyDown() reported a held key.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, KEY_HELD_UP					, 21, "KeyUp() reported a held key.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, KEY_RELEASE_DOWN				, 22, "KeyDown() reported a key release.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, KEY_RELEASE_UP				, 23, "KeyUp() missed a key release.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, BUTTON_IDLE_DOWN				, 24, "ButtonDown() reported an idle button.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, BUTTON_IDLE_UP				, 25, "ButtonUp() reported an idle button.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, BUTTON_PRESS_DOWN			, 26, "ButtonDown() missed a button press.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, BUTTON_PRESS_UP				, 27, "ButtonUp() reported a button press.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, BUTTON_HELD_DOWN				, 28, "ButtonDown() reported a held button.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, BUTTON_HELD_UP				, 29, "ButtonUp() reported a held button.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, BUTTON_RELEASE_DOWN			, 30, "ButtonDown() reported a button release.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, BUTTON_RELEASE_UP			, 31, "ButtonUp() missed a button release.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, BUTTON_OUT_OF_RANGE_DOWN	, 32, "ButtonDown() accepted an out-of-range button.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, BUTTON_OUT_OF_RANGE_UP		, 33, "ButtonUp() accepted an out-of-range button.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_KEY_PREVIOUS_STATE	, 34, "inputNextFrame() stored the wrong previous key state.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_KEY_PREVIOUS_TIME		, 35, "inputNextFrame() stored the wrong previous key time.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_KEY_PREVIOUS_STRENGTH	, 36, "inputNextFrame() stored the wrong previous key strength.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_KEY_CURRENT_STATE		, 37, "inputNextFrame() changed the current key state.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_KEY_CURRENT_TIME		, 38, "inputNextFrame() changed the current key time.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_KEY_CURRENT_STRENGTH	, 39, "inputNextFrame() changed the current key strength.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_BUTTON_PREVIOUS_STATE	, 40, "inputNextFrame() stored the wrong previous button state.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_BUTTON_PREVIOUS_TIME	, 41, "inputNextFrame() stored the wrong previous button time.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_BUTTON_PREVIOUS_STRENGTH, 42, "inputNextFrame() stored the wrong previous button strength.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_BUTTON_CURRENT_STATE	, 43, "inputNextFrame() changed the current button state.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_BUTTON_CURRENT_TIME		, 44, "inputNextFrame() changed the current button time.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_BUTTON_CURRENT_STRENGTH	, 45, "inputNextFrame() changed the current button strength.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_MOUSE_PREVIOUS_POSITION	, 46, "inputNextFrame() stored the wrong previous mouse position.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_MOUSE_CURRENT_POSITION	, 47, "inputNextFrame() changed the current mouse position.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_MOUSE_PREVIOUS_DELTA	, 48, "inputNextFrame() stored the wrong previous mouse delta.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_MOUSE_CURRENT_DELTA		, 49, "inputNextFrame() retained a current mouse delta.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_JOYSTICK_COUNT			, 50, "inputNextFrame() stored the wrong previous joystick count.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_JOYSTICK_POSITION		, 51, "inputNextFrame() stored the wrong previous joystick position.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_JOYSTICK_BUTTONS		, 52, "inputNextFrame() stored the wrong previous joystick buttons.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_JOYSTICK_PREVIOUS_DELTA	, 53, "inputNextFrame() stored the wrong previous joystick delta.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_JOYSTICK_CURRENT_DELTA	, 54, "inputNextFrame() retained a current joystick delta.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_HISTORY_KEYBOARD		, 55, "inputNextFrame() changed keyboard history.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_HISTORY_MOUSE			, 56, "inputNextFrame() changed mouse history.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_TIMESTAMP_KEYBOARD		, 57, "inputNextFrame() changed keyboard timestamps.");
GDEFINE_ENUM_VALUED(INPUT_TEST_RESULT, FRAME_TIMESTAMP_MOUSE			, 58, "inputNextFrame() changed mouse timestamps.");

sttc ::llc::err_t testInputTypes(ATestError & errors) {
	LLC_TEST_CHECK(errors, INPUT_TEST_RESULT_JOYSTICK_BUTTON_TYPE, false == (::std::is_same_v<decltype(::llc::SInputJoystick{}.ButtonState), ::llc::u2_t>));
	LLC_TEST_CHECK(errors, INPUT_TEST_RESULT_MOUSE_BUTTON_TYPE, false == (::std::is_same_v<decltype(::llc::SInputMouse{}.ButtonState), ::llc::astu0_t<32>>));
	LLC_TEST_CHECK(errors, INPUT_TEST_RESULT_MOUSE_TIME_TYPE, false == (::std::is_same_v<decltype(::llc::SInputMouse{}.ButtonTime), ::llc::astf3_t<32>>));
	LLC_TEST_CHECK(errors, INPUT_TEST_RESULT_MOUSE_STRENGTH_TYPE, false == (::std::is_same_v<decltype(::llc::SInputMouse{}.ButtonStrength), ::llc::asts3_t<32>>));
	LLC_TEST_CHECK(errors, INPUT_TEST_RESULT_KEYBOARD_STATE_TYPE, false == (::std::is_same_v<decltype(::llc::SInputKeyboard{}.KeyState), ::llc::astu0_t<256>>));
	LLC_TEST_CHECK(errors, INPUT_TEST_RESULT_KEYBOARD_TIME_TYPE, false == (::std::is_same_v<decltype(::llc::SInputKeyboard{}.KeyTime), ::llc::astf3_t<256>>));
	LLC_TEST_CHECK(errors, INPUT_TEST_RESULT_KEYBOARD_STRENGTH_TYPE, false == (::std::is_same_v<decltype(::llc::SInputKeyboard{}.KeyStrength), ::llc::asts3_t<256>>));
	LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_JOYSTICK_PACKED_SIZE, sizeof(::llc::SInputJoystick) != 14, "Size:%u, expected:14.", (::llc::u2_t)sizeof(::llc::SInputJoystick));
	LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_MOUSE_PACKED_SIZE, sizeof(::llc::SInputMouse) != 554, "Size:%u, expected:554.", (::llc::u2_t)sizeof(::llc::SInputMouse));
	LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_KEYBOARD_PACKED_SIZE, sizeof(::llc::SInputKeyboard) != 4352, "Size:%u, expected:4352.", (::llc::u2_t)sizeof(::llc::SInputKeyboard));
	rtrn 0;
}

sttc ::llc::err_t testInputDefaults(ATestError & errors) {
	cnst ::llc::SInput input = {};
	for(::llc::u2_t iKey = 0; iKey < input.KeyboardCurrent.KeyState.size(); ++iKey) {
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_DEFAULT_KEY_STATE, input.KeyboardCurrent.KeyState[iKey], "Key:%u state:%u.", iKey, input.KeyboardCurrent.KeyState[iKey]);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_DEFAULT_KEY_TIME, input.KeyboardCurrent.KeyTime[iKey], "Key:%u time:%f.", iKey, input.KeyboardCurrent.KeyTime[iKey]);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_DEFAULT_KEY_STRENGTH, input.KeyboardCurrent.KeyStrength[iKey], "Key:%u strength:%" LLC_FMT_S3 ".", iKey, input.KeyboardCurrent.KeyStrength[iKey]);
	}
	for(::llc::u2_t iButton = 0; iButton < input.MouseCurrent.ButtonState.size(); ++iButton) {
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_DEFAULT_BUTTON_STATE, input.MouseCurrent.ButtonState[iButton], "Button:%u state:%u.", iButton, input.MouseCurrent.ButtonState[iButton]);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_DEFAULT_BUTTON_TIME, input.MouseCurrent.ButtonTime[iButton], "Button:%u time:%f.", iButton, input.MouseCurrent.ButtonTime[iButton]);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_DEFAULT_BUTTON_STRENGTH, input.MouseCurrent.ButtonStrength[iButton], "Button:%u strength:%" LLC_FMT_S3 ".", iButton, input.MouseCurrent.ButtonStrength[iButton]);
	}
	rtrn 0;
}

sttc ::llc::err_t testInputTransitions(ATestError & errors) {
	::llc::SInput input = {};
	for(::llc::u2_t iKey = 0; iKey < input.KeyboardCurrent.KeyState.size(); ++iKey) {
		::llc::u0_c key = (::llc::u0_t)iKey;
		input.KeyboardPrevious.KeyState[iKey] = 0; input.KeyboardCurrent.KeyState[iKey] = 0;
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_KEY_IDLE_DOWN, input.KeyDown(key), "Key:%u.", iKey);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_KEY_IDLE_UP, input.KeyUp(key), "Key:%u.", iKey);
		input.KeyboardCurrent.KeyState[iKey] = 1;
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_KEY_PRESS_DOWN, false == input.KeyDown(key), "Key:%u.", iKey);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_KEY_PRESS_UP, input.KeyUp(key), "Key:%u.", iKey);
		input.KeyboardPrevious.KeyState[iKey] = 1;
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_KEY_HELD_DOWN, input.KeyDown(key), "Key:%u.", iKey);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_KEY_HELD_UP, input.KeyUp(key), "Key:%u.", iKey);
		input.KeyboardCurrent.KeyState[iKey] = 0;
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_KEY_RELEASE_DOWN, input.KeyDown(key), "Key:%u.", iKey);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_KEY_RELEASE_UP, false == input.KeyUp(key), "Key:%u.", iKey);
		input.KeyboardPrevious.KeyState[iKey] = 0;
	}
	for(::llc::u2_t iButton = 0; iButton < input.MouseCurrent.ButtonState.size(); ++iButton) {
		::llc::u0_c button = (::llc::u0_t)iButton;
		input.MousePrevious.ButtonState[iButton] = 0; input.MouseCurrent.ButtonState[iButton] = 0;
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_BUTTON_IDLE_DOWN, input.ButtonDown(button), "Button:%u.", iButton);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_BUTTON_IDLE_UP, input.ButtonUp(button), "Button:%u.", iButton);
		input.MouseCurrent.ButtonState[iButton] = 1;
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_BUTTON_PRESS_DOWN, false == input.ButtonDown(button), "Button:%u.", iButton);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_BUTTON_PRESS_UP, input.ButtonUp(button), "Button:%u.", iButton);
		input.MousePrevious.ButtonState[iButton] = 1;
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_BUTTON_HELD_DOWN, input.ButtonDown(button), "Button:%u.", iButton);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_BUTTON_HELD_UP, input.ButtonUp(button), "Button:%u.", iButton);
		input.MouseCurrent.ButtonState[iButton] = 0;
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_BUTTON_RELEASE_DOWN, input.ButtonDown(button), "Button:%u.", iButton);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_BUTTON_RELEASE_UP, false == input.ButtonUp(button), "Button:%u.", iButton);
		input.MousePrevious.ButtonState[iButton] = 0;
	}
	for(::llc::u2_t iButton = input.MouseCurrent.ButtonState.size(); iButton <= 0xFFU; ++iButton) {
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_BUTTON_OUT_OF_RANGE_DOWN, input.ButtonDown((::llc::u0_t)iButton), "Button:%u.", iButton);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_BUTTON_OUT_OF_RANGE_UP, input.ButtonUp((::llc::u0_t)iButton), "Button:%u.", iButton);
	}
	rtrn 0;
}

sttc ::llc::err_t testInputFrame(ATestError & errors) {
	::llc::SInput input = {};
	for(::llc::u2_t iKey = 0; iKey < input.KeyboardCurrent.KeyState.size(); ++iKey) {
		input.KeyboardCurrent.KeyState[iKey] = (::llc::u0_t)(iKey & 1U);
		input.KeyboardCurrent.KeyTime[iKey] = (::llc::f3_t)iKey + 0.25;
		input.KeyboardCurrent.KeyStrength[iKey] = -(::llc::s3_t)iKey;
	}
	for(::llc::u2_t iButton = 0; iButton < input.MouseCurrent.ButtonState.size(); ++iButton) {
		input.MouseCurrent.ButtonState[iButton] = (::llc::u0_t)(iButton & 1U);
		input.MouseCurrent.ButtonTime[iButton] = (::llc::f3_t)iButton + 0.5;
		input.MouseCurrent.ButtonStrength[iButton] = -(::llc::s3_t)iButton;
	}
	input.MouseCurrent.Position = {13, -21};
	input.MouseCurrent.Deltas = {5, -7, 11};

	LLC_TEST_REQUIRE(errors, INPUT_TEST_RESULT_FRAME_HISTORY_KEYBOARD, ::llc::failed(input.StateKeyboard.resize(1)));
	LLC_TEST_REQUIRE(errors, INPUT_TEST_RESULT_FRAME_HISTORY_MOUSE, ::llc::failed(input.StateMouse.resize(1)));
	input.StateKeyboard[0].KeyState[7] = 1;
	input.StateMouse[0].ButtonState[3] = 1;
	LLC_TEST_REQUIRE(errors, INPUT_TEST_RESULT_FRAME_TIMESTAMP_KEYBOARD, ::llc::failed(input.TimeStampKeyboard.push_back(1.25f)));
	LLC_TEST_REQUIRE(errors, INPUT_TEST_RESULT_FRAME_TIMESTAMP_MOUSE, ::llc::failed(input.TimeStampMouse.push_back(2.5f)));

	LLC_TEST_REQUIRE(errors, INPUT_TEST_RESULT_FRAME_JOYSTICK_COUNT, ::llc::failed(input.JoystickCurrent.resize(4)));
	for(::llc::u2_t iJoystick = 0; iJoystick < input.JoystickCurrent.size(); ++iJoystick) {
		input.JoystickCurrent[iJoystick].Position = {(::llc::s1_t)(iJoystick + 1), (::llc::s1_t)(iJoystick + 2)};
		input.JoystickCurrent[iJoystick].Deltas = {(::llc::s1_t)(iJoystick + 3), (::llc::s1_t)(iJoystick + 4), (::llc::s1_t)(iJoystick + 5)};
		input.JoystickCurrent[iJoystick].ButtonState = 1U << iJoystick;
	}

	::llc::inputNextFrame(input);

	for(::llc::u2_t iKey = 0; iKey < input.KeyboardCurrent.KeyState.size(); ++iKey) {
		::llc::u0_c expectedState = (::llc::u0_t)(iKey & 1U);
		::llc::f3_c expectedTime = (::llc::f3_t)iKey + 0.25;
		::llc::s3_c expectedStrength = -(::llc::s3_t)iKey;
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_KEY_PREVIOUS_STATE, input.KeyboardPrevious.KeyState[iKey] != expectedState, "Key:%u previous:%u, expected:%u.", iKey, input.KeyboardPrevious.KeyState[iKey], expectedState);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_KEY_PREVIOUS_TIME, input.KeyboardPrevious.KeyTime[iKey] != expectedTime, "Key:%u previous:%f, expected:%f.", iKey, input.KeyboardPrevious.KeyTime[iKey], expectedTime);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_KEY_PREVIOUS_STRENGTH, input.KeyboardPrevious.KeyStrength[iKey] != expectedStrength, "Key:%u previous:%" LLC_FMT_S3 ", expected:%" LLC_FMT_S3 ".", iKey, input.KeyboardPrevious.KeyStrength[iKey], expectedStrength);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_KEY_CURRENT_STATE, input.KeyboardCurrent.KeyState[iKey] != expectedState, "Key:%u current:%u, expected:%u.", iKey, input.KeyboardCurrent.KeyState[iKey], expectedState);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_KEY_CURRENT_TIME, input.KeyboardCurrent.KeyTime[iKey] != expectedTime, "Key:%u current:%f, expected:%f.", iKey, input.KeyboardCurrent.KeyTime[iKey], expectedTime);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_KEY_CURRENT_STRENGTH, input.KeyboardCurrent.KeyStrength[iKey] != expectedStrength, "Key:%u current:%" LLC_FMT_S3 ", expected:%" LLC_FMT_S3 ".", iKey, input.KeyboardCurrent.KeyStrength[iKey], expectedStrength);
	}
	for(::llc::u2_t iButton = 0; iButton < input.MouseCurrent.ButtonState.size(); ++iButton) {
		::llc::u0_c expectedState = (::llc::u0_t)(iButton & 1U);
		::llc::f3_c expectedTime = (::llc::f3_t)iButton + 0.5;
		::llc::s3_c expectedStrength = -(::llc::s3_t)iButton;
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_BUTTON_PREVIOUS_STATE, input.MousePrevious.ButtonState[iButton] != expectedState, "Button:%u previous:%u, expected:%u.", iButton, input.MousePrevious.ButtonState[iButton], expectedState);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_BUTTON_PREVIOUS_TIME, input.MousePrevious.ButtonTime[iButton] != expectedTime, "Button:%u previous:%f, expected:%f.", iButton, input.MousePrevious.ButtonTime[iButton], expectedTime);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_BUTTON_PREVIOUS_STRENGTH, input.MousePrevious.ButtonStrength[iButton] != expectedStrength, "Button:%u previous:%" LLC_FMT_S3 ", expected:%" LLC_FMT_S3 ".", iButton, input.MousePrevious.ButtonStrength[iButton], expectedStrength);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_BUTTON_CURRENT_STATE, input.MouseCurrent.ButtonState[iButton] != expectedState, "Button:%u current:%u, expected:%u.", iButton, input.MouseCurrent.ButtonState[iButton], expectedState);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_BUTTON_CURRENT_TIME, input.MouseCurrent.ButtonTime[iButton] != expectedTime, "Button:%u current:%f, expected:%f.", iButton, input.MouseCurrent.ButtonTime[iButton], expectedTime);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_BUTTON_CURRENT_STRENGTH, input.MouseCurrent.ButtonStrength[iButton] != expectedStrength, "Button:%u current:%" LLC_FMT_S3 ", expected:%" LLC_FMT_S3 ".", iButton, input.MouseCurrent.ButtonStrength[iButton], expectedStrength);
	}
	LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_MOUSE_PREVIOUS_POSITION, input.MousePrevious.Position.x != 13, "Previous x:%i, expected:13.", input.MousePrevious.Position.x);
	LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_MOUSE_PREVIOUS_POSITION, input.MousePrevious.Position.y != -21, "Previous y:%i, expected:-21.", input.MousePrevious.Position.y);
	LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_MOUSE_CURRENT_POSITION, input.MouseCurrent.Position.x != 13, "Current x:%i, expected:13.", input.MouseCurrent.Position.x);
	LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_MOUSE_CURRENT_POSITION, input.MouseCurrent.Position.y != -21, "Current y:%i, expected:-21.", input.MouseCurrent.Position.y);
	LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_MOUSE_PREVIOUS_DELTA, input.MousePrevious.Deltas.x != 5, "Previous x:%i, expected:5.", input.MousePrevious.Deltas.x);
	LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_MOUSE_PREVIOUS_DELTA, input.MousePrevious.Deltas.y != -7, "Previous y:%i, expected:-7.", input.MousePrevious.Deltas.y);
	LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_MOUSE_PREVIOUS_DELTA, input.MousePrevious.Deltas.z != 11, "Previous z:%i, expected:11.", input.MousePrevious.Deltas.z);
	LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_MOUSE_CURRENT_DELTA, input.MouseCurrent.Deltas.x, "Current x:%i, expected:0.", input.MouseCurrent.Deltas.x);
	LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_MOUSE_CURRENT_DELTA, input.MouseCurrent.Deltas.y, "Current y:%i, expected:0.", input.MouseCurrent.Deltas.y);
	LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_MOUSE_CURRENT_DELTA, input.MouseCurrent.Deltas.z, "Current z:%i, expected:0.", input.MouseCurrent.Deltas.z);

	LLC_TEST_REQUIREF(errors, INPUT_TEST_RESULT_FRAME_JOYSTICK_COUNT, input.JoystickPrevious.size() != input.JoystickCurrent.size(), "Previous:%u, current:%u.", input.JoystickPrevious.size(), input.JoystickCurrent.size());
	for(::llc::u2_t iJoystick = 0; iJoystick < input.JoystickCurrent.size(); ++iJoystick) {
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_JOYSTICK_POSITION, input.JoystickPrevious[iJoystick].Position.x != (::llc::s1_t)(iJoystick + 1), "Joystick:%u previous x:%i.", iJoystick, input.JoystickPrevious[iJoystick].Position.x);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_JOYSTICK_POSITION, input.JoystickPrevious[iJoystick].Position.y != (::llc::s1_t)(iJoystick + 2), "Joystick:%u previous y:%i.", iJoystick, input.JoystickPrevious[iJoystick].Position.y);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_JOYSTICK_BUTTONS, input.JoystickPrevious[iJoystick].ButtonState != (1U << iJoystick), "Joystick:%u previous buttons:0x%X.", iJoystick, input.JoystickPrevious[iJoystick].ButtonState);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_JOYSTICK_PREVIOUS_DELTA, input.JoystickPrevious[iJoystick].Deltas.x != (::llc::s1_t)(iJoystick + 3), "Joystick:%u previous x:%i.", iJoystick, input.JoystickPrevious[iJoystick].Deltas.x);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_JOYSTICK_PREVIOUS_DELTA, input.JoystickPrevious[iJoystick].Deltas.y != (::llc::s1_t)(iJoystick + 4), "Joystick:%u previous y:%i.", iJoystick, input.JoystickPrevious[iJoystick].Deltas.y);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_JOYSTICK_PREVIOUS_DELTA, input.JoystickPrevious[iJoystick].Deltas.z != (::llc::s1_t)(iJoystick + 5), "Joystick:%u previous z:%i.", iJoystick, input.JoystickPrevious[iJoystick].Deltas.z);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_JOYSTICK_CURRENT_DELTA, input.JoystickCurrent[iJoystick].Deltas.x, "Joystick:%u current x:%i.", iJoystick, input.JoystickCurrent[iJoystick].Deltas.x);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_JOYSTICK_CURRENT_DELTA, input.JoystickCurrent[iJoystick].Deltas.y, "Joystick:%u current y:%i.", iJoystick, input.JoystickCurrent[iJoystick].Deltas.y);
		LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_JOYSTICK_CURRENT_DELTA, input.JoystickCurrent[iJoystick].Deltas.z, "Joystick:%u current z:%i.", iJoystick, input.JoystickCurrent[iJoystick].Deltas.z);
	}
	LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_HISTORY_KEYBOARD, input.StateKeyboard.size() != 1, "History size:%u, expected:1.", input.StateKeyboard.size());
	LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_HISTORY_KEYBOARD, input.StateKeyboard[0].KeyState[7] != 1, "History state:%u, expected:1.", input.StateKeyboard[0].KeyState[7]);
	LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_HISTORY_MOUSE, input.StateMouse.size() != 1, "History size:%u, expected:1.", input.StateMouse.size());
	LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_HISTORY_MOUSE, input.StateMouse[0].ButtonState[3] != 1, "History state:%u, expected:1.", input.StateMouse[0].ButtonState[3]);
	LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_TIMESTAMP_KEYBOARD, input.TimeStampKeyboard.size() != 1, "Timestamp count:%u, expected:1.", input.TimeStampKeyboard.size());
	LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_TIMESTAMP_KEYBOARD, input.TimeStampKeyboard[0] != 1.25f, "Timestamp:%f, expected:1.25.", input.TimeStampKeyboard[0]);
	LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_TIMESTAMP_MOUSE, input.TimeStampMouse.size() != 1, "Timestamp count:%u, expected:1.", input.TimeStampMouse.size());
	LLC_TEST_CHECKF(errors, INPUT_TEST_RESULT_FRAME_TIMESTAMP_MOUSE, input.TimeStampMouse[0] != 2.5f, "Timestamp:%f, expected:2.5.", input.TimeStampMouse[0]);
	rtrn 0;
}

::llc::err_t testInput(ATestError & errors) {
	if_fail_fe(::testInputTypes(errors));
	if_fail_fe(::testInputDefaults(errors));
	if_fail_fe(::testInputTransitions(errors));
	if_fail_fe(::testInputFrame(errors));
	rtrn 0;
}
