#include "llc_ascii_color.h"

#include "llc_test_core.h"

#include <type_traits>

GDEFINE_ENUM_TYPE(ASCII_COLOR_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, INDEX_UNDERLYING_TYPE	,  0, "ASCII_COLOR_INDEX no longer uses the LLC unsigned 32-bit type.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, INDEX_SIZE				,  1, "ASCII_COLOR_INDEX no longer occupies 32 bits.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, COLOR_UNDERLYING_TYPE	,  2, "ASCII_COLOR no longer uses the LLC unsigned 16-bit type.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, COLOR_SIZE				,  3, "ASCII_COLOR no longer occupies 16 bits.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, INDEX_COUNT				,  4, "ASCII_COLOR_INDEX_COUNT is not sixteen.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, COLOR_COUNT				,  5, "ASCII_COLOR_COUNT is not sixteen.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, PALETTE_COUNT			,  6, "ASCII_PALETTE does not contain sixteen entries.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, INDEX_0					,  7, "ASCII_COLOR_INDEX_0 is not black.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, INDEX_1					,  8, "ASCII_COLOR_INDEX_1 is not dark blue.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, INDEX_2					,  9, "ASCII_COLOR_INDEX_2 is not dark green.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, INDEX_3					, 10, "ASCII_COLOR_INDEX_3 is not dark cyan.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, INDEX_4					, 11, "ASCII_COLOR_INDEX_4 is not dark red.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, INDEX_5					, 12, "ASCII_COLOR_INDEX_5 is not dark magenta.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, INDEX_6					, 13, "ASCII_COLOR_INDEX_6 is not dark yellow.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, INDEX_7					, 14, "ASCII_COLOR_INDEX_7 is not light grey.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, INDEX_8					, 15, "ASCII_COLOR_INDEX_8 is not dark grey.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, INDEX_9					, 16, "ASCII_COLOR_INDEX_9 is not blue.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, INDEX_10					, 17, "ASCII_COLOR_INDEX_10 is not green.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, INDEX_11					, 18, "ASCII_COLOR_INDEX_11 is not cyan.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, INDEX_12					, 19, "ASCII_COLOR_INDEX_12 is not red.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, INDEX_13					, 20, "ASCII_COLOR_INDEX_13 is not magenta.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, INDEX_14					, 21, "ASCII_COLOR_INDEX_14 is not yellow.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, INDEX_15					, 22, "ASCII_COLOR_INDEX_15 is not white.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, PALETTE_0				, 23, "ASCII_PALETTE[0] is not opaque black.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, PALETTE_1				, 24, "ASCII_PALETTE[1] is not opaque dark blue.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, PALETTE_2				, 25, "ASCII_PALETTE[2] is not opaque dark green.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, PALETTE_3				, 26, "ASCII_PALETTE[3] is not opaque dark cyan.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, PALETTE_4				, 27, "ASCII_PALETTE[4] is not opaque dark red.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, PALETTE_5				, 28, "ASCII_PALETTE[5] is not opaque dark magenta.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, PALETTE_6				, 29, "ASCII_PALETTE[6] is not opaque dark yellow.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, PALETTE_7				, 30, "ASCII_PALETTE[7] is not opaque light grey.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, PALETTE_8				, 31, "ASCII_PALETTE[8] is not opaque dark grey.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, PALETTE_9				, 32, "ASCII_PALETTE[9] is not opaque blue.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, PALETTE_10				, 33, "ASCII_PALETTE[10] is not opaque green.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, PALETTE_11				, 34, "ASCII_PALETTE[11] is not opaque cyan.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, PALETTE_12				, 35, "ASCII_PALETTE[12] is not opaque red.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, PALETTE_13				, 36, "ASCII_PALETTE[13] is not opaque magenta.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, PALETTE_14				, 37, "ASCII_PALETTE[14] is not opaque yellow.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, PALETTE_15				, 38, "ASCII_PALETTE[15] is not opaque white.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, COLOR_BLACK				, 39, "ASCII_COLOR_BLACK is not zero.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, COLOR_DARKBLUE		, 40, "ASCII_COLOR_DARKBLUE is not one.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, COLOR_DARKGREEN		, 41, "ASCII_COLOR_DARKGREEN is not two.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, COLOR_DARKCYAN		, 42, "ASCII_COLOR_DARKCYAN is not three.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, COLOR_DARKRED			, 43, "ASCII_COLOR_DARKRED is not four.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, COLOR_DARKMAGENTA		, 44, "ASCII_COLOR_DARKMAGENTA is not five.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, COLOR_DARKYELLOW		, 45, "ASCII_COLOR_DARKYELLOW is not six.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, COLOR_LIGHTGREY		, 46, "ASCII_COLOR_LIGHTGREY is not seven.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, COLOR_DARKGREY		, 47, "ASCII_COLOR_DARKGREY is not eight.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, COLOR_BLUE				, 48, "ASCII_COLOR_BLUE is not nine.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, COLOR_GREEN			, 49, "ASCII_COLOR_GREEN is not ten.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, COLOR_CYAN				, 50, "ASCII_COLOR_CYAN is not eleven.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, COLOR_RED				, 51, "ASCII_COLOR_RED is not twelve.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, COLOR_MAGENTA			, 52, "ASCII_COLOR_MAGENTA is not thirteen.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, COLOR_YELLOW			, 53, "ASCII_COLOR_YELLOW is not fourteen.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, COLOR_WHITE			, 54, "ASCII_COLOR_WHITE is not fifteen.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, INVERT_FOREGROUND		, 55, "ascii_color_invert() did not move the background into the foreground nibble.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, INVERT_BACKGROUND		, 56, "ascii_color_invert() did not move the foreground into the background nibble.");
GDEFINE_ENUM_VALUED(ASCII_COLOR_TEST_RESULT, INVERT_ROUND_TRIP		, 57, "Two calls to ascii_color_invert() did not restore the attribute.");

