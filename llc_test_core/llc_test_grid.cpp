#include "llc_test_core_suites.h"
#include "llc_grid.h"

GDEFINE_ENUM_TYPE(TEST_GRID_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_GRID_RESULT, METRICS , 0, "A grid derives width, height, area, and byte count from its storage.");
GDEFINE_ENUM_VALUED(TEST_GRID_RESULT, ACCESS  , 1, "Grid row and cell access address the same underlying storage.");
GDEFINE_ENUM_VALUED(TEST_GRID_RESULT, FILL    , 2, "Grid fill applies to the selected flat index interval.");
GDEFINE_ENUM_VALUED(TEST_GRID_RESULT, CONST   , 3, "A mutable grid converts to a const grid without copying storage.");

::llc::err_t testGrid(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::u2_t storage[2][3] = {{1, 2, 3}, {4, 5, 6}};
	::llc::grid<::llc::u2_t> cells = storage;
	LLC_TEST_CHECK(results, TEST_GRID_RESULT_METRICS, cells.metrics().x != 3 || cells.metrics().y != 2 || cells.area() != 6 || cells.size() != 6 || cells.byte_count() != sizeof(storage));

	::llc::n2u2_t cell = {1, 1};
	cells[cell] = 8;
	LLC_TEST_CHECK(results, TEST_GRID_RESULT_ACCESS, cells[1].size() != 3 || cells[1][1] != 8 || storage[1][1] != 8);

	LLC_TEST_REQUIRE(results, TEST_GRID_RESULT_FILL, 0 != cells.fill(9, 2, 5));
	LLC_TEST_CHECK(results, TEST_GRID_RESULT_FILL, storage[0][0] != 1 || storage[0][1] != 2 || storage[0][2] != 9 || storage[1][0] != 9 || storage[1][1] != 9 || storage[1][2] != 6);

	cnst ::llc::grid<cnst ::llc::u2_t> readOnly = cells;
	LLC_TEST_CHECK(results, TEST_GRID_RESULT_CONST, readOnly.begin() != cells.begin() || readOnly[1][2] != 6);
	rtrn 0;
}
