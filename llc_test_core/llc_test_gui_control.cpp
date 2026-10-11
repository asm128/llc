#include "llc_gui_control.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(GUI_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, EVENT_ENQUEUE_RESULT		,  0, "A valid GUI state-change event could not be enqueued.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, EVENT_QUEUE_SIZE			,  1, "The GUI event queue contained the wrong number of events.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, EVENT_REFERENCE			,  2, "A GUI event queue entry did not own an event.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, EVENT_PARENT_TYPE			,  3, "A GUI event was wrapped in the wrong system-event type.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, EVENT_DISPATCH_RESULT		,  4, "GUI child-event dispatch reported failure for a valid event.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, EVENT_DISPATCH_COUNT		,  5, "GUI child-event dispatch invoked its handler the wrong number of times.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, EVENT_CHILD_TYPE			,  6, "A GUI child event contained the wrong event type.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, EVENT_PAYLOAD_SIZE			,  7, "A GUI state-change event contained the wrong payload size.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, EVENT_PAYLOAD_LOAD			,  8, "A valid GUI state-change payload could not be loaded.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, EVENT_PAYLOAD_CURSOR		,  9, "Loading a GUI state-change payload did not consume exactly its bytes.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, EVENT_CONTROL_ID			, 10, "A GUI state-change payload contained the wrong control id.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, EVENT_CLEAR_MASK			, 11, "A GUI state-change payload contained the wrong cleared flags.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, EVENT_SET_MASK				, 12, "A GUI state-change payload contained the wrong set flags.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, EVENT_HANDLER_RESULT		, 13, "GUI child-event dispatch did not propagate the handler result.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, EVENT_SNAPSHOT_SIZE		, 14, "Copying a GUI event queue changed its event count.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, EVENT_SNAPSHOT_REFERENCE	, 15, "Copying a GUI event queue changed the owned event identity.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, EVENT_SOURCE_CLEAR			, 16, "Clearing a GUI event producer queue retained its entries.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, EVENT_SNAPSHOT_LIFETIME	, 17, "A copied GUI event queue lost its event when the producer queue was cleared.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, EVENT_TRUNCATED_RESULT		, 18, "GUI child-event dispatch accepted a truncated event.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, EVENT_TRUNCATED_HANDLER	, 19, "GUI child-event dispatch invoked the handler for a truncated event.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_STORAGE_RESULT		, 20, "GUI control-state storage could not be created.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_HOVERED				, 21, "SetHovered() produced the wrong control state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_HOVERED_EVENT		, 22, "SetHovered() emitted the wrong event count.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_HOVERED_REPEAT		, 23, "Repeating SetHovered() changed the control state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_HOVERED_REPEAT_EVENT, 24, "Repeating SetHovered() emitted a redundant event.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_PRESSED				, 25, "SetPressed(true) produced the wrong pressed state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_PRESSED_ACTION		, 26, "SetPressed(true) produced an action before release.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_PRESSED_EVENT		, 27, "SetPressed(true) emitted the wrong event count.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_RELEASED				, 28, "SetPressed(false) retained the pressed state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_RELEASED_ACTION		, 29, "Releasing a hovered pressed control did not produce an action.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_RELEASED_EVENT		, 30, "SetPressed(false) emitted the wrong event count.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_ACTION_CLEAR			, 31, "SetAction(false) retained the action state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_ACTION_CLEAR_EVENT	, 32, "SetAction(false) emitted the wrong event count.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_MASKED_HIDDEN		, 33, "SetHidden() changed a flag excluded by the control mask.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_MASKED_HIDDEN_EVENT	, 34, "SetHidden() emitted an event for a flag excluded by the control mask.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_HIDDEN				, 35, "SetHidden(true) produced the wrong hidden state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_HIDDEN_EVENT			, 36, "SetHidden(true) emitted the wrong event count.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_DISABLED				, 37, "SetDisabled() produced the wrong control state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_FOCUSED				, 38, "SetFocused() produced the wrong control state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_SELECTED				, 39, "SetSelected() produced the wrong control state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_UPDATED				, 40, "SetUpdated() produced the wrong control state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_UNUSED_SET			, 41, "SetUnused(true) did not set the unused sentinel state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_UNUSED_SET_EVENT	, 42, "SetUnused(true) emitted the wrong event count.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_UNUSED_CLEAR			, 43, "SetUnused(false) did not clear the unused sentinel state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_UNUSED_CLEAR_EVENT	, 44, "SetUnused(false) emitted the wrong event count.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_CREATE_RESULT		, 45, "A GUI control entity could not be created.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_CREATE_ID			, 46, "A GUI control entity received the wrong id.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_ENTITY_STORAGE		, 47, "GUI entity storage was not aligned with control identity.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_STATE_STORAGE		, 48, "GUI state storage was not aligned with control identity.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_PLACEMENT_STORAGE	, 49, "GUI placement storage was not aligned with control identity.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_AREA_STORAGE		, 50, "GUI area storage was not aligned with control identity.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_CONSTRAINT_STORAGE	, 51, "GUI constraint storage was not aligned with control identity.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_DRAW_STORAGE		, 52, "GUI style storage was not aligned with control identity.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_TEXT_STORAGE		, 53, "GUI text storage was not aligned with control identity.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_CHILD_STORAGE		, 54, "GUI child storage was not aligned with control identity.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_EVENT_TYPE			, 55, "A GUI control lifecycle event contained the wrong event type.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_EVENT_PAYLOAD_SIZE	, 56, "A GUI control lifecycle event contained the wrong payload size.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_EVENT_PAYLOAD_LOAD	, 57, "A GUI control lifecycle event payload could not be loaded.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_EVENT_PAYLOAD_CURSOR, 58, "Loading a GUI control lifecycle event left trailing payload bytes.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_EVENT_ID			, 59, "A GUI control lifecycle event contained the wrong control id.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_PARENT_RESULT		, 60, "A valid GUI parent change was rejected.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_PARENT_VALUE		, 61, "A GUI control retained the wrong parent id.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_PARENT_CHILD_COUNT	, 62, "A GUI parent retained the wrong child count.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_PARENT_CHILD_ID	, 63, "A GUI parent retained the wrong child id.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_PARENT_EVENT_TYPE	, 64, "A GUI parent-change event contained the wrong event type.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_PARENT_PAYLOAD_SIZE	, 65, "A GUI parent-change event contained the wrong payload size.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_PARENT_PAYLOAD_LOAD	, 66, "A GUI parent-change event payload could not be loaded.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_PARENT_PAYLOAD_CURSOR, 67, "Loading a GUI parent-change event left trailing payload bytes.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_PARENT_EVENT_ID		, 68, "A GUI parent-change event contained the wrong control id.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_PARENT_EVENT_OLD	, 69, "A GUI parent-change event contained the wrong previous parent.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_PARENT_EVENT_NEW	, 70, "A GUI parent-change event contained the wrong new parent.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_PARENT_REPEAT_EVENT	, 71, "Repeating a GUI parent assignment emitted a redundant event.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_PARENT_SELF_RESULT	, 72, "A GUI control accepted itself as parent.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_PARENT_CYCLE_RESULT	, 73, "A GUI hierarchy accepted a parent cycle.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_PARENT_REJECT_VALUE	, 74, "A rejected GUI parent change modified the hierarchy.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_PARENT_REJECT_EVENT	, 75, "A rejected GUI parent change emitted an event.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_DELETE_RESULT		, 76, "A valid GUI control deletion failed.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_DELETE_UNUSED		, 77, "A deleted GUI control did not become an unused slot.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_DELETE_CHILD_PARENT	, 78, "A non-recursive GUI deletion retained the deleted parent in its child.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_REUSE_ID			, 79, "GUI control creation did not reuse an unused entity slot.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_REUSE_PARENT		, 80, "A reused GUI control entity retained its old parent.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_REUSE_CHILDREN		, 81, "A reused GUI control entity retained its old children.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_REUSE_PLACEMENT		, 82, "A reused GUI control entity retained its old placement.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_REUSE_TEXT			, 83, "A reused GUI control entity retained its old text.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_REUSE_STYLE			, 84, "A reused GUI control entity retained its old style.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_RECURSIVE_CHILD		, 85, "Recursive GUI deletion retained a child entity.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_RECURSIVE_PARENT	, 86, "Recursive GUI deletion retained the parent entity.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CONTROL_RECURSIVE_EVENT_ORDER, 87, "Recursive GUI deletion emitted lifecycle events in the wrong order.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_QUERY_HIDDEN			, 88, "SControlState::IsHidden() did not apply Flag and Mask.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_QUERY_DISABLED		, 89, "SControlState::IsDisabled() did not apply Flag and Mask.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_QUERY_HOVERED		, 90, "SControlState::IsHovered() did not apply Flag and Mask.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_QUERY_PRESSED		, 91, "SControlState::IsPressed() did not apply Flag and Mask.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_QUERY_FOCUSED		, 92, "SControlState::IsFocused() did not apply Flag and Mask.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_QUERY_SELECTED		, 93, "SControlState::IsSelected() did not apply Flag and Mask.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_QUERY_UPDATED		, 94, "SControlState::IsUpdated() did not apply Flag and Mask.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_QUERY_ACTION			, 95, "SControlState::IsAction() did not apply Flag and Mask.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_QUERY_UNUSED			, 96, "SControlState::IsUnused() did not recognize the full-byte sentinel independently of Mask.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_SET_HIDDEN_VALUE		, 97, "setHidden() stored the wrong control state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_SET_HIDDEN_RETURN	, 98, "setHidden() returned the wrong control state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_SET_DISABLED_VALUE	, 99, "setDisabled() stored the wrong control state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_SET_DISABLED_RETURN	, 100, "setDisabled() returned the wrong control state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_SET_HOVERED_VALUE		, 101, "setHovered() stored the wrong control state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_SET_HOVERED_RETURN	, 102, "setHovered() returned the wrong control state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_SET_FOCUSED_VALUE		, 103, "setFocused() stored the wrong control state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_SET_FOCUSED_RETURN	, 104, "setFocused() returned the wrong control state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_SET_SELECTED_VALUE	, 105, "setSelected() stored the wrong control state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_SET_SELECTED_RETURN	, 106, "setSelected() returned the wrong control state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_SET_UPDATED_VALUE		, 107, "setUpdated() stored the wrong control state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_SET_UPDATED_RETURN	, 108, "setUpdated() returned the wrong control state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_SET_ACTION_VALUE		, 109, "setAction() stored the wrong control state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_SET_ACTION_RETURN		, 110, "setAction() returned the wrong control state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_SET_PRESSED_VALUE		, 111, "setPressed() stored the wrong control state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_SET_PRESSED_RETURN	, 112, "setPressed() returned the wrong control state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_SET_UNUSED_VALUE		, 113, "setUnused() stored the wrong sentinel state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_SET_UNUSED_RETURN		, 114, "setUnused() returned the wrong sentinel state.");
sttc ::llc::err_t testGUIEventPayload(ATestError & errors, cnst ::llc::PEventSystem & eventSystem, ::llc::u2_c iEvent, ::llc::cid_t expectedId, ::llc::GUI_CONTROL_FLAG expectedClear, ::llc::GUI_CONTROL_FLAG expectedSet) {
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_EVENT_REFERENCE, 0 == eventSystem.get_ref(), "Event:%u has no owned reference.", iEvent);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_EVENT_PARENT_TYPE, eventSystem->Type != ::llc::SYSTEM_EVENT_GUI, "Event:%u parent type:%u, expected:%u.", iEvent, (::llc::u2_t)eventSystem->Type, (::llc::u2_t)::llc::SYSTEM_EVENT_GUI);

	::llc::SEView<::llc::EVENT_GUI_CONTROL>	childEvent		= {};
	::llc::u2_t							handlerCalls	= 0;
	cnst ::llc::err_t dispatchResult = ::llc::eventExtractAndHandle<::llc::EVENT_GUI_CONTROL>(*eventSystem, [&childEvent, &handlerCalls](cnst ::llc::SEView<::llc::EVENT_GUI_CONTROL> & eventView) {
		childEvent = eventView;
		++handlerCalls;
		rtrn 0;
	});
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_EVENT_DISPATCH_RESULT, ::llc::failed(dispatchResult), "Event:%u dispatch result:%i.", iEvent, dispatchResult);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_EVENT_DISPATCH_COUNT, handlerCalls != 1, "Event:%u handler calls:%u, expected:1.", iEvent, handlerCalls);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_EVENT_CHILD_TYPE, childEvent.Type != ::llc::EVENT_GUI_CONTROL_StateChange, "Event:%u child type:%u, expected:%u.", iEvent, (::llc::u2_t)childEvent.Type, (::llc::u2_t)::llc::EVENT_GUI_CONTROL_StateChange);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_EVENT_PAYLOAD_SIZE, childEvent.Data.byte_count() != szof(::llc::SChangeControlState), "Event:%u payload bytes:%u, expected:%u.", iEvent, childEvent.Data.byte_count(), (::llc::u2_t)szof(::llc::SChangeControlState));

	::llc::vcu0_t				payload			= childEvent.Data;
	::llc::SChangeControlState	stateChange		= {};
	cnst ::llc::err_t loadResult = ::llc::loadPOD(payload, stateChange);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_EVENT_PAYLOAD_LOAD, ::llc::failed(loadResult), "Event:%u load result:%i.", iEvent, loadResult);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_EVENT_PAYLOAD_CURSOR, payload.size(), "Event:%u trailing payload bytes:%u.", iEvent, payload.size());
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_EVENT_CONTROL_ID, stateChange.Id != expectedId, "Event:%u control:%i, expected:%i.", iEvent, stateChange.Id, expectedId);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_EVENT_CLEAR_MASK, stateChange.Clear != expectedClear, "Event:%u clear:0x%X, expected:0x%X.", iEvent, (::llc::u2_t)stateChange.Clear, (::llc::u2_t)expectedClear);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_EVENT_SET_MASK, stateChange.Set != expectedSet, "Event:%u set:0x%X, expected:0x%X.", iEvent, (::llc::u2_t)stateChange.Set, (::llc::u2_t)expectedSet);
	rtrn 0;
}

