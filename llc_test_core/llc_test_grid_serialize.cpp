#include "llc_grid_serialize.h"
#include "llc_test_core.h"

#include <cstring>

GDEFINE_ENUM_TYPE(GRID_SERIALIZE_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULT, READ_RESULT	, 0, "gridPascal() rejected a valid packed grid.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULT, METRICS		, 1, "gridPascal() did not restore the grid metrics.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULT, CELLS		, 2, "gridPascal() did not expose the packed cells.");

::llc::err_t testGridSerialize(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	::llc::u0_t bytes[szof(::llc::n2u2_t) + 4] = {};
	cnst ::llc::n2u2_t metrics = {2, 2};
	::memcpy(bytes, &metrics, szof(metrics));
	bytes[szof(metrics) + 0] = 1;
	bytes[szof(metrics) + 1] = 2;
	bytes[szof(metrics) + 2] = 3;
	bytes[szof(metrics) + 3] = 4;

	::llc::grid<cnst ::llc::u0_t> restored;
	cnst ::llc::err_t result = ::llc::gridPascal(restored, ::llc::vcu0_t{bytes});
	LLC_TEST_REQUIRE(errors, GRID_SERIALIZE_TEST_RESULT_READ_RESULT, result != (::llc::err_t)::llc::size(bytes));
	LLC_TEST_CHECK(errors, GRID_SERIALIZE_TEST_RESULT_METRICS, restored.metrics() != metrics);
	LLC_TEST_CHECK(errors, GRID_SERIALIZE_TEST_RESULT_CELLS, restored.size() != 4 || restored.begin()[0] != 1 || restored.begin()[1] != 2 || restored.begin()[2] != 3 || restored.begin()[3] != 4);
	rtrn 0;
}
