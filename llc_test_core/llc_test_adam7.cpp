#include "llc_test_core_suites.h"
#include "llc_adam7.h"
#include "llc_deflate.h"
#include "llc_png.h"

GDEFINE_ENUM_TYPE(TEST_ADAM7_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_ADAM7_RESULT, SCALE_VECTORS      , 0, "Adam7 exposes the seven standard multiplier and base vectors.");
GDEFINE_ENUM_VALUED(TEST_ADAM7_RESULT, INVALID_PASS       , 1, "Adam7 rejects pass indexes outside the seven-pass range.");
GDEFINE_ENUM_VALUED(TEST_ADAM7_RESULT, SHORT_STORAGE      , 2, "Adam7 size calculation rejects output storage shorter than seven entries.");
GDEFINE_ENUM_VALUED(TEST_ADAM7_RESULT, SIZES_8X8          , 3, "Adam7 reports the exact seven pass sizes for an 8x8 image.");
GDEFINE_ENUM_VALUED(TEST_ADAM7_RESULT, EXHAUSTIVE_COVERAGE, 4, "Adam7 pass sizes and vectors cover every pixel exactly once for dimensions 1 through 17.");
GDEFINE_ENUM_VALUED(TEST_ADAM7_RESULT, INTERLACED_PNG_SMALL, 5, "A real 3x2 interlaced PNG decodes every grayscale pixel into its source position.");

sttc ::llc::err_t adam7AppendU32BE(::llc::au0_t & output, ::llc::u2_t value) {
	if_fail_fe(output.push_back((::llc::u0_t)(value >> 24)));
	if_fail_fe(output.push_back((::llc::u0_t)(value >> 16)));
	if_fail_fe(output.push_back((::llc::u0_t)(value >> 8)));
	if_fail_fe(output.push_back((::llc::u0_t)value));
	rtrn 0;
}

sttc ::llc::err_t adam7AppendPNGChunk(::llc::au0_t & output, ::llc::vcst_t type, ::llc::vcu0_t data) {
	if_true_fe(type.size() != 4);
	if_fail_fe(::adam7AppendU32BE(output, data.size()));
	cnst ::llc::u2_t crcOffset = output.size();
	for(::llc::u2_t i = 0; i < 4; ++i)
		if_fail_fe(output.push_back((::llc::u0_t)type[i]));
	if_fail_fe(output.append(data));
	cnst ::llc::u2_t crc = ::llc::get_crc({output.begin() + crcOffset, 4 + data.size()});
	if_fail_fe(::adam7AppendU32BE(output, crc));
	rtrn 0;
}

sttc ::llc::err_t adam7PNGFixture(::llc::au0_t & output) {
	sttc cnst ::llc::u0_t signature[] = {0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A};
	sttc cnst ::llc::u0_t header[] = {0, 0, 0, 3, 0, 0, 0, 2, 8, 0, 0, 0, 1};
	sttc cnst ::llc::u0_t scanlines[] =
		{ 0, 10
		, 0, 30
		, 0, 20
		, 0, 40, 50, 60
		};
	output.clear();
	if_fail_fe(output.append(signature));
	if_fail_fe(::adam7AppendPNGChunk(output, "IHDR", header));
	::llc::au0_t compressed;
	if_fail_fe(::llc::arrayDeflate(scanlines, compressed));
	if_fail_fe(::adam7AppendPNGChunk(output, "IDAT", compressed));
	if_fail_fe(::adam7AppendPNGChunk(output, "IEND", {}));
	rtrn 0;
}

sttc ::llc::err_t testAdam7ScaleVectors(::llc::ATestError & results) {
	sttc cnst ::llc::n2u2_t expectedMultiplier[] = {{8, 8}, {8, 8}, {4, 8}, {4, 4}, {2, 4}, {2, 2}, {1, 2}};
	sttc cnst ::llc::n2u2_t expectedBase      [] = {{0, 0}, {4, 0}, {0, 4}, {2, 0}, {0, 2}, {1, 0}, {0, 1}};
	for(::llc::s2_t iPass = 0; iPass < 7; ++iPass) {
		::llc::n2u2_t multiplier = {}, base = {};
		LLC_TEST_REQUIRE(results, TEST_ADAM7_RESULT_SCALE_VECTORS, 0 != ::llc::adam7ScaleIndex(iPass, multiplier, base));
		LLC_TEST_CHECK(results, TEST_ADAM7_RESULT_SCALE_VECTORS, multiplier != expectedMultiplier[iPass] || base != expectedBase[iPass]);
	}
	rtrn 0;
}

sttc ::llc::err_t testAdam7InvalidPass(::llc::ATestError & results) {
	::llc::n2u2_t multiplier = {3, 5}, base = {7, 9};
	LLC_TEST_CHECK(results, TEST_ADAM7_RESULT_INVALID_PASS, 0 <= ::llc::adam7ScaleIndex(-1, multiplier, base));
	LLC_TEST_CHECK(results, TEST_ADAM7_RESULT_INVALID_PASS, 0 <= ::llc::adam7ScaleIndex( 7, multiplier, base));
	rtrn 0;
}

