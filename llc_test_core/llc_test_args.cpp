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
	for(::llc::u2_t iOption = 0; iOption < args.Options.size(); ++iOption) {
		LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_OPTIONS, args.Options[iOption].Key != expectedOptions[iOption].Key
			, "Option:%u key:'%.*s', expected:'%.*s'."
			, iOption, (int)args.Options[iOption].Key.size(), args.Options[iOption].Key.begin()
			, (int)expectedOptions[iOption].Key.size(), expectedOptions[iOption].Key.begin());
		LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_OPTIONS, args.Options[iOption].Val != expectedOptions[iOption].Val
			, "Option:%u value:'%.*s', expected:'%.*s'."
			, iOption, (int)args.Options[iOption].Val.size(), args.Options[iOption].Val.begin()
			, (int)expectedOptions[iOption].Val.size(), expectedOptions[iOption].Val.begin());
	}

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
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_OPTION_INDEX, firstInclude != 0
		, "First --include index:%i, expected:0.", firstInclude);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_OPTION_INDEX, secondInclude != 1
		, "Second --include index:%i, expected:1.", secondInclude);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_OPTION_INDEX, thirdInclude != 3
		, "Third --include index:%i, expected:3.", thirdInclude);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_OPTION_INDEX, afterInclude != -1
		, "--include index after offset 4:%i, expected:-1.", afterInclude);

	::llc::aobj<::llc::vcst_t> values = {LLC_CXS("seed")};
	cnst ::llc::err_t valueCount = ::llc::argsOptionValues(args, LLC_CXS("include"), values);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_OPTION_VALUES, valueCount != 3
		, "Collected --include value count:%i, expected:3.", valueCount);
	LLC_TEST_REQUIRE(errors, ARGS_TEST_RESULT_OPTION_VALUES, values.size() != 4
		, "Collected --include output size:%u, expected:4 including the seed.", values.size());
	cnst ::llc::vcst_t expectedValues[] = {LLC_CXS("seed"), LLC_CXS("a"), LLC_CXS("b"), LLC_CXS("c")};
	for(::llc::u2_t iValue = 0; iValue < values.size(); ++iValue)
		LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_OPTION_VALUES, values[iValue] != expectedValues[iValue]
			, "Collected value:%u is '%.*s', expected '%.*s'."
			, iValue, (int)values[iValue].size(), values[iValue].begin(), (int)expectedValues[iValue].size(), expectedValues[iValue].begin()
			);

	::llc::vcst_t singleValue = {};
	cnst ::llc::err_t singleIndex = ::llc::argsOptionValue(args, LLC_CXS("output"), singleValue);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_OPTION_VALUE, singleIndex != 4
		, "--output index:%i, expected:4.", singleIndex);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_OPTION_VALUE, singleValue != LLC_CXS("result")
		, "--output value:'%.*s', expected:'result'.", (int)singleValue.size(), singleValue.begin());

	::llc::vcst_t ambiguousValue = LLC_CXS("stale");
	::llc::vcst_t missingValue = LLC_CXS("stale");
	::llc::setupLogCallbacks(0, 0);
	cnst ::llc::err_t ambiguous = ::llc::argsOptionValue(args, LLC_CXS("include"), ambiguousValue);
	cnst ::llc::err_t missing = ::llc::argsOptionValue(args, LLC_CXS("missing"), missingValue);
	::llc::setupDefaultLogCallbacks();
	cnst ::llc::err_t missingCount = ::llc::argsOptionValues(args, LLC_CXS("missing"), values);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_AMBIGUOUS_VALUE, ambiguous != -2
		, "Repeated --include single-value result:%i, expected:-2.", ambiguous);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_AMBIGUOUS_VALUE, ambiguousValue.size()
		, "Repeated --include single-value output size:%u, expected:0.", ambiguousValue.size());
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_MISSING_VALUE, missing != -1
		, "Missing option single-value result:%i, expected:-1.", missing);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_MISSING_VALUE, missingValue.size()
		, "Missing option single-value output size:%u, expected:0.", missingValue.size());
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_MISSING_VALUE, missingCount
		, "Missing option collected value count:%i, expected:0.", missingCount);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_MISSING_VALUE, values.size() != 4
		, "Missing option changed collected output size:%u, expected:4.", values.size());
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
	LLC_TEST_REQUIRE(errors, ARGS_TEST_RESULT_PARSE_RESULT, 0 > parsed
		, "Initial argsParse() failed with result:%i.", parsed);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_PARSE_RESULT, parsed != 9
		, "Parse result:%i, expected:9."
		, parsed
		);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_PROGRAM_NAME, args.ProgramName != LLC_CXS("app")
		, "Program:'%.*s', expected:'app'."
		, (int)args.ProgramName.size(), args.ProgramName.begin()
		);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_ENVIRONMENT, args.Environment.size() != 1
		, "Environment entry count:%u, expected:1.", args.Environment.size());
	if(args.Environment.size()) {
		LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_ENVIRONMENT, args.Environment[0] != LLC_CXS("A=B")
			, "Environment entry 0:'%.*s', expected:'A=B'."
			, (int)args.Environment[0].size(), args.Environment[0].begin());
	}
	if_fail_fe(::testArgsParsedValues(errors, args));
	if_fail_fe(::testArgsAccessors(errors, args));

	::llc::vcst_t reuseArgv[] = {LLC_CXS("other"), LLC_CXS("only")};
	::llc::vcst_t reuseEnvp[] = {LLC_CXS("C=D")};
	cnst ::llc::err_t reused = ::llc::argsParse(args, reuseArgv, reuseEnvp);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_REUSE, reused != 1
		, "Reused parse result:%i, expected:1.", reused);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_REUSE, args.ProgramName != LLC_CXS("other")
		, "Reused program name:'%.*s', expected:'other'."
		, (int)args.ProgramName.size(), args.ProgramName.begin());
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_REUSE, args.Options.size()
		, "Reused option count:%u, expected:0.", args.Options.size());
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_REUSE, args.Positionals.size() != 1
		, "Reused positional count:%u, expected:1.", args.Positionals.size());
	if(args.Positionals.size()) {
		LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_REUSE, args.Positionals[0] != LLC_CXS("only")
			, "Reused positional 0:'%.*s', expected:'only'."
			, (int)args.Positionals[0].size(), args.Positionals[0].begin());
	}
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_REUSE, args.Environment.size() != 1
		, "Reused environment entry count:%u, expected:1.", args.Environment.size());
	if(args.Environment.size()) {
		LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_REUSE, args.Environment[0] != LLC_CXS("C=D")
			, "Reused environment entry 0:'%.*s', expected:'C=D'."
			, (int)args.Environment[0].size(), args.Environment[0].begin());
	}
	cnst ::llc::err_t cleared = ::llc::argsParse(args, ::llc::view<::llc::vcst_t>{});
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_REUSE, cleared
		, "Empty reused parse result:%i, expected:0.", cleared);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_REUSE, args.ProgramName.size()
		, "Empty reused program name length:%u, expected:0.", args.ProgramName.size());
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_REUSE, args.Options.size()
		, "Empty reused option count:%u, expected:0.", args.Options.size());
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_REUSE, args.Positionals.size()
		, "Empty reused positional count:%u, expected:0.", args.Positionals.size());
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_REUSE, args.Environment.size()
		, "Empty reused environment count:%u, expected:0.", args.Environment.size());

	::llc::vcst_t emptyArgv[] =
		{ LLC_CXS("app")
		, LLC_CXS("--empty=")
		, LLC_CXS("-")
		, LLC_CXS("--flag")
		};
	cnst ::llc::err_t emptyParsed = ::llc::argsParse(args, emptyArgv);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_EMPTY_VALUE, emptyParsed != 3
		, "Empty-value parse result:%i, expected:3.", emptyParsed);
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_EMPTY_VALUE, args.Options.size() != 2
		, "Empty-value option count:%u, expected:2.", args.Options.size());
	if(1 <= args.Options.size()) {
		LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_EMPTY_VALUE, args.Options[0].Key != LLC_CXS("empty")
			, "Empty-value option 0 key:'%.*s', expected:'empty'."
			, (int)args.Options[0].Key.size(), args.Options[0].Key.begin());
		LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_EMPTY_VALUE, args.Options[0].Val.size()
			, "Empty-value option 0 value length:%u, expected:0.", args.Options[0].Val.size());
	}
	if(2 <= args.Options.size()) {
		LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_EMPTY_VALUE, args.Options[1].Key != LLC_CXS("flag")
			, "Empty-value option 1 key:'%.*s', expected:'flag'."
			, (int)args.Options[1].Key.size(), args.Options[1].Key.begin());
		LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_EMPTY_VALUE, args.Options[1].Val.size()
			, "Empty-value option 1 value length:%u, expected:0.", args.Options[1].Val.size());
	}
	LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_EMPTY_VALUE, args.Positionals.size() != 1
		, "Empty-value positional count:%u, expected:1.", args.Positionals.size());
	if(args.Positionals.size()) {
		LLC_TEST_CHECK(errors, ARGS_TEST_RESULT_EMPTY_VALUE, args.Positionals[0] != LLC_CXS("-")
			, "Empty-value positional 0:'%.*s', expected:'-'."
			, (int)args.Positionals[0].size(), args.Positionals[0].begin());
	}
	rtrn 0;
}
