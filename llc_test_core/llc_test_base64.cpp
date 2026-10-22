#include "llc_base64.h"
#include "llc_noise.h"
#include "llc_font_defaults.h"
#include "llc_png.h"
#include "llc_file.h"
#include "llc_path.h"
#include "llc_string_compose.h"

#include "llc_test_core.h"
#include "llc_test_base64_font_data.h"

//#define LLC_TEST_BASE64_DRAW_FONTS

GDEFINE_ENUM_TYPE(BASE64_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, OK					,  0, "All Base64 tests passed.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, ENCODE_VECTOR			,  1, "base64Encode() did not produce an RFC 4648 reference value.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, DECODE_VECTOR			,  2, "base64Decode() did not recover an RFC 4648 reference value.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, ENCODE_SIZE			,  3, "base64Encode() produced the wrong output count.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, DECODE_SIZE			,  4, "base64Decode() produced the wrong output count.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, TERMINATOR			,  5, "A Base64 output did not retain its hidden null terminator.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, ENCODE_APPEND			,  6, "base64Encode() did not append without changing the existing output prefix.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, DECODE_APPEND			,  7, "base64Decode() did not append without changing the existing output prefix.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, COUNTED_TERMINATOR		,  8, "base64Decode() did not accept one counted null terminator.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, FILESYSTEM_ALPHABET		,  9, "The filesystem-safe Base64 alphabet produced or decoded the wrong value.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, CUSTOM_ALPHABET			, 10, "The configurable Base64 alphabet or padding symbol produced the wrong value.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, OVERLOAD_ENCODE			, 11, "A base64Encode() container overload produced the wrong value.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, OVERLOAD_DECODE			, 12, "A base64Decode() container overload produced the wrong value.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, RANDOM_ENCODE			, 13, "base64Encode() failed for deterministic generated binary input.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, RANDOM_DECODE			, 14, "base64Decode() failed for deterministic generated Base64 input.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, RANDOM_ROUND_TRIP		, 15, "A deterministic generated Base64 round trip changed the binary input.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, INVALID_LENGTH			, 16, "base64Decode() accepted an invalid encoded length or threw while rejecting it.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, INVALID_SYMBOL			, 17, "base64Decode() accepted a symbol outside the configured alphabet or threw while rejecting it.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, INVALID_PADDING			, 18, "base64Decode() accepted padding outside the final legal positions.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, NONCANONICAL_PADDING		, 19, "base64Decode() accepted nonzero unused bits in a padded final quartet.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, INVALID_ALPHABET			, 20, "A Base64 operation accepted an alphabet that was not exactly 64 unique symbols excluding its pad.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, STATIC_SYMBOL_MAP			, 22, "A constexpr Base64 symbol map produced the wrong encoded or decoded value.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, FONT_DECODE				, 23, "A field-tested CP437 font failed to decode.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, FONT_SIZE					, 24, "A decoded CP437 font produced the wrong storage size.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, FONT_CONTENT				, 25, "A decoded CP437 font did not preserve its known bitmap contents.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, FONT_ROUND_TRIP			, 26, "A decoded CP437 font did not encode back to its original Base64 text.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, FONT_ASCII_DRAW			, 27, "The CP437 font bits did not draw the known ASCII letters correctly.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, ENCODE_APPEND_RESULT		, 28, "base64Encode() failed while appending.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, DECODE_APPEND_RESULT		, 29, "base64Decode() failed while appending.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, FILESYSTEM_ENCODE_RESULT	, 30, "Filesystem-safe Base64 encoding failed.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, FILESYSTEM_DECODE_RESULT	, 31, "Filesystem-safe Base64 decoding failed.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, FILESYSTEM_DECODE_VALUE	, 32, "Filesystem-safe Base64 decoding returned the wrong value.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, CUSTOM_ENCODE_RESULT	, 33, "Custom-alphabet Base64 encoding failed.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, CUSTOM_DECODE_RESULT	, 34, "Custom-alphabet Base64 decoding failed.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, CUSTOM_DECODE_VALUE	, 35, "Custom-alphabet Base64 decoding returned the wrong value.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, STATIC_ENCODE_RESULT	, 36, "Static-map Base64 encoding failed.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, STATIC_DECODE_RESULT	, 37, "Static-map Base64 decoding failed.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, STATIC_DECODE_VALUE	, 38, "Static-map Base64 decoding returned the wrong value.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, OVERLOAD_ENCODE_RESULT	, 39, "A base64Encode() container overload failed.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, OVERLOAD_DECODE_RESULT	, 40, "A base64Decode() container overload failed.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, COUNTED_TERMINATOR_RESULT, 41, "base64Decode() failed on one counted null terminator.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, REJECT_THROW			, 42, "A rejected Base64 operation threw.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, PRESERVE_ADDRESS		, 43, "A rejected Base64 operation changed the output address.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, PRESERVE_COUNT			, 44, "A rejected Base64 operation changed the output count.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, PRESERVE_CAPACITY		, 45, "A rejected Base64 operation changed the output capacity.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, PRESERVE_CONTENT		, 46, "A rejected Base64 operation changed the output contents.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, PRESERVE_TERMINATOR		, 47, "A rejected Base64 operation changed the hidden terminator.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, FONT_LOGICAL_SIZE		, 48, "The decoded CP437 font is smaller than its logical bitmap size.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, FONT_ENCODE_RESULT		, 49, "Encoding a decoded CP437 font failed.");

