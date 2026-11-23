#include "test_llc_gui_control_corpus.h"

::llc::err_t	dry::setupPalettes		(::llc::SControlColors	& colors) {
	auto			& paletteNormal			= colors.Palettes[colors.DefaultColors[::llc::GUI_CONTROL_PALETTE_NORMAL	]];
	auto			& paletteDisabled		= colors.Palettes[colors.DefaultColors[::llc::GUI_CONTROL_PALETTE_DISABLED]];
	auto			& palettePressed			= colors.Palettes[colors.DefaultColors[::llc::GUI_CONTROL_PALETTE_PRESSED	]];
	auto			& paletteHover			= colors.Palettes[colors.DefaultColors[::llc::GUI_CONTROL_PALETTE_HOVER	]];
	auto			& paletteSelected			= colors.Palettes[colors.DefaultColors[::llc::GUI_CONTROL_PALETTE_SELECTED]];

	::llc::view<::llc::bgra>{paletteNormal}.fill(::llc::WHITE);
	paletteNormal[::llc::UI_CONTROL_AREA_BACKGROUND			] = ::llc::BLACK;
	paletteNormal[::llc::UI_CONTROL_AREA_CLIENT				] = ::llc::BLACK;

	::llc::view<::llc::bgra>{paletteDisabled}.fill(::llc::GRAY);
	paletteDisabled[::llc::UI_CONTROL_AREA_BACKGROUND			] = ::llc::DARKGRAY;
	paletteDisabled[::llc::UI_CONTROL_AREA_CLIENT				] = ::llc::DARKGRAY;

	::llc::view<::llc::bgra>{palettePressed}.fill(::llc::ORANGE);
	palettePressed[::llc::UI_CONTROL_AREA_BACKGROUND			] = ::llc::DARKBLUE / 2.f;
	palettePressed[::llc::UI_CONTROL_AREA_CLIENT				] = ::llc::DARKBLUE / 2.f;

	::llc::view<::llc::bgra>{paletteHover}.fill(::llc::ORANGE *.5 + ::llc::WHITE * .5);
	paletteHover[::llc::UI_CONTROL_AREA_BORDER_LEFT			] =
	paletteHover[::llc::UI_CONTROL_AREA_BORDER_TOP			] =
	paletteHover[::llc::UI_CONTROL_AREA_BORDER_RIGHT			] =
	paletteHover[::llc::UI_CONTROL_AREA_BORDER_BOTTOM			] = ::llc::WHITE;
	paletteHover[::llc::UI_CONTROL_AREA_BACKGROUND			] = ::llc::DARKBLUE;
	paletteHover[::llc::UI_CONTROL_AREA_CLIENT				] = ::llc::DARKBLUE;


	::llc::view<::llc::bgra>{paletteSelected}.fill(::llc::ORANGE);
	paletteSelected[::llc::UI_CONTROL_AREA_BACKGROUND			] = ::llc::BLACK;
	paletteSelected[::llc::UI_CONTROL_AREA_CLIENT				] = ::llc::BLACK;

	return 0;
}
