#include "llc_gui_render.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(GUI_RENDER_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, CREATE_RESULT		,  0, "The GUI render test could not create a valid control.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, GENERATION_INITIAL	,  1, "A new GUI control received the wrong generation.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, SYNC_RESULT			,  2, "GUI render-resource synchronization reported failure.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, IMAGE_COUNT			,  3, "GUI image-resource storage did not match the control count.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, GENERATION_COUNT	,  4, "GUI image-generation storage did not match the control count.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, GENERATION_VALUE	,  5, "A GUI image binding retained the wrong control generation.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, IMAGE_PRESERVE		,  6, "Synchronization discarded a valid GUI image binding.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, DELETE_RESULT		,  7, "The GUI render test could not delete a valid control.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, IMAGE_CLEAR_UNUSED	,  8, "Synchronization retained an image bound to an unused control.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, REUSE_ID			,  9, "GUI control creation did not reuse the deleted render-resource slot.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, GENERATION_REUSE	, 10, "A reused GUI control slot retained its former generation.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, IMAGE_CLEAR_REUSED	, 11, "Synchronization retained an image bound to a former control generation.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, PALETTE_INDEX		, 12, "A GUI Flag, Mask and inherited-disabled combination resolved to the wrong palette.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, RESOURCE_CREATE		, 13, "The GUI render test could not create required render resources.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, DRAW_RESULT			, 14, "GUI drawing reported failure for valid render resources.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, DIRECT_PIXEL		, 15, "The direct-palette GUI raster contains the wrong pixel channel.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, NO_BORDER_PIXEL		, 16, "A border pixel was written while control border drawing was disabled.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, NO_CLIENT_PIXEL		, 17, "A client pixel was written while control client drawing was disabled.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, HIERARCHY_PIXEL		, 18, "GUI hierarchy drawing produced the wrong parent or child pixel channel.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, HIDDEN_PIXEL		, 19, "A hidden child changed its parent raster.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, DISABLED_PIXEL		, 20, "Inherited disabled state selected the wrong parent or child palette.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, LEGACY_PIXEL		, 21, "The legacy theme and indexed-palette path produced the wrong pixel channel.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, IMAGE_PIXEL			, 22, "GUI image drawing produced the wrong pixel channel.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, IMAGE_INVERT_PIXEL	, 23, "GUI vertical image inversion produced the wrong pixel channel.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, TEXT_PIXEL			, 24, "GUI font rasterization produced the wrong pixel channel.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, OUTSIDE_PIXEL		, 25, "GUI drawing changed a pixel outside the control geometry.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, EVENT_COUNT			, 26, "GUI event generation produced the wrong command count.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, EVENT_RECTANGLE		, 27, "GUI event generation placed the wrong command in a rectangle slot.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, EVENT_TRIANGLE		, 28, "GUI event generation placed the wrong command in a triangle slot.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, EVENT_IMAGE			, 29, "GUI event generation placed the wrong image command.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, EVENT_TEXT			, 30, "GUI event generation placed the wrong text command.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, EVENT_PAYLOAD_LOAD	, 31, "A GUI command payload could not be decoded completely.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, EVENT_CONTROL		, 32, "A GUI command payload contains the wrong control identity.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, EVENT_GENERATION	, 33, "A GUI command payload contains the wrong control generation.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, EVENT_COLOR_SET		, 34, "A GUI command payload contains the wrong color set.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, EVENT_PALETTE		, 35, "A GUI command payload contains the wrong state palette.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, EVENT_AREA			, 36, "A GUI command payload contains the wrong control area.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, EVENT_COLOR_MODE	, 37, "A GUI command payload contains the wrong color mode.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, EVENT_DIRECT		, 38, "A GUI command payload contains the wrong palette-selection mode.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, EVENT_RECT_OFFSET	, 39, "A GUI rectangle command contains the wrong offset.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, EVENT_RECT_SIZE		, 40, "A GUI rectangle command contains the wrong size.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, EVENT_FONT			, 41, "A GUI text command contains the wrong font identity.");
GDEFINE_ENUM_VALUED(GUI_RENDER_TEST_RESULT, EVENT_TEXT_DATA		, 42, "A GUI text command did not own the complete source text.");

