#include "llc_test_core.h"

#ifndef LLC_TEST_CORE_SUITES_H
#define LLC_TEST_CORE_SUITES_H

::llc::err_t	testAdam7				(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testAES					(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testAlign				(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testAObjPObj			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testAObjPPod			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testAPodColor			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testAPodGauge			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testAPodMinMax			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testAPodN2				(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testAPodN3				(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testAPodRange			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testAPodSerialize		(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testAPodSlice			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testAPodTri				(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testAPodTri2			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testAPodTri3			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testAppImplCPP			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testAppImplH			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testAppendCSSCPP		(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testAppendCSSH			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testAppendFrontend		(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testView				(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testViewBit			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testArrayBase			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testArrayStatic			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testArrayPod			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testArrayObj			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testArrayPtr			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testArrayCircular		(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testGrid				(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testGridCopy			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testDeflate			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testEncoding			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testFile				(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testPNG				(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testPNGWorker			(::llc::vcst_t testName);
::llc::err_t	testBlockContainer		(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testBlockContainerNTS	(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testQueueAsync			(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testString				(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testPtrNCO				(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testPtrPOD				(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t	testPtrObj				(::llc::ATestError & results, ::llc::vcst_t outputFolder);

#endif // LLC_TEST_CORE_SUITES_H
