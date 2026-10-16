#include "llc_gui_control_list.h"
#include "llc_label.h"

::llc::cid_t			llc::createScreenLayout(::llc::SGUI & gui, ::llc::cid_t parent) {
	::llc::cid_t				controlId;
	llc_necs(controlId = ::llc::controlCreate(gui));

	gui.Controls.Placement	[controlId].Border				= 
	gui.Controls.Placement	[controlId].Margin				= {};
	gui.Controls.Constraints[controlId].AttachSizeToControl	= {controlId, controlId};
	gui.Controls.Draw		[controlId].NoClient			= true;
	gui.Controls.States		[controlId].Mask				&= ~::llc::GUI_CONTROL_FLAG_Hovered;

	if(parent != ::llc::CID_INVALID)
		llc_necs(llc::controlSetParent(gui, controlId, parent));

	return controlId;
}

::llc::error_t			llc::controlListInitialize	(::llc::SGUI & gui, ::llc::SControlList& menu)													{
	menu					= {};
	llc_necall(menu.IdControl = ::llc::controlCreate(gui), "%s", "Failed to create menu control!");
	gui.Controls.Placement	[menu.IdControl].Border				= {};
	gui.Controls.Placement	[menu.IdControl].Margin				= {};
	gui.Controls.Text		[menu.IdControl].Text				= " ";
	gui.Controls.Constraints[menu.IdControl].AttachSizeToText	= {false, false};
	//gui.Controls.Modes			[menu.IdControl].Design				= true;
	return 0;
}

::llc::error_t			llc::controlListArrange	(::llc::SGUI & gui, ::llc::SControlList& menu)													{
	const ::llc::SRasterFont	& selectedFont			= *gui.Fonts[::llc::in_range(gui.Controls.Text[menu.IdControl].FontSelected, (int16_t)0, (int16_t)gui.Fonts.size()) ? gui.Controls.Text[menu.IdControl].FontSelected : gui.SelectedFont];
	for(::llc::u2_t iItem = 0; iItem < menu.IdControls.size(); ++iItem) {
		const ::llc::u2_t				idControl				= menu.IdControls[iItem];
		::llc::SControlConstraints	& controlConstraints	= gui.Controls.Constraints	[idControl];
		::llc::SControlPlacement	& control				= gui.Controls.Placement	[idControl];
		::llc::SControlText			& controlText			= gui.Controls.Text			[idControl];
		const ::llc::u2_t				iPrevItem				= iItem - 1;
		const ::llc::n2u0_t			fontCharSize			= selectedFont.CharSize;
		control.Area.Size.y		= fontCharSize.y + ::llc::controlNCSpacing(control).y;
		switch(menu.Orientation) {
		default:
		case ::llc::DIRECTION_RIGHT	:
			control.Area.Size.x		= int16_t(fontCharSize.x * controlText.Text.size() + ::llc::controlNCSpacing(control).x);
			if(menu.IdControls.size() > iPrevItem)
				control.Area.Offset.x	= gui.Controls.Placement[menu.IdControls[iPrevItem]].Area.Limit().x;
			break;
		case ::llc::DIRECTION_DOWN:
			controlConstraints.AttachSizeToControl.x	= menu.IdControl;
			if(menu.IdControls.size() > iPrevItem)
				control.Area.Offset.y	= gui.Controls.Placement[menu.IdControls[iPrevItem]].Area.Limit().y;
			break;
		}
	}

	::llc::SControlPlacement	& controlMenu			= gui.Controls.Placement[menu.IdControl];
	controlMenu.Area.Size	= {};
	const ::llc::n2u0_t			fontCharSize			= selectedFont.CharSize;
	for(::llc::u2_t iItem = 0; iItem < menu.IdControls.size(); ++iItem) {
		const ::llc::u2_t				idControl			= menu.IdControls		[iItem];
		::llc::SControlPlacement	& control			= gui.Controls.Placement[idControl];
		::llc::SControlText			& controlText		= gui.Controls.Text		[idControl];
		switch(menu.Orientation) {
		default:
		case ::llc::DIRECTION_RIGHT	:
			controlMenu.Area.Size.x	+= gui.Controls.Placement[idControl].Area.Size.x;
			controlMenu.Area.Size.y	= ::llc::max(controlMenu.Area.Size.y, gui.Controls.Placement[idControl].Area.Size.y);
			break;
		case ::llc::DIRECTION_DOWN		:
			controlMenu.Area.Size.y	+= gui.Controls.Placement[idControl].Area.Size.y;
			controlMenu.Area.Size.x	= ::llc::max(controlMenu.Area.Size.x, (int16_t)(controlText.Text.size() * fontCharSize.x + ::llc::controlNCSpacing(control).x));
			break;
		}
	}
	if(::llc::DIRECTION_DOWN == menu.Orientation)
		controlMenu.Area.Size.x	+= 16;

	return 0;
}