sttc ::llc::err_t testASCIIColorIndices(ATestError & errors) {
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_INDEX_0 , ::llc::ASCII_COLOR_INDEX_0  != 0x000000U, "Value:0x%08X, expected:0x00000000.", (::llc::u2_t)::llc::ASCII_COLOR_INDEX_0 );
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_INDEX_1 , ::llc::ASCII_COLOR_INDEX_1  != 0x800000U, "Value:0x%08X, expected:0x00800000.", (::llc::u2_t)::llc::ASCII_COLOR_INDEX_1 );
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_INDEX_2 , ::llc::ASCII_COLOR_INDEX_2  != 0x008000U, "Value:0x%08X, expected:0x00008000.", (::llc::u2_t)::llc::ASCII_COLOR_INDEX_2 );
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_INDEX_3 , ::llc::ASCII_COLOR_INDEX_3  != 0x808000U, "Value:0x%08X, expected:0x00808000.", (::llc::u2_t)::llc::ASCII_COLOR_INDEX_3 );
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_INDEX_4 , ::llc::ASCII_COLOR_INDEX_4  != 0x000080U, "Value:0x%08X, expected:0x00000080.", (::llc::u2_t)::llc::ASCII_COLOR_INDEX_4 );
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_INDEX_5 , ::llc::ASCII_COLOR_INDEX_5  != 0x800080U, "Value:0x%08X, expected:0x00800080.", (::llc::u2_t)::llc::ASCII_COLOR_INDEX_5 );
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_INDEX_6 , ::llc::ASCII_COLOR_INDEX_6  != 0x008080U, "Value:0x%08X, expected:0x00008080.", (::llc::u2_t)::llc::ASCII_COLOR_INDEX_6 );
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_INDEX_7 , ::llc::ASCII_COLOR_INDEX_7  != 0xC0C0C0U, "Value:0x%08X, expected:0x00C0C0C0.", (::llc::u2_t)::llc::ASCII_COLOR_INDEX_7 );
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_INDEX_8 , ::llc::ASCII_COLOR_INDEX_8  != 0x808080U, "Value:0x%08X, expected:0x00808080.", (::llc::u2_t)::llc::ASCII_COLOR_INDEX_8 );
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_INDEX_9 , ::llc::ASCII_COLOR_INDEX_9  != 0xFF0000U, "Value:0x%08X, expected:0x00FF0000.", (::llc::u2_t)::llc::ASCII_COLOR_INDEX_9 );
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_INDEX_10, ::llc::ASCII_COLOR_INDEX_10 != 0x00FF00U, "Value:0x%08X, expected:0x0000FF00.", (::llc::u2_t)::llc::ASCII_COLOR_INDEX_10);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_INDEX_11, ::llc::ASCII_COLOR_INDEX_11 != 0xFFFF00U, "Value:0x%08X, expected:0x00FFFF00.", (::llc::u2_t)::llc::ASCII_COLOR_INDEX_11);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_INDEX_12, ::llc::ASCII_COLOR_INDEX_12 != 0x0000FFU, "Value:0x%08X, expected:0x000000FF.", (::llc::u2_t)::llc::ASCII_COLOR_INDEX_12);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_INDEX_13, ::llc::ASCII_COLOR_INDEX_13 != 0xFF00FFU, "Value:0x%08X, expected:0x00FF00FF.", (::llc::u2_t)::llc::ASCII_COLOR_INDEX_13);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_INDEX_14, ::llc::ASCII_COLOR_INDEX_14 != 0x00FFFFU, "Value:0x%08X, expected:0x0000FFFF.", (::llc::u2_t)::llc::ASCII_COLOR_INDEX_14);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_INDEX_15, ::llc::ASCII_COLOR_INDEX_15 != 0xFFFFFFU, "Value:0x%08X, expected:0x00FFFFFF.", (::llc::u2_t)::llc::ASCII_COLOR_INDEX_15);
	rtrn 0;
}

