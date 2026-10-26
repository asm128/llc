#include "llc_test_core_suites.h"
#include "llc_encoding.h"

GDEFINE_ENUM_TYPE(TEST_ENCODING_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_ENCODING_RESULT, SALT_ROUND_TRIP   , 0, "Salting and unsalting preserve every source byte.");
GDEFINE_ENUM_VALUED(TEST_ENCODING_RESULT, HEX_UNSIGNED      , 1, "Hexadecimal encoding and unsigned decoding preserve bytes and append to existing output.");
GDEFINE_ENUM_VALUED(TEST_ENCODING_RESULT, HEX_SIGNED        , 2, "Signed hexadecimal decoding preserves all byte bit patterns.");
GDEFINE_ENUM_VALUED(TEST_ENCODING_RESULT, HEX_INVALID       , 3, "Hexadecimal decoding reports invalid digits instead of accepting partial values.");
GDEFINE_ENUM_VALUED(TEST_ENCODING_RESULT, ARDELL_ROUND_TRIP , 4, "Ardell encoding and decoding preserve the unsalted input.");
GDEFINE_ENUM_VALUED(TEST_ENCODING_RESULT, UTF8              , 5, "UTF-8 conversion appends the complete encoding for one-, two-, three-, and four-byte code points.");
GDEFINE_ENUM_VALUED(TEST_ENCODING_RESULT, DIGEST_BYTES      , 6, "Binary digest generation is deterministic and produces data.");
GDEFINE_ENUM_VALUED(TEST_ENCODING_RESULT, DIGEST_TEXT       , 7, "Text digest generation is deterministic and produces text.");

sttc bool encodingEqual(::llc::vcu0_t left, ::llc::vcu0_t right) {
	rtrn left.size() == right.size() && (0 == left.size() || 0 == memcmp(left.begin(), right.begin(), left.size()));
}

sttc ::llc::vcu0_t encodingInput() {
	sttc const ::llc::u0_t bytes[] = {0, 1, 0x7F, 0x80, 0xA5, 0xFF};
	rtrn bytes;
}

sttc ::llc::err_t testEncodingSaltRoundTrip(::llc::ATestError & results) {
	::llc::au0_t salted, unsalted;
	LLC_TEST_REQUIRE(results, TEST_ENCODING_RESULT_SALT_ROUND_TRIP, 0 != ::llc::saltDataSalt(::encodingInput(), salted));
	LLC_TEST_REQUIRE(results, TEST_ENCODING_RESULT_SALT_ROUND_TRIP, salted.size() != ::encodingInput().size() * 2);
	LLC_TEST_REQUIRE(results, TEST_ENCODING_RESULT_SALT_ROUND_TRIP, 0 != ::llc::saltDataUnsalt(salted, unsalted));
	LLC_TEST_CHECK(results, TEST_ENCODING_RESULT_SALT_ROUND_TRIP, false == ::encodingEqual(::encodingInput(), unsalted));
	rtrn 0;
}

sttc ::llc::err_t testEncodingHexUnsigned(::llc::ATestError & results) {
	::llc::string encoded = "prefix:";
	LLC_TEST_REQUIRE(results, TEST_ENCODING_RESULT_HEX_UNSIGNED, 0 != ::llc::hexEncode(::encodingInput(), encoded));
	LLC_TEST_REQUIRE(results, TEST_ENCODING_RESULT_HEX_UNSIGNED, encoded != "prefix:00017F80A5FF");
	::llc::au0_t decoded = {9};
	LLC_TEST_REQUIRE(results, TEST_ENCODING_RESULT_HEX_UNSIGNED, 0 != ::llc::hexDecode({encoded.begin() + 7, encoded.size() - 7}, decoded));
	LLC_TEST_CHECK(results, TEST_ENCODING_RESULT_HEX_UNSIGNED, decoded.size() != 7 || decoded[0] != 9 || false == ::encodingEqual(::encodingInput(), {decoded.begin() + 1, decoded.size() - 1}));
	rtrn 0;
}

sttc ::llc::err_t testEncodingHexSigned(::llc::ATestError & results) {
	::llc::as0_t decoded;
	LLC_TEST_REQUIRE(results, TEST_ENCODING_RESULT_HEX_SIGNED, 0 != ::llc::hexDecode("007F80FF", decoded));
	LLC_TEST_CHECK(results, TEST_ENCODING_RESULT_HEX_SIGNED, decoded.size() != 4 || (::llc::u0_t)decoded[0] != 0 || (::llc::u0_t)decoded[1] != 0x7F || (::llc::u0_t)decoded[2] != 0x80 || (::llc::u0_t)decoded[3] != 0xFF);
	rtrn 0;
}

