#define LLC_DISABLE_DEBUG_BREAK_ON_ERROR_LOG

#include "llc_xml_reader.h"
#include "llc_file.h"
#include "llc_parse.h"

LLC_USING_TYPEINT();

#define xml_info_printf(...) // info_printf

sttc bool xmlTextEquals(::llc::vcst_t left, ::llc::vcst_t right) {
	rtrn left.size() == right.size() && (0 == left.size() || 0 == memcmp(left.begin(), right.begin(), left.size()));
}

sttc bool xmlStartsWith(::llc::vcst_t xmlDoc, ::llc::u2_t offset, ::llc::vcst_t text) {
	rtrn offset <= xmlDoc.size() && text.size() <= xmlDoc.size() - offset && 0 == memcmp(&xmlDoc[offset], text.begin(), text.size());
}

sttc ::llc::err_t xmlTokenAdd(::llc::SXMLReader & reader, ::llc::XML_TOKEN type, ::llc::u2_t offset, ::llc::u2_t count, ::llc::s2_t parent) {
	rtrn reader.Token.push_back({parent, type, false, {offset, offset + count}});
}

sttc ::llc::err_t xmlOpeningName(cnst ::llc::SXMLReader & reader, ::llc::vcst_t xmlDoc, ::llc::u2_t iNode, ::llc::vcst_t & output) {
	for(::llc::u2_t iToken = iNode + 1; iToken < reader.Token.size(); ++iToken) {
		cnst ::llc::SXMLToken & token	= reader.Token[iToken];
		if(token.ParentIndex == (::llc::s2_t)iNode && (token.Type == ::llc::XML_TOKEN_TAG_OPEN || token.Type == ::llc::XML_TOKEN_TAG_OPENCLOSE)) {
			for(::llc::u2_t iName = iToken + 1; iName < reader.Token.size(); ++iName)
				if(reader.Token[iName].ParentIndex == (::llc::s2_t)iToken && reader.Token[iName].Type == ::llc::XML_TOKEN_TAG_NAME)
					rtrn ::llc::xmlTokenView(xmlDoc, reader.Token[iName], output) < 0 ? -1 : (::llc::err_t)iName;
		}
	}
	rtrn -1;
}

sttc ::llc::err_t xmlSpecialParse(::llc::SXMLReader & reader, ::llc::vcst_t xmlDoc, ::llc::u2_t iParent, ::llc::u2_t & offset) {
	cnst ::llc::u2_t iBegin		= offset;
	::llc::XML_TOKEN type			= ::llc::XML_TOKEN_DOCTYPE;
	::llc::vcst_t terminator		= ">";
	if(::xmlStartsWith(xmlDoc, offset, "<!--")) {
		type						= ::llc::XML_TOKEN_COMMENT;
		terminator					= "-->";
		offset						+= 4;
	}
	else if(::xmlStartsWith(xmlDoc, offset, "<![CDATA[")) {
		type						= ::llc::XML_TOKEN_CDATA;
		terminator					= "]]>";
		offset						+= 9;
	}
	else if(::xmlStartsWith(xmlDoc, offset, "<!DOCTYPE"))
		offset						+= 9;
	else
		rtrn -1;
	cnst ::llc::err_t iEnd			= xmlDoc.find(terminator, offset);
	if_fail_fef(iEnd, "Unclosed XML special token at offset:%u.", iBegin);
	offset						= (::llc::u2_t)iEnd + terminator.size();
	rtrn ::xmlTokenAdd(reader, type, iBegin, offset - iBegin, (::llc::s2_t)iParent) < 0 ? -1 : 0;
}

sttc ::llc::err_t xmlOpenElement(::llc::SXMLReader & reader, ::llc::XML_TOKEN type, ::llc::s2_t iStartCharacter = -1) {
	::llc::SXMLReaderState	& state		= reader.State;
	cnst ::llc::s2_t		parent		= state.IndexCurrentElement;
	cnst ::llc::u2_t		offset		= iStartCharacter < 0 ? state.IndexCurrentChar : (::llc::u2_t)iStartCharacter;
	if_fail_fe(state.IndexCurrentElement = reader.Token.push_back({parent, type, false, {offset, offset}}));
	state.CurrentElement					= &reader.Token[state.IndexCurrentElement];
	rtrn ++state.NestLevel;
}

