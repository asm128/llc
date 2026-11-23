#include "llc_event_device.h"
#include "llc_event_input.h"
#include "llc_event_screen.h"
#include "llc_gui_control_flag.h"
#include "llc_color.h"
#include "llc_runtime.h"
#include "llc_event_system.h"

#include "test_llc_suites.h"

#include <cstring>

GDEFINE_ENUM_TYPE(EVENT_TEST_PARENT, ::llc::u0_t);
GDEFINE_ENUM_VALUE(EVENT_TEST_PARENT, Application, 0);
GDEFINE_ENUM_VALUE(EVENT_TEST_PARENT, Forwarded, 1);

GDEFINE_ENUM_TYPE(EVENT_TEST_CHILD, ::llc::u0_t);
GDEFINE_ENUM_VALUE(EVENT_TEST_CHILD, Bytes, 0);
GDEFINE_ENUM_VALUE(EVENT_TEST_CHILD, Values, 1);
GDEFINE_ENUM_VALUE(EVENT_TEST_CHILD, POD, 2);
GDEFINE_ENUM_VALUE(EVENT_TEST_CHILD, Text, 3);
GDEFINE_ENUM_VALUE(EVENT_TEST_CHILD, Empty, 4);

// ALC and MLC keep these event values private while sharing the SEvent envelope.
GDEFINE_ENUM_TYPE(EVENT_TEST_RASTER, ::llc::u0_t);
GDEFINE_ENUM_VALUE(EVENT_TEST_RASTER, Clear, 0);
GDEFINE_ENUM_VALUE(EVENT_TEST_RASTER, Text, 1);

GDEFINE_ENUM_TYPE(EVENT_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, VIEW_SERIALIZE       ,  0, "SEventView did not preserve its type, bytes, or input boundary.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, OWNED_COPY          ,  1, "SEvent did not own an independent copy of its payload.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, OWNED_SERIALIZE     ,  2, "SEvent did not preserve its type, bytes, or input boundary.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, WRAP_EXTRACT        ,  3, "eventWrapChild()/ExtractChild() did not preserve the nested event.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, ENQUEUE_RAW         ,  4, "The raw-byte eventEnqueueChild() overload changed the nested event.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, ENQUEUE_VIEW        ,  5, "The typed-view eventEnqueueChild() overload changed the nested event.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, ENQUEUE_POD         ,  6, "The POD eventEnqueueChild() overload changed the nested event.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, ENQUEUE_TEXT        ,  7, "The string-view eventEnqueueChild() consumer shape changed the text bytes.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, EMPTY_PAYLOAD       ,  8, "An empty child event did not round-trip as an empty payload.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, HANDLER_SUCCESS     ,  9, "eventExtractAndHandle() did not deliver the extracted child.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, HANDLER_RESULT      , 10, "eventExtractAndHandle() did not propagate the handler result.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, TRUNCATED_REJECTED  , 11, "A truncated child event was accepted or dispatched.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, FORWARDED_BYTES     , 12, "Forwarding an existing child Data view changed its bytes.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, SCREEN_RESIZE       , 13, "The normalized screen-resize event changed its dimensions.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, SCREEN_CLOSE        , 14, "The normalized screen-close event was not empty.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, KEYBOARD_EVENT      , 15, "The normalized keyboard event changed its key metadata.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, TEXT_EVENT          , 16, "The normalized text event changed its Unicode or UTF-8 metadata.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, INVALID_TEXT        , 17, "An invalid Unicode scalar was enqueued or changed the queue.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, DEVICE_STATUS_FRAME , 18, "The embedded device-status frame changed in the serialized stream.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, RASTER_CLEAR_FRAME  , 19, "The embedded raster-clear frame changed in the serialized stream.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, RASTER_TEXT_FRAME   , 20, "The embedded raster-text frame changed in the serialized stream.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, FRAME_BOUNDARY      , 21, "Concatenated embedded event frames did not preserve their boundaries.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, RUNTIME_INPUT       , 22, "SRuntimeValues::EventsIn did not post and return its event.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, RUNTIME_OUTPUT      , 23, "SRuntimeValues::EventsOut did not read and clear its events.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, RUNTIME_INDEPENDENT , 24, "SRuntimeValues input and output queues affected each other.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, MOUSE_EVENT         , 25, "The normalized mouse event changed its coordinates.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, TOUCH_EVENT         , 26, "The normalized touch event changed its coordinates.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, JOYPAD_EVENT        , 27, "The normalized joypad event changed its button.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, GUI_EVENT           , 28, "The GUI control-state event changed its control or flag delta.");

