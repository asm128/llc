#include "llc_gui.h"

#ifndef LLC_GUI_VIEWPORT_H_23627
#define LLC_GUI_VIEWPORT_H_23627

namespace llc
{
	stxp	::llc::s1_t	VIEWPORT_TITLE_HEIGHT	= 20;
	stxp	::llc::s1_t	VIEWPORT_CLIENT_GAP		= 1;

	stct SViewportSettings {
		bool				Dragging			= false;
		bool				Unfolded			= true;
		::llc::n2<bool>	DisplacementLock	= {};
	};

	stct SViewport {
		::llc::cid_t		IdRoot				= ::llc::CID_INVALID;
		::llc::cid_t		IdTitle			= ::llc::CID_INVALID;
		::llc::cid_t		IdClient			= ::llc::CID_INVALID;
		SViewportSettings	Settings			= {};
	};

	stct SViewportCreate {
		::llc::cid_t		Parent				= ::llc::CID_INVALID;
		::llc::n2s1_t		ClientSize			= {800, 450};
		::llc::vcst_t		Title				= LLC_CXS("Viewport");
		::llc::s1_t		TitleHeight		= ::llc::VIEWPORT_TITLE_HEIGHT;
	};

	::llc::err_t	viewportCreate			(::llc::SViewport & viewport, ::llc::SGUI & gui, cnst ::llc::SViewportCreate & setup = {});
	::llc::err_t	viewportFold			(::llc::SViewport & viewport, ::llc::SGUI & gui, bool fold);
	::llc::err_t	viewportUpdate			(::llc::SViewport & viewport, ::llc::SGUI & gui, ::llc::n2s2_c & mouseDeltas = {});
} // namespace llc

#endif // LLC_GUI_VIEWPORT_H_23627
