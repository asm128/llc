#include "llc_test_core.h"
#include "llc_grid_serialize.h"
#include "llc_noise.h"


GDEFINE_ENUM_TYPE(GRID_SERIALIZE_TEST_RESULTRESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, OK								,  0, "All grid-serialization tests passed.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, SAVE_UINT_RESULT					,  1, "saveUInt() reported the wrong serialized width.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, SAVE_UINT_SIZE					,  2, "saveUInt() appended the wrong number of bytes.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, LOAD_UINT_RESULT					,  3, "loadUInt() reported the wrong consumed width.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, LOAD_UINT_VALUE					,  4, "loadUInt() did not restore the serialized integer.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, LOAD_UINT_POSITION				,  5, "loadUInt() did not leave the input at the following field.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, SOURCE_RESIZE						,  6, "The source array could not be prepared for a grid test.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, SAVE_GRID_RESULT					,  7, "saveGRID() reported the wrong serialized size.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, SAVE_GRID_SIZE					,  8, "saveGRID() appended the wrong header or payload size.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, GRID_READ_RESULT					,  9, "gridRead() reported the wrong header-plus-payload size.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, GRID_READ_POSITION				, 10, "gridRead() produced the wrong element count or alias position.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, GRID_READ_VALUE					, 11, "gridRead() produced an alias with different values.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, GRID_LOAD_RESULT					, 12, "loadGRID() failed to restore a non-owning grid.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, GRID_LOAD_POSITION				, 13, "Non-owning loadGRID() consumed the wrong bytes or returned the wrong alias.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, GRID_LOAD_VALUE					, 14, "Non-owning loadGRID() restored different values.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, APOD_LOAD_RESULT					, 15, "loadGRID() failed to restore an owned POD array.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, APOD_LOAD_POSITION				, 16, "Owned loadGRID() consumed the wrong number of bytes.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, APOD_LOAD_VALUE					, 17, "Owned loadGRID() restored a different array.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, TRUNCATED_UINT					, 18, "loadUInt() accepted a truncated packed integer or modified its output/cursor.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, TRUNCATED_GRID					, 19, "loadGRID() accepted a truncated header or payload or modified its cursor.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, LOAD_PACKED_RESULT				, 20, "loadPacked() reported the wrong consumed width.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, LOAD_PACKED_VALUE					, 21, "loadPacked() did not decode the serialized packed value.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, LOAD_PACKED_POSITION				, 22, "loadPacked() did not leave the input at the following field.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, SAVE_POD_RESULT					, 23, "savePOD() reported the wrong serialized width.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, SAVE_POD_SIZE						, 24, "savePOD() appended the wrong bytes.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, LOAD_POD_RESULT					, 25, "loadPOD() reported the wrong consumed width.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, LOAD_POD_VALUE					, 26, "loadPOD() did not restore the serialized value.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, LOAD_POD_POSITION					, 27, "loadPOD() did not leave the input at the following field.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, TRUNCATED_POD						, 28, "loadPOD() accepted truncated input or modified its output/cursor.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, TRUNCATED_OWNED_GRID				, 29, "Owned loadGRID() accepted truncated input or modified its output/cursor.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, MALFORMED_GRID_COUNT				, 30, "loadGRID() accepted a count whose payload byte size overflowed.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, LEGACY_MUTABLE_RESULT				, 31, "Mutable gridPascal() reported the wrong consumed byte count.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, LEGACY_MUTABLE_ALIAS				, 32, "Mutable gridPascal() returned the wrong count or alias.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, LEGACY_MUTABLE_VALUE				, 33, "Mutable gridPascal() returned different values.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, LEGACY_MUTABLE_WRITE				, 34, "Mutable gridPascal() did not expose writable source storage.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, LEGACY_CONST_RESULT				, 35, "Const gridPascal() reported the wrong consumed byte count.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, LEGACY_CONST_ALIAS				, 36, "Const gridPascal() returned the wrong count or alias.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, LEGACY_CONST_VALUE				, 37, "Const gridPascal() returned different values.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, LEGACY_TRUNCATED_MUTABLE_RESULT	, 38, "Mutable gridPascal() accepted truncated input.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, LEGACY_TRUNCATED_MUTABLE_OUTPUT	, 39, "Mutable gridPascal() modified its output after rejecting truncated input.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, LEGACY_TRUNCATED_CONST_RESULT		, 40, "Const gridPascal() accepted truncated input.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, LEGACY_TRUNCATED_CONST_OUTPUT		, 41, "Const gridPascal() modified its output after rejecting truncated input.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, LEGACY_MALFORMED_COUNT_RESULT		, 42, "gridPascal() accepted a count larger than the available payload.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, LEGACY_MALFORMED_COUNT_OUTPUT		, 43, "gridPascal() modified its output after rejecting a malformed count.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, GRID_MUTABLE_RESULT				, 44, "Mutable gridPascal() reported the wrong consumed byte count.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, GRID_MUTABLE_ALIAS				, 45, "Mutable gridPascal() returned the wrong dimensions or alias.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, GRID_MUTABLE_VALUE				, 46, "Mutable gridPascal() returned different cell values.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, GRID_MUTABLE_WRITE				, 47, "Mutable gridPascal() did not expose writable source storage.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, GRID_CONST_RESULT					, 48, "Const gridPascal() reported the wrong consumed byte count.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, GRID_CONST_ALIAS					, 49, "Const gridPascal() returned the wrong dimensions or alias.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, GRID_CONST_VALUE					, 50, "Const gridPascal() returned different cell values.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, GRID_TRUNCATED_RESULT				, 51, "gridPascal() accepted truncated input.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, GRID_TRUNCATED_OUTPUT				, 52, "gridPascal() modified its output after rejecting truncated input.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, GRID_MALFORMED_RESULT				, 53, "gridPascal() accepted overflowing dimensions.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULTRESULT, GRID_MALFORMED_OUTPUT				, 54, "gridPascal() modified its output after rejecting overflowing dimensions.");