sttc ::llc::err_t testGUIRenderPixel(ATestError & errors, GUI_RENDER_TEST_RESULT result, cnst ::llc::vcst_t & name, cnst ::llc::bgra & actual, cnst ::llc::bgra & expected, ::llc::u2_t x, ::llc::u2_t y) {
	LLC_TEST_CHECKF(errors, result, actual.b != expected.b, "%.*s{%u, %u}.b:%u, expected:%u.", LLC_VIEW_ARGS(name), x, y, actual.b, expected.b);
	LLC_TEST_CHECKF(errors, result, actual.g != expected.g, "%.*s{%u, %u}.g:%u, expected:%u.", LLC_VIEW_ARGS(name), x, y, actual.g, expected.g);
	LLC_TEST_CHECKF(errors, result, actual.r != expected.r, "%.*s{%u, %u}.r:%u, expected:%u.", LLC_VIEW_ARGS(name), x, y, actual.r, expected.r);
	LLC_TEST_CHECKF(errors, result, actual.a != expected.a, "%.*s{%u, %u}.a:%u, expected:%u.", LLC_VIEW_ARGS(name), x, y, actual.a, expected.a);
	rtrn 0;
}

sttc ::llc::err_t testGUIRenderResourcesCreate(ATestError & errors, ::llc::SGUIRenderResources & resources, ::llc::u2_t paletteCount) {
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_RESOURCE_CREATE, 0 == resources.Colors.create(), "%s", "SGUIColors allocation failed.");
	cnst ::llc::err_t resizeResult = resources.Colors->Palettes.resize(paletteCount);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_RESOURCE_CREATE, ::llc::failed(resizeResult), "Palette count:%u, resize result:%i.", paletteCount, resizeResult);
	rtrn 0;
}

sttc ::llc::err_t testGUIRenderPaletteIndex(ATestError & errors) {
	for(::llc::u2_t stateValue = 0; stateValue < ::llc::GUI_CONTROL_FLAG_Unused; ++stateValue)
	for(::llc::u2_t maskValue = 0; maskValue <= ::llc::GUI_CONTROL_FLAG_All; ++maskValue)
	for(::llc::u2_t inheritedDisabled = 0; inheritedDisabled < 2; ++inheritedDisabled) {
		cnst ::llc::SControlState state = {(::llc::GUI_CONTROL_FLAG)stateValue, (::llc::GUI_CONTROL_FLAG)maskValue};
		cnst bool disabled = 0 != inheritedDisabled;
		cnst ::llc::GUI_CONTROL_PALETTE expected = state.IsSelected()
			? disabled ? ::llc::GUI_CONTROL_PALETTE_SELECTED_DISABLED : state.IsPressed() ? ::llc::GUI_CONTROL_PALETTE_SELECTED_PRESSED : state.IsHovered() ? ::llc::GUI_CONTROL_PALETTE_SELECTED_HOVER : ::llc::GUI_CONTROL_PALETTE_SELECTED
			: disabled ? ::llc::GUI_CONTROL_PALETTE_DISABLED : state.IsPressed() ? ::llc::GUI_CONTROL_PALETTE_PRESSED : state.IsHovered() ? ::llc::GUI_CONTROL_PALETTE_HOVER : ::llc::GUI_CONTROL_PALETTE_NORMAL;
		cnst ::llc::GUI_CONTROL_PALETTE actual = ::llc::controlPaletteIndex(disabled, state);
		LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_PALETTE_INDEX, actual != expected, "Flag:0x%02X, Mask:0x%02X, inherited disabled:%u, palette:%u, expected:%u.", stateValue, maskValue, inheritedDisabled, (::llc::u2_t)actual, (::llc::u2_t)expected);
	}
	rtrn 0;
}

