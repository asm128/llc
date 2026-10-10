#include "llc_grid_color.h"
#include "llc_img_color.h"
#include "llc_test_core.h"

#include <type_traits>
#include <utility>

GDEFINE_ENUM_TYPE(GRID_IMAGE_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, GRID_METRICS	, 1, "grid<> lost its dimensions or element count.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, GRID_ROW		, 2, "grid<> returned the wrong row or cell.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, GRID_FILL	, 3, "grid<>::fill() changed the wrong cells.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, GRID_BOUNDS	, 4, "grid<> accepted an invalid row or cell.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, IMAGE_COPY	, 5, "img<> did not own an independent copy of its texels.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, IMAGE_RESIZE	, 6, "img<>::resize() lost dimensions, values or view ownership.");

sttc ::llc::err_t testGrid(ATestError & errors) {
	::llc::u2_t cells[2][3] = {{1, 2, 3}, {4, 5, 6}};
	::llc::grid<::llc::u2_t> grid = {cells};
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_GRID_METRICS, 3 != grid.metrics().x, "Width:%u, expected:3.", grid.metrics().x);
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_GRID_METRICS, 2 != grid.metrics().y, "Height:%u, expected:2.", grid.metrics().y);
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_GRID_METRICS, 6 != grid.size(), "Cell count:%u, expected:6.", grid.size());
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_GRID_METRICS, 6 * szof(::llc::u2_t) != grid.byte_count(), "Byte count:%u, expected:%u.", grid.byte_count(), 6 * szof(::llc::u2_t));
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_GRID_METRICS, &cells[0][0] != grid.begin(), "Grid begin:%p, expected:%p.", grid.begin(), &cells[0][0]);
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_GRID_ROW, 3 != grid[0].size(), "First row length:%u, expected:3.", grid[0].size());
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_GRID_ROW, 5 != grid[1][1], "Cell (1,1):%u, expected:5.", grid[1][1]);
	cnst ::llc::n2u2_t cell = {2, 1};
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_GRID_ROW, 6 != grid[cell], "Cell (2,1):%u, expected:6.", grid[cell]);

	cnst ::llc::grid<::llc::u2_t> & constantGrid = grid;
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_GRID_ROW, 4 != constantGrid[1][0], "Const row cell (0,1):%u, expected:4.", constantGrid[1][0]);
	::llc::grid<cnst ::llc::u2_t> constantView = grid;
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_GRID_METRICS, grid.begin() != constantView.begin(), "Const grid view changed storage.");

	if_fail_fe(grid.fill(9, 2, 4));
	cnst ::llc::u2_t partial[] = {1, 2, 9, 9, 5, 6};
	for(::llc::u2_t iCell = 0; iCell < grid.size(); ++iCell) {
		LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_GRID_FILL, partial[iCell] != grid.begin()[iCell]
			, "Cell:%u, value:%u, expected:%u.", iCell, grid.begin()[iCell], partial[iCell]);
	}
	if_fail_fe(grid.fill(7));
	for(::llc::u2_t iCell = 0; iCell < grid.size(); ++iCell) {
		LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_GRID_FILL, 7 != grid.begin()[iCell]
			, "Cell:%u, value:%u, expected:7.", iCell, grid.begin()[iCell]);
	}

#ifdef LLC_WINDOWS
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_GRID_BOUNDS, false == testThrows([&]() { (void)grid[2]; })
		, "%s", "Row 2 was accepted in a 2-row grid.");
	cnst ::llc::n2u2_t invalidCell = {3, 1};
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_GRID_BOUNDS, false == testThrows([&]() { (void)grid[invalidCell]; })
		, "%s", "Column 3 was accepted in a 3-column grid.");
#endif
	rtrn 0;
}

