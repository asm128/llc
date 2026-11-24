#include "llc_gui.h"

#ifndef LLC_GUI_VIEWPORT_H_23627
#define LLC_GUI_VIEWPORT_H_23627

namespace llc
{
#pragma pack(push, 1)
	enum VIEWPORT_CONTROL : int8_t
		{ VIEWPORT_CONTROL_RESIZE_LEFT				= 0
		, VIEWPORT_CONTROL_RESIZE_TOP
		, VIEWPORT_CONTROL_RESIZE_RIGHT
		, VIEWPORT_CONTROL_RESIZE_BOTTOM
		, VIEWPORT_CONTROL_RESIZE_TOP_LEFT
		, VIEWPORT_CONTROL_RESIZE_TOP_RIGHT
		, VIEWPORT_CONTROL_RESIZE_BOTTOM_LEFT
		, VIEWPORT_CONTROL_RESIZE_BOTTOM_RIGHT
		, VIEWPORT_CONTROL_TITLE
		, VIEWPORT_CONTROL_CLOSE
		, VIEWPORT_CONTROL_TARGET
		, VIEWPORT_CONTROL_COUNT
		};

	enum GUI_CONTROL_TYPE : int8_t
		{ GUI_CONTROL_TYPE_Viewport					= 0
		, GUI_CONTROL_TYPE_PaletteGrid
		, GUI_CONTROL_TYPE_ControlList
		, GUI_CONTROL_TYPE_COUNT
		};

	struct SViewport {
		cid_t					IdControl			= (int32_t)-1;
		::llc::astcid<::llc::VIEWPORT_CONTROL_COUNT>	IdControls		= {};
		cid_t					IdDesktopElement	= (int32_t)-1;
		GUI_CONTROL_TYPE		ControlType			= (GUI_CONTROL_TYPE)-1;
		::llc::n2s1_t			UnfoldedSize		= {};
		bool					Folded				= false;
	};

	bool					viewportIsPressed		(cnst ::llc::SGUI & gui, cnst ::llc::SViewport & viewport)												nxpt;
	::llc::error_t			viewportActivate		(::llc::SGUI & gui, ::llc::cid_t idParent, ::llc::view<cnst ::llc::SViewport> viewports, ::llc::u0_t iViewport);
	::llc::error_t			viewportFold			(::llc::SGUI & gui, ::llc::SViewport & viewport);
	::llc::error_t			viewportInitialize		(::llc::SGUI & gui, ::llc::SViewport & viewport);
#pragma pack(pop)
}

#endif // LLC_GUI_VIEWPORT_H_23627
