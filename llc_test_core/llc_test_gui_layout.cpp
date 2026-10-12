#include "llc_gui.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(GUI_LAYOUT_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, UPDATE_RESULT		,  0, "GUI metric update reported failure for valid asymmetric geometry.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, UPDATED_STATE		,  1, "GUI metric update did not mark the control as updated.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, LAST_SIZE			,  2, "GUI metric update retained the wrong target size.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, LAST_ZOOM			,  3, "GUI metric update retained the wrong zoom.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, TOTAL_LOCAL		,  4, "The total local control rectangle is wrong.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, TOTAL_GLOBAL		,  5, "The total global control rectangle is wrong.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, CLIENT_LOCAL		,  6, "The client local control rectangle is wrong.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, CLIENT_GLOBAL		,  7, "The client global control rectangle is wrong.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, TEXT				,  8, "The text control rectangle is wrong.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, RECTANGLE_CLIENT	,  9, "The expanded client drawing rectangle is wrong.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, RECTANGLE_LEFT		, 10, "The left border drawing rectangle is wrong.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, RECTANGLE_TOP		, 11, "The top border drawing rectangle is wrong.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, RECTANGLE_RIGHT	, 12, "The right border drawing rectangle is wrong.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, RECTANGLE_BOTTOM	, 13, "The bottom border drawing rectangle is wrong.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, TRIANGLE_0			, 14, "The first top-left corner triangle is wrong.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, TRIANGLE_1			, 15, "The second top-left corner triangle is wrong.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, TRIANGLE_2			, 16, "The first top-right corner triangle is wrong.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, TRIANGLE_3			, 17, "The second top-right corner triangle is wrong.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, TRIANGLE_4			, 18, "The first bottom-left corner triangle is wrong.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, TRIANGLE_5			, 19, "The second bottom-left corner triangle is wrong.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, TRIANGLE_6			, 20, "The first bottom-right corner triangle is wrong.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, TRIANGLE_7			, 21, "The second bottom-right corner triangle is wrong.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, WINDING_0			, 22, "The first top-left corner triangle has the wrong winding.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, WINDING_1			, 23, "The second top-left corner triangle has the wrong winding.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, WINDING_2			, 24, "The first top-right corner triangle has the wrong winding.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, WINDING_3			, 25, "The second top-right corner triangle has the wrong winding.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, WINDING_4			, 26, "The first bottom-left corner triangle has the wrong winding.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, WINDING_5			, 27, "The second bottom-left corner triangle has the wrong winding.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, WINDING_6			, 28, "The first bottom-right corner triangle has the wrong winding.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, WINDING_7			, 29, "The second bottom-right corner triangle has the wrong winding.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, TEXT_MEASURE_COUNT	, 30, "The GUI did not request the size of the complete text exactly once.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, TEXT_MEASURE_FONT	, 31, "The GUI requested text size for the wrong font.");
GDEFINE_ENUM_VALUED(GUI_LAYOUT_TEST_RESULT, TEXT_MEASURE_TEXT	, 32, "The GUI split or changed the text passed to the measurement provider.");

sttc ::llc::err_t testGUILayoutPoint(ATestError & errors, GUI_LAYOUT_TEST_RESULT result, cnst ::llc::vcst_t & name, cnst ::llc::vcst_t & member, cnst ::llc::n2s1_t & actual, cnst ::llc::n2s1_t & expected) {
	LLC_TEST_CHECKF(errors, result, actual.x != expected.x, "%.*s.%.*s.x:%i, expected:%i.", LLC_VIEW_ARGS(name), LLC_VIEW_ARGS(member), actual.x, expected.x);
	LLC_TEST_CHECKF(errors, result, actual.y != expected.y, "%.*s.%.*s.y:%i, expected:%i.", LLC_VIEW_ARGS(name), LLC_VIEW_ARGS(member), actual.y, expected.y);
	rtrn 0;
}

sttc ::llc::err_t testGUILayoutRectangle(ATestError & errors, GUI_LAYOUT_TEST_RESULT result, cnst ::llc::vcst_t & name, cnst ::llc::rect2s1_t & actual, cnst ::llc::rect2s1_t & expected) {
	if_fail_fe(::testGUILayoutPoint(errors, result, name, "Offset", actual.Offset, expected.Offset));
	rtrn ::testGUILayoutPoint(errors, result, name, "Size", actual.Size, expected.Size);
}