sttc ::llc::err_t xmlCloseElement(::llc::SXMLReader & reader, ::llc::s2_t iStopCharacter = -1) {
	::llc::SXMLReaderState	& state		= reader.State;
	if_null_fef(state.CurrentElement, "%s", "No XML element is open.");
	cnst ::llc::u2_t		stop		= iStopCharacter < 0 ? state.IndexCurrentChar : (::llc::u2_t)iStopCharacter;
	if_true_fef(stop < state.CurrentElement->Span.Begin, "Invalid XML span:%u..%u.", state.CurrentElement->Span.Begin, stop);
	state.CurrentElement->Span.End		= stop;
	state.IndexCurrentElement			= state.CurrentElement->ParentIndex;
	state.CurrentElement					= state.IndexCurrentElement < 0 ? 0 : &reader.Token[state.IndexCurrentElement];
	rtrn --state.NestLevel;
}

sttc ::llc::err_t xmlClosingNameValidate(cnst ::llc::SXMLReader & reader, ::llc::vcst_t xmlDoc, ::llc::u2_t iClose) {
	if_true_fef(iClose >= reader.Token.size() || reader.Token[iClose].Type != ::llc::XML_TOKEN_TAG_CLOSE, "Invalid XML closing token:%u.", iClose);
	::llc::vcst_t openingName;
	if_fail_fef(::xmlOpeningName(reader, xmlDoc, reader.Token[iClose].ParentIndex, openingName), "Open XML node has no name at token:%i.", reader.Token[iClose].ParentIndex);
	for(::llc::u2_t iName = iClose + 1; iName < reader.Token.size(); ++iName) {
		cnst ::llc::SXMLToken & token = reader.Token[iName];
		if(token.ParentIndex != (::llc::s2_t)iClose || token.Type != ::llc::XML_TOKEN_TAG_NAME)
			continue;
		::llc::vcst_t closingName;
		if_fail_fe(::llc::xmlTokenView(xmlDoc, token, closingName));
		if_true_fef(false == ::xmlTextEquals(openingName, closingName), "Mismatched XML closing tag:'%.*s'; expected:'%.*s'.", (int)closingName.size(), closingName.begin(), (int)openingName.size(), openingName.begin());
		rtrn 0;
	}
	rtrn -1;
}

sttc ::llc::err_t xmlParseStringCharacter(::llc::SXMLReader & reader) {
	::llc::SXMLReaderState & state = reader.State;
	cnst ::llc::sc_t quote = state.Flags.ExpectingSeparator ? '\'' : '\"';
	if(state.CharCurrent != quote)
		rtrn 0;
	if_fail_fef(::xmlCloseElement(reader), "Invalid XML nesting level:%i.", state.NestLevel);
	if_true_fef(0 == state.CurrentElement || state.CurrentElement->Type != ::llc::XML_TOKEN_ATTR, "%s", "XML attribute value has no attribute owner.");
	if_fail_fef(::xmlCloseElement(reader, state.IndexCurrentChar + 1), "Invalid XML nesting level:%i.", state.NestLevel);
	state.Flags.InsideString				= false;
	state.Flags.ExpectingSeparator		= false;
	rtrn 0;
}

