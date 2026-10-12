#include "llc_gui_inputbox.h"

#include "llc_label.h"
#include "llc_view_serialize.h"

#include <limits.h>

stacxpr	::llc::n2u1_t		VIRTUAL_KEYBOARD_KEY_SIZE_437	= {14, 22};
stacxpr	::llc::n2u1_t		VIRTUAL_KEYBOARD_COMMAND_SIZE	= {88, 20};
stacxpr	::llc::SYSTEM_KEY	VIRTUAL_KEYBOARD_COMMANDS	[]	=
	{ ::llc::SYSTEM_KEY_Escape
	, ::llc::SYSTEM_KEY_Enter
	, ::llc::SYSTEM_KEY_Backspace
	, ::llc::SYSTEM_KEY_Clear
	, ::llc::SYSTEM_KEY_Left
	, ::llc::SYSTEM_KEY_Right
	, ::llc::SYSTEM_KEY_Up
	, ::llc::SYSTEM_KEY_Down
	};

sttc ::llc::err_t virtualKeyboardSetupFail(::llc::SGUI & gui, ::llc::SVirtualKeyboard & keyboard, ::llc::err_t error) {
	if(keyboard.IdRoot != ::llc::CID_INVALID)
		::llc::controlDelete(gui, keyboard.IdRoot);
	keyboard.IdRoot = ::llc::CID_INVALID;
	keyboard.Characters.clear();
	keyboard.Commands.clear();
	rtrn error;
}

::llc::err_t llc::virtualKeyboardSetup437(::llc::SGUI & gui, ::llc::SVirtualKeyboard & keyboard) {
	::llc::au1_t keys = {};
	for(::llc::u1_t iCharacter = 1; iCharacter < 255; ++iCharacter) {
		if_fail_fe(keys.push_back((::llc::u0_t)iCharacter));
	}
	if_fail_fe(keys.push_back('!'));
	if_fail_fe(keys.push_back('?'));
	if_fail_fe(::llc::virtualKeyboardSetup(gui, keyboard, 32, ::VIRTUAL_KEYBOARD_KEY_SIZE_437, keys));
	gui.Controls.SetHidden(keyboard.IdRoot, true);
	rtrn keyboard.IdRoot;
}

::llc::err_t llc::virtualKeyboardSetup(::llc::SGUI & gui, ::llc::SVirtualKeyboard & keyboard, ::llc::u0_t rowWidth, ::llc::n2u1_c & keySize, ::llc::vcu1_c & keys) {
	if_true_fef(keyboard.IdRoot != ::llc::CID_INVALID, "Virtual keyboard root already assigned:%i.", keyboard.IdRoot);
	if_zero_fef(rowWidth, "%s", "Virtual keyboard row width cannot be zero.");
	if_true_fef(0 == keySize.x || 0 == keySize.y, "Virtual keyboard key size cannot be zero:{%u, %u}.", keySize.x, keySize.y);

	cnst ::llc::u2_t rowCount = keys.size() / rowWidth + one_if(keys.size() % rowWidth);
	cnst ::llc::u3_t width = (::llc::u3_t)keySize.x * rowWidth + 4U + ::VIRTUAL_KEYBOARD_COMMAND_SIZE.x;
	cnst ::llc::u3_t heightCharacters = (::llc::u3_t)keySize.y * rowCount + 4U;
	cnst ::llc::u3_t heightCommands = (::llc::u3_t)::llc::size(::VIRTUAL_KEYBOARD_COMMANDS) * ::VIRTUAL_KEYBOARD_COMMAND_SIZE.y + 4U;
	cnst ::llc::u3_t height = ::llc::max(heightCharacters, heightCommands);
	if_true_fef(width > INT16_MAX || height > INT16_MAX, "Virtual keyboard size exceeds signed 16-bit control geometry:{%" LLC_FMT_U3 ", %" LLC_FMT_U3 "}.", width, height);
	if_fail_fe(keyboard.Characters.resize(keys.size()));
	if_fail_fe(keyboard.Commands.resize(::llc::size(::VIRTUAL_KEYBOARD_COMMANDS)));

	keyboard.IdRoot = ::llc::controlCreate(gui);
	if_true_fef(keyboard.IdRoot < 0, "Failed to create virtual keyboard root:%i.", keyboard.IdRoot);
	::llc::SControlPlacement & rootPlacement = gui.Controls.Placement[keyboard.IdRoot];
	rootPlacement.Area.Size = {(::llc::s1_t)width, (::llc::s1_t)height};
	rootPlacement.Align = ::llc::ALIGN_CENTER_BOTTOM;

	for(::llc::u2_t iCharacter = 0; iCharacter < keys.size(); ++iCharacter) {
		cnst ::llc::cid_t idControl = ::llc::controlCreateChild(gui, keyboard.IdRoot);
		if(idControl < 0)
			rtrn ::virtualKeyboardSetupFail(gui, keyboard, idControl);
		::llc::SControlPlacement & placement = gui.Controls.Placement[idControl];
		placement.Area.Size = keySize.s1();
		placement.Area.Offset.x = (::llc::s1_t)((iCharacter % rowWidth) * keySize.x);
		placement.Area.Offset.y = (::llc::s1_t)((iCharacter / rowWidth) * keySize.y);
		placement.Border = {1, 1, 1, 1};
		gui.Controls.Text[idControl].Align = ::llc::ALIGN_CENTER;
		cnst ::llc::sc_t characterText[] = {(::llc::sc_t)keys[iCharacter], 0};
		cnst ::llc::err_t textResult = ::llc::controlTextSet(gui, idControl, ::llc::label(characterText));
		if(::llc::failed(textResult))
			rtrn ::virtualKeyboardSetupFail(gui, keyboard, textResult);
		keyboard.Characters[iCharacter] = {idControl, keys[iCharacter]};
	}

	for(::llc::u2_t iCommand = 0; iCommand < ::llc::size(::VIRTUAL_KEYBOARD_COMMANDS); ++iCommand) {
		cnst ::llc::cid_t idControl = ::llc::controlCreateChild(gui, keyboard.IdRoot);
		if(idControl < 0)
			rtrn ::virtualKeyboardSetupFail(gui, keyboard, idControl);
		::llc::SControlPlacement & placement = gui.Controls.Placement[idControl];
		placement.Area.Size = ::VIRTUAL_KEYBOARD_COMMAND_SIZE.s1();
		placement.Area.Offset.y = (::llc::s1_t)(iCommand * ::VIRTUAL_KEYBOARD_COMMAND_SIZE.y);
		placement.Align = ::llc::ALIGN_TOP_RIGHT;
		placement.Border = {1, 1, 1, 1};
		gui.Controls.Text[idControl].Align = ::llc::ALIGN_CENTER;
		cnst ::llc::err_t textResult = ::llc::controlTextSet(gui, idControl, ::llc::get_value_label(::VIRTUAL_KEYBOARD_COMMANDS[iCommand]));
		if(::llc::failed(textResult))
			rtrn ::virtualKeyboardSetupFail(gui, keyboard, textResult);
		keyboard.Commands[iCommand] = {idControl, ::VIRTUAL_KEYBOARD_COMMANDS[iCommand]};
	}
	rtrn keyboard.IdRoot;
}

