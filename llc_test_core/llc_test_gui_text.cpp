#include "llc_gui_text.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(GUI_TEXT_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(GUI_TEXT_TEST_RESULT, WIDTH_RESULT				,  0, "Fixed-cell text width was calculated incorrectly.");
GDEFINE_ENUM_VALUED(GUI_TEXT_TEST_RESULT, FONT_CREATE_RESULT		,  1, "The GUI text test could not create a complete byte-font atlas.");
GDEFINE_ENUM_VALUED(GUI_TEXT_TEST_RESULT, COLOR_DRAW_RESULT		,  2, "Colored fixed-cell text drawing reported failure for valid inputs.");
GDEFINE_ENUM_VALUED(GUI_TEXT_TEST_RESULT, COLOR_GLYPH_CHANNEL		,  3, "Colored fixed-cell text drawing produced the wrong glyph channel.");
GDEFINE_ENUM_VALUED(GUI_TEXT_TEST_RESULT, COLOR_TRANSPARENT_CHANNEL,  4, "Colored fixed-cell text drawing changed a transparent glyph channel.");
GDEFINE_ENUM_VALUED(GUI_TEXT_TEST_RESULT, COLOR_OUTSIDE_CHANNEL	,  5, "Colored fixed-cell text drawing changed a channel outside the text.");
GDEFINE_ENUM_VALUED(GUI_TEXT_TEST_RESULT, COLOR_ALIGNED_CHANNEL	,  6, "Aligned colored text drawing placed a glyph channel incorrectly.");
GDEFINE_ENUM_VALUED(GUI_TEXT_TEST_RESULT, LIT_DRAW_RESULT			,  7, "Lit fixed-cell text drawing reported failure for valid inputs.");
GDEFINE_ENUM_VALUED(GUI_TEXT_TEST_RESULT, LIT_GLYPH_CHANNEL		,  8, "Lit fixed-cell text drawing produced the wrong glyph channel.");
GDEFINE_ENUM_VALUED(GUI_TEXT_TEST_RESULT, LIT_OUTSIDE_CHANNEL		,  9, "Lit fixed-cell text drawing changed a channel outside the light radius.");
GDEFINE_ENUM_VALUED(GUI_TEXT_TEST_RESULT, LIT_ALIGNED_CHANNEL		, 10, "Aligned lit text drawing placed a glyph channel incorrectly.");
GDEFINE_ENUM_VALUED(GUI_TEXT_TEST_RESULT, RASTER_RESULT				, 11, "Monochrome text rasterization reported the wrong result.");
GDEFINE_ENUM_VALUED(GUI_TEXT_TEST_RESULT, RASTER_SIZE				, 12, "Monochrome text rasterization produced the wrong coordinate count.");
GDEFINE_ENUM_VALUED(GUI_TEXT_TEST_RESULT, RASTER_APPEND				, 13, "Monochrome text rasterization discarded existing output.");
GDEFINE_ENUM_VALUED(GUI_TEXT_TEST_RESULT, RASTER_COORDINATE_X		, 14, "Monochrome text rasterization produced the wrong x coordinate.");
GDEFINE_ENUM_VALUED(GUI_TEXT_TEST_RESULT, RASTER_COORDINATE_Y		, 15, "Monochrome text rasterization produced the wrong y coordinate.");
GDEFINE_ENUM_VALUED(GUI_TEXT_TEST_RESULT, RASTER_CLIPPED_SIZE		, 16, "Clipped text rasterization crossed a glyph boundary.");
GDEFINE_ENUM_VALUED(GUI_TEXT_TEST_RESULT, RASTER_CLIPPED_X			, 17, "Clipped text rasterization produced the wrong x coordinate.");
GDEFINE_ENUM_VALUED(GUI_TEXT_TEST_RESULT, RASTER_CLIPPED_Y			, 18, "Clipped text rasterization produced the wrong y coordinate.");
GDEFINE_ENUM_VALUED(GUI_TEXT_TEST_RESULT, INVALID_RESULT			, 19, "Text rasterization accepted invalid font or target metrics.");
GDEFINE_ENUM_VALUED(GUI_TEXT_TEST_RESULT, INVALID_PRESERVE			, 20, "Rejected text rasterization changed existing output.");
GDEFINE_ENUM_VALUED(GUI_TEXT_TEST_RESULT, EMPTY_RESULT				, 21, "Empty text did not remain a resource-independent no-op.");
GDEFINE_ENUM_VALUED(GUI_TEXT_TEST_RESULT, EMPTY_PRESERVE			, 22, "Empty text changed existing output.");

