#include "llc_test_core_suites.h"
#include "llc_apod_serialize.h"

GDEFINE_ENUM_TYPE(TEST_APOD_SERIALIZE_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_APOD_SERIALIZE_RESULT, PACKED_BOUNDARIES      ,  0, "Packed unsigned boundary values round-trip and consume their exact widths.");
GDEFINE_ENUM_VALUED(TEST_APOD_SERIALIZE_RESULT, PACKED_APPEND          ,  1, "Saving a packed unsigned value appends without changing an existing prefix.");
GDEFINE_ENUM_VALUED(TEST_APOD_SERIALIZE_RESULT, VIEW_EMPTY             ,  2, "An empty POD view round-trips and clears a non-empty destination.");
GDEFINE_ENUM_VALUED(TEST_APOD_SERIALIZE_RESULT, VIEW_ROUNDTRIP         ,  3, "A POD view round-trips every element and consumes its input.");
GDEFINE_ENUM_VALUED(TEST_APOD_SERIALIZE_RESULT, VIEW_APPEND            ,  4, "Saving a POD view appends without changing an existing prefix.");
GDEFINE_ENUM_VALUED(TEST_APOD_SERIALIZE_RESULT, VIEW_TRAILING          ,  5, "Loading a POD view leaves trailing input bytes untouched.");
GDEFINE_ENUM_VALUED(TEST_APOD_SERIALIZE_RESULT, TRUNCATED_COUNTER      ,  6, "A missing view counter fails without changing input or destination.");
GDEFINE_ENUM_VALUED(TEST_APOD_SERIALIZE_RESULT, TRUNCATED_PAYLOAD      ,  7, "A truncated view payload fails without changing input or destination.");
GDEFINE_ENUM_VALUED(TEST_APOD_SERIALIZE_RESULT, DECLARED_COUNT         ,  8, "A declared count larger than the payload is rejected.");
GDEFINE_ENUM_VALUED(TEST_APOD_SERIALIZE_RESULT, VIEW_DEEP_COPY         ,  9, "Loading a POD view owns a copy independent from serialized storage.");
GDEFINE_ENUM_VALUED(TEST_APOD_SERIALIZE_RESULT, POD_UNSIGNED           , 10, "The unsigned-byte POD overload round-trips an exact object representation.");
GDEFINE_ENUM_VALUED(TEST_APOD_SERIALIZE_RESULT, POD_SIGNED             , 11, "The signed-byte POD wrappers round-trip an exact object representation.");
GDEFINE_ENUM_VALUED(TEST_APOD_SERIALIZE_RESULT, VIEW_SIGNED            , 12, "The signed-byte view wrappers round-trip every element.");
GDEFINE_ENUM_VALUED(TEST_APOD_SERIALIZE_RESULT, EXACT_LAYOUT           , 13, "A serialized view is the packed count followed by exact element bytes.");

sttc ::llc::err_t testAPodSerializePackedBoundaries(::llc::ATestError & results) {
	cnst ::llc::u2_t values[] = {0, 1, 0x3F, 0x40, 0x3FFF, 0x4000, 0x3FFFFFFF};
	for(::llc::u2_t i = 0; i < ::llc::size(values); ++i) {
		::llc::au0_t encoded;
		cnst ::llc::err_t bytesSaved = ::llc::saveUInt(encoded, values[i]);
		LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_PACKED_BOUNDARIES, 0 >= bytesSaved);
		LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_PACKED_BOUNDARIES, (::llc::u2_t)bytesSaved != encoded.size());
		::llc::vcu0_t input = encoded;
		::llc::u2_t decoded = (::llc::u2_t)-1;
		cnst ::llc::err_t bytesLoaded = ::llc::loadUInt(input, decoded);
		LLC_TEST_CHECK(results, TEST_APOD_SERIALIZE_RESULT_PACKED_BOUNDARIES, bytesLoaded != bytesSaved || input.size() || decoded != values[i]);
	}
	rtrn 0;
}

