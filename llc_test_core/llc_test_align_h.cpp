#include "llc_test_core_suites.h"

#if __has_include("llc_align.h") && __has_include("llc_axis.h")
#	include "llc_align.h"
#	define LLC_TEST_ALIGN_AVAILABLE 1
#else
#	define LLC_TEST_ALIGN_AVAILABLE 0
#endif

GDEFINE_ENUM_TYPE(TEST_ALIGN_H_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_ALIGN_H_RESULT, HEADER_AVAILABLE  , 0, "The canonical llc_align.h and its axis dependency are available.");
GDEFINE_ENUM_VALUED(TEST_ALIGN_H_RESULT, ORIGIN            , 1, "Origin alignment preserves both coordinates.");
GDEFINE_ENUM_VALUED(TEST_ALIGN_H_RESULT, RETURN_REFERENCE  , 2, "realignCoord returns the exact output object supplied by the caller.");
GDEFINE_ENUM_VALUED(TEST_ALIGN_H_RESULT, RIGHT_BOTTOM      , 3, "Right/bottom alignment reflects both coordinates inside the target.");
GDEFINE_ENUM_VALUED(TEST_ALIGN_H_RESULT, HCENTER_GPK_DIFF  , 4, "Horizontal center adds half-width rather than taking GPK's right branch.");
GDEFINE_ENUM_VALUED(TEST_ALIGN_H_RESULT, CENTER            , 5, "Full center applies independent horizontal and vertical centering.");
GDEFINE_ENUM_VALUED(TEST_ALIGN_H_RESULT, CENTER_RIGHT      , 6, "Right reflection composes with independent vertical centering.");
GDEFINE_ENUM_VALUED(TEST_ALIGN_H_RESULT, MIXED_TYPES       , 7, "Signed coordinates accept an unsigned target without changing result type.");
GDEFINE_ENUM_VALUED(TEST_ALIGN_H_RESULT, UNSIGNED_WRAP     , 8, "Unsigned right/bottom reflection preserves the historical zero-target wrap.");

sttc ::llc::err_t testAlignHAvailability(::llc::ATestError & results) {
	LLC_TEST_CHECK(results, TEST_ALIGN_H_RESULT_HEADER_AVAILABLE, 0 == LLC_TEST_ALIGN_AVAILABLE);
	rtrn 0;
}

#if LLC_TEST_ALIGN_AVAILABLE
sttc ::llc::err_t testAlignHOrigin(::llc::ATestError & results) {
	::llc::n2s2_t output = {};
	::llc::realignCoord(::llc::n2u2_t{10, 8}, ::llc::n2s2_t{-3, 5}, output, ::llc::ALIGN_ORIGIN);
	cnst ::llc::n2s2_t expected = {-3, 5};
	LLC_TEST_CHECK(results, TEST_ALIGN_H_RESULT_ORIGIN, output != expected);
	rtrn 0;
}

sttc ::llc::err_t testAlignHReturnReference(::llc::ATestError & results) {
	::llc::n2s2_t output = {};
	::llc::n2s2_t & returned = ::llc::realignCoord(::llc::n2u2_t{10, 8}, ::llc::n2s2_t{2, 1}, output, ::llc::ALIGN_ORIGIN);
	LLC_TEST_CHECK(results, TEST_ALIGN_H_RESULT_RETURN_REFERENCE, &returned != &output);
	rtrn 0;
}

sttc ::llc::err_t testAlignHRightBottom(::llc::ATestError & results) {
	::llc::n2s2_t output = {};
	::llc::realignCoord(::llc::n2u2_t{10, 8}, ::llc::n2s2_t{2, 1}, output, ::llc::ALIGN_BOTTOM_RIGHT);
	cnst ::llc::n2s2_t expected = {7, 6};
	LLC_TEST_CHECK(results, TEST_ALIGN_H_RESULT_RIGHT_BOTTOM, output != expected);
	rtrn 0;
}

sttc ::llc::err_t testAlignHHCenterGPKDifferential(::llc::ATestError & results) {
	::llc::n2s2_t output = {};
	::llc::realignCoord(::llc::n2u2_t{10, 8}, ::llc::n2s2_t{1, 2}, output, ::llc::ALIGN_HCENTER);
	cnst ::llc::n2s2_t expected = {6, 2};
	LLC_TEST_CHECKF(results, TEST_ALIGN_H_RESULT_HCENTER_GPK_DIFF, output != expected
		, "actual:%i,%i expected:%i,%i", output.x, output.y, expected.x, expected.y);
	rtrn 0;
}

sttc ::llc::err_t testAlignHCenter(::llc::ATestError & results) {
	::llc::n2s2_t output = {};
	::llc::realignCoord(::llc::n2u2_t{10, 8}, ::llc::n2s2_t{2, 1}, output, ::llc::ALIGN_CENTER);
	cnst ::llc::n2s2_t expected = {7, 5};
	LLC_TEST_CHECK(results, TEST_ALIGN_H_RESULT_CENTER, output != expected);
	rtrn 0;
}

sttc ::llc::err_t testAlignHCenterRight(::llc::ATestError & results) {
	::llc::n2s2_t output = {};
	::llc::realignCoord(::llc::n2u2_t{10, 8}, ::llc::n2s2_t{2, 1}, output, ::llc::ALIGN_CENTER_RIGHT);
	cnst ::llc::n2s2_t expected = {7, 5};
	LLC_TEST_CHECK(results, TEST_ALIGN_H_RESULT_CENTER_RIGHT, output != expected);
	rtrn 0;
}

sttc ::llc::err_t testAlignHMixedTypes(::llc::ATestError & results) {
	::llc::n2s1_t output = {};
	::llc::realignCoord(::llc::n2u2_t{101, 55}, ::llc::n2s1_t{-4, 3}, output, ::llc::ALIGN_CENTER_TOP);
	cnst ::llc::n2s1_t expected = {46, 3};
	LLC_TEST_CHECK(results, TEST_ALIGN_H_RESULT_MIXED_TYPES, output != expected);
	rtrn 0;
}

sttc ::llc::err_t testAlignHUnsignedWrap(::llc::ATestError & results) {
	::llc::n2u2_t output = {};
	::llc::realignCoord(::llc::n2u2_t{0, 0}, ::llc::n2u2_t{0, 0}, output, ::llc::ALIGN_BOTTOM_RIGHT);
	cnst ::llc::n2u2_t expected = {::llc::u2_t(-1), ::llc::u2_t(-1)};
	LLC_TEST_CHECK(results, TEST_ALIGN_H_RESULT_UNSIGNED_WRAP, output != expected);
	rtrn 0;
}
#endif

::llc::err_t testAlignH(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testAlignHAvailability(results));
#if LLC_TEST_ALIGN_AVAILABLE
	result = ::llc::min(result, ::testAlignHOrigin(results));
	result = ::llc::min(result, ::testAlignHReturnReference(results));
	result = ::llc::min(result, ::testAlignHRightBottom(results));
	result = ::llc::min(result, ::testAlignHHCenterGPKDifferential(results));
	result = ::llc::min(result, ::testAlignHCenter(results));
	result = ::llc::min(result, ::testAlignHCenterRight(results));
	result = ::llc::min(result, ::testAlignHMixedTypes(results));
	result = ::llc::min(result, ::testAlignHUnsignedWrap(results));
#endif
	rtrn result;
}
