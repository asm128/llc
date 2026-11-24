#include "llc_test_core_suites.h"

using ::llc::test_reporting;

sttc ::llc::err_t test_core_entry_point(::llc::SRuntimeValues & runtimeValues);
LLC_OS_ENTRY_POINT(::test_core_entry_point);

#if defined(LLC_WINDOWS) || defined(LLC_LINUX)
extern ::llc::err_t testProcessEchoChild();
#endif
#ifdef LLC_LINUX
extern ::llc::err_t testProcessMarkerChild();
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
	if(0 <= ::llc::argsOptionIndex(runtimeValues.EntryPointArgs, "process-echo"))
		rtrn ::testProcessEchoChild();
	if(0 <= ::llc::argsOptionIndex(runtimeValues.EntryPointArgs, "process-child"))
		rtrn ::testProcessMarkerChild();
#endif

	rtrn ::llc::runSuites(runtimeValues
		, LLC_TEST_SUITE(noise)
		, LLC_TEST_SUITE(stl)
		, LLC_TEST_SUITE(adam7)
		//, LLC_TEST_SUITE(app)
		, LLC_TEST_SUITE(ascii_color)
		, LLC_TEST_SUITE(ascii_target)
		, LLC_TEST_SUITE(args)
		, LLC_TEST_SUITE(bit)
		, LLC_TEST_SUITE(bitmap_target)
		, LLC_TEST_SUITE(block)
		, LLC_TEST_SUITE(block_container)
		, LLC_TEST_SUITE(camera)
		, LLC_TEST_SUITE(collision)
		, LLC_TEST_SUITE(complus)
		, LLC_TEST_SUITE(cpow)
		, LLC_TEST_SUITE(ctti)
		, LLC_TEST_SUITE(deflate)
		, LLC_TEST_SUITE(enum)
		, LLC_TEST_SUITE(expression)
		, LLC_TEST_SUITE(geometry)
		, LLC_TEST_SUITE(grid)
		, LLC_TEST_SUITE(grid_serialize)
		, LLC_TEST_SUITE(grid_image)
		, LLC_TEST_SUITE(grid_operations)
		, LLC_TEST_SUITE(gui)
		, LLC_TEST_SUITE(input)
		, LLC_TEST_SUITE(str)
		, LLC_TEST_SUITE(system_key)
		, LLC_TEST_SUITE(type_metadata)
		, LLC_TEST_SUITE(array_static)
		, LLC_TEST_SUITE(array_bit)
		, LLC_TEST_SUITE(apod_minmax)
		, LLC_TEST_SUITE(apod_serialize)
		, LLC_TEST_SUITE(array_obj)
		, LLC_TEST_SUITE(array_pod)
		, LLC_TEST_SUITE(base64)
		, LLC_TEST_SUITE(index)
        , LLC_TEST_SUITE(json_expression)
		, LLC_TEST_SUITE(json)
		, LLC_TEST_SUITE(keyval)
		, LLC_TEST_SUITE(label)
		, LLC_TEST_SUITE(linear_map)
		, LLC_TEST_SUITE(xml)
		, LLC_TEST_SUITE(msbuild_xml)
		, LLC_TEST_SUITE(path)
		, LLC_TEST_SUITE(ptr)
		, LLC_TEST_SUITE(png)
		, LLC_TEST_SUITE(process_broker)
		, LLC_TEST_SUITE(reporting)
		//, LLC_TEST_SUITE(runtime_module)
		, LLC_TEST_SUITE(view)
		, LLC_TEST_SUITE(view_aliases)
		, LLC_TEST_SUITE(view_bit)
		, LLC_TEST_SUITE(packed_int)
		, LLC_TEST_SUITE(view_serialize)
		, LLC_TEST_SUITE(view_stream)
		, LLC_TEST_SUITE(vox)
		, LLC_TEST_SUITE(voxel)
		);
}
