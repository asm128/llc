#include "llc_args.h"
#include "llc_log.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(ARGS_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(ARGS_TEST_RESULT, PARSE_RESULT		, 0, "argsParse() returned an unexpected argument count.");
GDEFINE_ENUM_VALUED(ARGS_TEST_RESULT, PROGRAM_NAME		, 1, "argsParse() did not preserve the program name.");
GDEFINE_ENUM_VALUED(ARGS_TEST_RESULT, ENVIRONMENT		, 2, "argsParse() did not preserve the environment views.");
GDEFINE_ENUM_VALUED(ARGS_TEST_RESULT, OPTIONS			, 3, "argsParse() produced unexpected option records.");
GDEFINE_ENUM_VALUED(ARGS_TEST_RESULT, POSITIONALS		, 4, "argsParse() produced unexpected positional arguments.");
GDEFINE_ENUM_VALUED(ARGS_TEST_RESULT, OPTION_INDEX		, 5, "argsOptionIndex() did not find repeated options from an offset.");
GDEFINE_ENUM_VALUED(ARGS_TEST_RESULT, OPTION_VALUES		, 6, "argsOptionValues() did not append every matching value in order.");
GDEFINE_ENUM_VALUED(ARGS_TEST_RESULT, OPTION_VALUE		, 7, "argsOptionValue() did not return the sole matching option value.");
GDEFINE_ENUM_VALUED(ARGS_TEST_RESULT, AMBIGUOUS_VALUE	, 8, "argsOptionValue() accepted a key with multiple values.");
GDEFINE_ENUM_VALUED(ARGS_TEST_RESULT, MISSING_VALUE		, 9, "The option accessors did not report a missing key consistently.");
GDEFINE_ENUM_VALUED(ARGS_TEST_RESULT, REUSE				, 10, "argsParse() retained state from an earlier parse.");
GDEFINE_ENUM_VALUED(ARGS_TEST_RESULT, EMPTY_VALUE		, 11, "argsParse() did not preserve empty inline and valueless options.");

sttc ::llc::err_t testArgsParsedValues(ATestError & errors, cnst ::llc::SCommandLineArgs & args) {
	cnst ::llc::keyval<::llc::vcst_t> expectedOptions[] =
		{ {LLC_CXS("include"), LLC_CXS("a")}
		, {LLC_CXS("include"), LLC_CXS("b")}
		, {LLC_CXS("flag")   , {}}
		, {LLC_CXS("include"), LLC_CXS("c")}
		, {LLC_CXS("output") , LLC_CXS("result")}
		};
	LLC_TEST_REQUIRE(errors, ARGS_TEST_RESULT_OPTIONS, args.Options.size() != ::llc::size(expectedOptions)
		, "Option count:%u, expected:%u."
		, args.Options.size(), (::llc::u2_t)::llc::size(expectedOptions)
		);
	for(::llc::u2_t iOption = 0; iOption < args.Options.size(); ++iOption)
		LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_OPTIONS, false == (args.Options[iOption] == expectedOptions[iOption])
			, "Option:%u is '%.*s'='%.*s', expected '%.*s'='%.*s'."
			, iOption
			, (int)args.Options[iOption].Key.size(), args.Options[iOption].Key.begin(), (int)args.Options[iOption].Val.size(), args.Options[iOption].Val.begin()
			, (int)expectedOptions[iOption].Key.size(), expectedOptions[iOption].Key.begin(), (int)expectedOptions[iOption].Val.size(), expectedOptions[iOption].Val.begin()
			);

	cnst ::llc::vcst_t expectedPositionals[] = {LLC_CXS("first"), LLC_CXS("last"), LLC_CXS("--literal"), LLC_CXS("")};
	LLC_TEST_REQUIRE(errors, ARGS_TEST_RESULT_POSITIONALS, args.Positionals.size() != ::llc::size(expectedPositionals)
		, "Positional count:%u, expected:%u."
		, args.Positionals.size(), (::llc::u2_t)::llc::size(expectedPositionals)
		);
	for(::llc::u2_t iPositional = 0; iPositional < args.Positionals.size(); ++iPositional)
		LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_POSITIONALS, args.Positionals[iPositional] != expectedPositionals[iPositional]
			, "Positional:%u is '%.*s', expected '%.*s'."
			, iPositional, (int)args.Positionals[iPositional].size(), args.Positionals[iPositional].begin(), (int)expectedPositionals[iPositional].size(), expectedPositionals[iPositional].begin()
			);
	rtrn 0;
}

