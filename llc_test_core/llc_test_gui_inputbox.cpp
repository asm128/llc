#include "llc_gui_inputbox.h"

#include "llc_test_core.h"

#include <limits.h>

GDEFINE_ENUM_TYPE(GUI_INPUTBOX_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, DEFAULT_ROOT				,  0, "A default input box has a valid root control.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, DEFAULT_TEXT				,  1, "A default input box has a valid text control.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, DEFAULT_LENGTH			,  2, "A default input box has the wrong maximum length.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, DEFAULT_EDITING			,  3, "A default input box is editing.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, ZERO_ROW_RESULT			,  4, "Virtual-keyboard setup accepted a zero row width.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, ZERO_ROW_CONTROLS		,  5, "Rejected virtual-keyboard setup created controls.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, ZERO_ROW_ROOT			,  6, "Rejected virtual-keyboard setup assigned a root.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, SETUP_RESULT				,  7, "Virtual-keyboard setup failed.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, REUSED_ROOT				,  8, "Virtual-keyboard setup did not reuse the first free control slot.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, ROOT_WIDTH				,  9, "A virtual keyboard has the wrong width.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, ROOT_HEIGHT				, 10, "A virtual keyboard has the wrong height.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, ROOT_ALIGN				, 11, "A virtual keyboard has the wrong root alignment.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, CHARACTER_COUNT			, 12, "A virtual keyboard has the wrong character count.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, COMMAND_COUNT			, 13, "A virtual keyboard has the wrong command count.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, CHARACTER_ID				, 14, "A virtual keyboard recorded the wrong character control.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, CHARACTER_VALUE			, 15, "A virtual keyboard recorded the wrong character value.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, CHARACTER_PARENT		, 16, "A virtual-keyboard character has the wrong parent.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, CHARACTER_SIZE			, 17, "A virtual-keyboard character has the wrong size.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, CHARACTER_OFFSET		, 18, "A virtual-keyboard character has the wrong offset.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, CHARACTER_TEXT_SIZE		, 19, "A virtual-keyboard character has the wrong caption length.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, CHARACTER_TEXT_VALUE	, 20, "A virtual-keyboard character has the wrong caption.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, COMMAND_KEY				, 21, "A virtual keyboard recorded the wrong command key.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, COMMAND_PARENT			, 22, "A virtual-keyboard command has the wrong parent.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, COMMAND_SIZE				, 23, "A virtual-keyboard command has the wrong size.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, COMMAND_OFFSET			, 24, "A virtual-keyboard command has the wrong offset.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, COMMAND_ALIGN			, 25, "A virtual-keyboard command has the wrong alignment.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, COMMAND_TEXT				, 26, "A virtual-keyboard command has the wrong caption.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, REENTRY_RESULT			, 27, "Virtual-keyboard setup accepted a second root.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, CP437_RESULT				, 28, "CP437 virtual-keyboard setup failed.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, CP437_CHARACTER_COUNT	, 29, "The CP437 virtual keyboard has the wrong character count.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, CP437_FIRST				, 30, "The CP437 virtual keyboard has the wrong first character.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, CP437_LAST				, 31, "The CP437 virtual keyboard has the wrong last character.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, CP437_WIDTH				, 32, "The CP437 virtual keyboard has the wrong width.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, CP437_HEIGHT				, 33, "The CP437 virtual keyboard has the wrong height.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, CP437_HIDDEN				, 34, "The standalone CP437 virtual keyboard is visible.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, CREATE_RESULT				, 35, "Input-box creation failed.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, ROOT_PARENT				, 36, "An input box has the wrong parent.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, TEXT_PARENT				, 37, "An input-box text control has the wrong parent.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, KEYBOARD_PARENT			, 38, "An input-box keyboard has the wrong parent.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, INPUT_WIDTH				, 39, "An input box has the wrong width.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, INPUT_HEIGHT				, 40, "An input box has the wrong height.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, TEXT_WIDTH				, 41, "An input-box text control has the wrong width.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, TEXT_HEIGHT				, 42, "An input-box text control has the wrong height.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, KEYBOARD_OFFSET			, 43, "An input-box keyboard has the wrong vertical offset.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, INITIAL_HIDDEN			, 44, "A newly created input box is visible while not editing.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, EDIT_RESULT				, 45, "Input-box edit mode returned the wrong result.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, EDIT_STATE				, 46, "Input-box edit mode stored the wrong state.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, EDIT_VISIBILITY			, 47, "Input-box edit mode set the wrong visibility.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, SET_TEXT_RESULT			, 48, "Input-box text assignment failed.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, SET_TEXT_STORAGE			, 49, "Input-box text assignment stored the wrong text.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, SET_TEXT_CONTROL			, 50, "Input-box text assignment left the control caption stale.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, INACTIVE_RESULT			, 51, "An inactive input box processed input.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, INACTIVE_TEXT			, 52, "An inactive input box changed its text.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, CHARACTER_RESULT			, 53, "A virtual character returned the wrong handled control.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, CHARACTER_APPEND			, 54, "A virtual character was not appended.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, CHARACTER_CONTROL_TEXT	, 55, "A virtual character left the input control caption stale.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, CP437_APPEND				, 56, "A CP437 character was not appended intact.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, MAXIMUM_LENGTH			, 57, "An input box exceeded its maximum length.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, FRAME_CHARACTER			, 58, "A frame text event was not appended.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, UNREPRESENTABLE_TEXT		, 59, "An input box accepted a character outside its byte-text contract.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, BACKSPACE_RESULT			, 60, "Virtual backspace returned the wrong handled control.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, BACKSPACE_TEXT			, 61, "Virtual backspace removed the wrong character.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, CLEAR_RESULT				, 62, "Virtual clear returned the wrong handled control.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, CLEAR_TEXT				, 63, "Virtual clear did not empty the input box.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, ARROW_RESULT				, 64, "A virtual arrow returned the wrong handled control.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, ARROW_TEXT				, 65, "A virtual arrow changed input text.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, ESCAPE_RESULT			, 66, "Virtual escape returned the wrong handled control.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, ESCAPE_STATE				, 67, "Virtual escape did not leave edit mode.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, ESCAPE_VISIBILITY		, 68, "Virtual escape did not hide the input box.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, ENTER_RESULT				, 69, "Virtual enter did not return INT_MAX.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, MALFORMED_RESULT			, 70, "An input box accepted a truncated text payload.");
GDEFINE_ENUM_VALUED(GUI_INPUTBOX_TEST_RESULT, MALFORMED_TEXT			, 71, "A truncated text payload changed the input box.");

