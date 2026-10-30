#include "llc_gui_viewport.h"
#include "llc_label.h"

bool					llc::viewportIsPressed	(cnst ::llc::SGUI & gui, cnst ::llc::SViewport & viewport)											nxpt	{
	rtrn gui.Controls.IsPressed(viewport.IdControl)
		|| gui.Controls.IsPressed(viewport.IdControls[::llc::VIEWPORT_CONTROL_TITLE])
		|| gui.Controls.IsPressed(viewport.IdControls[::llc::VIEWPORT_CONTROL_TARGET])
		;
}

::llc::err_t			llc::viewportActivate	(::llc::SGUI & gui, ::llc::cid_t idParent, ::llc::view<cnst ::llc::SViewport> viewports, ::llc::u0_t iViewport) {
	if_true_fe(iViewport >= viewports.size());
	cnst ::llc::cid_t			idRoot				= viewports[iViewport].IdControl;
	::llc::acid					& parentChildren		= gui.Controls.Children[idParent];
	if(0 == parentChildren.size() || parentChildren[parentChildren.size() - 1] != idRoot) {
		cnst ::llc::err_t			iChild				= parentChildren.find(idRoot);
		if(iChild >= 0)
			if_fail_fe(parentChildren.remove((::llc::u2_t)iChild));
		if_fail_fe(parentChildren.push_back(idRoot));
	}
	for(::llc::u0_t iOther = 0; iOther < viewports.size(); ++iOther) {
		cnst bool					active				= iOther == iViewport;
		gui.Controls.SetSelected(viewports[iOther].IdControl, active);
		gui.Controls.SetSelected(viewports[iOther].IdControls[::llc::VIEWPORT_CONTROL_TITLE], active);
	}
	rtrn 0;
}

::llc::err_t			llc::viewportFold		(::llc::SGUI & gui, ::llc::SViewport & viewport) {
	::llc::SControlPlacement	& root					= gui.Controls.Placement[viewport.IdControl];
	cnst ::llc::cid_t			idClient				= viewport.IdControls[::llc::VIEWPORT_CONTROL_TARGET];
	if(viewport.Folded) {
		root.Area.Size		= viewport.UnfoldedSize;
		gui.Controls.SetHidden(idClient, false);
	}
	else {
		viewport.UnfoldedSize	= root.Area.Size;
		root.Area.Size.y		= 30;
		gui.Controls.SetHidden(idClient, true);
	}
	viewport.Folded			= false == viewport.Folded;
	rtrn ::llc::controlMetricsInvalidate(gui, viewport.IdControl);
}

