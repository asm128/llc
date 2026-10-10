#include "llc_grid_copy.h"
#include "llc_grid_scale.h"
#include "llc_grid_static.h"
#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(GRID_OPERATIONS_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, COPY_RESULT			,  1, "grid_copy() reported the wrong copied cell count.");
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, COPY_CELL			,  2, "grid_copy() wrote the wrong destination cell.");
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, COPY_CLIPPED_RESULT	,  3, "grid_copy() reported the wrong clipped cell count.");
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, COPY_CLIPPED_CELL	,  4, "grid_copy() clipped or positioned the source incorrectly.");
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, COPY_ALPHA_RESULT	,  5, "grid_copy_alpha() reported the wrong visited cell count.");
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, COPY_ALPHA_CELL		,  6, "grid_copy_alpha() mishandled a transparent or opaque cell.");
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, COPY_COLOR_RESULT	,  7, "Colored grid_copy_alpha() reported the wrong visited cell count.");
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, COPY_COLOR_CELL		,  8, "Colored grid_copy_alpha() mishandled a transparent or opaque cell.");
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, MIRROR_X_RESULT		,  9, "grid_mirror_x() reported the wrong copied cell count.");
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, MIRROR_X_CELL		, 10, "grid_mirror_x() wrote the wrong destination cell.");
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, MIRROR_Y_RESULT		, 11, "grid_mirror_y() reported the wrong copied cell count.");
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, MIRROR_Y_CELL		, 12, "grid_mirror_y() wrote the wrong destination cell.");
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, SCALE_RESULT			, 13, "grid_scale() reported failure for valid grids.");
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, SCALE_CELL			, 14, "grid_scale() selected the wrong source cell.");
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, SCALE_FACTOR_RESULT	, 15, "Explicit-factor grid_scale() reported failure for valid grids.");
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, SCALE_FACTOR_CELL	, 16, "Explicit-factor grid_scale() crossed or misapplied its axis factors.");
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, STATIC_METRICS		, 17, "grid_static<> exposed the wrong dimensions, count or storage.");
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, STATIC_WRITE_RESULT	, 18, "grid_static<>::write() reported the wrong result.");
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, STATIC_WRITE_SIZE	, 19, "grid_static<>::write() reported the wrong byte count.");
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, STATIC_WRITE_VALUE	, 20, "grid_static<>::write() serialized a different cell value.");
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, STATIC_READ_RESULT	, 21, "grid_static<>::read() reported the wrong result.");
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, STATIC_READ_SIZE		, 22, "grid_static<>::read() reported the wrong byte count.");
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, STATIC_READ_VALUE	, 23, "grid_static<>::read() restored a different cell value.");
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, STATIC_INVALID_READ	, 24, "grid_static<>::read() accepted a missing source or byte counter.");
GDEFINE_ENUM_VALUED(GRID_OPERATIONS_TEST_RESULT, STATIC_INVALID_WRITE	, 25, "grid_static<>::write() accepted no destination and no byte counter.");