stacxpr ::llc::SYSTEM_KEY GUI_INPUTBOX_EXPECTED_COMMANDS[] =
	{ ::llc::SYSTEM_KEY_Escape
	, ::llc::SYSTEM_KEY_Enter
	, ::llc::SYSTEM_KEY_Backspace
	, ::llc::SYSTEM_KEY_Clear
	, ::llc::SYSTEM_KEY_Left
	, ::llc::SYSTEM_KEY_Right
	, ::llc::SYSTEM_KEY_Up
	, ::llc::SYSTEM_KEY_Down
	};

sttc ::llc::err_t inputBoxProcessControl(::llc::SInputBox & inputBox, ::llc::SGUI & gui, ::llc::cid_t idControl) {
	gui.Controls.SetAction(idControl, true);
	::llc::cid_t processable[] = {idControl};
	cnst ::llc::err_t result = inputBox.Update(gui, inputBox.VirtualKeyboard, {}, processable);
	gui.Controls.SetAction(idControl, false);
	rtrn result;
}

sttc ::llc::err_t testGUIInputBoxDefaults(ATestError & errors) {
	cnst ::llc::SInputBox inputBox = {};
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_DEFAULT_ROOT, inputBox.IdRoot != ::llc::CID_INVALID, "Root:%i, expected:%i.", inputBox.IdRoot, ::llc::CID_INVALID);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_DEFAULT_TEXT, inputBox.IdText != ::llc::CID_INVALID, "Text:%i, expected:%i.", inputBox.IdText, ::llc::CID_INVALID);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_DEFAULT_LENGTH, inputBox.MaxLength != 16, "Maximum length:%u, expected:16.", inputBox.MaxLength);
	LLC_TEST_CHECK(errors, GUI_INPUTBOX_TEST_RESULT_DEFAULT_EDITING, inputBox.Editing);
	rtrn 0;
}

