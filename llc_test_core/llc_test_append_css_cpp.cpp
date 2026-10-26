#include "llc_test_core_suites.h"
#include "llc_append_css.h"

GDEFINE_ENUM_TYPE(TEST_APPEND_CSS_CPP_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_APPEND_CSS_CPP_RESULT, BORDER_ABSOLUTE    ,  0, "Absolute border width emits a pixel declaration.");
GDEFINE_ENUM_VALUED(TEST_APPEND_CSS_CPP_RESULT, BORDER_RELATIVE    ,  1, "Relative border width emits a percent declaration.");
GDEFINE_ENUM_VALUED(TEST_APPEND_CSS_CPP_RESULT, WIDTH_UNITS        ,  2, "Control width emits the selected unit at numeric boundaries.");
GDEFINE_ENUM_VALUED(TEST_APPEND_CSS_CPP_RESULT, HEIGHT_UNITS       ,  3, "Control height emits the selected unit at numeric boundaries.");
GDEFINE_ENUM_VALUED(TEST_APPEND_CSS_CPP_RESULT, SIZE_ORDER         ,  4, "Control size emits width before height and returns their combined length.");
GDEFINE_ENUM_VALUED(TEST_APPEND_CSS_CPP_RESULT, ABSOLUTE_BORDER    ,  5, "Absolute bordered size subtracts both border edges from each dimension.");
GDEFINE_ENUM_VALUED(TEST_APPEND_CSS_CPP_RESULT, ABSOLUTE_UNDERFLOW ,  6, "Absolute bordered size preserves the historical signed-to-u16 wrap contract.");
GDEFINE_ENUM_VALUED(TEST_APPEND_CSS_CPP_RESULT, RELATIVE_BORDER    ,  7, "Relative bordered size emits calc expressions and an absolute border.");
GDEFINE_ENUM_VALUED(TEST_APPEND_CSS_CPP_RESULT, MAX_VALUES         ,  8, "Relative bordered size preserves maximum dimension and border values.");
GDEFINE_ENUM_VALUED(TEST_APPEND_CSS_CPP_RESULT, DISPATCH           ,  9, "The bordered-size dispatcher selects the requested representation.");
GDEFINE_ENUM_VALUED(TEST_APPEND_CSS_CPP_RESULT, APPEND_PRESERVATION, 10, "CSS declarations append without changing an existing prefix.");

sttc bool testAppendCSSOutput(cnst ::llc::asc_t & output, ::llc::vcst_t expected) {
	rtrn output.size() != expected.size() || 0 != memcmp(output.begin(), expected.begin(), expected.size());
}

sttc ::llc::err_t testAppendCSSCPPBorderAbsolute(::llc::ATestError & results) {
	::llc::asc_t output;
	cnst ::llc::vcst_t expected = "border-width:3px;";
	cnst ::llc::err_t appended = ::llc::appendCSSBorderWidth(output, 3, true);
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_CPP_RESULT_BORDER_ABSOLUTE, appended != (::llc::err_t)expected.size() || ::testAppendCSSOutput(output, expected));
	rtrn 0;
}

sttc ::llc::err_t testAppendCSSCPPBorderRelative(::llc::ATestError & results) {
	::llc::asc_t output;
	cnst ::llc::vcst_t expected = "border-width:25%;";
	cnst ::llc::err_t appended = ::llc::appendCSSBorderWidth(output, 25, false);
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_CPP_RESULT_BORDER_RELATIVE, appended != (::llc::err_t)expected.size() || ::testAppendCSSOutput(output, expected));
	rtrn 0;
}

sttc ::llc::err_t testAppendCSSCPPWidthUnits(::llc::ATestError & results) {
	::llc::asc_t relative;
	cnst ::llc::vcst_t expectedRelative = "width:0%;";
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_CPP_RESULT_WIDTH_UNITS, ::llc::appendCSSControlWidth(relative, 0, false) != (::llc::err_t)expectedRelative.size() || ::testAppendCSSOutput(relative, expectedRelative));
	::llc::asc_t absolute;
	cnst ::llc::vcst_t expectedAbsolute = "width:65535px;";
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_CPP_RESULT_WIDTH_UNITS, ::llc::appendCSSControlWidth(absolute, 65535, true) != (::llc::err_t)expectedAbsolute.size() || ::testAppendCSSOutput(absolute, expectedAbsolute));
	rtrn 0;
}

sttc ::llc::err_t testAppendCSSCPPHeightUnits(::llc::ATestError & results) {
	::llc::asc_t relative;
	cnst ::llc::vcst_t expectedRelative = "height:0%;";
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_CPP_RESULT_HEIGHT_UNITS, ::llc::appendCSSControlHeight(relative, 0, false) != (::llc::err_t)expectedRelative.size() || ::testAppendCSSOutput(relative, expectedRelative));
	::llc::asc_t absolute;
	cnst ::llc::vcst_t expectedAbsolute = "height:65535px;";
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_CPP_RESULT_HEIGHT_UNITS, ::llc::appendCSSControlHeight(absolute, 65535, true) != (::llc::err_t)expectedAbsolute.size() || ::testAppendCSSOutput(absolute, expectedAbsolute));
	rtrn 0;
}

