#include "llc_gui_viewport.h"

#include "llc_test_core.h"

#include <type_traits>

GDEFINE_ENUM_TYPE(GUI_VIEWPORT_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, DEFAULT_ROOT				,  0, "A default viewport has an assigned root control.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, DEFAULT_TITLE				,  1, "A default viewport has an assigned title control.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, DEFAULT_CLIENT				,  2, "A default viewport has an assigned client control.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, DEFAULT_DRAGGING			,  3, "A default viewport is dragging.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, DEFAULT_UNFOLDED			,  4, "A default viewport is folded.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, DEFAULT_LOCK_X				,  5, "A default viewport locks horizontal displacement.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, DEFAULT_LOCK_Y				,  6, "A default viewport locks vertical displacement.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, INVALID_PARENT_RESULT		,  7, "Viewport creation accepted an invalid parent.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, INVALID_PARENT_CONTROLS	,  8, "Rejected viewport creation changed the live-control count.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, INVALID_WIDTH_RESULT		,  9, "Viewport creation accepted a negative client width.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, INVALID_HEIGHT_RESULT		, 10, "Viewport creation accepted a negative client height.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, INVALID_TITLE_RESULT		, 11, "Viewport creation accepted a non-positive title height.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, OVERFLOW_WIDTH_RESULT		, 12, "Viewport creation accepted a width outside signed 16-bit geometry.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, OVERFLOW_HEIGHT_RESULT		, 13, "Viewport creation accepted a height outside signed 16-bit geometry.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, REJECTED_VIEWPORT_STATE	, 14, "Rejected viewport creation assigned a control handle.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, CREATE_RESULT				, 15, "Viewport creation failed.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, CREATE_RETURN				, 16, "Viewport creation did not return the root control handle.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, REUSED_ROOT				, 17, "Viewport creation did not reuse the first available root slot.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, REUSED_CLIENT				, 18, "Viewport creation did not reuse the first available client slot.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, NONCONTIGUOUS_HANDLES		, 19, "Viewport control handles were inferred from adjacency.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, ROOT_PARENT				, 20, "A viewport root has the wrong parent.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, TITLE_PARENT				, 21, "A viewport title has the wrong parent.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, CLIENT_PARENT				, 22, "A viewport client has the wrong parent.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, ROOT_CHILD_COUNT			, 23, "A viewport root has the wrong child count.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, ROOT_TITLE_CHILD			, 24, "A viewport root does not own its title control.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, ROOT_CLIENT_CHILD			, 25, "A viewport root does not own its client control.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, ROOT_WIDTH				, 26, "A viewport root has the wrong width.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, ROOT_HEIGHT				, 27, "A viewport root has the wrong height.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, ROOT_ALIGN				, 28, "A viewport root has the wrong alignment.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, ROOT_BORDER_LEFT			, 29, "A viewport root has the wrong left border.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, ROOT_BORDER_TOP			, 30, "A viewport root has the wrong top border.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, ROOT_BORDER_RIGHT			, 31, "A viewport root has the wrong right border.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, ROOT_BORDER_BOTTOM		, 32, "A viewport root has the wrong bottom border.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, ROOT_MARGIN_LEFT			, 33, "A viewport root has the wrong left margin.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, ROOT_MARGIN_TOP			, 34, "A viewport root has the wrong top margin.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, ROOT_MARGIN_RIGHT			, 35, "A viewport root has the wrong right margin.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, ROOT_MARGIN_BOTTOM		, 36, "A viewport root has the wrong bottom margin.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, ROOT_FRAME				, 37, "A viewport root does not draw an outward frame.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, TITLE_WIDTH				, 38, "A viewport title has the wrong width.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, TITLE_HEIGHT				, 39, "A viewport title has the wrong height.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, TITLE_ALIGN				, 40, "A viewport title has the wrong control alignment.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, TITLE_TEXT_ALIGN			, 41, "A viewport title has the wrong text alignment.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, TITLE_MARGIN				, 42, "A viewport title has the wrong text-side margin.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, TITLE_ATTACH				, 43, "A viewport title does not fill the root width.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, TITLE_TEXT				, 44, "A viewport title has the wrong text.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, CLIENT_WIDTH				, 45, "A viewport client has the wrong width.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, CLIENT_HEIGHT				, 46, "A viewport client has the wrong height.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, CLIENT_OFFSET_X			, 47, "A viewport client has the wrong horizontal offset.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, CLIENT_OFFSET_Y			, 48, "A viewport client has the wrong vertical offset.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, CLIENT_MARGIN_LEFT		, 49, "A viewport client has the wrong left margin.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, CLIENT_MARGIN_TOP			, 50, "A viewport client has the wrong top margin.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, CLIENT_MARGIN_RIGHT		, 51, "A viewport client has the wrong right margin.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, CLIENT_MARGIN_BOTTOM		, 52, "A viewport client has the wrong bottom margin.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, CLIENT_ATTACH				, 53, "A viewport client does not fill the root width.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, REENTRY_RESULT				, 54, "Viewport creation accepted an already assigned viewport.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, REENTRY_ROOT				, 55, "Rejected viewport reentry changed the root handle.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, REENTRY_TITLE				, 56, "Rejected viewport reentry changed the title handle.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, REENTRY_CLIENT				, 57, "Rejected viewport reentry changed the client handle.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, REENTRY_CONTROLS			, 58, "Rejected viewport reentry changed the live-control count.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, FOLD_RESULT				, 59, "Viewport folding returned the wrong state.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, FOLD_STATE				, 60, "Viewport folding stored the wrong state.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, FOLD_HEIGHT				, 61, "Viewport folding produced the wrong root height.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, FOLD_VISIBILITY			, 62, "Viewport folding left the client visible.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, UNFOLD_RESULT				, 63, "Viewport unfolding returned the wrong state.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, UNFOLD_STATE				, 64, "Viewport unfolding stored the wrong state.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, UNFOLD_HEIGHT				, 65, "Viewport unfolding produced the wrong root height.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, UNFOLD_VISIBILITY			, 66, "Viewport unfolding left the client hidden.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, ACTION_RESULT				, 67, "Viewport title action processing failed.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, ACTION_FOLD				, 68, "A viewport title action did not fold the viewport.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, DRAG_RESULT				, 69, "Viewport dragging failed.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, DRAG_OFFSET_X				, 70, "Viewport dragging produced the wrong horizontal offset.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, DRAG_OFFSET_Y				, 71, "Viewport dragging produced the wrong vertical offset.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, DRAG_STATE				, 72, "Viewport dragging stored the wrong state.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, DRAG_RELEASE_STATE		, 73, "Viewport drag release did not clear the dragging state.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, DRAG_RELEASE_FOLD			, 74, "Viewport drag release was interpreted as a title action.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, LOCK_OFFSET_X				, 75, "Horizontal displacement lock changed the viewport x offset.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, LOCK_OFFSET_Y				, 76, "Vertical displacement lock changed the viewport y offset.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, SCALE_OFFSET_X				, 77, "Scaled dragging produced the wrong horizontal offset.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, SCALE_OFFSET_Y				, 78, "Scaled dragging produced the wrong vertical offset.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, ALIGN_OFFSET_X				, 79, "Right-aligned dragging produced the wrong horizontal offset.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, ALIGN_OFFSET_Y				, 80, "Bottom-aligned dragging produced the wrong vertical offset.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, RESIZE_RESULT				, 81, "Viewport client-size propagation failed.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, RESIZE_CLIENT_WIDTH		, 82, "Viewport resizing propagated the wrong client width.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, RESIZE_CLIENT_HEIGHT		, 83, "Viewport resizing propagated the wrong client height.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, FOLDED_RESIZE_WIDTH		, 84, "A folded viewport changed its stored client width.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, FOLDED_RESIZE_HEIGHT		, 85, "A folded viewport changed its stored client height.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, INVALID_VIEWPORT_FOLD	, 86, "Viewport folding accepted invalid control handles.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, INVALID_VIEWPORT_UPDATE	, 87, "Viewport update accepted invalid control handles.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, INVALID_SCALE_RESULT		, 88, "Viewport dragging accepted a zero GUI scale.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, INVALID_SCALE_OFFSET_X	, 89, "Rejected zero-scale dragging changed the viewport x offset.");
GDEFINE_ENUM_VALUED(GUI_VIEWPORT_TEST_RESULT, INVALID_SCALE_OFFSET_Y	, 90, "Rejected zero-scale dragging changed the viewport y offset.");

static_assert(::std::is_trivially_copyable_v<::llc::SViewport>);

sttc ::llc::u2_t viewportLiveControlCount(cnst ::llc::SGUI & gui) {
	::llc::u2_t count = 0;
	for(::llc::u2_t iControl = 0; iControl < gui.Controls.Entities.size(); ++iControl) {
		count += one_if(false == ::llc::controlInvalid(gui, (::llc::cid_t)iControl));
	}
	rtrn count;
}

sttc ::llc::err_t testGUIViewportDefaults(ATestError & errors) {
	cnst ::llc::SViewport viewport = {};
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_DEFAULT_ROOT, viewport.IdRoot != ::llc::CID_INVALID, "Root:%i, expected:%i.", viewport.IdRoot, ::llc::CID_INVALID);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_DEFAULT_TITLE, viewport.IdTitle != ::llc::CID_INVALID, "Title:%i, expected:%i.", viewport.IdTitle, ::llc::CID_INVALID);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_DEFAULT_CLIENT, viewport.IdClient != ::llc::CID_INVALID, "Client:%i, expected:%i.", viewport.IdClient, ::llc::CID_INVALID);
	LLC_TEST_CHECK(errors, GUI_VIEWPORT_TEST_RESULT_DEFAULT_DRAGGING, viewport.Settings.Dragging);
	LLC_TEST_CHECK(errors, GUI_VIEWPORT_TEST_RESULT_DEFAULT_UNFOLDED, false == viewport.Settings.Unfolded);
	LLC_TEST_CHECK(errors, GUI_VIEWPORT_TEST_RESULT_DEFAULT_LOCK_X, viewport.Settings.DisplacementLock.x);
	LLC_TEST_CHECK(errors, GUI_VIEWPORT_TEST_RESULT_DEFAULT_LOCK_Y, viewport.Settings.DisplacementLock.y);
	rtrn 0;
}