sttc ::llc::err_t testGUITextColor(ATestError & errors, GUI_TEXT_TEST_RESULT result, ::llc::vcst_t name, cnst ::llc::bgra & actual, cnst ::llc::bgra & expected) {
	LLC_TEST_CHECKF(errors, result, actual.b != expected.b, "%.*s.b:%u, expected:%u.", LLC_VIEW_ARGS(name), actual.b, expected.b);
	LLC_TEST_CHECKF(errors, result, actual.g != expected.g, "%.*s.g:%u, expected:%u.", LLC_VIEW_ARGS(name), actual.g, expected.g);
	LLC_TEST_CHECKF(errors, result, actual.r != expected.r, "%.*s.r:%u, expected:%u.", LLC_VIEW_ARGS(name), actual.r, expected.r);
	LLC_TEST_CHECKF(errors, result, actual.a != expected.a, "%.*s.a:%u, expected:%u.", LLC_VIEW_ARGS(name), actual.a, expected.a);
	rtrn 0;
}

sttc ::llc::err_t testGUITextMonoFontCreate(::llc::imgmonou3_t & font, ::llc::n2u0_c & charSize, ::llc::u2_t characterCellsX) {
	cnst ::llc::u2_t characterRows = (256U + characterCellsX - 1U) / characterCellsX;
	if_fail_fe(font.resize(characterCellsX * charSize.x, characterRows * charSize.y));
	rtrn font.Texels.fill(0);
}

sttc ::llc::err_t testGUITextWidth(ATestError & errors) {
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_WIDTH_RESULT, ::llc::textLineCalcWidth({0, 7}, LLC_CXS("width")) != 0, "Zero-width result:%i, expected:0.", ::llc::textLineCalcWidth({0, 7}, LLC_CXS("width")));
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_WIDTH_RESULT, ::llc::textLineCalcWidth({3, 7}, LLC_CXS("")) != 0, "Empty-text result:%i, expected:0.", ::llc::textLineCalcWidth({3, 7}, LLC_CXS("")));
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_WIDTH_RESULT, ::llc::textLineCalcWidth({3, 7}, LLC_CXS("width")) != 15, "Text width:%i, expected:15.", ::llc::textLineCalcWidth({3, 7}, LLC_CXS("width")));
	rtrn 0;
}

sttc ::llc::err_t testGUITextColored(ATestError & errors) {
	cnst ::llc::bgra transparent = {0xFF, 0x00, 0xFF, 0xFF}, background = {1, 2, 3, 4}, firstColor = {11, 12, 13, 14}, lastColor = {21, 22, 23, 24};
	::llc::bgra atlasCells[16][16] = {};
	::llc::grid<::llc::bgra> atlas = atlasCells;
	atlas.fill(transparent);
	atlas[0][0] = firstColor;
	atlas[15][15] = lastColor;
	::llc::bgra targetCells[4][8] = {};
	::llc::grid<::llc::bgra> target = targetCells;
	target.fill(background);
	cnst ::llc::u0_t textStorage[] = {0, 127, 255};
	cnst ::llc::vcst_t text = {(cnst char*)textStorage, ::llc::size(textStorage)};
	cnst ::llc::err_t drawResult = ::llc::textLineDrawFixedSize(target, atlas, 16, 1, {1, 1}, text, {1, 0});
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_COLOR_DRAW_RESULT, ::llc::failed(drawResult), "Draw result:%i.", drawResult);
	if_fail_fe(::testGUITextColor(errors, GUI_TEXT_TEST_RESULT_COLOR_GLYPH_CHANNEL, LLC_CXS("first glyph"), target[1][1], firstColor));
	if_fail_fe(::testGUITextColor(errors, GUI_TEXT_TEST_RESULT_COLOR_TRANSPARENT_CHANNEL, LLC_CXS("transparent glyph"), target[1][2], background));
	if_fail_fe(::testGUITextColor(errors, GUI_TEXT_TEST_RESULT_COLOR_GLYPH_CHANNEL, LLC_CXS("unsigned glyph"), target[1][3], lastColor));
	if_fail_fe(::testGUITextColor(errors, GUI_TEXT_TEST_RESULT_COLOR_OUTSIDE_CHANNEL, LLC_CXS("outside"), target[0][0], background));

	::llc::bgra alignedAtlasCells[8][32] = {};
	::llc::grid<::llc::bgra> alignedAtlas = alignedAtlasCells;
	alignedAtlas.fill(transparent);
	alignedAtlas[0][1] = firstColor;
	alignedAtlas[0][2] = lastColor;
	target.fill(background);
	cnst char alignedTextStorage[] = {1, 2};
	cnst ::llc::vcst_t alignedText = {alignedTextStorage, ::llc::size(alignedTextStorage)};
	cnst ::llc::err_t alignedResult = ::llc::textLineDrawAlignedFixedSize(target, alignedAtlas, 2, target.metrics().u1(), {1, 1}, alignedText);
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_COLOR_DRAW_RESULT, ::llc::failed(alignedResult), "Aligned draw result:%i.", alignedResult);
	if_fail_fe(::testGUITextColor(errors, GUI_TEXT_TEST_RESULT_COLOR_ALIGNED_CHANNEL, LLC_CXS("aligned first"), target[2][3], firstColor));
	if_fail_fe(::testGUITextColor(errors, GUI_TEXT_TEST_RESULT_COLOR_ALIGNED_CHANNEL, LLC_CXS("aligned second"), target[2][4], lastColor));
	rtrn 0;
}

