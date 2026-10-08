#include "llc_base64.h"
#include "llc_noise.h"

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
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, FAILURE_PRESERVATION		, 21, "A rejected Base64 operation changed the output allocation, count, contents or terminator.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, STATIC_SYMBOL_MAP			, 22, "A constexpr Base64 symbol map produced the wrong encoded or decoded value.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, FONT_DECODE				, 23, "A field-tested CP437 font failed to decode.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, FONT_SIZE					, 24, "A decoded CP437 font produced the wrong storage size.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, FONT_CONTENT				, 25, "A decoded CP437 font did not preserve its known bitmap contents.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, FONT_ROUND_TRIP			, 26, "A decoded CP437 font did not encode back to its original Base64 text.");
GDEFINE_ENUM_VALUED(BASE64_TEST_RESULT, FONT_ASCII_DRAW			, 27, "The CP437 font bits did not draw the known ASCII letters correctly.");

tplt<tpnm T>
stin ::llc::vcu0_t byteView(cnst T & value) { rtrn value.cu8(); }

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
	::llc::vcsc_t	Binary;
	::llc::vcst_t	Encoded;
};

stxp ::llc::vcst_t			BASE64_REVERSED_SYMBOLS	= LLC_CXS("/+9876543210zyxwvutsrqponmlkjihgfedcbaZYXWVUTSRQPONMLKJIHGFEDCBA");
stxp ::llc::SBase64SymbolMap	BASE64_REVERSED_MAP		= ::llc::base64SymbolMap(BASE64_REVERSED_SYMBOLS, '*');
stxp ::llc::SBase64SymbolMap	BASE64_SHORT_MAP			= ::llc::base64SymbolMap(LLC_CXS("ABC"));
stxp ::llc::SBase64SymbolMap	BASE64_PAD_CONFLICT_MAP	= ::llc::base64SymbolMap(::llc::b64Symbols, 'A');
stxp ::llc::SBase64SymbolMap	BASE64_DUPLICATE_MAP		= ::llc::base64SymbolMap(LLC_CXS("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+A"));
static_assert( 0 == BASE64_REVERSED_MAP.Error);
static_assert(-1 == BASE64_SHORT_MAP.Error);
static_assert(-2 == BASE64_PAD_CONFLICT_MAP.Error);
static_assert(-3 == BASE64_DUPLICATE_MAP.Error);
static_assert( 0 == BASE64_REVERSED_MAP.Decode.Storage[(::llc::u0_t)'/']);
static_assert(63 == BASE64_REVERSED_MAP.Decode.Storage[(::llc::u0_t)'A']);

