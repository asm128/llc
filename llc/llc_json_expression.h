#include "llc_json.h"
#include "llc_expression.h"

#ifndef LLC_JSON_EXPRESSION_9238749283744
#define LLC_JSON_EXPRESSION_9238749283744

namespace llc
{
	stxp	vcst_c			strEmptyObj				= "{}";
	stxp	vcst_c			strEmptyArr				= "[]";
	stxp	vcst_c			strNull					= "null";
	stxp	vcst_c			strBool		[2]			= {"false", "true"};
	stxp	vcst_c			strZero					= "0";
	struct SEvaluationStepResult {
		u2_t					IndexRootJSONNode		;
		s2_t					IndexJSONResult			;
		u3_t					LastValue				;
		vcst_t					Output					;
		vcst_t					Expression				;
		bool					LastValueCarry			;

		error_t					SetBoolCarry			(bool evalResult, vcst_t & output)	{
			IndexJSONResult			= -5;
			LastValueCarry			= true	;
			LastValue				= evalResult ? 1 : 0;
			output					= strBool[LastValue];
			return 0;
		}
	};

	struct SJSONExpressionSolver {
		aobj<SEvaluationStepResult>	Results;
	};

			error_t		jsonExpressionResolve		(const SExpressionReader & reader, const SJSONReader & inputJSON, u2_t indexNodeJSON, SJSONExpressionSolver & results, vcst_t & output);
			error_t		jsonExpressionResolve		(const SExpressionReader & reader, const SJSONReader & inputJSON, u2_t indexNodeJSON, vcst_t & output);
	stin	err_t		jsonExpressionResolve		(const SExpressionReader & reader, const SJSONReader & inputJSON, u2_t indexNodeJSON)	{ vcst_t dead; return jsonExpressionResolve(reader, inputJSON, indexNodeJSON, dead); }

			error_t		jsonExpressionResolve		(vcst_t expression, const SJSONReader & inputJSON, u2_t indexNodeJSON, vcst_t & output);
	stin	err_t		jsonExpressionResolve		(vcst_t expression, const SJSONReader & inputJSON, u2_t indexNodeJSON) { vcst_t dead; return jsonExpressionResolve(expression, inputJSON, indexNodeJSON, dead); }

			error_t		jsonStringFormat			(vcst_t format, const SJSONReader & inputJSON, u2_t indexNodeJSON, asc_t & output);
} // namespace

#endif // LLC_JSON_EXPRESSION_9238749283744
