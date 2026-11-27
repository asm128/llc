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
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, STEP_PARSE		, 8, "xmlParseStep() did not reproduce xmlParse().");

tydf ::llc::err_t (*TXMLParse)(::llc::SXMLReader &, ::llc::vcst_t);
tydf ::llc::err_t (*TXMLTokenView)(::llc::vcst_t, cnst ::llc::SXMLToken &, ::llc::vcst_t &);
tydf ::llc::err_t (*TXMLNodeView)(cnst ::llc::SXMLReader &, ::llc::vcst_t, ::llc::u2_t, ::llc::vcst_t &);
tydf ::llc::err_t (*TXMLNodeAttribute)(cnst ::llc::SXMLReader &, ::llc::vcst_t, ::llc::u2_t, ::llc::vcst_t, ::llc::vcst_t &);
tydf ::llc::err_t (*TXMLNodeChild)(cnst ::llc::SXMLReader &, ::llc::vcst_t, ::llc::u2_t, ::llc::vcst_t);
tydf ::llc::err_t (*TXMLFileRead)(::llc::SXMLFile &, ::llc::vcst_t);

static_assert(::llc::is_cnst<::llc::vcst_t::T>::Value, "XML string views must expose const characters.");
static_assert(requires {
	static_cast<TXMLParse>(&::llc::xmlParse);
	static_cast<TXMLParse>(&::llc::xmlParseStep);
	static_cast<TXMLTokenView>(&::llc::xmlTokenView);
	static_cast<TXMLNodeView>(&::llc::xmlNodeName);
	static_cast<TXMLNodeAttribute>(&::llc::xmlNodeAttribute);
	static_cast<TXMLNodeChild>(&::llc::xmlNodeChild);
	static_cast<TXMLNodeView>(&::llc::xmlNodeText);
	static_cast<TXMLFileRead>(&::llc::xmlFileRead);
	}, "XML operations must preserve their string-view contracts.");

sttc ::llc::err_t testXMLDocument
	(::llc::ATestError & errors, ::llc::vcst_t input, XML_READER_TEST_RESULT parseTest = XML_READER_TEST_RESULT_PARSE) {
	::llc::SXMLReader reader;
	cnst ::llc::err_t parseResult = ::llc::xmlParse(reader, input);
	always_printf("XML input (%u bytes):\n%.*s\nExpected parse success; return:%i, tokens:%u.", input.size(), LLC_VIEW_ARGS(input), parseResult, reader.Token.size());
	LLC_TEST_REQUIREF(errors, parseTest, ::llc::failed(parseResult) , "Valid XML parse failed. result:%i, input:'%.*s'." , parseResult, (int)input.size(), input.begin() );
	for(::llc::u2_t iToken = 0; iToken < reader.Token.size(); ++iToken) {
		cnst ::llc::SXMLToken & token = reader.Token[iToken];
		always_printf("XML token:%u parent:%i begin:%u end:%u.", iToken, token.ParentIndex, token.Span.Begin, token.Span.End);
		if(token.Span.Begin <= token.Span.End && token.Span.End <= input.size())
			always_printf("Token text:'%.*s'.", (int)(token.Span.End - token.Span.Begin), input.begin() + token.Span.Begin);
		LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_TOKEN_RANGE, token.Span.Begin > token.Span.End || token.Span.End > input.size() , "Token:%u span:%u..%u exceeds input:%u. Document:'%.*s'." , iToken, token.Span.Begin, token.Span.End, input.size(), (int)input.size(), input.begin() );
		if(0 == iToken) {
			LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_TOKEN_PARENT, -1 != token.ParentIndex , "Root token parent:%i, expected:-1. Document:'%.*s'." , token.ParentIndex, (int)input.size(), input.begin() );
		}
		else {
			LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_TOKEN_PARENT, token.ParentIndex < 0 , "Token:%u parent:%i, expected nonnegative. Document:'%.*s'." , iToken, token.ParentIndex, (int)input.size(), input.begin() );
			if(0 <= token.ParentIndex) {
				LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_TOKEN_PARENT, (::llc::u2_t)token.ParentIndex >= iToken , "Token:%u parent:%i is not earlier. Document:'%.*s'." , iToken, token.ParentIndex, (int)input.size(), input.begin() );
			}
		}
	}
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_READER_STATE, reader.State.IndexCurrentChar != input.size() , "Final position:%u, expected:%u. Document:'%.*s'." , reader.State.IndexCurrentChar, input.size(), (int)input.size(), input.begin() );
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_READER_STATE, reader.State.IndexCurrentElement != -1 , "Final element index:%i, expected:-1. Document:'%.*s'." , reader.State.IndexCurrentElement, (int)input.size(), input.begin() );
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_READER_STATE, reader.State.CurrentElement , "Final current element:%p, expected:null. Document:'%.*s'." , reader.State.CurrentElement, (int)input.size(), input.begin() );
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_READER_STATE, reader.State.NestLevel , "Final nesting level:%u, expected:0. Document:'%.*s'." , reader.State.NestLevel, (int)input.size(), input.begin() );
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_READER_STATE, reader.State.CharCurrent , "Final character:%i, expected:0. Document:'%.*s'." , reader.State.CharCurrent, (int)input.size(), input.begin() );
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_READER_STATE, false == reader.State.Flags.DoneReading , "Final done flag:%s, expected:true. Document:'%.*s'." , reader.State.Flags.DoneReading ? "true" : "false", (int)input.size(), input.begin() );
	rtrn 0;
}

