#include "llc_xml_reader.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(MSBUILD_XML_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(MSBUILD_XML_TEST_RESULT, PARSE				, 0, "xmlParse() rejected valid MSBuild XML.");
GDEFINE_ENUM_VALUED(MSBUILD_XML_TEST_RESULT, PROJECT_NODE		, 1, "The XML reader did not expose the expected MSBuild project node.");
GDEFINE_ENUM_VALUED(MSBUILD_XML_TEST_RESULT, PROJECT_ATTRIBUTE	, 2, "The XML reader did not expose the expected MSBuild project attribute.");
GDEFINE_ENUM_VALUED(MSBUILD_XML_TEST_RESULT, PROJECT_VALUE		, 3, "The XML reader did not expose the expected MSBuild project value.");

::llc::err_t testMSBuildXML(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	cnst ::llc::vcst_t input = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
		"<Project DefaultTargets=\"Build\">"
		"<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">"
		"<OutDir>$(SolutionDir)../$(Platform).$(Configuration)/</OutDir>"
		"<IntDir>$(SolutionDir)../obj/$(Platform).$(Configuration)/$(ProjectName)/</IntDir>"
		"</PropertyGroup>"
		"<Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\"/>"
		"</Project>"
		;
	::llc::SXMLReader reader;
	cnst ::llc::err_t parseResult = ::llc::xmlParse(reader, input);
	always_printf("MSBUILD XML fixture (parsing only, not a build):\n%.*s\nExpected parse success; result:%i.", LLC_VIEW_ARGS(input), parseResult);
	LLC_TEST_REQUIREF(errors, MSBUILD_XML_TEST_RESULT_PARSE, ::llc::failed(parseResult) , "Valid MSBuild XML parse failed. result:%i, input:'%.*s'." , parseResult, (int)input.size(), input.begin() );

	cnst ::llc::err_t iProject = ::llc::xmlNodeChild(reader, input, 0, "Project");
	LLC_TEST_REQUIREF(errors, MSBUILD_XML_TEST_RESULT_PROJECT_NODE, ::llc::failed(iProject) , "Project node not found. result:%i, tokens:%u." , iProject, reader.Token.size() );
	cnst ::llc::err_t iPropertyGroup = ::llc::xmlNodeChild(reader, input, (::llc::u2_t)iProject, "PropertyGroup");
	LLC_TEST_REQUIREF(errors, MSBUILD_XML_TEST_RESULT_PROJECT_NODE, ::llc::failed(iPropertyGroup) , "PropertyGroup node not found. result:%i." , iPropertyGroup );

	::llc::vcst_t condition = {};
	cnst ::llc::err_t iCondition = ::llc::xmlNodeAttribute(reader, input, (::llc::u2_t)iPropertyGroup, "Condition", condition);
	always_printf("Condition attribute: return:%i; actual:'%.*s'; expected: '$(Configuration)|$(Platform)'=='Debug|x64'.", iCondition, LLC_VIEW_ARGS(condition));
	LLC_TEST_CHECKF(errors, MSBUILD_XML_TEST_RESULT_PROJECT_ATTRIBUTE, ::llc::failed(iCondition) , "Condition attribute result:%i, expected success.", iCondition );
	if(0 <= iCondition) {
		LLC_TEST_CHECKF(errors, MSBUILD_XML_TEST_RESULT_PROJECT_ATTRIBUTE, condition != "'$(Configuration)|$(Platform)'=='Debug|x64'" , "Condition value:'%.*s', expected Debug|x64." , (int)condition.size(), condition.begin() );
	}

	cnst ::llc::err_t iOutDir = ::llc::xmlNodeChild(reader, input, (::llc::u2_t)iPropertyGroup, "OutDir");
	::llc::vcst_t outDir = {};
	cnst ::llc::err_t iOutText = ::llc::failed(iOutDir) ? -1 : ::llc::xmlNodeText(reader, input, (::llc::u2_t)iOutDir, outDir);
	always_printf("OutDir text: return:%i; actual:'%.*s'; expected: $(SolutionDir)../$(Platform).$(Configuration)/.", iOutText, LLC_VIEW_ARGS(outDir));
	LLC_TEST_CHECKF(errors, MSBUILD_XML_TEST_RESULT_PROJECT_NODE, ::llc::failed(iOutDir) , "OutDir node result:%i, expected success.", iOutDir );
	if(0 <= iOutDir) {
		LLC_TEST_CHECKF(errors, MSBUILD_XML_TEST_RESULT_PROJECT_VALUE, ::llc::failed(iOutText) , "OutDir text result:%i, expected success.", iOutText );
		if(0 <= iOutText) {
			LLC_TEST_CHECKF(errors, MSBUILD_XML_TEST_RESULT_PROJECT_VALUE, outDir != "$(SolutionDir)../$(Platform).$(Configuration)/" , "OutDir value:'%.*s', expected solution-relative output." , (int)outDir.size(), outDir.begin() );
		}
	}

	cnst ::llc::err_t iIntDir = ::llc::xmlNodeChild(reader, input, (::llc::u2_t)iPropertyGroup, "IntDir");
	::llc::vcst_t intDir = {};
	cnst ::llc::err_t iIntText = ::llc::failed(iIntDir) ? -1 : ::llc::xmlNodeText(reader, input, (::llc::u2_t)iIntDir, intDir);
	always_printf("IntDir text: return:%i; actual:'%.*s'; expected: $(SolutionDir)../obj/$(Platform).$(Configuration)/$(ProjectName)/.", iIntText, LLC_VIEW_ARGS(intDir));
	LLC_TEST_CHECKF(errors, MSBUILD_XML_TEST_RESULT_PROJECT_NODE, ::llc::failed(iIntDir) , "IntDir node result:%i, expected success.", iIntDir );
	if(0 <= iIntDir) {
		LLC_TEST_CHECKF(errors, MSBUILD_XML_TEST_RESULT_PROJECT_VALUE, ::llc::failed(iIntText) , "IntDir text result:%i, expected success.", iIntText );
		if(0 <= iIntText) {
			LLC_TEST_CHECKF(errors, MSBUILD_XML_TEST_RESULT_PROJECT_VALUE, intDir != "$(SolutionDir)../obj/$(Platform).$(Configuration)/$(ProjectName)/" , "IntDir value:'%.*s', expected project-specific intermediate output." , (int)intDir.size(), intDir.begin() );
		}
	}
	rtrn 0;
}