sttc ::llc::err_t testBase64Vectors(ATestError & errors) {
	stxp SBase64Vector vectors[] =
		{ {LLC_CXS("")		, LLC_CXS("")}
		, {LLC_CXS("f")		, LLC_CXS("Zg==")}
		, {LLC_CXS("fo")		, LLC_CXS("Zm8=")}
		, {LLC_CXS("foo")	, LLC_CXS("Zm9v")}
		, {LLC_CXS("foob")	, LLC_CXS("Zm9vYg==")}
		, {LLC_CXS("fooba")	, LLC_CXS("Zm9vYmE=")}
		, {LLC_CXS("foobar")	, LLC_CXS("Zm9vYmFy")}
		};
	for(::llc::u2_t iVector = 0; iVector < ::llc::size(vectors); ++iVector) {
		cnst SBase64Vector & vector = vectors[iVector];
		::llc::au0_t encoded;
		cnst ::llc::err_t encodeResult = ::llc::base64Encode(vector.Binary, encoded);
		LLC_TEST_REQUIRE(errors, BASE64_TEST_RESULT_ENCODE_VECTOR, ::llc::failed(encodeResult)
			, "Reference encode failed. vector:%u, input:%u, result:%i."
			, iVector, vector.Binary.size(), encodeResult
			);
		LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_ENCODE_SIZE, encoded.size() != vector.Encoded.size()
			, "Encoded size mismatch. vector:%u, input:%u, actual:%u, expected:%u."
			, iVector, vector.Binary.size(), encoded.size(), vector.Encoded.size()
			);
		LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_ENCODE_VECTOR, bytesMismatch(byteView(encoded), byteView(vector.Encoded))
			, "Encoded value mismatch. vector:%u, input:'%.*s', actual:'%.*s', expected:'%.*s'."
			, iVector, (int)vector.Binary.size(), vector.Binary.begin(), (int)encoded.size(), encoded.begin(), (int)vector.Encoded.size(), vector.Encoded.begin()
			);
		if(encoded.size())
			LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_TERMINATOR, encoded.begin()[encoded.size()]
				, "Encoded terminator mismatch. vector:%u, size:%u, terminator:0x%02X."
				, iVector, encoded.size(), encoded.begin()[encoded.size()]
				);

		::llc::au0_t decoded;
		cnst ::llc::err_t decodeResult = ::llc::base64Decode(vector.Encoded, decoded);
		LLC_TEST_REQUIRE(errors, BASE64_TEST_RESULT_DECODE_VECTOR, ::llc::failed(decodeResult)
			, "Reference decode failed. vector:%u, input:%u, result:%i."
			, iVector, vector.Encoded.size(), decodeResult
			);
		LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_DECODE_SIZE, decoded.size() != vector.Binary.size()
			, "Decoded size mismatch. vector:%u, input:%u, actual:%u, expected:%u."
			, iVector, vector.Encoded.size(), decoded.size(), vector.Binary.size()
			);
		LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_DECODE_VECTOR, bytesMismatch(byteView(decoded), byteView(vector.Binary))
			, "Decoded value mismatch. vector:%u, encoded:'%.*s', actual:%u bytes, expected:%u bytes."
			, iVector, (int)vector.Encoded.size(), vector.Encoded.begin(), decoded.size(), vector.Binary.size()
			);
		if(decoded.size())
			LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_TERMINATOR, decoded.begin()[decoded.size()]
				, "Decoded terminator mismatch. vector:%u, size:%u, terminator:0x%02X."
				, iVector, decoded.size(), decoded.begin()[decoded.size()]
				);
	}
	rtrn 0;
}

sttc ::llc::err_t testBase64Append(ATestError & errors) {
	::llc::au0_t encoded = {'p', 'r', 'e', ':'};
	cnst ::llc::u0_t expectedEncoded[] = {'p', 'r', 'e', ':', 'T', 'Q', '=', '='};
	cnst ::llc::err_t encodeResult = ::llc::base64Encode(LLC_CXS("M"), encoded);
	LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_ENCODE_APPEND, ::llc::failed(encodeResult) || bytesMismatch(encoded, {expectedEncoded})
		, "Encode append mismatch. result:%i, actual:%u, expected:%u."
		, encodeResult, encoded.size(), ::llc::size(expectedEncoded)
		);
	LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_TERMINATOR, encoded.begin()[encoded.size()]
		, "Encode append terminator mismatch. size:%u, terminator:0x%02X."
		, encoded.size(), encoded.begin()[encoded.size()]
		);

	::llc::au0_t decoded = {0xA5, 0x5A};
	cnst ::llc::u0_t expectedDecoded[] = {0xA5, 0x5A, 'M'};
	cnst ::llc::err_t decodeResult = ::llc::base64Decode(LLC_CXS("TQ=="), decoded);
	LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_DECODE_APPEND, ::llc::failed(decodeResult) || bytesMismatch(decoded, {expectedDecoded})
		, "Decode append mismatch. result:%i, actual:%u, expected:%u."
		, decodeResult, decoded.size(), ::llc::size(expectedDecoded)
		);
	LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_TERMINATOR, decoded.begin()[decoded.size()]
		, "Decode append terminator mismatch. size:%u, terminator:0x%02X."
		, decoded.size(), decoded.begin()[decoded.size()]
		);

	cnst ::llc::u0_t encodedBefore[] = {'p', 'r', 'e', ':', 'T', 'Q', '=', '='};
	cnst ::llc::u0_t decodedBefore[] = {0xA5, 0x5A, 'M'};
	if_fail_fe(::llc::base64Encode(LLC_CXS(""), encoded));
	if_fail_fe(::llc::base64Decode(LLC_CXS(""), decoded));
	LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_ENCODE_APPEND, bytesMismatch(encoded, {encodedBefore})
		, "Empty encode changed existing output. actual:%u, expected:%u."
		, encoded.size(), ::llc::size(encodedBefore)
		);
	LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_DECODE_APPEND, bytesMismatch(decoded, {decodedBefore})
		, "Empty decode changed existing output. actual:%u, expected:%u."
		, decoded.size(), ::llc::size(decodedBefore)
		);
	rtrn 0;
}