sttc ::llc::err_t testASCIIPalette(ATestError & errors) {
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_PALETTE_0 , ::llc::ASCII_PALETTE[ 0] != 0xFF000000U, "Value:0x%08X, expected:0xFF000000.", ::llc::ASCII_PALETTE[ 0]);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_PALETTE_1 , ::llc::ASCII_PALETTE[ 1] != 0xFF800000U, "Value:0x%08X, expected:0xFF800000.", ::llc::ASCII_PALETTE[ 1]);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_PALETTE_2 , ::llc::ASCII_PALETTE[ 2] != 0xFF008000U, "Value:0x%08X, expected:0xFF008000.", ::llc::ASCII_PALETTE[ 2]);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_PALETTE_3 , ::llc::ASCII_PALETTE[ 3] != 0xFF808000U, "Value:0x%08X, expected:0xFF808000.", ::llc::ASCII_PALETTE[ 3]);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_PALETTE_4 , ::llc::ASCII_PALETTE[ 4] != 0xFF000080U, "Value:0x%08X, expected:0xFF000080.", ::llc::ASCII_PALETTE[ 4]);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_PALETTE_5 , ::llc::ASCII_PALETTE[ 5] != 0xFF800080U, "Value:0x%08X, expected:0xFF800080.", ::llc::ASCII_PALETTE[ 5]);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_PALETTE_6 , ::llc::ASCII_PALETTE[ 6] != 0xFF008080U, "Value:0x%08X, expected:0xFF008080.", ::llc::ASCII_PALETTE[ 6]);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_PALETTE_7 , ::llc::ASCII_PALETTE[ 7] != 0xFFC0C0C0U, "Value:0x%08X, expected:0xFFC0C0C0.", ::llc::ASCII_PALETTE[ 7]);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_PALETTE_8 , ::llc::ASCII_PALETTE[ 8] != 0xFF808080U, "Value:0x%08X, expected:0xFF808080.", ::llc::ASCII_PALETTE[ 8]);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_PALETTE_9 , ::llc::ASCII_PALETTE[ 9] != 0xFFFF0000U, "Value:0x%08X, expected:0xFFFF0000.", ::llc::ASCII_PALETTE[ 9]);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_PALETTE_10, ::llc::ASCII_PALETTE[10] != 0xFF00FF00U, "Value:0x%08X, expected:0xFF00FF00.", ::llc::ASCII_PALETTE[10]);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_PALETTE_11, ::llc::ASCII_PALETTE[11] != 0xFFFFFF00U, "Value:0x%08X, expected:0xFFFFFF00.", ::llc::ASCII_PALETTE[11]);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_PALETTE_12, ::llc::ASCII_PALETTE[12] != 0xFF0000FFU, "Value:0x%08X, expected:0xFF0000FF.", ::llc::ASCII_PALETTE[12]);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_PALETTE_13, ::llc::ASCII_PALETTE[13] != 0xFFFF00FFU, "Value:0x%08X, expected:0xFFFF00FF.", ::llc::ASCII_PALETTE[13]);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_PALETTE_14, ::llc::ASCII_PALETTE[14] != 0xFF00FFFFU, "Value:0x%08X, expected:0xFF00FFFF.", ::llc::ASCII_PALETTE[14]);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_PALETTE_15, ::llc::ASCII_PALETTE[15] != 0xFFFFFFFFU, "Value:0x%08X, expected:0xFFFFFFFF.", ::llc::ASCII_PALETTE[15]);
	rtrn 0;
}

