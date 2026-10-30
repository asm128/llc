#include "llc_test_core.h"
#include "llc_apod_serialize.h"
#include "llc_file.h"
#include "llc_ro_map.h"

GDEFINE_ENUM_TYPE(RAGNAROK_GAT_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(RAGNAROK_GAT_RESULT, FIXTURE			, 1, "The in-memory GAT fixture could not be assembled.");
GDEFINE_ENUM_VALUED(RAGNAROK_GAT_RESULT, LOAD				, 2, "A valid GAT 1.2 stream must load and report its consumed bytes.");
GDEFINE_ENUM_VALUED(RAGNAROK_GAT_RESULT, HEADER			, 3, "GAT version and dimensions must match the serialized header.");
GDEFINE_ENUM_VALUED(RAGNAROK_GAT_RESULT, TILES			, 4, "GAT tiles must preserve heights, type and opaque bytes.");
GDEFINE_ENUM_VALUED(RAGNAROK_GAT_RESULT, SIGNATURE		, 5, "A non-GRAT signature must be rejected without modifying output.");
GDEFINE_ENUM_VALUED(RAGNAROK_GAT_RESULT, VERSION			, 6, "An unsupported GAT version must be rejected while retaining the decoded version for analysis.");
GDEFINE_ENUM_VALUED(RAGNAROK_GAT_RESULT, TRUNCATED		, 7, "Every truncated GAT prefix must be rejected while retaining the last decoded state.");
GDEFINE_ENUM_VALUED(RAGNAROK_GAT_RESULT, OVERFLOW		, 8, "Overflowing GAT dimensions must be rejected while retaining the decoded header and prior tile allocation.");
GDEFINE_ENUM_VALUED(RAGNAROK_GAT_RESULT, REUSE			, 9, "Repeated GAT loads of equal-sized data must reuse the tile allocation.");
GDEFINE_ENUM_VALUED(RAGNAROK_GAT_RESULT, CORPUS_FILE		, 10, "The real GAT corpus fixture could not be read.");
GDEFINE_ENUM_VALUED(RAGNAROK_GAT_RESULT, CORPUS_LOAD		, 11, "The real GAT 1.2 corpus fixture could not be decoded completely.");
GDEFINE_ENUM_VALUED(RAGNAROK_GAT_RESULT, CORPUS_LAYOUT	, 12, "The real GAT corpus fixture produced unexpected dimensions or tile count.");

stxp ::llc::u0_t GAT_MAGIC[] = {'G', 'R', 'A', 'T'};
stxp ::llc::vcst_t GAT_CORPUS_PATH = "../llc_ro_data/extracted_grf/ragnarok_2006_04_25_rag/data_grf/data/job_soul.gat";

sttc ::llc::err_t gatFixture(::llc::au0_t & bytes, ::llc::SROGATTile (&tiles)[2]) {
	stxp ::llc::SROVersion version = {1, 2};
	stxp ::llc::n2u2_t dimensions = {2, 1};
	tiles[0] = {{1, 2, 3, 4}, 1, {5, 6, 7}};
	tiles[1] = {{-1, -2, -3, -4}, 6, {8, 9, 10}};
	if_fail_fe(bytes.append(GAT_MAGIC));
	if_fail_fe(::llc::savePOD(bytes, version));
	if_fail_fe(::llc::savePOD(bytes, dimensions));
	return bytes.append(::llc::view<cnst ::llc::SROGATTile>{tiles}.cu8());
}

sttc ::llc::err_t gatSentinel(::llc::SROGAT & output) {
	output.Version = {9, 8};
	output.Dimensions = {7, 6};
	if_fail_fe(output.Tiles.resize(1));
	output.Tiles[0] = {{5, 4, 3, 2}, 0xA5, {1, 3, 5}};
	return 0;
}

sttc bool gatIsSentinel(cnst ::llc::SROGAT & output) {
	stxp ::llc::SROGATTile sentinel = {{5, 4, 3, 2}, 0xA5, {1, 3, 5}};
	return output.Version.Major == 9 && output.Version.Minor == 8
		&& output.Dimensions == ::llc::n2u2_t{7, 6}
		&& output.Tiles.size() == 1
		&& 0 == memcmp(&output.Tiles[0], &sentinel, szof(sentinel));
}

::llc::err_t testRagnarokGAT(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	::llc::au0_t bytes;
	::llc::SROGATTile expectedTiles[2] = {};
	LLC_TEST_REQUIRE(errors, RAGNAROK_GAT_RESULT_FIXTURE, ::llc::failed(gatFixture(bytes, expectedTiles)));

	::llc::SROGAT loaded;
	cnst ::llc::err_t loadResult = ::llc::roGATLoad(loaded, bytes);
	LLC_TEST_REQUIREF(errors, RAGNAROK_GAT_RESULT_LOAD, loadResult != (::llc::err_t)bytes.size(), "Result:%i, expected:%u.", loadResult, bytes.size());
	LLC_TEST_CHECK(errors, RAGNAROK_GAT_RESULT_HEADER, (loaded.Version.Major != 1 || loaded.Version.Minor != 2 || loaded.Dimensions != ::llc::n2u2_t{2, 1}));
	LLC_TEST_REQUIRE(errors, RAGNAROK_GAT_RESULT_TILES, loaded.Tiles.size() != ::llc::size(expectedTiles));
	LLC_TEST_CHECK(errors, RAGNAROK_GAT_RESULT_TILES, 0 != memcmp(loaded.Tiles.begin(), expectedTiles, szof(expectedTiles)));

	::llc::au0_t invalid = bytes;
	invalid[0] = 'X';
	::llc::SROGAT rejected;
	if_fail_fe(gatSentinel(rejected));
	LLC_TEST_CHECK(errors, RAGNAROK_GAT_RESULT_SIGNATURE, 0 <= ::llc::roGATLoad(rejected, invalid) || false == gatIsSentinel(rejected));

	invalid = bytes;
	invalid[4] = 2;
	if_fail_fe(gatSentinel(rejected));
	LLC_TEST_CHECK(errors, RAGNAROK_GAT_RESULT_VERSION, (0 <= ::llc::roGATLoad(rejected, invalid) || rejected.Version.Major != 2 || rejected.Version.Minor != 2
		|| rejected.Dimensions != ::llc::n2u2_t{7, 6} || rejected.Tiles.size() != 1 || rejected.Tiles[0].Height[0] != 5));

	for(::llc::u2_t byteCount = 0; byteCount < bytes.size(); ++byteCount) {
		if_fail_fe(gatSentinel(rejected));
		cnst ::llc::err_t truncatedResult = ::llc::roGATLoad(rejected, {bytes.begin(), byteCount});
		LLC_TEST_CHECKF(errors, RAGNAROK_GAT_RESULT_TRUNCATED, 0 <= truncatedResult, "Accepted prefix:%u/%u, result:%i.", byteCount, bytes.size(), truncatedResult);
	}
	if_fail_fe(gatSentinel(rejected));
	cnst ::llc::err_t partialResult = ::llc::roGATLoad(rejected, {bytes.begin(), bytes.size() - 1});
	LLC_TEST_CHECK(errors, RAGNAROK_GAT_RESULT_TRUNCATED, (0 <= partialResult || rejected.Version.Major != 1 || rejected.Version.Minor != 2
		|| rejected.Dimensions != ::llc::n2u2_t{2, 1} || rejected.Tiles.size() != 1 || rejected.Tiles[0].Height[0] != 5));

	::llc::SROGAT reused;
	LLC_TEST_REQUIRE(errors, RAGNAROK_GAT_RESULT_REUSE, ::llc::failed(::llc::roGATLoad(reused, bytes)));
	cnst ::llc::SROGATTile * const tileAllocation = reused.Tiles.begin();
	LLC_TEST_REQUIRE(errors, RAGNAROK_GAT_RESULT_REUSE, ::llc::failed(::llc::roGATLoad(reused, bytes)));
	LLC_TEST_CHECK(errors, RAGNAROK_GAT_RESULT_REUSE, tileAllocation != reused.Tiles.begin());

	::llc::au0_t overflow;
	stxp ::llc::SROVersion version = {1, 2};
	stxp ::llc::n2u2_t dimensions = {(::llc::u2_t)-1, (::llc::u2_t)-1};
	if_fail_fe(overflow.append(GAT_MAGIC));
	if_fail_fe(::llc::savePOD(overflow, version));
	if_fail_fe(::llc::savePOD(overflow, dimensions));
	if_fail_fe(gatSentinel(rejected));
	LLC_TEST_CHECK(errors, RAGNAROK_GAT_RESULT_OVERFLOW, 0 <= ::llc::roGATLoad(rejected, overflow) || rejected.Version.Major != 1 || rejected.Version.Minor != 2
		|| rejected.Dimensions != dimensions || rejected.Tiles.size() != 1 || rejected.Tiles[0].Height[0] != 5);

	::llc::au0_t corpusBytes;
	LLC_TEST_REQUIRE(errors, RAGNAROK_GAT_RESULT_CORPUS_FILE, ::llc::failed(::llc::fileToMemory(GAT_CORPUS_PATH, corpusBytes)));
	::llc::SROGAT corpusLoaded;
	cnst ::llc::err_t corpusResult = ::llc::roGATLoad(corpusLoaded, corpusBytes);
	LLC_TEST_REQUIRE(errors, RAGNAROK_GAT_RESULT_CORPUS_LOAD, corpusResult != (::llc::err_t)corpusBytes.size());
	LLC_TEST_CHECK(errors, RAGNAROK_GAT_RESULT_CORPUS_LAYOUT, (corpusLoaded.Version.Major != 1 || corpusLoaded.Version.Minor != 2
		|| corpusLoaded.Dimensions != ::llc::n2u2_t{60, 60} || corpusLoaded.Tiles.size() != 3600));
	return 0;
}

GDEFINE_ENUM_TYPE(RAGNAROK_GND_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(RAGNAROK_GND_RESULT, FIXTURE			, 1, "The in-memory GND fixture could not be assembled.");
GDEFINE_ENUM_VALUED(RAGNAROK_GND_RESULT, LOAD_15			, 2, "A valid GND 1.5 stream must load and report its consumed bytes.");
GDEFINE_ENUM_VALUED(RAGNAROK_GND_RESULT, LOAD_17			, 3, "A valid GND 1.7 stream must load and report its consumed bytes.");
GDEFINE_ENUM_VALUED(RAGNAROK_GND_RESULT, HEADER			, 4, "GND version, dimensions and scale must match the serialized header.");
GDEFINE_ENUM_VALUED(RAGNAROK_GND_RESULT, TEXTURES		, 5, "GND fixed-width texture names must be preserved.");
GDEFINE_ENUM_VALUED(RAGNAROK_GND_RESULT, LIGHTMAPS		, 6, "GND lightmap metadata, brightness and RGB values must be preserved.");
GDEFINE_ENUM_VALUED(RAGNAROK_GND_RESULT, SKINS			, 7, "GND UV, texture, lightmap and color skin data must be preserved.");
GDEFINE_ENUM_VALUED(RAGNAROK_GND_RESULT, TILES			, 8, "GND heights, face skins and the 1.5 flag must be preserved.");
GDEFINE_ENUM_VALUED(RAGNAROK_GND_RESULT, SIGNATURE		, 9, "A non-GRGN signature must be rejected before modifying output.");
GDEFINE_ENUM_VALUED(RAGNAROK_GND_RESULT, VERSION			, 10, "An unsupported GND version must be rejected while retaining the decoded version for analysis.");
GDEFINE_ENUM_VALUED(RAGNAROK_GND_RESULT, TRUNCATED		, 11, "Every truncated GND prefix must be rejected while retaining the last decoded state.");
GDEFINE_ENUM_VALUED(RAGNAROK_GND_RESULT, OVERFLOW		, 12, "Overflowing GND dimensions must be rejected while retaining the decoded header and prior tile allocation.");
GDEFINE_ENUM_VALUED(RAGNAROK_GND_RESULT, DETACHED_REFERENCES, 13, "Structurally valid GND streams must preserve face references even when optional skin tables are absent.");
GDEFINE_ENUM_VALUED(RAGNAROK_GND_RESULT, REUSE			, 14, "Repeated GND loads of equal-sized data must reuse owned allocations.");
GDEFINE_ENUM_VALUED(RAGNAROK_GND_RESULT, CORPUS_FILE		, 15, "A real GND corpus fixture could not be read.");
GDEFINE_ENUM_VALUED(RAGNAROK_GND_RESULT, CORPUS_LOAD		, 16, "A real GND corpus fixture could not be decoded completely.");
GDEFINE_ENUM_VALUED(RAGNAROK_GND_RESULT, CORPUS_LAYOUT	, 17, "A real GND corpus fixture produced unexpected metadata or section counts.");
GDEFINE_ENUM_VALUED(RAGNAROK_GND_RESULT, CORPUS_REUSE	, 18, "A smaller real GND load did not reuse allocations retained from the larger corpus fixture.");

stxp ::llc::u0_t GND_MAGIC[] = {'G', 'R', 'G', 'N'};

struct SGNDCorpusCase {
	::llc::vcst_t			Path						= {};
	::llc::SROVersion		Version					= {};
	::llc::n2u2_t			Dimensions					= {};
	::llc::u2_t			TextureCount					= {};
	::llc::u2_t			LightmapCount					= {};
	::llc::u2_t			SkinCount						= {};
};

stxp SGNDCorpusCase GND_CORPUS_CASES[] =
	{ {"../llc_ro_data/extracted_grf/alpha1_2001_08_30/pdata000_grf/data/moc_intr04.gnd", {1, 5}, {100, 100}, 6, 1, 3862}
	, {"../llc_ro_data/extracted_grf/ragnarok_2006_04_25_rag/data_grf/data/job_soul.gnd", {1, 7}, {30, 30}, 1, 1, 900}
	};

struct SGNDFixtureOffsets {
	::llc::u2_t				LightmapWidth				= {};
};

sttc ::llc::err_t gndFixture(::llc::au0_t & bytes, ::llc::u0_t minor, SGNDFixtureOffsets & offsets) {
	bytes.clear();
	cnst ::llc::SROVersion version = {1, minor};
	stxp ::llc::n2u2_t dimensions = {1, 1};
	stxp ::llc::f2_t tileScale = 10;
	stxp ::llc::u2_t textureCount = 1;
	stxp ::llc::u2_t textureNameSize = 8;
	stxp ::llc::u0_t textureName[textureNameSize] = {'g', 'r', 'a', 's', 's', '.', 'b', 'm'};
	stxp ::llc::u2_t lightmapCount = 1;
	stxp ::llc::n2u2_t lightmapDimensions = {8, 8};
	stxp ::llc::u2_t lightmapTiles = 1;
	::llc::SROGNDLightmap lightmap = {};
	lightmap.Brightness[0][0] = 17;
	lightmap.Brightness[7][7] = 29;
	lightmap.ColorRGB[0][0] = {1, 2, 3};
	lightmap.ColorRGB[7][7] = {4, 5, 6};
	stxp ::llc::u2_t skinCount = 1;
	::llc::SROGNDSkin skin = {};
	skin.TextureIndex = 0;
	skin.LightmapIndex = 0;
	skin.Color = 0xAABBCCDDU;
	stxp ::llc::f2_t heights[4] = {1, 2, 3, 4};

	if_fail_fe(bytes.append(GND_MAGIC));
	if_fail_fe(::llc::savePOD(bytes, version));
	if_fail_fe(::llc::savePOD(bytes, dimensions));
	if_fail_fe(::llc::savePOD(bytes, tileScale));
	if_fail_fe(::llc::savePOD(bytes, textureCount));
	if_fail_fe(::llc::savePOD(bytes, textureNameSize));
	if_fail_fe(bytes.append(textureName));
	if_fail_fe(::llc::savePOD(bytes, lightmapCount));
	offsets.LightmapWidth = bytes.size();
	if_fail_fe(::llc::savePOD(bytes, lightmapDimensions));
	if_fail_fe(::llc::savePOD(bytes, lightmapTiles));
	if_fail_fe(::llc::savePOD(bytes, lightmap));
	if_fail_fe(::llc::savePOD(bytes, skinCount));
	if_fail_fe(::llc::savePOD(bytes, skin));
	if_fail_fe(::llc::savePOD(bytes, heights));
	if(5 == minor) {
		stxp ::llc::s1_t indicesAndFlags[4] = {0, -1, -1, 0x1234};
		return ::llc::savePOD(bytes, indicesAndFlags);
	}
	stxp ::llc::s2_t indices[3] = {0, -1, -1};
	return ::llc::savePOD(bytes, indices);
}

sttc ::llc::err_t gndDetachedFixture(::llc::au0_t & bytes) {
	bytes.clear();
	stxp ::llc::SROVersion version = {1, 7};
	stxp ::llc::n2u2_t dimensions = {1, 1};
	stxp ::llc::f2_t tileScale = 10;
	stxp ::llc::u2_t textureCount = 0;
	stxp ::llc::u2_t textureNameSize = 80;
	stxp ::llc::u2_t lightmapCount = 0;
	stxp ::llc::n2u2_t lightmapDimensions = {};
	stxp ::llc::u2_t lightmapTiles = 0;
	stxp ::llc::u2_t skinCount = 0;
	stxp ::llc::f2_t heights[4] = {1, 2, 3, 4};
	stxp ::llc::s2_t indices[3] = {0, -1, 0};
	if_fail_fe(bytes.append(GND_MAGIC));
	if_fail_fe(::llc::savePOD(bytes, version));
	if_fail_fe(::llc::savePOD(bytes, dimensions));
	if_fail_fe(::llc::savePOD(bytes, tileScale));
	if_fail_fe(::llc::savePOD(bytes, textureCount));
	if_fail_fe(::llc::savePOD(bytes, textureNameSize));
	if_fail_fe(::llc::savePOD(bytes, lightmapCount));
	if_fail_fe(::llc::savePOD(bytes, lightmapDimensions));
	if_fail_fe(::llc::savePOD(bytes, lightmapTiles));
	if_fail_fe(::llc::savePOD(bytes, skinCount));
	if_fail_fe(::llc::savePOD(bytes, heights));
	return ::llc::savePOD(bytes, indices);
}

sttc ::llc::err_t gndSentinel(::llc::SROGND & output) {
	output.Version = {9, 8};
	output.Dimensions = {7, 6};
	output.TileScale = 5;
	output.TextureNameSize = 8;
	if_fail_fe(output.TextureNames.resize(1));
	output.TextureNames[0].clear();
	if_fail_fe(output.TextureNames[0].append_string("sentinel"));
	output.LightmapDimensions = {4, 3};
	output.LightmapTiles = 2;
	if_fail_fe(output.Lightmaps.resize(1));
	output.Lightmaps[0].Brightness[0][0] = 0xA5;
	output.Lightmaps[0].ColorRGB[0][0] = {9, 7, 5};
	if_fail_fe(output.Skins.resize(1));
	output.Skins[0].TextureIndex = 6;
	output.Skins[0].LightmapIndex = 4;
	output.Skins[0].Color = 0x12345678U;
	if_fail_fe(output.Tiles.resize(1));
	output.Tiles[0] = {{5, 4, 3, 2}, 3, 2, 1, 0x4321};
	return 0;
}

sttc bool gndIsSentinel(cnst ::llc::SROGND & output) {
	stxp ::llc::sc_t sentinel[] = "sentinel";
	return output.Version.Major == 9 && output.Version.Minor == 8
		&& output.Dimensions == ::llc::n2u2_t{7, 6}
		&& output.TileScale == 5 && output.TextureNameSize == 8
		&& output.TextureNames.size() == 1 && output.TextureNames[0].size() == 8
		&& 0 == memcmp(output.TextureNames[0].begin(), sentinel, 8)
		&& output.LightmapDimensions == ::llc::n2u2_t{4, 3} && output.LightmapTiles == 2
		&& output.Lightmaps.size() == 1 && output.Lightmaps[0].Brightness[0][0] == 0xA5 && output.Lightmaps[0].ColorRGB[0][0] == ::llc::n3u0_t{9, 7, 5}
		&& output.Skins.size() == 1 && output.Skins[0].TextureIndex == 6 && output.Skins[0].LightmapIndex == 4 && output.Skins[0].Color == 0x12345678U
		&& output.Tiles.size() == 1 && output.Tiles[0].Height[0] == 5 && output.Tiles[0].SkinIndexTop == 3 && output.Tiles[0].Flags == 0x4321;
}

::llc::err_t testRagnarokGND(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	::llc::au0_t bytes15, bytes17;
	SGNDFixtureOffsets offsets15 = {}, offsets17 = {};
	LLC_TEST_REQUIRE(errors, RAGNAROK_GND_RESULT_FIXTURE, ::llc::failed(gndFixture(bytes15, 5, offsets15)) || ::llc::failed(gndFixture(bytes17, 7, offsets17)));

	for(::llc::u2_t iFixture = 0; iFixture < 2; ++iFixture) {
		cnst ::llc::au0_t & bytes = iFixture ? bytes17 : bytes15;
		::llc::SROGND loaded;
		cnst ::llc::err_t loadResult = ::llc::roGNDLoad(loaded, bytes);
		cnst RAGNAROK_GND_RESULT loadCode = iFixture ? RAGNAROK_GND_RESULT_LOAD_17 : RAGNAROK_GND_RESULT_LOAD_15;
		LLC_TEST_REQUIREF(errors, loadCode, loadResult != (::llc::err_t)bytes.size(), "Result:%i, expected:%u.", loadResult, bytes.size());
		LLC_TEST_CHECK(errors, RAGNAROK_GND_RESULT_HEADER, (loaded.Version.Major != 1 || loaded.Version.Minor != (iFixture ? 7 : 5) || loaded.Dimensions != ::llc::n2u2_t{1, 1} || loaded.TileScale != 10));
		LLC_TEST_CHECK(errors, RAGNAROK_GND_RESULT_TEXTURES, loaded.TextureNameSize != 8 || loaded.TextureNames.size() != 1 || loaded.TextureNames[0].size() != 8 || 0 != memcmp(loaded.TextureNames[0].begin(), "grass.bm", 8));
		LLC_TEST_CHECK(errors, RAGNAROK_GND_RESULT_LIGHTMAPS, (loaded.LightmapDimensions != ::llc::n2u2_t{8, 8} || loaded.LightmapTiles != 1 || loaded.Lightmaps.size() != 1
			|| loaded.Lightmaps[0].Brightness[0][0] != 17 || loaded.Lightmaps[0].Brightness[7][7] != 29 || loaded.Lightmaps[0].ColorRGB[0][0] != ::llc::n3u0_t{1, 2, 3} || loaded.Lightmaps[0].ColorRGB[7][7] != ::llc::n3u0_t{4, 5, 6}));
		LLC_TEST_CHECK(errors, RAGNAROK_GND_RESULT_SKINS, loaded.Skins.size() != 1 || loaded.Skins[0].U[1] != 1 || loaded.Skins[0].V[2] != 1 || loaded.Skins[0].TextureIndex != 0 || loaded.Skins[0].LightmapIndex != 0 || loaded.Skins[0].Color != 0xAABBCCDDU);
		LLC_TEST_CHECK(errors, RAGNAROK_GND_RESULT_TILES, loaded.Tiles.size() != 1 || loaded.Tiles[0].Height[0] != 1 || loaded.Tiles[0].Height[3] != 4 || loaded.Tiles[0].SkinIndexTop != 0 || loaded.Tiles[0].SkinIndexRight != -1 || loaded.Tiles[0].SkinIndexFront != -1 || loaded.Tiles[0].Flags != (iFixture ? 0 : 0x1234));
	}

	::llc::SROGND corpusLoaded;
	::llc::au0_t corpusBytes;
	cnst ::llc::sc_t * corpusNameAllocation = {};
	cnst ::llc::SROGNDLightmap * corpusLightmapAllocation = {};
	cnst ::llc::SROGNDSkin * corpusSkinAllocation = {};
	cnst ::llc::SROGNDTile * corpusTileAllocation = {};
	for(::llc::u2_t iCase = 0; iCase < ::llc::size(GND_CORPUS_CASES); ++iCase) {
		cnst SGNDCorpusCase & corpusCase = GND_CORPUS_CASES[iCase];
		LLC_TEST_REQUIREF(errors, RAGNAROK_GND_RESULT_CORPUS_FILE, ::llc::failed(::llc::fileToMemory(corpusCase.Path, corpusBytes)), "File:'%.*s'.", LLC_VIEW_ARGS(corpusCase.Path));
		cnst ::llc::err_t corpusResult = ::llc::roGNDLoad(corpusLoaded, corpusBytes);
		LLC_TEST_REQUIREF(errors, RAGNAROK_GND_RESULT_CORPUS_LOAD, corpusResult != (::llc::err_t)corpusBytes.size(), "File:'%.*s', result:%i, expected:%u.", LLC_VIEW_ARGS(corpusCase.Path), corpusResult, corpusBytes.size());
		LLC_TEST_REQUIREF(errors, RAGNAROK_GND_RESULT_CORPUS_LAYOUT, (corpusLoaded.Version.Major != corpusCase.Version.Major || corpusLoaded.Version.Minor != corpusCase.Version.Minor
			|| corpusLoaded.Dimensions != corpusCase.Dimensions || corpusLoaded.TileScale != 10 || corpusLoaded.TextureNameSize != 80
			|| corpusLoaded.TextureNames.size() != corpusCase.TextureCount || corpusLoaded.LightmapDimensions != ::llc::n2u2_t{8, 8}
			|| corpusLoaded.LightmapTiles != 1 || corpusLoaded.Lightmaps.size() != corpusCase.LightmapCount || corpusLoaded.Skins.size() != corpusCase.SkinCount
			|| corpusLoaded.Tiles.size() != corpusCase.Dimensions.x * corpusCase.Dimensions.y), "File:'%.*s'.", LLC_VIEW_ARGS(corpusCase.Path));
		if(0 == iCase) {
			corpusNameAllocation = corpusLoaded.TextureNames[0].begin();
			corpusLightmapAllocation = corpusLoaded.Lightmaps.begin();
			corpusSkinAllocation = corpusLoaded.Skins.begin();
			corpusTileAllocation = corpusLoaded.Tiles.begin();
		}
		else
			LLC_TEST_CHECK(errors, RAGNAROK_GND_RESULT_CORPUS_REUSE, corpusNameAllocation != corpusLoaded.TextureNames[0].begin() || corpusLightmapAllocation != corpusLoaded.Lightmaps.begin()
				|| corpusSkinAllocation != corpusLoaded.Skins.begin() || corpusTileAllocation != corpusLoaded.Tiles.begin());
	}

	::llc::au0_t invalid = bytes15;
	invalid[0] = 'X';
	::llc::SROGND rejected;
	if_fail_fe(gndSentinel(rejected));
	LLC_TEST_CHECK(errors, RAGNAROK_GND_RESULT_SIGNATURE, 0 <= ::llc::roGNDLoad(rejected, invalid) || false == gndIsSentinel(rejected));

	invalid = bytes15;
	invalid[5] = 6;
	if_fail_fe(gndSentinel(rejected));
	LLC_TEST_CHECK(errors, RAGNAROK_GND_RESULT_VERSION, (0 <= ::llc::roGNDLoad(rejected, invalid) || rejected.Version.Major != 1 || rejected.Version.Minor != 6
		|| rejected.Dimensions != ::llc::n2u2_t{7, 6} || rejected.TextureNames.size() != 1 || 0 != memcmp(rejected.TextureNames[0].begin(), "sentinel", 8)));

	for(::llc::u2_t iFixture = 0; iFixture < 2; ++iFixture) {
		cnst ::llc::au0_t & bytes = iFixture ? bytes17 : bytes15;
		for(::llc::u2_t byteCount = 0; byteCount < bytes.size(); ++byteCount) {
			if_fail_fe(gndSentinel(rejected));
			cnst ::llc::err_t truncatedResult = ::llc::roGNDLoad(rejected, {bytes.begin(), byteCount});
			LLC_TEST_CHECKF(errors, RAGNAROK_GND_RESULT_TRUNCATED, 0 <= truncatedResult, "Version:%u, accepted prefix:%u/%u, result:%i.", iFixture ? 7U : 5U, byteCount, bytes.size(), truncatedResult);
		}
	}
	if_fail_fe(gndSentinel(rejected));
	cnst ::llc::err_t partialResult = ::llc::roGNDLoad(rejected, {bytes15.begin(), bytes15.size() - 1});
	LLC_TEST_CHECK(errors, RAGNAROK_GND_RESULT_TRUNCATED, (0 <= partialResult || rejected.Version.Major != 1 || rejected.Version.Minor != 5 || rejected.Dimensions != ::llc::n2u2_t{1, 1}
		|| rejected.TextureNames.size() != 1 || 0 != memcmp(rejected.TextureNames[0].begin(), "grass.bm", 8) || rejected.Lightmaps.size() != 1 || rejected.Skins.size() != 1
		|| rejected.Tiles.size() != 1 || rejected.Tiles[0].Height[0] != 5 || rejected.Tiles[0].Flags != 0x4321));

	invalid = bytes15;
	invalid[offsets15.LightmapWidth] = 7;
	if_fail_fe(gndSentinel(rejected));
	LLC_TEST_CHECK(errors, RAGNAROK_GND_RESULT_LIGHTMAPS, (0 <= ::llc::roGNDLoad(rejected, invalid) || rejected.Version.Minor != 5 || rejected.Dimensions != ::llc::n2u2_t{1, 1}
		|| rejected.TextureNames.size() != 1 || 0 != memcmp(rejected.TextureNames[0].begin(), "grass.bm", 8) || rejected.LightmapDimensions != ::llc::n2u2_t{7, 8}
		|| rejected.Lightmaps.size() != 1 || rejected.Lightmaps[0].Brightness[0][0] != 0xA5));

	::llc::au0_t detached;
	LLC_TEST_REQUIRE(errors, RAGNAROK_GND_RESULT_FIXTURE, ::llc::failed(gndDetachedFixture(detached)));
	::llc::SROGND detachedLoaded;
	cnst ::llc::err_t detachedResult = ::llc::roGNDLoad(detachedLoaded, detached);
	LLC_TEST_CHECK(errors, RAGNAROK_GND_RESULT_DETACHED_REFERENCES, detachedResult != (::llc::err_t)detached.size() || detachedLoaded.TextureNames.size() || detachedLoaded.Lightmaps.size() || detachedLoaded.Skins.size()
		|| detachedLoaded.Tiles.size() != 1 || detachedLoaded.Tiles[0].SkinIndexTop != 0 || detachedLoaded.Tiles[0].SkinIndexRight != -1 || detachedLoaded.Tiles[0].SkinIndexFront != 0);

	::llc::SROGND reused;
	LLC_TEST_REQUIRE(errors, RAGNAROK_GND_RESULT_REUSE, ::llc::failed(::llc::roGNDLoad(reused, bytes15)));
	cnst ::llc::sc_t * const nameAllocation = reused.TextureNames[0].begin();
	cnst ::llc::SROGNDLightmap * const lightmapAllocation = reused.Lightmaps.begin();
	cnst ::llc::SROGNDSkin * const skinAllocation = reused.Skins.begin();
	cnst ::llc::SROGNDTile * const tileAllocation = reused.Tiles.begin();
	LLC_TEST_REQUIRE(errors, RAGNAROK_GND_RESULT_REUSE, ::llc::failed(::llc::roGNDLoad(reused, bytes17)));
	LLC_TEST_CHECK(errors, RAGNAROK_GND_RESULT_REUSE, nameAllocation != reused.TextureNames[0].begin() || lightmapAllocation != reused.Lightmaps.begin() || skinAllocation != reused.Skins.begin() || tileAllocation != reused.Tiles.begin());

	::llc::au0_t overflow;
	stxp ::llc::SROVersion version = {1, 7};
	stxp ::llc::n2u2_t dimensions = {(::llc::u2_t)-1, (::llc::u2_t)-1};
	stxp ::llc::f2_t tileScale = 10;
	stxp ::llc::u2_t textureCount = 0;
	stxp ::llc::u2_t textureNameSize = 80;
	stxp ::llc::u2_t lightmapCount = 0;
	stxp ::llc::n2u2_t lightmapDimensions = {};
	stxp ::llc::u2_t lightmapTiles = 0;
	stxp ::llc::u2_t skinCount = 0;
	if_fail_fe(overflow.append(GND_MAGIC));
	if_fail_fe(::llc::savePOD(overflow, version));
	if_fail_fe(::llc::savePOD(overflow, dimensions));
	if_fail_fe(::llc::savePOD(overflow, tileScale));
	if_fail_fe(::llc::savePOD(overflow, textureCount));
	if_fail_fe(::llc::savePOD(overflow, textureNameSize));
	if_fail_fe(::llc::savePOD(overflow, lightmapCount));
	if_fail_fe(::llc::savePOD(overflow, lightmapDimensions));
	if_fail_fe(::llc::savePOD(overflow, lightmapTiles));
	if_fail_fe(::llc::savePOD(overflow, skinCount));
	if_fail_fe(gndSentinel(rejected));
	LLC_TEST_CHECK(errors, RAGNAROK_GND_RESULT_OVERFLOW, 0 <= ::llc::roGNDLoad(rejected, overflow) || rejected.Version.Major != 1 || rejected.Version.Minor != 7
		|| rejected.Dimensions != dimensions || rejected.TextureNames.size() || rejected.Lightmaps.size() || rejected.Skins.size()
		|| rejected.Tiles.size() != 1 || rejected.Tiles[0].Height[0] != 5 || rejected.Tiles[0].Flags != 0x4321);
	return 0;
}
