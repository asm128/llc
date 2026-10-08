#ifndef LLC_DISABLE_DEBUG_BREAK_ON_ERROR_LOG
#	define LLC_DISABLE_DEBUG_BREAK_ON_ERROR_LOG
#endif

#include "llc_json.h"

#include "llc_file.h"
#include "llc_parse.h"
#include "llc_math.h"
#include "llc_cstring.h"

LLC_USING_TYPEINT();

#define json_info_printf(...) // info_printf
#define json_error_printf error_printf

llc::err_t			llc::jsonCompareObject		(cnst llc::SJSONNode & node, llc::view<cnst vcst_t> views, cnst llc::SJSONNode & other, llc::view<cnst vcst_t> otherViews) { if(node.Children.size() != other.Children.size()) rtrn 0; if(node.Children.size() == 0) rtrn 1; for(u2_t iChild = 0; iChild < node.Children.size(); iChild += 2)	if(node.Token->Type != other.Token->Type) rtrn 0; rtrn (views[node.ObjectIndex] == otherViews[other.ObjectIndex]) ? 1 : 0; }
llc::err_t			llc::jsonCompareNumber		(cnst llc::SJSONNode & node, llc::view<cnst vcst_t> views, cnst llc::SJSONNode & other, llc::view<cnst vcst_t> otherViews) { if(node.Children.size() != other.Children.size()) rtrn 0; if(node.Children.size() == 0) rtrn 1; if(node.Token->Type != other.Token->Type) rtrn 0; rtrn (views[node.ObjectIndex] == otherViews[other.ObjectIndex]) ? 1 : 0; }

llc::err_t			llc::jsonMapToFields
(	llc::as2_t											& indicesOfFields
,	llc::view<cnst llc::SJSONFieldBinding>	fields
,	llc::view<cnst llc::TKeyValConstChar>	fieldMaps
) {
	for(u2_t iMap = 0; iMap < fieldMaps.size(); ++iMap)
	for(u2_t iField = 0; iField < fields.size(); ++iField) {
		cnst llc::SJSONFieldBinding	& fieldToAdd			= fields[iField];
		if(fieldToAdd.Field.size() && fieldToAdd.Field == fieldMaps[iMap].Key) {
			if_fail_fe(indicesOfFields.push_back(iField));
			break;
		}
	}
	rtrn indicesOfFields.size();
}

llc::err_t			llc::jsonFieldsToMap
(	llc::as2_t											& indicesOfMaps
,	llc::view<cnst llc::SJSONFieldBinding>	fields
,	llc::view<cnst llc::TKeyValConstChar>	fieldMaps
) {
	u2_t					countFields					= fields.size();
	for(u2_t iField = 0; iField < countFields; ++iField) {
		b8_t						bNotAdded					= true;
		for(u2_t iMap = 0; iMap < fieldMaps.size(); ++iMap) {
			cnst llc::SJSONFieldBinding	& fieldToAdd			= fields[iField];
			if(fieldToAdd.Field.size() && fieldToAdd.Field == fieldMaps[iMap].Key) {
				if_fail_fe(indicesOfMaps.push_back(iMap));
				bNotAdded				= false;
				break;
			}
		}
		if(bNotAdded)
			if_fail_fe(indicesOfMaps.push_back(-1));
	}
	rtrn indicesOfMaps.size();
}

llc::err_t			llc::jsonFileRead			(llc::SJSONFile & file, llc::vcst_t filename) {
	json_info_printf("Loading json file: %.*s.", (int)filename.size(), filename.begin());
	if_fail_fef(llc::fileToMemory(filename, file.Bytes), "Failed to load file: '%.*s'", (int)filename.size(), filename.begin());
	rtrn llc::jsonParse(file.Reader, file.Bytes);
}

llc::err_t			llc::jsonArraySplit			(cnst llc::SJSONNode & jsonArrayToSplit, llc::view<cnst vcst_t> jsonViews, u2_c blockSize, llc::aobj<llc::string> & outputJsons)		{
	u2_c				remainder					= jsonArrayToSplit.Children.size() % blockSize;
	u2_c				countParts					= jsonArrayToSplit.Children.size() / blockSize + one_if(remainder);
	if_fail_fe(outputJsons.resize(countParts));
	u2_t					iSourceRecord						= 0;
	for(u2_t iPart = 0; iPart < outputJsons.size(); ++iPart) {
		llc::string			& outputJson				= outputJsons[iPart];
		if_fail_fe(outputJson.push_back('['));
		for(u2_t iPartRecord = 0, countPartRecords = (remainder && iPart == countParts - 1) ? remainder : blockSize
			; iPartRecord < countPartRecords
			; ++iPartRecord) {
			if_fail_fe(llc::jsonWrite(jsonArrayToSplit.Children[iSourceRecord++], jsonViews, outputJson));;
			if(iPartRecord < countPartRecords - 1)
				if_fail_fe(outputJson.push_back(','));
		}
		if_fail_fe(outputJson.push_back(']'));
	}
	rtrn 0;
}

llc::err_t			llc::jsonWrite				(cnst llc::SJSONNode* node, llc::view<cnst vcst_t> jsonViews, llc::string & output)			{
	if(node->Token->Type == llc::JSON_TYPE_VALUE && node->Children.size())
		node				= node->Children[0];
	switch(node->Token->Type) {
	case llc::JSON_TYPE_INTEGER		: {
		sc_t					temp[64]					= {};
		if_fail_fe(::llc::sprintf_s(temp, "%" LLC_FMT_S3, (s3_t)node->Token->Value));
		if_fail_fe(output.append_string(temp));
	}
		break;
	case llc::JSON_TYPE_DECIMAL		: {
		f3_t					f							= 0;
		memcpy(&f, &node->Token->Value, szof(f3_t));
		sc_t					temp[64]					= {};
		err_t					lenNum						= ::llc::sprintf_s(temp, "%.16f", f);
		if_fail_fe(lenNum);
		if_true_fef(lenNum >= (err_t)::llc::size(temp), "JSON decimal exceeds the formatting buffer: %.16f.", f);
		while(lenNum > 0 && (0 == temp[lenNum] || '0' == temp[lenNum]))
			temp[lenNum--]					= 0;
		if_fail_fe(output.append_string(temp));
	}
		break;
	case llc::JSON_TYPE_BOOLEAN			:
		if_fail_fe(output.append(bool2vcc(node->Token->Value)));
		break;
	case llc::JSON_TYPE_NULL			:
		if_fail_fe(output.append(jsonViews[node->ObjectIndex]));
		break;
	case llc::JSON_TYPE_STRING		:
		if_fail_fe(output.push_back('"'));
		if_fail_fe(output.append(jsonViews[node->ObjectIndex]));
		if_fail_fe(output.push_back('"'));
		break;
	case llc::JSON_TYPE_OBJECT		:
		if_fail_fe(output.push_back('{'));
		for(u2_t iChildren = 0; iChildren < node->Children.size(); iChildren += 2) {
			if_fail_fe(llc::jsonWrite(node->Children[iChildren + 0], jsonViews, output));;
			if_fail_fe(output.push_back(':'));
			if_fail_fe(llc::jsonWrite(node->Children[iChildren + 1], jsonViews, output));;
			if(iChildren < node->Children.size() - 2)
				if_fail_fe(output.push_back(','));
		}
		if_fail_fe(output.push_back('}'));
		break;
	case llc::JSON_TYPE_ARRAY			:
		if_fail_fe(output.push_back('['));
		for(u2_t iChildren = 0; iChildren < node->Children.size(); ++iChildren) {
			if_fail_fe(llc::jsonWrite(node->Children[iChildren], jsonViews, output));;
			if(iChildren < node->Children.size() - 1)
				if_fail_fe(output.push_back(','));
		}
		if_fail_fe(output.push_back(']'));
	}
	rtrn 0;
}

sttc	llc::err_t	jsonCloseElement			(llc::SJSONReaderState & stateReader, llc::apod<llc::SJSONToken> & tokens, u2_t indexChar) {
	ree_if(tokens.size() <= (u2_t)stateReader.IndexCurrentElement, "Invalid parser state. Cannot close element: %" LLC_FMT_S2 ".", stateReader.IndexCurrentElement);
	llc::SJSONToken			* closing					= 0;
	closing					= stateReader.CurrentElement; //&object[stateReader.IndexCurrentElement];
	closing->Span.End		= (u2_t)indexChar + 1;
	llc::vcst_t			labelType				= llc::get_value_label(closing->Type);
	json_info_printf("%.*s closed. Index %.2i. Level: %" LLC_FMT_S2 ". Parent index: %" LLC_FMT_S2 ". Node type: %" LLC_FMT_S2 ". Begin: %" LLC_FMT_S2 ". End: %" LLC_FMT_S2 ".", (int)labelType.size(), labelType.begin(), stateReader.IndexCurrentElement, stateReader.NestLevel, closing->ParentIndex, closing->Type, closing->Span.Begin, closing->Span.End);
	stateReader.IndexCurrentElement					= closing->ParentIndex;
	--stateReader.NestLevel;
	stateReader.CurrentElement						= ((u2_t)stateReader.IndexCurrentElement < tokens.size()) ? &tokens[stateReader.IndexCurrentElement] : 0;
	if(0 == stateReader.NestLevel)
		stateReader.DoneReading							= true;
	rtrn 0;	// Need to report that a list has been exited
}

