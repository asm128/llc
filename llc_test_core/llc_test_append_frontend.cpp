#include "llc_test_core_suites.h"
#include "llc_append_frontend.h"

GDEFINE_ENUM_TYPE(TEST_APPEND_FRONTEND_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_APPEND_FRONTEND_RESULT, INCLUDE_GUARDS , 0, "The umbrella exposes the CSS, HTML, JavaScript and JSON headers.");
GDEFINE_ENUM_VALUED(TEST_APPEND_FRONTEND_RESULT, CSS_EXPORT     , 1, "CSS declarations are callable through the umbrella alone.");
GDEFINE_ENUM_VALUED(TEST_APPEND_FRONTEND_RESULT, CSS_TEMPLATE   , 2, "The CSS rule template is callable through the umbrella alone.");
GDEFINE_ENUM_VALUED(TEST_APPEND_FRONTEND_RESULT, HTML_EXPORT    , 3, "HTML appenders are callable through the umbrella alone.");
GDEFINE_ENUM_VALUED(TEST_APPEND_FRONTEND_RESULT, JSON_EXPORT    , 4, "JSON appenders are callable through the umbrella alone.");
GDEFINE_ENUM_VALUED(TEST_APPEND_FRONTEND_RESULT, API_SURFACE    , 5, "Representative CSS, HTML and JSON signatures remain visible.");
GDEFINE_ENUM_VALUED(TEST_APPEND_FRONTEND_RESULT, SHARED_OUTPUT  , 6, "Frontend families append in order to one shared output.");

sttc bool testAppendFrontendOutput(cnst ::llc::asc_t & output, ::llc::vcst_t expected) {
	rtrn output.size() != expected.size() || 0 != memcmp(output.begin(), expected.begin(), expected.size());
}

sttc ::llc::err_t testAppendFrontendIncludeGuards(::llc::ATestError & results) {
#if defined(LLC_APPEND_FRONTEND_H) && defined(LLC_APPEND_CSS_H) && defined(LLC_APPEND_HTML_H) && defined(LLC_APPEND_JS_H) && defined(LLC_APPEND_JSON_H)
	stxp bool allHeadersVisible = true;
#else
	stxp bool allHeadersVisible = false;
#endif
	LLC_TEST_CHECK(results, TEST_APPEND_FRONTEND_RESULT_INCLUDE_GUARDS, !allHeadersVisible);
	rtrn 0;
}

sttc ::llc::err_t testAppendFrontendCSSExport(::llc::ATestError & results) {
	::llc::asc_t output;
	cnst ::llc::vcst_t expected = "width:25%;";
	LLC_TEST_CHECK(results, TEST_APPEND_FRONTEND_RESULT_CSS_EXPORT, ::llc::appendCSSControlWidth(output, 25) != (::llc::err_t)expected.size() || ::testAppendFrontendOutput(output, expected));
	rtrn 0;
}

sttc ::llc::err_t testAppendFrontendCSSTemplate(::llc::ATestError & results) {
	::llc::asc_t output;
	cnst ::llc::vcst_t expected = ".item{height:10px;}\n";
	LLC_TEST_CHECK(results, TEST_APPEND_FRONTEND_RESULT_CSS_TEMPLATE, ::llc::appendCSSTag(output, ".item", "height:10px;") != (::llc::err_t)expected.size() || ::testAppendFrontendOutput(output, expected));
	rtrn 0;
}

sttc ::llc::err_t testAppendFrontendHTMLExport(::llc::ATestError & results) {
	::llc::asc_t output;
	cnst ::llc::vcst_t expected = "<body>ready</body>";
	LLC_TEST_CHECK(results, TEST_APPEND_FRONTEND_RESULT_HTML_EXPORT, ::llc::appendHtmlBody(output, {}, "ready") != (::llc::err_t)expected.size() || ::testAppendFrontendOutput(output, expected));
	rtrn 0;
}

sttc ::llc::err_t testAppendFrontendJSONExport(::llc::ATestError & results) {
	::llc::asc_t output;
	cnst ::llc::vcst_t expected = "\"state\":\"ready\"";
	LLC_TEST_CHECK(results, TEST_APPEND_FRONTEND_RESULT_JSON_EXPORT, ::llc::appendKeyString(output, "state", "ready") != (::llc::err_t)expected.size() || ::testAppendFrontendOutput(output, expected));
	rtrn 0;
}

sttc ::llc::err_t testAppendFrontendAPISurface(::llc::ATestError & results) {
	typedef ::llc::err_t (*TCSS)(::llc::asc_t &, uint16_t, bool);
	typedef ::llc::err_t (*THTML)(::llc::asc_t &, ::llc::vcst_t, ::llc::vcst_t);
	typedef ::llc::err_t (*TJSON)(::llc::asc_t &, ::llc::vcst_t, ::llc::vcst_t, ::llc::b8_t);
	cnst TCSS css = &::llc::appendCSSControlWidth;
	cnst THTML html = &::llc::appendHtmlBody;
	cnst TJSON json = &::llc::appendKeyString;
	LLC_TEST_CHECK(results, TEST_APPEND_FRONTEND_RESULT_API_SURFACE, !css || !html || !json);
	rtrn 0;
}

sttc ::llc::err_t testAppendFrontendSharedOutput(::llc::ATestError & results) {
	::llc::asc_t output;
	if_fail_fe(::llc::appendCSSTag(output, ".item", "width:1px;"));
	if_fail_fe(::llc::appendHtmlBody(output, {}, "ready"));
	if_fail_fe(::llc::appendKeyString(output, "state", "ready"));
	cnst ::llc::vcst_t expected = ".item{width:1px;}\n<body>ready</body>\"state\":\"ready\"";
	LLC_TEST_CHECK(results, TEST_APPEND_FRONTEND_RESULT_SHARED_OUTPUT, ::testAppendFrontendOutput(output, expected));
	rtrn 0;
}

::llc::err_t testAppendFrontend(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testAppendFrontendIncludeGuards(results));
	result = ::llc::min(result, ::testAppendFrontendCSSExport(results));
	result = ::llc::min(result, ::testAppendFrontendCSSTemplate(results));
	result = ::llc::min(result, ::testAppendFrontendHTMLExport(results));
	result = ::llc::min(result, ::testAppendFrontendJSONExport(results));
	result = ::llc::min(result, ::testAppendFrontendAPISurface(results));
	result = ::llc::min(result, ::testAppendFrontendSharedOutput(results));
	rtrn result;
}