sttc ::llc::err_t testGridCopy(ATestError & errors) {
	::llc::u2_t sourceCells[2][3] = {{1, 2, 3}, {4, 5, 6}};
	cnst ::llc::grid<::llc::u2_t> source = {sourceCells};
	::llc::u2_t copiedCells[2][3] = {};
	::llc::grid<::llc::u2_t> copied = {copiedCells};
	cnst ::llc::err_t copyResult = ::llc::grid_copy(copied, source);
	LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_COPY_RESULT, copyResult != 6, "Copied cells:%i, expected:6.", copyResult);
	for(::llc::u2_t y = 0; y < copied.metrics().y; ++y)
		for(::llc::u2_t x = 0; x < copied.metrics().x; ++x)
			LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_COPY_CELL, copied[y][x] != source[y][x], "Cell:%u,%u, value:%u, expected:%u.", x, y, copied[y][x], source[y][x]);

	::llc::u2_t clippedCells[3][4] = {{9, 9, 9, 9}, {9, 9, 9, 9}, {9, 9, 9, 9}};
	::llc::grid<::llc::u2_t> clipped = {clippedCells};
	cnst ::llc::n2s2_t negativeOffset = {-1, 1};
	cnst ::llc::err_t clippedResult = ::llc::grid_copy(clipped, source, negativeOffset);
	LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_COPY_CLIPPED_RESULT, clippedResult != 4, "Clipped cells:%i, expected:4.", clippedResult);
	cnst ::llc::u2_t clippedExpected[3][4] = {{9, 9, 9, 9}, {2, 3, 9, 9}, {5, 6, 9, 9}};
	for(::llc::u2_t y = 0; y < clipped.metrics().y; ++y)
		for(::llc::u2_t x = 0; x < clipped.metrics().x; ++x)
			LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_COPY_CLIPPED_CELL, clipped[y][x] != clippedExpected[y][x], "Cell:%u,%u, value:%u, expected:%u.", x, y, clipped[y][x], clippedExpected[y][x]);

	::llc::u2_t alphaSourceCells[2][3] = {{0, 2, 0}, {4, 0, 6}};
	cnst ::llc::grid<::llc::u2_t> alphaSource = {alphaSourceCells};
	::llc::u2_t alphaCells[2][3] = {{9, 9, 9}, {9, 9, 9}};
	::llc::grid<::llc::u2_t> alpha = {alphaCells};
	cnst ::llc::err_t alphaResult = ::llc::grid_copy_alpha(alpha, alphaSource, ::llc::u2_t(0));
	LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_COPY_ALPHA_RESULT, alphaResult != 6, "Visited cells:%i, expected:6.", alphaResult);
	cnst ::llc::u2_t alphaExpected[2][3] = {{9, 2, 9}, {4, 9, 6}};
	for(::llc::u2_t y = 0; y < alpha.metrics().y; ++y)
		for(::llc::u2_t x = 0; x < alpha.metrics().x; ++x)
			LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_COPY_ALPHA_CELL, alpha[y][x] != alphaExpected[y][x], "Cell:%u,%u, value:%u, expected:%u.", x, y, alpha[y][x], alphaExpected[y][x]);

	::llc::u2_t coloredCells[2][3] = {{9, 9, 9}, {9, 9, 9}};
	::llc::grid<::llc::u2_t> colored = {coloredCells};
	cnst ::llc::n2s2_t origin = {};
	cnst ::llc::err_t colorResult = ::llc::grid_copy_alpha(colored, alphaSource, origin, ::llc::u2_t(0), ::llc::u2_t(7));
	LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_COPY_COLOR_RESULT, colorResult != 6, "Visited cells:%i, expected:6.", colorResult);
	cnst ::llc::u2_t colorExpected[2][3] = {{9, 7, 9}, {7, 9, 7}};
	for(::llc::u2_t y = 0; y < colored.metrics().y; ++y)
		for(::llc::u2_t x = 0; x < colored.metrics().x; ++x)
			LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_COPY_COLOR_CELL, colored[y][x] != colorExpected[y][x], "Cell:%u,%u, value:%u, expected:%u.", x, y, colored[y][x], colorExpected[y][x]);

	::llc::u2_t mirrorXCells[2][3] = {};
	::llc::grid<::llc::u2_t> mirrorX = {mirrorXCells};
	cnst ::llc::err_t mirrorXResult = ::llc::grid_mirror_x(mirrorX, source);
	LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_MIRROR_X_RESULT, mirrorXResult != 6, "Mirrored cells:%i, expected:6.", mirrorXResult);
	cnst ::llc::u2_t mirrorXExpected[2][3] = {{3, 2, 1}, {6, 5, 4}};
	for(::llc::u2_t y = 0; y < mirrorX.metrics().y; ++y)
		for(::llc::u2_t x = 0; x < mirrorX.metrics().x; ++x)
			LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_MIRROR_X_CELL, mirrorX[y][x] != mirrorXExpected[y][x], "Cell:%u,%u, value:%u, expected:%u.", x, y, mirrorX[y][x], mirrorXExpected[y][x]);

	::llc::u2_t mirrorYCells[2][3] = {};
	::llc::grid<::llc::u2_t> mirrorY = {mirrorYCells};
	cnst ::llc::err_t mirrorYResult = ::llc::grid_mirror_y(mirrorY, source);
	LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_MIRROR_Y_RESULT, mirrorYResult != 6, "Mirrored cells:%i, expected:6.", mirrorYResult);
	cnst ::llc::u2_t mirrorYExpected[2][3] = {{4, 5, 6}, {1, 2, 3}};
	for(::llc::u2_t y = 0; y < mirrorY.metrics().y; ++y)
		for(::llc::u2_t x = 0; x < mirrorY.metrics().x; ++x)
			LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_MIRROR_Y_CELL, mirrorY[y][x] != mirrorYExpected[y][x], "Cell:%u,%u, value:%u, expected:%u.", x, y, mirrorY[y][x], mirrorYExpected[y][x]);
	return 0;
}

sttc ::llc::err_t testGridScale(ATestError & errors) {
	::llc::u2_t sourceCells[2][3] = {{1, 2, 3}, {4, 5, 6}};
	cnst ::llc::grid<::llc::u2_t> source = {sourceCells};
	::llc::u2_t scaledCells[2][6] = {};
	::llc::grid<::llc::u2_t> scaled = {scaledCells};
	cnst ::llc::err_t scaleResult = ::llc::grid_scale(scaled, source);
	LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_SCALE_RESULT, ::llc::failed(scaleResult), "Scale result:%i.", scaleResult);
	cnst ::llc::u2_t expected[2][6] = {{1, 1, 2, 2, 3, 3}, {4, 4, 5, 5, 6, 6}};
	for(::llc::u2_t y = 0; y < scaled.metrics().y; ++y)
		for(::llc::u2_t x = 0; x < scaled.metrics().x; ++x)
			LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_SCALE_CELL, scaled[y][x] != expected[y][x], "Cell:%u,%u, value:%u, expected:%u.", x, y, scaled[y][x], expected[y][x]);

	::llc::u2_t explicitCells[2][6] = {};
	::llc::grid<::llc::u2_t> explicitScale = {explicitCells};
	cnst ::llc::err_t factorResult = ::llc::grid_scale(explicitScale, source, 0.5F, 1.0F);
	LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_SCALE_FACTOR_RESULT, ::llc::failed(factorResult), "Explicit-factor scale result:%i.", factorResult);
	for(::llc::u2_t y = 0; y < explicitScale.metrics().y; ++y)
		for(::llc::u2_t x = 0; x < explicitScale.metrics().x; ++x)
			LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_SCALE_FACTOR_CELL, explicitScale[y][x] != expected[y][x], "Cell:%u,%u, value:%u, expected:%u. factors:0.5,1.0.", x, y, explicitScale[y][x], expected[y][x]);
	return 0;
}