sttc ::llc::err_t xmlParseTagCharacter(::llc::SXMLReader & reader, ::llc::vcst_t xmlDoc) {
	::llc::SXMLReaderState & state = reader.State;
	cnst ::llc::XML_TOKEN type = state.CurrentElement->Type;
	if(::llc::is_space(state.CharCurrent))
		rtrn 0;
	switch(type) {
	case ::llc::XML_TOKEN_TAG_OPEN:
		if(state.CharCurrent == '/') {
			if_true_fef(state.IndexCurrentChar + 1 >= xmlDoc.size() || xmlDoc[state.IndexCurrentChar + 1] != '>', "Invalid '/' in XML opening tag at offset:%u.", state.IndexCurrentChar);
			state.CurrentElement->Type = ::llc::XML_TOKEN_TAG_OPENCLOSE;
			rtrn 0;
		}
		if(state.CharCurrent == '>')
			rtrn ::xmlCloseElement(reader, state.IndexCurrentChar + 1);
		if_fail_fe(::xmlOpenElement(reader, ::llc::XML_TOKEN_ATTR));
		if_fail_fe(::xmlOpenElement(reader, ::llc::XML_TOKEN_ATTR_NAME));
		rtrn 0;
	case ::llc::XML_TOKEN_TAG_OPENCLOSE:
		if_true_fef(state.CharCurrent != '>', "Invalid character '%c' after '/' in XML opening tag.", state.CharCurrent);
		if_fail_fe(::xmlCloseElement(reader, state.IndexCurrentChar + 1));
		rtrn ::xmlCloseElement(reader, state.IndexCurrentChar + 1);
	case ::llc::XML_TOKEN_TAG_CLOSE:
		if_true_fef(state.CharCurrent != '>', "Invalid character '%c' after XML closing name.", state.CharCurrent);
		if_fail_fe(::xmlClosingNameValidate(reader, xmlDoc, state.IndexCurrentElement));
		if_fail_fe(::xmlCloseElement(reader, state.IndexCurrentChar + 1));
		rtrn ::xmlCloseElement(reader, state.IndexCurrentChar + 1);
	case ::llc::XML_TOKEN_PI:
		if(state.CharCurrent == '?') {
			state.Flags.ExpectingSeparator = true;
			rtrn 0;
		}
		if(state.CharCurrent == '>') {
			if_true_fef(false == state.Flags.ExpectingSeparator, "Missing '?' before XML processing-instruction close at offset:%u.", state.IndexCurrentChar);
			state.Flags.ExpectingSeparator = false;
			rtrn ::xmlCloseElement(reader, state.IndexCurrentChar + 1);
		}
		if_fail_fe(::xmlOpenElement(reader, ::llc::XML_TOKEN_ATTR));
		if_fail_fe(::xmlOpenElement(reader, ::llc::XML_TOKEN_ATTR_NAME));
		rtrn 0;
	case ::llc::XML_TOKEN_ATTR:
		if(state.CharCurrent == '=') {
			if_true_fef(false == state.Flags.ExpectingSeparator, "Unexpected '=' in XML attribute at offset:%u.", state.IndexCurrentChar);
			state.Flags.ExpectingSeparator = false;
			rtrn 0;
		}
		if_true_fef(state.Flags.ExpectingSeparator, "Missing '=' after XML attribute name at offset:%u.", state.IndexCurrentChar);
		if_true_fef(state.CharCurrent != '\"' && state.CharCurrent != '\'', "Missing quote for XML attribute at offset:%u.", state.IndexCurrentChar);
		state.Flags.ExpectingSeparator = state.CharCurrent == '\'';
		state.Flags.InsideString = true;
		rtrn ::xmlOpenElement(reader, ::llc::XML_TOKEN_ATTR_VALUE, state.IndexCurrentChar + 1);
	default:
		rtrn -1;
	}
}

