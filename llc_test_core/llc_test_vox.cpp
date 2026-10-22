#include "llc_vox.h"
#include "llc_apod_serialize.h"
#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(VOX_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, DOCUMENT_BUILD			,  1, "The synthetic VOX document could not be built.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, LOAD_RESULT				,  2, "SVOXData::Load() rejected a valid document.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, INPUT_CONSUMED			,  3, "SVOXData::Load() did not consume the valid document.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, HEADER_MAGIC			,  4, "SVOXData::Load() stored the wrong file magic.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, HEADER_VERSION			,  5, "SVOXData::Load() stored the wrong file version.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, DIMENSION_X				,  6, "SVOXData::GetDimensions() returned the wrong X dimension.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, DIMENSION_Y				,  7, "SVOXData::GetDimensions() returned the wrong Y dimension.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, DIMENSION_Z				,  8, "SVOXData::GetDimensions() returned the wrong Z dimension.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, VOXEL_COUNT				,  9, "SVOXData::GetXYZI() returned the wrong voxel count.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, VOXEL_X					, 10, "SVOXData::Load() transformed the voxel X coordinate incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, VOXEL_Y					, 11, "SVOXData::Load() transformed the voxel Y coordinate incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, VOXEL_Z					, 12, "SVOXData::Load() transformed the voxel Z coordinate incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, VOXEL_COLOR				, 13, "SVOXData::Load() changed the voxel color index.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, PALETTE_COUNT			, 14, "SVOXData::GetBGRA() returned the wrong palette size.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, PALETTE_BLUE			, 15, "SVOXData::Load() converted the palette blue channel incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, PALETTE_GREEN			, 16, "SVOXData::Load() converted the palette green channel incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, PALETTE_RED				, 17, "SVOXData::Load() converted the palette red channel incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, PALETTE_ALPHA			, 18, "SVOXData::Load() converted the palette alpha channel incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, MATERIAL_COUNT			, 19, "SVOXData::Load() stored the wrong material count.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, MATERIAL_ID				, 20, "SVOXData::Load() parsed the material identifier incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, MATERIAL_VALUE			, 21, "SVOXData::Load() parsed the material dictionary incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, CAMERA_COUNT			, 22, "SVOXData::Load() stored the wrong camera count.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, CAMERA_ID				, 23, "SVOXData::Load() parsed the camera identifier incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, LAYER_COUNT				, 24, "SVOXData::Load() stored the wrong layer count.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, LAYER_ID					, 25, "SVOXData::Load() parsed the layer identifier incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, LAYER_RESERVED			, 26, "SVOXData::Load() parsed the layer reserved field incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, RENDER_COUNT			, 27, "SVOXData::Load() stored the wrong render-attribute count.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, RENDER_VALUE			, 28, "SVOXData::Load() parsed the render dictionary incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, NOTE_CHUNK_COUNT		, 29, "SVOXData::Load() stored the wrong note-chunk count.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, NOTE_COUNT				, 30, "SVOXData::Load() parsed the wrong note count.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, NOTE_FIRST				, 31, "SVOXData::Load() parsed the first note incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, NOTE_SECOND				, 32, "SVOXData::Load() parsed the second note incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, GROUP_COUNT				, 33, "SVOXData::Load() stored the wrong group count.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, GROUP_ID					, 34, "SVOXData::Load() parsed the group identifier incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, GROUP_CHILD_COUNT		, 35, "SVOXData::Load() parsed the wrong group-child count.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, GROUP_FIRST_CHILD		, 36, "SVOXData::Load() parsed the first group child incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, GROUP_SECOND_CHILD		, 37, "SVOXData::Load() parsed the second group child incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, SHAPE_COUNT				, 38, "SVOXData::Load() stored the wrong shape count.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, SHAPE_ID					, 39, "SVOXData::Load() parsed the shape identifier incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, SHAPE_MODEL_COUNT		, 40, "SVOXData::Load() parsed the wrong shape-model count.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, SHAPE_MODEL_ID			, 41, "SVOXData::Load() parsed the shape-model identifier incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, TRANSFORM_COUNT			, 42, "SVOXData::Load() stored the wrong transform count.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, TRANSFORM_FRAME_COUNT	, 43, "SVOXData::Load() parsed the wrong transform-frame count.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, TRANSFORM_ID			, 44, "SVOXData::Load() parsed the transform identifier incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, TRANSFORM_CHILD			, 45, "SVOXData::Load() parsed the transform child incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, TRANSFORM_LAYER			, 46, "SVOXData::Load() parsed the transform layer incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, TRANSFORM_ROTATION		, 47, "SVOXData::Load() parsed transform rotation as a byte instead of a number.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, TRANSFORM_TRANSLATION_X, 48, "SVOXData::Load() parsed transform translation X incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, TRANSFORM_TRANSLATION_Y, 49, "SVOXData::Load() parsed transform translation Y incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, TRANSFORM_TRANSLATION_Z, 50, "SVOXData::Load() parsed transform translation Z incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, TRANSFORM_FRAME			, 51, "SVOXData::Load() parsed the transform frame index incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, TRUNCATION_RESULT		, 52, "SVOXData::Load() accepted a truncated document.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, TRUNCATION_ADDRESS		, 53, "SVOXData::Load() changed the input address after a failed parse.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, TRUNCATION_SIZE			, 54, "SVOXData::Load() changed the input size after a failed parse.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, TRUNCATION_OUTPUT		, 55, "SVOXData::Load() changed output after a failed parse.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, INVALID_MAGIC			, 56, "SVOXData::Load() accepted an invalid file magic.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, OWNED_DICTIONARY		, 57, "A parsed VOX dictionary retained a view into the source bytes.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, OWNED_NOTE				, 58, "A parsed VOX note retained a view into the source bytes.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, OWNED_VOXEL				, 59, "Parsed VOX coordinates retained a view into the source bytes.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, OWNED_PALETTE			, 60, "The parsed VOX palette retained a view into the source bytes.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, DEFAULT_PALETTE_COUNT	, 61, "SVOXData::GetBGRA() did not expose the default palette when RGBA was absent.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, DEFAULT_PALETTE_VALUE	, 62, "SVOXData::GetBGRA() exposed the wrong default palette.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, MATERIAL_ATTRIBUTE_COUNT, 63, "SVOXData::Load() parsed the wrong material-attribute count.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, CAMERA_ATTRIBUTE_COUNT	, 64, "SVOXData::Load() parsed the wrong camera-attribute count.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, CAMERA_VALUE				, 65, "SVOXData::Load() parsed the camera dictionary incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, LAYER_ATTRIBUTE_COUNT	, 66, "SVOXData::Load() parsed the wrong layer-attribute count.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, LAYER_VALUE				, 67, "SVOXData::Load() parsed the layer dictionary incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, RENDER_ATTRIBUTE_COUNT	, 68, "SVOXData::Load() parsed the wrong render-attribute count.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, MODEL_ATTRIBUTE_COUNT	, 69, "SVOXData::Load() parsed the wrong model-attribute count.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, MODEL_VALUE				, 70, "SVOXData::Load() parsed the model dictionary incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, TRANSFORM_ATTRIBUTE_COUNT, 71, "SVOXData::Load() parsed the wrong transform-attribute count.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, TRANSFORM_VALUE			, 72, "SVOXData::Load() parsed the transform dictionary incorrectly.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, PACK_COUNT				, 73, "SVOXData::Load() stored the wrong PACK chunk count.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, PACK_SIZE					, 74, "SVOXData::Load() stored the wrong PACK payload size.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, PACK_VALUE				, 75, "SVOXData::Load() stored the wrong PACK model count.");

