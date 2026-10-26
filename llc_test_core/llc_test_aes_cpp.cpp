#include "llc_test_core_suites.h"
#include "llc_aes.h"

GDEFINE_ENUM_TYPE(TEST_AES_CPP_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_AES_CPP_RESULT, VECTOR_128          ,  0, "AES-128 ECB matches the NIST encrypt/decrypt vector.");
GDEFINE_ENUM_VALUED(TEST_AES_CPP_RESULT, VECTOR_192          ,  1, "AES-192 ECB matches the NIST encrypt/decrypt vector.");
GDEFINE_ENUM_VALUED(TEST_AES_CPP_RESULT, VECTOR_256          ,  2, "AES-256 ECB matches the NIST encrypt/decrypt vector.");
GDEFINE_ENUM_VALUED(TEST_AES_CPP_RESULT, APPENDED_IV         ,  3, "The convenience format appends one IV and roundtrips the original bytes.");
GDEFINE_ENUM_VALUED(TEST_AES_CPP_RESULT, LEGAL_PADDING       ,  4, "Every legal PKCS#7 pad length from one through sixteen roundtrips.");
GDEFINE_ENUM_VALUED(TEST_AES_CPP_RESULT, KEY_LENGTH          ,  5, "High-level encoding rejects keys shorter or longer than 32 bytes.");
GDEFINE_ENUM_VALUED(TEST_AES_CPP_RESULT, IV_LENGTH           ,  6, "High-level encoding rejects IVs shorter or longer than 16 bytes.");
GDEFINE_ENUM_VALUED(TEST_AES_CPP_RESULT, EMPTY_CIPHERTEXT    ,  7, "Decoding rejects empty ciphertext without changing the output.");
GDEFINE_ENUM_VALUED(TEST_AES_CPP_RESULT, UNALIGNED_CIPHERTEXT,  8, "Decoding rejects non-block ciphertext without changing the output.");
GDEFINE_ENUM_VALUED(TEST_AES_CPP_RESULT, ZERO_PADDING        ,  9, "Decoding rejects a decrypted block whose final padding byte is zero.");
GDEFINE_ENUM_VALUED(TEST_AES_CPP_RESULT, CORRUPT_PADDING     , 10, "Decoding validates every PKCS#7 padding byte before truncation.");

