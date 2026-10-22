#include "llc_voxel.h"
#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(VOXEL_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(VOXEL_TEST_RESULT, SET_CHUNK			,  1, "SVoxelMap<>::SetValue() returned or stored the wrong chunk.");
GDEFINE_ENUM_VALUED(VOXEL_TEST_RESULT, SET_VALUE			,  2, "SVoxelMap<>::SetValue() stored the wrong cell value.");
GDEFINE_ENUM_VALUED(VOXEL_TEST_RESULT, SET_VOXEL_COUNT	,  3, "SVoxelMap<>::SetValue() stored the wrong sparse voxel count.");
GDEFINE_ENUM_VALUED(VOXEL_TEST_RESULT, SET_VOXEL_POSITION	,  4, "SVoxelMap<>::SetValue() stored the wrong sparse voxel position.");
GDEFINE_ENUM_VALUED(VOXEL_TEST_RESULT, SET_VOXEL_COLOR	,  5, "SVoxelMap<>::SetValue() stored the wrong sparse voxel color.");
GDEFINE_ENUM_VALUED(VOXEL_TEST_RESULT, UPDATE_COUNT		,  6, "Updating a voxel duplicated its sparse entry.");
GDEFINE_ENUM_VALUED(VOXEL_TEST_RESULT, UPDATE_VALUE		,  7, "Updating a voxel did not update its cell and sparse entry.");
GDEFINE_ENUM_VALUED(VOXEL_TEST_RESULT, DELETE_COUNT		,  8, "Clearing a voxel retained or created a sparse entry.");
GDEFINE_ENUM_VALUED(VOXEL_TEST_RESULT, DELETE_VALUE		,  9, "Clearing a voxel did not clear its cell.");
GDEFINE_ENUM_VALUED(VOXEL_TEST_RESULT, CHUNK_POSITION		, 10, "SVoxelMap<> assigned a voxel to the wrong horizontal chunk.");
GDEFINE_ENUM_VALUED(VOXEL_TEST_RESULT, MISSING_RESULT		, 11, "SVoxelMap<>::GetValue() accepted a position without a chunk.");
GDEFINE_ENUM_VALUED(VOXEL_TEST_RESULT, MISSING_OUTPUT		, 12, "SVoxelMap<>::GetValue() modified output for a missing position.");
GDEFINE_ENUM_VALUED(VOXEL_TEST_RESULT, FACE_BOTTOM_MINIMUM	, 13, "SVoxel<>::Limits() rejected the bottom face at the minimum Y coordinate.");
GDEFINE_ENUM_VALUED(VOXEL_TEST_RESULT, FACE_BACK_MINIMUM		, 14, "SVoxel<>::Limits() rejected the back face at the minimum X coordinate.");
GDEFINE_ENUM_VALUED(VOXEL_TEST_RESULT, FACE_LEFT_MINIMUM		, 15, "SVoxel<>::Limits() rejected the left face at the minimum Z coordinate.");
GDEFINE_ENUM_VALUED(VOXEL_TEST_RESULT, FACE_TOP_MINIMUM		, 16, "SVoxel<>::Limits() accepted the top face at the minimum Y coordinate.");
GDEFINE_ENUM_VALUED(VOXEL_TEST_RESULT, FACE_FRONT_MINIMUM	, 17, "SVoxel<>::Limits() accepted the front face at the minimum X coordinate.");
GDEFINE_ENUM_VALUED(VOXEL_TEST_RESULT, FACE_RIGHT_MINIMUM	, 18, "SVoxel<>::Limits() accepted the right face at the minimum Z coordinate.");
GDEFINE_ENUM_VALUED(VOXEL_TEST_RESULT, FACE_TOP_MAXIMUM		, 19, "SVoxel<>::Limits() rejected the top face at the maximum Y coordinate.");
GDEFINE_ENUM_VALUED(VOXEL_TEST_RESULT, FACE_FRONT_MAXIMUM	, 20, "SVoxel<>::Limits() rejected the front face at the maximum X coordinate.");
GDEFINE_ENUM_VALUED(VOXEL_TEST_RESULT, FACE_RIGHT_MAXIMUM	, 21, "SVoxel<>::Limits() rejected the right face at the maximum Z coordinate.");
GDEFINE_ENUM_VALUED(VOXEL_TEST_RESULT, FACE_BOTTOM_MAXIMUM	, 22, "SVoxel<>::Limits() accepted the bottom face at the maximum Y coordinate.");
GDEFINE_ENUM_VALUED(VOXEL_TEST_RESULT, FACE_BACK_MAXIMUM		, 23, "SVoxel<>::Limits() accepted the back face at the maximum X coordinate.");
GDEFINE_ENUM_VALUED(VOXEL_TEST_RESULT, FACE_LEFT_MAXIMUM		, 24, "SVoxel<>::Limits() accepted the left face at the maximum Z coordinate.");