#pragma pack(push, 1)
stct SEventTestPOD {
	::llc::u2_t	Sequence	= {};
	::llc::u1_t	Value		= {};
	::llc::u0_t	Flags		= {};
};
#pragma pack(pop)

sttc b8_t eventTestBytesEqual(::llc::vcu0_t left, ::llc::vcu0_t right) {
	rtrn left.size() == right.size() && (0 == left.size() || 0 == ::memcmp(left.begin(), right.begin(), left.size()));
}

tplt<tpnm T>
sttc b8_t eventTestPODRead(::llc::vcu0_t bytes, T & value) {
	if(bytes.size() != szof(T))
		rtrn false;
	::memcpy(&value, bytes.begin(), szof(T));
	rtrn true;
}

sttc ::llc::err_t testEventSerialization(::llc::ATestError & errors) {
	cnst ::llc::u0_t raw[] = {0x10, 0x20, 0x30, 0x40};
	::llc::SEView<EVENT_TEST_CHILD> sourceView = {EVENT_TEST_CHILD_Bytes, {raw}};
	::llc::au0_t serializedView;
	if_fail_fe(sourceView.Save(serializedView));
	::llc::vcu0_t viewInput = serializedView;
	::llc::SEView<EVENT_TEST_CHILD> restoredView;
	cnst ::llc::err_t viewResult = restoredView.Load(viewInput);
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_VIEW_SERIALIZE, ::llc::failed(viewResult) || restoredView.Type != sourceView.Type || !eventTestBytesEqual(restoredView.Data, sourceView.Data) || viewInput.size());

	::llc::SEvent<EVENT_TEST_CHILD> owned = sourceView;
	::llc::SEvent<EVENT_TEST_CHILD> copied = owned;
	owned.Data[0] = 0xFF;
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_OWNED_COPY, copied.Data.size() != ::llc::size(raw) || copied.Data[0] != raw[0] || copied.Data.begin() == owned.Data.begin());

	::llc::au0_t serializedOwned;
	if_fail_fe(copied.Save(serializedOwned));
	::llc::vcu0_t ownedInput = serializedOwned;
	::llc::SEvent<EVENT_TEST_CHILD> restoredOwned;
	cnst ::llc::err_t ownedResult = restoredOwned.Load(ownedInput);
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_OWNED_SERIALIZE, ::llc::failed(ownedResult) || restoredOwned.Type != copied.Type || !eventTestBytesEqual(restoredOwned.Data, copied.Data) || ownedInput.size());
	rtrn 0;
}

