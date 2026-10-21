#include "llc_test_core.h"

#ifndef LLC_TEST_CORE_SUITES_H
#define LLC_TEST_CORE_SUITES_H

// The runner creates outputFolder and owns its results.json and console.log.
// Suites may write any other files/subdirectories there; the runner does not inspect them.
// The folder view remains valid for the duration of the suite call.

::llc::err_t testArrayStatic		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testAdam7			(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
//::llc::err_t testApp			(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testASCIIColor		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testASCIITarget		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testArrayBit		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testArrayMinMax		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testArrayObj		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testArrayPod		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testArgs			(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testBase64			(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testBitField		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testBitmapTarget	(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testBlock			(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testBlockContainer	(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testCamera			(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testCollision		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testCOMPlus		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testPackedUInt		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testCPow			(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testCTTIParser		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testD3D			(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testDeflate		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testEngineManagers	(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testEngineSerialization(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testEnum			(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testExpression		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testGeometry		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testGridImage		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testGridOperations	(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testGUI			(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testInput			(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testJSONExpression	(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testJSONReader		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testKeyVal			(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testLabel			(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testLinearMap		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testMSBuildXML		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testXMLReader		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testPath			(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testPointers		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testPNG			(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testProcess		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
//::llc::err_t testRuntimeModule	(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testSPRNG			(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testSTL			(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testStr			(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testSystemKey		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testTypeMetadata	(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testView			(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testViewAliases	(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testViewBit		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testViewSerialize	(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testViewStream		(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testVOX			(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testVoxel			(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);

#endif // LLC_TEST_CORE_SUITES_H
