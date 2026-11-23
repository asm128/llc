#include "llc_test_core.h"

#ifndef LLC_TEST_ENGINE_SUITES_H
#define LLC_TEST_ENGINE_SUITES_H

// The runner creates outputFolder and owns its results.json and console.log.
// Suites may write any other files/subdirectories there; the runner does not inspect them.
// The folder view remains valid for the duration of the suite call.

::llc::err_t test_engine_managers			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_engine_serialization	(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_ro_rsm					(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_d3d					(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_ragnarok_gat			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t test_ragnarok_gnd			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);

#endif // LLC_TEST_ENGINE_SUITES_H
