#include "llc_parse.h"
#include "test_llc_suites.h"

GDEFINE_ENUM_TYPE(PARSE_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(PARSE_TEST_RESULT, HEX_COUNT		, 0, "The hexadecimal parser consumed an unexpected character count.");
GDEFINE_ENUM_VALUED(PARSE_TEST_RESULT, HEX_VALUE		, 1, "The hexadecimal parser returned an unexpected value.");
GDEFINE_ENUM_VALUED(PARSE_TEST_RESULT, BASE_LIMIT_COUNT	, 2, "The parser searched beyond the requested base.");
GDEFINE_ENUM_VALUED(PARSE_TEST_RESULT, BASE_LIMIT_VALUE	, 3, "The base-limited parser returned an unexpected value.");
GDEFINE_ENUM_VALUED(PARSE_TEST_RESULT, INVALID_COUNT		, 4, "The parser consumed an invalid leading symbol.");
GDEFINE_ENUM_VALUED(PARSE_TEST_RESULT, INVALID_VALUE		, 5, "The parser changed the zero result for invalid input.");

::llc::err_t test_parse(STestRunnerState &, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	::llc::u3_t value = 0;
	::llc::err_t result = ::llc::parseArbitraryBaseInteger(16, "0123456789abcdef", "1aFz", &value);
	LLC_TEST_CHECK(errors, PARSE_TEST_RESULT_HEX_COUNT, result != 3);
	LLC_TEST_CHECK(errors, PARSE_TEST_RESULT_HEX_VALUE, value != 0x1AF);

	value = 0;
	result = ::llc::parseArbitraryBaseInteger(10, "0123456789abcdef", "9a", &value);
	LLC_TEST_CHECK(errors, PARSE_TEST_RESULT_BASE_LIMIT_COUNT, result != 1);
	LLC_TEST_CHECK(errors, PARSE_TEST_RESULT_BASE_LIMIT_VALUE, value != 9);

	value = 0;
	result = ::llc::parseArbitraryBaseInteger(16, "0123456789abcdef", "x1", &value);
	LLC_TEST_CHECK(errors, PARSE_TEST_RESULT_INVALID_COUNT, result != 0);
	LLC_TEST_CHECK(errors, PARSE_TEST_RESULT_INVALID_VALUE, value != 0);
	rtrn 0;
}
