#include "llc_test_core.h"
#include "llc_expression.h"

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

::llc::err_t testExpression(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	cnst ::llc::vcst_t			validExpression			= "application.test_udp";
	::llc::SExpressionReader	validReader				= {};
	cnst ::llc::err_t			validResult				= ::llc::expressionReaderParse(validReader, validExpression);
	LLC_TEST_REQUIREF(errors, EXPRESSION_TEST_RESULT_VALID_RESULT, ::llc::failed(validResult) , "Result:%i." , validResult );
	LLC_TEST_REQUIREF(errors, EXPRESSION_TEST_RESULT_VALID_TOKEN_COUNT, 0 == validReader.Token.size() , "Expression:%.*s." , LLC_VIEW_ARGS(validExpression) );
	LLC_TEST_REQUIREF(errors, EXPRESSION_TEST_RESULT_VALID_VIEW_COUNT, validReader.View.size() != validReader.Token.size() , "Views:%u, tokens:%u." , validReader.View.size(), validReader.Token.size() );
	LLC_TEST_REQUIREF(errors, EXPRESSION_TEST_RESULT_VALID_TREE_COUNT, validReader.Tree.size() != validReader.Token.size() , "Tree nodes:%u, tokens:%u." , validReader.Tree.size(), validReader.Token.size() );
	LLC_TEST_CHECKF(errors, EXPRESSION_TEST_RESULT_VALID_ROOT_VIEW, validReader.View[0] != validExpression , "Root:%.*s, expected:%.*s." , LLC_VIEW_ARGS(validReader.View[0]), LLC_VIEW_ARGS(validExpression) );
	for(::llc::u2_t iNode = 0; iNode < validReader.Tree.size(); ++iNode) {
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
	LLC_TEST_CHECKF(errors, EXPRESSION_TEST_RESULT_UNCLOSED_LEVEL, 0 <= unclosedLevelResult , "Result:%i, expression:%.*s." , unclosedLevelResult, LLC_VIEW_ARGS(unclosedLevel) );
	return 0;
}