::llc::err_t testVoxel(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	::llc::SVoxelMap<::llc::u0_t> map = {};
	cnst ::llc::n3u0_t position = {1, 2, 3};
	cnst ::llc::err_t firstChunk = map.SetValue(position, 7);
	LLC_TEST_REQUIREF(errors, VOXEL_TEST_RESULT_SET_CHUNK, firstChunk != 0, "Chunk:%i, expected:0.", firstChunk);
	LLC_TEST_CHECKF(errors, VOXEL_TEST_RESULT_SET_CHUNK, map.Chunks.size() != 1, "Chunk count:%u, expected:1.", map.Chunks.size());
	LLC_TEST_CHECKF(errors, VOXEL_TEST_RESULT_SET_CHUNK, map.ChunkPositions.size() != 1, "Chunk-position count:%u, expected:1.", map.ChunkPositions.size());
	cnst ::llc::n2u0_t firstChunkPosition = {0, 0};
	LLC_TEST_CHECKF(errors, VOXEL_TEST_RESULT_CHUNK_POSITION, map.ChunkPositions[0] != firstChunkPosition, "Chunk position:%u,%u, expected:0,0.", map.ChunkPositions[0].x, map.ChunkPositions[0].y);
	::llc::u0_t value = {};
	cnst ::llc::err_t getResult = map.GetValue(position, value);
	LLC_TEST_CHECKF(errors, VOXEL_TEST_RESULT_SET_VALUE, ::llc::failed(getResult), "Get result:%i.", getResult);
	LLC_TEST_CHECKF(errors, VOXEL_TEST_RESULT_SET_VALUE, value != 7, "Value:%u, expected:7.", value);
	LLC_TEST_REQUIREF(errors, VOXEL_TEST_RESULT_SET_VOXEL_COUNT, map.Voxels.size() != 1, "Voxel count:%u, expected:1.", map.Voxels.size());
	LLC_TEST_CHECKF(errors, VOXEL_TEST_RESULT_SET_VOXEL_POSITION, map.Voxels[0].Position != position, "Position:%u,%u,%u, expected:%u,%u,%u.", map.Voxels[0].Position.x, map.Voxels[0].Position.y, map.Voxels[0].Position.z, position.x, position.y, position.z);
	LLC_TEST_CHECKF(errors, VOXEL_TEST_RESULT_SET_VOXEL_COLOR, map.Voxels[0].ColorIndex != 7, "Color:%u, expected:7.", map.Voxels[0].ColorIndex);

	LLC_TEST_CHECKF(errors, VOXEL_TEST_RESULT_UPDATE_VALUE, map.SetValue(position, 9) != firstChunk, "%s", "Update returned a different chunk.");
	LLC_TEST_CHECKF(errors, VOXEL_TEST_RESULT_UPDATE_COUNT, map.Voxels.size() != 1, "Voxel count:%u, expected:1.", map.Voxels.size());
	LLC_TEST_CHECKF(errors, VOXEL_TEST_RESULT_UPDATE_VALUE, map.Voxels[0].ColorIndex != 9, "Sparse color:%u, expected:9.", map.Voxels[0].ColorIndex);
	value = {};
	LLC_TEST_CHECKF(errors, VOXEL_TEST_RESULT_UPDATE_VALUE, ::llc::failed(map.GetValue(position, value)), "%s", "Updated cell could not be read.");
	LLC_TEST_CHECKF(errors, VOXEL_TEST_RESULT_UPDATE_VALUE, value != 9, "Cell value:%u, expected:9.", value);

	LLC_TEST_CHECKF(errors, VOXEL_TEST_RESULT_DELETE_VALUE, map.SetValue(position, 0) != firstChunk, "%s", "Clear returned a different chunk.");
	LLC_TEST_CHECKF(errors, VOXEL_TEST_RESULT_DELETE_COUNT, map.Voxels.size(), "Voxel count:%u, expected:0.", map.Voxels.size());
	value = 0xFFU;
	LLC_TEST_CHECKF(errors, VOXEL_TEST_RESULT_DELETE_VALUE, ::llc::failed(map.GetValue(position, value)), "%s", "Cleared cell could not be read.");
	LLC_TEST_CHECKF(errors, VOXEL_TEST_RESULT_DELETE_VALUE, value, "Cell value:%u, expected:0.", value);

	cnst ::llc::n3u0_t nextXChunk = {17, 2, 3};
	cnst ::llc::err_t xChunk = map.SetValue(nextXChunk, 3);
	LLC_TEST_REQUIREF(errors, VOXEL_TEST_RESULT_SET_CHUNK, xChunk != 1, "X chunk:%i, expected:1.", xChunk);
	cnst ::llc::n2u0_t xChunkPosition = {1, 0};
	LLC_TEST_CHECKF(errors, VOXEL_TEST_RESULT_CHUNK_POSITION, map.ChunkPositions[xChunk] != xChunkPosition, "Chunk position:%u,%u, expected:1,0.", map.ChunkPositions[xChunk].x, map.ChunkPositions[xChunk].y);
	cnst ::llc::n3u0_t nextZChunk = {1, 2, 17};
	cnst ::llc::err_t zChunk = map.SetValue(nextZChunk, 4);
	LLC_TEST_REQUIREF(errors, VOXEL_TEST_RESULT_SET_CHUNK, zChunk != 2, "Z chunk:%i, expected:2.", zChunk);
	cnst ::llc::n2u0_t zChunkPosition = {0, 1};
	LLC_TEST_CHECKF(errors, VOXEL_TEST_RESULT_CHUNK_POSITION, map.ChunkPositions[zChunk] != zChunkPosition, "Chunk position:%u,%u, expected:0,1.", map.ChunkPositions[zChunk].x, map.ChunkPositions[zChunk].y);

	value = 0xA5U;
	cnst ::llc::err_t missingResult = map.GetValue({250, 1, 250}, value);
	LLC_TEST_CHECKF(errors, VOXEL_TEST_RESULT_MISSING_RESULT, 0 <= missingResult, "Result:%i.", missingResult);
	LLC_TEST_CHECKF(errors, VOXEL_TEST_RESULT_MISSING_OUTPUT, value != 0xA5U, "Value:%u, expected:165.", value);

	cnst ::llc::SVoxel<::llc::u0_t> minimum = {{0, 0, 0}, 1};
	LLC_TEST_CHECK(errors, VOXEL_TEST_RESULT_FACE_BOTTOM_MINIMUM, false == minimum.Limits(::llc::VOXEL_FACE_Bottom));
	LLC_TEST_CHECK(errors, VOXEL_TEST_RESULT_FACE_BACK_MINIMUM, false == minimum.Limits(::llc::VOXEL_FACE_Back));
	LLC_TEST_CHECK(errors, VOXEL_TEST_RESULT_FACE_LEFT_MINIMUM, false == minimum.Limits(::llc::VOXEL_FACE_Left));
	LLC_TEST_CHECK(errors, VOXEL_TEST_RESULT_FACE_TOP_MINIMUM, minimum.Limits(::llc::VOXEL_FACE_Top));
	LLC_TEST_CHECK(errors, VOXEL_TEST_RESULT_FACE_FRONT_MINIMUM, minimum.Limits(::llc::VOXEL_FACE_Front));
	LLC_TEST_CHECK(errors, VOXEL_TEST_RESULT_FACE_RIGHT_MINIMUM, minimum.Limits(::llc::VOXEL_FACE_Right));
	cnst ::llc::SVoxel<::llc::u0_t> maximum = {{0xFFU, 0xFFU, 0xFFU}, 1};
	LLC_TEST_CHECK(errors, VOXEL_TEST_RESULT_FACE_TOP_MAXIMUM, false == maximum.Limits(::llc::VOXEL_FACE_Top));
	LLC_TEST_CHECK(errors, VOXEL_TEST_RESULT_FACE_FRONT_MAXIMUM, false == maximum.Limits(::llc::VOXEL_FACE_Front));
	LLC_TEST_CHECK(errors, VOXEL_TEST_RESULT_FACE_RIGHT_MAXIMUM, false == maximum.Limits(::llc::VOXEL_FACE_Right));
	LLC_TEST_CHECK(errors, VOXEL_TEST_RESULT_FACE_BOTTOM_MAXIMUM, maximum.Limits(::llc::VOXEL_FACE_Bottom));
	LLC_TEST_CHECK(errors, VOXEL_TEST_RESULT_FACE_BACK_MAXIMUM, maximum.Limits(::llc::VOXEL_FACE_Back));
	LLC_TEST_CHECK(errors, VOXEL_TEST_RESULT_FACE_LEFT_MAXIMUM, maximum.Limits(::llc::VOXEL_FACE_Left));
	cnst ::llc::SVoxel<::llc::u1_t> wideMaximum = {{0xFFFFU, 0xFFFFU, 0xFFFFU}, 1};
	LLC_TEST_CHECK(errors, VOXEL_TEST_RESULT_FACE_TOP_MAXIMUM, false == wideMaximum.Limits(::llc::VOXEL_FACE_Top));
	LLC_TEST_CHECK(errors, VOXEL_TEST_RESULT_FACE_FRONT_MAXIMUM, false == wideMaximum.Limits(::llc::VOXEL_FACE_Front));
	LLC_TEST_CHECK(errors, VOXEL_TEST_RESULT_FACE_RIGHT_MAXIMUM, false == wideMaximum.Limits(::llc::VOXEL_FACE_Right));
	return 0;
}
