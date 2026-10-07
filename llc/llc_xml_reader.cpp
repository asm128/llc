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
	rtrn reader.Token.push_back({type, {offset, count}, parent});
}

sttc ::llc::err_t xmlNameEnd(::llc::vcst_t xmlDoc, ::llc::u2_t offset, ::llc::u2_t & end) {
	end						= offset;
	while(end < xmlDoc.size()) {
		cnst ::llc::sc_t current	= xmlDoc[end];
		if(::llc::isSpaceCharacter(current) || current == '/' || current == '>' || current == '?' || current == '=')
			break;
		++end;
	}
	if_true_fef(end == offset, "Missing XML name at offset:%u.", offset);
	rtrn 0;
}

sttc ::llc::err_t xmlSkipSpaces(::llc::vcst_t xmlDoc, ::llc::u2_t & offset) {
	while(offset < xmlDoc.size() && ::llc::isSpaceCharacter(xmlDoc[offset]))
		++offset;
	rtrn offset;
}

sttc ::llc::err_t xmlAttributesParse
	(::llc::SXMLReader & reader, ::llc::vcst_t xmlDoc, ::llc::u2_t iOwner, ::llc::u2_t & offset, bool processingInstruction, bool & selfClosing) {
	selfClosing					= false;
	while(offset < xmlDoc.size()) {
		if_fail_fe(::xmlSkipSpaces(xmlDoc, offset));
		if(processingInstruction) {
			if(::xmlStartsWith(xmlDoc, offset, LLC_CXS("?>"))) {
				offset					+= 2;
				rtrn 0;
			}
		}
		else {
			if(::xmlStartsWith(xmlDoc, offset, LLC_CXS("/>"))) {
				selfClosing				= true;
				offset					+= 2;
				rtrn 0;
			}
			if(offset < xmlDoc.size() && xmlDoc[offset] == '>') {
				++offset;
				rtrn 0;
			}
		}

		cnst ::llc::u2_t iAttribute	= offset;
		::llc::u2_t iNameEnd			= {};
		if_fail_fe(::xmlNameEnd(xmlDoc, offset, iNameEnd));
		offset						= iNameEnd;
		if_fail_fe(::xmlSkipSpaces(xmlDoc, offset));
		if_true_fef(offset >= xmlDoc.size() || xmlDoc[offset] != '=', "Missing '=' after XML attribute at offset:%u.", iAttribute);
		if_fail_fe(::xmlSkipSpaces(xmlDoc, ++offset));
		if_true_fef(offset >= xmlDoc.size() || (xmlDoc[offset] != '\"' && xmlDoc[offset] != '\''), "Missing quote for XML attribute at offset:%u.", iAttribute);
		cnst ::llc::sc_t quote			= xmlDoc[offset++];
		cnst ::llc::u2_t iValue		= offset;
		while(offset < xmlDoc.size() && xmlDoc[offset] != quote)
			++offset;
		if_true_fef(offset >= xmlDoc.size(), "Unclosed XML attribute at offset:%u.", iAttribute);
		cnst ::llc::u2_t iAttributeEnd	= ++offset;
		cnst ::llc::err_t iToken		= ::xmlTokenAdd(reader, ::llc::XML_TOKEN_ATTR, iAttribute, iAttributeEnd - iAttribute, (::llc::s2_t)iOwner);
		if_fail_fe(iToken);
		if_fail_fe(::xmlTokenAdd(reader, ::llc::XML_TOKEN_ATTR_NAME , iAttribute, iNameEnd - iAttribute, iToken));
		if_fail_fe(::xmlTokenAdd(reader, ::llc::XML_TOKEN_ATTR_VALUE, iValue, iAttributeEnd - iValue - 1, iToken));
	}
	rtrn -1;
}