sttc	llc::err_t	jsonCloseElement			(llc::SJSONReaderState & stateReader, llc::apod<llc::SJSONToken> & object, u2_t indexChar, llc::JSON_TYPE jsonType) {
	cnst llc::SJSONToken		* open						= stateReader.CurrentElement; //&object[stateReader.IndexCurrentElement];
	cnst llc::vcst_t			labelOpen					= llc::get_value_label(open->Type);
	cnst llc::vcst_t			labelClose				= llc::get_value_label(jsonType);
	ree_if(jsonType != open->Type, "Invalid object type: open: %" LLC_FMT_U2 " (%.*s). closing: %" LLC_FMT_U2 " (%.*s).", open->Type, (int)labelOpen.size(), labelOpen.begin(), jsonType, (int)labelClose.size(), labelClose.begin());
	rtrn ::jsonCloseElement(stateReader, object, indexChar);
}

sttc	llc::err_t	jsonTestAndCloseValue		(llc::SJSONReaderState & stateReader, llc::apod<llc::SJSONToken> & tokens) {
	if_null_fe(stateReader.CurrentElement);
	if_true_vv(1, llc::JSON_TYPE_VALUE != stateReader.CurrentElement->Type);
	llc::err_c			result			= ::jsonCloseElement(stateReader, tokens, stateReader.IndexCurrentChar);
	stateReader.ExpectingSeparator	= true;
	if(stateReader.CurrentElement) {
		llc::SJSONToken		& element		= *stateReader.CurrentElement;
		if(llc::JSON_TYPE_ARRAY  == element.Type
		|| llc::JSON_TYPE_OBJECT == element.Type
		) ++element.Value;
	}
	rtrn result;
}

sttc	llc::err_t	jsonTestAndCloseKey			(llc::SJSONReaderState & stateReader, llc::apod<llc::SJSONToken> & tokens) {
	if_null_fe(stateReader.CurrentElement);
	if_true_vv(1, llc::JSON_TYPE_KEY != stateReader.CurrentElement->Type);
	stateReader.ExpectingSeparator	= true;	// actually we expect the separator AFTER calling jsonCloseElement(). However, such function doesn't care about this value, so we can simplify the code by reversing the operations.
	rtrn ::jsonCloseElement(stateReader, tokens, stateReader.IndexCurrentChar);
}

sttc	llc::err_t	jsonCloseOrDiscardEmptyKOrV	(llc::SJSONReaderState & stateReader, llc::apod<llc::SJSONToken> & tokens, llc::JSON_TYPE containerType) {
	llc::err_t				errVal						= 0;
	cnst llc::vcst_t			labelType				= llc::get_value_label(containerType);
	if(tokens[tokens.size() - 1].Type == containerType) {
		json_info_printf("Discarding empty container element at index %" LLC_FMT_S2 " (%.*s). Level: %" LLC_FMT_S2 "", tokens.size() - 1, (int)labelType.size(), labelType.begin(), stateReader.NestLevel);
		stateReader.IndexCurrentElement	= tokens[tokens.size() - 1].ParentIndex;
		if_fail_fe(tokens.pop_back());
		--stateReader.NestLevel;
		if((u2_t)stateReader.IndexCurrentElement < tokens.size())
			stateReader.CurrentElement	= &tokens[stateReader.IndexCurrentElement];
	}
	else {
		json_info_printf("Closing container at index %" LLC_FMT_S2 " (%.*s).", tokens.size() - 1, (int)labelType.size(), labelType.begin());
			 if(llc::JSON_TYPE_VALUE	== containerType) errVal = ::jsonTestAndCloseValue	(stateReader, tokens);
		else if(llc::JSON_TYPE_KEY		== containerType) errVal = ::jsonTestAndCloseKey	(stateReader, tokens);
	}
	rtrn errVal;
}

#define seterr_break_if(condition, format, ...)	\
	if(condition) {								\
		errVal					= -1;			\
		json_error_printf(format, __VA_ARGS__);	\
		break;									\
	}

#define seterr_if(condition, format, ...)	\
	if(condition) {								\
		errVal					= -1;			\
		json_error_printf(format, __VA_ARGS__);	\
	}
#define seterr_break_if_failed(condition, format, ...)	seterr_break_if	(llc::failed(condition), format, __VA_ARGS__)
#define seterr_if_failed(condition, format, ...)		seterr_if		(llc::failed(condition), format, __VA_ARGS__)

sttc	llc::err_t	jsonParseStringCharacter	(llc::SJSONReaderState & stateReader, llc::apod<llc::SJSONToken> & tokens, llc::vcst_t jsonAsString)	{
	llc::SJSONToken			currentElement				= {};
	llc::err_t				errVal						= 0;
	switch(stateReader.CharCurrent) {
	default:
		seterr_break_if(((llc::uc_t)stateReader.CharCurrent) < 0x20 || ((llc::uc_t)stateReader.CharCurrent) > 0xFE, "Invalid character: %" LLC_FMT_S2 " (%" LLC_FMT_U2 ") '%c'.", stateReader.CharCurrent, (llc::uc_t)stateReader.CharCurrent, stateReader.CharCurrent);
		seterr_break_if(stateReader.Escaping, "Cannot escape character: %" LLC_FMT_S2 " (%" LLC_FMT_U2 ") '%c'.", stateReader.CharCurrent, (llc::uc_t)stateReader.CharCurrent, stateReader.CharCurrent);
		break;
	case ' '	: case '\t'	: case '\r'	: case '\n'	: case '\f'	: case '\b'	: // Skip these characters without error.
		llc::skipToNextCharacter(stateReader.IndexCurrentChar, jsonAsString);
		--stateReader.IndexCurrentChar;
		break;
	case 'b': case 'f': case 'n': case 'r': case 't':
		break;	// these characters are both valid as part of the string and as escapable characters.
	case 'u':
		if(false == stateReader.Escaping)
			break;
		stateReader.IndexCurrentChar	+= 1;	// skip the u to get the next 4 digits.
		seterr_break_if(jsonAsString.size() - stateReader.IndexCurrentChar < 4, "End of stream during unicode code point parsing. JSON length: %.*s. Current index: %" LLC_FMT_U2 ".", (int)(jsonAsString.size() - stateReader.IndexCurrentChar), &jsonAsString[stateReader.IndexCurrentChar], stateReader.IndexCurrentChar);
		json_info_printf("Unicode code point found: %4.4s", &jsonAsString[stateReader.IndexCurrentChar]);
		currentElement		= {stateReader.IndexCurrentElement, llc::JSON_TYPE_CODEPOINT, {stateReader.IndexCurrentChar, stateReader.IndexCurrentChar + 4}, {}};
		seterr_if_failed(tokens.push_back(currentElement), "token count: %" LLC_FMT_S2 "", tokens.size());
		stateReader.CurrentElement		= &tokens[stateReader.IndexCurrentElement];
		stateReader.IndexCurrentChar	+= 3;	// Parse unicode code point
		break;
	case '\\'	:
		if(false == stateReader.Escaping) {
			stateReader.Escaping			= true;
			rtrn 0;
		}
		break;
	case '"'	:
		if(false == stateReader.Escaping) {
			seterr_if_failed(::jsonCloseElement(stateReader, tokens, stateReader.IndexCurrentChar - 1, llc::JSON_TYPE_STRING), "Failed to close string elment at character %" LLC_FMT_S2 "", stateReader.IndexCurrentChar - 1);//llc::get_value_namep(tokens));
			stateReader.InsideString		= false;
			::jsonTestAndCloseValue(stateReader, tokens);
			stateReader.ExpectingSeparator	= true;	// actually we expect the separator AFTER calling jsonCloseElement(). However, such function doesn't care about this value, so we can simplify the code by reversing the operations.
		}
	}
	stateReader.Escaping	= false;
	rtrn errVal;
}