sttc bool bytesMismatch(::llc::vcu0_c & actual, ::llc::vcu0_c & expected) {
	rtrn actual.size() != expected.size() || (actual.size() && 0 != memcmp(actual.begin(), expected.begin(), actual.size()));
}

stin ::llc::u2_t hashByte(::llc::u2_t hash, ::llc::u0_t value) { rtrn (hash ^ value) * 16777619U; }

sttc ::llc::u2_t hashBytes(::llc::vcu0_c & bytes) {
	::llc::u2_t hash = 2166136261U;
	for(::llc::u2_t iByte = 0; iByte < bytes.size(); ++iByte)
		hash = hashByte(hash, bytes[iByte]);
	rtrn hash;
}

sttc ::llc::u2_t drawBase64FontLetter(cnst SBase64FontFixture & font, ::llc::vcu0_c & bitmap, ::llc::u0_t character, ::llc::u2_t hash, ::llc::u2_t iFont) {
#ifndef LLC_TEST_BASE64_DRAW_FONTS
	(void)iFont;
#endif
	::llc::astsc_t<512> drawing = {};
	::llc::u2_t count = 0;
	::llc::u2_c glyphOffset = character * font.Width * font.Height;
	for(::llc::u2_t y = 0; y < font.Height; ++y) {
		for(::llc::u2_t x = 0; x < font.Width; ++x) {
			::llc::u2_c bitIndex = glyphOffset + y * font.Width + x;
			cnst ::llc::sc_t symbol = bitmap[bitIndex >> 3] & (1U << (bitIndex & 7)) ? '#' : '.';
			drawing[count++] = symbol;
			hash = hashByte(hash, (::llc::u0_t)symbol);
		}
		drawing[count++] = '\n';
		hash = hashByte(hash, '\n');
	}
#ifdef LLC_TEST_BASE64_DRAW_FONTS
	always_printf("CP437 font %u (%ux%u), '%c':\n%.*s", iFont, font.Width, font.Height, character, (int)count, drawing.begin());
#endif
	rtrn hash;
}

stct SBase64Vector {
	::llc::vcs0_t	Binary;
	::llc::vcst_t	Encoded;
};

stxp ::llc::vcst_t			BASE64_REVERSED_SYMBOLS	= "/+9876543210zyxwvutsrqponmlkjihgfedcbaZYXWVUTSRQPONMLKJIHGFEDCBA";
stxp ::llc::SBase64SymbolMap	BASE64_REVERSED_MAP		= ::llc::base64SymbolMap(BASE64_REVERSED_SYMBOLS, '*');
stxp ::llc::SBase64SymbolMap	BASE64_SHORT_MAP			= ::llc::base64SymbolMap("ABC");
stxp ::llc::SBase64SymbolMap	BASE64_PAD_CONFLICT_MAP	= ::llc::base64SymbolMap(::llc::b64Symbols, 'A');
stxp ::llc::SBase64SymbolMap	BASE64_DUPLICATE_MAP		= ::llc::base64SymbolMap("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+A");
static_assert( 0 == BASE64_REVERSED_MAP.Error);
static_assert(-1 == BASE64_SHORT_MAP.Error);
static_assert(-2 == BASE64_PAD_CONFLICT_MAP.Error);
static_assert(-3 == BASE64_DUPLICATE_MAP.Error);
static_assert( 0 == BASE64_REVERSED_MAP.Decode.Storage[(::llc::u0_t)'/']);
static_assert(63 == BASE64_REVERSED_MAP.Decode.Storage[(::llc::u0_t)'A']);