sttc ::llc::err_t testGUIRenderDirect(ATestError & errors) {
	cnst ::llc::bgra clearColor = {1, 2, 3, 255}, clientColor = {11, 12, 13, 255}, leftColor = {21, 22, 23, 255}, topColor = {31, 32, 33, 255}, rightColor = {41, 42, 43, 255}, bottomColor = {51, 52, 53, 255};
	::llc::SGUI gui = {};
	cnst ::llc::cid_t idControl = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_CREATE_RESULT, idControl < 0, "Control creation result:%i.", idControl);
	::llc::SControlPlacement & placement = gui.Controls.Placement[idControl];
	placement.Area = {{1, 1}, {5, 5}};
	placement.Margin = {};
	placement.Border = {1, 1, 1, 1};
	::llc::SControlDraw & draw = gui.Controls.Draw[idControl];
	draw.UseNewPalettes = true;
	draw.ColorMode = ::llc::GUI_COLOR_MODE_Flat;
	::llc::SGUIRenderResources resources = {};
	if_fail_fe(::testGUIRenderResourcesCreate(errors, resources, ::llc::GUI_CONTROL_PALETTE_COUNT));
	::llc::astatic<::llc::bgra, ::llc::UI_CONTROL_AREA_COUNT> & palette = resources.Colors->Palettes[::llc::GUI_CONTROL_PALETTE_NORMAL];
	palette[::llc::UI_CONTROL_AREA_CLIENT] = clientColor;
	palette[::llc::UI_CONTROL_AREA_BORDER_LEFT] = leftColor;
	palette[::llc::UI_CONTROL_AREA_BORDER_TOP] = topColor;
	palette[::llc::UI_CONTROL_AREA_BORDER_RIGHT] = rightColor;
	palette[::llc::UI_CONTROL_AREA_BORDER_BOTTOM] = bottomColor;
	::llc::bgra pixels[7][7] = {};
	::llc::g8bgra target = pixels;
	::llc::TGUIRenderEventQueue events = {};
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_SYNC_RESULT, 0 > ::llc::guiRenderResourcesSync(resources, gui), "%s", "Direct-palette resource sync failed.");
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_DRAW_RESULT, 0 > ::llc::guiRenderEvents(gui, events, target.metrics().u1()), "%s", "Direct-palette event generation failed.");
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_EVENT_COUNT, events.size() != 14, "Command count:%u, expected:14.", events.size());
	for(::llc::u2_t iEvent = 0; iEvent < 5; ++iEvent) LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_EVENT_RECTANGLE, events[iEvent].Type != ::llc::GUI_RENDER_COMMAND_Rectangle, "Command[%u]:%u, expected rectangle:%u.", iEvent, (::llc::u2_t)events[iEvent].Type, (::llc::u2_t)::llc::GUI_RENDER_COMMAND_Rectangle);
	for(::llc::u2_t iEvent = 5; iEvent < 13; ++iEvent) LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_EVENT_TRIANGLE, events[iEvent].Type != ::llc::GUI_RENDER_COMMAND_Triangle, "Command[%u]:%u, expected triangle:%u.", iEvent, (::llc::u2_t)events[iEvent].Type, (::llc::u2_t)::llc::GUI_RENDER_COMMAND_Triangle);
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_EVENT_IMAGE, events[13].Type != ::llc::GUI_RENDER_COMMAND_Image, "Command[13]:%u, expected image:%u.", (::llc::u2_t)events[13].Type, (::llc::u2_t)::llc::GUI_RENDER_COMMAND_Image);
	::llc::vcu0_t commandInput = events[0].Data;
	::llc::SGUIRenderRectangle command = {};
	cnst ::llc::err_t commandLoadResult = ::llc::loadPOD(commandInput, command);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_EVENT_PAYLOAD_LOAD, ::llc::failed(commandLoadResult), "Rectangle command load result:%i.", commandLoadResult);
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_EVENT_PAYLOAD_LOAD, commandInput.size(), "Rectangle payload trailing bytes:%u, expected:0.", commandInput.size());
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_EVENT_CONTROL, command.Style.Control != idControl, "Control:%i, expected:%i.", command.Style.Control, idControl);
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_EVENT_GENERATION, command.Style.Generation != gui.Controls.Entities[idControl].Generation, "Generation:%u, expected:%u.", command.Style.Generation, gui.Controls.Entities[idControl].Generation);
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_EVENT_COLOR_SET, command.Style.ColorSet != 0, "Color set:%i, expected:0.", command.Style.ColorSet);
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_EVENT_PALETTE, command.Style.Palette != ::llc::GUI_CONTROL_PALETTE_NORMAL, "Palette:%u, expected:%u.", (::llc::u2_t)command.Style.Palette, (::llc::u2_t)::llc::GUI_CONTROL_PALETTE_NORMAL);
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_EVENT_AREA, command.Style.Area != ::llc::UI_CONTROL_AREA_CLIENT, "Area:%u, expected:%u.", (::llc::u2_t)command.Style.Area, (::llc::u2_t)::llc::UI_CONTROL_AREA_CLIENT);
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_EVENT_COLOR_MODE, command.Style.ColorMode != ::llc::GUI_COLOR_MODE_Flat, "Color mode:%u, expected:%u.", (::llc::u2_t)command.Style.ColorMode, (::llc::u2_t)::llc::GUI_COLOR_MODE_Flat);
	LLC_TEST_CHECK(errors, GUI_RENDER_TEST_RESULT_EVENT_DIRECT, false == command.Style.DirectPalette);
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_EVENT_RECT_OFFSET, command.Rectangle.Offset != (::llc::n2s1_t{2, 2}), "Offset:{%i, %i}, expected:{2, 2}.", command.Rectangle.Offset.x, command.Rectangle.Offset.y);
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_EVENT_RECT_SIZE, command.Rectangle.Size != (::llc::n2s1_t{3, 3}), "Size:{%i, %i}, expected:{3, 3}.", command.Rectangle.Size.x, command.Rectangle.Size.y);
	target.fill(clearColor);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_DRAW_RESULT, 0 > ::llc::guiDraw(events, resources, target), "%s", "Direct-palette event interpretation failed.");
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_DIRECT_PIXEL, LLC_CXS("client"), target[3][3], clientColor, 3, 3));
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_DIRECT_PIXEL, LLC_CXS("left"), target[3][1], leftColor, 1, 3));
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_DIRECT_PIXEL, LLC_CXS("top"), target[1][3], topColor, 3, 1));
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_DIRECT_PIXEL, LLC_CXS("right"), target[3][5], rightColor, 5, 3));
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_DIRECT_PIXEL, LLC_CXS("bottom"), target[5][3], bottomColor, 3, 5));
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_OUTSIDE_PIXEL, LLC_CXS("outside top left"), target[0][0], clearColor, 0, 0));
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_OUTSIDE_PIXEL, LLC_CXS("outside right"), target[1][6], clearColor, 6, 1));
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_OUTSIDE_PIXEL, LLC_CXS("outside bottom"), target[6][1], clearColor, 1, 6));
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_OUTSIDE_PIXEL, LLC_CXS("outside bottom right"), target[6][6], clearColor, 6, 6));
	draw.NoBorder = true;
	target.fill(clearColor);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_DRAW_RESULT, 0 > ::llc::guiRenderEvents(gui, events, target.metrics().u1()), "%s", "Border-disabled event generation failed.");
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_EVENT_COUNT, events.size() != 2, "Border-disabled command count:%u, expected:2.", events.size());
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_EVENT_RECTANGLE, events[0].Type != ::llc::GUI_RENDER_COMMAND_Rectangle, "Border-disabled command[0]:%u, expected rectangle:%u.", (::llc::u2_t)events[0].Type, (::llc::u2_t)::llc::GUI_RENDER_COMMAND_Rectangle);
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_EVENT_IMAGE, events[1].Type != ::llc::GUI_RENDER_COMMAND_Image, "Border-disabled command[1]:%u, expected image:%u.", (::llc::u2_t)events[1].Type, (::llc::u2_t)::llc::GUI_RENDER_COMMAND_Image);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_DRAW_RESULT, 0 > ::llc::guiDraw(events, resources, target), "%s", "Border-disabled event interpretation failed.");
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_NO_BORDER_PIXEL, LLC_CXS("no border"), target[3][1], clearColor, 1, 3));
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_NO_BORDER_PIXEL, LLC_CXS("client with no border"), target[3][3], clientColor, 3, 3));
	draw.NoBorder = false;
	draw.NoClient = true;
	target.fill(clearColor);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_DRAW_RESULT, 0 > ::llc::guiRenderEvents(gui, events, target.metrics().u1()), "%s", "Client-disabled event generation failed.");
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_EVENT_COUNT, events.size() != 13, "Client-disabled command count:%u, expected:13.", events.size());
	for(::llc::u2_t iEvent = 0; iEvent < 4; ++iEvent) LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_EVENT_RECTANGLE, events[iEvent].Type != ::llc::GUI_RENDER_COMMAND_Rectangle, "Client-disabled command[%u]:%u, expected rectangle:%u.", iEvent, (::llc::u2_t)events[iEvent].Type, (::llc::u2_t)::llc::GUI_RENDER_COMMAND_Rectangle);
	for(::llc::u2_t iEvent = 4; iEvent < 12; ++iEvent) LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_EVENT_TRIANGLE, events[iEvent].Type != ::llc::GUI_RENDER_COMMAND_Triangle, "Client-disabled command[%u]:%u, expected triangle:%u.", iEvent, (::llc::u2_t)events[iEvent].Type, (::llc::u2_t)::llc::GUI_RENDER_COMMAND_Triangle);
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_EVENT_IMAGE, events[12].Type != ::llc::GUI_RENDER_COMMAND_Image, "Client-disabled command[12]:%u, expected image:%u.", (::llc::u2_t)events[12].Type, (::llc::u2_t)::llc::GUI_RENDER_COMMAND_Image);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_DRAW_RESULT, 0 > ::llc::guiDraw(events, resources, target), "%s", "Client-disabled event interpretation failed.");
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_NO_CLIENT_PIXEL, LLC_CXS("no client"), target[3][3], clearColor, 3, 3));
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_NO_CLIENT_PIXEL, LLC_CXS("border with no client"), target[3][1], leftColor, 1, 3));
	rtrn 0;
}