sttc	llc::err_t	jsonParseKeyword			(llc::vcst_t token, llc::JSON_TYPE jsonType, llc::SJSONReaderState & stateReader, llc::apod<llc::SJSONToken> & object, llc::vcst_t jsonAsString)	{
	ree_if(token.size() > jsonAsString.size() - stateReader.IndexCurrentChar, "End of stream while parsing token: %.*s.", int(token.size()), token.begin());
	ree_if(0 != strncmp(token.begin(), &jsonAsString[stateReader.IndexCurrentChar], token.size()), "Unrecognized token found while looking for '%.*s'.", int(token.size()),	 token.begin());
	json_info_printf("JSON token found: %.*s.", int(token.size()), token.begin());

	llc::SJSONToken			boolElement					= {stateReader.IndexCurrentElement, jsonType, {stateReader.IndexCurrentChar, stateReader.IndexCurrentChar + token.size()}, llc::vcc2bool(token)};
	llc_necall(object.push_back(boolElement), "Failed to push! Out of memory? object count: %" LLC_FMT_U2 ".", object.size());
	stateReader.IndexCurrentChar	+= token.size() - 1;
	stateReader.CurrentElement		= &object[stateReader.IndexCurrentElement];
	rtrn 0;
}

sttc	llc::err_t	lengthJsonNumber			(u2_t indexCurrentChar, llc::vcst_t jsonAsString)	{
	u2_c				offset						= indexCurrentChar;
	sc_t						charCurrent					= jsonAsString[indexCurrentChar];
	while(indexCurrentChar < jsonAsString.size() &&
		( ( charCurrent >= '1' && charCurrent <= '9')
		 || charCurrent == '0'
		 || charCurrent == '.'
		 )
		) {
		++indexCurrentChar;
		if(indexCurrentChar >= jsonAsString.size())
			break;
		charCurrent				= jsonAsString[indexCurrentChar];
	}
	rtrn indexCurrentChar - offset;
}

//sttc	llc::err_t	parseJsonAbsolutePart		(llc::SJSONReaderState & stateReader, llc::apod<llc::SJSONToken> & tokens, vcst_t  absolutePart, llc::SJSONToken & output, u2_c sizeNum, const b8_t isFloat, u2_c index, const s2_t indexCurrentElement, const s2_t indexCurrentChar)	{
//	output					= {indexCurrentElement, isFloat ? llc::JSON_TYPE_DECIMAL : llc::JSON_TYPE_INTEGER, {(u2_t)indexCurrentChar, indexCurrentChar + sizeNum + (index - indexCurrentChar)}};
//	llc::err_t				intCount					= llc::parseIntegerDecimal(absolutePart, output.Value);
//	if(intCount < (s2_t)sizeNum) {
//		output.Type		= llc::JSON_TYPE_DECIMAL;
//		f3_t						finalValue					= (f3_t)output.Value;
//		json_info_printf("Integer part: %f.", finalValue);
//		++intCount;	// Skip dot.
//		u2_c				offsetStart					= output.Span.Begin + sizeNum - (sizeNum - intCount);
//		u2_c				lenDec						= sizeNum - intCount;
//		if(lenDec) {
//			f3_t						decValue					= 0;
//			if(absolutePart[0] == '.')
//				if_fail_fe(absolutePart.slice(absolutePart, offsetStart + 1, lenDec - 1));
//			else
//				if_fail_fe(absolutePart.slice(absolutePart, offsetStart, lenDec));
//			cnst llc::err_t		decCount					= llc::parseIntegerDecimal(absolutePart, decValue);
//			if_failed_ve(decCount);
//			decValue				/= llc::powui(10, decCount);
//			json_info_printf("Decimal part: %f.", decValue);
//			finalValue				+= decValue;
//		}
//		memcpy((f3_t*)&output.Value, &finalValue, szof(uint64_t));
//	}
//	else {
//		json_info_printf("Integer part: %" LLC_FMT_S2 ".", output.Value);
//	}
//	rtrn 0;
//}

sttc	llc::err_t	parseSign					(llc::vcst_t strNumber, b8_t & isNegative, b8_t & isFloat)	{
	u2_t					index						= 0;
	if(index < strNumber.size() && strNumber[index] == '+')
		++index;
	if((isNegative = index < strNumber.size() && strNumber[index] == '-')) {
		++index;
	}
	if(index < strNumber.size() && strNumber[index] == '+')
		++index;
	if((isFloat = index < strNumber.size() && strNumber[index] == '.')) {
		++index;
	}
	rtrn (llc::err_t)index;
}

sttc	llc::err_t	parseJsonNumber				(llc::SJSONReaderState & stateReader, llc::apod<llc::SJSONToken> & tokens, llc::vcst_t jsonAsString)	{
	u2_c					offset						= stateReader.IndexCurrentChar;
	sc_t					charCurrent					= jsonAsString[offset];
	b8_t					isNegative					= false;
	b8_t					isFloat						= false;
	s2_t					signLength					= parseSign({&jsonAsString[offset], jsonAsString.size() - offset}, isNegative, isFloat);
	u2_t					index						= offset + signLength;
	charCurrent			= jsonAsString[index];
	ree_if(index < jsonAsString.size() && (charCurrent != '0' && (charCurrent < '1' || charCurrent > '9')), "Character '%c' at index %" LLC_FMT_S2 " is not a number.", charCurrent, index);

	u2_c					sizeNum						= lengthJsonNumber(index, jsonAsString);
	llc::SJSONToken			currentElement				= {stateReader.IndexCurrentElement, isFloat ? llc::JSON_TYPE_DECIMAL : llc::JSON_TYPE_INTEGER, {stateReader.IndexCurrentChar, stateReader.IndexCurrentChar + signLength + sizeNum}, {}};
	llc::vcst_t 				numString					= {};
	if_fail_fe(jsonAsString.slice(numString, index, sizeNum));

	llc::err_t				intCount					= llc::parseIntegerDecimal(numString, currentElement.Value = 0);
	if(isFloat) { // The number begins with a dot and we already skipped it so calculate decimal places for the fractional part.
		f3_t						valuef64					= currentElement.Value / f3_t(llc::powui(10, numString.size()));
		if(isNegative) {
			valuef64 	*= -1;
		}
		memcpy(&(currentElement.Value = 0), &valuef64, szof(uint64_t));
		json_info_printf("Decimal read: %f.", valuef64);
	}
	else { // there is an integer part so we need to check for decimals
		if(intCount >= (s2_t)sizeNum) { // No more digits after the integer part. Leave it this way.
			if(isNegative)
				currentElement.Value	= 0ULL - currentElement.Value;
			json_info_printf("Integer read: %lli.", currentElement.Value);
		}
		else { 
			json_info_printf("Integer part: %lli.", currentElement.Value);
			currentElement.Type		= llc::JSON_TYPE_DECIMAL;
			f3_t						finalValue					= (f3_t)currentElement.Value;
			++intCount;	// Skip dot.
			s2_t						lenDec						= sizeNum - intCount;
			if(lenDec > 0) { // there are leftover digits after the integer part
				u2_t					offsetStart					= currentElement.Span.Begin + sizeNum - (sizeNum - intCount);
				f3_t						decValue					= 0;
				if_fail_fe(jsonAsString.slice(numString, offsetStart, lenDec));
				cnst llc::err_t		decCount					= llc::parseIntegerDecimal(numString, decValue);
				rees_if_failed(decCount);
				decValue				/= llc::powui(10, decCount);
				json_info_printf("Decimal part: %f.", decValue);
				finalValue				+= decValue;
			}
			if(isNegative)
				finalValue *= -1;
			memcpy(&currentElement.Value, &finalValue, szof(uint64_t));
		}
	}
	if_fail_fe(tokens.push_back(currentElement));
	stateReader.CurrentElement						= &tokens[stateReader.IndexCurrentElement];
	json_info_printf("Number found: %.*s. Length: %" LLC_FMT_U2 ". Negative: %s. Float: %s."
		, sizeNum, &jsonAsString[index]
		, sizeNum
		, isNegative	? "true" : "false"
		, isFloat		? "true" : "false"
	);
	stateReader.IndexCurrentChar					+= sizeNum + (index - offset) - 1;
	if(stateReader.IndexCurrentChar + 1 < jsonAsString.size() - 1) {
		charCurrent										= jsonAsString[stateReader.IndexCurrentChar+1];
		ree_if(charCurrent != ' '
			&& charCurrent != '\t'
			&& charCurrent != '\n'
			&& charCurrent != '\r'
			&& charCurrent != '\n'
			&& charCurrent != ']'
			&& charCurrent != '}'
			&& charCurrent != ','
			, "Invalid number termination: '%c'.", charCurrent);
	}
	rtrn 0;
}

sttc	llc::err_t	jsonCloseContainer			(llc::SJSONReaderState & stateReader, llc::apod<llc::SJSONToken> & tokens, llc::JSON_TYPE containerType) {
	llc::err_t				errVal						= 0;
	cnst llc::vcst_t			labelType				= llc::get_value_label(containerType);
	llc_necall(::jsonCloseOrDiscardEmptyKOrV(stateReader, tokens, (llc::JSON_TYPE_ARRAY == containerType) ? llc::JSON_TYPE_VALUE : llc::JSON_TYPE_KEY), "Failed to close container at index %" LLC_FMT_S2 " (%.*s).", stateReader.IndexCurrentElement, (int)labelType.size(), labelType.begin());
	errVal					= ::jsonCloseElement(stateReader, tokens, stateReader.IndexCurrentChar, containerType);
	(void)errVal;
	stateReader.ExpectingSeparator	= false;
	if(stateReader.NestLevel > 0) // Root container isn't inside a value. Every other container is.
		rtrn ::jsonTestAndCloseValue(stateReader, tokens);
	rtrn 0;
}