sttc ::llc::err_t testASCIIColorValues(ATestError & errors) {
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_COLOR_BLACK		, ::llc::ASCII_COLOR_BLACK		!=  0, "Value:%u, expected:0.", (::llc::u2_t)::llc::ASCII_COLOR_BLACK);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_COLOR_DARKBLUE	, ::llc::ASCII_COLOR_DARKBLUE	!=  1, "Value:%u, expected:1.", (::llc::u2_t)::llc::ASCII_COLOR_DARKBLUE);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_COLOR_DARKGREEN	, ::llc::ASCII_COLOR_DARKGREEN	!=  2, "Value:%u, expected:2.", (::llc::u2_t)::llc::ASCII_COLOR_DARKGREEN);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_COLOR_DARKCYAN	, ::llc::ASCII_COLOR_DARKCYAN	!=  3, "Value:%u, expected:3.", (::llc::u2_t)::llc::ASCII_COLOR_DARKCYAN);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_COLOR_DARKRED		, ::llc::ASCII_COLOR_DARKRED		!=  4, "Value:%u, expected:4.", (::llc::u2_t)::llc::ASCII_COLOR_DARKRED);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_COLOR_DARKMAGENTA	, ::llc::ASCII_COLOR_DARKMAGENTA	!=  5, "Value:%u, expected:5.", (::llc::u2_t)::llc::ASCII_COLOR_DARKMAGENTA);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_COLOR_DARKYELLOW	, ::llc::ASCII_COLOR_DARKYELLOW	!=  6, "Value:%u, expected:6.", (::llc::u2_t)::llc::ASCII_COLOR_DARKYELLOW);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_COLOR_LIGHTGREY	, ::llc::ASCII_COLOR_LIGHTGREY	!=  7, "Value:%u, expected:7.", (::llc::u2_t)::llc::ASCII_COLOR_LIGHTGREY);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_COLOR_DARKGREY	, ::llc::ASCII_COLOR_DARKGREY	!=  8, "Value:%u, expected:8.", (::llc::u2_t)::llc::ASCII_COLOR_DARKGREY);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_COLOR_BLUE		, ::llc::ASCII_COLOR_BLUE		!=  9, "Value:%u, expected:9.", (::llc::u2_t)::llc::ASCII_COLOR_BLUE);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_COLOR_GREEN		, ::llc::ASCII_COLOR_GREEN		!= 10, "Value:%u, expected:10.", (::llc::u2_t)::llc::ASCII_COLOR_GREEN);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_COLOR_CYAN		, ::llc::ASCII_COLOR_CYAN		!= 11, "Value:%u, expected:11.", (::llc::u2_t)::llc::ASCII_COLOR_CYAN);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_COLOR_RED			, ::llc::ASCII_COLOR_RED			!= 12, "Value:%u, expected:12.", (::llc::u2_t)::llc::ASCII_COLOR_RED);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_COLOR_MAGENTA	, ::llc::ASCII_COLOR_MAGENTA		!= 13, "Value:%u, expected:13.", (::llc::u2_t)::llc::ASCII_COLOR_MAGENTA);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_COLOR_YELLOW		, ::llc::ASCII_COLOR_YELLOW		!= 14, "Value:%u, expected:14.", (::llc::u2_t)::llc::ASCII_COLOR_YELLOW);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_COLOR_WHITE		, ::llc::ASCII_COLOR_WHITE		!= 15, "Value:%u, expected:15.", (::llc::u2_t)::llc::ASCII_COLOR_WHITE);
	rtrn 0;
}