sttc ::llc::err_t testGUIQueueEventPayload(ATestError & errors, cnst ::llc::TQueueSystemEvent & queue, ::llc::u2_c iEvent, ::llc::cid_t expectedId, ::llc::GUI_CONTROL_FLAG expectedClear, ::llc::GUI_CONTROL_FLAG expectedSet) {
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_EVENT_QUEUE_SIZE, queue.size() != iEvent + 1, "Event:%u queue size:%u, expected:%u.", iEvent, queue.size(), iEvent + 1);
	rtrn testGUIEventPayload(errors, queue[iEvent], iEvent, expectedId, expectedClear, expectedSet);
}

sttc ::llc::err_t testGUIControlEventPayload(ATestError & errors, cnst ::llc::TQueueSystemEvent & queue, ::llc::u2_c iEvent, ::llc::EVENT_GUI_CONTROL expectedType, ::llc::cid_t expectedId) {
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_EVENT_QUEUE_SIZE, queue.size() <= iEvent, "Event:%u queue size:%u.", iEvent, queue.size());
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_EVENT_REFERENCE, 0 == queue[iEvent].get_ref(), "Event:%u has no owned reference.", iEvent);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_EVENT_PARENT_TYPE, queue[iEvent]->Type != ::llc::SYSTEM_EVENT_GUI, "Event:%u parent type:%u, expected:%u.", iEvent, (::llc::u2_t)queue[iEvent]->Type, (::llc::u2_t)::llc::SYSTEM_EVENT_GUI);
	::llc::SEView<::llc::EVENT_GUI_CONTROL> childEvent = {};
	cnst ::llc::err_t dispatchResult = ::llc::eventExtractAndHandle<::llc::EVENT_GUI_CONTROL>(*queue[iEvent], [&childEvent](cnst ::llc::SEView<::llc::EVENT_GUI_CONTROL> & eventView) { childEvent = eventView; rtrn 0; });
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_EVENT_DISPATCH_RESULT, ::llc::failed(dispatchResult), "Event:%u dispatch result:%i.", iEvent, dispatchResult);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_EVENT_TYPE, childEvent.Type != expectedType, "Event:%u child type:%u, expected:%u.", iEvent, (::llc::u2_t)childEvent.Type, (::llc::u2_t)expectedType);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CONTROL_EVENT_PAYLOAD_SIZE, childEvent.Data.byte_count() != szof(::llc::SControlEvent), "Event:%u payload bytes:%u, expected:%u.", iEvent, childEvent.Data.byte_count(), (::llc::u2_t)szof(::llc::SControlEvent));
	::llc::vcu0_t payload = childEvent.Data;
	::llc::SControlEvent controlEvent = {};
	cnst ::llc::err_t loadResult = ::llc::loadPOD(payload, controlEvent);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CONTROL_EVENT_PAYLOAD_LOAD, ::llc::failed(loadResult), "Event:%u load result:%i.", iEvent, loadResult);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_EVENT_PAYLOAD_CURSOR, payload.size(), "Event:%u trailing payload bytes:%u.", iEvent, payload.size());
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_EVENT_ID, controlEvent.Id != expectedId, "Event:%u control:%i, expected:%i.", iEvent, controlEvent.Id, expectedId);
	rtrn 0;
}