sttc ::llc::err_t xmlParseNameCharacter(::llc::SXMLReader & reader, ::llc::vcst_t xmlDoc) {
	::llc::SXMLReaderState & state = reader.State;
	cnst ::llc::XML_TOKEN type = state.CurrentElement->Type;
	if(::llc::is_space(state.CharCurrent)) {
		if_fail_fe(::xmlCloseElement(reader));
		state.Flags.ExpectingSeparator = state.CurrentElement && state.CurrentElement->Type == ::llc::XML_TOKEN_ATTR;
		rtrn 0;
	}
	switch(state.CharCurrent) {
	case '/':
		if_true_fef(type != ::llc::XML_TOKEN_TAG_NAME, "Invalid '/' in XML name at offset:%u.", state.IndexCurrentChar);
		if(state.CurrentElement->Span.Begin == state.IndexCurrentChar) {
			state.IndexCurrentElement = reader.Token.pop_back() - 1;
			state.IndexCurrentElement = reader.Token.pop_back() - 1;
			state.NestLevel -= 2;
			state.CurrentElement = &reader.Token[state.IndexCurrentElement];
			state.CurrentElement->Type = ::llc::XML_TOKEN_TAG_CLOSE;
			rtrn ::xmlOpenElement(reader, ::llc::XML_TOKEN_TAG_NAME, state.IndexCurrentChar + 1);
		}
		if_true_fef(state.IndexCurrentChar + 1 >= xmlDoc.size() || xmlDoc[state.IndexCurrentChar + 1] != '>', "Invalid '/' in XML tag name at offset:%u.", state.IndexCurrentChar);
		reader.Token[state.CurrentElement->ParentIndex].Type = ::llc::XML_TOKEN_TAG_OPENCLOSE;
		rtrn 0;
	case '?':
		if(type == ::llc::XML_TOKEN_PI_NAME) {
			if_fail_fe(::xmlCloseElement(reader));
			state.Flags.ExpectingSeparator = true;
			rtrn 0;
		}
		if_true_fef(type != ::llc::XML_TOKEN_TAG_NAME || state.CurrentElement->Span.Begin != state.IndexCurrentChar, "Invalid '?' in XML name at offset:%u.", state.IndexCurrentChar);
		state.IndexCurrentElement = reader.Token.pop_back() - 1;
		state.IndexCurrentElement = reader.Token.pop_back() - 1;
		state.NestLevel -= 2;
		state.CurrentElement = &reader.Token[state.IndexCurrentElement];
		state.CurrentElement->Type = ::llc::XML_TOKEN_PI;
		rtrn ::xmlOpenElement(reader, ::llc::XML_TOKEN_PI_NAME, state.IndexCurrentChar + 1);
	case '=':
		if_true_fef(type != ::llc::XML_TOKEN_ATTR_NAME, "Invalid '=' in XML name at offset:%u.", state.IndexCurrentChar);
		if_fail_fe(::xmlCloseElement(reader));
		state.Flags.ExpectingSeparator = false;
		rtrn 0;
	case '>': {
		if_true_fef(type == ::llc::XML_TOKEN_PI_NAME || type == ::llc::XML_TOKEN_ATTR_NAME, "Invalid '>' in XML name at offset:%u.", state.IndexCurrentChar);
		if_fail_fe(::xmlCloseElement(reader));
		cnst ::llc::XML_TOKEN ownerType = state.CurrentElement->Type;
		if(ownerType == ::llc::XML_TOKEN_TAG_CLOSE)
			if_fail_fe(::xmlClosingNameValidate(reader, xmlDoc, state.IndexCurrentElement));
		if_fail_fe(::xmlCloseElement(reader, state.IndexCurrentChar + 1));
		if(ownerType == ::llc::XML_TOKEN_TAG_CLOSE || ownerType == ::llc::XML_TOKEN_TAG_OPENCLOSE)
			if_fail_fe(::xmlCloseElement(reader, state.IndexCurrentChar + 1));
		rtrn 0;
	}
	default:
		rtrn 0;
	}
}

sttc ::llc::err_t xmlParseDocumentCharacter(::llc::SXMLReader & reader, ::llc::vcst_t xmlDoc) {
	::llc::SXMLReaderState & state = reader.State;
	if(state.CharCurrent != '<') {
		if(state.CurrentElement->Type == ::llc::XML_TOKEN_DOCUMENT || state.CurrentElement->Type == ::llc::XML_TOKEN_TAG_NODE)
			rtrn ::xmlOpenElement(reader, ::llc::XML_TOKEN_TAG_TEXT);
		if_true_fef(state.CurrentElement->Type != ::llc::XML_TOKEN_TAG_TEXT, "Invalid character '%c' in XML token type:%i.", state.CharCurrent, state.CurrentElement->Type);
		rtrn 0;
	}
	if(state.CurrentElement->Type == ::llc::XML_TOKEN_TAG_TEXT)
		if_fail_fe(::xmlCloseElement(reader));
	if_true_fef(state.CurrentElement->Type != ::llc::XML_TOKEN_DOCUMENT && state.CurrentElement->Type != ::llc::XML_TOKEN_TAG_NODE, "Invalid '<' in XML token type:%i.", state.CurrentElement->Type);
	if(::xmlStartsWith(xmlDoc, state.IndexCurrentChar, "<!")) {
		::llc::u2_t offset = state.IndexCurrentChar;
		if_fail_fef(::xmlSpecialParse(reader, xmlDoc, state.IndexCurrentElement, offset), "Invalid XML special token at offset:%u.", state.IndexCurrentChar);
		state.IndexCurrentChar = offset - 1;
		state.CurrentElement = &reader.Token[state.IndexCurrentElement];
		rtrn 0;
	}
	if_fail_fe(::xmlOpenElement(reader, ::llc::XML_TOKEN_TAG_NODE));
	if_fail_fe(::xmlOpenElement(reader, ::llc::XML_TOKEN_TAG_OPEN));
	rtrn ::xmlOpenElement(reader, ::llc::XML_TOKEN_TAG_NAME, state.IndexCurrentChar + 1);
}