sttc ::llc::err_t testAPodSerializePackedAppend(::llc::ATestError & results) {
	::llc::au0_t encoded = {0xA1, 0xB2};
	LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_PACKED_APPEND, 0 > ::llc::saveUInt(encoded, (::llc::u2_t)0x1234));
	LLC_TEST_CHECK(results, TEST_APOD_SERIALIZE_RESULT_PACKED_APPEND, encoded.size() <= 2 || encoded[0] != 0xA1 || encoded[1] != 0xB2);
	::llc::vcu0_t input = {&encoded[2], encoded.size() - 2};
	::llc::u2_t decoded = 0;
	LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_PACKED_APPEND, 0 > ::llc::loadUInt(input, decoded));
	LLC_TEST_CHECK(results, TEST_APOD_SERIALIZE_RESULT_PACKED_APPEND, input.size() || decoded != 0x1234);
	rtrn 0;
}

sttc ::llc::err_t testAPodSerializeViewEmpty(::llc::ATestError & results) {
	::llc::au1_t source;
	::llc::au0_t encoded;
	cnst ::llc::view<cnst ::llc::u1_t> sourceView = source;
	LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_VIEW_EMPTY, 0 >= ::llc::saveView(encoded, sourceView));
	::llc::vcu0_t input = encoded;
	::llc::au1_t decoded = {7};
	LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_VIEW_EMPTY, 0 != ::llc::loadView(input, decoded));
	LLC_TEST_CHECK(results, TEST_APOD_SERIALIZE_RESULT_VIEW_EMPTY, input.size() || decoded.size() || decoded.begin());
	rtrn 0;
}

sttc ::llc::err_t testAPodSerializeViewRoundtrip(::llc::ATestError & results) {
	::llc::au1_t source = {1, 0x1234, 0xFFFF};
	::llc::au0_t encoded;
	cnst ::llc::view<cnst ::llc::u1_t> sourceView = source;
	LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_VIEW_ROUNDTRIP, 0 > ::llc::saveView(encoded, sourceView));
	::llc::vcu0_t input = encoded;
	::llc::au1_t decoded;
	LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_VIEW_ROUNDTRIP, 0 != ::llc::loadView(input, decoded));
	LLC_TEST_CHECK(results, TEST_APOD_SERIALIZE_RESULT_VIEW_ROUNDTRIP, input.size() || decoded.size() != source.size() || 0 != memcmp(decoded.begin(), source.begin(), source.byte_count()));
	rtrn 0;
}

sttc ::llc::err_t testAPodSerializeViewAppend(::llc::ATestError & results) {
	::llc::au1_t source = {1, 2};
	::llc::au0_t encoded = {0xA1, 0xB2};
	cnst ::llc::view<cnst ::llc::u1_t> sourceView = source;
	LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_VIEW_APPEND, 0 > ::llc::saveView(encoded, sourceView));
	LLC_TEST_CHECK(results, TEST_APOD_SERIALIZE_RESULT_VIEW_APPEND, encoded[0] != 0xA1 || encoded[1] != 0xB2);
	::llc::vcu0_t input = {&encoded[2], encoded.size() - 2};
	::llc::au1_t decoded;
	LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_VIEW_APPEND, 0 != ::llc::loadView(input, decoded));
	LLC_TEST_CHECK(results, TEST_APOD_SERIALIZE_RESULT_VIEW_APPEND, input.size() || decoded.size() != source.size() || 0 != memcmp(decoded.begin(), source.begin(), source.byte_count()));
	rtrn 0;
}

sttc ::llc::err_t testAPodSerializeViewTrailing(::llc::ATestError & results) {
	::llc::au1_t source = {1, 2};
	::llc::au0_t encoded;
	cnst ::llc::view<cnst ::llc::u1_t> sourceView = source;
	LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_VIEW_TRAILING, 0 > ::llc::saveView(encoded, sourceView));
	LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_VIEW_TRAILING, 0 > encoded.push_back(0xA5));
	::llc::vcu0_t input = encoded;
	::llc::au1_t decoded;
	LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_VIEW_TRAILING, 0 != ::llc::loadView(input, decoded));
	LLC_TEST_CHECK(results, TEST_APOD_SERIALIZE_RESULT_VIEW_TRAILING, input.size() != 1 || input[0] != 0xA5 || decoded.size() != source.size() || 0 != memcmp(decoded.begin(), source.begin(), source.byte_count()));
	rtrn 0;
}

