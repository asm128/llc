#include "llc_test_core_suites.h"
#include "llc_adam7.h"
#include "llc_png.h"

GDEFINE_ENUM_TYPE(TEST_ADAM7_CPP_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_ADAM7_CPP_RESULT, SCALE_VECTORS       , 0, "All seven Adam7 passes expose the canonical multiplier/base vectors.");
GDEFINE_ENUM_VALUED(TEST_ADAM7_CPP_RESULT, NEGATIVE_PASS       , 1, "A negative pass index fails without changing either output vector.");
GDEFINE_ENUM_VALUED(TEST_ADAM7_CPP_RESULT, PAST_LAST_PASS      , 2, "Pass index seven fails without changing either output vector.");
GDEFINE_ENUM_VALUED(TEST_ADAM7_CPP_RESULT, SHORT_SIZE_STORAGE  , 3, "Size calculation rejects storage for fewer than seven passes.");
GDEFINE_ENUM_VALUED(TEST_ADAM7_CPP_RESULT, SIZE_8X8            , 4, "An 8x8 image produces the seven canonical pass sizes.");
GDEFINE_ENUM_VALUED(TEST_ADAM7_CPP_RESULT, COVERAGE_1_TO_17    , 5, "Every pixel in dimensions 1x1 through 17x17 belongs to exactly one pass.");
GDEFINE_ENUM_VALUED(TEST_ADAM7_CPP_RESULT, INTERLACE_8X8       , 6, "The seven pass images land on their exact 8x8 coordinates.");
GDEFINE_ENUM_VALUED(TEST_ADAM7_CPP_RESULT, REAL_PNG_1X1        , 7, "A real 1x1 indexed Adam7 PNG decodes to its blue pixel.");

namespace {
	stxp ::llc::n2u2_t ADAM7_MULTIPLIERS[7] =
		{{8, 8}, {8, 8}, {4, 8}, {4, 4}, {2, 4}, {2, 2}, {1, 2}};
	stxp ::llc::n2u2_t ADAM7_BASES[7] =
		{{0, 0}, {4, 0}, {0, 4}, {2, 0}, {0, 2}, {1, 0}, {0, 1}};
	stxp ::llc::n2u2_t ADAM7_SIZES_8X8[7] =
		{{1, 1}, {1, 1}, {2, 1}, {2, 2}, {4, 2}, {4, 4}, {8, 4}};

	stxp ::llc::u0_t PNG_ADAM7_1X1_BLUE[] = {
		0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52,
		0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x00, 0x00, 0x01, 0x52, 0xDC, 0x66,
		0x5C, 0x00, 0x00, 0x00, 0x04, 0x67, 0x41, 0x4D, 0x41, 0x00, 0x01, 0x86, 0xA0, 0x31, 0xE8, 0x96,
		0x5F, 0x00, 0x00, 0x00, 0x03, 0x73, 0x42, 0x49, 0x54, 0x04, 0x04, 0x04, 0x77, 0xF8, 0xB5, 0xA3,
		0x00, 0x00, 0x00, 0x03, 0x50, 0x4C, 0x54, 0x45, 0x00, 0x00, 0xFF, 0x8A, 0x78, 0xD2, 0x57, 0x00,
		0x00, 0x00, 0x0A, 0x49, 0x44, 0x41, 0x54, 0x78, 0x9C, 0x63, 0x60, 0x00, 0x00, 0x00, 0x02, 0x00,
		0x01, 0x48, 0xAF, 0xA4, 0x71, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60,
		0x82,
	};
}

sttc ::llc::err_t testAdam7CPPScaleVectors(::llc::ATestError & results) {
	for(::llc::s2_t iPass = 0; iPass < 7; ++iPass) {
		::llc::n2u2_t multiplier = {}, base = {};
		cnst ::llc::err_t result = ::llc::adam7ScaleIndex(iPass, multiplier, base);
		LLC_TEST_CHECKF(results, TEST_ADAM7_CPP_RESULT_SCALE_VECTORS
			, ::llc::failed(result) || multiplier != ADAM7_MULTIPLIERS[iPass] || base != ADAM7_BASES[iPass]
			, "pass:%i result:%i multiplier:%u,%u base:%u,%u", iPass, result, multiplier.x, multiplier.y, base.x, base.y);
	}
	rtrn 0;
}