sttc ::llc::err_t testImage(ATestError & errors) {
	::llc::u2_t cells[2][2] = {{1, 2}, {3, 4}};
	::llc::grid<::llc::u2_t> source = {cells};
	::llc::img<::llc::u2_t> image = {source};
	LLC_TEST_REQUIRE(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, 4 != image.size(), "Copied image size:%u, expected:4.", image.size());
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, source.begin() == image.begin(), "%s", "Image aliases source storage.");
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, image.Texels.begin() != image.View.begin(), "%s", "Image view does not point into owned texels.");
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, 4 != image[1][1], "Copied cell (1,1):%u, expected:4.", image[1][1]);
	cnst ::llc::img<::llc::u2_t> & constantImage = image;
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, 3 != constantImage[1][0], "Const image cell (0,1):%u, expected:3.", constantImage[1][0]);
	image[0][0] = 8;
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, 1 != source[0][0], "Source cell changed to:%u.", source[0][0]);

	::llc::img<::llc::u2_t> copied = image;
	LLC_TEST_REQUIRE(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, 4 != copied.size(), "Copy size:%u, expected:4.", copied.size());
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, copied.begin() == image.begin(), "%s", "Image copy aliases original storage.");
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, copied.View.begin() != copied.Texels.begin(), "%s", "Copied view does not point into copied texels.");
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, 2 != copied.metrics().y, "Copied height:%u, expected:2.", copied.metrics().y);
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, 8 != copied[0][0], "Copied first cell:%u, expected:8.", copied[0][0]);

	cnst ::llc::n2u2_t newSize = {3, 2};
	cnst ::llc::err_t resizeResult = image.resize(newSize, 12);
	LLC_TEST_REQUIRE(errors, GRID_IMAGE_TEST_RESULT_IMAGE_RESIZE, ::llc::failed(resizeResult), "Resize failed:%i.", resizeResult);
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_RESIZE, 6 != resizeResult, "Resize result:%i, expected:6.", resizeResult);
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_RESIZE, 3 != image.metrics().x, "Resized width:%u, expected:3.", image.metrics().x);
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_RESIZE, 2 != image.metrics().y, "Resized height:%u, expected:2.", image.metrics().y);
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_RESIZE, image.Texels.begin() != image.View.begin(), "%s", "Resized view does not point into owned texels.");
	for(::llc::u2_t iCell = 0; iCell < image.size(); ++iCell) {
		LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_RESIZE, 12 != image.begin()[iCell]
			, "Cell:%u, value:%u, expected:12.", iCell, image.begin()[iCell]);
	}
	cnst ::llc::n2u2_t smallerSize = {2, 1};
	cnst ::llc::err_t smallerResult = image.resize(smallerSize);
	LLC_TEST_REQUIRE(errors, GRID_IMAGE_TEST_RESULT_IMAGE_RESIZE, ::llc::failed(smallerResult), "Resize without fill failed:%i.", smallerResult);
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_RESIZE, 0 != smallerResult, "Resize without fill returned:%i, expected:0.", smallerResult);
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_RESIZE, 2 != image.size(), "Reduced image size:%u, expected:2.", image.size());
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_RESIZE, 2 != image.metrics().x, "Reduced image width:%u, expected:2.", image.metrics().x);
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_RESIZE, 1 != image.metrics().y, "Reduced image height:%u, expected:1.", image.metrics().y);
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_RESIZE, image.Texels.begin() != image.View.begin(), "%s", "Reduced view does not point into owned texels.");

	::llc::img<::llc::u2_t> assigned = {};
	assigned = source;
	LLC_TEST_REQUIRE(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, 4 != assigned.size(), "Assigned image size:%u, expected:4.", assigned.size());
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, source.begin() == assigned.begin(), "%s", "Assigned image aliases source storage.");
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, assigned.Texels.begin() != assigned.View.begin(), "%s", "Assigned view does not point into owned texels.");
	assigned = image;
	LLC_TEST_REQUIRE(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, 2 != assigned.size(), "Copy-assigned size:%u, expected:2.", assigned.size());
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, assigned.begin() == image.begin(), "%s", "Copy assignment aliases original storage.");
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, assigned.Texels.begin() != assigned.View.begin(), "%s", "Copy-assigned view does not point into owned texels.");
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, 1 != assigned.metrics().y, "Copy-assigned height:%u, expected:1.", assigned.metrics().y);
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, 12 != assigned[0][0], "Copy-assigned first cell:%u, expected:12.", assigned[0][0]);
	rtrn 0;
}

::llc::err_t testGridImage(ATestError & errors) {
	static_assert(::std::is_same_v<::llc::gu8, ::llc::grid<::llc::u0_t>>);
	static_assert(::std::is_same_v<::llc::gu16, ::llc::grid<::llc::u1_t>>);
	static_assert(::std::is_same_v<::llc::g8bgra, ::llc::grid<::llc::bgra>>);
	static_assert(::std::is_same_v<::llc::gc8bgra, ::llc::grid<cnst ::llc::bgra>>);
	static_assert(::std::is_same_v<::llc::imgu8, ::llc::img<::llc::u0_t>>);
	static_assert(::std::is_same_v<::llc::imgu16, ::llc::img<::llc::u1_t>>);
	static_assert(::std::is_same_v<::llc::img8bgra, ::llc::img<::llc::bgra>>);
	static_assert(::std::is_same_v<decltype(::std::declval<cnst ::llc::g8bgra &>()[0]), ::llc::view<cnst ::llc::bgra>>);
	static_assert(::std::is_same_v<decltype(::std::declval<cnst ::llc::img8bgra &>()[0]), ::llc::view<cnst ::llc::bgra>>);
	if_fail_fe(::testGrid(errors));
	rtrn ::testImage(errors);
}