sttc ::llc::err_t testAPodSerializeTruncatedCounter(::llc::ATestError & results) {
	::llc::vcu0_t input;
	::llc::au1_t decoded = {7};
	::llc::u1_t * originalData = decoded.begin();
	LLC_TEST_CHECK(results, TEST_APOD_SERIALIZE_RESULT_TRUNCATED_COUNTER, 0 <= ::llc::loadView(input, decoded));
	LLC_TEST_CHECK(results, TEST_APOD_SERIALIZE_RESULT_TRUNCATED_COUNTER, input.size() || decoded.size() != 1 || decoded.begin() != originalData || decoded[0] != 7);
	rtrn 0;
}

sttc ::llc::err_t testAPodSerializeTruncatedPayload(::llc::ATestError & results) {
	::llc::au1_t source = {1, 2};
	::llc::au0_t encoded;
	cnst ::llc::view<cnst ::llc::u1_t> sourceView = source;
	LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_TRUNCATED_PAYLOAD, 0 > ::llc::saveView(encoded, sourceView));
	::llc::vcu0_t input = {encoded.begin(), encoded.size() - 1};
	cnst ::llc::u2_t originalInputSize = input.size();
	::llc::au1_t decoded = {7};
	::llc::u1_t * originalData = decoded.begin();
	LLC_TEST_CHECK(results, TEST_APOD_SERIALIZE_RESULT_TRUNCATED_PAYLOAD, 0 <= ::llc::loadView(input, decoded));
	LLC_TEST_CHECK(results, TEST_APOD_SERIALIZE_RESULT_TRUNCATED_PAYLOAD, input.size() != originalInputSize || decoded.size() != 1 || decoded.begin() != originalData || decoded[0] != 7);
	rtrn 0;
}

sttc ::llc::err_t testAPodSerializeDeclaredCount(::llc::ATestError & results) {
	::llc::au0_t encoded;
	LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_DECLARED_COUNT, 0 > ::llc::savePacked(encoded, ::llc::packedu32{3}));
	cnst ::llc::u1_t values[] = {1, 2};
	LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_DECLARED_COUNT, 0 > encoded.append((cnst ::llc::u0_t*)values, szof(values)));
	::llc::vcu0_t input = encoded;
	::llc::au1_t decoded = {7};
	LLC_TEST_CHECK(results, TEST_APOD_SERIALIZE_RESULT_DECLARED_COUNT, 0 <= ::llc::loadView(input, decoded));
	LLC_TEST_CHECK(results, TEST_APOD_SERIALIZE_RESULT_DECLARED_COUNT, input.size() != encoded.size() || decoded.size() != 1 || decoded[0] != 7);
	rtrn 0;
}

sttc ::llc::err_t testAPodSerializeViewDeepCopy(::llc::ATestError & results) {
	::llc::au1_t source = {1, 2};
	::llc::au0_t encoded;
	cnst ::llc::view<cnst ::llc::u1_t> sourceView = source;
	LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_VIEW_DEEP_COPY, 0 > ::llc::saveView(encoded, sourceView));
	::llc::vcu0_t input = encoded;
	::llc::au1_t decoded;
	LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_VIEW_DEEP_COPY, 0 != ::llc::loadView(input, decoded));
	memset(encoded.begin(), 0, encoded.size());
	LLC_TEST_CHECK(results, TEST_APOD_SERIALIZE_RESULT_VIEW_DEEP_COPY, decoded.size() != 2 || decoded[0] != 1 || decoded[1] != 2);
	rtrn 0;
}

