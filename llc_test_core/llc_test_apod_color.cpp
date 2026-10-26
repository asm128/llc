#include "llc_test_core_suites.h"
#include "llc_apod_color.h"

GDEFINE_ENUM_TYPE(TEST_APOD_COLOR_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_APOD_COLOR_RESULT, ALIASES        ,  0, "Each color-array alias preserves its declared apod element type.");
GDEFINE_ENUM_VALUED(TEST_APOD_COLOR_RESULT, EMPTY          ,  1, "Every color-array alias begins empty.");
GDEFINE_ENUM_VALUED(TEST_APOD_COLOR_RESULT, FRGBA_VALUES   ,  2, "Floating-point RGBA values retain every channel.");
GDEFINE_ENUM_VALUED(TEST_APOD_COLOR_RESULT, BGRA_VALUES    ,  3, "Byte BGRA values retain every channel.");
GDEFINE_ENUM_VALUED(TEST_APOD_COLOR_RESULT, BGR_VALUES     ,  4, "Byte BGR values retain every channel.");
GDEFINE_ENUM_VALUED(TEST_APOD_COLOR_RESULT, RGBA_VALUES    ,  5, "Byte RGBA values retain every channel.");
GDEFINE_ENUM_VALUED(TEST_APOD_COLOR_RESULT, RGB_VALUES     ,  6, "Byte RGB values retain every channel.");
GDEFINE_ENUM_VALUED(TEST_APOD_COLOR_RESULT, COPY           ,  7, "Copying a color array duplicates storage and values.");
GDEFINE_ENUM_VALUED(TEST_APOD_COLOR_RESULT, MOVE           ,  8, "Moving a color array transfers storage and clears the source.");
GDEFINE_ENUM_VALUED(TEST_APOD_COLOR_RESULT, SELF_ASSIGNMENT,  9, "Self-assignment preserves color-array storage and values.");
GDEFINE_ENUM_VALUED(TEST_APOD_COLOR_RESULT, BOUNDARIES     , 10, "Removing the first and final colors preserves the remaining value.");
GDEFINE_ENUM_VALUED(TEST_APOD_COLOR_RESULT, VIEW_ALIASING  , 11, "A color-array view aliases the array's live storage.");
GDEFINE_ENUM_VALUED(TEST_APOD_COLOR_RESULT, BYTE_LAYOUT    , 12, "A BGRA array exposes its exact packed channel layout.");

tplt<tpnm TArray, tpnm TColor>
sttc ::llc::err_t testAPodColorValuesT(::llc::ATestError & results, TEST_APOD_COLOR_RESULT resultCase, cnst TColor & first, cnst TColor & second) {
	TArray values;
	LLC_TEST_REQUIRE(results, resultCase, 0 != values.push_back(first));
	LLC_TEST_REQUIRE(results, resultCase, 1 != values.push_back(second));
	LLC_TEST_CHECK(results, resultCase, values.size() != 2 || 0 != memcmp(&values[0], &first, szof(TColor)) || 0 != memcmp(&values[1], &second, szof(TColor)));
	rtrn 0;
}

sttc ::llc::err_t testAPodColorAliases(::llc::ATestError & results) {
	::llc::apod<::llc::rgbaf> * expectedFRGBA = (::llc::afrgba*)0;
	::llc::apod<::llc::bgra > * expectedBGRA  = (::llc::a8bgra*)0;
	::llc::apod<::llc::bgr  > * expectedBGR   = (::llc::a8bgr *)0;
	::llc::apod<::llc::rgba > * expectedRGBA  = (::llc::a8rgba*)0;
	::llc::apod<::llc::rgb  > * expectedRGB   = (::llc::a8rgb *)0;
	LLC_TEST_CHECK(results, TEST_APOD_COLOR_RESULT_ALIASES, expectedFRGBA || expectedBGRA || expectedBGR || expectedRGBA || expectedRGB);
	rtrn 0;
}

sttc ::llc::err_t testAPodColorEmpty(::llc::ATestError & results) {
	::llc::afrgba floats;
	::llc::a8bgra bgra;
	::llc::a8bgr  bgr;
	::llc::a8rgba rgba;
	::llc::a8rgb  rgb;
	LLC_TEST_CHECK(results, TEST_APOD_COLOR_RESULT_EMPTY
		, floats.size() || floats.begin()
		|| bgra.size()   || bgra.begin()
		|| bgr.size()    || bgr.begin()
		|| rgba.size()   || rgba.begin()
		|| rgb.size()    || rgb.begin()
		);
	rtrn 0;
}

sttc ::llc::err_t testAPodColorFRGBAValues(::llc::ATestError & results) {
	rtrn ::testAPodColorValuesT<::llc::afrgba>(results, TEST_APOD_COLOR_RESULT_FRGBA_VALUES, ::llc::rgbaf{0.25f, 0.50f, 0.75f, 1.00f}, ::llc::rgbaf{1.00f, 0.75f, 0.50f, 0.25f});
}

sttc ::llc::err_t testAPodColorBGRAValues(::llc::ATestError & results) {
	rtrn ::testAPodColorValuesT<::llc::a8bgra>(results, TEST_APOD_COLOR_RESULT_BGRA_VALUES, ::llc::bgra{1, 2, 3, 4}, ::llc::bgra{5, 6, 7, 8});
}

