#include "llc_gui.h"
#include "llc_rect_align.h"

sttc ::llc::n2f3_t guiScale(cnst ::llc::SGUI & gui) nxpt {
	::llc::n2f3_t scale = gui.Zoom.DPI * gui.Zoom.ZoomLevel;
	if(::fabs(1.0 - scale.x) < 0.001) scale.x = 1.0;
	if(::fabs(1.0 - scale.y) < 0.001) scale.y = 1.0;
	rtrn scale;
}

sttc ::llc::err_t controlTextMeasure(cnst ::llc::SGUI & gui, cnst ::llc::SControlText & controlText, cnst ::llc::FGUITextMeasure & textMeasure, ::llc::n2u1_t & textSize) {
	textSize = {};
	if(0 == controlText.Text.size())
		rtrn 0;
	if_true_fef(false == (bool)textMeasure, "Font:%i, text:\"%.*s\".", controlText.FontSelected, LLC_VIEW_ARGS(controlText.Text));
	cnst ::llc::s1_t iFont = controlText.FontSelected >= 0 ? controlText.FontSelected : (::llc::s1_t)gui.SelectedFont;
	rtrn textMeasure(iFont, controlText.Text, textSize);
}

sttc ::llc::rects1_t controlScaledRect(cnst ::llc::rectu1_t & rectangle, cnst ::llc::n2f3_t & scale) nxpt {
	rtrn
		{ (::llc::s1_t)(rectangle.Left   * scale.x)
		, (::llc::s1_t)(rectangle.Top    * scale.y)
		, (::llc::s1_t)(rectangle.Right  * scale.x)
		, (::llc::s1_t)(rectangle.Bottom * scale.y)
		};
}

sttc ::llc::err_t controlGeometryBuild(cnst ::llc::SControlPlacement & placement, cnst ::llc::SControlArea & metrics, cnst ::llc::n2f3_t & scale, ::llc::view<::llc::rect2s1_t> rectangles, ::llc::view<::llc::tri2s1_t> triangles) {
	if_true_fe(rectangles.size() < ::llc::GUI_CONTROL_AREA_OLD_COUNT);
	if_true_fe(triangles.size() < 8);
	cnst ::llc::rects1_t border = ::controlScaledRect(placement.Border, scale);
	cnst ::llc::rects1_t margin = ::controlScaledRect(placement.Margin, scale);
	cnst ::llc::rect2s1_t & total = metrics.Total.Global;

	rectangles[::llc::GUI_CONTROL_AREA_OLD_CLIENT] = metrics.Client.Global;
	rectangles[::llc::GUI_CONTROL_AREA_OLD_CLIENT].Offset -= {margin.Left, margin.Top};
	rectangles[::llc::GUI_CONTROL_AREA_OLD_CLIENT].Size += {::llc::s1_t(margin.Left + margin.Right), ::llc::s1_t(margin.Top + margin.Bottom)};
	rectangles[::llc::GUI_CONTROL_AREA_OLD_BORDER_LEFT  ] = {total.Offset, {border.Left, total.Size.y}};
	rectangles[::llc::GUI_CONTROL_AREA_OLD_BORDER_TOP   ] = {total.Offset, {total.Size.x, border.Top}};
	rectangles[::llc::GUI_CONTROL_AREA_OLD_BORDER_RIGHT ] = {total.Offset + ::llc::n2s1_t{::llc::s1_t(total.Size.x - border.Right), 0}, {border.Right, total.Size.y}};
	rectangles[::llc::GUI_CONTROL_AREA_OLD_BORDER_BOTTOM] = {total.Offset + ::llc::n2s1_t{0, ::llc::s1_t(total.Size.y - border.Bottom)}, {total.Size.x, border.Bottom}};

	cnst ::llc::n2s1_t outerLeftTop     = total.Offset;
	cnst ::llc::n2s1_t innerLeftTop     = outerLeftTop + ::llc::n2s1_t{border.Left, border.Top};
	cnst ::llc::n2s1_t outerRightTop    = total.Offset + ::llc::n2s1_t{total.Size.x, 0};
	cnst ::llc::n2s1_t innerRightTop    = outerRightTop - ::llc::n2s1_t{border.Right, -border.Top};
	cnst ::llc::n2s1_t outerLeftBottom  = total.Offset + ::llc::n2s1_t{0, total.Size.y};
	cnst ::llc::n2s1_t innerLeftBottom  = outerLeftBottom + ::llc::n2s1_t{border.Left, -border.Bottom};
	cnst ::llc::n2s1_t outerRightBottom = total.Offset + total.Size;
	cnst ::llc::n2s1_t innerRightBottom = outerRightBottom - ::llc::n2s1_t{border.Right, border.Bottom};

	triangles[0] = {outerLeftTop, innerLeftTop, {innerLeftTop.x, outerLeftTop.y}};
	triangles[1] = {outerLeftTop, {outerLeftTop.x, innerLeftTop.y}, innerLeftTop};
	triangles[2] = {{innerRightTop.x, outerRightTop.y}, innerRightTop, outerRightTop};
	triangles[3] = {outerRightTop, innerRightTop, {outerRightTop.x, innerRightTop.y}};
	triangles[4] = {{outerLeftBottom.x, innerLeftBottom.y}, outerLeftBottom, innerLeftBottom};
	triangles[5] = {innerLeftBottom, outerLeftBottom, {innerLeftBottom.x, outerLeftBottom.y}};
	triangles[6] = {innerRightBottom, outerRightBottom, {outerRightBottom.x, innerRightBottom.y}};
	triangles[7] = {innerRightBottom, {innerRightBottom.x, outerRightBottom.y}, outerRightBottom};
	rtrn 0;
}

