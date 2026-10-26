#include "llc_test_core_suites.h"
#include "llc_aes.h"

GDEFINE_ENUM_TYPE(TEST_AES_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_AES_RESULT, ECB_128         , 0, "AES-128 ECB matches the NIST SP 800-38A block vector and decrypts it.");
GDEFINE_ENUM_VALUED(TEST_AES_RESULT, ECB_192         , 1, "AES-192 ECB matches the NIST SP 800-38A block vector and decrypts it.");
GDEFINE_ENUM_VALUED(TEST_AES_RESULT, ECB_256         , 2, "AES-256 ECB matches the NIST SP 800-38A block vector and decrypts it.");
GDEFINE_ENUM_VALUED(TEST_AES_RESULT, CBC_LEVELS      , 3, "High-level CBC encoding round-trips at every supported AES level.");
GDEFINE_ENUM_VALUED(TEST_AES_RESULT, APPENDED_IV     , 4, "The appended-IV overload round-trips without changing the message.");
GDEFINE_ENUM_VALUED(TEST_AES_RESULT, SHORT_IV_KEY    , 5, "High-level AES rejects IV and key views shorter than their required sizes.");
GDEFINE_ENUM_VALUED(TEST_AES_RESULT, CIPHERTEXT_SIZE , 6, "AES decoding rejects empty and non-block-aligned ciphertext.");
GDEFINE_ENUM_VALUED(TEST_AES_RESULT, LEGAL_PADDING   , 7, "AES round-trips every legal PKCS#7 padding width from one through sixteen.");
GDEFINE_ENUM_VALUED(TEST_AES_RESULT, CORRUPT_PADDING , 8, "AES decoding rejects zero and inconsistent PKCS#7 padding.");
GDEFINE_ENUM_VALUED(TEST_AES_RESULT, LEVEL_PROPERTIES, 9, "The AES header exposes the key and expanded-key sizes for all supported levels.");
GDEFINE_ENUM_VALUED(TEST_AES_RESULT, POINTER_OVERLOADS, 10, "Pointer-and-length AES overloads round-trip with explicit and appended IVs.");
GDEFINE_ENUM_VALUED(TEST_AES_RESULT, STRING_OVERLOADS , 11, "Signed-byte view AES overloads round-trip into signed-byte output.");

sttc ::llc::err_t aesVector
	( ::llc::ATestError & results
	, TEST_AES_RESULT resultIndex
	, ::llc::AES_LEVEL level
	, ::llc::vcu0_t key
	, ::llc::vcu0_t expectedCipher
	) {
	sttc cnst ::llc::u0_t plain[] = {0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A};
	::llc::u0_t block[szof(plain)] = {};
	memcpy(block, plain, szof(block));
	::llc::SAESContext context;
	::llc::aesInitCtx(&context, key.begin(), level);
	::llc::aesECBEncrypt(&context, block);
	LLC_TEST_REQUIRE(results, resultIndex, 0 != memcmp(block, expectedCipher.begin(), szof(block)));
	::llc::aesECBDecrypt(&context, block);
	LLC_TEST_CHECK(results, resultIndex, 0 != memcmp(block, plain, szof(block)));
	rtrn 0;
}

sttc ::llc::err_t testAESECB128(::llc::ATestError & results) {
	sttc cnst ::llc::u0_t key[] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};
	sttc cnst ::llc::u0_t cipher[] = {0x3A, 0xD7, 0x7B, 0xB4, 0x0D, 0x7A, 0x36, 0x60, 0xA8, 0x9E, 0xCA, 0xF3, 0x24, 0x66, 0xEF, 0x97};
	rtrn ::aesVector(results, TEST_AES_RESULT_ECB_128, ::llc::AES_LEVEL_128, key, cipher);
}

sttc ::llc::err_t testAESECB192(::llc::ATestError & results) {
	sttc cnst ::llc::u0_t key[] = {0x8E, 0x73, 0xB0, 0xF7, 0xDA, 0x0E, 0x64, 0x52, 0xC8, 0x10, 0xF3, 0x2B, 0x80, 0x90, 0x79, 0xE5, 0x62, 0xF8, 0xEA, 0xD2, 0x52, 0x2C, 0x6B, 0x7B};
	sttc cnst ::llc::u0_t cipher[] = {0xBD, 0x33, 0x4F, 0x1D, 0x6E, 0x45, 0xF2, 0x5F, 0xF7, 0x12, 0xA2, 0x14, 0x57, 0x1F, 0xA5, 0xCC};
	rtrn ::aesVector(results, TEST_AES_RESULT_ECB_192, ::llc::AES_LEVEL_192, key, cipher);
}

