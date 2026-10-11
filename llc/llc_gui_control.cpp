#include "llc_gui_control.h"

sttc ::llc::err_t controlInstanceReset(::llc::SControlTable & controlTable, ::llc::cid_t iControl, ::llc::vi16 defaultColors) {
	::llc::SControlEntity & entity = controlTable.Entities[iControl];
	cnst ::llc::u2_t generation = entity.Generation + 1;
	entity = {};
	entity.Generation = generation ? generation : 1;
	controlTable.States		[iControl]	= {};
	controlTable.Placement	[iControl]	= {};
	controlTable.Area		[iControl]	= {};
	controlTable.Constraints[iControl]	= {};
	controlTable.Draw		[iControl]	= {};
	controlTable.Text		[iControl]	= {};
	controlTable.Children	[iControl].clear();
	::llc::SControlDraw & controlDraw = controlTable.Draw[iControl];
	for(::llc::u2_t iColor = 0, countColors = ::llc::min(controlDraw.Palettes.size(), defaultColors.size()); iColor < countColors; ++iColor)
		controlDraw.Palettes[iColor] = defaultColors[iColor];
	rtrn 0;
}

::llc::error_t llc::controlInvalid(cnst ::llc::SControlTable & controlTable, ::llc::cid_t iControl) {
	rtrn iControl < 0 || (::llc::u2_t)iControl >= controlTable.Entities.size() || controlTable.IsUnused(iControl);
}

::llc::cid_t llc::controlCreate(::llc::SControlTable & controlTable, ::llc::vi16 defaultColors) {
	for(::llc::u2_t iControl = 0; iControl < controlTable.States.size(); ++iControl) {
		if(false == controlTable.IsUnused((::llc::cid_t)iControl))
			continue;
		if_fail_fe(::controlInstanceReset(controlTable, (::llc::cid_t)iControl, defaultColors));
		if_fail_fe(::llc::eventEnqueueGuiControlCreate(controlTable.EventQueue, (::llc::cid_t)iControl));
		rtrn (::llc::cid_t)iControl;
	}
	if_true_fe(controlTable.States.size() >= 0x7FFFU);
	::llc::cid_t iControl = (::llc::cid_t)controlTable.States.size();
	if_fail_fe(::llc::resize(controlTable.States.size() + 1, controlTable.Entities, controlTable.States, controlTable.Placement, controlTable.Area, controlTable.Constraints, controlTable.Draw, controlTable.Text, controlTable.Children));
	if_fail_fe(::controlInstanceReset(controlTable, iControl, defaultColors));
	if_fail_fe(::llc::eventEnqueueGuiControlCreate(controlTable.EventQueue, iControl));
	rtrn iControl;
}

sttc ::llc::err_t controlIsDescendant(cnst ::llc::SControlTable & controlTable, ::llc::cid_t iControl, ::llc::cid_t iParent) {
	for(::llc::u2_t iDepth = 0; iParent != ::llc::CID_INVALID && iDepth < controlTable.Entities.size(); ++iDepth) {
		if(iParent == iControl)
			rtrn 1;
		if(::llc::controlInvalid(controlTable, iParent))
			rtrn 0;
		iParent = controlTable.Entities[iParent].Parent;
	}
	rtrn 0;
}

::llc::error_t llc::controlSetParent(::llc::SControlTable & controlTable, ::llc::cid_t iControl, ::llc::cid_t iParent) {
	if_true_fe(::llc::controlInvalid(controlTable, iControl));
	if_true_fe(iParent != ::llc::CID_INVALID && ::llc::controlInvalid(controlTable, iParent));
	if_true_fe(iParent == iControl || ::controlIsDescendant(controlTable, iControl, iParent));
	::llc::cid_t & parentOld = controlTable.Entities[iControl].Parent;
	if(parentOld == iParent)
		rtrn 0;
	if(iParent != ::llc::CID_INVALID) {
		::llc::acid & childrenNew = controlTable.Children[iParent];
		if(childrenNew.find(iControl) < 0)
			if_fail_fe(childrenNew.push_back(iControl));
	}
	if(parentOld != ::llc::CID_INVALID && false == ::llc::controlInvalid(controlTable, parentOld)) {
		::llc::acid & childrenOld = controlTable.Children[parentOld];
		cnst ::llc::err_t iChild = childrenOld.find(iControl);
		if(iChild >= 0)
			if_fail_fe(childrenOld.remove((::llc::u2_t)iChild));
	}
	cnst ::llc::cid_t parentPrevious = parentOld;
	parentOld = iParent;
	controlTable.States[iControl].Flag = ::llc::bit_clear(controlTable.States[iControl].Flag, ::llc::GUI_CONTROL_FLAG_Updated);
	if(parentPrevious != ::llc::CID_INVALID && false == ::llc::controlInvalid(controlTable, parentPrevious))
		controlTable.States[parentPrevious].Flag = ::llc::bit_clear(controlTable.States[parentPrevious].Flag, ::llc::GUI_CONTROL_FLAG_Updated);
	if(iParent != ::llc::CID_INVALID)
		controlTable.States[iParent].Flag = ::llc::bit_clear(controlTable.States[iParent].Flag, ::llc::GUI_CONTROL_FLAG_Updated);
	rtrn ::llc::eventEnqueueGuiControlParentChange(controlTable.EventQueue, iControl, parentPrevious, iParent);
}

::llc::error_t llc::controlDelete(::llc::SControlTable & controlTable, ::llc::cid_t iControl, bool recursive) {
	if_true_fe(::llc::controlInvalid(controlTable, iControl));
	::llc::acid children = controlTable.Children[iControl];
	for(::llc::u2_t iChild = 0; iChild < children.size(); ++iChild) {
		if(recursive) {
			if_fail_fe(::llc::controlDelete(controlTable, children[iChild], true));
		}
		else {
			if_fail_fe(::llc::controlSetParent(controlTable, children[iChild], ::llc::CID_INVALID));
		}
	}
	cnst ::llc::cid_t parent = controlTable.Entities[iControl].Parent;
	if(parent != ::llc::CID_INVALID && false == ::llc::controlInvalid(controlTable, parent)) {
		::llc::acid & siblings = controlTable.Children[parent];
		cnst ::llc::err_t iChild = siblings.find(iControl);
		if(iChild >= 0)
			if_fail_fe(siblings.remove((::llc::u2_t)iChild));
	}
	controlTable.Children[iControl].clear();
	controlTable.Entities[iControl].Parent = ::llc::CID_INVALID;
	controlTable.States[iControl].Flag = ::llc::GUI_CONTROL_FLAG_Unused;
	rtrn ::llc::eventEnqueueGuiControlDelete(controlTable.EventQueue, iControl);
}