sttc ::llc::err_t testGUIRenderHierarchy(ATestError & errors) {
	cnst ::llc::bgra clearColor = {1, 2, 3, 255}, parentColor = {11, 12, 13, 255}, childColor = {21, 22, 23, 255}, parentDisabled = {31, 32, 33, 255}, childDisabled = {41, 42, 43, 255};
	::llc::SGUI gui = {};
	cnst ::llc::cid_t idParent = ::llc::controlCreate(gui);
	cnst ::llc::cid_t idChild = ::llc::controlCreateChild(gui, idParent);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_CREATE_RESULT, idParent < 0 || idChild < 0, "Parent:%i, child:%i.", idParent, idChild);
	gui.Controls.Placement[idParent].Area = {{}, {5, 5}};
	gui.Controls.Placement[idChild].Area = {{1, 1}, {2, 2}};
	for(cnst ::llc::cid_t idControl : {idParent, idChild}) {
		gui.Controls.Placement[idControl].Margin = {};
		gui.Controls.Placement[idControl].Border = {};
		gui.Controls.Draw[idControl].UseNewPalettes = true;
		gui.Controls.Draw[idControl].ColorMode = ::llc::GUI_COLOR_MODE_Flat;
		gui.Controls.Draw[idControl].NoBorder = true;
	}
	gui.Controls.Draw[idParent].Palettes[::llc::GUI_CONTROL_PALETTE_NORMAL] = 0;
	gui.Controls.Draw[idChild].Palettes[::llc::GUI_CONTROL_PALETTE_NORMAL] = 1;
	gui.Controls.Draw[idParent].Palettes[::llc::GUI_CONTROL_PALETTE_DISABLED] = 2;
	gui.Controls.Draw[idChild].Palettes[::llc::GUI_CONTROL_PALETTE_DISABLED] = 3;
	::llc::SGUIRenderResources resources = {};
	if_fail_fe(::testGUIRenderResourcesCreate(errors, resources, 4));
	resources.Colors->Palettes[0][::llc::UI_CONTROL_AREA_CLIENT] = parentColor;
	resources.Colors->Palettes[1][::llc::UI_CONTROL_AREA_CLIENT] = childColor;
	resources.Colors->Palettes[2][::llc::UI_CONTROL_AREA_CLIENT] = parentDisabled;
	resources.Colors->Palettes[3][::llc::UI_CONTROL_AREA_CLIENT] = childDisabled;
	::llc::bgra pixels[5][5] = {};
	::llc::g8bgra target = pixels;
	target.fill(clearColor);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_DRAW_RESULT, 0 > ::llc::guiDraw(gui, resources, target), "%s", "Hierarchy draw failed.");
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_HIERARCHY_PIXEL, LLC_CXS("parent"), target[0][0], parentColor, 0, 0));
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_HIERARCHY_PIXEL, LLC_CXS("child"), target[1][1], childColor, 1, 1));
	gui.Controls.SetHidden(idChild, true);
	target.fill(clearColor);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_DRAW_RESULT, 0 > ::llc::guiDraw(gui, resources, target), "%s", "Hidden-child draw failed.");
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_HIDDEN_PIXEL, LLC_CXS("hidden child"), target[1][1], parentColor, 1, 1));
	gui.Controls.SetHidden(idChild, false);
	gui.Controls.SetDisabled(idParent, true);
	target.fill(clearColor);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_DRAW_RESULT, 0 > ::llc::guiDraw(gui, resources, target), "%s", "Disabled-hierarchy draw failed.");
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_DISABLED_PIXEL, LLC_CXS("disabled parent"), target[0][0], parentDisabled, 0, 0));
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_DISABLED_PIXEL, LLC_CXS("inherited disabled child"), target[1][1], childDisabled, 1, 1));
	rtrn 0;
}