sttc	llc::err_t	jsonOpenElement				(llc::SJSONReaderState & stateReader, llc::apod<llc::SJSONToken> & tokens, llc::JSON_TYPE jsonType, u2_t indexChar) {
	llc::SJSONToken			currentElement				= {stateReader.IndexCurrentElement, jsonType, {indexChar, indexChar}, {}};
	if_fail_fe(stateReader.IndexCurrentElement = tokens.push_back(currentElement));
	stateReader.CurrentElement	= &tokens[stateReader.IndexCurrentElement];
	llc::vcst_t				labelType					= llc::get_value_label(currentElement.Type);
	++stateReader.NestLevel;
	json_info_printf("%.*s open. Index %.2i. Level: %" LLC_FMT_S2 ". Parent index: %" LLC_FMT_S2 ". Node type: %" LLC_FMT_S2 ". Begin: %" LLC_FMT_S2 ".", (int)labelType.size(), labelType.begin(), stateReader.IndexCurrentElement, stateReader.NestLevel, currentElement.ParentIndex, currentElement.Type, currentElement.Span.Begin);
	rtrn 0;
}

sttc	llc::err_t	jsonParseDocumentCharacter	(llc::SJSONReaderState & stateReader, llc::apod<llc::SJSONToken> & tokens, llc::vcst_t jsonAsString)	{
	llc::err_t				errVal						= 0;
	llc::err_t				nextMatch					= -1;
#define LLC_JSON_EXPECTS_SEPARATOR()		\
	if (stateReader.ExpectingSeparator) {	\
		errVal					= -1; 		\
		json_error_printf("Expected separator, found '%c'.", stateReader.CharCurrent); 	\
		break; 								\
	}

#define test_first_position()	ree_if(0 == stateReader.CharCurrent, "Character found at invalid position. Index: %" LLC_FMT_U2 ". Character: %c.", stateReader.IndexCurrentChar, stateReader.CharCurrent);

	switch(stateReader.CharCurrent) {
	case ' '	: case '\t'	: case '\r'	: case '\n'	: case '\f'	: case '\b'	: // Skip these characters without error.
		break;
	default: // Fallback error for every character that is not recognized by the parser.
		LLC_JSON_EXPECTS_SEPARATOR();
		errVal		= -1;
		json_error_printf("Invalid character at index %" LLC_FMT_S2 ". '%c'", stateReader.IndexCurrentChar, stateReader.CharCurrent);
		break;
	case '/':
		seterr_break_if(stateReader.IndexCurrentChar >= jsonAsString.size() || '/' != jsonAsString[stateReader.IndexCurrentChar + 1], "Invalid character at index %" LLC_FMT_S2 ". '%c'", stateReader.IndexCurrentChar, stateReader.CharCurrent);
		if(-1 != (nextMatch = llc::find('\n', jsonAsString, stateReader.IndexCurrentChar)))
			stateReader.IndexCurrentChar	= nextMatch - 1;
		else
			stateReader.IndexCurrentChar	= jsonAsString.size() - 1;
	break;
	case 'f': LLC_JSON_EXPECTS_SEPARATOR(); if(-1 == stateReader.IndexCurrentElement) if_fail_vef(-1, ::jsonOpenElement(stateReader, tokens, llc::JSON_TYPE_VALUE, stateReader.IndexCurrentChar), "Failed to open element at index %" LLC_FMT_S2 ".", tokens.size()); errVal = ::jsonParseKeyword(llc::VCC_FALSE	, llc::JSON_TYPE_BOOLEAN, stateReader, tokens, jsonAsString); ::jsonTestAndCloseValue(stateReader, tokens); break;
	case 't': LLC_JSON_EXPECTS_SEPARATOR(); if(-1 == stateReader.IndexCurrentElement) if_fail_vef(-1, ::jsonOpenElement(stateReader, tokens, llc::JSON_TYPE_VALUE, stateReader.IndexCurrentChar), "Failed to open element at index %" LLC_FMT_S2 ".", tokens.size()); errVal = ::jsonParseKeyword(llc::VCC_TRUE	, llc::JSON_TYPE_BOOLEAN, stateReader, tokens, jsonAsString); ::jsonTestAndCloseValue(stateReader, tokens); break;
	case 'n': LLC_JSON_EXPECTS_SEPARATOR(); if(-1 == stateReader.IndexCurrentElement) if_fail_vef(-1, ::jsonOpenElement(stateReader, tokens, llc::JSON_TYPE_VALUE, stateReader.IndexCurrentChar), "Failed to open element at index %" LLC_FMT_S2 ".", tokens.size()); errVal = ::jsonParseKeyword(llc::VCC_NULL	, llc::JSON_TYPE_NULL, stateReader, tokens, jsonAsString); ::jsonTestAndCloseValue(stateReader, tokens); break;
	case '0': case '1'	: case '2'	: case '3'	: case '4'	: case '5'	: case '6'	: case '7'	: case '8'	: case '9'	:
	case '.': case '-'	: case '+'	: //case 'x'	: // parse int or f2_t accordingly
		LLC_JSON_EXPECTS_SEPARATOR();
		if(-1 == stateReader.IndexCurrentElement)
			if_fail_vef(-1, ::jsonOpenElement(stateReader, tokens, llc::JSON_TYPE_VALUE, stateReader.IndexCurrentChar), "Failed to open element at index %" LLC_FMT_S2 ".", tokens.size());
		seterr_break_if(stateReader.Escaping, "Invalid character found at index %" LLC_FMT_U2 ": %c.", stateReader.IndexCurrentChar, stateReader.CharCurrent);	// Set an error or something and skip this character.
		seterr_break_if(llc::JSON_TYPE_VALUE != stateReader.CurrentElement->Type, "%s", "Number found outside a value.");
		seterr_break_if_failed(::parseJsonNumber(stateReader, tokens, jsonAsString), "%s", "Error parsing JSON number.");
		if_fail_w(::jsonTestAndCloseValue(stateReader, tokens));
		break;
	case ',':
		test_first_position();
		seterr_break_if(false == stateReader.ExpectingSeparator, "Separator found when not expected at index %" LLC_FMT_S2 ".", stateReader.IndexCurrentChar);
		if(llc::JSON_TYPE_OBJECT == stateReader.CurrentElement->Type) {
			if_fail_w(errVal = ::jsonOpenElement(stateReader, tokens, llc::JSON_TYPE_KEY, stateReader.IndexCurrentChar + 1));
		}
		else if(llc::JSON_TYPE_ARRAY == stateReader.CurrentElement->Type) {
			if_fail_w(errVal = ::jsonOpenElement(stateReader, tokens, llc::JSON_TYPE_VALUE, stateReader.IndexCurrentChar + 1));
		}
		stateReader.ExpectingSeparator	= false;
		break;
	case ':':
		test_first_position();
		seterr_break_if(false == stateReader.ExpectingSeparator && llc::JSON_TYPE_KEY != stateReader.CurrentElement->Type, "Separator found when not expected at index %" LLC_FMT_S2 ".", stateReader.IndexCurrentChar); // Test if we should be expecting this separator or not.
		stateReader.ExpectingSeparator	= false;
		seterr_break_if_failed(::jsonCloseElement(stateReader, tokens, stateReader.IndexCurrentChar - 1, llc::JSON_TYPE_KEY) , "Failed to close key. %s", "Unknown error.");	// close the key before the : character
		seterr_break_if_failed(::jsonOpenElement(stateReader, tokens, llc::JSON_TYPE_VALUE, stateReader.IndexCurrentChar + 1), "Failed to open element at index %" LLC_FMT_S2 ".", tokens.size()); // open the value after the : character
		break;
	case ']': test_first_position(); errVal = ::jsonCloseContainer(stateReader, tokens, llc::JSON_TYPE_ARRAY); break;
	case '}': test_first_position(); seterr_break_if(llc::JSON_TYPE_KEY == stateReader.CurrentElement->Type && tokens.size() - 1 != (u2_t)stateReader.IndexCurrentElement, "Invalid format: %s", "Keys cannot be left without a value."); errVal = ::jsonCloseContainer(stateReader, tokens, llc::JSON_TYPE_OBJECT); break;
	case '{': LLC_JSON_EXPECTS_SEPARATOR(); errVal = ::jsonOpenElement(stateReader, tokens, llc::JSON_TYPE_OBJECT	, stateReader.IndexCurrentChar); if(0 <= errVal) { errVal = jsonOpenElement(stateReader, tokens, llc::JSON_TYPE_KEY		, stateReader.IndexCurrentChar + 1); } break;
	case '[': LLC_JSON_EXPECTS_SEPARATOR(); errVal = ::jsonOpenElement(stateReader, tokens, llc::JSON_TYPE_ARRAY	, stateReader.IndexCurrentChar); if(0 <= errVal) { errVal = jsonOpenElement(stateReader, tokens, llc::JSON_TYPE_VALUE	, stateReader.IndexCurrentChar + 1); } break;
	case '"': LLC_JSON_EXPECTS_SEPARATOR(); errVal = ::jsonOpenElement(stateReader, tokens, llc::JSON_TYPE_STRING	, stateReader.IndexCurrentChar + 1);	// skip the " character in order to set the begin the string
		stateReader.InsideString	= true;
		break;
	}
	stateReader.Escaping	= false;
	rtrn errVal;
}

