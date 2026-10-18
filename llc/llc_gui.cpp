#include "llc_gui.h"
#include "llc_gui_text.h"
#include "llc_base64.h"
#include "llc_rect_align.h"
#include "llc_apod_color.h"

::llc::error_t			llc::controlInvalid			(const ::llc::SGUI & gui, cid_t iControl)				{
	if(gui.Controls.States.size() <= ::llc::u2_t(iControl)	) 
		return -1;
	if(gui.Controls.IsUnused(iControl)) 
		return -1;
	return 0;
}

static	::llc::error_t	paletteSetupDefault			(::llc::a8bgra & palette, const ::llc::view<const ::llc::bgra> & colors, ::llc::u2_t iShades)	{
	const ::llc::u2_t				newPaletteSize				= colors.size() * iShades;
	if(palette.size() < newPaletteSize)
		llc_necs(palette.resize(newPaletteSize));

	for(::llc::u2_t iTone = 0; iTone < colors.size(); ++iTone)
		palette[iTone * iShades]	= colors[iTone];

	for(::llc::u2_t iShade = 0; iShade < iShades; ++iShade) {
		const ::llc::bgra			& baseColor					= palette[iShade * iShades];
		::llc::bgra					& paletteItem				= palette[iShade];
		paletteItem				= baseColor;
	}

	for(::llc::u2_t iTone = 1; iTone < colors.size(); ++iTone)
		for(::llc::u2_t iShade = 0; iShade < iShades; ++iShade) {
			if(0 == (iShade % iShades))
				continue;

			const int32_t				toneIndex					= iTone * iShades;
			const ::llc::bgra			& baseColor					= colors[iTone];
			::llc::bgra					& paletteItem				= palette[toneIndex + iShade];
			paletteItem				= ::llc::rgbaf(baseColor) / (float)iShades * (iShades - (float)iShade);
			paletteItem.a			= 0xFF;
			verbose_printf("Original color: {r: 0x%X, g: 0x%X, b: 0x%X}.", baseColor	.r, baseColor	.g, baseColor	.b);
			verbose_printf("Shaded color  : {r: 0x%X, g: 0x%X, b: 0x%X}.", paletteItem	.r, paletteItem	.g, paletteItem	.b);
		}

	return 0;
}

static	::llc::error_t	themeSetupDefault						(const ::llc::view<const ::llc::bgra> & palette, int32_t iColor, ::llc::SControlTheme & theme, ::llc::u2_t iShades)	{
	::llc::astu32<::llc::UI_CONTROL_AREA_COUNT>		& colorComboDisabled									= theme.ColorCombos[::llc::GUI_CONTROL_PALETTE_DISABLED	]	= {};
	::llc::astu32<::llc::UI_CONTROL_AREA_COUNT>		& colorComboPressed 									= theme.ColorCombos[::llc::GUI_CONTROL_PALETTE_PRESSED	]	= {};
	::llc::astu32<::llc::UI_CONTROL_AREA_COUNT>		& colorComboSelected									= theme.ColorCombos[::llc::GUI_CONTROL_PALETTE_SELECTED	]	= {};
	::llc::astu32<::llc::UI_CONTROL_AREA_COUNT>		& colorComboHover 										= theme.ColorCombos[::llc::GUI_CONTROL_PALETTE_HOVER	]	= {};
	::llc::astu32<::llc::UI_CONTROL_AREA_COUNT>		& colorComboNormal										= theme.ColorCombos[::llc::GUI_CONTROL_PALETTE_NORMAL	]	= {};

	//const int32_t														colorBase												= iColor / iShades;
	const int32_t														colorShade												= iColor % iShades;
	const int32_t														colorText												= (::llc::ASCII_COLOR_WHITE * iShades);

	colorComboDisabled	[::llc::UI_CONTROL_AREA_BACKGROUND	]	= iColor;//(colorShade > 7) ? iColor : iColor;
	colorComboDisabled	[::llc::UI_CONTROL_AREA_BORDER_LEFT	]	= (colorShade > 7) ? iColor - 1 : iColor + 1;
	colorComboDisabled	[::llc::UI_CONTROL_AREA_BORDER_TOP	]	= (colorShade > 7) ? iColor - 1 : iColor + 1;
	colorComboDisabled	[::llc::UI_CONTROL_AREA_BORDER_RIGHT	]	= (colorShade > 7) ? iColor - 1 : iColor + 1;
	colorComboDisabled	[::llc::UI_CONTROL_AREA_BORDER_BOTTOM	]	= (colorShade > 7) ? iColor - 1 : iColor + 1;
	colorComboDisabled	[::llc::UI_CONTROL_AREA_TEXT_FACE		]	= (colorShade > 7) ? iColor - 3 : iColor + 3;
//
	colorComboPressed 	[::llc::UI_CONTROL_AREA_BACKGROUND	]	= iColor;//(colorShade > 7) ? iColor : iColor;
	colorComboPressed 	[::llc::UI_CONTROL_AREA_BORDER_LEFT	]	= (colorShade > 7) ? iColor - 3 : iColor + 3;
	colorComboPressed 	[::llc::UI_CONTROL_AREA_BORDER_TOP	]	= (colorShade > 7) ? iColor - 3 : iColor + 3;
	colorComboPressed 	[::llc::UI_CONTROL_AREA_BORDER_RIGHT	]	= (colorShade > 7) ? iColor - 3 : iColor + 3;
	colorComboPressed 	[::llc::UI_CONTROL_AREA_BORDER_BOTTOM	]	= (colorShade > 7) ? iColor - 3 : iColor + 3;
	colorComboPressed 	[::llc::UI_CONTROL_AREA_TEXT_FACE		]	= (colorShade > 7) ? colorText + 7 : colorText + 9;
//
	colorComboSelected	[::llc::UI_CONTROL_AREA_BACKGROUND	]	= (colorShade > 7) ? colorText - 7 : colorText + 9;
	colorComboSelected	[::llc::UI_CONTROL_AREA_BORDER_LEFT	]	= (colorShade > 7) ? colorText - 7 : colorText + 9;
	colorComboSelected	[::llc::UI_CONTROL_AREA_BORDER_TOP	]	= (colorShade > 7) ? colorText - 7 : colorText + 9;
	colorComboSelected	[::llc::UI_CONTROL_AREA_BORDER_RIGHT	]	= (colorShade > 7) ? colorText - 7 : colorText + 9;
	colorComboSelected	[::llc::UI_CONTROL_AREA_BORDER_BOTTOM	]	= (colorShade > 7) ? colorText - 7 : colorText + 9;
	colorComboSelected	[::llc::UI_CONTROL_AREA_TEXT_FACE		]	= (colorShade > 7) ? colorText : colorText + 8;
//
	colorComboHover 	[::llc::UI_CONTROL_AREA_BACKGROUND	]	= iColor;//(colorShade > 7) ? iColor : iColor;
	colorComboHover 	[::llc::UI_CONTROL_AREA_BORDER_LEFT	]	= (colorShade > 7) ? iColor - 7: iColor + 7;
	colorComboHover 	[::llc::UI_CONTROL_AREA_BORDER_TOP	]	= (colorShade > 7) ? iColor - 7: iColor + 7;
	colorComboHover 	[::llc::UI_CONTROL_AREA_BORDER_RIGHT	]	= (colorShade > 7) ? iColor - 7: iColor + 7;
	colorComboHover 	[::llc::UI_CONTROL_AREA_BORDER_BOTTOM	]	= (colorShade > 7) ? iColor - 7: iColor + 7;
	colorComboHover 	[::llc::UI_CONTROL_AREA_TEXT_FACE		]	= (colorShade > 7) ? colorText + 4: colorText + 12;
//
	colorComboNormal	[::llc::UI_CONTROL_AREA_BACKGROUND	]	= iColor;//(colorShade > 7) ? iColor : iColor;
	colorComboNormal	[::llc::UI_CONTROL_AREA_BORDER_LEFT	]	= (colorShade > 7) ? iColor - 5: iColor + 5;
	colorComboNormal	[::llc::UI_CONTROL_AREA_BORDER_TOP	]	= (colorShade > 7) ? iColor - 5: iColor + 5;
	colorComboNormal	[::llc::UI_CONTROL_AREA_BORDER_RIGHT	]	= (colorShade > 7) ? iColor - 5: iColor + 5;
	colorComboNormal	[::llc::UI_CONTROL_AREA_BORDER_BOTTOM	]	= (colorShade > 7) ? iColor - 5: iColor + 5;
	colorComboNormal	[::llc::UI_CONTROL_AREA_TEXT_FACE		]	= (colorShade > 7) ? colorText : colorText + 15;

	colorComboDisabled	[::llc::UI_CONTROL_AREA_TEXT_BACKGROUND	]	= colorComboDisabled	[::llc::UI_CONTROL_AREA_BACKGROUND];
	colorComboDisabled	[::llc::UI_CONTROL_AREA_CLIENT			]	= colorComboDisabled	[::llc::UI_CONTROL_AREA_BACKGROUND];
	colorComboPressed 	[::llc::UI_CONTROL_AREA_TEXT_BACKGROUND	]	= colorComboPressed		[::llc::UI_CONTROL_AREA_BACKGROUND];
	colorComboPressed 	[::llc::UI_CONTROL_AREA_CLIENT			]	= colorComboPressed 	[::llc::UI_CONTROL_AREA_BACKGROUND];
	colorComboSelected	[::llc::UI_CONTROL_AREA_TEXT_BACKGROUND	]	= colorComboSelected	[::llc::UI_CONTROL_AREA_BACKGROUND];
	colorComboSelected	[::llc::UI_CONTROL_AREA_CLIENT			]	= colorComboSelected	[::llc::UI_CONTROL_AREA_BACKGROUND];
	colorComboHover 	[::llc::UI_CONTROL_AREA_TEXT_BACKGROUND	]	= colorComboHover		[::llc::UI_CONTROL_AREA_BACKGROUND];
	colorComboHover 	[::llc::UI_CONTROL_AREA_CLIENT			]	= colorComboHover 		[::llc::UI_CONTROL_AREA_BACKGROUND];
	colorComboNormal	[::llc::UI_CONTROL_AREA_TEXT_BACKGROUND	]	= colorComboNormal		[::llc::UI_CONTROL_AREA_BACKGROUND];
	colorComboNormal	[::llc::UI_CONTROL_AREA_CLIENT			]	= colorComboNormal		[::llc::UI_CONTROL_AREA_BACKGROUND];

	for(::llc::u2_t iState = 0; iState < theme.ColorCombos.size(); ++iState)
		for(::llc::u2_t iArea = 0; iArea < ::llc::UI_CONTROL_AREA_COUNT; ++iArea) {
			theme.ColorCombos[iState][iArea]	= ::llc::min((::llc::u2_t)theme.ColorCombos[iState][iArea], palette.size() - 1);
		}
	return 0;
}

