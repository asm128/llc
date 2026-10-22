#include "llc_ascii_target.h"

#include "llc_test_core.h"

#include <type_traits>

GDEFINE_ENUM_TYPE(ASCII_TARGET_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, CELL_CHARACTER_TYPE				,  0, "SASCIICell::Character no longer uses the LLC unsigned 8-bit type.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, CELL_COLOR_TYPE				,  1, "SASCIICell::Color no longer uses the LLC unsigned 16-bit type.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, CELL_SIZE						,  2, "SASCIICell is no longer a packed three-byte value.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, CELL_CHARACTER_DEFAULT			,  3, "SASCIICell::Character has the wrong default value.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, CELL_COLOR_DEFAULT				,  4, "SASCIICell::Color has the wrong default value.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, DEFAULT_VALID					,  5, "A default SASCIITarget is invalid.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, DEFAULT_METRICS_X				,  6, "A default SASCIITarget has a nonzero width.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, DEFAULT_METRICS_Y				,  7, "A default SASCIITarget has a nonzero height.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, DEFAULT_SIZE					,  8, "A default SASCIITarget has cells.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, RESIZE_RESULT					,  9, "SASCIITarget::resize() rejected valid dimensions.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, RESIZE_VALID					, 10, "SASCIITarget::resize() produced mismatched planes.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, RESIZE_CHARACTER_METRICS_X		, 11, "SASCIITarget::resize() set the wrong character-plane width.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, RESIZE_CHARACTER_METRICS_Y		, 12, "SASCIITarget::resize() set the wrong character-plane height.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, RESIZE_COLOR_METRICS_X			, 13, "SASCIITarget::resize() set the wrong color-plane width.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, RESIZE_COLOR_METRICS_Y			, 14, "SASCIITarget::resize() set the wrong color-plane height.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, RESIZE_CHARACTER_COUNT			, 15, "SASCIITarget::resize() allocated the wrong character count.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, RESIZE_COLOR_COUNT				, 16, "SASCIITarget::resize() allocated the wrong color count.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, CUSTOM_CLEAR_RESULT				, 17, "asciiTargetClear() rejected a valid target.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, CUSTOM_CHARACTER_GRID			, 18, "asciiTargetClear() left the wrong character in the grid view.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, CUSTOM_CHARACTER_LINEAR			, 19, "asciiTargetClear() left the wrong character in linear storage.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, CUSTOM_COLOR_GRID				, 20, "asciiTargetClear() left the wrong color in the grid view.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, CUSTOM_COLOR_LINEAR				, 21, "asciiTargetClear() left the wrong color in linear storage.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, DEFAULT_CLEAR_RESULT				, 22, "asciiTargetClear() rejected its default values.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, DEFAULT_CHARACTER_GRID			, 23, "asciiTargetClear() did not write the default space character.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, DEFAULT_COLOR_GRID				, 24, "asciiTargetClear() did not write the default white color.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, COPY_VALID						, 25, "Copying an ASCII target produced mismatched planes.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, COPY_CHARACTER_STORAGE			, 26, "Copying an ASCII target borrowed its character storage.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, COPY_COLOR_STORAGE				, 27, "Copying an ASCII target borrowed its color storage.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, COPY_CHARACTER_VALUE			, 28, "Copying an ASCII target lost a character.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, COPY_COLOR_VALUE				, 29, "Copying an ASCII target lost a color.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, COPY_CHARACTER_INDEPENDENCE		, 30, "Changing an ASCII-target copy changed the original character plane.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, COPY_COLOR_INDEPENDENCE			, 31, "Changing an ASCII-target copy changed the original color plane.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, MISMATCH_DETECTED				, 32, "SASCIITarget::valid() accepted mismatched plane dimensions.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, MISMATCH_CLEAR_RESULT			, 33, "asciiTargetClear() accepted mismatched plane dimensions.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, MISMATCH_CHARACTER_PRESERVED	, 34, "A rejected clear changed the character plane.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, MISMATCH_COLOR_PRESERVED		, 35, "A rejected clear changed the color plane.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, OVERFLOW_RESIZE_RESULT			, 36, "SASCIITarget::resize() accepted an unrepresentable cell count.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, OVERFLOW_VALID					, 37, "A rejected resize invalidated the original target.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, OVERFLOW_METRICS_X_PRESERVED	, 38, "A rejected resize changed the target width.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, OVERFLOW_METRICS_Y_PRESERVED	, 39, "A rejected resize changed the target height.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, OVERFLOW_CHARACTER_POINTER		, 40, "A rejected resize changed the character allocation.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, OVERFLOW_COLOR_POINTER			, 41, "A rejected resize changed the color allocation.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, OVERFLOW_CHARACTER_VALUE		, 42, "A rejected resize changed a character.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, OVERFLOW_COLOR_VALUE			, 43, "A rejected resize changed a color.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, ZERO_RESIZE_RESULT				, 44, "SASCIITarget::resize() rejected a zero-area target.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, ZERO_VALID						, 45, "A zero-area ASCII target is invalid.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, ZERO_CHARACTER_METRICS_X		, 46, "A zero-area resize set the wrong character-plane width.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, ZERO_CHARACTER_METRICS_Y		, 47, "A zero-area resize set the wrong character-plane height.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, ZERO_COLOR_METRICS_X			, 48, "A zero-area resize set the wrong color-plane width.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, ZERO_COLOR_METRICS_Y			, 49, "A zero-area resize set the wrong color-plane height.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, ZERO_CHARACTER_COUNT			, 50, "A zero-area resize left characters in the target.");
GDEFINE_ENUM_VALUED(ASCII_TARGET_TEST_RESULT, ZERO_COLOR_COUNT				, 51, "A zero-area resize left colors in the target.");

