#include "llc_apod_serialize.h"
#include "llc_enum.h"

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

stxp ::llc::u0_t PRECEDING_FIELD[] = {0x91U, 0xE4U, 0x2BU};
stxp ::llc::u0_t FOLLOWING_FIELD[] = {0xC3U, 0x5AU, 0x7EU, 0x19U};

bool isFollowingField(cnst ::llc::vcu0_t & input) {
	return input.size() == ::llc::size(FOLLOWING_FIELD) && 0 == memcmp(input.begin(), FOLLOWING_FIELD, ::llc::size(FOLLOWING_FIELD));
}

tplt<tpnm T>
VIEW_SERIALIZE_TEST_RESULT testUIntValue(::llc::u3_c source) {
	cnst T value = (T)source;
	cnst ::llc::packed_uint<T> packed{value};
	cnst ::llc::u2_t expectedWidth = packed.ValueWidth();
	::llc::au0_t serialized;
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_SAVE_UINT_SIZE, 0 > serialized.append(PRECEDING_FIELD)
		, "%u-bit saveUInt() test could not append its preceding field for value:%" LLC_FMT_U3 "."
		, ::llc::u2_t(szof(T) * 8), source
		);
	cnst ::llc::err_t saveResult = ::llc::saveUInt(serialized, value);
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_SAVE_UINT_RESULT, saveResult != (::llc::err_t)expectedWidth
		, "%u-bit saveUInt() result mismatch for value:%" LLC_FMT_U3 ". result:%i, expected:%u."
		, ::llc::u2_t(szof(T) * 8), source, saveResult, expectedWidth
		);
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_SAVE_UINT_SIZE, serialized.size() != ::llc::size(PRECEDING_FIELD) + expectedWidth || 0 != memcmp(serialized.begin(), PRECEDING_FIELD, ::llc::size(PRECEDING_FIELD))
		, "%u-bit saveUInt() append mismatch for value:%" LLC_FMT_U3 ". size:%u, expected:%u, prefix bytes:%u."
		, ::llc::u2_t(szof(T) * 8), source, serialized.size(), ::llc::u2_t(::llc::size(PRECEDING_FIELD) + expectedWidth), ::llc::u2_t(::llc::size(PRECEDING_FIELD))
		);
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_SAVE_UINT_SIZE, 0 > serialized.append(FOLLOWING_FIELD)
		, "%u-bit saveUInt() test could not append its following field for value:%" LLC_FMT_U3 "."
		, ::llc::u2_t(szof(T) * 8), source
		);

	::llc::vcu0_t packedInput{&serialized[::llc::size(PRECEDING_FIELD)], serialized.size() - ::llc::size(PRECEDING_FIELD)};
	T loadedPacked = {};
	cnst ::llc::err_t packedResult = ::llc::loadPacked(packedInput, loadedPacked);
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_LOAD_PACKED_RESULT, packedResult != (::llc::err_t)expectedWidth
		, "%u-bit loadPacked() result mismatch for value:%" LLC_FMT_U3 ". result:%i, expected:%u."
		, ::llc::u2_t(szof(T) * 8), source, packedResult, expectedWidth
		);
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_LOAD_PACKED_VALUE, ::llc::u3_t(loadedPacked) != source
		, "%u-bit loadPacked() value mismatch. loaded:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "."
		, ::llc::u2_t(szof(T) * 8), ::llc::u3_t(loadedPacked), source
		);
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_LOAD_PACKED_POSITION, !isFollowingField(packedInput)
		, "%u-bit loadPacked() cursor mismatch for value:%" LLC_FMT_U3 ". remaining:%u, expected:%u."
		, ::llc::u2_t(szof(T) * 8), source, packedInput.size(), ::llc::u2_t(::llc::size(FOLLOWING_FIELD))
		);

	::llc::vcu0_t input{&serialized[::llc::size(PRECEDING_FIELD)], serialized.size() - ::llc::size(PRECEDING_FIELD)};
	T loaded = {};
	cnst ::llc::err_t loadResult = ::llc::loadUInt(input, loaded);
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_LOAD_UINT_RESULT, loadResult != (::llc::err_t)expectedWidth
		, "%u-bit loadUInt() result mismatch for value:%" LLC_FMT_U3 ". result:%i, expected:%u."
		, ::llc::u2_t(szof(T) * 8), source, loadResult, expectedWidth
		);
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_LOAD_UINT_VALUE, ::llc::u3_t(loaded) != source
		, "%u-bit loadUInt() value mismatch. loaded:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "."
		, ::llc::u2_t(szof(T) * 8), ::llc::u3_t(loaded), source
		);
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_LOAD_UINT_POSITION, !isFollowingField(input)
		, "%u-bit loadUInt() cursor mismatch for value:%" LLC_FMT_U3 ". remaining:%u, expected:%u."
		, ::llc::u2_t(szof(T) * 8), source, input.size(), ::llc::u2_t(::llc::size(FOLLOWING_FIELD))
		);
	return VIEW_SERIALIZE_TEST_RESULT_OK;
}