sttc ::llc::err_t testGUIVirtualKeyboardSetup(ATestError & errors) {
	{
		::llc::SGUI gui = {};
		::llc::SVirtualKeyboard keyboard = {};
		cnst ::llc::u1_t keys[] = {'A'};
		cnst ::llc::err_t setupResult = ::llc::virtualKeyboardSetup(gui, keyboard, 0, {14, 22}, keys);
		LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_ZERO_ROW_RESULT, false == ::llc::failed(setupResult), "Setup result:%i.", setupResult);
		LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_ZERO_ROW_CONTROLS, gui.Controls.Entities.size(), "Control count:%u, expected:0.", gui.Controls.Entities.size());
		LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_ZERO_ROW_ROOT, keyboard.IdRoot != ::llc::CID_INVALID, "Root:%i, expected:%i.", keyboard.IdRoot, ::llc::CID_INVALID);
	}
	{
		::llc::SGUI gui = {};
		::llc::acid controls = {};
		for(::llc::u0_t iControl = 0; iControl < 5; ++iControl) {
			if_fail_fe(controls.push_back(::llc::controlCreate(gui)));
		}
		if_fail_fe(::llc::controlDelete(gui, controls[1]));
		if_fail_fe(::llc::controlDelete(gui, controls[3]));
		::llc::SVirtualKeyboard keyboard = {};
		cnst ::llc::u1_t keys[] = {'A', 'B', 0xFE};
		cnst ::llc::n2u1_t keySize = {14, 100};
		cnst ::llc::n2s1_t commandSize = {88, 20};
		cnst ::llc::err_t setupResult = ::llc::virtualKeyboardSetup(gui, keyboard, 2, keySize, keys);
		LLC_TEST_REQUIREF(errors, GUI_INPUTBOX_TEST_RESULT_SETUP_RESULT, ::llc::failed(setupResult), "Setup result:%i.", setupResult);
		LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_REUSED_ROOT, keyboard.IdRoot != controls[1], "Root:%i, expected reused slot:%i.", keyboard.IdRoot, controls[1]);
		LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_ROOT_WIDTH, gui.Controls.Placement[keyboard.IdRoot].Area.Size.x != 120, "Width:%i, expected:120.", gui.Controls.Placement[keyboard.IdRoot].Area.Size.x);
		LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_ROOT_HEIGHT, gui.Controls.Placement[keyboard.IdRoot].Area.Size.y != 204, "Height:%i, expected:204.", gui.Controls.Placement[keyboard.IdRoot].Area.Size.y);
		LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_ROOT_ALIGN, gui.Controls.Placement[keyboard.IdRoot].Align != ::llc::ALIGN_CENTER_BOTTOM, "Alignment:0x%X, expected:0x%X.", gui.Controls.Placement[keyboard.IdRoot].Align, ::llc::ALIGN_CENTER_BOTTOM);
		LLC_TEST_REQUIREF(errors, GUI_INPUTBOX_TEST_RESULT_CHARACTER_COUNT, keyboard.Characters.size() != ::llc::size(keys), "Character count:%u, expected:%u.", keyboard.Characters.size(), (::llc::u2_t)::llc::size(keys));
		LLC_TEST_REQUIREF(errors, GUI_INPUTBOX_TEST_RESULT_COMMAND_COUNT, keyboard.Commands.size() != ::llc::size(::GUI_INPUTBOX_EXPECTED_COMMANDS), "Command count:%u, expected:%u.", keyboard.Commands.size(), (::llc::u2_t)::llc::size(::GUI_INPUTBOX_EXPECTED_COMMANDS));
		LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_CHARACTER_ID, keyboard.Characters[0].IdControl != controls[3], "First character control:%i, expected reused slot:%i.", keyboard.Characters[0].IdControl, controls[3]);
		LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_CHARACTER_ID, keyboard.Characters[1].IdControl == keyboard.Characters[0].IdControl + 1, "Character controls unexpectedly contiguous:%i, %i.", keyboard.Characters[0].IdControl, keyboard.Characters[1].IdControl);
		for(::llc::u2_t iCharacter = 0; iCharacter < keyboard.Characters.size(); ++iCharacter) {
			cnst ::llc::SVirtualKeyboardCharacter & character = keyboard.Characters[iCharacter];
			LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_CHARACTER_VALUE, character.Character != keys[iCharacter], "Character:%u value:0x%X, expected:0x%X.", iCharacter, character.Character, keys[iCharacter]);
			LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_CHARACTER_PARENT, gui.Controls.Entities[character.IdControl].Parent != keyboard.IdRoot, "Character:%u parent:%i, expected:%i.", iCharacter, gui.Controls.Entities[character.IdControl].Parent, keyboard.IdRoot);
			LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_CHARACTER_SIZE, gui.Controls.Placement[character.IdControl].Area.Size != keySize.s1(), "Character:%u size:{%i, %i}, expected:{%u, %u}.", iCharacter, gui.Controls.Placement[character.IdControl].Area.Size.x, gui.Controls.Placement[character.IdControl].Area.Size.y, keySize.x, keySize.y);
			cnst ::llc::n2s1_t expectedOffset = {(::llc::s1_t)((iCharacter % 2) * keySize.x), (::llc::s1_t)((iCharacter / 2) * keySize.y)};
			LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_CHARACTER_OFFSET, gui.Controls.Placement[character.IdControl].Area.Offset != expectedOffset, "Character:%u offset:{%i, %i}, expected:{%i, %i}.", iCharacter, gui.Controls.Placement[character.IdControl].Area.Offset.x, gui.Controls.Placement[character.IdControl].Area.Offset.y, expectedOffset.x, expectedOffset.y);
			LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_CHARACTER_TEXT_SIZE, gui.Controls.Text[character.IdControl].Text.size() != 1, "Character:%u caption length:%u, expected:1.", iCharacter, gui.Controls.Text[character.IdControl].Text.size());
			LLC_TEST_REQUIREF(errors, GUI_INPUTBOX_TEST_RESULT_CHARACTER_TEXT_SIZE, 0 == gui.Controls.Text[character.IdControl].Text.size(), "Character:%u caption is empty.", iCharacter);
			LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_CHARACTER_TEXT_VALUE, (::llc::u0_t)gui.Controls.Text[character.IdControl].Text[0] != keys[iCharacter], "Character:%u caption:0x%X, expected:0x%X.", iCharacter, (::llc::u0_t)gui.Controls.Text[character.IdControl].Text[0], keys[iCharacter]);
		}
		for(::llc::u2_t iCommand = 0; iCommand < keyboard.Commands.size(); ++iCommand) {
			cnst ::llc::SVirtualKeyboardCommand & command = keyboard.Commands[iCommand];
			cnst ::llc::vcst_t controlLabel = gui.Controls.Text[command.IdControl].Text;
			cnst ::llc::vcst_t commandLabel = ::llc::get_value_label(command.Key);
			LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_COMMAND_KEY, command.Key != ::GUI_INPUTBOX_EXPECTED_COMMANDS[iCommand], "Command:%u key:0x%X, expected:0x%X.", iCommand, (::llc::u0_t)command.Key, (::llc::u0_t)::GUI_INPUTBOX_EXPECTED_COMMANDS[iCommand]);
			LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_COMMAND_PARENT, gui.Controls.Entities[command.IdControl].Parent != keyboard.IdRoot, "Command:%u parent:%i, expected:%i.", iCommand, gui.Controls.Entities[command.IdControl].Parent, keyboard.IdRoot);
			LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_COMMAND_SIZE, gui.Controls.Placement[command.IdControl].Area.Size != commandSize, "Command:%u size:{%i, %i}, expected:{88, 20}.", iCommand, gui.Controls.Placement[command.IdControl].Area.Size.x, gui.Controls.Placement[command.IdControl].Area.Size.y);
			LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_COMMAND_OFFSET, gui.Controls.Placement[command.IdControl].Area.Offset.y != (::llc::s1_t)(iCommand * 20), "Command:%u offset y:%i, expected:%u.", iCommand, gui.Controls.Placement[command.IdControl].Area.Offset.y, iCommand * 20);
			LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_COMMAND_ALIGN, gui.Controls.Placement[command.IdControl].Align != ::llc::ALIGN_TOP_RIGHT, "Command:%u alignment:0x%X, expected:0x%X.", iCommand, gui.Controls.Placement[command.IdControl].Align, ::llc::ALIGN_TOP_RIGHT);
			LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_COMMAND_TEXT, controlLabel != commandLabel, "Command:%u caption:'%.*s', expected:'%.*s'.", iCommand, LLC_VIEW_ARGS(controlLabel), LLC_VIEW_ARGS(commandLabel));
		}
		cnst ::llc::err_t reentryResult = ::llc::virtualKeyboardSetup(gui, keyboard, 2, keySize, keys);
		LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_REENTRY_RESULT, false == ::llc::failed(reentryResult), "Second setup result:%i.", reentryResult);
	}
	rtrn 0;
}

