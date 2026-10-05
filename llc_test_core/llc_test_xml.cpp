#include "llc_xml_reader.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(XML_READER_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, OK				, 0, "All XML reader tests passed.");
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, PARSE				, 1, "xmlParse() rejected a valid XML document.");
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, TOKEN_RANGE			, 2, "An XML token range escaped its source document.");
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, PROJECT_NODE			, 3, "The XML reader did not expose the expected project node.");
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, PROJECT_ATTRIBUTE		, 4, "The XML reader did not expose the expected project attribute.");
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, PROJECT_VALUE			, 5, "The XML reader did not expose the expected project value.");
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, CLOSING_TAG			, 6, "The XML reader accepted a mismatched closing tag.");
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, COUNTED_INPUT			, 7, "The XML reader depended on a terminator beyond counted input.");
GDEFINE_ENUM_VALUED(XML_READER_TEST_RESULT, RESET_REUSE			, 8, "The XML reader did not clear and reuse its token storage.");

sttc bool xmlTestTextEquals(::llc::vcsc_t left, ::llc::vcsc_t right) {
	rtrn left.size() == right.size() && (0 == left.size() || 0 == memcmp(left.begin(), right.begin(), left.size()));
}

sttc ::llc::err_t testXMLProject(ATestError & errors) {
	cnst ::llc::sc_t inputText[] =
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>"
		"<Project DefaultTargets=\"Build\">"
		"<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">"
		"<OutDir>$(SolutionDir)../$(Platform).$(Configuration)/</OutDir>"
		"<IntDir>$(SolutionDir)../obj/$(Platform).$(Configuration)/$(ProjectName)/</IntDir>"
		"</PropertyGroup>"
		"<Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\"/>"
		"</Project>";
	cnst ::llc::vcsc_t input = {inputText, ::llc::size(inputText) - 1};
	::llc::SXMLReader reader;
	cnst ::llc::err_t parseResult = ::llc::xmlParse(reader, input);
	LLC_TEST_REQUIRE(errors, XML_READER_TEST_RESULT_PARSE, ::llc::failed(parseResult), "Valid project XML parse failed. result:%i.", parseResult);
	for(::llc::u2_t iToken = 0; iToken < reader.Token.size(); ++iToken) {
		cnst ::llc::SXMLToken & token = reader.Token[iToken];
		LLC_TEST_CHECK(errors, XML_READER_TEST_RESULT_TOKEN_RANGE, token.Range.Offset > input.size() || token.Range.Count > input.size() - token.Range.Offset
			, "Token %u escaped input. type:%i, offset:%u, count:%u, input:%u."
			, iToken, (::llc::s2_t)token.Type, token.Range.Offset, token.Range.Count, input.size()
			);
	}
	cnst ::llc::err_t iProject = ::llc::xmlNodeChild(reader, input, 0, LLC_CXS("Project"));
	LLC_TEST_REQUIRE(errors, XML_READER_TEST_RESULT_PROJECT_NODE, ::llc::failed(iProject), "Project node not found. result:%i, tokens:%u.", iProject, reader.Token.size());
	cnst ::llc::err_t iPropertyGroup = ::llc::xmlNodeChild(reader, input, (::llc::u2_t)iProject, LLC_CXS("PropertyGroup"));
	LLC_TEST_REQUIRE(errors, XML_READER_TEST_RESULT_PROJECT_NODE, ::llc::failed(iPropertyGroup), "PropertyGroup node not found. result:%i.", iPropertyGroup);
	::llc::vcsc_t condition;
	cnst ::llc::err_t iCondition = ::llc::xmlNodeAttribute(reader, input, (::llc::u2_t)iPropertyGroup, LLC_CXS("Condition"), condition);
	LLC_TEST_CHECK(errors, XML_READER_TEST_RESULT_PROJECT_ATTRIBUTE, ::llc::failed(iCondition) || false == ::xmlTestTextEquals(condition, LLC_CXS("'$(Configuration)|$(Platform)'=='Debug|x64'"))
		, "Condition mismatch. result:%i, value:'%.*s'.", iCondition, (int)condition.size(), condition.begin()
		);
	cnst ::llc::err_t iOutDir = ::llc::xmlNodeChild(reader, input, (::llc::u2_t)iPropertyGroup, LLC_CXS("OutDir"));
	::llc::vcsc_t outDir;
	cnst ::llc::err_t iOutText = ::llc::failed(iOutDir) ? -1 : ::llc::xmlNodeText(reader, input, (::llc::u2_t)iOutDir, outDir);
	LLC_TEST_CHECK(errors, XML_READER_TEST_RESULT_PROJECT_VALUE, ::llc::failed(iOutText) || false == ::xmlTestTextEquals(outDir, LLC_CXS("$(SolutionDir)../$(Platform).$(Configuration)/"))
		, "OutDir mismatch. node:%i, text:%i, value:'%.*s'.", iOutDir, iOutText, (int)outDir.size(), outDir.begin()
		);
	cnst ::llc::err_t iIntDir = ::llc::xmlNodeChild(reader, input, (::llc::u2_t)iPropertyGroup, LLC_CXS("IntDir"));
	::llc::vcsc_t intDir;
	cnst ::llc::err_t iIntText = ::llc::failed(iIntDir) ? -1 : ::llc::xmlNodeText(reader, input, (::llc::u2_t)iIntDir, intDir);
	LLC_TEST_CHECK(errors, XML_READER_TEST_RESULT_PROJECT_VALUE, ::llc::failed(iIntText) || false == ::xmlTestTextEquals(intDir, LLC_CXS("$(SolutionDir)../obj/$(Platform).$(Configuration)/$(ProjectName)/"))
		, "IntDir mismatch. node:%i, text:%i, value:'%.*s'.", iIntDir, iIntText, (int)intDir.size(), intDir.begin()
		);
	rtrn 0;
}

