#include "llc_gui.h"
#include "llc_gui_inputbox.h"
#include "llc_gui_viewport.h"
#include "llc_gui_text.h"
#include "llc_rect_align.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(GUI_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CREATE             ,  0, "A valid GUI control could not be created.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, TABLES             ,  1, "GUI control parallel storage lost index correspondence.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, PARENT             ,  2, "GUI parent and child linkage is inconsistent.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, INHERITED_STATE    ,  3, "A child did not inherit hidden or disabled state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, INVALIDATION       ,  4, "Metric invalidation did not reach the control hierarchy.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, DELETE             ,  5, "Recursive deletion left a live control.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, REUSE              ,  6, "Control creation did not reuse a deleted slot.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, METRICS            ,  7, "GUI metrics are inconsistent with placement and alignment.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, PENDING_METRICS    ,  8, "Input processing published pending geometry.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, HOVER              ,  9, "GUI hit testing selected the wrong control.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, PRESS              , 10, "GUI press capture was lost or created an early action.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, CHILD_CROSSING     , 11, "Crossing a child cleared its parent drag press.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, OUTSIDE_CAPTURE    , 12, "Leaving a hierarchy cleared its held press.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, NO_HOVER           , 13, "No-hover input returned failure instead of the control-count sentinel.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, OUTSIDE_RELEASE    , 14, "Outside release retained press or generated action.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, DRAW               , 15, "GUI drawing did not modify a visible control area.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, HIDDEN_DRAW        , 16, "A hidden control changed the render target.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, KEYBOARD_SETUP     , 17, "Virtual-keyboard hierarchy or geometry is invalid.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, KEYBOARD_EVENT     , 18, "Virtual-keyboard event mapping is invalid.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, INPUTBOX           , 19, "Input-box hierarchy, visibility, or text state is invalid.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, VIEWPORT           , 20, "Viewport hierarchy or control geometry is invalid.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_QUERY        , 21, "A control-state query ignored its flag mask.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_MUTATION     , 22, "A masked control-state mutation stored or returned the wrong value.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_PRESS        , 23, "Pressed-state mutation violated release-to-action semantics.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_UNUSED       , 24, "Unused-state mutation violated the full-byte sentinel contract.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_EVENT        , 25, "A control-state mutation emitted the wrong event payload.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, STATE_REPEAT       , 26, "An unchanged control state emitted a redundant event.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, REPARENT           , 27, "Control reparenting left inconsistent parent or child storage.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, PARENT_REJECT      , 28, "A rejected parent assignment changed the hierarchy.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, DELETE_DETACH      , 29, "Non-recursive deletion left children attached to the deleted control.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, REUSE_RESET        , 30, "A reused control slot retained state from its previous lifetime.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, METRIC_ALIGN       , 31, "GUI metrics diverged from rectangle alignment.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, METRIC_AREA        , 32, "GUI total or client rectangles are inconsistent.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, METRIC_HIERARCHY   , 33, "Child metrics are inconsistent with the parent client rectangle.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, METRIC_CONSTRAINT  , 34, "GUI size constraints produced inconsistent metrics.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, METRIC_ATTACHMENT  , 35, "GUI size attachment produced inconsistent metrics.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, METRIC_STABILITY   , 36, "An unchanged metric update changed geometry or emitted an event.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, HIT_BOUNDARY       , 37, "GUI hit testing violated rectangle boundaries.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, HIT_ORDER          , 38, "GUI hit testing violated hierarchy or display order.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, HIT_ELIGIBILITY    , 39, "Hidden or disabled controls participated in hit testing.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, INPUT_SEQUENCE     , 40, "GUI hover, press, release, or action sequencing is inconsistent.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, INPUT_STABILITY    , 41, "Stable input retained stale events or changed control state.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, DRAW_PALETTE       , 42, "Control state selected the wrong drawing palette.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, DRAW_CLIENT        , 43, "Client drawing or suppression changed the wrong pixels.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, DRAW_BORDER        , 44, "Border drawing or suppression changed the wrong pixels.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, DRAW_ORDER         , 45, "GUI hierarchy drawing order is inconsistent.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, DRAW_CLIP          , 46, "GUI drawing escaped the target or control intersection.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, TEXT_WIDTH         , 47, "Fixed-cell text width is inconsistent.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, TEXT_RASTER        , 48, "Text rasterization produced inconsistent coordinates.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, TEXT_CLIP          , 49, "Text rasterization escaped the render target.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, TEXT_ALIGN         , 50, "Control text alignment is inconsistent with its client rectangle.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, TEXT_PALETTE       , 51, "Control text used the wrong palette face color.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, TEXT_STATE         , 52, "Hovered or pressed text did not preserve the one-pixel state offset.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, TEXT_FONT          , 53, "Control text selected the wrong raster font.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, IMAGE_ALIGN        , 54, "Control image alignment is inconsistent with its client rectangle.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, IMAGE_OFFSET       , 55, "Control image source offset selected the wrong texels.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, IMAGE_STATE        , 56, "Hovered image did not preserve the one-pixel state offset.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, IMAGE_INVERT       , 57, "Control image Y inversion produced the wrong texels.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, IMAGE_CLIP         , 58, "Control image escaped the target or client rectangle.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, KEYBOARD_GEOMETRY  , 59, "Virtual-keyboard control geometry is inconsistent.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, KEYBOARD_LABEL     , 60, "Virtual-keyboard control text is inconsistent.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, KEYBOARD_COMMAND   , 61, "Virtual-keyboard command mapping is inconsistent.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, KEYBOARD_INPUT     , 62, "Virtual-keyboard input did not preserve press-release activation.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, KEYBOARD_MOVE      , 63, "Moving the virtual-keyboard root detached its children.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, VIEWPORT_PRESS     , 64, "Viewport press aggregation ignored the root, title, or target control.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, VIEWPORT_ACTIVATE  , 65, "Viewport activation did not update display order or selection.");
GDEFINE_ENUM_VALUED(GUI_TEST_RESULT, VIEWPORT_FOLD      , 66, "Viewport folding did not preserve and restore client geometry or visibility.");

sttc ::llc::err_t testGUITextFontCreate(::llc::po<::llc::SRasterFont> & font, ::llc::n2u0_c & charSize) {
	font->CharSize = charSize;
	if_fail_fe(font->Texture.resize(16U * charSize.x, 16U * charSize.y));
	rtrn font->Texture.Texels.fill(0);
}

sttc ::llc::err_t testGUITextCore(ATestError & errors) {
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_TEXT_WIDTH, ::llc::textLineCalcWidth({3, 5}, LLC_CXS("TEXT")) != 12);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_TEXT_WIDTH, ::llc::textLineCalcWidth({3, 5}, {}) != 0);
	::llc::po<::llc::SRasterFont> font = {};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_TEXT_RASTER, 0 > ::testGUITextFontCreate(font, {2, 2}), "%s", "Font creation failed.");
	cnst ::llc::u0_t character = 'A';
	cnst ::llc::n2u2_t cell = {(character % 16U) * 2U, (character / 16U) * 2U};
	font->Texture.View[(cell.y + 0U) * font->Texture.Pitch + cell.x + 1U] = true;
	font->Texture.View[(cell.y + 1U) * font->Texture.Pitch + cell.x + 0U] = true;
	::llc::apod<::llc::n2u1_t> coordinates = {{99, 98}};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_TEXT_RASTER, 0 > ::llc::textLineRaster({8, 6}, font->CharSize, {{2, 1}, {4, 2}}, font->Texture, LLC_CXS("AA"), coordinates), "%s", "Raster failed.");
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_TEXT_RASTER, coordinates.size() != 5, "Coordinate count:%u expected:5.", coordinates.size());
	stxp ::llc::n2u1_t expected[] = {{99, 98}, {3, 1}, {2, 2}, {5, 1}, {4, 2}};
	for(::llc::u2_t iCoord = 0; iCoord < ::llc::size(expected) && iCoord < coordinates.size(); ++iCoord)
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_TEXT_RASTER, coordinates[iCoord] != expected[iCoord], "Coordinate:%u actual:{%u,%u} expected:{%u,%u}.", iCoord, coordinates[iCoord].x, coordinates[iCoord].y, expected[iCoord].x, expected[iCoord].y);
	coordinates.clear();
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_TEXT_CLIP, 0 > ::llc::textLineRaster({3, 2}, font->CharSize, {{-1, -1}, {4, 2}}, font->Texture, LLC_CXS("AA"), coordinates), "%s", "Clipped raster failed.");
	cnst ::llc::n2u1_t clipped = {1, 0};
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_TEXT_CLIP, coordinates.size() != 1 || coordinates[0] != clipped, "Coordinate count:%u.", coordinates.size());
	rtrn 0;
}

sttc ::llc::err_t testGUIStateEvent(ATestError & errors, cnst ::llc::TQueueSystemEvent & queue, ::llc::u2_c iEvent, ::llc::cid_t expectedId, ::llc::GUI_CONTROL_FLAG expectedClear, ::llc::GUI_CONTROL_FLAG expectedSet) {
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_STATE_EVENT, queue.size() <= iEvent || 0 == queue[iEvent].get_ref(), "Event:%u queue size:%u.", iEvent, queue.size());
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_STATE_EVENT, queue[iEvent]->Type != ::llc::SYSTEM_EVENT_GUI, "Event:%u parent type:%u.", iEvent, (::llc::u2_t)queue[iEvent]->Type);
	::llc::SEView<::llc::EVENT_GUI_CONTROL> childEvent = {};
	cnst ::llc::err_t dispatchResult = ::llc::eventExtractAndHandle<::llc::EVENT_GUI_CONTROL>(*queue[iEvent], [&childEvent](cnst ::llc::SEView<::llc::EVENT_GUI_CONTROL> & eventView) { childEvent = eventView; rtrn 0; });
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_STATE_EVENT, ::llc::failed(dispatchResult) || childEvent.Type != ::llc::EVENT_GUI_CONTROL_StateChange, "Event:%u dispatch:%i child:%u.", iEvent, dispatchResult, (::llc::u2_t)childEvent.Type);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_STATE_EVENT, childEvent.Data.byte_count() != szof(::llc::SChangeControlState), "Event:%u payload:%u.", iEvent, childEvent.Data.byte_count());
	::llc::vcu0_t payload = childEvent.Data;
	::llc::SChangeControlState stateChange = {};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_STATE_EVENT, ::llc::failed(::llc::loadPOD(payload, stateChange)), "Event:%u payload load failed.", iEvent);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_EVENT, payload.size() || stateChange.Id != expectedId || stateChange.Clear != expectedClear || stateChange.Set != expectedSet, "Event:%u id:%i clear:0x%X set:0x%X.", iEvent, stateChange.Id, (::llc::u2_t)stateChange.Clear, (::llc::u2_t)stateChange.Set);
	rtrn 0;
}

