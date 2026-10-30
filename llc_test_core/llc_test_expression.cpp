#include "llc_expression.h"
#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(EXPRESSION_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(EXPRESSION_TEST_RESULT, PARSE_RESULT	, 0, "expressionReaderParse() rejected a valid key.");
GDEFINE_ENUM_VALUED(EXPRESSION_TEST_RESULT, TOKENS		, 1, "The expression reader did not produce tokens and matching views.");
GDEFINE_ENUM_VALUED(EXPRESSION_TEST_RESULT, ROOT_VIEW		, 2, "The root expression view does not preserve the source text.");

::llc::err_t testExpression(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	stxp ::llc::vcst_t expression = "alpha";
	::llc::SExpressionReader reader;
	cnst ::llc::err_t result = ::llc::expressionReaderParse(reader, expression);
	LLC_TEST_REQUIRE(errors, EXPRESSION_TEST_RESULT_PARSE_RESULT, ::llc::failed(result));
	LLC_TEST_REQUIRE(errors, EXPRESSION_TEST_RESULT_TOKENS, 0 == reader.Token.size() || reader.Token.size() != reader.View.size());
	LLC_TEST_CHECK(errors, EXPRESSION_TEST_RESULT_ROOT_VIEW, reader.View[0] != expression);
	rtrn 0;
}