sttc ::llc::err_t testEventNesting(::llc::ATestError & errors) {
	cnst ::llc::u0_t raw[] = {1, 3, 5, 7};
	::llc::SEvent<EVENT_TEST_PARENT> wrapped = {EVENT_TEST_PARENT_Application};
	if_fail_fe(::llc::eventWrapChild(wrapped, EVENT_TEST_CHILD_Bytes, ::llc::vcu0_t{raw}));
	::llc::SEView<EVENT_TEST_CHILD> extracted;
	cnst ::llc::err_t extractResult = wrapped.ExtractChild(extracted);
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_WRAP_EXTRACT, ::llc::failed(extractResult) || extracted.Type != EVENT_TEST_CHILD_Bytes || !eventTestBytesEqual(extracted.Data, {raw}));

	::llc::TEventQueue<EVENT_TEST_PARENT> queue;
	if_fail_fe(::llc::eventEnqueueChild(queue, EVENT_TEST_PARENT_Application, EVENT_TEST_CHILD_Bytes, ::llc::vcu0_t{raw}));
	cnst ::llc::u1_t values[] = {0x1122, 0x3344, 0x5566};
	if_fail_fe(::llc::eventEnqueueChild(queue, EVENT_TEST_PARENT_Application, EVENT_TEST_CHILD_Values, ::llc::vcu1_t{values}));
	cnst SEventTestPOD pod = {77, 0xA55A, 0xC3};
	if_fail_fe(::llc::eventEnqueueChild(queue, EVENT_TEST_PARENT_Application, EVENT_TEST_CHILD_POD, pod));
	cnst ::llc::vcst_t text = {".join embedded"};
	if_fail_fe(::llc::eventEnqueueChild(queue, EVENT_TEST_PARENT_Application, EVENT_TEST_CHILD_Text, text.bytes()));
	if_fail_fe(::llc::eventEnqueueChild(queue, EVENT_TEST_PARENT_Application, EVENT_TEST_CHILD_Empty, ::llc::vcu0_t{}));
	LLC_TEST_REQUIRE(errors, EVENT_TEST_RESULT_ENQUEUE_RAW, queue.size() != 5);

	::llc::SEView<EVENT_TEST_CHILD> child;
	if_fail_fe(queue[0]->ExtractChild(child));
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_ENQUEUE_RAW, queue[0]->Type != EVENT_TEST_PARENT_Application || child.Type != EVENT_TEST_CHILD_Bytes || !eventTestBytesEqual(child.Data, {raw}));
	if_fail_fe(queue[1]->ExtractChild(child));
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_ENQUEUE_VIEW, child.Type != EVENT_TEST_CHILD_Values || !eventTestBytesEqual(child.Data, {(cnst ::llc::u0_t*)values, szof(values)}));
	if_fail_fe(queue[2]->ExtractChild(child));
	SEventTestPOD restoredPOD;
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_ENQUEUE_POD, child.Type != EVENT_TEST_CHILD_POD || !eventTestPODRead(child.Data, restoredPOD) || 0 != ::memcmp(&restoredPOD, &pod, szof(pod)));
	if_fail_fe(queue[3]->ExtractChild(child));
	LLC_TEST_CHECKF(errors, EVENT_TEST_RESULT_ENQUEUE_TEXT, child.Type != EVENT_TEST_CHILD_Text || !eventTestBytesEqual(child.Data, {(cnst ::llc::u0_t*)text.begin(), text.size()}), "Type:%u, bytes:%u, expected type:%u, bytes:%u.", (::llc::u2_t)child.Type, child.Data.size(), (::llc::u2_t)EVENT_TEST_CHILD_Text, text.size());
	if_fail_fe(queue[4]->ExtractChild(child));
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_EMPTY_PAYLOAD, child.Type != EVENT_TEST_CHILD_Empty || child.Data.size());

	b8_t handled = false;
	cnst ::llc::function<::llc::err_t(cnst ::llc::SEView<EVENT_TEST_CHILD> &)> successHandler = [&handled, &raw](cnst ::llc::SEView<EVENT_TEST_CHILD> & eventView) {
		handled = eventView.Type == EVENT_TEST_CHILD_Bytes && eventTestBytesEqual(eventView.Data, {raw});
		rtrn 0;
	};
	cnst ::llc::err_t handleResult = ::llc::eventExtractAndHandle(*queue[0], successHandler);
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_HANDLER_SUCCESS, ::llc::failed(handleResult) || !handled);
	cnst ::llc::function<::llc::err_t(cnst ::llc::SEView<EVENT_TEST_CHILD> &)> failureHandler = [](cnst ::llc::SEView<EVENT_TEST_CHILD> &) { rtrn -7; };
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_HANDLER_RESULT, ::llc::eventExtractAndHandle(*queue[0], failureHandler) != -7);

	::llc::SEvent<EVENT_TEST_PARENT> truncated = {EVENT_TEST_PARENT_Application, ::llc::vcu0_t{raw, 1}};
	b8_t malformedDispatched = false;
	cnst ::llc::function<::llc::err_t(cnst ::llc::SEView<EVENT_TEST_CHILD> &)> malformedHandler = [&malformedDispatched](cnst ::llc::SEView<EVENT_TEST_CHILD> &) { malformedDispatched = true; rtrn 0; };
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_TRUNCATED_REJECTED, ::llc::succeeded(::llc::eventExtractAndHandle(truncated, malformedHandler)) || malformedDispatched);

	// SSiege and LLS clients forward an already-extracted child Data view under another parent.
	::llc::TEventQueue<EVENT_TEST_PARENT> forwarded;
	if_fail_fe(::llc::eventEnqueueChild(forwarded, EVENT_TEST_PARENT_Forwarded, extracted.Type, extracted.Data));
	::llc::SEView<EVENT_TEST_CHILD> forwardedChild;
	if_fail_fe(forwarded[0]->ExtractChild(forwardedChild));
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_FORWARDED_BYTES, forwarded[0]->Type != EVENT_TEST_PARENT_Forwarded || forwardedChild.Type != extracted.Type || !eventTestBytesEqual(forwardedChild.Data, extracted.Data));
	rtrn 0;
}

