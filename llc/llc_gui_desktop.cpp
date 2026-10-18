#include "llc_gui_desktop.h"
#include "llc_label.h"

#define NEW_OR_UNUSED(ctoken, utoken)							\
	::llc::u2_t					i##ctoken				= 0;	\
	while(i##ctoken < desktop.Items.ctoken##s.size()) {			\
		if(desktop.Items.ctoken##s.Unused[i##ctoken]) 			\
			break;												\
		++i##ctoken;											\
	}															\
	if(i##ctoken >= desktop.Items.ctoken##s.size())				\
		llc_necs(i##ctoken = desktop.Items.ctoken##s.push_back({}));						\
	desktop.Items.ctoken##s.Unused[i##ctoken]	= false;									\
	::llc::S##ctoken	& elementToSetUp	= desktop.Items.ctoken##s[i##ctoken]	= {};	\
	llc_necs(llc::utoken##Initialize(gui, elementToSetUp));								\
	llc_necall(llc::controlSetParent(gui, elementToSetUp.IdControl, desktop.IdControl), "%I - %I", elementToSetUp.IdControl, desktop.IdControl);

::llc::error_t			llc::desktopDeletePaletteGrid	(::llc::SGUI & gui, ::llc::SDesktop & desktop, ::llc::cid_t iElement) { 
	desktop.Items.PaletteGrids.Unused[iElement] = true; 
	::llc::controlDelete(gui, desktop.Items.PaletteGrids[iElement].IdControl); 
	desktop.Items.PaletteGrids[iElement] = {}; 
	return 0; 
}

::llc::error_t			llc::desktopCreateViewport		(::llc::SGUI & gui, ::llc::SDesktop & desktop)	{ NEW_OR_UNUSED(Viewport	, viewport		); return iViewport		; }
::llc::error_t			llc::desktopCreatePaletteGrid	(::llc::SGUI & gui, ::llc::SDesktop & desktop)	{ NEW_OR_UNUSED(PaletteGrid	, paletteGrid	); return iPaletteGrid	; }
::llc::error_t			llc::desktopCreateControlList	(::llc::SGUI & gui, ::llc::SDesktop & desktop)	{ NEW_OR_UNUSED(ControlList	, controlList	);
	if(iControlList >= desktop.Children.size())
		llc_necs(desktop.Children.resize(iControlList + 1));

	desktop.Children[iControlList].clear();
	return iControlList;
}

::llc::error_t			llc::desktopDeleteControlList	(::llc::SGUI & gui, ::llc::SDesktop & desktop, ::llc::cid_t iElement)	{
	llc_necall(desktop.Items.ControlLists.size() <= (::llc::u2_t)iElement, "%s", "Invalid control list.");
	::llc::SControlList			& controlListToDelete			= desktop.Items.ControlLists[iElement];
	::llc::acid					childLists						= desktop.Children[iElement];
	for(::llc::u2_t iOption = 0, countOptions = childLists.size(); iOption < countOptions; ++iOption) {
		::llc::cid_t				childControlListIndex			= childLists[iOption];
		if(desktop.Items.ControlLists.size() > (::llc::u2_t)childControlListIndex && false == desktop.Items.ControlLists.Unused[childControlListIndex])
			llc_necall(llc::desktopDeleteControlList(gui, desktop, childControlListIndex), "%s", "??");
		else
			if_true_ef(childControlListIndex != -1, "%s", "");
	}
	if(desktop.Items.ControlLists.size() > (::llc::u2_t)controlListToDelete.IndexParentList && false == desktop.Items.ControlLists.Unused[controlListToDelete.IndexParentList]) {
		if (desktop.Children[controlListToDelete.IndexParentList].size() > (::llc::u2_t)controlListToDelete.IndexParentItem)
			desktop.Children[controlListToDelete.IndexParentList][controlListToDelete.IndexParentItem]	= -1;
	}
	desktop.Items.ControlLists.Unused[iElement]	= true;
	llc_necall(llc::controlDelete(gui, controlListToDelete.IdControl, true), "%s", "Failed to delete control! Deleted already?");
	controlListToDelete		= {};
	return 0;
}

::llc::error_t			llc::desktopDeleteViewport	(::llc::SGUI & gui, ::llc::SDesktop & desktop, ::llc::cid_t iElement)						{
	desktop.Items.Viewports.Unused[iElement]	= true;
	const ::llc::SViewport		& viewport						= desktop.Items.Viewports[iElement];
	switch(viewport.ControlType) {
	case ::llc::GUI_CONTROL_TYPE_PaletteGrid	: desktop.Items.PaletteGrids	.Unused[viewport.IdDesktopElement] = true; info_printf("Deleting viewport's palette grid: %i. Control Id: %i."	, viewport.IdDesktopElement, desktop.Items.PaletteGrids	[viewport.IdDesktopElement].IdControl);break;
	case ::llc::GUI_CONTROL_TYPE_Viewport		: desktop.Items.Viewports		.Unused[viewport.IdDesktopElement] = true; info_printf("Deleting viewport's vp: %i. Control Id: %i."			, viewport.IdDesktopElement, desktop.Items.Viewports	[viewport.IdDesktopElement].IdControl);break;
	case ::llc::GUI_CONTROL_TYPE_ControlList	: ::llc::desktopDeleteControlList(gui, desktop, viewport.IdDesktopElement); info_printf("Deleting viewport's vp: %i. Control Id: %i."			, viewport.IdDesktopElement, desktop.Items.Viewports	[viewport.IdDesktopElement].IdControl);break;
	case ::llc::GUI_CONTROL_TYPE_COUNT			:
		error_printf("%s", "Some team of lammer idiots who can't code decided that not casing every enum value would be an error.");
		break;
	}
	info_printf("Deleting control for viewport: %i. Control Id: %i.", iElement, viewport.IdControl);
	::llc::controlDelete(gui, viewport.IdControl);
	desktop.Items.Viewports[iElement]										= {};
	return 0;
}

static	::llc::error_t	displace					(::llc::SGUI & gui, ::llc::cid_t iControl, const ::llc::n2<double> & mouseDeltas)	{
	const ::llc::n2<double>		currentScale									= gui.Zoom.DPI * gui.Zoom.ZoomLevel;
	::llc::n2<double>			deltasScaled									= mouseDeltas.f3().GetScaled(1.0 / currentScale.x, 1.0 / currentScale.y);
	gui.Controls.Placement[iControl].Area.Offset								+= deltasScaled.s1();
	return ::llc::controlMetricsInvalidate(gui, iControl);
}

static	::llc::error_t	pushToFrontAndDisplace		(::llc::SGUI & gui, ::llc::cid_t iControl, ::llc::SInput & input)							{
	const ::llc::cid_t			parentControlId				= gui.Controls.States[iControl].Parent;
	for(::llc::u2_t iChild = 0, childCount = gui.Controls.Children[parentControlId].size(); iChild < childCount; ++iChild)
		if(gui.Controls.Children[parentControlId][iChild] == iControl) {
			llc_necs(gui.Controls.Children[parentControlId].remove(iChild));
			llc_necs(gui.Controls.Children[parentControlId].push_back(iControl));
			break;
		}
	if(input.MouseCurrent.Deltas.x || input.MouseCurrent.Deltas.y)
		llc_necall(::displace(gui, iControl, {(double)input.MouseCurrent.Deltas.x, (double)input.MouseCurrent.Deltas.y}), "%s", "Unknown issue!");
	return 0;
}

static	::llc::error_t	unhideMenuHierarchy			(::llc::SGUI & gui, ::llc::SDesktop& desktop, ::llc::SControlList& menu)	{
	if(menu.IndexParentList != -1 && false == desktop.Items.ControlLists.Unused[menu.IndexParentList])
		::unhideMenuHierarchy(gui, desktop, desktop.Items.ControlLists[menu.IndexParentList]);

	gui.Controls.SetHidden(menu.IdControl, false);
	const ::llc::cid_t			parentControlIndex			= gui.Controls.States[menu.IdControl].Parent;
	if(-1 != parentControlIndex) {
		::llc::acid & parentChildren = gui.Controls.Children[parentControlIndex];
		for(::llc::u2_t iChild = 0, childCount = parentChildren.size(); iChild < childCount; ++iChild) {
			if(parentChildren[iChild] == menu.IdControl) {
				parentChildren.remove(iChild);
				parentChildren.push_back(menu.IdControl);
				::llc::controlMetricsInvalidate(gui, parentControlIndex);
				break;
			}
		}
	}
	return 0;
}

static	::llc::error_t	selectMenuHierarchy				(::llc::SGUI & gui, ::llc::SDesktop& desktop, ::llc::SControlList& menu)	{
	if(menu.IndexParentList != -1 && false == desktop.Items.ControlLists.Unused[menu.IndexParentList]) {
		::selectMenuHierarchy(gui, desktop, desktop.Items.ControlLists[menu.IndexParentList]);
		desktop.Items.ControlLists[menu.IndexParentList].IdSelected	= menu.IndexParentItem;
	}

	gui.Controls.SetHidden(menu.IdControl, false);
	return 0;
}

static	::llc::error_t	unhoverMenuHierarchy						(::llc::SGUI & gui, ::llc::SDesktop & desktop, ::llc::SControlList & menu)	{
	if(desktop.Items.ControlLists.size() > (::llc::u2_t)menu.IndexParentList)
		::unhoverMenuHierarchy(gui, desktop, desktop.Items.ControlLists[menu.IndexParentList]);
	
	gui.Controls.SetHovered(menu.IdControl, false);

	for(::llc::u2_t iItem = 0; iItem < menu.IdControls.size(); ++iItem)
		gui.Controls.SetHovered(menu.IdControls[iItem], false);

	return 0;
}

static	::llc::error_t	clearMenuHierarchy	(::llc::SGUI & gui, ::llc::SDesktop & desktop)	{
	::llc::SRecyclableElementContainer<::llc::SControlList>	& menus	= desktop.Items.ControlLists;
	for(::llc::u2_t iMenu = 0, countMenus = menus.size(); iMenu < countMenus; ++iMenu) {
		::llc::SControlList			& menu				= menus[iMenu];
		menu.IdSelected			= -1;
		if(desktop.Children.size() <= (::llc::u2_t)menu.IndexParentList || desktop.Children[menu.IndexParentList].size() <= (::llc::u2_t)menu.IndexParentItem) // skip root menus
			continue;

		gui.Controls.SetHidden(menu.IdControl, true);
	}
	return 0;
}

static	int64_t			viewportUpdate		(::llc::SGUI & gui, ::llc::SDesktop & desktop, ::llc::SInput & input, ::llc::cid_t iViewport)							{
	::llc::SViewport			& vp				= desktop.Items.Viewports[iViewport];
	if(gui.Controls.States[vp.IdControls[::llc::VIEWPORT_CONTROL_CLOSE]].IsAction())
		::llc::desktopDeleteViewport(gui, desktop, iViewport);
	else if(gui.Controls.IsPressed(vp.IdControls[::llc::VIEWPORT_CONTROL_TITLE]))
		::pushToFrontAndDisplace(gui, vp.IdControl, input);
	else if(gui.Controls.IsPressed(vp.IdControls[::llc::VIEWPORT_CONTROL_RESIZE_BOTTOM_RIGHT])) {
		if(input.MouseCurrent.Deltas.x || input.MouseCurrent.Deltas.y) {
			//::llc::n2<int32_t> nextSize = gui.Controls.Controls[(::llc::u2_t)vp.IdControl].Area.Size + ::llc::n2<int32_t>{input.MouseCurrent.Deltas.x, input.MouseCurrent.Deltas.y};
			//if(::llc::in_range(nextSize, {{}, gui.LastSize.s2()})) {
				::llc::n2s2_t			mouseDeltas			= {input.MouseCurrent.Deltas.x, input.MouseCurrent.Deltas.y};
				const ::llc::n2f3_t		currentScale		= gui.Zoom.DPI * gui.Zoom.ZoomLevel;
				::llc::n2s1_t			deltasScaled		= mouseDeltas.GetScaled(1.0 / currentScale.x, 1.0 / currentScale.y).s1();
				gui.Controls.Placement[vp.IdControl].Area.Size									+= deltasScaled;
				gui.Controls.Placement[vp.IdControls[::llc::VIEWPORT_CONTROL_TARGET]].Area.Size	+= deltasScaled;
				::llc::controlMetricsInvalidate(gui, vp.IdControl);
			//}
		}
	}
	return 0;
}
int64_t					llc::desktopUpdate						(::llc::SGUI & gui, ::llc::SDesktop & desktop, ::llc::SInput & input)							{
	for(::llc::u2_t iViewport = 0; iViewport < desktop.Items.Viewports.size(); ++iViewport) {
		if(desktop.Items.Viewports.Unused[iViewport])
			continue;
		::viewportUpdate(gui, desktop, input, (::llc::cid_t)iViewport);
	}

	::llc::acid					controlsToProcess						= {};
	::llc::guiGetProcessableControls(gui, controlsToProcess);

	bool						inControlArea						= false;
	//bool						anyControlPressed					= false;
	for(::llc::u2_t iControlToProcess = 0, countControls = controlsToProcess.size(); iControlToProcess < countControls; ++iControlToProcess) {
		const ::llc::u2_t				iControl							= controlsToProcess[iControlToProcess];
		const ::llc::SControlArea	& controlMetrics					= gui.Controls.Area[iControl];
		const ::llc::SControlState	& controlState						= gui.Controls.States[iControl];
		if(iControl != (::llc::u2_t)desktop.IdControl)
			inControlArea			= inControlArea || ::llc::in_range(gui.CursorPos.s1(), controlMetrics.Total.Global);
		//if(controlState.Pressed && (int32_t)iControl != app.Desktop.IdControl)
		//	anyControlPressed															= true;

		if(controlState.IsAction()) {
			info_printf("Executed %u.", iControl);
			::clearMenuHierarchy(gui, desktop);
		}
	}

	bool						selectedMenu					= false;
	::llc::SRecyclableElementContainer<::llc::SControlList>	& menus			= desktop.Items.ControlLists;
	for(::llc::u2_t iMenu = 0, countMenus = menus.size(); iMenu < countMenus; ++iMenu) {
		::llc::SControlList			& menu				= menus[iMenu];
		if(desktop.Children.size() <= (::llc::u2_t)menu.IndexParentList)
			continue;
		else if(desktop.Children[menu.IndexParentList].size() <= (::llc::u2_t)menu.IndexParentItem) // parent
			continue;

		::llc::SControlList					& parentMenu		= menus[menu.IndexParentList];
		const ::llc::SControlState			& parentItemState	= gui.Controls.States[parentMenu.IdControls[menu.IndexParentItem]];
		if(parentItemState.IsHovered()) {
			::unhideMenuHierarchy(gui, desktop, menu);
			if(parentItemState.IsAction()) {
				::clearMenuHierarchy(gui, desktop);
				::selectMenuHierarchy(gui, desktop, menu);
				selectedMenu																= true;
			}
		}
		else {
			const ::llc::SControlArea		& controlListMetrics				= gui.Controls.Area[menu.IdControl];
			if(::llc::in_range(gui.CursorPos.s1(), controlListMetrics.Total.Global) && ::llc::controlHidden(gui, menu.IdControl))
				::unhideMenuHierarchy(gui, desktop, menu);

			else if(parentMenu.IdSelected != menu.IndexParentItem)
				gui.Controls.SetHidden(menu.IdControl, true);
		}
	}

	if(input.ButtonDown(1) || input.ButtonDown(2)) {
		::clearMenuHierarchy(gui, desktop);
		for(::llc::u2_t iMenu = 0, countMenus = menus.size(); iMenu < countMenus; ++iMenu) {
			::llc::SControlList																& menu								= menus[iMenu];
			if(desktop.Children.size() <= (::llc::u2_t)menu.IndexParentList || desktop.Children[menu.IndexParentList].size() <= (::llc::u2_t)menu.IndexParentItem) // parent
				continue;
			::unhoverMenuHierarchy(gui, desktop, menu);
		}
	}
	if(input.ButtonUp(0) && false == selectedMenu)
		::clearMenuHierarchy(gui, desktop);

	int64_t																	resultEvent							= 0;
	for(::llc::u2_t iMenu = 0, countMenus = menus.size(); iMenu < countMenus; ++iMenu) {
		::llc::SControlList																& menu								= menus[iMenu];
		bool																			eventFound							= false;
		for(::llc::u2_t iItem = 0, countItems = menu.IdControls.size(); iItem < countItems; ++iItem) {
			if(false == gui.Controls.States[menu.IdControls[iItem]].IsAction())
				continue;
			if(desktop.Children[iMenu].size() < menu.IdControls.size())
				desktop.Children[iMenu].resize(menu.IdControls.size(), -1);
			if(desktop.Children[iMenu][iItem] != -1)
				continue;
			resultEvent			= menu.Events[iItem];
			eventFound			= true;
		}
		if(eventFound)
			break;
	}
	return resultEvent;
}

::llc::error_t			llc::desktopControlListSetParent(::llc::SGUI & gui, ::llc::SDesktop & desktop, ::llc::cid_t iControlList, ::llc::cid_t iParentControlList, ::llc::cid_t iParentListItem)			{
	if_true_fef((desktop.Items.ControlLists.size() <= (::llc::u2_t)iControlList) || desktop.Items.ControlLists.Unused[iControlList], "Invalid control list: %u.", iControlList);
	::llc::SControlList			& controlList				= desktop.Items.ControlLists[iControlList];
	const int32_t				oldParentList				= controlList.IndexParentList;
	const int32_t				oldParentItem				= controlList.IndexParentItem;
	if(oldParentItem == iParentListItem && oldParentList == iParentControlList)	// Exit early if there is nothing to do here.
		return 0;
	controlList.IndexParentList = iParentControlList;
	controlList.IndexParentItem	= iParentListItem;
	
	if((desktop.Items.ControlLists.size() <= (::llc::u2_t)iParentControlList) || desktop.Items.ControlLists.Unused[iParentControlList]) {
		if_true_fef(-1 != iParentControlList, "Invalid parent control list: %i. List item: %i.", iParentControlList, iParentListItem);
		gui.Controls.SetHidden(controlList.IdControl, false);
	}
	else { // update tree
		::llc::SControlList			& parentControlList			= desktop.Items.ControlLists[iParentControlList];
		if(desktop.Children.size() > (::llc::u2_t)oldParentList && desktop.Children[oldParentList].size() > (::llc::u2_t)oldParentItem)
			desktop.Children[oldParentList][oldParentItem]							= -1;

		::llc::acid					& parentChildren			= desktop.Children[iParentControlList];
		if(parentChildren.size() < parentControlList.IdControls.size())
			parentChildren.resize(parentControlList.IdControls.size(), -1);

		if_true_fef(parentChildren.size() <= ((::llc::u2_t)iParentListItem) && iParentListItem != -1, "Invalid parent item: %i.", iParentListItem);
		::llc::SControlConstraints	& controlListConstraints	= gui.Controls.Constraints[controlList.IdControl];
		if(iParentListItem == -1) 
			gui.Controls.SetHidden(controlList.IdControl, false);
		else {
			controlList.Orientation	= ::llc::DIRECTION_DOWN;
			parentChildren[iParentListItem]	= iControlList;
			gui.Controls.SetHidden(controlList.IdControl, true);
			::llc::controlListArrange(gui, parentControlList);
			if(controlList.Orientation != ::llc::DIRECTION_RIGHT)
				controlListConstraints.DockToControl.Left	= parentControlList.IdControls[iParentListItem];
			if(parentControlList.IndexParentItem != -1 && parentControlList.IndexParentList != -1) {
				controlListConstraints.DockToControl		= {-1, -1, -1, -1};
				controlListConstraints.DockToControl.Right	= parentControlList.IdControls[iParentListItem];
				::llc::SControlPlacement	& controlListControl				= gui.Controls.Placement[controlList.IdControl];
				::llc::SControlConstraints	& parentControlListConstraints		= gui.Controls.Constraints[parentControlList.IdControl];
				if(gui.Controls.Constraints[parentControlList.IdControl].DockToControl.Bottom != -1) {
					int32_t offsety = gui.Controls.Placement[parentControlListConstraints.DockToControl.Bottom].Area.Offset.y + gui.Controls.Placement[parentControlListConstraints.DockToControl.Bottom].Area.Size.y + gui.Controls.Placement[parentControlList.IdControls[iParentListItem]].Area.Size.y * iParentListItem;
					controlListControl.Area.Offset.y	= (int16_t)offsety;
				}
				else
					controlListControl.Area.Offset.y	= gui.Controls.Placement[parentControlList.IdControl].Area.Offset.y + gui.Controls.Placement[parentControlList.IdControls[iParentListItem]].Area.Offset.y;
			}
			else
				controlListConstraints.DockToControl.Bottom	= parentControlList.IdControl;
		}
	}
	return ::llc::controlListArrange(gui, controlList);
}