sttc ::llc::err_t testASCIITargetDefaults(::llc::ATestError& errors) {
	LLC_TEST_CHECK(errors, ASCII_TARGET_TEST_RESULT_CELL_CHARACTER_TYPE, (false == ::std::is_same_v<decltype(::llc::SASCIICell::Character), ::llc::u0_t>));
	LLC_TEST_CHECK(errors, ASCII_TARGET_TEST_RESULT_CELL_COLOR_TYPE, (false == ::std::is_same_v<decltype(::llc::SASCIICell::Color), ::llc::u1_t>));
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_CELL_SIZE, sizeof(::llc::SASCIICell) != 3, "Size:%u, expected:3.", (::llc::u2_t)sizeof(::llc::SASCIICell));
	cnst ::llc::SASCIICell cell = {};
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_CELL_CHARACTER_DEFAULT, cell.Character != 0, "Character:%u, expected:0.", (::llc::u2_t)cell.Character);
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_CELL_COLOR_DEFAULT, cell.Color != 0, "Color:%u, expected:0.", (::llc::u2_t)cell.Color);
	cnst ::llc::SASCIITarget target = {};
	LLC_TEST_CHECK(errors, ASCII_TARGET_TEST_RESULT_DEFAULT_VALID, false == target.valid());
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_DEFAULT_METRICS_X, target.metrics().x != 0, "Width:%u, expected:0.", target.metrics().x);
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_DEFAULT_METRICS_Y, target.metrics().y != 0, "Height:%u, expected:0.", target.metrics().y);
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_DEFAULT_SIZE, target.size() != 0, "Size:%u, expected:0.", target.size());
	rtrn 0;
}