sttc ::llc::err_t testEventPlatformInput(::llc::ATestError & errors) {
	::llc::apobj<::llc::SEventSystem> queue;
	cnst ::llc::n2u1_t dimensions = {480, 320};
	if_fail_fe(::llc::eventEnqueueScreenResize(queue, dimensions));
	if_fail_fe(::llc::eventEnqueueScreenClose(queue));
	cnst ::llc::u0_t key = 13;
	if_fail_fe(::llc::eventEnqueueKeyboardDown(queue, key));
	cnst ::llc::u2_t text = 0x00E9;
	if_fail_fe(::llc::eventEnqueueTextChar(queue, text));
	cnst ::llc::n2s1_t mousePosition = {123, -45};
	if_fail_fe(::llc::eventEnqueueMousePosition(queue, mousePosition));
	cnst ::llc::n2u1_t touchPosition = {320, 240};
	if_fail_fe(::llc::eventEnqueueTouchBegin(queue, touchPosition));
	cnst ::llc::u0_t joypadButton = 2;
	if_fail_fe(::llc::eventEnqueueJoyPadDown(queue, joypadButton));
	if_fail_fe(::llc::eventEnqueueGuiControlStateChange(queue, 42, ::llc::GUI_CONTROL_FLAG_Hidden, ::llc::GUI_CONTROL_FLAG_Disabled));
	LLC_TEST_REQUIRE(errors, EVENT_TEST_RESULT_SCREEN_RESIZE, queue.size() != 8);

	::llc::SScreenEventView screenEvent;
	if_fail_fe(queue[0]->ExtractChild(screenEvent));
	::llc::n2u1_t restoredDimensions;
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_SCREEN_RESIZE, queue[0]->Type != ::llc::SYSTEM_EVENT_Screen || screenEvent.Type != ::llc::EVENT_SCREEN_Resize || !eventTestPODRead(screenEvent.Data, restoredDimensions) || restoredDimensions.x != dimensions.x || restoredDimensions.y != dimensions.y);
	if_fail_fe(queue[1]->ExtractChild(screenEvent));
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_SCREEN_CLOSE, screenEvent.Type != ::llc::EVENT_SCREEN_Close || screenEvent.Data.size());

	::llc::SEView<::llc::EVENT_KEYBOARD> keyboardEvent;
	if_fail_fe(queue[2]->ExtractChild(keyboardEvent));
	::llc::u0_t restoredKey;
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_KEYBOARD_EVENT, queue[2]->Type != ::llc::SYSTEM_EVENT_Keyboard || keyboardEvent.Type != ::llc::EVENT_KEYBOARD_Down || !eventTestPODRead(keyboardEvent.Data, restoredKey) || restoredKey != key);

	::llc::SEView<::llc::EVENT_TEXT> textEvent;
	if_fail_fe(queue[3]->ExtractChild(textEvent));
	::llc::u2_t restoredText;
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_TEXT_EVENT, queue[3]->Type != ::llc::SYSTEM_EVENT_Text || textEvent.Type != ::llc::EVENT_TEXT_Char || !eventTestPODRead(textEvent.Data, restoredText) || restoredText != text);

	::llc::SEView<::llc::EVENT_MOUSE> mouseEvent;
	if_fail_fe(queue[4]->ExtractChild(mouseEvent));
	::llc::n2s1_t restoredMouse;
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_MOUSE_EVENT, queue[4]->Type != ::llc::SYSTEM_EVENT_Mouse || mouseEvent.Type != ::llc::EVENT_MOUSE_Position || !eventTestPODRead(mouseEvent.Data, restoredMouse) || restoredMouse.x != mousePosition.x || restoredMouse.y != mousePosition.y);
	::llc::SEView<::llc::EVENT_TOUCH> touchEvent;
	if_fail_fe(queue[5]->ExtractChild(touchEvent));
	::llc::n2u1_t restoredTouch;
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_TOUCH_EVENT, queue[5]->Type != ::llc::SYSTEM_EVENT_Touch || touchEvent.Type != ::llc::EVENT_TOUCH_Begin || !eventTestPODRead(touchEvent.Data, restoredTouch) || restoredTouch.x != touchPosition.x || restoredTouch.y != touchPosition.y);
	::llc::SEView<::llc::EVENT_JOYPAD> joypadEvent;
	if_fail_fe(queue[6]->ExtractChild(joypadEvent));
	::llc::u0_t restoredButton = {};
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_JOYPAD_EVENT, queue[6]->Type != ::llc::SYSTEM_EVENT_Joypad || joypadEvent.Type != ::llc::EVENT_JOYPAD_Button_down || !eventTestPODRead(joypadEvent.Data, restoredButton) || restoredButton != joypadButton);
	::llc::SEView<::llc::EVENT_GUI_CONTROL> guiEvent;
	if_fail_fe(queue[7]->ExtractChild(guiEvent));
	::llc::SChangeControlState stateChange;
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_GUI_EVENT, queue[7]->Type != ::llc::SYSTEM_EVENT_GUI || guiEvent.Type != ::llc::EVENT_GUI_CONTROL_StateChange || !eventTestPODRead(guiEvent.Data, stateChange) || stateChange.Id != 42 || stateChange.Clear != ::llc::GUI_CONTROL_FLAG_Hidden || stateChange.Set != ::llc::GUI_CONTROL_FLAG_Disabled);

	cnst ::llc::u2_t invalidText = 0xD800;
	cnst ::llc::u2_t countBeforeInvalid = queue.size();
	cnst ::llc::err_t invalidResult = ::llc::eventEnqueueTextChar(queue, invalidText);
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_INVALID_TEXT, ::llc::succeeded(invalidResult) || queue.size() != countBeforeInvalid);
	rtrn 0;
}