#ifndef LLC_ATMEL
::llc::err_t llc::SInputBox::Update(::llc::SGUI & gui, cnst ::llc::SVirtualKeyboard & virtualKeyboard, ::llc::view<cnst ::llc::pobj<::llc::SEventSystem>> frameEvents, ::llc::vcid processableControls) {
	if(false == Editing)
		rtrn 0;

	::llc::TQueueSystemEvent systemEvents = frameEvents;
	::llc::cid_t handledControl = ::llc::CID_INVALID;
	cnst ::llc::err_t processResult = ::llc::guiProcessControls(gui, processableControls, [&virtualKeyboard, &systemEvents, &handledControl](::llc::cid_t iControl) {
		cnst ::llc::err_t keyboardResult = ::llc::virtualKeyboardHandleEvent(virtualKeyboard, iControl, systemEvents);
		if(::llc::failed(keyboardResult))
			rtrn keyboardResult;
		if(keyboardResult)
			handledControl = iControl;
		rtrn keyboardResult;
	});
	if_fail_fe(processResult);

	bool enterKeyPressed = false;
	for(::llc::u2_t iEvent = 0; iEvent < systemEvents.size(); ++iEvent) {
		if_zero_fef(systemEvents[iEvent].get_ref(), "Null system event at index:%u.", iEvent);
		switch(systemEvents[iEvent]->Type) {
		case ::llc::SYSTEM_EVENT_Text:
			if_fail_fe(::llc::eventExtractAndHandle<::llc::EVENT_TEXT>(*systemEvents[iEvent], [this, &gui](cnst ::llc::SEView<::llc::EVENT_TEXT> & eventText) {
				if(eventText.Type != ::llc::EVENT_TEXT_Char)
					rtrn 0;
				::llc::vcu0_t input = eventText.Data;
				::llc::u2_t character = {};
				if_fail_fe(::llc::loadPOD(input, character));
				if_true_fef(input.size(), "Text character payload has %u trailing bytes.", input.size());
				if(Text.size() < MaxLength && character >= 0x20 && character <= 0xFF) {
					if_fail_fe(Text.push_back((::llc::sc_t)character));
					if_fail_fe(::llc::controlTextSet(gui, IdText, {Text.begin(), Text.size()}));
				}
				rtrn 0;
			}));
			break;
		case ::llc::SYSTEM_EVENT_Keyboard:
			if_fail_fe(::llc::eventExtractAndHandle<::llc::EVENT_KEYBOARD>(*systemEvents[iEvent], [this, &gui, &enterKeyPressed](cnst ::llc::SEView<::llc::EVENT_KEYBOARD> & eventKeyboard) {
				if(eventKeyboard.Type != ::llc::EVENT_KEYBOARD_Down)
					rtrn 0;
				::llc::vcu0_t input = eventKeyboard.Data;
				::llc::u0_t key = {};
				if_fail_fe(::llc::loadPOD(input, key));
				if_true_fef(input.size(), "Keyboard payload has %u trailing bytes.", input.size());
				switch((::llc::SYSTEM_KEY)key) {
				case ::llc::SYSTEM_KEY_Escape:		Edit(gui, false);		break;
				case ::llc::SYSTEM_KEY_Enter:		enterKeyPressed = true;	break;
				case ::llc::SYSTEM_KEY_Backspace:
					if(Text.size()) {
						if_fail_fe(Text.pop_back());
						if_fail_fe(::llc::controlTextSet(gui, IdText, {Text.begin(), Text.size()}));
					}
					break;
				case ::llc::SYSTEM_KEY_Clear:
					Text.clear();
					if_fail_fe(::llc::controlTextSet(gui, IdText, {Text.begin(), Text.size()}));
					break;
				}
				rtrn 0;
			}));
			break;
		}
	}
	rtrn enterKeyPressed ? INT_MAX : handledControl != ::llc::CID_INVALID ? handledControl : 0;
}
#endif