sttc ::llc::err_t testGUITextLit(ATestError & errors) {
	::llc::imgmonou3_t font = {};
	LLC_TEST_REQUIREF(errors, GUI_TEXT_TEST_RESULT_FONT_CREATE_RESULT, 0 > ::testGUITextMonoFontCreate(font, {1, 1}, 32), "%s", "Lit-font allocation failed.");
	font.View[1] = true;
	cnst ::llc::bgra background = {1, 2, 3, 4}, color = {11, 12, 13, 14};
	::llc::bgra targetCells[5][8] = {};
	::llc::grid<::llc::bgra> target = targetCells;
	target.fill(background);
	cnst char textStorage[] = {1};
	cnst ::llc::vcst_t text = {textStorage, ::llc::size(textStorage)};
	cnst ::llc::err_t drawResult = ::llc::textLineDrawFixedSizeLit(target, font.View, font.metrics(), 32, 1, {1, 1}, text, {2, 0}, color);
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_LIT_DRAW_RESULT, ::llc::failed(drawResult), "Draw result:%i.", drawResult);
	if_fail_fe(::testGUITextColor(errors, GUI_TEXT_TEST_RESULT_LIT_GLYPH_CHANNEL, LLC_CXS("lit glyph"), target[1][2], color));
	if_fail_fe(::testGUITextColor(errors, GUI_TEXT_TEST_RESULT_LIT_OUTSIDE_CHANNEL, LLC_CXS("lit outside"), target[4][7], background));
	target.fill(background);
	cnst ::llc::err_t alignedResult = ::llc::textLineDrawAlignedFixedSizeLit(target, font.View, font.metrics(), 2, target.metrics().u1(), {1, 1}, text, color);
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_LIT_DRAW_RESULT, ::llc::failed(alignedResult), "Aligned draw result:%i.", alignedResult);
	if_fail_fe(::testGUITextColor(errors, GUI_TEXT_TEST_RESULT_LIT_ALIGNED_CHANNEL, LLC_CXS("aligned lit glyph"), target[2][4], color));
	rtrn 0;
}

