#include "llc_test_core_suites.h"
#include "llc_adam7.h"

GDEFINE_ENUM_TYPE(TEST_ADAM7_H_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_ADAM7_H_RESULT, U8_INSTANTIATION        , 0, "The interlace template instantiates and places an 8-bit texel.");
GDEFINE_ENUM_VALUED(TEST_ADAM7_H_RESULT, U16_INSTANTIATION       , 1, "The interlace template instantiates and places a 16-bit texel.");
GDEFINE_ENUM_VALUED(TEST_ADAM7_H_RESULT, INVALID_PASS_PROPAGATION, 2, "An eighth pass propagates the invalid-index failure without writing.");
GDEFINE_ENUM_VALUED(TEST_ADAM7_H_RESULT, CLIPPING                , 3, "Source texels mapped outside the destination grid are clipped.");
GDEFINE_ENUM_VALUED(TEST_ADAM7_H_RESULT, PLACEMENT_8X8           , 4, "All seven pass values occupy their exact 8x8 coordinates.");
GDEFINE_ENUM_VALUED(TEST_ADAM7_H_RESULT, PUBLIC_SIGNATURES       , 5, "The two functions and two template instantiations retain their public signatures.");

namespace {
	stxp ::llc::n2u2_t ADAM7_H_MULTIPLIERS[7] =
		{{8, 8}, {8, 8}, {4, 8}, {4, 4}, {2, 4}, {2, 2}, {1, 2}};
	stxp ::llc::n2u2_t ADAM7_H_BASES[7] =
		{{0, 0}, {4, 0}, {0, 4}, {2, 0}, {0, 2}, {1, 0}, {0, 1}};
	stxp ::llc::n2u2_t ADAM7_H_SIZES_8X8[7] =
		{{1, 1}, {1, 1}, {2, 1}, {2, 2}, {4, 2}, {4, 4}, {8, 4}};
}

sttc ::llc::err_t testAdam7HU8Instantiation(::llc::ATestError & results) {
	::llc::imgu0_t pass;
	if_fail_fe(pass.resize({1, 1}, ::llc::u0_t(0x5A)));
	::llc::imgu0_t output;
	if_fail_fe(output.resize({1, 1}, ::llc::u0_t(0)));
	cnst ::llc::err_t result = ::llc::adam7Interlace(::llc::view<::llc::imgu0_t>{&pass, 1}, output.View);
	LLC_TEST_CHECKF(results, TEST_ADAM7_H_RESULT_U8_INSTANTIATION
		, ::llc::failed(result) || output.Texels[0] != 0x5A
		, "result:%i value:%u", result, output.Texels[0]);
	rtrn 0;
}

sttc ::llc::err_t testAdam7HU16Instantiation(::llc::ATestError & results) {
	::llc::imgu1_t pass;
	if_fail_fe(pass.resize({1, 1}, ::llc::u1_t(0x5AA5)));
	::llc::imgu1_t output;
	if_fail_fe(output.resize({1, 1}, ::llc::u1_t(0)));
	cnst ::llc::err_t result = ::llc::adam7Interlace(::llc::view<::llc::imgu1_t>{&pass, 1}, output.View);
	LLC_TEST_CHECKF(results, TEST_ADAM7_H_RESULT_U16_INSTANTIATION
		, ::llc::failed(result) || output.Texels[0] != 0x5AA5
		, "result:%i value:%u", result, output.Texels[0]);
	rtrn 0;
}

sttc ::llc::err_t testAdam7HInvalidPassPropagation(::llc::ATestError & results) {
	::llc::imgu0_t passes[8];
	::llc::imgu0_t output;
	if_fail_fe(output.resize({1, 1}, ::llc::u0_t(0x33)));
	::llc::STestLogBuffer expectedLog;
	::llc::err_t result = 0;
	{
		::llc::STestLogCapture capture(expectedLog);
		result = ::llc::adam7Interlace(::llc::view<::llc::imgu0_t>{passes}, output.View);
	}
	LLC_TEST_CHECKF(results, TEST_ADAM7_H_RESULT_INVALID_PASS_PROPAGATION
		, !::llc::failed(result) || !expectedLog.Failed || output.Texels[0] != 0x33
		, "result:%i value:%u", result, output.Texels[0]);
	rtrn 0;
}

