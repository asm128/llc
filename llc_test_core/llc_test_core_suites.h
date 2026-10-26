#include "llc_test_core.h"

#ifndef LLC_TEST_CORE_SUITES_H
#define LLC_TEST_CORE_SUITES_H

::llc::err_t	testView				(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testViewBit			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testArrayBase			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testArrayStatic			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testArrayPod			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testArrayObj			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testArrayPtr			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testArrayCircular		(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testGrid				(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testBlockContainer		(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testBlockContainerNTS	(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testQueueAsync			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testString				(::llc::ATestError & results, ::llc::vcst_t outputFolder);

#endif // LLC_TEST_CORE_SUITES_H