sttc ::llc::err_t testXMLDocuments(::llc::ATestError & errors) {
	cnst ::llc::sc_t countedInput[] = {'<', 'A', '/', '>'};
	cnst ::llc::vcst_t documents[] =
		{ "<Root/>"
		, "<?xml?><Root/>"
		, "<?xml version=\"1.0\"?><Root attribute=\"value\"><Child>text</Child><Child/></Root>"
		, "<Root single='quoted'><!--comment--><![CDATA[<raw>]]></Root>"
		, "<!DOCTYPE Root><Root/>"
		};
	for(::llc::u2_t iDocument = 0; iDocument < ::llc::size(documents); ++iDocument)
		if_fail_fe(::testXMLDocument(errors, documents[iDocument]));
	rtrn ::testXMLDocument(errors, {countedInput}, XML_READER_TEST_RESULT_COUNTED_INPUT);
}

sttc ::llc::err_t testXMLFailures(::llc::ATestError & errors) {
	::llc::SXMLReader reader;
	cnst ::llc::err_t result = ::llc::xmlParse(reader, "<Root><Child></Root>");
	always_printf("XML mismatched tags: '<Root><Child></Root>'; expected rejection; result:%i.", result);
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_CLOSING_TAG, false == ::llc::failed(result), "Mismatched closing tag accepted. result:%i.", result);
	rtrn 0;
}

sttc ::llc::err_t testXMLStorage(::llc::ATestError & errors) {
	cnst ::llc::sc_t countedInput[] = {'<', 'A', '/', '>'};
	::llc::SXMLReader reader;
	if_fail_fe(::llc::xmlParse(reader, {countedInput}));
	cnst ::llc::view<const ::llc::SXMLToken> storage = {reader.Token.begin(), reader.Token.size()};
	cnst ::llc::err_t resetResult = reader.Reset();
	cnst ::llc::err_t parseResult = ::llc::xmlParse(reader, {countedInput});
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_RESET_REUSE, resetResult , "Reader reset result:%i, expected:0.", resetResult );
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_RESET_REUSE, ::llc::failed(parseResult) , "Reader reuse parse result:%i, expected success.", parseResult );
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_RESET_REUSE, reader.Token.begin() != storage.begin() , "Reader reuse storage:%p, expected:%p.", reader.Token.begin(), storage.begin() );
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_RESET_REUSE, reader.State.IndexCurrentElement != -1 , "Reader reuse element index:%i, expected:-1.", reader.State.IndexCurrentElement );
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_RESET_REUSE, reader.State.CurrentElement , "Reader reuse current element:%p, expected:null.", reader.State.CurrentElement );
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_RESET_REUSE, reader.State.NestLevel , "Reader reuse nesting level:%u, expected:0.", reader.State.NestLevel );
	rtrn 0;
}

sttc ::llc::err_t testXMLStep(::llc::ATestError & errors) {
	cnst ::llc::vcst_t input {"<?xml version=\"1.0\"?><Root single='quoted'><!--comment--><![CDATA[<raw>]]><Child>text</Child><Child/></Root>"};
	::llc::SXMLReader parsed;
	::llc::SXMLReader stepped;
	if_fail_fe(::llc::xmlParse(parsed, input));
	for(stepped.State.IndexCurrentChar = 0; stepped.State.IndexCurrentChar < input.size(); ++stepped.State.IndexCurrentChar) {
		cnst ::llc::err_t result = ::llc::xmlParseStep(stepped, input);
		LLC_TEST_REQUIREF(errors, XML_READER_TEST_RESULT_STEP_PARSE, ::llc::failed(result), "Step parse failed at offset:%u with result:%i.", stepped.State.IndexCurrentChar, result);
	}
	LLC_TEST_REQUIREF(errors, XML_READER_TEST_RESULT_STEP_PARSE, parsed.Token.size() != stepped.Token.size(), "Step token count:%u, whole parse token count:%u.", stepped.Token.size(), parsed.Token.size());
	for(::llc::u2_t iToken = 0; iToken < parsed.Token.size(); ++iToken) {
		cnst ::llc::SXMLToken & expected = parsed.Token[iToken];
		cnst ::llc::SXMLToken & actual = stepped.Token[iToken];
		LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_STEP_PARSE
			, expected.ParentIndex != actual.ParentIndex || expected.Type != actual.Type || expected.Span.Begin != actual.Span.Begin || expected.Span.End != actual.Span.End
			, "Token:%u whole(parent:%i,type:%i,span:%u..%u) step(parent:%i,type:%i,span:%u..%u)."
			, iToken, expected.ParentIndex, expected.Type, expected.Span.Begin, expected.Span.End, actual.ParentIndex, actual.Type, actual.Span.Begin, actual.Span.End
			);
	}
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_STEP_PARSE, false == stepped.State.Flags.DoneReading, "%s", "Step reader did not set DoneReading.");
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_STEP_PARSE, stepped.State.NestLevel, "Step reader nesting level:%i, expected:0.", stepped.State.NestLevel);
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_STEP_PARSE, stepped.State.IndexCurrentElement != -1, "Step reader element index:%i, expected:-1.", stepped.State.IndexCurrentElement);
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_STEP_PARSE, stepped.State.CurrentElement, "Step reader current element:%p, expected:null.", stepped.State.CurrentElement);
	rtrn 0;
}

::llc::err_t test_xml(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	cnst ::llc::u2_t failureCountBefore = testErrorCount(errors);
	if_fail_fe(::testXMLDocuments(errors));
	if_fail_fe(::testXMLFailures(errors));
	if_fail_fe(::testXMLStorage(errors));
	if_fail_fe(::testXMLStep(errors));
	LLC_TEST_CHECKF(errors, XML_READER_TEST_RESULT_OK, testErrorCount(errors) != failureCountBefore, "%s", "Earlier XML reader checks failed.");
	rtrn 0;
}