sttc ::llc::err_t testAdam7ShortStorage(::llc::ATestError & results) {
	::llc::n2u2_t storage[6] = {};
	::llc::view<::llc::n2u2_t> sizes = storage;
	LLC_TEST_CHECK(results, TEST_ADAM7_RESULT_SHORT_STORAGE, 0 <= ::llc::adam7Sizes(sizes, {8, 8}));
	rtrn 0;
}

sttc ::llc::err_t testAdam7Sizes8x8(::llc::ATestError & results) {
	::llc::n2u2_t storage[7] = {};
	::llc::view<::llc::n2u2_t> sizes = storage;
	sttc cnst ::llc::n2u2_t expected[] = {{1, 1}, {1, 1}, {2, 1}, {2, 2}, {4, 2}, {4, 4}, {8, 4}};
	LLC_TEST_REQUIRE(results, TEST_ADAM7_RESULT_SIZES_8X8, 0 != ::llc::adam7Sizes(sizes, {8, 8}));
	for(::llc::u2_t iPass = 0; iPass < 7; ++iPass)
		LLC_TEST_CHECK(results, TEST_ADAM7_RESULT_SIZES_8X8, sizes[iPass] != expected[iPass]);
	rtrn 0;
}

sttc ::llc::err_t testAdam7ExhaustiveCoverage(::llc::ATestError & results) {
	for(::llc::u2_t height = 1; height <= 17; ++height)
	for(::llc::u2_t width = 1; width <= 17; ++width) {
		::llc::n2u2_t storage[7] = {};
		::llc::view<::llc::n2u2_t> sizes = storage;
		LLC_TEST_REQUIRE(results, TEST_ADAM7_RESULT_EXHAUSTIVE_COVERAGE, 0 != ::llc::adam7Sizes(sizes, {width, height}));
		::llc::au0_t coverage;
		LLC_TEST_REQUIRE(results, TEST_ADAM7_RESULT_EXHAUSTIVE_COVERAGE, width * height != (::llc::u2_t)coverage.resize(width * height, 0));
		::llc::u2_t visited = 0;
		bool invalidCoordinate = false;
		for(::llc::u2_t iPass = 0; iPass < 7; ++iPass) {
			::llc::n2u2_t multiplier = {}, base = {};
			LLC_TEST_REQUIRE(results, TEST_ADAM7_RESULT_EXHAUSTIVE_COVERAGE, 0 != ::llc::adam7ScaleIndex(iPass, multiplier, base));
			for(::llc::u2_t y = 0; y < sizes[iPass].y; ++y)
			for(::llc::u2_t x = 0; x < sizes[iPass].x; ++x) {
				cnst ::llc::n2u2_t target = {x * multiplier.x + base.x, y * multiplier.y + base.y};
				if(target.x >= width || target.y >= height) {
					invalidCoordinate = true;
					continue;
				}
				++coverage[target.y * width + target.x];
				++visited;
			}
		}
		bool repeatedOrMissing = false;
		for(::llc::u2_t iPixel = 0; iPixel < coverage.size(); ++iPixel)
			if(1 != coverage[iPixel]) {
				repeatedOrMissing = true;
				break;
			}
		LLC_TEST_CHECKF(results, TEST_ADAM7_RESULT_EXHAUSTIVE_COVERAGE, invalidCoordinate || repeatedOrMissing || visited != width * height, "Coverage failed for %ux%u. Visited:%u.", width, height, visited);
	}
	rtrn 0;
}

sttc ::llc::err_t testAdam7InterlacedPNGSmall(::llc::ATestError & results) {
	::llc::au0_t fixture;
	LLC_TEST_REQUIRE(results, TEST_ADAM7_RESULT_INTERLACED_PNG_SMALL, 0 != ::adam7PNGFixture(fixture));
	::llc::SPNGData png;
	LLC_TEST_REQUIRE(results, TEST_ADAM7_RESULT_INTERLACED_PNG_SMALL, 0 != ::llc::pngFileLoad(png, fixture));
	::llc::imgu0_t image;
	LLC_TEST_REQUIRE(results, TEST_ADAM7_RESULT_INTERLACED_PNG_SMALL, 0 != ::llc::pngDecode(png, image));
	cnst ::llc::n2u2_t expectedSize = {3, 2};
	LLC_TEST_CHECK(results, TEST_ADAM7_RESULT_INTERLACED_PNG_SMALL
		, image.metrics() != expectedSize
		|| image[0][0] != 10 || image[0][1] != 20 || image[0][2] != 30
		|| image[1][0] != 40 || image[1][1] != 50 || image[1][2] != 60
		);
	rtrn 0;
}

::llc::err_t testAdam7(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testAdam7ScaleVectors(results));
	result = ::llc::min(result, ::testAdam7InvalidPass(results));
	result = ::llc::min(result, ::testAdam7ShortStorage(results));
	result = ::llc::min(result, ::testAdam7Sizes8x8(results));
	result = ::llc::min(result, ::testAdam7ExhaustiveCoverage(results));
	result = ::llc::min(result, ::testAdam7InterlacedPNGSmall(results));
	rtrn result;
}
