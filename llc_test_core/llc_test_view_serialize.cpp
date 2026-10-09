#include "llc_test_core.h"
#include "llc_apod_serialize.h"
#include "llc_noise.h"

GDEFINE_ENUM_TYPE(VIEW_SERIALIZE_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, OK					,  0, "All view-serialization tests passed.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, SAVE_UINT_RESULT		,  1, "saveUInt() reported the wrong serialized width.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, SAVE_UINT_SIZE		,  2, "saveUInt() appended the wrong number of bytes.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, LOAD_UINT_RESULT		,  3, "loadUInt() reported the wrong consumed width.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, LOAD_UINT_VALUE		,  4, "loadUInt() did not restore the serialized integer.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, LOAD_UINT_POSITION		,  5, "loadUInt() did not leave the input at the following field.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, SOURCE_RESIZE			,  6, "The source array could not be prepared for a view test.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, SAVE_VIEW_RESULT		,  7, "saveView() reported the wrong serialized size.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, SAVE_VIEW_SIZE			,  8, "saveView() appended the wrong header or payload size.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, VIEW_READ_RESULT		,  9, "viewRead() reported the wrong header-plus-payload size.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, VIEW_READ_POSITION		, 10, "viewRead() produced the wrong element count or alias position.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, VIEW_READ_VALUE		, 11, "viewRead() produced an alias with different values.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, VIEW_LOAD_RESULT		, 12, "loadView() failed to restore a non-owning view.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, VIEW_LOAD_POSITION		, 13, "Non-owning loadView() consumed the wrong bytes or returned the wrong alias.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, VIEW_LOAD_VALUE		, 14, "Non-owning loadView() restored different values.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, APOD_LOAD_RESULT		, 15, "loadView() failed to restore an owned POD array.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, APOD_LOAD_POSITION		, 16, "Owned loadView() consumed the wrong number of bytes.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, APOD_LOAD_VALUE		, 17, "Owned loadView() restored a different array.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, TRUNCATED_UINT			, 18, "loadUInt() accepted a truncated packed integer or modified its output/cursor.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, TRUNCATED_VIEW			, 19, "loadView() accepted a truncated header or payload or modified its cursor.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, LOAD_PACKED_RESULT		, 20, "loadPacked() reported the wrong consumed width.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, LOAD_PACKED_VALUE		, 21, "loadPacked() did not decode the serialized packed value.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, LOAD_PACKED_POSITION	, 22, "loadPacked() did not leave the input at the following field.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, SAVE_POD_RESULT			, 23, "savePOD() reported the wrong serialized width.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, SAVE_POD_SIZE			, 24, "savePOD() appended the wrong bytes.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, LOAD_POD_RESULT			, 25, "loadPOD() reported the wrong consumed width.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, LOAD_POD_VALUE			, 26, "loadPOD() did not restore the serialized value.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, LOAD_POD_POSITION		, 27, "loadPOD() did not leave the input at the following field.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, TRUNCATED_POD			, 28, "loadPOD() accepted truncated input or modified its output/cursor.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, TRUNCATED_OWNED_VIEW	, 29, "Owned loadView() accepted truncated input or modified its output/cursor.");
GDEFINE_ENUM_VALUED(VIEW_SERIALIZE_TEST_RESULT, MALFORMED_VIEW_COUNT	, 30, "loadView() accepted a count whose payload byte size overflowed.");

stxp ::llc::u0_t PRECEDING_FIELD[] = {0x91U, 0xE4U, 0x2BU};
stxp ::llc::u0_t FOLLOWING_FIELD[] = {0xC3U, 0x5AU, 0x7EU, 0x19U};
stxp ::llc::u3_t SERIALIZE_UINT_RANDOM_SEED	= 0x53455249414C495AULL;
stxp ::llc::u3_t SERIALIZE_VIEW_RANDOM_SEED	= 0x56494557434F554EULL;
stxp ::llc::u2_t SERIALIZE_UINT_RANDOM_COUNT	= 256;
stxp ::llc::u2_t SERIALIZE_VIEW_RANDOM_COUNT	= 32;

sttc bool isFollowingField(cnst ::llc::vcu0_t & input) {
	return input.size() == ::llc::size(FOLLOWING_FIELD) && 0 == memcmp(input.begin(), FOLLOWING_FIELD, ::llc::size(FOLLOWING_FIELD));
}

tplt<tpnm T>
sttc ::llc::err_t testPOD(ATestError & errors) {
	cnst T source = T(0x5AU);
	::llc::au0_t serialized;
	LLC_TEST_REQUIRE(errors, VIEW_SERIALIZE_TEST_RESULT_SAVE_POD_SIZE, 0 > serialized.append(PRECEDING_FIELD)
		, "%u-bit savePOD() test could not append its preceding field."
		, bcof(T)
		);
	cnst ::llc::err_t saveResult = ::llc::savePOD(serialized, source);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_SAVE_POD_RESULT, saveResult != szof(T)
		, "%u-bit savePOD() result mismatch. result:%i, expected:%u."
		, bcof(T), saveResult, ::llc::u2_t(szof(T))
		);
	LLC_TEST_REQUIRE(errors, VIEW_SERIALIZE_TEST_RESULT_SAVE_POD_SIZE, serialized.size() != ::llc::size(PRECEDING_FIELD) + szof(T)
		, "%u-bit savePOD() size mismatch. size:%u, expected:%u."
		, bcof(T), serialized.size(), ::llc::u2_t(::llc::size(PRECEDING_FIELD) + szof(T))
		);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_SAVE_POD_SIZE, 0 != memcmp(serialized.begin(), PRECEDING_FIELD, ::llc::size(PRECEDING_FIELD))
		, "%u-bit savePOD() modified its preceding field."
		, bcof(T)
		);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_SAVE_POD_SIZE, 0 != memcmp(&serialized[::llc::size(PRECEDING_FIELD)], &source, szof(T))
		, "%u-bit savePOD() payload differs from the source."
		, bcof(T)
		);
	LLC_TEST_REQUIRE(errors, VIEW_SERIALIZE_TEST_RESULT_SAVE_POD_SIZE, 0 > serialized.append(FOLLOWING_FIELD)
		, "%u-bit savePOD() test could not append its following field."
		, bcof(T)
		);
	::llc::vcu0_t input{&serialized[::llc::size(PRECEDING_FIELD)], serialized.size() - ::llc::size(PRECEDING_FIELD)};
	T loaded = {};
	cnst ::llc::err_t loadResult = ::llc::loadPOD(input, loaded);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_LOAD_POD_RESULT, loadResult != szof(T)
		, "%u-bit loadPOD() result mismatch. result:%i, expected:%u."
		, bcof(T), loadResult, ::llc::u2_t(szof(T))
		);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_LOAD_POD_VALUE, loaded != source
		, "%u-bit loadPOD() value mismatch. loaded:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "."
		, bcof(T), ::llc::u3_t(loaded), ::llc::u3_t(source)
		);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_LOAD_POD_POSITION, !isFollowingField(input)
		, "%u-bit loadPOD() cursor mismatch. remaining:%u, expected:%u."
		, bcof(T), input.size(), ::llc::u2_t(::llc::size(FOLLOWING_FIELD))
		);
	cnst T sentinel = (T)~source;
	for(::llc::u2_t byteCount = 0; byteCount < szof(T); ++byteCount) {
		::llc::vcu0_t truncated{&serialized[::llc::size(PRECEDING_FIELD)], byteCount};
		cnst ::llc::vcu0_t originalInput = truncated;
		T unchanged = sentinel;
		::llc::setupLogCallbacks(0, 0);
		cnst ::llc::err_t truncatedResult = ::llc::loadPOD(truncated, unchanged);
		::llc::setupDefaultLogCallbacks();
		LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_TRUNCATED_POD, 0 <= truncatedResult
			, "%u-bit loadPOD() accepted truncated input. supplied:%u, required:%u, result:%i."
			, bcof(T), byteCount, ::llc::u2_t(szof(T)), truncatedResult
			);
		LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_TRUNCATED_POD, unchanged != sentinel
			, "%u-bit loadPOD() modified output on truncated input. supplied:%u, value:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "."
			, bcof(T), byteCount, ::llc::u3_t(unchanged), ::llc::u3_t(sentinel)
			);
		LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_TRUNCATED_POD, truncated != originalInput
			, "%u-bit loadPOD() moved cursor on truncated input. supplied:%u, remaining:%u, expected:%u."
			, bcof(T), byteCount, truncated.size(), originalInput.size()
			);
	}
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testUIntValue(ATestError & errors, ::llc::u3_c source) {
	cnst T value = (T)source;
	cnst ::llc::packed_uint<T> packed{value};
	cnst ::llc::u2_t expectedWidth = packed.ValueWidth();
	::llc::au0_t serialized;
	LLC_TEST_REQUIRE(errors, VIEW_SERIALIZE_TEST_RESULT_SAVE_UINT_SIZE, 0 > serialized.append(PRECEDING_FIELD)
		, "%u-bit saveUInt() test could not append its preceding field for value:%" LLC_FMT_U3 "."
		, bcof(T), source
		);
	cnst ::llc::err_t saveResult = ::llc::saveUInt(serialized, value);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_SAVE_UINT_RESULT, saveResult != (::llc::err_t)expectedWidth
		, "%u-bit saveUInt() result mismatch for value:%" LLC_FMT_U3 ". result:%i, expected:%u."
		, bcof(T), source, saveResult, expectedWidth
		);
	LLC_TEST_REQUIRE(errors, VIEW_SERIALIZE_TEST_RESULT_SAVE_UINT_SIZE, serialized.size() != ::llc::size(PRECEDING_FIELD) + expectedWidth
		, "%u-bit saveUInt() size mismatch for value:%" LLC_FMT_U3 ". size:%u, expected:%u."
		, bcof(T), source, serialized.size(), ::llc::u2_t(::llc::size(PRECEDING_FIELD) + expectedWidth)
		);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_SAVE_UINT_SIZE, 0 != memcmp(serialized.begin(), PRECEDING_FIELD, ::llc::size(PRECEDING_FIELD))
		, "%u-bit saveUInt() modified its preceding field for value:%" LLC_FMT_U3 "."
		, bcof(T), source
		);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_SAVE_UINT_SIZE, 0 > serialized.append(FOLLOWING_FIELD)
		, "%u-bit saveUInt() test could not append its following field for value:%" LLC_FMT_U3 "."
		, bcof(T), source
		);

	::llc::vcu0_t packedInput{&serialized[::llc::size(PRECEDING_FIELD)], serialized.size() - ::llc::size(PRECEDING_FIELD)};
	T loadedPacked = {};
	cnst ::llc::err_t packedResult = ::llc::loadPacked(packedInput, loadedPacked);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_LOAD_PACKED_RESULT, packedResult != (::llc::err_t)expectedWidth
		, "%u-bit loadPacked() result mismatch for value:%" LLC_FMT_U3 ". result:%i, expected:%u."
		, bcof(T), source, packedResult, expectedWidth
		);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_LOAD_PACKED_VALUE, ::llc::u3_t(loadedPacked) != source
		, "%u-bit loadPacked() value mismatch. loaded:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "."
		, bcof(T), ::llc::u3_t(loadedPacked), source
		);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_LOAD_PACKED_POSITION, !isFollowingField(packedInput)
		, "%u-bit loadPacked() cursor mismatch for value:%" LLC_FMT_U3 ". remaining:%u, expected:%u."
		, bcof(T), source, packedInput.size(), ::llc::u2_t(::llc::size(FOLLOWING_FIELD))
		);

	::llc::vcu0_t input{&serialized[::llc::size(PRECEDING_FIELD)], serialized.size() - ::llc::size(PRECEDING_FIELD)};
	T loaded = {};
	cnst ::llc::err_t loadResult = ::llc::loadUInt(input, loaded);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_LOAD_UINT_RESULT, loadResult != (::llc::err_t)expectedWidth
		, "%u-bit loadUInt() result mismatch for value:%" LLC_FMT_U3 ". result:%i, expected:%u."
		, bcof(T), source, loadResult, expectedWidth
		);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_LOAD_UINT_VALUE, ::llc::u3_t(loaded) != source
		, "%u-bit loadUInt() value mismatch. loaded:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "."
		, bcof(T), ::llc::u3_t(loaded), source
		);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_LOAD_UINT_POSITION, !isFollowingField(input)
		, "%u-bit loadUInt() cursor mismatch for value:%" LLC_FMT_U3 ". remaining:%u, expected:%u."
		, bcof(T), source, input.size(), ::llc::u2_t(::llc::size(FOLLOWING_FIELD))
		);
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testUIntType(ATestError & errors) {
	stxp ::llc::u0_t MULTIPLIER_BITS = 8 - ::llc::uint_width_field_size<T>();
	::llc::u3_t previousMaximum = 0;
	for(::llc::u0_t tailWidth = 0; tailWidth < szof(T); ++tailWidth) {
		::llc::u0_c valueBits = MULTIPLIER_BITS + tailWidth * 8;
		::llc::u3_c maximum = (::llc::u3_t(1) << valueBits) - 1;
		::llc::u3_c minimum = tailWidth ? previousMaximum + 1 : 0;
		if_fail_fe(testUIntValue<T>(errors, minimum));
		if(maximum != minimum)
			if_fail_fe(testUIntValue<T>(errors, maximum));
		previousMaximum = maximum;
	}
	::llc::SPRNG random = {SERIALIZE_UINT_RANDOM_SEED};
	stxp ::llc::u0_t VALUE_BITS = 8 - ::llc::uint_width_field_size<T>() + (szof(T) - 1) * 8;
	stxp ::llc::u3_t VALUE_MASK = (::llc::u3_t(1) << VALUE_BITS) - 1;
	for(::llc::u2_t iRandom = 0; iRandom < SERIALIZE_UINT_RANDOM_COUNT; ++iRandom) {
		cnst T value = T(random.Next() & VALUE_MASK);
		cnst ::llc::u2_t failureCount = testErrorCount(errors);
		if_fail_fe(testUIntValue<T>(errors, value));
		if(failureCount != testErrorCount(errors))
			error_printf("%u-bit randomized integer serialization case failed. seed:%" LLC_FMT_U3 ", iteration:%u, value:%" LLC_FMT_U3 "."
				, bcof(T), SERIALIZE_UINT_RANDOM_SEED, iRandom, ::llc::u3_t(value)
				);
	}
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testTruncatedUInt(ATestError & errors) {
	stxp ::llc::u0_t VALUE_BITS = 8 - ::llc::uint_width_field_size<T>() + (szof(T) - 1) * 8;
	cnst T value = (T)((::llc::u3_t(1) << VALUE_BITS) - 1);
	cnst ::llc::packed_uint<T> packed{value};
	cnst ::llc::vcu0_t bytes = packed.tplt cu8<::llc::vcu0_t>();
	for(::llc::u2_t byteCount = 0; byteCount < bytes.size(); ++byteCount) {
		::llc::vcu0_t input{bytes.begin(), byteCount};
		cnst ::llc::vcu0_t originalInput = input;
		cnst T sentinel = (T)~T(0);
		T loaded = sentinel;
		::llc::setupLogCallbacks(0, 0);
		cnst ::llc::err_t loadResult = ::llc::loadUInt(input, loaded);
		::llc::setupDefaultLogCallbacks();
		LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_TRUNCATED_UINT, 0 <= loadResult
			, "%u-bit loadUInt() accepted truncated input. supplied:%u, required:%u, result:%i."
			, bcof(T), byteCount, bytes.size(), loadResult
			);
		LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_TRUNCATED_UINT, loaded != sentinel
			, "%u-bit loadUInt() modified output on truncated input. supplied:%u, loaded:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "."
			, bcof(T), byteCount, ::llc::u3_t(loaded), ::llc::u3_t(sentinel)
			);
		LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_TRUNCATED_UINT, input != originalInput
			, "%u-bit loadUInt() moved cursor on truncated input. supplied:%u, remaining:%u, expected:%u."
			, bcof(T), byteCount, input.size(), originalInput.size()
			);
	}
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testViewCount(ATestError & errors, ::llc::u2_c elementCount) {
	::llc::apod<T> source;
	cnst ::llc::err_t resizeResult = source.resize(elementCount);
	LLC_TEST_REQUIRE(errors, VIEW_SERIALIZE_TEST_RESULT_SOURCE_RESIZE, resizeResult != (::llc::err_t)elementCount
		, "%u-bit source resize mismatch. result:%i, expected:%u."
		, bcof(T), resizeResult, elementCount
		);
	for(::llc::u2_t iElement = 0; iElement < elementCount; ++iElement)
		source[iElement] = T(::llc::u3_t(iElement) * 0x9E3779B1ULL + elementCount + szof(T));

	cnst ::llc::view<cnst T> expected = source;
	cnst ::llc::packedu32 countHeader = elementCount;
	cnst ::llc::u2_t headerWidth = countHeader.ValueWidth();
	cnst ::llc::u2_t expectedSize = headerWidth + expected.byte_count();
	::llc::au0_t serialized;
	LLC_TEST_REQUIRE(errors, VIEW_SERIALIZE_TEST_RESULT_SAVE_VIEW_SIZE, 0 > serialized.append(PRECEDING_FIELD)
		, "%u-bit saveView() test could not append its preceding field for %u elements."
		, bcof(T), elementCount
		);
	cnst ::llc::err_t saveResult = ::llc::saveView(serialized, expected);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_SAVE_VIEW_RESULT, saveResult != (::llc::err_t)expectedSize
		, "%u-bit saveView() result mismatch for %u elements. result:%i, expected:%u."
		, bcof(T), elementCount, saveResult, expectedSize
		);
	LLC_TEST_REQUIRE(errors, VIEW_SERIALIZE_TEST_RESULT_SAVE_VIEW_SIZE, serialized.size() != ::llc::size(PRECEDING_FIELD) + expectedSize
		, "%u-bit saveView() append-size mismatch for %u elements. size:%u, expected:%u, prefix:%u, header:%u, payload:%u."
		, bcof(T), elementCount, serialized.size(), ::llc::u2_t(::llc::size(PRECEDING_FIELD) + expectedSize), ::llc::u2_t(::llc::size(PRECEDING_FIELD)), headerWidth, expected.byte_count()
		);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_SAVE_VIEW_SIZE, 0 != memcmp(serialized.begin(), PRECEDING_FIELD, ::llc::size(PRECEDING_FIELD))
		, "%u-bit saveView() modified its preceding field for %u elements. prefix:%u, header:%u, payload:%u."
		, bcof(T), elementCount, ::llc::u2_t(::llc::size(PRECEDING_FIELD)), headerWidth, expected.byte_count()
		);

	cnst ::llc::vcu0_t exactInput{&serialized[::llc::size(PRECEDING_FIELD)], expectedSize};
	::llc::view<cnst T> readView;
	cnst ::llc::err_t readResult = ::llc::viewRead(readView, exactInput);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_VIEW_READ_RESULT, readResult != (::llc::err_t)expectedSize
		, "%u-bit viewRead() result mismatch for %u elements. result:%i, expected:%u."
		, bcof(T), elementCount, readResult, expectedSize
		);
	cnst ::llc::vcu0_t expectedReadBytes = elementCount
		? ::llc::vcu0_t{&serialized[::llc::size(PRECEDING_FIELD) + headerWidth], expected.byte_count()}
		: ::llc::vcu0_t{}
		;
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_VIEW_READ_POSITION, readView.size() != elementCount
		, "%u-bit viewRead() count mismatch. count:%u, expected:%u."
		, bcof(T), readView.size(), elementCount
		);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_VIEW_READ_POSITION, readView.cu8().begin() != expectedReadBytes.begin()
		, "%u-bit viewRead() alias position mismatch for %u elements. begin:%p, expected:%p."
		, bcof(T), elementCount, readView.begin(), expectedReadBytes.begin()
		);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_VIEW_READ_VALUE, readView != expected
		, "%u-bit viewRead() value mismatch for %u elements."
		, bcof(T), elementCount
		);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_SAVE_VIEW_SIZE, 0 > serialized.append(FOLLOWING_FIELD)
		, "%u-bit view test could not append its following field for %u elements."
		, bcof(T), elementCount
		);

	::llc::vcu0_t viewInput{&serialized[::llc::size(PRECEDING_FIELD)], serialized.size() - ::llc::size(PRECEDING_FIELD)};
	::llc::view<cnst T> loadedView;
	cnst ::llc::err_t viewLoadResult = ::llc::loadView(viewInput, loadedView);
	cnst ::llc::vcu0_t expectedLoadBytes = elementCount
		? ::llc::vcu0_t{&serialized[::llc::size(PRECEDING_FIELD) + headerWidth], expected.byte_count()}
		: ::llc::vcu0_t{}
		;
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_VIEW_LOAD_RESULT, viewLoadResult
		, "%u-bit non-owning loadView() failed for %u elements. result:%i."
		, bcof(T), elementCount, viewLoadResult
		);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_VIEW_LOAD_POSITION, loadedView.cu8().begin() != expectedLoadBytes.begin()
		, "%u-bit non-owning loadView() alias position mismatch for %u elements. begin:%p, expected:%p."
		, bcof(T), elementCount, loadedView.begin(), expectedLoadBytes.begin()
		);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_VIEW_LOAD_POSITION, loadedView.size() != elementCount
		, "%u-bit non-owning loadView() count mismatch. count:%u, expected:%u."
		, bcof(T), loadedView.size(), elementCount
		);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_VIEW_LOAD_POSITION, !isFollowingField(viewInput)
		, "%u-bit non-owning loadView() cursor mismatch for %u elements. remaining:%u, expected:%u."
		, bcof(T), elementCount, viewInput.size(), ::llc::u2_t(::llc::size(FOLLOWING_FIELD))
		);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_VIEW_LOAD_VALUE, loadedView != expected
		, "%u-bit non-owning loadView() value mismatch for %u elements."
		, bcof(T), elementCount
		);

	::llc::vcu0_t ownedInput{&serialized[::llc::size(PRECEDING_FIELD)], serialized.size() - ::llc::size(PRECEDING_FIELD)};
	::llc::apod<T> loadedArray;
	cnst ::llc::err_t arrayLoadResult = ::llc::loadView(ownedInput, loadedArray);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_APOD_LOAD_RESULT, arrayLoadResult
		, "%u-bit owned loadView() failed for %u elements. result:%i."
		, bcof(T), elementCount, arrayLoadResult
		);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_APOD_LOAD_POSITION, !isFollowingField(ownedInput)
		, "%u-bit owned loadView() cursor mismatch for %u elements. remaining:%u, expected:%u."
		, bcof(T), elementCount, ownedInput.size(), ::llc::u2_t(::llc::size(FOLLOWING_FIELD))
		);
	cnst ::llc::view<cnst T> loadedValues = loadedArray;
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_APOD_LOAD_VALUE, loadedValues != expected
		, "%u-bit owned loadView() value mismatch for %u elements. loaded count:%u."
		, bcof(T), elementCount, loadedValues.size()
		);
	if(elementCount) {
		LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_APOD_LOAD_VALUE, loadedValues.begin() == expected.begin()
			, "%u-bit owned loadView() aliases its source for %u elements. loaded begin:%p, source begin:%p."
			, bcof(T), elementCount, loadedValues.begin(), expected.begin()
			);
	}
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testViewType(ATestError & errors) {
	stxp ::llc::u2_t COUNTS[] = {0, 1, 2, 63, 64, 255, 16383, 16384};
	for(::llc::u2_c count : COUNTS)
		if_fail_fe(testViewCount<T>(errors, count));
	::llc::SPRNG random = {SERIALIZE_VIEW_RANDOM_SEED};
	for(::llc::u2_t iRandom = 0; iRandom < SERIALIZE_VIEW_RANDOM_COUNT; ++iRandom) {
		cnst ::llc::u2_t elementCount = random.Next() & 0xFFFU;
		cnst ::llc::u2_t failureCount = testErrorCount(errors);
		if_fail_fe(testViewCount<T>(errors, elementCount));
		if(failureCount != testErrorCount(errors))
			error_printf("%u-bit randomized view serialization case failed. seed:%" LLC_FMT_U3 ", iteration:%u, element count:%u."
				, bcof(T), SERIALIZE_VIEW_RANDOM_SEED, iRandom, elementCount
				);
	}
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testTruncatedView(ATestError & errors) {
	::llc::apod<T> source;
	cnst ::llc::u2_t elementCount = 64;
	cnst ::llc::err_t resizeResult = source.resize(elementCount);
	LLC_TEST_REQUIRE(errors, VIEW_SERIALIZE_TEST_RESULT_SOURCE_RESIZE, resizeResult != (::llc::err_t)elementCount
		, "%u-bit truncation source resize mismatch. result:%i, expected:%u."
		, bcof(T), resizeResult, elementCount
		);
	::llc::au0_t serialized;
	cnst ::llc::view<cnst T> sourceView = source;
	cnst ::llc::err_t saveResult = ::llc::saveView(serialized, sourceView);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_SAVE_VIEW_RESULT, saveResult != (::llc::err_t)serialized.size()
		, "%u-bit truncation source serialization mismatch. result:%i, size:%u."
		, bcof(T), saveResult, serialized.size()
		);
	cnst ::llc::u2_t headerWidth = ::llc::packedu32{elementCount}.ValueWidth();
	LLC_TEST_REQUIRE(errors, VIEW_SERIALIZE_TEST_RESULT_SAVE_VIEW_RESULT, serialized.size() <= headerWidth
		, "%u-bit truncation source serialization was too short. size:%u, header:%u."
		, bcof(T), serialized.size(), headerWidth
		);
	cnst ::llc::u2_t cuts[] = {0, headerWidth - 1, headerWidth, serialized.size() - 1};
	cnst T sentinel = (T)~T(0);
	for(::llc::u2_t iCut = 0; iCut < ::llc::size(cuts); ++iCut) {
		cnst ::llc::u2_t byteCount = cuts[iCut];
		if(iCut && byteCount == cuts[iCut - 1]) continue;
		::llc::vcu0_t input{serialized.begin(), byteCount};
		cnst ::llc::vcu0_t originalInput = input;
		::llc::view<cnst T> loaded{&sentinel, 1};
		::llc::setupLogCallbacks(0, 0);
		cnst ::llc::err_t loadResult = ::llc::loadView(input, loaded);
		::llc::setupDefaultLogCallbacks();
		LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_TRUNCATED_VIEW, 0 <= loadResult
			, "%u-bit loadView() accepted truncated input. supplied:%u, required:%u, header:%u, result:%i."
			, bcof(T), byteCount, serialized.size(), headerWidth, loadResult
			);
		LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_TRUNCATED_VIEW, input != originalInput
			, "%u-bit loadView() moved cursor on truncated input. supplied:%u, remaining:%u, expected:%u."
			, bcof(T), byteCount, input.size(), originalInput.size()
			);
		LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_TRUNCATED_VIEW, loaded.size() != 1
			, "%u-bit loadView() changed output count on truncated input. supplied:%u, count:%u, expected:1."
			, bcof(T), byteCount, loaded.size()
			);
		LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_TRUNCATED_VIEW, loaded.begin() != &sentinel
			, "%u-bit loadView() changed output alias on truncated input. supplied:%u, begin:%p, expected:%p."
			, bcof(T), byteCount, loaded.begin(), &sentinel
			);
		::llc::vcu0_t ownedInput{serialized.begin(), byteCount};
		cnst ::llc::vcu0_t originalOwnedInput = ownedInput;
		::llc::apod<T> owned = {sentinel};
		::llc::setupLogCallbacks(0, 0);
		cnst ::llc::err_t ownedResult = ::llc::loadView(ownedInput, owned);
		::llc::setupDefaultLogCallbacks();
		LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_TRUNCATED_OWNED_VIEW, 0 <= ownedResult
			, "%u-bit owned loadView() accepted truncated input. supplied:%u, required:%u, header:%u, result:%i."
			, bcof(T), byteCount, serialized.size(), headerWidth, ownedResult
			);
		LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_TRUNCATED_OWNED_VIEW, ownedInput != originalOwnedInput
			, "%u-bit owned loadView() moved cursor on truncated input. supplied:%u, remaining:%u, expected:%u."
			, bcof(T), byteCount, ownedInput.size(), originalOwnedInput.size()
			);
		LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_TRUNCATED_OWNED_VIEW, owned.size() != 1
			, "%u-bit owned loadView() changed output count on truncated input. supplied:%u, count:%u, expected:1."
			, bcof(T), byteCount, owned.size()
			);
		if(owned.size()) {
			LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_TRUNCATED_OWNED_VIEW, owned[0] != sentinel
				, "%u-bit owned loadView() changed output value on truncated input. supplied:%u, value:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "."
				, bcof(T), byteCount, ::llc::u3_t(owned[0]), ::llc::u3_t(sentinel)
				);
		}
	}
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testMalformedViewCount(ATestError & errors) {
	if(8 != szof(T))
		return 0;
	stxp ::llc::u2_t ELEMENT_COUNT = 0x20000000U;
	::llc::au0_t serialized;
	cnst ::llc::err_t saveResult = ::llc::saveUInt(serialized, ELEMENT_COUNT);
	LLC_TEST_REQUIRE(errors, VIEW_SERIALIZE_TEST_RESULT_MALFORMED_VIEW_COUNT, saveResult <= 0
		, "%u-bit malformed-count test could not serialize count:%u. result:%i."
		, bcof(T), ELEMENT_COUNT, saveResult
		);
	LLC_TEST_REQUIRE(errors, VIEW_SERIALIZE_TEST_RESULT_MALFORMED_VIEW_COUNT, serialized.size() != (::llc::u2_t)saveResult
		, "%u-bit malformed-count serialized size mismatch. count:%u, size:%u, expected:%u."
		, bcof(T), ELEMENT_COUNT, serialized.size(), ::llc::u2_t(saveResult)
		);
	::llc::vcu0_t input = serialized;
	cnst ::llc::vcu0_t originalInput = input;
	cnst T sentinel = (T)~T(0);
	::llc::view<cnst T> loaded{&sentinel, 1};
	::llc::setupLogCallbacks(0, 0);
	cnst ::llc::err_t loadResult = ::llc::loadView(input, loaded);
	::llc::setupDefaultLogCallbacks();
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_MALFORMED_VIEW_COUNT, 0 <= loadResult
		, "%u-bit loadView() accepted overflowing count. declared elements:%u, supplied bytes:%u, result:%i."
		, bcof(T), ELEMENT_COUNT, serialized.size(), loadResult
		);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_MALFORMED_VIEW_COUNT, input != originalInput
		, "%u-bit loadView() moved cursor for overflowing count. remaining:%u, expected:%u."
		, bcof(T), input.size(), originalInput.size()
		);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_MALFORMED_VIEW_COUNT, loaded.begin() != &sentinel
		, "%u-bit loadView() changed output alias for overflowing count. begin:%p, expected:%p."
		, bcof(T), loaded.begin(), &sentinel
		);
	LLC_TEST_CHECK(errors, VIEW_SERIALIZE_TEST_RESULT_MALFORMED_VIEW_COUNT, loaded.size() != 1
		, "%u-bit loadView() changed output count for overflowing count. count:%u, expected:1."
		, bcof(T), loaded.size()
		);
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testType(ATestError & errors) {
	if_fail_fe(testPOD<T>			(errors));
	if_fail_fe(testUIntType<T>		(errors));
	if_fail_fe(testTruncatedUInt<T>	(errors));
	if_fail_fe(testViewType<T>		(errors));
	if_fail_fe(testTruncatedView<T>	(errors));
	return testMalformedViewCount<T>(errors);
}

