#include "llc_gui_render.h"

#include "llc_bitmap_target.h"
#include "llc_grid_copy.h"
#include "llc_gui_text.h"
#include "llc_rect_align.h"

sttc ::llc::err_t rectangleDraw(::llc::g8bgra target, cnst ::llc::rect2s1_t & rectangle, cnst ::llc::bgra & color) {
	cnst ::llc::s2_t xBegin = ::llc::max<::llc::s2_t>(0, rectangle.Offset.x);
	cnst ::llc::s2_t yBegin = ::llc::max<::llc::s2_t>(0, rectangle.Offset.y);
	cnst ::llc::s2_t xEnd = ::llc::min<::llc::s2_t>(target.metrics().x, ::llc::s2_t(rectangle.Offset.x) + rectangle.Size.x);
	cnst ::llc::s2_t yEnd = ::llc::min<::llc::s2_t>(target.metrics().y, ::llc::s2_t(rectangle.Offset.y) + rectangle.Size.y);
	for(::llc::s2_t y = yBegin; y < yEnd; ++y)
	for(::llc::s2_t x = xBegin; x < xEnd; ++x)
		target[y][x] = color;
	rtrn ::llc::max<::llc::s2_t>(0, xEnd - xBegin) * ::llc::max<::llc::s2_t>(0, yEnd - yBegin);
}

sttc ::llc::SGUIRenderStyle guiRenderStyle(cnst ::llc::SGUI & gui, ::llc::cid_t iControl, ::llc::UI_CONTROL_AREA area) {
	cnst ::llc::SControlDraw & draw = gui.Controls.Draw[iControl];
	cnst ::llc::SControlState & state = gui.Controls.States[iControl];
	cnst ::llc::GUI_CONTROL_PALETTE palette = ::llc::controlPaletteIndex(0 != ::llc::controlDisabled(gui, iControl), state);
	::llc::SGUIRenderStyle style = {};
	style.Control = iControl;
	style.Generation = gui.Controls.Entities[iControl].Generation;
	style.ColorSet = draw.UseNewPalettes ? draw.Palettes[palette] : draw.ColorTheme ? draw.ColorTheme - 1 : gui.ThemeDefault;
	style.Palette = palette;
	style.Area = area;
	style.ColorMode = (::llc::GUI_COLOR_MODE)(draw.ColorMode == ::llc::GUI_COLOR_MODE_Default ? gui.ColorModeDefault : draw.ColorMode);
	style.DirectPalette = draw.UseNewPalettes;
	style.Pressed = state.IsPressed();
	style.Hovered = state.IsHovered();
	rtrn style;
}

tplt<tpnm T>
sttc ::llc::err_t guiRenderEventPush(::llc::TGUIRenderEventQueue & output, ::llc::GUI_RENDER_COMMAND type, cnst T & command) {
	cnst ::llc::SEventGUIRender event = {type, ::llc::vcu0_t{(cnst ::llc::u0_t*)&command, szof(T)}};
	rtrn output.push_back(event);
}

sttc ::llc::err_t guiRenderTextPush(::llc::TGUIRenderEventQueue & output, cnst ::llc::SGUIRenderText & command, ::llc::vcst_t text) {
	::llc::SEventGUIRender event = {::llc::GUI_RENDER_COMMAND_Text};
	if_fail_fe(::llc::savePOD(event.Data, command));
	if_fail_fe(::llc::saveView(event.Data, text));
	rtrn output.push_back(event);
}

