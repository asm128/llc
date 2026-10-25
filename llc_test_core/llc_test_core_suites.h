#include "llc_test_core.h"

#ifndef LLC_TEST_CORE_SUITES_H
#define LLC_TEST_CORE_SUITES_H

// The runner creates outputFolder and owns its results.json and console.log.
// Suites may write any other files/subdirectories there; the runner does not inspect them.
// The folder view remains valid for the duration of the suite call.

::llc::err_t testArrayStatic		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testAdam7			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
//::llc::err_t testApp			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testASCIIColor		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testASCIITarget		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testArrayBit		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testArrayMinMax		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testArrayObj		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testArrayPod		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testArgs			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testBase64			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testBitField		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testBitmapTarget	(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testBlock			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testBlockContainer	(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testCamera			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testCollision		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testCOMPlus		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testPackedUInt		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testCPow			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testCTTIParser		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testDeflate		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testEnum			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testExpression		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testFile			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testGeometry		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testGrid(::llc::ATestError & errors, ::llc::vcst_t outputFolder);
::llc::err_t testGridSerialize(::llc::ATestError & errors, ::llc::vcst_t outputFolder);
::llc::err_t testGridImage		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testImageBounds	(::llc::ATestError & results, ::llc::vcst_t outputFolder);
::llc::err_t testGridOperations	(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testGUI			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testInput			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testJSONExpression	(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testJSONReader		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testKeyVal			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testLabel			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testLinearMap		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testMSBuildXML		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testXMLReader		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testPath			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testPointers		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testPNG			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testProcess		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
//::llc::err_t testRuntimeModule	(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testSPRNG			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testSTL			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testStr			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testSystemKey		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testTypeMetadata	(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testWordDict		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testView			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testViewAliases	(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testViewBit		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testViewSerialize	(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testViewStream		(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testVOX			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);
::llc::err_t testVoxel			(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder);

#endif // LLC_TEST_CORE_SUITES_H