sttc ::llc::err_t testGUIViewportInvalidCreation(ATestError & errors) {
	::llc::SGUI gui = {};
	::llc::SViewport viewport = {};
	::llc::SViewportCreate setup = {};
	::llc::setupLogCallbacks(0, 0);
	setup.Parent = 0;
	cnst ::llc::err_t invalidParentResult = ::llc::viewportCreate(viewport, gui, setup);
	setup = {};
	setup.ClientSize.x = -1;
	cnst ::llc::err_t invalidWidthResult = ::llc::viewportCreate(viewport, gui, setup);
	setup = {};
	setup.ClientSize.y = -1;
	cnst ::llc::err_t invalidHeightResult = ::llc::viewportCreate(viewport, gui, setup);
	setup = {};
	setup.TitleHeight = 0;
	cnst ::llc::err_t invalidTitleResult = ::llc::viewportCreate(viewport, gui, setup);
	setup = {};
	setup.ClientSize = {0x7FFF, 0};
	cnst ::llc::err_t overflowWidthResult = ::llc::viewportCreate(viewport, gui, setup);
	setup = {};
	setup.ClientSize = {0, 0x7FFF};
	cnst ::llc::err_t overflowHeightResult = ::llc::viewportCreate(viewport, gui, setup);
	::llc::setupDefaultLogCallbacks();
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_INVALID_PARENT_RESULT, false == ::llc::failed(invalidParentResult), "Result:%i.", invalidParentResult);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_INVALID_PARENT_CONTROLS, ::viewportLiveControlCount(gui), "Live controls:%u, expected:0.", ::viewportLiveControlCount(gui));
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_INVALID_WIDTH_RESULT, false == ::llc::failed(invalidWidthResult), "Result:%i.", invalidWidthResult);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_INVALID_HEIGHT_RESULT, false == ::llc::failed(invalidHeightResult), "Result:%i.", invalidHeightResult);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_INVALID_TITLE_RESULT, false == ::llc::failed(invalidTitleResult), "Result:%i.", invalidTitleResult);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_OVERFLOW_WIDTH_RESULT, false == ::llc::failed(overflowWidthResult), "Result:%i.", overflowWidthResult);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_OVERFLOW_HEIGHT_RESULT, false == ::llc::failed(overflowHeightResult), "Result:%i.", overflowHeightResult);
	LLC_TEST_CHECK(errors, GUI_VIEWPORT_TEST_RESULT_REJECTED_VIEWPORT_STATE, viewport.IdRoot != ::llc::CID_INVALID);
	LLC_TEST_CHECK(errors, GUI_VIEWPORT_TEST_RESULT_REJECTED_VIEWPORT_STATE, viewport.IdTitle != ::llc::CID_INVALID);
	LLC_TEST_CHECK(errors, GUI_VIEWPORT_TEST_RESULT_REJECTED_VIEWPORT_STATE, viewport.IdClient != ::llc::CID_INVALID);
	rtrn 0;
}