sttc ::llc::err_t testGUITextRasterCharacters(ATestError & errors) {
	cnst ::llc::n2u0_t charSize = {3, 2};
	cnst ::llc::u2_t characterCellsX = 16;
	::llc::imgmonou3_t font = {};
	LLC_TEST_REQUIREF(errors, GUI_TEXT_TEST_RESULT_FONT_CREATE_RESULT, 0 > ::testGUITextMonoFontCreate(font, charSize, characterCellsX), "%s", "Raster-font allocation failed.");
	for(::llc::u2_t character = 0; character < 256; ++character) {
		cnst ::llc::n2u2_t cell = {(character % characterCellsX) * charSize.x, (character / characterCellsX) * charSize.y};
		cnst ::llc::n2u2_t local = {character % charSize.x, character / charSize.x % charSize.y};
		font.View[(cell.y + local.y) * font.Pitch + cell.x + local.x] = true;
	}
	cnst ::llc::n2u1_t sentinel = {99, 98};
	for(::llc::u2_t character = 0; character < 256; ++character) {
		cnst ::llc::u0_t characterStorage = (::llc::u0_t)character;
		cnst ::llc::vcst_t text = {(cnst char*)&characterStorage, 1};
		::llc::apod<::llc::n2u1_t> coordinates = {sentinel};
		cnst ::llc::err_t rasterResult = ::llc::textLineRaster({8, 8}, charSize, {{2, 3}, charSize.s1()}, font, text, coordinates);
		cnst ::llc::n2u1_t expected = {::llc::u1_t(2 + character % charSize.x), ::llc::u1_t(3 + character / charSize.x % charSize.y)};
		LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_RASTER_RESULT, ::llc::failed(rasterResult), "Character:%u, result:%i.", character, rasterResult);
		LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_RASTER_SIZE, coordinates.size() != 2, "Character:%u, coordinate count:%u, expected:2.", character, coordinates.size());
		LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_RASTER_APPEND, coordinates[0].x != sentinel.x, "Character:%u, sentinel.x:%u, expected:%u.", character, coordinates[0].x, sentinel.x);
		LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_RASTER_APPEND, coordinates[0].y != sentinel.y, "Character:%u, sentinel.y:%u, expected:%u.", character, coordinates[0].y, sentinel.y);
		LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_RASTER_COORDINATE_X, coordinates.size() > 1 && coordinates[1].x != expected.x, "Character:%u, x:%u, expected:%u.", character, coordinates.size() > 1 ? coordinates[1].x : 0U, expected.x);
		LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_RASTER_COORDINATE_Y, coordinates.size() > 1 && coordinates[1].y != expected.y, "Character:%u, y:%u, expected:%u.", character, coordinates.size() > 1 ? coordinates[1].y : 0U, expected.y);
	}
	rtrn 0;
}

sttc ::llc::err_t testGUITextRasterSequence(ATestError & errors) {
	cnst ::llc::n2u0_t charSize = {3, 2};
	cnst ::llc::u2_t characterCellsX = 16;
	::llc::imgmonou3_t font = {};
	LLC_TEST_REQUIREF(errors, GUI_TEXT_TEST_RESULT_FONT_CREATE_RESULT, 0 > ::testGUITextMonoFontCreate(font, charSize, characterCellsX), "%s", "Sequence-font allocation failed.");
	cnst ::llc::u0_t textStorage[] = {0, 127, 128, 255};
	for(::llc::u2_t iCharacter = 0; iCharacter < ::llc::size(textStorage); ++iCharacter) {
		cnst ::llc::u2_t character = (::llc::u0_t)textStorage[iCharacter];
		cnst ::llc::n2u2_t cell = {(character % characterCellsX) * charSize.x, (character / characterCellsX) * charSize.y};
		cnst ::llc::n2u2_t local = {character % charSize.x, character / charSize.x % charSize.y};
		font.View[(cell.y + local.y) * font.Pitch + cell.x + local.x] = true;
	}
	cnst ::llc::n2u1_t sentinel = {99, 98};
	::llc::apod<::llc::n2u1_t> coordinates = {sentinel};
	cnst ::llc::vcst_t text = {(cnst char*)textStorage, ::llc::size(textStorage)};
	cnst ::llc::err_t rasterResult = ::llc::textLineRaster({20, 8}, charSize, {{1, 1}, {12, 2}}, font, text, coordinates);
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_RASTER_RESULT, ::llc::failed(rasterResult), "Sequence result:%i.", rasterResult);
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_RASTER_SIZE, coordinates.size() != 5, "Sequence coordinate count:%u, expected:5.", coordinates.size());
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_RASTER_APPEND, coordinates[0].x != sentinel.x, "Sequence sentinel.x:%u, expected:%u.", coordinates[0].x, sentinel.x);
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_RASTER_APPEND, coordinates[0].y != sentinel.y, "Sequence sentinel.y:%u, expected:%u.", coordinates[0].y, sentinel.y);
	for(::llc::u2_t iCharacter = 0; iCharacter < ::llc::size(textStorage); ++iCharacter) {
		cnst ::llc::u2_t character = (::llc::u0_t)textStorage[iCharacter];
		cnst ::llc::n2u1_t expected = {::llc::u1_t(1 + iCharacter * charSize.x + character % charSize.x), ::llc::u1_t(1 + character / charSize.x % charSize.y)};
		LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_RASTER_COORDINATE_X, coordinates.size() > iCharacter + 1 && coordinates[iCharacter + 1].x != expected.x, "Sequence index:%u, character:%u, x:%u, expected:%u.", iCharacter, character, coordinates.size() > iCharacter + 1 ? coordinates[iCharacter + 1].x : 0U, expected.x);
		LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_RASTER_COORDINATE_Y, coordinates.size() > iCharacter + 1 && coordinates[iCharacter + 1].y != expected.y, "Sequence index:%u, character:%u, y:%u, expected:%u.", iCharacter, character, coordinates.size() > iCharacter + 1 ? coordinates[iCharacter + 1].y : 0U, expected.y);
	}
	rtrn 0;
}

