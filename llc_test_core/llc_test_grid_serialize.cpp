#include "llc_test_core.h"
#include "llc_grid_serialize.h"
#include "llc_apod_serialize.h"

GDEFINE_ENUM_TYPE(GRID_SERIALIZE_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULT, GRID_MUTABLE_RESULT			, 44, "Mutable gridPascal() reported the wrong consumed byte count.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULT, GRID_MUTABLE_ALIAS			, 45, "Mutable gridPascal() returned the wrong dimensions or alias.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULT, GRID_MUTABLE_VALUE			, 46, "Mutable gridPascal() returned different cell values.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULT, GRID_MUTABLE_WRITE			, 47, "Mutable gridPascal() did not expose writable source storage.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULT, GRID_CONST_RESULT			, 48, "Const gridPascal() reported the wrong consumed byte count.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULT, GRID_CONST_ALIAS			, 49, "Const gridPascal() returned the wrong dimensions or alias.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULT, GRID_CONST_VALUE			, 50, "Const gridPascal() returned different cell values.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULT, GRID_TRUNCATED_RESULT		, 51, "gridPascal() accepted truncated input.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULT, GRID_TRUNCATED_OUTPUT		, 52, "gridPascal() modified its output after rejecting truncated input.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULT, GRID_MALFORMED_RESULT		, 53, "gridPascal() accepted overflowing dimensions.");
GDEFINE_ENUM_VALUED(GRID_SERIALIZE_TEST_RESULT, GRID_MALFORMED_OUTPUT		, 54, "gridPascal() modified its output after rejecting overflowing dimensions.");

tplt<tpnm TOutput, tpnm TInput>
concept canGridReadLegacy = requires(TOutput & output, TInput input) { ::llc::gridPascal(output, input); };

static_assert( canGridReadLegacy<::llc::grid<::llc::u2_t>, ::llc::vu0_t>);
static_assert( canGridReadLegacy<::llc::grid<cnst ::llc::u2_t>, ::llc::vcu0_t>);
static_assert( canGridReadLegacy<::llc::grid<cnst ::llc::u2_t>, ::llc::vu0_t>);
static_assert(false == canGridReadLegacy<::llc::grid<::llc::u2_t>, ::llc::vcu0_t>);
static_assert(false == canGridReadLegacy<::llc::grid<::llc::u2_t>, ::llc::vcst_t>);

tplt<tpnm T>
sttc ::llc::err_t testLegacyGrid(::llc::ATestError & errors) {
	stxp ::llc::u2_t WIDTH = 3;
	stxp ::llc::u2_t HEIGHT = 2;
	stxp ::llc::u2_t CELL_COUNT = WIDTH * HEIGHT;
	stxp ::llc::n2u2_t metrics = {WIDTH, HEIGHT};
	T source[HEIGHT][WIDTH] = {
		{T(0x11U), T(0x22U), T(0x33U)},
		{T(0x44U), T(0x55U), T(0x66U)},
	};
	::llc::au0_t serialized;
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULT_GRID_MUTABLE_RESULT, ::llc::savePOD(serialized, metrics) != szof(metrics), "%u-bit grid test could not serialize its dimensions.", bcof(T));
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULT_GRID_MUTABLE_RESULT, 0 > serialized.append(::llc::view<cnst T>{&source[0][0], CELL_COUNT}.cu8()), "%u-bit grid test could not serialize its cells.", bcof(T));
	cnst ::llc::u2_t expectedSize = szof(metrics) + szof(source);

	::llc::grid<T> mutableGrid;
	cnst ::llc::err_t mutableResult = ::llc::gridPascal(mutableGrid, (::llc::vu0_t)serialized);
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULT_GRID_MUTABLE_RESULT, mutableResult != (::llc::err_t)expectedSize, "%u-bit mutable grid result:%i, expected:%u.", bcof(T), mutableResult, expectedSize);
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULT_GRID_MUTABLE_ALIAS, mutableGrid.metrics() != metrics || mutableGrid.begin() != reinterpret_cast<T*>(&serialized[szof(metrics)]), "%u-bit mutable grid dimensions or alias differ.", bcof(T));
	for(::llc::u2_t iCell = 0; iCell < CELL_COUNT; ++iCell)
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULT_GRID_MUTABLE_VALUE, mutableGrid.begin()[iCell] != (&source[0][0])[iCell], "%u-bit mutable grid cell:%u differs.", bcof(T), iCell);
	mutableGrid[{1, 1}] = T(0x7AU);
	T storedValue = {};
	memcpy(&storedValue, &serialized[szof(metrics) + (metrics.x + 1) * szof(T)], szof(T));
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULT_GRID_MUTABLE_WRITE, storedValue != T(0x7AU), "%u-bit mutable grid write was not reflected in source storage.", bcof(T));

	::llc::grid<cnst T> constGrid;
	cnst ::llc::err_t constResult = ::llc::gridPascal(constGrid, (::llc::vcu0_t)serialized);
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULT_GRID_CONST_RESULT, constResult != (::llc::err_t)expectedSize, "%u-bit const grid result:%i, expected:%u.", bcof(T), constResult, expectedSize);
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULT_GRID_CONST_ALIAS, constGrid.metrics() != metrics || constGrid.begin() != reinterpret_cast<cnst T*>(&serialized[szof(metrics)]), "%u-bit const grid dimensions or alias differ.", bcof(T));
	for(::llc::u2_t iCell = 0; iCell < CELL_COUNT; ++iCell)
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULT_GRID_CONST_VALUE, constGrid.begin()[iCell] != mutableGrid.begin()[iCell], "%u-bit const grid cell:%u differs.", bcof(T), iCell);

	cnst T sentinel = T(0x6BU);
	for(::llc::u2_t byteCount = 0; byteCount < expectedSize; ++byteCount) {
		::llc::grid<cnst T> preserved = {&sentinel, 1, 1};
		cnst ::llc::err_t truncatedResult = ::llc::gridPascal(preserved, ::llc::vcu0_t{serialized.begin(), byteCount});
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULT_GRID_TRUNCATED_RESULT, 0 <= truncatedResult, "%u-bit grid read accepted %u/%u bytes. result:%i.", bcof(T), byteCount, expectedSize, truncatedResult);
		LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULT_GRID_TRUNCATED_OUTPUT, (preserved.begin() != &sentinel || preserved.metrics() != ::llc::n2u2_t{1, 1}), "%u-bit grid read changed output at byte:%u.", bcof(T), byteCount);
	}

	::llc::au0_t malformed;
	cnst ::llc::n2u2_t impossibleMetrics = {(::llc::u2_t)-1, (::llc::u2_t)-1};
	LLC_TEST_REQUIREF(errors, GRID_SERIALIZE_TEST_RESULT_GRID_MALFORMED_RESULT, ::llc::savePOD(malformed, impossibleMetrics) != szof(impossibleMetrics), "%u-bit grid test could not serialize malformed dimensions.", bcof(T));
	::llc::grid<cnst T> preserved = {&sentinel, 1, 1};
	cnst ::llc::err_t malformedResult = ::llc::gridPascal(preserved, (::llc::vcu0_t)malformed);
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULT_GRID_MALFORMED_RESULT, 0 <= malformedResult, "%u-bit grid read accepted dimensions:%u x %u. result:%i.", bcof(T), impossibleMetrics.x, impossibleMetrics.y, malformedResult);
	LLC_TEST_CHECKF(errors, GRID_SERIALIZE_TEST_RESULT_GRID_MALFORMED_OUTPUT, (preserved.begin() != &sentinel || preserved.metrics() != ::llc::n2u2_t{1, 1}), "%u-bit malformed grid read changed output.", bcof(T));
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testTypeLogged(::llc::ATestError & errors) {
	cnst ::llc::u2_t checkCount = testCheckCount(errors);
	cnst ::llc::u2_t failureCount = testErrorCount(errors);
	if_fail_fe(testLegacyGrid<T>(errors));
	cnst ::llc::u2_t typeFailures = testErrorCount(errors) - failureCount;
	cnst ::llc::u2_t typeChecks = testCheckCount(errors) - checkCount;
	if(typeFailures) error_printf("%2u-bit suite completed: %u/%u checks passed, %u failed.", bcof(T), typeChecks - typeFailures, typeChecks, typeFailures);
	return 0;
}

::llc::err_t testGridSerialize(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	cnst ::llc::u2_t failureCount = testErrorCount(errors);
	if_fail_fe(::llc::testTypePack(::llc::unsigned_integer_types{}, [&](auto type) { rtrn testTypeLogged<typename decltype(type)::type>(errors); }));
	if(failureCount == testErrorCount(errors))
		always_printf("Element widths tested successfully:\n%u, %u, %u and %u bits."
			, ::llc::u2_t(szof(::llc::u0_t) * 8), ::llc::u2_t(szof(::llc::u1_t) * 8), ::llc::u2_t(szof(::llc::u2_t) * 8), ::llc::u2_t(szof(::llc::u3_t) * 8)
			);
	return 0;
}