sttc ::llc::err_t testGUIControlStateQueries(ATestError & errors) {
	for(::llc::u2_t iFlag = 0; iFlag < 0x100U; ++iFlag)
	for(::llc::u2_t iMask = 0; iMask < 0x100U; ++iMask) {
		::llc::SControlState state = {};
		state.Flag = (::llc::GUI_CONTROL_FLAG)iFlag;
		state.Mask = (::llc::GUI_CONTROL_FLAG)iMask;
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_QUERY, state.IsHidden  () != (0 != (iFlag & iMask & ::llc::GUI_CONTROL_FLAG_Hidden  )), "Hidden flag:0x%02X mask:0x%02X.", iFlag, iMask);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_QUERY, state.IsDisabled() != (0 != (iFlag & iMask & ::llc::GUI_CONTROL_FLAG_Disabled)), "Disabled flag:0x%02X mask:0x%02X.", iFlag, iMask);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_QUERY, state.IsHovered () != (0 != (iFlag & iMask & ::llc::GUI_CONTROL_FLAG_Hovered )), "Hovered flag:0x%02X mask:0x%02X.", iFlag, iMask);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_QUERY, state.IsPressed () != (0 != (iFlag & iMask & ::llc::GUI_CONTROL_FLAG_Pressed )), "Pressed flag:0x%02X mask:0x%02X.", iFlag, iMask);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_QUERY, state.IsFocused () != (0 != (iFlag & iMask & ::llc::GUI_CONTROL_FLAG_Focused )), "Focused flag:0x%02X mask:0x%02X.", iFlag, iMask);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_QUERY, state.IsSelected() != (0 != (iFlag & iMask & ::llc::GUI_CONTROL_FLAG_Selected)), "Selected flag:0x%02X mask:0x%02X.", iFlag, iMask);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_QUERY, state.IsUpdated () != (0 != (iFlag & iMask & ::llc::GUI_CONTROL_FLAG_Updated )), "Updated flag:0x%02X mask:0x%02X.", iFlag, iMask);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_QUERY, state.IsAction  () != (0 != (iFlag & iMask & ::llc::GUI_CONTROL_FLAG_Action  )), "Action flag:0x%02X mask:0x%02X.", iFlag, iMask);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_QUERY, state.IsUnused  () != (iFlag == ::llc::GUI_CONTROL_FLAG_Unused), "Unused flag:0x%02X mask:0x%02X.", iFlag, iMask);
	}
	rtrn 0;
}

tplt<tpnm TSetter>
sttc ::llc::err_t testGUIControlStateSetter(ATestError & errors, ::llc::GUI_CONTROL_FLAG bit, TSetter setter) {
	for(::llc::u2_t iFlag = 0; iFlag < 0x100U; ++iFlag)
	for(::llc::u2_t iMask = 0; iMask < 0x100U; ++iMask)
	for(::llc::u2_t iValue = 0; iValue < 2; ++iValue) {
		cnst ::llc::GUI_CONTROL_FLAG mask = (::llc::GUI_CONTROL_FLAG)iMask;
		::llc::GUI_CONTROL_FLAG state = (::llc::GUI_CONTROL_FLAG)iFlag;
		cnst ::llc::GUI_CONTROL_FLAG returned = setter(state, mask, 0 != iValue);
		cnst ::llc::GUI_CONTROL_FLAG expected = (::llc::GUI_CONTROL_FLAG)((iMask & bit) ? (iValue ? iFlag | bit : iFlag & ~bit) : iFlag);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_MUTATION, state != expected || returned != expected, "Flag:0x%02X mask:0x%02X value:%u stored:0x%02X returned:0x%02X expected:0x%02X.", iFlag, iMask, iValue, (::llc::u2_t)state, (::llc::u2_t)returned, (::llc::u2_t)expected);
	}
	rtrn 0;
}

sttc ::llc::err_t testGUIControlStateMutations(ATestError & errors) {
	if_fail_fe(::testGUIControlStateSetter(errors, ::llc::GUI_CONTROL_FLAG_Hidden  , ::llc::setHidden));
	if_fail_fe(::testGUIControlStateSetter(errors, ::llc::GUI_CONTROL_FLAG_Disabled, ::llc::setDisabled));
	if_fail_fe(::testGUIControlStateSetter(errors, ::llc::GUI_CONTROL_FLAG_Hovered , ::llc::setHovered));
	if_fail_fe(::testGUIControlStateSetter(errors, ::llc::GUI_CONTROL_FLAG_Focused , ::llc::setFocused));
	if_fail_fe(::testGUIControlStateSetter(errors, ::llc::GUI_CONTROL_FLAG_Selected, ::llc::setSelected));
	if_fail_fe(::testGUIControlStateSetter(errors, ::llc::GUI_CONTROL_FLAG_Updated , ::llc::setUpdated));
	if_fail_fe(::testGUIControlStateSetter(errors, ::llc::GUI_CONTROL_FLAG_Action  , ::llc::setAction));
	for(::llc::u2_t iFlag = 0; iFlag < 0x100U; ++iFlag)
	for(::llc::u2_t iMask = 0; iMask < 0x100U; ++iMask)
	for(::llc::u2_t iValue = 0; iValue < 2; ++iValue) {
		cnst ::llc::GUI_CONTROL_FLAG mask = (::llc::GUI_CONTROL_FLAG)iMask;
		::llc::GUI_CONTROL_FLAG state = (::llc::GUI_CONTROL_FLAG)iFlag;
		cnst ::llc::GUI_CONTROL_FLAG returned = ::llc::setPressed(state, mask, 0 != iValue);
		::llc::u2_t expected = (iMask & ::llc::GUI_CONTROL_FLAG_Pressed) ? (iValue ? iFlag | ::llc::GUI_CONTROL_FLAG_Pressed : iFlag & ~::llc::GUI_CONTROL_FLAG_Pressed) : iFlag;
		if(0 == iValue && (iFlag & iMask & ::llc::GUI_CONTROL_FLAG_Pressed) && (iFlag & iMask & ::llc::GUI_CONTROL_FLAG_Hovered) && (iMask & ::llc::GUI_CONTROL_FLAG_Action))
			expected |= ::llc::GUI_CONTROL_FLAG_Action;
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_PRESS, state != expected || returned != expected, "Flag:0x%02X mask:0x%02X value:%u stored:0x%02X returned:0x%02X expected:0x%02X.", iFlag, iMask, iValue, (::llc::u2_t)state, (::llc::u2_t)returned, expected);
	}
	for(::llc::u2_t iFlag = 0; iFlag < 0x100U; ++iFlag)
	for(::llc::u2_t iValue = 0; iValue < 2; ++iValue) {
		::llc::GUI_CONTROL_FLAG state = (::llc::GUI_CONTROL_FLAG)iFlag;
		cnst ::llc::GUI_CONTROL_FLAG returned = ::llc::setUnused(state, 0 != iValue);
		cnst ::llc::GUI_CONTROL_FLAG expected = iValue ? ::llc::GUI_CONTROL_FLAG_Unused : (::llc::GUI_CONTROL_FLAG)0;
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_UNUSED, state != expected || returned != expected, "Flag:0x%02X value:%u stored:0x%02X returned:0x%02X.", iFlag, iValue, (::llc::u2_t)state, (::llc::u2_t)returned);
	}
	rtrn 0;
}

sttc ::llc::err_t testGUIControlStateEvents(ATestError & errors) {
	::llc::SControlTable controls = {};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_STATE_EVENT, 0 > controls.States.resize(1), "%s", "State allocation failed.");
	controls.SetHovered(0, true);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_STATE_EVENT, false == controls.IsHovered(0));
	if_fail_fe(::testGUIStateEvent(errors, controls.EventQueue, 0, 0, {}, ::llc::GUI_CONTROL_FLAG_Hovered));
	controls.SetHovered(0, true);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_REPEAT, controls.EventQueue.size() != 1, "Queue size:%u expected:1.", controls.EventQueue.size());
	controls.SetPressed(0, true);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_STATE_PRESS, false == controls.IsPressed(0) || controls.IsAction(0));
	if_fail_fe(::testGUIStateEvent(errors, controls.EventQueue, 1, 0, {}, ::llc::GUI_CONTROL_FLAG_Pressed));
	controls.SetPressed(0, false);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_STATE_PRESS, controls.IsPressed(0) || false == controls.IsAction(0));
	if_fail_fe(::testGUIStateEvent(errors, controls.EventQueue, 2, 0, ::llc::GUI_CONTROL_FLAG_Pressed, ::llc::GUI_CONTROL_FLAG_Action));
	controls.SetAction(0, false);
	if_fail_fe(::testGUIStateEvent(errors, controls.EventQueue, 3, 0, ::llc::GUI_CONTROL_FLAG_Action, {}));
	controls.States[0].Mask = ::llc::bit_clear(::llc::GUI_CONTROL_FLAG_All, ::llc::GUI_CONTROL_FLAG_Hidden);
	controls.SetHidden(0, true);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_STATE_MUTATION, controls.IsHidden(0));
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_STATE_REPEAT, controls.EventQueue.size() != 4, "Queue size:%u expected:4.", controls.EventQueue.size());
	rtrn 0;
}