sttc ::llc::err_t testASCIITargetClear(::llc::ATestError& errors, ::llc::SASCIITarget& target) {
	cnst ::llc::n2u2_t metrics = {64, 33};
	::llc::err_t result = target.resize(metrics);
	LLC_TEST_REQUIREF(errors, ASCII_TARGET_TEST_RESULT_RESIZE_RESULT, ::llc::failed(result), "Result:%i, expected success.", result);
	LLC_TEST_CHECK(errors, ASCII_TARGET_TEST_RESULT_RESIZE_VALID, false == target.valid());
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_RESIZE_CHARACTER_METRICS_X, target.Characters.metrics().x != metrics.x, "Width:%u, expected:%u.", target.Characters.metrics().x, metrics.x);
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_RESIZE_CHARACTER_METRICS_Y, target.Characters.metrics().y != metrics.y, "Height:%u, expected:%u.", target.Characters.metrics().y, metrics.y);
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_RESIZE_COLOR_METRICS_X, target.Colors.metrics().x != metrics.x, "Width:%u, expected:%u.", target.Colors.metrics().x, metrics.x);
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_RESIZE_COLOR_METRICS_Y, target.Colors.metrics().y != metrics.y, "Height:%u, expected:%u.", target.Colors.metrics().y, metrics.y);
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_RESIZE_CHARACTER_COUNT, target.Characters.size() != metrics.x * metrics.y, "Count:%u, expected:%u.", target.Characters.size(), metrics.x * metrics.y);
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_RESIZE_COLOR_COUNT, target.Colors.size() != metrics.x * metrics.y, "Count:%u, expected:%u.", target.Colors.size(), metrics.x * metrics.y);

	cnst ::llc::u0_t character = 0xDB;
	cnst ::llc::u1_t color = ::llc::u1_t(::llc::ASCII_COLOR_BLUE | (::llc::ASCII_COLOR_LIGHTGREY << 4));
	result = ::llc::asciiTargetClear(target, character, color);
	LLC_TEST_REQUIREF(errors, ASCII_TARGET_TEST_RESULT_CUSTOM_CLEAR_RESULT, ::llc::failed(result), "Result:%i, expected success.", result);
	for(::llc::u2_t y = 0; y < metrics.y; ++y)
	for(::llc::u2_t x = 0; x < metrics.x; ++x) {
		cnst ::llc::u2_t index = y * metrics.x + x;
		LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_CUSTOM_CHARACTER_GRID, target.Characters[y][x] != character, "Cell:{%u, %u}, character:%u, expected:%u.", x, y, (::llc::u2_t)target.Characters[y][x], (::llc::u2_t)character);
		LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_CUSTOM_CHARACTER_LINEAR, target.Characters.begin()[index] != character, "Index:%u, character:%u, expected:%u.", index, (::llc::u2_t)target.Characters.begin()[index], (::llc::u2_t)character);
		LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_CUSTOM_COLOR_GRID, target.Colors[y][x] != color, "Cell:{%u, %u}, color:%u, expected:%u.", x, y, (::llc::u2_t)target.Colors[y][x], (::llc::u2_t)color);
		LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_CUSTOM_COLOR_LINEAR, target.Colors.begin()[index] != color, "Index:%u, color:%u, expected:%u.", index, (::llc::u2_t)target.Colors.begin()[index], (::llc::u2_t)color);
	}

	result = ::llc::asciiTargetClear(target);
	LLC_TEST_REQUIREF(errors, ASCII_TARGET_TEST_RESULT_DEFAULT_CLEAR_RESULT, ::llc::failed(result), "Result:%i, expected success.", result);
	for(::llc::u2_t y = 0; y < metrics.y; ++y)
	for(::llc::u2_t x = 0; x < metrics.x; ++x) {
		LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_DEFAULT_CHARACTER_GRID, target.Characters[y][x] != ' ', "Cell:{%u, %u}, character:%u, expected:%u.", x, y, (::llc::u2_t)target.Characters[y][x], (::llc::u2_t)' ');
		LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_DEFAULT_COLOR_GRID, target.Colors[y][x] != ::llc::ASCII_COLOR_WHITE, "Cell:{%u, %u}, color:%u, expected:%u.", x, y, (::llc::u2_t)target.Colors[y][x], (::llc::u2_t)::llc::ASCII_COLOR_WHITE);
	}
	rtrn 0;
}

