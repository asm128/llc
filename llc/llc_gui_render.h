#include "llc_gui.h"
#include "llc_img_color.h"
#include "llc_grid_color.h"
#include "llc_font.h"
#include "llc_event.h"

#ifndef LLC_GUI_RENDER_H_23627
#define LLC_GUI_RENDER_H_23627

namespace llc
{
	GDEFINE_ENUM_TYPE(GUI_RENDER_COMMAND, u0_t);
	GDEFINE_ENUM_VALUE(GUI_RENDER_COMMAND, Rectangle	, 0);
	GDEFINE_ENUM_VALUE(GUI_RENDER_COMMAND, Triangle	, 1);
	GDEFINE_ENUM_VALUE(GUI_RENDER_COMMAND, Image		, 2);
	GDEFINE_ENUM_VALUE(GUI_RENDER_COMMAND, Text		, 3);

#pragma pack(push, 1)
	struct SGUIRenderStyle {
		cid_t					Control			= CID_INVALID;
		u2_t					Generation		= 0;
		s1_t					ColorSet		= 0;
		GUI_CONTROL_PALETTE		Palette			= GUI_CONTROL_PALETTE_NORMAL;
		UI_CONTROL_AREA			Area			= UI_CONTROL_AREA_BACKGROUND;
		GUI_COLOR_MODE			ColorMode		= GUI_COLOR_MODE_Default;
		bool					DirectPalette	= false;
		bool					Pressed			= false;
		bool					Hovered			= false;
	};

	struct SGUIRenderRectangle {
		SGUIRenderStyle			Style			= {};
		rect2s1_t				Rectangle		= {};
	};

	struct SGUIRenderTriangle {
		SGUIRenderStyle			Style			= {};
		tri2s1_t				Triangle		= {};
	};

	struct SGUIRenderImage {
		cid_t					Control			= CID_INVALID;
		u2_t					Generation		= 0;
		rect2s1_t				Client			= {};
		bool					Hovered			= false;
	};

	struct SGUIRenderText {
		SGUIRenderStyle			Style			= {};
		rect2s1_t				Rectangle		= {};
		s1_t					Font			= -1;
	};

	struct SControlRectangleGPU {
		rect2f2_t					Local				= {};
		rect2f2_t					Global				= {};
	};

	struct SControlAreaGPU {
		SControlRectangleGPU		Total				= {};
		SControlRectangleGPU		Client				= {};
		rect2f2_t					Text				= {};
		rect2f2_t					Rectangles	[GUI_CONTROL_AREA_OLD_COUNT]	= {};
		tri2f2_t					Triangles	[8]		= {};
	};

	struct SControlTheme {
		astatic<astu32<UI_CONTROL_AREA_COUNT>, GUI_CONTROL_PALETTE_COUNT>
									ColorCombos			= {};
	};
#pragma pack(pop)

	tydf SEvent<GUI_RENDER_COMMAND>	SEventGUIRender;
	tydf aobj<SEventGUIRender>		TGUIRenderEventQueue;

	struct SControlImage {
		rt<bgra, u2_t>				Temp				= {};
		g8bgra						Image				= {};
		n2s1_t						ImageOffset			= {};
		uint8_t						ImageAlign			= ALIGN_CENTER;
		uint8_t						ImageInvertX		: 1;
		uint8_t						ImageInvertY		: 1;
		uint8_t						Rotation			: 2;
	};

	struct SGUIColors {
		papod<bgra>						Palette					= {};
		papod<SControlTheme>				ControlThemes			= {};
		apod<astatic<bgra, UI_CONTROL_AREA_COUNT>>
									Palettes				= {};
	};

	struct SGUIRenderResources {
		pobj<SGUIColors>					Colors					= {};
		apobj<SRasterFont>					Fonts					= {};
		aobj<SControlImage>				Images					= {};
		apod<u2_t>						ImageGenerations		= {};
	};

	GUI_CONTROL_PALETTE	controlPaletteIndex			(bool disabled, const SControlState & controlState) noexcept;
	error_t				guiRenderResourcesSync		(SGUIRenderResources & resources, const SGUI & gui);
	error_t				guiMeasureTextRaster		(const SGUIRenderResources & resources, s1_t font, vcst_t text, n2u1_t & size);
	error_t				guiRenderEvents				(SGUI & gui, TGUIRenderEventQueue & output, n2u1_c & targetSize, const FGUITextMeasure & textMeasure);
	error_t				guiRenderEvents				(SGUI & gui, TGUIRenderEventQueue & output, n2u1_c & targetSize);
	error_t				guiDraw						(const TGUIRenderEventQueue & events, SGUIRenderResources & resources, g8bgra target);
	error_t				guiDraw						(SGUI & gui, SGUIRenderResources & resources, g8bgra target);
} // namespace

#endif // LLC_GUI_RENDER_H_23627
