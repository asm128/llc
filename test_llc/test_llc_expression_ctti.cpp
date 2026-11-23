//#include "test_llc_suites.h"
//#include "llc_expression.h"
//#include "llc_expression_ctti.h"
//
//enum EXPRESSION_CTTI_TEST_RESULT : ::llc::u0_t
//	{ EXPRESSION_CTTI_TEST_RESULT_PARSE		= 0
//	, EXPRESSION_CTTI_TEST_RESULT_COUNT		= 1
//	, EXPRESSION_CTTI_TEST_RESULT_TOKEN		= 2
//	};
//
//stxp auto expressionCTTIValid = ::llc::expressionParseCTTI("parents['1'].name");
//static_assert(expressionCTTIValid.Success());
//static_assert(5 == expressionCTTIValid.Count);
//static_assert(::llc::EXPRESSION_READER_TYPE_TERM_ANY == expressionCTTIValid.Token.Storage[0].Type);
//
//stxp auto expressionCTTIInvalid = ::llc::expressionParseCTTI("left & right");
//static_assert(false == expressionCTTIInvalid.Success());
//static_assert(::llc::CTTI_EXPRESSION_PARSE_ERROR_INVALID_OPERATOR == expressionCTTIInvalid.Error);
//
//tplt<size_t N>
//sttc ::llc::err_t testExpressionCTTI(::llc::ATestError & errors, ::llc::sc_c (&source)[N]) {
//	cnst auto fixed = ::llc::expressionParseCTTI(source);
//	LLC_TEST_REQUIREF(errors, EXPRESSION_CTTI_TEST_RESULT_PARSE, false == fixed.Success(), "Error:%u, offset:%u, expression:'%s'.", fixed.Error, fixed.ErrorOffset, source);
//	::llc::SExpressionReader dynamic = {};
//	cnst ::llc::err_t parseResult = ::llc::expressionReaderParse(dynamic, source);
//	LLC_TEST_REQUIREF(errors, EXPRESSION_CTTI_TEST_RESULT_PARSE, ::llc::failed(parseResult), "Result:%i, expression:'%s'.", parseResult, source);
//	LLC_TEST_REQUIREF(errors, EXPRESSION_CTTI_TEST_RESULT_COUNT, fixed.Count != dynamic.Token.size(), "Static:%u, dynamic:%u, expression:'%s'.", fixed.Count, dynamic.Token.size(), source);
//	for(::llc::u2_t iToken = 0; iToken < fixed.Count; ++iToken) {
//		cnst ::llc::SExpressionToken & tokenFixed = fixed.Token.Storage[iToken];
//		cnst ::llc::SExpressionToken & tokenDynamic = dynamic.Token[iToken];
//		LLC_TEST_CHECKF(errors, EXPRESSION_CTTI_TEST_RESULT_TOKEN
//			, tokenFixed.ParentIndex != tokenDynamic.ParentIndex
//			|| tokenFixed.Type != tokenDynamic.Type
//			|| tokenFixed.Span != tokenDynamic.Span
//			|| tokenFixed.ClosingCondition != tokenDynamic.ClosingCondition
//			, "Token:%u, static:{parent:%i,type:%i,span:%u..%u,closing:%u}, dynamic:{parent:%i,type:%i,span:%u..%u,closing:%u}, expression:'%s'."
//			, iToken
//			, tokenFixed.ParentIndex, tokenFixed.Type, tokenFixed.Span.Begin, tokenFixed.Span.End, tokenFixed.ClosingCondition
//			, tokenDynamic.ParentIndex, tokenDynamic.Type, tokenDynamic.Span.Begin, tokenDynamic.Span.End, tokenDynamic.ClosingCondition
//			, source
//			);
//	}
//	rtrn 0;
//}
//
//::llc::err_t test_expression_ctti(STestRunnerState &, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
//	if_fail_fe(::testExpressionCTTI(errors, "name"));
//	if_fail_fe(::testExpressionCTTI(errors, "parents['1'].name"));
//	if_fail_fe(::testExpressionCTTI(errors, "!alive == !married"));
//	if_fail_fe(::testExpressionCTTI(errors, "alive && !married ? 'alive' : 'dead'"));
//	if_fail_fe(::testExpressionCTTI(errors, "children[(selection.index)]"));
//	if_fail_fe(::testExpressionCTTI(errors, "parents['1'].property.{selection.default_property_name_path}"));
//	if_fail_fe(::testExpressionCTTI(errors, "!!(!!alive) ? ('true') : ('false')"));
//	rtrn 0;
//}
