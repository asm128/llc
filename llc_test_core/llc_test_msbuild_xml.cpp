#include "llc_xml_reader.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(MSBUILD_XML_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(MSBUILD_XML_TEST_RESULT, PARSE				, 0, "xmlParse() rejected valid MSBuild XML.");
GDEFINE_ENUM_VALUED(MSBUILD_XML_TEST_RESULT, PROJECT_NODE		, 1, "The XML reader did not expose the expected MSBuild project node.");
GDEFINE_ENUM_VALUED(MSBUILD_XML_TEST_RESULT, PROJECT_ATTRIBUTE	, 2, "The XML reader did not expose the expected MSBuild project attribute.");
GDEFINE_ENUM_VALUED(MSBUILD_XML_TEST_RESULT, PROJECT_VALUE		, 3, "The XML reader did not expose the expected MSBuild project value.");

::llc::err_t testMSBuildXML(ATestError & errors) {
	cnst ::llc::vcst_t input = LLC_CXS
		("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
		"<Project DefaultTargets=\"Build\">"
		"<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">"
		"<OutDir>$(SolutionDir)../$(Platform).$(Configuration)/</OutDir>"
		"<IntDir>$(SolutionDir)../obj/$(Platform).$(Configuration)/$(ProjectName)/</IntDir>"
		"</PropertyGroup>"
		"<Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\"/>"
		"</Project>"
		);
	::llc::SXMLReader reader;
	cnst ::llc::err_t parseResult = ::llc::xmlParse(reader, input);
	LLC_TEST_REQUIRE(errors, MSBUILD_XML_TEST_RESULT_PARSE, ::llc::failed(parseResult)
		, "Valid MSBuild XML parse failed. result:%i, input:'%.*s'."
		, parseResult, (int)input.size(), input.begin()
		);

	cnst ::llc::err_t iProject = ::llc::xmlNodeChild(reader, input, 0, LLC_CXS("Project"));
	LLC_TEST_REQUIRE(errors, MSBUILD_XML_TEST_RESULT_PROJECT_NODE, ::llc::failed(iProject)
		, "Project node not found. result:%i, tokens:%u."
		, iProject, reader.Token.size()
		);
	cnst ::llc::err_t iPropertyGroup = ::llc::xmlNodeChild(reader, input, (::llc::u2_t)iProject, LLC_CXS("PropertyGroup"));
	LLC_TEST_REQUIRE(errors, MSBUILD_XML_TEST_RESULT_PROJECT_NODE, ::llc::failed(iPropertyGroup)
		, "PropertyGroup node not found. result:%i."
		, iPropertyGroup
		);

	::llc::vcst_t condition = {};
	cnst ::llc::err_t iCondition = ::llc::xmlNodeAttribute(reader, input, (::llc::u2_t)iPropertyGroup, LLC_CXS("Condition"), condition);
	LLC_TEST_CHECK(errors, MSBUILD_XML_TEST_RESULT_PROJECT_ATTRIBUTE
		, ::llc::failed(iCondition) || condition != LLC_CXS("'$(Configuration)|$(Platform)'=='Debug|x64'")
		, "Condition mismatch. result:%i, value:'%.*s'."
		, iCondition, (int)condition.size(), condition.begin()
		);

	cnst ::llc::err_t iOutDir = ::llc::xmlNodeChild(reader, input, (::llc::u2_t)iPropertyGroup, LLC_CXS("OutDir"));
	::llc::vcst_t outDir = {};
	cnst ::llc::err_t iOutText = ::llc::failed(iOutDir) ? -1 : ::llc::xmlNodeText(reader, input, (::llc::u2_t)iOutDir, outDir);
	LLC_TEST_CHECK(errors, MSBUILD_XML_TEST_RESULT_PROJECT_VALUE
		, ::llc::failed(iOutText) || outDir != LLC_CXS("$(SolutionDir)../$(Platform).$(Configuration)/")
		, "OutDir mismatch. node:%i, text:%i, value:'%.*s'."
		, iOutDir, iOutText, (int)outDir.size(), outDir.begin()
		);

	cnst ::llc::err_t iIntDir = ::llc::xmlNodeChild(reader, input, (::llc::u2_t)iPropertyGroup, LLC_CXS("IntDir"));
	::llc::vcst_t intDir = {};
	cnst ::llc::err_t iIntText = ::llc::failed(iIntDir) ? -1 : ::llc::xmlNodeText(reader, input, (::llc::u2_t)iIntDir, intDir);
	LLC_TEST_CHECK(errors, MSBUILD_XML_TEST_RESULT_PROJECT_VALUE
		, ::llc::failed(iIntText) || intDir != LLC_CXS("$(SolutionDir)../obj/$(Platform).$(Configuration)/$(ProjectName)/")
		, "IntDir mismatch. node:%i, text:%i, value:'%.*s'."
		, iIntDir, iIntText, (int)intDir.size(), intDir.begin()
		);
	rtrn 0;
}
