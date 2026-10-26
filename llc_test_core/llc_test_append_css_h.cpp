#include "llc_test_core_suites.h"
#include "llc_append_css.h"

GDEFINE_ENUM_TYPE(TEST_APPEND_CSS_H_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_APPEND_CSS_H_RESULT, TAG_EMPTY_NAME        ,  0, "An empty selector produces an empty CSS rule.");
GDEFINE_ENUM_VALUED(TEST_APPEND_CSS_H_RESULT, TAG_NO_ATTRIBUTES    ,  1, "A selector without attributes produces a complete CSS rule.");
GDEFINE_ENUM_VALUED(TEST_APPEND_CSS_H_RESULT, TAG_ONE_ATTRIBUTE   ,  2, "A single attribute is preserved inside the CSS rule.");
GDEFINE_ENUM_VALUED(TEST_APPEND_CSS_H_RESULT, TAG_MANY_ATTRIBUTES ,  3, "Variadic attributes retain their source order.");
GDEFINE_ENUM_VALUED(TEST_APPEND_CSS_H_RESULT, TAG_APPEND          ,  4, "A CSS rule appends without changing an existing prefix.");
GDEFINE_ENUM_VALUED(TEST_APPEND_CSS_H_RESULT, DEFAULT_BORDER      ,  5, "Border width defaults to absolute pixels.");
GDEFINE_ENUM_VALUED(TEST_APPEND_CSS_H_RESULT, DEFAULT_WIDTH       ,  6, "Control width defaults to a relative percentage.");
GDEFINE_ENUM_VALUED(TEST_APPEND_CSS_H_RESULT, DEFAULT_HEIGHT      ,  7, "Control height defaults to a relative percentage.");
GDEFINE_ENUM_VALUED(TEST_APPEND_CSS_H_RESULT, DEFAULT_SIZE        ,  8, "Control size defaults to relative percentages.");
GDEFINE_ENUM_VALUED(TEST_APPEND_CSS_H_RESULT, DEFAULT_BORDERED_SIZE, 9, "Bordered size defaults to the relative representation.");
GDEFINE_ENUM_VALUED(TEST_APPEND_CSS_H_RESULT, API_SIGNATURES      , 10, "Every non-template declaration has the expected public signature.");
GDEFINE_ENUM_VALUED(TEST_APPEND_CSS_H_RESULT, RULE_COMPOSITION    , 11, "Generated declarations compose into one complete selector rule.");

sttc bool testAppendCSSHeaderOutput(cnst ::llc::asc_t & output, ::llc::vcst_t expected) {
	rtrn output.size() != expected.size() || 0 != memcmp(output.begin(), expected.begin(), expected.size());
}

sttc ::llc::err_t testAppendCSSHTagEmptyName(::llc::ATestError & results) {
	::llc::asc_t output;
	cnst ::llc::vcst_t expected = "{}\n";
	cnst ::llc::err_t appended = ::llc::appendCSSTag(output, "");
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_H_RESULT_TAG_EMPTY_NAME, appended != (::llc::err_t)expected.size() || ::testAppendCSSHeaderOutput(output, expected));
	rtrn 0;
}

sttc ::llc::err_t testAppendCSSHTagNoAttributes(::llc::ATestError & results) {
	::llc::asc_t output;
	cnst ::llc::vcst_t expected = "body{}\n";
	cnst ::llc::err_t appended = ::llc::appendCSSTag(output, "body");
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_H_RESULT_TAG_NO_ATTRIBUTES, appended != (::llc::err_t)expected.size() || ::testAppendCSSHeaderOutput(output, expected));
	rtrn 0;
}

sttc ::llc::err_t testAppendCSSHTagOneAttribute(::llc::ATestError & results) {
	::llc::asc_t output;
	cnst ::llc::vcst_t expected = "body{width:10px;}\n";
	cnst ::llc::err_t appended = ::llc::appendCSSTag(output, "body", "width:10px;");
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_H_RESULT_TAG_ONE_ATTRIBUTE, appended != (::llc::err_t)expected.size() || ::testAppendCSSHeaderOutput(output, expected));
	rtrn 0;
}

sttc ::llc::err_t testAppendCSSHTagManyAttributes(::llc::ATestError & results) {
	::llc::asc_t output;
	cnst ::llc::vcst_t expected = ".panel{width:80%;height:25%;border-width:3px;}\n";
	cnst ::llc::err_t appended = ::llc::appendCSSTag(output, ".panel", "width:80%;", "height:25%;", "border-width:3px;");
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_H_RESULT_TAG_MANY_ATTRIBUTES, appended != (::llc::err_t)expected.size() || ::testAppendCSSHeaderOutput(output, expected));
	rtrn 0;
}

sttc ::llc::err_t testAppendCSSHTagAppend(::llc::ATestError & results) {
	::llc::asc_t output = "prefix\n";
	cnst ::llc::vcst_t expected = "prefix\nbutton{height:20px;}\n";
	cnst ::llc::err_t appended = ::llc::appendCSSTag(output, "button", "height:20px;");
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_H_RESULT_TAG_APPEND, appended != (::llc::err_t)(expected.size() - 7) || ::testAppendCSSHeaderOutput(output, expected));
	rtrn 0;
}

sttc ::llc::err_t testAppendCSSHDefaultBorder(::llc::ATestError & results) {
	::llc::asc_t output;
	cnst ::llc::vcst_t expected = "border-width:3px;";
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_H_RESULT_DEFAULT_BORDER, ::llc::appendCSSBorderWidth(output, 3) != (::llc::err_t)expected.size() || ::testAppendCSSHeaderOutput(output, expected));
	rtrn 0;
}