sttc ::llc::err_t testBase64Vectors(::llc::ATestError & errors) {
	always_printf("%s", "BASE64 reference vectors: input -> encoded text -> decoded bytes; lengths exclude terminators.");
	stxp SBase64Vector vectors[] =
		{ {{0, ""}		, ""}
		, {{1, "f"}		, "Zg=="}
		, {{2, "fo"}		, "Zm8="}
		, {{3, "foo"}	, "Zm9v"}
		, {{4, "foob"}	, "Zm9vYg=="}
		, {{5, "fooba"}	, "Zm9vYmE="}
		, {{6, "foobar"}	, "Zm9vYmFy"}
		};
	for(::llc::u2_t iVector = 0; iVector < ::llc::size(vectors); ++iVector) {
		cnst SBase64Vector & vector = vectors[iVector];
		::llc::au0_t encoded;
		cnst ::llc::err_t encodeResult = ::llc::base64Encode(vector.Binary, encoded);
		always_printf("Vector:%u input:'%.*s' (%u bytes)\nEncoded:'%.*s'\nExpected:'%.*s'\nReturn:%i.", iVector, (int)vector.Binary.size(), vector.Binary.begin(), vector.Binary.size(), (int)encoded.size(), encoded.begin(), LLC_VIEW_ARGS(vector.Encoded), encodeResult);
		LLC_TEST_REQUIREF(errors, BASE64_TEST_RESULT_ENCODE_VECTOR, ::llc::failed(encodeResult) , "Reference encode failed. vector:%u, input:%u, result:%i." , iVector, vector.Binary.size(), encodeResult );
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_ENCODE_SIZE, encoded.size() != vector.Encoded.size() , "Encoded size mismatch. vector:%u, input:%u, actual:%u, expected:%u." , iVector, vector.Binary.size(), encoded.size(), vector.Encoded.size() );
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_ENCODE_VECTOR, bytesMismatch(encoded.cu8(), vector.Encoded.cu8()) , "Encoded value mismatch. vector:%u, input:'%.*s', actual:'%.*s', expected:'%.*s'." , iVector, (int)vector.Binary.size(), vector.Binary.begin(), (int)encoded.size(), encoded.begin(), (int)vector.Encoded.size(), vector.Encoded.begin() );
		if(encoded.size())
			LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_TERMINATOR, encoded.begin()[encoded.size()] , "Encoded terminator mismatch. vector:%u, size:%u, terminator:0x%02X." , iVector, encoded.size(), encoded.begin()[encoded.size()] );

		::llc::au0_t decoded;
		cnst ::llc::err_t decodeResult = ::llc::base64Decode(vector.Encoded, decoded);
		always_printf("Decoded:'%.*s' (%u bytes); expected:'%.*s' (%u bytes); return:%i; bytes:%s.", (int)decoded.size(), decoded.begin(), decoded.size(), (int)vector.Binary.size(), vector.Binary.begin(), vector.Binary.size(), decodeResult, bytesMismatch(decoded.cu8(), vector.Binary.cu8()) ? "FAIL" : "PASS");
		LLC_TEST_REQUIREF(errors, BASE64_TEST_RESULT_DECODE_VECTOR, ::llc::failed(decodeResult) , "Reference decode failed. vector:%u, input:%u, result:%i." , iVector, vector.Encoded.size(), decodeResult );
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_DECODE_SIZE, decoded.size() != vector.Binary.size() , "Decoded size mismatch. vector:%u, input:%u, actual:%u, expected:%u." , iVector, vector.Encoded.size(), decoded.size(), vector.Binary.size() );
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_DECODE_VECTOR, bytesMismatch(decoded.cu8(), vector.Binary.cu8()) , "Decoded value mismatch. vector:%u, encoded:'%.*s', actual:%u bytes, expected:%u bytes." , iVector, (int)vector.Encoded.size(), vector.Encoded.begin(), decoded.size(), vector.Binary.size() );
		if(decoded.size())
			LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_TERMINATOR, decoded.begin()[decoded.size()] , "Decoded terminator mismatch. vector:%u, size:%u, terminator:0x%02X." , iVector, decoded.size(), decoded.begin()[decoded.size()] );
	}
	rtrn 0;
}

sttc ::llc::err_t testBase64Append(::llc::ATestError & errors) {
	::llc::au0_t encoded = {'p', 'r', 'e', ':'};
	cnst ::llc::u0_t expectedEncoded[] = {'p', 'r', 'e', ':', 'T', 'Q', '=', '='};
	cnst ::llc::err_t encodeResult = ::llc::base64Encode(::llc::vcst_t{"M"}, encoded);
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_ENCODE_APPEND_RESULT, ::llc::failed(encodeResult) , "Encode append result:%i.", encodeResult);
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_ENCODE_APPEND, bytesMismatch(encoded, {expectedEncoded}) , "Encode append actual:%u, expected:%u.", encoded.size(), ::llc::size(expectedEncoded));
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_TERMINATOR, encoded.begin()[encoded.size()] , "Encode append terminator mismatch. size:%u, terminator:0x%02X." , encoded.size(), encoded.begin()[encoded.size()] );

	::llc::au0_t decoded = {0xA5, 0x5A};
	cnst ::llc::u0_t expectedDecoded[] = {0xA5, 0x5A, 'M'};
	cnst ::llc::err_t decodeResult = ::llc::base64Decode("TQ==", decoded);
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_DECODE_APPEND_RESULT, ::llc::failed(decodeResult) , "Decode append result:%i.", decodeResult);
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_DECODE_APPEND, bytesMismatch(decoded, {expectedDecoded}) , "Decode append actual:%u, expected:%u.", decoded.size(), ::llc::size(expectedDecoded));
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_TERMINATOR, decoded.begin()[decoded.size()] , "Decode append terminator mismatch. size:%u, terminator:0x%02X." , decoded.size(), decoded.begin()[decoded.size()] );

	cnst ::llc::u0_t encodedBefore[] = {'p', 'r', 'e', ':', 'T', 'Q', '=', '='};
	cnst ::llc::u0_t decodedBefore[] = {0xA5, 0x5A, 'M'};
	if_fail_fe(::llc::base64Encode(::llc::vcst_t{""}, encoded));
	if_fail_fe(::llc::base64Decode("", decoded));
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_ENCODE_APPEND, bytesMismatch(encoded, {encodedBefore}) , "Empty encode changed existing output. actual:%u, expected:%u." , encoded.size(), ::llc::size(encodedBefore) );
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_DECODE_APPEND, bytesMismatch(decoded, {decodedBefore}) , "Empty decode changed existing output. actual:%u, expected:%u." , decoded.size(), ::llc::size(decodedBefore) );
	rtrn 0;
}