tplt<tpnm T>
VIEW_SERIALIZE_TEST_RESULT testUIntType() {
	VIEW_SERIALIZE_TEST_RESULT result = VIEW_SERIALIZE_TEST_RESULT_OK;
	stxp ::llc::u0_t MULTIPLIER_BITS = 8 - ::llc::uint_width_field_size<T>();
	::llc::u3_t previousMaximum = 0;
	for(::llc::u0_t tailWidth = 0; tailWidth < szof(T); ++tailWidth) {
		::llc::u0_c valueBits = MULTIPLIER_BITS + tailWidth * 8;
		::llc::u3_c maximum = (::llc::u3_t(1) << valueBits) - 1;
		::llc::u3_c minimum = tailWidth ? previousMaximum + 1 : 0;
		if(VIEW_SERIALIZE_TEST_RESULT_OK != (result = testUIntValue<T>(minimum))) return result;
		if(maximum != minimum && VIEW_SERIALIZE_TEST_RESULT_OK != (result = testUIntValue<T>(maximum))) return result;
		previousMaximum = maximum;
	}
	return VIEW_SERIALIZE_TEST_RESULT_OK;
}

tplt<tpnm T>
VIEW_SERIALIZE_TEST_RESULT testTruncatedUInt() {
	stxp ::llc::u0_t VALUE_BITS = 8 - ::llc::uint_width_field_size<T>() + (szof(T) - 1) * 8;
	cnst T value = (T)((::llc::u3_t(1) << VALUE_BITS) - 1);
	cnst ::llc::packed_uint<T> packed{value};
	cnst ::llc::vcu0_t bytes = packed.tplt cu8<::llc::vcu0_t>();
	for(::llc::u2_t byteCount = 0; byteCount < bytes.size(); ++byteCount) {
		::llc::vcu0_t input{bytes.begin(), byteCount};
		cnst ::llc::u0_t * originalPosition = input.begin();
		T loaded = {};
		::llc::setupLogCallbacks(0, 0);
		cnst ::llc::err_t loadResult = ::llc::loadUInt(input, loaded);
		::llc::setupDefaultLogCallbacks();
		if_true_vef(VIEW_SERIALIZE_TEST_RESULT_TRUNCATED_UINT, 0 <= loadResult || loaded || input.begin() != originalPosition || input.size() != byteCount
			, "%u-bit loadUInt() truncation mismatch. supplied:%u, required:%u, result:%i, loaded:%" LLC_FMT_U3 ", remaining:%u."
			, ::llc::u2_t(szof(T) * 8), byteCount, bytes.size(), loadResult, ::llc::u3_t(loaded), input.size()
			);
	}
	return VIEW_SERIALIZE_TEST_RESULT_OK;
}