sttc ::llc::err_t testAppendCSSHDefaultWidth(::llc::ATestError & results) {
	::llc::asc_t output;
	cnst ::llc::vcst_t expected = "width:80%;";
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_H_RESULT_DEFAULT_WIDTH, ::llc::appendCSSControlWidth(output, 80) != (::llc::err_t)expected.size() || ::testAppendCSSHeaderOutput(output, expected));
	rtrn 0;
}

sttc ::llc::err_t testAppendCSSHDefaultHeight(::llc::ATestError & results) {
	::llc::asc_t output;
	cnst ::llc::vcst_t expected = "height:25%;";
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_H_RESULT_DEFAULT_HEIGHT, ::llc::appendCSSControlHeight(output, 25) != (::llc::err_t)expected.size() || ::testAppendCSSHeaderOutput(output, expected));
	rtrn 0;
}

sttc ::llc::err_t testAppendCSSHDefaultSize(::llc::ATestError & results) {
	::llc::asc_t output;
	cnst ::llc::vcst_t expected = "width:80%;height:25%;";
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_H_RESULT_DEFAULT_SIZE, ::llc::appendCSSControlSize(output, {80, 25}) != (::llc::err_t)expected.size() || ::testAppendCSSHeaderOutput(output, expected));
	rtrn 0;
}

sttc ::llc::err_t testAppendCSSHDefaultBorderedSize(::llc::ATestError & results) {
	::llc::asc_t output;
	cnst ::llc::vcst_t expected = "width:calc(80% - 6px);height:calc(25% - 6px);border-width:3px;";
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_H_RESULT_DEFAULT_BORDERED_SIZE, ::llc::appendCSSSizeWithBorderWidth(output, {80, 25}, 3) != (::llc::err_t)expected.size() || ::testAppendCSSHeaderOutput(output, expected));
	rtrn 0;
}

sttc ::llc::err_t testAppendCSSHAPISignatures(::llc::ATestError & results) {
	typedef ::llc::err_t (*TBorderWidth)(::llc::asc_t &, uint8_t, bool);
	typedef ::llc::err_t (*TControlLimit)(::llc::asc_t &, uint16_t, bool);
	typedef ::llc::err_t (*TControlSize)(::llc::asc_t &, ::llc::n2u1_t, bool);
	typedef ::llc::err_t (*TBorderedSize)(::llc::asc_t &, ::llc::n2u1_t, uint8_t);
	typedef ::llc::err_t (*TBorderedSizeDispatch)(::llc::asc_t &, ::llc::n2u1_t, uint8_t, bool);
	cnst TBorderWidth borderWidth = &::llc::appendCSSBorderWidth;
	cnst TControlLimit controlWidth = &::llc::appendCSSControlWidth;
	cnst TControlLimit controlHeight = &::llc::appendCSSControlHeight;
	cnst TControlSize controlSize = &::llc::appendCSSControlSize;
	cnst TBorderedSize borderedAbsolute = &::llc::appendCSSSizeWithBorderWidthAbsolute;
	cnst TBorderedSize borderedRelative = &::llc::appendCSSSizeWithBorderWidthRelative;
	cnst TBorderedSizeDispatch borderedDispatch = &::llc::appendCSSSizeWithBorderWidth;
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_H_RESULT_API_SIGNATURES, !borderWidth || !controlWidth || !controlHeight || !controlSize || !borderedAbsolute || !borderedRelative || !borderedDispatch);
	rtrn 0;
}

sttc ::llc::err_t testAppendCSSHRuleComposition(::llc::ATestError & results) {
	::llc::asc_t attributes;
	if_fail_fe(::llc::appendCSSControlSize(attributes, {80, 25}));
	if_fail_fe(::llc::appendCSSBorderWidth(attributes, 3));
	::llc::asc_t output;
	cnst ::llc::vcst_t expected = ".panel{width:80%;height:25%;border-width:3px;}\n";
	cnst ::llc::err_t appended = ::llc::appendCSSTag(output, ".panel", attributes);
	LLC_TEST_CHECK(results, TEST_APPEND_CSS_H_RESULT_RULE_COMPOSITION, appended != (::llc::err_t)expected.size() || ::testAppendCSSHeaderOutput(output, expected));
	rtrn 0;
}

::llc::err_t testAppendCSSH(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testAppendCSSHTagEmptyName(results));
	result = ::llc::min(result, ::testAppendCSSHTagNoAttributes(results));
	result = ::llc::min(result, ::testAppendCSSHTagOneAttribute(results));
	result = ::llc::min(result, ::testAppendCSSHTagManyAttributes(results));
	result = ::llc::min(result, ::testAppendCSSHTagAppend(results));
	result = ::llc::min(result, ::testAppendCSSHDefaultBorder(results));
	result = ::llc::min(result, ::testAppendCSSHDefaultWidth(results));
	result = ::llc::min(result, ::testAppendCSSHDefaultHeight(results));
	result = ::llc::min(result, ::testAppendCSSHDefaultSize(results));
	result = ::llc::min(result, ::testAppendCSSHDefaultBorderedSize(results));
	result = ::llc::min(result, ::testAppendCSSHAPISignatures(results));
	result = ::llc::min(result, ::testAppendCSSHRuleComposition(results));
	rtrn result;
}
