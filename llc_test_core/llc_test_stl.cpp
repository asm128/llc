#include "llc_stl.h"
#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(STL_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(STL_TEST_RESULT, LOAD_RESULT	, 0, "stlFileLoad() rejected a valid empty binary STL.");
GDEFINE_ENUM_VALUED(STL_TEST_RESULT, HEADER		, 1, "stlFileLoad() did not expose the 80-byte header.");
GDEFINE_ENUM_VALUED(STL_TEST_RESULT, TRIANGLES	, 2, "stlFileLoad() produced triangles for an empty binary STL.");

::llc::err_t testSTL(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	::llc::u0_t bytes[80 + szof(::llc::u2_t)] = {};
	for(::llc::u2_t iByte = 0; iByte < 80; ++iByte)
		bytes[iByte] = (::llc::u0_t)iByte;
	::llc::vcu0_t header;
	::llc::view<cnst ::llc::SSTLTriangle> triangles;
	cnst ::llc::err_t result = ::llc::stlFileLoad(::llc::vcu0_t{bytes}, header, triangles);
	LLC_TEST_REQUIRE(errors, STL_TEST_RESULT_LOAD_RESULT, result != (::llc::err_t)szof(::llc::u2_t));
	LLC_TEST_CHECK(errors, STL_TEST_RESULT_HEADER, 80 != header.size() || header.begin() != bytes);
	LLC_TEST_CHECK(errors, STL_TEST_RESULT_TRIANGLES, 0 != triangles.size());
	rtrn 0;
}