sttc ::llc::err_t testGUITextRasterClipping(ATestError & errors) {
	cnst ::llc::n2u0_t charSize = {3, 2};
	::llc::imgmonou3_t font = {};
	LLC_TEST_REQUIREF(errors, GUI_TEXT_TEST_RESULT_FONT_CREATE_RESULT, 0 > ::testGUITextMonoFontCreate(font, charSize, 16), "%s", "Clipping-font allocation failed.");
	cnst ::llc::u2_t character = 5;
	cnst ::llc::n2u2_t cell = {(character % 16) * charSize.x, (character / 16) * charSize.y};
	for(::llc::u2_t y = 0; y < charSize.y; ++y) {
		for(::llc::u2_t x = 0; x < charSize.x; ++x)
			font.View[(cell.y + y) * font.Pitch + cell.x + x] = true;
	}
	cnst char textStorage[] = {(char)character, (char)character};
	cnst ::llc::vcst_t text = {textStorage, ::llc::size(textStorage)};
	::llc::apod<::llc::n2u1_t> coordinates = {};
	cnst ::llc::err_t rasterResult = ::llc::textLineRaster({4, 3}, charSize, {{-4, -1}, {6, 2}}, font, text, coordinates);
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_RASTER_RESULT, ::llc::failed(rasterResult), "Clipped result:%i.", rasterResult);
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_RASTER_CLIPPED_SIZE, coordinates.size() != 2, "Clipped coordinate count:%u, expected:2.", coordinates.size());
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_RASTER_CLIPPED_X, coordinates.size() > 0 && coordinates[0].x != 0, "Clipped coordinate[0].x:%u, expected:0.", coordinates.size() ? coordinates[0].x : 0U);
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_RASTER_CLIPPED_Y, coordinates.size() > 0 && coordinates[0].y != 0, "Clipped coordinate[0].y:%u, expected:0.", coordinates.size() ? coordinates[0].y : 0U);
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_RASTER_CLIPPED_X, coordinates.size() > 1 && coordinates[1].x != 1, "Clipped coordinate[1].x:%u, expected:1.", coordinates.size() > 1 ? coordinates[1].x : 0U);
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_RASTER_CLIPPED_Y, coordinates.size() > 1 && coordinates[1].y != 0, "Clipped coordinate[1].y:%u, expected:0.", coordinates.size() > 1 ? coordinates[1].y : 0U);
	rtrn 0;
}

