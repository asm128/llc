#include "llc_json_core.h"

#include "test_llc_suites.h"

GDEFINE_ENUM_TYPE(JSON_CORE_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(JSON_CORE_TEST_RESULT, OK				, 0, "All JSON core tests passed.");
GDEFINE_ENUM_VALUED(JSON_CORE_TEST_RESULT, TOKEN_TYPE		, 1, "SJSONToken does not preserve its declared fields.");
GDEFINE_ENUM_VALUED(JSON_CORE_TEST_RESULT, TOKEN_NEXT		, 2, "Implicit JSON token traversal returned the wrong sibling.");
GDEFINE_ENUM_VALUED(JSON_CORE_TEST_RESULT, ARRAY_SIZE		, 3, "Implicit JSON array traversal returned the wrong size.");
GDEFINE_ENUM_VALUED(JSON_CORE_TEST_RESULT, OBJECT_KEY_COUNT	, 4, "Implicit JSON object traversal returned the wrong key count.");
GDEFINE_ENUM_VALUED(JSON_CORE_TEST_RESULT, OBJECT_VALUE		, 5, "Implicit JSON object traversal returned the wrong value.");
GDEFINE_ENUM_VALUED(JSON_CORE_TEST_RESULT, ARRAY_VALUE		, 6, "Implicit JSON array traversal returned the wrong value.");
GDEFINE_ENUM_VALUED(JSON_CORE_TEST_RESULT, ARRAY_FOREACH		, 7, "Implicit JSON array enumeration returned the wrong values.");
GDEFINE_ENUM_VALUED(JSON_CORE_TEST_RESULT, OBJECT_FOREACH	, 8, "Implicit JSON object enumeration returned the wrong pairs.");
GDEFINE_ENUM_VALUED(JSON_CORE_TEST_RESULT, COMPARE			, 9, "Implicit JSON comparison returned the wrong result.");

stxp ::llc::sc_t JSON_CORE_SOURCE[] = "{\"values\":[1,2,3],\"flag\":true}";
stxp ::llc::SJSONToken JSON_CORE_TOKENS[] =
	{ {-1, ::llc::JSON_TYPE_OBJECT , { 0, 30}, {}}
	, {-1, ::llc::JSON_TYPE_KEY    , { 1,  9}, {}}
	, {-1, ::llc::JSON_TYPE_STRING , { 2,  8}, {}}
	, {-1, ::llc::JSON_TYPE_VALUE  , {10, 17}, {}}
	, {-1, ::llc::JSON_TYPE_ARRAY  , {10, 17}, {}}
	, {-1, ::llc::JSON_TYPE_VALUE  , {11, 12}, {}}
	, {-1, ::llc::JSON_TYPE_INTEGER, {11, 12}, 1 }
	, {-1, ::llc::JSON_TYPE_VALUE  , {13, 14}, {}}
	, {-1, ::llc::JSON_TYPE_INTEGER, {13, 14}, 2 }
	, {-1, ::llc::JSON_TYPE_VALUE  , {15, 16}, {}}
	, {-1, ::llc::JSON_TYPE_INTEGER, {15, 16}, 3 }
	, {-1, ::llc::JSON_TYPE_KEY    , {18, 24}, {}}
	, {-1, ::llc::JSON_TYPE_STRING , {19, 23}, {}}
	, {-1, ::llc::JSON_TYPE_VALUE  , {25, 29}, {}}
	, {-1, ::llc::JSON_TYPE_BOOLEAN, {25, 29}, 1 }
	};
stxp ::llc::vcst_t JSON_CORE_VIEWS[] =
	{ {}, {}, "values", {}, {}, {}, "1", {}, "2", {}, "3", {}, "flag", {}, "true" };
stxp ::llc::view<cnst ::llc::SJSONToken> JSON_CORE_TOKEN_VIEW = JSON_CORE_TOKENS;
stxp ::llc::view<::llc::vcst_c> JSON_CORE_VIEW_LIST = JSON_CORE_VIEWS;
static_assert(JSON_CORE_TOKEN_VIEW[0].Type == ::llc::JSON_TYPE_OBJECT);
static_assert(::llc::jsonObjectValueGet(JSON_CORE_TOKEN_VIEW, JSON_CORE_VIEW_LIST, 0, "values") == 4);
static_assert(::llc::jsonObjectValueGet(JSON_CORE_TOKEN_VIEW, JSON_CORE_VIEW_LIST, 0, "flag") == 14);
static_assert(::llc::jsonObjectValueGet(JSON_CORE_TOKEN_VIEW, JSON_CORE_VIEW_LIST, 0, "missing") == -1);

::llc::err_t test_json_core(STestRunnerState &, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t) {
	cnst ::llc::SJSONToken token = {-1, ::llc::JSON_TYPE_STRING, {2, 7}, 11};
	LLC_TEST_CHECK(errors, JSON_CORE_TEST_RESULT_TOKEN_TYPE
		, token.ParentIndex != -1 || token.Type != ::llc::JSON_TYPE_STRING || token.Span.Begin != 2 || token.Span.End != 7 || token.Value != 11
		);
	cnst ::llc::view<cnst ::llc::SJSONToken> tokens = JSON_CORE_TOKENS;
	cnst ::llc::vcst_t source = JSON_CORE_SOURCE;
	LLC_TEST_CHECK(errors, JSON_CORE_TEST_RESULT_TOKEN_NEXT, ::llc::jsonTokenNext(tokens, 3) != 11);
	LLC_TEST_CHECK(errors, JSON_CORE_TEST_RESULT_ARRAY_SIZE, ::llc::jsonArraySize(tokens, 4) != 3);
	LLC_TEST_CHECK(errors, JSON_CORE_TEST_RESULT_OBJECT_KEY_COUNT, ::llc::jsonObjectKeyCount(tokens, 0) != 2);
	LLC_TEST_CHECK(errors, JSON_CORE_TEST_RESULT_OBJECT_VALUE, ::llc::jsonObjectValueGet(tokens, source, 0, "values") != 4);
	LLC_TEST_CHECK(errors, JSON_CORE_TEST_RESULT_OBJECT_VALUE, ::llc::jsonObjectValueGet(tokens, source, 0, "flag") != 14);
	LLC_TEST_CHECK(errors, JSON_CORE_TEST_RESULT_OBJECT_VALUE, 0 <= ::llc::jsonObjectValueGet(tokens, source, 0, "missing"));
	LLC_TEST_CHECK(errors, JSON_CORE_TEST_RESULT_ARRAY_VALUE, ::llc::jsonArrayValueGet(tokens, 4, 1) != 8);

	stxp ::llc::u2_t expectedValues[] = {6, 8, 10};
	::llc::u2_t valueCount = 0;
	cnst ::llc::err_t arrayResult = ::llc::jsonArrayForEach(tokens, 4, [&valueCount](::llc::u2_t iValue) -> ::llc::err_t {
		if(valueCount >= ::llc::size(expectedValues) || iValue != expectedValues[valueCount++])
			rtrn -1;
		rtrn 0;
		});
	LLC_TEST_CHECK(errors, JSON_CORE_TEST_RESULT_ARRAY_FOREACH, arrayResult != 3 || valueCount != 3);

	stxp ::llc::u2_t expectedKeys[] = {2, 12};
	stxp ::llc::u2_t expectedObjectValues[] = {4, 14};
	::llc::u2_t pairCount = 0;
	cnst ::llc::err_t objectResult = ::llc::jsonObjectForEach(tokens, 0, [&pairCount](::llc::u2_t iKey, ::llc::u2_t iValue) -> ::llc::err_t {
		if(pairCount >= ::llc::size(expectedKeys) || iKey != expectedKeys[pairCount] || iValue != expectedObjectValues[pairCount])
			rtrn -1;
		++pairCount;
		rtrn 0;
		});
	LLC_TEST_CHECK(errors, JSON_CORE_TEST_RESULT_OBJECT_FOREACH, objectResult != 2 || pairCount != 2);
	LLC_TEST_CHECK(errors, JSON_CORE_TEST_RESULT_COMPARE, 1 != ::llc::jsonCompare(tokens, source, 0, tokens, source, 0));
	LLC_TEST_CHECK(errors, JSON_CORE_TEST_RESULT_COMPARE, 0 != ::llc::jsonCompareNumber(tokens, source, 6, tokens, source, 8));
	rtrn 0;
}