sttc ::llc::err_t testGUIVirtualKeyboard437(ATestError & errors) {
	::llc::SGUI gui = {};
	::llc::SVirtualKeyboard keyboard = {};
	cnst ::llc::err_t setupResult = ::llc::virtualKeyboardSetup437(gui, keyboard);
	LLC_TEST_REQUIREF(errors, GUI_INPUTBOX_TEST_RESULT_CP437_RESULT, ::llc::failed(setupResult), "Setup result:%i.", setupResult);
	LLC_TEST_REQUIREF(errors, GUI_INPUTBOX_TEST_RESULT_CP437_CHARACTER_COUNT, keyboard.Characters.size() != 256, "Character count:%u, expected:256.", keyboard.Characters.size());
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_CP437_FIRST, keyboard.Characters[0].Character != 1, "First character:0x%X, expected:0x1.", keyboard.Characters[0].Character);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_CP437_LAST, keyboard.Characters[253].Character != 254, "Last CP437 character:0x%X, expected:0xFE.", keyboard.Characters[253].Character);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_CP437_LAST, keyboard.Characters[254].Character != '!', "Penultimate character:0x%X, expected:'!'.", keyboard.Characters[254].Character);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_CP437_LAST, keyboard.Characters[255].Character != '?', "Final character:0x%X, expected:'?'.", keyboard.Characters[255].Character);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_CP437_WIDTH, gui.Controls.Placement[keyboard.IdRoot].Area.Size.x != 540, "Width:%i, expected:540.", gui.Controls.Placement[keyboard.IdRoot].Area.Size.x);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_CP437_HEIGHT, gui.Controls.Placement[keyboard.IdRoot].Area.Size.y != 180, "Height:%i, expected:180.", gui.Controls.Placement[keyboard.IdRoot].Area.Size.y);
	LLC_TEST_CHECK(errors, GUI_INPUTBOX_TEST_RESULT_CP437_HIDDEN, false == gui.Controls.IsHidden(keyboard.IdRoot));
	rtrn 0;
}