sttc ::llc::err_t testVOXAppendText(::llc::au0_t & output, ::llc::vcst_t text) {
	if_fail_fe(::llc::savePOD(output, text.size()));
	if_fail_fe(output.append(text.cu8()));
	rtrn 0;
}

sttc ::llc::err_t testVOXAppendDict(::llc::au0_t & output, ::llc::view<cnst ::llc::kv<::llc::vcst_t>> attributes) {
	if_fail_fe(::llc::savePOD(output, attributes.size()));
	for(::llc::u2_t iAttribute = 0; iAttribute < attributes.size(); ++iAttribute) {
		if_fail_fe(testVOXAppendText(output, attributes[iAttribute].Key));
		if_fail_fe(testVOXAppendText(output, attributes[iAttribute].Val));
	}
	rtrn 0;
}

sttc ::llc::err_t testVOXAppendChunk(::llc::au0_t & output, ::llc::vcst_t type, ::llc::vcu0_t content, ::llc::u2_t childBytes = 0) {
	if_true_fe(4 != type.size());
	::llc::SVOXChunkHeader header = {};
	memcpy(header.Type.begin(), type.begin(), type.size());
	header.ChunkSize = content.size();
	header.ChildChunks = childBytes;
	if_fail_fe(::llc::savePOD(output, header));
	if_fail_fe(output.append(content));
	rtrn 0;
}

