#include "llc_block.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(BLOCK_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(BLOCK_TEST_RESULT, OK					,  0, "All block-address tests passed.");
GDEFINE_ENUM_VALUED(BLOCK_TEST_RESULT, TYPE_LAYOUT			,  1, "SRecordMap does not contain exactly two 32-bit coordinates.");
GDEFINE_ENUM_VALUED(BLOCK_TEST_RESULT, DEFAULT_VALUE			,  2, "SRecordMap did not initialize both coordinates to zero.");
GDEFINE_ENUM_VALUED(BLOCK_TEST_RESULT, ID_RESULT			,  3, "blockRecordId() rejected a valid record map.");
GDEFINE_ENUM_VALUED(BLOCK_TEST_RESULT, ID_VALUE				,  4, "blockRecordId() returned the wrong linear record identifier.");
GDEFINE_ENUM_VALUED(BLOCK_TEST_RESULT, INDICES_RESULT		,  5, "blockRecordIndices() rejected a representable record identifier.");
GDEFINE_ENUM_VALUED(BLOCK_TEST_RESULT, BLOCK_INDEX			,  6, "blockRecordIndices() returned the wrong block identifier.");
GDEFINE_ENUM_VALUED(BLOCK_TEST_RESULT, RECORD_INDEX			,  7, "blockRecordIndices() returned the wrong record index.");
GDEFINE_ENUM_VALUED(BLOCK_TEST_RESULT, ROUND_TRIP			,  8, "A record identifier did not survive an indices round trip.");
GDEFINE_ENUM_VALUED(BLOCK_TEST_RESULT, ZERO_BLOCK_SIZE		,  9, "A record-address conversion accepted a zero block size.");
GDEFINE_ENUM_VALUED(BLOCK_TEST_RESULT, RECORD_RANGE			, 10, "blockRecordId() accepted a record index outside its block.");
GDEFINE_ENUM_VALUED(BLOCK_TEST_RESULT, BLOCK_RANGE			, 11, "blockRecordIndices() accepted a block coordinate wider than SRecordMap.");
GDEFINE_ENUM_VALUED(BLOCK_TEST_RESULT, OUTPUT_STABILITY		, 12, "A failed record-address conversion changed its output.");
GDEFINE_ENUM_VALUED(BLOCK_TEST_RESULT, FULL_WIDTH			, 13, "Record-address conversion discarded high identifier bits.");

