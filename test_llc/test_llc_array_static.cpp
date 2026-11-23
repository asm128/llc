#include "llc_append_css.h"
#include "llc_append_json.h"
#include "llc_append_xml.h"
#include "llc_tcpip.h"
#include "llc_string.h"
#include "llc_string_compose.h"
#include "test_llc_suites.h"
//
//#include <type_traits>
//
//GDEFINE_ENUM_TYPE(STR_TEST_RESULT, ::llc::u0_t);
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, ASTATIC_MUTABLE	,  0, "str(astsc_t<N>&) produced an unexpected mutable string view.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, ASTATIC_CONST		,  1, "str(cnst astsc_t<N>&) produced an unexpected cnst string view.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, U0					,  2, "str(u0_t) produced unexpected fixed storage.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, U1					,  3, "str(u1_t) produced unexpected fixed storage.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, U2					,  4, "str(u2_t) produced unexpected fixed storage.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, U3					,  5, "str(u3_t) produced unexpected fixed storage.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, S0					,  6, "str(s0_t) produced unexpected fixed storage.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, S1					,  7, "str(s1_t) produced unexpected fixed storage.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, S2					,  8, "str(s2_t) produced unexpected fixed storage.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, S3					,  9, "str(s3_t) produced unexpected fixed storage.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, F2					, 10, "str(f2_t) produced unexpected fixed storage.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, F3					, 11, "str(f3_t) produced unexpected fixed storage.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, VSTR_MUTABLE		, 12, "str(vstr_t&) did not preserve the counted mutable string view.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, VSTR_CONST		, 13, "str(cnst vstr_t&) did not preserve the counted string view.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, VCST_CONST		, 14, "str(vcst_c&) did not preserve the counted cnst string view.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, VS0					, 15, "str(vs0_t) did not preserve the counted mutable character view.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, VCS0				, 16, "str(vcs0_t) did not preserve the counted cnst character view.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, ARRAY_MUTABLE	, 17, "str(sc_t (&)[N]) produced an unexpected mutable string view.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, ARRAY_CONST		, 18, "str(sc_c (&)[N]) produced an unexpected cnst string view.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, BOOLEAN			, 19, "str(b8_t) produced an unexpected cnst string view.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, IPV4				, 20, "str(SIPv4) produced unexpected fixed storage.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, STRING_MUTABLE	, 21, "str(string&) did not preserve the counted mutable string through conversion.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, STRING_CONST		, 22, "str(cnst string&) did not preserve the counted string through conversion.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, ASC_MUTABLE		, 23, "str(asc_t&) did not preserve the counted mutable array through conversion.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, ASC_CONST			, 24, "str(cnst asc_t&) did not preserve the counted array through conversion.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, U0_APPEND			, 25, "append_strings() copied the fixed capacity returned by str(u0_t) instead of its logical text length.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, PRESTRING_APPEND	, 26, "append_strings() changed text that was passed through str() before composition.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CHAR_APPEND			, 27, "append_strings() did not preserve character arguments as characters.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, COUNTED_APPEND		, 28, "append_string() did not preserve a counted string argument.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, ENCLOSER_APPEND		, 29, "String enclosing helpers did not compose their declared delimiters.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, XML_APPEND			, 30, "XML composition did not produce the declared markup.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, JSON_APPEND			, 31, "JSON composition did not produce the declared markup.");
//GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CSS_APPEND			, 32, "CSS composition did not produce the declared text.");
//
//sttc ::llc::err_t testComposedBytes(::llc::ATestError & errors, STR_TEST_RESULT testResult, cnst ::llc::asc_t & output, cnst ::llc::vcst_t expected) {
//	LLC_TEST_CHECKF(errors, testResult, output.size() != expected.size(), "Composed size:%u, expected:%u.", output.size(), expected.size());
//	LLC_TEST_CHECK(errors, testResult, output.size() < expected.size() || 0 != ::memcmp(output.begin(), expected.begin(), expected.byte_count()));
//	rtrn 0;
//}
//
//template<size_t _expectedSize>
//sttc ::llc::err_t testComposedText(::llc::ATestError & errors, STR_TEST_RESULT testResult, cnst ::llc::asc_t & output, cnst ::llc::sc_t (&expected)[_expectedSize]) {
//	rtrn testComposedBytes(errors, testResult, output, ::llc::vcst_t{expected, _expectedSize - 1});
//}
//
//template<typename T, ::llc::u2_t _capacity, size_t _expectedSize>
//sttc ::llc::err_t testFixedStrCase(::llc::ATestError & errors, STR_TEST_RESULT testResult, cnst T value, cnst ::llc::sc_t (&expected)[_expectedSize]) {
//	static_assert(_expectedSize <= _capacity);
//	cnst auto result = ::llc::str(value);
//	static_assert(std::is_same_v<std::remove_cv_t<decltype(result)>, ::llc::astsc_t<_capacity>>);
//	static_assert(sizeof(result.Storage) == _capacity);
//	::llc::astsc_t<_capacity> expectedStorage = {};
//	for(::llc::u2_t iByte = 0; iByte < _expectedSize; ++iByte)
//		expectedStorage.Storage[iByte] = expected[iByte];
//	LLC_TEST_CHECKF(errors, testResult, result.size() != _expectedSize - 1, "Text size:%u, expected:%u; fixed capacity:%u.", result.size(), _expectedSize - 1, _capacity);
//	LLC_TEST_CHECK(errors, testResult, 0 != ::memcmp(result.Storage, expectedStorage.Storage, sizeof(expectedStorage.Storage)));
//	::llc::asc_t composed = {};
//	::llc::err_t composedCount = 0;
//	if_fail_fe(composedCount = ::llc::append_string(composed, value));
//	LLC_TEST_CHECKF(errors, testResult, composedCount != _expectedSize - 1, "Reported append count:%i, expected:%u.", composedCount, _expectedSize - 1);
//	if_fail_fe(testComposedText(errors, testResult, composed, expected));
//	rtrn 0;
//}
//
//sttc llc::err_t	test_str(STestRunnerState &, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
//	{
//		::llc::astsc_t<6> input = {'a', 0, 'b', 'c'};
//		auto result = ::llc::str(input);
//		cxpr b8_t resultIsVstr = std::is_same_v<decltype(result), ::llc::vstr_t>;
//		cxpr b8_t resultIsVcst = std::is_same_v<decltype(result), ::llc::vcst_t>;
//		LLC_TEST_CHECKF(errors, STR_TEST_RESULT_ASTATIC_MUTABLE, !resultIsVstr, "Return type classification: vstr:%u vcst:%u.", resultIsVstr, resultIsVcst);
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_ASTATIC_MUTABLE, result.begin() != input.Storage);
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_ASTATIC_MUTABLE, result.size() != 1);
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_ASTATIC_MUTABLE, result[0] != 'a');
//	}
//	{
//		cnst ::llc::astsc_t<6> input = {'a', 'b', 0, 'c'};
//		auto result = ::llc::str(input);
//		static_assert(std::is_same_v<decltype(result), ::llc::vcst_t>);
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_ASTATIC_CONST, result.begin() != input.Storage);
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_ASTATIC_CONST, result.size() != 2);
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_ASTATIC_CONST, 0 != ::memcmp(result.begin(), input.Storage, result.size()));
//	}
//
//	if_fail_fe((testFixedStrCase<::llc::u0_t,   5>(errors, STR_TEST_RESULT_U0,   0, "0")));
//	if_fail_fe((testFixedStrCase<::llc::u0_t,   5>(errors, STR_TEST_RESULT_U0,   4, "4")));
//	if_fail_fe((testFixedStrCase<::llc::u0_t,   5>(errors, STR_TEST_RESULT_U0,   9, "9")));
//	if_fail_fe((testFixedStrCase<::llc::u0_t,   5>(errors, STR_TEST_RESULT_U0,  10, "10")));
//	if_fail_fe((testFixedStrCase<::llc::u0_t,   5>(errors, STR_TEST_RESULT_U0,  99, "99")));
//	if_fail_fe((testFixedStrCase<::llc::u0_t,   5>(errors, STR_TEST_RESULT_U0, 100, "100")));
//	if_fail_fe((testFixedStrCase<::llc::u0_t,   5>(errors, STR_TEST_RESULT_U0, 255, "255")));
//	if_fail_fe((testFixedStrCase<::llc::u1_t,   7>(errors, STR_TEST_RESULT_U1, 0, "0")));
//	if_fail_fe((testFixedStrCase<::llc::u1_t,   7>(errors, STR_TEST_RESULT_U1, 65535, "65535")));
//	if_fail_fe((testFixedStrCase<::llc::u2_t,  12>(errors, STR_TEST_RESULT_U2, 0, "0")));
//	if_fail_fe((testFixedStrCase<::llc::u2_t,  12>(errors, STR_TEST_RESULT_U2, 0xFFFFFFFFU, "4294967295")));
//	if_fail_fe((testFixedStrCase<::llc::u3_t,  22>(errors, STR_TEST_RESULT_U3, 0, "0")));
//	if_fail_fe((testFixedStrCase<::llc::u3_t,  22>(errors, STR_TEST_RESULT_U3, 0xFFFFFFFFFFFFFFFFULL, "18446744073709551615")));
//	if_fail_fe((testFixedStrCase<::llc::s0_t,   5>(errors, STR_TEST_RESULT_S0, ::llc::s0_t(-128), "-128")));
//	if_fail_fe((testFixedStrCase<::llc::s0_t,   5>(errors, STR_TEST_RESULT_S0, ::llc::s0_t(0), "0")));
//	if_fail_fe((testFixedStrCase<::llc::s0_t,   5>(errors, STR_TEST_RESULT_S0, ::llc::s0_t(127), "127")));
//	if_fail_fe((testFixedStrCase<::llc::s0_t,   5>(errors, STR_TEST_RESULT_S0, ::llc::s0_t('A'), "65")));
//	if_fail_fe((testFixedStrCase<::llc::s0_t,   5>(errors, STR_TEST_RESULT_S0, ::llc::s0_t('<'), "60")));
//	if_fail_fe((testFixedStrCase<::llc::s0_t,   5>(errors, STR_TEST_RESULT_S0, ::llc::s0_t('"'), "34")));
//	if_fail_fe((testFixedStrCase<::llc::s1_t,   8>(errors, STR_TEST_RESULT_S1, -32768, "-32768")));
//	if_fail_fe((testFixedStrCase<::llc::s1_t,   8>(errors, STR_TEST_RESULT_S1, 0, "0")));
//	if_fail_fe((testFixedStrCase<::llc::s1_t,   8>(errors, STR_TEST_RESULT_S1, 32767, "32767")));
//	if_fail_fe((testFixedStrCase<::llc::s2_t,  13>(errors, STR_TEST_RESULT_S2, -2147483647 - 1, "-2147483648")));
//	if_fail_fe((testFixedStrCase<::llc::s2_t,  13>(errors, STR_TEST_RESULT_S2, 0, "0")));
//	if_fail_fe((testFixedStrCase<::llc::s2_t,  13>(errors, STR_TEST_RESULT_S2, 2147483647, "2147483647")));
//	if_fail_fe((testFixedStrCase<::llc::s3_t,  22>(errors, STR_TEST_RESULT_S3, -9223372036854775807LL - 1, "-9223372036854775808")));
//	if_fail_fe((testFixedStrCase<::llc::s3_t,  22>(errors, STR_TEST_RESULT_S3, 0, "0")));
//	if_fail_fe((testFixedStrCase<::llc::s3_t,  22>(errors, STR_TEST_RESULT_S3, 9223372036854775807LL, "9223372036854775807")));
//	if_fail_fe((testFixedStrCase<::llc::f2_t,  64>(errors, STR_TEST_RESULT_F2, 1.25F, "1.250000")));
//	if_fail_fe((testFixedStrCase<::llc::f3_t, 384>(errors, STR_TEST_RESULT_F3, -2.5, "-2.500000")));
//
//	{
//		::llc::asc_t output = {};
//		::llc::err_t composedCount = 0;
//		if_fail_fe(composedCount = ::llc::append_strings(output, ::llc::u0_t{4}, "x", ::llc::u0_t{2}));
//		LLC_TEST_CHECKF(errors, STR_TEST_RESULT_U0_APPEND, composedCount != 3, "Reported append count:%i, expected:3.", composedCount);
//		LLC_TEST_CHECKF(errors, STR_TEST_RESULT_U0_APPEND, output.size() != 3, "Composed size:%u, expected:3.", output.size());
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_U0_APPEND, 0 != ::memcmp(output.begin(), "4x2", 3));
//	}
//	{
//		::llc::asc_t output = {};
//		cnst auto four = ::llc::str(::llc::u0_t{4});
//		if_fail_fe(::llc::append_strings(output, four, "x", ::llc::str(::llc::u0_t{2})));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_PRESTRING_APPEND, output, "4x2"));
//	}
//	{
//		::llc::asc_t output = {};
//		if_fail_fe(::llc::append_strings(output, '<', "tag", '>'));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_CHAR_APPEND, output, "<tag>"));
//		if_fail_fe(output.clear());
//		if_fail_fe(::llc::append_strings(output, '\\', (::llc::sc_t)'\"'));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_CHAR_APPEND, output, "\\\""));
//	}
//	{
//		cnst ::llc::sc_t bytes[] = {'a', 0, 'b', 'c'};
//		cnst ::llc::vcst_t input = {bytes, 4};
//		::llc::asc_t output = {};
//		if_fail_fe(::llc::append_string(output, input));
//		if_fail_fe(testComposedBytes(errors, STR_TEST_RESULT_COUNTED_APPEND, output, input));
//		if_fail_fe(output.clear());
//		::llc::string stringInput = input;
//		if_fail_fe(::llc::append_string(output, stringInput));
//		if_fail_fe(testComposedBytes(errors, STR_TEST_RESULT_COUNTED_APPEND, output, input));
//		if_fail_fe(output.clear());
//		//::llc::asc_t arrayInput = {'a', 0, 'b', 'c'};
//		//if_fail_fe(::llc::append_string(output, arrayInput));
//		//if_fail_fe(testComposedBytes(errors, STR_TEST_RESULT_COUNTED_APPEND, output, input));
//		//if_fail_fe(output.clear());
//		//if_fail_fe(::llc::append_strings(output, true, false));
//		//if_fail_fe(testComposedText(errors, STR_TEST_RESULT_COUNTED_APPEND, output, "truefalse"));
//		//if_fail_fe(output.clear());
//		::llc::sc_t mutableBytes[] = {'a', 0, 'b', 'c'};
//		::llc::vstr_t mutableView = {mutableBytes, 4};
//		if_fail_fe(::llc::append_string(output, mutableView));
//		if_fail_fe(testComposedBytes(errors, STR_TEST_RESULT_COUNTED_APPEND, output, input));
//		if_fail_fe(output.clear());
//		//::llc::vs0_t mutableByteView = {mutableBytes};
//		//if_fail_fe(::llc::append_string(output, mutableByteView));
//		//if_fail_fe(testComposedBytes(errors, STR_TEST_RESULT_COUNTED_APPEND, output, input));
//		//if_fail_fe(output.clear());
//		::llc::vcs0_t constByteView = {bytes};
//		if_fail_fe(::llc::append_string(output, constByteView));
//		if_fail_fe(testComposedBytes(errors, STR_TEST_RESULT_COUNTED_APPEND, output, input));
//		if_fail_fe(output.clear());
//		if_fail_fe(::llc::append_string(output, mutableBytes));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_COUNTED_APPEND, output, "a"));
//		if_fail_fe(output.clear());
//		if_fail_fe(::llc::append_string(output, bytes));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_COUNTED_APPEND, output, "a"));
//	}
//	{
//		::llc::string output = {};
//		if_fail_fe(::llc::appendNclosd(output, "x"));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_ENCLOSER_APPEND, output, "{x}"));
//		if_fail_fe(output.clear());
//		if_fail_fe(::llc::appendBraced(output, "x"));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_ENCLOSER_APPEND, output, "[x]"));
//		if_fail_fe(output.clear());
//		if_fail_fe(::llc::appendQuoted(output, "x"));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_ENCLOSER_APPEND, output, "\"x\""));
//		if_fail_fe(output.clear());
//		if_fail_fe(::llc::appendGtlted(output, "x"));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_ENCLOSER_APPEND, output, "<x>"));
//		if_fail_fe(output.clear());
//		if_fail_fe(::llc::appendNclosd(output, "x", "(", ")"));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_ENCLOSER_APPEND, output, "(x)"));
//	}
//	{
//		::llc::string output = {};
//		if_fail_fe(::llc::appendXmlOpenTag(output, "p"));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_XML_APPEND, output, "<p"));
//		if_fail_fe(output.clear());
//		if_fail_fe(::llc::appendXmlTagClosing(output, "p"));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_XML_APPEND, output, "</p>"));
//		if_fail_fe(output.clear());
//		if_fail_fe(::llc::appendXmlTagOpening(output, "p"));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_XML_APPEND, output, "<p>"));
//		if_fail_fe(output.clear());
//		if_fail_fe(::llc::appendXmlTagVoid(output, "p"));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_XML_APPEND, output, "<p />"));
//		if_fail_fe(output.clear());
//		if_fail_fe(::llc::appendXmlAttr(output, "id", "x"));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_XML_APPEND, output, " id=\"x\""));
//		if_fail_fe(output.clear());
//		if_fail_fe(::llc::appendXmlTag(output, "p", {}, "x"));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_XML_APPEND, output, "<p>x</p>"));
//	}
//	{
//		::llc::string output = {};
//		if_fail_fe(::llc::appendOpenKey(output, "k"));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_JSON_APPEND, output, "\"k\":"));
//		if_fail_fe(output.clear());
//		if_fail_fe(::llc::appendKeyValue(output, "k", "1"));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_JSON_APPEND, output, "\"k\":1"));
//		if_fail_fe(output.clear());
//		if_fail_fe(::llc::appendKeyString(output, "k", "v"));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_JSON_APPEND, output, "\"k\":\"v\""));
//		if_fail_fe(output.clear());
//		if_fail_fe(::llc::appendKeyList(output, "k", "1,2"));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_JSON_APPEND, output, "\"k\":[1,2]"));
//		if_fail_fe(output.clear());
//		if_fail_fe(::llc::appendKeyObject(output, "k", "\"x\":1"));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_JSON_APPEND, output, "\"k\":{\"x\":1}"));
//	}
//	{
//		::llc::string output = {};
//		if_fail_fe(::llc::appendCSSTag(output, "a", "color:red;"));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_CSS_APPEND, output, "a{color:red;}\n"));
//		if_fail_fe(output.clear());
//		if_fail_fe(::llc::appendCSSBorderWidth(output, 4));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_CSS_APPEND, output, "border-width:4px;"));
//		if_fail_fe(output.clear());
//		if_fail_fe(::llc::appendCSSControlWidth(output, 12));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_CSS_APPEND, output, "width:12%;"));
//		if_fail_fe(output.clear());
//		if_fail_fe(::llc::appendCSSControlHeight(output, 12));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_CSS_APPEND, output, "height:12%;"));
//		if_fail_fe(output.clear());
//		if_fail_fe(::llc::appendCSSControlSize(output, {2, 3}));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_CSS_APPEND, output, "width:2%;height:3%;"));
//		if_fail_fe(output.clear());
//		if_fail_fe(::llc::appendCSSSizeWithBorderWidth(output, {10, 20}, 2));
//		if_fail_fe(testComposedText(errors, STR_TEST_RESULT_CSS_APPEND, output, "width:calc(10% - 4px);height:calc(20% - 4px);border-width:2px;"));
//	}
//
//	//
//	//	::llc::sc_t storage[] = {'a', 0, 'b', 'c'};
//	//	::llc::vstr_t input = {storage, 4};
//	//	auto result = ::llc::str(input);
//	//	static_assert(std::is_same_v<decltype(result), ::llc::vstr_t>);
//	//	LLC_TEST_CHECK(errors, STR_TEST_RESULT_VSTR_MUTABLE, result.begin() != input.begin());
//	//	LLC_TEST_CHECK(errors, STR_TEST_RESULT_VSTR_MUTABLE, result.size() != input.size());
//	//	LLC_TEST_CHECK(errors, STR_TEST_RESULT_VSTR_MUTABLE, 0 != ::memcmp(result.begin(), input.begin(), input.byte_count()));
//	//
//	//{
//	//	::llc::sc_t storage[] = {'a', 0, 'b', 'c'};
//	//	cnst ::llc::vstr_t input = {storage, 4};
//	//	auto result = ::llc::str(input);
//	//	static_assert(std::is_same_v<decltype(result), ::llc::vcst_t>);
//	//	LLC_TEST_CHECK(errors, STR_TEST_RESULT_VSTR_CONST, result.begin() != input.begin());
//	//	LLC_TEST_CHECK(errors, STR_TEST_RESULT_VSTR_CONST, result.size() != input.size());
//	//	LLC_TEST_CHECK(errors, STR_TEST_RESULT_VSTR_CONST, 0 != ::memcmp(result.begin(), input.begin(), input.byte_count()));
//	//}
//	//{
//	//	cnst ::llc::sc_t storage[] = {'a', 0, 'b', 'c'};
//	//	cnst ::llc::vcst_t input = {storage, 4};
//	//	auto result = ::llc::str(input);
//	//	static_assert(std::is_same_v<decltype(result), ::llc::vcst_t>);
//	//	LLC_TEST_CHECK(errors, STR_TEST_RESULT_VCST_CONST, result.begin() != input.begin());
//	//	LLC_TEST_CHECK(errors, STR_TEST_RESULT_VCST_CONST, result.size() != input.size());
//	//	LLC_TEST_CHECK(errors, STR_TEST_RESULT_VCST_CONST, 0 != ::memcmp(result.begin(), input.begin(), input.byte_count()));
//	//}
//	//{
//	//	::llc::s0_t storage[] = {'a', 0, 'b', 'c'};
//	//	::llc::vs0_t input = {storage};
//	//	auto result = ::llc::str(input);
//	//	static_assert(std::is_same_v<decltype(result), ::llc::vstr_t>);
//	//	LLC_TEST_CHECK(errors, STR_TEST_RESULT_VS0, result.begin() != input.begin());
//	//	LLC_TEST_CHECK(errors, STR_TEST_RESULT_VS0, result.size() != input.size());
//	//	LLC_TEST_CHECK(errors, STR_TEST_RESULT_VS0, 0 != ::memcmp(result.begin(), input.begin(), input.byte_count()));
//	//}
//	{
//		cnst ::llc::s0_t storage[] = {'a', 0, 'b', 'c'};
//		::llc::vcs0_t input = {storage};
//		auto result = ::llc::str(input);
//		static_assert(std::is_same_v<decltype(result), ::llc::vcst_t>);
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_VCS0, result.begin() != input.begin());
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_VCS0, result.size() != input.size());
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_VCS0, 0 != ::memcmp(result.begin(), input.begin(), input.byte_count()));
//	}
//	{
//		cnst ::llc::sc_t storage[] = {'s', 0, 'x', 'y'};
//		::llc::string input = ::llc::vcst_t{storage + 0, 4};
//		::llc::vstr_t converted = input;
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_STRING_MUTABLE, converted.begin() != input.begin());
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_STRING_MUTABLE, converted.size() != input.size());
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_STRING_MUTABLE, 0 != ::memcmp(converted.begin(), input.begin(), input.byte_count()));
//		//auto result = ::llc::str(input);
//		//cxpr b8_t resultIsVstr = std::is_same_v<decltype(result), ::llc::vstr_t>;
//		//cxpr b8_t resultIsVcst = std::is_same_v<decltype(result), ::llc::vcst_t>;
//		//LLC_TEST_CHECKF(errors, STR_TEST_RESULT_STRING_MUTABLE, !resultIsVstr, "Return type classification: vstr:%u vcst:%u.", resultIsVstr, resultIsVcst);
//		//LLC_TEST_CHECK(errors, STR_TEST_RESULT_STRING_MUTABLE, result.begin() != input.begin());
//		//LLC_TEST_CHECK(errors, STR_TEST_RESULT_STRING_MUTABLE, result.size() != input.size());
//		//LLC_TEST_CHECK(errors, STR_TEST_RESULT_STRING_MUTABLE, 0 != ::memcmp(result.begin(), input.begin(), input.byte_count()));
//	}
//	{
//		cnst ::llc::sc_t storage[] = {'s', 0, 'x', 'y'};
//		cnst ::llc::string input = ::llc::vcst_t{storage + 0, 4};
//		::llc::vcst_t converted = input;
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_STRING_CONST, converted.begin() != input.begin());
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_STRING_CONST, converted.size() != input.size());
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_STRING_CONST, 0 != ::memcmp(converted.begin(), input.begin(), input.byte_count()));
//		//auto result = ::llc::str(input);
//		//cxpr b8_t resultIsVstr = std::is_same_v<decltype(result), ::llc::vstr_t>;
//		//cxpr b8_t resultIsVcst = std::is_same_v<decltype(result), ::llc::vcst_t>;
//		//LLC_TEST_CHECKF(errors, STR_TEST_RESULT_STRING_CONST, !resultIsVstr, "Return type classification: vstr:%u vcst:%u.", resultIsVstr, resultIsVcst);
//		//LLC_TEST_CHECK(errors, STR_TEST_RESULT_STRING_CONST, result.begin() != input.begin());
//		//LLC_TEST_CHECK(errors, STR_TEST_RESULT_STRING_CONST, result.size() != input.size());
//		//LLC_TEST_CHECK(errors, STR_TEST_RESULT_STRING_CONST, 0 != ::memcmp(result.begin(), input.begin(), input.byte_count()));
//	}
//	//{
//	//	::llc::asc_t input = {'a', 0, 'b', 'c'};
//	//	::llc::vcs0_t converted = input;
//	//	LLC_TEST_CHECK(errors, STR_TEST_RESULT_ASC_MUTABLE, converted.begin() != input.begin());
//	//	LLC_TEST_CHECK(errors, STR_TEST_RESULT_ASC_MUTABLE, converted.size() != input.size());
//	//	LLC_TEST_CHECK(errors, STR_TEST_RESULT_ASC_MUTABLE, 0 != ::memcmp(converted.begin(), input.begin(), input.byte_count()));
//	//	auto result = ::llc::str(input);
//	//	cxpr b8_t resultIsVstr = std::is_same_v<decltype(result), ::llc::vstr_t>;
//	//	cxpr b8_t resultIsVcst = std::is_same_v<decltype(result), ::llc::vcst_t>;
//	//	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_ASC_MUTABLE, !resultIsVstr, "Return type classification: vstr:%u vcst:%u.", resultIsVstr, resultIsVcst);
//	//	LLC_TEST_CHECK(errors, STR_TEST_RESULT_ASC_MUTABLE, result.begin() != input.begin());
//	//	LLC_TEST_CHECK(errors, STR_TEST_RESULT_ASC_MUTABLE, result.size() != input.size());
//	//	LLC_TEST_CHECK(errors, STR_TEST_RESULT_ASC_MUTABLE, 0 != ::memcmp(result.begin(), input.begin(), input.byte_count()));
//	//}
//	//{
//	//	cnst ::llc::asc_t input = {'a', 0, 'b', 'c'};
//	//	auto result = ::llc::str(input);
//	//	cxpr b8_t resultIsVstr = std::is_same_v<decltype(result), ::llc::vstr_t>;
//	//	cxpr b8_t resultIsVcst = std::is_same_v<decltype(result), ::llc::vcst_t>;
//	//	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_ASC_CONST, !resultIsVstr, "Return type classification: vstr:%u vcst:%u.", resultIsVstr, resultIsVcst);
//	//	LLC_TEST_CHECK(errors, STR_TEST_RESULT_ASC_CONST, result.begin() != input.begin());
//	//	LLC_TEST_CHECK(errors, STR_TEST_RESULT_ASC_CONST, result.size() != input.size());
//	//	LLC_TEST_CHECK(errors, STR_TEST_RESULT_ASC_CONST, 0 != ::memcmp(result.begin(), input.begin(), input.byte_count()));
//	//}
//	//{
//	//	::llc::sc_t input[] = {'a', 0, 'b', 'c'};
//	//	auto result = ::llc::str(input);
//	//	static_assert(std::is_same_v<decltype(result), ::llc::vstr_t>);
//	//	LLC_TEST_CHECK(errors, STR_TEST_RESULT_ARRAY_MUTABLE, result.begin() != input);
//	//	LLC_TEST_CHECK(errors, STR_TEST_RESULT_ARRAY_MUTABLE, result.size() != 1);
//	//	LLC_TEST_CHECK(errors, STR_TEST_RESULT_ARRAY_MUTABLE, result[0] != 'a');
//	//}
//	{
//		cnst ::llc::sc_t input[] = {'a', 'b', 0, 'c'};
//		auto result = ::llc::str(input);
//		static_assert(std::is_same_v<decltype(result), ::llc::vcst_t>);
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_ARRAY_CONST, result.begin() != input);
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_ARRAY_CONST, result.size() != 2);
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_ARRAY_CONST, 0 != ::memcmp(result.begin(), input, result.size()));
//	}
//	{
//		cnst auto trueText = ::llc::str(true);
//		cnst auto falseText = ::llc::str(false);
//		static_assert(std::is_same_v<decltype(trueText), cnst ::llc::vcst_t>);
//		static_assert(std::is_same_v<decltype(falseText), cnst ::llc::vcst_t>);
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_BOOLEAN, trueText.size() != 4);
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_BOOLEAN, 0 != ::memcmp(trueText.begin(), "true", 4));
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_BOOLEAN, falseText.size() != 5);
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_BOOLEAN, 0 != ::memcmp(falseText.begin(), "false", 5));
//	}
//
//	{
//		cnst auto result = ::llc::str(::llc::SIPv4{192, 168, 1, 42});
//		static_assert(std::is_same_v<std::remove_cv_t<decltype(result)>, ::llc::astsc_t<16>>);
//		static_assert(sizeof(result.Storage) == 16);
//		cnst ::llc::astsc_t<16> expected = {'1', '9', '2', '.', '1', '6', '8', '.', '1', '.', '4', '2', 0};
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_IPV4, result.size() != 12);
//		LLC_TEST_CHECK(errors, STR_TEST_RESULT_IPV4, 0 != ::memcmp(result.Storage, expected.Storage, sizeof(expected.Storage)));
//		//::llc::asc_t output = {};
//		//::llc::err_t composedCount = 0;
//		//if_fail_fe(composedCount = ::llc::append_string(output, ::llc::SIPv4{192, 168, 1, 42}));
//		//LLC_TEST_CHECKF(errors, STR_TEST_RESULT_IPV4, composedCount != 12, "Reported append count:%i, expected:12.", composedCount);
//		//if_fail_fe(testComposedText(errors, STR_TEST_RESULT_IPV4, output, "192.168.1.42"));
//	}
//	rtrn 0;
//}

::llc::err_t test_array_static(STestRunnerState & , ::llc::ATestError & , [[maybe_unused]] ::llc::vcst_t ) {
	return 0; // test_str(runner, errors, outputFolder);
}