sttc ::llc::err_t testBase64Alphabets(ATestError & errors) {
	cnst ::llc::u0_t binary[] = {0xFB, 0xFF};
	::llc::au0_t encoded;
	cnst ::llc::err_t encodeResult = ::llc::base64EncodeFS({binary}, encoded);
	LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_FILESYSTEM_ALPHABET, ::llc::failed(encodeResult) || bytesMismatch(encoded, byteView(LLC_CXS("-_8=")))
		, "Filesystem-safe encode mismatch. result:%i, actual:'%.*s', expected:'-_8='."
		, encodeResult, (int)encoded.size(), encoded.begin()
		);
	::llc::au0_t decoded;
	cnst ::llc::err_t decodeResult = ::llc::base64DecodeFS(encoded, decoded);
	LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_FILESYSTEM_ALPHABET, ::llc::failed(decodeResult) || bytesMismatch(decoded, {binary})
		, "Filesystem-safe decode mismatch. result:%i, actual:%u bytes, expected:%u bytes."
		, decodeResult, decoded.size(), ::llc::size(binary)
		);

	cnst ::llc::u0_t zero[] = {0};
	encoded.clear();
	cnst ::llc::err_t customEncode = ::llc::base64Encode(BASE64_REVERSED_SYMBOLS, '*', {zero}, encoded);
	LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_CUSTOM_ALPHABET, ::llc::failed(customEncode) || bytesMismatch(encoded, byteView(LLC_CXS("//**")))
		, "Custom encode mismatch. result:%i, actual:'%.*s', expected:'//**'."
		, customEncode, (int)encoded.size(), encoded.begin()
		);
	decoded.clear();
	cnst ::llc::err_t customDecode = ::llc::base64Decode(BASE64_REVERSED_SYMBOLS, '*', encoded, decoded);
	LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_CUSTOM_ALPHABET, ::llc::failed(customDecode) || bytesMismatch(decoded, {zero})
		, "Custom decode mismatch. result:%i, actual:%u bytes, expected one zero byte."
		, customDecode, decoded.size()
		);

	encoded.clear();
	cnst ::llc::err_t staticEncode = ::llc::base64Encode(BASE64_REVERSED_MAP, {zero}, encoded);
	LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_STATIC_SYMBOL_MAP, ::llc::failed(staticEncode) || bytesMismatch(encoded, byteView(LLC_CXS("//**")))
		, "Static-map encode mismatch. result:%i, actual:'%.*s', expected:'//**'."
		, staticEncode, (int)encoded.size(), encoded.begin()
		);
	decoded.clear();
	cnst ::llc::err_t staticDecode = ::llc::base64Decode(BASE64_REVERSED_MAP, encoded, decoded);
	LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_STATIC_SYMBOL_MAP, ::llc::failed(staticDecode) || bytesMismatch(decoded, {zero})
		, "Static-map decode mismatch. result:%i, actual:%u bytes, expected one zero byte."
		, staticDecode, decoded.size()
		);
	rtrn 0;
}