sttc ::llc::err_t testVOXDocumentBuild(::llc::au0_t & output) {
	::llc::au0_t children = {}, content = {};
	cnst ::llc::n3u2_t dimensions = {3, 4, 5};
	if_fail_fe(::llc::savePOD(content, dimensions));
	if_fail_fe(testVOXAppendChunk(children, "SIZE", content));
	content.clear();
	cnst ::llc::u2_t packCount = 2;
	if_fail_fe(::llc::savePOD(content, packCount));
	if_fail_fe(testVOXAppendChunk(children, "PACK", content));

	content.clear();
	cnst ::llc::u2_t voxelCount = 1;
	cnst ::llc::SVOXVoxel voxel = {1, 2, 3, 4};
	if_fail_fe(::llc::savePOD(content, voxelCount));
	if_fail_fe(::llc::savePOD(content, voxel));
	if_fail_fe(testVOXAppendChunk(children, "XYZI", content));

	if_true_fe(1024 != content.resize(1024));
	content.fill(0);
	content[0] = 1; content[1] = 2; content[2] = 3; content[3] = 4;
	if_fail_fe(testVOXAppendChunk(children, "RGBA", content));

	content.clear();
	cnst ::llc::s2_t materialId = 11;
	cnst ::llc::kv<::llc::vcst_t> materialAttributes[] = {{"_type", "_diffuse"}};
	if_fail_fe(::llc::savePOD(content, materialId));
	if_fail_fe(testVOXAppendDict(content, materialAttributes));
	if_fail_fe(testVOXAppendChunk(children, "MATL", content));

	content.clear();
	cnst ::llc::s2_t cameraId = 12;
	cnst ::llc::kv<::llc::vcst_t> cameraAttributes[] = {{"_mode", "pers"}};
	if_fail_fe(::llc::savePOD(content, cameraId));
	if_fail_fe(testVOXAppendDict(content, cameraAttributes));
	if_fail_fe(testVOXAppendChunk(children, "rCAM", content));

	content.clear();
	cnst ::llc::s2_t layerId = 13, layerReserved = -1;
	cnst ::llc::kv<::llc::vcst_t> layerAttributes[] = {{"_name", "Layer"}};
	if_fail_fe(::llc::savePOD(content, layerId));
	if_fail_fe(testVOXAppendDict(content, layerAttributes));
	if_fail_fe(::llc::savePOD(content, layerReserved));
	if_fail_fe(testVOXAppendChunk(children, "LAYR", content));

	content.clear();
	cnst ::llc::kv<::llc::vcst_t> renderAttributes[] = {{"_type", "_sky"}};
	if_fail_fe(testVOXAppendDict(content, renderAttributes));
	if_fail_fe(testVOXAppendChunk(children, "rOBJ", content));

	content.clear();
	cnst ::llc::u2_t noteCount = 2;
	if_fail_fe(::llc::savePOD(content, noteCount));
	if_fail_fe(testVOXAppendText(content, "red"));
	if_fail_fe(testVOXAppendText(content, "blue"));
	if_fail_fe(testVOXAppendChunk(children, "NOTE", content));

	content.clear();
	cnst ::llc::s2_t groupId = 14, childIds[] = {20, 21};
	cnst ::llc::u2_t emptyDict = 0, childCount = ::llc::size(childIds);
	if_fail_fe(::llc::savePOD(content, groupId));
	if_fail_fe(::llc::savePOD(content, emptyDict));
	if_fail_fe(::llc::savePOD(content, childCount));
	if_fail_fe(content.append(::llc::view<cnst ::llc::s2_t>{childIds}.cu8()));
	if_fail_fe(testVOXAppendChunk(children, "nGRP", content));

	content.clear();
	cnst ::llc::s2_t shapeId = 15, modelId = 16;
	cnst ::llc::u2_t modelCount = 1;
	cnst ::llc::kv<::llc::vcst_t> modelAttributes[] = {{"_name", "Model"}};
	if_fail_fe(::llc::savePOD(content, shapeId));
	if_fail_fe(::llc::savePOD(content, emptyDict));
	if_fail_fe(::llc::savePOD(content, modelCount));
	if_fail_fe(::llc::savePOD(content, modelId));
	if_fail_fe(testVOXAppendDict(content, modelAttributes));
	if_fail_fe(testVOXAppendChunk(children, "nSHP", content));

	content.clear();
	cnst ::llc::s2_t transformId = 17, transformChild = 18, transformReserved = -1, transformLayer = 13;
	cnst ::llc::u2_t frameCount = 1;
	cnst ::llc::kv<::llc::vcst_t> transformAttributes[] = {{"_name", "Root"}};
	cnst ::llc::kv<::llc::vcst_t> frameAttributes[] = {{"_r", "17"}, {"_t", "-1 2 -3"}, {"_f", "4"}};
	if_fail_fe(::llc::savePOD(content, transformId));
	if_fail_fe(testVOXAppendDict(content, transformAttributes));
	if_fail_fe(::llc::savePOD(content, transformChild));
	if_fail_fe(::llc::savePOD(content, transformReserved));
	if_fail_fe(::llc::savePOD(content, transformLayer));
	if_fail_fe(::llc::savePOD(content, frameCount));
	if_fail_fe(testVOXAppendDict(content, frameAttributes));
	if_fail_fe(testVOXAppendChunk(children, "nTRN", content));

	::llc::SVOXFileHeader fileHeader = {};
	memcpy(fileHeader.MagicNumber.begin(), "VOX ", fileHeader.MagicNumber.size());
	fileHeader.Version = 150;
	if_fail_fe(::llc::savePOD(output, fileHeader));
	if_fail_fe(testVOXAppendChunk(output, "MAIN", {}, children.size()));
	if_fail_fe(output.append(children));
	rtrn 0;
}