sttc ::llc::err_t testAppendCSSCPPSizeOrder(::llc::ATestError & results) {
	::llc::asc_t output;
	cnst ::llc::vcst_t expected = "width:80%;height:25%;";
	cnst ::llc::err_t appended = ::llc::appendCSSControlSize(output, {80, 25}, false);
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_CPP_RESULT_SIZE_ORDER, appended != (::llc::err_t)expected.size() || ::testAppendCSSOutput(output, expected));
	rtrn 0;
}

sttc ::llc::err_t testAppendCSSCPPAbsoluteBorder(::llc::ATestError & results) {
	::llc::asc_t output;
	cnst ::llc::vcst_t expected = "width:94px;height:44px;border-width:3px;";
	cnst ::llc::err_t appended = ::llc::appendCSSSizeWithBorderWidthAbsolute(output, {100, 50}, 3);
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_CPP_RESULT_ABSOLUTE_BORDER, appended != (::llc::err_t)expected.size() || ::testAppendCSSOutput(output, expected));
	rtrn 0;
}

sttc ::llc::err_t testAppendCSSCPPAbsoluteUnderflow(::llc::ATestError & results) {
	::llc::asc_t output;
	cnst ::llc::vcst_t expected = "width:65531px;height:65532px;border-width:3px;";
	cnst ::llc::err_t appended = ::llc::appendCSSSizeWithBorderWidthAbsolute(output, {1, 2}, 3);
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_CPP_RESULT_ABSOLUTE_UNDERFLOW, appended != (::llc::err_t)expected.size() || ::testAppendCSSOutput(output, expected));
	rtrn 0;
}

sttc ::llc::err_t testAppendCSSCPPRelativeBorder(::llc::ATestError & results) {
	::llc::asc_t output;
	cnst ::llc::vcst_t expected = "width:calc(80% - 6px);height:calc(25% - 6px);border-width:3px;";
	cnst ::llc::err_t appended = ::llc::appendCSSSizeWithBorderWidthRelative(output, {80, 25}, 3);
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_CPP_RESULT_RELATIVE_BORDER, appended != (::llc::err_t)expected.size() || ::testAppendCSSOutput(output, expected));
	rtrn 0;
}

sttc ::llc::err_t testAppendCSSCPPMaxValues(::llc::ATestError & results) {
	::llc::asc_t output;
	cnst ::llc::vcst_t expected = "width:calc(65535% - 510px);height:calc(65535% - 510px);border-width:255px;";
	cnst ::llc::err_t appended = ::llc::appendCSSSizeWithBorderWidthRelative(output, {65535, 65535}, 255);
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_CPP_RESULT_MAX_VALUES, appended != (::llc::err_t)expected.size() || ::testAppendCSSOutput(output, expected));
	rtrn 0;
}

sttc ::llc::err_t testAppendCSSCPPDispatch(::llc::ATestError & results) {
	::llc::asc_t absolute;
	cnst ::llc::vcst_t expectedAbsolute = "width:94px;height:44px;border-width:3px;";
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_CPP_RESULT_DISPATCH, ::llc::appendCSSSizeWithBorderWidth(absolute, {100, 50}, 3, true) != (::llc::err_t)expectedAbsolute.size() || ::testAppendCSSOutput(absolute, expectedAbsolute));
	::llc::asc_t relative;
	cnst ::llc::vcst_t expectedRelative = "width:calc(80% - 6px);height:calc(25% - 6px);border-width:3px;";
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_CPP_RESULT_DISPATCH, ::llc::appendCSSSizeWithBorderWidth(relative, {80, 25}, 3, false) != (::llc::err_t)expectedRelative.size() || ::testAppendCSSOutput(relative, expectedRelative));
	rtrn 0;
}

sttc ::llc::err_t testAppendCSSCPPAppendPreservation(::llc::ATestError & results) {
	::llc::asc_t output = "prefix;";
	cnst ::llc::vcst_t expected = "prefix;width:50%;";
	cnst ::llc::err_t appended = ::llc::appendCSSControlWidth(output, 50, false);
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_CPP_RESULT_APPEND_PRESERVATION, appended != (::llc::err_t)(expected.size() - 7) || ::testAppendCSSOutput(output, expected));
	rtrn 0;
}

::llc::err_t testAppendCSSCPP(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testAppendCSSCPPBorderAbsolute(results));
	result = ::llc::min(result, ::testAppendCSSCPPBorderRelative(results));
	result = ::llc::min(result, ::testAppendCSSCPPWidthUnits(results));
	result = ::llc::min(result, ::testAppendCSSCPPHeightUnits(results));
	result = ::llc::min(result, ::testAppendCSSCPPSizeOrder(results));
	result = ::llc::min(result, ::testAppendCSSCPPAbsoluteBorder(results));
	result = ::llc::min(result, ::testAppendCSSCPPAbsoluteUnderflow(results));
	result = ::llc::min(result, ::testAppendCSSCPPRelativeBorder(results));
	result = ::llc::min(result, ::testAppendCSSCPPMaxValues(results));
	result = ::llc::min(result, ::testAppendCSSCPPDispatch(results));
	result = ::llc::min(result, ::testAppendCSSCPPAppendPreservation(results));
	rtrn result;
}