sttc ::llc::err_t testGUIViewportCreation(ATestError & errors) {
	::llc::SGUI gui = {};
	::llc::acid controls = {};
	for(::llc::u0_t iControl = 0; iControl < 8; ++iControl) {
		if_fail_fe(controls.push_back(::llc::controlCreate(gui)));
	}
	if_fail_fe(::llc::controlDelete(gui, controls[1]));
	if_fail_fe(::llc::controlDelete(gui, controls[3]));
	if_fail_fe(::llc::controlDelete(gui, controls[6]));
	cnst ::llc::cid_t idParent = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_VIEWPORT_TEST_RESULT_CREATE_RESULT, idParent < 0, "Parent creation result:%i.", idParent);
	::llc::SViewport viewport = {};
	::llc::SViewportCreate setup = {};
	setup.Parent = idParent;
	setup.ClientSize = {320, 200};
	setup.Title = LLC_CXS("Scene viewport");
	setup.TitleHeight = 18;
	cnst ::llc::err_t createResult = ::llc::viewportCreate(viewport, gui, setup);
	LLC_TEST_REQUIREF(errors, GUI_VIEWPORT_TEST_RESULT_CREATE_RESULT, ::llc::failed(createResult), "Creation result:%i.", createResult);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_CREATE_RETURN, createResult != viewport.IdRoot, "Creation result:%i, root:%i.", createResult, viewport.IdRoot);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_REUSED_ROOT, viewport.IdRoot != controls[3], "Root:%i, expected reused slot:%i.", viewport.IdRoot, controls[3]);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_REUSED_CLIENT, viewport.IdClient != controls[6], "Client:%i, expected reused slot:%i.", viewport.IdClient, controls[6]);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_NONCONTIGUOUS_HANDLES, viewport.IdClient == viewport.IdRoot + 1, "Root:%i and client:%i unexpectedly contiguous.", viewport.IdRoot, viewport.IdClient);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_NONCONTIGUOUS_HANDLES, viewport.IdTitle == viewport.IdClient + 1, "Client:%i and title:%i unexpectedly contiguous.", viewport.IdClient, viewport.IdTitle);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_ROOT_PARENT, gui.Controls.Entities[viewport.IdRoot].Parent != idParent, "Parent:%i, expected:%i.", gui.Controls.Entities[viewport.IdRoot].Parent, idParent);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_TITLE_PARENT, gui.Controls.Entities[viewport.IdTitle].Parent != viewport.IdRoot, "Parent:%i, expected:%i.", gui.Controls.Entities[viewport.IdTitle].Parent, viewport.IdRoot);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_CLIENT_PARENT, gui.Controls.Entities[viewport.IdClient].Parent != viewport.IdRoot, "Parent:%i, expected:%i.", gui.Controls.Entities[viewport.IdClient].Parent, viewport.IdRoot);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_ROOT_CHILD_COUNT, gui.Controls.Children[viewport.IdRoot].size() != 2, "Child count:%u, expected:2.", gui.Controls.Children[viewport.IdRoot].size());
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_ROOT_TITLE_CHILD, gui.Controls.Children[viewport.IdRoot].find(viewport.IdTitle) < 0, "Title:%i not found.", viewport.IdTitle);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_ROOT_CLIENT_CHILD, gui.Controls.Children[viewport.IdRoot].find(viewport.IdClient) < 0, "Client:%i not found.", viewport.IdClient);
	cnst ::llc::SControlPlacement & rootPlacement = gui.Controls.Placement[viewport.IdRoot];
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_ROOT_WIDTH, rootPlacement.Area.Size.x != 324, "Width:%i, expected:324.", rootPlacement.Area.Size.x);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_ROOT_HEIGHT, rootPlacement.Area.Size.y != 223, "Height:%i, expected:223.", rootPlacement.Area.Size.y);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_ROOT_ALIGN, rootPlacement.Align != ::llc::ALIGN_TOP_LEFT, "Align:0x%X, expected:0x%X.", rootPlacement.Align, ::llc::ALIGN_TOP_LEFT);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_ROOT_BORDER_LEFT, rootPlacement.Border.Left != 1, "Left:%u, expected:1.", rootPlacement.Border.Left);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_ROOT_BORDER_TOP, rootPlacement.Border.Top != 1, "Top:%u, expected:1.", rootPlacement.Border.Top);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_ROOT_BORDER_RIGHT, rootPlacement.Border.Right != 1, "Right:%u, expected:1.", rootPlacement.Border.Right);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_ROOT_BORDER_BOTTOM, rootPlacement.Border.Bottom != 1, "Bottom:%u, expected:1.", rootPlacement.Border.Bottom);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_ROOT_MARGIN_LEFT, rootPlacement.Margin.Left != 1, "Left:%u, expected:1.", rootPlacement.Margin.Left);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_ROOT_MARGIN_TOP, rootPlacement.Margin.Top != 1, "Top:%u, expected:1.", rootPlacement.Margin.Top);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_ROOT_MARGIN_RIGHT, rootPlacement.Margin.Right != 1, "Right:%u, expected:1.", rootPlacement.Margin.Right);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_ROOT_MARGIN_BOTTOM, rootPlacement.Margin.Bottom != 1, "Bottom:%u, expected:1.", rootPlacement.Margin.Bottom);
	LLC_TEST_CHECK(errors, GUI_VIEWPORT_TEST_RESULT_ROOT_FRAME, false == gui.Controls.Draw[viewport.IdRoot].FrameOut);
	cnst ::llc::SControlPlacement & titlePlacement = gui.Controls.Placement[viewport.IdTitle];
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_TITLE_WIDTH, titlePlacement.Area.Size.x, "Width:%i, expected:0.", titlePlacement.Area.Size.x);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_TITLE_HEIGHT, titlePlacement.Area.Size.y != 18, "Height:%i, expected:18.", titlePlacement.Area.Size.y);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_TITLE_ALIGN, titlePlacement.Align != ::llc::ALIGN_TOP_LEFT, "Align:0x%X, expected:0x%X.", titlePlacement.Align, ::llc::ALIGN_TOP_LEFT);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_TITLE_TEXT_ALIGN, gui.Controls.Text[viewport.IdTitle].Align != ::llc::ALIGN_CENTER_LEFT, "Text align:0x%X, expected:0x%X.", gui.Controls.Text[viewport.IdTitle].Align, ::llc::ALIGN_CENTER_LEFT);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_TITLE_MARGIN, titlePlacement.Margin.Left != 4, "Left margin:%u, expected:4.", titlePlacement.Margin.Left);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_TITLE_ATTACH, gui.Controls.Constraints[viewport.IdTitle].AttachSizeToControl.x != viewport.IdTitle, "Attached control:%i, expected:%i.", gui.Controls.Constraints[viewport.IdTitle].AttachSizeToControl.x, viewport.IdTitle);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_TITLE_TEXT, gui.Controls.Text[viewport.IdTitle].Text != setup.Title, "Text:'%.*s', expected:'%.*s'.", LLC_VIEW_ARGS(gui.Controls.Text[viewport.IdTitle].Text), LLC_VIEW_ARGS(setup.Title));
	cnst ::llc::SControlPlacement & clientPlacement = gui.Controls.Placement[viewport.IdClient];
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_CLIENT_WIDTH, clientPlacement.Area.Size.x != 320, "Width:%i, expected:320.", clientPlacement.Area.Size.x);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_CLIENT_HEIGHT, clientPlacement.Area.Size.y != 200, "Height:%i, expected:200.", clientPlacement.Area.Size.y);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_CLIENT_OFFSET_X, clientPlacement.Area.Offset.x, "Offset x:%i, expected:0.", clientPlacement.Area.Offset.x);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_CLIENT_OFFSET_Y, clientPlacement.Area.Offset.y != 19, "Offset y:%i, expected:19.", clientPlacement.Area.Offset.y);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_CLIENT_MARGIN_LEFT, clientPlacement.Margin.Left, "Left:%u, expected:0.", clientPlacement.Margin.Left);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_CLIENT_MARGIN_TOP, clientPlacement.Margin.Top, "Top:%u, expected:0.", clientPlacement.Margin.Top);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_CLIENT_MARGIN_RIGHT, clientPlacement.Margin.Right, "Right:%u, expected:0.", clientPlacement.Margin.Right);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_CLIENT_MARGIN_BOTTOM, clientPlacement.Margin.Bottom, "Bottom:%u, expected:0.", clientPlacement.Margin.Bottom);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_CLIENT_ATTACH, gui.Controls.Constraints[viewport.IdClient].AttachSizeToControl.x != viewport.IdClient, "Attached control:%i, expected:%i.", gui.Controls.Constraints[viewport.IdClient].AttachSizeToControl.x, viewport.IdClient);
	cnst ::llc::SViewport original = viewport;
	cnst ::llc::u2_t controlCount = ::viewportLiveControlCount(gui);
	::llc::setupLogCallbacks(0, 0);
	cnst ::llc::err_t reentryResult = ::llc::viewportCreate(viewport, gui, setup);
	::llc::setupDefaultLogCallbacks();
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_REENTRY_RESULT, false == ::llc::failed(reentryResult), "Result:%i.", reentryResult);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_REENTRY_ROOT, viewport.IdRoot != original.IdRoot, "Root:%i, expected:%i.", viewport.IdRoot, original.IdRoot);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_REENTRY_TITLE, viewport.IdTitle != original.IdTitle, "Title:%i, expected:%i.", viewport.IdTitle, original.IdTitle);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_REENTRY_CLIENT, viewport.IdClient != original.IdClient, "Client:%i, expected:%i.", viewport.IdClient, original.IdClient);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_REENTRY_CONTROLS, ::viewportLiveControlCount(gui) != controlCount, "Live controls:%u, expected:%u.", ::viewportLiveControlCount(gui), controlCount);
	rtrn 0;
}

