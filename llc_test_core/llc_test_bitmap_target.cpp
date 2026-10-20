#include "llc_bitmap_target.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(BITMAP_TARGET_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, VERTICAL_RESULT				,  0, "drawLineVertical() reported the wrong written-cell count.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, VERTICAL_CELL				,  1, "drawLineVertical() wrote the wrong cell.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, HORIZONTAL_RESULT			,  2, "drawLineHorizontal() reported the wrong written-cell count.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, HORIZONTAL_CELL				,  3, "drawLineHorizontal() wrote the wrong cell.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, AXIS_OFFSCREEN_RESULT		,  4, "An offscreen axis-aligned line reported written cells.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, AXIS_OFFSCREEN_CELL			,  5, "An offscreen axis-aligned line changed the target.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LINE_RESULT					,  6, "drawLine() reported the wrong written-cell count.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LINE_CELL					,  7, "drawLine() wrote the wrong cell.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LINE_REVERSE_RESULT			,  8, "Reversing a line changed its written-cell count.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LINE_REVERSE_CELL			,  9, "Reversing a line changed its rasterized cells.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LINE_STEEP_RESULT			, 10, "A steep line reported the wrong written-cell count.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LINE_STEEP_CELL				, 11, "A steep line wrote the wrong cell.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LINE_CLIPPED_RESULT			, 12, "A clipped line reported the wrong written-cell count.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LINE_CLIPPED_CELL			, 13, "A clipped line wrote the wrong cell.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LINE_SINGLE_RESULT			, 14, "A single-cell line reported the wrong written-cell count.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LINE_SINGLE_CELL			, 15, "A single-cell line wrote the wrong cell.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LINE_CALLBACK_RESULT		, 16, "rasterLine() did not aggregate typed callback results.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LINE_CALLBACK_CELL			, 17, "rasterLine() called the typed callback for the wrong cell.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LINE_OUTPUT_RESULT			, 18, "The line point-output overload reported the wrong point count.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LINE_OUTPUT_SIZE			, 19, "The line point-output overload produced the wrong storage size.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LINE_OUTPUT_CELL			, 20, "The line point-output overload produced the wrong point.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LINE_OUTPUT_APPEND			, 21, "The line point-output overload discarded existing output.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LINE3_OUTPUT_RESULT			, 22, "The 3D-line point-output overload reported the wrong point count.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LINE3_OUTPUT_CELL			, 23, "The 3D-line point-output overload produced the wrong projected point.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LINE_OUTPUT_RANGE			, 24, "The line point-output overload accepted unrepresentable metrics.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LINE_OUTPUT_PRESERVED		, 25, "A rejected line point-output operation changed existing output.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, RECTANGLE_RESULT			, 26, "drawRectangleBorder() reported the wrong written-cell count.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, RECTANGLE_CELL				, 27, "drawRectangleBorder() wrote the wrong cell.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, RECTANGLE_CLIPPED_RESULT	, 28, "A clipped rectangle border reported the wrong written-cell count.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, RECTANGLE_CLIPPED_CELL		, 29, "A clipped rectangle border manufactured or lost an edge.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, RECTANGLE_THIN_RESULT		, 30, "A one-cell-wide rectangle border reported the wrong count.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, RECTANGLE_THIN_CELL			, 31, "A one-cell-wide rectangle border wrote the wrong cell.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, RECTANGLE_EMPTY_RESULT		, 32, "An empty rectangle border reported written cells.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, RECTANGLE_EMPTY_CELL		, 33, "An empty rectangle border changed the target.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, CIRCLE_RESULT				, 34, "drawCircle() reported the wrong written-cell count.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, CIRCLE_CELL					, 35, "drawCircle() wrote the wrong cell.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, CIRCLE_CLIPPED_RESULT		, 36, "A clipped circle reported the wrong written-cell count.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, CIRCLE_CLIPPED_CELL			, 37, "A clipped circle wrote the wrong cell.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, CIRCLE_ZERO_RESULT			, 38, "A zero-radius circle reported the wrong written-cell count.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, CIRCLE_ZERO_CELL			, 39, "A zero-radius circle wrote the wrong cell.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, CIRCLE_CALLBACK_RESULT		, 40, "rasterCircle() did not aggregate typed callback results.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, CIRCLE_OUTPUT_RESULT			, 41, "The circle point-output overload reported the wrong point count.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, CIRCLE_OUTPUT_SIZE			, 42, "The circle point-output overload produced the wrong storage size.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, CIRCLE_OUTPUT_CELL			, 43, "The circle point-output overload produced the wrong point set.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, CIRCLE_OUTPUT_APPEND			, 44, "The circle point-output overload discarded existing output.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, CIRCLE_OUTPUT_RANGE			, 45, "The circle point-output overload accepted unrepresentable metrics.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, CIRCLE_OUTPUT_PRESERVED		, 46, "A rejected circle point-output operation changed existing output.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, TRIANGLE_RESULT				, 47, "drawTriangle() reported the wrong written-cell count.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, TRIANGLE_CELL				, 48, "drawTriangle() wrote the wrong cell.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, TRIANGLE_REVERSE_RESULT		, 49, "Reversing triangle winding changed its written-cell count.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, TRIANGLE_REVERSE_CELL		, 50, "Reversing triangle winding changed its rasterized cells.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, TRIANGLE_DEGENERATE_RESULT	, 51, "A degenerate triangle reported written cells.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, TRIANGLE_DEGENERATE_CELL	, 52, "A degenerate triangle changed the target.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, TRIANGLE_CALLBACK_RESULT	, 53, "rasterTriangle() did not aggregate typed callback results.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, TRIANGLE_OUTPUT_RESULT		, 54, "The triangle point-output overload reported the wrong point count.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, TRIANGLE_OUTPUT_SIZE			, 55, "The triangle point-output overload produced the wrong storage size.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, TRIANGLE_OUTPUT_CELL			, 56, "The triangle point-output overload produced the wrong point set.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, TRIANGLE_OUTPUT_APPEND		, 57, "The triangle point-output overload discarded existing output.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, TRIANGLE_OUTPUT_RANGE		, 58, "The triangle point-output overload accepted unrepresentable metrics.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, TRIANGLE_OUTPUT_PRESERVED	, 59, "A rejected triangle point-output operation changed existing output.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LIGHT_RESULT					, 60, "drawPixelLight() reported the wrong affected-cell count.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LIGHT_CELL					, 61, "drawPixelLight() wrote the wrong cell intensity.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LIGHT_CLIPPED_RESULT			, 62, "A clipped pixel light reported the wrong affected-cell count.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LIGHT_CLIPPED_CELL			, 63, "A clipped pixel light wrote the wrong cell intensity.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LIGHT_ZERO_RANGE_RESULT		, 64, "A zero-range pixel light reported the wrong affected-cell count.");
GDEFINE_ENUM_VALUED(BITMAP_TARGET_TEST_RESULT, LIGHT_ZERO_RANGE_CELL		, 65, "A zero-range pixel light changed cells outside its center.");