sttc ::llc::err_t testGUIControlLifecycle(ATestError & errors) {
	::llc::SGUI gui = {};
	cnst ::llc::cid_t root = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CREATE, root < 0, "Root result:%i.", root);
	cnst ::llc::cid_t child = ::llc::controlCreateChild(gui, root);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CREATE, child < 0, "Child result:%i.", child);
	cnst ::llc::u2_t count = gui.Controls.States.size();
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_TABLES, gui.Controls.Placement.size() != count || gui.Controls.Area.size() != count || gui.Controls.Constraints.size() != count || gui.Controls.Draw.size() != count || gui.Controls.Text.size() != count || gui.Controls.Images.size() != count || gui.Controls.Children.size() != count);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_PARENT, gui.Controls.States[child].Parent != root || gui.Controls.Children[root].size() != 1 || gui.Controls.Children[root][0] != child);
	gui.Controls.SetHidden(root, true);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_INHERITED_STATE, false == ::llc::controlHidden(gui, child));
	gui.Controls.SetHidden(root, false);
	gui.Controls.SetDisabled(root, true);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_INHERITED_STATE, false == ::llc::controlDisabled(gui, child));
	gui.Controls.SetDisabled(root, false);
	cnst ::llc::cid_t parentNew = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CREATE, parentNew < 0, "Second parent result:%i.", parentNew);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_REPARENT, 0 > ::llc::controlSetParent(gui, child, parentNew), "%s", "Reparent failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_REPARENT, gui.Controls.States[child].Parent != parentNew || gui.Controls.Children[root].size() || gui.Controls.Children[parentNew].size() != 1 || gui.Controls.Children[parentNew][0] != child);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_REPARENT, 0 > ::llc::controlSetParent(gui, child, parentNew), "%s", "Repeated parent failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_REPARENT, gui.Controls.Children[parentNew].size() != 1);
	cnst ::llc::cid_t parentBeforeReject = gui.Controls.States[child].Parent;
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_PARENT_REJECT, 0 <= ::llc::controlSetParent(gui, child, (::llc::cid_t)0x7FFF));
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_PARENT_REJECT, gui.Controls.States[child].Parent != parentBeforeReject || gui.Controls.Children[parentNew].size() != 1);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_PARENT_REJECT, 0 <= ::llc::controlSetParent(gui, child, child));
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_PARENT_REJECT, gui.Controls.States[child].Parent != parentBeforeReject || gui.Controls.Children[parentNew].size() != 1);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_PARENT_REJECT, 0 <= ::llc::controlSetParent(gui, parentNew, child));
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_PARENT_REJECT, gui.Controls.States[parentNew].Parent != ::llc::CID_INVALID || gui.Controls.Children[parentNew].size() != 1);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_REPARENT, 0 > ::llc::controlSetParent(gui, child, ::llc::CID_INVALID), "%s", "Detach failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_REPARENT, gui.Controls.States[child].Parent != ::llc::CID_INVALID || gui.Controls.Children[parentNew].size());
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_REPARENT, 0 > ::llc::controlSetParent(gui, child, parentNew), "%s", "Reattach failed.");
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_DELETE_DETACH, 0 > ::llc::controlDelete(gui, parentNew, false), "%s", "Non-recursive delete failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_DELETE_DETACH, false == ::llc::controlInvalid(gui, parentNew) || ::llc::controlInvalid(gui, child) || gui.Controls.States[child].Parent != ::llc::CID_INVALID);
	cnst ::llc::cid_t reusedParent = ::llc::controlCreate(gui);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_REUSE, reusedParent != parentNew, "Reused:%i expected:%i.", reusedParent, parentNew);
	gui.Controls.Placement[reusedParent].Area = {{7, 9}, {31, 27}};
	gui.Controls.Text[reusedParent].Text = LLC_CXS("STALE");
	gui.Controls.Draw[reusedParent].FrameOut = true;
	gui.Controls.SetSelected(reusedParent, true);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_DELETE, 0 > ::llc::controlDelete(gui, reusedParent, true), "%s", "Prepared-slot delete failed.");
	cnst ::llc::cid_t resetControl = ::llc::controlCreate(gui);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_REUSE, resetControl != reusedParent, "Reused:%i expected:%i.", resetControl, reusedParent);
	cnst ::llc::SControlPlacement placementDefault = {};
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_REUSE_RESET, gui.Controls.States[resetControl].Parent != ::llc::CID_INVALID || gui.Controls.States[resetControl].Flag || gui.Controls.Placement[resetControl].Area != placementDefault.Area || gui.Controls.Text[resetControl].Text.size() || gui.Controls.Draw[resetControl].FrameOut || gui.Controls.Children[resetControl].size());
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_REPARENT, 0 > ::llc::controlSetParent(gui, child, root), "%s", "Recursive hierarchy restore failed.");
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_METRICS, 0 > ::llc::guiUpdateMetrics(gui, {120, 90}, true), "%s", "Initial metrics failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_METRICS, false == gui.Controls.IsUpdated(root) || false == gui.Controls.IsUpdated(child));
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_INVALIDATION, 0 > ::llc::controlMetricsInvalidate(gui, root), "%s", "Invalidation failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_INVALIDATION, gui.Controls.IsUpdated(root) || gui.Controls.IsUpdated(child));
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_DELETE, 0 > ::llc::controlDelete(gui, root, true), "%s", "Recursive delete failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_DELETE, false == ::llc::controlInvalid(gui, root) || false == ::llc::controlInvalid(gui, child));
	cnst ::llc::cid_t reused = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CREATE, reused < 0, "Reuse result:%i.", reused);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_REUSE, reused != root, "Reused:%i, expected:%i.", reused, root);
	rtrn 0;
}

sttc ::llc::err_t testGUIMetricAlignment(ATestError & errors) {
	stxp ::llc::ALIGN alignments[] =
		{ ::llc::ALIGN_TOP_LEFT, ::llc::ALIGN_TOP_RIGHT, ::llc::ALIGN_BOTTOM_LEFT, ::llc::ALIGN_BOTTOM_RIGHT
		, ::llc::ALIGN_CENTER_TOP, ::llc::ALIGN_CENTER_BOTTOM, ::llc::ALIGN_CENTER_LEFT, ::llc::ALIGN_CENTER_RIGHT, ::llc::ALIGN_CENTER
		};
	::llc::SGUI gui = {};
	cnst ::llc::cid_t control = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CREATE, control < 0, "Control result:%i.", control);
	::llc::SControlPlacement & placement = gui.Controls.Placement[control];
	placement.Area = {{7, 11}, {20, 14}};
	placement.Margin = placement.Border = {};
	cnst ::llc::n2u1_t targetSize = {100, 80};
	for(::llc::u2_t iAlign = 0; iAlign < ::llc::size(alignments); ++iAlign) {
		placement.Align = alignments[iAlign];
		::llc::rect2s1_t expected = {};
		::llc::realignRectangle(targetSize.u2(), placement.Area, expected, placement.Align);
		LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_METRIC_ALIGN, 0 > ::llc::guiUpdateMetrics(gui, targetSize, true), "Alignment:%s.", ::llc::get_value_namep(placement.Align));
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_METRIC_ALIGN, gui.Controls.Area[control].Total.Local != expected, "Alignment:%s.", ::llc::get_value_namep(placement.Align));
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_METRIC_ALIGN, gui.Controls.Area[control].Total.Global != expected, "Alignment:%s.", ::llc::get_value_namep(placement.Align));
	}
	rtrn 0;
}

sttc ::llc::err_t testGUIMetricAreas(ATestError & errors) {
	::llc::SGUI gui = {};
	cnst ::llc::cid_t control = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CREATE, control < 0, "Control result:%i.", control);
	::llc::SControlPlacement & placement = gui.Controls.Placement[control];
	placement.Area = {{10, 12}, {50, 40}};
	placement.Margin = {2, 3, 4, 5};
	placement.Border = {1, 2, 3, 4};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_METRIC_AREA, 0 > ::llc::guiUpdateMetrics(gui, {100, 80}, true), "%s", "Area metrics failed.");
	cnst ::llc::SControlArea & area = gui.Controls.Area[control];
	cnst ::llc::rect2s1_t totalExpected = placement.Area;
	cnst ::llc::rect2s1_t clientLocalExpected = {{3, 5}, {40, 26}};
	cnst ::llc::rect2s1_t clientGlobalExpected = {{13, 17}, {40, 26}};
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_METRIC_AREA, area.Total.Local != totalExpected);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_METRIC_AREA, area.Total.Global != totalExpected);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_METRIC_AREA, area.Client.Local != clientLocalExpected);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_METRIC_AREA, area.Client.Global != clientGlobalExpected);
	rtrn 0;
}

sttc ::llc::err_t testGUIMetricHierarchy(ATestError & errors) {
	::llc::SGUI gui = {};
	cnst ::llc::cid_t parent = ::llc::controlCreate(gui);
	cnst ::llc::cid_t child = ::llc::controlCreateChild(gui, parent);
	cnst ::llc::cid_t unrelated = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CREATE, parent < 0 || child < 0 || unrelated < 0, "Parent:%i child:%i unrelated:%i.", parent, child, unrelated);
	gui.Controls.Placement[parent].Area = {{10, 12}, {60, 40}};
	gui.Controls.Placement[parent].Margin = gui.Controls.Placement[parent].Border = {1, 1, 1, 1};
	gui.Controls.Placement[child].Area = {{4, 5}, {20, 10}};
	gui.Controls.Placement[child].Margin = gui.Controls.Placement[child].Border = {};
	gui.Controls.Placement[child].Align = ::llc::ALIGN_BOTTOM_RIGHT;
	gui.Controls.Placement[unrelated].Area = {{70, 2}, {10, 10}};
	gui.Controls.Placement[unrelated].Margin = gui.Controls.Placement[unrelated].Border = {};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_METRIC_HIERARCHY, 0 > ::llc::guiUpdateMetrics(gui, {100, 80}, true), "%s", "Hierarchy metrics failed.");
	::llc::rect2s1_t childLocalExpected = {};
	::llc::realignRectangle(gui.Controls.Area[parent].Client.Global.Size.u2(), gui.Controls.Placement[child].Area, childLocalExpected, gui.Controls.Placement[child].Align);
	cnst ::llc::n2s1_t childGlobalExpected = childLocalExpected.Offset + gui.Controls.Area[parent].Client.Global.Offset;
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_METRIC_HIERARCHY, gui.Controls.Area[child].Total.Local != childLocalExpected);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_METRIC_HIERARCHY, gui.Controls.Area[child].Total.Global.Offset != childGlobalExpected);
	cnst ::llc::SControlRectangle unrelatedBefore = gui.Controls.Area[unrelated].Total;
	cnst ::llc::u2_t eventsBefore = gui.Controls.EventQueue.size();
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_INVALIDATION, 0 > ::llc::controlMetricsInvalidate(gui, parent), "%s", "Hierarchy invalidation failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_INVALIDATION, gui.Controls.IsUpdated(parent) || gui.Controls.IsUpdated(child) || false == gui.Controls.IsUpdated(unrelated));
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_METRIC_HIERARCHY, 0 > ::llc::guiUpdateMetrics(gui, {100, 80}, false), "%s", "Hierarchy recalculation failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_METRIC_HIERARCHY, false == gui.Controls.IsUpdated(parent) || false == gui.Controls.IsUpdated(child));
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_METRIC_HIERARCHY, gui.Controls.Area[unrelated].Total.Local != unrelatedBefore.Local || gui.Controls.Area[unrelated].Total.Global != unrelatedBefore.Global);
	cnst ::llc::u2_t eventsAfterRecalculation = gui.Controls.EventQueue.size();
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_METRIC_HIERARCHY, eventsAfterRecalculation != eventsBefore + 4, "Events before:%u after:%u expected:%u.", eventsBefore, eventsAfterRecalculation, eventsBefore + 4);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_METRIC_STABILITY, 0 > ::llc::guiUpdateMetrics(gui, {100, 80}, false), "%s", "Stable metrics failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_METRIC_STABILITY, gui.Controls.EventQueue.size() != eventsAfterRecalculation);
	rtrn 0;
}