sttc ::llc::err_t testGUIViewportGeometry(ATestError & errors) {
	::llc::SGUI gui = {};
	stacxpr ::llc::s1_t widths[] = {0, 1, 2, 15, 16, 31, 32, 255, 1024, 32763};
	stacxpr ::llc::s1_t heights[] = {0, 1, 2, 15, 16, 31, 32, 255, 1024, 16378};
	stacxpr ::llc::s1_t titleHeights[] = {1, 2, 7, 8, 20, 127, 1024, 16384};
	for(cnst ::llc::s1_t titleHeight : titleHeights) {
		for(cnst ::llc::s1_t height : heights) {
			for(cnst ::llc::s1_t width : widths) {
				::llc::SViewport viewport = {};
				::llc::SViewportCreate setup = {};
				setup.ClientSize = {width, height};
				setup.Title = LLC_CXS("Geometry");
				setup.TitleHeight = titleHeight;
				cnst ::llc::err_t createResult = ::llc::viewportCreate(viewport, gui, setup);
				LLC_TEST_REQUIREF(errors, GUI_VIEWPORT_TEST_RESULT_CREATE_RESULT, ::llc::failed(createResult), "Client:{%i, %i}, title:%i, result:%i.", width, height, titleHeight, createResult);
				LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_ROOT_WIDTH, gui.Controls.Placement[viewport.IdRoot].Area.Size.x != width + 4, "Client width:%i, root width:%i, expected:%i.", width, gui.Controls.Placement[viewport.IdRoot].Area.Size.x, width + 4);
				LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_ROOT_HEIGHT, gui.Controls.Placement[viewport.IdRoot].Area.Size.y != height + titleHeight + ::llc::VIEWPORT_CLIENT_GAP + 4, "Client height:%i, title:%i, root height:%i, expected:%i.", height, titleHeight, gui.Controls.Placement[viewport.IdRoot].Area.Size.y, height + titleHeight + ::llc::VIEWPORT_CLIENT_GAP + 4);
				LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_TITLE_HEIGHT, gui.Controls.Placement[viewport.IdTitle].Area.Size.y != titleHeight, "Title height:%i, expected:%i.", gui.Controls.Placement[viewport.IdTitle].Area.Size.y, titleHeight);
				LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_CLIENT_WIDTH, gui.Controls.Placement[viewport.IdClient].Area.Size.x != width, "Client width:%i, expected:%i.", gui.Controls.Placement[viewport.IdClient].Area.Size.x, width);
				LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_CLIENT_HEIGHT, gui.Controls.Placement[viewport.IdClient].Area.Size.y != height, "Client height:%i, expected:%i.", gui.Controls.Placement[viewport.IdClient].Area.Size.y, height);
				LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_CLIENT_OFFSET_Y, gui.Controls.Placement[viewport.IdClient].Area.Offset.y != titleHeight + ::llc::VIEWPORT_CLIENT_GAP, "Client offset:%i, expected:%i.", gui.Controls.Placement[viewport.IdClient].Area.Offset.y, titleHeight + ::llc::VIEWPORT_CLIENT_GAP);
				cnst ::llc::err_t foldResult = ::llc::viewportFold(viewport, gui, true);
				LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_FOLD_RESULT, foldResult != 1, "Result:%i, expected:1.", foldResult);
				LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_FOLD_HEIGHT, gui.Controls.Placement[viewport.IdRoot].Area.Size.y != titleHeight + 4, "Folded height:%i, expected:%i.", gui.Controls.Placement[viewport.IdRoot].Area.Size.y, titleHeight + 4);
				LLC_TEST_CHECK(errors, GUI_VIEWPORT_TEST_RESULT_FOLD_VISIBILITY, false == gui.Controls.IsHidden(viewport.IdClient));
				cnst ::llc::err_t unfoldResult = ::llc::viewportFold(viewport, gui, false);
				LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_UNFOLD_RESULT, unfoldResult, "Result:%i, expected:0.", unfoldResult);
				LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_UNFOLD_HEIGHT, gui.Controls.Placement[viewport.IdRoot].Area.Size.y != height + titleHeight + ::llc::VIEWPORT_CLIENT_GAP + 4, "Unfolded height:%i, expected:%i.", gui.Controls.Placement[viewport.IdRoot].Area.Size.y, height + titleHeight + ::llc::VIEWPORT_CLIENT_GAP + 4);
				LLC_TEST_CHECK(errors, GUI_VIEWPORT_TEST_RESULT_UNFOLD_VISIBILITY, gui.Controls.IsHidden(viewport.IdClient));
				if_fail_fe(::llc::controlDelete(gui, viewport.IdRoot));
			}
		}
	}
	rtrn 0;
}

