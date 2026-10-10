#include "llc_process.h"
#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(PROCESS_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, BLOCK_RESULT			, 1, "environmentBlockViews() rejected a valid environment block.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, BLOCK_COUNT			, 2, "environmentBlockViews() returned the wrong variable count.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, BLOCK_KEY			, 3, "environmentBlockViews() returned the wrong variable key.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, BLOCK_VALUE			, 4, "environmentBlockViews() returned the wrong variable value.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, BLOCK_ALIAS			, 5, "environmentBlockViews() copied or misplaced a variable view.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, BARE_KEY			, 6, "environmentBlockViews() mishandled a variable without a value.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, EMPTY_BLOCK			, 7, "environmentBlockViews() modified output for an empty block.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, UNTERMINATED_RESULT	, 8, "environmentBlockViews() accepted an unterminated variable.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, UNTERMINATED_OUTPUT	, 9, "environmentBlockViews() modified output after rejecting an unterminated variable.");

::llc::err_t testProcess(ATestError & errors) {
	cnst ::llc::sc_t environment[] = {'A', '=', '1', 0, 'B', '=', 't', 'w', 'o', 0, 0};
	::llc::aobj<::llc::kv<::llc::vcst_t>> variables = {};
	cnst ::llc::err_t result = ::llc::environmentBlockViews(environment, variables);
	LLC_TEST_REQUIREF(errors, PROCESS_TEST_RESULT_BLOCK_RESULT, ::llc::failed(result), "Result:%i.", result);
	LLC_TEST_REQUIREF(errors, PROCESS_TEST_RESULT_BLOCK_COUNT, variables.size() != 2, "Variable count:%u, expected:2.", variables.size());
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_BLOCK_KEY, variables[0].Key != LLC_CXS("A"), "First key:%.*s, expected:A.", LLC_VIEW_ARGS(variables[0].Key));
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_BLOCK_VALUE, variables[0].Val != LLC_CXS("1"), "First value:%.*s, expected:1.", LLC_VIEW_ARGS(variables[0].Val));
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_BLOCK_KEY, variables[1].Key != LLC_CXS("B"), "Second key:%.*s, expected:B.", LLC_VIEW_ARGS(variables[1].Key));
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_BLOCK_VALUE, variables[1].Val != LLC_CXS("two"), "Second value:%.*s, expected:two.", LLC_VIEW_ARGS(variables[1].Val));
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_BLOCK_ALIAS, variables[0].Key.begin() != &environment[0], "First key begin:%p, expected:%p.", variables[0].Key.begin(), &environment[0]);
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_BLOCK_ALIAS, variables[0].Val.begin() != &environment[2], "First value begin:%p, expected:%p.", variables[0].Val.begin(), &environment[2]);
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_BLOCK_ALIAS, variables[1].Key.begin() != &environment[4], "Second key begin:%p, expected:%p.", variables[1].Key.begin(), &environment[4]);
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_BLOCK_ALIAS, variables[1].Val.begin() != &environment[6], "Second value begin:%p, expected:%p.", variables[1].Val.begin(), &environment[6]);

	cnst ::llc::sc_t bareStorage[] = {'F', 'L', 'A', 'G', 0, 0};
	::llc::aobj<::llc::kv<::llc::vcst_t>> bare = {};
	cnst ::llc::err_t bareResult = ::llc::environmentBlockViews(bareStorage, bare);
	LLC_TEST_REQUIREF(errors, PROCESS_TEST_RESULT_BARE_KEY, ::llc::failed(bareResult), "Result:%i.", bareResult);
	LLC_TEST_REQUIREF(errors, PROCESS_TEST_RESULT_BARE_KEY, bare.size() != 1, "Variable count:%u, expected:1.", bare.size());
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_BARE_KEY, bare[0].Key != LLC_CXS("FLAG"), "Key:%.*s, expected:FLAG.", LLC_VIEW_ARGS(bare[0].Key));
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_BARE_KEY, bare[0].Val.size(), "Value count:%u, expected:0.", bare[0].Val.size());

	cnst ::llc::u2_t countBeforeEmpty = variables.size();
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_EMPTY_BLOCK, ::llc::environmentBlockViews({}, variables) < 0, "%s", "Empty block was rejected.");
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_EMPTY_BLOCK, variables.size() != countBeforeEmpty, "Variable count:%u, expected:%u.", variables.size(), countBeforeEmpty);

	cnst ::llc::sc_t unterminated[] = {'X', '=', '9', 0, 'Y', '=', '8'};
	cnst ::llc::u2_t countBeforeInvalid = variables.size();
	::llc::setupLogCallbacks(0, 0);
	cnst ::llc::err_t invalidResult = ::llc::environmentBlockViews(unterminated, variables);
	::llc::setupDefaultLogCallbacks();
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_UNTERMINATED_RESULT, 0 <= invalidResult, "Result:%i.", invalidResult);
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_UNTERMINATED_OUTPUT, variables.size() != countBeforeInvalid, "Variable count:%u, expected:%u.", variables.size(), countBeforeInvalid);
	return 0;
}
