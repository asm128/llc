#include "llc_json_expression.h"

#include "llc_expression.h"
#include "llc_parse.h"

#define llc_jexpr_info_printf(...) // info_printf

//#define LLC_JSON_EXPRESSION_DEBUG

#if defined(LLC_JSON_EXPRESSION_DEBUG)
static	::llc::error_t	printNode		(const ::llc::SExpressionNode* node, const ::llc::vcst_t & expression)			{
	const ::llc::apod<sc_t>					viewPrintable					= ::llc::string({&expression[node->Token->Span.Begin], node->Token->Span.End - node->Token->Span.Begin});
	const ::llc::vcst_t						viewTokenType					= ::llc::get_value_label(node->Token->Type);
	info_printf("Entering expression node type: %u (%s). Node Span: {%u, %u}. Parent index: %u. Object index: %u. Text: %s", node->Token->Type, viewTokenType.begin(), node->Token->Span.Begin, node->Token->Span.End, node->Token->ParentIndex, node->ObjectIndex, viewPrintable.begin());
	for(::llc::u2_t iChildren = 0; iChildren < node->Children.size(); ++iChildren)
		::printNode(node->Children[iChildren], expression);
	info_printf("Exiting expression node type: %u (%s). Node Span: {%u, %u}. Parent index: %u. Object index: %u. Text: %s", node->Token->Type, viewTokenType.begin(), node->Token->Span.Begin, node->Token->Span.End, node->Token->ParentIndex, node->ObjectIndex, viewPrintable.begin());
	return 0;
}
#endif

static	::llc::error_t	evaluateJSONBool		(const ::llc::SJSONNode & jsonToEvaluate, ::llc::vcst_c & boolView)	{
	int32_t												evalResult								= -1;
	if(jsonToEvaluate.Token->Type == ::llc::JSON_TYPE_NULL)
		evalResult										= 0;
	else if(jsonToEvaluate.Token->Type == ::llc::JSON_TYPE_BOOLEAN)
		evalResult										= (boolView == ::llc::strBool[1]) ? 1 : 0;
	else if(jsonToEvaluate.Token->Type == ::llc::JSON_TYPE_INTEGER)
		evalResult										= jsonToEvaluate.Token->Value == 0 ? 0 : 1;
	else if(jsonToEvaluate.Token->Type == ::llc::JSON_TYPE_DECIMAL) {
		::llc::f3_t									value									= {};
		memcpy(&value, &jsonToEvaluate.Token->Value, sizeof(value));
		evalResult										= value == 0 ? 0 : 1;
	}
	else if(jsonToEvaluate.Token->Type == ::llc::JSON_TYPE_STRING)
		evalResult										= boolView.size() ? 1 : 0;
	else if(jsonToEvaluate.Token->Type == ::llc::JSON_TYPE_ARRAY)
		evalResult										= jsonToEvaluate.Children.size() ? 1 : 0;
	else if(jsonToEvaluate.Token->Type == ::llc::JSON_TYPE_OBJECT)
		evalResult										= jsonToEvaluate.Children.size() ? 1 : 0;
	else
		error_printf("Failed to solve expression: %s", ::llc::string(boolView).begin());
	return evalResult;
}

static	::llc::error_t	evaluateBoolResult		(const ::llc::SJSONNode * jsonToEvaluate, int32_t indexNodeJSON, const ::llc::vcst_t & boolView)	{
	::llc::error_t				evalResult								= -1;
	if(indexNodeJSON < 0)
		evalResult										= (0 == boolView.size() || boolView == ::llc::strZero || boolView == ::llc::strBool[0] ||  boolView == ::llc::strNull || boolView == ::llc::strEmptyObj || boolView == ::llc::strEmptyArr) ? 0 : 1;
	else if(indexNodeJSON >= 0)
		evalResult										= ::evaluateJSONBool(*jsonToEvaluate, boolView);
	return evalResult;
}

