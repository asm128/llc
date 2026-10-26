#include "llc_test_core_suites.h"
#include "llc_string.h"

GDEFINE_ENUM_TYPE(TEST_STRING_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_STRING_RESULT, CONSTRUCT , 0, "A string copies literal contents without counting the literal terminator.");
GDEFINE_ENUM_VALUED(TEST_STRING_RESULT, APPEND    , 1, "String append operations preserve text and the trailing terminator.");
GDEFINE_ENUM_VALUED(TEST_STRING_RESULT, COUNTED   , 2, "A counted string preserves embedded null bytes.");
GDEFINE_ENUM_VALUED(TEST_STRING_RESULT, COPY      , 3, "A copied string owns independent mutable storage.");

sttc ::llc::err_t testStringConstruct(::llc::ATestError & results) {
	::llc::string text = "alpha";
	LLC_TEST_CHECK(results, TEST_STRING_RESULT_CONSTRUCT, text.size() != 5 || text != "alpha" || text.begin()[text.size()] != 0);
	rtrn 0;
}

sttc ::llc::err_t testStringAppend(::llc::ATestError & results) {
	::llc::string text = "alpha";
	LLC_TEST_REQUIRE(results, TEST_STRING_RESULT_APPEND, 1 != text.append_string('-'));
	LLC_TEST_REQUIRE(results, TEST_STRING_RESULT_APPEND, 4 != text.append_string("beta"));
	LLC_TEST_CHECK(results, TEST_STRING_RESULT_APPEND, text.size() != 10 || text != "alpha-beta" || text.begin()[text.size()] != 0);
	rtrn 0;
}

sttc ::llc::err_t testStringCounted(::llc::ATestError & results) {
	cnst char bytes[] = {'A', 0, 'B'};
	::llc::string counted = ::llc::vcst_t{bytes, ::llc::size(bytes)};
	LLC_TEST_CHECK(results, TEST_STRING_RESULT_COUNTED, counted.size() != 3 || counted[0] != 'A' || counted[1] != 0 || counted[2] != 'B' || counted.begin()[counted.size()] != 0);
	rtrn 0;
}

sttc ::llc::err_t testStringCopy(::llc::ATestError & results) {
	::llc::string source = "alpha-beta";
	::llc::string copied = source;
	copied[0] = 'A';
	LLC_TEST_CHECK(results, TEST_STRING_RESULT_COPY, copied != "Alpha-beta" || source != "alpha-beta");
	rtrn 0;
}

::llc::err_t testString(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testStringConstruct(results));
	result = ::llc::min(result, ::testStringAppend(results));
	result = ::llc::min(result, ::testStringCounted(results));
	result = ::llc::min(result, ::testStringCopy(results));
	rtrn result;
}
