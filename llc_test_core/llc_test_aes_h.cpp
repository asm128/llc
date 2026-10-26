#include "llc_test_core_suites.h"
#include "llc_aes.h"

GDEFINE_ENUM_TYPE(TEST_AES_H_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_AES_H_RESULT, LEVEL_ENUMS              ,  0, "AES level enumerators preserve their public values.");
GDEFINE_ENUM_VALUED(TEST_AES_H_RESULT, MODE_ENUMS               ,  1, "AES mode enumerators preserve their public values.");
GDEFINE_ENUM_VALUED(TEST_AES_H_RESULT, LEVEL_PROPERTIES         ,  2, "Every level exposes its canonical key and expanded-key lengths.");
GDEFINE_ENUM_VALUED(TEST_AES_H_RESULT, BLOCK_IV_CONSTANTS       ,  3, "AES block and IV constants remain sixteen bytes.");
GDEFINE_ENUM_VALUED(TEST_AES_H_RESULT, CONTEXT_DEFAULTS         ,  4, "A default AES context has level 128, no round key and a zero IV.");
GDEFINE_ENUM_VALUED(TEST_AES_H_RESULT, LOW_LEVEL_SIGNATURES     ,  5, "All eight low-level operations retain their exact signatures.");
GDEFINE_ENUM_VALUED(TEST_AES_H_RESULT, BASE_VIEW_SIGNATURES     ,  6, "The four non-inline view overloads retain their exact signatures.");
GDEFINE_ENUM_VALUED(TEST_AES_H_RESULT, POINTER_FIXED_ROUNDTRIP  ,  7, "Pointer overloads with an explicit IV preserve the message.");
GDEFINE_ENUM_VALUED(TEST_AES_H_RESULT, POINTER_APPEND_ROUNDTRIP ,  8, "Pointer overloads with an appended IV preserve the message.");
GDEFINE_ENUM_VALUED(TEST_AES_H_RESULT, TEXT_KEY_ROUNDTRIP       ,  9, "Text-key overloads preserve a byte-view message.");
GDEFINE_ENUM_VALUED(TEST_AES_H_RESULT, TEXT_MESSAGE_ROUNDTRIP   , 10, "Text-message overloads decode into the signed-byte output type.");