static	::llc::error_t	evaluateAndClearBoolCarry				(::llc::SEvaluationStepResult & lastResult, const ::llc::SJSONNode * jsonToEvaluate, int32_t indexNodeJSON, const ::llc::vcst_t & boolView)	{
	if(false == lastResult.LastValueCarry)
		return ::evaluateBoolResult(jsonToEvaluate, (int32_t)indexNodeJSON, boolView);
	else {
		lastResult.LastValueCarry						= false;
		return lastResult.LastValue ? 1 : 0;
	}
}

static	::llc::error_t	evaluateExpression						(::llc::SJSONExpressionSolver& results, const ::llc::SExpressionReader & readerExpression, const ::llc::u2_t indexNodeExpression, const ::llc::SJSONReader& inputJSON, ::llc::u2_t indexNodeJSON, ::llc::vcst_t & output);

static	::llc::error_t	evaluateExpressionAndBoolResult			(::llc::SJSONExpressionSolver& results, const ::llc::SExpressionReader & readerExpression, ::llc::u2_t indexExpressionToken, const ::llc::SJSONReader & jsonInput, const int32_t indexJSONNode)	{
	::llc::vcst_t								viewOfExpressionResult					= {};
	const int32_t										indexOfResolvedSubExpression			= ::evaluateExpression(results, readerExpression, indexExpressionToken, jsonInput, indexJSONNode, viewOfExpressionResult);
	if_true_fef(-1 == indexOfResolvedSubExpression, "Failed to resolve subexpression: '%s'.", ::llc::string(readerExpression.View[indexExpressionToken]).begin());
	return ::evaluateBoolResult((indexOfResolvedSubExpression >= 0) ? (const ::llc::SJSONNode*)jsonInput[indexOfResolvedSubExpression] : 0, indexOfResolvedSubExpression, viewOfExpressionResult);
}