sttc ::llc::err_t testGUIRenderLegacy(ATestError & errors) {
	cnst ::llc::bgra clearColor = {1, 2, 3, 255}, clientColor = {11, 12, 13, 255};
	::llc::SGUI gui = {};
	cnst ::llc::cid_t idControl = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_CREATE_RESULT, idControl < 0, "Control creation result:%i.", idControl);
	gui.Controls.Placement[idControl].Area = {{}, {2, 2}};
	gui.Controls.Placement[idControl].Margin = {};
	gui.Controls.Placement[idControl].Border = {};
	gui.Controls.Draw[idControl].NoBorder = true;
	gui.Controls.Draw[idControl].ColorMode = ::llc::GUI_COLOR_MODE_Flat;
	::llc::SGUIRenderResources resources = {};
	if_fail_fe(::testGUIRenderResourcesCreate(errors, resources, 0));
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_RESOURCE_CREATE, 0 == resources.Colors->Palette.create(), "%s", "Legacy palette allocation failed.");
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_RESOURCE_CREATE, 0 == resources.Colors->ControlThemes.create(), "%s", "Legacy theme allocation failed.");
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_RESOURCE_CREATE, 0 > resources.Colors->Palette->push_back(clientColor), "%s", "Legacy color insertion failed.");
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_RESOURCE_CREATE, 0 > resources.Colors->ControlThemes->push_back({}), "%s", "Legacy theme insertion failed.");
	::llc::bgra pixels[2][2] = {};
	::llc::g8bgra target = pixels;
	target.fill(clearColor);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_DRAW_RESULT, 0 > ::llc::guiDraw(gui, resources, target), "%s", "Legacy-palette draw failed.");
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_LEGACY_PIXEL, LLC_CXS("legacy client"), target[0][0], clientColor, 0, 0));
	rtrn 0;
}