static	::llc::error_t	themeSetupDefault	(const ::llc::a8bgra & palette, ::llc::apod<::llc::SControlTheme>& themes, ::llc::u2_t iShades)	{
	for(::llc::u2_t iColor = 0; iColor < palette.size(); ++iColor) {
		const int32_t				indexTheme			= themes.push_back({});
		::llc::SControlTheme		& theme				= themes[indexTheme];
		::themeSetupDefault(palette, iColor, theme, iShades);
	}
	return 0;
}

static	::llc::error_t	initDefaults		(::llc::pobj<::llc::a8bgra> & palette, ::llc::pobj<::llc::apod<::llc::SControlTheme>> & controlThemes) {
	stacxpr	::llc::u2_t			iShades				= 16;
	static	::llc::bgra			paletteColors []	=
		// 16 Base colors
		{ ::llc::SColorRGBA{::llc::ASCII_COLOR_INDEX_0	}
		, ::llc::SColorRGBA{::llc::ASCII_COLOR_INDEX_1	}
		, ::llc::SColorRGBA{::llc::ASCII_COLOR_INDEX_2	}
		, ::llc::SColorRGBA{::llc::ASCII_COLOR_INDEX_3	}
		, ::llc::SColorRGBA{::llc::ASCII_COLOR_INDEX_4	}
		, ::llc::SColorRGBA{::llc::ASCII_COLOR_INDEX_5	}
		, ::llc::SColorRGBA{::llc::ASCII_COLOR_INDEX_6	}
		, ::llc::SColorRGBA{::llc::ASCII_COLOR_INDEX_7	}
		, ::llc::SColorRGBA{::llc::ASCII_COLOR_INDEX_8	}
		, ::llc::SColorRGBA{::llc::ASCII_COLOR_INDEX_9	}
		, ::llc::SColorRGBA{::llc::ASCII_COLOR_INDEX_10	}
		, ::llc::SColorRGBA{::llc::ASCII_COLOR_INDEX_11	}
		, ::llc::SColorRGBA{::llc::ASCII_COLOR_INDEX_12	}
		, ::llc::SColorRGBA{::llc::ASCII_COLOR_INDEX_13	}
		, ::llc::SColorRGBA{::llc::ASCII_COLOR_INDEX_14	}
		, ::llc::SColorRGBA{::llc::ASCII_COLOR_INDEX_15	}
		// 16 Extended colors
		, ::llc::SColorRGBA{::llc::ASCII_COLOR_INDEX_14	+ ::llc::ASCII_COLOR_INDEX_1}
		, ::llc::SColorRGBA{::llc::ASCII_COLOR_INDEX_13	+ ::llc::ASCII_COLOR_INDEX_2}
		, ::llc::SColorRGBA{::llc::ASCII_COLOR_INDEX_12	+ ::llc::ASCII_COLOR_INDEX_3}
		, ::llc::SColorRGBA{::llc::ASCII_COLOR_INDEX_11	+ ::llc::ASCII_COLOR_INDEX_4}
		, ::llc::SColorRGBA{::llc::ASCII_COLOR_INDEX_10	+ ::llc::ASCII_COLOR_INDEX_5}
		, ::llc::SColorRGBA{::llc::ASCII_COLOR_INDEX_9 	+ ::llc::ASCII_COLOR_INDEX_6}
		, ::llc::SColorRGBA{::llc::ASCII_COLOR_INDEX_8 	+ ::llc::ASCII_COLOR_INDEX_7}
		, ::llc::SColorRGBA{0xFFFFFFFFU & (::llc::ASCII_COLOR_INDEX_4 * (uint64_t)::llc::ASCII_COLOR_INDEX_11)}
		, ::llc::SColorRGBA{0xFFFFFFFFU & (::llc::ASCII_COLOR_INDEX_5 * (uint64_t)::llc::ASCII_COLOR_INDEX_10)}
		, ::llc::SColorRGBA{0xFFFFFFFFU & (::llc::ASCII_COLOR_INDEX_7 * (uint64_t)::llc::ASCII_COLOR_INDEX_8 )}
		, ::llc::SColorRGBA{(::llc::u2_t)(::llc::ASCII_COLOR_INDEX_1 + ::llc::ASCII_COLOR_INDEX_4)}
		, ::llc::SColorRGBA{(::llc::u2_t)(::llc::ASCII_COLOR_INDEX_3 + ::llc::ASCII_COLOR_INDEX_7)}
		, ::llc::SColorRGBA{(::llc::u2_t)(::llc::ASCII_COLOR_INDEX_1 + ::llc::ASCII_COLOR_INDEX_8)}
		, ::llc::SColorRGBA{(::llc::u2_t)(::llc::ASCII_COLOR_INDEX_4 + ::llc::ASCII_COLOR_INDEX_7)}

		, ::llc::BROWN
		, ::llc::LIGHTORANGE
	};
	for(::llc::u2_t iColor = 0; iColor < ::llc::size(paletteColors); ++iColor) 
		paletteColors[iColor].a = 255;

	if(palette && 0 == palette->size())
		llc_necs(::paletteSetupDefault(*palette, paletteColors, iShades));
	else if(!palette) 
		llc_necs(::paletteSetupDefault(*palette.create(), paletteColors, iShades));

	if(controlThemes && 0 == controlThemes->size())
		llc_necs(::themeSetupDefault(*palette, *controlThemes, iShades));
	else if(!controlThemes)
		llc_necs(::themeSetupDefault(*palette, *controlThemes.create(), iShades));
	return 0;
}