sttc ::llc::err_t testGUIMetricConstraints(ATestError & errors) {
	::llc::SGUI gui = {};
	cnst ::llc::cid_t reference = ::llc::controlCreate(gui);
	cnst ::llc::cid_t constrained = ::llc::controlCreate(gui);
	cnst ::llc::cid_t attached = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CREATE, reference < 0 || constrained < 0 || attached < 0, "Reference:%i constrained:%i attached:%i.", reference, constrained, attached);
	gui.Controls.Placement[reference].Area = {{5, 5}, {37, 23}};
	gui.Controls.Placement[constrained].Area = {{0, 0}, {5, 80}};
	gui.Controls.Placement[attached].Area = {{9, 11}, {3, 4}};
	for(::llc::cid_t control : {reference, constrained, attached})
		gui.Controls.Placement[control].Margin = gui.Controls.Placement[control].Border = {};
	gui.Controls.Constraints[constrained].SizeMinMax = {{10, 12}, {30, 40}};
	gui.Controls.Constraints[attached].AttachSizeToControl = {reference, attached};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_METRIC_CONSTRAINT, 0 > ::llc::guiUpdateMetrics(gui, {100, 80}, true), "%s", "Constraint metrics failed.");
	cnst ::llc::n2s1_t constrainedExpected = {10, 40};
	cnst ::llc::n2s1_t attachedExpected = {37, 80};
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_METRIC_CONSTRAINT, gui.Controls.Area[constrained].Total.Global.Size != constrainedExpected);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_METRIC_CONSTRAINT, gui.Controls.Area[constrained].Client.Global.Size != constrainedExpected);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_METRIC_ATTACHMENT, gui.Controls.Area[attached].Total.Global.Size != attachedExpected);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_METRIC_ATTACHMENT, gui.Controls.Area[attached].Client.Global.Size != attachedExpected);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_METRIC_ATTACHMENT, gui.Controls.Area[attached].Total.Global.Offset != ::llc::n2s1_t{});
	gui.Controls.Placement[reference].Area.Size = {29, 17};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_METRIC_ATTACHMENT, 0 > ::llc::guiUpdateMetrics(gui, {90, 70}, true), "%s", "Attachment recalculation failed.");
	cnst ::llc::n2s1_t attachedChangedExpected = {29, 70};
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_METRIC_ATTACHMENT, gui.Controls.Area[attached].Total.Global.Size != attachedChangedExpected);
	rtrn 0;
}

sttc ::llc::err_t testGUIHitTesting(ATestError & errors) {
	::llc::SGUI gui = {};
	cnst ::llc::cid_t back = ::llc::controlCreate(gui);
	cnst ::llc::cid_t front = ::llc::controlCreate(gui);
	cnst ::llc::cid_t child = ::llc::controlCreateChild(gui, front);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CREATE, back < 0 || front < 0 || child < 0, "Back:%i front:%i child:%i.", back, front, child);
	gui.Controls.Placement[back ].Area = {{ 0,  0}, {40, 40}};
	gui.Controls.Placement[front].Area = {{10, 10}, {40, 40}};
	gui.Controls.Placement[child].Area = {{ 5,  5}, {10, 10}};
	for(::llc::cid_t control : {back, front, child})
		gui.Controls.Placement[control].Margin = gui.Controls.Placement[control].Border = {};
	gui.LastSize = {80, 60};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_METRICS, 0 > ::llc::guiUpdateMetrics(gui, gui.LastSize, true), "%s", "Hit-test metrics failed.");
	::llc::SInput input = {};
	input.MouseCurrent.Position = {5, 5};
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_HIT_BOUNDARY, ::llc::guiProcessInput(gui, input) != back, "Expected back control:%i.", back);
	input.MouseCurrent.Position = {10, 10};
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_HIT_ORDER, ::llc::guiProcessInput(gui, input) != front, "Expected front control:%i.", front);
	input.MouseCurrent.Position = {15, 15};
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_HIT_ORDER, ::llc::guiProcessInput(gui, input) != child, "Expected child control:%i.", child);
	input.MouseCurrent.Position = {50, 20};
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_HIT_BOUNDARY, ::llc::guiProcessInput(gui, input) != (::llc::err_t)gui.Controls.States.size(), "Right edge accepted at x:%i.", input.MouseCurrent.Position.x);
	input.MouseCurrent.Position = {20, 50};
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_HIT_BOUNDARY, ::llc::guiProcessInput(gui, input) != (::llc::err_t)gui.Controls.States.size(), "Bottom edge accepted at y:%i.", input.MouseCurrent.Position.y);
	input.MouseCurrent.Position = {15, 15};
	gui.Controls.SetHidden(child, true);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_HIT_ELIGIBILITY, ::llc::guiProcessInput(gui, input) != front, "Hidden child selected:%i.", child);
	gui.Controls.SetHidden(child, false);
	gui.Controls.SetDisabled(child, true);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_HIT_ELIGIBILITY, ::llc::guiProcessInput(gui, input) != front, "Disabled child selected:%i.", child);
	gui.Controls.SetDisabled(child, false);
	gui.Controls.SetDisabled(front, true);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_HIT_ELIGIBILITY, ::llc::guiProcessInput(gui, input) != back, "Disabled root hierarchy selected:%i.", front);
	gui.Controls.SetDisabled(front, false);
	gui.Controls.SetHidden(front, true);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_HIT_ELIGIBILITY, ::llc::guiProcessInput(gui, input) != back, "Hidden root hierarchy selected:%i.", front);
	rtrn 0;
}

sttc ::llc::err_t testGUIInputSequence(ATestError & errors) {
	::llc::SGUI gui = {};
	cnst ::llc::cid_t control = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CREATE, control < 0, "Control result:%i.", control);
	gui.Controls.Placement[control].Area = {{10, 10}, {30, 20}};
	gui.Controls.Placement[control].Margin = gui.Controls.Placement[control].Border = {};
	gui.LastSize = {80, 60};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_METRICS, 0 > ::llc::guiUpdateMetrics(gui, gui.LastSize, true), "%s", "Input metrics failed.");
	::llc::SInput input = {};
	input.MouseCurrent.Position = {20, 20};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_INPUT_SEQUENCE, ::llc::guiProcessInput(gui, input) != control, "%s", "Initial hover failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_INPUT_SEQUENCE, false == gui.Controls.IsHovered(control) || gui.Controls.IsPressed(control) || gui.Controls.IsAction(control));
	::llc::inputNextFrame(input);
	input.MouseCurrent.ButtonState[0] = 1;
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_INPUT_SEQUENCE, ::llc::guiProcessInput(gui, input) != control, "%s", "Press failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_INPUT_SEQUENCE, false == gui.Controls.IsHovered(control) || false == gui.Controls.IsPressed(control) || gui.Controls.IsAction(control));
	::llc::inputNextFrame(input);
	input.MouseCurrent.Position = {70, 50};
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_INPUT_SEQUENCE, ::llc::guiProcessInput(gui, input) != (::llc::err_t)gui.Controls.States.size());
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_INPUT_SEQUENCE, gui.Controls.IsHovered(control) || false == gui.Controls.IsPressed(control) || gui.Controls.IsAction(control));
	::llc::inputNextFrame(input);
	input.MouseCurrent.Position = {20, 20};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_INPUT_SEQUENCE, ::llc::guiProcessInput(gui, input) != control, "%s", "Held return failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_INPUT_SEQUENCE, false == gui.Controls.IsHovered(control) || false == gui.Controls.IsPressed(control) || gui.Controls.IsAction(control));
	::llc::inputNextFrame(input);
	input.MouseCurrent.ButtonState[0] = 0;
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_INPUT_SEQUENCE, ::llc::guiProcessInput(gui, input) != control, "%s", "Release failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_INPUT_SEQUENCE, false == gui.Controls.IsHovered(control) || gui.Controls.IsPressed(control) || false == gui.Controls.IsAction(control));
	::llc::inputNextFrame(input);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_INPUT_STABILITY, ::llc::guiProcessInput(gui, input) != control, "%s", "Action reset failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_INPUT_STABILITY, false == gui.Controls.IsHovered(control) || gui.Controls.IsPressed(control) || gui.Controls.IsAction(control));
	::llc::inputNextFrame(input);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_INPUT_STABILITY, ::llc::guiProcessInput(gui, input) != control, "%s", "Stable input failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_INPUT_STABILITY, gui.Controls.EventQueue.size());
	rtrn 0;
}

