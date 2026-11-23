#include "llc_test_core.h"

#ifndef TEST_LLC_SUITES_H
#define TEST_LLC_SUITES_H

usns llc;
// The runner creates outputFolder and owns its results.json and console.log.
// Suites may write any other files/subdirectories there; the runner does not inspect them.
// The folder view remains valid for the duration of the suite call.

::llc::err_t test_adam7				(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_args				(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_array_obj			(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_array_pod			(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_base64			(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_ctti_enum		(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_ctti_enum_json	(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_ctti_stct		(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_ctti_stct_json	(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_deflate			(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_enum				(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_event			(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_gui_control_serialization(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_expression_core	(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_expression_ctti	(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_expression		(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_gui_json			(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_index				(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_index_catalog		(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_index_path		(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_json				(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_json_core			(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_json_ctti			(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_json_expression	(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_json_expression_ctti(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_log				(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_png				(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_string			(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_array_static		(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_view_serialize	(STestRunnerState & runner, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);

#endif //TEST_LLC_CORE_SUITES_H
