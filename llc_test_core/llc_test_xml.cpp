#include "llc_xml_reader.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(XML_READER_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, OK				, 0, "All XML reader tests passed.");
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, PARSE				, 1, "xmlParse() rejected a valid XML document.");
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, TOKEN_RANGE			, 2, "An XML token range escaped its source document.");
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, TOKEN_PARENT			, 3, "An XML token referenced an invalid parent.");
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, READER_STATE			, 4, "The XML reader did not finish a valid document in a closed state.");
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, CLOSING_TAG			, 5, "The XML reader accepted a mismatched closing tag.");
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, COUNTED_INPUT			, 6, "xmlParse() depended on a terminator beyond counted input.");
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, RESET_REUSE			, 7, "The XML reader did not clear and reuse its token storage.");

sttc ::llc::err_t testXMLDocument
	(ATestError & errors, ::llc::vcsc_t input, XML_READER_TEST_RESULT parseTest = XML_READER_TEST_RESULT_PARSE) {
	::llc::SXMLReader reader;
	cnst ::llc::err_t parseResult = ::llc::xmlParse(reader, input);
	LLC_TEST_REQUIRE(errors, parseTest, ::llc::failed(parseResult)
		, "Valid XML parse failed. result:%i, input:'%.*s'."
		, parseResult, (int)input.size(), input.begin()
		);
	for(::llc::u2_t iToken = 0; iToken < reader.Token.size(); ++iToken) {
		cnst ::llc::SXMLToken & token = reader.Token[iToken];
		LLC_TEST_CHECK(errors, XML_READER_TEST_RESULT_TOKEN_RANGE, token.Range.Offset > input.size() || token.Range.Count > input.size() - token.Range.Offset
			, "Token %u escaped input. type:%i, offset:%u, count:%u, input:%u, document:'%.*s'."
			, iToken, (::llc::s2_t)token.Type, token.Range.Offset, token.Range.Count, input.size()
			, (int)input.size(), input.begin()
			);
		LLC_TEST_CHECK(errors, XML_READER_TEST_RESULT_TOKEN_PARENT
			, (0 == iToken) ? -1 != token.Parent : token.Parent < 0 || (::llc::u2_t)token.Parent >= iToken
			, "Token %u has invalid parent:%i. type:%i, document:'%.*s'."
			, iToken, token.Parent, (::llc::s2_t)token.Type, (int)input.size(), input.begin()
			);
	}
	LLC_TEST_CHECK(errors, XML_READER_TEST_RESULT_READER_STATE
		, reader.StateRead.IndexCurrentChar != input.size() || reader.StateRead.IndexCurrentElement != -1
		|| reader.StateRead.CurrentElement || reader.StateRead.NestLevel || reader.StateRead.CharCurrent
		, "Reader final state mismatch. position:%u/%u, element:%i, current:%p, level:%u, character:%i, document:'%.*s'."
		, reader.StateRead.IndexCurrentChar, input.size(), reader.StateRead.IndexCurrentElement
		, reader.StateRead.CurrentElement, reader.StateRead.NestLevel, reader.StateRead.CharCurrent
		, (int)input.size(), input.begin()
		);
	rtrn 0;
}

sttc ::llc::err_t testXMLDocuments(ATestError & errors) {
	cnst ::llc::sc_t countedInput[] = {'<', 'A', '/', '>'};
	cnst ::llc::vcsc_t documents[] =
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
	LLC_TEST_CHECK(errors, XML_READER_TEST_RESULT_CLOSING_TAG, false == ::llc::failed(result), "Mismatched closing tag accepted. result:%i.", result);
	rtrn 0;
}

sttc ::llc::err_t testXMLStorage(ATestError & errors) {
	cnst ::llc::sc_t countedInput[] = {'<', 'A', '/', '>'};
	::llc::SXMLReader reader;
	if_fail_fe(::llc::xmlParse(reader, {countedInput}));
	cnst ::llc::view<const ::llc::SXMLToken> storage = {reader.Token.begin(), reader.Token.size()};
	cnst ::llc::err_t resetResult = reader.Reset();
	cnst ::llc::err_t parseResult = ::llc::xmlParse(reader, {countedInput});
	LLC_TEST_CHECK(errors, XML_READER_TEST_RESULT_RESET_REUSE
		, resetResult || ::llc::failed(parseResult) || reader.Token.begin() != storage.begin() || reader.StateRead.IndexCurrentElement != -1 || reader.StateRead.CurrentElement || reader.StateRead.NestLevel
		, "Reader reuse mismatch. reset:%i, parse:%i, storage:%p/%p, index:%i, current:%p, level:%u."
		, resetResult, parseResult, reader.Token.begin(), storage.begin(), reader.StateRead.IndexCurrentElement, reader.StateRead.CurrentElement, reader.StateRead.NestLevel
		);
	rtrn 0;
}

::llc::err_t testXMLReader(ATestError & errors) {
	cnst ::llc::u2_t failureCountBefore = testErrorCount(errors);
	if_fail_fe(::testXMLDocuments(errors));
	if_fail_fe(::testXMLFailures(errors));
	if_fail_fe(::testXMLStorage(errors));
	LLC_TEST_CHECK(errors, XML_READER_TEST_RESULT_OK, testErrorCount(errors) != failureCountBefore, "%s", "Earlier XML reader checks failed.");
	rtrn 0;
}