sttc ::llc::err_t controlRenderEvents(cnst ::llc::SGUI & gui, ::llc::cid_t iControl, ::llc::TGUIRenderEventQueue & output) {
	if_true_fef(::llc::controlInvalid(gui, iControl), "Invalid control id:%i.", iControl);
	if(::llc::controlHidden(gui, iControl))
		rtrn 0;
	cnst ::llc::SControlArea & area = gui.Controls.Area[iControl];
	cnst ::llc::SControlDraw & draw = gui.Controls.Draw[iControl];
	cnst ::llc::UI_CONTROL_AREA rectangleAreas[::llc::GUI_CONTROL_AREA_OLD_COUNT] = {::llc::UI_CONTROL_AREA_CLIENT, ::llc::UI_CONTROL_AREA_BORDER_LEFT, ::llc::UI_CONTROL_AREA_BORDER_TOP, ::llc::UI_CONTROL_AREA_BORDER_RIGHT, ::llc::UI_CONTROL_AREA_BORDER_BOTTOM};
	for(::llc::u2_t iRectangle = 0; iRectangle < ::llc::GUI_CONTROL_AREA_OLD_COUNT; ++iRectangle) {
		if((draw.NoBorder && iRectangle != ::llc::GUI_CONTROL_AREA_OLD_CLIENT) || (draw.NoClient && iRectangle == ::llc::GUI_CONTROL_AREA_OLD_CLIENT))
			continue;
		cnst ::llc::SGUIRenderRectangle command = {::guiRenderStyle(gui, iControl, rectangleAreas[iRectangle]), area.Rectangles[iRectangle]};
		if_fail_fe(::guiRenderEventPush(output, ::llc::GUI_RENDER_COMMAND_Rectangle, command));
	}
	cnst ::llc::UI_CONTROL_AREA triangleAreas[8] = {::llc::UI_CONTROL_AREA_BORDER_TOP, ::llc::UI_CONTROL_AREA_BORDER_LEFT, ::llc::UI_CONTROL_AREA_BORDER_TOP, ::llc::UI_CONTROL_AREA_BORDER_RIGHT, ::llc::UI_CONTROL_AREA_BORDER_LEFT, ::llc::UI_CONTROL_AREA_BORDER_BOTTOM, ::llc::UI_CONTROL_AREA_BORDER_RIGHT, ::llc::UI_CONTROL_AREA_BORDER_BOTTOM};
	if(false == draw.NoBorder && ::llc::controlNCSpacing(gui.Controls.Placement[iControl]).Area())
		for(::llc::u2_t iTriangle = 0; iTriangle < ::llc::size(area.Triangles); ++iTriangle) {
			cnst ::llc::SGUIRenderTriangle command = {::guiRenderStyle(gui, iControl, triangleAreas[iTriangle]), area.Triangles[iTriangle]};
			if_fail_fe(::guiRenderEventPush(output, ::llc::GUI_RENDER_COMMAND_Triangle, command));
		}
	cnst ::llc::SGUIRenderImage imageCommand = {iControl, gui.Controls.Entities[iControl].Generation, area.Client.Global, gui.Controls.States[iControl].IsHovered()};
	if_fail_fe(::guiRenderEventPush(output, ::llc::GUI_RENDER_COMMAND_Image, imageCommand));
	cnst ::llc::SControlText & text = gui.Controls.Text[iControl];
	if(text.Text.size()) {
		::llc::rect2s1_t textRectangle = area.Text;
		if(gui.Controls.States[iControl].IsPressed() || gui.Controls.States[iControl].IsHovered()) {
			textRectangle.Offset += {1, 1};
			textRectangle.Size -= {1, 1};
			textRectangle.Size.x = ::llc::max<::llc::s1_t>(0, textRectangle.Size.x);
			textRectangle.Size.y = ::llc::max<::llc::s1_t>(0, textRectangle.Size.y);
		}
		cnst ::llc::s1_t iFont = text.FontSelected >= 0 ? text.FontSelected : (::llc::s1_t)gui.SelectedFont;
		cnst ::llc::SGUIRenderText textCommand = {::guiRenderStyle(gui, iControl, ::llc::UI_CONTROL_AREA_TEXT_FACE), textRectangle, iFont};
		if_fail_fe(::guiRenderTextPush(output, textCommand, text.Text));
	}
	cnst ::llc::vcid children = gui.Controls.Children[iControl];
	for(::llc::u2_t iChild = 0; iChild < children.size(); ++iChild)
		if_fail_fe(::controlRenderEvents(gui, children[iChild], output));
	rtrn 0;
}

