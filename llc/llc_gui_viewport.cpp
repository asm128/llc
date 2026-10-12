#include "llc_gui_viewport.h"

#include "llc_label.h"

sttc ::llc::err_t viewportValidate(cnst ::llc::SViewport & viewport, cnst ::llc::SGUI & gui) {
	if_true_fef(::llc::controlInvalid(gui, viewport.IdRoot), "Invalid viewport root:%i.", viewport.IdRoot);
	if_true_fef(::llc::controlInvalid(gui, viewport.IdTitle), "Invalid viewport title:%i.", viewport.IdTitle);
	if_true_fef(::llc::controlInvalid(gui, viewport.IdClient), "Invalid viewport client:%i.", viewport.IdClient);
	if_true_fef(gui.Controls.Entities[viewport.IdTitle].Parent != viewport.IdRoot, "Viewport title:%i parent:%i, expected:%i.", viewport.IdTitle, gui.Controls.Entities[viewport.IdTitle].Parent, viewport.IdRoot);
	if_true_fef(gui.Controls.Entities[viewport.IdClient].Parent != viewport.IdRoot, "Viewport client:%i parent:%i, expected:%i.", viewport.IdClient, gui.Controls.Entities[viewport.IdClient].Parent, viewport.IdRoot);
	rtrn 0;
}

sttc ::llc::err_t viewportCreateFail(::llc::SViewport & viewport, ::llc::SGUI & gui, ::llc::err_t error) {
	if(viewport.IdRoot != ::llc::CID_INVALID) {
		::llc::controlDelete(gui, viewport.IdRoot);
	}
	viewport = {};
	rtrn error;
}

sttc ::llc::err_t viewportRootSize(::llc::n2s1_t & rootSize, ::llc::n2s1_c & clientSize, ::llc::s1_t titleHeight, cnst ::llc::SControlPlacement & rootPlacement) {
	if_true_fef(clientSize.x < 0 || clientSize.y < 0, "Invalid viewport client size:{%i, %i}.", clientSize.x, clientSize.y);
	if_true_fef(titleHeight <= 0, "Invalid viewport title height:%i.", titleHeight);
	cnst ::llc::n2s2_t result = clientSize.s2() + ::llc::controlNCSpacing(rootPlacement).s2() + ::llc::n2s2_t{0, (::llc::s2_t)titleHeight + ::llc::VIEWPORT_CLIENT_GAP};
	if_true_fef(result.x > 0x7FFF || result.y > 0x7FFF, "Viewport size exceeds signed 16-bit geometry:{%" LLC_FMT_S2 ", %" LLC_FMT_S2 "}.", result.x, result.y);
	rootSize = result.s1();
	rtrn 0;
}

sttc ::llc::err_t viewportClientSizeUpdate(::llc::SViewport & viewport, ::llc::SGUI & gui) {
	if_fail_fe(::viewportValidate(viewport, gui));
	if(false == viewport.Settings.Unfolded) {
		rtrn 0;
	}
	cnst ::llc::SControlPlacement & rootPlacement = gui.Controls.Placement[viewport.IdRoot];
	cnst ::llc::SControlPlacement & titlePlacement = gui.Controls.Placement[viewport.IdTitle];
	cnst ::llc::n2s1_t nonClientSize = ::llc::controlNCSpacing(rootPlacement).s1();
	cnst ::llc::n2s2_t clientSize = rootPlacement.Area.Size.s2() - nonClientSize.s2() - ::llc::n2s2_t{0, (::llc::s2_t)titlePlacement.Area.Size.y + ::llc::VIEWPORT_CLIENT_GAP};
	if_true_fef(clientSize.x < 0 || clientSize.y < 0 || clientSize.x > 0x7FFF || clientSize.y > 0x7FFF, "Invalid viewport client size:{%" LLC_FMT_S2 ", %" LLC_FMT_S2 "}.", clientSize.x, clientSize.y);
	::llc::SControlPlacement & clientPlacement = gui.Controls.Placement[viewport.IdClient];
	if(clientPlacement.Area.Size != clientSize.s1()) {
		clientPlacement.Area.Size = clientSize.s1();
		if_fail_fe(::llc::controlMetricsInvalidate(gui, viewport.IdClient));
	}
	rtrn 0;
}