sttc ::llc::err_t testAdam7CPPInvalidPass
	( ::llc::ATestError & results
	, ::llc::s2_t iPass
	, TEST_ADAM7_CPP_RESULT testResult
	) {
	cnst ::llc::n2u2_t expectedMultiplier = {31, 37};
	cnst ::llc::n2u2_t expectedBase = {41, 43};
	::llc::n2u2_t multiplier = expectedMultiplier, base = expectedBase;
	::llc::STestLogBuffer expectedLog;
	::llc::err_t result = 0;
	{
		::llc::STestLogCapture capture(expectedLog);
		result = ::llc::adam7ScaleIndex(iPass, multiplier, base);
	}
	LLC_TEST_CHECKF(results, testResult
		, !::llc::failed(result) || !expectedLog.Failed || multiplier != expectedMultiplier || base != expectedBase
		, "pass:%i result:%i", iPass, result);
	rtrn 0;
}

sttc ::llc::err_t testAdam7CPPShortSizeStorage(::llc::ATestError & results) {
	::llc::n2u2_t storage[6] = {{11, 13}, {11, 13}, {11, 13}, {11, 13}, {11, 13}, {11, 13}};
	::llc::view<::llc::n2u2_t> sizes = storage;
	::llc::STestLogBuffer expectedLog;
	::llc::err_t result = 0;
	{
		::llc::STestLogCapture capture(expectedLog);
		result = ::llc::adam7Sizes(sizes, {8, 8});
	}
	bool changed = false;
	for(cnst ::llc::n2u2_t & size : storage)
		changed |= size != ::llc::n2u2_t{11, 13};
	LLC_TEST_CHECKF(results, TEST_ADAM7_CPP_RESULT_SHORT_SIZE_STORAGE
		, !::llc::failed(result) || !expectedLog.Failed || changed
		, "result:%i changed:%u", result, changed);
	rtrn 0;
}

sttc ::llc::err_t testAdam7CPPSize8x8(::llc::ATestError & results) {
	::llc::n2u2_t storage[7] = {};
	::llc::view<::llc::n2u2_t> sizes = storage;
	cnst ::llc::err_t result = ::llc::adam7Sizes(sizes, {8, 8});
	for(::llc::u2_t iPass = 0; iPass < 7; ++iPass)
		LLC_TEST_CHECKF(results, TEST_ADAM7_CPP_RESULT_SIZE_8X8
			, ::llc::failed(result) || storage[iPass] != ADAM7_SIZES_8X8[iPass]
			, "pass:%u result:%i size:%u,%u", iPass, result, storage[iPass].x, storage[iPass].y);
	rtrn 0;
}

sttc ::llc::err_t testAdam7CPPCoverage1To17(::llc::ATestError & results) {
	::llc::u0_t coverage[17 * 17] = {};
	for(::llc::u2_t height = 1; height <= 17; ++height)
	for(::llc::u2_t width = 1; width <= 17; ++width) {
		memset(coverage, 0, szof(coverage));
		::llc::n2u2_t sizes[7] = {};
		::llc::view<::llc::n2u2_t> sizesView = sizes;
		cnst ::llc::err_t sizeResult = ::llc::adam7Sizes(sizesView, {width, height});
		bool invalid = ::llc::failed(sizeResult);
		for(::llc::u2_t iPass = 0; !invalid && iPass < 7; ++iPass)
		for(::llc::u2_t y = 0; y < sizes[iPass].y; ++y)
		for(::llc::u2_t x = 0; x < sizes[iPass].x; ++x) {
			cnst ::llc::u2_t targetX = x * ADAM7_MULTIPLIERS[iPass].x + ADAM7_BASES[iPass].x;
			cnst ::llc::u2_t targetY = y * ADAM7_MULTIPLIERS[iPass].y + ADAM7_BASES[iPass].y;
			if(targetX >= width || targetY >= height) {
				invalid = true;
				break;
			}
			++coverage[targetY * width + targetX];
		}
		for(::llc::u2_t iPixel = 0; !invalid && iPixel < width * height; ++iPixel)
			invalid |= coverage[iPixel] != 1;
		LLC_TEST_CHECKF(results, TEST_ADAM7_CPP_RESULT_COVERAGE_1_TO_17, invalid
			, "size:%u,%u result:%i", width, height, sizeResult);
	}
	rtrn 0;
}