static	::llc::error_t	paletteSetupDefaultColors	(::llc::pobj<::llc::a8bgra> & palette, ::llc::pobj<::llc::apod<::llc::SControlTheme>> & controlThemes)	{
	static ::llc::papod<::llc::bgra>			globalDefaultPalette		= {};
	static ::llc::papod<::llc::SControlTheme>	globalDefaultTheme			= {};
	
	if(!globalDefaultPalette || 0 == globalDefaultPalette->size()) 
		::initDefaults(globalDefaultPalette, globalDefaultTheme);

	palette			= globalDefaultPalette;
	controlThemes	= globalDefaultTheme;
	return 0;
}

static	::llc::error_t	uiColorsSetupDefault (::llc::SGUIColors & guiColors) {
	guiColors.DefaultColors[::llc::GUI_CONTROL_PALETTE_NORMAL				]	= (::llc::u1_t)guiColors.Palettes.push_back({{::llc::BLUE		, ::llc::GRAY	, ::llc::YELLOW, ::llc::MAGENTA, ::llc::CYAN, {}, ::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,}});
	guiColors.DefaultColors[::llc::GUI_CONTROL_PALETTE_DISABLED				]	= (::llc::u1_t)guiColors.Palettes.push_back({{::llc::LIGHTGRAY	, ::llc::ORANGE	, ::llc::YELLOW, ::llc::MAGENTA, ::llc::CYAN, {}, ::llc::LIGHTGRAY * 1.1	,::llc::LIGHTGRAY * 1.1	,}});::llc::LIGHTGRAY * 1.1	,	 ::llc::LIGHTGRAY * 1.1	,	 ::llc::LIGHTGRAY * 1.1	,	 ::llc::LIGHTGRAY * 1.1	,	 ::llc::LIGHTGRAY * 1.1	,	 ::llc::LIGHTGRAY * 1.1	,	 ::llc::LIGHTGRAY * 1.1	,
	guiColors.DefaultColors[::llc::GUI_CONTROL_PALETTE_HOVER				]	= (::llc::u1_t)guiColors.Palettes.push_back({{::llc::CYAN		, ::llc::YELLOW	, ::llc::MAGENTA, ::llc::GREEN, ::llc::ORANGE, {}, ::llc::DARKCYAN / 2.0	, ::llc::DARKCYAN / 2.0	,}}); ::llc::DARKCYAN / 2.0	,	  ::llc::DARKCYAN / 2.0	,	  ::llc::DARKCYAN / 2.0	,	  ::llc::DARKCYAN / 2.0	,	  ::llc::DARKCYAN / 2.0	,	  ::llc::DARKCYAN / 2.0	,	  ::llc::DARKCYAN / 2.0	,
	guiColors.DefaultColors[::llc::GUI_CONTROL_PALETTE_PRESSED				]	= (::llc::u1_t)guiColors.Palettes.push_back({{::llc::RED		, ::llc::ORANGE	, ::llc::YELLOW, ::llc::MAGENTA, ::llc::CYAN, {}, ::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,}});
	guiColors.DefaultColors[::llc::GUI_CONTROL_PALETTE_SELECTED				]	= (::llc::u1_t)guiColors.Palettes.push_back({{::llc::ORANGE	, ::llc::ORANGE	, ::llc::YELLOW, ::llc::MAGENTA, ::llc::CYAN, {}, ::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,}});
	guiColors.DefaultColors[::llc::GUI_CONTROL_PALETTE_SELECTED_DISABLED	]	= (::llc::u1_t)guiColors.Palettes.push_back({{::llc::BLUE		, ::llc::ORANGE	, ::llc::YELLOW, ::llc::MAGENTA, ::llc::CYAN, {}, ::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,}});
	guiColors.DefaultColors[::llc::GUI_CONTROL_PALETTE_SELECTED_HOVER		]	= (::llc::u1_t)guiColors.Palettes.push_back({{::llc::BLUE		, ::llc::ORANGE	, ::llc::YELLOW, ::llc::MAGENTA, ::llc::CYAN, {}, ::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,}});
	guiColors.DefaultColors[::llc::GUI_CONTROL_PALETTE_SELECTED_PRESSED		]	= (::llc::u1_t)guiColors.Palettes.push_back({{::llc::BLUE		, ::llc::ORANGE	, ::llc::YELLOW, ::llc::MAGENTA, ::llc::CYAN, {}, ::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,}});
	guiColors.DefaultColors[::llc::GUI_CONTROL_PALETTE_ACTION				]	= (::llc::u1_t)guiColors.Palettes.push_back({{::llc::BLUE		, ::llc::ORANGE	, ::llc::YELLOW, ::llc::MAGENTA, ::llc::CYAN, {}, ::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,}});
	guiColors.DefaultColors[::llc::GUI_CONTROL_PALETTE_OUTDATED				]	= (::llc::u1_t)guiColors.Palettes.push_back({{::llc::BLUE		, ::llc::ORANGE	, ::llc::YELLOW, ::llc::MAGENTA, ::llc::CYAN, {}, ::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,::llc::WHITE				,}});
	return 0;
}

