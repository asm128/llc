//#include "llc_runtime_module.h"
//#include "llc_test_core.h"
//
//GDEFINE_ENUM_TYPE(RUNTIME_MODULE_TEST_RESULT, ::llc::u0_t);
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, MISSING_RESULT			,  1, "loadRuntimeModule() accepted a missing module.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, MISSING_STATE			,  2, "loadRuntimeModule() retained state after rejecting a missing module.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, LOAD_RESULT				,  3, "loadRuntimeModule() rejected the test module.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, LOAD_HANDLE				,  4, "loadRuntimeModule() did not retain the module handle.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, LOAD_TITLE				,  5, "loadRuntimeModule() did not resolve llc_moduleTitle().");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, LOAD_VERSION			,  6, "loadRuntimeModule() did not resolve llc_moduleVersion().");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, LOAD_CREATE				,  7, "loadRuntimeModule() did not resolve llc_moduleCreate().");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, LOAD_DELETE				,  8, "loadRuntimeModule() did not resolve llc_moduleDelete().");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, LOAD_SETUP				,  9, "loadRuntimeModule() did not resolve llc_moduleSetup().");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, LOAD_CLEANUP			, 10, "loadRuntimeModule() did not resolve llc_moduleCleanup().");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, LOAD_RENDER				, 11, "loadRuntimeModule() did not resolve llc_moduleRender().");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, LOAD_UPDATE				, 12, "loadRuntimeModule() did not resolve llc_moduleUpdate().");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, LOAD_DUPLICATE			, 13, "loadRuntimeModule() replaced a loaded module.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, VERSION_RESULT			, 14, "The loaded module rejected a version query.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, VERSION_VALUE			, 15, "The loaded module returned the wrong version.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, TITLE_QUERY_RESULT		, 16, "The loaded module rejected a title-size query.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, TITLE_QUERY_LENGTH		, 17, "The loaded module returned the wrong title length.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, TITLE_COPY_RESULT		, 18, "The loaded module rejected an exactly-sized title buffer.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, TITLE_COPY_CONTENT		, 19, "The loaded module copied the wrong title.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, CREATE_RESULT			, 20, "The loaded module rejected application creation.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, CREATE_OUTPUT			, 21, "The loaded module did not return its application instance.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, UNLOAD_BUSY_RESULT		, 22, "unloadRuntimeModule() unloaded a module with a live application.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, UNLOAD_BUSY_STATE		, 23, "unloadRuntimeModule() damaged a module with a live application.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, SETUP_RESULT				, 24, "The loaded module rejected application setup.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, UPDATE_RESULT			, 25, "The loaded module rejected a normal update.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, RENDER_RESULT			, 26, "The loaded module rejected a render call.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, EXIT_RESULT				, 27, "The loaded module did not report a requested exit.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, CLEANUP_RESULT			, 28, "The loaded module rejected application cleanup.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, DELETE_RESULT			, 29, "The loaded module rejected application deletion.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, DELETE_OUTPUT			, 30, "The loaded module retained its deleted application pointer.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, UNLOAD_RESULT			, 31, "unloadRuntimeModule() rejected an unused module.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, UNLOAD_HANDLE			, 32, "unloadRuntimeModule() retained the module handle.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, UNLOAD_FUNCTIONS		, 33, "unloadRuntimeModule() retained function pointers.");
//GDEFINE_ENUM_VALUED(RUNTIME_MODULE_TEST_RESULT, UNLOAD_EMPTY			, 34, "unloadRuntimeModule() rejected an empty module.");
//
//::llc::err_t testRuntimeModule(ATestError & errors) {
//	::llc::SRuntimeModule module = {};
//	::llc::setupLogCallbacks(0, 0);
//	cnst ::llc::err_t missingResult = ::llc::loadRuntimeModule(module, "llc_test_module_missing.dll");
//	::llc::setupDefaultLogCallbacks();
//	LLC_TEST_CHECKF(errors, RUNTIME_MODULE_TEST_RESULT_MISSING_RESULT, ::llc::OS_NOT_FOUND != missingResult, "Result:%i, expected:%i.", missingResult, ::llc::OS_NOT_FOUND);
//	LLC_TEST_CHECKF(errors, RUNTIME_MODULE_TEST_RESULT_MISSING_STATE, module.Handle || module.Title || module.Version || module.Create || module.Delete || module.Setup || module.Cleanup || module.Render || module.Update || module.Application, "Handle:%p, application:%p.", module.Handle, module.Application);
//
//	cnst ::llc::err_t loadResult = ::llc::loadRuntimeModule(module, "llc_test_module.dll");
//	LLC_TEST_REQUIREF(errors, RUNTIME_MODULE_TEST_RESULT_LOAD_RESULT, ::llc::failed(loadResult), "Result:%i.", loadResult);
//	LLC_TEST_REQUIRE(errors, RUNTIME_MODULE_TEST_RESULT_LOAD_HANDLE, 0 == module.Handle);
//	LLC_TEST_CHECK(errors, RUNTIME_MODULE_TEST_RESULT_LOAD_TITLE, 0 == module.Title);
//	LLC_TEST_CHECK(errors, RUNTIME_MODULE_TEST_RESULT_LOAD_VERSION, 0 == module.Version);
//	LLC_TEST_CHECK(errors, RUNTIME_MODULE_TEST_RESULT_LOAD_CREATE, 0 == module.Create);
//	LLC_TEST_CHECK(errors, RUNTIME_MODULE_TEST_RESULT_LOAD_DELETE, 0 == module.Delete);
//	LLC_TEST_CHECK(errors, RUNTIME_MODULE_TEST_RESULT_LOAD_SETUP, 0 == module.Setup);
//	LLC_TEST_CHECK(errors, RUNTIME_MODULE_TEST_RESULT_LOAD_CLEANUP, 0 == module.Cleanup);
//	LLC_TEST_CHECK(errors, RUNTIME_MODULE_TEST_RESULT_LOAD_RENDER, 0 == module.Render);
//	LLC_TEST_CHECK(errors, RUNTIME_MODULE_TEST_RESULT_LOAD_UPDATE, 0 == module.Update);
//	void* const loadedHandle = module.Handle;
//	LLC_TEST_CHECKF(errors, RUNTIME_MODULE_TEST_RESULT_LOAD_DUPLICATE, ::llc::OS_BUSY != ::llc::loadRuntimeModule(module, "llc_test_module.dll"), "Handle:%p.", module.Handle);
//
//	::llc::u2_t version = {};
//	LLC_TEST_CHECK(errors, RUNTIME_MODULE_TEST_RESULT_VERSION_RESULT, ::llc::failed(module.Version(&version)));
//	LLC_TEST_CHECKF(errors, RUNTIME_MODULE_TEST_RESULT_VERSION_VALUE, 1 != version, "Version:%u, expected:1.", version);
//	stxp ::llc::sc_t expectedTitle[] = "LLC test module";
//	::llc::u0_t titleLength = {};
//	LLC_TEST_CHECK(errors, RUNTIME_MODULE_TEST_RESULT_TITLE_QUERY_RESULT, ::llc::failed(module.Title(0, &titleLength)));
//	LLC_TEST_CHECKF(errors, RUNTIME_MODULE_TEST_RESULT_TITLE_QUERY_LENGTH, ::llc::size(expectedTitle) != titleLength, "Length:%u, expected:%u.", titleLength, ::llc::size(expectedTitle));
//	::llc::sc_t title[::llc::size(expectedTitle)] = {};
//	LLC_TEST_CHECK(errors, RUNTIME_MODULE_TEST_RESULT_TITLE_COPY_RESULT, ::llc::failed(module.Title(title, &titleLength)));
//	LLC_TEST_CHECK(errors, RUNTIME_MODULE_TEST_RESULT_TITLE_COPY_CONTENT, 0 != ::memcmp(title, expectedTitle, ::llc::size(expectedTitle)));
//
//	::llc::SRuntimeValues runtimeValues = {};
//	LLC_TEST_CHECK(errors, RUNTIME_MODULE_TEST_RESULT_CREATE_RESULT, ::llc::failed(module.Create(&module.Application, &runtimeValues)));
//	LLC_TEST_REQUIRE(errors, RUNTIME_MODULE_TEST_RESULT_CREATE_OUTPUT, 0 == module.Application);
//	LLC_TEST_CHECKF(errors, RUNTIME_MODULE_TEST_RESULT_UNLOAD_BUSY_RESULT, ::llc::OS_BUSY != ::llc::unloadRuntimeModule(module), "Handle:%p, application:%p.", module.Handle, module.Application);
//	LLC_TEST_CHECKF(errors, RUNTIME_MODULE_TEST_RESULT_UNLOAD_BUSY_STATE, loadedHandle != module.Handle || 0 == module.Application, "Handle:%p, expected:%p; application:%p.", module.Handle, loadedHandle, module.Application);
//	LLC_TEST_CHECK(errors, RUNTIME_MODULE_TEST_RESULT_SETUP_RESULT, ::llc::failed(module.Setup(module.Application)));
//	LLC_TEST_CHECK(errors, RUNTIME_MODULE_TEST_RESULT_UPDATE_RESULT, ::llc::failed(module.Update(module.Application, false)));
//	LLC_TEST_CHECK(errors, RUNTIME_MODULE_TEST_RESULT_RENDER_RESULT, ::llc::failed(module.Render(module.Application)));
//	LLC_TEST_CHECKF(errors, RUNTIME_MODULE_TEST_RESULT_EXIT_RESULT, ::llc::APPLICATION_STATE_EXIT != module.Update(module.Application, true), "Expected:%u.", ::llc::APPLICATION_STATE_EXIT);
//	LLC_TEST_CHECK(errors, RUNTIME_MODULE_TEST_RESULT_CLEANUP_RESULT, ::llc::failed(module.Cleanup(module.Application)));
//	LLC_TEST_CHECK(errors, RUNTIME_MODULE_TEST_RESULT_DELETE_RESULT, ::llc::failed(module.Delete(&module.Application)));
//	LLC_TEST_CHECKF(errors, RUNTIME_MODULE_TEST_RESULT_DELETE_OUTPUT, 0 != module.Application, "Application:%p, expected:null.", module.Application);
//	LLC_TEST_CHECK(errors, RUNTIME_MODULE_TEST_RESULT_UNLOAD_RESULT, ::llc::failed(::llc::unloadRuntimeModule(module)));
//	LLC_TEST_CHECKF(errors, RUNTIME_MODULE_TEST_RESULT_UNLOAD_HANDLE, module.Handle, "Handle:%p, expected:null.", module.Handle);
//	LLC_TEST_CHECK(errors, RUNTIME_MODULE_TEST_RESULT_UNLOAD_FUNCTIONS, module.Title || module.Version || module.Create || module.Delete || module.Setup || module.Cleanup || module.Render || module.Update);
//	LLC_TEST_CHECK(errors, RUNTIME_MODULE_TEST_RESULT_UNLOAD_EMPTY, ::llc::failed(::llc::unloadRuntimeModule(module)));
//	rtrn 0;
//}
