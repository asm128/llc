#include "llc_virtual_keyboard.h"

#include "llc_test_core.h"

#include <type_traits>

GDEFINE_ENUM_TYPE(VIRTUAL_KEYBOARD_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, CONTROL_TYPE			,  0, "A virtual-keyboard mapping has the wrong control-id type.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, CHARACTER_TYPE		,  1, "A virtual-keyboard character has the wrong scalar type.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, COMMAND_TYPE			,  2, "A virtual-keyboard command has the wrong key type.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, DEFAULT_ROOT			,  3, "A default virtual keyboard has a valid root control.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, DEFAULT_CHARACTERS	,  4, "A default virtual keyboard contains character mappings.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, DEFAULT_COMMANDS		,  5, "A default virtual keyboard contains command mappings.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, CHARACTER_APPEND		,  6, "A virtual-keyboard character mapping could not be stored.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, COMMAND_APPEND		,  7, "A virtual-keyboard command mapping could not be stored.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, PREFIX_ENQUEUE		,  8, "The preexisting event could not be queued.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, UNMAPPED_RESULT		,  9, "An unmapped control was handled.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, UNMAPPED_QUEUE		, 10, "An unmapped control changed the event queue.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, UNMAPPED_REFERENCE	, 11, "An unmapped control replaced a preexisting event.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, CHARACTER_RESULT		, 12, "A mapped character control was not handled exactly once.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, CHARACTER_QUEUE		, 13, "A mapped character control appended the wrong number of events.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, CHARACTER_REFERENCE	, 14, "A character event has no owned reference.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, CHARACTER_PARENT		, 15, "A character event has the wrong system-event type.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, CHARACTER_DISPATCH	, 16, "A character event could not be extracted.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, CHARACTER_CHILD		, 17, "A character event has the wrong child-event type.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, CHARACTER_SIZE		, 18, "A character event has the wrong payload size.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, CHARACTER_LOAD		, 19, "A character payload could not be loaded.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, CHARACTER_CURSOR		, 20, "A character payload left unread bytes.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, CHARACTER_VALUE		, 21, "A character payload lost or changed its value.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, COMMAND_RESULT		, 22, "A mapped command control was not handled exactly once.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, COMMAND_QUEUE			, 23, "A mapped command control appended the wrong number of events.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, COMMAND_REFERENCE		, 24, "A command event has no owned reference.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, COMMAND_PARENT		, 25, "A command event has the wrong system-event type.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, COMMAND_DISPATCH		, 26, "A command event could not be extracted.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, COMMAND_CHILD			, 27, "A command event has the wrong child-event type.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, COMMAND_SIZE			, 28, "A command event has the wrong payload size.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, COMMAND_LOAD			, 29, "A command payload could not be loaded.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, COMMAND_CURSOR		, 30, "A command payload left unread bytes.");
GDEFINE_ENUM_VALUED(VIRTUAL_KEYBOARD_TEST_RESULT, COMMAND_VALUE			, 31, "A command payload lost or changed its value.");

sttc ::llc::err_t testVirtualKeyboardTypesAndDefaults(ATestError & errors) {
	LLC_TEST_CHECK(errors, VIRTUAL_KEYBOARD_TEST_RESULT_CONTROL_TYPE, false == (::std::is_same_v<decltype(::llc::SVirtualKeyboardCharacter{}.IdControl), ::llc::cid_t>));
	LLC_TEST_CHECK(errors, VIRTUAL_KEYBOARD_TEST_RESULT_CONTROL_TYPE, false == (::std::is_same_v<decltype(::llc::SVirtualKeyboardCommand{}.IdControl), ::llc::cid_t>));
	LLC_TEST_CHECK(errors, VIRTUAL_KEYBOARD_TEST_RESULT_CHARACTER_TYPE, false == (::std::is_same_v<decltype(::llc::SVirtualKeyboardCharacter{}.Character), ::llc::u2_t>));
	LLC_TEST_CHECK(errors, VIRTUAL_KEYBOARD_TEST_RESULT_COMMAND_TYPE, false == (::std::is_same_v<decltype(::llc::SVirtualKeyboardCommand{}.Key), ::llc::SYSTEM_KEY>));
	cnst ::llc::SVirtualKeyboard keyboard = {};
	LLC_TEST_CHECKF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_DEFAULT_ROOT, keyboard.IdRoot != ::llc::CID_INVALID, "Root:%i, expected:%i.", keyboard.IdRoot, ::llc::CID_INVALID);
	LLC_TEST_CHECKF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_DEFAULT_CHARACTERS, keyboard.Characters.size(), "Character mappings:%u.", keyboard.Characters.size());
	LLC_TEST_CHECKF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_DEFAULT_COMMANDS, keyboard.Commands.size(), "Command mappings:%u.", keyboard.Commands.size());
	rtrn 0;
}

