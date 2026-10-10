#include "llc_xml_reader.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(XML_READER_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, OK				, 0, "All XML reader tests passed.");
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, PARSE			, 1, "xmlParse() rejected a valid XML document.");
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, TOKEN_RANGE		, 2, "An XML token range escaped its source document.");
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, TOKEN_PARENT	, 3, "An XML token referenced an invalid parent.");
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, READER_STATE	, 4, "The XML reader did not finish a valid document in a closed state.");
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, CLOSING_TAG		, 5, "The XML reader accepted a mismatched closing tag.");
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, COUNTED_INPUT	, 6, "xmlParse() depended on a terminator beyond counted input.");
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, RESET_REUSE		, 7, "The XML reader did not clear and reuse its token storage.");

tydf ::llc::err_t (*TXMLParse)(::llc::SXMLReader &, ::llc::vcst_t);
tydf ::llc::err_t (*TXMLTokenView)(::llc::vcst_t, cnst ::llc::SXMLToken &, ::llc::vcst_t &);
tydf ::llc::err_t (*TXMLNodeView)(cnst ::llc::SXMLReader &, ::llc::vcst_t, ::llc::u2_t, ::llc::vcst_t &);
tydf ::llc::err_t (*TXMLNodeAttribute)(cnst ::llc::SXMLReader &, ::llc::vcst_t, ::llc::u2_t, ::llc::vcst_t, ::llc::vcst_t &);
tydf ::llc::err_t (*TXMLNodeChild)(cnst ::llc::SXMLReader &, ::llc::vcst_t, ::llc::u2_t, ::llc::vcst_t);
tydf ::llc::err_t (*TXMLFileRead)(::llc::SXMLFile &, ::llc::vcst_t);

static_assert(::llc::is_cnst<::llc::vcst_t::T>::Value, "XML string views must expose const characters.");
static_assert(requires {
	static_cast<TXMLParse>(&::llc::xmlParse);
	static_cast<TXMLTokenView>(&::llc::xmlTokenView);
	static_cast<TXMLNodeView>(&::llc::xmlNodeName);
	static_cast<TXMLNodeAttribute>(&::llc::xmlNodeAttribute);
	static_cast<TXMLNodeChild>(&::llc::xmlNodeChild);
	static_cast<TXMLNodeView>(&::llc::xmlNodeText);
	static_cast<TXMLFileRead>(&::llc::xmlFileRead);
	}, "XML operations must preserve their string-view contracts.");

sttc ::llc::err_t testXMLDocument
	(ATestError & errors, ::llc::vcst_t input, XML_READER_TEST_RESULT parseTest = XML_READER_TEST_RESULT_PARSE) {
	::llc::SXMLReader reader;
	cnst ::llc::err_t parseResult = ::llc::xmlParse(reader, input);
	LLC_TEST_REQUIREF(errors, parseTest, ::llc::failed(parseResult) , "Valid XML parse failed. result:%i, input:'%.*s'." , parseResult, (int)input.size(), input.begin() );
	for(::llc::u2_t iToken = 0; iToken < reader.Token.size(); ++iToken) {
		cnst ::llc::SXMLToken & token = reader.Token[iToken];
		LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_TOKEN_RANGE, token.Range.Offset > input.size() , "Token:%u offset:%u exceeds input:%u. Document:'%.*s'." , iToken, token.Range.Offset, input.size(), (int)input.size(), input.begin() );
		if(token.Range.Offset <= input.size()) {
			LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_TOKEN_RANGE, token.Range.Count > input.size() - token.Range.Offset , "Token:%u count:%u exceeds remaining input:%u. Document:'%.*s'." , iToken, token.Range.Count, input.size() - token.Range.Offset, (int)input.size(), input.begin() );
		}
		if(0 == iToken) {
			LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_TOKEN_PARENT, -1 != token.Parent , "Root token parent:%i, expected:-1. Document:'%.*s'." , token.Parent, (int)input.size(), input.begin() );
		}
		else {
			LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_TOKEN_PARENT, token.Parent < 0 , "Token:%u parent:%i, expected nonnegative. Document:'%.*s'." , iToken, token.Parent, (int)input.size(), input.begin() );
			if(0 <= token.Parent) {
				LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_TOKEN_PARENT, (::llc::u2_t)token.Parent >= iToken , "Token:%u parent:%i is not earlier. Document:'%.*s'." , iToken, token.Parent, (int)input.size(), input.begin() );
			}
		}
	}
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_READER_STATE, reader.StateRead.IndexCurrentChar != input.size() , "Final position:%u, expected:%u. Document:'%.*s'." , reader.StateRead.IndexCurrentChar, input.size(), (int)input.size(), input.begin() );
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_READER_STATE, reader.StateRead.IndexCurrentElement != -1 , "Final element index:%i, expected:-1. Document:'%.*s'." , reader.StateRead.IndexCurrentElement, (int)input.size(), input.begin() );
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_READER_STATE, reader.StateRead.CurrentElement , "Final current element:%p, expected:null. Document:'%.*s'." , reader.StateRead.CurrentElement, (int)input.size(), input.begin() );
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_READER_STATE, reader.StateRead.NestLevel , "Final nesting level:%u, expected:0. Document:'%.*s'." , reader.StateRead.NestLevel, (int)input.size(), input.begin() );
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_READER_STATE, reader.StateRead.CharCurrent , "Final character:%i, expected:0. Document:'%.*s'." , reader.StateRead.CharCurrent, (int)input.size(), input.begin() );
	rtrn 0;
}