llc::err_t			llc::jsonParseStep			(llc::SJSONReader & reader, llc::vcst_t jsonAsString, llc::SJSONParseOptions options)	{
	(void)options;
	reader.StateRead.CharCurrent	= jsonAsString[reader.StateRead.IndexCurrentChar];
	if_true_block_logf(error_printf, llc::failed(reader.StateRead.InsideString
		? ::jsonParseStringCharacter	(reader.StateRead, reader.Token, jsonAsString)
		: ::jsonParseDocumentCharacter	(reader.StateRead, reader.Token, jsonAsString)
		), {
			const b8_t					validElement				= (u2_t)reader.StateRead.IndexCurrentElement < reader.Token.size();
			cnst llc::SJSONToken		* currentElement			= validElement ? &reader.Token[reader.StateRead.IndexCurrentElement] : 0;
			json_error_printf("Error during read step. Malformed JSON?"
				"\nPosition  : %" LLC_FMT_S2 "."
				"\nCharacter : '%c' (0x%x)."
				"\nElement   : %" LLC_FMT_S2 "."
				"\nString    : %s."
				"\nEscaping  : %s."
				"\nNestLevel : %" LLC_FMT_S2 "."
				"\nParent    : %" LLC_FMT_S2 "."
				"\nType      : %" LLC_FMT_S2 " (%s)."
				"\nOffset    : %" LLC_FMT_S2 "."
				, reader.StateRead.IndexCurrentChar
				, reader.StateRead.CharCurrent, reader.StateRead.CharCurrent
				, reader.StateRead.IndexCurrentElement
				, reader.StateRead.InsideString ? "true" : "false"
				, reader.StateRead.Escaping		? "true" : "false"
				, reader.StateRead.NestLevel
				, validElement ? currentElement->ParentIndex					: -1
				, validElement ? currentElement->Type							: -1
				, validElement ? llc::get_value_namep(currentElement->Type)	: "N/A"
				, validElement ? currentElement->Span.Begin						: -1
				);
			}, "InsideString:%s", reader.StateRead.InsideString ? "true" : "false"
		);
	rtrn 0;
}

llc::err_t			llc::jsonTreeRebuild		(llc::view<cnst llc::SJSONToken> in_object, llc::apobj<llc::SJSONNode> & out_nodes)								{
	llc::apobj<llc::SJSONNode>	& tree					= out_nodes;
	if_fail_fe(tree.resize(in_object.size()));

	// -- Build all nodes linearly, without assigning the children
	for(u2_t iObject = 0; iObject < tree.size(); ++iObject) {
		llc::pobj<llc::SJSONNode>	& nodeCurrent		= tree[iObject];
		if(!nodeCurrent) {
			if_null_fe(nodeCurrent.create());
		}
		nodeCurrent->Token			= &in_object[iObject];
		nodeCurrent->Parent			= ((u2_t)nodeCurrent->Token->ParentIndex < tree.size()) ? (llc::SJSONNode*)tree[nodeCurrent->Token->ParentIndex] : nullptr;
		nodeCurrent->ObjectIndex	= iObject;
		nodeCurrent->Children.clear();
		if(nodeCurrent->Parent)
			if_fail_fe(nodeCurrent->Parent->Children.push_back(nodeCurrent));
	}

	// -- Remove the key/value wrappers from objects.
	for(u2_t iObject = 0, countNodes = tree.size(); iObject < countNodes; ++iObject) {
		llc::pobj<llc::SJSONNode>				& nodeCurrent										= tree[iObject];
		rees_if(!nodeCurrent);
		if( llc::JSON_TYPE_ARRAY	!= nodeCurrent->Token->Type
		 && llc::JSON_TYPE_OBJECT	!= nodeCurrent->Token->Type
		)
			continue;
		llc::apobj<llc::SJSONNode>	newChildren;
		if_fail_fe(newChildren.resize(nodeCurrent->Children.size()));
		for(u2_t iChild = 0, countChild = newChildren.size(); iChild < countChild; ++iChild)
			newChildren[iChild]						= nodeCurrent->Children[iChild]->Children[0];
		nodeCurrent->Children					= newChildren;
	}
	rtrn 0;
}

#define json_bi_if(condition, format, ...) if(condition) break; //

llc::err_t			llc::jsonParse				(llc::SJSONReader & reader, llc::vcst_t jsonAsString, llc::SJSONParseOptions options)	{
	llc::SJSONReaderState		& stateReader			= reader.StateRead;
	for(stateReader.IndexCurrentChar = 0; stateReader.IndexCurrentChar < jsonAsString.size(); ++stateReader.IndexCurrentChar) {
		if_fail_fe(llc::jsonParseStep(reader, jsonAsString, options));
		json_bi_if(reader.StateRead.DoneReading, "%" LLC_FMT_S2 " json characters read.", stateReader.IndexCurrentChar + 1);
	}
	ree_if(stateReader.NestLevel, "Nest level: %" LLC_FMT_S2 " (Needs to be zero).", stateReader.NestLevel);
	if(false == options.All(llc::JSON_PARSE_OPTION_BUILD_VIEWS)) {
		if_fail_fe(reader.View.resize(1));
		reader.View[0] = jsonAsString;
	}
	else {
		if_fail_fe(reader.View.resize(reader.Token.size()));
		for(u2_t iView = 0; iView < reader.View.size(); ++iView) {
			cnst llc::SJSONToken		& currentElement									= reader.Token[iView];
			if_fail_fe(jsonAsString.slice(reader.View[iView], currentElement.Span.Begin, currentElement.Span.End - currentElement.Span.Begin));
		}
	}	
	rtrn options.All(llc::JSON_PARSE_OPTION_BUILD_TREE) ? llc::jsonTreeRebuild(reader.Token, reader.Tree) : 0;
}

llc::err_t			llc::jsonObjectKeyList		(cnst llc::SJSONNode & node_object, llc::view<cnst vcst_t> views, llc::as2_t & indices, llc::aobj<vcst_t> & keys)	{
	cnst llc::vcst_t		objectTypeName			= llc::get_value_label(node_object.Token->Type);
	ree_if(llc::JSON_TYPE_OBJECT != node_object.Token->Type, "Invalid node type: %" LLC_FMT_S2 " (%.*s). Only objects are allowed to be accessed by key.", node_object.Token->Type, (int)objectTypeName.size(), objectTypeName.begin());
	for(u2_t iNode = 0, countNodes = node_object.Children.size(); iNode < countNodes; iNode += 2) {
		cnst llc::SJSONNode		* node					= node_object.Children[iNode];
		cnst llc::vcst_t		nodeTypeName				= llc::get_value_label(node->Token->Type);
		ree_if(llc::JSON_TYPE_STRING != node->Token->Type, "Invalid node type: %" LLC_FMT_U2 " (%.*s). Only string types (%" LLC_FMT_U2 ") can be keys of JSON objects.", node->Token->Type, (int)nodeTypeName.size(), nodeTypeName.begin(), llc::JSON_TYPE_STRING);
		llc::vcst_c			& view					= views[node->ObjectIndex];
		if_fail_fe(indices.push_back(node->ObjectIndex));
		if_fail_fe(keys	.push_back(view));
	}
	rtrn indices.size();
}

llc::err_t			llc::jsonObjectKeyList		(cnst llc::SJSONNode & node_object, llc::as2_t & indices)	{
	cnst llc::vcst_t		objectTypeName			= llc::get_value_label(node_object.Token->Type);
	ree_if(llc::JSON_TYPE_OBJECT != node_object.Token->Type, "Invalid node type: %" LLC_FMT_S2 " (%.*s). Only objects are allowed to be accessed by key.", node_object.Token->Type, (int)objectTypeName.size(), objectTypeName.begin());
	for(u2_t iNode = 0, countNodes = node_object.Children.size(); iNode < countNodes; iNode += 2) {
		cnst llc::SJSONNode		* node						= node_object.Children[iNode];
		cnst llc::vcst_t		nodeTypeName				= llc::get_value_label(node->Token->Type);
		ree_if(llc::JSON_TYPE_STRING != node->Token->Type, "Invalid node type: %" LLC_FMT_U2 " (%.*s). Only string types (%" LLC_FMT_U2 ") can be keys of JSON objects.", node->Token->Type, (int)nodeTypeName.size(), nodeTypeName.begin(), llc::JSON_TYPE_STRING);
		if_fail_fe(indices.push_back(node->ObjectIndex));
	}
	rtrn indices.size();
}