sttc ::llc::err_t testVirtualKeyboardCharacterEvent(ATestError & errors, cnst ::llc::TQueueSystemEvent & events, ::llc::u2_c iEvent, ::llc::u2_c expectedCharacter) {
	LLC_TEST_REQUIREF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_CHARACTER_REFERENCE, 0 == events[iEvent].get_ref(), "Event:%u has no owned reference.", iEvent);
	LLC_TEST_CHECKF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_CHARACTER_PARENT, events[iEvent]->Type != ::llc::SYSTEM_EVENT_Text, "Event:%u parent:%u, expected:%u.", iEvent, (::llc::u2_t)events[iEvent]->Type, (::llc::u2_t)::llc::SYSTEM_EVENT_Text);
	::llc::SEView<::llc::EVENT_TEXT> child = {};
	cnst ::llc::err_t dispatchResult = ::llc::eventExtractAndHandle<::llc::EVENT_TEXT>(*events[iEvent], [&child](cnst ::llc::SEView<::llc::EVENT_TEXT> & eventView) { child = eventView; rtrn 0; });
	LLC_TEST_REQUIREF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_CHARACTER_DISPATCH, ::llc::failed(dispatchResult), "Event:%u dispatch result:%i.", iEvent, dispatchResult);
	LLC_TEST_CHECKF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_CHARACTER_CHILD, child.Type != ::llc::EVENT_TEXT_Char, "Event:%u child:%u, expected:%u.", iEvent, (::llc::u2_t)child.Type, (::llc::u2_t)::llc::EVENT_TEXT_Char);
	LLC_TEST_REQUIREF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_CHARACTER_SIZE, child.Data.byte_count() != szof(::llc::u2_t), "Event:%u payload bytes:%u, expected:%u.", iEvent, child.Data.byte_count(), (::llc::u2_t)szof(::llc::u2_t));
	::llc::vcu0_t payload = child.Data;
	::llc::u2_t character = {};
	cnst ::llc::err_t loadResult = ::llc::loadPOD(payload, character);
	LLC_TEST_REQUIREF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_CHARACTER_LOAD, ::llc::failed(loadResult), "Event:%u load result:%i.", iEvent, loadResult);
	LLC_TEST_CHECKF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_CHARACTER_CURSOR, payload.size(), "Event:%u trailing payload bytes:%u.", iEvent, payload.size());
	LLC_TEST_CHECKF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_CHARACTER_VALUE, character != expectedCharacter, "Event:%u character:0x%X, expected:0x%X.", iEvent, character, expectedCharacter);
	rtrn 0;
}

sttc ::llc::err_t testVirtualKeyboardCommandEvent(ATestError & errors, cnst ::llc::TQueueSystemEvent & events, ::llc::u2_c iEvent, ::llc::SYSTEM_KEY expectedKey) {
	LLC_TEST_REQUIREF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_COMMAND_REFERENCE, 0 == events[iEvent].get_ref(), "Event:%u has no owned reference.", iEvent);
	LLC_TEST_CHECKF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_COMMAND_PARENT, events[iEvent]->Type != ::llc::SYSTEM_EVENT_Keyboard, "Event:%u parent:%u, expected:%u.", iEvent, (::llc::u2_t)events[iEvent]->Type, (::llc::u2_t)::llc::SYSTEM_EVENT_Keyboard);
	::llc::SEView<::llc::EVENT_KEYBOARD> child = {};
	cnst ::llc::err_t dispatchResult = ::llc::eventExtractAndHandle<::llc::EVENT_KEYBOARD>(*events[iEvent], [&child](cnst ::llc::SEView<::llc::EVENT_KEYBOARD> & eventView) { child = eventView; rtrn 0; });
	LLC_TEST_REQUIREF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_COMMAND_DISPATCH, ::llc::failed(dispatchResult), "Event:%u dispatch result:%i.", iEvent, dispatchResult);
	LLC_TEST_CHECKF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_COMMAND_CHILD, child.Type != ::llc::EVENT_KEYBOARD_Down, "Event:%u child:%u, expected:%u.", iEvent, (::llc::u2_t)child.Type, (::llc::u2_t)::llc::EVENT_KEYBOARD_Down);
	LLC_TEST_REQUIREF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_COMMAND_SIZE, child.Data.byte_count() != szof(::llc::u0_t), "Event:%u payload bytes:%u, expected:%u.", iEvent, child.Data.byte_count(), (::llc::u2_t)szof(::llc::u0_t));
	::llc::vcu0_t payload = child.Data;
	::llc::u0_t key = {};
	cnst ::llc::err_t loadResult = ::llc::loadPOD(payload, key);
	LLC_TEST_REQUIREF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_COMMAND_LOAD, ::llc::failed(loadResult), "Event:%u load result:%i.", iEvent, loadResult);
	LLC_TEST_CHECKF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_COMMAND_CURSOR, payload.size(), "Event:%u trailing payload bytes:%u.", iEvent, payload.size());
	LLC_TEST_CHECKF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_COMMAND_VALUE, key != (::llc::u0_t)expectedKey, "Event:%u key:0x%X, expected:0x%X.", iEvent, key, (::llc::u0_t)expectedKey);
	rtrn 0;
}

