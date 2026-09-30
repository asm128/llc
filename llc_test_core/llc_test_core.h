#ifndef LLC_TEST_CORE_H
#define LLC_TEST_CORE_H

#include "llc_enum.h"

struct STestError {
	::llc::s3_t		Value		= 0;
	::llc::sc_c		* EnumName	= 0;
	::llc::sc_c		* ValueName	= 0;
	::llc::sc_c		* Description	= 0;
	::llc::u2_t		Count		= 0;
};

tydf ::llc::aobj<STestError> ATestError;

tplt<tpnm TEnum>
::llc::err_t testErrorRecord(ATestError & errors, TEnum value) {
	::llc::sc_c * enumName = ::llc::get_enum_namep(value);
	for(::llc::u2_t iError = 0; iError < errors.size(); ++iError)
		if(errors[iError].Value == (::llc::s3_t)value && 0 == strcmp(errors[iError].EnumName, enumName)) {
			++errors[iError].Count;
			return 0;
		}
	cnst ::llc::err_t result = errors.push_back({(::llc::s3_t)value, enumName, ::llc::get_value_namep(value), ::llc::get_value_descp(value), 1});
	return ::llc::failed(result) ? result : 0;
}

inln ::llc::u2_t testErrorCount(cnst ATestError & errors) {
	::llc::u2_t count = 0;
	for(::llc::u2_t iError = 0; iError < errors.size(); ++iError)
		count += errors[iError].Count;
	return count;
}

#define LLC_TEST_CHECK(errors, result, condition, format, ...) do { \
	if_true_block_logf(error_printf, (condition), if_fail_fe(testErrorRecord((errors), (result))) \
		, "%s::%s(%" LLC_FMT_S3 "): %s " format, ::llc::get_enum_namep(result), ::llc::get_value_namep(result), (::llc::s3_t)(result), ::llc::get_value_descp(result), __VA_ARGS__); \
} while(0)

#define LLC_TEST_REQUIRE(errors, result, condition, format, ...) do { \
	if_true_block_logf(error_printf, (condition), { if_fail_fe(testErrorRecord((errors), (result))); return 0; } \
		, "%s::%s(%" LLC_FMT_S3 "): %s " format, ::llc::get_enum_namep(result), ::llc::get_value_namep(result), (::llc::s3_t)(result), ::llc::get_value_descp(result), __VA_ARGS__); \
} while(0)

::llc::err_t testPackedUInt(ATestError & errors);
::llc::err_t testSPRNG(ATestError & errors);
::llc::err_t testViewBit(ATestError & errors);
::llc::err_t testViewSerialize(ATestError & errors);

#endif // LLC_TEST_CORE_H
