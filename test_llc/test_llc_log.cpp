#include "llc_log.h"
#include "llc_file.h"

#include "test_llc_suites.h"

#include <cstdio>

GDEFINE_ENUM_TYPE(LOG_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(LOG_TEST_RESULT, COMPILE_TIME, 0, "append_log() or llc_log() failed during constant evaluation.");
GDEFINE_ENUM_VALUED(LOG_TEST_RESULT, RUNTIME_COUNT, 1, "llc_log() returned the wrong runtime byte count.");
GDEFINE_ENUM_VALUED(LOG_TEST_RESULT, OUTER_TEXT, 2, "The outer capture did not receive stdout, stderr, diagnostics and restored output exactly once.");
GDEFINE_ENUM_VALUED(LOG_TEST_RESULT, INNER_TEXT, 3, "The nested capture did not isolate its stdout and stderr bytes.");

static_assert(::llc::append_log("value", ':', "42") == 8);
static_assert(::llc::_llc_log(3, "file.cpp", 42, "func", "value", ':', "42") == 30);
stxp ::llc::b8_t testLogMacroConstant() { rtrn llc_log(3, "x") > 2; }
static_assert(::testLogMacroConstant());

::llc::err_t test_log(STestRunnerState &, ::llc::ATestError & errors, ::llc::vcst_t outputFolder) {
	::llc::string outerPath, innerPath;
	if_fail_fe(::llc::pathNameCompose(outputFolder, "capture_outer.log", outerPath));
	if_fail_fe(::llc::pathNameCompose(outputFolder, "capture_inner.log", innerPath));

	::llc::STestLogCapture outer;
	if_fail_fe(outer.open(outerPath));
	::fprintf(stdout, "%s", "stdout\n");
	::fflush(stdout);
	::fprintf(stderr, "%s", "stderr\n");
	::fflush(stderr);
	cnst ::llc::err_t result = ::llc::_llc_log(3, "file.cpp", 42, "func", "value", ':', "42");

	::llc::STestLogCapture inner;
	if_fail_fe(inner.open(innerPath));
	::fprintf(stdout, "%s", "inner-out\n");
	::fflush(stdout);
	::fprintf(stderr, "%s", "inner-err\n");
	::fflush(stderr);
	if_fail_fe(inner.close());
	::fprintf(stderr, "%s", "restored\n");
	::fflush(stderr);
	if_fail_fe(outer.close());

	::llc::au0_t outerBytes, innerBytes;
	if_fail_fe(::llc::fileToMemory(outerPath, outerBytes));
	if_fail_fe(::llc::fileToMemory(innerPath, innerBytes));
	stxp ::llc::sc_c expectedOuter[] = "stdout\nstderr\n3|file.cpp(42){func}:value:42\nrestored\n";
	stxp ::llc::sc_c expectedInner[] = "inner-out\ninner-err\n";
	cnst ::llc::vcu0_c outerExpected = {(cnst ::llc::u0_t*)expectedOuter, ::llc::size(expectedOuter) - 1};
	cnst ::llc::vcu0_c innerExpected = {(cnst ::llc::u0_t*)expectedInner, ::llc::size(expectedInner) - 1};
	LLC_TEST_CHECK(errors, LOG_TEST_RESULT_RUNTIME_COUNT, result != 30);
	LLC_TEST_CHECK(errors, LOG_TEST_RESULT_OUTER_TEXT, outerBytes != outerExpected);
	LLC_TEST_CHECK(errors, LOG_TEST_RESULT_INNER_TEXT, innerBytes != innerExpected);
	rtrn 0;
}