sttc ::llc::err_t xmlOpeningName(cnst ::llc::SXMLReader & reader, ::llc::vcst_t xmlDoc, ::llc::u2_t iNode, ::llc::vcst_t & output) {
	for(::llc::u2_t iToken = iNode + 1; iToken < reader.Token.size(); ++iToken) {
		cnst ::llc::SXMLToken & token	= reader.Token[iToken];
		if(token.Parent == (::llc::s2_t)iNode && (token.Type == ::llc::XML_TOKEN_TAG_OPEN || token.Type == ::llc::XML_TOKEN_TAG_OPENCLOSE)) {
			for(::llc::u2_t iName = iToken + 1; iName < reader.Token.size(); ++iName)
				if(reader.Token[iName].Parent == (::llc::s2_t)iToken && reader.Token[iName].Type == ::llc::XML_TOKEN_TAG_NAME)
					rtrn ::llc::xmlTokenView(xmlDoc, reader.Token[iName], output) < 0 ? -1 : (::llc::err_t)iName;
		}
	}
	rtrn -1;
}

sttc ::llc::err_t xmlSpecialParse(::llc::SXMLReader & reader, ::llc::vcst_t xmlDoc, ::llc::u2_t iParent, ::llc::u2_t & offset) {
	cnst ::llc::u2_t iBegin		= offset;
	::llc::XML_TOKEN type			= ::llc::XML_TOKEN_DOCTYPE;
	::llc::vcst_t terminator		= LLC_CXS(">");
	if(::xmlStartsWith(xmlDoc, offset, LLC_CXS("<!--"))) {
		type						= ::llc::XML_TOKEN_COMMENT;
		terminator					= LLC_CXS("-->");
		offset						+= 4;
	}
	else if(::xmlStartsWith(xmlDoc, offset, LLC_CXS("<![CDATA["))) {
		type						= ::llc::XML_TOKEN_CDATA;
		terminator					= LLC_CXS("]]>");
		offset						+= 9;
	}
	else if(::xmlStartsWith(xmlDoc, offset, LLC_CXS("<!DOCTYPE")))
		offset						+= 9;
	else
		rtrn -1;
	cnst ::llc::err_t iEnd			= ::llc::find_sequence_pod(terminator, xmlDoc, offset);
	if_fail_fef(iEnd, "Unclosed XML special token at offset:%u.", iBegin);
	offset						= (::llc::u2_t)iEnd + terminator.size();
	rtrn ::xmlTokenAdd(reader, type, iBegin, offset - iBegin, (::llc::s2_t)iParent) < 0 ? -1 : 0;
}

llc::err_t llc::xmlTokenView(::llc::vcst_t xmlDoc, cnst ::llc::SXMLToken & token, ::llc::vcst_t & output) {
	if_true_fef(token.Range.Offset > xmlDoc.size() || token.Range.Count > xmlDoc.size() - token.Range.Offset, "XML token range outside document. offset:%u, count:%u, size:%u.", token.Range.Offset, token.Range.Count, xmlDoc.size());
	rtrn xmlDoc.slice(output, token.Range.Offset, token.Range.Count);
}

llc::err_t llc::xmlNodeName(cnst ::llc::SXMLReader & reader, ::llc::vcst_t xmlDoc, ::llc::u2_t iNode, ::llc::vcst_t & output) {
	if_true_fef(iNode >= reader.Token.size() || reader.Token[iNode].Type != ::llc::XML_TOKEN_TAG_NODE, "Invalid XML node index:%u.", iNode);
	rtrn ::xmlOpeningName(reader, xmlDoc, iNode, output);
}