sttc ::llc::err_t testGUIRenderImage(ATestError & errors) {
	cnst ::llc::bgra clearColor = {1, 2, 3, 255}, topLeft = {11, 12, 13, 255}, topRight = {21, 22, 23, 255}, bottomLeft = {31, 32, 33, 255}, bottomRight = {41, 42, 43, 255};
	::llc::SGUI gui = {};
	cnst ::llc::cid_t idControl = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_CREATE_RESULT, idControl < 0, "Control creation result:%i.", idControl);
	gui.Controls.Placement[idControl].Area = {{1, 1}, {2, 2}};
	gui.Controls.Placement[idControl].Margin = {};
	gui.Controls.Placement[idControl].Border = {};
	gui.Controls.Draw[idControl].UseNewPalettes = true;
	gui.Controls.Draw[idControl].ColorMode = ::llc::GUI_COLOR_MODE_Flat;
	gui.Controls.Draw[idControl].NoBorder = true;
	gui.Controls.Draw[idControl].NoClient = true;
	::llc::SGUIRenderResources resources = {};
	if_fail_fe(::testGUIRenderResourcesCreate(errors, resources, ::llc::GUI_CONTROL_PALETTE_COUNT));
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_SYNC_RESULT, 0 > ::llc::guiRenderResourcesSync(resources, gui), "%s", "Image-resource sync failed.");
	::llc::bgra imagePixels[2][2] = {{topLeft, topRight}, {bottomLeft, bottomRight}};
	resources.Images[idControl].Image = imagePixels;
	resources.Images[idControl].ImageAlign = ::llc::ALIGN_TOP_LEFT;
	::llc::bgra pixels[4][4] = {};
	::llc::g8bgra target = pixels;
	target.fill(clearColor);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_DRAW_RESULT, 0 > ::llc::guiDraw(gui, resources, target), "%s", "Image draw failed.");
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_IMAGE_PIXEL, LLC_CXS("image top left"), target[1][1], topLeft, 1, 1));
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_IMAGE_PIXEL, LLC_CXS("image top right"), target[1][2], topRight, 2, 1));
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_IMAGE_PIXEL, LLC_CXS("image bottom left"), target[2][1], bottomLeft, 1, 2));
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_IMAGE_PIXEL, LLC_CXS("image bottom right"), target[2][2], bottomRight, 2, 2));
	resources.Images[idControl].ImageInvertY = true;
	target.fill(clearColor);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_DRAW_RESULT, 0 > ::llc::guiDraw(gui, resources, target), "%s", "Vertically inverted image draw failed.");
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_IMAGE_INVERT_PIXEL, LLC_CXS("inverted top left"), target[1][1], bottomLeft, 1, 1));
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_IMAGE_INVERT_PIXEL, LLC_CXS("inverted bottom right"), target[2][2], topRight, 2, 2));
	rtrn 0;
}