tplt<tpnm T>
sttc ::llc::err_t testTypeLogged(ATestError & errors) {
	cnst ::llc::u2_t checkCount = testCheckCount(errors);
	cnst ::llc::u2_t failureCount = testErrorCount(errors);
	if_fail_fe(testType<T>(errors));
	cnst ::llc::u2_t typeFailures = testErrorCount(errors) - failureCount;
	cnst ::llc::u2_t typeChecks = testCheckCount(errors) - checkCount;
	if(typeFailures) error_printf("%2u-bit suite completed: %u/%u checks passed, %u failed.", bcof(T), typeChecks - typeFailures, typeChecks, typeFailures);
	return 0;
}

::llc::err_t testViewSerialize(ATestError & errors) {
	cnst ::llc::u2_t failureCount = testErrorCount(errors);
	if_fail_fe(testTypeLogged<::llc::u0_t>(errors));
	if_fail_fe(testTypeLogged<::llc::u1_t>(errors));
	if_fail_fe(testTypeLogged<::llc::u2_t>(errors));
	if_fail_fe(testTypeLogged<::llc::u3_t>(errors));
	if(failureCount == testErrorCount(errors))
		always_printf("Element widths tested successfully:\n%u, %u, %u and %u bits."
			, ::llc::u2_t(szof(::llc::u0_t) * 8), ::llc::u2_t(szof(::llc::u1_t) * 8), ::llc::u2_t(szof(::llc::u2_t) * 8), ::llc::u2_t(szof(::llc::u3_t) * 8)
			);
	return 0;
}