sttc ::llc::err_t testGUIParentEventPayload(ATestError & errors, cnst ::llc::TQueueSystemEvent & queue, ::llc::u2_c iEvent, ::llc::cid_t expectedId, ::llc::cid_t expectedParentOld, ::llc::cid_t expectedParentNew) {
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_EVENT_QUEUE_SIZE, queue.size() <= iEvent, "Event:%u queue size:%u.", iEvent, queue.size());
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_EVENT_REFERENCE, 0 == queue[iEvent].get_ref(), "Event:%u has no owned reference.", iEvent);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_EVENT_PARENT_TYPE, queue[iEvent]->Type != ::llc::SYSTEM_EVENT_GUI, "Event:%u parent type:%u, expected:%u.", iEvent, (::llc::u2_t)queue[iEvent]->Type, (::llc::u2_t)::llc::SYSTEM_EVENT_GUI);
	::llc::SEView<::llc::EVENT_GUI_CONTROL> childEvent = {};
	cnst ::llc::err_t dispatchResult = ::llc::eventExtractAndHandle<::llc::EVENT_GUI_CONTROL>(*queue[iEvent], [&childEvent](cnst ::llc::SEView<::llc::EVENT_GUI_CONTROL> & eventView) { childEvent = eventView; rtrn 0; });
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_EVENT_DISPATCH_RESULT, ::llc::failed(dispatchResult), "Event:%u dispatch result:%i.", iEvent, dispatchResult);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_PARENT_EVENT_TYPE, childEvent.Type != ::llc::EVENT_GUI_CONTROL_ParentChange, "Event:%u child type:%u, expected:%u.", iEvent, (::llc::u2_t)childEvent.Type, (::llc::u2_t)::llc::EVENT_GUI_CONTROL_ParentChange);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CONTROL_PARENT_PAYLOAD_SIZE, childEvent.Data.byte_count() != szof(::llc::SChangeControlParent), "Event:%u payload bytes:%u, expected:%u.", iEvent, childEvent.Data.byte_count(), (::llc::u2_t)szof(::llc::SChangeControlParent));
	::llc::vcu0_t payload = childEvent.Data;
	::llc::SChangeControlParent parentChange = {};
	cnst ::llc::err_t loadResult = ::llc::loadPOD(payload, parentChange);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CONTROL_PARENT_PAYLOAD_LOAD, ::llc::failed(loadResult), "Event:%u load result:%i.", iEvent, loadResult);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_PARENT_PAYLOAD_CURSOR, payload.size(), "Event:%u trailing payload bytes:%u.", iEvent, payload.size());
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_PARENT_EVENT_ID, parentChange.Id != expectedId, "Event:%u control:%i, expected:%i.", iEvent, parentChange.Id, expectedId);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_PARENT_EVENT_OLD, parentChange.ParentOld != expectedParentOld, "Event:%u old parent:%i, expected:%i.", iEvent, parentChange.ParentOld, expectedParentOld);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_PARENT_EVENT_NEW, parentChange.ParentNew != expectedParentNew, "Event:%u new parent:%i, expected:%i.", iEvent, parentChange.ParentNew, expectedParentNew);
	rtrn 0;
}

