#include "llc_png.h"
#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(PNG_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(PNG_TEST_RESULT, CRC32, 0, "PNG CRC32 does not match the standard check value.");

::llc::err_t testPNG(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	cnst ::llc::u0_t bytes[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
	LLC_TEST_CHECKF(errors, PNG_TEST_RESULT_CRC32, 0xCBF43926U != ::llc::get_crc(::llc::vcu0_t{bytes}), "Expected:0x%08X.", 0xCBF43926U);
	rtrn 0;
}