tplt<tpnm T, size_t N>
sttc bool pointFound(T const (&points)[N], T const& point) {
	for(size_t iPoint = 0; iPoint < N; ++iPoint)
		if(points[iPoint] == point)
			rtrn true;
	rtrn false;
}

tplt<tpnm T>
sttc bool pointFound(::llc::apod<T> const& points, T const& point, ::llc::u2_t offset = 0) {
	for(::llc::u2_t iPoint = offset; iPoint < points.size(); ++iPoint)
		if(points[iPoint] == point)
			rtrn true;
	rtrn false;
}

sttc ::llc::err_t testBitmapAxisLines(ATestError& errors) {
	::llc::u2_t cells[7][7] = {};
	::llc::grid<::llc::u2_t> target = {cells};
	::llc::err_t result = ::llc::drawLineVertical(target, 7U, 2, 5, -2);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_VERTICAL_RESULT, result != 6, "Written cells:%i, expected:6.", result);
	for(::llc::u2_t y = 0; y < target.metrics().y; ++y)
	for(::llc::u2_t x = 0; x < target.metrics().x; ++x) {
		cnst ::llc::u2_t expected = x == 2 && y <= 5 ? 7U : 0U;
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_VERTICAL_CELL, target[y][x] != expected, "Cell:{%u,%u}, value:%u, expected:%u.", x, y, target[y][x], expected);
	}

	memset(cells, 0, sizeof(cells));
	result = ::llc::drawLineHorizontal(target, 9U, 4, 8, 3);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_HORIZONTAL_RESULT, result != 4, "Written cells:%i, expected:4.", result);
	for(::llc::u2_t y = 0; y < target.metrics().y; ++y)
	for(::llc::u2_t x = 0; x < target.metrics().x; ++x) {
		cnst ::llc::u2_t expected = y == 4 && x >= 3 ? 9U : 0U;
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_HORIZONTAL_CELL, target[y][x] != expected, "Cell:{%u,%u}, value:%u, expected:%u.", x, y, target[y][x], expected);
	}

	memset(cells, 0, sizeof(cells));
	result = ::llc::drawLineVertical(target, 5U, -1, 0, 6);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_AXIS_OFFSCREEN_RESULT, result != 0, "Written cells:%i, expected:0.", result);
	for(::llc::u2_t y = 0; y < target.metrics().y; ++y)
	for(::llc::u2_t x = 0; x < target.metrics().x; ++x)
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_AXIS_OFFSCREEN_CELL, target[y][x] != 0, "Cell:{%u,%u}, value:%u, expected:0.", x, y, target[y][x]);
	rtrn 0;
}

