#include "llc_test_core_suites.h"
#include "llc_string.h"

GDEFINE_ENUM_TYPE(TEST_STRING_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_STRING_RESULT, CONSTRUCT  , 0, "A string copies literal contents without counting the literal terminator.");
GDEFINE_ENUM_VALUED(TEST_STRING_RESULT, APPEND     , 1, "String append operations preserve text and the trailing terminator.");
GDEFINE_ENUM_VALUED(TEST_STRING_RESULT, COUNTED    , 2, "A counted string preserves embedded null bytes.");
GDEFINE_ENUM_VALUED(TEST_STRING_RESULT, COPY       , 3, "A copied string owns independent mutable storage.");

::llc::err_t testString(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::string text = "alpha";
	LLC_TEST_CHECK(results, TEST_STRING_RESULT_CONSTRUCT, text.size() != 5 || text != "alpha" || text.begin()[text.size()] != 0);
	LLC_TEST_REQUIRE(results, TEST_STRING_RESULT_APPEND, 1 != text.append_string('-') || 4 != text.append_string("beta"));
	LLC_TEST_CHECK(results, TEST_STRING_RESULT_APPEND, text.size() != 10 || text != "alpha-beta" || text.begin()[text.size()] != 0);

	cnst char bytes[] = {'A', 0, 'B'};
	::llc::string counted = ::llc::vcst_t{bytes, ::llc::size(bytes)};
	LLC_TEST_CHECK(results, TEST_STRING_RESULT_COUNTED, counted.size() != 3 || counted[0] != 'A' || counted[1] != 0 || counted[2] != 'B' || counted.begin()[counted.size()] != 0);

	::llc::string copied = text;
	copied[0] = 'A';
	LLC_TEST_CHECK(results, TEST_STRING_RESULT_COPY, copied != "Alpha-beta" || text != "alpha-beta");
	rtrn 0;
}
