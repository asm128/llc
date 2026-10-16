#include "llc_gui_control_state.h"

#include "llc_tri2.h"
#include "llc_rect.h"
#include "llc_rect2.h"
#include "llc_align.h"
#include "llc_array_static.h"

#include "llc_img_color.h"
#include "llc_grid_color.h"
#include "llc_minmax_n2.h"

#include "llc_axis.h"

#ifndef LLC_GUI_CONTROL_H_23627
#define LLC_GUI_CONTROL_H_23627

namespace llc
{
#pragma pack(push, 1)
	typedef	n2<cid_t>		n2cid;
	typedef	rect<cid_t>		rectcid;

	tplt<uint32_t _sizeArray>	
	using	astcid	= ::llc::astatic<cid_t, _sizeArray>;

	GDEFINE_ENUM_TYPE(GUI_COLOR_MODE, uint8_t)
	GDEFINE_ENUM_VALUE(GUI_COLOR_MODE, Default	, 0);
	GDEFINE_ENUM_VALUE(GUI_COLOR_MODE, 3D		, 1);
	GDEFINE_ENUM_VALUE(GUI_COLOR_MODE, Flat		, 2);

	GDEFINE_ENUM_TYPE(GUI_CONTROL_LIMIT, uint8_t);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_LIMIT, Left		, ::llc::ALIGN_LEFT		);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_LIMIT, Top		, ::llc::ALIGN_TOP		);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_LIMIT, Right		, ::llc::ALIGN_RIGHT	);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_LIMIT, Bottom	, ::llc::ALIGN_BOTTOM	);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_LIMIT, Outer		, ::llc::ALIGN_FAR		);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_LIMIT, Inner		, ::llc::ALIGN_NEAR		);
	

	GDEFINE_ENUM_TYPE (GUI_CONTROL_METRICS_FLAG, uint16_t);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_METRICS_FLAG, XSize		, 1 << 0);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_METRICS_FLAG, YSize		, 1 << 1);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_METRICS_FLAG, XSizeMin	, 1 << 2);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_METRICS_FLAG, YSizeMin	, 1 << 3);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_METRICS_FLAG, XSizeMax	, 1 << 4);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_METRICS_FLAG, YSizeMax	, 1 << 5);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_METRICS_FLAG, XOffset	, 1 << 6);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_METRICS_FLAG, YOffset	, 1 << 7);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_METRICS_FLAG, XOffsetMin	, 1 << 8);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_METRICS_FLAG, YOffsetMin	, 1 << 9);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_METRICS_FLAG, XOffsetMax	, 1 << 10);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_METRICS_FLAG, YOffsetMax	, 1 << 11);

	struct SControlPlacement {
		::llc::rect2s1_t				Area				= {{}, {16, 16}};
		::llc::rectu1_t				Margin				= {1, 1, 1, 1};
		::llc::rectu1_t				Border				= {1, 1, 1, 1};
		::llc::ALIGN				Align				= ::llc::ALIGN_TOP_LEFT;
		GUI_CONTROL_LIMIT			RelativeBorderSize	: 4;
		GUI_CONTROL_LIMIT			RelativeMarginSize	: 4;
		GUI_CONTROL_METRICS_FLAG	RelativeMetrics		= {};
	}; 

	stincxp	::llc::rectu1_t	controlNCRect		(const ::llc::SControlPlacement & ctl)	noexcept	{ return ctl.Border + ctl.Margin; }
	stacxpr	::llc::n2u1_t	controlNCSpacing	(const ::llc::SControlPlacement & ctl)	noexcept	{ const rectu1_t rc = controlNCRect(ctl); return ::llc::n2s2_t{rc.Left + rc.Right, rc.Top + rc.Bottom}.u1(); }
	stincxp	void			controlNCSpacing	(const ::llc::SControlPlacement & ctl, ::llc::rectu1_t & ncSpacing)	noexcept	{ ncSpacing = controlNCRect(ctl); }
	stincxp	void			controlNCSpacing	(const ::llc::SControlPlacement & ctl, ::llc::n2u1_t   & ncSpacing)	noexcept	{ ncSpacing = controlNCSpacing(ctl); }

	GDEFINE_ENUM_TYPE(GUI_CONTROL_PALETTE, uint8_t)
	GDEFINE_ENUM_VALUE(GUI_CONTROL_PALETTE, NORMAL				, 0);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_PALETTE, DISABLED			, 1);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_PALETTE, HOVER				, 2);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_PALETTE, PRESSED				, 3);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_PALETTE, SELECTED			, 4);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_PALETTE, SELECTED_DISABLED	, 5);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_PALETTE, SELECTED_HOVER		, 6);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_PALETTE, SELECTED_PRESSED	, 7);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_PALETTE, ACTION				, 8);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_PALETTE, OUTDATED			, 9);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_PALETTE, COUNT				, 10);
	

	struct SControlDraw {
		uint8_t			ColorMode			: 2;
		uint8_t			FrameOut			: 1;
		uint8_t			NoBorder			: 1;	// skip drawing border area
		uint8_t			NoClient			: 1;	// skip drawing client area
		
		bool			UseNewPalettes		: 1;	// -- Legacy property (don't use in the new implementation!)

		int16_t			ColorTheme			= 0;
		::llc::asts1_t<::llc::GUI_CONTROL_PALETTE_COUNT>	
						Palettes			= {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9,}};
	};

	enum CONTROL_AREA_BASE : uint8_t 
		{ CONTROL_AREA_BASE_BACKGROUND		= 0
		, CONTROL_AREA_BASE_CONTENT
		, CONTROL_AREA_BASE_BORDER_LEFT
		, CONTROL_AREA_BASE_BORDER_TOP
		, CONTROL_AREA_BASE_BORDER_RIGHT
		, CONTROL_AREA_BASE_BORDER_BOTTOM
		, CONTROL_AREA_BASE_CORNER_TOP_LEFT
		, CONTROL_AREA_BASE_CORNER_RIGHT_TOP
		, CONTROL_AREA_BASE_CORNER_BOTTOM_RIGHT
		, CONTROL_AREA_BASE_CORNER_LEFT_BOTTOM
		, CONTROL_AREA_BASE_COUNT
		};

	enum GUI_CONTROL_AREA_OLD : uint8_t
		{ GUI_CONTROL_AREA_OLD_CLIENT		= 0
		, GUI_CONTROL_AREA_OLD_BORDER_LEFT
		, GUI_CONTROL_AREA_OLD_BORDER_TOP
		, GUI_CONTROL_AREA_OLD_BORDER_RIGHT
		, GUI_CONTROL_AREA_OLD_BORDER_BOTTOM
		, GUI_CONTROL_AREA_OLD_COUNT
		};


	enum UI_CONTROL_AREA : uint8_t
		{ UI_CONTROL_AREA_BACKGROUND		= 0
		, UI_CONTROL_AREA_CLIENT
		, UI_CONTROL_AREA_BORDER_LEFT
		, UI_CONTROL_AREA_BORDER_TOP
		, UI_CONTROL_AREA_BORDER_RIGHT
		, UI_CONTROL_AREA_BORDER_BOTTOM
		, UI_CONTROL_AREA_TEXT_BACKGROUND
		, UI_CONTROL_AREA_TEXT_FACE
		, UI_CONTROL_AREA_MARGIN_LEFT
		, UI_CONTROL_AREA_MARGIN_TOP
		, UI_CONTROL_AREA_MARGIN_RIGHT
		, UI_CONTROL_AREA_MARGIN_BOTTOM
		, UI_CONTROL_AREA_CORNER_LEFT_TOP_TOP
		, UI_CONTROL_AREA_CORNER_LEFT_TOP_LEFT
		, UI_CONTROL_AREA_CORNER_RIGHT_TOP_TOP
		, UI_CONTROL_AREA_CORNER_RIGHT_TOP_RIGHT
		, UI_CONTROL_AREA_CORNER_LEFT_BOTTOM_LEFT
		, UI_CONTROL_AREA_CORNER_LEFT_BOTTOM_BOTTOM
		, UI_CONTROL_AREA_CORNER_RIGHT_BOTTOM_RIGHT
		, UI_CONTROL_AREA_CORNER_RIGHT_BOTTOM_BOTTOM
		, UI_CONTROL_AREA_COUNT
		};

	struct SControlRectangle {
		::llc::rect2s1_t				Local				= {};
		::llc::rect2s1_t				Global				= {};
	};

	struct SControlArea {
		::llc::SControlRectangle	Total				= {};
		::llc::SControlRectangle	Client				= {};
		::llc::rect2s1_t				Text				= {};
		::llc::rect2s1_t				Rectangles	[::llc::GUI_CONTROL_AREA_OLD_COUNT]	= {};
		::llc::tri2s1_t				Triangles	[8]		= {};
	};

	struct SControlRectangleGPU {
		::llc::rect2f2_t				Local				= {};
		::llc::rect2f2_t				Global				= {};
	};

	struct SControlAreaGPU {
		::llc::SControlRectangleGPU	Total				= {};
		::llc::SControlRectangleGPU	Client				= {};
		::llc::rect2f2_t				Text				= {};
		::llc::rect2f2_t				Rectangles	[::llc::GUI_CONTROL_AREA_OLD_COUNT]	= {};
		::llc::tri2f2_t				Triangles	[8]		= {};
	};

	struct SControlAttachId {
		cid_t						IdControl			: 15;
		cid_t						Total				: 1;
	};

	struct SControlConstraints {
		::llc::n2cid				AttachSizeToControl	= {-1, -1};
		::llc::n2<bool>				AttachSizeToText	= {};
		::llc::rectcid				DockToControl		= {-1, -1, -1, -1};
		::llc::minmax2s1_t			SizeMinMax			= {{}, {0x7FFF, 0x7FFF}};
	};

	struct SControlText {
		::llc::vcs					Text				= {};
		::llc::ALIGN				Align				= ::llc::ALIGN_CENTER;
		::llc::minmaxu1_t			Selection			= {0, 1};
		int16_t						FontSelected		= -1;
	};

	struct SControlTheme {
		::llc::astatic<::llc::astu2_t<::llc::UI_CONTROL_AREA_COUNT>, ::llc::GUI_CONTROL_PALETTE_COUNT>
									ColorCombos			= {};
	};