static	::llc::error_t	uiColorsSetupDefault (::llc::pobj<::llc::SGUIColors> & guiColors) {
	if(!guiColors) {
		static ::llc::pobj<::llc::SGUIColors>	globalDefaultColors			= {};
		if(!globalDefaultColors) {
			llc_necs(::uiColorsSetupDefault(*globalDefaultColors.create()));
			llc_necs(::paletteSetupDefaultColors(globalDefaultColors->Palette, globalDefaultColors->ControlThemes));
		}
		guiColors	= globalDefaultColors;
	}
	return 0;
}

static	::llc::error_t	setupDefaultFontTexture	(::llc::SGUI & gui)			{
	static ::llc::SRasterFontManager	g_managedFonts;
	static const ::llc::u2_t				g_resultFontsLoaded	= ::llc::rasterFontDefaults(g_managedFonts);
	(void)g_resultFontsLoaded;
	gui.Fonts.resize(g_managedFonts.Fonts.size());
	for(::llc::u2_t iFont = 0; iFont < g_managedFonts.Fonts.size(); ++iFont)
		gui.Fonts[iFont] = g_managedFonts.Fonts[iFont];

	llc_necs(llc::guiUpdateMetrics(gui, gui.LastSize, true));
	return 0;
}

::llc::cid_t			llc::controlCreateChild	(::llc::SGUI & gui, cid_t iParent)						{
	::llc::cid_t				idControl				= ::llc::controlCreate(gui);
	::llc::controlSetParent(gui, idControl, iParent);
	return idControl;
}

::llc::cid_t			llc::controlCreate		(::llc::SGUI & gui)							{
	llc_necs(::uiColorsSetupDefault(gui.Colors));

	if(0 == gui.Fonts.size())
		llc_necs(::setupDefaultFontTexture(gui));

	if(gui.Fonts.size() <= gui.SelectedFont)
		gui.SelectedFont = ::llc::u1_t(gui.Fonts.size() - 1);

	return ::llc::controlCreate(gui.Controls, gui.Colors->DefaultColors);
}

::llc::error_t			llc::controlDelete		(::llc::SGUI & gui, cid_t iControl, bool recursive)			{
	llc_necall(llc::controlInvalid(gui, iControl), "Invalid control id: %u.", iControl);
	info_printf("Deleting control id: %i.", iControl);
	{
		::llc::acid					children				= gui.Controls.Children[iControl];
		for(::llc::u2_t iChild = 0, countChild = children.size(); iChild < countChild; ++iChild) {
			if(recursive) {
				if_true_ef(failed(::llc::controlDelete(gui, children[iChild], true)), "Failed to delete control! Invalid control id? %i.", children[iChild]);
			}
			else {
				if_true_ef(failed(::llc::controlSetParent(gui, children[iChild], ::llc::CID_INVALID)), "Failed to detach child control: %i.", children[iChild]);
			}
		}
	}
	// Remove from parent list
	const ::llc::cid_t			indexParent				= gui.Controls.States[iControl].Parent;
#if defined(LLC_DEBUG_ENABLED)
	::llc::u2_t					childrenRemoved			= 0;
#endif
	if((::llc::u2_t)indexParent < gui.Controls.States.size() && false == gui.Controls.IsUnused(indexParent)) {
		::llc::acid					& children				= gui.Controls.Children[indexParent];
		for(::llc::u2_t iChild = 0; iChild < children.size(); ++iChild)
			if(children[iChild] == iControl) {
				llc_necall(children.remove_unordered(iChild), "Failed to remove child at index: %u.", iChild);
#if defined(LLC_DEBUG_ENABLED)
				--iChild;
				++childrenRemoved;
#else
				break;
#endif
			}
	}
#if defined(LLC_DEBUG_ENABLED)
	if_true_ef(childrenRemoved > 1, "Parent should not reference a child control more than once. References found: %i.", childrenRemoved);
#endif
	gui.Controls.SetUnused(iControl, true);
	return 0;
}

::llc::error_t			llc::controlSetParent	(::llc::SGUI & gui, cid_t iControl, cid_t iParent)	{
	llc_necall(llc::controlInvalid(gui, iControl), "Invalid control id: %u.", iControl);
	const ::llc::cid_t			oldParent				= gui.Controls.States[iControl].Parent;
	::llc::SControlState		& control				= gui.Controls.States[iControl];
	if(control.Parent == iParent)	// Exit early if there is nothing to do here.
		return 0;

	if_true_fef(::llc::CID_INVALID != iParent && ::llc::controlInvalid(gui, iParent), "Invalid parent control id: %u.", iParent);
	if_true_fef(iControl == iParent, "A control cannot be its own parent: %u.", iControl);
	for(::llc::cid_t ancestor = iParent, iDepth = 0; false == ::llc::controlInvalid(gui, ancestor) && iDepth < (::llc::cid_t)gui.Controls.States.size(); ++iDepth) {
		if_true_fef(ancestor == iControl, "Parent cycle for control: %u.", iControl);
		ancestor = gui.Controls.States[ancestor].Parent;
	}

	if((::llc::u2_t)oldParent < gui.Controls.States.size() && false == gui.Controls.IsUnused(oldParent)) {
		::llc::acid					& children				= gui.Controls.Children[oldParent];
		for(int32_t iChild = 0; iChild < (int32_t)children.size(); ++iChild)
			if(children[iChild] == iControl) {
				gui.Controls.SetUpdated(oldParent, false);
				llc_necall(children.remove(iChild), "Failed to remove child at index: %u.", iChild);
				break;
			}
	}
	control.Parent			= iParent;
	gui.Controls.SetUpdated(iControl, false);
	if(::llc::CID_INVALID == iParent)
		return 0;

	// Set iControl to parent's children array.
	::llc::acid					& children				= gui.Controls.Children[iParent];
	for(int32_t iChild = 0, countChild = children.size(); iChild < countChild; ++iChild)
		if(children[iChild] == iControl)
			return 0;

	gui.Controls.SetUpdated(iParent , false);
	llc_necs(children.push_back(iControl));
	return 0;
}