sttc ::llc::err_t testAdam7HClipping(::llc::ATestError & results) {
	::llc::imgu0_t pass;
	if_fail_fe(pass.resize({3, 2}, ::llc::u0_t(0x6D)));
	::llc::imgu0_t output;
	if_fail_fe(output.resize({1, 1}, ::llc::u0_t(0)));
	cnst ::llc::err_t result = ::llc::adam7Interlace(::llc::view<::llc::imgu0_t>{&pass, 1}, output.View);
	LLC_TEST_CHECKF(results, TEST_ADAM7_H_RESULT_CLIPPING
		, ::llc::failed(result) || output.Texels[0] != 0x6D
		, "result:%i value:%u", result, output.Texels[0]);
	rtrn 0;
}

sttc ::llc::err_t testAdam7HPlacement8x8(::llc::ATestError & results) {
	::llc::imgu0_t passes[7];
	::llc::u0_t expected[8 * 8] = {};
	for(::llc::u2_t iPass = 0; iPass < 7; ++iPass) {
		cnst ::llc::u0_t value = (::llc::u0_t)(0x10 + iPass);
		if_fail_fe(passes[iPass].resize(ADAM7_H_SIZES_8X8[iPass], value));
		for(::llc::u2_t y = 0; y < ADAM7_H_SIZES_8X8[iPass].y; ++y)
		for(::llc::u2_t x = 0; x < ADAM7_H_SIZES_8X8[iPass].x; ++x) {
			cnst ::llc::u2_t targetX = x * ADAM7_H_MULTIPLIERS[iPass].x + ADAM7_H_BASES[iPass].x;
			cnst ::llc::u2_t targetY = y * ADAM7_H_MULTIPLIERS[iPass].y + ADAM7_H_BASES[iPass].y;
			expected[targetY * 8 + targetX] = value;
		}
	}
	::llc::imgu0_t output;
	if_fail_fe(output.resize({8, 8}, ::llc::u0_t(0)));
	cnst ::llc::err_t result = ::llc::adam7Interlace(::llc::view<::llc::imgu0_t>{passes}, output.View);
	bool mismatch = ::llc::failed(result);
	for(::llc::u2_t iPixel = 0; !mismatch && iPixel < output.size(); ++iPixel)
		mismatch |= output.Texels[iPixel] != expected[iPixel];
	LLC_TEST_CHECKF(results, TEST_ADAM7_H_RESULT_PLACEMENT_8X8, mismatch, "result:%i", result);
	rtrn 0;
}

sttc ::llc::err_t testAdam7HPublicSignatures(::llc::ATestError & results) {
	typedef ::llc::err_t (*TScaleIndex)(::llc::s2_t, ::llc::n2u2_t &, ::llc::n2u2_t &);
	typedef ::llc::err_t (*TSizes)(::llc::view<::llc::n2u2_t> &, cnst ::llc::n2u2_t &);
	typedef ::llc::err_t (*TInterlaceU8)(::llc::view<::llc::imgu0_t>, ::llc::grid<::llc::u0_t> &);
	typedef ::llc::err_t (*TInterlaceU16)(::llc::view<::llc::imgu1_t>, ::llc::grid<::llc::u1_t> &);
	cnst TScaleIndex scaleIndex = &::llc::adam7ScaleIndex;
	cnst TSizes sizes = &::llc::adam7Sizes;
	cnst TInterlaceU8 interlaceU8 = &::llc::adam7Interlace<::llc::u0_t>;
	cnst TInterlaceU16 interlaceU16 = &::llc::adam7Interlace<::llc::u1_t>;
	LLC_TEST_CHECK(results, TEST_ADAM7_H_RESULT_PUBLIC_SIGNATURES, !scaleIndex || !sizes || !interlaceU8 || !interlaceU16);
	rtrn 0;
}

::llc::err_t testAdam7H(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testAdam7HU8Instantiation(results));
	result = ::llc::min(result, ::testAdam7HU16Instantiation(results));
	result = ::llc::min(result, ::testAdam7HInvalidPassPropagation(results));
	result = ::llc::min(result, ::testAdam7HClipping(results));
	result = ::llc::min(result, ::testAdam7HPlacement8x8(results));
	result = ::llc::min(result, ::testAdam7HPublicSignatures(results));
	rtrn result;
}
