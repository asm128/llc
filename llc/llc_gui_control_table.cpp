#include "llc_gui_control_table.h"
#include "llc_apod_serialize.h"


::llc::err_t		llc::controlTextSet		(::llc::SControlTable & controls, ::llc::cid_t iControl, ::llc::vcst_t text) {
	::llc::vcst_t					& oldText				= controls.Text[iControl].Text;
	if(text != oldText) {
		oldText					= text;
		controls.SetUpdated(iControl, false);
	}
	return 0;
}

::llc::err_t		llc::controlFontSet		(::llc::SControlTable & controls, ::llc::cid_t iControl, int16_t iFont) {
	int16_t							& oldText				= controls.Text[iControl].FontSelected;
	if(iFont != oldText) {
		oldText					= iFont;
		controls.SetUpdated(iControl, false);
	}
	return 0;
}

::llc::err_t		llc::controlDeselect		(::llc::SControlTable & controls) {
	for(::llc::u2_t iControl = 0; iControl < controls.States.size(); ++iControl)
		controls.SetSelected((::llc::cid_t)iControl, false);
	return 0;
}

::llc::err_t		llc::controlSelect		(::llc::SControlTable & controls, ::llc::cid_t controlToSelect) {
	for(::llc::u2_t iControl = 0; iControl < controls.States.size(); ++iControl)
		controls.SetSelected((::llc::cid_t)iControl, controlToSelect == (::llc::cid_t)iControl);
	return 0;
}

::llc::err_t		llc::controlHidden		(const ::llc::SControlTable & controls, ::llc::cid_t iControl) {
	const bool						imHidden				= ::llc::controlInvalid(controls, iControl) || controls.States[iControl].IsHidden();
	return imHidden ? imHidden : (0 == ::llc::controlInvalid(controls, controls.States[iControl].Parent) && ::llc::controlHidden(controls, controls.States[iControl].Parent));
}

