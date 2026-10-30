#include "llc_vox.h"
#include "llc_test_core.h"

#include <cstring>

GDEFINE_ENUM_TYPE(VOX_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, LOAD_RESULT	, 0, "SVOXData::Load() rejected a minimal valid VOX file.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, INPUT		, 1, "SVOXData::Load() did not consume the complete VOX file.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, HEADER		, 2, "SVOXData::Load() did not preserve the VOX header.");
GDEFINE_ENUM_VALUED(VOX_TEST_RESULT, MAIN_CHUNK	, 3, "SVOXData::Load() did not retain the MAIN chunk.");

::llc::err_t testVOX(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	::llc::SVOXFileHeader header = {};
	::memcpy(header.MagicNumber.begin(), "VOX ", 4);
	header.Version = 150;
	::llc::SVOXChunkHeader mainChunk = {};
	::memcpy(mainChunk.Type.begin(), "MAIN", 4);
	::llc::u0_t bytes[szof(header) + szof(mainChunk)] = {};
	::memcpy(bytes, &header, szof(header));
	::memcpy(bytes + szof(header), &mainChunk, szof(mainChunk));

	::llc::vcu0_t input = {bytes};
	::llc::SVOXData vox;
	cnst ::llc::err_t result = vox.Load(input);
	LLC_TEST_REQUIRE(errors, VOX_TEST_RESULT_LOAD_RESULT, ::llc::failed(result));
	LLC_TEST_CHECK(errors, VOX_TEST_RESULT_INPUT, input.size());
	LLC_TEST_CHECK(errors, VOX_TEST_RESULT_HEADER, vox.Header.Version != 150 || vox.Header.MagicNumber[0] != 'V' || vox.Header.MagicNumber[1] != 'O' || vox.Header.MagicNumber[2] != 'X' || vox.Header.MagicNumber[3] != ' ');
	LLC_TEST_CHECK(errors, VOX_TEST_RESULT_MAIN_CHUNK, vox.Chunks.size() != 1 || false == vox.Chunks[0].Header.IsType("MAIN"));
	rtrn 0;
}
