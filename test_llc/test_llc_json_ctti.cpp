#include "llc_json_ctti.h"

#include "test_llc_suites.h"

GDEFINE_ENUM_TYPE(JSON_CTTI_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(JSON_CTTI_TEST_RESULT, OK				, 0, "All constexpr JSON reader tests passed.");
GDEFINE_ENUM_VALUED(JSON_CTTI_TEST_RESULT, PARSE			, 1, "The constexpr JSON reader rejected a valid document.");
GDEFINE_ENUM_VALUED(JSON_CTTI_TEST_RESULT, TOKEN_COUNT		, 2, "The constexpr JSON reader produced no tokens.");
GDEFINE_ENUM_VALUED(JSON_CTTI_TEST_RESULT, TOKEN_SPAN		, 3, "A constexpr JSON token has an invalid source span.");
GDEFINE_ENUM_VALUED(JSON_CTTI_TEST_RESULT, TOKEN_PARENT	, 4, "A constexpr JSON token has an invalid parent index.");
GDEFINE_ENUM_VALUED(JSON_CTTI_TEST_RESULT, INVALID_INPUT	, 5, "The constexpr JSON reader accepted invalid input.");

stxp ::llc::sc_t JSON_CTTI_DOCUMENT[] = "{\"name\":\"gpk\",\"values\":[1,true,null,\"x\"],\"nested\":{\"enabled\":true}}";
stxp auto JSON_CTTI_READER = ::llc::jsonParseCTTI(JSON_CTTI_DOCUMENT);
stxp auto JSON_CTTI_INVALID = ::llc::jsonParseCTTI("{\"name\":}");
static_assert(JSON_CTTI_READER.Success());
static_assert(JSON_CTTI_READER.Count);
static_assert(false == JSON_CTTI_INVALID.Success());

::llc::err_t test_json_ctti(STestRunnerState &, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t) {
	LLC_TEST_REQUIRE(errors, JSON_CTTI_TEST_RESULT_PARSE, false == JSON_CTTI_READER.Success());
	LLC_TEST_REQUIRE(errors, JSON_CTTI_TEST_RESULT_TOKEN_COUNT, 0 == JSON_CTTI_READER.Count);
	cnst ::llc::view<cnst ::llc::SJSONToken> tokens = JSON_CTTI_READER.Tokens();
	for(::llc::u2_t iToken = 0; iToken < tokens.size(); ++iToken) {
		cnst ::llc::SJSONToken & token = tokens[iToken];
		LLC_TEST_CHECKF(errors, JSON_CTTI_TEST_RESULT_TOKEN_SPAN
			, token.Span.Begin > token.Span.End || token.Span.End > ::llc::size(JSON_CTTI_DOCUMENT) - 1
			, "Token:%u, span:%u..%u.", iToken, token.Span.Begin, token.Span.End
			);
		LLC_TEST_CHECKF(errors, JSON_CTTI_TEST_RESULT_TOKEN_PARENT
			, 0 <= token.ParentIndex && (::llc::u2_t)token.ParentIndex >= iToken
			, "Token:%u, parent:%i.", iToken, token.ParentIndex
			);
	}
	LLC_TEST_CHECK(errors, JSON_CTTI_TEST_RESULT_INVALID_INPUT, JSON_CTTI_INVALID.Success());
	rtrn 0;
}