sttc ::llc::err_t testGUILayoutTriangle(ATestError & errors, GUI_LAYOUT_TEST_RESULT result, GUI_LAYOUT_TEST_RESULT windingResult, ::llc::u2_t index, cnst ::llc::tri2s1_t & actual, cnst ::llc::tri2s1_t & expected) {
	LLC_TEST_CHECKF(errors, result, actual.A.x != expected.A.x, "Triangle[%u].A.x:%i, expected:%i.", index, actual.A.x, expected.A.x);
	LLC_TEST_CHECKF(errors, result, actual.A.y != expected.A.y, "Triangle[%u].A.y:%i, expected:%i.", index, actual.A.y, expected.A.y);
	LLC_TEST_CHECKF(errors, result, actual.B.x != expected.B.x, "Triangle[%u].B.x:%i, expected:%i.", index, actual.B.x, expected.B.x);
	LLC_TEST_CHECKF(errors, result, actual.B.y != expected.B.y, "Triangle[%u].B.y:%i, expected:%i.", index, actual.B.y, expected.B.y);
	LLC_TEST_CHECKF(errors, result, actual.C.x != expected.C.x, "Triangle[%u].C.x:%i, expected:%i.", index, actual.C.x, expected.C.x);
	LLC_TEST_CHECKF(errors, result, actual.C.y != expected.C.y, "Triangle[%u].C.y:%i, expected:%i.", index, actual.C.y, expected.C.y);
	cnst ::llc::s2_t signedArea = (::llc::s2_t(actual.B.x) - actual.A.x) * (::llc::s2_t(actual.C.y) - actual.A.y) - (::llc::s2_t(actual.B.y) - actual.A.y) * (::llc::s2_t(actual.C.x) - actual.A.x);
	LLC_TEST_CHECKF(errors, windingResult, signedArea >= 0, "Triangle[%u] signed area:%i, expected a negative winding.", index, signedArea);
	rtrn 0;
}