sttc ::llc::err_t testGUIEventQueue(ATestError & errors) {
	::llc::TQueueSystemEvent	queue		= {};
	cnst ::llc::GUI_CONTROL_FLAG oldState = (::llc::GUI_CONTROL_FLAG)(::llc::GUI_CONTROL_FLAG_Hidden | ::llc::GUI_CONTROL_FLAG_Pressed);
	cnst ::llc::GUI_CONTROL_FLAG newState = (::llc::GUI_CONTROL_FLAG)(::llc::GUI_CONTROL_FLAG_Disabled | ::llc::GUI_CONTROL_FLAG_Pressed);
	cnst ::llc::err_t enqueueResult = ::llc::eventEnqueueGuiControlStateChange(queue, 7, oldState, newState);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_EVENT_ENQUEUE_RESULT, ::llc::failed(enqueueResult), "Enqueue result:%i.", enqueueResult);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_EVENT_QUEUE_SIZE, queue.size() != 1, "Queue size:%u, expected:1.", queue.size());
	if_fail_fe(testGUIEventPayload(errors, queue[0], 0, 7, ::llc::GUI_CONTROL_FLAG_Hidden, ::llc::GUI_CONTROL_FLAG_Disabled));

	cnst ::llc::err_t expectedHandlerResult = -17;
	cnst ::llc::err_t handlerResult = ::llc::eventExtractAndHandle<::llc::EVENT_GUI_CONTROL>(*queue[0], [expectedHandlerResult](cnst ::llc::SEView<::llc::EVENT_GUI_CONTROL> &) { rtrn expectedHandlerResult; });
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_EVENT_HANDLER_RESULT, handlerResult != expectedHandlerResult, "Handler result:%i, expected:%i.", handlerResult, expectedHandlerResult);

	auto						* eventReference	= queue[0].get_ref();
	::llc::TQueueSystemEvent	snapshot		= queue;
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_EVENT_SNAPSHOT_SIZE, snapshot.size() != queue.size(), "Snapshot size:%u, source:%u.", snapshot.size(), queue.size());
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_EVENT_SNAPSHOT_REFERENCE, snapshot[0].get_ref() != eventReference, "Snapshot reference:%p, source:%p.", snapshot[0].get_ref(), eventReference);
	queue.clear();
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_EVENT_SOURCE_CLEAR, queue.size(), "Source size after clear:%u.", queue.size());
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_EVENT_SNAPSHOT_LIFETIME, 0 == snapshot[0].get_ref(), "Snapshot reference after source clear:%p.", snapshot[0].get_ref());
	if_fail_fe(testGUIEventPayload(errors, snapshot[0], 0, 7, ::llc::GUI_CONTROL_FLAG_Hidden, ::llc::GUI_CONTROL_FLAG_Disabled));

	for(::llc::u2_t prefix = 0; prefix < snapshot[0]->Data.size(); ++prefix) {
		::llc::SEventSystem truncated = *snapshot[0];
		if_fail_fe(truncated.Data.resize(prefix));
		::llc::u2_t handlerCalls = 0;
		cnst ::llc::err_t truncatedResult = ::llc::eventExtractAndHandle<::llc::EVENT_GUI_CONTROL>(truncated, [&handlerCalls](cnst ::llc::SEView<::llc::EVENT_GUI_CONTROL> &) { ++handlerCalls; rtrn 0; });
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_EVENT_TRUNCATED_RESULT, false == ::llc::failed(truncatedResult), "Accepted prefix:%u of %u bytes.", prefix, snapshot[0]->Data.size());
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_EVENT_TRUNCATED_HANDLER, handlerCalls, "Prefix:%u invoked handler %u times.", prefix, handlerCalls);
	}
	rtrn 0;
}