sttc ::llc::err_t testBitmapLines(ATestError& errors) {
	::llc::u0_t cells[7][7] = {};
	::llc::grid<::llc::u0_t> target = {cells};
	cnst ::llc::line2s2_t diagonal = {{0, 0}, {6, 6}};
	::llc::err_t result = ::llc::drawLine(target, ::llc::u0_t(3), diagonal);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LINE_RESULT, result != 7, "Written cells:%i, expected:7.", result);
	for(::llc::u2_t y = 0; y < target.metrics().y; ++y)
	for(::llc::u2_t x = 0; x < target.metrics().x; ++x) {
		cnst ::llc::u0_t expected = x == y ? 3 : 0;
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LINE_CELL, target[y][x] != expected, "Cell:{%u,%u}, value:%u, expected:%u.", x, y, (::llc::u2_t)target[y][x], (::llc::u2_t)expected);
	}

	::llc::u0_t reverseCells[7][7] = {};
	::llc::grid<::llc::u0_t> reverse = {reverseCells};
	cnst ::llc::line2s2_t reverseDiagonal = {{6, 6}, {0, 0}};
	result = ::llc::drawLine(reverse, ::llc::u0_t(3), reverseDiagonal);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LINE_REVERSE_RESULT, result != 7, "Written cells:%i, expected:7.", result);
	for(::llc::u2_t y = 0; y < target.metrics().y; ++y)
	for(::llc::u2_t x = 0; x < target.metrics().x; ++x)
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LINE_REVERSE_CELL, reverse[y][x] != target[y][x], "Cell:{%u,%u}, reverse:%u, forward:%u.", x, y, (::llc::u2_t)reverse[y][x], (::llc::u2_t)target[y][x]);

	memset(cells, 0, sizeof(cells));
	cnst ::llc::line2s2_t steep = {{1, 6}, {3, 0}};
	cnst ::llc::n2s2_t steepPoints[] = {{1, 6}, {1, 5}, {2, 4}, {2, 3}, {2, 2}, {3, 1}, {3, 0}};
	result = ::llc::drawLine(target, ::llc::u0_t(4), steep);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LINE_STEEP_RESULT, result != ::llc::err_t(::llc::size(steepPoints)), "Written cells:%i, expected:%u.", result, (::llc::u2_t)::llc::size(steepPoints));
	for(::llc::u2_t y = 0; y < target.metrics().y; ++y)
	for(::llc::u2_t x = 0; x < target.metrics().x; ++x) {
		cnst ::llc::u0_t expected = ::pointFound(steepPoints, ::llc::n2s2_t{(::llc::s2_t)x, (::llc::s2_t)y}) ? 4 : 0;
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LINE_STEEP_CELL, target[y][x] != expected, "Cell:{%u,%u}, value:%u, expected:%u.", x, y, (::llc::u2_t)target[y][x], (::llc::u2_t)expected);
	}

	memset(cells, 0, sizeof(cells));
	cnst ::llc::line2s2_t clipped = {{-2, 2}, {3, 2}};
	result = ::llc::drawLine(target, ::llc::u0_t(5), clipped);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LINE_CLIPPED_RESULT, result != 4, "Written cells:%i, expected:4.", result);
	for(::llc::u2_t y = 0; y < target.metrics().y; ++y)
	for(::llc::u2_t x = 0; x < target.metrics().x; ++x) {
		cnst ::llc::u0_t expected = y == 2 && x <= 3 ? 5 : 0;
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LINE_CLIPPED_CELL, target[y][x] != expected, "Cell:{%u,%u}, value:%u, expected:%u.", x, y, (::llc::u2_t)target[y][x], (::llc::u2_t)expected);
	}

	memset(cells, 0, sizeof(cells));
	cnst ::llc::line2s2_t single = {{4, 5}, {4, 5}};
	result = ::llc::drawLine(target, ::llc::u0_t(6), single);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LINE_SINGLE_RESULT, result != 1, "Written cells:%i, expected:1.", result);
	for(::llc::u2_t y = 0; y < target.metrics().y; ++y)
	for(::llc::u2_t x = 0; x < target.metrics().x; ++x) {
		cnst ::llc::u0_t expected = x == 4 && y == 5 ? 6 : 0;
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LINE_SINGLE_CELL, target[y][x] != expected, "Cell:{%u,%u}, value:%u, expected:%u.", x, y, (::llc::u2_t)target[y][x], (::llc::u2_t)expected);
	}

	::llc::u0_t callbackCells[3][5] = {};
	cnst ::llc::n2u2_t callbackMetrics = {5, 3};
	result = ::llc::rasterLine(callbackMetrics, ::llc::line2s2_t{{-2, 1}, {6, 1}}, [&callbackCells](::llc::n2u2_c& cell) -> ::llc::err_t { callbackCells[cell.y][cell.x] = 1; rtrn 2; });
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LINE_CALLBACK_RESULT, result != 10, "Aggregated result:%i, expected:10.", result);
	for(::llc::u2_t y = 0; y < callbackMetrics.y; ++y)
	for(::llc::u2_t x = 0; x < callbackMetrics.x; ++x) {
		cnst ::llc::u0_t expected = y == 1 ? 1 : 0;
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LINE_CALLBACK_CELL, callbackCells[y][x] != expected, "Cell:{%u,%u}, callbacks:%u, expected:%u.", x, y, (::llc::u2_t)callbackCells[y][x], (::llc::u2_t)expected);
	}
	rtrn 0;
}

