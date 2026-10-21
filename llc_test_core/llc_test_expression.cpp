#include "llc_test_core.h"
#include "llc_expression.h"
#include "llc_chrono.h"
#include "llc_timer.h"
#include "llc_file.h"
#include "llc_string_compose.h"

GDEFINE_ENUM_TYPE(EXPRESSION_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(EXPRESSION_TEST_RESULT, OK					, 0, "All expression-reader tests passed.");
GDEFINE_ENUM_VALUED(EXPRESSION_TEST_RESULT, VALID_RESULT			, 1, "expressionReaderParse() rejected a valid expression.");
GDEFINE_ENUM_VALUED(EXPRESSION_TEST_RESULT, VALID_TOKEN_COUNT		, 2, "A valid expression produced no tokens.");
GDEFINE_ENUM_VALUED(EXPRESSION_TEST_RESULT, VALID_VIEW_COUNT		, 3, "Expression token and view counts differ.");
GDEFINE_ENUM_VALUED(EXPRESSION_TEST_RESULT, VALID_TREE_COUNT		, 4, "Expression token and tree counts differ.");
GDEFINE_ENUM_VALUED(EXPRESSION_TEST_RESULT, VALID_ROOT_VIEW		, 5, "The root expression view differs from its source.");
GDEFINE_ENUM_VALUED(EXPRESSION_TEST_RESULT, VALID_TREE_NODE		, 6, "An expression tree node is null or refers to the wrong token.");
GDEFINE_ENUM_VALUED(EXPRESSION_TEST_RESULT, STEP_RANGE				, 7, "expressionReaderParseStep() accepted an out-of-range source position.");
GDEFINE_ENUM_VALUED(EXPRESSION_TEST_RESULT, INVALID_CHARACTER		, 8, "expressionReaderParse() accepted an invalid string character.");
GDEFINE_ENUM_VALUED(EXPRESSION_TEST_RESULT, TRUNCATED_CODEPOINT		, 9, "expressionReaderParse() accepted a truncated unicode escape.");
GDEFINE_ENUM_VALUED(EXPRESSION_TEST_RESULT, UNCLOSED_LEVEL			, 10, "expressionReaderParse() accepted an unclosed expression level.");
GDEFINE_ENUM_VALUED(EXPRESSION_TEST_RESULT, FORMAT					, 11, "Lambda formatting returned the wrong text or failed.");
GDEFINE_ENUM_VALUED(EXPRESSION_TEST_RESULT, REUSE					, 12, "Reused format tokens did not observe updated bindings.");
GDEFINE_ENUM_VALUED(EXPRESSION_TEST_RESULT, ROLLBACK					, 13, "Failed formatting changed the caller output.");
GDEFINE_ENUM_VALUED(EXPRESSION_TEST_RESULT, DATE					, 14, "Date field formatting or cached formatting differs.");

sttc ::llc::err_t testExpressionFormatting(ATestError & errors, ::llc::vcst_t outputFolder) {
	::llc::keyval<::llc::vcst_t> bindings[] = {{"hh", "03"}, {"mm", "04"}, {"empty", ""}};
	::llc::string examples;
	if_fail_fe(examples.append_string("Lambda bindings (quoted text is literal):\n"));
	for(cnst auto & binding : bindings)
		if_fail_fe(::llc::append_strings(examples, binding.Key, " = \"", binding.Val, "\"\n"));
	::llc::u2_t calls = 0;
	cnst ::llc::FError<::llc::vcst_c &, ::llc::vcst_t &> resolve = [&](::llc::vcst_c & key, ::llc::vcst_t & value) {
		++calls;
		rtrn ::llc::find(key, ::llc::view<cnst ::llc::keyval<::llc::vcst_t>>{bindings}, value);
	};
	cnst ::llc::keyval<::llc::vcst_t> cases[] = {
		{"", ""}, {"literal", "literal"}, {"{hh}{mm}", "0304"}, {"\\{hh}:{mm}", "\\{hh}:04"}, {"{empty}{hh}", "03"}
	};
	for(cnst auto & item : cases) {
		::llc::string output;
		cnst ::llc::err_t result = ::llc::expressionStringFormat(item.Key, resolve, output);
		LLC_TEST_CHECKF(errors, EXPRESSION_TEST_RESULT_FORMAT, ::llc::failed(result) || output != item.Val, "Format:%.*s, output:%.*s.", LLC_VIEW_ARGS(item.Key), LLC_VIEW_ARGS(output));
		if_fail_fe(::llc::append_strings(examples, "\nTemplate: \"", item.Key, "\"\nExpected: \"", item.Val, "\"\nActual:   \"", output, "\"\nResult: ", ::llc::str(result), "\n"));
	}
	LLC_TEST_CHECK(errors, EXPRESSION_TEST_RESULT_FORMAT, calls != 5);
	::llc::apod<::llc::SStripLiteralType> tokens;
	::llc::SStripLiteralState state;
	cnst ::llc::vcst_t format = "{hh}:{mm}";
	if_fail_fe(::llc::stripLiteralParse(state, tokens, format));
	bindings[0].Val = "23";
	::llc::string output;
	if_fail_fe(output.append_string("prefix:"));
	LLC_TEST_REQUIRE(errors, EXPRESSION_TEST_RESULT_REUSE, ::llc::failed(::llc::expressionStringFormat(format, tokens, resolve, output)));
	LLC_TEST_CHECK(errors, EXPRESSION_TEST_RESULT_REUSE, output != "prefix:23:04");
	if_fail_fe(::llc::append_strings(examples, "\nReused tokens after hh = \"", bindings[0].Val, "\"; initial output = \"prefix:\"\nTemplate: \"", format, "\"\nExpected: \"prefix:23:04\"\nActual:   \"", output, "\"\n"));
	cnst ::llc::sc_t * before = output.begin();
	cnst ::llc::err_t missingResult = ::llc::expressionStringFormat("{hh}:{missing}", resolve, output);
	LLC_TEST_CHECK(errors, EXPRESSION_TEST_RESULT_ROLLBACK, 0 <= missingResult);
	LLC_TEST_CHECK(errors, EXPRESSION_TEST_RESULT_ROLLBACK, output.begin() != before || output != "prefix:23:04" || output.begin()[output.size()] != 0);
	if_fail_fe(::llc::append_strings(examples, "\nTemplate: \"{hh}:{missing}\"\nExpected: negative result; output unchanged\nResult: ", ::llc::str(missingResult), "\nActual:   \"", output, "\"\n"));
	tokens[0].Span.End = format.size() + 1;
	LLC_TEST_CHECK(errors, EXPRESSION_TEST_RESULT_ROLLBACK, 0 <= ::llc::expressionStringFormat(format, tokens, resolve, output));
	LLC_TEST_CHECK(errors, EXPRESSION_TEST_RESULT_ROLLBACK, output.begin() != before || output != "prefix:23:04");

	cnst ::llc::s3_t now = ::llc::timeCurrentInMs();
	if_fail_fe(::llc::append_strings(examples, "\nLocal calendar bindings: yy=year, MM=month, dd=day, hh=hour, mm=minute, ss=second, ff=hundredths. All two digits.\nTimestamp (milliseconds since epoch): ", ::llc::str(now), "\n"));
	for(::llc::u2_t fraction : {0U, 7U, 73U, 990U, 999U}) {
		output.clear();
		LLC_TEST_REQUIRE(errors, EXPRESSION_TEST_RESULT_DATE, ::llc::failed(::llc::timeStringFormat("{ff}", now / 1000 * 1000 + fraction, output)));
		::llc::u2_t hundredths = 0;
		LLC_TEST_CHECK(errors, EXPRESSION_TEST_RESULT_DATE, output.size() != 2 || ::llc::parseIntegerDecimal(output, hundredths) != 2 || hundredths != fraction / 10);
		if_fail_fe(::llc::append_strings(examples, "Template: \"{ff}\"; milliseconds: ", ::llc::str(fraction), "; expected hundredths: ", ::llc::str(fraction / 10), "; actual: \"", output, "\"\n"));
	}
	cnst ::llc::vcst_t dateFormat = "{yy}{MM}{dd}_{hh}{mm}{ss}{ff}";
	tokens.clear(); state = {};
	if_fail_fe(::llc::stripLiteralParse(state, tokens, dateFormat));
	::llc::string fresh, cached;
	if_fail_fe(::llc::timeStringFormat(dateFormat, now, fresh));
	if_fail_fe(::llc::timeStringFormat(dateFormat, tokens, now, cached));
	LLC_TEST_CHECK(errors, EXPRESSION_TEST_RESULT_DATE, fresh != cached || cached.size() != 15 || cached[6] != '_');
	if_fail_fe(::llc::append_strings(examples, "\nTemplate: \"", dateFormat, "\"\nParse + format: \"", fresh, "\"\nReused tokens:  \"", cached, "\"\n"));
	before = cached.begin();
	LLC_TEST_CHECK(errors, EXPRESSION_TEST_RESULT_ROLLBACK, 0 <= ::llc::timeStringFormat("{hh}{unknown}", now, cached));
	LLC_TEST_CHECK(errors, EXPRESSION_TEST_RESULT_ROLLBACK, cached.begin() != before || cached != fresh);

	stxp ::llc::u2_t iterations = 10000;
	::llc::STimer timer;
	for(::llc::u2_t i = 0; i < iterations; ++i) {
		fresh.clear();
		if_fail_fe(::llc::timeStringFormat(dateFormat, now + i, fresh));
	}
	timer.Frame();
	cnst ::llc::u3_t parseAndFormat = timer.LastTimeMicroseconds;
	for(::llc::u2_t i = 0; i < iterations; ++i) {
		cached.clear();
		if_fail_fe(::llc::timeStringFormat(dateFormat, tokens, now + i, cached));
	}
	timer.Frame();
	LLC_TEST_CHECK(errors, EXPRESSION_TEST_RESULT_DATE, fresh != cached);
	always_printf("Date formatting, %u values: parse+format=%" LLC_FMT_U3 " us, reused tokens=%" LLC_FMT_U3 " us.", iterations, parseAndFormat, timer.LastTimeMicroseconds);
	if_fail_fe(::llc::append_strings(examples, "\nTiming for ", ::llc::str(iterations), " dates (microseconds):\nParse + format: ", ::llc::str(parseAndFormat), "\nReused tokens:  ", ::llc::str(timer.LastTimeMicroseconds), "\n"));
	if_fail_fe(::llc::fileFromMemory(outputFolder, "formatting_examples.txt", examples.cc().cu8()));
	rtrn 0;
}

::llc::err_t testExpression(ATestError & errors, ::llc::vcst_t outputFolder) {
	if_fail_fe(::testExpressionFormatting(errors, outputFolder));
	cnst ::llc::vcst_t			validExpression			= "application.test_udp";
	::llc::SExpressionReader	validReader				= {};
	cnst ::llc::err_t			validResult				= ::llc::expressionReaderParse(validReader, validExpression);
	always_printf("EXPRESSION input:'%.*s'; expected parse success; result:%i, tokens:%u.", LLC_VIEW_ARGS(validExpression), validResult, validReader.Token.size());
	LLC_TEST_REQUIREF(errors, EXPRESSION_TEST_RESULT_VALID_RESULT, ::llc::failed(validResult) , "Result:%i." , validResult );
	LLC_TEST_REQUIREF(errors, EXPRESSION_TEST_RESULT_VALID_TOKEN_COUNT, 0 == validReader.Token.size() , "Expression:%.*s." , LLC_VIEW_ARGS(validExpression) );
	LLC_TEST_REQUIREF(errors, EXPRESSION_TEST_RESULT_VALID_VIEW_COUNT, validReader.View.size() != validReader.Token.size() , "Views:%u, tokens:%u." , validReader.View.size(), validReader.Token.size() );
	LLC_TEST_REQUIREF(errors, EXPRESSION_TEST_RESULT_VALID_TREE_COUNT, validReader.Tree.size() != validReader.Token.size() , "Tree nodes:%u, tokens:%u." , validReader.Tree.size(), validReader.Token.size() );
	LLC_TEST_CHECKF(errors, EXPRESSION_TEST_RESULT_VALID_ROOT_VIEW, validReader.View[0] != validExpression , "Root:%.*s, expected:%.*s." , LLC_VIEW_ARGS(validReader.View[0]), LLC_VIEW_ARGS(validExpression) );
	for(::llc::u2_t iNode = 0; iNode < validReader.Tree.size(); ++iNode) {
		always_printf("Token:%u type:%s parent:%i span:%u..%u text:'%.*s'.", iNode, ::llc::get_value_namep(validReader.Token[iNode].Type), validReader.Token[iNode].ParentIndex, validReader.Token[iNode].Span.Begin, validReader.Token[iNode].Span.End, LLC_VIEW_ARGS(validReader.View[iNode]));
		LLC_TEST_CHECKF(errors, EXPRESSION_TEST_RESULT_VALID_TREE_NODE, 0 == validReader.Tree[iNode].get_ref() , "Null node:%u." , iNode );
		if(validReader.Tree[iNode])
			LLC_TEST_CHECKF(errors, EXPRESSION_TEST_RESULT_VALID_TREE_NODE, validReader.Tree[iNode]->Token != &validReader.Token[iNode] , "Node:%u, token:%p, expected:%p." , iNode, validReader.Tree[iNode]->Token, &validReader.Token[iNode] );
	}

	::llc::SExpressionReader	invalidReader			= {};
	cnst ::llc::err_t			emptyStepResult			= ::llc::expressionReaderParseStep(invalidReader, {});
	LLC_TEST_CHECKF(errors, EXPRESSION_TEST_RESULT_STEP_RANGE, 0 <= emptyStepResult , "Result:%i." , emptyStepResult );

	cnst ::llc::sc_t			invalidCharacterStorage[]	= {'\"', '\x01', '\"'};
	cnst ::llc::vcst_t			invalidCharacter			= invalidCharacterStorage;
	invalidReader										= {};
	cnst ::llc::err_t			invalidCharacterResult		= ::llc::expressionReaderParse(invalidReader, invalidCharacter);
	LLC_TEST_CHECKF(errors, EXPRESSION_TEST_RESULT_INVALID_CHARACTER, 0 <= invalidCharacterResult , "Result:%i, byte:0x%02X." , invalidCharacterResult, (::llc::u0_t)invalidCharacter[1] );

	cnst ::llc::vcst_t			truncatedCodepoint			= "\"\\u12\"";
	invalidReader										= {};
	cnst ::llc::err_t			truncatedCodepointResult	= ::llc::expressionReaderParse(invalidReader, truncatedCodepoint);
	LLC_TEST_CHECKF(errors, EXPRESSION_TEST_RESULT_TRUNCATED_CODEPOINT, 0 <= truncatedCodepointResult , "Result:%i, expression:%.*s." , truncatedCodepointResult, LLC_VIEW_ARGS(truncatedCodepoint) );

	cnst ::llc::vcst_t			unclosedLevel				= "application[";
	invalidReader										= {};
	cnst ::llc::err_t			unclosedLevelResult			= ::llc::expressionReaderParse(invalidReader, unclosedLevel);
	always_printf("EXPRESSION rejection cases (expected negative): empty parse-step=%i; quoted byte 01=%i; '%.*s'=%i; '%.*s'=%i.", emptyStepResult, invalidCharacterResult, LLC_VIEW_ARGS(truncatedCodepoint), truncatedCodepointResult, LLC_VIEW_ARGS(unclosedLevel), unclosedLevelResult);
	LLC_TEST_CHECKF(errors, EXPRESSION_TEST_RESULT_UNCLOSED_LEVEL, 0 <= unclosedLevelResult , "Result:%i, expression:%.*s." , unclosedLevelResult, LLC_VIEW_ARGS(unclosedLevel) );
	return 0;
}
