#include "llc_enum.h"

#ifndef LLC_TEST_CORE_H
#define LLC_TEST_CORE_H

struct STestError {
	::llc::s3_t		Value			= {};
	::llc::vcst_t	EnumName		= {};
	::llc::vcst_t	ValueName		= {};
	::llc::vcst_t	Description		= {};
	::llc::u2_t		Count			= {};
	::llc::u2_t		SuccessCount	= {};
};

tydf ::llc::aobj<STestError> ATestError;

tplt<tpnm TEnum>			::llc::err_t testResultRecord(ATestError & errors, TEnum value, bool failed) {
	cnst ::llc::vcsc_t & enumName = ::llc::get_enum_namev(value);
	for(::llc::u2_t iError = 0; iError < errors.size(); ++iError)
		if(errors[iError].Value == (::llc::s3_t)value && errors[iError].EnumName == enumName) {
			failed ? ++errors[iError].Count : ++errors[iError].SuccessCount;
			return 0;
		}
	cnst ::llc::err_t result = errors.push_back
		({ (::llc::s3_t)value
		, enumName
		, ::llc::get_value_namev(value)
		, ::llc::get_value_descv(value)
		, failed ? 1U : 0U
		, failed ? 0U : 1U
		});
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

#define LLC_TEST_CHECK_BASE(errors, result, condition, block, ...) do {														\
	if_true_block_logf(error_printf, (condition), block																		\
		, "%.*s::%.*s(%" LLC_FMT_S3 "):\"%.*s\" " __VA_ARGS__													\
		, (int)::llc::get_enum_namev(result).size(), ::llc::get_enum_namev(result).begin()									\
		, (int)::llc::get_value_namev(result).size(), ::llc::get_value_namev(result).begin(), (::llc::s3_t)(result)			\
		, (int)::llc::get_value_descv(result).size(), ::llc::get_value_descv(result).begin()								\
		)																															\
	else {																														\
		if_fail_fe(testSuccessRecord((errors), (result)));																		\
	}																															\
} while(0)

#define LLC_TEST_CHECK(errors, result, condition, ...)		LLC_TEST_CHECK_BASE(errors, result, (condition), { if_fail_fe(testErrorRecord((errors), (result))); }, __VA_ARGS__)
#define LLC_TEST_REQUIRE(errors, result, condition, ...)	LLC_TEST_CHECK_BASE(errors, result, (condition), { if_fail_fe(testErrorRecord((errors), (result))); return 0; }, __VA_ARGS__)

#endif // LLC_TEST_CORE_H