sttc ::llc::err_t guiRenderColorResolve(cnst ::llc::SGUIRenderResources & resources, cnst ::llc::SGUIRenderStyle & style, ::llc::bgra & color) {
	if_true_fef(0 == resources.Colors.get_ref(), "Control:%i has no GUI color resources.", style.Control);
	::llc::astatic<::llc::bgra, ::llc::UI_CONTROL_AREA_COUNT> colors = {};
	if(style.DirectPalette) {
		if_true_fef(style.ColorSet < 0 || (::llc::u2_t)style.ColorSet >= resources.Colors->Palettes.size(), "Control:%i palette:%i, palette count:%u.", style.Control, style.ColorSet, resources.Colors->Palettes.size());
		colors = resources.Colors->Palettes[style.ColorSet];
	}
	else {
		if_true_fef(0 == resources.Colors->ControlThemes.get_ref(), "Control:%i has no legacy themes.", style.Control);
		if_true_fef(0 == resources.Colors->Palette.get_ref(), "Control:%i has no legacy palette.", style.Control);
		if_true_fef(style.ColorSet < 0 || (::llc::u2_t)style.ColorSet >= resources.Colors->ControlThemes->size(), "Control:%i theme:%i, theme count:%u.", style.Control, style.ColorSet, resources.Colors->ControlThemes->size());
		cnst ::llc::astu32<::llc::UI_CONTROL_AREA_COUNT> & colorIndices = (*resources.Colors->ControlThemes)[style.ColorSet].ColorCombos[style.Palette];
		for(::llc::u2_t iColor = 0; iColor < colors.size(); ++iColor) {
			cnst ::llc::u2_t iPaletteColor = colorIndices[iColor];
			if_true_fef(iPaletteColor >= resources.Colors->Palette->size(), "Control:%i color:%u palette index:%u, palette size:%u.", style.Control, iColor, iPaletteColor, resources.Colors->Palette->size());
			colors[iColor] = (*resources.Colors->Palette)[iPaletteColor];
		}
	}
	if(style.ColorMode == ::llc::GUI_COLOR_MODE_3D || style.ColorMode == ::llc::GUI_COLOR_MODE_Default) {
		cnst double bright = style.Pressed ? 0.4 : style.Hovered ? 0.8 : 1.2;
		cnst double shaded = style.Pressed ? 1.5 : style.Hovered ? 1.2 : 0.8;
		cnst ::llc::bgra background = colors[::llc::UI_CONTROL_AREA_CLIENT];
		colors[::llc::UI_CONTROL_AREA_BORDER_LEFT] = background * bright;
		colors[::llc::UI_CONTROL_AREA_BORDER_TOP] = background * bright;
		colors[::llc::UI_CONTROL_AREA_BORDER_RIGHT] = background * shaded;
		colors[::llc::UI_CONTROL_AREA_BORDER_BOTTOM] = background * shaded;
	}
	color = colors[style.Area];
	rtrn 0;
}

sttc ::llc::err_t guiRenderImageDraw(cnst ::llc::SGUIRenderImage & command, ::llc::SGUIRenderResources & resources, ::llc::g8bgra target) {
	if(command.Control < 0 || (::llc::u2_t)command.Control >= resources.Images.size() || (::llc::u2_t)command.Control >= resources.ImageGenerations.size())
		rtrn 0;
	if(resources.ImageGenerations[command.Control] != command.Generation)
		rtrn 0;
	::llc::SControlImage & controlImage = resources.Images[command.Control];
	if(0 == controlImage.Image.size())
		rtrn 0;
	::llc::rect2s1_t imageRectangle = {{}, controlImage.Image.metrics().s1()};
	::llc::realignRectangle(command.Client.Size, imageRectangle, imageRectangle, (::llc::ALIGN)controlImage.ImageAlign);
	imageRectangle.Offset += command.Client.Offset;
	if(command.Hovered)
		imageRectangle.Offset += {1, 1};
	cnst ::llc::n2s1_t clippedOffset = {::llc::max(imageRectangle.Offset.x, command.Client.Offset.x), ::llc::max(imageRectangle.Offset.y, command.Client.Offset.y)};
	cnst ::llc::n2s1_t clippedLimit = {::llc::min(imageRectangle.Limit().x, command.Client.Limit().x), ::llc::min(imageRectangle.Limit().y, command.Client.Limit().y)};
	if(clippedOffset.x >= clippedLimit.x || clippedOffset.y >= clippedLimit.y)
		rtrn 0;
	cnst ::llc::rect2s1_t clippedRectangle = {clippedOffset, clippedLimit - clippedOffset};
	cnst ::llc::n2s1_t sourceOffset = controlImage.ImageOffset + clippedOffset - imageRectangle.Offset;
	if(false == controlImage.ImageInvertY)
		rtrn ::llc::grid_copy_blend(target, controlImage.Image, clippedRectangle, sourceOffset);
	if_fail_fe(controlImage.Temp.resize(controlImage.Image.metrics()));
	if_fail_fe(::llc::grid_mirror_y(controlImage.Temp.Color.View, controlImage.Image));
	rtrn ::llc::grid_copy_blend(target, controlImage.Temp.Color.View, clippedRectangle, sourceOffset);
}