::llc::err_t testGUILayout(ATestError & errors) {
	::llc::SGUI gui = {};
	cnst ::llc::cid_t idControl = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_LAYOUT_TEST_RESULT_UPDATE_RESULT, idControl < 0, "Control creation result:%i.", idControl);
	gui.Controls.Placement[idControl].Area = {{10, 20}, {100, 80}};
	gui.Controls.Placement[idControl].Border = {2, 3, 5, 7};
	gui.Controls.Placement[idControl].Margin = {11, 13, 17, 19};
	gui.Controls.Text[idControl].Text = LLC_CXS("abcd");
	gui.Controls.Text[idControl].Align = ::llc::ALIGN_CENTER;
	gui.SelectedFont = 17;
	gui.Zoom.DPI = {2, 3};
	gui.Zoom.ZoomLevel = 1;
	::llc::u2_t measureCount = 0;
	::llc::s1_t measuredFont = -1;
	::llc::vcst_t measuredText = {};
	cnst ::llc::FGUITextMeasure textMeasure = [&measureCount, &measuredFont, &measuredText](::llc::s1_t font, ::llc::vcst_t text, ::llc::n2u1_t & size) { ++measureCount; measuredFont = font; measuredText = text; size = {23, 9}; rtrn 0; };
	cnst ::llc::n2u1_t targetSize = {400, 350};
	cnst ::llc::err_t updateResult = ::llc::guiUpdateMetrics(gui, targetSize, textMeasure, true);
	LLC_TEST_REQUIREF(errors, GUI_LAYOUT_TEST_RESULT_UPDATE_RESULT, ::llc::failed(updateResult), "Metric update result:%i.", updateResult);
	LLC_TEST_CHECKF(errors, GUI_LAYOUT_TEST_RESULT_TEXT_MEASURE_COUNT, measureCount != 1, "Measurement requests:%u, expected:1.", measureCount);
	LLC_TEST_CHECKF(errors, GUI_LAYOUT_TEST_RESULT_TEXT_MEASURE_FONT, measuredFont != 17, "Measured font:%i, expected:17.", measuredFont);
	LLC_TEST_CHECKF(errors, GUI_LAYOUT_TEST_RESULT_TEXT_MEASURE_TEXT, measuredText != LLC_CXS("abcd"), "Measured text:\"%.*s\", expected:\"abcd\".", LLC_VIEW_ARGS(measuredText));
	LLC_TEST_CHECK(errors, GUI_LAYOUT_TEST_RESULT_UPDATED_STATE, false == gui.Controls.IsUpdated(idControl));
	if_fail_fe(::testGUILayoutPoint(errors, GUI_LAYOUT_TEST_RESULT_LAST_SIZE, "GUI", "LastSize", gui.LastSize.s1(), targetSize.s1()));
	LLC_TEST_CHECKF(errors, GUI_LAYOUT_TEST_RESULT_LAST_ZOOM, gui.LastZoom != gui.Zoom, "Last zoom:{%g, %g} x %g, expected:{%g, %g} x %g.", gui.LastZoom.DPI.x, gui.LastZoom.DPI.y, gui.LastZoom.ZoomLevel, gui.Zoom.DPI.x, gui.Zoom.DPI.y, gui.Zoom.ZoomLevel);

	cnst ::llc::SControlArea & area = gui.Controls.Area[idControl];
	if_fail_fe(::testGUILayoutRectangle(errors, GUI_LAYOUT_TEST_RESULT_TOTAL_LOCAL, "Total.Local", area.Total.Local, {{20, 60}, {200, 240}}));
	if_fail_fe(::testGUILayoutRectangle(errors, GUI_LAYOUT_TEST_RESULT_TOTAL_GLOBAL, "Total.Global", area.Total.Global, {{20, 60}, {200, 240}}));
	if_fail_fe(::testGUILayoutRectangle(errors, GUI_LAYOUT_TEST_RESULT_CLIENT_LOCAL, "Client.Local", area.Client.Local, {{26, 48}, {130, 114}}));
	if_fail_fe(::testGUILayoutRectangle(errors, GUI_LAYOUT_TEST_RESULT_CLIENT_GLOBAL, "Client.Global", area.Client.Global, {{46, 108}, {130, 114}}));
	if_fail_fe(::testGUILayoutRectangle(errors, GUI_LAYOUT_TEST_RESULT_TEXT, "Text", area.Text, {{88, 152}, {46, 27}}));
	if_fail_fe(::testGUILayoutRectangle(errors, GUI_LAYOUT_TEST_RESULT_RECTANGLE_CLIENT, "Rectangles.Client", area.Rectangles[::llc::GUI_CONTROL_AREA_OLD_CLIENT], {{24, 69}, {186, 210}}));
	if_fail_fe(::testGUILayoutRectangle(errors, GUI_LAYOUT_TEST_RESULT_RECTANGLE_LEFT, "Rectangles.Left", area.Rectangles[::llc::GUI_CONTROL_AREA_OLD_BORDER_LEFT], {{20, 60}, {4, 240}}));
	if_fail_fe(::testGUILayoutRectangle(errors, GUI_LAYOUT_TEST_RESULT_RECTANGLE_TOP, "Rectangles.Top", area.Rectangles[::llc::GUI_CONTROL_AREA_OLD_BORDER_TOP], {{20, 60}, {200, 9}}));
	if_fail_fe(::testGUILayoutRectangle(errors, GUI_LAYOUT_TEST_RESULT_RECTANGLE_RIGHT, "Rectangles.Right", area.Rectangles[::llc::GUI_CONTROL_AREA_OLD_BORDER_RIGHT], {{210, 60}, {10, 240}}));
	if_fail_fe(::testGUILayoutRectangle(errors, GUI_LAYOUT_TEST_RESULT_RECTANGLE_BOTTOM, "Rectangles.Bottom", area.Rectangles[::llc::GUI_CONTROL_AREA_OLD_BORDER_BOTTOM], {{20, 279}, {200, 21}}));

	if_fail_fe(::testGUILayoutTriangle(errors, GUI_LAYOUT_TEST_RESULT_TRIANGLE_0, GUI_LAYOUT_TEST_RESULT_WINDING_0, 0, area.Triangles[0], {{20, 60}, {23, 68}, {23, 60}}));
	if_fail_fe(::testGUILayoutTriangle(errors, GUI_LAYOUT_TEST_RESULT_TRIANGLE_1, GUI_LAYOUT_TEST_RESULT_WINDING_1, 1, area.Triangles[1], {{20, 60}, {20, 68}, {23, 68}}));
	if_fail_fe(::testGUILayoutTriangle(errors, GUI_LAYOUT_TEST_RESULT_TRIANGLE_2, GUI_LAYOUT_TEST_RESULT_WINDING_2, 2, area.Triangles[2], {{210, 60}, {210, 68}, {219, 60}}));
	if_fail_fe(::testGUILayoutTriangle(errors, GUI_LAYOUT_TEST_RESULT_TRIANGLE_3, GUI_LAYOUT_TEST_RESULT_WINDING_3, 3, area.Triangles[3], {{219, 60}, {210, 68}, {219, 68}}));
	if_fail_fe(::testGUILayoutTriangle(errors, GUI_LAYOUT_TEST_RESULT_TRIANGLE_4, GUI_LAYOUT_TEST_RESULT_WINDING_4, 4, area.Triangles[4], {{20, 279}, {20, 299}, {23, 279}}));
	if_fail_fe(::testGUILayoutTriangle(errors, GUI_LAYOUT_TEST_RESULT_TRIANGLE_5, GUI_LAYOUT_TEST_RESULT_WINDING_5, 5, area.Triangles[5], {{23, 279}, {20, 299}, {23, 299}}));
	if_fail_fe(::testGUILayoutTriangle(errors, GUI_LAYOUT_TEST_RESULT_TRIANGLE_6, GUI_LAYOUT_TEST_RESULT_WINDING_6, 6, area.Triangles[6], {{210, 279}, {219, 299}, {219, 279}}));
	if_fail_fe(::testGUILayoutTriangle(errors, GUI_LAYOUT_TEST_RESULT_TRIANGLE_7, GUI_LAYOUT_TEST_RESULT_WINDING_7, 7, area.Triangles[7], {{210, 279}, {210, 299}, {219, 299}}));
	rtrn 0;
}
