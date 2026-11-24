#include "llc_gui_control.h"
#include "llc_gui_image_flag.h"

#ifndef LLC_GUI_CONTROL_TABLE_H_23627
#define LLC_GUI_CONTROL_TABLE_H_23627

namespace llc
{
	struct SControlTable {
		::llc::n2u1_t						TargetMetrics	= {};
		::llc::apod<::llc::SControlState		>	States			= {};
		::llc::aobj<::llc::apod<::llc::STextState>> TextState	= {};
		::llc::apod<::llc::SControlPlacement	>	Placement		= {};
		::llc::apod<::llc::SControlArea			>	Area			= {};
		::llc::apod<::llc::SControlConstraints	>	Constraints		= {};
		::llc::apod<::llc::SControlDraw			>	Draw			= {};
		::llc::aobj<::llc::SControlText			>	Text			= {};
		::llc::aobj<::llc::SControlImage		>	Images			= {};
		::llc::aobj<::llc::acid					>	Children		= {};
		//
		::llc::apobj<::llc::SEventSystem		>	EventQueue		= {};

		::llc::err_t	Save			(::llc::au0_t & output) const;
		// Loaded text views borrow input bytes; keep the input storage alive while the table uses them.
		::llc::err_t	Load			(::llc::vcu0_t & input);

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

	::llc::err_t		controlHidden					(const ::llc::SControlTable & controls, ::llc::cid_t iControl);
	::llc::err_t		controlDisabled				(const ::llc::SControlTable & controls, ::llc::cid_t iControl);
	::llc::err_t		controlInvalid					(const ::llc::SControlTable & controls, ::llc::cid_t iControl);
	::llc::err_t		controlActions					(const ::llc::SControlTable & controls, ::llc::acid & controlIndices);
	::llc::err_t		controlHandle					(const ::llc::SControlTable & controls, const ::llc::function<::llc::err_t(::llc::cid_t iControl)> & funcOnExecute);
	::llc::err_t		controlHandle					(const ::llc::SControlTable & controls, ::llc::vcid controlsToProcess, const ::llc::function<::llc::err_t(::llc::cid_t iControl)> & funcOnExecute);
	inline	::llc::cid_t	controlParent					(const ::llc::SControlTable & controls, ::llc::cid_t iControl) { return ::llc::controlInvalid(controls, iControl) ? ::llc::CID_INVALID : controls.States[iControl].Parent; }
	::llc::err_t		controlDeselect				(::llc::SControlTable & controls);
	::llc::err_t		controlSelect					(::llc::SControlTable & controls, ::llc::cid_t controlToSelect);
	::llc::err_t		controlDelete					(::llc::SControlTable & controls, ::llc::cid_t iControl, bool recursive = true);
	::llc::err_t		controlSetParent				(::llc::SControlTable & controls, ::llc::cid_t iControl, ::llc::cid_t iParent);
	::llc::err_t		controlTextSet					(::llc::SControlTable & controls, ::llc::cid_t iControl, ::llc::vcst_t text);
	::llc::err_t		controlFontSet					(::llc::SControlTable & controls, ::llc::cid_t iControl, int16_t iFont);
	::llc::err_t		controlMetricsInvalidate		(::llc::SControlTable & controls, ::llc::cid_t iControl);
	::llc::err_t		controlUpdateHierarchy			(::llc::SControlTable & controls, ::llc::cid_t iControl, const ::llc::n2u1_t & targetMetrics);
	::llc::err_t		controlInstanceReset			(::llc::SControlTable & controls, ::llc::cid_t iControl, ::llc::vs1_t defaultColors);
	::llc::cid_t		controlCreate					(::llc::SControlTable & controls, ::llc::vs1_t defaultColors);
	inline	::llc::err_t	controlToFront					(::llc::SControlTable & controls, ::llc::cid_t iControl) {
		const ::llc::cid_t				iParent				= ::llc::controlParent(controls, iControl);
		if_fail_fe(::llc::controlSetParent(controls, iControl, ::llc::CID_INVALID));
		if_fail_fe(::llc::controlSetParent(controls, iControl, iParent));
		return 0;
	}
} // namespace

#endif // LLC_GUI_CONTROL_TABLE_H_23627
