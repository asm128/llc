#include "llc_parse.h"
#include "llc_array_pod.h"
#include "llc_safe.h"
#include "llc_math.h"
#include <ctype.h>

bool					llc::isSpaceCharacter						(const char characterToTest)		{
	switch(characterToTest) {
	case ' ': case '\t': case '\r': case '\n'	: case '\f'	: case '\b'	:
		return true;
	default:
		return false;
	}
}

::llc::error_t			llc::skipToNextCharacter					(u2_t & indexCurrentChar, ::llc::vcsc_c & expression)		{
	while(indexCurrentChar < expression.size()) {
		if(::llc::isSpaceCharacter(expression[indexCurrentChar]))
			++indexCurrentChar;
		else
			break;
	}
	return 0;
}

::llc::error_t			llc::parseArbitraryBaseInteger				(u2_t base, ::llc::vcsc_c & symbolList, ::llc::vcsc_c & sourceChars, u3_t * number_)	{
	u2_t										totalCharsProcessed							= 0;
	::llc::apod<char>								stringToParse								= {};
	for(u2_t iChar = 0; iChar < sourceChars.size() && 0 != sourceChars[iChar];) {
		const char										sourceChar									= (char)tolower(sourceChars[iChar]);
		bool											bSymbolProcessed							= false;
		for(u2_t iSymbol = 0; iSymbol < base; ++iSymbol) {
			if(symbolList[iSymbol] == sourceChar) {
				bSymbolProcessed							= true;
				llc_necs(stringToParse.push_back(sourceChar));
				++iChar;
				++totalCharsProcessed;
				break;
			}
		}

		if(!bSymbolProcessed)
			break;	// number ends with any character that is not a symbol
	}
	llc_necs(llc::reverse(stringToParse));		// we assigned the digits backwards so we need to reverse the string.
	u3_t										number										= 0;
	totalCharsProcessed							= 0;
	for(u2_t iChar = 0; iChar < stringToParse.size() && 0 != stringToParse[iChar];) {
		bool											bSymbolProcessed							= false;
		for( u2_t iSymbol = 0; iSymbol < base; ++iSymbol )
			if(symbolList[iSymbol] == stringToParse[iChar]) {
				number										+= iSymbol * ::llc::powui(base, totalCharsProcessed);
				bSymbolProcessed							= true;
				++iChar;
				++totalCharsProcessed;
				break;
			}

		if(!bSymbolProcessed)
			break;	// number ends with any character that is not a symbol
	}
	llc_safe_assign(number_, number);
	return totalCharsProcessed;
}

::llc::error_t			stripLiteralsParseToken		(::llc::SStripLiteralState & work_state, ::llc::apod<::llc::SStripLiteralType> & out_types, ::llc::vcsc_c & in_format)		{
	(void)in_format;
	switch(work_state.CharCurrent) {
	default		: break;
	case '{'	:
		++work_state.BracketsToSkip;
		break;
	case '}'	:
		if(work_state.Escaping)
			break;	// do nothing if this bracket is escaped
		if(0 < work_state.BracketsToSkip) {
			--work_state.BracketsToSkip;
			break;
		}
		out_types[work_state.IndexCurrentElement].Span.End	= work_state.IndexCurrentChar + 1;
		work_state.InsideToken						= false;
		llc_necs(work_state.IndexCurrentElement = out_types.push_back({-1, ::llc::STRIP_LITERAL_TYPE_LITERAL, {work_state.IndexCurrentChar + 1, work_state.IndexCurrentChar + 1}}));
		work_state.CurrentElement					= &out_types[work_state.IndexCurrentElement];
	case '\\'	:
		if(false == work_state.Escaping) {
			work_state.Escaping							= true;
			return 0;
		}
		break;
	}
	work_state.Escaping							= false;
	if(work_state.IndexCurrentChar == in_format.size() - 1) { // if this is the last character, make sure to close open key and root expression
		++work_state.IndexCurrentChar;
		out_types[out_types.size() - 1].Span.End	= work_state.IndexCurrentChar;
		work_state.InsideToken						= false;
	}
	return 0;
}