sttc ::llc::err_t testAPodSerializePODUnsigned(::llc::ATestError & results) {
	cnst ::llc::u3_t source = 0x0123456789ABCDEFULL;
	::llc::au0_t encoded;
	LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_POD_UNSIGNED, (::llc::err_t)szof(source) != ::llc::savePOD(encoded, source));
	::llc::vcu0_t input = encoded;
	::llc::u3_t decoded = 0;
	LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_POD_UNSIGNED, (::llc::err_t)szof(decoded) != ::llc::loadPOD(input, decoded));
	LLC_TEST_CHECK(results, TEST_APOD_SERIALIZE_RESULT_POD_UNSIGNED, input.size() || decoded != source);
	rtrn 0;
}

sttc ::llc::err_t testAPodSerializePODSigned(::llc::ATestError & results) {
	cnst ::llc::u3_t source = 0x0123456789ABCDEFULL;
	::llc::as0_t encoded;
	LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_POD_SIGNED, (::llc::err_t)szof(source) != ::llc::savePOD(encoded, source));
	::llc::vcs0_t input = encoded;
	::llc::u3_t decoded = 0;
	LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_POD_SIGNED, (::llc::err_t)szof(decoded) != ::llc::loadPOD(input, decoded));
	LLC_TEST_CHECK(results, TEST_APOD_SERIALIZE_RESULT_POD_SIGNED, input.size() || decoded != source);
	rtrn 0;
}

sttc ::llc::err_t testAPodSerializeViewSigned(::llc::ATestError & results) {
	::llc::au1_t source = {1, 0x1234, 0xFFFF};
	::llc::as0_t encoded;
	cnst ::llc::view<cnst ::llc::u1_t> sourceView = source;
	LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_VIEW_SIGNED, 0 > ::llc::saveView(encoded, sourceView));
	::llc::vcs0_t input = encoded;
	::llc::au1_t decoded;
	LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_VIEW_SIGNED, 0 != ::llc::loadView(input, decoded));
	LLC_TEST_CHECK(results, TEST_APOD_SERIALIZE_RESULT_VIEW_SIGNED, input.size() || decoded.size() != source.size() || 0 != memcmp(decoded.begin(), source.begin(), source.byte_count()));
	rtrn 0;
}

sttc ::llc::err_t testAPodSerializeExactLayout(::llc::ATestError & results) {
	::llc::au1_t source = {0x1122, 0x3344};
	::llc::au0_t encoded;
	cnst ::llc::view<cnst ::llc::u1_t> sourceView = source;
	LLC_TEST_REQUIRE(results, TEST_APOD_SERIALIZE_RESULT_EXACT_LAYOUT, 0 > ::llc::saveView(encoded, sourceView));
	cnst ::llc::packedu32 count = source.size();
	cnst ::llc::u0_t countWidth = count.ValueWidth();
	LLC_TEST_CHECK(results, TEST_APOD_SERIALIZE_RESULT_EXACT_LAYOUT
		, encoded.size() != countWidth + source.byte_count()
		|| 0 != memcmp(encoded.begin(), &count, countWidth)
		|| 0 != memcmp(&encoded[countWidth], source.begin(), source.byte_count())
		);
	rtrn 0;
}

::llc::err_t testAPodSerialize(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testAPodSerializePackedBoundaries(results));
	result = ::llc::min(result, ::testAPodSerializePackedAppend(results));
	result = ::llc::min(result, ::testAPodSerializeViewEmpty(results));
	result = ::llc::min(result, ::testAPodSerializeViewRoundtrip(results));
	result = ::llc::min(result, ::testAPodSerializeViewAppend(results));
	result = ::llc::min(result, ::testAPodSerializeViewTrailing(results));
	result = ::llc::min(result, ::testAPodSerializeTruncatedCounter(results));
	result = ::llc::min(result, ::testAPodSerializeTruncatedPayload(results));
	result = ::llc::min(result, ::testAPodSerializeDeclaredCount(results));
	result = ::llc::min(result, ::testAPodSerializeViewDeepCopy(results));
	result = ::llc::min(result, ::testAPodSerializePODUnsigned(results));
	result = ::llc::min(result, ::testAPodSerializePODSigned(results));
	result = ::llc::min(result, ::testAPodSerializeViewSigned(results));
	result = ::llc::min(result, ::testAPodSerializeExactLayout(results));
	rtrn result;
}