static	::llc::error_t	buildControlGeometry	(const ::llc::SControlPlacement & control, const ::llc::SControlArea & controlMetrics, const ::llc::SGUIZoom & zoom, ::llc::view<::llc::rect2s1_t> finalRects, ::llc::view<::llc::tri2s1_t> triangles)					{
	::llc::rects1_t				scaledBorders			= {};
	const ::llc::n2f3_t			scaleFinal				= zoom.DPI * zoom.ZoomLevel;
	scaledBorders.Left		= (int16_t)(control.Border.Left		* scaleFinal.x);
	scaledBorders.Top		= (int16_t)(control.Border.Top		* scaleFinal.y);
	scaledBorders.Right		= (int16_t)(control.Border.Right	* scaleFinal.x);
	scaledBorders.Bottom	= (int16_t)(control.Border.Bottom	* scaleFinal.y);
	(void)scaledBorders;

	const ::llc::rect2s1_t		& rectTotal				= controlMetrics.Total.Global;
	finalRects[::llc::GUI_CONTROL_AREA_OLD_CLIENT			]	= controlMetrics.Client.Global;
	finalRects[::llc::GUI_CONTROL_AREA_OLD_BORDER_LEFT		]	= {rectTotal.Offset , ::llc::n2s1_t{(int16_t)control.Border.Left, rectTotal.Size.y}};
	finalRects[::llc::GUI_CONTROL_AREA_OLD_BORDER_TOP		]	= {rectTotal.Offset , ::llc::n2s1_t{rectTotal.Size.x, (int16_t)control.Border.Top}};
	finalRects[::llc::GUI_CONTROL_AREA_OLD_BORDER_RIGHT		]	= {rectTotal.Offset + ::llc::n2s1_t{int16_t(rectTotal.Size.x - control.Border.Right), 0} , ::llc::n2s1_t{(int16_t)control.Border.Right, controlMetrics.Total.Global.Size.y}};
	finalRects[::llc::GUI_CONTROL_AREA_OLD_BORDER_BOTTOM	]	= {rectTotal.Offset + ::llc::n2s1_t{0, int16_t(rectTotal.Size.y - control.Border.Bottom)}, ::llc::n2s1_t{rectTotal.Size.x, (int16_t)control.Border.Bottom}};
	//finalRects[::llc::GUI_CONTROL_AREA_OLD_BACKGROUND		]	= rectTotal;
	finalRects[::llc::GUI_CONTROL_AREA_OLD_CLIENT			].Offset.x	-= control.Margin.Left	;
	finalRects[::llc::GUI_CONTROL_AREA_OLD_CLIENT			].Offset.y	-= control.Margin.Top	;
	finalRects[::llc::GUI_CONTROL_AREA_OLD_CLIENT			].Size	.x	+= control.Margin.Left	+ control.Margin.Right	;
	finalRects[::llc::GUI_CONTROL_AREA_OLD_CLIENT			].Size	.y	+= control.Margin.Top	+ control.Margin.Bottom	;

	// --- Draw control corners
	::llc::n2s1_t				startOffset				= controlMetrics.Total.Global.Offset;
	triangles[0]			= {startOffset, startOffset + ::llc::n2u1_t{control.Border.Left, control.Border.Top}.s1(), startOffset + ::llc::n2u1_t{control.Border.Left, 0U}.s1()	};
	triangles[1]			= {startOffset, startOffset + ::llc::n2u1_t{0U, control.Border.Top}.s1(), startOffset + ::llc::n2u1_t{control.Border.Left, control.Border.Top}.s1()	};

	int16_t						startOffsetX			= startOffset.x + controlMetrics.Total.Global.Size.x - control.Border.Right;
	startOffset				= {startOffsetX, controlMetrics.Total.Global.Offset.y};
	triangles[2]			= {startOffset, startOffset + ::llc::n2u1_t{0U, control.Border.Top}.s1(), startOffset + ::llc::n2u1_t{control.Border.Right, 0U}.s1()	};
	triangles[3]			= {startOffset + ::llc::n2u1_t{control.Border.Right, 0U}.s1(), startOffset + ::llc::n2u1_t{0U, control.Border.Top}.s1(), startOffset + ::llc::n2u1_t{control.Border.Right, control.Border.Top}.s1()	};

	int16_t						startOffsetY			= startOffset.y + controlMetrics.Total.Global.Size.y - control.Border.Bottom;
	startOffset				= {controlMetrics.Total.Global.Offset.x, startOffsetY};
	triangles[4]			= {startOffset, startOffset + ::llc::n2u1_t{0U, control.Border.Bottom}.s1(), startOffset + ::llc::n2u1_t{control.Border.Left, 0U}.s1()	};
	triangles[5]			= {startOffset + ::llc::n2u1_t{control.Border.Right, 0U}.s1(), startOffset + ::llc::n2u1_t{0U, control.Border.Top}.s1(), startOffset + ::llc::n2u1_t{control.Border.Right, control.Border.Top}.s1()	};

	startOffset				= controlMetrics.Total.Global.Offset + controlMetrics.Total.Global.Size - ::llc::n2u1_t{control.Border.Right, control.Border.Bottom}.s1();
	triangles[6]			= {startOffset, startOffset + ::llc::n2u1_t{control.Border.Right, control.Border.Bottom}.s1(), startOffset + ::llc::n2u1_t{control.Border.Right, 0U}.s1()	};
	triangles[7]			= {startOffset, startOffset + ::llc::n2u1_t{0U, control.Border.Bottom}.s1(), startOffset + ::llc::n2u1_t{control.Border.Right, control.Border.Bottom}.s1()	};
	return 0;
}