sttc ::llc::err_t testGUIInputBoxCreation(ATestError & errors, ::llc::SGUI & gui, ::llc::SInputBox & inputBox) {
	cnst ::llc::cid_t idParent = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_INPUTBOX_TEST_RESULT_CREATE_RESULT, idParent < 0, "Parent creation result:%i.", idParent);
	cnst ::llc::err_t createResult = ::llc::inputBoxCreate(inputBox, gui, idParent);
	LLC_TEST_REQUIREF(errors, GUI_INPUTBOX_TEST_RESULT_CREATE_RESULT, ::llc::failed(createResult), "Creation result:%i.", createResult);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_ROOT_PARENT, gui.Controls.Entities[inputBox.IdRoot].Parent != idParent, "Root parent:%i, expected:%i.", gui.Controls.Entities[inputBox.IdRoot].Parent, idParent);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_TEXT_PARENT, gui.Controls.Entities[inputBox.IdText].Parent != inputBox.IdRoot, "Text parent:%i, expected:%i.", gui.Controls.Entities[inputBox.IdText].Parent, inputBox.IdRoot);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_KEYBOARD_PARENT, gui.Controls.Entities[inputBox.VirtualKeyboard.IdRoot].Parent != inputBox.IdRoot, "Keyboard parent:%i, expected:%i.", gui.Controls.Entities[inputBox.VirtualKeyboard.IdRoot].Parent, inputBox.IdRoot);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_INPUT_WIDTH, gui.Controls.Placement[inputBox.IdRoot].Area.Size.x != 540, "Input width:%i, expected:540.", gui.Controls.Placement[inputBox.IdRoot].Area.Size.x);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_INPUT_HEIGHT, gui.Controls.Placement[inputBox.IdRoot].Area.Size.y != 202, "Input height:%i, expected:202.", gui.Controls.Placement[inputBox.IdRoot].Area.Size.y);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_TEXT_WIDTH, gui.Controls.Placement[inputBox.IdText].Area.Size.x != 540, "Text width:%i, expected:540.", gui.Controls.Placement[inputBox.IdText].Area.Size.x);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_TEXT_HEIGHT, gui.Controls.Placement[inputBox.IdText].Area.Size.y != 22, "Text height:%i, expected:22.", gui.Controls.Placement[inputBox.IdText].Area.Size.y);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_KEYBOARD_OFFSET, gui.Controls.Placement[inputBox.VirtualKeyboard.IdRoot].Area.Offset.y != 22, "Keyboard offset:%i, expected:22.", gui.Controls.Placement[inputBox.VirtualKeyboard.IdRoot].Area.Offset.y);
	LLC_TEST_CHECK(errors, GUI_INPUTBOX_TEST_RESULT_INITIAL_HIDDEN, 0 == ::llc::controlHidden(gui, inputBox.IdRoot));
	cnst ::llc::err_t editResult = inputBox.Edit(gui, true);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_EDIT_RESULT, editResult != 1, "Edit result:%i, expected:1.", editResult);
	LLC_TEST_CHECK(errors, GUI_INPUTBOX_TEST_RESULT_EDIT_STATE, false == inputBox.Editing);
	LLC_TEST_CHECK(errors, GUI_INPUTBOX_TEST_RESULT_EDIT_VISIBILITY, 0 != ::llc::controlHidden(gui, inputBox.IdRoot));
	cnst ::llc::err_t textResult = inputBox.SetText(gui, "AB");
	LLC_TEST_REQUIREF(errors, GUI_INPUTBOX_TEST_RESULT_SET_TEXT_RESULT, ::llc::failed(textResult), "SetText result:%i.", textResult);
	LLC_TEST_CHECK(errors, GUI_INPUTBOX_TEST_RESULT_SET_TEXT_STORAGE, inputBox.Text != LLC_CXS("AB"));
	LLC_TEST_CHECK(errors, GUI_INPUTBOX_TEST_RESULT_SET_TEXT_CONTROL, gui.Controls.Text[inputBox.IdText].Text != LLC_CXS("AB"));
	rtrn 0;
}