sttc ::llc::err_t testBase64Overloads(ATestError & errors, bool fileSafe) {
	cnst ::llc::u0_t binary[] = {'M', 'a', 'n'};
	cnst ::llc::u0_t encodedBytes[] = {'T', 'W', 'F', 'u'};
	cnst ::llc::s0_t binarySigned[] = {'M', 'a', 'n'};
	cnst ::llc::s0_t encodedSigned[] = {'T', 'W', 'F', 'u'};
	cnst ::llc::sc_t binaryChars[] = {'M', 'a', 'n'};
	cnst ::llc::sc_t encodedChars[] = {'T', 'W', 'F', 'u'};
	cnst ::llc::vcu0_t binaryU = {binary};
	cnst ::llc::vcs0_t binaryS = {binarySigned};
	cnst ::llc::vcsc_t binaryC = {binaryChars};
	cnst ::llc::vcu0_t encodedU = {encodedBytes};
	cnst ::llc::vcs0_t encodedS = {encodedSigned};
	cnst ::llc::vcst_t encodedC = {encodedChars};
	auto testEncode = [&](auto input, auto & output, ::llc::vcst_t name) -> ::llc::err_t {
		cnst ::llc::err_t result = fileSafe ? ::llc::base64EncodeFS(input, output) : ::llc::base64Encode(input, output);
		LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_OVERLOAD_ENCODE, ::llc::failed(result) || bytesMismatch(byteView(output), encodedU)
			, "%s encode overload mismatch. overload:%.*s, result:%i, actual:%u, expected:%u."
			, fileSafe ? "Filesystem-safe" : "Standard", (int)name.size(), name.begin(), result, output.size(), encodedU.size()
			);
		rtrn 0;
	};
	auto testDecode = [&](auto input, auto & output, ::llc::vcst_t name) -> ::llc::err_t {
		cnst ::llc::err_t result = fileSafe ? ::llc::base64DecodeFS(input, output) : ::llc::base64Decode(input, output);
		LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_OVERLOAD_DECODE, ::llc::failed(result) || bytesMismatch(byteView(output), binaryU)
			, "%s decode overload mismatch. overload:%.*s, result:%i, actual:%u, expected:%u."
			, fileSafe ? "Filesystem-safe" : "Standard", (int)name.size(), name.begin(), result, output.size(), binaryU.size()
			);
		rtrn 0;
	};
	{
		::llc::au0_t output;
		if_fail_fe(testEncode(binaryU, output, LLC_CXS("vcu0_t -> au0_t")));
		output.clear(); if_fail_fe(testEncode(binaryS, output, LLC_CXS("vcs0_t -> au0_t")));
		output.clear(); if_fail_fe(testEncode(binaryC, output, LLC_CXS("vcsc_t -> au0_t")));
		output.clear(); if_fail_fe(testDecode(encodedU, output, LLC_CXS("vcu0_t -> au0_t")));
		output.clear(); if_fail_fe(testDecode(encodedS, output, LLC_CXS("vcs0_t -> au0_t")));
		output.clear(); if_fail_fe(testDecode(encodedC, output, LLC_CXS("vcsc_t -> au0_t")));
	}
	{
		::llc::as0_t output;
		if_fail_fe(testEncode(binaryU, output, LLC_CXS("vcu0_t -> as0_t")));
		output.clear(); if_fail_fe(testEncode(binaryS, output, LLC_CXS("vcs0_t -> as0_t")));
		output.clear(); if_fail_fe(testDecode(encodedU, output, LLC_CXS("vcu0_t -> as0_t")));
		output.clear(); if_fail_fe(testDecode(encodedS, output, LLC_CXS("vcs0_t -> as0_t")));
	}
	{
		::llc::string output;
		if_fail_fe(testEncode(binaryU, output, LLC_CXS("vcu0_t -> string")));
		output.clear(); if_fail_fe(testEncode(binaryC, output, LLC_CXS("vcsc_t -> string")));
	}
	{
		::llc::asc_t output;
		if_fail_fe(testDecode(encodedU, output, LLC_CXS("vcu0_t -> asc_t")));
		output.clear(); if_fail_fe(testDecode(encodedC, output, LLC_CXS("vcst_t -> asc_t")));
	}
	rtrn 0;
}