sttc ::llc::err_t testBitmapLineOutput(ATestError& errors) {
	cnst ::llc::n2u1_t metrics = {7, 7};
	::llc::apod<::llc::n2s1_t> points = {{99, 99}};
	::llc::err_t result = ::llc::drawLine(metrics, ::llc::line2s2_t{{0, 0}, {6, 6}}, points);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LINE_OUTPUT_RESULT, result != 7, "Point count:%i, expected:7.", result);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LINE_OUTPUT_SIZE, points.size() != 8, "Output size:%u, expected:8.", points.size());
	cnst ::llc::n2s1_t sentinel = {99, 99};
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LINE_OUTPUT_APPEND, points[0] != sentinel, "First point:{%i,%i}, expected:{99,99}.", points[0].x, points[0].y);
	for(::llc::s1_t iPoint = 0; iPoint < 7; ++iPoint) {
		cnst ::llc::n2s1_t expected = {iPoint, iPoint};
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LINE_OUTPUT_CELL, points[::llc::u2_t(iPoint) + 1] != expected, "Point:%i, value:{%i,%i}, expected:{%i,%i}.", iPoint, points[::llc::u2_t(iPoint) + 1].x, points[::llc::u2_t(iPoint) + 1].y, iPoint, iPoint);
	}

	::llc::apod<::llc::n2s1_t> projected = {};
	result = ::llc::drawLine(metrics, ::llc::line3s2_t{{6, 0, 77}, {0, 6, -77}}, projected);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LINE3_OUTPUT_RESULT, result != 7, "Point count:%i, expected:7.", result);
	for(::llc::s1_t iPoint = 0; iPoint < 7; ++iPoint) {
		cnst ::llc::n2s1_t expected = {::llc::s1_t(6 - iPoint), iPoint};
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LINE3_OUTPUT_CELL, projected[iPoint] != expected, "Point:%i, value:{%i,%i}, expected:{%i,%i}.", iPoint, projected[iPoint].x, projected[iPoint].y, 6 - iPoint, iPoint);
	}

	cnst ::llc::n2u1_t oversized = {0x8001U, 1};
	cnst ::llc::u2_t oldSize = points.size();
	result = ::llc::drawLine(oversized, ::llc::line2s2_t{{0, 0}, {1, 0}}, points);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LINE_OUTPUT_RANGE, false == ::llc::failed(result), "Result:%i, expected failure.", result);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LINE_OUTPUT_PRESERVED, points.size() != oldSize, "Output size:%u, expected:%u.", points.size(), oldSize);
	rtrn 0;
}

