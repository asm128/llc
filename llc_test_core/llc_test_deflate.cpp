#include "llc_deflate.h"
#include "llc_test_core.h"

#include <cstring>

GDEFINE_ENUM_TYPE(DEFLATE_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(DEFLATE_TEST_RESULT, COMPRESS_RESULT	, 0, "arrayDeflate() rejected valid input.");
GDEFINE_ENUM_VALUED(DEFLATE_TEST_RESULT, INFLATE_RESULT	, 1, "arrayInflate() rejected a valid compressed stream.");
GDEFINE_ENUM_VALUED(DEFLATE_TEST_RESULT, ROUND_TRIP_SIZE	, 2, "The inflated byte count differs from the source.");
GDEFINE_ENUM_VALUED(DEFLATE_TEST_RESULT, ROUND_TRIP_BYTES	, 3, "The inflated bytes differ from the source.");
GDEFINE_ENUM_VALUED(DEFLATE_TEST_RESULT, EMPTY_REJECTED	, 4, "arrayInflate() accepted an empty compressed stream.");
GDEFINE_ENUM_VALUED(DEFLATE_TEST_RESULT, TRUNCATED_REJECTED, 5, "arrayInflate() accepted a truncated compressed stream.");
GDEFINE_ENUM_VALUED(DEFLATE_TEST_RESULT, CORRUPT_REJECTED	, 6, "arrayInflate() accepted a corrupted compressed stream.");

::llc::err_t testDeflate(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	::llc::u0_t source[1025] = {};
	for(::llc::u2_t iByte = 0; iByte < ::llc::size(source); ++iByte)
		source[iByte] = (::llc::u0_t)((iByte * 73U) ^ (iByte >> 3));

	cnst ::llc::u2_t lengths[] = {0, 1, ::llc::size(source)};
	cnst ::llc::u2_t chunks [] = {1, 7, ::llc::DEFLATE_DEFAULT_CHUNK_SIZE};
	for(::llc::u2_t length : lengths)
		for(::llc::u2_t chunkSize : chunks) {
			::llc::au0_t compressed = {}, restored = {};
			cnst ::llc::err_t compression = ::llc::arrayDeflate({source, length}, compressed, chunkSize);
			LLC_TEST_CHECKF(errors, DEFLATE_TEST_RESULT_COMPRESS_RESULT, ::llc::failed(compression), "Input:%u, chunk:%u, result:%i.", length, chunkSize, compression);
			if(::llc::failed(compression))
				continue;
			cnst ::llc::err_t inflation = ::llc::arrayInflate(compressed, restored, chunkSize);
			LLC_TEST_CHECKF(errors, DEFLATE_TEST_RESULT_INFLATE_RESULT, ::llc::failed(inflation), "Input:%u, chunk:%u, result:%i.", length, chunkSize, inflation);
			if(::llc::failed(inflation))
				continue;
			LLC_TEST_CHECKF(errors, DEFLATE_TEST_RESULT_ROUND_TRIP_SIZE, restored.size() != length, "Input:%u, chunk:%u, output:%u.", length, chunkSize, restored.size());
			if(restored.size() == length)
				LLC_TEST_CHECKF(errors, DEFLATE_TEST_RESULT_ROUND_TRIP_BYTES, length && 0 != ::memcmp(restored.begin(), source, length), "Input:%u, chunk:%u.", length, chunkSize);
		}

#if !defined(LLC_ESP32) && !defined(LLC_ARDUINO)
	::llc::au0_t output = {}, compressed = {};
	LLC_TEST_CHECK(errors, DEFLATE_TEST_RESULT_EMPTY_REJECTED, false == ::llc::failed(::llc::arrayInflate(::llc::vcu0_t{}, output, 7)));
	cnst ::llc::err_t compression = ::llc::arrayDeflate({source, ::llc::size(source)}, compressed, 7);
	LLC_TEST_REQUIREF(errors, DEFLATE_TEST_RESULT_COMPRESS_RESULT, ::llc::failed(compression), "Invalid-stream fixture result:%i.", compression);
	LLC_TEST_REQUIRE(errors, DEFLATE_TEST_RESULT_COMPRESS_RESULT, compressed.size() < 2);
	output.clear();
	LLC_TEST_CHECK(errors, DEFLATE_TEST_RESULT_TRUNCATED_REJECTED, false == ::llc::failed(::llc::arrayInflate({compressed.begin(), compressed.size() - 1}, output, 7)));
	compressed[0] ^= 0xFF;
	output.clear();
	LLC_TEST_CHECK(errors, DEFLATE_TEST_RESULT_CORRUPT_REJECTED, false == ::llc::failed(::llc::arrayInflate(compressed, output, 7)));
#endif
	rtrn 0;
}
