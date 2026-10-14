#include "llc_gui.h"

#include "llc_grid_copy.h"
#include "llc_bitmap_target.h"
#include "llc_gui_text.h"
#include "llc_rect_align.h"
#include "llc_view_color.h"

tplt<tpnm _tCoord, tpnm _tColor>
static ::llc::error_t drawRectangle(::llc::grid<_tColor> & target, cnst _tColor & color, cnst ::llc::rect2<_tCoord> & rectangle) {
	cnst ::llc::s2_t yStart = ::llc::max<::llc::s2_t>(0, rectangle.Offset.y);
	cnst ::llc::s2_t yStop  = ::llc::min<::llc::s2_t>(target.metrics().y, (::llc::s2_t)rectangle.Offset.y + rectangle.Size.y);
	cnst ::llc::s2_t xStart = ::llc::max<::llc::s2_t>(0, rectangle.Offset.x);
	cnst ::llc::s2_t xStop  = ::llc::min<::llc::s2_t>(target.metrics().x, (::llc::s2_t)rectangle.Offset.x + rectangle.Size.x);
	for(::llc::s2_t y = yStart; y < yStop; ++y)
		for(::llc::s2_t x = xStart; x < xStop; ++x)
			target[y][x] = color;
	rtrn (yStop - yStart) * (xStop - xStart);
}