sttc ::llc::err_t testBitmapRectangles(ATestError& errors) {
	::llc::u0_t cells[6][7] = {};
	::llc::grid<::llc::u0_t> target = {cells};
	::llc::err_t result = ::llc::drawRectangleBorder(target, ::llc::u0_t(3), ::llc::rect2s2_t{{1, 1}, {4, 3}});
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_RECTANGLE_RESULT, result != 10, "Written cells:%i, expected:10.", result);
	for(::llc::u2_t y = 0; y < target.metrics().y; ++y)
	for(::llc::u2_t x = 0; x < target.metrics().x; ++x) {
		cnst bool onBorder = x >= 1 && x <= 4 && y >= 1 && y <= 3 && (x == 1 || x == 4 || y == 1 || y == 3);
		cnst ::llc::u0_t expected = onBorder ? 3 : 0;
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_RECTANGLE_CELL, target[y][x] != expected, "Cell:{%u,%u}, value:%u, expected:%u.", x, y, (::llc::u2_t)target[y][x], (::llc::u2_t)expected);
	}

	memset(cells, 0, sizeof(cells));
	result = ::llc::drawRectangleBorder(target, ::llc::u0_t(4), ::llc::rect2s2_t{{-2, 1}, {4, 3}});
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_RECTANGLE_CLIPPED_RESULT, result != 5, "Written cells:%i, expected:5.", result);
	for(::llc::u2_t y = 0; y < target.metrics().y; ++y)
	for(::llc::u2_t x = 0; x < target.metrics().x; ++x) {
		cnst bool onBorder = (y == 1 || y == 3) && x <= 1 || y == 2 && x == 1;
		cnst ::llc::u0_t expected = onBorder ? 4 : 0;
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_RECTANGLE_CLIPPED_CELL, target[y][x] != expected, "Cell:{%u,%u}, value:%u, expected:%u.", x, y, (::llc::u2_t)target[y][x], (::llc::u2_t)expected);
	}

	memset(cells, 0, sizeof(cells));
	result = ::llc::drawRectangleBorder(target, ::llc::u0_t(5), ::llc::rect2s2_t{{3, 1}, {1, 4}});
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_RECTANGLE_THIN_RESULT, result != 4, "Written cells:%i, expected:4.", result);
	for(::llc::u2_t y = 0; y < target.metrics().y; ++y)
	for(::llc::u2_t x = 0; x < target.metrics().x; ++x) {
		cnst ::llc::u0_t expected = x == 3 && y >= 1 && y <= 4 ? 5 : 0;
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_RECTANGLE_THIN_CELL, target[y][x] != expected, "Cell:{%u,%u}, value:%u, expected:%u.", x, y, (::llc::u2_t)target[y][x], (::llc::u2_t)expected);
	}

	memset(cells, 0, sizeof(cells));
	result = ::llc::drawRectangleBorder(target, ::llc::u0_t(6), ::llc::rect2s2_t{{2, 2}, {0, 3}});
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_RECTANGLE_EMPTY_RESULT, result != 0, "Written cells:%i, expected:0.", result);
	for(::llc::u2_t y = 0; y < target.metrics().y; ++y)
	for(::llc::u2_t x = 0; x < target.metrics().x; ++x)
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_RECTANGLE_EMPTY_CELL, target[y][x] != 0, "Cell:{%u,%u}, value:%u, expected:0.", x, y, (::llc::u2_t)target[y][x]);
	rtrn 0;
}