sttc ::llc::err_t testVirtualKeyboardMapping(ATestError & errors) {
	::llc::SVirtualKeyboard keyboard = {};
	keyboard.IdRoot = 200;
	cnst ::llc::err_t characterAppend = keyboard.Characters.push_back({73, 0x1F642});
	LLC_TEST_REQUIREF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_CHARACTER_APPEND, ::llc::failed(characterAppend), "Append result:%i.", characterAppend);
	cnst ::llc::err_t commandAppend = keyboard.Commands.push_back({400, ::llc::SYSTEM_KEY_Left});
	LLC_TEST_REQUIREF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_COMMAND_APPEND, ::llc::failed(commandAppend), "Append result:%i.", commandAppend);

	::llc::TQueueSystemEvent events = {};
	cnst ::llc::err_t prefixResult = ::llc::eventEnqueueTextChar(events, 0x41);
	LLC_TEST_REQUIREF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_PREFIX_ENQUEUE, ::llc::failed(prefixResult), "Enqueue result:%i.", prefixResult);
	auto * prefixReference = events[0].get_ref();
	cnst ::llc::err_t unmappedResult = ::llc::virtualKeyboardHandleEvent(keyboard, keyboard.IdRoot + 1, events);
	LLC_TEST_CHECKF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_UNMAPPED_RESULT, unmappedResult, "Control:%i result:%i.", keyboard.IdRoot + 1, unmappedResult);
	LLC_TEST_REQUIREF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_UNMAPPED_QUEUE, events.size() != 1, "Queue size:%u, expected:1.", events.size());
	LLC_TEST_CHECKF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_UNMAPPED_REFERENCE, events[0].get_ref() != prefixReference, "Reference:%p, expected:%p.", events[0].get_ref(), prefixReference);

	cnst ::llc::err_t characterResult = ::llc::virtualKeyboardHandleEvent(keyboard, 73, events);
	LLC_TEST_CHECKF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_CHARACTER_RESULT, characterResult != 1, "Control:73 result:%i, expected:1.", characterResult);
	LLC_TEST_REQUIREF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_CHARACTER_QUEUE, events.size() != 2, "Queue size:%u, expected:2.", events.size());
	if_fail_fe(testVirtualKeyboardCharacterEvent(errors, events, 1, 0x1F642));

	cnst ::llc::err_t commandResult = ::llc::virtualKeyboardHandleEvent(keyboard, 400, events);
	LLC_TEST_CHECKF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_COMMAND_RESULT, commandResult != 1, "Control:400 result:%i, expected:1.", commandResult);
	LLC_TEST_REQUIREF(errors, VIRTUAL_KEYBOARD_TEST_RESULT_COMMAND_QUEUE, events.size() != 3, "Queue size:%u, expected:3.", events.size());
	if_fail_fe(testVirtualKeyboardCommandEvent(errors, events, 2, ::llc::SYSTEM_KEY_Left));
	rtrn 0;
}

::llc::err_t testVirtualKeyboard(ATestError & errors) {
	if_fail_fe(testVirtualKeyboardTypesAndDefaults(errors));
	if_fail_fe(testVirtualKeyboardMapping(errors));
	rtrn 0;
}