sttc ::llc::err_t testGUITextInvalid(ATestError & errors) {
	::llc::imgmonou3_t font = {};
	LLC_TEST_REQUIREF(errors, GUI_TEXT_TEST_RESULT_FONT_CREATE_RESULT, 0 > ::testGUITextMonoFontCreate(font, {1, 1}, 16), "%s", "Validation-font allocation failed.");
	font.View[0] = true;
	cnst char textStorage[] = {0};
	cnst ::llc::vcst_t text = {textStorage, ::llc::size(textStorage)};
	cnst ::llc::n2u1_t sentinel = {99, 98};
	::llc::apod<::llc::n2u1_t> coordinates = {sentinel};
	::llc::setupLogCallbacks(0, 0);
	::llc::err_t rasterResult = ::llc::textLineRaster({8, 8}, {0, 1}, {{}, {1, 1}}, font, text, coordinates);
	::llc::setupDefaultLogCallbacks();
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_INVALID_RESULT, false == ::llc::failed(rasterResult), "Zero-width result:%i, expected failure.", rasterResult);
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_INVALID_PRESERVE, coordinates.size() != 1, "Zero-width output size:%u, expected:1.", coordinates.size());
	::llc::setupLogCallbacks(0, 0);
	rasterResult = ::llc::textLineRaster({8, 8}, {1, 0}, {{}, {1, 1}}, font, text, coordinates);
	::llc::setupDefaultLogCallbacks();
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_INVALID_RESULT, false == ::llc::failed(rasterResult), "Zero-height result:%i, expected failure.", rasterResult);
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_INVALID_PRESERVE, coordinates.size() != 1, "Zero-height output size:%u, expected:1.", coordinates.size());
	::llc::setupLogCallbacks(0, 0);
	rasterResult = ::llc::textLineRaster({0x10001U, 1}, {1, 1}, {{}, {1, 1}}, font, text, coordinates);
	::llc::setupDefaultLogCallbacks();
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_INVALID_RESULT, false == ::llc::failed(rasterResult), "Oversized-target result:%i, expected failure.", rasterResult);
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_INVALID_PRESERVE, coordinates.size() != 1, "Oversized-target output size:%u, expected:1.", coordinates.size());
	::llc::imgmonou3_t shortFont = {};
	LLC_TEST_REQUIREF(errors, GUI_TEXT_TEST_RESULT_FONT_CREATE_RESULT, 0 > shortFont.resize(16, 1), "%s", "Short-font allocation failed.");
	::llc::setupLogCallbacks(0, 0);
	rasterResult = ::llc::textLineRaster({8, 8}, {1, 1}, {{}, {1, 1}}, shortFont, text, coordinates);
	::llc::setupDefaultLogCallbacks();
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_INVALID_RESULT, false == ::llc::failed(rasterResult), "Short-font result:%i, expected failure.", rasterResult);
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_INVALID_PRESERVE, coordinates.size() != 1, "Short-font output size:%u, expected:1.", coordinates.size());
	::llc::bgra atlasCells[16][16] = {}, targetCells[2][2] = {};
	::llc::grid<::llc::bgra> atlas = atlasCells, target = targetCells;
	::llc::setupLogCallbacks(0, 0);
	cnst ::llc::err_t colorResult = ::llc::textLineDrawFixedSize(target, atlas, 0, 0, {1, 1}, text, {});
	::llc::setupDefaultLogCallbacks();
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_INVALID_RESULT, false == ::llc::failed(colorResult), "Zero-column colored result:%i, expected failure.", colorResult);
	for(::llc::u2_t y = 0; y < target.metrics().y; ++y) {
		for(::llc::u2_t x = 0; x < target.metrics().x; ++x) {
			LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_INVALID_PRESERVE, target[y][x].b != 0, "Rejected colored target{%u,%u}.b:%u, expected:0.", x, y, target[y][x].b);
			LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_INVALID_PRESERVE, target[y][x].g != 0, "Rejected colored target{%u,%u}.g:%u, expected:0.", x, y, target[y][x].g);
			LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_INVALID_PRESERVE, target[y][x].r != 0, "Rejected colored target{%u,%u}.r:%u, expected:0.", x, y, target[y][x].r);
			LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_INVALID_PRESERVE, target[y][x].a != 0, "Rejected colored target{%u,%u}.a:%u, expected:0.", x, y, target[y][x].a);
		}
	}
	cnst ::llc::vcst_t emptyText = {};
	cnst ::llc::imgmonou3_t emptyFont = {};
	rasterResult = ::llc::textLineRaster({}, {}, {}, emptyFont, emptyText, coordinates);
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_EMPTY_RESULT, ::llc::failed(rasterResult), "Empty-text result:%i.", rasterResult);
	LLC_TEST_CHECKF(errors, GUI_TEXT_TEST_RESULT_EMPTY_PRESERVE, coordinates.size() != 1, "Empty-text output size:%u, expected:1.", coordinates.size());
	rtrn 0;
}

::llc::err_t testGUIText(ATestError & errors) {
	if_fail_fe(::testGUITextWidth(errors));
	if_fail_fe(::testGUITextColored(errors));
	if_fail_fe(::testGUITextLit(errors));
	if_fail_fe(::testGUITextRasterCharacters(errors));
	if_fail_fe(::testGUITextRasterSequence(errors));
	if_fail_fe(::testGUITextRasterClipping(errors));
	if_fail_fe(::testGUITextInvalid(errors));
	rtrn 0;
}