sttc ::llc::err_t testBase64Alphabets(::llc::ATestError & errors) {
	cnst ::llc::u0_t binary[] = {0xFB, 0xFF};
	::llc::au0_t encoded;
	cnst ::llc::err_t encodeResult = ::llc::base64EncodeFS({binary}, encoded);
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_FILESYSTEM_ENCODE_RESULT, ::llc::failed(encodeResult) , "Filesystem-safe encode result:%i.", encodeResult);
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_FILESYSTEM_ALPHABET, bytesMismatch(encoded, ::llc::vcst_t{"-_8="}.cu8()) , "Filesystem-safe encoded:'%.*s', expected:'-_8='.", (int)encoded.size(), encoded.begin());
	::llc::au0_t decoded;
	cnst ::llc::err_t decodeResult = ::llc::base64DecodeFS(encoded, decoded);
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_FILESYSTEM_DECODE_RESULT, ::llc::failed(decodeResult) , "Filesystem-safe decode result:%i.", decodeResult);
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_FILESYSTEM_DECODE_VALUE, bytesMismatch(decoded, {binary}) , "Filesystem-safe decoded bytes:%u, expected:%u.", decoded.size(), ::llc::size(binary));

	cnst ::llc::u0_t zero[] = {0};
	encoded.clear();
	cnst ::llc::err_t customEncode = ::llc::base64Encode(BASE64_REVERSED_SYMBOLS, '*', {zero}, encoded);
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_CUSTOM_ENCODE_RESULT, ::llc::failed(customEncode) , "Custom encode result:%i.", customEncode);
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_CUSTOM_ALPHABET, bytesMismatch(encoded, ::llc::vcst_t{"//**"}.cu8()) , "Custom encoded:'%.*s', expected:'//**'.", (int)encoded.size(), encoded.begin());
	decoded.clear();
	cnst ::llc::err_t customDecode = ::llc::base64Decode(BASE64_REVERSED_SYMBOLS, '*', encoded, decoded);
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_CUSTOM_DECODE_RESULT, ::llc::failed(customDecode) , "Custom decode result:%i.", customDecode);
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_CUSTOM_DECODE_VALUE, bytesMismatch(decoded, {zero}) , "Custom decoded bytes:%u, expected:1.", decoded.size());

	encoded.clear();
	cnst ::llc::err_t staticEncode = ::llc::base64Encode(BASE64_REVERSED_MAP, {zero}, encoded);
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_STATIC_ENCODE_RESULT, ::llc::failed(staticEncode) , "Static-map encode result:%i.", staticEncode);
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_STATIC_SYMBOL_MAP, bytesMismatch(encoded, ::llc::vcst_t{"//**"}.cu8()) , "Static-map encoded:'%.*s', expected:'//**'.", (int)encoded.size(), encoded.begin());
	decoded.clear();
	cnst ::llc::err_t staticDecode = ::llc::base64Decode(BASE64_REVERSED_MAP, encoded, decoded);
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_STATIC_DECODE_RESULT, ::llc::failed(staticDecode) , "Static-map decode result:%i.", staticDecode);
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_STATIC_DECODE_VALUE, bytesMismatch(decoded, {zero}) , "Static-map decoded bytes:%u, expected:1.", decoded.size());
	rtrn 0;
}

sttc ::llc::err_t testBase64Overloads(::llc::ATestError & errors, bool fileSafe) {
	cnst ::llc::u0_t binary[] = {'M', 'a', 'n'};
	cnst ::llc::u0_t encodedBytes[] = {'T', 'W', 'F', 'u'};
	cnst ::llc::s0_t binarySigned[] = {'M', 'a', 'n'};
	cnst ::llc::s0_t encodedSigned[] = {'T', 'W', 'F', 'u'};
	cnst ::llc::sc_t encodedChars[] = {'T', 'W', 'F', 'u'};
	cnst ::llc::vcu0_t binaryU = {binary};
	cnst ::llc::vcs0_t binaryS = {binarySigned};
	cnst ::llc::vcu0_t encodedU = {encodedBytes};
	cnst ::llc::vcs0_t encodedS = {encodedSigned};
	cnst ::llc::vcst_t encodedC = {encodedChars};
	auto testEncode = [&](auto input, auto & output, ::llc::vcst_t name) -> ::llc::err_t {
		cnst ::llc::err_t result = fileSafe ? ::llc::base64EncodeFS(input, output) : ::llc::base64Encode(input, output);
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_OVERLOAD_ENCODE_RESULT, ::llc::failed(result) , "%s encode overload:%.*s, result:%i." , fileSafe ? "Filesystem-safe" : "Standard", (int)name.size(), name.begin(), result);
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_OVERLOAD_ENCODE, bytesMismatch(output.cu8(), encodedU) , "%s encode overload:%.*s, actual:%u, expected:%u." , fileSafe ? "Filesystem-safe" : "Standard", (int)name.size(), name.begin(), output.size(), encodedU.size());
		rtrn 0;
	};
	auto testDecode = [&](auto input, auto & output, ::llc::vcst_t name) -> ::llc::err_t {
		cnst ::llc::err_t result = fileSafe ? ::llc::base64DecodeFS(input, output) : ::llc::base64Decode(input, output);
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_OVERLOAD_DECODE_RESULT, ::llc::failed(result) , "%s decode overload:%.*s, result:%i." , fileSafe ? "Filesystem-safe" : "Standard", (int)name.size(), name.begin(), result);
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_OVERLOAD_DECODE, bytesMismatch(output.cu8(), binaryU) , "%s decode overload:%.*s, actual:%u, expected:%u." , fileSafe ? "Filesystem-safe" : "Standard", (int)name.size(), name.begin(), output.size(), binaryU.size());
		rtrn 0;
	};
	{
		::llc::au0_t output;
		if_fail_fe(testEncode(binaryU, output, "vcu0_t -> au0_t"));
		output.clear(); if_fail_fe(testEncode(binaryS, output, "vcs0_t -> au0_t"));
		output.clear(); if_fail_fe(testDecode(encodedU, output, "vcu0_t -> au0_t"));
		output.clear(); if_fail_fe(testDecode(encodedS, output, "vcs0_t -> au0_t"));
		output.clear(); if_fail_fe(testDecode(encodedC, output, "vcst_t -> au0_t"));
	}
	{
		::llc::as0_t output;
		if_fail_fe(testEncode(binaryU, output, "vcu0_t -> as0_t"));
		output.clear(); if_fail_fe(testEncode(binaryS, output, "vcs0_t -> as0_t"));
		output.clear(); if_fail_fe(testDecode(encodedU, output, "vcu0_t -> as0_t"));
		output.clear(); if_fail_fe(testDecode(encodedS, output, "vcs0_t -> as0_t"));
	}
	{
		::llc::string output;
		if_fail_fe(testEncode(binaryU, output, "vcu0_t -> string"));
		output.clear(); if_fail_fe(testEncode(binaryS, output, "vcs0_t -> string"));
	}
	{
		::llc::asc_t output;
		if_fail_fe(testDecode(encodedU, output, "vcu0_t -> asc_t"));
		output.clear(); if_fail_fe(testDecode(encodedC, output, "vcst_t -> asc_t"));
	}
	rtrn 0;
}