llc::err_t llc::xmlTokenView(::llc::vcst_t xmlDoc, cnst ::llc::SXMLToken & token, ::llc::vcst_t & output) {
	if_true_fef(token.Span.Begin > token.Span.End || token.Span.End > xmlDoc.size(), "XML token span outside document. begin:%u, end:%u, size:%u.", token.Span.Begin, token.Span.End, xmlDoc.size());
	rtrn xmlDoc.slice(output, token.Span.Begin, token.Span.End - token.Span.Begin);
}

llc::err_t llc::xmlNodeName(cnst ::llc::SXMLReader & reader, ::llc::vcst_t xmlDoc, ::llc::u2_t iNode, ::llc::vcst_t & output) {
	if_true_fef(iNode >= reader.Token.size() || reader.Token[iNode].Type != ::llc::XML_TOKEN_TAG_NODE, "Invalid XML node index:%u.", iNode);
	rtrn ::xmlOpeningName(reader, xmlDoc, iNode, output);
}

llc::err_t llc::xmlNodeAttribute(cnst ::llc::SXMLReader & reader, ::llc::vcst_t xmlDoc, ::llc::u2_t iNode, ::llc::vcst_t name, ::llc::vcst_t & output) {
	if_true_fef(iNode >= reader.Token.size() || reader.Token[iNode].Type != ::llc::XML_TOKEN_TAG_NODE, "Invalid XML node index:%u.", iNode);
	for(::llc::u2_t iToken = iNode + 1; iToken < reader.Token.size(); ++iToken) {
		cnst ::llc::SXMLToken & owner	= reader.Token[iToken];
		if(owner.ParentIndex != (::llc::s2_t)iNode || (owner.Type != ::llc::XML_TOKEN_TAG_OPEN && owner.Type != ::llc::XML_TOKEN_TAG_OPENCLOSE))
			continue;
		for(::llc::u2_t iAttribute = iToken + 1; iAttribute < reader.Token.size(); ++iAttribute) {
			cnst ::llc::SXMLToken & attribute = reader.Token[iAttribute];
			if(attribute.ParentIndex != (::llc::s2_t)iToken || attribute.Type != ::llc::XML_TOKEN_ATTR)
				continue;
			::llc::vcst_t attributeName;
			::llc::vcst_t attributeValue;
			::llc::err_t iValue			= -1;
			for(::llc::u2_t iPart = iAttribute + 1; iPart < reader.Token.size(); ++iPart) {
				cnst ::llc::SXMLToken & part = reader.Token[iPart];
				if(part.ParentIndex != (::llc::s2_t)iAttribute)
					continue;
				if(part.Type == ::llc::XML_TOKEN_ATTR_NAME ) if_fail_fe(::llc::xmlTokenView(xmlDoc, part, attributeName));
				if(part.Type == ::llc::XML_TOKEN_ATTR_VALUE) { if_fail_fe(::llc::xmlTokenView(xmlDoc, part, attributeValue)); iValue = iPart; }
			}
			if(::xmlTextEquals(attributeName, name)) {
				output					= attributeValue;
				rtrn iValue;
			}
		}
	}
	rtrn -1;
}

llc::err_t llc::xmlNodeChild(cnst ::llc::SXMLReader & reader, ::llc::vcst_t xmlDoc, ::llc::u2_t iNode, ::llc::vcst_t name) {
	if_true_fef(iNode >= reader.Token.size(), "Invalid XML parent index:%u.", iNode);
	for(::llc::u2_t iToken = iNode + 1; iToken < reader.Token.size(); ++iToken) {
		cnst ::llc::SXMLToken & token	= reader.Token[iToken];
		if(token.ParentIndex != (::llc::s2_t)iNode || token.Type != ::llc::XML_TOKEN_TAG_NODE)
			continue;
		::llc::vcst_t nodeName;
		if(0 <= ::llc::xmlNodeName(reader, xmlDoc, iToken, nodeName) && ::xmlTextEquals(nodeName, name))
			rtrn iToken;
	}
	rtrn -1;
}