sttc ::llc::err_t testXMLFailures(ATestError & errors) {
	::llc::SXMLReader reader;
	cnst ::llc::err_t result = ::llc::xmlParse(reader, LLC_CXS("<Project><Group></Project>"));
	LLC_TEST_CHECK(errors, XML_READER_TEST_RESULT_CLOSING_TAG, false == ::llc::failed(result), "Mismatched closing tag accepted. result:%i.", result);
	rtrn 0;
}

sttc ::llc::err_t testXMLStorage(ATestError & errors) {
	cnst ::llc::sc_t countedInput[] = {'<', 'A', '/', '>'};
	::llc::SXMLReader reader;
	cnst ::llc::err_t countedResult = ::llc::xmlParse(reader, {countedInput});
	LLC_TEST_REQUIRE(errors, XML_READER_TEST_RESULT_COUNTED_INPUT, ::llc::failed(countedResult), "Counted XML parse failed. result:%i.", countedResult);
	cnst ::llc::SXMLToken * storage = reader.Token.begin();
	cnst ::llc::err_t resetResult = reader.Reset();
	cnst ::llc::err_t parseResult = ::llc::xmlParse(reader, {countedInput});
	LLC_TEST_CHECK(errors, XML_READER_TEST_RESULT_RESET_REUSE
		, resetResult || ::llc::failed(parseResult) || reader.Token.begin() != storage || reader.StateRead.IndexCurrentElement != -1 || reader.StateRead.CurrentElement || reader.StateRead.NestLevel
		, "Reader reuse mismatch. reset:%i, parse:%i, storage:%p/%p, index:%i, current:%p, level:%u."
		, resetResult, parseResult, reader.Token.begin(), storage, reader.StateRead.IndexCurrentElement, reader.StateRead.CurrentElement, reader.StateRead.NestLevel
		);
	rtrn 0;
}

::llc::err_t testXMLReader(ATestError & errors) {
	cnst ::llc::u2_t failureCountBefore = testErrorCount(errors);
	if_fail_fe(::testXMLProject(errors));
	if_fail_fe(::testXMLFailures(errors));
	if_fail_fe(::testXMLStorage(errors));
	LLC_TEST_CHECK(errors, XML_READER_TEST_RESULT_OK, testErrorCount(errors) != failureCountBefore, "%s", "Earlier XML reader checks failed.");
	rtrn 0;
}
