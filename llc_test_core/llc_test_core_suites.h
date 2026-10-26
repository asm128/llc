#include "llc_test_core.h"

#ifndef LLC_TEST_CORE_SUITES_H
#define LLC_TEST_CORE_SUITES_H

::llc::err_t	testAdam7CPP			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testAdam7H			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testAESCPP				(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testAESH				(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testAlignH				(::llc::ATestError & results, ::llc::vcst_t outputFolder);

#endif // LLC_TEST_CORE_SUITES_H