sttc ::llc::err_t testGUIControlStateQueries(ATestError & errors) {
	for(::llc::u2_t iFlag = 0; iFlag < 0x100U; ++iFlag)
	for(::llc::u2_t iMask = 0; iMask < 0x100U; ++iMask) {
		cnst ::llc::SControlState state = {(::llc::GUI_CONTROL_FLAG)iFlag, (::llc::GUI_CONTROL_FLAG)iMask};
		cnst bool expectedHidden		= 0 != (iFlag & iMask & ::llc::GUI_CONTROL_FLAG_Hidden);
		cnst bool expectedDisabled	= 0 != (iFlag & iMask & ::llc::GUI_CONTROL_FLAG_Disabled);
		cnst bool expectedHovered	= 0 != (iFlag & iMask & ::llc::GUI_CONTROL_FLAG_Hovered);
		cnst bool expectedPressed	= 0 != (iFlag & iMask & ::llc::GUI_CONTROL_FLAG_Pressed);
		cnst bool expectedFocused	= 0 != (iFlag & iMask & ::llc::GUI_CONTROL_FLAG_Focused);
		cnst bool expectedSelected	= 0 != (iFlag & iMask & ::llc::GUI_CONTROL_FLAG_Selected);
		cnst bool expectedUpdated	= 0 != (iFlag & iMask & ::llc::GUI_CONTROL_FLAG_Updated);
		cnst bool expectedAction		= 0 != (iFlag & iMask & ::llc::GUI_CONTROL_FLAG_Action);
		cnst bool expectedUnused		= iFlag == ::llc::GUI_CONTROL_FLAG_Unused;
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_QUERY_HIDDEN, state.IsHidden() != expectedHidden, "Flag:0x%02X, mask:0x%02X, actual:%u, expected:%u.", iFlag, iMask, (::llc::u2_t)state.IsHidden(), (::llc::u2_t)expectedHidden);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_QUERY_DISABLED, state.IsDisabled() != expectedDisabled, "Flag:0x%02X, mask:0x%02X, actual:%u, expected:%u.", iFlag, iMask, (::llc::u2_t)state.IsDisabled(), (::llc::u2_t)expectedDisabled);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_QUERY_HOVERED, state.IsHovered() != expectedHovered, "Flag:0x%02X, mask:0x%02X, actual:%u, expected:%u.", iFlag, iMask, (::llc::u2_t)state.IsHovered(), (::llc::u2_t)expectedHovered);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_QUERY_PRESSED, state.IsPressed() != expectedPressed, "Flag:0x%02X, mask:0x%02X, actual:%u, expected:%u.", iFlag, iMask, (::llc::u2_t)state.IsPressed(), (::llc::u2_t)expectedPressed);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_QUERY_FOCUSED, state.IsFocused() != expectedFocused, "Flag:0x%02X, mask:0x%02X, actual:%u, expected:%u.", iFlag, iMask, (::llc::u2_t)state.IsFocused(), (::llc::u2_t)expectedFocused);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_QUERY_SELECTED, state.IsSelected() != expectedSelected, "Flag:0x%02X, mask:0x%02X, actual:%u, expected:%u.", iFlag, iMask, (::llc::u2_t)state.IsSelected(), (::llc::u2_t)expectedSelected);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_QUERY_UPDATED, state.IsUpdated() != expectedUpdated, "Flag:0x%02X, mask:0x%02X, actual:%u, expected:%u.", iFlag, iMask, (::llc::u2_t)state.IsUpdated(), (::llc::u2_t)expectedUpdated);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_QUERY_ACTION, state.IsAction() != expectedAction, "Flag:0x%02X, mask:0x%02X, actual:%u, expected:%u.", iFlag, iMask, (::llc::u2_t)state.IsAction(), (::llc::u2_t)expectedAction);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_QUERY_UNUSED, state.IsUnused() != expectedUnused, "Flag:0x%02X, mask:0x%02X, actual:%u, expected:%u.", iFlag, iMask, (::llc::u2_t)state.IsUnused(), (::llc::u2_t)expectedUnused);
	}
	rtrn 0;
}

tplt<tpnm TSetter>
sttc ::llc::err_t testGUIControlStateSetter(ATestError & errors, GUI_TEST_RESULT resultValue, GUI_TEST_RESULT resultReturn, ::llc::GUI_CONTROL_FLAG bit, TSetter setter) {
	for(::llc::u2_t iFlag = 0; iFlag < 0x100U; ++iFlag)
	for(::llc::u2_t iMask = 0; iMask < 0x100U; ++iMask)
	for(::llc::u2_t iValue = 0; iValue < 2; ++iValue) {
		cnst ::llc::GUI_CONTROL_FLAG mask		= (::llc::GUI_CONTROL_FLAG)iMask;
		::llc::GUI_CONTROL_FLAG state			= (::llc::GUI_CONTROL_FLAG)iFlag;
		cnst ::llc::GUI_CONTROL_FLAG returned	= setter(state, mask, 0 != iValue);
		cnst ::llc::GUI_CONTROL_FLAG expected	= (::llc::GUI_CONTROL_FLAG)((iMask & bit) ? (iValue ? iFlag | bit : iFlag & ~bit) : iFlag);
		LLC_TEST_CHECKF(errors, resultValue, state != expected, "Flag:0x%02X, mask:0x%02X, value:%u, stored:0x%02X, expected:0x%02X.", iFlag, iMask, iValue, (::llc::u2_t)state, (::llc::u2_t)expected);
		LLC_TEST_CHECKF(errors, resultReturn, returned != expected, "Flag:0x%02X, mask:0x%02X, value:%u, returned:0x%02X, expected:0x%02X.", iFlag, iMask, iValue, (::llc::u2_t)returned, (::llc::u2_t)expected);
	}
	rtrn 0;
}

sttc ::llc::err_t testGUIControlStatePressed(ATestError & errors) {
	for(::llc::u2_t iFlag = 0; iFlag < 0x100U; ++iFlag)
	for(::llc::u2_t iMask = 0; iMask < 0x100U; ++iMask)
	for(::llc::u2_t iValue = 0; iValue < 2; ++iValue) {
		cnst ::llc::GUI_CONTROL_FLAG mask		= (::llc::GUI_CONTROL_FLAG)iMask;
		::llc::GUI_CONTROL_FLAG state			= (::llc::GUI_CONTROL_FLAG)iFlag;
		cnst ::llc::GUI_CONTROL_FLAG returned	= ::llc::setPressed(state, mask, 0 != iValue);
		::llc::u2_t expected = (iMask & ::llc::GUI_CONTROL_FLAG_Pressed) ? (iValue ? iFlag | ::llc::GUI_CONTROL_FLAG_Pressed : iFlag & ~::llc::GUI_CONTROL_FLAG_Pressed) : iFlag;
		if(0 == iValue && (iFlag & iMask & ::llc::GUI_CONTROL_FLAG_Pressed) && (iFlag & iMask & ::llc::GUI_CONTROL_FLAG_Hovered) && (iMask & ::llc::GUI_CONTROL_FLAG_Action))
			expected |= ::llc::GUI_CONTROL_FLAG_Action;
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_SET_PRESSED_VALUE, state != expected, "Flag:0x%02X, mask:0x%02X, value:%u, stored:0x%02X, expected:0x%02X.", iFlag, iMask, iValue, (::llc::u2_t)state, expected);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_SET_PRESSED_RETURN, returned != expected, "Flag:0x%02X, mask:0x%02X, value:%u, returned:0x%02X, expected:0x%02X.", iFlag, iMask, iValue, (::llc::u2_t)returned, expected);
	}
	rtrn 0;
}

sttc ::llc::err_t testGUIControlStateUnused(ATestError & errors) {
	for(::llc::u2_t iFlag = 0; iFlag < 0x100U; ++iFlag)
	for(::llc::u2_t iValue = 0; iValue < 2; ++iValue) {
		::llc::GUI_CONTROL_FLAG state			= (::llc::GUI_CONTROL_FLAG)iFlag;
		cnst ::llc::GUI_CONTROL_FLAG returned	= ::llc::setUnused(state, 0 != iValue);
		cnst ::llc::GUI_CONTROL_FLAG expected	= iValue ? ::llc::GUI_CONTROL_FLAG_Unused : (::llc::GUI_CONTROL_FLAG)0;
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_SET_UNUSED_VALUE, state != expected, "Flag:0x%02X, value:%u, stored:0x%02X, expected:0x%02X.", iFlag, iValue, (::llc::u2_t)state, (::llc::u2_t)expected);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_SET_UNUSED_RETURN, returned != expected, "Flag:0x%02X, value:%u, returned:0x%02X, expected:0x%02X.", iFlag, iValue, (::llc::u2_t)returned, (::llc::u2_t)expected);
	}
	rtrn 0;
}