::llc::error_t			llc::controlListPush	(::llc::SGUI & gui, ::llc::SControlList& menu, const ::llc::vcs& text, int64_t eventCode)				{
	if(menu.IdControl == -1)
		llc_necs(llc::controlListInitialize(gui, menu));

	const cid_t					idControl				= ::llc::controlCreate(gui);
	llc_necall(idControl, "%s", "Failed to create control! Out of memory?");
	::llc::controlSetParent(gui, idControl, menu.IdControl);
	::llc::SControlPlacement	& control				= gui.Controls.Placement[idControl];
	::llc::SControlText			& controlText			= gui.Controls.Text		[idControl];
	control		.Margin		= {4, 2, 4, 2};
	controlText	.Text		= {text.begin(), text.size()};
	controlText	.Align		= ::llc::ALIGN_CENTER_LEFT;
	menu.IdControls	.push_back(idControl);
	menu.Events		.push_back(eventCode);
	llc::controlListArrange(gui, menu);
	return 0;
}

::llc::error_t			llc::paletteGridInitialize	(::llc::SGUI & gui, ::llc::SPaletteGrid& palette)				{
	const ::llc::n2f3_t			targetSize					= {256.0, 256.0};
	palette.IdControl		= ::llc::controlCreate(gui);

	::llc::SControlPlacement	& control					= gui.Controls.Placement[palette.IdControl];
	const ::llc::u2_t				widthViewport				= (::llc::u2_t)(targetSize.x + ((int64_t)control.Border.Left + control.Border.Right + control.Margin.Left + control.Margin.Right));
	const ::llc::u2_t				heightViewport				= (::llc::u2_t)(targetSize.y + ((int64_t)control.Border.Top + control.Border.Bottom + control.Margin.Top + control.Margin.Bottom));
	control.Area.Size		= {(int16_t)widthViewport, (int16_t)heightViewport};
	control.Align			= ::llc::ALIGN_CENTER;
	return 0;
}