sttc ::llc::err_t controlUpdateMetrics(::llc::SGUI & gui, ::llc::cid_t iControl, cnst ::llc::n2u1_t & rootSize, cnst ::llc::FGUITextMeasure & textMeasure) {
	if_true_fe(::llc::controlInvalid(gui, iControl));
	cnst ::llc::SControlEntity & entity = gui.Controls.Entities[iControl];
	cnst ::llc::SControlPlacement & placement = gui.Controls.Placement[iControl];
	cnst ::llc::SControlConstraints & constraints = gui.Controls.Constraints[iControl];
	cnst ::llc::SControlText & controlText = gui.Controls.Text[iControl];
	::llc::SControlArea & metrics = gui.Controls.Area[iControl];
	cnst ::llc::n2f3_t scale = ::guiScale(gui);
	::llc::n2f3_t scaledPosition = {placement.Area.Offset.x * scale.x, placement.Area.Offset.y * scale.y};
	::llc::n2f3_t scaledSize = {placement.Area.Size.x * scale.x, placement.Area.Size.y * scale.y};
	cnst ::llc::rects1_t nonClient = ::controlScaledRect(::llc::controlNCRect(placement), scale);
	cnst ::llc::n2f3_t nonClientSize = {(double)nonClient.Left + nonClient.Right, (double)nonClient.Top + nonClient.Bottom};
	::llc::n2u1_t measuredTextSize = {};
	if_fail_fe(::controlTextMeasure(gui, controlText, textMeasure, measuredTextSize));
	cnst ::llc::n2f3_t textSize = {measuredTextSize.x * scale.x, measuredTextSize.y * scale.y};
	if(constraints.AttachSizeToText.x) scaledSize.x = textSize.x + nonClientSize.x;
	if(constraints.AttachSizeToText.y) scaledSize.y = textSize.y + nonClientSize.y;
	cnst bool hasParent = false == ::llc::controlInvalid(gui, entity.Parent);
	cnst ::llc::n2u1_t targetSize = hasParent ? gui.Controls.Area[entity.Parent].Client.Global.Size.u1() : rootSize;

	if(constraints.AttachSizeToControl.x == iControl) {
		if(constraints.DockToControl.Left == ::llc::CID_INVALID) scaledPosition.x = 0;
		scaledSize.x = targetSize.x;
	}
	else if(false == ::llc::controlInvalid(gui, constraints.AttachSizeToControl.x)) {
		if(constraints.DockToControl.Left == ::llc::CID_INVALID) scaledPosition.x = 0;
		scaledSize.x = gui.Controls.Area[constraints.AttachSizeToControl.x].Total.Global.Size.x;
	}
	if(constraints.AttachSizeToControl.y == iControl) {
		if(constraints.DockToControl.Top == ::llc::CID_INVALID) scaledPosition.y = 0;
		scaledSize.y = targetSize.y;
	}
	else if(false == ::llc::controlInvalid(gui, constraints.AttachSizeToControl.y)) {
		if(constraints.DockToControl.Top == ::llc::CID_INVALID) scaledPosition.y = 0;
		scaledSize.y = gui.Controls.Area[constraints.AttachSizeToControl.y].Total.Global.Size.y;
	}

	cnst ::llc::n2f3_t minimumSize = constraints.SizeMinMax.Min.f3().GetScaled(scale);
	cnst ::llc::n2f3_t maximumSize = constraints.SizeMinMax.Max.f3().GetScaled(scale);
	scaledSize.x = ::llc::max(minimumSize.x, ::llc::min(maximumSize.x, scaledSize.x));
	scaledSize.y = ::llc::max(minimumSize.y, ::llc::min(maximumSize.y, scaledSize.y));
	::llc::realignRectangle(targetSize, ::llc::rect2s1_t{scaledPosition.s1(), scaledSize.s1()}, metrics.Total.Local, placement.Align);
	metrics.Client.Local = {{nonClient.Left, nonClient.Top}, {::llc::s1_t(metrics.Total.Local.Size.x - nonClientSize.x), ::llc::s1_t(metrics.Total.Local.Size.y - nonClientSize.y)}};
	metrics.Total.Global = metrics.Total.Local;
	metrics.Client.Global = metrics.Client.Local;
	metrics.Client.Global.Offset += metrics.Total.Local.Offset;
	if(hasParent) {
		cnst ::llc::SControlArea & parentMetrics = gui.Controls.Area[entity.Parent];
		metrics.Total.Global.Offset += parentMetrics.Client.Global.Offset;
		metrics.Client.Global.Offset += parentMetrics.Client.Global.Offset;
	}

	cnst ::llc::rectcid & dock = constraints.DockToControl;
	if(dock.Right != ::llc::CID_INVALID) {
		if_true_fe(::llc::controlInvalid(gui, dock.Right));
		if_fail_fe(::llc::controlUpdateMetricsTopToDown(gui, dock.Right, rootSize, textMeasure, true));
		cnst ::llc::SControlPlacement & otherPlacement = gui.Controls.Placement[dock.Right];
		cnst ::llc::SControlArea & otherMetrics = gui.Controls.Area[dock.Right];
		if(::llc::bit_true(otherPlacement.Align, ::llc::ALIGN_RIGHT) && ::llc::bit_false(otherPlacement.Align, ::llc::ALIGN_HCENTER)) {
			metrics.Total.Global.Offset.x = otherMetrics.Total.Global.Offset.x - metrics.Total.Global.Size.x;
			metrics.Client.Global.Offset.x = metrics.Total.Global.Offset.x + nonClient.Left;
			cnst ::llc::s1_t difference = targetSize.x - otherMetrics.Total.Local.Offset.x;
			if(constraints.AttachSizeToControl.x == iControl) {
				metrics.Total.Global.Size.x -= difference;
				metrics.Client.Global.Size.x -= difference;
				metrics.Total.Global.Offset.x += difference;
				metrics.Client.Global.Offset.x += difference;
			}
		}
		else {
			metrics.Total.Global.Offset.x = otherMetrics.Total.Global.Offset.x + otherMetrics.Total.Global.Size.x;
			metrics.Client.Global.Offset.x = metrics.Total.Global.Offset.x + nonClient.Left;
		}
	}
	if(dock.Bottom != ::llc::CID_INVALID) {
		if_true_fe(::llc::controlInvalid(gui, dock.Bottom));
		cnst ::llc::SControlPlacement & otherPlacement = gui.Controls.Placement[dock.Bottom];
		cnst ::llc::SControlArea & otherMetrics = gui.Controls.Area[dock.Bottom];
		if(::llc::bit_true(otherPlacement.Align, ::llc::ALIGN_BOTTOM) && ::llc::bit_false(otherPlacement.Align, ::llc::ALIGN_VCENTER))
			metrics.Total.Global.Offset.y = otherMetrics.Total.Global.Offset.y - metrics.Total.Global.Size.y;
		else
			metrics.Total.Global.Offset.y = otherMetrics.Total.Global.Offset.y + otherMetrics.Total.Global.Size.y;
		metrics.Client.Global.Offset.y = metrics.Total.Global.Offset.y + nonClient.Top;
	}
	if(dock.Left != ::llc::CID_INVALID) {
		if_true_fe(::llc::controlInvalid(gui, dock.Left));
		if_fail_fe(::llc::controlUpdateMetricsTopToDown(gui, dock.Left, rootSize, textMeasure, true));
		cnst ::llc::SControlPlacement & otherPlacement = gui.Controls.Placement[dock.Left];
		cnst ::llc::SControlArea & otherMetrics = gui.Controls.Area[dock.Left];
		metrics.Total.Global.Offset.x = ::llc::bit_true(otherPlacement.Align, ::llc::ALIGN_RIGHT) && ::llc::bit_false(otherPlacement.Align, ::llc::ALIGN_HCENTER)
			? otherMetrics.Total.Global.Offset.x + otherMetrics.Total.Global.Size.x - metrics.Total.Global.Size.x
			: otherMetrics.Total.Global.Offset.x;
		metrics.Client.Global.Offset.x = metrics.Total.Global.Offset.x + nonClient.Left;
	}
	if(dock.Top != ::llc::CID_INVALID) {
		if_true_fe(::llc::controlInvalid(gui, dock.Top));
		cnst ::llc::SControlPlacement & otherPlacement = gui.Controls.Placement[dock.Top];
		cnst ::llc::SControlArea & otherMetrics = gui.Controls.Area[dock.Top];
		metrics.Total.Global.Offset.y = ::llc::bit_true(otherPlacement.Align, ::llc::ALIGN_BOTTOM) && ::llc::bit_false(otherPlacement.Align, ::llc::ALIGN_VCENTER)
			? otherMetrics.Total.Global.Offset.y - metrics.Total.Global.Size.y
			: otherMetrics.Total.Global.Offset.y + metrics.Total.Global.Offset.y;
		metrics.Client.Global.Offset.y = metrics.Total.Global.Offset.y + nonClient.Top;
	}

	metrics.Text = {{}, textSize.s1()};
	::llc::realignRectangle(metrics.Client.Global.Size.u1(), metrics.Text, metrics.Text, controlText.Align);
	metrics.Text.Offset += metrics.Client.Global.Offset;
	if_fail_fe(::controlGeometryBuild(placement, metrics, scale, metrics.Rectangles, metrics.Triangles));
	gui.Controls.SetUpdated(iControl, true);
	rtrn 0;
}

