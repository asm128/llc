#include "llc_grid.h"
#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(GRID_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(GRID_TEST_RESULT, GRID_METRICS	, 1, "grid<> lost its dimensions or element count.");
GDEFINE_ENUM_VALUED(GRID_TEST_RESULT, GRID_ROW		, 2, "grid<> returned the wrong row or cell.");
GDEFINE_ENUM_VALUED(GRID_TEST_RESULT, GRID_FILL	, 3, "grid<>::fill() changed the wrong cells.");
GDEFINE_ENUM_VALUED(GRID_TEST_RESULT, GRID_BOUNDS	, 4, "grid<> accepted an invalid row or cell.");

::llc::err_t testGrid(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	::llc::u2_t cells[2][3] = {{1, 2, 3}, {4, 5, 6}};
	::llc::grid<::llc::u2_t> grid = {cells};
	LLC_TEST_CHECKF(errors, GRID_TEST_RESULT_GRID_METRICS, 3 != grid.metrics().x, "Width:%u, expected:3.", grid.metrics().x);
	LLC_TEST_CHECKF(errors, GRID_TEST_RESULT_GRID_METRICS, 2 != grid.metrics().y, "Height:%u, expected:2.", grid.metrics().y);
	LLC_TEST_CHECKF(errors, GRID_TEST_RESULT_GRID_METRICS, 6 != grid.size(), "Cell count:%u, expected:6.", grid.size());
	LLC_TEST_CHECKF(errors, GRID_TEST_RESULT_GRID_METRICS, 6 * szof(::llc::u2_t) != grid.byte_count(), "Byte count:%u, expected:%u.", grid.byte_count(), 6 * szof(::llc::u2_t));
	LLC_TEST_CHECKF(errors, GRID_TEST_RESULT_GRID_METRICS, &cells[0][0] != grid.begin(), "Grid begin:%p, expected:%p.", grid.begin(), &cells[0][0]);
	LLC_TEST_CHECKF(errors, GRID_TEST_RESULT_GRID_ROW, 3 != grid[0].size(), "First row length:%u, expected:3.", grid[0].size());
	LLC_TEST_CHECKF(errors, GRID_TEST_RESULT_GRID_ROW, 5 != grid[1][1], "Cell (1,1):%u, expected:5.", grid[1][1]);
	cnst ::llc::n2u2_t cell = {2, 1};
	LLC_TEST_CHECKF(errors, GRID_TEST_RESULT_GRID_ROW, 6 != grid[cell], "Cell (2,1):%u, expected:6.", grid[cell]);

	cnst ::llc::grid<::llc::u2_t> & constantGrid = grid;
	LLC_TEST_CHECKF(errors, GRID_TEST_RESULT_GRID_ROW, 4 != constantGrid[1][0], "Const row cell (0,1):%u, expected:4.", constantGrid[1][0]);
	::llc::grid<cnst ::llc::u2_t> constantView = grid;
	LLC_TEST_CHECK(errors, GRID_TEST_RESULT_GRID_METRICS, grid.begin() != constantView.begin());

	if_fail_fe(grid.fill(9, 2, 4));
	cnst ::llc::u2_t partial[] = {1, 2, 9, 9, 5, 6};
	for(::llc::u2_t iCell = 0; iCell < grid.size(); ++iCell) {
		LLC_TEST_CHECKF(errors, GRID_TEST_RESULT_GRID_FILL, partial[iCell] != grid.begin()[iCell] , "Cell:%u, value:%u, expected:%u.", iCell, grid.begin()[iCell], partial[iCell]);
	}
	if_fail_fe(grid.fill(7));
	for(::llc::u2_t iCell = 0; iCell < grid.size(); ++iCell) {
		LLC_TEST_CHECKF(errors, GRID_TEST_RESULT_GRID_FILL, 7 != grid.begin()[iCell] , "Cell:%u, value:%u, expected:7.", iCell, grid.begin()[iCell]);
	}

#ifdef LLC_WINDOWS
	LLC_TEST_CHECKF(errors, GRID_TEST_RESULT_GRID_BOUNDS, false == ::llc::testThrows([&]() { (void)grid[2]; }) , "%s", "Row 2 was accepted in a 2-row grid.");
	cnst ::llc::n2u2_t invalidCell = {3, 1};
	LLC_TEST_CHECKF(errors, GRID_TEST_RESULT_GRID_BOUNDS, false == ::llc::testThrows([&]() { (void)grid[invalidCell]; }) , "%s", "Column 3 was accepted in a 3-column grid.");
#endif
	rtrn 0;
}
