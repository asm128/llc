#include "llc_enum.h"
#include "llc_reader.h"
#include "llc_array_ptr.h"
#include "llc_slice.h"
#include "llc_parse.h"
#include "llc_functional.h"

#ifndef LLC_EXPRESSION_H_23627
#define LLC_EXPRESSION_H_23627

namespace llc
{
	GDEFINE_ENUM_TYPE (EXPRESSION_READER_TYPE, int8_t);
	GDEFINE_ENUM_VALUE(EXPRESSION_READER_TYPE, TERM_ANY		,  0);
	GDEFINE_ENUM_VALUE(EXPRESSION_READER_TYPE, TERM_KEY		,  1);
	GDEFINE_ENUM_VALUE(EXPRESSION_READER_TYPE, TERM_INDEX	,  2);
	GDEFINE_ENUM_VALUE(EXPRESSION_READER_TYPE, TERM_BOOL	,  3);
	GDEFINE_ENUM_VALUE(EXPRESSION_READER_TYPE, TERM_AND		,  4);
	GDEFINE_ENUM_VALUE(EXPRESSION_READER_TYPE, TERM_OR		,  5);
	GDEFINE_ENUM_VALUE(EXPRESSION_READER_TYPE, TERM_EQUALS	,  6);
	GDEFINE_ENUM_VALUE(EXPRESSION_READER_TYPE, KEY			,  7);
	GDEFINE_ENUM_VALUE(EXPRESSION_READER_TYPE, LITERAL		,  8);
	GDEFINE_ENUM_VALUE(EXPRESSION_READER_TYPE, UNARY_NOT	,  9);
	GDEFINE_ENUM_VALUE(EXPRESSION_READER_TYPE, CODEPOINT	, 10);
	GDEFINE_ENUM_VALUE(EXPRESSION_READER_TYPE, UNKNOWN		, -1);

#pragma pack(push, 1)
	tydf SToken<EXPRESSION_READER_TYPE> SExpressionToken;
	struct SExpressionTokenData {
		bool					ClosingCondition				= false;
	};

	struct SExpressionReaderState : public SReaderState<EXPRESSION_READER_TYPE> {
		sc_t					ClosingQuotes					= '0';
	};
#pragma pack(pop)

	struct SExpressionNode {
		SExpressionToken		* Token							= 0;
		SExpressionNode			* Parent						= 0;
		apobj<SExpressionNode>	Children						= {};
		int32_t					ObjectIndex						= -1;
	};

	struct SExpressionReader : public SReader<EXPRESSION_READER_TYPE> {
		apod<SExpressionTokenData>	Data;
		aobj<vcst_t>			View;
		apobj<SExpressionNode>	Tree;
		SExpressionReaderState	StateRead;
		pobj<SExpressionReader>	SubExpressionCache;
	};

	err_t					expressionReaderParse			(SExpressionReader & reader, vcst_t expression);
	err_t					expressionReaderParseStep		(SExpressionReader & reader, vcst_t expression);

	// Token spans refer to format. Resolved views must survive the callback until the formatter copies them.
	err_t					expressionStringFormat		(vcst_t format, view<cnst SStripLiteralType> tokens, cnst FError<vcst_c &, vcst_t &> & resolve, asc_t & output);
	err_t					expressionStringFormat		(vcst_t format, cnst FError<vcst_c &, vcst_t &> & resolve, asc_t & output);
} // namespace

#endif // LLC_EXPRESSION_H_23627