sttc ::llc::err_t testEncodingHexInvalid(::llc::ATestError & results) {
	::llc::au0_t decoded;
	LLC_TEST_CHECK(results, TEST_ENCODING_RESULT_HEX_INVALID, 0 <= ::llc::hexDecode("G0", decoded));
	decoded.clear();
	LLC_TEST_CHECK(results, TEST_ENCODING_RESULT_HEX_INVALID, 0 <= ::llc::hexDecode("0G", decoded));
	rtrn 0;
}

sttc ::llc::err_t testEncodingArdellRoundTrip(::llc::ATestError & results) {
	::llc::au0_t encoded, decoded;
	LLC_TEST_REQUIRE(results, TEST_ENCODING_RESULT_ARDELL_ROUND_TRIP, 0 != ::llc::ardellEncode(::encodingInput(), 0x12345678, false, encoded));
	LLC_TEST_REQUIRE(results, TEST_ENCODING_RESULT_ARDELL_ROUND_TRIP, 0 != ::llc::ardellDecode(encoded, 0x12345678, false, decoded));
	LLC_TEST_CHECK(results, TEST_ENCODING_RESULT_ARDELL_ROUND_TRIP, false == ::encodingEqual(::encodingInput(), decoded));
	rtrn 0;
}

sttc ::llc::err_t testEncodingUTF8(::llc::ATestError & results) {
	::llc::string encoded;
	LLC_TEST_REQUIRE(results, TEST_ENCODING_RESULT_UTF8, 0 != ::llc::utf8FromCodePoint(0x24, encoded));
	LLC_TEST_REQUIRE(results, TEST_ENCODING_RESULT_UTF8, 0 != ::llc::utf8FromCodePoint(0xA2, encoded));
	LLC_TEST_REQUIRE(results, TEST_ENCODING_RESULT_UTF8, 0 != ::llc::utf8FromCodePoint(0x20AC, encoded));
	LLC_TEST_REQUIRE(results, TEST_ENCODING_RESULT_UTF8, 0 != ::llc::utf8FromCodePoint(0x10348, encoded));
	sttc const ::llc::u0_t expected[] = {0x24, 0xC2, 0xA2, 0xE2, 0x82, 0xAC, 0xF0, 0x90, 0x8D, 0x88};
	LLC_TEST_CHECK(results, TEST_ENCODING_RESULT_UTF8, encoded.size() != szof(expected) || 0 != memcmp(encoded.begin(), expected, szof(expected)));
	rtrn 0;
}

sttc ::llc::err_t testEncodingDigestBytes(::llc::ATestError & results) {
	::llc::u0_t source[64] = {};
	for(::llc::u2_t i = 0; i < szof(source); ++i)
		source[i] = (::llc::u0_t)i;
	::llc::au2_t first, second;
	LLC_TEST_REQUIRE(results, TEST_ENCODING_RESULT_DIGEST_BYTES, 0 != ::llc::digest(source, first));
	LLC_TEST_REQUIRE(results, TEST_ENCODING_RESULT_DIGEST_BYTES, 0 != ::llc::digest(source, second));
	LLC_TEST_CHECK(results, TEST_ENCODING_RESULT_DIGEST_BYTES, 0 == first.size() || first.size() != second.size() || 0 != memcmp(first.begin(), second.begin(), first.size() * szof(::llc::u2_t)));
	rtrn 0;
}

sttc ::llc::err_t testEncodingDigestText(::llc::ATestError & results) {
	::llc::u0_t source[64] = {};
	for(::llc::u2_t i = 0; i < szof(source); ++i)
		source[i] = (::llc::u0_t)(63 - i);
	::llc::string first, second;
	LLC_TEST_REQUIRE(results, TEST_ENCODING_RESULT_DIGEST_TEXT, 0 != ::llc::digest(source, first));
	LLC_TEST_REQUIRE(results, TEST_ENCODING_RESULT_DIGEST_TEXT, 0 != ::llc::digest(source, second));
	LLC_TEST_CHECK(results, TEST_ENCODING_RESULT_DIGEST_TEXT, 0 == first.size() || first != second);
	rtrn 0;
}

::llc::err_t testEncoding(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testEncodingSaltRoundTrip(results));
	result = ::llc::min(result, ::testEncodingHexUnsigned(results));
	result = ::llc::min(result, ::testEncodingHexSigned(results));
	result = ::llc::min(result, ::testEncodingHexInvalid(results));
	result = ::llc::min(result, ::testEncodingArdellRoundTrip(results));
	result = ::llc::min(result, ::testEncodingUTF8(results));
	result = ::llc::min(result, ::testEncodingDigestBytes(results));
	result = ::llc::min(result, ::testEncodingDigestText(results));
	rtrn result;
}