sttc ::llc::err_t viewportDragRelease(::llc::SViewport & viewport, ::llc::SGUI & gui) {
	gui.Controls.SetPressed(viewport.IdTitle, false);
	gui.Controls.SetAction(viewport.IdTitle, false);
	rtrn ::llc::viewportUpdate(viewport, gui);
}

sttc ::llc::err_t testGUIViewportBehavior(ATestError & errors) {
	::llc::SGUI gui = {};
	::llc::SViewport viewport = {};
	if_fail_fe(::llc::viewportCreate(viewport, gui));
	cnst ::llc::s1_t unfoldedHeight = gui.Controls.Placement[viewport.IdRoot].Area.Size.y;
	cnst ::llc::err_t foldResult = ::llc::viewportFold(viewport, gui, true);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_FOLD_RESULT, foldResult != 1, "Result:%i, expected:1.", foldResult);
	LLC_TEST_CHECK(errors, GUI_VIEWPORT_TEST_RESULT_FOLD_STATE, viewport.Settings.Unfolded);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_FOLD_HEIGHT, gui.Controls.Placement[viewport.IdRoot].Area.Size.y != ::llc::VIEWPORT_TITLE_HEIGHT + 4, "Height:%i, expected:%i.", gui.Controls.Placement[viewport.IdRoot].Area.Size.y, ::llc::VIEWPORT_TITLE_HEIGHT + 4);
	LLC_TEST_CHECK(errors, GUI_VIEWPORT_TEST_RESULT_FOLD_VISIBILITY, false == gui.Controls.IsHidden(viewport.IdClient));
	cnst ::llc::err_t foldRepeatResult = ::llc::viewportFold(viewport, gui, true);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_FOLD_RESULT, foldRepeatResult != 1, "Repeated result:%i, expected:1.", foldRepeatResult);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_FOLD_HEIGHT, gui.Controls.Placement[viewport.IdRoot].Area.Size.y != ::llc::VIEWPORT_TITLE_HEIGHT + 4, "Repeated height:%i, expected:%i.", gui.Controls.Placement[viewport.IdRoot].Area.Size.y, ::llc::VIEWPORT_TITLE_HEIGHT + 4);
	cnst ::llc::err_t unfoldResult = ::llc::viewportFold(viewport, gui, false);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_UNFOLD_RESULT, unfoldResult, "Result:%i, expected:0.", unfoldResult);
	LLC_TEST_CHECK(errors, GUI_VIEWPORT_TEST_RESULT_UNFOLD_STATE, false == viewport.Settings.Unfolded);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_UNFOLD_HEIGHT, gui.Controls.Placement[viewport.IdRoot].Area.Size.y != unfoldedHeight, "Height:%i, expected:%i.", gui.Controls.Placement[viewport.IdRoot].Area.Size.y, unfoldedHeight);
	LLC_TEST_CHECK(errors, GUI_VIEWPORT_TEST_RESULT_UNFOLD_VISIBILITY, gui.Controls.IsHidden(viewport.IdClient));
	gui.Controls.SetAction(viewport.IdTitle, true);
	cnst ::llc::err_t actionResult = ::llc::viewportUpdate(viewport, gui);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_ACTION_RESULT, ::llc::failed(actionResult), "Result:%i.", actionResult);
	LLC_TEST_CHECK(errors, GUI_VIEWPORT_TEST_RESULT_ACTION_FOLD, viewport.Settings.Unfolded);
	gui.Controls.SetAction(viewport.IdTitle, false);
	if_fail_fe(::llc::viewportFold(viewport, gui, false));
	::llc::SControlPlacement & rootPlacement = gui.Controls.Placement[viewport.IdRoot];
	rootPlacement.Area.Offset = {100, 200};
	gui.Controls.SetPressed(viewport.IdTitle, true);
	cnst ::llc::err_t dragResult = ::llc::viewportUpdate(viewport, gui, {10, -6});
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_DRAG_RESULT, ::llc::failed(dragResult), "Result:%i.", dragResult);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_DRAG_OFFSET_X, rootPlacement.Area.Offset.x != 110, "Offset x:%i, expected:110.", rootPlacement.Area.Offset.x);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_DRAG_OFFSET_Y, rootPlacement.Area.Offset.y != 194, "Offset y:%i, expected:194.", rootPlacement.Area.Offset.y);
	LLC_TEST_CHECK(errors, GUI_VIEWPORT_TEST_RESULT_DRAG_STATE, false == viewport.Settings.Dragging);
	gui.Controls.SetPressed(viewport.IdTitle, false);
	gui.Controls.SetAction(viewport.IdTitle, true);
	if_fail_fe(::llc::viewportUpdate(viewport, gui));
	LLC_TEST_CHECK(errors, GUI_VIEWPORT_TEST_RESULT_DRAG_RELEASE_STATE, viewport.Settings.Dragging);
	LLC_TEST_CHECK(errors, GUI_VIEWPORT_TEST_RESULT_DRAG_RELEASE_FOLD, false == viewport.Settings.Unfolded);
	gui.Controls.SetAction(viewport.IdTitle, false);
	viewport.Settings.DisplacementLock = {true, false};
	rootPlacement.Area.Offset = {};
	gui.Controls.SetPressed(viewport.IdTitle, true);
	if_fail_fe(::llc::viewportUpdate(viewport, gui, {7, 8}));
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_LOCK_OFFSET_X, rootPlacement.Area.Offset.x, "Offset x:%i, expected:0.", rootPlacement.Area.Offset.x);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_DRAG_OFFSET_Y, rootPlacement.Area.Offset.y != 8, "Offset y:%i, expected:8.", rootPlacement.Area.Offset.y);
	if_fail_fe(::viewportDragRelease(viewport, gui));
	viewport.Settings.DisplacementLock = {false, true};
	rootPlacement.Area.Offset = {};
	gui.Controls.SetPressed(viewport.IdTitle, true);
	if_fail_fe(::llc::viewportUpdate(viewport, gui, {7, 8}));
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_DRAG_OFFSET_X, rootPlacement.Area.Offset.x != 7, "Offset x:%i, expected:7.", rootPlacement.Area.Offset.x);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_LOCK_OFFSET_Y, rootPlacement.Area.Offset.y, "Offset y:%i, expected:0.", rootPlacement.Area.Offset.y);
	if_fail_fe(::viewportDragRelease(viewport, gui));
	viewport.Settings.DisplacementLock = {};
	rootPlacement.Area.Offset = {};
	gui.Zoom.DPI = {2, 3};
	gui.Zoom.ZoomLevel = 2;
	gui.Controls.SetPressed(viewport.IdTitle, true);
	if_fail_fe(::llc::viewportUpdate(viewport, gui, {8, 18}));
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_SCALE_OFFSET_X, rootPlacement.Area.Offset.x != 2, "Offset x:%i, expected:2.", rootPlacement.Area.Offset.x);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_SCALE_OFFSET_Y, rootPlacement.Area.Offset.y != 3, "Offset y:%i, expected:3.", rootPlacement.Area.Offset.y);
	if_fail_fe(::viewportDragRelease(viewport, gui));
	gui.Zoom = {};
	rootPlacement.Align = ::llc::ALIGN_BOTTOM_RIGHT;
	rootPlacement.Area.Offset = {50, 60};
	gui.Controls.SetPressed(viewport.IdTitle, true);
	if_fail_fe(::llc::viewportUpdate(viewport, gui, {4, 6}));
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_ALIGN_OFFSET_X, rootPlacement.Area.Offset.x != 46, "Offset x:%i, expected:46.", rootPlacement.Area.Offset.x);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_ALIGN_OFFSET_Y, rootPlacement.Area.Offset.y != 54, "Offset y:%i, expected:54.", rootPlacement.Area.Offset.y);
	if_fail_fe(::viewportDragRelease(viewport, gui));
	rootPlacement.Align = ::llc::ALIGN_TOP_LEFT;
	rootPlacement.Area.Size = {404, 325};
	cnst ::llc::err_t resizeResult = ::llc::viewportUpdate(viewport, gui);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_RESIZE_RESULT, ::llc::failed(resizeResult), "Result:%i.", resizeResult);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_RESIZE_CLIENT_WIDTH, gui.Controls.Placement[viewport.IdClient].Area.Size.x != 400, "Width:%i, expected:400.", gui.Controls.Placement[viewport.IdClient].Area.Size.x);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_RESIZE_CLIENT_HEIGHT, gui.Controls.Placement[viewport.IdClient].Area.Size.y != 300, "Height:%i, expected:300.", gui.Controls.Placement[viewport.IdClient].Area.Size.y);
	if_fail_fe(::llc::viewportFold(viewport, gui, true));
	rootPlacement.Area.Size = {504, 25};
	if_fail_fe(::llc::viewportUpdate(viewport, gui));
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_FOLDED_RESIZE_WIDTH, gui.Controls.Placement[viewport.IdClient].Area.Size.x != 400, "Width:%i, expected:400.", gui.Controls.Placement[viewport.IdClient].Area.Size.x);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_FOLDED_RESIZE_HEIGHT, gui.Controls.Placement[viewport.IdClient].Area.Size.y != 300, "Height:%i, expected:300.", gui.Controls.Placement[viewport.IdClient].Area.Size.y);
	if_fail_fe(::llc::viewportFold(viewport, gui, false));
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_UNFOLD_HEIGHT, rootPlacement.Area.Size.y != 325, "Height:%i, expected:325.", rootPlacement.Area.Size.y);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_CLIENT_HEIGHT, gui.Controls.Placement[viewport.IdClient].Area.Size.y != 300, "Client height:%i, expected:300.", gui.Controls.Placement[viewport.IdClient].Area.Size.y);
	rootPlacement.Area.Offset = {12, 34};
	gui.Zoom.DPI.x = 0;
	gui.Controls.SetPressed(viewport.IdTitle, true);
	::llc::setupLogCallbacks(0, 0);
	cnst ::llc::err_t invalidScaleResult = ::llc::viewportUpdate(viewport, gui, {1, 1});
	::llc::setupDefaultLogCallbacks();
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_INVALID_SCALE_RESULT, false == ::llc::failed(invalidScaleResult), "Result:%i.", invalidScaleResult);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_INVALID_SCALE_OFFSET_X, rootPlacement.Area.Offset.x != 12, "Offset x:%i, expected:12.", rootPlacement.Area.Offset.x);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_INVALID_SCALE_OFFSET_Y, rootPlacement.Area.Offset.y != 34, "Offset y:%i, expected:34.", rootPlacement.Area.Offset.y);
	::llc::SViewport invalid = {};
	::llc::setupLogCallbacks(0, 0);
	cnst ::llc::err_t invalidFoldResult = ::llc::viewportFold(invalid, gui, true);
	cnst ::llc::err_t invalidUpdateResult = ::llc::viewportUpdate(invalid, gui);
	::llc::setupDefaultLogCallbacks();
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_INVALID_VIEWPORT_FOLD, false == ::llc::failed(invalidFoldResult), "Result:%i.", invalidFoldResult);
	LLC_TEST_CHECKF(errors, GUI_VIEWPORT_TEST_RESULT_INVALID_VIEWPORT_UPDATE, false == ::llc::failed(invalidUpdateResult), "Result:%i.", invalidUpdateResult);
	rtrn 0;
}

::llc::err_t testGUIViewport(ATestError & errors) {
	if_fail_fe(::testGUIViewportDefaults(errors));
	if_fail_fe(::testGUIViewportInvalidCreation(errors));
	if_fail_fe(::testGUIViewportCreation(errors));
	if_fail_fe(::testGUIViewportGeometry(errors));
	if_fail_fe(::testGUIViewportBehavior(errors));
	rtrn 0;
}
