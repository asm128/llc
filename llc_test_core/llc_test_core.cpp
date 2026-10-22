#include "llc_test_core_suites.h"

using ::llc::testReporting;

sttc ::llc::err_t test_core_entry_point(::llc::SRuntimeValues & runtimeValues);
LLC_OS_ENTRY_POINT(::test_core_entry_point);

#if defined(LLC_WINDOWS) || defined(LLC_LINUX)
extern ::llc::err_t testProcessEchoChild();
#endif
#ifdef LLC_LINUX
extern ::llc::err_t testProcessMarkerChild();
extern ::llc::err_t testProcessSignalChild();
#endif


sttc ::llc::err_t test_core_entry_point(::llc::SRuntimeValues & runtimeValues) {
#ifdef LLC_WINDOWS
	if(0 <= ::llc::argsOptionIndex(runtimeValues.EntryPointArgs, "process-echo"))
		rtrn ::testProcessEchoChild();
	if(0 <= ::llc::argsOptionIndex(runtimeValues.EntryPointArgs, "process-child")) {
		stxp ::llc::sc_t childOutput[] = "llc process child";
		DWORD written = 0;
		if(FALSE == WriteFile(GetStdHandle(STD_ERROR_HANDLE), childOutput, ::llc::size(childOutput) - 1, &written, 0) || written != ::llc::size(childOutput) - 1)
			rtrn -1;
		rtrn -1;
	}
#elif defined(LLC_LINUX)
	if(0 <= ::llc::argsOptionIndex(runtimeValues.EntryPointArgs, "process-signal"))
		rtrn ::testProcessSignalChild();
	if(0 <= ::llc::argsOptionIndex(runtimeValues.EntryPointArgs, "process-echo"))
		rtrn ::testProcessEchoChild();
	if(0 <= ::llc::argsOptionIndex(runtimeValues.EntryPointArgs, "process-child"))
		rtrn ::testProcessMarkerChild();
#endif

	rtrn ::llc::runSuites(runtimeValues
		, LLC_TEST_SUITE(testSPRNG, "noise")
		, LLC_TEST_SUITE(testSTL, "stl")
		, LLC_TEST_SUITE(testAdam7, "adam7")
		//, LLC_TEST_SUITE(testApp, "app")
		, LLC_TEST_SUITE(testASCIIColor, "ascii_color")
		, LLC_TEST_SUITE(testASCIITarget, "ascii_target")
		, LLC_TEST_SUITE(testArgs, "args")
		, LLC_TEST_SUITE(testBitField, "bit")
		, LLC_TEST_SUITE(testBitmapTarget, "bitmap_target")
		, LLC_TEST_SUITE(testBlock, "block")
		, LLC_TEST_SUITE(testBlockContainer, "block_container")
		, LLC_TEST_SUITE(testCamera, "camera")
		, LLC_TEST_SUITE(testCollision, "collision")
		, LLC_TEST_SUITE(testCOMPlus, "complus")
		, LLC_TEST_SUITE(testCPow, "cpow")
		, LLC_TEST_SUITE(testCTTIParser, "ctti")
		, LLC_TEST_SUITE(testDeflate, "deflate")
		, LLC_TEST_SUITE(testEnum, "enum")
		, LLC_TEST_SUITE(testExpression, "expression")
		, LLC_TEST_SUITE(testGeometry, "geometry")
		, LLC_TEST_SUITE(testGrid, "grid")
		, LLC_TEST_SUITE(testGridSerialize, "grid_serialize")
		, LLC_TEST_SUITE(testGridImage, "grid_image")
		, LLC_TEST_SUITE(testGridOperations, "grid_operations")
		, LLC_TEST_SUITE(testGUI, "gui")
		, LLC_TEST_SUITE(testInput, "input")
		, LLC_TEST_SUITE(testStr, "str")
		, LLC_TEST_SUITE(testSystemKey, "system_key")
		, LLC_TEST_SUITE(testTypeMetadata, "type_metadata")
		, LLC_TEST_SUITE(testArrayStatic, "array_static")
		, LLC_TEST_SUITE(testArrayBit, "array_bit")
		, LLC_TEST_SUITE(testArrayMinMax, "apod_minmax")
		, LLC_TEST_SUITE(testArrayObj, "array_obj")
		, LLC_TEST_SUITE(testArrayPod, "array_pod")
		, LLC_TEST_SUITE(testBase64, "base64")
		, LLC_TEST_SUITE(testJSONExpression, "json_expression")
		, LLC_TEST_SUITE(testJSONReader, "json")
		, LLC_TEST_SUITE(testKeyVal, "keyval")
		, LLC_TEST_SUITE(testLabel, "label")
		, LLC_TEST_SUITE(testLinearMap, "linear_map")
		, LLC_TEST_SUITE(testXMLReader, "xml")
		, LLC_TEST_SUITE(testMSBuildXML, "msbuild_xml")
		, LLC_TEST_SUITE(testPath, "path")
		, LLC_TEST_SUITE(testPointers, "ptr")
		, LLC_TEST_SUITE(testPNG, "png")
		, LLC_TEST_SUITE(testProcess, "process")
		, LLC_TEST_SUITE(testReporting, "reporting")
		//, LLC_TEST_SUITE(testRuntimeModule, "runtime_module")
		, LLC_TEST_SUITE(testView, "view")
		, LLC_TEST_SUITE(testViewAliases, "view_aliases")
		, LLC_TEST_SUITE(testViewBit, "view_bit")
		, LLC_TEST_SUITE(testPackedUInt, "packed_int")
		, LLC_TEST_SUITE(testViewSerialize, "view_serialize")
		, LLC_TEST_SUITE(testViewStream, "view_stream")
		, LLC_TEST_SUITE(testVOX, "vox")
		, LLC_TEST_SUITE(testVoxel, "voxel")
		);
}
