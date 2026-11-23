#include "llc_enum.h"
#include "test_llc_suites.h"

GDEFINE_ENUM_TYPE(ENUM_TEST_COMMAND, ::llc::u0_t);
GDEFINE_ENUM_VALUE(ENUM_TEST_COMMAND, Join, 0);
GDEFINE_ENUM_VALUE(ENUM_TEST_COMMAND, Quit, 1);

GDEFINE_ENUM_TYPE(ENUM_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(ENUM_TEST_RESULT, SLICE_RESULT		, 0, "The command prefix could not be removed.");
GDEFINE_ENUM_VALUED(ENUM_TEST_RESULT, SPLIT_RESULT		, 1, "The command name and arguments could not be split.");
GDEFINE_ENUM_VALUED(ENUM_TEST_RESULT, COMMAND_NAME		, 2, "The parsed command name is not 'join'.");
GDEFINE_ENUM_VALUED(ENUM_TEST_RESULT, COMMAND_ARGS		, 3, "The parsed command arguments are not 'abc'.");
GDEFINE_ENUM_VALUED(ENUM_TEST_RESULT, REGISTERED_LABEL	, 4, "The registered command label is not 'Join'.");
GDEFINE_ENUM_VALUED(ENUM_TEST_RESULT, LOWERCASE_LOOKUP	, 5, "The lowercase command did not resolve to Join.");

::llc::err_t test_enum(STestRunnerState &, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	::llc::vcsc_t inputLine = ::llc::vcst_t{".join abc"};
	cnst ::llc::err_t sliceResult = inputLine.slice(inputLine, 1);
	LLC_TEST_REQUIRE(errors, ENUM_TEST_RESULT_SLICE_RESULT, ::llc::failed(sliceResult));

	::llc::vcsc_t commandName, commandArgs;
	cnst ::llc::err_t splitResult = ::llc::split(::llc::vcst_t{" "}, inputLine, commandName, commandArgs);
	LLC_TEST_REQUIRE(errors, ENUM_TEST_RESULT_SPLIT_RESULT, ::llc::failed(splitResult));
	LLC_TEST_CHECK(errors, ENUM_TEST_RESULT_COMMAND_NAME, commandName != ::llc::vcst_t{"join"});
	LLC_TEST_CHECK(errors, ENUM_TEST_RESULT_COMMAND_ARGS, commandArgs != ::llc::vcst_t{"abc"});
	LLC_TEST_CHECK(errors, ENUM_TEST_RESULT_REGISTERED_LABEL, ::llc::get_value_namev(ENUM_TEST_COMMAND_Join) != ::llc::vcst_t{"Join"});

	cnst ENUM_TEST_COMMAND command = ::llc::get_value_camelcased<ENUM_TEST_COMMAND>(commandName);
	LLC_TEST_CHECK(errors, ENUM_TEST_RESULT_LOWERCASE_LOOKUP, command != ENUM_TEST_COMMAND_Join);
	rtrn 0;
}