// Returns an index pointing to the resulting JSON element, or a -0xF - the index of the resulting expression element.
static	::llc::error_t	evaluateExpression						(::llc::SJSONExpressionSolver& results, const ::llc::SExpressionReader & readerExpression, const ::llc::u2_t indexNodeExpression, const ::llc::SJSONReader& inputJSON, ::llc::u2_t indexNodeJSON, ::llc::vcst_t & output)	{
	if_true_fef(indexNodeExpression >= readerExpression.Tree.size() || 0 == readerExpression.Tree[indexNodeExpression], "Invalid expression node: %u.", indexNodeExpression);
	if_true_fef(indexNodeJSON >= inputJSON.Tree.size() || 0 == inputJSON.Tree[indexNodeJSON], "Invalid input json or index node: %u.", indexNodeJSON);
	const ::llc::SExpressionNode	* nodeExpression					= readerExpression.Tree[indexNodeExpression];	// Take this
	llc_jexpr_info_printf("Processing expression: '%s'", ::llc::string(readerExpression.View[indexNodeExpression]).begin());
	::llc::SEvaluationStepResult	lastResult							= {indexNodeJSON,};
	for(::llc::u2_t iFirstLevelExpression = 0; iFirstLevelExpression < nodeExpression->Children.size(); ++iFirstLevelExpression) {
		const ::llc::SExpressionNode		& childToSolve					= *nodeExpression->Children[iFirstLevelExpression];
		const ::llc::SJSONNode				* currentJSON					= (0 <= (int32_t)indexNodeJSON) ? (const ::llc::SJSONNode*)inputJSON.Tree[indexNodeJSON] : 0;
		//const ::llc::SExpressionNode		* currentLiteral				= (0 <= (int32_t)indexNodeJSON) ? 0 : readerExpression.Tree[(((int32_t)indexNodeJSON) + 0xF) * -1];
		const ::llc::vcst_t					currentView						= lastResult.Output;
		const ::llc::vcst_t					currentExpressionView			= readerExpression.View[childToSolve.ObjectIndex];
		const ::llc::JSON_TYPE				currentType						= currentJSON ? currentJSON->Token->Type : ::llc::JSON_TYPE_UNKNOWN;
		const ::llc::EXPRESSION_READER_TYPE	expressionType					= childToSolve.Token->Type;
		lastResult.Expression			= currentExpressionView;
		llc_jexpr_info_printf("Reading first-level expression: %u.", iFirstLevelExpression);
		llc_jexpr_info_printf("Current json view  (%u): '%s'", ::llc::string(currentView).begin());
		llc_jexpr_info_printf("Current child view (%u): '%s'", ::llc::string(currentExpressionView).begin());
		if(::llc::EXPRESSION_READER_TYPE_LITERAL == expressionType) {
			lastResult.Output			= output = currentExpressionView;
			lastResult.IndexJSONResult	= -(childToSolve.ObjectIndex + 0xF);
		}
		else {
			if(::llc::EXPRESSION_READER_TYPE_KEY == expressionType) {
				::llc::vcst_t				strKey							= currentExpressionView;
				if_true_fef(currentJSON && currentType != ::llc::JSON_TYPE_OBJECT, "Only objects can be accessed by key. JSON type: %s. Key to solve: %s.", ::llc::get_value_label(currentType).begin(), ::llc::string(currentExpressionView).begin());
				if(1 == childToSolve.Children.size()
					&& (::llc::EXPRESSION_READER_TYPE_TERM_KEY == childToSolve.Children[0]->Token->Type || ::llc::EXPRESSION_READER_TYPE_TERM_ANY == childToSolve.Children[0]->Token->Type)) {
					cnst ::llc::err_t indexOfResolvedSubExpression = ::evaluateExpression(results, readerExpression, childToSolve.ObjectIndex, inputJSON, (0 >= lastResult.IndexJSONResult) ? lastResult.IndexJSONResult : lastResult.IndexRootJSONNode, strKey);
					(void)indexOfResolvedSubExpression;
				}
				if_fail_fef(lastResult.IndexJSONResult = ::llc::jsonObjectValueGet(*currentJSON, inputJSON.View, {strKey.begin(), strKey.size()}), "Key not found: %s.", ::llc::string(strKey).begin());
				lastResult.Output	= output = inputJSON.View[lastResult.IndexJSONResult];
			}
			else {
				::llc::vcst_t				viewOfExpressionResult			= {};
				if(::llc::EXPRESSION_READER_TYPE_TERM_INDEX == expressionType) {
					if_true_fef(currentJSON && currentType != ::llc::JSON_TYPE_ARRAY && currentType != ::llc::JSON_TYPE_STRING, "Only arrays can be accessed by index. JSON type: %s.", ::llc::get_value_label(currentType).begin());
					const int32_t			indexOfResolvedSubExpression	= ::evaluateExpression(results, readerExpression, childToSolve.ObjectIndex, inputJSON, lastResult.IndexRootJSONNode, viewOfExpressionResult);
					if_true_fef(-1 == indexOfResolvedSubExpression, "Failed to solve expression: %s.", ::llc::string(currentExpressionView).begin());
					if_true_fef(indexOfResolvedSubExpression >= 0 && inputJSON.Token[indexOfResolvedSubExpression].Type != ::llc::JSON_TYPE_INTEGER, "Arrays can only be accessed by index with a json integer. Type found: %s.",::llc::get_value_label(inputJSON.Token[indexOfResolvedSubExpression].Type).begin());
					uint64_t				numberRead						= 0;
					llc_necs(llc::parseIntegerDecimal(viewOfExpressionResult, numberRead));
					if(currentType == ::llc::JSON_TYPE_STRING) {
						if_true_fef(numberRead >= currentView.size(), "String index out of range: %" LLC_FMT_U3 ". String size: %u.", numberRead, currentView.size());
						lastResult.Output	= output = {&inputJSON.View[lastResult.IndexJSONResult][(::llc::u2_t)numberRead], 1};
					}
					else {
						if_fail_fef(lastResult.IndexJSONResult = ::llc::jsonArrayValueGet(*currentJSON, (::llc::u2_t)numberRead), "Value not found for index: %lli.", numberRead);
						lastResult.Output	= output = inputJSON.View[lastResult.IndexJSONResult];
					}
				}
				else if(::llc::EXPRESSION_READER_TYPE_TERM_KEY == expressionType) {
					if_true_fef(currentJSON && currentType != ::llc::JSON_TYPE_OBJECT, "Only objects can be accessed by key. JSON type: %s.", ::llc::get_value_label(currentType).begin());
					int32_t						indexOfResolvedSubExpression			= ::evaluateExpression(results, readerExpression, childToSolve.ObjectIndex, inputJSON, lastResult.IndexRootJSONNode, viewOfExpressionResult);
					if_true_fef(-1 == (lastResult.IndexJSONResult = indexOfResolvedSubExpression), "Failed to solve expression: %s.", ::llc::string(currentExpressionView).begin());
					::llc::SExpressionReader	reader;
					llc_necall(llc::expressionReaderParse(reader, viewOfExpressionResult), "Failed to read JSONeN expression: '%s'.", ::llc::string(viewOfExpressionResult).begin());
					indexOfResolvedSubExpression	= ::llc::jsonExpressionResolve(reader, inputJSON, indexNodeJSON, results, output);
					lastResult.Output		= output;
					if_true_fef(-1 == (lastResult.IndexJSONResult = indexOfResolvedSubExpression), "Failed to evaluate child expression: %.*s.", LLC_VIEW_ARGS(viewOfExpressionResult));
				}
				else if(::llc::EXPRESSION_READER_TYPE_UNARY_NOT == expressionType) {
					const int32_t										evalResult							= ::evaluateExpressionAndBoolResult(results, readerExpression, childToSolve.ObjectIndex, inputJSON, lastResult.IndexJSONResult);
					lastResult.SetBoolCarry(0 == evalResult, output);
					lastResult.Output								= output;
				}
				else if(::llc::EXPRESSION_READER_TYPE_TERM_AND == expressionType) {
					const int32_t										prevResult							= ::evaluateAndClearBoolCarry(lastResult, currentJSON, (int32_t)indexNodeJSON, currentView);
					int32_t												evalResult							= ::evaluateExpressionAndBoolResult(results, readerExpression, childToSolve.ObjectIndex, inputJSON, lastResult.IndexRootJSONNode);
					lastResult.SetBoolCarry(evalResult && prevResult, output);
					lastResult.Output								= output;
				}
				else if(::llc::EXPRESSION_READER_TYPE_TERM_OR == expressionType) {
					const int32_t										prevResult							= ::evaluateAndClearBoolCarry(lastResult, currentJSON, (int32_t)indexNodeJSON, currentView);
					const int32_t										evalResult							= ::evaluateExpressionAndBoolResult(results, readerExpression, childToSolve.ObjectIndex, inputJSON, lastResult.IndexRootJSONNode);
					lastResult.SetBoolCarry(evalResult || prevResult, output);
					lastResult.Output								= output;
				}
				else if(::llc::EXPRESSION_READER_TYPE_TERM_ANY == expressionType) {
					const int32_t										indexOfResolvedSubExpression		= ::evaluateExpression(results, readerExpression, childToSolve.ObjectIndex, inputJSON, lastResult.IndexJSONResult, viewOfExpressionResult);
					lastResult.Output								= output = viewOfExpressionResult;
					if_true_fef(-1 == indexOfResolvedSubExpression, "Failed to resolve expression: %s.", ::llc::string(currentExpressionView).begin());
					lastResult.IndexJSONResult									= indexOfResolvedSubExpression;
				}
				else {
					if(::llc::EXPRESSION_READER_TYPE_TERM_BOOL == expressionType) {
						const int32_t										prevResult							= ::evaluateAndClearBoolCarry(lastResult, currentJSON, (int32_t)indexNodeJSON, currentView);
						const int32_t										evalResult							= prevResult ? 0 : 1;
						{
							const ::llc::u2_t										expressionResultChildIndex			= (iFirstLevelExpression == nodeExpression->Children.size()-1) ? iFirstLevelExpression : iFirstLevelExpression + evalResult;
							const ::llc::SExpressionNode						& childEval							= *nodeExpression->Children[expressionResultChildIndex];
							const ::llc::vcst_t						& childEvalView						= readerExpression.View[childEval.ObjectIndex];(void)childEvalView;
							const int32_t										indexOfResolvedSubExpression		= ::evaluateExpression(results, readerExpression, childEval.ObjectIndex, inputJSON, lastResult.IndexRootJSONNode, viewOfExpressionResult);
							if_true_fef(-1 == indexOfResolvedSubExpression, "Failed to resolve subexpression: '%s'.", ::llc::string(childEvalView).begin());
							lastResult.Output								= output = viewOfExpressionResult;
							lastResult.IndexJSONResult						= indexOfResolvedSubExpression;
						}
						iFirstLevelExpression							+= prevResult;
					}
					else if(::llc::EXPRESSION_READER_TYPE_TERM_EQUALS == expressionType) {
						const int32_t										indexOfResolvedSubExpression		= ::evaluateExpression(results, readerExpression, childToSolve.ObjectIndex, inputJSON, lastResult.IndexRootJSONNode, viewOfExpressionResult);
						if_true_fef(-1 == indexOfResolvedSubExpression, "Failed to resolve subexpression: '%s'.", ::llc::string(currentExpressionView).begin());
						const ::llc::JSON_TYPE								resultType							= (indexOfResolvedSubExpression >= 0) ? inputJSON.Token[indexOfResolvedSubExpression].Type : ::llc::JSON_TYPE_UNKNOWN;
						const ::llc::SJSONNode								* resultJSON						= (indexOfResolvedSubExpression >= 0) ? (const ::llc::SJSONNode*)inputJSON[indexOfResolvedSubExpression] : nullptr;
						if( viewOfExpressionResult == lastResult.Output
						 || (0 == lastResult.Output			.size() && ::llc::strNull		== viewOfExpressionResult	)
						 || (0 == lastResult.Output			.size() && ::llc::strBool[0]	== viewOfExpressionResult	)
						 || (0 == viewOfExpressionResult	.size() && ::llc::strNull		== lastResult.Output		)
						 || (0 == viewOfExpressionResult	.size() && ::llc::strBool[0]	== lastResult.Output		)
						 || (::llc::strBool[0]	== viewOfExpressionResult	&& ::llc::strNull		== lastResult.Output)
						 || (::llc::strNull		== viewOfExpressionResult	&& ::llc::strBool[0]	== lastResult.Output)
						 ) {
							lastResult.SetBoolCarry(true, output);
							lastResult.Output								= output;
						} else {
							if(currentType == ::llc::JSON_TYPE_STRING || resultType == ::llc::JSON_TYPE_STRING) {
								lastResult.SetBoolCarry(false, output);
								lastResult.Output								= output;
							}
							else if((currentType == ::llc::JSON_TYPE_INTEGER	&& resultType == ::llc::JSON_TYPE_INTEGER	)
								 || (currentType == ::llc::JSON_TYPE_DECIMAL		&& resultType == ::llc::JSON_TYPE_DECIMAL	)
							) {
								double												valueLeft				= 0;
								double												valueRight				= 0;
								if(currentType == ::llc::JSON_TYPE_INTEGER) {
									valueLeft										= (double)currentJSON->Token->Value;
									valueRight										= (double)resultJSON->Token->Value;
								}
								else {
									memcpy(&valueLeft , &currentJSON->Token->Value, sizeof(uint64_t));
									memcpy(&valueRight, &resultJSON ->Token->Value, sizeof(uint64_t));
								}
								lastResult.SetBoolCarry(valueLeft == valueRight, output);
								lastResult.Output								= output;
							}
							else if(currentType == ::llc::JSON_TYPE_INTEGER	&& resultType == ::llc::JSON_TYPE_DECIMAL) {
								const double 										valueLeft				= (double)currentJSON->Token->Value;
								double 												valueRight				= 0;
								memcpy(&valueRight, &resultJSON->Token->Value, sizeof(uint64_t));
								lastResult.SetBoolCarry(valueLeft == valueRight, output);
								lastResult.Output								= output;
							}
							else if(currentType == ::llc::JSON_TYPE_DECIMAL && resultType == ::llc::JSON_TYPE_INTEGER) {
								const double 										valueRight				= (double)resultJSON->Token->Value;
								double 												valueLeft				= 0;
								memcpy(&valueLeft, &currentJSON->Token->Value, sizeof(uint64_t));
								lastResult.SetBoolCarry(valueLeft == valueRight, output);
								lastResult.Output								= output;
							}
							else if(::llc::JSON_TYPE_BOOLEAN == resultType	|| ::llc::JSON_TYPE_NULL == resultType
								 || ::llc::JSON_TYPE_BOOLEAN == currentType	|| ::llc::JSON_TYPE_NULL == currentType	) {
								const int32_t										prevResult							= ::evaluateAndClearBoolCarry	(lastResult, currentJSON, (int32_t)indexNodeJSON, currentView);
								const int32_t										evalResult							= ::evaluateBoolResult			(resultJSON, indexOfResolvedSubExpression, viewOfExpressionResult);;
								lastResult.SetBoolCarry(evalResult == prevResult, output);
								lastResult.Output								= output;
							}
							else if(currentJSON) {
								if((indexOfResolvedSubExpression >= 0) && currentType != resultType) {
									lastResult.SetBoolCarry(false, output);
									lastResult.Output								= output;
								}
								else if(currentType == ::llc::JSON_TYPE_ARRAY	) { if(indexOfResolvedSubExpression < 0) { lastResult.SetBoolCarry(lastResult.Output == viewOfExpressionResult, output); lastResult.Output = output; } else { lastResult.SetBoolCarry(::llc::jsonCompareArray	(*currentJSON, *resultJSON, inputJSON.View), output); lastResult.Output = output; }  }
								else if(currentType == ::llc::JSON_TYPE_OBJECT	) { if(indexOfResolvedSubExpression < 0) { lastResult.SetBoolCarry(lastResult.Output == viewOfExpressionResult, output); lastResult.Output = output; } else { lastResult.SetBoolCarry(::llc::jsonCompareObject	(*currentJSON, *resultJSON, inputJSON.View), output); lastResult.Output = output; }  }
							}
							else {
								if(indexOfResolvedSubExpression < 0) {
									lastResult.SetBoolCarry(lastResult.Output == viewOfExpressionResult, output);
									lastResult.Output								= output;
								}
								else if(resultType == ::llc::JSON_TYPE_ARRAY	) { lastResult.SetBoolCarry(lastResult.Output == viewOfExpressionResult, output); lastResult.Output = output; }
								else if(resultType == ::llc::JSON_TYPE_OBJECT	) { lastResult.SetBoolCarry(lastResult.Output == viewOfExpressionResult, output); lastResult.Output = output; }
							}
						}
					}
					else {
						error_printf("Unrecognized expression reader type: '%s'.", ::llc::get_value_label(childToSolve.Token->Type).begin());
						return -1;
					}
				}
			}
		}
		if_fail_fe(results.Results.push_back(lastResult));
		indexNodeJSON									= lastResult.IndexJSONResult;
	}
	return lastResult.IndexJSONResult;
}