llc::err_t llc::xmlNodeAttribute(cnst ::llc::SXMLReader & reader, ::llc::vcst_t xmlDoc, ::llc::u2_t iNode, ::llc::vcst_t name, ::llc::vcst_t & output) {
	if_true_fef(iNode >= reader.Token.size() || reader.Token[iNode].Type != ::llc::XML_TOKEN_TAG_NODE, "Invalid XML node index:%u.", iNode);
	for(::llc::u2_t iToken = iNode + 1; iToken < reader.Token.size(); ++iToken) {
		cnst ::llc::SXMLToken & owner	= reader.Token[iToken];
		if(owner.Parent != (::llc::s2_t)iNode || (owner.Type != ::llc::XML_TOKEN_TAG_OPEN && owner.Type != ::llc::XML_TOKEN_TAG_OPENCLOSE))
			continue;
		for(::llc::u2_t iAttribute = iToken + 1; iAttribute < reader.Token.size(); ++iAttribute) {
			cnst ::llc::SXMLToken & attribute = reader.Token[iAttribute];
			if(attribute.Parent != (::llc::s2_t)iToken || attribute.Type != ::llc::XML_TOKEN_ATTR)
				continue;
			::llc::vcst_t attributeName;
			::llc::vcst_t attributeValue;
			::llc::err_t iValue			= -1;
			for(::llc::u2_t iPart = iAttribute + 1; iPart < reader.Token.size(); ++iPart) {
				cnst ::llc::SXMLToken & part = reader.Token[iPart];
				if(part.Parent != (::llc::s2_t)iAttribute)
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
		if(token.Parent != (::llc::s2_t)iNode || token.Type != ::llc::XML_TOKEN_TAG_NODE)
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
		if(reader.Token[iToken].Parent == (::llc::s2_t)iNode && reader.Token[iToken].Type == ::llc::XML_TOKEN_TAG_TEXT)
			rtrn ::llc::xmlTokenView(xmlDoc, reader.Token[iToken], output) < 0 ? -1 : (::llc::err_t)iToken;
	rtrn -1;
}

llc::err_t llc::xmlFileRead(::llc::SXMLFile & file, ::llc::vcst_t filename) {
	xml_info_printf("Loading xml file: %.*s.", (int)filename.size(), filename.begin());
	if_fail_fef(::llc::fileToMemory(filename, file.Bytes), "Failed to load XML file:'%.*s'.", (int)filename.size(), filename.begin());
	rtrn ::llc::xmlParse(file.Reader, file.Bytes);
}

llc::err_t llc::xmlParse(::llc::SXMLReader & reader, ::llc::vcst_t xmlDoc) {
	if_fail_fe(reader.Reset());
	cnst ::llc::err_t iDocument	= ::xmlTokenAdd(reader, ::llc::XML_TOKEN_DOCUMENT, 0, xmlDoc.size(), -1);
	if_fail_fe(iDocument);
	::llc::u2_t iCurrentNode		= (::llc::u2_t)iDocument;
	::llc::u2_t offset				= 0;
	while(offset < xmlDoc.size()) {
		reader.StateRead.IndexCurrentChar = offset;
		reader.StateRead.CharCurrent	= xmlDoc[offset];
		if(xmlDoc[offset] != '<') {
			cnst ::llc::u2_t iText		= offset;
			while(offset < xmlDoc.size() && xmlDoc[offset] != '<')
				++offset;
			if(offset > iText)
				if_fail_fe(::xmlTokenAdd(reader, ::llc::XML_TOKEN_TAG_TEXT, iText, offset - iText, (::llc::s2_t)iCurrentNode));
			continue;
		}
		if(::xmlStartsWith(xmlDoc, offset, LLC_CXS("<!"))) {
			if_fail_fef(::xmlSpecialParse(reader, xmlDoc, iCurrentNode, offset), "Invalid XML special token at offset:%u.", offset);
			continue;
		}
		if(::xmlStartsWith(xmlDoc, offset, LLC_CXS("<?"))) {
			cnst ::llc::u2_t iBegin	= offset;
			offset					+= 2;
			::llc::u2_t iNameEnd		= {};
			if_fail_fe(::xmlNameEnd(xmlDoc, offset, iNameEnd));
			cnst ::llc::err_t iPI		= ::xmlTokenAdd(reader, ::llc::XML_TOKEN_PI, iBegin, 0, (::llc::s2_t)iCurrentNode);
			if_fail_fe(iPI);
			if_fail_fe(::xmlTokenAdd(reader, ::llc::XML_TOKEN_PI_NAME, offset, iNameEnd - offset, iPI));
			offset					= iNameEnd;
			bool ignored				= false;
			if_fail_fe(::xmlAttributesParse(reader, xmlDoc, iPI, offset, true, ignored));
			reader.Token[iPI].Range.Count = offset - iBegin;
			continue;
		}
		if(::xmlStartsWith(xmlDoc, offset, LLC_CXS("</"))) {
			if_true_fef(iCurrentNode == (::llc::u2_t)iDocument, "Closing XML tag without an open node at offset:%u.", offset);
			cnst ::llc::u2_t iBegin	= offset;
			offset					+= 2;
			::llc::u2_t iNameEnd		= {};
			if_fail_fe(::xmlNameEnd(xmlDoc, offset, iNameEnd));
			::llc::vcst_t openingName;
			if_fail_fef(::xmlOpeningName(reader, xmlDoc, iCurrentNode, openingName), "Open XML node has no name at token:%u.", iCurrentNode);
			::llc::vcst_t closingName	= {&xmlDoc[offset], iNameEnd - offset};
			if_true_fef(false == ::xmlTextEquals(openingName, closingName), "Mismatched XML closing tag:'%.*s'; expected:'%.*s'.", (int)closingName.size(), closingName.begin(), (int)openingName.size(), openingName.begin());
			offset					= iNameEnd;
			if_fail_fe(::xmlSkipSpaces(xmlDoc, offset));
			if_true_fef(offset >= xmlDoc.size() || xmlDoc[offset] != '>', "Missing '>' for closing XML tag at offset:%u.", iBegin);
			++offset;
			cnst ::llc::err_t iClose	= ::xmlTokenAdd(reader, ::llc::XML_TOKEN_TAG_CLOSE, iBegin, offset - iBegin, (::llc::s2_t)iCurrentNode);
			if_fail_fe(iClose);
			if_fail_fe(::xmlTokenAdd(reader, ::llc::XML_TOKEN_TAG_NAME, iBegin + 2, iNameEnd - iBegin - 2, iClose));
			reader.Token[iCurrentNode].Range.Count = offset - reader.Token[iCurrentNode].Range.Offset;
			iCurrentNode				= (::llc::u2_t)reader.Token[iCurrentNode].Parent;
			--reader.StateRead.NestLevel;
			continue;
		}

		cnst ::llc::u2_t iBegin		= offset++;
		::llc::u2_t iNameEnd			= {};
		if_fail_fe(::xmlNameEnd(xmlDoc, offset, iNameEnd));
		cnst ::llc::err_t iNode		= ::xmlTokenAdd(reader, ::llc::XML_TOKEN_TAG_NODE, iBegin, 0, (::llc::s2_t)iCurrentNode);
		if_fail_fe(iNode);
		cnst ::llc::err_t iOpen		= ::xmlTokenAdd(reader, ::llc::XML_TOKEN_TAG_OPEN, iBegin, 0, iNode);
		if_fail_fe(iOpen);
		if_fail_fe(::xmlTokenAdd(reader, ::llc::XML_TOKEN_TAG_NAME, offset, iNameEnd - offset, iOpen));
		offset						= iNameEnd;
		bool selfClosing				= false;
		if_fail_fe(::xmlAttributesParse(reader, xmlDoc, iOpen, offset, false, selfClosing));
		reader.Token[iOpen].Range.Count = offset - iBegin;
		if(selfClosing) {
			reader.Token[iOpen].Type	= ::llc::XML_TOKEN_TAG_OPENCLOSE;
			reader.Token[iNode].Range.Count = offset - iBegin;
		}
		else {
			iCurrentNode				= (::llc::u2_t)iNode;
			++reader.StateRead.NestLevel;
		}
	}
	if_true_fef(iCurrentNode != (::llc::u2_t)iDocument, "Unclosed XML node at token:%u.", iCurrentNode);
	reader.StateRead.IndexCurrentChar = xmlDoc.size();
	reader.StateRead.IndexCurrentElement = -1;
	reader.StateRead.CurrentElement = 0;
	reader.StateRead.NestLevel	= 0;
	reader.StateRead.CharCurrent	= 0;
	rtrn reader.Token.size();
}