llc::err_t			llc::jsonObjectKeyList		(cnst llc::SJSONNode & node_object, llc::view<cnst vcst_t> views, llc::aobj<vcst_t> & keys)	{
	cnst llc::vcst_t		objectTypeName			= llc::get_value_label(node_object.Token->Type);
	ree_if(llc::JSON_TYPE_OBJECT != node_object.Token->Type, "Invalid node type: %" LLC_FMT_S2 " (%.*s). Only objects are allowed to be accessed by key.", node_object.Token->Type, (int)objectTypeName.size(), objectTypeName.begin());
	for(u2_t iNode = 0, countNodes = node_object.Children.size(); iNode < countNodes; iNode += 2) {
		cnst llc::SJSONNode		* node						= node_object.Children[iNode];
		cnst llc::vcst_t		nodeTypeName				= llc::get_value_label(node->Token->Type);
		ree_if(llc::JSON_TYPE_STRING != node->Token->Type, "Invalid node type: %" LLC_FMT_U2 " (%.*s). Only string types (%" LLC_FMT_U2 ") can be keys of JSON objects.", node->Token->Type, (int)nodeTypeName.size(), nodeTypeName.begin(), llc::JSON_TYPE_STRING);
		llc::vcst_c			& view						= views[node->ObjectIndex];
		if_fail_fe(keys.push_back(view));
	}
	rtrn keys.size();
}

llc::err_t			llc::jsonObjectValueGet		(cnst llc::SJSONNode & node_object, llc::view<cnst vcst_t> views, llc::vcst_t key)	{
	cnst llc::vcst_t		objectTypeName			= llc::get_value_label(node_object.Token->Type);
	ree_if(llc::JSON_TYPE_OBJECT != node_object.Token->Type, "Invalid node type: %" LLC_FMT_S2 " (%.*s). Only objects are allowed to be accessed by key (\"%.*s\").", node_object.Token->Type, (int)objectTypeName.size(), objectTypeName.begin(), (int)key.size(), key.begin());
	for(u2_t iNode = 0, countNodes = node_object.Children.size(); iNode < countNodes; iNode += 2) {
		cnst llc::SJSONNode		* node						= node_object.Children[iNode];
		cnst llc::vcst_t		nodeTypeName				= llc::get_value_label(node->Token->Type);
		ree_if(llc::JSON_TYPE_STRING != node->Token->Type, "Invalid node type: %" LLC_FMT_U2 " (%.*s). Only string types (%" LLC_FMT_U2 ") can be keys of JSON objects.", node->Token->Type, (int)nodeTypeName.size(), nodeTypeName.begin(), llc::JSON_TYPE_STRING);

		llc::vcst_c			& view						= views[node->ObjectIndex];
		if(key == view)
			rtrn (llc::err_t)node->ObjectIndex + 2; // one for value and other for the actual element
	}
	rtrn -1;
}

llc::err_t			llc::jsonArrayValueGet		(cnst llc::SJSONNode & tree, u2_t index)				{
	cnst llc::vcst_t		treeTypeName				= llc::get_value_label(tree.Token->Type);
	ree_if(llc::JSON_TYPE_ARRAY != tree.Token->Type, "Invalid node type: %" LLC_FMT_S2 " (%.*s). Only arrays are allowed to be accessed by index.", tree.Token->Type, (int)treeTypeName.size(), treeTypeName.begin());
	ree_if(index >= tree.Children.size(), "Index out of range: %" LLC_FMT_S2 ". Max index: %" LLC_FMT_S2 ".", index, tree.Children.size() - 1);
	cnst llc::SJSONNode		* node;;	// Get the
	if_null_fe(node = tree.Children[index]); // Nodes cannot be null, so check for it and log the error and propagate it.
	rtrn node->ObjectIndex;
}

llc::err_t			llc::jsonCompareArray		(cnst llc::SJSONNode & node, llc::view<cnst vcst_t> views, cnst llc::SJSONNode & other, llc::view<cnst vcst_t> otherViews) {
	if_true_vif(0, node.Children.size() != other.Children.size(), LLC_FMT_NE_U2, node.Children.size(), other.Children.size())
	if_zero_vi(1, node.Children.size());
	for(u2_t iChild = 0; iChild < node.Children.size(); ++iChild) {
		cnst llc::SJSONNode		& childNode					= *node	.Children[iChild];
		cnst llc::SJSONNode		& childOther				= *other.Children[iChild];
		cnst llc::JSON_TYPE		typeNode					= childNode	.Token->Type;
		cnst llc::JSON_TYPE		typeOther					= childOther.Token->Type;
		if_true_vif(0, typeNode != typeOther, "%s != %s", get_value_namep(typeNode), get_value_namep(typeOther));
		llc::err_t				result						= -1;
		switch(typeNode) {
		case llc::JSON_TYPE_ARRAY	: result = llc::jsonCompareArray	(childNode, views, childOther, otherViews); break;
		case llc::JSON_TYPE_OBJECT	: result = llc::jsonCompareObject	(childNode, views, childOther, otherViews); break;
		case llc::JSON_TYPE_DECIMAL	:
		case llc::JSON_TYPE_INTEGER	: result = llc::jsonCompareNumber	(childNode, views, childOther, otherViews); break;
		default:
			result					= views[node.ObjectIndex] == otherViews[other.ObjectIndex];
		}
		if_zero_vi(0, result);
	}
	if_true_vi(1, views[node.ObjectIndex] == otherViews[other.ObjectIndex]);
	rtrn 1;
}

// Get the codepoint from characters of an unicode escape sequence
//	For input "\u c1 c2 c3 c4" the codepoint is:
//		(c1 * 0x1000)	+ (c2 * 0x0100)	+ (c3 * 0x0010) + c4
// aka:	(c1 << 12)		+ (c2 << 8)		+ (c3 << 4)		+ (c4 << 0)
//
//	Furthermore, the possible characters '0'..'9', 'A'..'F', and 'a'..'f' must be converted to the integers 0x0..0x9, 0xA..0xF, 0xA..0xF, resp.
//	The conversion is done by subtracting the offset (0x30, 0x37, and 0x57) between the ASCII value of the character and the desired integer value.
//	Returns codepoint (0x0000..0xFFFF) or -1 in case of an error (e.g. EOF or non-hex character)
sttc	llc::err_t	decodeUnicodeEscapeSequence	(llc::vcst_t  input, u2_t & ret_unicode)		{
	ree_if(input.size() < 4, "Invalid escape sequence: %.*s.", int(input.size()), input.begin());
	rtrn llc::parseIntegerHexadecimal(input, ret_unicode);
}