tplt<tpnm T>
VIEW_SERIALIZE_TEST_RESULT testViewCount(::llc::u2_c elementCount) {
	::llc::apod<T> source;
	cnst ::llc::err_t resizeResult = source.resize(elementCount);
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_SOURCE_RESIZE, resizeResult != (::llc::err_t)elementCount
		, "%u-bit source resize mismatch. result:%i, expected:%u."
		, ::llc::u2_t(szof(T) * 8), resizeResult, elementCount
		);
	for(::llc::u2_t iElement = 0; iElement < elementCount; ++iElement)
		source[iElement] = T(::llc::u3_t(iElement) * 0x9E3779B1ULL + elementCount + szof(T));

	cnst ::llc::view<cnst T> expected = source;
	cnst ::llc::packedu32 countHeader = elementCount;
	cnst ::llc::u2_t headerWidth = countHeader.ValueWidth();
	cnst ::llc::u2_t expectedSize = headerWidth + expected.byte_count();
	::llc::au0_t serialized;
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_SAVE_VIEW_SIZE, 0 > serialized.append(PRECEDING_FIELD)
		, "%u-bit saveView() test could not append its preceding field for %u elements."
		, ::llc::u2_t(szof(T) * 8), elementCount
		);
	cnst ::llc::err_t saveResult = ::llc::saveView(serialized, expected);
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_SAVE_VIEW_RESULT, saveResult != (::llc::err_t)expectedSize
		, "%u-bit saveView() result mismatch for %u elements. result:%i, expected:%u."
		, ::llc::u2_t(szof(T) * 8), elementCount, saveResult, expectedSize
		);
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_SAVE_VIEW_SIZE, serialized.size() != ::llc::size(PRECEDING_FIELD) + expectedSize || 0 != memcmp(serialized.begin(), PRECEDING_FIELD, ::llc::size(PRECEDING_FIELD))
		, "%u-bit saveView() append mismatch for %u elements. size:%u, expected:%u, prefix:%u, header:%u, payload:%u."
		, ::llc::u2_t(szof(T) * 8), elementCount, serialized.size(), ::llc::u2_t(::llc::size(PRECEDING_FIELD) + expectedSize), ::llc::u2_t(::llc::size(PRECEDING_FIELD)), headerWidth, expected.byte_count()
		);

	cnst ::llc::vcu0_t exactInput{&serialized[::llc::size(PRECEDING_FIELD)], expectedSize};
	::llc::view<cnst T> readView;
	cnst ::llc::err_t readResult = ::llc::viewRead(readView, exactInput);
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_VIEW_READ_RESULT, readResult != (::llc::err_t)expectedSize
		, "%u-bit viewRead() result mismatch for %u elements. result:%i, expected:%u."
		, ::llc::u2_t(szof(T) * 8), elementCount, readResult, expectedSize
		);
	cnst T * expectedReadPosition = elementCount ? (cnst T*)(cnst void*)(serialized.begin() + ::llc::size(PRECEDING_FIELD) + headerWidth) : 0;
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_VIEW_READ_POSITION, readView.size() != elementCount || readView.begin() != expectedReadPosition
		, "%u-bit viewRead() position mismatch for %u elements. count:%u, begin:%p, expected begin:%p."
		, ::llc::u2_t(szof(T) * 8), elementCount, readView.size(), (cnst void*)readView.begin(), (cnst void*)expectedReadPosition
		);
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_VIEW_READ_VALUE, readView != expected
		, "%u-bit viewRead() value mismatch for %u elements."
		, ::llc::u2_t(szof(T) * 8), elementCount
		);
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_SAVE_VIEW_SIZE, 0 > serialized.append(FOLLOWING_FIELD)
		, "%u-bit view test could not append its following field for %u elements."
		, ::llc::u2_t(szof(T) * 8), elementCount
		);

	::llc::vcu0_t viewInput{&serialized[::llc::size(PRECEDING_FIELD)], serialized.size() - ::llc::size(PRECEDING_FIELD)};
	::llc::view<cnst T> loadedView;
	cnst ::llc::err_t viewLoadResult = ::llc::loadView(viewInput, loadedView);
	cnst T * expectedLoadPosition = elementCount ? (cnst T*)(cnst void*)(serialized.begin() + ::llc::size(PRECEDING_FIELD) + headerWidth) : 0;
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_VIEW_LOAD_RESULT, viewLoadResult
		, "%u-bit non-owning loadView() failed for %u elements. result:%i."
		, ::llc::u2_t(szof(T) * 8), elementCount, viewLoadResult
		);
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_VIEW_LOAD_POSITION, loadedView.begin() != expectedLoadPosition || loadedView.size() != elementCount || !isFollowingField(viewInput)
		, "%u-bit non-owning loadView() position mismatch for %u elements. count:%u, begin:%p, expected begin:%p, remaining:%u."
		, ::llc::u2_t(szof(T) * 8), elementCount, loadedView.size(), (cnst void*)loadedView.begin(), (cnst void*)expectedLoadPosition, viewInput.size()
		);
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_VIEW_LOAD_VALUE, loadedView != expected
		, "%u-bit non-owning loadView() value mismatch for %u elements."
		, ::llc::u2_t(szof(T) * 8), elementCount
		);

	::llc::vcu0_t ownedInput{&serialized[::llc::size(PRECEDING_FIELD)], serialized.size() - ::llc::size(PRECEDING_FIELD)};
	::llc::apod<T> loadedArray;
	cnst ::llc::err_t arrayLoadResult = ::llc::loadView(ownedInput, loadedArray);
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_APOD_LOAD_RESULT, arrayLoadResult
		, "%u-bit owned loadView() failed for %u elements. result:%i."
		, ::llc::u2_t(szof(T) * 8), elementCount, arrayLoadResult
		);
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_APOD_LOAD_POSITION, !isFollowingField(ownedInput)
		, "%u-bit owned loadView() cursor mismatch for %u elements. remaining:%u, expected:%u."
		, ::llc::u2_t(szof(T) * 8), elementCount, ownedInput.size(), ::llc::u2_t(::llc::size(FOLLOWING_FIELD))
		);
	cnst ::llc::view<cnst T> loadedValues = loadedArray;
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_APOD_LOAD_VALUE, loadedValues != expected || (elementCount && loadedValues.begin() == expected.begin())
		, "%u-bit owned loadView() value/ownership mismatch for %u elements. loaded count:%u, loaded begin:%p, source begin:%p."
		, ::llc::u2_t(szof(T) * 8), elementCount, loadedValues.size(), (cnst void*)loadedValues.begin(), (cnst void*)expected.begin()
		);
	return VIEW_SERIALIZE_TEST_RESULT_OK;
}