sttc ::llc::err_t testGUIInputBoxEvents(ATestError & errors) {
	::llc::SGUI gui = {};
	::llc::SInputBox inputBox = {};
	if_fail_fe(testGUIInputBoxCreation(errors, gui, inputBox));

	inputBox.Edit(gui, false);
	cnst ::llc::cid_t idCharacterA = inputBox.VirtualKeyboard.Characters['A' - 1].IdControl;
	cnst ::llc::err_t inactiveResult = ::inputBoxProcessControl(inputBox, gui, idCharacterA);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_INACTIVE_RESULT, inactiveResult, "Inactive result:%i, expected:0.", inactiveResult);
	LLC_TEST_CHECK(errors, GUI_INPUTBOX_TEST_RESULT_INACTIVE_TEXT, inputBox.Text != LLC_CXS("AB"));
	inputBox.Edit(gui, true);

	cnst ::llc::err_t characterResult = ::inputBoxProcessControl(inputBox, gui, idCharacterA);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_CHARACTER_RESULT, characterResult != idCharacterA, "Character result:%i, expected control:%i.", characterResult, idCharacterA);
	LLC_TEST_CHECK(errors, GUI_INPUTBOX_TEST_RESULT_CHARACTER_APPEND, inputBox.Text != LLC_CXS("ABA"));
	LLC_TEST_CHECK(errors, GUI_INPUTBOX_TEST_RESULT_CHARACTER_CONTROL_TEXT, gui.Controls.Text[inputBox.IdText].Text != LLC_CXS("ABA"));

	cnst ::llc::cid_t idCharacterFE = inputBox.VirtualKeyboard.Characters[253].IdControl;
	cnst ::llc::err_t cp437Result = ::inputBoxProcessControl(inputBox, gui, idCharacterFE);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_CHARACTER_RESULT, cp437Result != idCharacterFE, "CP437 result:%i, expected control:%i.", cp437Result, idCharacterFE);
	LLC_TEST_REQUIREF(errors, GUI_INPUTBOX_TEST_RESULT_CP437_APPEND, inputBox.Text.size() != 4, "Text length:%u, expected:4.", inputBox.Text.size());
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_CP437_APPEND, (::llc::u0_t)inputBox.Text[3] != 0xFE, "Final byte:0x%X, expected:0xFE.", (::llc::u0_t)inputBox.Text[3]);

	inputBox.MaxLength = inputBox.Text.size();
	if_fail_fe(::inputBoxProcessControl(inputBox, gui, idCharacterA));
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_MAXIMUM_LENGTH, inputBox.Text.size() != inputBox.MaxLength, "Text length:%u, maximum:%u.", inputBox.Text.size(), inputBox.MaxLength);
	inputBox.MaxLength = 16;

	::llc::TQueueSystemEvent frameEvents = {};
	if_fail_fe(::llc::eventEnqueueTextChar(frameEvents, 'Z'));
	cnst ::llc::err_t frameResult = inputBox.Update(gui, inputBox.VirtualKeyboard, frameEvents, {});
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_FRAME_CHARACTER, frameResult, "Frame character result:%i, expected:0.", frameResult);
	LLC_TEST_REQUIREF(errors, GUI_INPUTBOX_TEST_RESULT_FRAME_CHARACTER, inputBox.Text.size() != 5, "Text length:%u, expected:5.", inputBox.Text.size());
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_FRAME_CHARACTER, inputBox.Text[4] != 'Z', "Final character:0x%X, expected:'Z'.", (::llc::u0_t)inputBox.Text[4]);
	frameEvents.clear();
	if_fail_fe(::llc::eventEnqueueTextChar(frameEvents, 0x1F642));
	if_fail_fe(inputBox.Update(gui, inputBox.VirtualKeyboard, frameEvents, {}));
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_UNREPRESENTABLE_TEXT, inputBox.Text.size() != 5, "Text length:%u, expected:5.", inputBox.Text.size());

	cnst ::llc::cid_t idBackspace = inputBox.VirtualKeyboard.Commands[2].IdControl;
	cnst ::llc::err_t backspaceResult = ::inputBoxProcessControl(inputBox, gui, idBackspace);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_BACKSPACE_RESULT, backspaceResult != idBackspace, "Backspace result:%i, expected control:%i.", backspaceResult, idBackspace);
	LLC_TEST_REQUIREF(errors, GUI_INPUTBOX_TEST_RESULT_BACKSPACE_TEXT, inputBox.Text.size() != 4, "Text length:%u, expected:4.", inputBox.Text.size());
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_BACKSPACE_TEXT, (::llc::u0_t)inputBox.Text[3] != 0xFE, "Final byte:0x%X, expected:0xFE.", (::llc::u0_t)inputBox.Text[3]);

	cnst ::llc::cid_t idLeft = inputBox.VirtualKeyboard.Commands[4].IdControl;
	cnst ::llc::err_t leftResult = ::inputBoxProcessControl(inputBox, gui, idLeft);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_ARROW_RESULT, leftResult != idLeft, "Left result:%i, expected control:%i.", leftResult, idLeft);
	LLC_TEST_REQUIREF(errors, GUI_INPUTBOX_TEST_RESULT_ARROW_TEXT, inputBox.Text.size() != 4, "Text length after left:%u, expected:4.", inputBox.Text.size());

	cnst ::llc::cid_t idClear = inputBox.VirtualKeyboard.Commands[3].IdControl;
	cnst ::llc::err_t clearResult = ::inputBoxProcessControl(inputBox, gui, idClear);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_CLEAR_RESULT, clearResult != idClear, "Clear result:%i, expected control:%i.", clearResult, idClear);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_CLEAR_TEXT, inputBox.Text.size(), "Text length:%u, expected:0.", inputBox.Text.size());
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_CLEAR_TEXT, gui.Controls.Text[inputBox.IdText].Text.size(), "Control caption length:%u, expected:0.", gui.Controls.Text[inputBox.IdText].Text.size());

	inputBox.SetText(gui, "Escape");
	cnst ::llc::cid_t idEscape = inputBox.VirtualKeyboard.Commands[0].IdControl;
	cnst ::llc::err_t escapeResult = ::inputBoxProcessControl(inputBox, gui, idEscape);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_ESCAPE_RESULT, escapeResult != idEscape, "Escape result:%i, expected control:%i.", escapeResult, idEscape);
	LLC_TEST_CHECK(errors, GUI_INPUTBOX_TEST_RESULT_ESCAPE_STATE, inputBox.Editing);
	LLC_TEST_CHECK(errors, GUI_INPUTBOX_TEST_RESULT_ESCAPE_VISIBILITY, 0 == ::llc::controlHidden(gui, inputBox.IdRoot));

	inputBox.Edit(gui, true);
	cnst ::llc::cid_t idEnter = inputBox.VirtualKeyboard.Commands[1].IdControl;
	cnst ::llc::err_t enterResult = ::inputBoxProcessControl(inputBox, gui, idEnter);
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_ENTER_RESULT, enterResult != INT_MAX, "Enter result:%i, expected:%i.", enterResult, INT_MAX);

	::llc::TQueueSystemEvent malformedEvents = {};
	::llc::PEventSystem malformed = {};
	malformed->Type = ::llc::SYSTEM_EVENT_Text;
	if_fail_fe(::llc::eventWrapChild(*malformed, ::llc::EVENT_TEXT_Char, ::llc::vcu0_t{}));
	if_fail_fe(malformedEvents.push_back(malformed));
	cnst ::llc::asc_t textBeforeMalformed = inputBox.Text;
	cnst ::llc::err_t malformedResult = inputBox.Update(gui, inputBox.VirtualKeyboard, malformedEvents, {});
	LLC_TEST_CHECKF(errors, GUI_INPUTBOX_TEST_RESULT_MALFORMED_RESULT, false == ::llc::failed(malformedResult), "Malformed result:%i.", malformedResult);
	LLC_TEST_CHECK(errors, GUI_INPUTBOX_TEST_RESULT_MALFORMED_TEXT, inputBox.Text != textBeforeMalformed);
	rtrn 0;
}

::llc::err_t testGUIInputBox(ATestError & errors) {
	if_fail_fe(testGUIInputBoxDefaults(errors));
	if_fail_fe(testGUIVirtualKeyboardSetup(errors));
	if_fail_fe(testGUIVirtualKeyboard437(errors));
	rtrn testGUIInputBoxEvents(errors);
}