sttc ::llc::err_t testBase64CountedTerminator(::llc::ATestError & errors) {
	cnst ::llc::u0_t counted[] = {'T', 'Q', '=', '=', 0};
	::llc::au0_t decoded;
	cnst ::llc::err_t result = ::llc::base64Decode({counted}, decoded);
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_COUNTED_TERMINATOR_RESULT, ::llc::failed(result) , "Counted terminator decode result:%i, input:%u.", result, ::llc::size(counted));
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_COUNTED_TERMINATOR, bytesMismatch(decoded, ::llc::vcst_t{"M"}.cu8()) , "Counted terminator output:%u bytes, expected:1.", decoded.size());
	rtrn 0;
}

sttc ::llc::err_t testBase64DecodeRejected(::llc::ATestError & errors, ::llc::vcu0_c & input, BASE64_TEST_RESULT rejection, ::llc::vcst_t name) {
	::llc::au0_t output = {0xA5, 0x5A, 0xC3};
	if_fail_fe(output.reserve(64));
	cnst ::llc::u0_t expected[] = {0xA5, 0x5A, 0xC3};
	cnst ::llc::vcu0_t outputBefore = output;
	cnst ::llc::u2_t capacityBefore = output.Size;
	::llc::err_t decodeResult = 0;
	cnst bool threw = ::llc::testThrows([&]() { decodeResult = ::llc::base64Decode(input, output); });
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_REJECT_THROW, threw , "Invalid decode threw. case:%.*s, input:%u." , (int)name.size(), name.begin(), input.size());
	LLC_TEST_CHECKF(errors, rejection, false == ::llc::failed(decodeResult) , "Invalid decode returned success. case:%.*s, input:%u, result:%i." , (int)name.size(), name.begin(), input.size(), decodeResult);
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_PRESERVE_ADDRESS, output.begin() != outputBefore.begin() , "Rejected decode address. case:%.*s, actual:%p, expected:%p." , (int)name.size(), name.begin(), output.begin(), outputBefore.begin());
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_PRESERVE_COUNT, output.size() != outputBefore.size() , "Rejected decode count. case:%.*s, actual:%u, expected:%u." , (int)name.size(), name.begin(), output.size(), outputBefore.size());
	LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_PRESERVE_CAPACITY, output.Size != capacityBefore , "Rejected decode capacity. case:%.*s, actual:%u, expected:%u." , (int)name.size(), name.begin(), output.Size, capacityBefore);
	if(output.begin() == outputBefore.begin()) {
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_PRESERVE_CONTENT, bytesMismatch(output, {expected}) , "Rejected decode content. case:%.*s, actual:%u bytes." , (int)name.size(), name.begin(), output.size());
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_PRESERVE_TERMINATOR , output.size() > capacityBefore || output.begin()[output.size()] , "Rejected decode terminator. case:%.*s, address:%p, count:%u, capacity:%u." , (int)name.size(), name.begin(), output.begin(), output.size(), capacityBefore);
	}
	rtrn 0;
}

