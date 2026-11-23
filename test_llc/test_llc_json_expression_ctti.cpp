#include "test_llc_suites.h"
#include "llc_json_expression.h"
#include "llc_json_expression_ctti.h"

enum JSON_EXPRESSION_CTTI_TEST_RESULT : ::llc::u0_t
	{ JSON_EXPRESSION_CTTI_TEST_RESULT_PARSE	= 0
	, JSON_EXPRESSION_CTTI_TEST_RESULT_RESOLVE	= 1
	, JSON_EXPRESSION_CTTI_TEST_RESULT_VALUE	= 2
	};

stxp auto jsonExpressionCTTI = ::llc::jsonExpressionParseCTTI("application.name", "{\"application\":{\"name\":\"gpk\"}}");
static_assert(jsonExpressionCTTI.Success());
static_assert(3 == jsonExpressionCTTI.Expression.Count);
static_assert(9 == jsonExpressionCTTI.JSON.Count);

tplt<size_t NExpression, size_t NJSON>
sttc ::llc::err_t testJSONExpressionCTTI
	( ::llc::ATestError & errors
	, ::llc::sc_c (&expression)[NExpression]
	, ::llc::sc_c (&json)[NJSON]
	, ::llc::vcst_t expected
	) {
	cnst auto reader = ::llc::jsonExpressionParseCTTI(expression, json);
	LLC_TEST_REQUIREF(errors, JSON_EXPRESSION_CTTI_TEST_RESULT_PARSE, false == reader.Success()
		, "Expression error:%u at %u; JSON error:%u at %u."
		, reader.Expression.Error, reader.Expression.ErrorOffset, reader.JSON.Error, reader.JSON.ErrorOffset
		);
	::llc::vcst_t output = "unchanged";
	cnst ::llc::err_t result = ::llc::jsonExpressionResolve(reader.Expression.Tokens(), expression, reader.JSON.Tokens(), json, 0, output);
	LLC_TEST_REQUIREF(errors, JSON_EXPRESSION_CTTI_TEST_RESULT_RESOLVE, ::llc::failed(result), "Result:%i, expression:'%s', JSON:'%s'.", result, expression, json);
	LLC_TEST_CHECKF(errors, JSON_EXPRESSION_CTTI_TEST_RESULT_VALUE, output != expected, "Output:'%.*s', expected:'%.*s', expression:'%s'.", LLC_VIEW_ARGS(output), LLC_VIEW_ARGS(expected), expression);
	rtrn 0;
}

::llc::err_t test_json_expression_ctti(STestRunnerState &, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	if_fail_fe(::testJSONExpressionCTTI(errors, "application.name", "{\"application\":{\"name\":\"gpk\",\"enabled\":true,\"value\":12.5}}", "gpk"));
	if_fail_fe(::testJSONExpressionCTTI(errors, "application.enabled", "{\"application\":{\"name\":\"gpk\",\"enabled\":true}}", "true"));
	if_fail_fe(::testJSONExpressionCTTI(errors, "application.value", "{\"application\":{\"value\":12.5}}", "12.5"));
	if_fail_fe(::testJSONExpressionCTTI(errors, "application.enabled ? application.name : 'no'", "{\"application\":{\"name\":\"gpk\",\"enabled\":true}}", "gpk"));
	if_fail_fe(::testJSONExpressionCTTI(errors, "items['1']", "{\"items\":[\"zero\",\"one\",\"two\"]}", "one"));
	rtrn 0;
}