sttc ::llc::err_t testAESECB256(::llc::ATestError & results) {
	sttc cnst ::llc::u0_t key[] = {0x60, 0x3D, 0xEB, 0x10, 0x15, 0xCA, 0x71, 0xBE, 0x2B, 0x73, 0xAE, 0xF0, 0x85, 0x7D, 0x77, 0x81, 0x1F, 0x35, 0x2C, 0x07, 0x3B, 0x61, 0x08, 0xD7, 0x2D, 0x98, 0x10, 0xA3, 0x09, 0x14, 0xDF, 0xF4};
	sttc cnst ::llc::u0_t cipher[] = {0xF3, 0xEE, 0xD1, 0xBD, 0xB5, 0xD2, 0xA0, 0x3C, 0x06, 0x4B, 0x5A, 0x7E, 0x3D, 0xB1, 0x81, 0xF8};
	rtrn ::aesVector(results, TEST_AES_RESULT_ECB_256, ::llc::AES_LEVEL_256, key, cipher);
}

sttc ::llc::err_t aesRoundTrip(::llc::AES_LEVEL level, ::llc::vcu0_t message, ::llc::vcu0_t iv, ::llc::vcu0_t key) {
	::llc::au0_t encrypted, decrypted;
	if_fail_fe(::llc::aesEncode(message, iv, key, level, encrypted));
	if_fail_fe(::llc::aesDecode(encrypted, iv, key, level, decrypted));
	if_true_fe(decrypted.size() != message.size());
	if_true_fe(0 != memcmp(decrypted.begin(), message.begin(), message.size()));
	rtrn 0;
}