::llc::err_t llc::viewportCreate(::llc::SViewport & viewport, ::llc::SGUI & gui, cnst ::llc::SViewportCreate & setup) {
	if_true_fef(viewport.IdRoot != ::llc::CID_INVALID || viewport.IdTitle != ::llc::CID_INVALID || viewport.IdClient != ::llc::CID_INVALID, "Viewport controls already assigned:{%i, %i, %i}.", viewport.IdRoot, viewport.IdTitle, viewport.IdClient);
	if_true_fef(setup.Parent != ::llc::CID_INVALID && ::llc::controlInvalid(gui, setup.Parent), "Invalid viewport parent:%i.", setup.Parent);
	if_true_fef(setup.ClientSize.x < 0 || setup.ClientSize.y < 0, "Invalid viewport client size:{%i, %i}.", setup.ClientSize.x, setup.ClientSize.y);
	if_true_fef(setup.TitleHeight <= 0, "Invalid viewport title height:%i.", setup.TitleHeight);
	::llc::SControlPlacement rootPlacement = {};
	rootPlacement.Align = ::llc::ALIGN_TOP_LEFT;
	rootPlacement.Border = rootPlacement.Margin = {1, 1, 1, 1};
	if_fail_fe(::viewportRootSize(rootPlacement.Area.Size, setup.ClientSize, setup.TitleHeight, rootPlacement));

	viewport.IdRoot = ::llc::controlCreate(gui);
	if_true_fef(viewport.IdRoot < 0, "Failed to create viewport root:%i.", viewport.IdRoot);
	if(setup.Parent != ::llc::CID_INVALID && ::llc::failed(::llc::controlSetParent(gui, viewport.IdRoot, setup.Parent))) {
		rtrn ::viewportCreateFail(viewport, gui, -1);
	}

	gui.Controls.Placement[viewport.IdRoot] = rootPlacement;
	gui.Controls.Draw[viewport.IdRoot].FrameOut = true;
	viewport.IdClient = ::llc::controlCreateChild(gui, viewport.IdRoot);
	if(viewport.IdClient < 0) {
		rtrn ::viewportCreateFail(viewport, gui, viewport.IdClient);
	}
	viewport.IdTitle = ::llc::controlCreateChild(gui, viewport.IdRoot);
	if(viewport.IdTitle < 0) {
		rtrn ::viewportCreateFail(viewport, gui, viewport.IdTitle);
	}

	::llc::SControlPlacement & titlePlacement = gui.Controls.Placement[viewport.IdTitle];
	titlePlacement.Area.Size = {0, setup.TitleHeight};
	titlePlacement.Align = ::llc::ALIGN_TOP_LEFT;
	titlePlacement.Margin.Left = 4;
	gui.Controls.Constraints[viewport.IdTitle].AttachSizeToControl.x = viewport.IdTitle;
	gui.Controls.Text[viewport.IdTitle].Align = ::llc::ALIGN_CENTER_LEFT;
	cnst ::llc::err_t titleResult = ::llc::controlTextSet(gui, viewport.IdTitle, ::llc::label(setup.Title));
	if(::llc::failed(titleResult)) {
		rtrn ::viewportCreateFail(viewport, gui, titleResult);
	}

	::llc::SControlPlacement & clientPlacement = gui.Controls.Placement[viewport.IdClient];
	clientPlacement.Area.Offset = {0, (::llc::s1_t)(setup.TitleHeight + ::llc::VIEWPORT_CLIENT_GAP)};
	clientPlacement.Area.Size = setup.ClientSize;
	clientPlacement.Margin = {};
	gui.Controls.Constraints[viewport.IdClient].AttachSizeToControl.x = viewport.IdClient;
	viewport.Settings = {};
	rtrn viewport.IdRoot;
}