sttc ::llc::err_t testBlockRoundTrips(::llc::ATestError & errors) {
	cnst ::llc::u2_t blockSizes[] = {1, 2, 3, 7, 255, 256, 65535, ::llc::u2_t(-1)};
	cnst ::llc::u2_t blockIds[] = {0, 1, 2, 255, 65535, 0x7FFFFFFF, ::llc::u2_t(-1)};
	for(::llc::u2_t iSize = 0; iSize < ::llc::size(blockSizes); ++iSize)
	for(::llc::u2_t iBlock = 0; iBlock < ::llc::size(blockIds); ++iBlock) {
		cnst ::llc::u2_t blockSize = blockSizes[iSize];
		cnst ::llc::u2_t recordIndices[] = {0, blockSize / 2, blockSize - 1};
		for(::llc::u2_t iRecord = 0; iRecord < ::llc::size(recordIndices); ++iRecord) {
			cnst ::llc::SRecordMap expected = {blockIds[iBlock], recordIndices[iRecord]};
			cnst ::llc::u3_t expectedId = ::llc::u3_t(expected.IdBlock) * blockSize + expected.IndexRecord;
			::llc::u3_t encoded = ::llc::u3_t(-1);
			cnst ::llc::err_t encodeResult = ::llc::blockRecordId(expected, blockSize, encoded);
			LLC_TEST_CHECKF(errors, BLOCK_TEST_RESULT_ID_RESULT, ::llc::failed(encodeResult), "Block:%u, record:%u, size:%u, result:%i.", expected.IdBlock, expected.IndexRecord, blockSize, encodeResult);
			LLC_TEST_CHECKF(errors, BLOCK_TEST_RESULT_ID_VALUE, encoded != expectedId, "Block:%u, record:%u, size:%u, id:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 ".", expected.IdBlock, expected.IndexRecord, blockSize, encoded, expectedId);

			::llc::SRecordMap decoded = {17, 19};
			cnst ::llc::err_t decodeResult = ::llc::blockRecordIndices(expectedId, blockSize, decoded);
			LLC_TEST_CHECKF(errors, BLOCK_TEST_RESULT_INDICES_RESULT, ::llc::failed(decodeResult), "Id:%" LLC_FMT_U3 ", size:%u, result:%i.", expectedId, blockSize, decodeResult);
			LLC_TEST_CHECKF(errors, BLOCK_TEST_RESULT_BLOCK_INDEX, decoded.IdBlock != expected.IdBlock, "Id:%" LLC_FMT_U3 ", size:%u, block:%u, expected:%u.", expectedId, blockSize, decoded.IdBlock, expected.IdBlock);
			LLC_TEST_CHECKF(errors, BLOCK_TEST_RESULT_RECORD_INDEX, decoded.IndexRecord != expected.IndexRecord, "Id:%" LLC_FMT_U3 ", size:%u, record:%u, expected:%u.", expectedId, blockSize, decoded.IndexRecord, expected.IndexRecord);

			::llc::u3_t roundTrip = {};
			cnst ::llc::err_t roundTripResult = ::llc::blockRecordId(decoded, blockSize, roundTrip);
			LLC_TEST_CHECKF(errors, BLOCK_TEST_RESULT_ID_RESULT, ::llc::failed(roundTripResult), "Decoded block:%u, record:%u, size:%u, result:%i.", decoded.IdBlock, decoded.IndexRecord, blockSize, roundTripResult);
			LLC_TEST_CHECKF(errors, BLOCK_TEST_RESULT_ROUND_TRIP, roundTrip != expectedId, "Id:%" LLC_FMT_U3 ", round trip:%" LLC_FMT_U3 ", size:%u.", expectedId, roundTrip, blockSize);
		}
	}
	rtrn 0;
}

sttc ::llc::err_t testBlockFailures(::llc::ATestError & errors) {
	cnst ::llc::SRecordMap originalMap = {17, 19};
	::llc::SRecordMap outputMap = originalMap;
	cnst ::llc::err_t zeroDecodeResult = ::llc::blockRecordIndices(37, 0, outputMap);
	LLC_TEST_CHECKF(errors, BLOCK_TEST_RESULT_ZERO_BLOCK_SIZE, zeroDecodeResult != ::llc::OS_INVALID_PARAMETER, "Decode result:%i, expected:%i.", zeroDecodeResult, ::llc::OS_INVALID_PARAMETER);
	LLC_TEST_CHECKF(errors, BLOCK_TEST_RESULT_OUTPUT_STABILITY, outputMap.IdBlock != originalMap.IdBlock, "Failed decode block:%u, expected unchanged:%u.", outputMap.IdBlock, originalMap.IdBlock);
	LLC_TEST_CHECKF(errors, BLOCK_TEST_RESULT_OUTPUT_STABILITY, outputMap.IndexRecord != originalMap.IndexRecord, "Failed decode record:%u, expected unchanged:%u.", outputMap.IndexRecord, originalMap.IndexRecord);

	::llc::u3_t outputId = 0x123456789ABCDEF0ULL;
	cnst ::llc::u3_t originalId = outputId;
	cnst ::llc::err_t zeroEncodeResult = ::llc::blockRecordId({17, 0}, 0, outputId);
	LLC_TEST_CHECKF(errors, BLOCK_TEST_RESULT_ZERO_BLOCK_SIZE, zeroEncodeResult != ::llc::OS_INVALID_PARAMETER, "Encode result:%i, expected:%i.", zeroEncodeResult, ::llc::OS_INVALID_PARAMETER);
	LLC_TEST_CHECKF(errors, BLOCK_TEST_RESULT_OUTPUT_STABILITY, outputId != originalId, "Failed zero-size encode changed id:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 ".", outputId, originalId);

	cnst ::llc::err_t recordRangeResult = ::llc::blockRecordId({17, 8}, 8, outputId);
	LLC_TEST_CHECKF(errors, BLOCK_TEST_RESULT_RECORD_RANGE, recordRangeResult != ::llc::OS_INVALID_PARAMETER, "Record-range result:%i, expected:%i.", recordRangeResult, ::llc::OS_INVALID_PARAMETER);
	LLC_TEST_CHECKF(errors, BLOCK_TEST_RESULT_OUTPUT_STABILITY, outputId != originalId, "Failed record-range encode changed id:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 ".", outputId, originalId);

	outputMap = originalMap;
	cnst ::llc::err_t blockRangeResult = ::llc::blockRecordIndices(0x100000000ULL, 1, outputMap);
	LLC_TEST_CHECKF(errors, BLOCK_TEST_RESULT_BLOCK_RANGE, blockRangeResult != ::llc::OS_OVERRUN, "Block-range result:%i, expected:%i.", blockRangeResult, ::llc::OS_OVERRUN);
	LLC_TEST_CHECKF(errors, BLOCK_TEST_RESULT_OUTPUT_STABILITY, outputMap.IdBlock != originalMap.IdBlock, "Failed block-range decode changed block:%u, expected:%u.", outputMap.IdBlock, originalMap.IdBlock);
	LLC_TEST_CHECKF(errors, BLOCK_TEST_RESULT_OUTPUT_STABILITY, outputMap.IndexRecord != originalMap.IndexRecord, "Failed block-range decode changed record:%u, expected:%u.", outputMap.IndexRecord, originalMap.IndexRecord);
	rtrn 0;
}

sttc ::llc::err_t testBlockFullWidth(::llc::ATestError & errors) {
	cnst ::llc::u3_t expectedId = 0x8000000000001234ULL;
	cnst ::llc::u2_t blockSize = ::llc::u2_t(-1);
	::llc::SRecordMap indices = {};
	cnst ::llc::err_t decodeResult = ::llc::blockRecordIndices(expectedId, blockSize, indices);
	LLC_TEST_REQUIREF(errors, BLOCK_TEST_RESULT_FULL_WIDTH, ::llc::failed(decodeResult), "High-bit id:%" LLC_FMT_U3 ", size:%u, result:%i.", expectedId, blockSize, decodeResult);
	::llc::u3_t actualId = {};
	cnst ::llc::err_t encodeResult = ::llc::blockRecordId(indices, blockSize, actualId);
	LLC_TEST_CHECKF(errors, BLOCK_TEST_RESULT_FULL_WIDTH, ::llc::failed(encodeResult), "Decoded block:%u, record:%u, size:%u, result:%i.", indices.IdBlock, indices.IndexRecord, blockSize, encodeResult);
	LLC_TEST_CHECKF(errors, BLOCK_TEST_RESULT_FULL_WIDTH, actualId != expectedId, "High-bit id:%" LLC_FMT_U3 ", round trip:%" LLC_FMT_U3 ".", expectedId, actualId);
	rtrn 0;
}

::llc::err_t testBlock(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	LLC_TEST_CHECKF(errors, BLOCK_TEST_RESULT_TYPE_LAYOUT, sizeof(::llc::SRecordMap) != sizeof(::llc::u2_t) * 2, "SRecordMap size:%u, expected:%u.", (::llc::u2_t)sizeof(::llc::SRecordMap), (::llc::u2_t)(sizeof(::llc::u2_t) * 2));
	cnst ::llc::SRecordMap defaultMap = {};
	LLC_TEST_CHECKF(errors, BLOCK_TEST_RESULT_DEFAULT_VALUE, defaultMap.IdBlock, "Default block:%u, expected:0.", defaultMap.IdBlock);
	LLC_TEST_CHECKF(errors, BLOCK_TEST_RESULT_DEFAULT_VALUE, defaultMap.IndexRecord, "Default record:%u, expected:0.", defaultMap.IndexRecord);
	if_fail_fe(::testBlockRoundTrips(errors));
	if_fail_fe(::testBlockFailures(errors));
	rtrn ::testBlockFullWidth(errors);
}