sttc ::llc::err_t testAESCBCLevels(::llc::ATestError & results) {
	sttc cnst ::llc::u0_t message[] = {'L', 'L', 'C', ' ', 'A', 'E', 'S', ' ', 'r', 'o', 'u', 'n', 'd', 't', 'r', 'i', 'p'};
	sttc cnst ::llc::u0_t iv[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	sttc cnst ::llc::u0_t key[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
	LLC_TEST_CHECK(results, TEST_AES_RESULT_CBC_LEVELS, 0 != ::aesRoundTrip(::llc::AES_LEVEL_128, message, iv, key));
	LLC_TEST_CHECK(results, TEST_AES_RESULT_CBC_LEVELS, 0 != ::aesRoundTrip(::llc::AES_LEVEL_192, message, iv, key));
	LLC_TEST_CHECK(results, TEST_AES_RESULT_CBC_LEVELS, 0 != ::aesRoundTrip(::llc::AES_LEVEL_256, message, iv, key));
	rtrn 0;
}

sttc ::llc::err_t testAESAppendedIV(::llc::ATestError & results) {
	sttc cnst ::llc::u0_t message[] = {'a', 'p', 'p', 'e', 'n', 'd', 'e', 'd', '-', 'i', 'v'};
	sttc cnst ::llc::u0_t key[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
	::llc::au0_t encrypted, decrypted;
	LLC_TEST_REQUIRE(results, TEST_AES_RESULT_APPENDED_IV, 0 != ::llc::aesEncode(::llc::vcu0_t{message}, ::llc::vcu0_t{key}, ::llc::AES_LEVEL_256, encrypted));
	LLC_TEST_REQUIRE(results, TEST_AES_RESULT_APPENDED_IV, 0 != ::llc::aesDecode(encrypted, ::llc::vcu0_t{key}, ::llc::AES_LEVEL_256, decrypted));
	LLC_TEST_CHECK(results, TEST_AES_RESULT_APPENDED_IV, decrypted.size() != szof(message) || 0 != memcmp(decrypted.begin(), message, szof(message)));
	rtrn 0;
}

sttc ::llc::err_t testAESShortIVKey(::llc::ATestError & results) {
	sttc cnst ::llc::u0_t message[] = {1, 2, 3};
	::llc::u0_t ivStorage[16] = {};
	::llc::u0_t keyStorage[32] = {};
	cnst ::llc::vcu0_t shortIV = {ivStorage, 15};
	cnst ::llc::vcu0_t fullIV = ivStorage;
	cnst ::llc::vcu0_t shortKey = {keyStorage, 31};
	cnst ::llc::vcu0_t fullKey = keyStorage;
	::llc::au0_t output;
	LLC_TEST_CHECK(results, TEST_AES_RESULT_SHORT_IV_KEY, 0 <= ::llc::aesEncode(::llc::vcu0_t{message}, shortIV, fullKey, ::llc::AES_LEVEL_256, output));
	LLC_TEST_CHECK(results, TEST_AES_RESULT_SHORT_IV_KEY, 0 <= ::llc::aesEncode(::llc::vcu0_t{message}, fullIV, shortKey, ::llc::AES_LEVEL_256, output));
	rtrn 0;
}

sttc ::llc::err_t testAESCiphertextSize(::llc::ATestError & results) {
	::llc::u0_t ivStorage[16] = {};
	::llc::u0_t keyStorage[32] = {};
	::llc::u0_t cipherStorage[15] = {};
	cnst ::llc::vcu0_t iv = ivStorage;
	cnst ::llc::vcu0_t key = keyStorage;
	::llc::au0_t output;
	LLC_TEST_CHECK(results, TEST_AES_RESULT_CIPHERTEXT_SIZE, 0 <= ::llc::aesDecode(::llc::vcu0_t{}, iv, key, ::llc::AES_LEVEL_256, output));
	LLC_TEST_CHECK(results, TEST_AES_RESULT_CIPHERTEXT_SIZE, 0 <= ::llc::aesDecode(::llc::vcu0_t{cipherStorage}, iv, key, ::llc::AES_LEVEL_256, output));
	rtrn 0;
}

sttc ::llc::err_t testAESLegalPadding(::llc::ATestError & results) {
	::llc::u0_t ivStorage[16] = {};
	::llc::u0_t keyStorage[32] = {};
	::llc::u0_t messageStorage[16] = {};
	for(::llc::u2_t i = 0; i < 16; ++i)
		messageStorage[i] = (::llc::u0_t)(i + 1);
	cnst ::llc::vcu0_t iv = ivStorage;
	cnst ::llc::vcu0_t key = keyStorage;
	for(::llc::u2_t messageLength = 1; messageLength <= 16; ++messageLength)
		LLC_TEST_CHECKF(results, TEST_AES_RESULT_LEGAL_PADDING, 0 != ::aesRoundTrip(::llc::AES_LEVEL_256, {messageStorage, messageLength}, iv, key), "Roundtrip failed for message length:%u.", messageLength);
	rtrn 0;
}

sttc ::llc::err_t aesEncryptInvalidPadding(::llc::u0_t secondLast, ::llc::u0_t last, ::llc::au0_t & encrypted) {
	::llc::u0_t ivStorage[16] = {};
	::llc::u0_t keyStorage[32] = {};
	::llc::u0_t block[16] = {};
	block[14] = secondLast;
	block[15] = last;
	::llc::SAESContext context;
	::llc::aesInitCtxIV(&context, keyStorage, ::llc::AES_LEVEL_256, ivStorage);
	::llc::aesCBCEncryptBuffer(&context, block, szof(block));
	rtrn encrypted.append(block);
}

sttc ::llc::err_t testAESCorruptPadding(::llc::ATestError & results) {
	::llc::u0_t ivStorage[16] = {};
	::llc::u0_t keyStorage[32] = {};
	cnst ::llc::vcu0_t iv = ivStorage;
	cnst ::llc::vcu0_t key = keyStorage;
	::llc::au0_t encrypted, output;
	LLC_TEST_REQUIRE(results, TEST_AES_RESULT_CORRUPT_PADDING, 0 != ::aesEncryptInvalidPadding(0, 0, encrypted));
	LLC_TEST_CHECK(results, TEST_AES_RESULT_CORRUPT_PADDING, 0 <= ::llc::aesDecode(encrypted, iv, key, ::llc::AES_LEVEL_256, output));
	encrypted.clear();
	LLC_TEST_REQUIRE(results, TEST_AES_RESULT_CORRUPT_PADDING, 0 != ::aesEncryptInvalidPadding(3, 2, encrypted));
	LLC_TEST_CHECK(results, TEST_AES_RESULT_CORRUPT_PADDING, 0 <= ::llc::aesDecode(encrypted, iv, key, ::llc::AES_LEVEL_256, output));
	rtrn 0;
}

sttc ::llc::err_t testAESLevelProperties(::llc::ATestError & results) {
	LLC_TEST_CHECK(results, TEST_AES_RESULT_LEVEL_PROPERTIES
		, ::llc::AES_SIZEBLOCK != 16 || ::llc::AES_SIZEIV != 16
		|| ::llc::AES_LEVEL_PROPERTIES[::llc::AES_LEVEL_128].KeyLength != 16
		|| ::llc::AES_LEVEL_PROPERTIES[::llc::AES_LEVEL_128].KeyExpSize != 176
		|| ::llc::AES_LEVEL_PROPERTIES[::llc::AES_LEVEL_192].KeyLength != 24
		|| ::llc::AES_LEVEL_PROPERTIES[::llc::AES_LEVEL_192].KeyExpSize != 208
		|| ::llc::AES_LEVEL_PROPERTIES[::llc::AES_LEVEL_256].KeyLength != 32
		|| ::llc::AES_LEVEL_PROPERTIES[::llc::AES_LEVEL_256].KeyExpSize != 240
		);
	rtrn 0;
}

sttc ::llc::err_t testAESPointerOverloads(::llc::ATestError & results) {
	sttc cnst ::llc::u0_t message[] = {5, 10, 15, 20, 25};
	::llc::u0_t ivStorage[16] = {};
	::llc::u0_t keyStorage[32] = {};
	cnst ::llc::vcu0_t iv = ivStorage;
	cnst ::llc::vcu0_t key = keyStorage;
	::llc::au0_t encrypted, decrypted;
	LLC_TEST_REQUIRE(results, TEST_AES_RESULT_POINTER_OVERLOADS, 0 != ::llc::aesEncode(message, szof(message), iv, key, ::llc::AES_LEVEL_256, encrypted));
	LLC_TEST_REQUIRE(results, TEST_AES_RESULT_POINTER_OVERLOADS, 0 != ::llc::aesDecode(encrypted.begin(), encrypted.size(), iv, key, ::llc::AES_LEVEL_256, decrypted));
	LLC_TEST_CHECK(results, TEST_AES_RESULT_POINTER_OVERLOADS, decrypted.size() != szof(message) || 0 != memcmp(decrypted.begin(), message, szof(message)));
	encrypted.clear();
	decrypted.clear();
	LLC_TEST_REQUIRE(results, TEST_AES_RESULT_POINTER_OVERLOADS, 0 != ::llc::aesEncode(message, szof(message), key, ::llc::AES_LEVEL_256, encrypted));
	LLC_TEST_REQUIRE(results, TEST_AES_RESULT_POINTER_OVERLOADS, 0 != ::llc::aesDecode(encrypted.begin(), encrypted.size(), key, ::llc::AES_LEVEL_256, decrypted));
	LLC_TEST_CHECK(results, TEST_AES_RESULT_POINTER_OVERLOADS, decrypted.size() != szof(message) || 0 != memcmp(decrypted.begin(), message, szof(message)));
	rtrn 0;
}

sttc ::llc::err_t testAESStringOverloads(::llc::ATestError & results) {
	sttc cnst ::llc::s0_t messageStorage[] = {'s', 'i', 'g', 'n', 'e', 'd'};
	::llc::s0_t keyStorage[32] = {};
	::llc::u0_t ivStorage[16] = {};
	cnst ::llc::vcs0_t message = messageStorage;
	cnst ::llc::vcs0_t key = keyStorage;
	cnst ::llc::vcu0_t iv = ivStorage;
	::llc::au0_t encrypted;
	::llc::as0_t decrypted;
	LLC_TEST_REQUIRE(results, TEST_AES_RESULT_STRING_OVERLOADS, 0 != ::llc::aesEncode(message, iv, key, ::llc::AES_LEVEL_256, encrypted));
	LLC_TEST_REQUIRE(results, TEST_AES_RESULT_STRING_OVERLOADS, 0 != ::llc::aesDecode(encrypted, iv, key, ::llc::AES_LEVEL_256, decrypted));
	LLC_TEST_CHECK(results, TEST_AES_RESULT_STRING_OVERLOADS, decrypted.size() != message.size() || 0 != memcmp(decrypted.begin(), message.begin(), message.size()));
	encrypted.clear();
	decrypted.clear();
	LLC_TEST_REQUIRE(results, TEST_AES_RESULT_STRING_OVERLOADS, 0 != ::llc::aesEncode(message, key, ::llc::AES_LEVEL_256, encrypted));
	LLC_TEST_REQUIRE(results, TEST_AES_RESULT_STRING_OVERLOADS, 0 != ::llc::aesDecode(encrypted, key, ::llc::AES_LEVEL_256, decrypted));
	LLC_TEST_CHECK(results, TEST_AES_RESULT_STRING_OVERLOADS, decrypted.size() != message.size() || 0 != memcmp(decrypted.begin(), message.begin(), message.size()));
	rtrn 0;
}

::llc::err_t testAES(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testAESECB128(results));
	result = ::llc::min(result, ::testAESECB192(results));
	result = ::llc::min(result, ::testAESECB256(results));
	result = ::llc::min(result, ::testAESCBCLevels(results));
	result = ::llc::min(result, ::testAESAppendedIV(results));
	result = ::llc::min(result, ::testAESShortIVKey(results));
	result = ::llc::min(result, ::testAESCiphertextSize(results));
	result = ::llc::min(result, ::testAESLegalPadding(results));
	result = ::llc::min(result, ::testAESCorruptPadding(results));
	result = ::llc::min(result, ::testAESLevelProperties(results));
	result = ::llc::min(result, ::testAESPointerOverloads(results));
	result = ::llc::min(result, ::testAESStringOverloads(results));
	rtrn result;
}