tplt<tpnm T>
VIEW_SERIALIZE_TEST_RESULT testViewType() {
	stxp ::llc::u2_t COUNTS[] = {0, 1, 2, 63, 64, 255, 16383, 16384};
	for(::llc::u2_c count : COUNTS) {
		cnst VIEW_SERIALIZE_TEST_RESULT result = testViewCount<T>(count);
		if(result) return result;
	}
	return VIEW_SERIALIZE_TEST_RESULT_OK;
}

tplt<tpnm T>
VIEW_SERIALIZE_TEST_RESULT testTruncatedView() {
	::llc::apod<T> source;
	cnst ::llc::u2_t elementCount = 64;
	cnst ::llc::err_t resizeResult = source.resize(elementCount);
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_SOURCE_RESIZE, resizeResult != (::llc::err_t)elementCount
		, "%u-bit truncation source resize mismatch. result:%i, expected:%u."
		, ::llc::u2_t(szof(T) * 8), resizeResult, elementCount
		);
	::llc::au0_t serialized;
	cnst ::llc::view<cnst T> sourceView = source;
	cnst ::llc::err_t saveResult = ::llc::saveView(serialized, sourceView);
	if_true_vef(VIEW_SERIALIZE_TEST_RESULT_SAVE_VIEW_RESULT, saveResult != (::llc::err_t)serialized.size()
		, "%u-bit truncation source serialization mismatch. result:%i, size:%u."
		, ::llc::u2_t(szof(T) * 8), saveResult, serialized.size()
		);
	cnst ::llc::u2_t headerWidth = ::llc::packedu32{elementCount}.ValueWidth();
	cnst ::llc::u2_t cuts[] = {0, headerWidth - 1, headerWidth, serialized.size() - 1};
	for(::llc::u2_t iCut = 0; iCut < ::llc::size(cuts); ++iCut) {
		cnst ::llc::u2_t byteCount = cuts[iCut];
		if(iCut && byteCount == cuts[iCut - 1]) continue;
		::llc::vcu0_t input{serialized.begin(), byteCount};
		cnst ::llc::u0_t * originalPosition = input.begin();
		::llc::view<cnst T> loaded;
		::llc::setupLogCallbacks(0, 0);
		cnst ::llc::err_t loadResult = ::llc::loadView(input, loaded);
		::llc::setupDefaultLogCallbacks();
		if_true_vef(VIEW_SERIALIZE_TEST_RESULT_TRUNCATED_VIEW, 0 <= loadResult || input.begin() != originalPosition || input.size() != byteCount || loaded.size() || loaded.begin()
			, "%u-bit loadView() truncation mismatch. supplied:%u, required:%u, header:%u, result:%i, remaining:%u, loaded count:%u, loaded begin:%p."
			, ::llc::u2_t(szof(T) * 8), byteCount, serialized.size(), headerWidth, loadResult, input.size(), loaded.size(), (cnst void*)loaded.begin()
			);
	}
	return VIEW_SERIALIZE_TEST_RESULT_OK;
}

tplt<tpnm T>
VIEW_SERIALIZE_TEST_RESULT testType() {
	VIEW_SERIALIZE_TEST_RESULT result = testUIntType<T>();
	if(result) return result;
	if(VIEW_SERIALIZE_TEST_RESULT_OK != (result = testTruncatedUInt<T>())) return result;
	if(VIEW_SERIALIZE_TEST_RESULT_OK != (result = testViewType<T>())) return result;
	return testTruncatedView<T>();
}

int testViewSerialize() {
	VIEW_SERIALIZE_TEST_RESULT testResult = VIEW_SERIALIZE_TEST_RESULT_OK;
	if_true_vef(testResult, testResult = testType<::llc::u0_t>(), " 8-bit suite failed. %s: %s", ::llc::get_value_namep(testResult), ::llc::get_value_descp(testResult)) else always_printf(" 8-bit suite OK.");;
	if_true_vef(testResult, testResult = testType<::llc::u1_t>(), "16-bit suite failed. %s: %s", ::llc::get_value_namep(testResult), ::llc::get_value_descp(testResult)) else always_printf("16-bit suite OK.");;
	if_true_vef(testResult, testResult = testType<::llc::u2_t>(), "32-bit suite failed. %s: %s", ::llc::get_value_namep(testResult), ::llc::get_value_descp(testResult)) else always_printf("32-bit suite OK.");;
	if_true_vef(testResult, testResult = testType<::llc::u3_t>(), "64-bit suite failed. %s: %s", ::llc::get_value_namep(testResult), ::llc::get_value_descp(testResult)) else always_printf("64-bit suite OK.");;
	return VIEW_SERIALIZE_TEST_RESULT_OK;
}