sttc ::llc::err_t testXMLDocuments(ATestError & errors) {
	cnst ::llc::sc_t countedInput[] = {'<', 'A', '/', '>'};
	cnst ::llc::vcst_t documents[] =
		{ LLC_CXS("<Root/>")
		, LLC_CXS("<?xml version=\"1.0\"?><Root attribute=\"value\"><Child>text</Child><Child/></Root>")
		, LLC_CXS("<Root single='quoted'><!--comment--><![CDATA[<raw>]]></Root>")
		, LLC_CXS("<!DOCTYPE Root><Root/>")
		};
	for(::llc::u2_t iDocument = 0; iDocument < ::llc::size(documents); ++iDocument)
		if_fail_fe(::testXMLDocument(errors, documents[iDocument]));
	rtrn ::testXMLDocument(errors, {countedInput}, XML_READER_TEST_RESULT_COUNTED_INPUT);
}

sttc ::llc::err_t testXMLFailures(ATestError & errors) {
	::llc::SXMLReader reader;
	cnst ::llc::err_t result = ::llc::xmlParse(reader, LLC_CXS("<Root><Child></Root>"));
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_CLOSING_TAG, false == ::llc::failed(result), "Mismatched closing tag accepted. result:%i.", result);
	rtrn 0;
}

sttc ::llc::err_t testXMLStorage(ATestError & errors) {
	cnst ::llc::sc_t countedInput[] = {'<', 'A', '/', '>'};
	::llc::SXMLReader reader;
	if_fail_fe(::llc::xmlParse(reader, {countedInput}));
	cnst ::llc::view<const ::llc::SXMLToken> storage = {reader.Token.begin(), reader.Token.size()};
	cnst ::llc::err_t resetResult = reader.Reset();
	cnst ::llc::err_t parseResult = ::llc::xmlParse(reader, {countedInput});
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_RESET_REUSE, resetResult , "Reader reset result:%i, expected:0.", resetResult );
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_RESET_REUSE, ::llc::failed(parseResult) , "Reader reuse parse result:%i, expected success.", parseResult );
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_RESET_REUSE, reader.Token.begin() != storage.begin() , "Reader reuse storage:%p, expected:%p.", reader.Token.begin(), storage.begin() );
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_RESET_REUSE, reader.StateRead.IndexCurrentElement != -1 , "Reader reuse element index:%i, expected:-1.", reader.StateRead.IndexCurrentElement );
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_RESET_REUSE, reader.StateRead.CurrentElement , "Reader reuse current element:%p, expected:null.", reader.StateRead.CurrentElement );
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_RESET_REUSE, reader.StateRead.NestLevel , "Reader reuse nesting level:%u, expected:0.", reader.StateRead.NestLevel );
	rtrn 0;
}

::llc::err_t testXMLReader(ATestError & errors) {
	cnst ::llc::u2_t failureCountBefore = testErrorCount(errors);
	if_fail_fe(::testXMLDocuments(errors));
	if_fail_fe(::testXMLFailures(errors));
	if_fail_fe(::testXMLStorage(errors));
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_OK, testErrorCount(errors) != failureCountBefore, "%s", "Earlier XML reader checks failed.");
	rtrn 0;
}