static	::llc::error_t	controlTextDraw		(::llc::SGUI & gui, int32_t iControl, ::llc::g8bgra & target, bool bDisabled)				{
	::llc::SControlState		& controlState		= gui.Controls.States	[iControl];
	::llc::SControlDraw			& controlDraw		= gui.Controls.Draw	[iControl];
	::llc::bgra					colorFace			= {0xFF, 0x00, 0xFF, 0xFF};
	if(0 == gui.Controls.Draw[iControl].UseNewPalettes) {
		const ::llc::SControlTheme	& theme				= (*gui.Colors->ControlThemes)[(0 == controlDraw.ColorTheme) ? gui.ThemeDefault : controlDraw.ColorTheme - 1];
		const ::llc::astu32<::llc::UI_CONTROL_AREA_COUNT>	
									& colorCombo		= theme.ColorCombos
			[ bDisabled					? ::llc::GUI_CONTROL_PALETTE_DISABLED
			: controlState.IsPressed () ? ::llc::GUI_CONTROL_PALETTE_PRESSED
			: controlState.IsSelected() ? ::llc::GUI_CONTROL_PALETTE_SELECTED
			: controlState.IsHovered () ? ::llc::GUI_CONTROL_PALETTE_HOVER
			: ::llc::GUI_CONTROL_PALETTE_NORMAL
			];
		colorFace				= (*gui.Colors->Palette)[colorCombo[::llc::UI_CONTROL_AREA_TEXT_FACE]];
		//const ::llc::bgra			colorBack			= gui.Palette[colorCombo[::llc::UI_CONTROL_AREA_TEXT_BACKGROUND]];
	}
	else {
		const ::llc::astatic<::llc::bgra, ::llc::UI_CONTROL_AREA_COUNT>	& colorCombo											=
			gui.Colors->Palettes[controlDraw.Palettes
				[ bDisabled					? ::llc::GUI_CONTROL_PALETTE_DISABLED
				: controlState.IsPressed () ? ::llc::GUI_CONTROL_PALETTE_PRESSED
				: controlState.IsSelected() ? ::llc::GUI_CONTROL_PALETTE_SELECTED
				: controlState.IsHovered () ? ::llc::GUI_CONTROL_PALETTE_HOVER
				: ::llc::GUI_CONTROL_PALETTE_NORMAL
				]]
				;
		colorFace				= colorCombo[::llc::UI_CONTROL_AREA_TEXT_FACE];
	}

	::llc::SControlArea		& controlMetrics	= gui.Controls.Area[iControl];
	::llc::rect2s1_t				rectText			= controlMetrics.Text;
	if(controlState.IsPressed() || controlState.IsHovered()) { // shift text offset by one pixel to give the effect of being pushed.
		rectText.Offset			+= ::llc::n2s1_t{1, 1};
		rectText.Size			-= ::llc::n2s1_t{1, 1};
		if(rectText.Size.x < 0) rectText.Size.x = 0;
		if(rectText.Size.y < 0) rectText.Size.y = 0;
	} // This isn't done in updateMetrics() because the function is only executed for outdated metrics.
	// Changhing the state from idle to hover however doesn't cause the control metrics to become outdated (and in general it's pointless for other than the effect we're applying here).

	::llc::SControlText			& controlText		= gui.Controls.Text		[iControl];
	::llc::apod<::llc::n2u1_t>	dstCoords;
	const uint32_t				iFont				= ::llc::in_range(controlText.FontSelected, (int16_t)0, (int16_t)gui.Fonts.size()) ? controlText.FontSelected : gui.SelectedFont;
	const ::llc::SRasterFont	& selectedFont		= *gui.Fonts[iFont];
	llc_necs(llc::textLineRaster(target.metrics(), selectedFont.CharSize, rectText, selectedFont.Texture, controlText.Text, dstCoords));
	for(uint32_t iCoord = 0; iCoord < dstCoords.size(); ++iCoord) {
		const ::llc::n2u1_t			dstCoord			= dstCoords[iCoord];
		target[dstCoord.y][dstCoord.x]	= colorFace;
		//::llc::drawPixelLight(target, dstCoords[iCoord], colorFace, controlState.Pressed ? 0.75f : 0.5f, controlState.Pressed ? 1.0f : 0.95);
	}
	return 0;
}
static	::llc::GUI_CONTROL_PALETTE	paletteIndexFromState	(bool disabled, const ::llc::SControlState & controlState)					{
	return disabled					? ::llc::GUI_CONTROL_PALETTE_DISABLED
		: controlState.IsPressed () ? ::llc::GUI_CONTROL_PALETTE_PRESSED
		: controlState.IsSelected() ? ::llc::GUI_CONTROL_PALETTE_SELECTED
		: controlState.IsHovered () ? ::llc::GUI_CONTROL_PALETTE_HOVER
		: ::llc::GUI_CONTROL_PALETTE_NORMAL
		;
}
static	::llc::error_t	fillColorTableBorders3D			(const ::llc::SControlState & controlState, ::llc::view<::llc::bgra> & colors)					{
	double						bright;
	double						shaded;
	bright					= controlState.IsPressed() ? 0.4 : controlState.IsHovered() ? 0.8 : 1.2;
	shaded					= controlState.IsPressed() ? 1.5 : controlState.IsHovered() ? 1.2 : 0.8;
	::llc::bgra					colorBackground											= colors[::llc::GUI_CONTROL_AREA_OLD_CLIENT];
	colors[::llc::GUI_CONTROL_AREA_OLD_BORDER_LEFT		]	= colorBackground * bright;
	colors[::llc::GUI_CONTROL_AREA_OLD_BORDER_TOP		]	= colorBackground * bright;
	colors[::llc::GUI_CONTROL_AREA_OLD_BORDER_RIGHT		]	= colorBackground * shaded;
	colors[::llc::GUI_CONTROL_AREA_OLD_BORDER_BOTTOM	]	= colorBackground * shaded;
	return 0;					  
}
static	::llc::error_t	fillColorTableNew				(::llc::SGUI & gui, ::llc::GUI_COLOR_MODE colorMode, const ::llc::SControlState & controlState, const ::llc::SControlDraw & controlDraw, bool disabled, ::llc::v8bgra & colors)					{
	const ::llc::astatic<::llc::bgra, ::llc::UI_CONTROL_AREA_COUNT>
								& colorCombo					= gui.Colors->Palettes[controlDraw.Palettes[::paletteIndexFromState(disabled, controlState)]];
	//colors[::llc::GUI_CONTROL_AREA_OLD_BACKGROUND	]	= colorCombo[::llc::UI_CONTROL_AREA_BACKGROUND	];
	colors[::llc::GUI_CONTROL_AREA_OLD_CLIENT		]	= colorCombo[::llc::UI_CONTROL_AREA_CLIENT		];
	if(colorMode == ::llc::GUI_COLOR_MODE_3D || colorMode == ::llc::GUI_COLOR_MODE_Default)
		return ::fillColorTableBorders3D(controlState, colors);

	colors[::llc::GUI_CONTROL_AREA_OLD_BORDER_LEFT	]	= colorCombo[::llc::UI_CONTROL_AREA_BORDER_LEFT	];
	colors[::llc::GUI_CONTROL_AREA_OLD_BORDER_TOP	]	= colorCombo[::llc::UI_CONTROL_AREA_BORDER_TOP	];
	colors[::llc::GUI_CONTROL_AREA_OLD_BORDER_RIGHT	]	= colorCombo[::llc::UI_CONTROL_AREA_BORDER_RIGHT	];
	colors[::llc::GUI_CONTROL_AREA_OLD_BORDER_BOTTOM]	= colorCombo[::llc::UI_CONTROL_AREA_BORDER_BOTTOM	];
	return 0;
}
static	::llc::error_t	fillColorTableOld				(::llc::SGUI & gui, ::llc::GUI_COLOR_MODE colorMode, const ::llc::SControlState & controlState, const ::llc::SControlDraw & controlDraw, bool disabled, ::llc::v8bgra & colors)					{
	const ::llc::SControlTheme	& theme							= (*gui.Colors->ControlThemes)[(0 == controlDraw.ColorTheme) ? gui.ThemeDefault : controlDraw.ColorTheme - 1];
	const ::llc::astu32<::llc::UI_CONTROL_AREA_COUNT>	
								& colorCombo					= theme.ColorCombos[::paletteIndexFromState(disabled, controlState)];
	//colors[::llc::GUI_CONTROL_AREA_OLD_BACKGROUND	]	= (*gui.Colors->Palette)[colorCombo[::llc::UI_CONTROL_AREA_BACKGROUND	]];
	colors[::llc::GUI_CONTROL_AREA_OLD_CLIENT		]	= (*gui.Colors->Palette)[colorCombo[::llc::UI_CONTROL_AREA_CLIENT		]];
	if(colorMode == ::llc::GUI_COLOR_MODE_3D || colorMode == ::llc::GUI_COLOR_MODE_Default)
		return ::fillColorTableBorders3D(controlState, colors);

	colors[::llc::GUI_CONTROL_AREA_OLD_BORDER_LEFT	]	= (*gui.Colors->Palette)[colorCombo[::llc::UI_CONTROL_AREA_BORDER_LEFT	]];
	colors[::llc::GUI_CONTROL_AREA_OLD_BORDER_TOP	]	= (*gui.Colors->Palette)[colorCombo[::llc::UI_CONTROL_AREA_BORDER_TOP	]];
	colors[::llc::GUI_CONTROL_AREA_OLD_BORDER_RIGHT	]	= (*gui.Colors->Palette)[colorCombo[::llc::UI_CONTROL_AREA_BORDER_RIGHT	]];
	colors[::llc::GUI_CONTROL_AREA_OLD_BORDER_BOTTOM]	= (*gui.Colors->Palette)[colorCombo[::llc::UI_CONTROL_AREA_BORDER_BOTTOM]];
	return 0;
}
static	::llc::error_t	fillColorTable					(::llc::SGUI & gui, ::llc::cid_t iControl, bool disabled, ::llc::view<::llc::bgra> colors)					{
	const ::llc::SControlDraw	& draw							= gui.Controls.Draw		[iControl];
	const ::llc::SControlState	& state							= gui.Controls.States	[iControl];
	::llc::GUI_COLOR_MODE		colorMode						= (::llc::GUI_COLOR_MODE)((draw.ColorMode == ::llc::GUI_COLOR_MODE_Default) ? gui.ColorModeDefault : draw.ColorMode);
	llc_necs((false == draw.UseNewPalettes) 
		? fillColorTableOld(gui, colorMode, state, draw, disabled, colors) 
		: fillColorTableNew(gui, colorMode, state, draw, disabled, colors)
		);

	return 0;
}
static	::llc::error_t	actualControlDraw				(::llc::SGUI & gui, ::llc::cid_t iControl, ::llc::g8bgra & target)					{
	::llc::bgra					colors[::llc::GUI_CONTROL_AREA_OLD_COUNT]	= {}; // -- Fill color table
	const bool					disabled						= ::llc::controlDisabled(gui, iControl);
	const ::llc::SControlArea	& controlMetrics				= gui.Controls.Area[iControl];
	const ::llc::SControlDraw	& controlDraw					= gui.Controls.Draw  [iControl];
	{
		::fillColorTable(gui, iControl, disabled, colors);

		for(uint32_t iElement = 0; iElement < ::llc::GUI_CONTROL_AREA_OLD_COUNT; ++iElement) {
			if( (controlDraw.NoBorder && iElement != ::llc::GUI_CONTROL_AREA_OLD_CLIENT) // (iElement >= ::llc::GUI_CONTROL_AREA_OLD_BORDER_LEFT || iElement <= ::llc::GUI_CONTROL_AREA_OLD_BORDER_BOTTOM))
			 || (controlDraw.NoClient && iElement == ::llc::GUI_CONTROL_AREA_OLD_CLIENT) //(iElement == ::llc::GUI_CONTROL_AREA_OLD_BACKGROUND || iElement == ::llc::GUI_CONTROL_AREA_OLD_CLIENT))
			)
				continue;
			::drawRectangle(target, colors[iElement], controlMetrics.Rectangles[iElement]);
		}
	}
	int32_t						colorIndices [8]				=
		{ ::llc::GUI_CONTROL_AREA_OLD_BORDER_TOP
		, ::llc::GUI_CONTROL_AREA_OLD_BORDER_LEFT
		, ::llc::GUI_CONTROL_AREA_OLD_BORDER_TOP
		, ::llc::GUI_CONTROL_AREA_OLD_BORDER_RIGHT
		, ::llc::GUI_CONTROL_AREA_OLD_BORDER_LEFT
		, ::llc::GUI_CONTROL_AREA_OLD_BORDER_BOTTOM
		, ::llc::GUI_CONTROL_AREA_OLD_BORDER_RIGHT
		, ::llc::GUI_CONTROL_AREA_OLD_BORDER_BOTTOM
		};
	if(false == controlDraw.NoBorder && ::llc::controlNCSpacing(gui.Controls.Placement[iControl]).Area()) {
		for(uint32_t iTri = 0; iTri < 8; ++iTri)
			::llc::drawTriangle(target, colors[colorIndices[iTri]], controlMetrics.Triangles[iTri]);
	}

	const ::llc::SControlImage	& controlImage			= gui.Controls.Images[iControl];
	if(controlImage.Image.metrics().x && controlImage.Image.metrics().y) {
		const ::llc::SControlState	& controlState			= gui.Controls.States[iControl];
		//::llc::rect2<int32_t>	rectImage						=
		//	{ controlMetrics.Client.Global.Offset
		//	, controlMetrics.Client.Global.Size
		//		+ ::llc::n2<int32_t>
		//			{ ::llc::min(0, ::llc::min(controlMetrics.Client.Global.Offset.x, controlMetrics.Client.Local.Offset.x))
		//			, ::llc::min(0, ::llc::min(controlMetrics.Client.Global.Offset.y, controlMetrics.Client.Local.Offset.y))
		//			}
		//	};
		::llc::rect2s1_t				rectImage				= {{}, controlImage.Image.metrics().s1()};
		::llc::realignRectangle(controlMetrics.Client.Global.Size, rectImage, rectImage, (::llc::ALIGN)controlImage.ImageAlign);
		rectImage.Offset		+= controlMetrics.Client.Global.Offset;
		rectImage.Size.x		= ::llc::min(rectImage.Limit().x, controlMetrics.Client.Global.Limit().x) - rectImage.Offset.x;
		rectImage.Size.y		= ::llc::min(rectImage.Limit().y, controlMetrics.Client.Global.Limit().y) - rectImage.Offset.y;
		if(controlState.IsHovered())
			rectImage.Offset		+= {1, 1};
		if(0 == controlImage.ImageInvertY)
			::llc::grid_copy_blend(target, controlImage.Image, rectImage, controlImage.ImageOffset);
		else {
			::llc::img8bgra				& inverted				= gui.Controls.Images[iControl].Temp.Color;
			llc_necs(gui.Controls.Images[iControl].Temp.resize(controlImage.Image.metrics()));
			::llc::grid_mirror_y(inverted.View, controlImage.Image);
			::llc::grid_copy_blend(target, inverted.View, rectImage, controlImage.ImageOffset);
		}
	}

	es_if(::llc::failed(::controlTextDraw(gui, iControl, target, disabled)));
	return 0;
}
::llc::error_t			llc::controlDrawHierarchy		(::llc::SGUI & gui, ::llc::cid_t iControl, ::llc::g8bgra target)								{
	llc_necall(llc::controlInvalid(gui, iControl), "Invalid control id: %u.", iControl);
	if(gui.LastSize != target.metrics16()) {
		for(uint32_t iOutdated = 0; iOutdated < gui.Controls.States.size(); ++iOutdated)
			gui.Controls.SetUpdated((::llc::cid_t)iOutdated, false);
		gui.LastSize			= target.metrics().u1();
	}
	if(false == ::llc::controlHidden(gui, iControl)) {
		llc_necs(::actualControlDraw(gui, iControl, target));
		::llc::vccid				children						= gui.Controls.Children[iControl];
		for(uint32_t iChild = 0, countChild = children.size(); iChild < countChild; ++iChild)
			llc_necs(llc::controlDrawHierarchy(gui, children[iChild], target));
	}
	return 0;
}
::llc::error_t			llc::guiDraw					(::llc::SGUI & gui, ::llc::g8bgra target)													{
	if(gui.LastSize != target.metrics().u1() || gui.LastZoom != gui.Zoom) {
		for(uint32_t iOutdated = 0; iOutdated < gui.Controls.States.size(); ++iOutdated)
			gui.Controls.SetUpdated((::llc::cid_t)iOutdated, false);
		gui.LastSize			= target.metrics().u1();
		gui.LastZoom			= gui.Zoom;
	}
	llc_necs(llc::guiUpdateMetrics(gui, gui.LastSize, false));;
	for(uint32_t iControl = 0; iControl < gui.Controls.States.size(); ++iControl)
		if(false == ::llc::controlInvalid(gui, (::llc::cid_t)iControl) && ::llc::controlInvalid(gui, gui.Controls.States[iControl].Parent))
			es_if(::llc::failed(::llc::controlDrawHierarchy(gui, (::llc::cid_t)iControl, target)));
	return 0;
}