sttc ::llc::err_t guiRenderTextDraw(cnst ::llc::SGUIRenderText & command, ::llc::vcst_t text, cnst ::llc::SGUIRenderResources & resources, ::llc::g8bgra target, cnst ::llc::bgra & color) {
	if_true_fef(command.Font < 0 || (::llc::u2_t)command.Font >= resources.Fonts.size(), "Control:%i font:%i, font count:%u.", command.Style.Control, command.Font, resources.Fonts.size());
	if_true_fef(0 == resources.Fonts[command.Font].get_ref(), "Control:%i font:%i is null.", command.Style.Control, command.Font);
	cnst ::llc::SRasterFont & font = *resources.Fonts[command.Font];
	if_true_fef(0 == font.CharSize.x || 0 == font.CharSize.y || font.Texture.Pitch < font.CharSize.x, "Control:%i font:%i has invalid metrics:{%u, %u}, pitch:%u.", command.Style.Control, command.Font, font.CharSize.x, font.CharSize.y, font.Texture.Pitch);
	::llc::apod<::llc::n2u1_t> coordinates = {};
	if_fail_fe(::llc::textLineRaster(target.metrics(), font.CharSize, command.Rectangle, font.Texture, text, coordinates));
	for(::llc::u2_t iCoordinate = 0; iCoordinate < coordinates.size(); ++iCoordinate)
		target[coordinates[iCoordinate].y][coordinates[iCoordinate].x] = color;
	rtrn 0;
}

sttc ::llc::err_t guiRenderEventDraw(cnst ::llc::SEventGUIRender & event, ::llc::SGUIRenderResources & resources, ::llc::g8bgra target) {
	::llc::vcu0_t input = event.Data;
	switch(event.Type) {
	case ::llc::GUI_RENDER_COMMAND_Rectangle: {
		::llc::SGUIRenderRectangle command = {};
		if_fail_fe(::llc::loadPOD(input, command));
		::llc::bgra color = {};
		if_fail_fe(::guiRenderColorResolve(resources, command.Style, color));
		::rectangleDraw(target, command.Rectangle, color);
		break;
	}
	case ::llc::GUI_RENDER_COMMAND_Triangle: {
		::llc::SGUIRenderTriangle command = {};
		if_fail_fe(::llc::loadPOD(input, command));
		::llc::bgra color = {};
		if_fail_fe(::guiRenderColorResolve(resources, command.Style, color));
		::llc::drawTriangle(target, color, command.Triangle);
		break;
	}
	case ::llc::GUI_RENDER_COMMAND_Image: {
		::llc::SGUIRenderImage command = {};
		if_fail_fe(::llc::loadPOD(input, command));
		if_fail_fe(::guiRenderImageDraw(command, resources, target));
		break;
	}
	case ::llc::GUI_RENDER_COMMAND_Text: {
		::llc::SGUIRenderText command = {};
		::llc::vcst_t text = {};
		if_fail_fe(::llc::loadPOD(input, command));
		if_fail_fe(::llc::loadView(input, text));
		::llc::bgra color = {};
		if_fail_fe(::guiRenderColorResolve(resources, command.Style, color));
		if_fail_fe(::guiRenderTextDraw(command, text, resources, target, color));
		break;
	}
	default:
		if_true_fef(true, "Unknown GUI render command:%u.", (::llc::u2_t)event.Type);
	}
	rtrn 0;
}

::llc::GUI_CONTROL_PALETTE llc::controlPaletteIndex(bool disabled, cnst ::llc::SControlState & controlState) nxpt {
	cnst bool highlighted = controlState.IsHovered() || controlState.IsFocused();
	if(controlState.IsSelected()) {
		if(disabled)                    rtrn ::llc::GUI_CONTROL_PALETTE_SELECTED_DISABLED;
		if(controlState.IsPressed())    rtrn ::llc::GUI_CONTROL_PALETTE_SELECTED_PRESSED;
		if(highlighted)                 rtrn ::llc::GUI_CONTROL_PALETTE_SELECTED_HOVER;
		rtrn ::llc::GUI_CONTROL_PALETTE_SELECTED;
	}
	if(disabled)                    rtrn ::llc::GUI_CONTROL_PALETTE_DISABLED;
	if(controlState.IsPressed())    rtrn ::llc::GUI_CONTROL_PALETTE_PRESSED;
	if(highlighted)                 rtrn ::llc::GUI_CONTROL_PALETTE_HOVER;
	rtrn ::llc::GUI_CONTROL_PALETTE_NORMAL;
}