static	::llc::error_t	controlUpdateMetrics	(::llc::SGUI & gui, ::llc::cid_t iControl, const ::llc::n2<::llc::u1_t> & _targetSize)					{
	llc_necall(llc::controlInvalid(gui, iControl), "Invalid control id: %u.", iControl);

	::llc::SControlState		& controlState			= gui.Controls.States[iControl];
	const ::llc::SControlPlacement	& controlPlacement	= gui.Controls.Placement[iControl];
	::llc::n2f3_t				scale					= gui.Zoom.DPI * gui.Zoom.ZoomLevel;
	if(fabs(1.0 - scale.x) < 0.001) scale.x = 1.0;
	if(fabs(1.0 - scale.y) < 0.001) scale.y = 1.0;
	::llc::n2f3_t				scaledPosition			= {controlPlacement.Area.Offset	.x * scale.x, controlPlacement.Area.Offset	.y * scale.y};
	::llc::n2f3_t				scaledSize				= {controlPlacement.Area.Size	.x * scale.x, controlPlacement.Area.Size	.y * scale.y};
	//::llc::rect<double>	scaledBorders		= {(control.Border.Left * scale.x), (control.Border.Top * scale.y), (control.Border.Right * scale.x), (control.Border.Bottom * scale.y)};
	//::llc::rect<double>	scaledMargins		= {(control.Margin.Left * scale.x), (control.Margin.Top * scale.y), (control.Margin.Right * scale.x), (control.Margin.Bottom * scale.y)};

	::llc::SControlArea			& controlMetrics		= gui.Controls.Area[iControl];
	::llc::rect2f3_t				rectText				= {};
	::llc::SControlText			& controlText			= gui.Controls.Text[iControl];
	const ::llc::SRasterFont	& selectedFont			= *gui.Fonts[::llc::in_range(controlText.FontSelected, (int16_t)0, (int16_t)gui.Fonts.size()) ? controlText.FontSelected : gui.SelectedFont];

	const ::llc::n2u0_t			fontCharSize			= selectedFont.CharSize;
	rectText.Size			= {(double)(fontCharSize.x * controlText.Text.size()), (double)fontCharSize.y};

	const ::llc::SControlConstraints	& controlConstraints	= gui.Controls.Constraints[iControl];
	const ::llc::rectu1_t		ncSizes					= ::llc::controlNCRect(controlPlacement);
	const ::llc::rectf3_t		ncSizesScaled			= {(ncSizes.Left * scale.x), (ncSizes.Top * scale.y), (ncSizes.Right * scale.x), (ncSizes.Bottom * scale.y)};
	const ::llc::n2f3_t			ncTotalSize				= {(double)ncSizes.Left + ncSizes.Right, (double)ncSizes.Top + ncSizes.Bottom};
	const ::llc::n2f3_t			ncTotalSizeScaled		= ncTotalSize.GetScaled(scale.x, scale.y);
	if(controlConstraints.AttachSizeToText.x) scaledSize.x	= rectText.Size.x + ncTotalSizeScaled.x;
	if(controlConstraints.AttachSizeToText.y) scaledSize.y	= rectText.Size.y + ncTotalSizeScaled.y;
	const bool					isValidParent			= 0 == ::llc::controlInvalid(gui, controlState.Parent);
	const ::llc::n2u1_t			targetSize				= isValidParent ? gui.Controls.Area[controlState.Parent].Client.Global.Size.u1() : _targetSize.u1();

	if(controlConstraints.AttachSizeToControl.x == iControl) { if(controlConstraints.DockToControl.Left	!= -1) {} else { scaledPosition.x = 0; } scaledSize.x = targetSize.x; } else if(false == ::llc::controlInvalid(gui, controlConstraints.AttachSizeToControl.x)) { if(controlConstraints.DockToControl.Left != -1) {} else { scaledPosition.x = 0;} scaledSize.x = gui.Controls.Area[controlConstraints.AttachSizeToControl.x].Total.Global.Size.x; }
	if(controlConstraints.AttachSizeToControl.y == iControl) { if(controlConstraints.DockToControl.Top	!= -1) {} else { scaledPosition.y = 0; } scaledSize.y = targetSize.y; } else if(false == ::llc::controlInvalid(gui, controlConstraints.AttachSizeToControl.y)) { if(controlConstraints.DockToControl.Top  != -1) {} else { scaledPosition.y = 0;} scaledSize.y = gui.Controls.Area[controlConstraints.AttachSizeToControl.y].Total.Global.Size.y; }

	::llc::n2f3_t				minSizeScaled			= (controlConstraints.SizeMinMax.Min.f3().InPlaceScale(scale.x, scale.y));
	::llc::n2f3_t				maxSizeScaled			= (controlConstraints.SizeMinMax.Max.f3().InPlaceScale(scale.x, scale.y));
	scaledSize.x			= ::llc::max(minSizeScaled.x, ::llc::min(maxSizeScaled.x, scaledSize.x));
	scaledSize.y			= ::llc::max(minSizeScaled.y, ::llc::min(maxSizeScaled.y, scaledSize.y));
	controlMetrics.Client.Local	= {{(int16_t)ncSizesScaled.Left, (int16_t)ncSizesScaled.Top}, (scaledSize - ncTotalSizeScaled).s1()};
	::llc::realignRectangle(targetSize.u2(), ::llc::rect2<int16_t>{scaledPosition.s1(), scaledSize.s1()}, controlMetrics.Total.Local, controlPlacement.Align);
	controlMetrics.Total .Global	= controlMetrics.Total	.Local;
	controlMetrics.Client.Global	= controlMetrics.Client	.Local;
	controlMetrics.Client.Global.Offset	+= controlMetrics.Total	.Local.Offset;
	if(isValidParent) {
		::llc::SControlArea		& parentMetrics			= gui.Controls.Area[controlState.Parent];
		controlMetrics.Client.Global.Offset	+= parentMetrics.Client.Global.Offset;
		controlMetrics.Total .Global.Offset	+= parentMetrics.Client.Global.Offset;
	}

	const ::llc::rectcid		& dockToControl			= controlConstraints.DockToControl;
	if(dockToControl.Right != -1) {
		llc_necall(llc::controlInvalid(gui, dockToControl.Right), "Invalid control id: %i.", dockToControl.Right);
		::llc::controlUpdateMetricsTopToDown(gui, dockToControl.Right, targetSize.u1(), true);
		const ::llc::SControlPlacement	& other				= gui.Controls.Placement[dockToControl.Right];
		const ::llc::SControlArea		& otherMetrics		= gui.Controls.Area	[dockToControl.Right];
		if(::llc::bit_true(other.Align, ::llc::ALIGN_RIGHT) && ::llc::bit_false(other.Align, ::llc::ALIGN_HCENTER)) {
			controlMetrics.Total .Global.Offset.x	= otherMetrics.Total.Global.Offset.x - controlMetrics.Total.Global.Size.x;
			controlMetrics.Client.Global.Offset.x	= (int16_t)(controlMetrics.Total.Global.Offset.x + ncSizesScaled.Left);
			int16_t						diffToSubstract		= targetSize.x - otherMetrics.Total.Local.Offset.x;
			if(controlConstraints.AttachSizeToControl.x == iControl) {
				controlMetrics.Total .Global.Size.x		-= diffToSubstract;
				controlMetrics.Client.Global.Size.x		-= diffToSubstract;
				controlMetrics.Total .Global.Offset.x	+= diffToSubstract;
				controlMetrics.Client.Global.Offset.x	+= diffToSubstract;
			}
		}
		else {
			controlMetrics.Total .Global.Offset.x	= otherMetrics.Total.Global.Offset.x + otherMetrics.Total.Global.Size.x;
			controlMetrics.Client.Global.Offset.x	= (int16_t)(controlMetrics.Total.Global.Offset.x + ncSizesScaled.Left);
		}
	}
	if(dockToControl.Bottom != -1) {
		llc_necall(llc::controlInvalid(gui, dockToControl.Bottom), "Invalid control id: %i.", dockToControl.Bottom);
		const ::llc::SControlPlacement	& other					= gui.Controls.Placement[dockToControl.Bottom];
		const ::llc::SControlArea		& otherMetrics			= gui.Controls.Area[dockToControl.Bottom	];
		if(::llc::bit_true(other.Align, ::llc::ALIGN_BOTTOM	) && ::llc::bit_false(other.Align, ::llc::ALIGN_VCENTER)) {
			controlMetrics.Total.Global.Offset.y	= otherMetrics.Total.Global.Offset.y - controlMetrics.Total.Global.Size.y;
			controlMetrics.Client.Global.Offset.y	= (int16_t)(controlMetrics.Total.Global.Offset.y + ncSizesScaled.Top);
		} else {
			controlMetrics.Total.Global.Offset.y	= otherMetrics.Total.Global.Offset.y + otherMetrics.Total.Global.Size.y;
			controlMetrics.Client.Global.Offset.y	= (int16_t)(controlMetrics.Total.Global.Offset.y + ncSizesScaled.Top);
		}
	}
	if(dockToControl.Left != -1) {
		llc_necall(llc::controlInvalid(gui, dockToControl.Left), "Invalid control id: %i.", dockToControl.Left);
		::llc::controlUpdateMetricsTopToDown(gui, dockToControl.Left, targetSize, true);
		const ::llc::SControlPlacement	& other			= gui.Controls.Placement[dockToControl.Left];
		const ::llc::SControlArea		& otherMetrics	= gui.Controls.Area	[dockToControl.Left];
		if(::llc::bit_true(other.Align, ::llc::ALIGN_RIGHT) && ::llc::bit_false(other.Align, ::llc::ALIGN_HCENTER)) {
			controlMetrics.Total	.Global.Offset.x	= otherMetrics				.Total.Global.Offset.x + otherMetrics.Total.Global.Size.x - controlMetrics.Total.Global.Size.x;
			controlMetrics.Client	.Global.Offset.x	= int16_t(controlMetrics	.Total.Global.Offset.x + ncSizesScaled.Left);
		}
		else {
			controlMetrics.Total	.Global.Offset.x	= otherMetrics				.Total.Global.Offset.x;
			controlMetrics.Client	.Global.Offset.x	= int16_t(controlMetrics	.Total.Global.Offset.x + ncSizesScaled.Left);
		}
	}
	if(dockToControl.Top	!= -1) {
		llc_necall(llc::controlInvalid(gui, dockToControl.Top), "Invalid control id: %i.", dockToControl.Top);
		const ::llc::SControlPlacement	& other					= gui.Controls.Placement[dockToControl.Top ];
		const ::llc::SControlArea		& otherMetrics			= gui.Controls.Area	[dockToControl.Top ];
		if(::llc::bit_true(other.Align, ::llc::ALIGN_BOTTOM	) && ::llc::bit_false(other.Align, ::llc::ALIGN_VCENTER)) {
			controlMetrics.Total.Global.Offset.y	= otherMetrics.Total.Global.Offset.y - controlMetrics.Total.Global.Size.y;
			controlMetrics.Client.Global.Offset.y	= controlMetrics.Total.Global.Offset.y + ncSizes.Top ;
		}
		else {
			controlMetrics.Total.Global.Offset.y	= otherMetrics.Total.Global.Offset.y + controlMetrics.Total.Global.Offset.y;
			controlMetrics.Client.Global.Offset.y	= controlMetrics.Total.Global.Offset.y + ncSizes.Top ;
		}
	}
	{ // calculate text rectangle
		const ::llc::rect2s1_t		& targetRect			= controlMetrics.Client.Global;
		controlMetrics.Text.Offset	= {};
		controlMetrics.Text.Size	= rectText.Size.s1();
		::llc::realignRectangle(targetRect.Size.u2(), controlMetrics.Text, controlMetrics.Text, controlText.Align);
		controlMetrics.Text.Offset	+= controlMetrics.Client.Global.Offset.s1();
	}
	::buildControlGeometry(controlPlacement, controlMetrics, gui.Zoom, controlMetrics.Rectangles, controlMetrics.Triangles);
	gui.Controls.SetUpdated(iControl, true);
	
	return 0;
}