namespace {
	stxp ::llc::u0_t AES_PLAIN_BLOCK[16] =
		{0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A};
	stxp ::llc::u0_t AES_KEY_128[16] =
		{0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};
	stxp ::llc::u0_t AES_KEY_192[24] =
		{0x8E, 0x73, 0xB0, 0xF7, 0xDA, 0x0E, 0x64, 0x52, 0xC8, 0x10, 0xF3, 0x2B, 0x80, 0x90, 0x79, 0xE5,
		 0x62, 0xF8, 0xEA, 0xD2, 0x52, 0x2C, 0x6B, 0x7B};
	stxp ::llc::u0_t AES_KEY_256[32] =
		{0x60, 0x3D, 0xEB, 0x10, 0x15, 0xCA, 0x71, 0xBE, 0x2B, 0x73, 0xAE, 0xF0, 0x85, 0x7D, 0x77, 0x81,
		 0x1F, 0x35, 0x2C, 0x07, 0x3B, 0x61, 0x08, 0xD7, 0x2D, 0x98, 0x10, 0xA3, 0x09, 0x14, 0xDF, 0xF4};
	stxp ::llc::u0_t AES_CIPHER_128[16] =
		{0x3A, 0xD7, 0x7B, 0xB4, 0x0D, 0x7A, 0x36, 0x60, 0xA8, 0x9E, 0xCA, 0xF3, 0x24, 0x66, 0xEF, 0x97};
	stxp ::llc::u0_t AES_CIPHER_192[16] =
		{0xBD, 0x33, 0x4F, 0x1D, 0x6E, 0x45, 0xF2, 0x5F, 0xF7, 0x12, 0xA2, 0x14, 0x57, 0x1F, 0xA5, 0xCC};
	stxp ::llc::u0_t AES_CIPHER_256[16] =
		{0xF3, 0xEE, 0xD1, 0xBD, 0xB5, 0xD2, 0xA0, 0x3C, 0x06, 0x4B, 0x5A, 0x7E, 0x3D, 0xB1, 0x81, 0xF8};
	stxp ::llc::u0_t AES_IV[16] =
		{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
}

sttc ::llc::err_t testAESCPPVector
	( ::llc::ATestError & results
	, TEST_AES_CPP_RESULT testResult
	, ::llc::AES_LEVEL level
	, cnst ::llc::u0_t * key
	, cnst ::llc::u0_t * expectedCipher
	) {
	::llc::u0_t block[16] = {};
	memcpy(block, AES_PLAIN_BLOCK, szof(block));
	::llc::SAESContext context;
	::llc::aesInitCtx(&context, key, level);
	::llc::aesECBEncrypt(&context, block);
	cnst bool encryptMismatch = 0 != memcmp(block, expectedCipher, szof(block));
	::llc::aesECBDecrypt(&context, block);
	cnst bool decryptMismatch = 0 != memcmp(block, AES_PLAIN_BLOCK, szof(block));
	LLC_TEST_CHECKF(results, testResult, encryptMismatch || decryptMismatch
		, "level:%u encrypted:%u decrypted:%u", (::llc::u2_t)level, encryptMismatch, decryptMismatch);
	rtrn 0;
}

sttc ::llc::err_t testAESCPPAppendedIV(::llc::ATestError & results) {
	::llc::u0_t messageStorage[37] = {};
	for(::llc::u2_t i = 0; i < szof(messageStorage); ++i)
		messageStorage[i] = (::llc::u0_t)(i * 7 + 3);
	cnst ::llc::vcu0_t message = messageStorage;
	cnst ::llc::vcu0_t key = AES_KEY_256;
	::llc::au0_t encrypted, decrypted;
	cnst ::llc::err_t encodeResult = ::llc::aesEncode(message, key, ::llc::AES_LEVEL_256, encrypted);
	cnst ::llc::vcu0_t encryptedView = encrypted;
	cnst ::llc::err_t decodeResult = ::llc::failed(encodeResult) ? -1 : ::llc::aesDecode(encryptedView, key, ::llc::AES_LEVEL_256, decrypted);
	cnst bool mismatch = decrypted.size() != message.size() || (decrypted.size() && 0 != memcmp(decrypted.begin(), message.begin(), message.size()));
	LLC_TEST_CHECKF(results, TEST_AES_CPP_RESULT_APPENDED_IV
		, ::llc::failed(encodeResult) || ::llc::failed(decodeResult) || encrypted.size() != 64 || mismatch
		, "encode:%i decode:%i encrypted:%u decrypted:%u", encodeResult, decodeResult, encrypted.size(), decrypted.size());
	rtrn 0;
}

sttc ::llc::err_t testAESCPPLegalPadding(::llc::ATestError & results) {
	::llc::u0_t messageStorage[16] = {};
	for(::llc::u2_t i = 0; i < szof(messageStorage); ++i)
		messageStorage[i] = (::llc::u0_t)(0x80 + i);
	cnst ::llc::vcu0_t iv = AES_IV;
	cnst ::llc::vcu0_t key = AES_KEY_256;
	for(::llc::u2_t padding = 1; padding <= 16; ++padding) {
		cnst ::llc::u2_t messageSize = padding == 16 ? 16 : 16 - padding;
		cnst ::llc::vcu0_t message = {messageStorage, messageSize};
		::llc::au0_t encrypted, decrypted;
		cnst ::llc::err_t encodeResult = ::llc::aesEncode(message, iv, key, ::llc::AES_LEVEL_256, encrypted);
		cnst ::llc::vcu0_t encryptedView = encrypted;
		cnst ::llc::err_t decodeResult = ::llc::failed(encodeResult) ? -1 : ::llc::aesDecode(encryptedView, iv, key, ::llc::AES_LEVEL_256, decrypted);
		cnst bool mismatch = decrypted.size() != message.size() || (decrypted.size() && 0 != memcmp(decrypted.begin(), message.begin(), message.size()));
		LLC_TEST_CHECKF(results, TEST_AES_CPP_RESULT_LEGAL_PADDING
			, ::llc::failed(encodeResult) || ::llc::failed(decodeResult) || mismatch
			, "padding:%u encode:%i decode:%i decrypted:%u", padding, encodeResult, decodeResult, decrypted.size());
	}
	rtrn 0;
}

sttc ::llc::err_t testAESCPPKeyLength(::llc::ATestError & results) {
	::llc::u0_t keyStorage[33] = {};
	::llc::u0_t messageStorage[1] = {0x5A};
	cnst ::llc::vcu0_t message = messageStorage;
	cnst ::llc::vcu0_t iv = AES_IV;
	for(cnst ::llc::u2_t keySize : ::llc::astu2_t<2>{31, 33}) {
		cnst ::llc::vcu0_t key = {keyStorage, keySize};
		::llc::au0_t output = {0xA5};
		::llc::STestLogBuffer expectedLog;
		::llc::err_t result = 0;
		{
			::llc::STestLogCapture capture(expectedLog);
			result = ::llc::aesEncode(message, iv, key, ::llc::AES_LEVEL_256, output);
		}
		LLC_TEST_CHECKF(results, TEST_AES_CPP_RESULT_KEY_LENGTH
			, !::llc::failed(result) || !expectedLog.Failed || output.size() != 1 || output[0] != 0xA5
			, "key-size:%u result:%i output:%u", keySize, result, output.size());
	}
	rtrn 0;
}

sttc ::llc::err_t testAESCPPIVLength(::llc::ATestError & results) {
	::llc::u0_t ivStorage[17] = {};
	::llc::u0_t messageStorage[1] = {0x5A};
	cnst ::llc::vcu0_t message = messageStorage;
	cnst ::llc::vcu0_t key = AES_KEY_256;
	for(cnst ::llc::u2_t ivSize : ::llc::astu2_t<2>{15, 17}) {
		cnst ::llc::vcu0_t iv = {ivStorage, ivSize};
		::llc::au0_t output;
		::llc::STestLogBuffer expectedLog;
		::llc::err_t result = 0;
		{
			::llc::STestLogCapture capture(expectedLog);
			result = ::llc::aesEncode(message, iv, key, ::llc::AES_LEVEL_256, output);
		}
		LLC_TEST_CHECKF(results, TEST_AES_CPP_RESULT_IV_LENGTH
			, !::llc::failed(result)
			, "iv-size:%u result:%i output:%u", ivSize, result, output.size());
	}
	rtrn 0;
}

sttc ::llc::err_t testAESCPPEmptyCiphertext(::llc::ATestError & results) {
	cnst ::llc::vcu0_t empty;
	cnst ::llc::vcu0_t iv = AES_IV;
	cnst ::llc::vcu0_t key = AES_KEY_256;
	::llc::au0_t output = {0xA5};
	::llc::STestLogBuffer expectedLog;
	::llc::err_t result = 0;
	{
		::llc::STestLogCapture capture(expectedLog);
		result = ::llc::aesDecode(empty, iv, key, ::llc::AES_LEVEL_256, output);
	}
	LLC_TEST_CHECKF(results, TEST_AES_CPP_RESULT_EMPTY_CIPHERTEXT
		, !::llc::failed(result) || !expectedLog.Failed || output.size() != 1 || output[0] != 0xA5
		, "result:%i output:%u", result, output.size());
	rtrn 0;
}

sttc ::llc::err_t testAESCPPUnalignedCiphertext(::llc::ATestError & results) {
	::llc::u0_t ciphertextStorage[17] = {};
	cnst ::llc::vcu0_t iv = AES_IV;
	cnst ::llc::vcu0_t key = AES_KEY_256;
	for(cnst ::llc::u2_t ciphertextSize : ::llc::astu2_t<2>{1, 17}) {
		cnst ::llc::vcu0_t ciphertext = {ciphertextStorage, ciphertextSize};
		::llc::au0_t output = {0xA5};
		::llc::STestLogBuffer expectedLog;
		::llc::err_t result = 0;
		{
			::llc::STestLogCapture capture(expectedLog);
			result = ::llc::aesDecode(ciphertext, iv, key, ::llc::AES_LEVEL_256, output);
		}
		LLC_TEST_CHECKF(results, TEST_AES_CPP_RESULT_UNALIGNED_CIPHERTEXT
			, !::llc::failed(result) || !expectedLog.Failed || output.size() != 1 || output[0] != 0xA5
			, "ciphertext-size:%u result:%i output:%u", ciphertextSize, result, output.size());
	}
	rtrn 0;
}

sttc ::llc::err_t testAESCPPRejectedPadding
	( ::llc::ATestError & results
	, TEST_AES_CPP_RESULT testResult
	, ::llc::u0_t beforeLast
	, ::llc::u0_t last
	) {
	::llc::u0_t ciphertext[16] = {};
	for(::llc::u2_t i = 0; i < szof(ciphertext); ++i)
		ciphertext[i] = (::llc::u0_t)(i + 1);
	ciphertext[14] = beforeLast;
	ciphertext[15] = last;
	::llc::SAESContext context;
	::llc::aesInitCtxIV(&context, AES_KEY_256, ::llc::AES_LEVEL_256, AES_IV);
	::llc::aesCBCEncryptBuffer(&context, ciphertext, szof(ciphertext));
	cnst ::llc::vcu0_t ciphertextView = ciphertext;
	cnst ::llc::vcu0_t iv = AES_IV;
	cnst ::llc::vcu0_t key = AES_KEY_256;
	::llc::au0_t output;
	::llc::STestLogBuffer expectedLog;
	::llc::err_t result = 0;
	{
		::llc::STestLogCapture capture(expectedLog);
		result = ::llc::aesDecode(ciphertextView, iv, key, ::llc::AES_LEVEL_256, output);
	}
	LLC_TEST_CHECKF(results, testResult, !::llc::failed(result)
		, "tail:%u,%u result:%i output:%u", beforeLast, last, result, output.size());
	rtrn 0;
}

::llc::err_t testAESCPP(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testAESCPPVector(results, TEST_AES_CPP_RESULT_VECTOR_128, ::llc::AES_LEVEL_128, AES_KEY_128, AES_CIPHER_128));
	result = ::llc::min(result, ::testAESCPPVector(results, TEST_AES_CPP_RESULT_VECTOR_192, ::llc::AES_LEVEL_192, AES_KEY_192, AES_CIPHER_192));
	result = ::llc::min(result, ::testAESCPPVector(results, TEST_AES_CPP_RESULT_VECTOR_256, ::llc::AES_LEVEL_256, AES_KEY_256, AES_CIPHER_256));
	result = ::llc::min(result, ::testAESCPPAppendedIV(results));
	result = ::llc::min(result, ::testAESCPPLegalPadding(results));
	result = ::llc::min(result, ::testAESCPPKeyLength(results));
	result = ::llc::min(result, ::testAESCPPIVLength(results));
	result = ::llc::min(result, ::testAESCPPEmptyCiphertext(results));
	result = ::llc::min(result, ::testAESCPPUnalignedCiphertext(results));
	result = ::llc::min(result, ::testAESCPPRejectedPadding(results, TEST_AES_CPP_RESULT_ZERO_PADDING, 0x7F, 0x00));
	result = ::llc::min(result, ::testAESCPPRejectedPadding(results, TEST_AES_CPP_RESULT_CORRUPT_PADDING, 0x7F, 0x02));
	rtrn result;
}
