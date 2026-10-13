#include "llc_gui_control.h"
#include "llc_input.h"
#include "llc_functional.h"

#ifndef LLC_GUI_H_23627
#define LLC_GUI_H_23627

namespace llc
{
	tydf function<err_t(s1_t, vcst_t, n2u1_t &)> FGUITextMeasure;

	struct SGUIZoom {
		::llc::n2f3_t		DPI					= {1.0, 1.0};
		::llc::minmaxf3_t	ZoomLimits			= {0.1, 10.0};
		double				ZoomLevel			= 1.0;

		bool				operator==			(const ::llc::SGUIZoom & other)		const	noexcept	{
			return DPI			== other.DPI
				&& ZoomLevel	== other.ZoomLevel
				;
		}
		inline	bool		operator!=			(const ::llc::SGUIZoom & other)		const	noexcept	{ return !operator==(other); }
	};

	struct SGUI {
		::llc::n2u1_t						LastSize			= {};
		::llc::n2f2_t						CursorPos			= {};
		::llc::SControlTable				Controls			= {};
		::llc::asti16<::llc::GUI_CONTROL_PALETTE_COUNT>
										DefaultColors			= {};

		::llc::SGUIZoom						LastZoom			= {};
		::llc::SGUIZoom						Zoom				= {};
		uint16_t							SelectedFont		= 7;
		uint16_t							ThemeDefault		= 0;
		::llc::GUI_COLOR_MODE				ColorModeDefault	= ::llc::GUI_COLOR_MODE_3D;//::llc::GUI_COLOR_MODE_DEFAULT;
		bool								InputModeTouch		= true;
	};

	::llc::error_t			guiProcessInput					(::llc::SGUI & gui, const ::llc::SInput & input, ::llc::vpobj<::llc::SEventSystem> in_Events = {});
	::llc::error_t			guiProcessInput					(::llc::SGUI & gui, const ::llc::SInput & input, ::llc::vcid controlsKeyboard, ::llc::vpobj<::llc::SEventSystem> in_Events = {});
	::llc::error_t			guiUpdateMetrics				(::llc::SGUI & gui, const ::llc::n2u1_t & targetSize, const ::llc::FGUITextMeasure & textMeasure, bool forceUpdate);
	::llc::error_t			guiUpdateMetrics				(::llc::SGUI & gui, const ::llc::n2u1_t & targetSize, bool forceUpdate);
	::llc::error_t			guiGetProcessableControls		(const ::llc::SGUI & gui, ::llc::acid & controlIndices);
	::llc::error_t			guiDeselect						(::llc::SGUI & gui);
	::llc::error_t			guiSelect						(::llc::SGUI & gui, cid_t controlToSelect);

	::llc::error_t			controlDelete					(::llc::SGUI & gui, cid_t iControl, bool recursive = true);
	::llc::cid_t 			controlCreate					(::llc::SGUI & gui);
	::llc::cid_t			controlCreateChild				(::llc::SGUI & gui, cid_t iParent);
	::llc::error_t			controlSetParent				(::llc::SGUI & gui, cid_t iControl, cid_t iParent);
	::llc::error_t			controlTextSet					(::llc::SGUI & gui, cid_t iControl, const ::llc::vcst_t & text);
	::llc::error_t			controlFontSet					(::llc::SGUI & gui, cid_t iControl, int16_t iFont);
	//::llc::error_t			controlUpdateMetrics			(::llc::SGUI & gui, int32_t iControl, const ::llc::n2<uint32_t> & targetSize);
	::llc::error_t			controlUpdateMetricsTopToDown	(::llc::SGUI & gui, cid_t iControl, const ::llc::n2u1_t & targetSize, const ::llc::FGUITextMeasure & textMeasure, bool forceUpdate);
	::llc::error_t			controlUpdateMetricsTopToDown	(::llc::SGUI & gui, cid_t iControl, const ::llc::n2u1_t & targetSize, bool forceUpdate);
	::llc::error_t			controlMetricsInvalidate		(::llc::SGUI & gui, cid_t iControl);
	::llc::error_t			controlHidden					(const ::llc::SGUI & gui, cid_t iControl);
	::llc::error_t			controlDisabled					(const ::llc::SGUI & gui, cid_t iControl);
	::llc::error_t			controlInvalid					(const ::llc::SGUI & gui, cid_t iControl);


	::llc::error_t			guiProcessControls				(const ::llc::SGUI & gui, const ::llc::function<::llc::error_t(::llc::cid_t iControl)> & funcOnExecute);
	::llc::error_t			guiProcessControls				(const ::llc::SGUI & gui, ::llc::vcid controlsToProcess, const ::llc::function<::llc::error_t(::llc::cid_t iControl)> & funcOnExecute);
} // namespace

#endif // LLC_GUI_H_23627