sttc ::llc::err_t testBase64CountedTerminator(ATestError & errors) {
	cnst ::llc::u0_t counted[] = {'T', 'Q', '=', '=', 0};
	::llc::au0_t decoded;
	cnst ::llc::err_t result = ::llc::base64Decode({counted}, decoded);
	LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_COUNTED_TERMINATOR, ::llc::failed(result) || bytesMismatch(decoded, byteView(LLC_CXS("M")))
		, "Counted terminator decode mismatch. result:%i, input:%u, output:%u."
		, result, ::llc::size(counted), decoded.size()
		);
	rtrn 0;
}

sttc ::llc::err_t testBase64DecodeRejected(ATestError & errors, ::llc::vcu0_c & input, BASE64_TEST_RESULT rejection, ::llc::vcst_t name) {
	::llc::au0_t output = {0xA5, 0x5A, 0xC3};
	if_fail_fe(output.reserve(64));
	cnst ::llc::u0_t expected[] = {0xA5, 0x5A, 0xC3};
	cnst ::llc::vcu0_t outputBefore = output;
	cnst ::llc::u2_t capacityBefore = output.Size;
	::llc::err_t decodeResult = 0;
	cnst bool threw = testThrows([&]() { decodeResult = ::llc::base64Decode(input, output); });
	LLC_TEST_CHECK(errors, rejection, threw || false == ::llc::failed(decodeResult)
		, "Invalid decode was not rejected safely. case:%.*s, input:%u, result:%i, threw:%u."
		, (int)name.size(), name.begin(), input.size(), decodeResult, (::llc::u2_t)threw
		);
	LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_FAILURE_PRESERVATION
		, output.begin() != outputBefore.begin() || output.size() != outputBefore.size() || output.Size != capacityBefore
		|| bytesMismatch(output, {expected}) || output.begin()[output.size()]
		, "Rejected decode changed output. case:%.*s, address:%p/%p, count:%u/%u, capacity:%u/%u, terminator:0x%02X."
		, (int)name.size(), name.begin(), output.begin(), outputBefore.begin()
		, output.size(), outputBefore.size(), output.Size, capacityBefore, output.begin()[output.size()]
		);
	rtrn 0;
}