::llc::err_t llc::inputBoxCreate(::llc::SInputBox & inputBox, ::llc::SGUI & gui, ::llc::cid_t iParent) {
	if_true_fef(inputBox.IdRoot != ::llc::CID_INVALID, "Input-box root already assigned:%i.", inputBox.IdRoot);
	if_true_fef(iParent != ::llc::CID_INVALID && ::llc::controlInvalid(gui, iParent), "Invalid input-box parent:%i.", iParent);
	inputBox.IdRoot = ::llc::controlCreate(gui);
	if_true_fef(inputBox.IdRoot < 0, "Failed to create input-box root:%i.", inputBox.IdRoot);
	if(iParent != ::llc::CID_INVALID && ::llc::failed(::llc::controlSetParent(gui, inputBox.IdRoot, iParent))) {
		::llc::controlDelete(gui, inputBox.IdRoot);
		inputBox.IdRoot = ::llc::CID_INVALID;
		rtrn -1;
	}

	inputBox.IdText = ::llc::controlCreateChild(gui, inputBox.IdRoot);
	if(inputBox.IdText < 0) {
		::llc::controlDelete(gui, inputBox.IdRoot);
		inputBox.IdRoot = ::llc::CID_INVALID;
		rtrn inputBox.IdText;
	}
	gui.Controls.States[inputBox.IdRoot].Mask &= ~::llc::GUI_CONTROL_FLAG_Hovered;
	gui.Controls.Placement[inputBox.IdRoot].Border = {};
	gui.Controls.Placement[inputBox.IdRoot].Margin = {};
	gui.Controls.Draw[inputBox.IdRoot].NoClient = true;
	gui.Controls.Placement[inputBox.IdText].Border = {2, 2, 2, 2};
	gui.Controls.Placement[inputBox.IdText].Margin = {1, 1, 1, 1};
	gui.Controls.Placement[inputBox.IdText].Area.Size = {0, 22};
	gui.Controls.Text[inputBox.IdText].Align = ::llc::ALIGN_CENTER;

	cnst ::llc::err_t keyboardResult = ::llc::virtualKeyboardSetup437(gui, inputBox.VirtualKeyboard);
	if(::llc::failed(keyboardResult)) {
		::llc::controlDelete(gui, inputBox.IdRoot);
		inputBox.IdRoot = inputBox.IdText = ::llc::CID_INVALID;
		rtrn keyboardResult;
	}
	if(::llc::failed(::llc::controlSetParent(gui, inputBox.VirtualKeyboard.IdRoot, inputBox.IdRoot))) {
		::llc::controlDelete(gui, inputBox.VirtualKeyboard.IdRoot);
		::llc::controlDelete(gui, inputBox.IdRoot);
		inputBox.IdRoot = inputBox.IdText = ::llc::CID_INVALID;
		inputBox.VirtualKeyboard = {};
		rtrn -1;
	}
	::llc::SControlPlacement & textPlacement = gui.Controls.Placement[inputBox.IdText];
	::llc::SControlPlacement & keyboardPlacement = gui.Controls.Placement[inputBox.VirtualKeyboard.IdRoot];
	keyboardPlacement.Align = textPlacement.Align;
	keyboardPlacement.Area.Offset.y = textPlacement.Area.Offset.y + textPlacement.Area.Size.y;
	textPlacement.Area.Size.x = keyboardPlacement.Area.Size.x;
	gui.Controls.Placement[inputBox.IdRoot].Area.Size = {textPlacement.Area.Size.x, (::llc::s1_t)(textPlacement.Area.Size.y + keyboardPlacement.Area.Size.y)};
	inputBox.Edit(gui, inputBox.Editing);
	rtrn inputBox.IdRoot;
}
