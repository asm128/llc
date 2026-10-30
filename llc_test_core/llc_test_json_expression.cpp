#include "llc_json_expression.h"
#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(JSON_EXPRESSION_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, TRUE_RESULT	, 0, "SEvaluationStepResult::SetBoolCarry() rejected true.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, TRUE_STATE	, 1, "SEvaluationStepResult::SetBoolCarry() did not publish true state.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, FALSE_RESULT	, 2, "SEvaluationStepResult::SetBoolCarry() rejected false.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, FALSE_STATE	, 3, "SEvaluationStepResult::SetBoolCarry() did not publish false state.");

::llc::err_t testJSONExpression(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	::llc::SEvaluationStepResult step = {};
	::llc::vcst_t output;
	cnst ::llc::err_t trueResult = step.SetBoolCarry(true, output);
	LLC_TEST_CHECK(errors, JSON_EXPRESSION_TEST_RESULT_TRUE_RESULT, ::llc::failed(trueResult));
	LLC_TEST_CHECK(errors, JSON_EXPRESSION_TEST_RESULT_TRUE_STATE, false == step.LastValueCarry || 1 != step.LastValue || output != "true");
	cnst ::llc::err_t falseResult = step.SetBoolCarry(false, output);
	LLC_TEST_CHECK(errors, JSON_EXPRESSION_TEST_RESULT_FALSE_RESULT, ::llc::failed(falseResult));
	LLC_TEST_CHECK(errors, JSON_EXPRESSION_TEST_RESULT_FALSE_STATE, false == step.LastValueCarry || 0 != step.LastValue || output != "false");
	rtrn 0;
}