namespace {
	stxp ::llc::u0_t AES_H_KEY[32] =
		{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
		 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};
	stxp ::llc::s0_t AES_H_KEY_TEXT[32] =
		{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
		 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V'};
	stxp ::llc::u0_t AES_H_IV[16] =
		{0xF0, 0xE1, 0xD2, 0xC3, 0xB4, 0xA5, 0x96, 0x87, 0x78, 0x69, 0x5A, 0x4B, 0x3C, 0x2D, 0x1E, 0x0F};
	stxp ::llc::u0_t AES_H_MESSAGE[19] =
		{0x41, 0x50, 0x49, 0x2D, 0x6F, 0x76, 0x65, 0x72, 0x6C, 0x6F, 0x61, 0x64, 0x2D, 0x74, 0x65, 0x73,
		 0x74, 0x21, 0x7F};
	stxp ::llc::s0_t AES_H_MESSAGE_TEXT[18] =
		{'t', 'e', 'x', 't', '-', 'm', 'e', 's', 's', 'a', 'g', 'e', '-', 'A', 'E', 'S', '!', '?'};

	sttc bool bytesDiffer(cnst ::llc::u0_t * left, ::llc::u2_t leftSize, cnst ::llc::u0_t * right, ::llc::u2_t rightSize) {
		rtrn leftSize != rightSize || (leftSize && 0 != memcmp(left, right, leftSize));
	}
}

sttc ::llc::err_t testAESHLevelEnums(::llc::ATestError & results) {
	LLC_TEST_CHECK(results, TEST_AES_H_RESULT_LEVEL_ENUMS
		, (::llc::u2_t)::llc::AES_LEVEL_128 != 0 || (::llc::u2_t)::llc::AES_LEVEL_192 != 1 || (::llc::u2_t)::llc::AES_LEVEL_256 != 2);
	rtrn 0;
}

sttc ::llc::err_t testAESHModeEnums(::llc::ATestError & results) {
	LLC_TEST_CHECK(results, TEST_AES_H_RESULT_MODE_ENUMS
		, (::llc::u2_t)::llc::AES_MODE_ECB != 0 || (::llc::u2_t)::llc::AES_MODE_CBC != 1 || (::llc::u2_t)::llc::AES_MODE_CTR != 2);
	rtrn 0;
}

sttc ::llc::err_t testAESHLevelProperties(::llc::ATestError & results) {
	stxp ::llc::SAESLevelProperties expected[3] = {{16, 176}, {24, 208}, {32, 240}};
	for(::llc::u2_t iLevel = 0; iLevel < 3; ++iLevel)
		LLC_TEST_CHECKF(results, TEST_AES_H_RESULT_LEVEL_PROPERTIES
			, ::llc::AES_LEVEL_PROPERTIES[iLevel].KeyLength != expected[iLevel].KeyLength || ::llc::AES_LEVEL_PROPERTIES[iLevel].KeyExpSize != expected[iLevel].KeyExpSize
			, "level:%u key:%i expanded:%i", iLevel, ::llc::AES_LEVEL_PROPERTIES[iLevel].KeyLength, ::llc::AES_LEVEL_PROPERTIES[iLevel].KeyExpSize);
	rtrn 0;
}

sttc ::llc::err_t testAESHBlockIVConstants(::llc::ATestError & results) {
	LLC_TEST_CHECKF(results, TEST_AES_H_RESULT_BLOCK_IV_CONSTANTS
		, ::llc::AES_SIZEBLOCK != 16 || ::llc::AES_SIZEIV != 16
		, "block:%u iv:%u", ::llc::AES_SIZEBLOCK, ::llc::AES_SIZEIV);
	rtrn 0;
}

sttc ::llc::err_t testAESHContextDefaults(::llc::ATestError & results) {
	::llc::SAESContext context;
	bool nonzeroIV = false;
	for(::llc::u2_t iByte = 0; iByte < ::llc::AES_SIZEIV; ++iByte)
		nonzeroIV |= context.Iv[iByte] != 0;
	LLC_TEST_CHECKF(results, TEST_AES_H_RESULT_CONTEXT_DEFAULTS
		, context.Level != ::llc::AES_LEVEL_128 || context.RoundKey.size() || nonzeroIV
		, "level:%u round-key:%u nonzero-iv:%u", (::llc::u2_t)context.Level, context.RoundKey.size(), nonzeroIV);
	rtrn 0;
}

sttc ::llc::err_t testAESHLowLevelSignatures(::llc::ATestError & results) {
	typedef void (*TInit)(::llc::SAESContext *, cnst ::llc::u0_t *, ::llc::AES_LEVEL);
	typedef void (*TInitIV)(::llc::SAESContext *, cnst ::llc::u0_t *, ::llc::AES_LEVEL, cnst ::llc::u0_t *);
	typedef void (*TSetIV)(::llc::SAESContext *, cnst ::llc::u0_t *);
	typedef void (*TBlock)(::llc::SAESContext *, ::llc::u0_t *);
	typedef void (*TBuffer)(::llc::SAESContext *, ::llc::u0_t *, ::llc::u2_t);
	cnst TInit init = &::llc::aesInitCtx;
	cnst TInitIV initIV = &::llc::aesInitCtxIV;
	cnst TSetIV setIV = &::llc::aesCtxSetIV;
	cnst TBlock ecbEncrypt = &::llc::aesECBEncrypt;
	cnst TBlock ecbDecrypt = &::llc::aesECBDecrypt;
	cnst TBuffer cbcEncrypt = &::llc::aesCBCEncryptBuffer;
	cnst TBuffer cbcDecrypt = &::llc::aesCBCDecryptBuffer;
	cnst TBuffer ctrXCrypt = &::llc::aesCTRXCryptBuffer;
	LLC_TEST_CHECK(results, TEST_AES_H_RESULT_LOW_LEVEL_SIGNATURES
		, !init || !initIV || !setIV || !ecbEncrypt || !ecbDecrypt || !cbcEncrypt || !cbcDecrypt || !ctrXCrypt);
	rtrn 0;
}

sttc ::llc::err_t testAESHBaseViewSignatures(::llc::ATestError & results) {
	typedef ::llc::err_t (*TFixed)(::llc::vcu0_c &, ::llc::vcu0_c &, ::llc::vcu0_c &, ::llc::AES_LEVEL, ::llc::au0_t &);
	typedef ::llc::err_t (*TAppended)(::llc::vcu0_c &, ::llc::vcu0_c &, ::llc::AES_LEVEL, ::llc::au0_t &);
	cnst TFixed encodeFixed = static_cast<TFixed>(&::llc::aesEncode);
	cnst TFixed decodeFixed = static_cast<TFixed>(&::llc::aesDecode);
	cnst TAppended encodeAppended = static_cast<TAppended>(&::llc::aesEncode);
	cnst TAppended decodeAppended = static_cast<TAppended>(&::llc::aesDecode);
	LLC_TEST_CHECK(results, TEST_AES_H_RESULT_BASE_VIEW_SIGNATURES, !encodeFixed || !decodeFixed || !encodeAppended || !decodeAppended);
	rtrn 0;
}

sttc ::llc::err_t testAESHPointerFixedRoundtrip(::llc::ATestError & results) {
	cnst ::llc::vcu0_t iv = AES_H_IV;
	cnst ::llc::vcu0_t key = AES_H_KEY;
	::llc::au0_t encrypted, decrypted;
	cnst ::llc::err_t encodeResult = ::llc::aesEncode(AES_H_MESSAGE, szof(AES_H_MESSAGE), iv, key, ::llc::AES_LEVEL_256, encrypted);
	cnst ::llc::err_t decodeResult = ::llc::failed(encodeResult) ? -1 : ::llc::aesDecode(encrypted.begin(), encrypted.size(), iv, key, ::llc::AES_LEVEL_256, decrypted);
	cnst bool mismatch = bytesDiffer(decrypted.begin(), decrypted.size(), AES_H_MESSAGE, szof(AES_H_MESSAGE));
	LLC_TEST_CHECKF(results, TEST_AES_H_RESULT_POINTER_FIXED_ROUNDTRIP
		, ::llc::failed(encodeResult) || ::llc::failed(decodeResult) || mismatch
		, "encode:%i decode:%i", encodeResult, decodeResult);
	rtrn 0;
}

sttc ::llc::err_t testAESHPointerAppendRoundtrip(::llc::ATestError & results) {
	cnst ::llc::vcu0_t key = AES_H_KEY;
	::llc::au0_t encrypted, decrypted;
	cnst ::llc::err_t encodeResult = ::llc::aesEncode(AES_H_MESSAGE, szof(AES_H_MESSAGE), key, ::llc::AES_LEVEL_256, encrypted);
	cnst ::llc::err_t decodeResult = ::llc::failed(encodeResult) ? -1 : ::llc::aesDecode(encrypted.begin(), encrypted.size(), key, ::llc::AES_LEVEL_256, decrypted);
	cnst bool mismatch = bytesDiffer(decrypted.begin(), decrypted.size(), AES_H_MESSAGE, szof(AES_H_MESSAGE));
	LLC_TEST_CHECKF(results, TEST_AES_H_RESULT_POINTER_APPEND_ROUNDTRIP
		, ::llc::failed(encodeResult) || ::llc::failed(decodeResult) || mismatch
		, "encode:%i decode:%i", encodeResult, decodeResult);
	rtrn 0;
}

sttc ::llc::err_t testAESHTextKeyRoundtrip(::llc::ATestError & results) {
	cnst ::llc::vcu0_t message = AES_H_MESSAGE;
	cnst ::llc::vcu0_t iv = AES_H_IV;
	cnst ::llc::vcs0_c key = AES_H_KEY_TEXT;
	::llc::au0_t encrypted, decrypted;
	cnst ::llc::err_t encodeResult = ::llc::aesEncode(message, iv, key, ::llc::AES_LEVEL_256, encrypted);
	cnst ::llc::vcu0_t encryptedView = encrypted;
	cnst ::llc::err_t decodeResult = ::llc::failed(encodeResult) ? -1 : ::llc::aesDecode(encryptedView, iv, key, ::llc::AES_LEVEL_256, decrypted);
	cnst bool mismatch = bytesDiffer(decrypted.begin(), decrypted.size(), AES_H_MESSAGE, szof(AES_H_MESSAGE));
	LLC_TEST_CHECKF(results, TEST_AES_H_RESULT_TEXT_KEY_ROUNDTRIP
		, ::llc::failed(encodeResult) || ::llc::failed(decodeResult) || mismatch
		, "encode:%i decode:%i", encodeResult, decodeResult);
	rtrn 0;
}

sttc ::llc::err_t testAESHTextMessageRoundtrip(::llc::ATestError & results) {
	cnst ::llc::vcs0_c message = AES_H_MESSAGE_TEXT;
	cnst ::llc::vcu0_t iv = AES_H_IV;
	cnst ::llc::vcs0_c key = AES_H_KEY_TEXT;
	::llc::au0_t encrypted;
	::llc::as0_t decrypted;
	cnst ::llc::err_t encodeResult = ::llc::aesEncode(message, iv, key, ::llc::AES_LEVEL_256, encrypted);
	cnst ::llc::vcu0_t encryptedView = encrypted;
	cnst ::llc::err_t decodeResult = ::llc::failed(encodeResult) ? -1 : ::llc::aesDecode(encryptedView, iv, key, ::llc::AES_LEVEL_256, decrypted);
	cnst bool mismatch = decrypted.size() != message.size() || (decrypted.size() && 0 != memcmp(decrypted.begin(), message.begin(), message.size()));
	LLC_TEST_CHECKF(results, TEST_AES_H_RESULT_TEXT_MESSAGE_ROUNDTRIP
		, ::llc::failed(encodeResult) || ::llc::failed(decodeResult) || mismatch
		, "encode:%i decode:%i", encodeResult, decodeResult);
	rtrn 0;
}

::llc::err_t testAESH(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testAESHLevelEnums(results));
	result = ::llc::min(result, ::testAESHModeEnums(results));
	result = ::llc::min(result, ::testAESHLevelProperties(results));
	result = ::llc::min(result, ::testAESHBlockIVConstants(results));
	result = ::llc::min(result, ::testAESHContextDefaults(results));
	result = ::llc::min(result, ::testAESHLowLevelSignatures(results));
	result = ::llc::min(result, ::testAESHBaseViewSignatures(results));
	result = ::llc::min(result, ::testAESHPointerFixedRoundtrip(results));
	result = ::llc::min(result, ::testAESHPointerAppendRoundtrip(results));
	result = ::llc::min(result, ::testAESHTextKeyRoundtrip(results));
	result = ::llc::min(result, ::testAESHTextMessageRoundtrip(results));
	rtrn result;
}