sttc ::llc::err_t testASCIITargetCopy(::llc::ATestError& errors, ::llc::SASCIITarget& target) {
	for(::llc::u2_t iCell = 0; iCell < target.size(); ++iCell) {
		target.Characters.begin()[iCell] = ::llc::u0_t(iCell);
		target.Colors.begin()[iCell] = ::llc::u1_t(iCell * 17U);
	}
	::llc::SASCIITarget copy = target;
	LLC_TEST_CHECK(errors, ASCII_TARGET_TEST_RESULT_COPY_VALID, false == copy.valid());
	LLC_TEST_CHECK(errors, ASCII_TARGET_TEST_RESULT_COPY_CHARACTER_STORAGE, copy.Characters.begin() == target.Characters.begin());
	LLC_TEST_CHECK(errors, ASCII_TARGET_TEST_RESULT_COPY_COLOR_STORAGE, copy.Colors.begin() == target.Colors.begin());
	for(::llc::u2_t iCell = 0; iCell < target.size(); ++iCell) {
		LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_COPY_CHARACTER_VALUE, copy.Characters.begin()[iCell] != ::llc::u0_t(iCell), "Index:%u, character:%u, expected:%u.", iCell, (::llc::u2_t)copy.Characters.begin()[iCell], (::llc::u2_t)::llc::u0_t(iCell));
		LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_COPY_COLOR_VALUE, copy.Colors.begin()[iCell] != ::llc::u1_t(iCell * 17U), "Index:%u, color:%u, expected:%u.", iCell, (::llc::u2_t)copy.Colors.begin()[iCell], (::llc::u2_t)::llc::u1_t(iCell * 17U));
	}
	cnst ::llc::u0_t originalCharacter = target.Characters[0][0];
	cnst ::llc::u1_t originalColor = target.Colors[0][0];
	copy.Characters[0][0] = ::llc::u0_t(originalCharacter + 1U);
	copy.Colors[0][0] = ::llc::u1_t(originalColor + 1U);
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_COPY_CHARACTER_INDEPENDENCE, target.Characters[0][0] != originalCharacter, "Original:%u, expected:%u.", (::llc::u2_t)target.Characters[0][0], (::llc::u2_t)originalCharacter);
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_COPY_COLOR_INDEPENDENCE, target.Colors[0][0] != originalColor, "Original:%u, expected:%u.", (::llc::u2_t)target.Colors[0][0], (::llc::u2_t)originalColor);
	rtrn 0;
}

sttc ::llc::err_t testASCIITargetMismatch(::llc::ATestError& errors) {
	::llc::SASCIITarget target = {};
	cnst ::llc::n2u2_t characterMetrics = {6, 5};
	cnst ::llc::n2u2_t colorMetrics = {5, 5};
	if_fail_fe(target.Characters.resize(characterMetrics, ::llc::u0_t('A')));
	if_fail_fe(target.Colors.resize(colorMetrics, ::llc::u1_t(0x1234)));
	LLC_TEST_CHECK(errors, ASCII_TARGET_TEST_RESULT_MISMATCH_DETECTED, target.valid());
	cnst ::llc::err_t result = ::llc::asciiTargetClear(target, 'Z', 0x4321);
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_MISMATCH_CLEAR_RESULT, false == ::llc::failed(result), "Result:%i, expected failure.", result);
	for(::llc::u2_t iCell = 0; iCell < target.Characters.size(); ++iCell)
		LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_MISMATCH_CHARACTER_PRESERVED, target.Characters.begin()[iCell] != 'A', "Index:%u, character:%u, expected:%u.", iCell, (::llc::u2_t)target.Characters.begin()[iCell], (::llc::u2_t)'A');
	for(::llc::u2_t iCell = 0; iCell < target.Colors.size(); ++iCell)
		LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_MISMATCH_COLOR_PRESERVED, target.Colors.begin()[iCell] != 0x1234, "Index:%u, color:%u, expected:%u.", iCell, (::llc::u2_t)target.Colors.begin()[iCell], 0x1234U);
	rtrn 0;
}

