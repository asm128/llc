#include "llc_parse.h"
#include "llc_array_pod.h"
#include "llc_safe.h"
#include "llc_math.h"
#include <ctype.h>

bool				llc::isSpaceCharacter			(const char characterToTest)		{
	switch(characterToTest) {
	case ' ': case '\t': case '\r': case '\n'	: case '\f'	: case '\b'	:
		return true;
	default:
		return false;
	}
}
llc::err_t			llc::skipToNextCharacter		(u2_t & indexCurrentChar, vcst_t expression)		{
	while(indexCurrentChar < expression.size()) {
		if(isSpaceCharacter(expression[indexCurrentChar]))
			++indexCurrentChar;
		else
			break;
	}
	return 0;
}
llc::err_t			llc::parseArbitraryBaseInteger	(u2_t base, vcst_t symbolList, vcst_t sourceChars, u3_t * number_)	{
	u2_t					totalCharsProcessed							= 0;
	string					stringToParse								= {};
	for(u2_t iChar = 0; iChar < sourceChars.size() && 0 != sourceChars[iChar];) {
		const char				sourceChar									= (char)tolower(sourceChars[iChar]);
		bool					bSymbolProcessed							= false;
		for(u2_t iSymbol = 0; iSymbol < base; ++iSymbol) {
			if(symbolList[iSymbol] == sourceChar) {
				bSymbolProcessed							= true;
				if_fail_fe(stringToParse.push_back(sourceChar));
				++iChar;
				++totalCharsProcessed;
				break;
			}
		}

		if(!bSymbolProcessed)
			break;	// number ends with any character that is not a symbol
	}
	if_fail_fe(reverse(stringToParse));		// we assigned the digits backwards so we need to reverse the string.
	u3_t					number										= 0;
	totalCharsProcessed	= 0;
	for(u2_t iChar = 0; iChar < stringToParse.size() && 0 != stringToParse[iChar];) {
		bool											bSymbolProcessed							= false;
		for( u2_t iSymbol = 0; iSymbol < base; ++iSymbol )
			if(symbolList[iSymbol] == stringToParse[iChar]) {
				number										+= iSymbol * powui(base, totalCharsProcessed);
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
llc::err_t			stripLiteralsParseToken			(::llc::SStripLiteralState & work_state, ::llc::apod<::llc::SStripLiteralType> & out_types, ::llc::vcst_t in_format)		{
	(void)in_format;
	::llc::SReaderState<::llc::STRIP_LITERAL_TYPE> & readerState = work_state.ReaderState;
	switch(readerState.CharCurrent) {
	default		: break;
	case '{'	:
		++work_state.BracketsToSkip;
		break;
	case '}'	:
		if(readerState.Flags.Escaping)
			break;	// do nothing if this bracket is escaped
		if(0 < work_state.BracketsToSkip) {
			--work_state.BracketsToSkip;
			break;
		}
		out_types[readerState.IndexCurrentElement].Span.End	= readerState.IndexCurrentChar + 1;
		work_state.InsideToken						= false;
		if_fail_fe(readerState.IndexCurrentElement = out_types.push_back({-1, ::llc::STRIP_LITERAL_TYPE_LITERAL, false, {readerState.IndexCurrentChar + 1, readerState.IndexCurrentChar + 1}}));
		readerState.CurrentElement					= &out_types[readerState.IndexCurrentElement];
		break;
	case '\\'	:
		if(false == readerState.Flags.Escaping) {
			readerState.Flags.Escaping							= true;
			return 0;
		}
		break;
	}
	readerState.Flags.Escaping							= false;
	if(readerState.IndexCurrentChar == in_format.size() - 1) { // if this is the last character, make sure to close open key and root expression
		++readerState.IndexCurrentChar;
		out_types[out_types.size() - 1].Span.End	= readerState.IndexCurrentChar;
		work_state.InsideToken						= false;
	}
	return 0;
}
llc::err_t			stripLiteralsParseLiteral		(::llc::SStripLiteralState & work_state, ::llc::apod<::llc::SStripLiteralType> & out_types, ::llc::vcst_t in_format)		{
	(void)in_format;
	::llc::SReaderState<::llc::STRIP_LITERAL_TYPE> & readerState = work_state.ReaderState;
	switch(readerState.CharCurrent) {
	default		: break;
	case '{'	:
		if(readerState.Flags.Escaping)
			break;	// do nothing if this bracket is escaped
		out_types[out_types.size() - 1].Span.End	= readerState.IndexCurrentChar;
		work_state.InsideToken						= true;
		if_fail_fe(readerState.IndexCurrentElement = out_types.push_back({-1, ::llc::STRIP_LITERAL_TYPE_TOKEN, false, {readerState.IndexCurrentChar, readerState.IndexCurrentChar}}));
		readerState.CurrentElement					= &out_types[readerState.IndexCurrentElement];
		// do work
		break;
	case '\\'	:
		if(false == readerState.Flags.Escaping) {
			readerState.Flags.Escaping							= true;
			return 0;
		}
		break;
	}
	readerState.Flags.Escaping							= false;
	if(readerState.IndexCurrentChar == in_format.size() - 1) { // if this is the last character, make sure to close open key and root expression
		++readerState.IndexCurrentChar;
		out_types[out_types.size() - 1].Span.End	= readerState.IndexCurrentChar;
	}
	return 0;
}
llc::err_t			llc::stripLiteralParseStep		(SStripLiteralState & stripLiteralState, apod<SStripLiteralType> & out_types, vcst_t in_format)		{
	if_fail_fe(::llc::readerStateParseStep<STRIP_LITERAL_TYPE>(stripLiteralState.ReaderState, out_types, in_format, [&stripLiteralState](SReaderState<STRIP_LITERAL_TYPE> & readerState, apod<SStripLiteralType> & outputTokens, const vcst_t & documentCharacters) {
			if(stripLiteralState.InsideToken) {
				if_fail_fe(::stripLiteralsParseToken(stripLiteralState, outputTokens, documentCharacters));
				rtn0;
			}
			if(0 == outputTokens.size()) {	// The format string always start from a literal.
				readerState.IndexCurrentElement				= outputTokens.push_back({-1, STRIP_LITERAL_TYPE_LITERAL, false, {readerState.IndexCurrentChar, readerState.IndexCurrentChar}});
				readerState.CurrentElement					= &outputTokens[readerState.IndexCurrentElement];
			}
			if_fail_fe(::stripLiteralsParseLiteral(stripLiteralState, outputTokens, documentCharacters));
			rtn0;
		}
	));
	return 0;
}
llc::err_t			llc::stripLiteralParse			(SStripLiteralState & stateReading, apod<SStripLiteralType> & out_types, vcst_t in_format)		{
	for(stateReading.ReaderState.IndexCurrentChar = 0; stateReading.ReaderState.IndexCurrentChar < in_format.size(); ++stateReading.ReaderState.IndexCurrentChar) {
		if_fail_fe(llc::stripLiteralParseStep(stateReading, out_types, in_format));
	}
	return 0;
}
llc::err_t			llc::stripLiteralGetViews		(aobj<vcst_t> & out_views, const view<const SStripLiteralType> & in_resultOfParser, vcst_t in_format)		{
	for(u2_t iType = 0; iType < in_resultOfParser.size(); ++iType) {
		const SStripLiteralType					& type						= in_resultOfParser[iType];
		vcst_t						view						= {};
		u2_t										offsetView					= 0;
		u2_t										lenView						= 0;
		if(type.Type == STRIP_LITERAL_TYPE_TOKEN) {
			offsetView									= min(in_format.size() - 1, type.Span.Begin + 1);
			lenView										= type.Span.End - type.Span.Begin - 2;
		}
		else if(type.Type == STRIP_LITERAL_TYPE_LITERAL) {
			offsetView									= min(in_format.size() - 1, type.Span.Begin);
			lenView										= min(in_format.size() - type.Span.Begin, type.Span.End - type.Span.Begin);
		}
		if(0 >= lenView)
			continue;
		view										= {&in_format[offsetView], lenView};
		if_fail_fe(out_views.push_back(view));
	}
	return 0;
}