stxp ::llc::u0_t PRECEDING_FIELD[] = {0x91U, 0xE4U, 0x2BU};
stxp ::llc::u0_t FOLLOWING_FIELD[] = {0xC3U, 0x5AU, 0x7EU, 0x19U};
stxp ::llc::u3_t SERIALIZE_UINT_RANDOM_SEED	= 0x53455249414C495AULL;
stxp ::llc::u3_t SERIALIZE_GRID_RANDOM_SEED	= 0x56494557434F554EULL;
stxp ::llc::u2_t SERIALIZE_UINT_RANDOM_COUNT	= 256;
stxp ::llc::u2_t SERIALIZE_GRID_RANDOM_COUNT	= 32;

tplt<tpnm TOutput, tpnm TInput>
concept canGRIDReadLegacy = requires(TOutput & output, TInput input) { ::llc::gridPascal(output, input); };

tplt<tpnm TOutput, tpnm TInput>
concept canGridReadLegacy = requires(TOutput & output, TInput input) { ::llc::gridPascal(output, input); };

static_assert( canGRIDReadLegacy<::llc::grid<::llc::u2_t>, ::llc::vu0_t>);
static_assert( canGRIDReadLegacy<::llc::grid<cnst ::llc::u2_t>, ::llc::vcu0_t>);
static_assert( canGRIDReadLegacy<::llc::grid<cnst ::llc::u2_t>, ::llc::vu0_t>);
static_assert(false == canGRIDReadLegacy<::llc::grid<::llc::u2_t>, ::llc::vcu0_t>);
static_assert(false == canGRIDReadLegacy<::llc::grid<::llc::u2_t>, ::llc::vcst_t>);
static_assert( canGridReadLegacy<::llc::grid<::llc::u2_t>, ::llc::vu0_t>);
static_assert( canGridReadLegacy<::llc::grid<cnst ::llc::u2_t>, ::llc::vcu0_t>);
static_assert( canGridReadLegacy<::llc::grid<cnst ::llc::u2_t>, ::llc::vu0_t>);
static_assert(false == canGridReadLegacy<::llc::grid<::llc::u2_t>, ::llc::vcu0_t>);
static_assert(false == canGridReadLegacy<::llc::grid<::llc::u2_t>, ::llc::vcst_t>);

sttc bool isFollowingField(cnst ::llc::vcu0_t & input) {
	return input.size() == ::llc::size(FOLLOWING_FIELD) && 0 == memcmp(input.begin(), FOLLOWING_FIELD, ::llc::size(FOLLOWING_FIELD));
}