sttc ::llc::err_t testBase64InvalidInput(::llc::ATestError & errors) {
	if_fail_fe(testBase64DecodeRejected(errors, ::llc::vcst_t{"A"}.cu8(), BASE64_TEST_RESULT_INVALID_LENGTH, "one symbol"));
	if_fail_fe(testBase64DecodeRejected(errors, ::llc::vcst_t{"AA"}.cu8(), BASE64_TEST_RESULT_INVALID_LENGTH, "two symbols"));
	if_fail_fe(testBase64DecodeRejected(errors, ::llc::vcst_t{"AAA"}.cu8(), BASE64_TEST_RESULT_INVALID_LENGTH, "three symbols"));
	if_fail_fe(testBase64DecodeRejected(errors, ::llc::vcst_t{"AAAAA"}.cu8(), BASE64_TEST_RESULT_INVALID_LENGTH, "five symbols"));
	cnst ::llc::u0_t countedNull[] = {0};
	if_fail_fe(testBase64DecodeRejected(errors, {countedNull}, BASE64_TEST_RESULT_INVALID_LENGTH, "lone counted null"));

	if_fail_fe(testBase64DecodeRejected(errors, ::llc::vcst_t{"T?=="}.cu8(), BASE64_TEST_RESULT_INVALID_SYMBOL, "question mark"));
	if_fail_fe(testBase64DecodeRejected(errors, ::llc::vcst_t{"!!!!"}.cu8(), BASE64_TEST_RESULT_INVALID_SYMBOL, "punctuation quartet"));
	cnst ::llc::u0_t highSymbol[] = {0xFF, 'A', 'A', 'A'};
	cnst ::llc::u0_t interiorNull[] = {'T', 0, '=', '='};
	if_fail_fe(testBase64DecodeRejected(errors, {highSymbol}, BASE64_TEST_RESULT_INVALID_SYMBOL, "high byte"));
	if_fail_fe(testBase64DecodeRejected(errors, {interiorNull}, BASE64_TEST_RESULT_INVALID_SYMBOL, "interior null"));

	if_fail_fe(testBase64DecodeRejected(errors, ::llc::vcst_t{"=AAA"}.cu8(), BASE64_TEST_RESULT_INVALID_PADDING, "leading pad"));
	if_fail_fe(testBase64DecodeRejected(errors, ::llc::vcst_t{"A=AA"}.cu8(), BASE64_TEST_RESULT_INVALID_PADDING, "second-position pad"));
	if_fail_fe(testBase64DecodeRejected(errors, ::llc::vcst_t{"AA=A"}.cu8(), BASE64_TEST_RESULT_INVALID_PADDING, "third-position pad without final pad"));
	if_fail_fe(testBase64DecodeRejected(errors, ::llc::vcst_t{"A==="}.cu8(), BASE64_TEST_RESULT_INVALID_PADDING, "three pads"));
	if_fail_fe(testBase64DecodeRejected(errors, ::llc::vcst_t{"===="}.cu8(), BASE64_TEST_RESULT_INVALID_PADDING, "four pads"));
	if_fail_fe(testBase64DecodeRejected(errors, ::llc::vcst_t{"TQ=A"}.cu8(), BASE64_TEST_RESULT_INVALID_PADDING, "pad followed by symbol"));
	if_fail_fe(testBase64DecodeRejected(errors, ::llc::vcst_t{"TQ==AAAA"}.cu8(), BASE64_TEST_RESULT_INVALID_PADDING, "padding before final quartet"));

	if_fail_fe(testBase64DecodeRejected(errors, ::llc::vcst_t{"TR=="}.cu8(), BASE64_TEST_RESULT_NONCANONICAL_PADDING, "nonzero four pad bits"));
	rtrn testBase64DecodeRejected(errors, ::llc::vcst_t{"TWF="}.cu8(), BASE64_TEST_RESULT_NONCANONICAL_PADDING, "nonzero two pad bits");
}

sttc ::llc::err_t testBase64AlphabetRejected(::llc::ATestError & errors, ::llc::vcst_t alphabet, char pad, ::llc::vcst_t name) {
	for(::llc::u0_t iOperation = 0; iOperation < 2; ++iOperation) {
		::llc::au0_t output = {0xA5, 0x5A};
		if_fail_fe(output.reserve(64));
		cnst ::llc::u0_t expected[] = {0xA5, 0x5A};
		cnst ::llc::vcu0_t outputBefore = output;
		cnst ::llc::u2_t capacityBefore = output.Size;
		::llc::err_t result = 0;
		cnst bool decode = 0 != iOperation;
		cnst bool threw = ::llc::testThrows([&]() {
			result = decode
				? ::llc::base64Decode(alphabet, pad, ::llc::vcst_t{"TQ=="}.cu8(), output)
				: ::llc::base64Encode(alphabet, pad, ::llc::vcst_t{"M"}.cu8(), output)
				;
			});
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_REJECT_THROW, threw , "Invalid alphabet %s threw. case:%.*s." , decode ? "decode" : "encode", (int)name.size(), name.begin());
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_INVALID_ALPHABET, false == ::llc::failed(result) , "Invalid alphabet %s returned success. case:%.*s, result:%i." , decode ? "decode" : "encode", (int)name.size(), name.begin(), result);
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_PRESERVE_ADDRESS, output.begin() != outputBefore.begin() , "Rejected alphabet %s address. case:%.*s, actual:%p, expected:%p." , decode ? "decode" : "encode", (int)name.size(), name.begin(), output.begin(), outputBefore.begin());
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_PRESERVE_COUNT, output.size() != outputBefore.size() , "Rejected alphabet %s count. case:%.*s, actual:%u, expected:%u." , decode ? "decode" : "encode", (int)name.size(), name.begin(), output.size(), outputBefore.size());
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_PRESERVE_CAPACITY, output.Size != capacityBefore , "Rejected alphabet %s capacity. case:%.*s, actual:%u, expected:%u." , decode ? "decode" : "encode", (int)name.size(), name.begin(), output.Size, capacityBefore);
		if(output.begin() == outputBefore.begin()) {
			LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_PRESERVE_CONTENT, bytesMismatch(output, {expected}) , "Rejected alphabet %s content. case:%.*s, actual:%u bytes." , decode ? "decode" : "encode", (int)name.size(), name.begin(), output.size());
			LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_PRESERVE_TERMINATOR , output.size() > capacityBefore || output.begin()[output.size()] , "Rejected alphabet %s terminator. case:%.*s, address:%p, count:%u, capacity:%u." , decode ? "decode" : "encode", (int)name.size(), name.begin(), output.begin(), output.size(), capacityBefore);
		}
	}
	rtrn 0;
}