sttc ::llc::err_t testGUIControlStateMutations(ATestError & errors) {
	if_fail_fe(testGUIControlStateSetter(errors, GUI_TEST_RESULT_STATE_SET_HIDDEN_VALUE, GUI_TEST_RESULT_STATE_SET_HIDDEN_RETURN, ::llc::GUI_CONTROL_FLAG_Hidden, ::llc::setHidden));
	if_fail_fe(testGUIControlStateSetter(errors, GUI_TEST_RESULT_STATE_SET_DISABLED_VALUE, GUI_TEST_RESULT_STATE_SET_DISABLED_RETURN, ::llc::GUI_CONTROL_FLAG_Disabled, ::llc::setDisabled));
	if_fail_fe(testGUIControlStateSetter(errors, GUI_TEST_RESULT_STATE_SET_HOVERED_VALUE, GUI_TEST_RESULT_STATE_SET_HOVERED_RETURN, ::llc::GUI_CONTROL_FLAG_Hovered, ::llc::setHovered));
	if_fail_fe(testGUIControlStateSetter(errors, GUI_TEST_RESULT_STATE_SET_FOCUSED_VALUE, GUI_TEST_RESULT_STATE_SET_FOCUSED_RETURN, ::llc::GUI_CONTROL_FLAG_Focused, ::llc::setFocused));
	if_fail_fe(testGUIControlStateSetter(errors, GUI_TEST_RESULT_STATE_SET_SELECTED_VALUE, GUI_TEST_RESULT_STATE_SET_SELECTED_RETURN, ::llc::GUI_CONTROL_FLAG_Selected, ::llc::setSelected));
	if_fail_fe(testGUIControlStateSetter(errors, GUI_TEST_RESULT_STATE_SET_UPDATED_VALUE, GUI_TEST_RESULT_STATE_SET_UPDATED_RETURN, ::llc::GUI_CONTROL_FLAG_Updated, ::llc::setUpdated));
	if_fail_fe(testGUIControlStateSetter(errors, GUI_TEST_RESULT_STATE_SET_ACTION_VALUE, GUI_TEST_RESULT_STATE_SET_ACTION_RETURN, ::llc::GUI_CONTROL_FLAG_Action, ::llc::setAction));
	if_fail_fe(testGUIControlStatePressed(errors));
	rtrn testGUIControlStateUnused(errors);
}

sttc ::llc::err_t testGUIControlState(ATestError & errors) {
	if_fail_fe(testGUIControlStateQueries(errors));
	if_fail_fe(testGUIControlStateMutations(errors));
	::llc::SControlTable controls = {};
	cnst ::llc::err_t resizeResult = controls.States.resize(1);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_STATE_STORAGE_RESULT, ::llc::failed(resizeResult), "Resize result:%i.", resizeResult);
	::llc::SControlState & state = controls.States[0];

	controls.SetHovered(0, true);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_STATE_HOVERED, false == state.IsHovered());
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_HOVERED_EVENT, controls.EventQueue.size() != 1, "Queue size:%u, expected:1.", controls.EventQueue.size());
	if_fail_fe(testGUIQueueEventPayload(errors, controls.EventQueue, 0, 0, {}, ::llc::GUI_CONTROL_FLAG_Hovered));
	controls.SetHovered(0, true);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_STATE_HOVERED_REPEAT, false == state.IsHovered());
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_HOVERED_REPEAT_EVENT, controls.EventQueue.size() != 1, "Queue size:%u, expected:1.", controls.EventQueue.size());

	controls.SetPressed(0, true);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_STATE_PRESSED, false == state.IsPressed());
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_STATE_PRESSED_ACTION, state.IsAction());
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_PRESSED_EVENT, controls.EventQueue.size() != 2, "Queue size:%u, expected:2.", controls.EventQueue.size());
	if_fail_fe(testGUIQueueEventPayload(errors, controls.EventQueue, 1, 0, {}, ::llc::GUI_CONTROL_FLAG_Pressed));
	controls.SetPressed(0, false);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_STATE_RELEASED, state.IsPressed());
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_STATE_RELEASED_ACTION, false == state.IsAction());
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_RELEASED_EVENT, controls.EventQueue.size() != 3, "Queue size:%u, expected:3.", controls.EventQueue.size());
	if_fail_fe(testGUIQueueEventPayload(errors, controls.EventQueue, 2, 0, ::llc::GUI_CONTROL_FLAG_Pressed, ::llc::GUI_CONTROL_FLAG_Action));
	controls.SetAction(0, false);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_STATE_ACTION_CLEAR, state.IsAction());
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_ACTION_CLEAR_EVENT, controls.EventQueue.size() != 4, "Queue size:%u, expected:4.", controls.EventQueue.size());
	if_fail_fe(testGUIQueueEventPayload(errors, controls.EventQueue, 3, 0, ::llc::GUI_CONTROL_FLAG_Action, {}));

	state.Mask = ::llc::bit_clear(::llc::GUI_CONTROL_FLAG_All, ::llc::GUI_CONTROL_FLAG_Hidden);
	controls.SetHidden(0, true);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_STATE_MASKED_HIDDEN, state.IsHidden());
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_MASKED_HIDDEN_EVENT, controls.EventQueue.size() != 4, "Queue size:%u, expected:4.", controls.EventQueue.size());
	state.Mask = ::llc::GUI_CONTROL_FLAG_All;
	controls.SetHidden(0, true);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_STATE_HIDDEN, false == state.IsHidden());
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_HIDDEN_EVENT, controls.EventQueue.size() != 5, "Queue size:%u, expected:5.", controls.EventQueue.size());
	if_fail_fe(testGUIQueueEventPayload(errors, controls.EventQueue, 4, 0, {}, ::llc::GUI_CONTROL_FLAG_Hidden));

	controls.SetDisabled(0, true);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_STATE_DISABLED, false == state.IsDisabled());
	if_fail_fe(testGUIQueueEventPayload(errors, controls.EventQueue, 5, 0, {}, ::llc::GUI_CONTROL_FLAG_Disabled));
	controls.SetDisabled(0, false);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_STATE_DISABLED, state.IsDisabled());
	if_fail_fe(testGUIQueueEventPayload(errors, controls.EventQueue, 6, 0, ::llc::GUI_CONTROL_FLAG_Disabled, {}));
	controls.SetFocused(0, true);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_STATE_FOCUSED, false == state.IsFocused());
	if_fail_fe(testGUIQueueEventPayload(errors, controls.EventQueue, 7, 0, {}, ::llc::GUI_CONTROL_FLAG_Focused));
	controls.SetFocused(0, false);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_STATE_FOCUSED, state.IsFocused());
	if_fail_fe(testGUIQueueEventPayload(errors, controls.EventQueue, 8, 0, ::llc::GUI_CONTROL_FLAG_Focused, {}));
	controls.SetSelected(0, true);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_STATE_SELECTED, false == state.IsSelected());
	if_fail_fe(testGUIQueueEventPayload(errors, controls.EventQueue, 9, 0, {}, ::llc::GUI_CONTROL_FLAG_Selected));
	controls.SetSelected(0, false);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_STATE_SELECTED, state.IsSelected());
	if_fail_fe(testGUIQueueEventPayload(errors, controls.EventQueue, 10, 0, ::llc::GUI_CONTROL_FLAG_Selected, {}));
	controls.SetUpdated(0, true);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_STATE_UPDATED, false == state.IsUpdated());
	if_fail_fe(testGUIQueueEventPayload(errors, controls.EventQueue, 11, 0, {}, ::llc::GUI_CONTROL_FLAG_Updated));
	controls.SetUpdated(0, false);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_STATE_UPDATED, state.IsUpdated());
	if_fail_fe(testGUIQueueEventPayload(errors, controls.EventQueue, 12, 0, ::llc::GUI_CONTROL_FLAG_Updated, {}));

	cnst ::llc::GUI_CONTROL_FLAG stateBeforeUnused = state.Flag;
	controls.SetUnused(0, true);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_STATE_UNUSED_SET, false == state.IsUnused());
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_UNUSED_SET_EVENT, controls.EventQueue.size() != 14, "Queue size:%u, expected:14.", controls.EventQueue.size());
	if_fail_fe(testGUIQueueEventPayload(errors, controls.EventQueue, 13, 0, {}, ::llc::bit_clear(::llc::GUI_CONTROL_FLAG_All, stateBeforeUnused)));
	controls.SetUnused(0, false);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_STATE_UNUSED_CLEAR, state.Flag);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_UNUSED_CLEAR_EVENT, controls.EventQueue.size() != 15, "Queue size:%u, expected:15.", controls.EventQueue.size());
	if_fail_fe(testGUIQueueEventPayload(errors, controls.EventQueue, 14, 0, ::llc::GUI_CONTROL_FLAG_All, {}));
	rtrn 0;
}