sttc ::llc::err_t testASCIIColorInvert(ATestError & errors) {
	for(::llc::u2_t background = 0; background < 16; ++background)
	for(::llc::u2_t foreground = 0; foreground < 16; ++foreground) {
		cnst ::llc::u1_t attribute = (::llc::u1_t)((background << 4) | foreground);
		cnst ::llc::ASCII_COLOR inverted = ::llc::ascii_color_invert((::llc::ASCII_COLOR)attribute);
		cnst ::llc::u1_t invertedValue = (::llc::u1_t)inverted;
		LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_INVERT_FOREGROUND, (::llc::u2_t)(invertedValue & 0x0F) != background, "Attribute:0x%02X, inverted:0x%02X, foreground:%u, expected:%u.", (::llc::u2_t)attribute, (::llc::u2_t)invertedValue, (::llc::u2_t)(invertedValue & 0x0F), background);
		LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_INVERT_BACKGROUND, (::llc::u2_t)((invertedValue >> 4) & 0x0F) != foreground, "Attribute:0x%02X, inverted:0x%02X, background:%u, expected:%u.", (::llc::u2_t)attribute, (::llc::u2_t)invertedValue, (::llc::u2_t)((invertedValue >> 4) & 0x0F), foreground);
		cnst ::llc::u1_t restored = (::llc::u1_t)::llc::ascii_color_invert(inverted);
		LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_INVERT_ROUND_TRIP, restored != attribute, "Attribute:0x%02X, restored:0x%02X.", (::llc::u2_t)attribute, (::llc::u2_t)restored);
	}
	rtrn 0;
}

::llc::err_t testASCIIColor(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	LLC_TEST_CHECK(errors, ASCII_COLOR_TEST_RESULT_INDEX_UNDERLYING_TYPE, (false == std::is_same_v<std::underlying_type_t<::llc::ASCII_COLOR_INDEX>, ::llc::u2_t>));
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_INDEX_SIZE, sizeof(::llc::ASCII_COLOR_INDEX) != sizeof(::llc::u2_t), "Size:%u, expected:%u.", (::llc::u2_t)sizeof(::llc::ASCII_COLOR_INDEX), (::llc::u2_t)sizeof(::llc::u2_t));
	LLC_TEST_CHECK(errors, ASCII_COLOR_TEST_RESULT_COLOR_UNDERLYING_TYPE, (false == std::is_same_v<std::underlying_type_t<::llc::ASCII_COLOR>, ::llc::u1_t>));
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_COLOR_SIZE, sizeof(::llc::ASCII_COLOR) != sizeof(::llc::u1_t), "Size:%u, expected:%u.", (::llc::u2_t)sizeof(::llc::ASCII_COLOR), (::llc::u2_t)sizeof(::llc::u1_t));
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_INDEX_COUNT, ::llc::ASCII_COLOR_INDEX_COUNT != 16, "Count:%u, expected:16.", (::llc::u2_t)::llc::ASCII_COLOR_INDEX_COUNT);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_COLOR_COUNT, ::llc::ASCII_COLOR_COUNT != 16, "Count:%u, expected:16.", (::llc::u2_t)::llc::ASCII_COLOR_COUNT);
	LLC_TEST_CHECKF(errors, ASCII_COLOR_TEST_RESULT_PALETTE_COUNT, sizeof(::llc::ASCII_PALETTE) / sizeof(::llc::ASCII_PALETTE[0]) != 16, "Count:%u, expected:16.", (::llc::u2_t)(sizeof(::llc::ASCII_PALETTE) / sizeof(::llc::ASCII_PALETTE[0])));
	if_fail_fe(::testASCIIColorIndices(errors));
	if_fail_fe(::testASCIIPalette(errors));
	if_fail_fe(::testASCIIColorValues(errors));
	rtrn ::testASCIIColorInvert(errors);
}