sttc ::llc::err_t testEventEmbeddedStream(::llc::ATestError & errors) {
	// ALC reports device status with the packed SDeviceResult system-event frame.
	// MLC sends Raster/Clear and Raster/Text events; ALC loads those system frames
	// and then the private raster child event.
	::llc::au0_t wire;
	cnst ::llc::SDeviceResult deviceStatus = {::llc::RESULT_Busy};
	if_fail_fe(wire.append({(cnst ::llc::u0_t*)&deviceStatus, szof(deviceStatus)}));

	cnst ::llc::bgra clearColor = {0xE7, 0x39, 0x00, 0xFF};
	::llc::SEventSystem clearFrame = {::llc::SYSTEM_EVENT_Raster};
	::llc::SEView<EVENT_TEST_RASTER> clear = {EVENT_TEST_RASTER_Clear, {(cnst ::llc::u0_t*)&clearColor, szof(clearColor)}};
	if_fail_fe(clear.Save(clearFrame.Data));
	if_fail_fe(clearFrame.Save(wire));

	cnst ::llc::vcst_t text = {"ILI9486"};
	::llc::SEventSystem textFrame = {::llc::SYSTEM_EVENT_Raster};
	::llc::SEView<EVENT_TEST_RASTER> rasterText = {EVENT_TEST_RASTER_Text, {(cnst ::llc::u0_t*)text.begin(), text.size()}};
	if_fail_fe(rasterText.Save(textFrame.Data));
	if_fail_fe(textFrame.Save(wire));

	::llc::vcu0_t input = wire;
	::llc::SEViewSystem systemEvent;
	if_fail_fe(systemEvent.Load(input));
	::llc::RESULT restoredStatus = {};
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_DEVICE_STATUS_FRAME, systemEvent.Type != ::llc::SYSTEM_EVENT_Device || !eventTestPODRead(systemEvent.Data, restoredStatus) || restoredStatus != ::llc::RESULT_Busy);

	if_fail_fe(systemEvent.Load(input));
	::llc::vcu0_t childInput = systemEvent.Data;
	::llc::SEView<EVENT_TEST_RASTER> restoredRaster;
	if_fail_fe(restoredRaster.Load(childInput));
	::llc::bgra restoredColor = {};
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_RASTER_CLEAR_FRAME, systemEvent.Type != ::llc::SYSTEM_EVENT_Raster || restoredRaster.Type != EVENT_TEST_RASTER_Clear || !eventTestPODRead(restoredRaster.Data, restoredColor) || restoredColor != clearColor || childInput.size());

	if_fail_fe(systemEvent.Load(input));
	childInput = systemEvent.Data;
	if_fail_fe(restoredRaster.Load(childInput));
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_RASTER_TEXT_FRAME, systemEvent.Type != ::llc::SYSTEM_EVENT_Raster || restoredRaster.Type != EVENT_TEST_RASTER_Text || !eventTestBytesEqual(restoredRaster.Data, {(cnst ::llc::u0_t*)text.begin(), text.size()}) || childInput.size());
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_FRAME_BOUNDARY, input.size());
	rtrn 0;
}

