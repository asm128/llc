#include "llc_test_core.h"

#ifndef LLC_TEST_CORE_SUITES_H
#define LLC_TEST_CORE_SUITES_H

::llc::err_t testArrayStatic		(ATestError & errors);
::llc::err_t testArrayObj		(ATestError & errors);
::llc::err_t testArrayPod		(ATestError & errors);
::llc::err_t testArgs			(ATestError & errors);
::llc::err_t testBase64			(ATestError & errors);
::llc::err_t testBitField		(ATestError & errors);
::llc::err_t testBlockContainer	(ATestError & errors);
::llc::err_t testPackedUInt		(ATestError & errors);
::llc::err_t testCPow			(ATestError & errors);
::llc::err_t testCTTIParser		(ATestError & errors);
::llc::err_t testEnum			(ATestError & errors);
::llc::err_t testGeometry		(ATestError & errors);
::llc::err_t testGridImage		(ATestError & errors);
::llc::err_t testJSONReader		(ATestError & errors);
::llc::err_t testKeyVal			(ATestError & errors);
::llc::err_t testLabel			(ATestError & errors);
::llc::err_t testMSBuildXML		(ATestError & errors);
::llc::err_t testXMLReader		(ATestError & errors);
::llc::err_t testPath			(ATestError & errors);
::llc::err_t testPointers		(ATestError & errors);
::llc::err_t testSPRNG			(ATestError & errors);
::llc::err_t testStr			(ATestError & errors);
::llc::err_t testView			(ATestError & errors);
::llc::err_t testViewBit		(ATestError & errors);
::llc::err_t testViewSerialize	(ATestError & errors);

#endif // LLC_TEST_CORE_SUITES_H