sttc ::llc::err_t testBase64InvalidInput(ATestError & errors) {
	if_fail_fe(testBase64DecodeRejected(errors, byteView(LLC_CXS("A")), BASE64_TEST_RESULT_INVALID_LENGTH, LLC_CXS("one symbol")));
	if_fail_fe(testBase64DecodeRejected(errors, byteView(LLC_CXS("AA")), BASE64_TEST_RESULT_INVALID_LENGTH, LLC_CXS("two symbols")));
	if_fail_fe(testBase64DecodeRejected(errors, byteView(LLC_CXS("AAA")), BASE64_TEST_RESULT_INVALID_LENGTH, LLC_CXS("three symbols")));
	if_fail_fe(testBase64DecodeRejected(errors, byteView(LLC_CXS("AAAAA")), BASE64_TEST_RESULT_INVALID_LENGTH, LLC_CXS("five symbols")));
	cnst ::llc::u0_t countedNull[] = {0};
	if_fail_fe(testBase64DecodeRejected(errors, {countedNull}, BASE64_TEST_RESULT_INVALID_LENGTH, LLC_CXS("lone counted null")));

	if_fail_fe(testBase64DecodeRejected(errors, byteView(LLC_CXS("T?==")), BASE64_TEST_RESULT_INVALID_SYMBOL, LLC_CXS("question mark")));
	if_fail_fe(testBase64DecodeRejected(errors, byteView(LLC_CXS("!!!!")), BASE64_TEST_RESULT_INVALID_SYMBOL, LLC_CXS("punctuation quartet")));
	cnst ::llc::u0_t highSymbol[] = {0xFF, 'A', 'A', 'A'};
	cnst ::llc::u0_t interiorNull[] = {'T', 0, '=', '='};
	if_fail_fe(testBase64DecodeRejected(errors, {highSymbol}, BASE64_TEST_RESULT_INVALID_SYMBOL, LLC_CXS("high byte")));
	if_fail_fe(testBase64DecodeRejected(errors, {interiorNull}, BASE64_TEST_RESULT_INVALID_SYMBOL, LLC_CXS("interior null")));

	if_fail_fe(testBase64DecodeRejected(errors, byteView(LLC_CXS("=AAA")), BASE64_TEST_RESULT_INVALID_PADDING, LLC_CXS("leading pad")));
	if_fail_fe(testBase64DecodeRejected(errors, byteView(LLC_CXS("A=AA")), BASE64_TEST_RESULT_INVALID_PADDING, LLC_CXS("second-position pad")));
	if_fail_fe(testBase64DecodeRejected(errors, byteView(LLC_CXS("AA=A")), BASE64_TEST_RESULT_INVALID_PADDING, LLC_CXS("third-position pad without final pad")));
	if_fail_fe(testBase64DecodeRejected(errors, byteView(LLC_CXS("A===")), BASE64_TEST_RESULT_INVALID_PADDING, LLC_CXS("three pads")));
	if_fail_fe(testBase64DecodeRejected(errors, byteView(LLC_CXS("====")), BASE64_TEST_RESULT_INVALID_PADDING, LLC_CXS("four pads")));
	if_fail_fe(testBase64DecodeRejected(errors, byteView(LLC_CXS("TQ=A")), BASE64_TEST_RESULT_INVALID_PADDING, LLC_CXS("pad followed by symbol")));
	if_fail_fe(testBase64DecodeRejected(errors, byteView(LLC_CXS("TQ==AAAA")), BASE64_TEST_RESULT_INVALID_PADDING, LLC_CXS("padding before final quartet")));

	if_fail_fe(testBase64DecodeRejected(errors, byteView(LLC_CXS("TR==")), BASE64_TEST_RESULT_NONCANONICAL_PADDING, LLC_CXS("nonzero four pad bits")));
	rtrn testBase64DecodeRejected(errors, byteView(LLC_CXS("TWF=")), BASE64_TEST_RESULT_NONCANONICAL_PADDING, LLC_CXS("nonzero two pad bits"));
}

sttc ::llc::err_t testBase64AlphabetRejected(ATestError & errors, ::llc::vcst_t alphabet, char pad, ::llc::vcst_t name) {
	for(::llc::u0_t iOperation = 0; iOperation < 2; ++iOperation) {
		::llc::au0_t output = {0xA5, 0x5A};
		if_fail_fe(output.reserve(64));
		cnst ::llc::u0_t expected[] = {0xA5, 0x5A};
		cnst ::llc::vcu0_t outputBefore = output;
		cnst ::llc::u2_t capacityBefore = output.Size;
		::llc::err_t result = 0;
		cnst bool decode = 0 != iOperation;
		cnst bool threw = testThrows([&]() {
			result = decode
				? ::llc::base64Decode(alphabet, pad, byteView(LLC_CXS("TQ==")), output)
				: ::llc::base64Encode(alphabet, pad, byteView(LLC_CXS("M"   )), output)
				;
			});
		LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_INVALID_ALPHABET, threw || false == ::llc::failed(result)
			, "Invalid alphabet %s was not rejected safely. case:%.*s, symbols:%u, result:%i, threw:%u."
			, decode ? "decode" : "encode", (int)name.size(), name.begin(), alphabet.size(), result, (::llc::u2_t)threw
			);
		LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_FAILURE_PRESERVATION
			, output.begin() != outputBefore.begin() || output.size() != outputBefore.size() || output.Size != capacityBefore
			|| bytesMismatch(output, {expected}) || output.begin()[output.size()]
			, "Rejected alphabet %s changed output. case:%.*s, address:%p/%p, count:%u/%u, capacity:%u/%u."
			, decode ? "decode" : "encode", (int)name.size(), name.begin(), output.begin(), outputBefore.begin()
			, output.size(), outputBefore.size(), output.Size, capacityBefore
			);
	}
	rtrn 0;
}

