#include "llc_test_core_suites.h"
#include "llc_deflate.h"

GDEFINE_ENUM_TYPE(TEST_DEFLATE_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_DEFLATE_RESULT, ROUND_TRIP       , 0, "Deflate and inflate preserve a mixed repeated byte stream.");
GDEFINE_ENUM_VALUED(TEST_DEFLATE_RESULT, INVALID_SETTINGS , 1, "Deflate rejects invalid settings and preserves the failed lifecycle state.");
GDEFINE_ENUM_VALUED(TEST_DEFLATE_RESULT, INVALID_HEADER   , 2, "Inflate rejects an invalid zlib header during initialization.");
GDEFINE_ENUM_VALUED(TEST_DEFLATE_RESULT, TRUNCATED_STREAM , 3, "Inflate reports a truncated compressed stream.");
GDEFINE_ENUM_VALUED(TEST_DEFLATE_RESULT, CORRUPT_BLOCK    , 4, "Inflate reports a reserved DEFLATE block type.");
GDEFINE_ENUM_VALUED(TEST_DEFLATE_RESULT, ADLER_MISMATCH   , 5, "Inflate reports an Adler-32 mismatch.");

sttc ::llc::vcu0_t deflateInput() {
	sttc const ::llc::u0_t bytes[] = {0, 1, 2, 3, 3, 3, 3, 3, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0};
	rtrn bytes;
}

sttc bool deflateEqual(::llc::vcu0_t left, ::llc::vcu0_t right) {
	rtrn left.size() == right.size() && (0 == left.size() || 0 == memcmp(left.begin(), right.begin(), left.size()));
}

sttc ::llc::err_t testDeflateRoundTrip(::llc::ATestError & results) {
	::llc::au0_t deflated, inflated;
	LLC_TEST_REQUIRE(results, TEST_DEFLATE_RESULT_ROUND_TRIP, 0 != ::llc::arrayDeflate(::deflateInput(), deflated, 8));
	LLC_TEST_REQUIRE(results, TEST_DEFLATE_RESULT_ROUND_TRIP, 0 != ::llc::arrayInflate(deflated, inflated, 7));
	LLC_TEST_CHECK(results, TEST_DEFLATE_RESULT_ROUND_TRIP, false == ::deflateEqual(::deflateInput(), inflated));
	rtrn 0;
}

sttc ::llc::err_t testDeflateInvalidSettings(::llc::ATestError & results) {
	::llc::SDeflateSettings settings = {};
	settings.WindowBits = 7;
	::llc::SZipStream stream;
	LLC_TEST_CHECK(results, TEST_DEFLATE_RESULT_INVALID_SETTINGS, 0 <= ::llc::deflateInit(stream, ::deflateInput(), settings) || stream.Status != ::llc::LIFECYCLE_STATE_FAILED);
	::llc::au0_t deflated;
	LLC_TEST_CHECK(results, TEST_DEFLATE_RESULT_INVALID_SETTINGS, 0 <= ::llc::arrayDeflate(::deflateInput(), deflated, settings, 8));
	rtrn 0;
}

sttc ::llc::err_t testDeflateInvalidHeader(::llc::ATestError & results) {
	cnst ::llc::u0_t invalidHeader[] = {0, 0};
	::llc::SZipStream stream;
	LLC_TEST_CHECK(results, TEST_DEFLATE_RESULT_INVALID_HEADER, 0 <= ::llc::inflateInit(stream, invalidHeader) || stream.Status != ::llc::LIFECYCLE_STATE_FAILED);
	::llc::au0_t inflated;
	LLC_TEST_CHECK(results, TEST_DEFLATE_RESULT_INVALID_HEADER, 0 <= ::llc::arrayInflate(invalidHeader, inflated, 8));
	rtrn 0;
}

sttc ::llc::err_t testDeflateTruncatedStream(::llc::ATestError & results) {
	::llc::au0_t deflated, inflated;
	LLC_TEST_REQUIRE(results, TEST_DEFLATE_RESULT_TRUNCATED_STREAM, 0 != ::llc::arrayDeflate(::deflateInput(), deflated, 8));
	LLC_TEST_REQUIRE(results, TEST_DEFLATE_RESULT_TRUNCATED_STREAM, deflated.size() < 2 || 0 > deflated.resize(deflated.size() - 1));
	LLC_TEST_CHECK(results, TEST_DEFLATE_RESULT_TRUNCATED_STREAM, 0 <= ::llc::arrayInflate(deflated, inflated, 8));
	rtrn 0;
}

sttc ::llc::err_t testDeflateCorruptBlock(::llc::ATestError & results) {
	::llc::au0_t deflated, inflated;
	LLC_TEST_REQUIRE(results, TEST_DEFLATE_RESULT_CORRUPT_BLOCK, 0 != ::llc::arrayDeflate(::deflateInput(), deflated, 8));
	LLC_TEST_REQUIRE(results, TEST_DEFLATE_RESULT_CORRUPT_BLOCK, deflated.size() < 7);
	deflated[2] |= 0x06;
	LLC_TEST_CHECK(results, TEST_DEFLATE_RESULT_CORRUPT_BLOCK, 0 <= ::llc::arrayInflate(deflated, inflated, 8));
	rtrn 0;
}

sttc ::llc::err_t testDeflateAdlerMismatch(::llc::ATestError & results) {
	::llc::au0_t deflated, inflated;
	LLC_TEST_REQUIRE(results, TEST_DEFLATE_RESULT_ADLER_MISMATCH, 0 != ::llc::arrayDeflate(::deflateInput(), deflated, 8));
	LLC_TEST_REQUIRE(results, TEST_DEFLATE_RESULT_ADLER_MISMATCH, deflated.size() < 4);
	deflated[deflated.size() - 1] ^= 1;
	LLC_TEST_CHECK(results, TEST_DEFLATE_RESULT_ADLER_MISMATCH, 0 <= ::llc::arrayInflate(deflated, inflated, 8));
	rtrn 0;
}

::llc::err_t testDeflate(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testDeflateRoundTrip(results));
	result = ::llc::min(result, ::testDeflateInvalidSettings(results));
	result = ::llc::min(result, ::testDeflateInvalidHeader(results));
	result = ::llc::min(result, ::testDeflateTruncatedStream(results));
	result = ::llc::min(result, ::testDeflateCorruptBlock(results));
	result = ::llc::min(result, ::testDeflateAdlerMismatch(results));
	rtrn result;
}