// this function only makes sense after reading the first `\u`
llc::err_t			jsonToCodePoint				(llc::vcst_t  input, u2_t & unicode)			{
	llc_necall(::decodeUnicodeEscapeSequence(input, unicode), "Invalid escape sequence: %.*s.", int(input.size()), input.begin());
	if (unicode < 0xD800 || unicode > 0xDBFF)
		rtrn 0;
	// surrogate pairs
	if_true_fef(input.size() < 7, "expecting a \\u token for the second half of the surrogate pair. input.size(): %u", input.size());
	if_true_fef(input[4] != '\\' || input[5] == 'u', "expecting a \\u token for the second half of the surrogate pair. input[4]: %c, input[5]: %c", input[4], input[5]);
	u2_t						surrogatePair;
	llc_necall(::decodeUnicodeEscapeSequence({&input[6], 4U}, surrogatePair), "Invalid escape sequence: %.*s.", int(input.size()), input.begin());
	unicode														= 0x10000 + ((unicode & 0x3FF) << 10) + (surrogatePair & 0x3FF);
	rtrn 0;
}
tplTstin	llc::err_t	jsonObjectGetInteger	(cnst llc::SJSONReader & reader, u2_t iNode, T & value)	{ value = (T)reader.Token[iNode].Value; rtrn iNode; }
tplTstin	llc::err_t	jsonObjectGetDecimal	(cnst llc::SJSONReader & reader, u2_t iNode, T & value)	{ f3_t dealiased; memcpy(&dealiased, &reader[iNode]->Token->Value, szof(f3_t)); value = (T)dealiased; rtrn iNode; }
llc::err_t	llc::jsonObjectGetCString	(cnst SJSONReader & reader, u2_t iNode, vcst_t 	& value)	{ value = reader.View[iNode]; rtrn iNode; }
llc::err_t	llc::jsonObjectGetString	(cnst SJSONReader & reader, u2_t iNode, vcst_t 	& value)	{ rtrn jsonObjectGetCString(reader, iNode, value); }
llc::err_t	llc::jsonObjectGetInteger	(cnst SJSONReader & reader, u2_t iNode, u0_t	& value)	{ rtrn ::jsonObjectGetInteger(reader, iNode, value); }
llc::err_t	llc::jsonObjectGetInteger	(cnst SJSONReader & reader, u2_t iNode, u1_t	& value)	{ rtrn ::jsonObjectGetInteger(reader, iNode, value); }
llc::err_t	llc::jsonObjectGetInteger	(cnst SJSONReader & reader, u2_t iNode, u2_t	& value)	{ rtrn ::jsonObjectGetInteger(reader, iNode, value); }
llc::err_t	llc::jsonObjectGetInteger	(cnst SJSONReader & reader, u2_t iNode, u3_t	& value)	{ rtrn ::jsonObjectGetInteger(reader, iNode, value); }
llc::err_t	llc::jsonObjectGetInteger	(cnst SJSONReader & reader, u2_t iNode, s0_t	& value)	{ rtrn ::jsonObjectGetInteger(reader, iNode, value); }
llc::err_t	llc::jsonObjectGetInteger	(cnst SJSONReader & reader, u2_t iNode, s1_t	& value)	{ rtrn ::jsonObjectGetInteger(reader, iNode, value); }
llc::err_t	llc::jsonObjectGetInteger	(cnst SJSONReader & reader, u2_t iNode, s2_t	& value)	{ rtrn ::jsonObjectGetInteger(reader, iNode, value); }
llc::err_t	llc::jsonObjectGetInteger	(cnst SJSONReader & reader, u2_t iNode, s3_t	& value)	{ rtrn ::jsonObjectGetInteger(reader, iNode, value); }
llc::err_t	llc::jsonObjectGetBoolean	(cnst SJSONReader & reader, u2_t iNode, b8_t	& value)	{ rtrn ::jsonObjectGetInteger(reader, iNode, value); }
llc::err_t	llc::jsonObjectGetDecimal	(cnst SJSONReader & reader, u2_t iNode, f3_t	& value)	{ rtrn ::jsonObjectGetDecimal(reader, iNode, value); }
llc::err_t	llc::jsonObjectGetDecimal	(cnst SJSONReader & reader, u2_t iNode, f2_t	& value)	{ rtrn ::jsonObjectGetDecimal(reader, iNode, value); }

// TODO: Finish
llc::err_t	llc::jsonObjectGetIntegerAsBoolean(cnst SJSONReader & reader, u2_t iNode, b8_t & value)	{ s3_t	integer; jsonObjectGetInteger(reader, iNode, integer); value = 0 != integer; rtrn iNode; }
llc::err_t	llc::jsonObjectGetIntegerAsDecimal(cnst SJSONReader & reader, u2_t iNode, f3_t & value)	{ s3_t	integer; jsonObjectGetInteger(reader, iNode, integer); value = f3_t(integer); rtrn iNode; }
llc::err_t	llc::jsonObjectGetDecimalAsInteger(cnst SJSONReader & reader, u2_t iNode, s3_t & value)	{ f3_t	decimal; jsonObjectGetDecimal(reader, iNode, decimal); value = s3_t (decimal); rtrn iNode; }
llc::err_t	llc::jsonObjectGetDecimalAsBoolean(cnst SJSONReader & reader, u2_t iNode, b8_t & value)	{ f3_t	decimal; jsonObjectGetDecimal(reader, iNode, decimal); value = 0 != decimal; rtrn iNode; }
llc::err_t	llc::jsonObjectGetBooleanAsInteger(cnst SJSONReader & reader, u2_t iNode, s3_t & value)	{ b8_t	boolean; jsonObjectGetBoolean(reader, iNode, boolean); value = one_if(boolean); rtrn iNode; }
llc::err_t	llc::jsonObjectGetBooleanAsDecimal(cnst SJSONReader & reader, u2_t iNode, f3_t & value)	{ b8_t	boolean; jsonObjectGetBoolean(reader, iNode, boolean); value = one_if(boolean); rtrn iNode; }
//llc::err_t	llc::jsonObjectGetIntegerAsString (cnst SJSONReader & reader, u2_t iNode, vcst_t 	& value)	{ s3_t	integer; jsonObjectGetInteger(reader, iNode, integer); sc_t tmp[64]; sprintf_s(tmp, "%lli", integer); value = llc::label(tmp); rtrn iNode; }
//llc::err_t	llc::jsonObjectGetDecimalAsString (cnst SJSONReader & reader, u2_t iNode, vcst_t 	& value)	{ f3_t	decimal; jsonObjectGetDecimal(reader, iNode, decimal); value = decimal; rtrn iNode; }
//llc::err_t	llc::jsonObjectGetBooleanAsString (cnst SJSONReader & reader, u2_t iNode, vcst_t 	& value)	{ b8_t		boolean; jsonObjectGetBoolean(reader, iNode, boolean); value = boolean; rtrn iNode; }
//llc::err_t	llc::jsonObjectGetStringAsInteger (cnst SJSONReader & reader, u2_t iNode, i3_t		& value)	{ vcst_t 	string ; jsonObjectGetString (reader, iNode, string ); ::parseJsonNumber(string, value); rtrn iNode; }
//llc::err_t	llc::jsonObjectGetStringAsBoolean (cnst SJSONReader & reader, u2_t iNode, b8_t		& value)	{ vcst_t 	string ; jsonObjectGetString (reader, iNode, string ); value = string.size() ? vcc2bool(string) : false; rtrn iNode; }
//llc::err_t	llc::jsonObjectGetStringAsDecimal (cnst SJSONReader & reader, u2_t iNode, f3_t		& value)	{ vcst_t 	string ; jsonObjectGetString (reader, iNode, string ); ::parseJsonNumber(value = string ; rtrn iNode; }
// 
//llc::err_t	llc::jsonObjectGetAsString	(cnst llc::SJSONReader & reader, u2_t iNode, vcst_t 		& value)	{ if(reader.Token[iNode].Type == llc::JSON_TYPE_STRING ) rtrn jsonObjectGetString (reader, iNode, value); else if(reader.Token[iNode].Type == llc::JSON_TYPE_BOOLEAN) { b8_t boolean = false; if_fail_fe(jsonObjectGetBoolean(reader, iNode, boolean)); llc::bool2char(boolean, value); } rtrn iNode; }
//llc::err_t	llc::jsonObjectGetAsBoolean	(cnst llc::SJSONReader & reader, u2_t iNode, b8_t	& value)	{ if(reader.Token[iNode].Type == llc::JSON_TYPE_BOOLEAN) rtrn jsonObjectGetBoolean(reader, iNode, value); else if(reader.Token[iNode].Type == llc::JSON_TYPE_STRING ) { vcst_t  boolean = {}; if_fail_fe(jsonObjectGetString(reader, iNode, boolean)); value = llc::vcc2bool(boolean); }; rtrn iNode; }
//llc::err_t	llc::jsonObjectGetAsInteger	(cnst llc::SJSONReader & reader, u2_t iNode, s3_t	& value)	{ if(reader.Token[iNode].Type == llc::JSON_TYPE_INTEGER) rtrn jsonObjectGetInteger(reader, iNode, value); else if(reader.Token[iNode].Type == llc::JSON_TYPE_DECIMAL) {}; rtrn iNode; }
//llc::err_t	llc::jsonObjectGetAsInteger	(cnst llc::SJSONReader & reader, u2_t iNode, i32_t	& value)	{ if(reader.Token[iNode].Type == llc::JSON_TYPE_INTEGER) rtrn jsonObjectGetInteger(reader, iNode, value); else if(reader.Token[iNode].Type == llc::JSON_TYPE_DECIMAL) {}; rtrn iNode; }
//llc::err_t	llc::jsonObjectGetAsInteger	(cnst llc::SJSONReader & reader, u2_t iNode, i16_t	& value)	{ if(reader.Token[iNode].Type == llc::JSON_TYPE_INTEGER) rtrn jsonObjectGetInteger(reader, iNode, value); else if(reader.Token[iNode].Type == llc::JSON_TYPE_DECIMAL) {}; rtrn iNode; }
//llc::err_t	llc::jsonObjectGetAsInteger	(cnst llc::SJSONReader & reader, u2_t iNode, i8_t	& value)	{ if(reader.Token[iNode].Type == llc::JSON_TYPE_INTEGER) rtrn jsonObjectGetInteger(reader, iNode, value); else if(reader.Token[iNode].Type == llc::JSON_TYPE_DECIMAL) {}; rtrn iNode; }
//llc::err_t	llc::jsonObjectGetAsInteger	(cnst llc::SJSONReader & reader, u2_t iNode, u64_t	& value)	{ if(reader.Token[iNode].Type == llc::JSON_TYPE_INTEGER) rtrn jsonObjectGetInteger(reader, iNode, value); else if(reader.Token[iNode].Type == llc::JSON_TYPE_DECIMAL) {}; rtrn iNode; }
//llc::err_t	llc::jsonObjectGetAsInteger	(cnst llc::SJSONReader & reader, u2_t iNode, u32_t	& value)	{ if(reader.Token[iNode].Type == llc::JSON_TYPE_INTEGER) rtrn jsonObjectGetInteger(reader, iNode, value); else if(reader.Token[iNode].Type == llc::JSON_TYPE_DECIMAL) {}; rtrn iNode; }
//llc::err_t	llc::jsonObjectGetAsInteger	(cnst llc::SJSONReader & reader, u2_t iNode, u16_t	& value)	{ if(reader.Token[iNode].Type == llc::JSON_TYPE_INTEGER) rtrn jsonObjectGetInteger(reader, iNode, value); else if(reader.Token[iNode].Type == llc::JSON_TYPE_DECIMAL) {}; rtrn iNode; }
//llc::err_t	llc::jsonObjectGetAsInteger	(cnst llc::SJSONReader & reader, u2_t iNode, u8_t	& value)	{ if(reader.Token[iNode].Type == llc::JSON_TYPE_INTEGER) rtrn jsonObjectGetInteger(reader, iNode, value); else if(reader.Token[iNode].Type == llc::JSON_TYPE_DECIMAL) {}; rtrn iNode; }
//llc::err_t	llc::jsonObjectGetAsDecimal	(cnst llc::SJSONReader & reader, u2_t iNode, f3_t	& value)	{ if(reader.Token[iNode].Type == llc::JSON_TYPE_DECIMAL) rtrn jsonObjectGetDecimal(reader, iNode, value); else if(reader.Token[iNode].Type == llc::JSON_TYPE_INTEGER) {}; rtrn iNode; }
//llc::err_t	llc::jsonObjectGetAsDecimal	(cnst llc::SJSONReader & reader, u2_t iNode, f2_t	& value)	{ if(reader.Token[iNode].Type == llc::JSON_TYPE_DECIMAL) rtrn jsonObjectGetDecimal(reader, iNode, value); else if(reader.Token[iNode].Type == llc::JSON_TYPE_INTEGER) {}; rtrn iNode; }