::llc::error_t			llc::controlUpdateMetricsTopToDown							(::llc::SGUI & gui, ::llc::cid_t iControl, const ::llc::n2<::llc::u1_t> & targetSize, bool forceUpdate)				{
	if(false == gui.Controls.IsUpdated(iControl) || forceUpdate)
		llc_necall(::controlUpdateMetrics(gui, iControl, targetSize), "Failed to update metrics for control %i", iControl);

	::llc::vcid					& children												= gui.Controls.Children[iControl];
	for(::llc::u2_t iChild = 0; iChild < children.size(); ++iChild)
		llc_necall(llc::controlUpdateMetricsTopToDown(gui, children[iChild], targetSize, forceUpdate), "Failed to update metrics for control %i", children[iChild]);
	return 0;
}

::llc::error_t			llc::guiUpdateMetrics									(::llc::SGUI & gui, const ::llc::n2<::llc::u1_t> & targetSize, bool forceUpdate)								{
	for(::llc::u2_t iControl = 0; iControl < gui.Controls.States.size(); ++iControl)
		if(::llc::controlInvalid(gui, gui.Controls.States[iControl].Parent) && false == ::llc::controlInvalid(gui, (::llc::cid_t)iControl))
			llc_necall(llc::controlUpdateMetricsTopToDown(gui, (::llc::cid_t)iControl, targetSize, forceUpdate), "Failed to update metrics for control %i", iControl);
	return 0;
}

static	::llc::cid_t	updateGUIControlHovered	(::llc::SGUI & gui, ::llc::cid_t iControl, ::llc::SControlState & controlState, const ::llc::SInput & inputSystem, bool disabled)	noexcept	{
	if(controlState.IsHovered()) {
		if(inputSystem.ButtonDown(0) && false == controlState.IsPressed())
			gui.Controls.SetPressed(iControl, true);
		else if(inputSystem.ButtonUp(0))
			gui.Controls.SetPressed(iControl, false);
	}
	else {
		if(false == disabled) {
			gui.Controls.SetHovered(iControl, true);//controlFlags.Disabled;
		}
	}
	return controlState.IsHovered() ? iControl : -1;
}

static	::llc::cid_t	controlProcessInput		(::llc::SGUI & gui, const ::llc::SInput & input, ::llc::cid_t iControl)														{
	//--------------------
	::llc::SControlState		& controlState			= gui.Controls.States[iControl];
	::llc::cid_t				controlHovered			= -1;
	if(::llc::in_range(gui.CursorPos.s1(), gui.Controls.Area[iControl].Total.Global))
		controlHovered			= ::updateGUIControlHovered(gui, iControl, controlState, input, ::llc::controlDisabled(gui, iControl) || ::llc::controlHidden(gui, iControl));
	else {
		gui.Controls.SetHovered(iControl, false);
		if(input.ButtonUp(0))
			gui.Controls.SetPressed(iControl, false);
	}
	{
		::llc::vcid					& children				= gui.Controls.Children[iControl];
		for(::llc::u2_t iChild = 0, countChild = children.size(); iChild < countChild; ++iChild) {
			if(::llc::controlHidden(gui, children[iChild]))
				continue;
			::llc::cid_t				controlPressed			= ::controlProcessInput(gui, input, children[iChild]);
			if(gui.Controls.States.size() > (::llc::u2_t)controlPressed) {
				gui.Controls.SetHovered(iControl, false);
				controlHovered			= controlPressed;
			}
		}
	}
	return controlHovered;
}

::llc::error_t			llc::guiProcessInput	(::llc::SGUI & gui, const ::llc::SInput & input, ::llc::vpobj<::llc::SEventSystem> sysEvents)	{
	if(0 == gui.LastSize.LengthSquared()) 
		return 0;

	gui.Controls.EventQueue.clear();
	::llc::cid_t				controlHovered			= -1;
	::llc::acid					rootControlsToProcess	= {};
	rootControlsToProcess.reserve(1000);
	(void)sysEvents;
	for(::llc::u2_t iControl = 0, countControls = gui.Controls.States.size(); iControl < countControls; ++iControl) {	// Only process root parents
		const ::llc::SControlState	& controlState			= gui.Controls.States[iControl];
		if(controlState.IsUnused())
			continue;

		gui.Controls.SetAction((::llc::cid_t)iControl, false);

		if(controlState.IsDisabled())
			continue;

		if(::llc::controlHidden(gui, (::llc::cid_t)iControl))
			continue;

		if(false == ::llc::controlInvalid(gui, controlState.Parent))
			continue;

		rootControlsToProcess.push_back((::llc::cid_t)iControl);
	}

	gui.CursorPos			= {(float)input.MouseCurrent.Position.x, (float)input.MouseCurrent.Position.y}; //+= {(float)input.MouseCurrent.Deltas.x, (float)input.MouseCurrent.Deltas.y};
	for(::llc::u2_t iControl = 0, countControls = rootControlsToProcess.size(); iControl < countControls; ++iControl) {
		if(::llc::controlHidden(gui, rootControlsToProcess[iControl]))
			continue;

		::llc::cid_t				controlPressed		= ::controlProcessInput(gui, input, rootControlsToProcess[iControl]);
		if(gui.Controls.States.size() > (::llc::u2_t)controlPressed)
			controlHovered			= controlPressed;
	}
	if(controlHovered == -1)
		return gui.Controls.States.size();

	for(::llc::u2_t iControl = 0, countControls = gui.Controls.States.size(); iControl < countControls; ++iControl) {
		if(iControl != (::llc::u2_t)controlHovered) {
			gui.Controls.SetHovered((::llc::cid_t)iControl, false);;
			if(0 == input.MouseCurrent.ButtonState[0])
				gui.Controls.SetPressed((::llc::cid_t)iControl, false);
		}
	}

	return controlHovered;
}