sttc ::llc::err_t testGUIRenderText(ATestError & errors) {
	cnst ::llc::bgra clearColor = {1, 2, 3, 255}, textColor = {11, 12, 13, 255};
	::llc::SGUI gui = {};
	cnst ::llc::cid_t idControl = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_CREATE_RESULT, idControl < 0, "Control creation result:%i.", idControl);
	gui.Controls.Placement[idControl].Area = {{1, 1}, {1, 1}};
	gui.Controls.Placement[idControl].Margin = {};
	gui.Controls.Placement[idControl].Border = {};
	gui.Controls.Draw[idControl].UseNewPalettes = true;
	gui.Controls.Draw[idControl].ColorMode = ::llc::GUI_COLOR_MODE_Flat;
	gui.Controls.Draw[idControl].NoBorder = true;
	gui.Controls.Draw[idControl].NoClient = true;
	gui.SelectedFont = 0;
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_RESOURCE_CREATE, 0 > ::llc::controlTextSet(gui, idControl, LLC_CXS("A")), "%s", "Control text assignment failed.");
	::llc::SGUIRenderResources resources = {};
	if_fail_fe(::testGUIRenderResourcesCreate(errors, resources, ::llc::GUI_CONTROL_PALETTE_COUNT));
	resources.Colors->Palettes[::llc::GUI_CONTROL_PALETTE_NORMAL][::llc::UI_CONTROL_AREA_TEXT_FACE] = textColor;
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_RESOURCE_CREATE, 0 > resources.Fonts.resize(1), "%s", "Font-resource resize failed.");
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_RESOURCE_CREATE, 0 == resources.Fonts[0].create(), "%s", "Font allocation failed.");
	resources.Fonts[0]->CharSize = {1, 1};
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_RESOURCE_CREATE, 0 > resources.Fonts[0]->Texture.resize(1, 256), "%s", "Font texture resize failed.");
	resources.Fonts[0]->Texture.View['A'] = true;
	::llc::bgra pixels[3][3] = {};
	::llc::g8bgra target = pixels;
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_SYNC_RESULT, 0 > ::llc::guiRenderResourcesSync(resources, gui), "%s", "Text resource sync failed.");
	cnst ::llc::FGUITextMeasure textMeasure = [&resources](::llc::s1_t font, ::llc::vcst_t text, ::llc::n2u1_t & size) { rtrn ::llc::guiMeasureTextRaster(resources, font, text, size); };
	::llc::TGUIRenderEventQueue events = {};
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_DRAW_RESULT, 0 > ::llc::guiRenderEvents(gui, events, target.metrics().u1(), textMeasure), "%s", "Text event generation failed.");
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_EVENT_COUNT, events.size() != 2, "Text command count:%u, expected:2.", events.size());
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_EVENT_IMAGE, events[0].Type != ::llc::GUI_RENDER_COMMAND_Image, "Text command[0]:%u, expected image:%u.", (::llc::u2_t)events[0].Type, (::llc::u2_t)::llc::GUI_RENDER_COMMAND_Image);
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_EVENT_TEXT, events[1].Type != ::llc::GUI_RENDER_COMMAND_Text, "Text command[1]:%u, expected text:%u.", (::llc::u2_t)events[1].Type, (::llc::u2_t)::llc::GUI_RENDER_COMMAND_Text);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_RESOURCE_CREATE, 0 > ::llc::controlTextSet(gui, idControl, LLC_CXS("B")), "%s", "Source text mutation failed.");
	::llc::vcu0_t textInput = events[1].Data;
	::llc::SGUIRenderText textCommand = {};
	::llc::vcst_t eventText = {};
	cnst ::llc::err_t textCommandLoadResult = ::llc::loadPOD(textInput, textCommand);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_EVENT_PAYLOAD_LOAD, ::llc::failed(textCommandLoadResult), "Text command load result:%i.", textCommandLoadResult);
	cnst ::llc::err_t textLoadResult = ::llc::loadView(textInput, eventText);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_EVENT_PAYLOAD_LOAD, ::llc::failed(textLoadResult), "Text data load result:%i.", textLoadResult);
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_EVENT_PAYLOAD_LOAD, textInput.size(), "Text payload trailing bytes:%u, expected:0.", textInput.size());
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_EVENT_FONT, textCommand.Font != 0, "Text font:%i, expected:0.", textCommand.Font);
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_EVENT_TEXT_DATA, eventText != LLC_CXS("A"), "Event text:\"%.*s\", expected:\"A\".", LLC_VIEW_ARGS(eventText));
	target.fill(clearColor);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_DRAW_RESULT, 0 > ::llc::guiDraw(events, resources, target), "%s", "Text event interpretation failed.");
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_TEXT_PIXEL, LLC_CXS("text"), target[1][1], textColor, 1, 1));
	if_fail_fe(::testGUIRenderPixel(errors, GUI_RENDER_TEST_RESULT_OUTSIDE_PIXEL, LLC_CXS("text outside"), target[0][0], clearColor, 0, 0));
	rtrn 0;
}