::llc::err_t llc::viewportFold(::llc::SViewport & viewport, ::llc::SGUI & gui, bool fold) {
	if_fail_fe(::viewportValidate(viewport, gui));
	if(fold == false && viewport.Settings.Unfolded == true) {
		rtrn 0;
	}
	if(fold == true && viewport.Settings.Unfolded == false) {
		rtrn 1;
	}
	::llc::SControlPlacement & rootPlacement = gui.Controls.Placement[viewport.IdRoot];
	cnst ::llc::SControlPlacement & titlePlacement = gui.Controls.Placement[viewport.IdTitle];
	if(fold) {
		rootPlacement.Area.Size.y = (::llc::s1_t)(titlePlacement.Area.Size.y + ::llc::controlNCSpacing(rootPlacement).y);
	}
	else {
		cnst ::llc::SControlPlacement & clientPlacement = gui.Controls.Placement[viewport.IdClient];
		cnst ::llc::s2_t height = (::llc::s2_t)clientPlacement.Area.Size.y + titlePlacement.Area.Size.y + ::llc::VIEWPORT_CLIENT_GAP + ::llc::controlNCSpacing(rootPlacement).y;
		if_true_fef(height > 0x7FFF, "Viewport unfolded height exceeds signed 16-bit geometry:%" LLC_FMT_S2 ".", height);
		rootPlacement.Area.Size.y = (::llc::s1_t)height;
	}
	gui.Controls.SetHidden(viewport.IdClient, fold);
	viewport.Settings.Unfolded = false == fold;
	cnst ::llc::cid_t idInvalidate = gui.Controls.Entities[viewport.IdRoot].Parent != ::llc::CID_INVALID ? gui.Controls.Entities[viewport.IdRoot].Parent : viewport.IdRoot;
	if_fail_fe(::llc::controlMetricsInvalidate(gui, idInvalidate));
	rtrn one_if(fold);
}

::llc::err_t llc::viewportUpdate(::llc::SViewport & viewport, ::llc::SGUI & gui, ::llc::n2s2_c & mouseDeltas) {
	if_fail_fe(::viewportValidate(viewport, gui));
	::llc::SControlPlacement & rootPlacement = gui.Controls.Placement[viewport.IdRoot];
	cnst ::llc::SControlState & titleState = gui.Controls.States[viewport.IdTitle];
	if(titleState.IsPressed()) {
		cnst ::llc::n2f3_t scale = gui.Zoom.DPI * gui.Zoom.ZoomLevel;
		if_true_fef(0 == scale.x || 0 == scale.y, "Invalid GUI scale:{%f, %f}.", scale.x, scale.y);
		cnst ::llc::n2s1_t logicalDeltas = mouseDeltas.f3().GetScaled(1.0 / scale.x, 1.0 / scale.y).s1();
		if(logicalDeltas.x || logicalDeltas.y) {
			viewport.Settings.Dragging = true;
			if(false == viewport.Settings.DisplacementLock.x) {
				rootPlacement.Area.Offset.x += (::llc::bit_true(rootPlacement.Align, ::llc::ALIGN_RIGHT) && ::llc::bit_false(rootPlacement.Align, ::llc::ALIGN_HCENTER)) ? -logicalDeltas.x : logicalDeltas.x;
			}
			if(false == viewport.Settings.DisplacementLock.y) {
				rootPlacement.Area.Offset.y += (::llc::bit_true(rootPlacement.Align, ::llc::ALIGN_BOTTOM) && ::llc::bit_false(rootPlacement.Align, ::llc::ALIGN_VCENTER)) ? -logicalDeltas.y : logicalDeltas.y;
			}
			if_fail_fe(::llc::controlMetricsInvalidate(gui, viewport.IdRoot));
		}
	}
	else {
		if(titleState.IsAction() && false == viewport.Settings.Dragging) {
			if_fail_fe(::llc::viewportFold(viewport, gui, viewport.Settings.Unfolded));
		}
		viewport.Settings.Dragging = false;
	}
	rtrn ::viewportClientSizeUpdate(viewport, gui);
}