sttc ::llc::err_t testArgsAccessors(ATestError & errors, cnst ::llc::SCommandLineArgs & args) {
	cnst ::llc::err_t firstInclude	= ::llc::argsOptionIndex(args, LLC_CXS("include"));
	cnst ::llc::err_t secondInclude	= ::llc::argsOptionIndex(args, LLC_CXS("include"), 1);
	cnst ::llc::err_t thirdInclude	= ::llc::argsOptionIndex(args, LLC_CXS("include"), 2);
	cnst ::llc::err_t afterInclude	= ::llc::argsOptionIndex(args, LLC_CXS("include"), 4);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_OPTION_INDEX, firstInclude != 0 || secondInclude != 1 || thirdInclude != 3 || afterInclude != -1
		, "Repeated indices:%i,%i,%i,%i; expected:0,1,3,-1."
		, firstInclude, secondInclude, thirdInclude, afterInclude
		);

	::llc::aobj<::llc::vcst_t> values = {LLC_CXS("seed")};
	cnst ::llc::err_t valueCount = ::llc::argsOptionValues(args, LLC_CXS("include"), values);
	if_fail_fe(valueCount);
	LLC_TEST_REQUIRE(errors, ARGS_TEST_RESULT_OPTION_VALUES, valueCount != 3 || values.size() != 4
		, "Collected:%i, output size:%u; expected:3/4."
		, valueCount, values.size()
		);
	cnst ::llc::vcst_t expectedValues[] = {LLC_CXS("seed"), LLC_CXS("a"), LLC_CXS("b"), LLC_CXS("c")};
	for(::llc::u2_t iValue = 0; iValue < values.size(); ++iValue)
		LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_OPTION_VALUES, values[iValue] != expectedValues[iValue]
			, "Collected value:%u is '%.*s', expected '%.*s'."
			, iValue, (int)values[iValue].size(), values[iValue].begin(), (int)expectedValues[iValue].size(), expectedValues[iValue].begin()
			);

	::llc::vcst_t singleValue = {};
	cnst ::llc::err_t singleIndex = ::llc::argsOptionValue(args, LLC_CXS("output"), singleValue);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_OPTION_VALUE, singleIndex != 4 || singleValue != LLC_CXS("result")
		, "Single index:%i, value:'%.*s'; expected:4/'result'."
		, singleIndex, (int)singleValue.size(), singleValue.begin()
		);

	::llc::vcst_t ambiguousValue = LLC_CXS("stale");
	::llc::vcst_t missingValue = LLC_CXS("stale");
	::llc::setupLogCallbacks(0, 0);
	cnst ::llc::err_t ambiguous = ::llc::argsOptionValue(args, LLC_CXS("include"), ambiguousValue);
	cnst ::llc::err_t missing = ::llc::argsOptionValue(args, LLC_CXS("missing"), missingValue);
	::llc::setupDefaultLogCallbacks();
	cnst ::llc::err_t missingCount = ::llc::argsOptionValues(args, LLC_CXS("missing"), values);
	if_fail_fe(missingCount);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_AMBIGUOUS_VALUE, ambiguous != -2 || ambiguousValue.size()
		, "Ambiguous result:%i, output size:%u; expected:-2/0."
		, ambiguous, ambiguousValue.size()
		);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_MISSING_VALUE, missing != -1 || missingValue.size() || missingCount || values.size() != 4
		, "Missing result:%i, output size:%u, collected:%i, collected size:%u; expected:-1/0/0/4."
		, missing, missingValue.size(), missingCount, values.size()
		);
	rtrn 0;
}