#pragma pack(pop)

	struct SControlImage {
		::llc::rt<::llc::bgra, ::llc::u2_t>			Temp				= {};
		::llc::g8bgra				Image				= {};
		::llc::n2s1_t				ImageOffset			= {};
		uint8_t						ImageAlign			= ::llc::ALIGN_CENTER;
		uint8_t						ImageInvertX		: 1;
		uint8_t						ImageInvertY		: 1;
		uint8_t						Rotation			: 2;
	};

	struct SControlTable {
		::llc::apod<::llc::SControlState		>	States			= {};
		::llc::apod<::llc::SControlPlacement	>	Placement		= {};
		::llc::apod<::llc::SControlArea			>	Area			= {};
		::llc::apod<::llc::SControlConstraints	>	Constraints		= {};
		::llc::apod<::llc::SControlDraw			>	Draw			= {};
		::llc::aobj<::llc::SControlText			>	Text			= {};
		::llc::aobj<::llc::SControlImage		>	Images			= {};
		::llc::aobj<::llc::acid					>	Children		= {};
		//
		::llc::apobj<::llc::SEventSystem		>	EventQueue		= {};

		inline	bool		SetHovered		(cid_t iControl, bool value)	noexcept	{ return States[iControl].SetHovered (iControl, EventQueue, value); }
		inline	bool		SetPressed		(cid_t iControl, bool value)	noexcept	{ return States[iControl].SetPressed (iControl, EventQueue, value); }
		inline	bool		SetHidden		(cid_t iControl, bool value)	noexcept	{ return States[iControl].SetHidden  (iControl, EventQueue, value); }
		inline	bool		SetFocused		(cid_t iControl, bool value)	noexcept	{ return States[iControl].SetFocused (iControl, EventQueue, value); }
		inline	bool		SetSelected		(cid_t iControl, bool value)	noexcept	{ return States[iControl].SetSelected(iControl, EventQueue, value); }
		inline	bool		SetDisabled		(cid_t iControl, bool value)	noexcept	{ return States[iControl].SetDisabled(iControl, EventQueue, value); }
		inline	bool		SetUpdated		(cid_t iControl, bool value)	noexcept	{ return States[iControl].SetUpdated (iControl, EventQueue, value); }
		inline	bool		SetAction		(cid_t iControl, bool value)	noexcept	{ return States[iControl].SetAction  (iControl, EventQueue, value); }
		inline	bool		SetUnused		(cid_t iControl, bool value)	noexcept	{ return States[iControl].SetUnused  (iControl, EventQueue, value); }

		inline	bool		IsHovered		(cid_t iControl)		const	noexcept	{ return States[iControl].IsHovered  (); }
		inline	bool		IsPressed		(cid_t iControl)		const	noexcept	{ return States[iControl].IsPressed  (); }
		inline	bool		IsHidden		(cid_t iControl)		const	noexcept	{ return States[iControl].IsHidden   (); }
		inline	bool		IsFocused		(cid_t iControl)		const	noexcept	{ return States[iControl].IsFocused  (); }
		inline	bool		IsSelected		(cid_t iControl)		const	noexcept	{ return States[iControl].IsSelected (); }
		inline	bool		IsDisabled		(cid_t iControl)		const	noexcept	{ return States[iControl].IsDisabled (); }
		inline	bool		IsUpdated		(cid_t iControl)		const	noexcept	{ return States[iControl].IsUpdated  (); }
		inline	bool		IsAction		(cid_t iControl)		const	noexcept	{ return States[iControl].IsAction   (); }
		inline	bool		IsUnused		(cid_t iControl)		const	noexcept	{ return States[iControl].IsUnused   (); }
	};

	::llc::cid_t		controlCreate					(::llc::SControlTable & gui, ::llc::vi16 defaultColors);
} // namespace

#endif // LLC_GUI_CONTROL_H_23627