::llc::error_t			llc::paletteGridColorsSet	(::llc::SGUI & gui, ::llc::SPaletteGrid& palette, const ::llc::g8bgra& colors)			{
	if(-1 == palette.IdControl)
		::llc::paletteGridInitialize(gui, palette);

	//gui.Controls.Area[palette.IdControl];

	const ::llc::n2u2_t			& gridMetrics				= colors.metrics();
	palette.IdControls.resize(colors.size());
	palette.Colors			= colors;

	const ::llc::n2f3_t			targetSize					= gui.Controls.Area[palette.IdControl].Client.Global.Size.f3(); //{256.0, 256.0};
	::llc::n2f3_t				controlSize					= {targetSize.x / colors.metrics().x, targetSize.y / colors.metrics().y};

	for(::llc::u2_t y = 0; y < gridMetrics.y; ++y)
	for(::llc::u2_t x = 0; x < gridMetrics.x; ++x) {
		const int32_t				linearIndex					= y * gridMetrics.x + x;
		::llc::u2_t					iFoundColor					= 0;
		int32_t						themeIndex					= -1;
		const ::llc::bgra			& gridCellColor				= colors[y][x];
		for(; iFoundColor < gui.Colors->Palette->size(); ++iFoundColor)
			if((*gui.Colors->Palette)[iFoundColor] == gridCellColor) {
				themeIndex				= iFoundColor;
				break;
			}
		if(iFoundColor >= gui.Colors->Palette->size()) {
			int32_t						indexColor			= gui.Colors->Palette->push_back(gridCellColor);
			themeIndex				= gui.Colors->ControlThemes->push_back({});
			::llc::SControlTheme		& theme				= (*gui.Colors->ControlThemes)[themeIndex];
			::llc::astu32<::llc::UI_CONTROL_AREA_COUNT>	& comboDisabled		= theme.ColorCombos[::llc::GUI_CONTROL_PALETTE_DISABLED	] = {};
			::llc::astu32<::llc::UI_CONTROL_AREA_COUNT>	& comboPressed 		= theme.ColorCombos[::llc::GUI_CONTROL_PALETTE_PRESSED	] = {};
			::llc::astu32<::llc::UI_CONTROL_AREA_COUNT>	& comboSelected		= theme.ColorCombos[::llc::GUI_CONTROL_PALETTE_SELECTED	] = {};
			::llc::astu32<::llc::UI_CONTROL_AREA_COUNT>	& comboHover 		= theme.ColorCombos[::llc::GUI_CONTROL_PALETTE_HOVER	] = {};
			::llc::astu32<::llc::UI_CONTROL_AREA_COUNT>	& comboNormal		= theme.ColorCombos[::llc::GUI_CONTROL_PALETTE_NORMAL	] = {};
			comboDisabled	[::llc::UI_CONTROL_AREA_BACKGROUND	]	= indexColor;
			comboDisabled	[::llc::UI_CONTROL_AREA_CLIENT		]	= comboDisabled[::llc::UI_CONTROL_AREA_BACKGROUND];
			comboNormal		[::llc::UI_CONTROL_AREA_BACKGROUND	]	= indexColor;
			comboNormal		[::llc::UI_CONTROL_AREA_CLIENT		]	= comboDisabled[::llc::UI_CONTROL_AREA_BACKGROUND];
			comboSelected	[::llc::UI_CONTROL_AREA_BACKGROUND	]	= indexColor;
			comboSelected	[::llc::UI_CONTROL_AREA_CLIENT		]	= comboDisabled[::llc::UI_CONTROL_AREA_BACKGROUND];
			comboHover		[::llc::UI_CONTROL_AREA_BACKGROUND	]	= indexColor;
			comboHover		[::llc::UI_CONTROL_AREA_CLIENT		]	= comboDisabled[::llc::UI_CONTROL_AREA_BACKGROUND];
			comboPressed	[::llc::UI_CONTROL_AREA_BACKGROUND	]	= indexColor;
			comboPressed	[::llc::UI_CONTROL_AREA_CLIENT		]	= comboDisabled[::llc::UI_CONTROL_AREA_BACKGROUND];
			//for(::llc::u2_t iState = 0; iState < theme.ColorCombos.size(); ++iState)
			//for(::llc::u2_t iArea = 0; iArea < ::llc::GUI_CONTROL_COLOR_COUNT; ++iArea)
			//	theme.ColorCombos[iState][iArea]	= ::llc::min((::llc::u2_t)theme.ColorCombos[iState][iArea], gui.Palette.size() - 1);
		}
		const ::llc::u2_t				paletteElemIndex	= linearIndex;
		palette.IdControls[paletteElemIndex] = ::llc::controlCreate(gui);
		::llc::SControlPlacement	& control			= gui.Controls.Placement[palette.IdControls[paletteElemIndex]];
		::llc::SControlDraw			& controlDraw		= gui.Controls.Draw[palette.IdControls[paletteElemIndex]];
		controlDraw.ColorTheme	= int8_t(themeIndex + 1);
		control.Area.Offset		= {(int16_t)(controlSize.x * x), (int16_t)(controlSize.y * y)};
		control.Area.Size		= controlSize.s1();
		llc_necs(llc::controlSetParent(gui, palette.IdControls[paletteElemIndex], palette.IdControl));
	}
	return 0;
}