::llc::err_t testArgs(ATestError & errors) {
	::llc::vcst_t argv[] =
		{ LLC_CXS("app")
		, LLC_CXS("first")
		, LLC_CXS("--include"), LLC_CXS("a"), LLC_CXS("b")
		, LLC_CXS("--flag")
		, LLC_CXS("--include"), LLC_CXS("c")
		, LLC_CXS("--output=result"), LLC_CXS("last")
		, LLC_CXS("--"), LLC_CXS("--literal"), LLC_CXS("")
		};
	::llc::vcst_t envp[] = {LLC_CXS("A=B")};
	::llc::SCommandLineArgs args = {};
	cnst ::llc::err_t parsed = ::llc::argsParse(args, argv, envp);
	if_fail_fe(parsed);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_PARSE_RESULT, parsed != 9
		, "Parse result:%i, expected:9."
		, parsed
		);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_PROGRAM_NAME, args.ProgramName != LLC_CXS("app")
		, "Program:'%.*s', expected:'app'."
		, (int)args.ProgramName.size(), args.ProgramName.begin()
		);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_ENVIRONMENT, args.Environment.size() != 1 || args.Environment[0] != LLC_CXS("A=B")
		, "Environment count:%u, expected one 'A=B' entry."
		, args.Environment.size()
		);
	if_fail_fe(::testArgsParsedValues(errors, args));
	if_fail_fe(::testArgsAccessors(errors, args));

	::llc::vcst_t reuseArgv[] = {LLC_CXS("other"), LLC_CXS("only")};
	::llc::vcst_t reuseEnvp[] = {LLC_CXS("C=D")};
	cnst ::llc::err_t reused = ::llc::argsParse(args, reuseArgv, reuseEnvp);
	if_fail_fe(reused);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_REUSE
		, reused != 1 || args.ProgramName != LLC_CXS("other") || args.Options.size() || args.Positionals.size() != 1 || args.Positionals[0] != LLC_CXS("only") || args.Environment.size() != 1 || args.Environment[0] != LLC_CXS("C=D")
		, "Reuse result:%i, options:%u, positionals:%u, environment:%u."
		, reused, args.Options.size(), args.Positionals.size(), args.Environment.size()
		);
	cnst ::llc::err_t cleared = ::llc::argsParse(args, ::llc::view<::llc::vcst_t>{});
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_REUSE, cleared || args.ProgramName.size() || args.Options.size() || args.Positionals.size() || args.Environment.size()
		, "Empty reuse result:%i, program:%u, options:%u, positionals:%u, environment:%u."
		, cleared, args.ProgramName.size(), args.Options.size(), args.Positionals.size(), args.Environment.size()
		);

	::llc::vcst_t emptyArgv[] =
		{ LLC_CXS("app")
		, LLC_CXS("--empty=")
		, LLC_CXS("-")
		, LLC_CXS("--flag")
		};
	cnst ::llc::err_t emptyParsed = ::llc::argsParse(args, emptyArgv);
	if_fail_fe(emptyParsed);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_EMPTY_VALUE
		, emptyParsed != 3 || args.Options.size() != 2 || args.Options[0].Key != LLC_CXS("empty") || args.Options[0].Val.size() || args.Options[1].Key != LLC_CXS("flag") || args.Options[1].Val.size() || args.Positionals.size() != 1 || args.Positionals[0] != LLC_CXS("-")
		, "Empty parse result:%i, options:%u, positionals:%u."
		, emptyParsed, args.Options.size(), args.Positionals.size()
		);
	rtrn 0;
}
