#include "llc_test_core.h"

#ifndef LLC_TEST_CORE_SUITES_H
#define LLC_TEST_CORE_SUITES_H

// The runner creates outputFolder and owns its results.json and console.log.
// Suites may write any other files/subdirectories there; the runner does not inspect them.
// The folder view remains valid for the duration of the suite call.

::llc::err_t testEngineManagers	(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testEngineSerialization(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);

#endif // LLC_TEST_CORE_SUITES_H