sttc ::llc::err_t testGUILayoutAndInput(ATestError & errors) {
	::llc::SGUI gui = {};
	cnst ::llc::cid_t parent = ::llc::controlCreate(gui);
	cnst ::llc::cid_t child = ::llc::controlCreateChild(gui, parent);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CREATE, parent < 0 || child < 0, "Parent:%i child:%i.", parent, child);
	gui.Controls.Placement[parent].Area = {{10, 10}, {60, 40}};
	gui.Controls.Placement[parent].Margin = gui.Controls.Placement[parent].Border = {};
	gui.Controls.Placement[child].Area = {{20, 0}, {20, 20}};
	gui.Controls.Placement[child].Margin = gui.Controls.Placement[child].Border = {};
	gui.LastSize = {100, 80};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_METRICS, 0 > ::llc::guiUpdateMetrics(gui, gui.LastSize, true), "%s", "Hierarchy metrics failed.");
	cnst ::llc::n2s1_t parentOffset = {10, 10};
	cnst ::llc::n2s1_t childOffset = {30, 10};
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_METRICS, gui.Controls.Area[parent].Total.Global != gui.Controls.Placement[parent].Area);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_METRICS, gui.Controls.Area[child].Total.Global.Offset != childOffset);

	::llc::SInput input = {};
	input.MouseCurrent.Position = {15, 15};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_HOVER, 0 > ::llc::guiProcessInput(gui, input), "%s", "Hover failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_HOVER, false == gui.Controls.IsHovered(parent) || gui.Controls.IsHovered(child));
	input.MousePrevious = input.MouseCurrent;
	input.MouseCurrent.ButtonState[0] = 1;
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_PRESS, 0 > ::llc::guiProcessInput(gui, input), "%s", "Press failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_PRESS, false == gui.Controls.IsPressed(parent) || gui.Controls.IsAction(parent));
	input.MousePrevious = input.MouseCurrent;
	input.MouseCurrent.Position = {35, 15};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CHILD_CROSSING, 0 > ::llc::guiProcessInput(gui, input), "%s", "Child crossing failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_CHILD_CROSSING, false == gui.Controls.IsPressed(parent) || false == gui.Controls.IsHovered(child));
	input.MousePrevious = input.MouseCurrent;
	input.MouseCurrent.Position = {95, 75};
	cnst ::llc::err_t noHover = ::llc::guiProcessInput(gui, input);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_NO_HOVER, noHover < 0, "No-hover result:%i.", noHover);
	LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_NO_HOVER, noHover != (::llc::err_t)gui.Controls.States.size(), "Result:%i expected:%u.", noHover, gui.Controls.States.size());
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_OUTSIDE_CAPTURE, false == gui.Controls.IsPressed(parent));
	input.MousePrevious = input.MouseCurrent;
	input.MouseCurrent.ButtonState[0] = 0;
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_OUTSIDE_RELEASE, 0 > ::llc::guiProcessInput(gui, input), "%s", "Outside release failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_OUTSIDE_RELEASE, gui.Controls.IsPressed(parent) || gui.Controls.IsAction(parent));

	gui.Controls.Placement[parent].Area.Offset = {50, 10};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_PENDING_METRICS, 0 > ::llc::controlMetricsInvalidate(gui, parent), "%s", "Pending invalidation failed.");
	input.MouseCurrent.Position = {15, 15};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_PENDING_METRICS, 0 > ::llc::guiProcessInput(gui, input), "%s", "Pending hit test failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_PENDING_METRICS, gui.Controls.Area[parent].Total.Global.Offset != parentOffset || gui.Controls.IsUpdated(parent));
	rtrn 0;
}

sttc ::llc::err_t testGUIDrawCore(ATestError & errors) {
	::llc::SGUI gui = {};
	cnst ::llc::cid_t parent = ::llc::controlCreate(gui);
	cnst ::llc::cid_t child = ::llc::controlCreateChild(gui, parent);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CREATE, parent < 0 || child < 0, "Parent:%i child:%i.", parent, child);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_DRAW_PALETTE, 0 == gui.Colors.create() || 0 > gui.Colors->Palettes.resize(::llc::GUI_CONTROL_PALETTE_COUNT), "%s", "Palette allocation failed.");
	for(::llc::u0_t iState = 0; iState < ::llc::GUI_CONTROL_PALETTE_COUNT; ++iState) {
		::llc::astatic<::llc::bgra, ::llc::UI_CONTROL_AREA_COUNT> & palette = gui.Colors->Palettes[iState];
		for(::llc::u2_t iArea = 0; iArea < palette.size(); ++iArea)
			palette[iArea] = {(::llc::u0_t)(10 + iState), (::llc::u0_t)(20 + iArea), (::llc::u0_t)(30 + iState + iArea), 0xFF};
	}
	for(::llc::cid_t control : {parent, child}) {
		gui.Controls.Draw[control].UseNewPalettes = true;
		gui.Controls.Draw[control].ColorMode = ::llc::GUI_COLOR_MODE_Flat;
		gui.Controls.Text[control].Text = {};
		for(::llc::u0_t iState = 0; iState < ::llc::GUI_CONTROL_PALETTE_COUNT; ++iState)
			gui.Controls.Draw[control].Palettes[iState] = iState;
	}
	gui.Controls.Placement[parent].Area = {{2, 2}, {12, 10}};
	gui.Controls.Placement[parent].Margin = {};
	gui.Controls.Placement[parent].Border = {1, 1, 1, 1};
	gui.Controls.Placement[child].Area = {{3, 3}, {4, 3}};
	gui.Controls.Placement[child].Margin = gui.Controls.Placement[child].Border = {};
	::llc::img8bgra target = {};
	cnst ::llc::n2u2_t targetSize = {18, 14};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_DRAW, 0 > target.resize(targetSize), "%s", "Target resize failed.");
	stxp ::llc::GUI_CONTROL_FLAG states[] =
		{ {}, ::llc::GUI_CONTROL_FLAG_Hovered, ::llc::GUI_CONTROL_FLAG_Selected, ::llc::GUI_CONTROL_FLAG_Pressed, ::llc::GUI_CONTROL_FLAG_Disabled };
	stxp ::llc::GUI_CONTROL_PALETTE palettes[] =
		{ ::llc::GUI_CONTROL_PALETTE_NORMAL, ::llc::GUI_CONTROL_PALETTE_HOVER, ::llc::GUI_CONTROL_PALETTE_SELECTED, ::llc::GUI_CONTROL_PALETTE_PRESSED, ::llc::GUI_CONTROL_PALETTE_DISABLED };
	gui.Controls.SetHidden(child, true);
	for(::llc::u2_t iState = 0; iState < ::llc::size(states); ++iState) {
		target.View.fill({});
		gui.Controls.States[parent].Flag = states[iState];
		LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_DRAW_PALETTE, 0 > ::llc::guiDraw(gui, target.View), "State:%u.", iState);
		cnst ::llc::bgra expected = gui.Colors->Palettes[palettes[iState]][::llc::UI_CONTROL_AREA_CLIENT];
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_DRAW_PALETTE, target.View[6][7] != expected, "State:%u.", iState);
	}
	gui.Controls.States[parent].Flag = {};
	cnst ::llc::bgra untouched = {1, 2, 3, 4};
	target.View.fill(untouched);
	gui.Controls.Draw[parent].NoClient = true;
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_DRAW_CLIENT, 0 > ::llc::guiDraw(gui, target.View), "%s", "Client suppression failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_DRAW_CLIENT, target.View[6][7] != untouched);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_DRAW_BORDER, target.View[2][7] == untouched);
	target.View.fill(untouched);
	gui.Controls.Draw[parent].NoClient = false;
	gui.Controls.Draw[parent].NoBorder = true;
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_DRAW_BORDER, 0 > ::llc::guiDraw(gui, target.View), "%s", "Border suppression failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_DRAW_BORDER, target.View[2][7] != untouched);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_DRAW_CLIENT, target.View[6][7] == untouched);
	gui.Controls.Draw[parent].NoBorder = false;
	gui.Controls.SetHidden(child, false);
	gui.Controls.Draw[child].Palettes[::llc::GUI_CONTROL_PALETTE_NORMAL] = ::llc::GUI_CONTROL_PALETTE_HOVER;
	target.View.fill({});
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_DRAW_ORDER, 0 > ::llc::guiDraw(gui, target.View), "%s", "Hierarchy drawing failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_DRAW_ORDER, target.View[6][6] != gui.Colors->Palettes[::llc::GUI_CONTROL_PALETTE_HOVER][::llc::UI_CONTROL_AREA_CLIENT]);
	gui.Controls.SetHidden(child, true);
	gui.Controls.Placement[parent].Area = {{-4, -3}, {8, 7}};
	gui.Controls.Placement[parent].Margin = gui.Controls.Placement[parent].Border = {};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_DRAW_CLIP, 0 > ::llc::controlMetricsInvalidate(gui, parent), "%s", "Clip invalidation failed.");
	target.View.fill(untouched);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_DRAW_CLIP, 0 > ::llc::guiDraw(gui, target.View), "%s", "Clipped drawing failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_DRAW_CLIP, target.View[0][0] == untouched || target.View[3][3] == untouched || target.View[3][4] != untouched || target.View[4][3] != untouched);
	rtrn 0;
}