sttc ::llc::err_t testGridStatic(ATestError & errors) {
	::llc::grid_static<::llc::u2_t, 3, 2> source = {};
	LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_STATIC_METRICS, source.metrics().x != 3, "Width:%u, expected:3.", source.metrics().x);
	LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_STATIC_METRICS, source.metrics().y != 2, "Height:%u, expected:2.", source.metrics().y);
	LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_STATIC_METRICS, source.size() != 6, "Cell count:%u, expected:6.", source.size());
	LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_STATIC_METRICS, source.begin() != &source.Cells[0][0], "Grid begin:%p, cell storage:%p.", source.begin(), &source.Cells[0][0]);
	for(::llc::u2_t y = 0; y < source.metrics().y; ++y)
		for(::llc::u2_t x = 0; x < source.metrics().x; ++x)
			source[y][x] = y * source.metrics().x + x + 1;

	::llc::u2_t requiredBytes = 7;
	cnst ::llc::err_t countResult = source.write(nullptr, &requiredBytes);
	LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_STATIC_WRITE_RESULT, ::llc::failed(countResult), "Count-only write result:%i.", countResult);
	LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_STATIC_WRITE_SIZE, requiredBytes != 7 + source.byte_count(), "Accumulated byte count:%u, expected:%u.", requiredBytes, 7 + source.byte_count());

	::llc::s0_t serialized[szof(source.Cells)] = {};
	::llc::u2_t bytesWritten = {};
	cnst ::llc::err_t writeResult = source.write(serialized, &bytesWritten);
	LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_STATIC_WRITE_RESULT, ::llc::failed(writeResult), "Write result:%i.", writeResult);
	LLC_TEST_REQUIREF(errors, GRID_OPERATIONS_TEST_RESULT_STATIC_WRITE_SIZE, bytesWritten != source.byte_count(), "Written bytes:%u, expected:%u.", bytesWritten, source.byte_count());
	LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_STATIC_WRITE_VALUE, 0 != memcmp(serialized, source.Cells, bytesWritten), "%s", "Serialized cells differ from source storage.");

	::llc::grid_static<::llc::u2_t, 3, 2> loaded = {};
	::llc::u2_t bytesRead = 5;
	cnst ::llc::err_t readResult = loaded.read(serialized, &bytesRead);
	LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_STATIC_READ_RESULT, ::llc::failed(readResult), "Read result:%i.", readResult);
	LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_STATIC_READ_SIZE, bytesRead != 5 + source.byte_count(), "Accumulated bytes:%u, expected:%u.", bytesRead, 5 + source.byte_count());
	for(::llc::u2_t y = 0; y < loaded.metrics().y; ++y)
		for(::llc::u2_t x = 0; x < loaded.metrics().x; ++x)
			LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_STATIC_READ_VALUE, loaded[y][x] != source[y][x], "Cell:%u,%u, value:%u, expected:%u.", x, y, loaded[y][x], source[y][x]);

	::llc::setupLogCallbacks(0, 0);
	cnst ::llc::err_t missingSourceResult = loaded.read(nullptr, &bytesRead);
	cnst ::llc::err_t missingCounterResult = loaded.read(serialized, nullptr);
	cnst ::llc::err_t missingWriteResult = source.write(nullptr, nullptr);
	::llc::setupDefaultLogCallbacks();
	LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_STATIC_INVALID_READ, 0 <= missingSourceResult, "read() accepted a missing source. result:%i.", missingSourceResult);
	LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_STATIC_INVALID_READ, 0 <= missingCounterResult, "read() accepted a missing byte counter. result:%i.", missingCounterResult);
	LLC_TEST_CHECKF(errors, GRID_OPERATIONS_TEST_RESULT_STATIC_INVALID_WRITE, 0 <= missingWriteResult, "write() accepted no destination and no byte counter. result:%i.", missingWriteResult);
	return 0;
}

::llc::err_t testGridOperations(ATestError & errors) {
	if_fail_fe(testGridCopy(errors));
	if_fail_fe(testGridScale(errors));
	return testGridStatic(errors);
}