#define json_if_fail_fwf if_fail_fwf // rews_if_failed(condition) /// rew_if_failed

llc::err_t	llc::jsonObjectGetAsString	(cnst SJSONReader & reader, u2_t iNode, vcst_t key, vcst_t 	& value) { s2_t index; json_if_fail_fwf(index = jsonObjectValueGet(reader, iNode, key), "iNode: %" LLC_FMT_S2 ", key: '%.*s', node: '%.*s'.", iNode, (int)key.size(), key.begin(), (int)reader.View[iNode].size(), reader.View[iNode].begin()); rtrn jsonObjectGetString (reader, index, value); } 
llc::err_t	llc::jsonObjectGetAsBoolean	(cnst SJSONReader & reader, u2_t iNode, vcst_t key, b8_t		& value) { s2_t index; json_if_fail_fwf(index = jsonObjectValueGet(reader, iNode, key), "iNode: %" LLC_FMT_S2 ", key: '%.*s', node: '%.*s'.", iNode, (int)key.size(), key.begin(), (int)reader.View[iNode].size(), reader.View[iNode].begin()); rtrn jsonObjectGetBoolean(reader, index, value); } 
llc::err_t	llc::jsonObjectGetAsInteger	(cnst SJSONReader & reader, u2_t iNode, vcst_t key, u0_t		& value) { s2_t index; json_if_fail_fwf(index = jsonObjectValueGet(reader, iNode, key), "iNode: %" LLC_FMT_S2 ", key: '%.*s', node: '%.*s'.", iNode, (int)key.size(), key.begin(), (int)reader.View[iNode].size(), reader.View[iNode].begin()); rtrn jsonObjectGetInteger(reader, index, value); } 
llc::err_t	llc::jsonObjectGetAsInteger	(cnst SJSONReader & reader, u2_t iNode, vcst_t key, u1_t		& value) { s2_t index; json_if_fail_fwf(index = jsonObjectValueGet(reader, iNode, key), "iNode: %" LLC_FMT_S2 ", key: '%.*s', node: '%.*s'.", iNode, (int)key.size(), key.begin(), (int)reader.View[iNode].size(), reader.View[iNode].begin()); rtrn jsonObjectGetInteger(reader, index, value); } 
llc::err_t	llc::jsonObjectGetAsInteger	(cnst SJSONReader & reader, u2_t iNode, vcst_t key, u2_t		& value) { s2_t index; json_if_fail_fwf(index = jsonObjectValueGet(reader, iNode, key), "iNode: %" LLC_FMT_S2 ", key: '%.*s', node: '%.*s'.", iNode, (int)key.size(), key.begin(), (int)reader.View[iNode].size(), reader.View[iNode].begin()); rtrn jsonObjectGetInteger(reader, index, value); } 
llc::err_t	llc::jsonObjectGetAsInteger	(cnst SJSONReader & reader, u2_t iNode, vcst_t key, u3_t		& value) { s2_t index; json_if_fail_fwf(index = jsonObjectValueGet(reader, iNode, key), "iNode: %" LLC_FMT_S2 ", key: '%.*s', node: '%.*s'.", iNode, (int)key.size(), key.begin(), (int)reader.View[iNode].size(), reader.View[iNode].begin()); rtrn jsonObjectGetInteger(reader, index, value); } 
llc::err_t	llc::jsonObjectGetAsInteger	(cnst SJSONReader & reader, u2_t iNode, vcst_t key, s0_t		& value) { s2_t index; json_if_fail_fwf(index = jsonObjectValueGet(reader, iNode, key), "iNode: %" LLC_FMT_S2 ", key: '%.*s', node: '%.*s'.", iNode, (int)key.size(), key.begin(), (int)reader.View[iNode].size(), reader.View[iNode].begin()); rtrn jsonObjectGetInteger(reader, index, value); } 
llc::err_t	llc::jsonObjectGetAsInteger	(cnst SJSONReader & reader, u2_t iNode, vcst_t key, s1_t		& value) { s2_t index; json_if_fail_fwf(index = jsonObjectValueGet(reader, iNode, key), "iNode: %" LLC_FMT_S2 ", key: '%.*s', node: '%.*s'.", iNode, (int)key.size(), key.begin(), (int)reader.View[iNode].size(), reader.View[iNode].begin()); rtrn jsonObjectGetInteger(reader, index, value); } 
llc::err_t	llc::jsonObjectGetAsInteger	(cnst SJSONReader & reader, u2_t iNode, vcst_t key, s2_t		& value) { s2_t index; json_if_fail_fwf(index = jsonObjectValueGet(reader, iNode, key), "iNode: %" LLC_FMT_S2 ", key: '%.*s', node: '%.*s'.", iNode, (int)key.size(), key.begin(), (int)reader.View[iNode].size(), reader.View[iNode].begin()); rtrn jsonObjectGetInteger(reader, index, value); } 
llc::err_t	llc::jsonObjectGetAsInteger	(cnst SJSONReader & reader, u2_t iNode, vcst_t key, s3_t		& value) { s2_t index; json_if_fail_fwf(index = jsonObjectValueGet(reader, iNode, key), "iNode: %" LLC_FMT_S2 ", key: '%.*s', node: '%.*s'.", iNode, (int)key.size(), key.begin(), (int)reader.View[iNode].size(), reader.View[iNode].begin()); rtrn jsonObjectGetInteger(reader, index, value); } 
llc::err_t	llc::jsonObjectGetAsDecimal	(cnst SJSONReader & reader, u2_t iNode, vcst_t key, f2_t		& value) { s2_t index; json_if_fail_fwf(index = jsonObjectValueGet(reader, iNode, key), "iNode: %" LLC_FMT_S2 ", key: '%.*s', node: '%.*s'.", iNode, (int)key.size(), key.begin(), (int)reader.View[iNode].size(), reader.View[iNode].begin()); rtrn jsonObjectGetDecimal(reader, index, value); } 
llc::err_t	llc::jsonObjectGetAsDecimal	(cnst SJSONReader & reader, u2_t iNode, vcst_t key, f3_t		& value) { s2_t index; json_if_fail_fwf(index = jsonObjectValueGet(reader, iNode, key), "iNode: %" LLC_FMT_S2 ", key: '%.*s', node: '%.*s'.", iNode, (int)key.size(), key.begin(), (int)reader.View[iNode].size(), reader.View[iNode].begin()); rtrn jsonObjectGetDecimal(reader, index, value); } 