::llc::err_t llc::controlUpdateMetricsTopToDown(::llc::SGUI & gui, ::llc::cid_t iControl, cnst ::llc::n2u1_t & targetSize, cnst ::llc::FGUITextMeasure & textMeasure, bool forceUpdate) {
	if_true_fe(::llc::controlInvalid(gui, iControl));
	if(false == gui.Controls.IsUpdated(iControl) || forceUpdate)
		if_fail_fe(::controlUpdateMetrics(gui, iControl, targetSize, textMeasure));
	cnst ::llc::vcid children = gui.Controls.Children[iControl];
	for(::llc::u2_t iChild = 0; iChild < children.size(); ++iChild)
		if_fail_fe(::llc::controlUpdateMetricsTopToDown(gui, children[iChild], targetSize, textMeasure, forceUpdate));
	rtrn 0;
}

::llc::err_t llc::controlUpdateMetricsTopToDown(::llc::SGUI & gui, ::llc::cid_t iControl, cnst ::llc::n2u1_t & targetSize, bool forceUpdate) {
	rtrn ::llc::controlUpdateMetricsTopToDown(gui, iControl, targetSize, ::llc::FGUITextMeasure{}, forceUpdate);
}

::llc::err_t llc::guiUpdateMetrics(::llc::SGUI & gui, cnst ::llc::n2u1_t & targetSize, cnst ::llc::FGUITextMeasure & textMeasure, bool forceUpdate) {
	for(::llc::u2_t iControl = 0; iControl < gui.Controls.Entities.size(); ++iControl) {
		cnst ::llc::cid_t idControl = (::llc::cid_t)iControl;
		if(::llc::controlInvalid(gui, idControl))
			continue;
		cnst ::llc::cid_t idParent = gui.Controls.Entities[iControl].Parent;
		if(idParent == ::llc::CID_INVALID || ::llc::controlInvalid(gui, idParent))
			if_fail_fe(::llc::controlUpdateMetricsTopToDown(gui, idControl, targetSize, textMeasure, forceUpdate));
	}
	gui.LastSize = targetSize;
	gui.LastZoom = gui.Zoom;
	rtrn 0;
}

::llc::err_t llc::guiUpdateMetrics(::llc::SGUI & gui, cnst ::llc::n2u1_t & targetSize, bool forceUpdate) {
	rtrn ::llc::guiUpdateMetrics(gui, targetSize, ::llc::FGUITextMeasure{}, forceUpdate);
}