sttc ::llc::err_t testAdam7CPPInterlace8x8(::llc::ATestError & results) {
	::llc::imgu0_t passes[7];
	::llc::u0_t expected[8 * 8] = {};
	for(::llc::u2_t iPass = 0; iPass < 7; ++iPass) {
		if_fail_fe(passes[iPass].resize(ADAM7_SIZES_8X8[iPass], (::llc::u0_t)(iPass + 1)));
		for(::llc::u2_t y = 0; y < ADAM7_SIZES_8X8[iPass].y; ++y)
		for(::llc::u2_t x = 0; x < ADAM7_SIZES_8X8[iPass].x; ++x) {
			cnst ::llc::u2_t targetX = x * ADAM7_MULTIPLIERS[iPass].x + ADAM7_BASES[iPass].x;
			cnst ::llc::u2_t targetY = y * ADAM7_MULTIPLIERS[iPass].y + ADAM7_BASES[iPass].y;
			expected[targetY * 8 + targetX] = (::llc::u0_t)(iPass + 1);
		}
	}
	::llc::imgu0_t output;
	if_fail_fe(output.resize({8, 8}, ::llc::u0_t(0)));
	cnst ::llc::err_t result = ::llc::adam7Interlace(::llc::view<::llc::imgu0_t>{passes}, output.View);
	bool mismatch = ::llc::failed(result);
	for(::llc::u2_t iPixel = 0; !mismatch && iPixel < output.size(); ++iPixel)
		mismatch |= output.Texels[iPixel] != expected[iPixel];
	LLC_TEST_CHECKF(results, TEST_ADAM7_CPP_RESULT_INTERLACE_8X8, mismatch, "result:%i", result);
	rtrn 0;
}

sttc ::llc::err_t testAdam7CPPRealPNG1x1(::llc::ATestError & results) {
	::llc::SPNGData png;
	::llc::img8bgra image;
	cnst ::llc::vcu0_t source = PNG_ADAM7_1X1_BLUE;
	cnst ::llc::err_t result = ::llc::pngFileLoad(png, source, image);
	cnst bool invalidHeader = png.Header.Size != ::llc::n2u2_t{1, 1} || png.Header.MethodInterlace != 1;
	cnst bool invalidImage = image.metrics() != ::llc::n2u2_t{1, 1} || image.size() != 1;
	cnst bool invalidPixel = invalidImage || image.Texels[0].b != 0xFF || image.Texels[0].g || image.Texels[0].r || image.Texels[0].a != 0xFF;
	LLC_TEST_CHECKF(results, TEST_ADAM7_CPP_RESULT_REAL_PNG_1X1
		, ::llc::failed(result) || invalidHeader || invalidImage || invalidPixel
		, "result:%i interlace:%i size:%u,%u", result, png.Header.MethodInterlace, png.Header.Size.x, png.Header.Size.y);
	rtrn 0;
}

::llc::err_t testAdam7CPP(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testAdam7CPPScaleVectors(results));
	result = ::llc::min(result, ::testAdam7CPPInvalidPass(results, -1, TEST_ADAM7_CPP_RESULT_NEGATIVE_PASS));
	result = ::llc::min(result, ::testAdam7CPPInvalidPass(results,  7, TEST_ADAM7_CPP_RESULT_PAST_LAST_PASS));
	result = ::llc::min(result, ::testAdam7CPPShortSizeStorage(results));
	result = ::llc::min(result, ::testAdam7CPPSize8x8(results));
	result = ::llc::min(result, ::testAdam7CPPCoverage1To17(results));
	result = ::llc::min(result, ::testAdam7CPPInterlace8x8(results));
	result = ::llc::min(result, ::testAdam7CPPRealPNG1x1(results));
	rtrn result;
}