sttc ::llc::err_t testBase64InvalidAlphabet(ATestError & errors) {
	stxp ::llc::vcst_t alphabets[] =
		{ LLC_CXS("ABC")
		, LLC_CXS("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+A")
		, LLC_CXS("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/A")
		};
	stxp ::llc::vcst_t names[] =
		{ LLC_CXS("short")
		, LLC_CXS("duplicate")
		, LLC_CXS("long")
		};
	for(::llc::u2_t iAlphabet = 0; iAlphabet < ::llc::size(alphabets); ++iAlphabet)
		if_fail_fe(testBase64AlphabetRejected(errors, alphabets[iAlphabet], '=', names[iAlphabet]));
	rtrn testBase64AlphabetRejected(errors, ::llc::b64Symbols, 'A', LLC_CXS("pad conflict"));
}

sttc ::llc::err_t testBase64Fonts(ATestError & errors) {
	stct SLetterRange { ::llc::u0_t Begin, End; };
	stxp SLetterRange LETTER_RANGES[] = {{'A', 'Z'}, {'a', 'z'}};
	for(::llc::u2_t iFont = 0; iFont < ::llc::size(BASE64_FONT_FIXTURES); ++iFont) {
		cnst SBase64FontFixture & font = BASE64_FONT_FIXTURES[iFont];
		::llc::au0_t decoded;
		cnst ::llc::err_t decodeResult = ::llc::base64Decode(font.Encoded, decoded);
		LLC_TEST_REQUIRE(errors, BASE64_TEST_RESULT_FONT_DECODE, ::llc::failed(decodeResult)
			, "CP437 font decode failed. font:%u, size:%ux%u, input:%u, result:%i."
			, iFont, font.Width, font.Height, font.Encoded.size(), decodeResult
			);
		::llc::u2_c logicalByteCount = font.Width * font.Height * 256U / 8U;
		LLC_TEST_REQUIRE(errors, BASE64_TEST_RESULT_FONT_SIZE, decoded.size() != font.DecodedCount || logicalByteCount > decoded.size()
			, "CP437 font decoded size mismatch. font:%u, size:%ux%u, actual:%u, expected:%u, logical:%u."
			, iFont, font.Width, font.Height, decoded.size(), font.DecodedCount, logicalByteCount
			);
		cnst ::llc::u2_t decodedHash = hashBytes(decoded);
		LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_FONT_CONTENT, decodedHash != font.DecodedHash
			, "CP437 font bitmap mismatch. font:%u, size:%ux%u, actual:0x%08X, expected:0x%08X."
			, iFont, font.Width, font.Height, decodedHash, font.DecodedHash
			);

		::llc::au0_t encoded;
		cnst ::llc::err_t encodeResult = ::llc::base64Encode(decoded, encoded);
		LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_FONT_ROUND_TRIP
			, ::llc::failed(encodeResult) || bytesMismatch(encoded, byteView(font.Encoded))
			, "CP437 font round trip mismatch. font:%u, size:%ux%u, result:%i, actual:%u, expected:%u."
			, iFont, font.Width, font.Height, encodeResult, encoded.size(), font.Encoded.size()
			);

		::llc::u2_t letterHash = 2166136261U;
		for(::llc::u2_t iRange = 0; iRange < ::llc::size(LETTER_RANGES); ++iRange)
			for(::llc::u0_t character = LETTER_RANGES[iRange].Begin; character <= LETTER_RANGES[iRange].End; ++character)
				letterHash = drawBase64FontLetter(font, decoded, character, letterHash, iFont);
		LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_FONT_ASCII_DRAW, letterHash != font.LetterHash
			, "CP437 font ASCII drawing mismatch. font:%u, size:%ux%u, actual:0x%08X, expected:0x%08X."
			, iFont, font.Width, font.Height, letterHash, font.LetterHash
			);
	}
	rtrn 0;
}