sttc ::llc::err_t testBitmapCircles(ATestError& errors) {
	::llc::u0_t cells[7][7] = {};
	::llc::grid<::llc::u0_t> target = {cells};
	cnst ::llc::circles2_t circle = {2, {3, 3}};
	::llc::err_t result = ::llc::drawCircle(target, ::llc::u0_t(7), circle);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_CIRCLE_RESULT, result != 13, "Written cells:%i, expected:13.", result);
	for(::llc::s2_t y = 0; y < 7; ++y)
	for(::llc::s2_t x = 0; x < 7; ++x) {
		cnst bool inside = (x - 3) * (x - 3) + (y - 3) * (y - 3) <= 4;
		cnst ::llc::u0_t expected = inside ? 7 : 0;
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_CIRCLE_CELL, target[y][x] != expected, "Cell:{%i,%i}, value:%u, expected:%u.", x, y, (::llc::u2_t)target[y][x], (::llc::u2_t)expected);
	}

	memset(cells, 0, sizeof(cells));
	result = ::llc::drawCircle(target, ::llc::u0_t(8), ::llc::circles2_t{2, {0, 0}});
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_CIRCLE_CLIPPED_RESULT, result != 6, "Written cells:%i, expected:6.", result);
	for(::llc::s2_t y = 0; y < 7; ++y)
	for(::llc::s2_t x = 0; x < 7; ++x) {
		cnst bool inside = x * x + y * y <= 4;
		cnst ::llc::u0_t expected = inside ? 8 : 0;
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_CIRCLE_CLIPPED_CELL, target[y][x] != expected, "Cell:{%i,%i}, value:%u, expected:%u.", x, y, (::llc::u2_t)target[y][x], (::llc::u2_t)expected);
	}

	memset(cells, 0, sizeof(cells));
	result = ::llc::drawCircle(target, ::llc::u0_t(9), ::llc::circles2_t{0, {4, 4}});
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_CIRCLE_ZERO_RESULT, result != 1, "Written cells:%i, expected:1.", result);
	for(::llc::u2_t y = 0; y < target.metrics().y; ++y)
	for(::llc::u2_t x = 0; x < target.metrics().x; ++x) {
		cnst ::llc::u0_t expected = x == 4 && y == 4 ? 9 : 0;
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_CIRCLE_ZERO_CELL, target[y][x] != expected, "Cell:{%u,%u}, value:%u, expected:%u.", x, y, (::llc::u2_t)target[y][x], (::llc::u2_t)expected);
	}

	cnst ::llc::n2u2_t metrics = {7, 7};
	result = ::llc::rasterCircle(metrics, circle, [](::llc::n2u2_c&) -> ::llc::err_t { rtrn 2; });
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_CIRCLE_CALLBACK_RESULT, result != 26, "Aggregated result:%i, expected:26.", result);
	::llc::apod<::llc::n2s2_t> points = {{99, 99}};
	result = ::llc::drawCircle(metrics, circle, points);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_CIRCLE_OUTPUT_RESULT, result != 13, "Point count:%i, expected:13.", result);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_CIRCLE_OUTPUT_SIZE, points.size() != 14, "Output size:%u, expected:14.", points.size());
	cnst ::llc::n2s2_t sentinel = {99, 99};
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_CIRCLE_OUTPUT_APPEND, points[0] != sentinel, "First point:{%i,%i}, expected:{99,99}.", points[0].x, points[0].y);
	for(::llc::s2_t y = 0; y < 7; ++y)
	for(::llc::s2_t x = 0; x < 7; ++x) {
		cnst bool expected = (x - 3) * (x - 3) + (y - 3) * (y - 3) <= 4;
		cnst bool found = ::pointFound(points, ::llc::n2s2_t{x, y}, 1);
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_CIRCLE_OUTPUT_CELL, found != expected, "Cell:{%i,%i}, found:%u, expected:%u.", x, y, (::llc::u2_t)found, (::llc::u2_t)expected);
	}

	cnst ::llc::n2u2_t oversized = {0x80000001U, 1};
	cnst ::llc::u2_t oldSize = points.size();
	result = ::llc::drawCircle(oversized, circle, points);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_CIRCLE_OUTPUT_RANGE, false == ::llc::failed(result), "Result:%i, expected failure.", result);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_CIRCLE_OUTPUT_PRESERVED, points.size() != oldSize, "Output size:%u, expected:%u.", points.size(), oldSize);
	rtrn 0;
}

