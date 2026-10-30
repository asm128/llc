#include "llc_string.h"
#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(STR_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CONSTRUCTION	, 0, "string construction did not preserve literal text.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, ASSIGNMENT		, 1, "string assignment did not replace the previous text.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, COMPARISON		, 2, "string comparison disagrees with its stored text.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, TERMINATOR		, 3, "string storage is not terminated after construction or assignment.");

::llc::err_t testStr(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	::llc::string text = "alpha";
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_CONSTRUCTION, text.size() != 5 || text != "alpha");
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_TERMINATOR, 0 == text.begin() || 0 != text.begin()[text.size()]);
	text = "beta";
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_ASSIGNMENT, text.size() != 4 || text != "beta");
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_COMPARISON, false == ((text == "beta") || (text == "alpha")));
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_TERMINATOR, 0 == text.begin() || 0 != text.begin()[text.size()]);
	rtrn 0;
}