sttc ::llc::err_t testBase64Random(ATestError & errors, bool fileSafe) {
	stxp ::llc::u3_t SEED = 0xB6405EEDULL;
	::llc::SPRNG random = {SEED};
	::llc::au0_t binary, encoded, decoded;
	for(::llc::u2_t byteCount = 1; byteCount <= 256; ++byteCount) {
		if_fail_fe(binary.resize(byteCount));
		for(::llc::u2_t iByte = 0; iByte < byteCount; ++iByte)
			binary[iByte] = (::llc::u0_t)random.Next();
		encoded.clear();
		cnst ::llc::err_t encodeResult = fileSafe ? ::llc::base64EncodeFS(binary, encoded) : ::llc::base64Encode(binary, encoded);
		LLC_TEST_REQUIRE(errors, BASE64_TEST_RESULT_RANDOM_ENCODE, ::llc::failed(encodeResult)
			, "Generated encode failed. alphabet:%s, seed:%" LLC_FMT_U3 ", bytes:%u, result:%i."
			, fileSafe ? "filesystem" : "standard", SEED, byteCount, encodeResult
			);
		LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_ENCODE_SIZE, encoded.size() != ((byteCount + 2) / 3) * 4
			, "Generated encoded size mismatch. alphabet:%s, seed:%" LLC_FMT_U3 ", bytes:%u, actual:%u, expected:%u."
			, fileSafe ? "filesystem" : "standard", SEED, byteCount, encoded.size(), ((byteCount + 2) / 3) * 4
			);
		LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_TERMINATOR, encoded.begin()[encoded.size()]
			, "Generated encoded terminator mismatch. alphabet:%s, seed:%" LLC_FMT_U3 ", bytes:%u, terminator:0x%02X."
			, fileSafe ? "filesystem" : "standard", SEED, byteCount, encoded.begin()[encoded.size()]
			);
		decoded.clear();
		cnst ::llc::err_t decodeResult = fileSafe ? ::llc::base64DecodeFS(encoded, decoded) : ::llc::base64Decode(encoded, decoded);
		LLC_TEST_REQUIRE(errors, BASE64_TEST_RESULT_RANDOM_DECODE, ::llc::failed(decodeResult)
			, "Generated decode failed. alphabet:%s, seed:%" LLC_FMT_U3 ", bytes:%u, encoded:%u, result:%i."
			, fileSafe ? "filesystem" : "standard", SEED, byteCount, encoded.size(), decodeResult
			);
		LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_DECODE_SIZE, decoded.size() != byteCount
			, "Generated decoded size mismatch. alphabet:%s, seed:%" LLC_FMT_U3 ", actual:%u, expected:%u."
			, fileSafe ? "filesystem" : "standard", SEED, decoded.size(), byteCount
			);
		LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_RANDOM_ROUND_TRIP, bytesMismatch(decoded, binary)
			, "Generated round trip mismatch. alphabet:%s, seed:%" LLC_FMT_U3 ", bytes:%u, position:%" LLC_FMT_U3 "."
			, fileSafe ? "filesystem" : "standard", SEED, byteCount, random.Position
			);
		LLC_TEST_CHECK(errors, BASE64_TEST_RESULT_TERMINATOR, decoded.begin()[decoded.size()]
			, "Generated decoded terminator mismatch. alphabet:%s, seed:%" LLC_FMT_U3 ", bytes:%u, terminator:0x%02X."
			, fileSafe ? "filesystem" : "standard", SEED, byteCount, decoded.begin()[decoded.size()]
			);
	}
	rtrn 0;
}

::llc::err_t testBase64(ATestError & errors) {
	if_fail_fe(testBase64Vectors			(errors));
	if_fail_fe(testBase64Append			(errors));
	if_fail_fe(testBase64Alphabets		(errors));
	if_fail_fe(testBase64Overloads		(errors, false));
	if_fail_fe(testBase64Overloads		(errors, true));
	if_fail_fe(testBase64CountedTerminator(errors));
	if_fail_fe(testBase64InvalidInput		(errors));
	if_fail_fe(testBase64InvalidAlphabet	(errors));
	if_fail_fe(testBase64Fonts				(errors));
	if_fail_fe(testBase64Random			(errors, false));
	rtrn testBase64Random				(errors, true);
}
