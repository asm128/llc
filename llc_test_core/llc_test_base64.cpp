#include "llc_base64.h"
#include "llc_test_core.h"

#include <cstring>

GDEFINE_ENUM_TYPE(BASE64_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, ENCODE_RESULT	, 0, "base64Encode() rejected valid bytes.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, ENCODED_TEXT	, 1, "base64Encode() produced unexpected text.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, DECODE_RESULT	, 2, "base64Decode() rejected valid Base64 text.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, ROUND_TRIP		, 3, "Base64 decoding did not restore the source bytes.");

::llc::err_t testBase64(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
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
	rtrn 0;
}