sttc ::llc::err_t testBase64InvalidAlphabet(::llc::ATestError & errors) {
	stxp ::llc::vcst_t alphabets[] =
		{ "ABC"
		, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+A"
		, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/A"
		};
	stxp ::llc::vcst_t names[] =
		{ "short"
		, "duplicate"
		, "long"
		};
	for(::llc::u2_t iAlphabet = 0; iAlphabet < ::llc::size(alphabets); ++iAlphabet)
		if_fail_fe(testBase64AlphabetRejected(errors, alphabets[iAlphabet], '=', names[iAlphabet]));
	rtrn testBase64AlphabetRejected(errors, ::llc::b64Symbols, 'A', "pad conflict");
}

sttc ::llc::err_t testBase64FontImage(::llc::vcst_t outputFolder, cnst SBase64FontFixture & font, ::llc::vcu0_t bitmap, ::llc::u2_t index) {
	::llc::string folder;
	if_fail_fe(::llc::pathNameCompose(outputFolder, ::llc::get_value_namev(BASE64_TEST_RESULT_FONT_CONTENT), folder));
	if_fail_fe(::llc::pathCreate(folder));
	::llc::img8bgra image;
	stxp ::llc::u2_t scale = 3;
	if_fail_fe(image.resize(::llc::n2u2_t{font.Width * 16U * scale, font.Height * 16U * scale}));
	for(::llc::u2_t character = 0; character < 256; ++character)
		for(::llc::u2_t y = 0; y < font.Height; ++y)
			for(::llc::u2_t x = 0; x < font.Width; ++x) {
				cnst ::llc::u2_t bit = character * font.Width * font.Height + y * font.Width + x;
				cnst ::llc::u0_t color = (bitmap[bit / 8] & (1U << (bit % 8))) ? 255 : 0;
				for(::llc::u2_t sy = 0; sy < scale; ++sy)
					for(::llc::u2_t sx = 0; sx < scale; ++sx)
						image.View[(character / 16 * font.Height + y) * scale + sy][(character % 16 * font.Width + x) * scale + sx] = {color, color, color, 255};
			}
	::llc::au0_t bytes;
	if_fail_fe(::llc::pngFileWrite(image.View, bytes));
	char name[64] = {};
	if_fail_fe(::llc::sprintf_s(name, "font_%02u_%ux%u.png", index, (::llc::u2_t)font.Width, (::llc::u2_t)font.Height));
	rtrn ::llc::fileFromMemory(folder, ::llc::vcst_t{name}, bytes);
}

sttc ::llc::err_t testBase64Fonts(::llc::ATestError & errors, ::llc::vcst_t outputFolder) {
	stct SLetterRange { ::llc::u0_t Begin, End; };
	static_assert(::llc::size(BASE64_FONT_FIXTURES) == ::llc::size(::llc::RASTER_FONT_DEFAULTS));
	stxp SLetterRange LETTER_RANGES[] = {{'A', 'Z'}, {'a', 'z'}};
	for(::llc::u2_t iFont = 0; iFont < ::llc::size(BASE64_FONT_FIXTURES); ++iFont) {
		cnst SBase64FontFixture & font = BASE64_FONT_FIXTURES[iFont];
		cnst auto & source = ::llc::RASTER_FONT_DEFAULTS[iFont];
		LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_FONT_SIZE, source.CharSize.x != font.Width || source.CharSize.y != font.Height);
		::llc::au0_t decoded;
		cnst ::llc::err_t decodeResult = ::llc::base64Decode(source.Base64String, decoded);
		LLC_TEST_REQUIREF(errors, BASE64_TEST_RESULT_FONT_DECODE, ::llc::failed(decodeResult) , "CP437 font decode failed. font:%u, size:%ux%u, input:%u, result:%i." , iFont, font.Width, font.Height, source.Base64String.size(), decodeResult );
		::llc::u2_c logicalByteCount = font.Width * font.Height * 256U / 8U;
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_FONT_SIZE, decoded.size() != font.DecodedCount , "CP437 font:%u decoded bytes:%u, expected:%u." , iFont, decoded.size(), font.DecodedCount);
		LLC_TEST_REQUIREF(errors, BASE64_TEST_RESULT_FONT_LOGICAL_SIZE, logicalByteCount > decoded.size() , "CP437 font:%u logical bytes:%u, available:%u." , iFont, logicalByteCount, decoded.size());
		if_fail_fe(testBase64FontImage(outputFolder, font, decoded, iFont));
		cnst ::llc::u2_t decodedHash = hashBytes(decoded);
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_FONT_CONTENT, decodedHash != font.DecodedHash , "CP437 font bitmap mismatch. font:%u, size:%ux%u, actual:0x%08X, expected:0x%08X." , iFont, font.Width, font.Height, decodedHash, font.DecodedHash );

		::llc::au0_t encoded;
		cnst ::llc::err_t encodeResult = ::llc::base64Encode(decoded, encoded);
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_FONT_ENCODE_RESULT, ::llc::failed(encodeResult) , "CP437 font:%u encode result:%i.", iFont, encodeResult);
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_FONT_ROUND_TRIP, bytesMismatch(encoded, source.Base64String.cu8()) , "CP437 font:%u encoded bytes:%u, expected:%u." , iFont, encoded.size(), source.Base64String.size());

		::llc::u2_t letterHash = 2166136261U;
		for(::llc::u2_t iRange = 0; iRange < ::llc::size(LETTER_RANGES); ++iRange)
			for(::llc::u0_t character = LETTER_RANGES[iRange].Begin; character <= LETTER_RANGES[iRange].End; ++character)
				letterHash = drawBase64FontLetter(font, decoded, character, letterHash, iFont);
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_FONT_ASCII_DRAW, letterHash != font.LetterHash , "CP437 font ASCII drawing mismatch. font:%u, size:%ux%u, actual:0x%08X, expected:0x%08X." , iFont, font.Width, font.Height, letterHash, font.LetterHash );
	}
	rtrn 0;
}