sttc ::llc::err_t testEventRuntimeQueues(::llc::ATestError & errors) {
	::llc::SRuntimeValues runtime;
	::llc::PEventSystem inputEvent = {};
	inputEvent->Type = ::llc::SYSTEM_EVENT_Keyboard;
	inputEvent->Data = ::llc::vcu0_t{(cnst ::llc::u0_t*)"in", 2};
	if_fail_fe(runtime.EventsIn.Post(inputEvent));
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_RUNTIME_INDEPENDENT, runtime.EventsIn.Size() != 1 || runtime.EventsOut.Size());
	::llc::PEventSystem nextInput;
	cnst ::llc::err_t nextResult = runtime.EventsIn.Next(nextInput);
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_RUNTIME_INPUT, nextResult != 1 || 0 == (::llc::SEventSystem*)nextInput || nextInput->Type != inputEvent->Type || !eventTestBytesEqual(nextInput->Data, inputEvent->Data) || runtime.EventsIn.Size());

	::llc::PEventSystem outputEvent = {};
	outputEvent->Type = ::llc::SYSTEM_EVENT_Device;
	outputEvent->Data = ::llc::vcu0_t{(cnst ::llc::u0_t*)"out", 3};
	if_fail_fe(runtime.EventsOut.Post(outputEvent));
	::llc::apobj<::llc::SEventSystem> output;
	cnst ::llc::err_t readResult = runtime.EventsOut.Read(output);
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_RUNTIME_OUTPUT, ::llc::failed(readResult) || output.size() != 1 || output[0]->Type != outputEvent->Type || !eventTestBytesEqual(output[0]->Data, outputEvent->Data) || runtime.EventsOut.Size());
	LLC_TEST_CHECK(errors, EVENT_TEST_RESULT_RUNTIME_INDEPENDENT, runtime.EventsIn.Size() || runtime.EventsOut.Size() || 0 != runtime.EventsIn.Next(nextInput));
	rtrn 0;
}

::llc::err_t test_event(STestRunnerState &, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	if_fail_fe(testEventSerialization(errors));
	if_fail_fe(testEventNesting(errors));
	if_fail_fe(testEventPlatformInput(errors));
	if_fail_fe(testEventEmbeddedStream(errors));
	if_fail_fe(testEventRuntimeQueues(errors));
	rtrn 0;
}
