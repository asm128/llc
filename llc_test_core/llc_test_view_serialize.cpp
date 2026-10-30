#include "llc_view_serialize.h"
#include "llc_test_core.h"

#include <cstring>

GDEFINE_ENUM_TYPE(VIEW_SERIALIZE_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, READ_RESULT	, 0, "viewPascal() rejected a valid packed view.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, ELEMENTS		, 1, "viewPascal() did not expose the packed elements.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, POD_RESULT	, 2, "loadPOD() rejected a complete value.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, POD_VALUE	, 3, "loadPOD() did not restore or consume the value.");

::llc::err_t testViewSerialize(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	::llc::u0_t bytes[szof(::llc::u2_t) + szof(::llc::u1_t) * 3] = {};
	cnst ::llc::u2_t count = 3;
	cnst ::llc::u1_t values[] = {0x1122, 0x3344, 0x5566};
	::memcpy(bytes, &count, szof(count));
	::memcpy(bytes + szof(count), values, szof(values));
	::llc::view<cnst ::llc::u1_t> restored;
	cnst ::llc::err_t result = ::llc::viewPascal(restored, ::llc::vcu0_t{bytes});
	LLC_TEST_REQUIRE(errors, VIEW_SERIALIZE_TEST_RESULT_READ_RESULT, result != (::llc::err_t)::llc::size(bytes));
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_ELEMENTS, restored.size() != 3 || restored[0] != values[0] || restored[1] != values[1] || restored[2] != values[2]);

	::llc::vcu0_t input = {bytes, szof(::llc::u2_t)};
	::llc::u2_t loaded = 0;
	cnst ::llc::err_t loadResult = ::llc::loadPOD(input, loaded);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_POD_RESULT, loadResult != (::llc::err_t)szof(loaded));
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_POD_VALUE, loaded != count || input.size());
	rtrn 0;
}