::llc::error_t			llc::jsonExpressionResolve				(const ::llc::SExpressionReader & reader, const ::llc::SJSONReader & inputJSON, ::llc::u2_t indexNodeJSON, ::llc::SJSONExpressionSolver& results, ::llc::vcst_t & output) {
	const ::llc::aobj<::llc::vcst_t>	& expressionViews						= reader.View;
	for(::llc::u2_t iView = 0; iView < expressionViews.size(); ++iView) {
		const ::llc::vcst_t				& viewExpression						= expressionViews[iView];
		const ::llc::SExpressionToken	& typeExpression						= reader.Token[iView];
		(void)typeExpression;
		if(viewExpression.size()) {
			llc_jexpr_info_printf("Expression element: %s. Type: %s. Parent: %i. Begin: %u. End: %u.", ::llc::string(viewExpression).begin(), ::llc::get_value_namep(typeExpression.Type), typeExpression.ParentIndex, typeExpression.Span.Begin, typeExpression.Span.End);
		}
	}
	::llc::vcst_t					evaluated								= {};
	::llc::SJSONExpressionSolver	pendingResults							= {};
	int32_t						jsonNodeResultOfEvaluation				= -1;
	if(reader.Tree.size()) {
#if defined(LLC_JSON_EXPRESSION_DEBUG)
		::printNode(reader.Tree[0], reader.View[0]);
#endif
		jsonNodeResultOfEvaluation	= ::evaluateExpression(pendingResults, reader, 0, inputJSON, indexNodeJSON, evaluated);
		if_true_fef(-1 == jsonNodeResultOfEvaluation, "Failed to evaluate expression: %s.", ::llc::string(reader.View[0]).begin());
	}
	if(evaluated.size()) {
		llc_jexpr_info_printf("Result of expression evaluation: %s.", ::llc::string(evaluated).begin());
	}
	if_fail_fe(results.Results.reserve(results.Results.size() + pendingResults.Results.size()));
	for(::llc::u2_t iResult = 0; iResult < pendingResults.Results.size(); ++iResult)
		if_fail_fe(results.Results.push_back(pendingResults.Results[iResult]));
	output												= evaluated;
	return jsonNodeResultOfEvaluation;
}

