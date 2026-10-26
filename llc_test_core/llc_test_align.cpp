#include "llc_test_core_suites.h"
#include "llc_align.h"

GDEFINE_ENUM_TYPE(TEST_ALIGN_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_ALIGN_RESULT, ORIGIN            , 0, "Origin alignment preserves the supplied coordinate and returns the output reference.");
GDEFINE_ENUM_VALUED(TEST_ALIGN_RESULT, CENTER            , 1, "Center alignment offsets both axes by half the target extent.");
GDEFINE_ENUM_VALUED(TEST_ALIGN_RESULT, EDGES             , 2, "Right and bottom alignment mirror coordinates against the target edges.");
GDEFINE_ENUM_VALUED(TEST_ALIGN_RESULT, ALIASES_PRECEDENCE, 3, "Center aliases preserve center precedence while left and top preserve their coordinates.");
GDEFINE_ENUM_VALUED(TEST_ALIGN_RESULT, MIXED_TYPES        , 4, "Alignment accepts distinct target and coordinate scalar types.");

sttc ::llc::err_t testAlignOrigin(::llc::ATestError & results) {
	cnst ::llc::n2u2_t target = {10, 8};
	cnst ::llc::n2s2_t source = {2, 3};
	::llc::n2s2_t output = {};
	::llc::n2s2_t & returned = ::llc::realignCoord(target, source, output, ::llc::ALIGN_ORIGIN);
	LLC_TEST_CHECK(results, TEST_ALIGN_RESULT_ORIGIN, &returned != &output || output != source);
	rtrn 0;
}

sttc ::llc::err_t testAlignCenter(::llc::ATestError & results) {
	cnst ::llc::n2u2_t target = {10, 8};
	cnst ::llc::n2s2_t source = {2, 3};
	::llc::n2s2_t output = {};
	::llc::realignCoord(target, source, output, ::llc::ALIGN_CENTER);
	cnst ::llc::n2s2_t expected = {7, 7};
	LLC_TEST_CHECK(results, TEST_ALIGN_RESULT_CENTER, output != expected);
	rtrn 0;
}

sttc ::llc::err_t testAlignEdges(::llc::ATestError & results) {
	cnst ::llc::n2u2_t target = {10, 8};
	cnst ::llc::n2s2_t source = {2, 3};
	::llc::n2s2_t output = {};
	::llc::realignCoord(target, source, output, ::llc::ALIGN_BOTTOM_RIGHT);
	cnst ::llc::n2s2_t expected = {7, 4};
	LLC_TEST_CHECK(results, TEST_ALIGN_RESULT_EDGES, output != expected);
	rtrn 0;
}

sttc ::llc::err_t testAlignAliasesPrecedence(::llc::ATestError & results) {
	cnst ::llc::n2u2_t target = {10, 8};
	cnst ::llc::n2s2_t source = {2, 3};
	::llc::n2s2_t output = {};
	::llc::realignCoord(target, source, output, ::llc::ALIGN_XCENTER);
	LLC_TEST_CHECK(results, TEST_ALIGN_RESULT_ALIASES_PRECEDENCE, output.x != 7 || output.y != 3);
	::llc::realignCoord(target, source, output, ::llc::ALIGN_YCENTER);
	LLC_TEST_CHECK(results, TEST_ALIGN_RESULT_ALIASES_PRECEDENCE, output.x != 2 || output.y != 7);
	::llc::realignCoord(target, source, output, ::llc::ALIGN_TOP_LEFT);
	LLC_TEST_CHECK(results, TEST_ALIGN_RESULT_ALIASES_PRECEDENCE, output != source);
	rtrn 0;
}

sttc ::llc::err_t testAlignMixedTypes(::llc::ATestError & results) {
	cnst ::llc::n2u1_t target = {10, 8};
	cnst ::llc::n2s2_t source = {-2, -3};
	::llc::n2s2_t output = {};
	::llc::realignCoord(target, source, output, ::llc::ALIGN_BOTTOM_RIGHT);
	cnst ::llc::n2s2_t expected = {11, 10};
	LLC_TEST_CHECK(results, TEST_ALIGN_RESULT_MIXED_TYPES, output != expected);
	rtrn 0;
}

::llc::err_t testAlign(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testAlignOrigin(results));
	result = ::llc::min(result, ::testAlignCenter(results));
	result = ::llc::min(result, ::testAlignEdges(results));
	result = ::llc::min(result, ::testAlignAliasesPrecedence(results));
	result = ::llc::min(result, ::testAlignMixedTypes(results));
	rtrn result;
}