llc::err_t llc::xmlNodeText(cnst ::llc::SXMLReader & reader, ::llc::vcst_t xmlDoc, ::llc::u2_t iNode, ::llc::vcst_t & output) {
	if_true_fef(iNode >= reader.Token.size(), "Invalid XML parent index:%u.", iNode);
	for(::llc::u2_t iToken = iNode + 1; iToken < reader.Token.size(); ++iToken)
		if(reader.Token[iToken].ParentIndex == (::llc::s2_t)iNode && reader.Token[iToken].Type == ::llc::XML_TOKEN_TAG_TEXT)
			rtrn ::llc::xmlTokenView(xmlDoc, reader.Token[iToken], output) < 0 ? -1 : (::llc::err_t)iToken;
	rtrn -1;
}

llc::err_t llc::xmlFileRead(::llc::SXMLFile & file, ::llc::vcst_t filename) {
	xml_info_printf("Loading xml file: %.*s.", (int)filename.size(), filename.begin());
	if_fail_fef(::llc::fileToMemory(filename, file.Bytes), "Failed to load XML file:'%.*s'.", (int)filename.size(), filename.begin());
	rtrn ::llc::xmlParse(file.Reader, file.Bytes);
}

llc::err_t llc::xmlParseStep(::llc::SXMLReader & reader, ::llc::vcst_t xmlDoc) {
	::llc::SXMLReaderState & state = reader.State;
	if_true_fef(state.Flags.DoneReading, "%s", "XML reader already completed the document.");
	if(0 == reader.Token.size())
		if_fail_fe(::xmlOpenElement(reader, ::llc::XML_TOKEN_DOCUMENT));
	if_fail_fef(::llc::readerStateParseStep(state, xmlDoc), "Invalid XML index:%u. Document size:%u.", state.IndexCurrentChar, xmlDoc.size());
	switch(state.CurrentElement->Type) {
	case ::llc::XML_TOKEN_ATTR_VALUE:
		if_fail_fe(::xmlParseStringCharacter(reader));
		break;
	case ::llc::XML_TOKEN_TAG_OPEN:
	case ::llc::XML_TOKEN_TAG_CLOSE:
	case ::llc::XML_TOKEN_TAG_OPENCLOSE:
	case ::llc::XML_TOKEN_PI:
	case ::llc::XML_TOKEN_ATTR:
		if_fail_fe(::xmlParseTagCharacter(reader, xmlDoc));
		break;
	case ::llc::XML_TOKEN_TAG_NAME:
	case ::llc::XML_TOKEN_PI_NAME:
	case ::llc::XML_TOKEN_ATTR_NAME:
		if_fail_fe(::xmlParseNameCharacter(reader, xmlDoc));
		break;
	default:
		if_fail_fe(::xmlParseDocumentCharacter(reader, xmlDoc));
		break;
	}
	if(state.IndexCurrentElement >= 0)
		state.CurrentElement = &reader.Token[state.IndexCurrentElement];
	if(state.IndexCurrentChar + 1 == xmlDoc.size()) {
		if(state.CurrentElement && state.CurrentElement->Type == ::llc::XML_TOKEN_TAG_TEXT)
			if_fail_fe(::xmlCloseElement(reader, state.IndexCurrentChar + 1));
		if(state.CurrentElement && state.CurrentElement->Type == ::llc::XML_TOKEN_DOCUMENT)
			if_fail_fe(::xmlCloseElement(reader, state.IndexCurrentChar + 1));
		state.Flags.DoneReading = 0 == state.CurrentElement;
		if(state.Flags.DoneReading)
			state.CharCurrent = 0;
	}
	rtrn 0;
}

llc::err_t llc::xmlParse(::llc::SXMLReader & reader, ::llc::vcst_t xmlDoc) {
	if_fail_fe(reader.Reset());
	if(0 == xmlDoc.size()) {
		if_fail_fe(::xmlOpenElement(reader, ::llc::XML_TOKEN_DOCUMENT, 0));
		if_fail_fe(::xmlCloseElement(reader, 0));
		reader.State.Flags.DoneReading = true;
	}
	for(reader.State.IndexCurrentChar = 0; reader.State.IndexCurrentChar < xmlDoc.size(); ++reader.State.IndexCurrentChar)
		if_fail_fe(::llc::xmlParseStep(reader, xmlDoc));
	if_true_fef(reader.State.NestLevel, "Invalid XML nesting level:%i.", reader.State.NestLevel);
	if_true_fef(false == reader.State.Flags.DoneReading, "%s", "XML document did not reach a closed state.");
	reader.State.IndexCurrentChar = xmlDoc.size();
	reader.State.CharCurrent = 0;
	rtrn reader.Token.size();
}