sttc ::llc::err_t testAPodColorBGRValues(::llc::ATestError & results) {
	rtrn ::testAPodColorValuesT<::llc::a8bgr>(results, TEST_APOD_COLOR_RESULT_BGR_VALUES, ::llc::bgr{1, 2, 3}, ::llc::bgr{4, 5, 6});
}

sttc ::llc::err_t testAPodColorRGBAValues(::llc::ATestError & results) {
	rtrn ::testAPodColorValuesT<::llc::a8rgba>(results, TEST_APOD_COLOR_RESULT_RGBA_VALUES, ::llc::rgba{1, 2, 3, 4}, ::llc::rgba{5, 6, 7, 8});
}

sttc ::llc::err_t testAPodColorRGBValues(::llc::ATestError & results) {
	rtrn ::testAPodColorValuesT<::llc::a8rgb>(results, TEST_APOD_COLOR_RESULT_RGB_VALUES, ::llc::rgb{1, 2, 3}, ::llc::rgb{4, 5, 6});
}

sttc ::llc::err_t testAPodColorCopy(::llc::ATestError & results) {
	::llc::a8bgra source = {{1, 2, 3, 4}, {5, 6, 7, 8}};
	::llc::a8bgra copied = source;
	LLC_TEST_CHECK(results, TEST_APOD_COLOR_RESULT_COPY, copied.size() != source.size() || copied.begin() == source.begin() || 0 != memcmp(copied.begin(), source.begin(), source.byte_count()));
	rtrn 0;
}

sttc ::llc::err_t testAPodColorMove(::llc::ATestError & results) {
	::llc::a8bgra source = {{1, 2, 3, 4}, {5, 6, 7, 8}};
	::llc::bgra * sourceData = source.begin();
	::llc::a8bgra moved = (::llc::a8bgra&&)source;
	LLC_TEST_CHECK(results, TEST_APOD_COLOR_RESULT_MOVE, source.size() || source.begin() || moved.size() != 2 || moved.begin() != sourceData);
	rtrn 0;
}

sttc ::llc::err_t testAPodColorSelfAssignment(::llc::ATestError & results) {
	::llc::a8bgra values = {{1, 2, 3, 4}, {5, 6, 7, 8}};
	::llc::bgra * originalData = values.begin();
	values = values;
	LLC_TEST_CHECK(results, TEST_APOD_COLOR_RESULT_SELF_ASSIGNMENT, values.size() != 2 || values.begin() != originalData || values[0].b != 1 || values[1].a != 8);
	rtrn 0;
}

sttc ::llc::err_t testAPodColorBoundaries(::llc::ATestError & results) {
	::llc::a8rgb values = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
	LLC_TEST_REQUIRE(results, TEST_APOD_COLOR_RESULT_BOUNDARIES, 2 != values.remove(0));
	LLC_TEST_CHECK(results, TEST_APOD_COLOR_RESULT_BOUNDARIES, values[0].r != 4 || values[1].b != 9);
	LLC_TEST_REQUIRE(results, TEST_APOD_COLOR_RESULT_BOUNDARIES, 1 != values.pop_back());
	LLC_TEST_CHECK(results, TEST_APOD_COLOR_RESULT_BOUNDARIES, values.size() != 1 || values[0].r != 4 || values[0].g != 5 || values[0].b != 6);
	rtrn 0;
}

sttc ::llc::err_t testAPodColorViewAliasing(::llc::ATestError & results) {
	::llc::a8bgra values = {{1, 2, 3, 4}};
	::llc::vbgra & view = values;
	view[0].r = 37;
	LLC_TEST_CHECK(results, TEST_APOD_COLOR_RESULT_VIEW_ALIASING, view.begin() != values.begin() || values[0].r != 37);
	rtrn 0;
}

sttc ::llc::err_t testAPodColorByteLayout(::llc::ATestError & results) {
	::llc::a8bgra values = {{1, 2, 3, 4}, {5, 6, 7, 8}};
	cnst ::llc::u0_t expected[] = {1, 2, 3, 4, 5, 6, 7, 8};
	cnst ::llc::vcu8 bytes = values.cu8();
	LLC_TEST_CHECK(results, TEST_APOD_COLOR_RESULT_BYTE_LAYOUT, bytes.size() != ::llc::size(expected) || 0 != memcmp(bytes.begin(), expected, szof(expected)));
	rtrn 0;
}

::llc::err_t testAPodColor(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testAPodColorAliases(results));
	result = ::llc::min(result, ::testAPodColorEmpty(results));
	result = ::llc::min(result, ::testAPodColorFRGBAValues(results));
	result = ::llc::min(result, ::testAPodColorBGRAValues(results));
	result = ::llc::min(result, ::testAPodColorBGRValues(results));
	result = ::llc::min(result, ::testAPodColorRGBAValues(results));
	result = ::llc::min(result, ::testAPodColorRGBValues(results));
	result = ::llc::min(result, ::testAPodColorCopy(results));
	result = ::llc::min(result, ::testAPodColorMove(results));
	result = ::llc::min(result, ::testAPodColorSelfAssignment(results));
	result = ::llc::min(result, ::testAPodColorBoundaries(results));
	result = ::llc::min(result, ::testAPodColorViewAliasing(results));
	result = ::llc::min(result, ::testAPodColorByteLayout(results));
	rtrn result;
}