tplt<tpnm T>
sttc ::llc::err_t testPOD(ATestError & errors) {
	cnst T source = T(0x5AU);
	::llc::au0_t serialized;
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_SAVE_POD_SIZE, 0 > serialized.append(PRECEDING_FIELD) , "%u-bit savePOD() test could not append its preceding field." , bcof(T) );
	cnst ::llc::err_t saveResult = ::llc::savePOD(serialized, source);
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_SAVE_POD_RESULT, saveResult != szof(T) , "%u-bit savePOD() result mismatch. result:%i, expected:%u." , bcof(T), saveResult, ::llc::u2_t(szof(T)) );
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_SAVE_POD_SIZE, serialized.size() != ::llc::size(PRECEDING_FIELD) + szof(T) , "%u-bit savePOD() size mismatch. size:%u, expected:%u." , bcof(T), serialized.size(), ::llc::u2_t(::llc::size(PRECEDING_FIELD) + szof(T)) );
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_SAVE_POD_SIZE, 0 != memcmp(serialized.begin(), PRECEDING_FIELD, ::llc::size(PRECEDING_FIELD)) , "%u-bit savePOD() modified its preceding field." , bcof(T) );
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_SAVE_POD_SIZE, 0 != memcmp(&serialized[::llc::size(PRECEDING_FIELD)], &source, szof(T)) , "%u-bit savePOD() payload differs from the source." , bcof(T) );
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_SAVE_POD_SIZE, 0 > serialized.append(FOLLOWING_FIELD) , "%u-bit savePOD() test could not append its following field." , bcof(T) );
	::llc::vcu0_t input{&serialized[::llc::size(PRECEDING_FIELD)], serialized.size() - ::llc::size(PRECEDING_FIELD)};
	T loaded = {};
	cnst ::llc::err_t loadResult = ::llc::loadPOD(input, loaded);
	always_printf("SERIALIZE POD %u-bit input:%" LLC_FMT_U3 "; save:%i load:%i; expected/restored:%" LLC_FMT_U3 "/%" LLC_FMT_U3 "; trailing bytes:%u expected:%u.", (::llc::u2_t)bcof(T), (::llc::u3_t)source, saveResult, loadResult, (::llc::u3_t)source, (::llc::u3_t)loaded, input.size(), (::llc::u2_t)::llc::size(FOLLOWING_FIELD));
	::testTraceBytes("Complete stream (preceding field + POD + following field)", serialized);
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LOAD_POD_RESULT, loadResult != szof(T) , "%u-bit loadPOD() result mismatch. result:%i, expected:%u." , bcof(T), loadResult, ::llc::u2_t(szof(T)) );
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LOAD_POD_VALUE, loaded != source , "%u-bit loadPOD() value mismatch. loaded:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "." , bcof(T), ::llc::u3_t(loaded), ::llc::u3_t(source) );
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LOAD_POD_POSITION, !isFollowingField(input) , "%u-bit loadPOD() cursor mismatch. remaining:%u, expected:%u." , bcof(T), input.size(), ::llc::u2_t(::llc::size(FOLLOWING_FIELD)) );
	cnst T sentinel = (T)~source;
	for(::llc::u2_t byteCount = 0; byteCount < szof(T); ++byteCount) {
		::llc::vcu0_t truncated{&serialized[::llc::size(PRECEDING_FIELD)], byteCount};
		cnst ::llc::vcu0_t originalInput = truncated;
		T unchanged = sentinel;
		cnst ::llc::err_t truncatedResult = ::llc::loadPOD(truncated, unchanged);
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_TRUNCATED_POD, 0 <= truncatedResult , "%u-bit loadPOD() accepted truncated input. supplied:%u, required:%u, result:%i." , bcof(T), byteCount, ::llc::u2_t(szof(T)), truncatedResult );
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_TRUNCATED_POD, unchanged != sentinel , "%u-bit loadPOD() modified output on truncated input. supplied:%u, value:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "." , bcof(T), byteCount, ::llc::u3_t(unchanged), ::llc::u3_t(sentinel) );
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_TRUNCATED_POD, truncated != originalInput , "%u-bit loadPOD() moved cursor on truncated input. supplied:%u, remaining:%u, expected:%u." , bcof(T), byteCount, truncated.size(), originalInput.size() );
	}
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testUIntValue(ATestError & errors, ::llc::u3_c source, bool trace = true) {
	cnst T value = (T)source;
	cnst ::llc::packed_uint<T> packed{value};
	cnst ::llc::u2_t expectedWidth = packed.ValueWidth();
	::llc::au0_t serialized;
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_SAVE_UINT_SIZE, 0 > serialized.append(PRECEDING_FIELD) , "%u-bit saveUInt() test could not append its preceding field for value:%" LLC_FMT_U3 "." , bcof(T), source );
	cnst ::llc::err_t saveResult = ::llc::saveUInt(serialized, value);
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_SAVE_UINT_RESULT, saveResult != (::llc::err_t)expectedWidth , "%u-bit saveUInt() result mismatch for value:%" LLC_FMT_U3 ". result:%i, expected:%u." , bcof(T), source, saveResult, expectedWidth );
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_SAVE_UINT_SIZE, serialized.size() != ::llc::size(PRECEDING_FIELD) + expectedWidth , "%u-bit saveUInt() size mismatch for value:%" LLC_FMT_U3 ". size:%u, expected:%u." , bcof(T), source, serialized.size(), ::llc::u2_t(::llc::size(PRECEDING_FIELD) + expectedWidth) );
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_SAVE_UINT_SIZE, 0 != memcmp(serialized.begin(), PRECEDING_FIELD, ::llc::size(PRECEDING_FIELD)) , "%u-bit saveUInt() modified its preceding field for value:%" LLC_FMT_U3 "." , bcof(T), source );
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_SAVE_UINT_SIZE, 0 > serialized.append(FOLLOWING_FIELD) , "%u-bit saveUInt() test could not append its following field for value:%" LLC_FMT_U3 "." , bcof(T), source );

	::llc::vcu0_t packedInput{&serialized[::llc::size(PRECEDING_FIELD)], serialized.size() - ::llc::size(PRECEDING_FIELD)};
	T loadedPacked = {};
	cnst ::llc::err_t packedResult = ::llc::loadPacked(packedInput, loadedPacked);
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LOAD_PACKED_RESULT, packedResult != (::llc::err_t)expectedWidth , "%u-bit loadPacked() result mismatch for value:%" LLC_FMT_U3 ". result:%i, expected:%u." , bcof(T), source, packedResult, expectedWidth );
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LOAD_PACKED_VALUE, ::llc::u3_t(loadedPacked) != source , "%u-bit loadPacked() value mismatch. loaded:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "." , bcof(T), ::llc::u3_t(loadedPacked), source );
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LOAD_PACKED_POSITION, !isFollowingField(packedInput) , "%u-bit loadPacked() cursor mismatch for value:%" LLC_FMT_U3 ". remaining:%u, expected:%u." , bcof(T), source, packedInput.size(), ::llc::u2_t(::llc::size(FOLLOWING_FIELD)) );

	::llc::vcu0_t input{&serialized[::llc::size(PRECEDING_FIELD)], serialized.size() - ::llc::size(PRECEDING_FIELD)};
	T loaded = {};
	cnst ::llc::err_t loadResult = ::llc::loadUInt(input, loaded);
	if(trace) {
		always_printf("SERIALIZE UINT %u-bit input:%" LLC_FMT_U3 "; expected width:%u; save:%i load:%i; restored:%" LLC_FMT_U3 "; trailing bytes:%u expected:%u.", (::llc::u2_t)bcof(T), source, expectedWidth, saveResult, loadResult, (::llc::u3_t)loaded, input.size(), (::llc::u2_t)::llc::size(FOLLOWING_FIELD));
		::testTraceBytes("Complete stream (preceding + packed integer + following)", serialized);
	}
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LOAD_UINT_RESULT, loadResult != (::llc::err_t)expectedWidth , "%u-bit loadUInt() result mismatch for value:%" LLC_FMT_U3 ". result:%i, expected:%u." , bcof(T), source, loadResult, expectedWidth );
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LOAD_UINT_VALUE, ::llc::u3_t(loaded) != source , "%u-bit loadUInt() value mismatch. loaded:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "." , bcof(T), ::llc::u3_t(loaded), source );
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LOAD_UINT_POSITION, !isFollowingField(input) , "%u-bit loadUInt() cursor mismatch for value:%" LLC_FMT_U3 ". remaining:%u, expected:%u." , bcof(T), source, input.size(), ::llc::u2_t(::llc::size(FOLLOWING_FIELD)) );
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
	cnst ::llc::u3_t seed = testSeed(SERIALIZE_UINT_RANDOM_SEED);
	always_printf("SERIALIZE generated integer coverage: %u-bit, seed:%" LLC_FMT_U3 ", cases:%u; failures retain counterexamples.", (::llc::u2_t)bcof(T), seed, SERIALIZE_UINT_RANDOM_COUNT);
	::llc::SPRNG random = {seed};
	stxp ::llc::u0_t VALUE_BITS = 8 - ::llc::uint_width_field_size<T>() + (szof(T) - 1) * 8;
	stxp ::llc::u3_t VALUE_MASK = (::llc::u3_t(1) << VALUE_BITS) - 1;
	for(::llc::u2_t iRandom = 0; iRandom < SERIALIZE_UINT_RANDOM_COUNT; ++iRandom) {
		cnst T value = T(random.Next() & VALUE_MASK);
		cnst ::llc::u2_t failureCount = testErrorCount(errors);
		if_fail_fe(testUIntValue<T>(errors, value, false));
		if(failureCount != testErrorCount(errors))
			error_printf("%u-bit randomized integer serialization case failed. seed:%" LLC_FMT_U3 ", iteration:%u, value:%" LLC_FMT_U3 "."
				, bcof(T), seed, iRandom, ::llc::u3_t(value)
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
		cnst ::llc::err_t loadResult = ::llc::loadUInt(input, loaded);
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_TRUNCATED_UINT, 0 <= loadResult , "%u-bit loadUInt() accepted truncated input. supplied:%u, required:%u, result:%i." , bcof(T), byteCount, bytes.size(), loadResult );
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_TRUNCATED_UINT, loaded != sentinel , "%u-bit loadUInt() modified output on truncated input. supplied:%u, loaded:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "." , bcof(T), byteCount, ::llc::u3_t(loaded), ::llc::u3_t(sentinel) );
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_TRUNCATED_UINT, input != originalInput , "%u-bit loadUInt() moved cursor on truncated input. supplied:%u, remaining:%u, expected:%u." , bcof(T), byteCount, input.size(), originalInput.size() );
	}
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testGRIDCount(ATestError & errors, ::llc::u2_c elementCount) {
	::llc::apod<T> source;
	cnst ::llc::err_t resizeResult = source.resize(elementCount);
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_SOURCE_RESIZE, resizeResult != (::llc::err_t)elementCount , "%u-bit source resize mismatch. result:%i, expected:%u." , bcof(T), resizeResult, elementCount );
	for(::llc::u2_t iElement = 0; iElement < elementCount; ++iElement)
		source[iElement] = T(::llc::u3_t(iElement) * 0x9E3779B1ULL + elementCount + szof(T));

	cnst ::llc::grid<cnst T> expected = source;
	cnst ::llc::packedu32 countHeader = elementCount;
	cnst ::llc::u2_t headerWidth = countHeader.ValueWidth();
	cnst ::llc::u2_t expectedSize = headerWidth + expected.byte_count();
	::llc::au0_t serialized;
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_SAVE_GRID_SIZE, 0 > serialized.append(PRECEDING_FIELD) , "%u-bit saveGRID() test could not append its preceding field for %u elements." , bcof(T), elementCount );
	cnst ::llc::err_t saveResult = ::llc::saveGRID(serialized, expected);
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_SAVE_GRID_RESULT, saveResult != (::llc::err_t)expectedSize , "%u-bit saveGRID() result mismatch for %u elements. result:%i, expected:%u." , bcof(T), elementCount, saveResult, expectedSize );
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_SAVE_GRID_SIZE, serialized.size() != ::llc::size(PRECEDING_FIELD) + expectedSize , "%u-bit saveGRID() append-size mismatch for %u elements. size:%u, expected:%u, prefix:%u, header:%u, payload:%u." , bcof(T), elementCount, serialized.size(), ::llc::u2_t(::llc::size(PRECEDING_FIELD) + expectedSize), ::llc::u2_t(::llc::size(PRECEDING_FIELD)), headerWidth, expected.byte_count() );
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_SAVE_GRID_SIZE, 0 != memcmp(serialized.begin(), PRECEDING_FIELD, ::llc::size(PRECEDING_FIELD)) , "%u-bit saveGRID() modified its preceding field for %u elements. prefix:%u, header:%u, payload:%u." , bcof(T), elementCount, ::llc::u2_t(::llc::size(PRECEDING_FIELD)), headerWidth, expected.byte_count() );

	cnst ::llc::vcu0_t exactInput{&serialized[::llc::size(PRECEDING_FIELD)], expectedSize};
	::llc::grid<cnst T> readGRID;
	cnst ::llc::err_t readResult = ::llc::gridRead(readGRID, exactInput);
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_GRID_READ_RESULT, readResult != (::llc::err_t)expectedSize , "%u-bit gridRead() result mismatch for %u elements. result:%i, expected:%u." , bcof(T), elementCount, readResult, expectedSize );
	cnst ::llc::vcu0_t expectedReadBytes = elementCount
		? ::llc::vcu0_t{&serialized[::llc::size(PRECEDING_FIELD) + headerWidth], expected.byte_count()}
		: ::llc::vcu0_t{}
		;
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_GRID_READ_POSITION, readGRID.size() != elementCount , "%u-bit gridRead() count mismatch. count:%u, expected:%u." , bcof(T), readGRID.size(), elementCount );
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_GRID_READ_POSITION, readGRID.cu8().begin() != expectedReadBytes.begin() , "%u-bit gridRead() alias position mismatch for %u elements. begin:%p, expected:%p." , bcof(T), elementCount, readGRID.begin(), expectedReadBytes.begin() );
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_GRID_READ_VALUE, readGRID != expected , "%u-bit gridRead() value mismatch for %u elements." , bcof(T), elementCount );
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_SAVE_GRID_SIZE, 0 > serialized.append(FOLLOWING_FIELD) , "%u-bit grid test could not append its following field for %u elements." , bcof(T), elementCount );

	::llc::vcu0_t gridInput{&serialized[::llc::size(PRECEDING_FIELD)], serialized.size() - ::llc::size(PRECEDING_FIELD)};
	::llc::grid<cnst T> loadedGRID;
	cnst ::llc::err_t gridLoadResult = ::llc::loadGRID(gridInput, loadedGRID);
	cnst ::llc::vcu0_t expectedLoadBytes = elementCount
		? ::llc::vcu0_t{&serialized[::llc::size(PRECEDING_FIELD) + headerWidth], expected.byte_count()}
		: ::llc::vcu0_t{}
		;
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_GRID_LOAD_RESULT, gridLoadResult , "%u-bit non-owning loadGRID() failed for %u elements. result:%i." , bcof(T), elementCount, gridLoadResult );
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_GRID_LOAD_POSITION, loadedGRID.cu8().begin() != expectedLoadBytes.begin() , "%u-bit non-owning loadGRID() alias position mismatch for %u elements. begin:%p, expected:%p." , bcof(T), elementCount, loadedGRID.begin(), expectedLoadBytes.begin() );
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_GRID_LOAD_POSITION, loadedGRID.size() != elementCount , "%u-bit non-owning loadGRID() count mismatch. count:%u, expected:%u." , bcof(T), loadedGRID.size(), elementCount );
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_GRID_LOAD_POSITION, !isFollowingField(gridInput) , "%u-bit non-owning loadGRID() cursor mismatch for %u elements. remaining:%u, expected:%u." , bcof(T), elementCount, gridInput.size(), ::llc::u2_t(::llc::size(FOLLOWING_FIELD)) );
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_GRID_LOAD_VALUE, loadedGRID != expected , "%u-bit non-owning loadGRID() value mismatch for %u elements." , bcof(T), elementCount );

	::llc::vcu0_t ownedInput{&serialized[::llc::size(PRECEDING_FIELD)], serialized.size() - ::llc::size(PRECEDING_FIELD)};
	::llc::apod<T> loadedArray;
	cnst ::llc::err_t arrayLoadResult = ::llc::loadGRID(ownedInput, loadedArray);
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_APOD_LOAD_RESULT, arrayLoadResult , "%u-bit owned loadGRID() failed for %u elements. result:%i." , bcof(T), elementCount, arrayLoadResult );
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_APOD_LOAD_POSITION, !isFollowingField(ownedInput) , "%u-bit owned loadGRID() cursor mismatch for %u elements. remaining:%u, expected:%u." , bcof(T), elementCount, ownedInput.size(), ::llc::u2_t(::llc::size(FOLLOWING_FIELD)) );
	cnst ::llc::grid<cnst T> loadedValues = loadedArray;
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_APOD_LOAD_VALUE, loadedValues != expected , "%u-bit owned loadGRID() value mismatch for %u elements. loaded count:%u." , bcof(T), elementCount, loadedValues.size() );
	if(elementCount) {
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_APOD_LOAD_VALUE, loadedValues.begin() == expected.begin() , "%u-bit owned loadGRID() aliases its source for %u elements. loaded begin:%p, source begin:%p." , bcof(T), elementCount, loadedValues.begin(), expected.begin() );
	}
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testGRIDType(ATestError & errors) {
	stxp ::llc::u2_t COUNTS[] = {0, 1, 2, 63, 64, 255, 16383, 16384};
	for(::llc::u2_c count : COUNTS)
		if_fail_fe(testGRIDCount<T>(errors, count));
	cnst ::llc::u3_t seed = testSeed(SERIALIZE_GRID_RANDOM_SEED);
	::llc::SPRNG random = {seed};
	for(::llc::u2_t iRandom = 0; iRandom < SERIALIZE_GRID_RANDOM_COUNT; ++iRandom) {
		cnst ::llc::u2_t elementCount = random.Next() & 0xFFFU;
		cnst ::llc::u2_t failureCount = testErrorCount(errors);
		if_fail_fe(testGRIDCount<T>(errors, elementCount));
		if(failureCount != testErrorCount(errors))
			error_printf("%u-bit randomized grid serialization case failed. seed:%" LLC_FMT_U3 ", iteration:%u, element count:%u."
				, bcof(T), seed, iRandom, elementCount
				);
	}
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testTruncatedGRID(ATestError & errors) {
	::llc::apod<T> source;
	cnst ::llc::u2_t elementCount = 64;
	cnst ::llc::err_t resizeResult = source.resize(elementCount);
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_SOURCE_RESIZE, resizeResult != (::llc::err_t)elementCount , "%u-bit truncation source resize mismatch. result:%i, expected:%u." , bcof(T), resizeResult, elementCount );
	::llc::au0_t serialized;
	cnst ::llc::grid<cnst T> sourceGRID = source;
	cnst ::llc::err_t saveResult = ::llc::saveGRID(serialized, sourceGRID);
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_SAVE_GRID_RESULT, saveResult != (::llc::err_t)serialized.size() , "%u-bit truncation source serialization mismatch. result:%i, size:%u." , bcof(T), saveResult, serialized.size() );
	cnst ::llc::u2_t headerWidth = ::llc::packedu32{elementCount}.ValueWidth();
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_SAVE_GRID_RESULT, serialized.size() <= headerWidth , "%u-bit truncation source serialization was too short. size:%u, header:%u." , bcof(T), serialized.size(), headerWidth );
	cnst ::llc::u2_t cuts[] = {0, headerWidth - 1, headerWidth, serialized.size() - 1};
	cnst T sentinel = (T)~T(0);
	for(::llc::u2_t iCut = 0; iCut < ::llc::size(cuts); ++iCut) {
		cnst ::llc::u2_t byteCount = cuts[iCut];
		if(iCut && byteCount == cuts[iCut - 1]) continue;
		::llc::vcu0_t input{serialized.begin(), byteCount};
		cnst ::llc::vcu0_t originalInput = input;
		::llc::grid<cnst T> loaded{&sentinel, 1};
		cnst ::llc::err_t loadResult = ::llc::loadGRID(input, loaded);
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_TRUNCATED_GRID, 0 <= loadResult , "%u-bit loadGRID() accepted truncated input. supplied:%u, required:%u, header:%u, result:%i." , bcof(T), byteCount, serialized.size(), headerWidth, loadResult );
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_TRUNCATED_GRID, input != originalInput , "%u-bit loadGRID() moved cursor on truncated input. supplied:%u, remaining:%u, expected:%u." , bcof(T), byteCount, input.size(), originalInput.size() );
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_TRUNCATED_GRID, loaded.size() != 1 , "%u-bit loadGRID() changed output count on truncated input. supplied:%u, count:%u, expected:1." , bcof(T), byteCount, loaded.size() );
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_TRUNCATED_GRID, loaded.begin() != &sentinel , "%u-bit loadGRID() changed output alias on truncated input. supplied:%u, begin:%p, expected:%p." , bcof(T), byteCount, loaded.begin(), &sentinel );
		::llc::vcu0_t ownedInput{serialized.begin(), byteCount};
		cnst ::llc::vcu0_t originalOwnedInput = ownedInput;
		::llc::apod<T> owned = {sentinel};
		cnst ::llc::err_t ownedResult = ::llc::loadGRID(ownedInput, owned);
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_TRUNCATED_OWNED_GRID, 0 <= ownedResult , "%u-bit owned loadGRID() accepted truncated input. supplied:%u, required:%u, header:%u, result:%i." , bcof(T), byteCount, serialized.size(), headerWidth, ownedResult );
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_TRUNCATED_OWNED_GRID, ownedInput != originalOwnedInput , "%u-bit owned loadGRID() moved cursor on truncated input. supplied:%u, remaining:%u, expected:%u." , bcof(T), byteCount, ownedInput.size(), originalOwnedInput.size() );
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_TRUNCATED_OWNED_GRID, owned.size() != 1 , "%u-bit owned loadGRID() changed output count on truncated input. supplied:%u, count:%u, expected:1." , bcof(T), byteCount, owned.size() );
		if(owned.size()) {
			LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_TRUNCATED_OWNED_GRID, owned[0] != sentinel , "%u-bit owned loadGRID() changed output value on truncated input. supplied:%u, value:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "." , bcof(T), byteCount, ::llc::u3_t(owned[0]), ::llc::u3_t(sentinel) );
		}
	}
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testMalformedGRIDCount(ATestError & errors) {
	if(8 != szof(T))
		return 0;
	stxp ::llc::u2_t ELEMENT_COUNT = 0x20000000U;
	::llc::au0_t serialized;
	cnst ::llc::err_t saveResult = ::llc::saveUInt(serialized, ELEMENT_COUNT);
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_MALFORMED_GRID_COUNT, saveResult <= 0 , "%u-bit malformed-count test could not serialize count:%u. result:%i." , bcof(T), ELEMENT_COUNT, saveResult );
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_MALFORMED_GRID_COUNT, serialized.size() != (::llc::u2_t)saveResult , "%u-bit malformed-count serialized size mismatch. count:%u, size:%u, expected:%u." , bcof(T), ELEMENT_COUNT, serialized.size(), ::llc::u2_t(saveResult) );
	::llc::vcu0_t input = serialized;
	cnst ::llc::vcu0_t originalInput = input;
	cnst T sentinel = (T)~T(0);
	::llc::grid<cnst T> loaded{&sentinel, 1};
	cnst ::llc::err_t loadResult = ::llc::loadGRID(input, loaded);
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_MALFORMED_GRID_COUNT, 0 <= loadResult , "%u-bit loadGRID() accepted overflowing count. declared elements:%u, supplied bytes:%u, result:%i." , bcof(T), ELEMENT_COUNT, serialized.size(), loadResult );
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_MALFORMED_GRID_COUNT, input != originalInput , "%u-bit loadGRID() moved cursor for overflowing count. remaining:%u, expected:%u." , bcof(T), input.size(), originalInput.size() );
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_MALFORMED_GRID_COUNT, loaded.begin() != &sentinel , "%u-bit loadGRID() changed output alias for overflowing count. begin:%p, expected:%p." , bcof(T), loaded.begin(), &sentinel );
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_MALFORMED_GRID_COUNT, loaded.size() != 1 , "%u-bit loadGRID() changed output count for overflowing count. count:%u, expected:1." , bcof(T), loaded.size() );
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testLegacyGRID(ATestError & errors) {
	T source[] = {T(0x11U), T(0x22U), T(0x33U), T(0x44U)};
	cnst ::llc::u2_t elementCount = ::llc::size(source);
	::llc::au0_t serialized = {};
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LEGACY_MUTABLE_RESULT, ::llc::savePOD(serialized, elementCount) != szof(elementCount), "%u-bit legacy test could not serialize its element count.", bcof(T));
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LEGACY_MUTABLE_RESULT, 0 > serialized.append(::llc::grid<cnst T>{source}.cu8()), "%u-bit legacy test could not serialize its elements.", bcof(T));
	cnst ::llc::u2_t expectedSize = szof(elementCount) + szof(source);
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LEGACY_MUTABLE_RESULT, serialized.size() != expectedSize, "%u-bit legacy serialized size:%u, expected:%u.", bcof(T), serialized.size(), expectedSize);

	::llc::grid<T> mutableGRID = {};
	cnst ::llc::err_t mutableResult = ::llc::gridPascal(mutableGRID, (::llc::vu0_t)serialized);
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LEGACY_MUTABLE_RESULT, mutableResult != expectedSize, "%u-bit mutable result:%i, expected:%u.", bcof(T), mutableResult, expectedSize);
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LEGACY_MUTABLE_ALIAS, mutableGRID.size() != elementCount, "%u-bit mutable count:%u, expected:%u.", bcof(T), mutableGRID.size(), elementCount);
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LEGACY_MUTABLE_ALIAS, mutableGRID.cu8().begin() != &serialized[szof(elementCount)], "%u-bit mutable begin:%p, expected:%p.", bcof(T), mutableGRID.begin(), &serialized[szof(elementCount)]);
	for(::llc::u2_t iElement = 0; iElement < elementCount; ++iElement)
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LEGACY_MUTABLE_VALUE, mutableGRID[iElement] != source[iElement], "%u-bit mutable element:%u, value:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 ".", bcof(T), iElement, ::llc::u3_t(mutableGRID[iElement]), ::llc::u3_t(source[iElement]));
	mutableGRID[2] = T(0x5AU);
	T storedValue = {};
	memcpy(&storedValue, &serialized[szof(elementCount) + 2 * szof(T)], szof(T));
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LEGACY_MUTABLE_WRITE, mutableGRID[2] != storedValue, "%u-bit mutable write:%" LLC_FMT_U3 ", source:%" LLC_FMT_U3 ".", bcof(T), ::llc::u3_t(mutableGRID[2]), ::llc::u3_t(storedValue));

	::llc::grid<cnst T> constGRID = {};
	cnst ::llc::err_t constResult = ::llc::gridPascal(constGRID, (::llc::vcu0_t)serialized);
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LEGACY_CONST_RESULT, constResult != expectedSize, "%u-bit const result:%i, expected:%u.", bcof(T), constResult, expectedSize);
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LEGACY_CONST_ALIAS, constGRID.size() != elementCount, "%u-bit const count:%u, expected:%u.", bcof(T), constGRID.size(), elementCount);
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LEGACY_CONST_ALIAS, constGRID.cu8().begin() != &serialized[szof(elementCount)], "%u-bit const begin:%p, expected:%p.", bcof(T), constGRID.begin(), &serialized[szof(elementCount)]);
	for(::llc::u2_t iElement = 0; iElement < elementCount; ++iElement)
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LEGACY_CONST_VALUE, constGRID[iElement] != mutableGRID[iElement], "%u-bit const element:%u, value:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 ".", bcof(T), iElement, ::llc::u3_t(constGRID[iElement]), ::llc::u3_t(mutableGRID[iElement]));
	::llc::grid<cnst T> constFromMutableGRID = {};
	cnst ::llc::err_t constFromMutableResult = ::llc::gridPascal(constFromMutableGRID, (::llc::vu0_t)serialized);
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LEGACY_CONST_RESULT, constFromMutableResult != expectedSize, "%u-bit const-from-mutable result:%i, expected:%u.", bcof(T), constFromMutableResult, expectedSize);
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LEGACY_CONST_ALIAS, constFromMutableGRID.begin() != constGRID.begin(), "%u-bit const-from-mutable begin:%p, expected:%p.", bcof(T), constFromMutableGRID.begin(), constGRID.begin());
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LEGACY_CONST_ALIAS, constFromMutableGRID.size() != constGRID.size(), "%u-bit const-from-mutable count:%u, expected:%u.", bcof(T), constFromMutableGRID.size(), constGRID.size());

	T mutableSentinel = T(0x6BU);
	cnst T sentinel = mutableSentinel;
	for(::llc::u2_t byteCount = 0; byteCount < expectedSize; ++byteCount) {
		::llc::grid<T> preservedMutable = {&mutableSentinel, 1};
		cnst ::llc::err_t truncatedMutableResult = ::llc::gridPascal(preservedMutable, ::llc::vu0_t{serialized.begin(), byteCount});
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LEGACY_TRUNCATED_MUTABLE_RESULT, 0 <= truncatedMutableResult, "%u-bit mutable legacy read accepted %u/%u bytes. result:%i.", bcof(T), byteCount, expectedSize, truncatedMutableResult);
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LEGACY_TRUNCATED_MUTABLE_OUTPUT, preservedMutable.begin() != &mutableSentinel, "%u-bit mutable legacy read changed output begin at byte:%u. begin:%p, expected:%p.", bcof(T), byteCount, preservedMutable.begin(), &mutableSentinel);
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LEGACY_TRUNCATED_MUTABLE_OUTPUT, preservedMutable.size() != 1, "%u-bit mutable legacy read changed output count at byte:%u. count:%u.", bcof(T), byteCount, preservedMutable.size());

		::llc::grid<cnst T> preservedConst = {&sentinel, 1};
		cnst ::llc::err_t truncatedConstResult = ::llc::gridPascal(preservedConst, ::llc::vcu0_t{serialized.begin(), byteCount});
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LEGACY_TRUNCATED_CONST_RESULT, 0 <= truncatedConstResult, "%u-bit const legacy read accepted %u/%u bytes. result:%i.", bcof(T), byteCount, expectedSize, truncatedConstResult);
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LEGACY_TRUNCATED_CONST_OUTPUT, preservedConst.begin() != &sentinel, "%u-bit const legacy read changed output begin at byte:%u. begin:%p, expected:%p.", bcof(T), byteCount, preservedConst.begin(), &sentinel);
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LEGACY_TRUNCATED_CONST_OUTPUT, preservedConst.size() != 1, "%u-bit const legacy read changed output count at byte:%u. count:%u.", bcof(T), byteCount, preservedConst.size());
	}

	::llc::u0_t malformed[szof(::llc::u2_t)] = {};
	cnst ::llc::u2_t impossibleCount = (::llc::u2_t)-1;
	memcpy(malformed, &impossibleCount, szof(impossibleCount));
	::llc::grid<cnst T> preserved = {&sentinel, 1};
	cnst ::llc::err_t malformedResult = ::llc::gridPascal(preserved, ::llc::vcu0_t{malformed});
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LEGACY_MALFORMED_COUNT_RESULT, 0 <= malformedResult, "%u-bit legacy read accepted count:%u without a payload. result:%i.", bcof(T), impossibleCount, malformedResult);
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LEGACY_MALFORMED_COUNT_OUTPUT, preserved.begin() != &sentinel, "%u-bit malformed legacy read changed output begin to:%p, expected:%p.", bcof(T), preserved.begin(), &sentinel);
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_LEGACY_MALFORMED_COUNT_OUTPUT, preserved.size() != 1, "%u-bit malformed legacy read changed output count to:%u.", bcof(T), preserved.size());
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testLegacyGrid(ATestError & errors) {
	stxp ::llc::u2_t WIDTH = 3;
	stxp ::llc::u2_t HEIGHT = 2;
	stxp ::llc::u2_t CELL_COUNT = WIDTH * HEIGHT;
	stxp ::llc::n2u2_t metrics = {WIDTH, HEIGHT};
	T source[HEIGHT][WIDTH] = {
		{T(0x11U), T(0x22U), T(0x33U)},
		{T(0x44U), T(0x55U), T(0x66U)},
	};
	::llc::au0_t serialized;
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_GRID_MUTABLE_RESULT, ::llc::savePOD(serialized, metrics) != szof(metrics), "%u-bit grid test could not serialize its dimensions.", bcof(T));
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_GRID_MUTABLE_RESULT, 0 > serialized.append(::llc::grid<cnst T>{&source[0][0], CELL_COUNT}.cu8()), "%u-bit grid test could not serialize its cells.", bcof(T));
	cnst ::llc::u2_t expectedSize = szof(metrics) + szof(source);

	::llc::grid<T> mutableGrid;
	cnst ::llc::err_t mutableResult = ::llc::gridPascal(mutableGrid, (::llc::vu0_t)serialized);
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_GRID_MUTABLE_RESULT, mutableResult != (::llc::err_t)expectedSize, "%u-bit mutable grid result:%i, expected:%u.", bcof(T), mutableResult, expectedSize);
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_GRID_MUTABLE_ALIAS, mutableGrid.metrics() != metrics || mutableGrid.begin() != reinterpret_cast<T*>(&serialized[szof(metrics)]), "%u-bit mutable grid dimensions or alias differ.", bcof(T));
	for(::llc::u2_t iCell = 0; iCell < CELL_COUNT; ++iCell)
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_GRID_MUTABLE_VALUE, mutableGrid.begin()[iCell] != (&source[0][0])[iCell], "%u-bit mutable grid cell:%u differs.", bcof(T), iCell);
	mutableGrid[{1, 1}] = T(0x7AU);
	T storedValue = {};
	memcpy(&storedValue, &serialized[szof(metrics) + (metrics.x + 1) * szof(T)], szof(T));
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_GRID_MUTABLE_WRITE, storedValue != T(0x7AU), "%u-bit mutable grid write was not reflected in source storage.", bcof(T));

	::llc::grid<cnst T> constGrid;
	cnst ::llc::err_t constResult = ::llc::gridPascal(constGrid, (::llc::vcu0_t)serialized);
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_GRID_CONST_RESULT, constResult != (::llc::err_t)expectedSize, "%u-bit const grid result:%i, expected:%u.", bcof(T), constResult, expectedSize);
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_GRID_CONST_ALIAS, constGrid.metrics() != metrics || constGrid.begin() != reinterpret_cast<cnst T*>(&serialized[szof(metrics)]), "%u-bit const grid dimensions or alias differ.", bcof(T));
	for(::llc::u2_t iCell = 0; iCell < CELL_COUNT; ++iCell)
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_GRID_CONST_VALUE, constGrid.begin()[iCell] != mutableGrid.begin()[iCell], "%u-bit const grid cell:%u differs.", bcof(T), iCell);

	cnst T sentinel = T(0x6BU);
	for(::llc::u2_t byteCount = 0; byteCount < expectedSize; ++byteCount) {
		::llc::grid<cnst T> preserved = {&sentinel, 1, 1};
		cnst ::llc::err_t truncatedResult = ::llc::gridPascal(preserved, ::llc::vcu0_t{serialized.begin(), byteCount});
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_GRID_TRUNCATED_RESULT, 0 <= truncatedResult, "%u-bit grid read accepted %u/%u bytes. result:%i.", bcof(T), byteCount, expectedSize, truncatedResult);
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_GRID_TRUNCATED_OUTPUT, (preserved.begin() != &sentinel || preserved.metrics() != ::llc::n2u2_t{1, 1}), "%u-bit grid read changed output at byte:%u.", bcof(T), byteCount);
	}

	::llc::au0_t malformed;
	cnst ::llc::n2u2_t impossibleMetrics = {(::llc::u2_t)-1, (::llc::u2_t)-1};
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_GRID_MALFORMED_RESULT, ::llc::savePOD(malformed, impossibleMetrics) != szof(impossibleMetrics), "%u-bit grid test could not serialize malformed dimensions.", bcof(T));
	::llc::grid<cnst T> preserved = {&sentinel, 1, 1};
	cnst ::llc::err_t malformedResult = ::llc::gridPascal(preserved, (::llc::vcu0_t)malformed);
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_GRID_MALFORMED_RESULT, 0 <= malformedResult, "%u-bit grid read accepted dimensions:%u x %u. result:%i.", bcof(T), impossibleMetrics.x, impossibleMetrics.y, malformedResult);
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULTRESULT_GRID_MALFORMED_OUTPUT, (preserved.begin() != &sentinel || preserved.metrics() != ::llc::n2u2_t{1, 1}), "%u-bit malformed grid read changed output.", bcof(T));
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testType(ATestError & errors) {
	if_fail_fe(testPOD<T>			(errors));
	if_fail_fe(testUIntType<T>		(errors));
	if_fail_fe(testTruncatedUInt<T>	(errors));
	if_fail_fe(testGRIDType<T>		(errors));
	if_fail_fe(testTruncatedGRID<T>	(errors));
	if_fail_fe(testMalformedGRIDCount<T>(errors));
	if_fail_fe(testLegacyGRID<T>		(errors));
	return testLegacyGrid<T>(errors);
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

::llc::err_t testGRIDSerialize(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
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