::llc::err_t		llc::controlDisabled		(const ::llc::SControlTable & controls, ::llc::cid_t iControl) {
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

::llc::err_t		llc::controlInvalid		(const ::llc::SControlTable & controls, ::llc::cid_t iControl) {
	if(controls.States.size() <= ::llc::u2_t(iControl))
		return -1;
	if(controls.IsUnused(iControl))
		return -1;
	return 0;
}

static	::llc::err_t	actualControlUpdate		(::llc::SControlTable & controls, ::llc::cid_t iControl) {
	::llc::SControlImage			& controlImage			= controls.Images[iControl];
	const ::llc::n2u2_t			imageMetrics			= controlImage.Image.metrics();
	if(0 == (imageMetrics.x | imageMetrics.y))
		return 0;
	if(0 == controlImage.ImageInvertY)
		return 0;
	::llc::img8bgra				& inverted				= controlImage.Temp.Color;
	if_fail_fe(inverted.resize(imageMetrics));
	return 0;
}

::llc::err_t		llc::controlUpdateHierarchy	(::llc::SControlTable & controls, ::llc::cid_t iControl, const ::llc::n2u1_t & targetMetrics) {
	if_fail_fef(::llc::controlInvalid(controls, iControl), "Invalid control id: %u.", iControl);
	if(controls.TargetMetrics != targetMetrics) {
		for(::llc::u2_t iOutdated = 0; iOutdated < controls.States.size(); ++iOutdated)
			controls.SetUpdated((::llc::cid_t)iOutdated, false);
		controls.TargetMetrics		= targetMetrics;
	}
	if(false == ::llc::controlHidden(controls, iControl)) {
		if_fail_fe(::actualControlUpdate(controls, iControl));
		const ::llc::vccid		children				= controls.Children[iControl];
		for(::llc::u2_t iChild = 0, countChild = children.size(); iChild < countChild; ++iChild)
			if_fail_fe(::llc::controlUpdateHierarchy(controls, children[iChild], targetMetrics));
	}
	return 0;
}

::llc::err_t		llc::controlDelete		(::llc::SControlTable & controls, ::llc::cid_t iControl, bool recursive) {
	if_fail_fef(::llc::controlInvalid(controls, iControl), "Invalid control id: %u.", iControl);
	info_printf("Deleting control id: %i.", iControl);
	{
		::llc::acid					children				= controls.Children[iControl];
		for(::llc::u2_t iChild = 0, countChild = children.size(); iChild < countChild; ++iChild) {
			if(recursive) {
				if_true_ef(failed(::llc::controlDelete(controls, children[iChild], true)), "Failed to delete control! Invalid control id? %i.", children[iChild]);
			}
			else {
				if_true_ef(failed(::llc::controlSetParent(controls, children[iChild], ::llc::CID_INVALID)), "Failed to detach child control: %i.", children[iChild]);
			}
		}
	}
	const ::llc::cid_t			indexParent			= controls.States[iControl].Parent;
#if defined(LLC_DEBUG_ENABLED)
	::llc::u2_t					childrenRemoved		= 0;
#endif
	if((::llc::u2_t)indexParent < controls.States.size() && false == controls.IsUnused(indexParent)) {
		::llc::acid					& children				= controls.Children[indexParent];
		for(::llc::u2_t iChild = 0; iChild < children.size(); ++iChild)
			if(children[iChild] == iControl) {
				if_fail_fef(children.remove_unordered(iChild), "Failed to remove child at index: %u.", iChild);
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
	controls.SetUnused(iControl, true);
	return 0;
}

::llc::err_t		llc::controlSetParent		(::llc::SControlTable & controls, ::llc::cid_t iControl, ::llc::cid_t iParent) {
	if_fail_fef(::llc::controlInvalid(controls, iControl), "Invalid control id: %u.", iControl);
	const ::llc::cid_t			oldParent				= controls.States[iControl].Parent;
	::llc::SControlState			& control				= controls.States[iControl];
	if(control.Parent == iParent)
		return 0;

	if_true_fef(::llc::CID_INVALID != iParent && ::llc::controlInvalid(controls, iParent), "Invalid parent control id: %u.", iParent);
	if_true_fef(iControl == iParent, "A control cannot be its own parent: %u.", iControl);
	for(::llc::cid_t ancestor = iParent, iDepth = 0; false == ::llc::controlInvalid(controls, ancestor) && iDepth < (::llc::cid_t)controls.States.size(); ++iDepth) {
		if_true_fef(ancestor == iControl, "Parent cycle for control: %u.", iControl);
		ancestor = controls.States[ancestor].Parent;
	}

	if((::llc::u2_t)oldParent < controls.States.size() && false == controls.IsUnused(oldParent)) {
		::llc::acid					& children				= controls.Children[oldParent];
		for(int32_t iChild = 0; iChild < (int32_t)children.size(); ++iChild)
			if(children[iChild] == iControl) {
				controls.SetUpdated(oldParent, false);
				if_fail_fef(children.remove(iChild), "Failed to remove child at index: %u.", iChild);
				break;
			}
	}
	control.Parent			= iParent;
	controls.SetUpdated(iControl, false);
	if(::llc::CID_INVALID == iParent)
		return 0;

	::llc::acid					& children				= controls.Children[iParent];
	for(int32_t iChild = 0, countChild = children.size(); iChild < countChild; ++iChild)
		if(children[iChild] == iControl)
			return 0;

	controls.SetUpdated(iParent, false);
	if_fail_fe(children.push_back(iControl));
	return 0;
}

::llc::err_t		llc::controlActions		(const ::llc::SControlTable & controls, ::llc::acid & controlIndices) {
	for(::llc::u2_t iControl = 0, countControls = controls.States.size(); iControl < countControls; ++iControl) {
		const ::llc::SControlState	& controlState			= controls.States[iControl];
		if(controlState.IsUnused() || ::llc::controlDisabled(controls, (::llc::cid_t)iControl) || ::llc::controlHidden(controls, (::llc::cid_t)iControl))
			continue;
		if_fail_fe(controlIndices.push_back((::llc::cid_t)iControl));
	}
	return controlIndices.size();
}

::llc::err_t		llc::controlHandle		(const ::llc::SControlTable & controls, const ::llc::function<::llc::err_t(::llc::cid_t iControl)> & funcOnExecute) {
	::llc::acid					controlsToProcess		= {};
	if_fail_fe(::llc::controlActions(controls, controlsToProcess));
	return ::llc::controlHandle(controls, controlsToProcess, funcOnExecute);
}

::llc::err_t		llc::controlHandle		(const ::llc::SControlTable & controls, ::llc::vcid controlsToProcess, const ::llc::function<::llc::err_t(::llc::cid_t iControl)> & funcOnExecute) {
	for(::llc::u2_t iControl = 0, countControls = controlsToProcess.size(); iControl < countControls; ++iControl) {
		const ::llc::cid_t			idControl				= controlsToProcess[iControl];
		const ::llc::SControlState	& controlState			= controls.States[idControl];
		if(controlState.IsAction()) {
			info_printf("Executed control %i (0x%x).", idControl, idControl);
			return funcOnExecute(idControl);
		}
	}
	return 0;
}

::llc::err_t		llc::controlInstanceReset	(::llc::SControlTable & controls, ::llc::cid_t iControl, ::llc::vs1_t defaultColors) {
	controls.States		[iControl]	= {};
	controls.TextState	[iControl].clear();
	controls.Placement	[iControl]	= {};
	controls.Area		[iControl]	= {};
	controls.Constraints[iControl]	= {};
	controls.Draw		[iControl]	= {};
	controls.Text		[iControl]	= {};
	controls.Children	[iControl]	= ::llc::vcid{};
	controls.Images		[iControl]	= {};

	::llc::SControlDraw			& controlDraw			= controls.Draw[iControl];
	for(::llc::u2_t iColor = 0, countColors = ::llc::min(controlDraw.Palettes.size(), defaultColors.size()); iColor < countColors; ++iColor)
		controlDraw.Palettes[iColor] = defaultColors[iColor];
	return 0;
}

::llc::cid_t		llc::controlCreate		(::llc::SControlTable & controls, ::llc::vs1_t defaultColors) {
	for(::llc::u2_t iControl = 0; iControl < controls.States.size(); ++iControl) {
		if(controls.IsUnused((::llc::cid_t)iControl)) {
			if_fail_fef(::llc::controlInstanceReset(controls, (::llc::cid_t)iControl, defaultColors), "iControl: %i", iControl);
			return (::llc::cid_t)iControl;
		}
	}

	::llc::cid_t					iControl				= -1;
	if_fail_fe(iControl = (::llc::cid_t)::llc::resize(controls.States.size() + 1
		, controls.States
		, controls.TextState
		, controls.Placement
		, controls.Area
		, controls.Constraints
		, controls.Draw
		, controls.Text
		, controls.Images
		, controls.Children
		) - 1);
	if_fail_fe(::llc::controlInstanceReset(controls, iControl, defaultColors));
	return iControl;
}

::llc::err_t		llc::SControlTable::Save	(::llc::au0_t & output) const {
	const ::llc::u2_t			controlCount			= States.size();
	if_true_fe(controlCount != Placement.size());
	if_true_fe(controlCount != Area.size());
	if_true_fe(controlCount != Constraints.size());
	if_true_fe(controlCount != Draw.size());
	if_true_fe(controlCount != Text.size());
	if_true_fe(controlCount != TextState.size());
	if_true_fe(controlCount != Images.size());
	if_true_fe(controlCount != Children.size());

	if_fail_fe(::llc::save(output, States, Constraints, Placement, Area, Draw));
	for(::llc::u2_t iControl = 0; iControl < controlCount; ++iControl) {
		const ::llc::SControlText	& text					= Text[iControl];
		if_fail_fef(::llc::saveView(output, text.Text), "iControl: %u", iControl);
		if_fail_fef(::llc::save(output, text.Align, TextState[iControl], text.FontSelected), "iControl: %u", iControl);
	}
	for(::llc::u2_t iControl = 0; iControl < controlCount; ++iControl) {
		const ::llc::n2u2_t			imageMetrics			= Images[iControl].Image.metrics();
		if_fail_fef(::llc::savePOD(output, imageMetrics), "iControl: %u", iControl);
	}
	for(::llc::u2_t iControl = 0; iControl < controlCount; ++iControl)
		if_fail_fef(::llc::saveView(output, Children[iControl]), "iControl: %u", iControl);
	const ::llc::packedu32			eventCount				= EventQueue.size();
	if_fail_fe(::llc::savePacked(output, eventCount));
	for(::llc::u2_t iEvent = 0; iEvent < EventQueue.size(); ++iEvent) {
		const ::llc::SEventSystem	* event					= EventQueue[iEvent];
		if_fail_fef(event ? event->Save(output) : ::llc::SEventSystem{}.Save(output), "iEvent: %u", iEvent);
	}
	return 0;
}

::llc::err_t		llc::SControlTable::Load	(::llc::vcu0_t & input) {
	if_fail_fe(::llc::load(input, States));
	const ::llc::u2_t			controlCount			= States.size();
	if_fail_fe(::llc::resize(controlCount, Text, TextState, Images, Children));
	if_fail_fe(::llc::load(input, Constraints, Placement, Area, Draw));
	if_true_fe(controlCount != Constraints.size());
	if_true_fe(controlCount != Placement.size());
	if_true_fe(controlCount != Area.size());
	if_true_fe(controlCount != Draw.size());
	for(::llc::u2_t iControl = 0; iControl < controlCount; ++iControl) {
		::llc::SControlText			& text					= Text[iControl];
		if_fail_fef(::llc::loadView(input, text.Text), "iControl: %u", iControl);
		if_fail_fef(::llc::load(input, text.Align, TextState[iControl], text.FontSelected), "iControl: %u", iControl);
	}
	for(::llc::u2_t iControl = 0; iControl < controlCount; ++iControl) {
		::llc::n2u2_t				imageMetrics			= {};
		if_fail_fef(::llc::load(input, imageMetrics), "iControl: %u", iControl);
	}
	for(::llc::u2_t iControl = 0; iControl < controlCount; ++iControl)
		if_fail_fef(::llc::loadView(input, Children[iControl]), "iControl: %u", iControl);
	::llc::u2_t					eventCount				= {};
	if_fail_fe(::llc::loadPacked(input, eventCount));
	if_fail_fe(EventQueue.resize(eventCount));
	for(::llc::u2_t iEvent = 0; iEvent < eventCount; ++iEvent)
		if_fail_fef(EventQueue[iEvent]->Load(input), "iEvent: %u", iEvent);
	return 0;
}