::llc::err_t testVOX(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	cnst ::llc::view<cnst ::llc::bgra> defaultPalette = ::llc::SVOXData{}.GetBGRA();
	LLC_TEST_REQUIREF(errors, VOX_TEST_RESULT_DEFAULT_PALETTE_COUNT, defaultPalette.size() != 256, "Count:%u, expected:256.", defaultPalette.size());
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_DEFAULT_PALETTE_VALUE, defaultPalette[1] != ::llc::VOX_PALETTE_DEFAULT[1], "Value:0x%08X, expected:0x%08X.", (::llc::u2_t)defaultPalette[1], (::llc::u2_t)::llc::VOX_PALETTE_DEFAULT[1]);
	::llc::au0_t file = {};
	cnst ::llc::err_t buildResult = testVOXDocumentBuild(file);
	LLC_TEST_REQUIREF(errors, VOX_TEST_RESULT_DOCUMENT_BUILD, ::llc::failed(buildResult), "Result:%i.", buildResult);
	::llc::vcu0_t input = file;
	::llc::SVOXData data = {};
	cnst ::llc::err_t loadResult = data.Load(input);
	LLC_TEST_REQUIREF(errors, VOX_TEST_RESULT_LOAD_RESULT, ::llc::failed(loadResult), "Result:%i.", loadResult);
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_INPUT_CONSUMED, input.size(), "Remaining bytes:%u.", input.size());
	cnst ::llc::vcst_t loadedMagic{data.Header.MagicNumber};
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_HEADER_MAGIC, loadedMagic != "VOX ", "Magic:%.*s.", LLC_VIEW_ARGS(loadedMagic));
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_HEADER_VERSION, data.Header.Version != 150, "Version:%u, expected:150.", data.Header.Version);

	cnst ::llc::n3u0_t parsedDimensions = data.GetDimensions();
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_DIMENSION_X, parsedDimensions.x != 4, "X:%u, expected:4.", parsedDimensions.x);
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_DIMENSION_Y, parsedDimensions.y != 5, "Y:%u, expected:5.", parsedDimensions.y);
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_DIMENSION_Z, parsedDimensions.z != 3, "Z:%u, expected:3.", parsedDimensions.z);
	::llc::u2_t storedPackCount = 0, storedPackValue = 0;
	for(::llc::u2_t iChunk = 0; iChunk < data.Chunks.size(); ++iChunk)
		if(data.Chunks[iChunk].Header.IsType("PACK")) {
			++storedPackCount;
			LLC_TEST_REQUIREF(errors, VOX_TEST_RESULT_PACK_SIZE, data.Chunks[iChunk].Data.size() != szof(storedPackValue), "Size:%u, expected:%u.", data.Chunks[iChunk].Data.size(), (::llc::u2_t)szof(storedPackValue));
			memcpy(&storedPackValue, data.Chunks[iChunk].Data.begin(), szof(storedPackValue));
		}
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_PACK_COUNT, storedPackCount != 1, "Count:%u, expected:1.", storedPackCount);
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_PACK_VALUE, storedPackValue != 2, "Value:%u, expected:2.", storedPackValue);
	cnst ::llc::view<cnst ::llc::SVOXVoxel> voxels = data.GetXYZI();
	LLC_TEST_REQUIREF(errors, VOX_TEST_RESULT_VOXEL_COUNT, voxels.size() != 1, "Count:%u, expected:1.", voxels.size());
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_VOXEL_X, voxels[0].x != 2, "X:%u, expected:2.", voxels[0].x);
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_VOXEL_Y, voxels[0].y != 3, "Y:%u, expected:3.", voxels[0].y);
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_VOXEL_Z, voxels[0].z != 1, "Z:%u, expected:1.", voxels[0].z);
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_VOXEL_COLOR, voxels[0].i != 4, "Color:%u, expected:4.", voxels[0].i);
	cnst ::llc::view<cnst ::llc::bgra> palette = data.GetBGRA();
	LLC_TEST_REQUIREF(errors, VOX_TEST_RESULT_PALETTE_COUNT, palette.size() != 256, "Count:%u, expected:256.", palette.size());
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_PALETTE_BLUE, palette[0].b != 3, "Blue:%u, expected:3.", palette[0].b);
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_PALETTE_GREEN, palette[0].g != 2, "Green:%u, expected:2.", palette[0].g);
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_PALETTE_RED, palette[0].r != 1, "Red:%u, expected:1.", palette[0].r);
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_PALETTE_ALPHA, palette[0].a != 4, "Alpha:%u, expected:4.", palette[0].a);

	LLC_TEST_REQUIREF(errors, VOX_TEST_RESULT_MATERIAL_COUNT, data.ChunksMaterial.size() != 1, "Count:%u, expected:1.", data.ChunksMaterial.size());
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_MATERIAL_ID, data.ChunksMaterial[0].Id != 11, "Id:%i, expected:11.", data.ChunksMaterial[0].Id);
	LLC_TEST_REQUIREF(errors, VOX_TEST_RESULT_MATERIAL_ATTRIBUTE_COUNT, data.ChunksMaterial[0].Attributes.size() != 1, "Count:%u, expected:1.", data.ChunksMaterial[0].Attributes.size());
	cnst ::llc::vcst_t materialValue = data.ChunksMaterial[0].Attributes[0].Val;
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_MATERIAL_VALUE, materialValue != "_diffuse", "Value:%.*s.", LLC_VIEW_ARGS(materialValue));
	LLC_TEST_REQUIREF(errors, VOX_TEST_RESULT_CAMERA_COUNT, data.ChunksCamera.size() != 1, "Count:%u, expected:1.", data.ChunksCamera.size());
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_CAMERA_ID, data.ChunksCamera[0].Id != 12, "Id:%i, expected:12.", data.ChunksCamera[0].Id);
	LLC_TEST_REQUIREF(errors, VOX_TEST_RESULT_CAMERA_ATTRIBUTE_COUNT, data.ChunksCamera[0].Attributes.size() != 1, "Count:%u, expected:1.", data.ChunksCamera[0].Attributes.size());
	cnst ::llc::vcst_t cameraValue = data.ChunksCamera[0].Attributes[0].Val;
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_CAMERA_VALUE, cameraValue != "pers", "Value:%.*s.", LLC_VIEW_ARGS(cameraValue));
	LLC_TEST_REQUIREF(errors, VOX_TEST_RESULT_LAYER_COUNT, data.ChunksLayer.size() != 1, "Count:%u, expected:1.", data.ChunksLayer.size());
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_LAYER_ID, data.ChunksLayer[0].Id != 13, "Id:%i, expected:13.", data.ChunksLayer[0].Id);
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_LAYER_RESERVED, data.ChunksLayer[0].IdReserved != -1, "Reserved:%i, expected:-1.", data.ChunksLayer[0].IdReserved);
	LLC_TEST_REQUIREF(errors, VOX_TEST_RESULT_LAYER_ATTRIBUTE_COUNT, data.ChunksLayer[0].Attributes.size() != 1, "Count:%u, expected:1.", data.ChunksLayer[0].Attributes.size());
	cnst ::llc::vcst_t layerValue = data.ChunksLayer[0].Attributes[0].Val;
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_LAYER_VALUE, layerValue != "Layer", "Value:%.*s.", LLC_VIEW_ARGS(layerValue));
	LLC_TEST_REQUIREF(errors, VOX_TEST_RESULT_RENDER_COUNT, data.ChunksRenderAttributes.size() != 1, "Count:%u, expected:1.", data.ChunksRenderAttributes.size());
	LLC_TEST_REQUIREF(errors, VOX_TEST_RESULT_RENDER_ATTRIBUTE_COUNT, data.ChunksRenderAttributes[0].Attributes.size() != 1, "Count:%u, expected:1.", data.ChunksRenderAttributes[0].Attributes.size());
	cnst ::llc::vcst_t renderValue = data.ChunksRenderAttributes[0].Attributes[0].Val;
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_RENDER_VALUE, renderValue != "_sky", "Value:%.*s.", LLC_VIEW_ARGS(renderValue));
	LLC_TEST_REQUIREF(errors, VOX_TEST_RESULT_NOTE_CHUNK_COUNT, data.ChunksNote.size() != 1, "Count:%u, expected:1.", data.ChunksNote.size());
	LLC_TEST_REQUIREF(errors, VOX_TEST_RESULT_NOTE_COUNT, data.ChunksNote[0].ColorNames.size() != 2, "Count:%u, expected:2.", data.ChunksNote[0].ColorNames.size());
	cnst ::llc::vcst_t firstNote = data.ChunksNote[0].ColorNames[0];
	cnst ::llc::vcst_t secondNote = data.ChunksNote[0].ColorNames[1];
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_NOTE_FIRST, firstNote != "red", "Value:%.*s.", LLC_VIEW_ARGS(firstNote));
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_NOTE_SECOND, secondNote != "blue", "Value:%.*s.", LLC_VIEW_ARGS(secondNote));
	LLC_TEST_REQUIREF(errors, VOX_TEST_RESULT_GROUP_COUNT, data.ChunksNode.size() != 1, "Count:%u, expected:1.", data.ChunksNode.size());
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_GROUP_ID, data.ChunksNode[0].Id != 14, "Id:%i, expected:14.", data.ChunksNode[0].Id);
	LLC_TEST_REQUIREF(errors, VOX_TEST_RESULT_GROUP_CHILD_COUNT, data.ChunksNode[0].IdChildren.size() != 2, "Count:%u, expected:2.", data.ChunksNode[0].IdChildren.size());
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_GROUP_FIRST_CHILD, data.ChunksNode[0].IdChildren[0] != 20, "Id:%i, expected:20.", data.ChunksNode[0].IdChildren[0]);
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_GROUP_SECOND_CHILD, data.ChunksNode[0].IdChildren[1] != 21, "Id:%i, expected:21.", data.ChunksNode[0].IdChildren[1]);
	LLC_TEST_REQUIREF(errors, VOX_TEST_RESULT_SHAPE_COUNT, data.ChunksShape.size() != 1, "Count:%u, expected:1.", data.ChunksShape.size());
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_SHAPE_ID, data.ChunksShape[0].Id != 15, "Id:%i, expected:15.", data.ChunksShape[0].Id);
	LLC_TEST_REQUIREF(errors, VOX_TEST_RESULT_SHAPE_MODEL_COUNT, data.ChunksShape[0].Models.size() != 1, "Count:%u, expected:1.", data.ChunksShape[0].Models.size());
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_SHAPE_MODEL_ID, data.ChunksShape[0].Models[0].Id != 16, "Id:%i, expected:16.", data.ChunksShape[0].Models[0].Id);
	LLC_TEST_REQUIREF(errors, VOX_TEST_RESULT_MODEL_ATTRIBUTE_COUNT, data.ChunksShape[0].Models[0].Attributes.size() != 1, "Count:%u, expected:1.", data.ChunksShape[0].Models[0].Attributes.size());
	cnst ::llc::vcst_t modelValue = data.ChunksShape[0].Models[0].Attributes[0].Val;
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_MODEL_VALUE, modelValue != "Model", "Value:%.*s.", LLC_VIEW_ARGS(modelValue));

	LLC_TEST_REQUIREF(errors, VOX_TEST_RESULT_TRANSFORM_COUNT, data.ChunksTransform.size() != 1, "Count:%u, expected:1.", data.ChunksTransform.size());
	LLC_TEST_REQUIREF(errors, VOX_TEST_RESULT_TRANSFORM_FRAME_COUNT, data.ChunksTransform[0].Frames.size() != 1, "Count:%u, expected:1.", data.ChunksTransform[0].Frames.size());
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_TRANSFORM_ID, data.ChunksTransform[0].Id != 17, "Id:%i, expected:17.", data.ChunksTransform[0].Id);
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_TRANSFORM_CHILD, data.ChunksTransform[0].IdChild != 18, "Child:%i, expected:18.", data.ChunksTransform[0].IdChild);
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_TRANSFORM_LAYER, data.ChunksTransform[0].IdLayer != 13, "Layer:%i, expected:13.", data.ChunksTransform[0].IdLayer);
	LLC_TEST_REQUIREF(errors, VOX_TEST_RESULT_TRANSFORM_ATTRIBUTE_COUNT, data.ChunksTransform[0].Attributes.size() != 1, "Count:%u, expected:1.", data.ChunksTransform[0].Attributes.size());
	cnst ::llc::vcst_t transformValue = data.ChunksTransform[0].Attributes[0].Val;
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_TRANSFORM_VALUE, transformValue != "Root", "Value:%.*s.", LLC_VIEW_ARGS(transformValue));
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_TRANSFORM_ROTATION, data.ChunksTransform[0].Frames[0].Rotation != 17, "Rotation:%u, expected:17.", data.ChunksTransform[0].Frames[0].Rotation);
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_TRANSFORM_TRANSLATION_X, data.ChunksTransform[0].Frames[0].Translation.x != -1, "X:%i, expected:-1.", data.ChunksTransform[0].Frames[0].Translation.x);
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_TRANSFORM_TRANSLATION_Y, data.ChunksTransform[0].Frames[0].Translation.y != -3, "Y:%i, expected:-3.", data.ChunksTransform[0].Frames[0].Translation.y);
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_TRANSFORM_TRANSLATION_Z, data.ChunksTransform[0].Frames[0].Translation.z != 2, "Z:%i, expected:2.", data.ChunksTransform[0].Frames[0].Translation.z);
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_TRANSFORM_FRAME, data.ChunksTransform[0].Frames[0].IndexFrame != 4, "Frame:%u, expected:4.", data.ChunksTransform[0].Frames[0].IndexFrame);

	for(::llc::u2_t iByte = 0; iByte < file.size(); ++iByte) {
		::llc::vcu0_t truncated = {file.begin(), iByte};
		::llc::SVOXData truncatedData = {};
		truncatedData.Header.Version = 9;
		cnst ::llc::err_t truncatedResult = truncatedData.Load(truncated);
		LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_TRUNCATION_RESULT, 0 <= truncatedResult, "Prefix:%u, result:%i.", iByte, truncatedResult);
		LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_TRUNCATION_ADDRESS, truncated.begin() != file.begin(), "Prefix:%u changed the input address.", iByte);
		LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_TRUNCATION_SIZE, truncated.size() != iByte, "Prefix:%u left size:%u.", iByte, truncated.size());
		LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_TRUNCATION_OUTPUT, truncatedData.Header.Version != 9, "Prefix:%u changed version:%u.", iByte, truncatedData.Header.Version);
	}

	::llc::au0_t invalidMagic = file;
	invalidMagic[0] = 'B';
	::llc::vcu0_t invalidInput = invalidMagic;
	cnst ::llc::err_t invalidResult = data.Load(invalidInput);
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_INVALID_MAGIC, 0 <= invalidResult, "Result:%i.", invalidResult);

	file.fill(0);
	cnst ::llc::vcst_t ownedMaterialValue = data.ChunksMaterial[0].Attributes[0].Val;
	cnst ::llc::vcst_t ownedNoteValue = data.ChunksNote[0].ColorNames[1];
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_OWNED_DICTIONARY, ownedMaterialValue != "_diffuse", "Value:%.*s.", LLC_VIEW_ARGS(ownedMaterialValue));
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_OWNED_NOTE, ownedNoteValue != "blue", "Value:%.*s.", LLC_VIEW_ARGS(ownedNoteValue));
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_OWNED_VOXEL, voxels[0].x != 2, "X:%u, expected:2.", voxels[0].x);
	LLC_TEST_CHECKF(errors, VOX_TEST_RESULT_OWNED_PALETTE, palette[0].b != 3, "Blue:%u, expected:3.", palette[0].b);
	rtrn 0;
}