::llc::error_t llc::guiRenderResourcesSync(::llc::SGUIRenderResources & resources, cnst ::llc::SGUI & gui) {
	cnst ::llc::u2_t controlCount = gui.Controls.Entities.size();
	if_fail_fe(::llc::resize(controlCount, resources.Images, resources.ImageGenerations));
	for(::llc::u2_t iControl = 0; iControl < controlCount; ++iControl) {
		cnst ::llc::u2_t generation = gui.Controls.Entities[iControl].Generation;
		if(gui.Controls.IsUnused((::llc::cid_t)iControl) || resources.ImageGenerations[iControl] != generation) {
			resources.Images[iControl] = {};
			resources.ImageGenerations[iControl] = generation;
		}
	}
	rtrn 0;
}

::llc::error_t llc::guiMeasureTextRaster(cnst ::llc::SGUIRenderResources & resources, ::llc::s1_t font, ::llc::vcst_t text, ::llc::n2u1_t & size) {
	if_true_fef(font < 0 || (::llc::u2_t)font >= resources.Fonts.size(), "Font:%i, font count:%u.", font, resources.Fonts.size());
	if_true_fef(0 == resources.Fonts[font].get_ref(), "Font:%i is null.", font);
	cnst ::llc::n2u0_t characterSize = resources.Fonts[font]->CharSize;
	cnst ::llc::u2_t textWidth = characterSize.x * text.size();
	if_true_fef(textWidth > 0xFFFFU, "Font:%i, text length:%u, character width:%u, measured width:%u.", font, text.size(), characterSize.x, textWidth);
	size = {(::llc::u1_t)textWidth, characterSize.y};
	rtrn 0;
}

::llc::error_t llc::guiRenderEvents(::llc::SGUI & gui, ::llc::TGUIRenderEventQueue & output, ::llc::n2u1_c & targetSize, cnst ::llc::FGUITextMeasure & textMeasure) {
	output.clear();
	cnst bool forceUpdate = gui.LastSize != targetSize || gui.LastZoom != gui.Zoom;
	if_fail_fe(::llc::guiUpdateMetrics(gui, targetSize, textMeasure, forceUpdate));
	for(::llc::u2_t iControl = 0; iControl < gui.Controls.Entities.size(); ++iControl) {
		cnst ::llc::cid_t idControl = (::llc::cid_t)iControl;
		if(::llc::controlInvalid(gui, idControl))
			continue;
		cnst ::llc::cid_t idParent = gui.Controls.Entities[iControl].Parent;
		if(idParent == ::llc::CID_INVALID || ::llc::controlInvalid(gui, idParent))
			if_fail_fe(::controlRenderEvents(gui, idControl, output));
	}
	rtrn 0;
}

::llc::error_t llc::guiRenderEvents(::llc::SGUI & gui, ::llc::TGUIRenderEventQueue & output, ::llc::n2u1_c & targetSize) {
	rtrn ::llc::guiRenderEvents(gui, output, targetSize, ::llc::FGUITextMeasure{});
}

::llc::error_t llc::guiDraw(cnst ::llc::TGUIRenderEventQueue & events, ::llc::SGUIRenderResources & resources, ::llc::g8bgra target) {
	for(::llc::u2_t iEvent = 0; iEvent < events.size(); ++iEvent)
		if_fail_fe(::guiRenderEventDraw(events[iEvent], resources, target));
	rtrn 0;
}

::llc::error_t llc::guiDraw(::llc::SGUI & gui, ::llc::SGUIRenderResources & resources, ::llc::g8bgra target) {
	if_fail_fe(::llc::guiRenderResourcesSync(resources, gui));
	::llc::TGUIRenderEventQueue events = {};
	cnst ::llc::FGUITextMeasure textMeasure = [&resources](::llc::s1_t font, ::llc::vcst_t text, ::llc::n2u1_t & size) { rtrn ::llc::guiMeasureTextRaster(resources, font, text, size); };
	if_fail_fe(::llc::guiRenderEvents(gui, events, target.metrics().u1(), textMeasure));
	rtrn ::llc::guiDraw(events, resources, target);
}