sttc ::llc::err_t testBitmapTriangles(ATestError& errors) {
	::llc::u0_t cells[7][7] = {};
	::llc::grid<::llc::u0_t> target = {cells};
	cnst ::llc::tri2s2_t triangle = {{1, 1}, {5, 1}, {1, 5}};
	::llc::err_t result = ::llc::drawTriangle(target, ::llc::u0_t(3), triangle);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_TRIANGLE_RESULT, result != 15, "Written cells:%i, expected:15.", result);
	for(::llc::s2_t y = 0; y < 7; ++y)
	for(::llc::s2_t x = 0; x < 7; ++x) {
		cnst bool inside = x >= 1 && y >= 1 && x + y <= 6;
		cnst ::llc::u0_t expected = inside ? 3 : 0;
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_TRIANGLE_CELL, target[y][x] != expected, "Cell:{%i,%i}, value:%u, expected:%u.", x, y, (::llc::u2_t)target[y][x], (::llc::u2_t)expected);
	}

	::llc::u0_t reverseCells[7][7] = {};
	::llc::grid<::llc::u0_t> reverse = {reverseCells};
	cnst ::llc::tri2s2_t reverseTriangle = {{1, 1}, {1, 5}, {5, 1}};
	result = ::llc::drawTriangle(reverse, ::llc::u0_t(3), reverseTriangle);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_TRIANGLE_REVERSE_RESULT, result != 15, "Written cells:%i, expected:15.", result);
	for(::llc::u2_t y = 0; y < target.metrics().y; ++y)
	for(::llc::u2_t x = 0; x < target.metrics().x; ++x)
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_TRIANGLE_REVERSE_CELL, reverse[y][x] != target[y][x], "Cell:{%u,%u}, reverse:%u, forward:%u.", x, y, (::llc::u2_t)reverse[y][x], (::llc::u2_t)target[y][x]);

	memset(cells, 0, sizeof(cells));
	result = ::llc::drawTriangle(target, ::llc::u0_t(4), ::llc::tri2s2_t{{1, 1}, {3, 3}, {5, 5}});
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_TRIANGLE_DEGENERATE_RESULT, result != 0, "Written cells:%i, expected:0.", result);
	for(::llc::u2_t y = 0; y < target.metrics().y; ++y)
	for(::llc::u2_t x = 0; x < target.metrics().x; ++x)
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_TRIANGLE_DEGENERATE_CELL, target[y][x] != 0, "Cell:{%u,%u}, value:%u, expected:0.", x, y, (::llc::u2_t)target[y][x]);

	cnst ::llc::n2u2_t metrics = {7, 7};
	result = ::llc::rasterTriangle(metrics, triangle, [](::llc::n2u2_c&) -> ::llc::err_t { rtrn 2; });
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_TRIANGLE_CALLBACK_RESULT, result != 30, "Aggregated result:%i, expected:30.", result);
	::llc::apod<::llc::n2s1_t> points = {{99, 99}};
	result = ::llc::drawTriangle(metrics, triangle, points);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_TRIANGLE_OUTPUT_RESULT, result != 15, "Point count:%i, expected:15.", result);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_TRIANGLE_OUTPUT_SIZE, points.size() != 16, "Output size:%u, expected:16.", points.size());
	cnst ::llc::n2s1_t sentinel = {99, 99};
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_TRIANGLE_OUTPUT_APPEND, points[0] != sentinel, "First point:{%i,%i}, expected:{99,99}.", points[0].x, points[0].y);
	for(::llc::s1_t y = 0; y < 7; ++y)
	for(::llc::s1_t x = 0; x < 7; ++x) {
		cnst bool expected = x >= 1 && y >= 1 && x + y <= 6;
		cnst bool found = ::pointFound(points, ::llc::n2s1_t{x, y}, 1);
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_TRIANGLE_OUTPUT_CELL, found != expected, "Cell:{%i,%i}, found:%u, expected:%u.", x, y, (::llc::u2_t)found, (::llc::u2_t)expected);
	}

	cnst ::llc::n2u2_t oversized = {0x8001U, 1};
	cnst ::llc::u2_t oldSize = points.size();
	result = ::llc::drawTriangle(oversized, triangle, points);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_TRIANGLE_OUTPUT_RANGE, false == ::llc::failed(result), "Result:%i, expected failure.", result);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_TRIANGLE_OUTPUT_PRESERVED, points.size() != oldSize, "Output size:%u, expected:%u.", points.size(), oldSize);
	rtrn 0;
}