::llc::error_t			llc::jsonExpressionResolve				(::llc::vcst_t expression, const ::llc::SJSONReader & inputJSON, ::llc::u2_t indexNodeJSON, ::llc::vcst_t & output)								{
	::llc::SExpressionReader	reader;
	llc_necall(llc::expressionReaderParse(reader, expression), "Failed to read JSONeN expression: '%s'.", ::llc::string(expression).begin());
	return ::llc::jsonExpressionResolve(reader, inputJSON, indexNodeJSON, output);
}

::llc::error_t			llc::jsonExpressionResolve				(const ::llc::SExpressionReader & reader, const ::llc::SJSONReader & inputJSON, ::llc::u2_t indexNodeJSON, ::llc::vcst_t & output){
	llc::SJSONExpressionSolver	results;
	::llc::error_t				jsonNodeResultOfEvaluation;
	jsonNodeResultOfEvaluation = ::llc::jsonExpressionResolve(reader, inputJSON, indexNodeJSON, results, output);
	if_true_fef(-1 == jsonNodeResultOfEvaluation, "%s", "Failed to resolve JSON expression.");
	for(::llc::u2_t iResult = 0; iResult < results.Results.size(); ++iResult)
		llc_jexpr_info_printf("Result %u: '%s' | '%s'", iResult, ::llc::string(results.Results[iResult].Expression).begin(), ::llc::string(results.Results[iResult].Output).begin());
	return jsonNodeResultOfEvaluation;
}

::llc::error_t			llc::jsonStringFormat					(::llc::vcst_t format, const ::llc::SJSONReader& inputJSON, ::llc::u2_t indexNodeJSON, ::llc::asc_t & output)					{
	rtrn ::llc::expressionStringFormat(format, [&](::llc::vcst_c & expression, ::llc::vcst_t & resolved) {
		rtrn ::llc::jsonExpressionResolve(expression, inputJSON, indexNodeJSON, resolved);
	}, output);
}