::llc::err_t testGUIRender(ATestError & errors) {
	if_fail_fe(::testGUIRenderPaletteIndex(errors));
	if_fail_fe(::testGUIRenderDirect(errors));
	if_fail_fe(::testGUIRenderHierarchy(errors));
	if_fail_fe(::testGUIRenderLegacy(errors));
	if_fail_fe(::testGUIRenderImage(errors));
	if_fail_fe(::testGUIRenderText(errors));
	::llc::SGUI gui = {};
	cnst ::llc::cid_t idViewport = ::llc::controlCreate(gui);
	cnst ::llc::cid_t idDisposable = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_CREATE_RESULT, idViewport < 0, "Viewport creation result:%i.", idViewport);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_CREATE_RESULT, idDisposable < 0, "Disposable creation result:%i.", idDisposable);
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_GENERATION_INITIAL, gui.Controls.Entities[idViewport].Generation != 1, "Viewport generation:%u, expected:1.", gui.Controls.Entities[idViewport].Generation);
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_GENERATION_INITIAL, gui.Controls.Entities[idDisposable].Generation != 1, "Disposable generation:%u, expected:1.", gui.Controls.Entities[idDisposable].Generation);

	::llc::SGUIRenderResources resources = {};
	cnst ::llc::err_t syncResult = ::llc::guiRenderResourcesSync(resources, gui);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_SYNC_RESULT, ::llc::failed(syncResult), "Initial sync result:%i.", syncResult);
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_IMAGE_COUNT, resources.Images.size() != 2, "Image count:%u, expected:2.", resources.Images.size());
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_GENERATION_COUNT, resources.ImageGenerations.size() != 2, "Generation count:%u, expected:2.", resources.ImageGenerations.size());
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_GENERATION_VALUE, resources.ImageGenerations[idViewport] != 1, "Viewport image generation:%u, expected:1.", resources.ImageGenerations[idViewport]);
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_GENERATION_VALUE, resources.ImageGenerations[idDisposable] != 1, "Disposable image generation:%u, expected:1.", resources.ImageGenerations[idDisposable]);

	::llc::bgra viewportPixels[2][2] = {}, disposablePixels[1][1] = {};
	resources.Images[idViewport].Image = viewportPixels;
	resources.Images[idViewport].ImageOffset = {7, 11};
	resources.Images[idViewport].ImageInvertX = true;
	resources.Images[idDisposable].Image = disposablePixels;
	cnst ::llc::bgra * viewportPixelsBegin = resources.Images[idViewport].Image.begin();
	cnst ::llc::n2s1_t viewportOffset = {7, 11};
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_SYNC_RESULT, 0 > ::llc::guiRenderResourcesSync(resources, gui), "%s", "Unchanged sync failed.");
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_IMAGE_PRESERVE, resources.Images[idViewport].Image.begin() != viewportPixelsBegin, "Viewport image:%p, expected:%p.", resources.Images[idViewport].Image.begin(), viewportPixelsBegin);
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_IMAGE_PRESERVE, resources.Images[idViewport].ImageOffset != viewportOffset, "Viewport offset:{%i, %i}, expected:{7, 11}.", resources.Images[idViewport].ImageOffset.x, resources.Images[idViewport].ImageOffset.y);
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_IMAGE_PRESERVE, false == resources.Images[idViewport].ImageInvertX, "%s", "Viewport inversion was cleared.");

	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_DELETE_RESULT, 0 > ::llc::controlDelete(gui, idDisposable), "%s", "Disposable control deletion failed.");
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_SYNC_RESULT, 0 > ::llc::guiRenderResourcesSync(resources, gui), "%s", "Unused-slot sync failed.");
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_IMAGE_CLEAR_UNUSED, resources.Images[idDisposable].Image.size(), "Unused image size:%u, expected:0.", resources.Images[idDisposable].Image.size());
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_IMAGE_PRESERVE, resources.Images[idViewport].Image.begin() != viewportPixelsBegin, "Live viewport image:%p, expected:%p.", resources.Images[idViewport].Image.begin(), viewportPixelsBegin);

	cnst ::llc::u2_t formerGeneration = gui.Controls.Entities[idViewport].Generation;
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_DELETE_RESULT, 0 > ::llc::controlDelete(gui, idViewport), "%s", "Viewport control deletion failed.");
	cnst ::llc::cid_t idReplacement = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_CREATE_RESULT, idReplacement < 0, "Replacement creation result:%i.", idReplacement);
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_REUSE_ID, idReplacement != idViewport, "Replacement id:%i, expected:%i.", idReplacement, idViewport);
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_GENERATION_REUSE, gui.Controls.Entities[idReplacement].Generation == formerGeneration, "Replacement generation:%u, former:%u.", gui.Controls.Entities[idReplacement].Generation, formerGeneration);
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_IMAGE_CLEAR_REUSED, resources.Images[idReplacement].Image.begin() != viewportPixelsBegin, "Stale image changed before sync:%p, expected:%p.", resources.Images[idReplacement].Image.begin(), viewportPixelsBegin);
	LLC_TEST_REQUIREF(errors, GUI_RENDER_TEST_RESULT_SYNC_RESULT, 0 > ::llc::guiRenderResourcesSync(resources, gui), "%s", "Reused-slot sync failed.");
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_IMAGE_CLEAR_REUSED, resources.Images[idReplacement].Image.size(), "Replacement image size:%u, expected:0.", resources.Images[idReplacement].Image.size());
	LLC_TEST_CHECKF(errors, GUI_RENDER_TEST_RESULT_GENERATION_VALUE, resources.ImageGenerations[idReplacement] != gui.Controls.Entities[idReplacement].Generation, "Image generation:%u, control generation:%u.", resources.ImageGenerations[idReplacement], gui.Controls.Entities[idReplacement].Generation);
	rtrn 0;
}
