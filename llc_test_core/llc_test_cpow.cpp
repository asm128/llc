#include "llc_cpow.h"

#include "llc_test_core.h"

#include <type_traits>

GDEFINE_ENUM_TYPE(CPOW_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(CPOW_TEST_RESULT, OK				, 0, "All compile-time power tests passed.");
GDEFINE_ENUM_VALUED(CPOW_TEST_RESULT, RETURN_TYPE		, 1, "cpow<>() did not preserve the base type.");
GDEFINE_ENUM_VALUED(CPOW_TEST_RESULT, VALUE				, 2, "cpow<>() returned an incorrect power.");
GDEFINE_ENUM_VALUED(CPOW_TEST_RESULT, CONSTEXPR_VALUE		, 3, "cpow<>() did not produce the expected compile-time value.");
GDEFINE_ENUM_VALUED(CPOW_TEST_RESULT, NEGATIVE_BASE		, 4, "cpow<>() returned an incorrect signed power.");
GDEFINE_ENUM_VALUED(CPOW_TEST_RESULT, FRACTIONAL_BASE		, 5, "cpow<>() returned an incorrect fractional power.");
GDEFINE_ENUM_VALUED(CPOW_TEST_RESULT, DIGIT_ASCII_DECIMAL	, 6, "digit_ascii() did not map a decimal remainder correctly.");
GDEFINE_ENUM_VALUED(CPOW_TEST_RESULT, DIGIT_ASCII_LETTER	, 7, "digit_ascii() did not map an alphabetic remainder correctly.");
GDEFINE_ENUM_VALUED(CPOW_TEST_RESULT, DIGIT_ASCII_BASE		, 8, "digit_ascii() did not reduce and map a value in the requested base.");
GDEFINE_ENUM_VALUED(CPOW_TEST_RESULT, DIGIT_POSITION		, 9, "digit<>() did not extract the requested digit.");

tplt<size_t exp, tpnm T>
sttc ::llc::err_t testCPowValue(ATestError & errors, T base, T expected, CPOW_TEST_RESULT result = CPOW_TEST_RESULT_VALUE) {
	cnst T actual = ::llc::cpow<exp>(base);
	always_printf("CPOW type:%u-bit %s; exponent:%u; base:%.17g; expected:%.17g; actual:%.17g; %s.", (::llc::u2_t)bcof(T), ::std::is_floating_point_v<T> ? "floating" : ::std::is_signed_v<T> ? "signed" : "unsigned", (::llc::u2_t)exp, (::llc::f3_t)base, (::llc::f3_t)expected, (::llc::f3_t)actual, actual == expected ? "PASS" : "FAIL");
	LLC_TEST_CHECKF(errors, CPOW_TEST_RESULT_RETURN_TYPE, false == (::std::is_same_v<decltype(::llc::cpow<exp>(base)), T>) , "cpow<%u>() return type mismatch. base:%f." , (::llc::u2_t)exp, (::llc::f3_t)base );
	LLC_TEST_CHECKF(errors, result, actual != expected , "cpow<%u>(%f) mismatch. actual:%f, expected:%f." , (::llc::u2_t)exp, (::llc::f3_t)base, (::llc::f3_t)actual, (::llc::f3_t)expected );
	rtrn 0;
}

struct SCPowCase { size_t Exponent; ::llc::f3_t Base, Expected; };
stxp SCPowCase CPowCases[]			= {{0, 0, 1}, {1, 0, 0}, {6, 1, 1}, {1, 2, 2}, {3, 2, 8}, {6, 2, 64}};
stxp SCPowCase CPowSignedCases[]		= {{1, -2, -2}, {2, -2, 4}, {3, -2, -8}, {6, -2, 64}};
stxp SCPowCase CPowFractionalCases[]	= {{1, .5, .5}, {2, .5, .25}, {4, .5, .0625}};

tplt<tpnm T, auto & cases, CPOW_TEST_RESULT result = CPOW_TEST_RESULT_VALUE, size_t iCase = 0>
sttc ::llc::err_t testCPowCases(ATestError & errors) {
	stxp auto testCase = cases[iCase];
	static_assert(::std::is_same_v<decltype(::llc::cpow<testCase.Exponent>(T{})), T>);
	stxp T compileTimeValue = ::llc::cpow<testCase.Exponent>(T(testCase.Base));
	static_assert(compileTimeValue == T(testCase.Expected));
	LLC_TEST_CHECKF(errors, CPOW_TEST_RESULT_CONSTEXPR_VALUE, compileTimeValue != T(testCase.Expected) , "constexpr cpow<%u>(%f) mismatch. actual:%f, expected:%f." , (::llc::u2_t)testCase.Exponent, testCase.Base, (::llc::f3_t)compileTimeValue, testCase.Expected );
	if_fail_fe(testCPowValue<testCase.Exponent>(errors, T(testCase.Base), T(testCase.Expected), result));
	if constexpr(iCase + 1 < ::llc::size(cases))
		if_fail_fe((testCPowCases<T, cases, result, iCase + 1>(errors)));
	rtrn 0;
}

tplt<::llc::u0_t exp = 0, tpnm T, size_t count>
sttc ::llc::err_t testDigits(ATestError & errors, T value, T base, cnst char (&expected)[count]) {
	cnst char actual = (base == T(10)) ? ::llc::digit<exp>(value) : ::llc::digit<exp>(value, base);
	LLC_TEST_CHECKF(errors, CPOW_TEST_RESULT_DIGIT_POSITION, actual != expected[exp] , "digit<%u>(%" LLC_FMT_U3 ", base:%u) mismatch. actual:'%c', expected:'%c'." , (::llc::u2_t)exp, (::llc::u3_t)value, (::llc::u2_t)base, actual, expected[exp] );
	if constexpr(exp + 2 < count)
		if_fail_fe(testDigits<exp + 1>(errors, value, base, expected));
	rtrn 0;
}

tplt<tpnm T, tpnm... TRest>
sttc ::llc::err_t testCPowTypes(ATestError & errors) {
	if_fail_fe((testCPowCases<T, CPowCases>(errors)));
	if constexpr(::std::is_signed_v<T>)
		if_fail_fe((testCPowCases<T, CPowSignedCases, CPOW_TEST_RESULT_NEGATIVE_BASE>(errors)));
	if constexpr(::std::is_floating_point_v<T>)
		if_fail_fe((testCPowCases<T, CPowFractionalCases, CPOW_TEST_RESULT_FRACTIONAL_BASE>(errors)));
	if constexpr(::std::is_unsigned_v<T>) {
		if constexpr(sizeof(T) <= 2) {
			stxp SCPowCase narrowCases[] = {{2, ::llc::u2_t(1) << (sizeof(T) * 4), 0}};
			if_fail_fe((testCPowCases<T, narrowCases>(errors)));
		}
		if_fail_fe(testDigits(errors, T(123), T(10), "3210"));
	}
	if constexpr(sizeof...(TRest))
		if_fail_fe((testCPowTypes<TRest...>(errors)));
	rtrn 0;
}

::llc::err_t testCPow(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	if_fail_fe((testCPowTypes<
		::llc::i0u_t, ::llc::i1u_t, ::llc::i2u_t, ::llc::i3u_t,
		::llc::i0s_t, ::llc::i1s_t, ::llc::i2s_t, ::llc::i3s_t,
		::llc::f2s_t, ::llc::f3s_t
	>(errors)));

	stxp struct { char Remainder, Expected; CPOW_TEST_RESULT Result; } asciiCases[] = {
		{ 0, '0', CPOW_TEST_RESULT_DIGIT_ASCII_DECIMAL}, { 9, '9', CPOW_TEST_RESULT_DIGIT_ASCII_DECIMAL},
		{10, 'A', CPOW_TEST_RESULT_DIGIT_ASCII_LETTER }, {35, 'Z', CPOW_TEST_RESULT_DIGIT_ASCII_LETTER },
		{36, 'a', CPOW_TEST_RESULT_DIGIT_ASCII_LETTER }, {61, 'z', CPOW_TEST_RESULT_DIGIT_ASCII_LETTER },
	};
	for(cnst auto & testCase : asciiCases) {
		cnst char actual = ::llc::digit_ascii(testCase.Remainder);
		LLC_TEST_CHECKF(errors, testCase.Result, actual != testCase.Expected , "digit_ascii(%u) mismatch. actual:'%c', expected:'%c'." , (::llc::u2_t)testCase.Remainder, actual, testCase.Expected );
	}
	stxp struct { ::llc::u3_t Value; ::llc::u0_t Base; char Expected; } baseCases[] = {
		{255, 16, 'F'}, {61, ::llc::ASCII_DIGIT_COUNT, 'z'},
	};
	for(cnst auto & testCase : baseCases) {
		cnst char actual = ::llc::digit_ascii(testCase.Value, testCase.Base);
		LLC_TEST_CHECKF(errors, CPOW_TEST_RESULT_DIGIT_ASCII_BASE, actual != testCase.Expected , "digit_ascii(%" LLC_FMT_U3 ", base:%u) mismatch. actual:'%c', expected:'%c'." , testCase.Value, (::llc::u2_t)testCase.Base, actual, testCase.Expected );
	}
	if_fail_fe(testDigits(errors, ::llc::u2_t(0xBEEF), ::llc::u2_t(16), "FEEB"));
	rtrn 0;
}