sttc ::llc::err_t testGUIDrawText(ATestError & errors) {
	::llc::SGUI gui = {};
	cnst ::llc::cid_t control = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CREATE, control < 0, "Control result:%i.", control);
	gui.Fonts.clear();
	::llc::po<::llc::SRasterFont> & fontDefault = gui.Fonts[gui.Fonts.push_back({})];
	::llc::po<::llc::SRasterFont> & fontEmpty = gui.Fonts[gui.Fonts.push_back({})];
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_TEXT_FONT, 0 > ::testGUITextFontCreate(fontDefault, {2, 2}) || 0 > ::testGUITextFontCreate(fontEmpty, {2, 2}), "%s", "GUI font creation failed.");
	cnst ::llc::u0_t character = 'A';
	cnst ::llc::n2u2_t cell = {(character % 16U) * 2U, (character / 16U) * 2U};
	fontDefault->Texture.View[(cell.y + 0U) * fontDefault->Texture.Pitch + cell.x + 0U] = true;
	fontDefault->Texture.View[(cell.y + 1U) * fontDefault->Texture.Pitch + cell.x + 1U] = true;
	gui.SelectedFont = 0;
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_TEXT_PALETTE, 0 == gui.Colors.create() || 0 > gui.Colors->Palettes.resize(::llc::GUI_CONTROL_PALETTE_COUNT), "%s", "Palette allocation failed.");
	cnst ::llc::bgra colors[] = {{11, 12, 13, 0xFF}, {21, 22, 23, 0xFF}, {31, 32, 33, 0xFF}, {41, 42, 43, 0xFF}, {51, 52, 53, 0xFF}, {61, 62, 63, 0xFF}, {71, 72, 73, 0xFF}, {81, 82, 83, 0xFF}, {91, 92, 93, 0xFF}, {101, 102, 103, 0xFF}};
	static_assert(::llc::size(colors) == ::llc::GUI_CONTROL_PALETTE_COUNT);
	for(::llc::u0_t iState = 0; iState < ::llc::GUI_CONTROL_PALETTE_COUNT; ++iState) {
		gui.Controls.Draw[control].Palettes[iState] = iState;
		gui.Colors->Palettes[iState][::llc::UI_CONTROL_AREA_TEXT_FACE] = colors[iState];
	}
	gui.Controls.Draw[control].UseNewPalettes = true;
	gui.Controls.Draw[control].NoClient = gui.Controls.Draw[control].NoBorder = true;
	gui.Controls.Placement[control].Area = {{2, 2}, {10, 8}};
	gui.Controls.Placement[control].Margin = gui.Controls.Placement[control].Border = {};
	gui.Controls.Text[control].Text = LLC_CXS("A");
	::llc::img8bgra target = {};
	cnst ::llc::n2u2_t targetSize = {16, 12};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_TEXT_PALETTE, 0 > target.resize(targetSize), "%s", "Target allocation failed.");
	stxp ::llc::ALIGN alignments[] =
		{ ::llc::ALIGN_TOP_LEFT, ::llc::ALIGN_TOP_RIGHT, ::llc::ALIGN_BOTTOM_LEFT, ::llc::ALIGN_BOTTOM_RIGHT
		, ::llc::ALIGN_CENTER_TOP, ::llc::ALIGN_CENTER_BOTTOM, ::llc::ALIGN_CENTER_LEFT, ::llc::ALIGN_CENTER_RIGHT, ::llc::ALIGN_CENTER
		};
	for(::llc::u2_t iAlign = 0; iAlign < ::llc::size(alignments); ++iAlign) {
		gui.Controls.Text[control].Align = alignments[iAlign];
		LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_TEXT_ALIGN, 0 > ::llc::controlMetricsInvalidate(gui, control) || 0 > ::llc::guiUpdateMetrics(gui, target.metrics().u1(), true), "Alignment:%s.", ::llc::get_value_namep(alignments[iAlign]));
		::llc::rect2s1_t expected = {{}, {2, 2}};
		::llc::realignRectangle(gui.Controls.Area[control].Client.Global.Size.u2(), expected, expected, alignments[iAlign]);
		expected.Offset += gui.Controls.Area[control].Client.Global.Offset;
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_TEXT_ALIGN, gui.Controls.Area[control].Text != expected, "Alignment:%s.", ::llc::get_value_namep(alignments[iAlign]));
	}
	gui.Controls.Text[control].Align = ::llc::ALIGN_CENTER;
	stxp double dpiScales[] = {0.5, 1.0, 1.5};
	for(::llc::u2_t iScale = 0; iScale < ::llc::size(dpiScales); ++iScale) {
		gui.Zoom.DPI = {dpiScales[iScale], dpiScales[iScale]};
		LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_TEXT_ALIGN, 0 > ::llc::controlMetricsInvalidate(gui, control) || 0 > ::llc::guiUpdateMetrics(gui, target.metrics().u1(), true), "DPI:%f.", dpiScales[iScale]);
		::llc::rect2s1_t expected = {{}, fontDefault->CharSize.s1()};
		::llc::realignRectangle(gui.Controls.Area[control].Client.Global.Size.u2(), expected, expected, ::llc::ALIGN_CENTER);
		expected.Offset += gui.Controls.Area[control].Client.Global.Offset;
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_TEXT_ALIGN, gui.Controls.Area[control].Text != expected, "DPI:%f.", dpiScales[iScale]);
	}
	gui.Zoom.DPI = {1, 1};
	gui.Controls.Text[control].Align = ::llc::ALIGN_TOP_LEFT;
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_TEXT_ALIGN, 0 > ::llc::controlMetricsInvalidate(gui, control) || 0 > ::llc::guiUpdateMetrics(gui, target.metrics().u1(), true), "%s", "Final text metrics failed.");
	stxp ::llc::GUI_CONTROL_FLAG states[] = {{}, ::llc::GUI_CONTROL_FLAG_Hovered, ::llc::GUI_CONTROL_FLAG_Pressed, ::llc::GUI_CONTROL_FLAG_Selected, ::llc::GUI_CONTROL_FLAG_Disabled};
	stxp ::llc::GUI_CONTROL_PALETTE palettes[] = {::llc::GUI_CONTROL_PALETTE_NORMAL, ::llc::GUI_CONTROL_PALETTE_HOVER, ::llc::GUI_CONTROL_PALETTE_PRESSED, ::llc::GUI_CONTROL_PALETTE_SELECTED, ::llc::GUI_CONTROL_PALETTE_DISABLED};
	for(::llc::u2_t iState = 0; iState < ::llc::size(states); ++iState) {
		target.View.fill({});
		gui.Controls.States[control].Flag = states[iState];
		LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_TEXT_PALETTE, 0 > ::llc::guiDraw(gui, target.View), "State:%u.", iState);
		cnst ::llc::n2u1_t offset = (states[iState] & (::llc::GUI_CONTROL_FLAG_Hovered | ::llc::GUI_CONTROL_FLAG_Pressed)) ? ::llc::n2u1_t{1, 1} : ::llc::n2u1_t{};
		cnst ::llc::n2u1_t pixel = gui.Controls.Area[control].Text.Offset.u1() + offset;
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_TEXT_PALETTE, target.View[pixel.y][pixel.x] != colors[palettes[iState]], "State:%u pixel:{%u,%u}.", iState, pixel.x, pixel.y);
		if(offset.x)
			LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_TEXT_STATE, target.View[pixel.y - 1U][pixel.x - 1U] != ::llc::bgra{}, "State:%u retained unshifted text.", iState);
	}
	target.View.fill({});
	gui.Controls.States[control].Flag = {};
	gui.Controls.Text[control].FontSelected = 1;
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_TEXT_FONT, 0 > ::llc::guiDraw(gui, target.View), "%s", "Explicit font draw failed.");
	cnst ::llc::n2u1_t textOffset = gui.Controls.Area[control].Text.Offset.u1();
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_TEXT_FONT, target.View[textOffset.y][textOffset.x] != ::llc::bgra{});
	gui.Controls.Text[control].FontSelected = -1;
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_TEXT_FONT, 0 > ::llc::guiDraw(gui, target.View), "%s", "Default font draw failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_TEXT_FONT, target.View[textOffset.y][textOffset.x] != colors[::llc::GUI_CONTROL_PALETTE_NORMAL]);
	rtrn 0;
}

sttc ::llc::err_t testGUIDrawImage(ATestError & errors) {
	::llc::SGUI gui = {};
	cnst ::llc::cid_t control = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CREATE, control < 0, "Control result:%i.", control);
	gui.Controls.Draw[control].NoClient = gui.Controls.Draw[control].NoBorder = true;
	gui.Controls.Text[control].Text = {};
	gui.Controls.Placement[control].Area = {{2, 2}, {10, 8}};
	gui.Controls.Placement[control].Margin = gui.Controls.Placement[control].Border = {};
	::llc::img8bgra source = {};
	cnst ::llc::n2u2_t sourceSize = {2, 2};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_IMAGE_ALIGN, 0 > source.resize(sourceSize), "%s", "Source allocation failed.");
	cnst ::llc::bgra sourceColors[] = {{11, 12, 13, 0xFF}, {21, 22, 23, 0xFF}, {31, 32, 33, 0xFF}, {41, 42, 43, 0xFF}};
	cnst ::llc::bgra drawnColors [] = {{11, 12, 13, 0x00}, {21, 22, 23, 0x00}, {31, 32, 33, 0x00}, {41, 42, 43, 0x00}};
	for(::llc::u2_t iColor = 0; iColor < ::llc::size(sourceColors); ++iColor)
		source.Texels[iColor] = sourceColors[iColor];
	gui.Controls.Images[control].Image = source.View;
	::llc::img8bgra target = {};
	cnst ::llc::n2u2_t targetSize = {16, 12};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_IMAGE_ALIGN, 0 > target.resize(targetSize), "%s", "Target allocation failed.");
	stxp ::llc::ALIGN alignments[] =
		{ ::llc::ALIGN_TOP_LEFT, ::llc::ALIGN_TOP_RIGHT, ::llc::ALIGN_BOTTOM_LEFT, ::llc::ALIGN_BOTTOM_RIGHT
		, ::llc::ALIGN_CENTER_TOP, ::llc::ALIGN_CENTER_BOTTOM, ::llc::ALIGN_CENTER_LEFT, ::llc::ALIGN_CENTER_RIGHT, ::llc::ALIGN_CENTER
		};
	for(::llc::u2_t iAlign = 0; iAlign < ::llc::size(alignments); ++iAlign) {
		target.View.fill({});
		gui.Controls.Images[control].ImageAlign = alignments[iAlign];
		LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_IMAGE_ALIGN, 0 > ::llc::guiDraw(gui, target.View), "Alignment:%s.", ::llc::get_value_namep(alignments[iAlign]));
		::llc::rect2s1_t expected = {{}, sourceSize.s1()};
		::llc::realignRectangle(gui.Controls.Area[control].Client.Global.Size, expected, expected, alignments[iAlign]);
		expected.Offset += gui.Controls.Area[control].Client.Global.Offset;
		cnst ::llc::bgra actualTopLeft = target.View[expected.Offset.y][expected.Offset.x];
		cnst ::llc::bgra actualBottomRight = target.View[expected.Limit().y - 1][expected.Limit().x - 1];
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_IMAGE_ALIGN, actualTopLeft != drawnColors[0], "Alignment:%s top-left:{%u,%u,%u} at {%i,%i}.", ::llc::get_value_namep(alignments[iAlign]), actualTopLeft.b, actualTopLeft.g, actualTopLeft.r, expected.Offset.x, expected.Offset.y);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_IMAGE_ALIGN, actualBottomRight != drawnColors[3], "Alignment:%s bottom-right:{%u,%u,%u} at {%i,%i}.", ::llc::get_value_namep(alignments[iAlign]), actualBottomRight.b, actualBottomRight.g, actualBottomRight.r, expected.Limit().x - 1, expected.Limit().y - 1);
	}
	gui.Controls.Images[control].ImageAlign = ::llc::ALIGN_TOP_LEFT;
	gui.Controls.Images[control].ImageOffset = {1, 0};
	target.View.fill({});
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_IMAGE_OFFSET, 0 > ::llc::guiDraw(gui, target.View), "%s", "Offset draw failed.");
	cnst ::llc::n2s1_t imageOrigin = gui.Controls.Area[control].Client.Global.Offset;
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_IMAGE_OFFSET, target.View[imageOrigin.y][imageOrigin.x] != drawnColors[1]);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_IMAGE_OFFSET, target.View[imageOrigin.y][imageOrigin.x + 1] != ::llc::bgra{});
	gui.Controls.Images[control].ImageOffset = {};
	gui.Controls.States[control].Flag = ::llc::GUI_CONTROL_FLAG_Hovered;
	target.View.fill({});
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_IMAGE_STATE, 0 > ::llc::guiDraw(gui, target.View), "%s", "Hovered draw failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_IMAGE_STATE, target.View[imageOrigin.y][imageOrigin.x] != ::llc::bgra{} || target.View[imageOrigin.y + 1][imageOrigin.x + 1] != drawnColors[0]);
	gui.Controls.States[control].Flag = {};
	gui.Controls.Images[control].ImageInvertY = true;
	target.View.fill({});
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_IMAGE_INVERT, 0 > ::llc::guiDraw(gui, target.View), "%s", "Inverted draw failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_IMAGE_INVERT, target.View[imageOrigin.y][imageOrigin.x] != drawnColors[2] || target.View[imageOrigin.y + 1][imageOrigin.x + 1] != drawnColors[1]);
	gui.Controls.Images[control].ImageInvertY = false;
	gui.Controls.Placement[control].Area = {{-1, -1}, {2, 2}};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_IMAGE_CLIP, 0 > ::llc::controlMetricsInvalidate(gui, control), "%s", "Clip invalidation failed.");
	target.View.fill({});
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_IMAGE_CLIP, 0 > ::llc::guiDraw(gui, target.View), "%s", "Clipped draw failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_IMAGE_CLIP, target.View[0][0] != drawnColors[3] || target.View[0][1] != ::llc::bgra{} || target.View[1][0] != ::llc::bgra{});
	rtrn 0;
}