sttc ::llc::err_t testBase64Random(::llc::ATestError & errors, bool fileSafe) {
	stxp ::llc::u3_t SEED = 0xB6405EEDULL;
	cnst ::llc::u3_t seed = ::llc::testSeed(SEED);
	::llc::SPRNG random = {seed};
	::llc::au0_t binary, encoded, decoded;
	for(::llc::u2_t byteCount = 1; byteCount <= 256; ++byteCount) {
		if_fail_fe(binary.resize(byteCount));
		for(::llc::u2_t iByte = 0; iByte < byteCount; ++iByte)
			binary[iByte] = (::llc::u0_t)random.Next();
		encoded.clear();
		cnst ::llc::err_t encodeResult = fileSafe ? ::llc::base64EncodeFS(binary, encoded) : ::llc::base64Encode(binary, encoded);
		LLC_TEST_REQUIREF(errors, BASE64_TEST_RESULT_RANDOM_ENCODE, ::llc::failed(encodeResult) , "Generated encode failed. alphabet:%s, seed:%" LLC_FMT_U3 ", bytes:%u, result:%i." , fileSafe ? "filesystem" : "standard", seed, byteCount, encodeResult );
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_ENCODE_SIZE, encoded.size() != ((byteCount + 2) / 3) * 4 , "Generated encoded size mismatch. alphabet:%s, seed:%" LLC_FMT_U3 ", bytes:%u, actual:%u, expected:%u." , fileSafe ? "filesystem" : "standard", seed, byteCount, encoded.size(), ((byteCount + 2) / 3) * 4 );
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_TERMINATOR, encoded.begin()[encoded.size()] , "Generated encoded terminator mismatch. alphabet:%s, seed:%" LLC_FMT_U3 ", bytes:%u, terminator:0x%02X." , fileSafe ? "filesystem" : "standard", seed, byteCount, encoded.begin()[encoded.size()] );
		decoded.clear();
		cnst ::llc::err_t decodeResult = fileSafe ? ::llc::base64DecodeFS(encoded, decoded) : ::llc::base64Decode(encoded, decoded);
		LLC_TEST_REQUIREF(errors, BASE64_TEST_RESULT_RANDOM_DECODE, ::llc::failed(decodeResult) , "Generated decode failed. alphabet:%s, seed:%" LLC_FMT_U3 ", bytes:%u, encoded:%u, result:%i." , fileSafe ? "filesystem" : "standard", seed, byteCount, encoded.size(), decodeResult );
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_DECODE_SIZE, decoded.size() != byteCount , "Generated decoded size mismatch. alphabet:%s, seed:%" LLC_FMT_U3 ", actual:%u, expected:%u." , fileSafe ? "filesystem" : "standard", seed, decoded.size(), byteCount );
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_RANDOM_ROUND_TRIP, bytesMismatch(decoded, binary) , "Generated round trip mismatch. alphabet:%s, seed:%" LLC_FMT_U3 ", bytes:%u, position:%" LLC_FMT_U3 "." , fileSafe ? "filesystem" : "standard", seed, byteCount, random.Position );
		LLC_TEST_CHECKF(errors, BASE64_TEST_RESULT_TERMINATOR, decoded.begin()[decoded.size()] , "Generated decoded terminator mismatch. alphabet:%s, seed:%" LLC_FMT_U3 ", bytes:%u, terminator:0x%02X." , fileSafe ? "filesystem" : "standard", seed, byteCount, decoded.begin()[decoded.size()] );
	}
	rtrn 0;
}

::llc::err_t testBase64(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	if_fail_fe(testBase64Vectors			(errors));
	if_fail_fe(testBase64Append			(errors));
	if_fail_fe(testBase64Alphabets		(errors));
	if_fail_fe(testBase64Overloads		(errors, false));
	if_fail_fe(testBase64Overloads		(errors, true));
	if_fail_fe(testBase64CountedTerminator(errors));
	if_fail_fe(testBase64InvalidInput		(errors));
	if_fail_fe(testBase64InvalidAlphabet	(errors));
	if_fail_fe(testBase64Fonts				(errors, outputFolder));
	if_fail_fe(testBase64Random			(errors, false));
	rtrn testBase64Random				(errors, true);
}
