#include "llc_base64.h"
#include "test_llc_suites.h"

GDEFINE_ENUM_TYPE(BASE64_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, ENCODE_RESULT	, 0, "base64Encode() rejected valid bytes.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, ENCODED_TEXT	, 1, "base64Encode() produced unexpected text.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, DECODE_RESULT	, 2, "base64Decode() rejected valid Base64 text.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, ROUND_TRIP		, 3, "Base64 decoding did not restore the source bytes.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, TEMPLATE_ENCODE_RESULT	, 4, "base64() rejected valid bytes while encoding.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, TEMPLATE_ENCODED_TEXT	, 5, "base64() produced unexpected text.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, TEMPLATE_DECODE_RESULT	, 6, "base64() rejected valid Base64 text while decoding.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, TEMPLATE_ROUND_TRIP		, 7, "base64() decoding did not restore the source bytes.");

::llc::err_t test_base64(STestRunnerState &, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	cnst ::llc::u0_t source[] = {'h', 'e', 'l', 'l', 'o'};
	cnst ::llc::vcu0_t input = {source};
	::llc::string encoded;
	cnst ::llc::err_t encodeResult = ::llc::base64Encode(input, encoded);
	LLC_TEST_REQUIRE(errors, BASE64_TEST_RESULT_ENCODE_RESULT, ::llc::failed(encodeResult));
	LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_ENCODED_TEXT, encoded != "aGVsbG8=");

	::llc::au0_t decoded;
	cnst ::llc::err_t decodeResult = ::llc::base64Decode(encoded, decoded);
	LLC_TEST_REQUIRE(errors, BASE64_TEST_RESULT_DECODE_RESULT, ::llc::failed(decodeResult));
	LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_ROUND_TRIP, decoded.size() != input.size() || 0 != ::memcmp(decoded.begin(), input.begin(), input.size()));

	cnst ::llc::vcst_t templateInput = "hello";
	::llc::string templateEncoded;
	cnst ::llc::err_t templateEncodeResult = ::llc::base64(templateInput, templateEncoded);
	LLC_TEST_REQUIRE(errors, BASE64_TEST_RESULT_TEMPLATE_ENCODE_RESULT, ::llc::failed(templateEncodeResult));
	LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_TEMPLATE_ENCODED_TEXT, templateEncoded != "aGVsbG8=");

	::llc::string templateDecoded;
	cnst ::llc::err_t templateDecodeResult = ::llc::base64(::llc::vcst_t{templateEncoded}, templateDecoded, true);
	LLC_TEST_REQUIRE(errors, BASE64_TEST_RESULT_TEMPLATE_DECODE_RESULT, ::llc::failed(templateDecodeResult));
	LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_TEMPLATE_ROUND_TRIP, templateDecoded != templateInput);
	rtrn 0;
}
