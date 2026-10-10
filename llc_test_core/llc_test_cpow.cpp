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
	LLC_TEST_CHECKF(errors, CPOW_TEST_RESULT_RETURN_TYPE, false == (::std::is_same_v<decltype(::llc::cpow<exp>(base)), T>) , "cpow<%u>() return type mismatch. base:%f." , (::llc::u2_t)exp, (::llc::f3_t)base );
	LLC_TEST_CHECKF(errors, result, actual != expected , "cpow<%u>(%f) mismatch. actual:%f, expected:%f." , (::llc::u2_t)exp, (::llc::f3_t)base, (::llc::f3_t)actual, (::llc::f3_t)expected );
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testCPowType(ATestError & errors) {
	stxp T compileTimeValue = ::llc::cpow<6>(T(2));
	LLC_TEST_CHECKF(errors, CPOW_TEST_RESULT_CONSTEXPR_VALUE, compileTimeValue != T(64) , "constexpr cpow<6>(2) mismatch. actual:%f, expected:64." , (::llc::f3_t)compileTimeValue );
	if_fail_fe(testCPowValue<0>(errors, T(0), T(1)));
	if_fail_fe(testCPowValue<1>(errors, T(0), T(0)));
	if_fail_fe(testCPowValue<6>(errors, T(1), T(1)));
	if_fail_fe(testCPowValue<1>(errors, T(2), T(2)));
	if_fail_fe(testCPowValue<3>(errors, T(2), T(8)));
	if_fail_fe(testCPowValue<6>(errors, T(2), T(64)));
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testCPowSignedType(ATestError & errors) {
	if_fail_fe(testCPowValue<1>(errors, T(-2), T(-2), CPOW_TEST_RESULT_NEGATIVE_BASE));
	if_fail_fe(testCPowValue<2>(errors, T(-2), T( 4), CPOW_TEST_RESULT_NEGATIVE_BASE));
	if_fail_fe(testCPowValue<3>(errors, T(-2), T(-8), CPOW_TEST_RESULT_NEGATIVE_BASE));
	if_fail_fe(testCPowValue<6>(errors, T(-2), T(64), CPOW_TEST_RESULT_NEGATIVE_BASE));
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testCPowFractionalType(ATestError & errors) {
	if_fail_fe(testCPowValue<1>(errors, T(.5), T(.5), CPOW_TEST_RESULT_FRACTIONAL_BASE));
	if_fail_fe(testCPowValue<2>(errors, T(.5), T(.25), CPOW_TEST_RESULT_FRACTIONAL_BASE));
	if_fail_fe(testCPowValue<4>(errors, T(.5), T(.0625), CPOW_TEST_RESULT_FRACTIONAL_BASE));
	rtrn 0;
}

tplt<::llc::u0_t exp, tpnm T>
sttc ::llc::err_t testDigitValue(ATestError & errors, T value, char expected) {
	cnst char actual = ::llc::digit<exp>(value);
	LLC_TEST_CHECKF(errors, CPOW_TEST_RESULT_DIGIT_POSITION, actual != expected , "digit<%u>(%" LLC_FMT_U3 ") mismatch. actual:'%c', expected:'%c'." , (::llc::u2_t)exp, (::llc::u3_t)value, actual, expected );
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testDigitType(ATestError & errors) {
	if_fail_fe(testDigitValue<0>(errors, T(123), '3'));
	if_fail_fe(testDigitValue<1>(errors, T(123), '2'));
	if_fail_fe(testDigitValue<2>(errors, T(123), '1'));
	if_fail_fe(testDigitValue<3>(errors, T(123), '0'));
	rtrn 0;
}

::llc::err_t testCPow(ATestError & errors) {
	if_fail_fe(testCPowType<::llc::i0u_t>(errors));
	if_fail_fe(testCPowType<::llc::i1u_t>(errors));
	if_fail_fe(testCPowType<::llc::i2u_t>(errors));
	if_fail_fe(testCPowType<::llc::i3u_t>(errors));
	if_fail_fe(testCPowType<::llc::i0s_t>(errors));
	if_fail_fe(testCPowType<::llc::i1s_t>(errors));
	if_fail_fe(testCPowType<::llc::i2s_t>(errors));
	if_fail_fe(testCPowType<::llc::i3s_t>(errors));
	if_fail_fe(testCPowType<::llc::f2s_t>(errors));
	if_fail_fe(testCPowType<::llc::f3s_t>(errors));

	if_fail_fe(testCPowSignedType<::llc::i0s_t>(errors));
	if_fail_fe(testCPowSignedType<::llc::i1s_t>(errors));
	if_fail_fe(testCPowSignedType<::llc::i2s_t>(errors));
	if_fail_fe(testCPowSignedType<::llc::i3s_t>(errors));
	if_fail_fe(testCPowSignedType<::llc::f2s_t>(errors));
	if_fail_fe(testCPowSignedType<::llc::f3s_t>(errors));
	if_fail_fe(testCPowFractionalType<::llc::f2s_t>(errors));
	if_fail_fe(testCPowFractionalType<::llc::f3s_t>(errors));

	LLC_TEST_CHECKF(errors, CPOW_TEST_RESULT_DIGIT_ASCII_DECIMAL, ::llc::digit_ascii(0) != '0' , "Decimal digit 0:'%c', expected:'0'.", ::llc::digit_ascii(0) );
	LLC_TEST_CHECKF(errors, CPOW_TEST_RESULT_DIGIT_ASCII_DECIMAL, ::llc::digit_ascii(9) != '9' , "Decimal digit 9:'%c', expected:'9'.", ::llc::digit_ascii(9) );
	LLC_TEST_CHECKF(errors, CPOW_TEST_RESULT_DIGIT_ASCII_LETTER, ::llc::digit_ascii(10) != 'A' , "Alphabetic digit 10:'%c', expected:'A'.", ::llc::digit_ascii(10) );
	LLC_TEST_CHECKF(errors, CPOW_TEST_RESULT_DIGIT_ASCII_LETTER, ::llc::digit_ascii(35) != 'Z' , "Alphabetic digit 35:'%c', expected:'Z'.", ::llc::digit_ascii(35) );
	LLC_TEST_CHECKF(errors, CPOW_TEST_RESULT_DIGIT_ASCII_LETTER, ::llc::digit_ascii(36) != 'a' , "Alphabetic digit 36:'%c', expected:'a'.", ::llc::digit_ascii(36) );
	LLC_TEST_CHECKF(errors, CPOW_TEST_RESULT_DIGIT_ASCII_LETTER, ::llc::digit_ascii(61) != 'z' , "Alphabetic digit 61:'%c', expected:'z'.", ::llc::digit_ascii(61) );
	LLC_TEST_CHECKF(errors, CPOW_TEST_RESULT_DIGIT_ASCII_BASE, ::llc::digit_ascii(255, 16) != 'F' , "Digit 255 in base 16:'%c', expected:'F'.", ::llc::digit_ascii(255, 16) );
	LLC_TEST_CHECKF(errors, CPOW_TEST_RESULT_DIGIT_ASCII_BASE, ::llc::digit_ascii(61, ::llc::ASCII_DIGIT_COUNT) != 'z' , "Digit 61 in base %u:'%c', expected:'z'.", ::llc::ASCII_DIGIT_COUNT, ::llc::digit_ascii(61, ::llc::ASCII_DIGIT_COUNT) );

	if_fail_fe(testDigitType<::llc::i0u_t>(errors));
	if_fail_fe(testDigitType<::llc::i1u_t>(errors));
	if_fail_fe(testDigitType<::llc::i2u_t>(errors));
	if_fail_fe(testDigitType<::llc::i3u_t>(errors));
	LLC_TEST_CHECKF(errors, CPOW_TEST_RESULT_DIGIT_POSITION, ::llc::digit<0>(::llc::u2_t(0xBEEF), ::llc::u2_t(16)) != 'F' , "Hex digit position 0:'%c', expected:'F'.", ::llc::digit<0>(::llc::u2_t(0xBEEF), ::llc::u2_t(16)) );
	LLC_TEST_CHECKF(errors, CPOW_TEST_RESULT_DIGIT_POSITION, ::llc::digit<1>(::llc::u2_t(0xBEEF), ::llc::u2_t(16)) != 'E' , "Hex digit position 1:'%c', expected:'E'.", ::llc::digit<1>(::llc::u2_t(0xBEEF), ::llc::u2_t(16)) );
	LLC_TEST_CHECKF(errors, CPOW_TEST_RESULT_DIGIT_POSITION, ::llc::digit<2>(::llc::u2_t(0xBEEF), ::llc::u2_t(16)) != 'E' , "Hex digit position 2:'%c', expected:'E'.", ::llc::digit<2>(::llc::u2_t(0xBEEF), ::llc::u2_t(16)) );
	LLC_TEST_CHECKF(errors, CPOW_TEST_RESULT_DIGIT_POSITION, ::llc::digit<3>(::llc::u2_t(0xBEEF), ::llc::u2_t(16)) != 'B' , "Hex digit position 3:'%c', expected:'B'.", ::llc::digit<3>(::llc::u2_t(0xBEEF), ::llc::u2_t(16)) );
	rtrn 0;
}
