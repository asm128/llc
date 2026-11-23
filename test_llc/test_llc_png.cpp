#include "llc_png.h"
#include "test_llc_suites.h"

GDEFINE_ENUM_TYPE(PNG_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(PNG_TEST_RESULT, CRC32			, 0, "PNG CRC32 does not match the standard check value.");
GDEFINE_ENUM_VALUED(PNG_TEST_RESULT, INTERLACED_RESULT	, 1, "pngDecode() rejected a valid one-pixel Adam7 grayscale image.");
GDEFINE_ENUM_VALUED(PNG_TEST_RESULT, INTERLACED_VALUE	, 2, "pngDecode() returned the wrong Adam7 grayscale pixel.");

::llc::err_t test_png(STestRunnerState &, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	stxp u0_c bytes[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
	LLC_TEST_CHECKF(errors, PNG_TEST_RESULT_CRC32, 0xCBF43926U != ::llc::pngCRCGet(bytes), "Expected:0x%08X.", 0xCBF43926U);

	::llc::SPNGData	pngData				= {};
	pngData.Header.Size					= {1, 1};
	pngData.Header.BitDepth				= 8;
	pngData.Header.ColorType			= ::llc::COLOR_TYPE_GRAYSCALE;
	pngData.Header.MethodInterlace		= 1;
	pngData.Adam7Sizes[0]				= {1, 1};
	if_fail_fe(pngData.Scanlines.resize(1));
	if_fail_fe(pngData.Scanlines[0].push_back(0x7F));
	::llc::imgu0_t	decoded				= {};
	cnst ::llc::err_t decodeResult		= ::llc::pngDecode(pngData, decoded);
	LLC_TEST_CHECK(errors, PNG_TEST_RESULT_INTERLACED_RESULT, decodeResult < 0);
	cnst ::llc::n2u2_t expectedMetrics	= {1, 1};
	LLC_TEST_CHECK(errors, PNG_TEST_RESULT_INTERLACED_VALUE, decoded.metrics() != expectedMetrics || decoded[0][0] != 0x7F);
	rtrn 0;
}