::llc::err_t			llc::viewportInitialize	(::llc::SGUI & gui, ::llc::SViewport & viewport)			{
	const ::llc::n2u0_t			fontCharSize			= gui.Fonts[gui.SelectedFont]->CharSize;
	int16_t						heightTitleBar			= fontCharSize.y + 4;
	::llc::u2_t					widthTarget				= 800;
	const ::llc::n2f3_t			targetSize				= {(f3_t)widthTarget, widthTarget * (9 / 16.0)};

	//if(-1 == viewport.IdControl)
	{
		viewport.IdControl		= ::llc::controlCreate(gui);
		//::llc::SControlMode			& controlMode			= gui.Controls.Modes		[viewport.IdControl];
		//controlMode.Design		= true;
		::llc::SControlPlacement	& control				= gui.Controls.Placement	[viewport.IdControl];
		control.Align			= ::llc::ALIGN_TOP_LEFT;
		control.Border = control.Margin	= {1, 1, 1, 1};

		const ::llc::u2_t				widthViewport			= (::llc::u2_t)(targetSize.x + ::llc::controlNCSpacing(control).x);
		const ::llc::u2_t				heightViewport			= (::llc::u2_t)(targetSize.y + ((s3_t)heightTitleBar + ::llc::controlNCSpacing(control).y));
		control.Area.Size		= {(int16_t)widthViewport, (int16_t)heightViewport};
		for(::llc::u2_t iElement = 0; iElement < viewport.IdControls.size(); ++iElement) {
			if_fail_fe(viewport.IdControls[iElement] = ::llc::controlCreate(gui));
			::llc::SControlConstraints	& controlConstraints	= gui.Controls.Constraints	[viewport.IdControls[iElement]];
			controlConstraints.AttachSizeToControl.x	= viewport.IdControls[iElement];
			if_fail_fef(llc::controlSetParent(gui, viewport.IdControls[iElement], viewport.IdControl), "iElement: %i, viewport.IdControl: %i", iElement, viewport.IdControl);
		}
		for(::llc::u2_t iElement = 0; iElement < VIEWPORT_CONTROL_TITLE; ++iElement) {
			gui.Controls.Placement  [viewport.IdControls[iElement]].Area.Size			= {16, 16};
			gui.Controls.Placement  [viewport.IdControls[iElement]].Area.Offset			= {-2, -2};
			gui.Controls.Constraints[viewport.IdControls[iElement]].AttachSizeToControl	= {-1, -1};
		}
	}
	{
		gui.Controls.Placement[viewport.IdControls[VIEWPORT_CONTROL_RESIZE_LEFT			]].Align = ::llc::ALIGN_CENTER_LEFT		;
		gui.Controls.Placement[viewport.IdControls[VIEWPORT_CONTROL_RESIZE_TOP			]].Align = ::llc::ALIGN_CENTER_TOP		;
		gui.Controls.Placement[viewport.IdControls[VIEWPORT_CONTROL_RESIZE_RIGHT			]].Align = ::llc::ALIGN_CENTER_RIGHT	;
		gui.Controls.Placement[viewport.IdControls[VIEWPORT_CONTROL_RESIZE_BOTTOM		]].Align = ::llc::ALIGN_CENTER_BOTTOM	;
		gui.Controls.Placement[viewport.IdControls[VIEWPORT_CONTROL_RESIZE_TOP_LEFT		]].Align = ::llc::ALIGN_TOP_LEFT		;
		gui.Controls.Placement[viewport.IdControls[VIEWPORT_CONTROL_RESIZE_TOP_RIGHT		]].Align = ::llc::ALIGN_TOP_RIGHT		;
		gui.Controls.Placement[viewport.IdControls[VIEWPORT_CONTROL_RESIZE_BOTTOM_LEFT	]].Align = ::llc::ALIGN_BOTTOM_LEFT		;
		gui.Controls.Placement[viewport.IdControls[VIEWPORT_CONTROL_RESIZE_BOTTOM_RIGHT	]].Align = ::llc::ALIGN_BOTTOM_RIGHT	;

	}
	{
		::llc::SControlPlacement	& control				= gui.Controls.Placement[viewport.IdControls[VIEWPORT_CONTROL_CLOSE]];
		::llc::SControlText			& controlText			= gui.Controls.Text		[viewport.IdControls[VIEWPORT_CONTROL_CLOSE]];
		control.Area.Size		= {(int16_t)heightTitleBar, (int16_t)heightTitleBar};
		control.Align			= ::llc::ALIGN_TOP_RIGHT;
		controlText.Text		= {"X", (::llc::u2_t)1};
		controlText.Align		= ::llc::ALIGN_CENTER;
		::llc::SControlConstraints	& controlConstraints	= gui.Controls.Constraints[viewport.IdControls[VIEWPORT_CONTROL_CLOSE]];
		controlConstraints.AttachSizeToControl	= {-1, -1};
	}
	{
		::llc::SControlPlacement	& control				= gui.Controls.Placement[viewport.IdControls[VIEWPORT_CONTROL_TITLE]];
		::llc::SControlText			& controlText			= gui.Controls.Text		[viewport.IdControls[VIEWPORT_CONTROL_TITLE]];
		control.Area.Size.y		= heightTitleBar;
		static	s2_t				counterVPs				= 0;
		++counterVPs;
		sc_t						buffer [256];
		const s2_t				titleLen				= snprintf(buffer, ::llc::size(buffer) - 2, "Viewport #%i", counterVPs);
		controlText.Text		= {::llc::label(buffer).begin(), (::llc::u2_t)titleLen};
		controlText.Align		= ::llc::ALIGN_CENTER;
		control.Area.Offset.x	= heightTitleBar;
		control.Align			= ::llc::ALIGN_TOP_RIGHT;
		::llc::SControlConstraints	& controlConstraints	= gui.Controls.Constraints[viewport.IdControls[VIEWPORT_CONTROL_TITLE]];
		controlConstraints.DockToControl	= {-1, -1, viewport.IdControls[VIEWPORT_CONTROL_CLOSE], -1};
	}
	{
		::llc::SControlDraw				& controlDraw		= gui.Controls.Draw		[viewport.IdControls[VIEWPORT_CONTROL_TARGET]];
		::llc::SControlPlacement		& control			= gui.Controls.Placement[viewport.IdControls[VIEWPORT_CONTROL_TARGET]];
		::llc::SControlText				& controlText		= gui.Controls.Text		[viewport.IdControls[VIEWPORT_CONTROL_TARGET]];
		controlText.Align			= ::llc::ALIGN_CENTER_TOP;
		control.Border				= {};
		control.Margin				= {};
		control.Area.Size			= targetSize.s1();
		//control.Area.Offset.y		= heightTitleBar;
		controlDraw.ColorTheme			= 60;
		::llc::SControlConstraints		& controlConstraints	= gui.Controls.Constraints[viewport.IdControls[VIEWPORT_CONTROL_TARGET]];
		controlConstraints.DockToControl	= {-1, -1, -1, viewport.IdControls[VIEWPORT_CONTROL_TITLE]};
		//controlConstraints.AttachSizeToControl	= {-1, viewport.IdControl};
	}
	//for(::llc::u2_t i = 0; i < 2; ++i)
	//	::llc::controlUpdateMetricsTopToDown(gui, viewport.IdControl, gui.LastSize, true);
	return 0;
}