::llc::cid_t			llc::guiCreateControlList		(::llc::SGUI & gui, ::llc::DIRECTION direction, ::llc::view<::llc::vcst_c> buttonText, ::llc::cid_t iParent, const ::llc::SControlPlacement & args, ::llc::ALIGN textAlign, ::llc::vcid out_ids) {
	::llc::cid_t				result						= -1;
	for(::llc::u1_t iButton = 0; iButton < buttonText.size(); ++iButton) {
		::llc::cid_t				idControl					= ::llc::controlCreate(gui);
		if(out_ids.size() > iButton)
			out_ids[iButton]		= idControl;

		if(0 == iButton)
			result					= idControl;

		::llc::SControlPlacement	& control					= gui.Controls.Placement[idControl];
		control					= args;
		if(direction & (::llc::DIRECTION_LEFT | ::llc::DIRECTION_RIGHT))
			control.Area.Offset.x	+= int16_t(args.Area.Size.x * iButton);
		else
			control.Area.Offset.y	+= int16_t(args.Area.Size.y * iButton);

		::llc::SControlText			& controlText				= gui.Controls.Text[idControl];
		controlText.Text		= buttonText[iButton];
		controlText.Align		= textAlign;

		::llc::SControlConstraints	& itemConstraints	= gui.Controls.Constraints[idControl];
		if(direction & (::llc::DIRECTION_UP | ::llc::DIRECTION_DOWN))
			itemConstraints.AttachSizeToControl.x	= iParent;
		else if(direction & (::llc::DIRECTION_LEFT | ::llc::DIRECTION_RIGHT)) {
			itemConstraints.AttachSizeToText.x		= true;
			itemConstraints.AttachSizeToControl.y	= iParent;
		}

		if(iButton) {
			if(direction & ::llc::DIRECTION_LEFT)
				itemConstraints.DockToControl.Left	= out_ids[iButton - 1];
			else if(direction & ::llc::DIRECTION_RIGHT)
				itemConstraints.DockToControl.Right	= out_ids[iButton - 1];
			else if(direction & ::llc::DIRECTION_UP)
				itemConstraints.DockToControl.Top	= out_ids[iButton - 1];
			else if(direction & ::llc::DIRECTION_DOWN)
				itemConstraints.DockToControl.Bottom	= out_ids[iButton - 1];

		}
		llc_necall(llc::controlSetParent(gui, idControl, iParent), "idControl: %i, buttonText[iButton]: %.*s", idControl, LLC_VIEW_ARGS(buttonText[iButton])); // Make a tplt and macro for this pattern
	}
	return result;
}

::llc::cid_t			llc::guiCreateControlList		(::llc::SGUI & gui, ::llc::view<::llc::vcst_c> buttonText, ::llc::cid_t iParent, ::llc::n2u1_c & buttonSize, ::llc::n2s1_c & offset, ::llc::ALIGN controlAlign, ::llc::ALIGN textAlign, ::llc::vcid out_ids) {
	::llc::SControlPlacement	controlArgs					= {};
	controlArgs.Align		= controlAlign;
	controlArgs.Area		= {offset, buttonSize.s1()};
	controlArgs.Border		= {1, 1, 1, 1};
	controlArgs.Margin		= {1, 1, 1, 1};
	return ::llc::guiCreateControlList(gui, {}, buttonText, iParent, controlArgs, textAlign, out_ids);
}
