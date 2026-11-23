#include "llc_apod_serialize.h"
#include "llc_vox.h"
#include "test_llc_suites.h"

GDEFINE_ENUM_TYPE(VOX_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, TAG_MATCH			, 0, "SVOXChunkHeader::IsType() did not match exactly four tag bytes.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, LOAD_RESULT			, 1, "SVOXData::Load() rejected a valid SIZE chunk.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, LOAD_CONSUMPTION	, 2, "SVOXData::Load() did not consume the valid input.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, LOAD_CHUNK			, 3, "SVOXData::Load() did not retain the SIZE chunk.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, DIMENSIONS			, 4, "SVOXData::GetDimensions() did not apply the established YZX coordinate order.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, TRUNCATED_HEADER	, 5, "SVOXData::Load() accepted a truncated chunk header.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, TRUNCATED_CHUNK	, 6, "SVOXData::Load() accepted a chunk whose declared payload was missing.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, TRUNCATED_DICT		, 7, "voxDictLoad() accepted a truncated dictionary count.");

sttc void setTag(::llc::SVOXChunkHeader & header, cnst char (&tag)[5]) {
	::memcpy(header.Type.Storage, tag, 4);
}

::llc::err_t test_vox(STestRunnerState &, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	::llc::SVOXChunkHeader typeHeader = {};
	setTag(typeHeader, "SIZE");
	LLC_TEST_CHECK(errors, VOX_TEST_RESULT_TAG_MATCH, false == typeHeader.IsType("SIZE") || typeHeader.IsType("PACK"));

	::llc::SVOXFileHeader fileHeader = {};
	::memcpy(fileHeader.MagicNumber.Storage, "VOX ", 4);
	fileHeader.Version = 150;
	::llc::SVOXChunkHeader sizeHeader = {};
	setTag(sizeHeader, "SIZE");
	sizeHeader.ChunkSize = szof(::llc::n3u2_t);
	cnst ::llc::n3u2_t sourceDimensions = {2, 3, 4};
	::llc::au0_t encoded = {};
	if_fail_fe(::llc::savePOD(encoded, fileHeader));
	if_fail_fe(::llc::savePOD(encoded, sizeHeader));
	if_fail_fe(::llc::savePOD(encoded, sourceDimensions));
	::llc::vcu0_t input = encoded;
	::llc::SVOXData vox = {};
	cnst ::llc::err_t loadResult = vox.Load(input);
	LLC_TEST_CHECK(errors, VOX_TEST_RESULT_LOAD_RESULT, loadResult < 0);
	LLC_TEST_CHECK(errors, VOX_TEST_RESULT_LOAD_CONSUMPTION, input.size());
	LLC_TEST_CHECK(errors, VOX_TEST_RESULT_LOAD_CHUNK, vox.Chunks.size() != 1 || false == vox.Chunks[0].Header.IsType("SIZE"));
	cnst ::llc::n3u0_t expectedDimensions = {3, 4, 2};
	LLC_TEST_CHECK(errors, VOX_TEST_RESULT_DIMENSIONS, vox.GetDimensions() != expectedDimensions);

	::llc::au0_t truncatedHeader = {};
	if_fail_fe(::llc::savePOD(truncatedHeader, fileHeader));
	cnst ::llc::u2_t partialChunkHeader = 0;
	if_fail_fe(::llc::savePOD(truncatedHeader, partialChunkHeader));
	::llc::vcu0_t truncatedHeaderInput = truncatedHeader;
	::llc::SVOXData truncatedHeaderVox = {};
	LLC_TEST_CHECK(errors, VOX_TEST_RESULT_TRUNCATED_HEADER, 0 <= truncatedHeaderVox.Load(truncatedHeaderInput));

	::llc::SVOXChunkHeader missingPayloadHeader = {};
	setTag(missingPayloadHeader, "JUNK");
	missingPayloadHeader.ChunkSize = 8;
	::llc::au0_t truncatedChunk = {};
	if_fail_fe(::llc::savePOD(truncatedChunk, fileHeader));
	if_fail_fe(::llc::savePOD(truncatedChunk, missingPayloadHeader));
	::llc::vcu0_t truncatedChunkInput = truncatedChunk;
	::llc::SVOXData truncatedChunkVox = {};
	LLC_TEST_CHECK(errors, VOX_TEST_RESULT_TRUNCATED_CHUNK, 0 <= truncatedChunkVox.Load(truncatedChunkInput));

	cnst ::llc::u0_t oneByte[] = {0};
	::llc::vcu0_t truncatedDictInput = oneByte;
	::llc::SVOXDict attributes = {};
	LLC_TEST_CHECK(errors, VOX_TEST_RESULT_TRUNCATED_DICT, 0 <= ::llc::voxDictLoad(attributes, truncatedDictInput));
	return 0;
}