sttc ::llc::err_t testGUIControlEntities(ATestError & errors) {
	::llc::SControlTable controls = {};
	cnst ::llc::cid_t idRoot = ::llc::controlCreate(controls, ::llc::vi16{});
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CONTROL_CREATE_RESULT, idRoot < 0, "Root creation result:%i.", idRoot);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_CREATE_ID, idRoot != 0, "Root id:%i, expected:0.", idRoot);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_ENTITY_STORAGE, controls.Entities.size() != 1, "Entity count:%u, expected:1.", controls.Entities.size());
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_STATE_STORAGE, controls.States.size() != 1, "State count:%u, expected:1.", controls.States.size());
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_PLACEMENT_STORAGE, controls.Placement.size() != 1, "Placement count:%u, expected:1.", controls.Placement.size());
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_AREA_STORAGE, controls.Area.size() != 1, "Area count:%u, expected:1.", controls.Area.size());
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_CONSTRAINT_STORAGE, controls.Constraints.size() != 1, "Constraint count:%u, expected:1.", controls.Constraints.size());
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_DRAW_STORAGE, controls.Draw.size() != 1, "Style count:%u, expected:1.", controls.Draw.size());
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_TEXT_STORAGE, controls.Text.size() != 1, "Text count:%u, expected:1.", controls.Text.size());
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_CHILD_STORAGE, controls.Children.size() != 1, "Child-list count:%u, expected:1.", controls.Children.size());
	if_fail_fe(testGUIControlEventPayload(errors, controls.EventQueue, 0, ::llc::EVENT_GUI_CONTROL_Create, idRoot));

	cnst ::llc::cid_t idChild = ::llc::controlCreate(controls, ::llc::vi16{});
	cnst ::llc::cid_t idOther = ::llc::controlCreate(controls, ::llc::vi16{});
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CONTROL_CREATE_RESULT, idChild < 0, "Child creation result:%i.", idChild);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CONTROL_CREATE_RESULT, idOther < 0, "Other creation result:%i.", idOther);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_CREATE_ID, idChild != 1, "Child id:%i, expected:1.", idChild);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_CREATE_ID, idOther != 2, "Other id:%i, expected:2.", idOther);
	if_fail_fe(testGUIControlEventPayload(errors, controls.EventQueue, 1, ::llc::EVENT_GUI_CONTROL_Create, idChild));
	if_fail_fe(testGUIControlEventPayload(errors, controls.EventQueue, 2, ::llc::EVENT_GUI_CONTROL_Create, idOther));

	cnst ::llc::err_t parentResult = ::llc::controlSetParent(controls, idChild, idRoot);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CONTROL_PARENT_RESULT, ::llc::failed(parentResult), "Parent result:%i.", parentResult);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_PARENT_VALUE, controls.Entities[idChild].Parent != idRoot, "Child parent:%i, expected:%i.", controls.Entities[idChild].Parent, idRoot);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_PARENT_CHILD_COUNT, controls.Children[idRoot].size() != 1, "Root child count:%u, expected:1.", controls.Children[idRoot].size());
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_PARENT_CHILD_ID, controls.Children[idRoot][0] != idChild, "Root child:%i, expected:%i.", controls.Children[idRoot][0], idChild);
	if_fail_fe(testGUIParentEventPayload(errors, controls.EventQueue, 3, idChild, ::llc::CID_INVALID, idRoot));

	cnst ::llc::u2_t eventCountBeforeRepeat = controls.EventQueue.size();
	cnst ::llc::err_t repeatResult = ::llc::controlSetParent(controls, idChild, idRoot);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CONTROL_PARENT_RESULT, ::llc::failed(repeatResult), "Repeated parent result:%i.", repeatResult);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_PARENT_REPEAT_EVENT, controls.EventQueue.size() != eventCountBeforeRepeat, "Queue size:%u, expected:%u.", controls.EventQueue.size(), eventCountBeforeRepeat);

	cnst ::llc::err_t reparentResult = ::llc::controlSetParent(controls, idChild, idOther);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CONTROL_PARENT_RESULT, ::llc::failed(reparentResult), "Reparent result:%i.", reparentResult);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_PARENT_VALUE, controls.Entities[idChild].Parent != idOther, "Child parent:%i, expected:%i.", controls.Entities[idChild].Parent, idOther);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_PARENT_CHILD_COUNT, controls.Children[idRoot].size(), "Old parent child count:%u, expected:0.", controls.Children[idRoot].size());
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_PARENT_CHILD_COUNT, controls.Children[idOther].size() != 1, "New parent child count:%u, expected:1.", controls.Children[idOther].size());
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_PARENT_CHILD_ID, controls.Children[idOther][0] != idChild, "New parent child:%i, expected:%i.", controls.Children[idOther][0], idChild);
	if_fail_fe(testGUIParentEventPayload(errors, controls.EventQueue, 4, idChild, idRoot, idOther));

	cnst ::llc::u2_t eventCountBeforeReject = controls.EventQueue.size();
	cnst ::llc::err_t selfResult = ::llc::controlSetParent(controls, idChild, idChild);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_PARENT_SELF_RESULT, false == ::llc::failed(selfResult), "Self-parent result:%i.", selfResult);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_PARENT_REJECT_VALUE, controls.Entities[idChild].Parent != idOther, "Parent after self rejection:%i, expected:%i.", controls.Entities[idChild].Parent, idOther);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_PARENT_REJECT_EVENT, controls.EventQueue.size() != eventCountBeforeReject, "Queue size after self rejection:%u, expected:%u.", controls.EventQueue.size(), eventCountBeforeReject);
	cnst ::llc::err_t cycleResult = ::llc::controlSetParent(controls, idOther, idChild);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_PARENT_CYCLE_RESULT, false == ::llc::failed(cycleResult), "Cycle result:%i.", cycleResult);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_PARENT_REJECT_VALUE, controls.Entities[idOther].Parent != ::llc::CID_INVALID, "Parent after cycle rejection:%i, expected:%i.", controls.Entities[idOther].Parent, ::llc::CID_INVALID);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_PARENT_REJECT_EVENT, controls.EventQueue.size() != eventCountBeforeReject, "Queue size after cycle rejection:%u, expected:%u.", controls.EventQueue.size(), eventCountBeforeReject);

	controls.Placement[idOther].Area.Offset = {3, 4};
	controls.Text[idOther].Text = "dirty";
	controls.Draw[idOther].NoBorder = true;
	cnst ::llc::err_t deleteResult = ::llc::controlDelete(controls, idOther, false);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CONTROL_DELETE_RESULT, ::llc::failed(deleteResult), "Delete result:%i.", deleteResult);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_CONTROL_DELETE_UNUSED, false == controls.IsUnused(idOther));
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_DELETE_CHILD_PARENT, controls.Entities[idChild].Parent != ::llc::CID_INVALID, "Detached child parent:%i, expected:%i.", controls.Entities[idChild].Parent, ::llc::CID_INVALID);
	if_fail_fe(testGUIParentEventPayload(errors, controls.EventQueue, 5, idChild, idOther, ::llc::CID_INVALID));
	if_fail_fe(testGUIControlEventPayload(errors, controls.EventQueue, 6, ::llc::EVENT_GUI_CONTROL_Delete, idOther));

	cnst ::llc::cid_t idReused = ::llc::controlCreate(controls, ::llc::vi16{});
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CONTROL_CREATE_RESULT, idReused < 0, "Reused creation result:%i.", idReused);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_REUSE_ID, idReused != idOther, "Reused id:%i, expected:%i.", idReused, idOther);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_REUSE_PARENT, controls.Entities[idReused].Parent != ::llc::CID_INVALID, "Reused parent:%i, expected:%i.", controls.Entities[idReused].Parent, ::llc::CID_INVALID);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_REUSE_CHILDREN, controls.Children[idReused].size(), "Reused child count:%u, expected:0.", controls.Children[idReused].size());
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_REUSE_PLACEMENT, controls.Placement[idReused].Area.Offset != ::llc::n2s1_t{}, "Reused offset:{%i, %i}, expected:{0, 0}.", controls.Placement[idReused].Area.Offset.x, controls.Placement[idReused].Area.Offset.y);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_REUSE_TEXT, controls.Text[idReused].Text.size(), "Reused text size:%u, expected:0.", controls.Text[idReused].Text.size());
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_REUSE_STYLE, controls.Draw[idReused].NoBorder, "Reused NoBorder:%u, expected:0.", controls.Draw[idReused].NoBorder);
	if_fail_fe(testGUIControlEventPayload(errors, controls.EventQueue, 7, ::llc::EVENT_GUI_CONTROL_Create, idReused));

	if_fail_fe(::llc::controlSetParent(controls, idChild, idRoot));
	if_fail_fe(::llc::controlSetParent(controls, idReused, idChild));
	cnst ::llc::err_t recursiveResult = ::llc::controlDelete(controls, idRoot, true);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CONTROL_DELETE_RESULT, ::llc::failed(recursiveResult), "Recursive delete result:%i.", recursiveResult);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_CONTROL_RECURSIVE_CHILD, false == controls.IsUnused(idReused));
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_CONTROL_RECURSIVE_CHILD, false == controls.IsUnused(idChild));
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_CONTROL_RECURSIVE_PARENT, false == controls.IsUnused(idRoot));
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_CONTROL_RECURSIVE_EVENT_ORDER, controls.EventQueue.size() != 13, "Queue size:%u, expected:13.", controls.EventQueue.size());
	if_fail_fe(testGUIControlEventPayload(errors, controls.EventQueue, 10, ::llc::EVENT_GUI_CONTROL_Delete, idReused));
	if_fail_fe(testGUIControlEventPayload(errors, controls.EventQueue, 11, ::llc::EVENT_GUI_CONTROL_Delete, idChild));
	if_fail_fe(testGUIControlEventPayload(errors, controls.EventQueue, 12, ::llc::EVENT_GUI_CONTROL_Delete, idRoot));
	rtrn 0;
}

::llc::err_t testGUIControl(ATestError & errors) {
	if_fail_fe(testGUIEventQueue(errors));
	if_fail_fe(testGUIControlState(errors));
	rtrn testGUIControlEntities(errors);
}