::llc::error_t			stripLiteralsParseLiteral	(::llc::SStripLiteralState & work_state, ::llc::apod<::llc::SStripLiteralType> & out_types, ::llc::vcsc_c & in_format)		{
	(void)in_format;
	switch(work_state.CharCurrent) {
	default		: break;
	case '{'	:
		if(work_state.Escaping)
			break;	// do nothing if this bracket is escaped
		out_types[out_types.size() - 1].Span.End	= work_state.IndexCurrentChar;
		work_state.InsideToken						= true;
		llc_necs(work_state.IndexCurrentElement = out_types.push_back({-1, ::llc::STRIP_LITERAL_TYPE_TOKEN, {work_state.IndexCurrentChar, work_state.IndexCurrentChar}}));
		work_state.CurrentElement					= &out_types[work_state.IndexCurrentElement];
		// do work
		break;
	case '\\'	:
		if(false == work_state.Escaping) {
			work_state.Escaping							= true;
			return 0;
		}
		break;
	}
	work_state.Escaping							= false;
	if(work_state.IndexCurrentChar == in_format.size() - 1) { // if this is the last character, make sure to close open key and root expression
		++work_state.IndexCurrentChar;
		out_types[out_types.size() - 1].Span.End	= work_state.IndexCurrentChar;
	}
	return 0;
}

::llc::error_t			llc::stripLiteralParseStep		(::llc::SStripLiteralState & work_state, ::llc::apod<::llc::SStripLiteralType> & out_types, ::llc::vcsc_c & in_format)		{
	if(work_state.InsideToken)
		return ::stripLiteralsParseToken(work_state, out_types, in_format);

	if(0 == out_types.size()) {	// The format string always start from a literal.
		work_state.IndexCurrentElement				= out_types.push_back({-1, ::llc::STRIP_LITERAL_TYPE_LITERAL, {work_state.IndexCurrentChar, work_state.IndexCurrentChar}});
		work_state.CurrentElement					= &out_types[work_state.IndexCurrentElement];
	}
	return ::stripLiteralsParseLiteral(work_state, out_types, in_format);
}

::llc::error_t			llc::stripLiteralParse			(::llc::SStripLiteralState & stateReading, ::llc::apod<::llc::SStripLiteralType> & out_types, ::llc::vcsc_c & in_format)		{
	for(stateReading.IndexCurrentChar = 0; stateReading.IndexCurrentChar < in_format.size(); ++stateReading.IndexCurrentChar) {
		stateReading.CharCurrent					= in_format[stateReading.IndexCurrentChar];
		llc_necs(llc::stripLiteralParseStep(stateReading, out_types, in_format));
	}
	return 0;
}

::llc::error_t			llc::stripLiteralGetViews		(::llc::aobj<::llc::vcsc_t> & out_views, const ::llc::view<const ::llc::SStripLiteralType> & in_resultOfParser, ::llc::vcsc_c & in_format)		{
	for(u2_t iType = 0; iType < in_resultOfParser.size(); ++iType) {
		const ::llc::SStripLiteralType					& type						= in_resultOfParser[iType];
		::llc::vcst_t						view						= {};
		u2_t										offsetView					= 0;
		u2_t										lenView						= 0;
		if(type.Type == ::llc::STRIP_LITERAL_TYPE_TOKEN) {
			offsetView									= ::llc::min(in_format.size() - 1, type.Span.Begin + 1);
			lenView										= type.Span.End - type.Span.Begin - 2;
		}
		else if(type.Type == ::llc::STRIP_LITERAL_TYPE_LITERAL) {
			offsetView									= ::llc::min(in_format.size() - 1, type.Span.Begin);
			lenView										= ::llc::min(in_format.size() - type.Span.Begin, type.Span.End - type.Span.Begin);
		}
		if(0 >= lenView)
			continue;
		view										= {&in_format[offsetView], lenView};
		llc_necs(out_views.push_back(view));
	}
	return 0;
}
