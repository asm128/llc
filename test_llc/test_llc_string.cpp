#include "llc_string_compose.h"
#include "test_llc_suites.h"

GDEFINE_ENUM_TYPE(STRING_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(STRING_TEST_RESULT, EMPTY				,  0, "A default string is not empty.");
GDEFINE_ENUM_VALUED(STRING_TEST_RESULT, LITERAL_SIZE		,  1, "Literal construction produced the wrong size.");
GDEFINE_ENUM_VALUED(STRING_TEST_RESULT, LITERAL_CONTENT		,  2, "Literal construction produced the wrong content.");
GDEFINE_ENUM_VALUED(STRING_TEST_RESULT, TERMINATOR			,  3, "The string terminator was not preserved.");
GDEFINE_ENUM_VALUED(STRING_TEST_RESULT, VIEW_CONVERSIONS		,  4, "String view conversions changed the pointer or count.");
GDEFINE_ENUM_VALUED(STRING_TEST_RESULT, POINTER_CONVERSIONS	,  5, "String pointer conversions changed the pointer.");
GDEFINE_ENUM_VALUED(STRING_TEST_RESULT, COUNTED_VIEW			,  6, "Counted-view construction produced the wrong string.");
GDEFINE_ENUM_VALUED(STRING_TEST_RESULT, COPY_INDEPENDENCE		,  7, "A copied string shares mutable storage with its source.");
GDEFINE_ENUM_VALUED(STRING_TEST_RESULT, COMPARISON			,  8, "String comparison disagrees with its literal or view.");
GDEFINE_ENUM_VALUED(STRING_TEST_RESULT, CAMEL_CASE_RESULT		,  9, "camelCase() rejected valid input.");
GDEFINE_ENUM_VALUED(STRING_TEST_RESULT, CAMEL_CASE_TEXT		, 10, "camelCase() produced unexpected text.");
GDEFINE_ENUM_VALUED(STRING_TEST_RESULT, CAMEL_CASE_APPEND_RESULT, 11, "camelCase() rejected valid appended input.");
GDEFINE_ENUM_VALUED(STRING_TEST_RESULT, CAMEL_CASE_APPEND_TEXT	, 12, "camelCase() did not append the converted text.");
GDEFINE_ENUM_VALUED(STRING_TEST_RESULT, STR_U0				, 13, "str(u0_t) does not expose exactly its formatted digits.");
GDEFINE_ENUM_VALUED(STRING_TEST_RESULT, STR_U0_APPEND_RESULT	, 14, "append_strings() rejected a valid u0_t.");
GDEFINE_ENUM_VALUED(STRING_TEST_RESULT, STR_U0_APPEND_TEXT	, 15, "append_strings() appended the wrong u0_t text.");

::llc::err_t test_string(STestRunnerState &, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	::llc::string empty;
	LLC_TEST_CHECK(errors, STRING_TEST_RESULT_EMPTY, 0 != empty.size() || empty.begin() != empty.end());

	::llc::string literal = "alpha";
	LLC_TEST_CHECK(errors, STRING_TEST_RESULT_LITERAL_SIZE, 5 != literal.size());
	LLC_TEST_CHECK(errors, STRING_TEST_RESULT_LITERAL_CONTENT, literal != "alpha");
	LLC_TEST_CHECK(errors, STRING_TEST_RESULT_TERMINATOR, 0 != literal.begin()[literal.size()]);

	cnst ::llc::string & constLiteral = literal;
	cnst ::llc::vcst_t constView = constLiteral;
	::llc::vstr_t mutableView = literal;
	LLC_TEST_CHECK(errors, STRING_TEST_RESULT_VIEW_CONVERSIONS
		, constView.begin() != literal.begin() || constView.size() != literal.size()
		|| mutableView.begin() != literal.begin() || mutableView.size() != literal.size()
		);
	cnst ::llc::sc_c * constPointer = constLiteral.begin();
	::llc::sc_t * mutablePointer = literal.begin();
	LLC_TEST_CHECK(errors, STRING_TEST_RESULT_POINTER_CONVERSIONS, constPointer != literal.begin() || mutablePointer != literal.begin());

	cnst ::llc::vcst_t countedSource = {"bravo-tail", 5};
	::llc::string counted = countedSource;
	LLC_TEST_CHECK(errors, STRING_TEST_RESULT_COUNTED_VIEW, counted != "bravo" || 5 != counted.size());

	::llc::string copy = literal;
	copy.begin()[0] = 'A';
	LLC_TEST_CHECK(errors, STRING_TEST_RESULT_COPY_INDEPENDENCE, copy != "Alpha" || literal != "alpha" || copy.begin() == literal.begin());
	LLC_TEST_CHECK(errors, STRING_TEST_RESULT_COMPARISON, false == (literal == ::llc::vcst_t{"alpha"}) || false == (literal != "beta"));

	::llc::asc_t camelCased;
	cnst ::llc::err_t camelResult = ::llc::camelCase("hello_world-test", camelCased);
	LLC_TEST_REQUIRE(errors, STRING_TEST_RESULT_CAMEL_CASE_RESULT, ::llc::failed(camelResult));
	LLC_TEST_CHECK(errors, STRING_TEST_RESULT_CAMEL_CASE_TEXT, ::llc::vcst_t{camelCased} != "HelloWorldTest");

	::llc::asc_t appended = ::llc::vcst_t{"Prefix"};
	cnst ::llc::err_t appendResult = ::llc::camelCase("next_value", appended);
	LLC_TEST_REQUIRE(errors, STRING_TEST_RESULT_CAMEL_CASE_APPEND_RESULT, ::llc::failed(appendResult));
	LLC_TEST_CHECK(errors, STRING_TEST_RESULT_CAMEL_CASE_APPEND_TEXT, ::llc::vcst_t{appended} != "PrefixNextValue");

	for(::llc::u2_t iValue = 0; iValue <= 0xFF; ++iValue) {
		::llc::sc_t expected[5] = {};
		cnst ::llc::err_t expectedLength = ::llc::sprintf_s(expected, "%u", iValue);
		cnst auto actualStorage = ::llc::str((::llc::u0_t)iValue);
		cnst ::llc::vcst_t actual = actualStorage;
		LLC_TEST_CHECKF(errors, STRING_TEST_RESULT_STR_U0
			, ::llc::failed(expectedLength)
			|| actualStorage.Count != (::llc::u2_t)expectedLength
			|| actualStorage.size() != (::llc::u2_t)expectedLength
			|| actual.size() != (::llc::u2_t)expectedLength
			|| 0 != ::memcmp(actual.begin(), expected, expectedLength)
			|| 0 != actualStorage.Storage[expectedLength]
			, "value:%u count:%u actual-size:%u expected-size:%i"
			, iValue, actualStorage.Count, actual.size(), expectedLength
			);

		::llc::asc_t composed;
		cnst ::llc::err_t composeResult = ::llc::append_strings(composed, ::llc::str((::llc::u0_t)iValue), "x");
		LLC_TEST_CHECKF(errors, STRING_TEST_RESULT_STR_U0_APPEND_RESULT, ::llc::failed(composeResult), "value:%u result:%i", iValue, composeResult);
		expected[expectedLength] = 'x';
		expected[expectedLength + 1] = 0;
		LLC_TEST_CHECKF(errors, STRING_TEST_RESULT_STR_U0_APPEND_TEXT
			, composed.size() != (::llc::u2_t)expectedLength + 1
			|| 0 != ::memcmp(composed.begin(), expected, expectedLength + 1)
			, "value:%u actual-size:%u expected-size:%i"
			, iValue, composed.size(), expectedLength + 1
			);
	}
	rtrn 0;
}