sttc ::llc::err_t testBitmapLight(ATestError& errors) {
	::llc::u2_t cells[5][5] = {};
	::llc::grid<::llc::u2_t> target = {cells};
	::llc::err_t result = ::llc::drawPixelLight(target, ::llc::n2s2_t{2, 2}, 100U, 1.0F, 1.0);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LIGHT_RESULT, result != 9, "Affected cells:%i, expected:9.", result);
	for(::llc::s2_t y = 0; y < 5; ++y)
	for(::llc::s2_t x = 0; x < 5; ++x) {
		cnst ::llc::s2_t distanceX = x > 2 ? x - 2 : 2 - x;
		cnst ::llc::s2_t distanceY = y > 2 ? y - 2 : 2 - y;
		cnst ::llc::u2_t expected = 0 == distanceX && 0 == distanceY ? 100U : distanceX + distanceY == 1 ? 29U : 0U;
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LIGHT_CELL, target[y][x] != expected, "Cell:{%i,%i}, intensity:%u, expected:%u.", x, y, target[y][x], expected);
	}

	memset(cells, 0, sizeof(cells));
	result = ::llc::drawPixelLight(target, ::llc::n2s2_t{0, 0}, 100U, 1.0F, 1.0);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LIGHT_CLIPPED_RESULT, result != 4, "Affected cells:%i, expected:4.", result);
	for(::llc::u2_t y = 0; y < target.metrics().y; ++y)
	for(::llc::u2_t x = 0; x < target.metrics().x; ++x) {
		cnst ::llc::u2_t expected = 0 == x && 0 == y ? 100U : 1 == x + y ? 29U : 0U;
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LIGHT_CLIPPED_CELL, target[y][x] != expected, "Cell:{%u,%u}, intensity:%u, expected:%u.", x, y, target[y][x], expected);
	}

	memset(cells, 0, sizeof(cells));
	result = ::llc::drawPixelLight(target, ::llc::n2s2_t{3, 1}, 77U, 1.0F, 0.0);
	LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LIGHT_ZERO_RANGE_RESULT, result != 1, "Affected cells:%i, expected:1.", result);
	for(::llc::u2_t y = 0; y < target.metrics().y; ++y)
	for(::llc::u2_t x = 0; x < target.metrics().x; ++x) {
		cnst ::llc::u2_t expected = 3 == x && 1 == y ? 77U : 0U;
		LLC_TEST_CHECKF(errors, BITMAP_TARGET_TEST_RESULT_LIGHT_ZERO_RANGE_CELL, target[y][x] != expected, "Cell:{%u,%u}, intensity:%u, expected:%u.", x, y, target[y][x], expected);
	}
	rtrn 0;
}

::llc::err_t testBitmapTarget(ATestError& errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	if_fail_fe(::testBitmapAxisLines(errors));
	if_fail_fe(::testBitmapLines(errors));
	if_fail_fe(::testBitmapLineOutput(errors));
	if_fail_fe(::testBitmapRectangles(errors));
	if_fail_fe(::testBitmapCircles(errors));
	if_fail_fe(::testBitmapTriangles(errors));
	rtrn ::testBitmapLight(errors);
}