sttc ::llc::err_t testGUIVirtualKeyboard(ATestError & errors) {
	::llc::SGUI gui = {};
	::llc::SVirtualKeyboard keyboard = {};
	cnst ::llc::u1_t keys[] = {'A', 'B', 'C', 'D', 'E'};
	cnst ::llc::n2u1_t keySize = {10, 12};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_KEYBOARD_SETUP, 0 > ::llc::virtualKeyboardSetup(gui, keyboard, 2, keySize, keys), "%s", "Keyboard setup failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_KEYBOARD_SETUP, keyboard.IdRoot < 0 || keyboard.Characters.size() != ::llc::size(keys) || keyboard.Commands.size() != 8);
	cnst ::llc::SControlPlacement & rootPlacement = gui.Controls.Placement[keyboard.IdRoot];
	cnst ::llc::n2s1_t rootSizeExpected = {112, 164};
	cnst ::llc::rectu1_t borderExpected = {1, 1, 1, 1};
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_KEYBOARD_GEOMETRY, rootPlacement.Area.Size != rootSizeExpected || rootPlacement.Align != ::llc::ALIGN_CENTER_BOTTOM);
	for(::llc::u2_t iKey = 0; iKey < keyboard.Characters.size(); ++iKey) {
		cnst ::llc::cid_t idControl = keyboard.Characters[iKey].IdControl;
		cnst ::llc::SControlPlacement & placement = gui.Controls.Placement[idControl];
		cnst ::llc::n2s1_t expectedOffset = {(::llc::s1_t)((iKey % 2U) * keySize.x), (::llc::s1_t)((iKey / 2U) * keySize.y)};
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_KEYBOARD_GEOMETRY, gui.Controls.States[idControl].Parent != keyboard.IdRoot || placement.Area.Offset != expectedOffset || placement.Area.Size != keySize.s1() || placement.Border != borderExpected, "Character:%u control:%i.", iKey, idControl);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_KEYBOARD_LABEL, gui.Controls.Text[idControl].Text.size() != 1 || gui.Controls.Text[idControl].Text[0] != (::llc::sc_t)keys[iKey] || gui.Controls.Text[idControl].Align != ::llc::ALIGN_CENTER, "Character:%u control:%i.", iKey, idControl);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_KEYBOARD_EVENT, keyboard.Characters[iKey].Character != keys[iKey], "Character:%u value:%u.", iKey, keyboard.Characters[iKey].Character);
	}
	stxp ::llc::SYSTEM_KEY commands[] =
		{ ::llc::SYSTEM_KEY_Escape, ::llc::SYSTEM_KEY_Enter, ::llc::SYSTEM_KEY_Backspace, ::llc::SYSTEM_KEY_Clear
		, ::llc::SYSTEM_KEY_Left, ::llc::SYSTEM_KEY_Right, ::llc::SYSTEM_KEY_Up, ::llc::SYSTEM_KEY_Down
		};
	::llc::TQueueSystemEvent events = {};
	for(::llc::u2_t iCommand = 0; iCommand < keyboard.Commands.size(); ++iCommand) {
		cnst ::llc::cid_t idControl = keyboard.Commands[iCommand].IdControl;
		cnst ::llc::SControlPlacement & placement = gui.Controls.Placement[idControl];
		cnst ::llc::n2s1_t commandSizeExpected = {88, 20};
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_KEYBOARD_GEOMETRY, gui.Controls.States[idControl].Parent != keyboard.IdRoot || placement.Area.Offset.y != (::llc::s1_t)(iCommand * 20U) || placement.Area.Size != commandSizeExpected || placement.Align != ::llc::ALIGN_TOP_RIGHT || placement.Border != borderExpected, "Command:%u control:%i.", iCommand, idControl);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_KEYBOARD_LABEL, gui.Controls.Text[idControl].Text != ::llc::get_value_label(commands[iCommand]) || gui.Controls.Text[idControl].Align != ::llc::ALIGN_CENTER, "Command:%u control:%i.", iCommand, idControl);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_KEYBOARD_COMMAND, keyboard.Commands[iCommand].Key != commands[iCommand], "Command:%u key:%u.", iCommand, (::llc::u2_t)keyboard.Commands[iCommand].Key);
		cnst ::llc::u2_t eventIndex = events.size();
		LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_KEYBOARD_COMMAND, 1 != ::llc::virtualKeyboardHandleEvent(keyboard, idControl, events), "Command:%u.", iCommand);
		::llc::u0_t key = {};
		cnst ::llc::err_t extractResult = ::llc::eventExtractAndHandle<::llc::EVENT_KEYBOARD>(*events[eventIndex], [&key](cnst ::llc::SEView<::llc::EVENT_KEYBOARD> & eventView) {
			if(eventView.Type != ::llc::EVENT_KEYBOARD_Down) rtrn -1;
			::llc::vcu0_t payload = eventView.Data;
			rtrn ::llc::loadPOD(payload, key);
		});
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_KEYBOARD_COMMAND, ::llc::failed(extractResult) || key != (::llc::u0_t)commands[iCommand], "Command:%u payload:%u.", iCommand, key);
	}
	cnst ::llc::u2_t eventCount = events.size();
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_KEYBOARD_EVENT, 0 != ::llc::virtualKeyboardHandleEvent(keyboard, keyboard.IdRoot, events) || events.size() != eventCount);

	gui.Controls.Placement[keyboard.IdRoot].Align = ::llc::ALIGN_TOP_LEFT;
	gui.Controls.Placement[keyboard.IdRoot].Area.Offset = {5, 7};
	cnst ::llc::n2u1_t targetSize = {300, 220};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_KEYBOARD_MOVE, 0 > ::llc::controlMetricsInvalidate(gui, keyboard.IdRoot) || 0 > ::llc::guiUpdateMetrics(gui, targetSize, true), "%s", "Initial keyboard metrics failed.");
	cnst ::llc::n2s1_t childOrigin = gui.Controls.Area[keyboard.Characters[0].IdControl].Total.Global.Offset;
	cnst ::llc::n2s1_t movement = {11, 13};
	gui.Controls.Placement[keyboard.IdRoot].Area.Offset += movement;
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_KEYBOARD_MOVE, 0 > ::llc::controlMetricsInvalidate(gui, keyboard.IdRoot) || 0 > ::llc::guiUpdateMetrics(gui, targetSize, true), "%s", "Moved keyboard metrics failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_KEYBOARD_MOVE, gui.Controls.Area[keyboard.Characters[0].IdControl].Total.Global.Offset != childOrigin + movement);

	cnst ::llc::cid_t idKey = keyboard.Characters[0].IdControl;
	cnst ::llc::rect2s1_t keyArea = gui.Controls.Area[idKey].Total.Global;
	::llc::SInput input = {};
	input.MouseCurrent.Position = keyArea.Offset + ::llc::n2s1_t{1, 1};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_KEYBOARD_INPUT, ::llc::guiProcessInput(gui, input) != idKey, "%s", "Key hover failed.");
	::llc::inputNextFrame(input);
	input.MouseCurrent.ButtonState[0] = 1;
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_KEYBOARD_INPUT, ::llc::guiProcessInput(gui, input) != idKey, "%s", "Key press failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_KEYBOARD_INPUT, false == gui.Controls.IsPressed(idKey) || gui.Controls.IsAction(idKey));
	::llc::inputNextFrame(input);
	input.MouseCurrent.ButtonState[0] = 0;
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_KEYBOARD_INPUT, ::llc::guiProcessInput(gui, input) != idKey, "%s", "Key release failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_KEYBOARD_INPUT, gui.Controls.IsPressed(idKey) || false == gui.Controls.IsAction(idKey));
	::llc::acid processable = {idKey};
	::llc::TQueueSystemEvent inputEvents = {};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_KEYBOARD_INPUT, 1 != ::llc::guiProcessControls(gui, processable, [&keyboard, &inputEvents](::llc::cid_t iControl) { rtrn ::llc::virtualKeyboardHandleEvent(keyboard, iControl, inputEvents); }), "%s", "Key action dispatch failed.");
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_KEYBOARD_EVENT, inputEvents.size() != 1, "Event count:%u.", inputEvents.size());
	::llc::u2_t character = {};
	cnst ::llc::err_t extractResult = ::llc::eventExtractAndHandle<::llc::EVENT_TEXT>(*inputEvents[0], [&character](cnst ::llc::SEView<::llc::EVENT_TEXT> & eventView) {
		if(eventView.Type != ::llc::EVENT_TEXT_Char) rtrn -1;
		::llc::vcu0_t payload = eventView.Data;
		rtrn ::llc::loadPOD(payload, character);
	});
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_KEYBOARD_EVENT, ::llc::failed(extractResult) || character != keys[0]);
	rtrn 0;
}

