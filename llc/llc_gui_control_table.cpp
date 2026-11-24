#include "llc_gui_control.h"


::llc::err_t		llc::controlHidden			(const ::llc::SControlTable & controls, ::llc::cid_t iControl) {
	const bool						imHidden				= ::llc::controlInvalid(controls, iControl) || controls.States[iControl].IsHidden();
	return imHidden ? imHidden : (0 == ::llc::controlInvalid(controls, controls.States[iControl].Parent) && ::llc::controlHidden(controls, controls.States[iControl].Parent));
}

::llc::err_t		llc::controlDisabled			(const ::llc::SControlTable & controls, ::llc::cid_t iControl) {
	const bool						imDisabled				= ::llc::controlInvalid(controls, iControl) || controls.States[iControl].IsDisabled();
	return imDisabled ? imDisabled : (false == ::llc::controlInvalid(controls, controls.States[iControl].Parent) && ::llc::controlDisabled(controls, controls.States[iControl].Parent));
}

::llc::err_t		llc::controlMetricsInvalidate	(::llc::SControlTable & controls, ::llc::cid_t iControl) {
	controls.SetUpdated(iControl, false);
	const ::llc::vcid			& controlChildren		= controls.Children[iControl];
	for(::llc::u2_t iChild = 0, countChild = controlChildren.size(); iChild < countChild; ++iChild)
		if_fail_fef(::llc::controlMetricsInvalidate(controls, controlChildren[iChild]), "%s", "Invalid child?");

	return 0;
}

::llc::err_t		llc::controlInvalid			(const ::llc::SControlTable & controls, ::llc::cid_t iControl) {
	if(controls.States.size() <= ::llc::u2_t(iControl))
		return -1;
	if(controls.IsUnused(iControl))
		return -1;
	return 0;
}

static	::llc::err_t	actualControlUpdate		(::llc::SControlTable & controls, ::llc::cid_t iControl) {
	::llc::SControlImage		& controlImage			= controls.Images[iControl];
	const ::llc::n2u2_t		imageMetrics			= controlImage.Image.metrics();
	if(0 == (imageMetrics.x | imageMetrics.y))
		return 0;
	if(0 == controlImage.ImageInvertY)
		return 0;
	::llc::img8bgra			& inverted				= controlImage.Temp.Color;
	if_fail_fe(inverted.resize(imageMetrics));
	return 0;
}

::llc::err_t		llc::controlUpdateHierarchy		(::llc::SControlTable & controls, ::llc::cid_t iControl, const ::llc::n2u1_t & targetMetrics) {
	if_fail_fef(::llc::controlInvalid(controls, iControl), "Invalid control id: %u.", iControl);
	if(controls.LastSize != targetMetrics) {
		for(::llc::u2_t iOutdated = 0; iOutdated < controls.States.size(); ++iOutdated)
			controls.SetUpdated((::llc::cid_t)iOutdated, false);
		controls.LastSize			= targetMetrics;
	}
	if(false == ::llc::controlHidden(controls, iControl)) {
		if_fail_fe(::actualControlUpdate(controls, iControl));
		const ::llc::vccid		children				= controls.Children[iControl];
		for(::llc::u2_t iChild = 0, countChild = children.size(); iChild < countChild; ++iChild)
			if_fail_fe(::llc::controlUpdateHierarchy(controls, children[iChild], targetMetrics));
	}
	return 0;
}