::llc::error_t			llc::guiDeselect		(::llc::SGUI & gui)	{
	for(::llc::u2_t iControl = 0; iControl < gui.Controls.States.size(); ++iControl)
		gui.Controls.SetSelected((::llc::cid_t)iControl, false);

	return 0;
}

::llc::error_t			llc::guiSelect			(::llc::SGUI & gui, ::llc::cid_t controlToSelect)	{
	for(::llc::u2_t iControl = 0; iControl < gui.Controls.States.size(); ++iControl) 
		gui.Controls.SetSelected((::llc::cid_t)iControl, controlToSelect == (::llc::cid_t)iControl);

	return 0;
}

::llc::error_t			llc::controlHidden		(const ::llc::SGUI & gui, ::llc::cid_t iControl)	{
	bool						imHidden				= ::llc::controlInvalid(gui, iControl) || gui.Controls.States[iControl].IsHidden();
	return imHidden ? imHidden : (0 == ::llc::controlInvalid(gui, gui.Controls.States[iControl].Parent) && ::llc::controlHidden(gui, gui.Controls.States[iControl].Parent));
}

::llc::error_t			llc::controlDisabled	(const ::llc::SGUI & gui, ::llc::cid_t iControl)	{
	bool						imDisabled				= ::llc::controlInvalid(gui, iControl) || gui.Controls.States[iControl].IsDisabled();
	return imDisabled ? imDisabled : (false == ::llc::controlInvalid(gui, gui.Controls.States[iControl].Parent) && ::llc::controlDisabled(gui, gui.Controls.States[iControl].Parent));
}

::llc::error_t			llc::controlMetricsInvalidate	(::llc::SGUI & gui, ::llc::cid_t iControl)	{
	gui.Controls.SetUpdated(iControl, false);
	const ::llc::vcid			& controlChildren		= gui.Controls.Children[iControl];
	for(::llc::u2_t iChild = 0, countChild = controlChildren.size(); iChild < countChild; ++iChild)
		llc_necall(llc::controlMetricsInvalidate(gui, controlChildren[iChild]), "%s", "Invalid child?");

	return 0;
}

::llc::error_t			llc::guiGetProcessableControls	(const ::llc::SGUI & gui, ::llc::acid & controlIndices)													{
	for(::llc::u2_t iControl = 0, countControls = gui.Controls.States.size(); iControl < countControls; ++iControl) {	// Only process root parents
		const ::llc::SControlState	& controlState					= gui.Controls.States[iControl];
		if(controlState.IsUnused() || ::llc::controlDisabled(gui, (::llc::cid_t)iControl) || ::llc::controlHidden(gui, (::llc::cid_t)iControl))
			continue;
		llc_necs(controlIndices.push_back((::llc::cid_t)iControl));
	}
	return controlIndices.size();
}

::llc::error_t			llc::guiProcessControls		(const ::llc::SGUI & gui, const ::llc::function<::llc::error_t(::llc::cid_t iControl)> & funcOnExecute) {
	::llc::acid					controlsToProcess			= {};
	llc_necs(llc::guiGetProcessableControls(gui, controlsToProcess));
	return guiProcessControls(gui, controlsToProcess, funcOnExecute);
}

::llc::error_t			llc::guiProcessControls		(const ::llc::SGUI & gui, ::llc::vcid controlsToProcess, const ::llc::function<::llc::error_t(::llc::cid_t iControl)> & funcOnExecute) {
	for(::llc::u2_t iControl = 0, countControls = controlsToProcess.size(); iControl < countControls; ++iControl) {
		const ::llc::cid_t			idControl					= controlsToProcess[iControl];
		const ::llc::SControlState	& controlState				= gui.Controls.States[idControl];
		if(controlState.IsAction()) {
			info_printf("Executed control %i (0x%x).", idControl, idControl);
			return funcOnExecute(idControl);
		}
	}
	return 0;
}

::llc::error_t			llc::controlTextSet		(::llc::SGUI & gui, ::llc::cid_t iControl, const ::llc::vcs & text)				{
	::llc::vcs					& oldText				= gui.Controls.Text[iControl].Text;
	if(text != oldText) {
		oldText					= text;
		gui.Controls.SetUpdated(iControl, false);
	}
	return 0;
}

::llc::error_t			llc::controlFontSet		(::llc::SGUI & gui, ::llc::cid_t iControl, int16_t iFont)				{
	int16_t						& oldText				= gui.Controls.Text[iControl].FontSelected;
	if(iFont != oldText) {
		oldText					= iFont;
		gui.Controls.SetUpdated(iControl, false);
	}
	return 0;
}

static	::llc::error_t	controlInstanceReset	(::llc::SControlTable & controlTable, ::llc::cid_t iControl, ::llc::vi16 defaultColors)										{
	controlTable.States		[iControl]	= {};
	controlTable.Placement	[iControl]	= {};
	controlTable.Area		[iControl]	= {};
	controlTable.Constraints[iControl]	= {};
	controlTable.Draw		[iControl]	= {};
	controlTable.Text		[iControl]	= {};
	controlTable.Children	[iControl]	= ::llc::vcid{};
	controlTable.Images		[iControl]	= {};
	
	::llc::SControlDraw			& controlDraw			= controlTable.Draw[iControl];
	for(::llc::u2_t iColor = 0, countColors = ::llc::min(controlDraw.Palettes.size(), defaultColors.size()); iColor < countColors; ++iColor)
		controlDraw.Palettes[iColor] = defaultColors[iColor];

	return 0;
}


::llc::cid_t			llc::controlCreate		(::llc::SControlTable & controlTable, ::llc::vi16 defaultColors)	{
	for(::llc::u2_t iControl = 0; iControl < controlTable.States.size(); ++iControl) {
		if(controlTable.IsUnused((::llc::cid_t)iControl)) {
			llc_necall(::controlInstanceReset(controlTable, (::llc::cid_t)iControl, defaultColors), "iControl: %i", iControl);
			return (::llc::cid_t)iControl;
		}
	}

	::llc::cid_t				iControl				= -1;
	llc_necs(iControl = (::llc::cid_t)::llc::resize(controlTable.States.size() + 1
		, controlTable.States
		, controlTable.Placement
		, controlTable.Area
		, controlTable.Constraints
		, controlTable.Draw
		, controlTable.Text
		, controlTable.Images
		, controlTable.Children
		) - 1);
	llc_necs(::controlInstanceReset(controlTable, iControl, defaultColors));
	return iControl;
}