sttc ::llc::err_t testGUIDraw(ATestError & errors) {
	::llc::SGUI gui = {};
	cnst ::llc::cid_t control = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_CREATE, control < 0, "Control result:%i.", control);
	gui.Controls.Placement[control].Area = {{4, 4}, {20, 16}};
	::llc::img8bgra target = {};
	cnst ::llc::n2u2_t targetSize = {32, 24};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_DRAW, 0 > target.resize(targetSize), "%s", "Target resize failed.");
	for(::llc::u2_t y = 0; y < target.View.metrics().y; ++y)
		for(::llc::u2_t x = 0; x < target.View.metrics().x; ++x)
			target.View[y][x] = {};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_DRAW, 0 > ::llc::guiDraw(gui, target.View), "%s", "GUI draw failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_DRAW, target.View[8][8] == ::llc::bgra{});
	for(::llc::u2_t y = 0; y < target.View.metrics().y; ++y)
		for(::llc::u2_t x = 0; x < target.View.metrics().x; ++x)
			target.View[y][x] = {};
	gui.Controls.SetHidden(control, true);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_HIDDEN_DRAW, 0 > ::llc::guiDraw(gui, target.View), "%s", "Hidden draw failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_HIDDEN_DRAW, target.View[8][8] != ::llc::bgra{});
	rtrn 0;
}

sttc ::llc::err_t testGUIComponents(ATestError & errors) {
	::llc::SGUI			gui			= {};
	::llc::SInputBox	inputBox	= {};
	LLC_TEST_REQUIREF	(errors, GUI_TEST_RESULT_INPUTBOX, 0 > ::llc::inputBoxCreate(inputBox, gui), "%s", "Input box creation failed.");
	LLC_TEST_CHECK		(errors, GUI_TEST_RESULT_INPUTBOX, gui.Controls.States[inputBox.IdText].Parent != inputBox.IdRoot || gui.Controls.States[inputBox.VirtualKeyboard.IdRoot].Parent != inputBox.IdRoot);
	LLC_TEST_CHECK		(errors, GUI_TEST_RESULT_INPUTBOX, false == gui.Controls.IsHidden(inputBox.IdRoot));
	LLC_TEST_REQUIREF	(errors, GUI_TEST_RESULT_INPUTBOX, 0 > inputBox.SetText(gui, LLC_CXS("TEST")), "%s", "Input text failed.");
	LLC_TEST_CHECK		(errors, GUI_TEST_RESULT_INPUTBOX, inputBox.Text != LLC_CXS("TEST"));
	LLC_TEST_CHECK		(errors, GUI_TEST_RESULT_INPUTBOX, 1 != inputBox.Edit(gui, true) || gui.Controls.IsHidden(inputBox.IdRoot));

	::llc::SViewport	viewport = {};
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_VIEWPORT, 0 > ::llc::viewportInitialize(gui, viewport), "%s", "Viewport initialization failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_VIEWPORT, viewport.IdControl < 0 || viewport.IdControls.size() != ::llc::VIEWPORT_CONTROL_COUNT);
	for(::llc::u2_t iControl = 0; iControl < viewport.IdControls.size(); ++iControl)
		LLC_TEST_CHECK(errors, GUI_TEST_RESULT_VIEWPORT, gui.Controls.States[viewport.IdControls[iControl]].Parent != viewport.IdControl);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_VIEWPORT, gui.Controls.Placement[viewport.IdControls[::llc::VIEWPORT_CONTROL_RESIZE_BOTTOM_RIGHT]].Align != ::llc::ALIGN_BOTTOM_RIGHT);

	rtrn 0;
}

sttc ::llc::err_t testGUIViewportOperations(ATestError & errors) {
	::llc::SGUI						gui					= {};
	cnst ::llc::cid_t				idParent				= ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_VIEWPORT_ACTIVATE, idParent < 0, "Parent result:%i.", idParent);
	::llc::astatic<::llc::SViewport, 2>	viewports				= {};
	for(::llc::u0_t iViewport = 0; iViewport < viewports.size(); ++iViewport) {
		LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_VIEWPORT, 0 > ::llc::viewportInitialize(gui, viewports[iViewport]), "Viewport:%u initialization failed.", iViewport);
		LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_VIEWPORT_ACTIVATE, 0 > ::llc::controlSetParent(gui, viewports[iViewport].IdControl, idParent), "Viewport:%u parent assignment failed.", iViewport);
	}

	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_VIEWPORT_PRESS, ::llc::viewportIsPressed(gui, viewports[0]));
	cnst ::llc::cid_t pressedControls[] =
		{ viewports[0].IdControl
		, viewports[0].IdControls[::llc::VIEWPORT_CONTROL_TITLE]
		, viewports[0].IdControls[::llc::VIEWPORT_CONTROL_TARGET]
		};
	for(::llc::u0_t iControl = 0; iControl < ::llc::size(pressedControls); ++iControl) {
		gui.Controls.SetPressed(pressedControls[iControl], true);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_VIEWPORT_PRESS, false == ::llc::viewportIsPressed(gui, viewports[0]), "Control:%u id:%i.", iControl, pressedControls[iControl]);
		gui.Controls.SetPressed(pressedControls[iControl], false);
		LLC_TEST_CHECKF(errors, GUI_TEST_RESULT_VIEWPORT_PRESS, ::llc::viewportIsPressed(gui, viewports[0]), "Released control:%u id:%i.", iControl, pressedControls[iControl]);
	}

	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_VIEWPORT_ACTIVATE, 0 > ::llc::viewportActivate(gui, idParent, viewports, 0), "%s", "Viewport activation failed.");
	cnst ::llc::acid & parentChildren = gui.Controls.Children[idParent];
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_VIEWPORT_ACTIVATE, parentChildren.size() != viewports.size() || parentChildren[parentChildren.size() - 1] != viewports[0].IdControl);
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_VIEWPORT_ACTIVATE, false == gui.Controls.IsSelected(viewports[0].IdControl) || false == gui.Controls.IsSelected(viewports[0].IdControls[::llc::VIEWPORT_CONTROL_TITLE]));
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_VIEWPORT_ACTIVATE, gui.Controls.IsSelected(viewports[1].IdControl) || gui.Controls.IsSelected(viewports[1].IdControls[::llc::VIEWPORT_CONTROL_TITLE]));
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_VIEWPORT_ACTIVATE, 0 <= ::llc::viewportActivate(gui, idParent, viewports, (::llc::u0_t)viewports.size()));

	cnst ::llc::n2s1_t unfoldedExpected = {320, 200};
	cnst ::llc::n2s1_t foldedExpected = {320, 30};
	gui.Controls.Placement[viewports[0].IdControl].Area.Size = unfoldedExpected;
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_VIEWPORT_FOLD, 0 > ::llc::viewportFold(gui, viewports[0]), "%s", "Viewport fold failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_VIEWPORT_FOLD, false == viewports[0].Folded || viewports[0].UnfoldedSize != unfoldedExpected || gui.Controls.Placement[viewports[0].IdControl].Area.Size != foldedExpected || false == gui.Controls.IsHidden(viewports[0].IdControls[::llc::VIEWPORT_CONTROL_TARGET]));
	LLC_TEST_REQUIREF(errors, GUI_TEST_RESULT_VIEWPORT_FOLD, 0 > ::llc::viewportFold(gui, viewports[0]), "%s", "Viewport restore failed.");
	LLC_TEST_CHECK(errors, GUI_TEST_RESULT_VIEWPORT_FOLD, viewports[0].Folded || gui.Controls.Placement[viewports[0].IdControl].Area.Size != unfoldedExpected || gui.Controls.IsHidden(viewports[0].IdControls[::llc::VIEWPORT_CONTROL_TARGET]));
	rtrn 0;
}

::llc::err_t testGUI(ATestError & errors) {
	if_fail_fe(::testGUITextCore(errors));
	if_fail_fe(::testGUIControlStateQueries(errors));
	if_fail_fe(::testGUIControlStateMutations(errors));
	if_fail_fe(::testGUIControlStateEvents(errors));
	if_fail_fe(::testGUIControlLifecycle(errors));
	if_fail_fe(::testGUIMetricAlignment(errors));
	if_fail_fe(::testGUIMetricAreas(errors));
	if_fail_fe(::testGUIMetricHierarchy(errors));
	if_fail_fe(::testGUIMetricConstraints(errors));
	if_fail_fe(::testGUIHitTesting(errors));
	if_fail_fe(::testGUIInputSequence(errors));
	if_fail_fe(::testGUILayoutAndInput(errors));
	if_fail_fe(::testGUIDrawCore(errors));
	if_fail_fe(::testGUIDrawText(errors));
	if_fail_fe(::testGUIDrawImage(errors));
	if_fail_fe(::testGUIDraw(errors));
	if_fail_fe(::testGUIVirtualKeyboard(errors));
	if_fail_fe(::testGUIViewportOperations(errors));
	rtrn ::testGUIComponents(errors);
}
