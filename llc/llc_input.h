#include "llc_array_pod.h"
#include "llc_n3.h"
#include "llc_array_static.h"
#include "llc_system_key.h"

#ifndef LLC_INPUT_H_23627
#define LLC_INPUT_H_23627

namespace llc
{
#pragma pack(push, 1)
	struct SInputJoystick {
		::llc::n2s1_t				Position			= {};
		::llc::n3s1_t				Deltas				= {};
		::llc::u2_t				ButtonState			= 0;
	};
	struct SInputMouse {
		::llc::n2s1_t				Position			= {};
		::llc::n3s1_t				Deltas				= {};
		::llc::astu0_t<32>			ButtonState			= {};
		::llc::astf3_t<32>			ButtonTime			= {};
		::llc::asts3_t<32>			ButtonStrength		= {};
	};

	struct SInputKeyboard {
		::llc::astu0_t<256>			KeyState			= {};
		::llc::astf3_t<256>			KeyTime				= {};
		::llc::asts3_t<256>			KeyStrength			= {};
	};
#pragma pack(pop)
	stin void					inputKeyboardStateSet	(SInputKeyboard & state, u0_t keyCode, b8_t pressed) {
		if(keyCode)
			state.KeyState[keyCode] = pressed;
		switch(keyCode) {
		default: break;
		case ::llc::SYSTEM_KEY_LShift:
		case ::llc::SYSTEM_KEY_RShift	: state.KeyState[::llc::SYSTEM_KEY_Shift] = pressed; break;
		case ::llc::SYSTEM_KEY_LControl:
		case ::llc::SYSTEM_KEY_RControl	: state.KeyState[::llc::SYSTEM_KEY_Control] = pressed; break;
		case ::llc::SYSTEM_KEY_LMenu:
		case ::llc::SYSTEM_KEY_RMenu		: state.KeyState[::llc::SYSTEM_KEY_Menu] = pressed; break;
		}
	}

	struct SInput {
		::llc::apod<SInputKeyboard>	StateKeyboard		= {};
		::llc::apod<SInputMouse>	StateMouse			= {};
		::llc::af2_t				TimeStampKeyboard	= {};
		::llc::af2_t				TimeStampMouse		= {};

		::llc::apod<SInputJoystick>	JoystickPrevious	= {};
		::llc::apod<SInputJoystick>	JoystickCurrent		= {};

		SInputKeyboard				KeyboardPrevious	= {};
		SInputKeyboard				KeyboardCurrent		= {};
		SInputMouse					MousePrevious		= {};
		SInputMouse					MouseCurrent		= {};

		bool						ButtonUp			(::llc::u0_t buttonIndex)	const	{ return buttonIndex < MouseCurrent.ButtonState.size() && MouseCurrent.ButtonState[buttonIndex] == 0 && MousePrevious.ButtonState[buttonIndex] != 0; }
		bool						ButtonDown			(::llc::u0_t buttonIndex)	const	{ return buttonIndex < MouseCurrent.ButtonState.size() && MouseCurrent.ButtonState[buttonIndex] != 0 && MousePrevious.ButtonState[buttonIndex] == 0; }
		bool						KeyUp				(::llc::u0_t keyIndex)		const	{ return KeyboardCurrent.KeyState[keyIndex] == 0 && KeyboardPrevious.KeyState[keyIndex] != 0; }
		bool						KeyDown				(::llc::u0_t keyIndex)		const	{ return KeyboardCurrent.KeyState[keyIndex] != 0 && KeyboardPrevious.KeyState[keyIndex] == 0; }
	};

	stin err_t				inputKeyboardStateSet	(SInput & input, u0_t keyCode, b8_t pressed, s2_t deviceId, f2_t timeStamp = {}) {
		::llc::inputKeyboardStateSet(input.KeyboardCurrent, keyCode, pressed);
		if(0 > deviceId)
			return 0;
		u2_c	deviceIndex	= (u2_t)deviceId;
		if(input.StateKeyboard.size() <= deviceIndex)
			if_fail_fe(::llc::resize(deviceIndex + 1, input.StateKeyboard, input.TimeStampKeyboard));
		::llc::inputKeyboardStateSet(input.StateKeyboard[deviceIndex], keyCode, pressed);
		input.TimeStampKeyboard[deviceIndex] = timeStamp;
		return 0;
	}

	inln void					inputNextFrame		(::llc::SInput & input) {
		input.KeyboardPrevious		= input.KeyboardCurrent;
		input.MousePrevious			= input.MouseCurrent;
		input.MouseCurrent.Deltas	= {};
		input.JoystickPrevious		= input.JoystickCurrent;
		for(::llc::u2_t iJoystick = 0; iJoystick < input.JoystickCurrent.size(); ++iJoystick)
			input.JoystickCurrent[iJoystick].Deltas = {};
	}
} // namespace

#endif // LLC_INPUT_H_23627