sttc ::llc::err_t testASCIITargetResizeGuards(::llc::ATestError& errors, ::llc::SASCIITarget& target) {
	cnst ::llc::n2u2_t oldMetrics = target.metrics();
	cnst ::llc::u0_t* oldCharacters = target.Characters.begin();
	cnst ::llc::u1_t* oldColors = target.Colors.begin();
	cnst ::llc::n2u2_t overflowMetrics = {0xFFFFFFFFU, 2};
	::llc::err_t result = target.resize(overflowMetrics);
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_OVERFLOW_RESIZE_RESULT, false == ::llc::failed(result), "Result:%i, expected failure.", result);
	LLC_TEST_CHECK(errors, ASCII_TARGET_TEST_RESULT_OVERFLOW_VALID, false == target.valid());
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_OVERFLOW_METRICS_X_PRESERVED, target.metrics().x != oldMetrics.x, "Width:%u, expected:%u.", target.metrics().x, oldMetrics.x);
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_OVERFLOW_METRICS_Y_PRESERVED, target.metrics().y != oldMetrics.y, "Height:%u, expected:%u.", target.metrics().y, oldMetrics.y);
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_OVERFLOW_CHARACTER_POINTER, target.Characters.begin() != oldCharacters, "Pointer:%p, expected:%p.", target.Characters.begin(), oldCharacters);
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_OVERFLOW_COLOR_POINTER, target.Colors.begin() != oldColors, "Pointer:%p, expected:%p.", target.Colors.begin(), oldColors);
	for(::llc::u2_t iCell = 0; iCell < target.size(); ++iCell) {
		LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_OVERFLOW_CHARACTER_VALUE, target.Characters.begin()[iCell] != ::llc::u0_t(iCell), "Index:%u, character:%u, expected:%u.", iCell, (::llc::u2_t)target.Characters.begin()[iCell], (::llc::u2_t)::llc::u0_t(iCell));
		LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_OVERFLOW_COLOR_VALUE, target.Colors.begin()[iCell] != ::llc::u1_t(iCell * 17U), "Index:%u, color:%u, expected:%u.", iCell, (::llc::u2_t)target.Colors.begin()[iCell], (::llc::u2_t)::llc::u1_t(iCell * 17U));
	}

	cnst ::llc::n2u2_t zeroMetrics = {0, 7};
	result = target.resize(zeroMetrics);
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_ZERO_RESIZE_RESULT, ::llc::failed(result), "Result:%i, expected success.", result);
	LLC_TEST_CHECK(errors, ASCII_TARGET_TEST_RESULT_ZERO_VALID, false == target.valid());
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_ZERO_CHARACTER_METRICS_X, target.Characters.metrics().x != zeroMetrics.x, "Width:%u, expected:%u.", target.Characters.metrics().x, zeroMetrics.x);
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_ZERO_CHARACTER_METRICS_Y, target.Characters.metrics().y != zeroMetrics.y, "Height:%u, expected:%u.", target.Characters.metrics().y, zeroMetrics.y);
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_ZERO_COLOR_METRICS_X, target.Colors.metrics().x != zeroMetrics.x, "Width:%u, expected:%u.", target.Colors.metrics().x, zeroMetrics.x);
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_ZERO_COLOR_METRICS_Y, target.Colors.metrics().y != zeroMetrics.y, "Height:%u, expected:%u.", target.Colors.metrics().y, zeroMetrics.y);
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_ZERO_CHARACTER_COUNT, target.Characters.size() != 0, "Count:%u, expected:0.", target.Characters.size());
	LLC_TEST_CHECKF(errors, ASCII_TARGET_TEST_RESULT_ZERO_COLOR_COUNT, target.Colors.size() != 0, "Count:%u, expected:0.", target.Colors.size());
	rtrn 0;
}

::llc::err_t testASCIITarget(::llc::ATestError& errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	if_fail_fe(::testASCIITargetDefaults(errors));
	::llc::SASCIITarget target = {};
	if_fail_fe(::testASCIITargetClear(errors, target));
	if_fail_fe(::testASCIITargetCopy(errors, target));
	if_fail_fe(::testASCIITargetMismatch(errors));
	rtrn ::testASCIITargetResizeGuards(errors, target);
}
