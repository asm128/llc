#include "llc_enum.h"

#ifndef LLC_TEST_CORE_H
#define LLC_TEST_CORE_H

struct STestError {
	::llc::s3_t		Value			= {};
	::llc::sc_c		* EnumName		= {};
	::llc::sc_c		* ValueName		= {};
	::llc::sc_c		* Description	= {};
	::llc::u2_t		Count			= {};
	::llc::u2_t		SuccessCount	= {};
};

tydf ::llc::aobj<STestError> ATestError;

tplt<tpnm TEnum>			::llc::err_t testResultRecord(ATestError & errors, TEnum value, bool failed) {
	::llc::sc_c * enumName = ::llc::get_enum_namep(value);
	for(::llc::u2_t iError = 0; iError < errors.size(); ++iError)
		if(errors[iError].Value == (::llc::s3_t)value && 0 == strcmp(errors[iError].EnumName, enumName)) {
			failed ? ++errors[iError].Count : ++errors[iError].SuccessCount;
			return 0;
		}
	cnst ::llc::err_t result = errors.push_back({(::llc::s3_t)value, enumName, ::llc::get_value_namep(value), ::llc::get_value_descp(value), failed ? 1U : 0U, failed ? 0U : 1U});
	return ::llc::failed(result) ? result : 0;
}
tplt<tpnm TEnum>	stin	::llc::err_t testErrorRecord	(ATestError & errors, TEnum value) { return testResultRecord(errors, value, true); }
tplt<tpnm TEnum>	stin	::llc::err_t testSuccessRecord	(ATestError & errors, TEnum value) { return testResultRecord(errors, value, false); }

tplt<tpnm TGetter>
inln ::llc::u2_t testResultCount(cnst ATestError & errors, TGetter getter) {
	::llc::u2_t count = 0;
	for(::llc::u2_t iError = 0; iError < errors.size(); ++iError)
		count += getter(errors[iError]);
	return count;
}
stin ::llc::u2_t testErrorCount		(cnst ATestError & errors) { return testResultCount(errors, [](cnst STestError & error) { return error.Count; }); }
stin ::llc::u2_t testSuccessCount	(cnst ATestError & errors) { return testResultCount(errors, [](cnst STestError & error) { return error.SuccessCount; }); }
stin ::llc::u2_t testCheckCount		(cnst ATestError & errors) { return testErrorCount(errors) + testSuccessCount(errors); }

#ifdef LLC_WINDOWS
tplt<tpnm TCall>
bool testThrows(TCall call) {
	bool didThrow = false;
	::llc::setupLogCallbacks(0, 0);
	try { call(); }
	catch(...) { didThrow = true; }
	::llc::setupDefaultLogCallbacks();
	return didThrow;
}
#endif

#define LLC_TEST_CHECK_BASE(errors, result, condition, format, block, ...) do {													\
	if_true_block_logf(error_printf, condition, block																			\
		, "%s::%s(%" LLC_FMT_S3 "):\"%s\" " format																				\
		, ::llc::get_enum_namep(result), ::llc::get_value_namep(result), (::llc::s3_t)(result), ::llc::get_value_descp(result)	\
		, __VA_ARGS__)																											\
	else {																														\
		if_fail_fe(testSuccessRecord((errors), (result)));																		\
	}																															\
} while(0)

#define LLC_TEST_CHECK(errors, result, condition, format, ...)		LLC_TEST_CHECK_BASE(errors, result, condition, format, { if_fail_fe(testErrorRecord((errors), (result))); }, __VA_ARGS__)
#define LLC_TEST_REQUIRE(errors, result, condition, format, ...)	LLC_TEST_CHECK_BASE(errors, result, condition, format, { if_fail_fe(testErrorRecord((errors), (result))); return 0; }, __VA_ARGS__)

::llc::err_t testPackedUInt		(ATestError & errors);
::llc::err_t testSPRNG			(ATestError & errors);
::llc::err_t testStr			(ATestError & errors);
::llc::err_t testView			(ATestError & errors);
::llc::err_t testViewBit		(ATestError & errors);
::llc::err_t testViewSerialize	(ATestError & errors);

#endif // LLC_TEST_CORE_H
