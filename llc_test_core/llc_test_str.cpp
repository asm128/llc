#include "llc_array_pod.h"
#include "llc_array_static.h"

#include "llc_test_core.h"

#include <type_traits>

GDEFINE_ENUM_TYPE(STR_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, OK						, 0, "All str() adapter tests passed.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, MUTABLE_ARRAY_TYPE		, 1, "str() did not preserve mutable character-array access.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CONST_ARRAY_TYPE			, 2, "str() did not preserve const character-array access.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, ARRAY_RANGE				, 3, "str() did not expose the character-array text range.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, ARRAY_MUTATION			, 4, "A mutable character-array string view did not update its source.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, MUTABLE_VIEW_TYPE			, 5, "str() did not adapt a mutable character view to view_string.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CONST_VIEW_TYPE			, 6, "str() did not adapt a const character view to view_const_string.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, COUNTED_VIEW_RANGE		, 7, "str() did not preserve an explicitly counted character range.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, STRING_VIEW_TYPE			, 8, "str() did not preserve string-view mutability.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, STRING_VIEW_RANGE			, 9, "str() changed an existing string-view range.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, MUTABLE_STATIC_TYPE		, 10, "str() did not adapt mutable static character storage to view_string.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CONST_STATIC_TYPE		, 11, "str() did not adapt const static character storage to view_const_string.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, STATIC_RANGE				, 12, "str() did not expose the text stored in a static character array.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, MUTABLE_POD_TYPE			, 13, "str() did not adapt mutable POD character storage to view_string.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CONST_POD_TYPE			, 14, "str() did not adapt const POD character storage to view_const_string.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, POD_RANGE				, 15, "str() changed the counted POD character range.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, POD_MUTATION				, 16, "A mutable POD string view did not update its source.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, EMPTY_STRING				, 17, "str() did not produce a safe readable empty string.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, BOOL_TEXT					, 18, "str() did not expose the expected boolean text.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, NUMERIC_RESULT_TYPE		, 19, "Numeric str() returned an unexpected storage type.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, NUMERIC_TEXT				, 20, "Numeric str() produced unexpected text.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, NUMERIC_TERMINATION		, 21, "Numeric str() did not terminate its text within its static storage.");

tplt<tpnm TString>
sttc bool stringMismatch(cnst TString & value, ::llc::vcst_t expected) {
	rtrn value.size() != expected.size() || (value.size() && 0 != memcmp(value.begin(), expected.begin(), value.size()));
}

tplt<tpnm T, ::llc::u2_t N>
sttc ::llc::err_t testNumericStrValue(ATestError & errors, T value, ::llc::vcst_t expected) {
	auto				storage	= ::llc::str(value);
	auto				text	= ::llc::str(storage);
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_NUMERIC_RESULT_TYPE, false == (::std::is_same_v<decltype(storage), ::llc::astchar<N>>)
		, "%s result type mismatch. capacity:%u, expected:%u."
		, ::llc::get_type_namep<T>(), storage.size(), N
		);
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_NUMERIC_TEXT, stringMismatch(text, expected) || text.begin() != storage.begin()
		, "%s text mismatch. actual:'%.*s'/%u at:%p, expected:'%.*s'/%u at:%p."
		, ::llc::get_type_namep<T>(), (int)text.size(), text.begin(), text.size(), (cnst void*)text.begin(), (int)expected.size(), expected.begin(), expected.size(), (cnst void*)storage.begin()
		);
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_NUMERIC_TERMINATION, text.size() >= storage.size() || storage.Storage[text.size()]
		, "%s termination mismatch. text size:%u, capacity:%u, terminator:%i."
		, ::llc::get_type_namep<T>(), text.size(), storage.size(), text.size() < storage.size() ? storage.Storage[text.size()] : -1
		);
	rtrn 0;
}

sttc ::llc::err_t testNumericStr(ATestError & errors) {
	if_fail_fe((testNumericStrValue<::llc::u0_t,   5>(errors,   0, LLC_CXS("0"))));
	if_fail_fe((testNumericStrValue<::llc::u0_t,   5>(errors,   9, LLC_CXS("9"))));
	if_fail_fe((testNumericStrValue<::llc::u0_t,   5>(errors,  10, LLC_CXS("10"))));
	if_fail_fe((testNumericStrValue<::llc::u0_t,   5>(errors,  99, LLC_CXS("99"))));
	if_fail_fe((testNumericStrValue<::llc::u0_t,   5>(errors, 100, LLC_CXS("100"))));
	if_fail_fe((testNumericStrValue<::llc::u0_t,   5>(errors, 255, LLC_CXS("255"))));

	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors,    0, LLC_CXS("0"))));
	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors,    1, LLC_CXS("1"))));
	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors,   -1, LLC_CXS("-1"))));
	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors,    9, LLC_CXS("9"))));
	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors,   -9, LLC_CXS("-9"))));
	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors,   10, LLC_CXS("10"))));
	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors,  -10, LLC_CXS("-10"))));
	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors,   99, LLC_CXS("99"))));
	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors,  -99, LLC_CXS("-99"))));
	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors,  100, LLC_CXS("100"))));
	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors, -100, LLC_CXS("-100"))));
	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors,  127, LLC_CXS("127"))));
	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors, -128, LLC_CXS("-128"))));

	if_fail_fe((testNumericStrValue<::llc::u1_t,   7>(errors, ::llc::u1_t(    0), LLC_CXS("0"))));
	if_fail_fe((testNumericStrValue<::llc::u1_t,   7>(errors, ::llc::u1_t(   10), LLC_CXS("10"))));
	if_fail_fe((testNumericStrValue<::llc::u1_t,   7>(errors, ::llc::u1_t(65535), LLC_CXS("65535"))));
	if_fail_fe((testNumericStrValue<::llc::s1_t,   8>(errors, ::llc::s1_t(     0), LLC_CXS("0"))));
	if_fail_fe((testNumericStrValue<::llc::s1_t,   8>(errors, ::llc::s1_t(    -1), LLC_CXS("-1"))));
	if_fail_fe((testNumericStrValue<::llc::s1_t,   8>(errors, ::llc::s1_t( 32767), LLC_CXS("32767"))));
	if_fail_fe((testNumericStrValue<::llc::s1_t,   8>(errors, ::llc::s1_t(-32768), LLC_CXS("-32768"))));

	if_fail_fe((testNumericStrValue<::llc::u2_t,  12>(errors, ::llc::u2_t(         0U), LLC_CXS("0"))));
	if_fail_fe((testNumericStrValue<::llc::u2_t,  12>(errors, ::llc::u2_t(        10U), LLC_CXS("10"))));
	if_fail_fe((testNumericStrValue<::llc::u2_t,  12>(errors, ::llc::u2_t(4294967295U), LLC_CXS("4294967295"))));
	if_fail_fe((testNumericStrValue<::llc::s2_t,  13>(errors, ::llc::s2_t(          0), LLC_CXS("0"))));
	if_fail_fe((testNumericStrValue<::llc::s2_t,  13>(errors, ::llc::s2_t(         -1), LLC_CXS("-1"))));
	if_fail_fe((testNumericStrValue<::llc::s2_t,  13>(errors, ::llc::s2_t( 2147483647), LLC_CXS("2147483647"))));
	if_fail_fe((testNumericStrValue<::llc::s2_t,  13>(errors, ::llc::s2_t(-2147483647 - 1), LLC_CXS("-2147483648"))));

	if_fail_fe((testNumericStrValue<::llc::u3_t,  22>(errors, ::llc::u3_t(                   0ULL), LLC_CXS("0"))));
	if_fail_fe((testNumericStrValue<::llc::u3_t,  22>(errors, ::llc::u3_t(                  10ULL), LLC_CXS("10"))));
	if_fail_fe((testNumericStrValue<::llc::u3_t,  22>(errors, ::llc::u3_t(18446744073709551615ULL), LLC_CXS("18446744073709551615"))));
	if_fail_fe((testNumericStrValue<::llc::s3_t,  22>(errors, ::llc::s3_t(                   0LL), LLC_CXS("0"))));
	if_fail_fe((testNumericStrValue<::llc::s3_t,  22>(errors, ::llc::s3_t(                  -1LL), LLC_CXS("-1"))));
	if_fail_fe((testNumericStrValue<::llc::s3_t,  22>(errors, ::llc::s3_t( 9223372036854775807LL), LLC_CXS("9223372036854775807"))));
	if_fail_fe((testNumericStrValue<::llc::s3_t,  22>(errors, ::llc::s3_t(-9223372036854775807LL - 1), LLC_CXS("-9223372036854775808"))));

	if_fail_fe((testNumericStrValue<::llc::f2_t,  64>(errors, ::llc::f2_t(       0.0), LLC_CXS("0.000000"))));
	if_fail_fe((testNumericStrValue<::llc::f2_t,  64>(errors, ::llc::f2_t(      1.25), LLC_CXS("1.250000"))));
	if_fail_fe((testNumericStrValue<::llc::f2_t,  64>(errors, ::llc::f2_t(      -2.5), LLC_CXS("-2.500000"))));
	if_fail_fe((testNumericStrValue<::llc::f2_t,  64>(errors, ::llc::f2_t(  123456.5), LLC_CXS("123456.500000"))));
	if_fail_fe((testNumericStrValue<::llc::f3_t, 384>(errors, ::llc::f3_t(         0.0), LLC_CXS("0.000000"))));
	if_fail_fe((testNumericStrValue<::llc::f3_t, 384>(errors, ::llc::f3_t(       1.125), LLC_CXS("1.125000"))));
	if_fail_fe((testNumericStrValue<::llc::f3_t, 384>(errors, ::llc::f3_t(       -2.25), LLC_CXS("-2.250000"))));
	if_fail_fe((testNumericStrValue<::llc::f3_t, 384>(errors, ::llc::f3_t(123456789.125), LLC_CXS("123456789.125000"))));
	rtrn 0;
}

::llc::err_t testStr(ATestError & errors) {
	::llc::sc_t		mutableArray[]		= "alpha";
	::llc::sc_c		constArray[]		= "beta";
	auto				mutableArrayText	= ::llc::str(mutableArray);
	auto				constArrayText		= ::llc::str(constArray);
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_MUTABLE_ARRAY_TYPE, false == (::std::is_same_v<decltype(mutableArrayText), ::llc::vs>)
		, "Mutable array result type mismatch. size:%u."
		, mutableArrayText.size()
		);
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_CONST_ARRAY_TYPE, false == (::std::is_same_v<decltype(constArrayText), ::llc::vcst_t>)
		, "Const array result type mismatch. size:%u."
		, constArrayText.size()
		);
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_ARRAY_RANGE, stringMismatch(mutableArrayText, LLC_CXS("alpha")) || mutableArrayText.begin() != mutableArray
		, "Mutable array range mismatch. size:%u, expected:5, begin:%p, expected begin:%p."
		, mutableArrayText.size(), (cnst void*)mutableArrayText.begin(), (cnst void*)mutableArray
		);
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_ARRAY_RANGE, stringMismatch(constArrayText, LLC_CXS("beta")) || constArrayText.begin() != constArray
		, "Const array range mismatch. size:%u, expected:4, begin:%p, expected begin:%p."
		, constArrayText.size(), (cnst void*)constArrayText.begin(), (cnst void*)constArray
		);
	mutableArrayText[0] = 'A';
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_ARRAY_MUTATION, mutableArray[0] != 'A'
		, "Mutable array source was not updated. source[0]:%c, expected:A."
		, mutableArray[0]
		);

	::llc::sc_t		countedStorage[]	= {'c', 'o', 0, 'u', 'n', 't'};
	::llc::vsc_t	countedView			= {countedStorage, ::llc::size(countedStorage)};
	::llc::vcsc_t	countedConstView		= {countedStorage, ::llc::size(countedStorage)};
	auto				countedText			= ::llc::str(countedView);
	auto				countedConstText	= ::llc::str(countedConstView);
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_MUTABLE_VIEW_TYPE, false == (::std::is_same_v<decltype(countedText), ::llc::vs>)
		, "Mutable view result type mismatch. size:%u."
		, countedText.size()
		);
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_CONST_VIEW_TYPE, false == (::std::is_same_v<decltype(countedConstText), ::llc::vcst_t>)
		, "Const view result type mismatch. size:%u."
		, countedConstText.size()
		);
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_COUNTED_VIEW_RANGE
		, countedText.size() != ::llc::size(countedStorage) || countedText.begin() != countedStorage || countedConstText.size() != ::llc::size(countedStorage) || countedConstText.begin() != countedStorage
		, "Counted range mismatch. mutable size:%u, const size:%u, expected:%u, mutable begin:%p, const begin:%p, expected begin:%p."
		, countedText.size(), countedConstText.size(), (::llc::u2_t)::llc::size(countedStorage), (cnst void*)countedText.begin(), (cnst void*)countedConstText.begin(), (cnst void*)countedStorage
		);
	countedText[3] = 'U';
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_ARRAY_MUTATION, countedStorage[3] != 'U'
		, "Counted mutable source was not updated. source[3]:%c, expected:U."
		, countedStorage[3]
		);

	::llc::vs		mutableString		= mutableArray;
	cnst ::llc::vs & constMutableString	= mutableString;
	::llc::vcst_t	constString			= constArray;
	auto				mutableStringText	= ::llc::str(mutableString);
	auto				constMutableText	= ::llc::str(constMutableString);
	auto				constStringText		= ::llc::str(constString);
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_STRING_VIEW_TYPE
		, false == (::std::is_same_v<decltype(mutableStringText), ::llc::vs>) || false == (::std::is_same_v<decltype(constMutableText), ::llc::vcst_t>) || false == (::std::is_same_v<decltype(constStringText), ::llc::vcst_t>)
		, "String-view result type mismatch. mutable size:%u, const mutable size:%u, const size:%u."
		, mutableStringText.size(), constMutableText.size(), constStringText.size()
		);
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_STRING_VIEW_RANGE
		, mutableStringText.begin() != mutableString.begin() || mutableStringText.size() != mutableString.size() || constMutableText.begin() != mutableString.begin() || constMutableText.size() != mutableString.size() || constStringText.begin() != constString.begin() || constStringText.size() != constString.size()
		, "String-view range mismatch. mutable:%p/%u, const mutable:%p/%u, const:%p/%u."
		, (cnst void*)mutableStringText.begin(), mutableStringText.size(), (cnst void*)constMutableText.begin(), constMutableText.size(), (cnst void*)constStringText.begin(), constStringText.size()
		);

	::llc::astchar<8>	staticText		= {'s', 't', 'a', 't', 'i', 'c', 0};
	cnst ::llc::astchar<8> & constStaticText	= staticText;
	auto				mutableStaticView	= ::llc::str(staticText);
	auto				constStaticView		= ::llc::str(constStaticText);
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_MUTABLE_STATIC_TYPE, false == (::std::is_same_v<decltype(mutableStaticView), ::llc::vs>)
		, "Mutable static result type mismatch. size:%u."
		, mutableStaticView.size()
		);
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_CONST_STATIC_TYPE, false == (::std::is_same_v<decltype(constStaticView), ::llc::vcst_t>)
		, "Const static result type mismatch. size:%u."
		, constStaticView.size()
		);
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_STATIC_RANGE
		, stringMismatch(mutableStaticView, LLC_CXS("static")) || mutableStaticView.begin() != staticText.begin() || stringMismatch(constStaticView, LLC_CXS("static")) || constStaticView.begin() != staticText.begin()
		, "Static range mismatch. mutable:%p/%u, const:%p/%u, expected begin:%p."
		, (cnst void*)mutableStaticView.begin(), mutableStaticView.size(), (cnst void*)constStaticView.begin(), constStaticView.size(), (cnst void*)staticText.begin()
		);

	::llc::asc_t	dynamicText		= {'p', 'o', 'd'};
	cnst ::llc::asc_t & constDynamicText	= dynamicText;
	auto				mutableDynamicView	= ::llc::str(dynamicText);
	auto				constDynamicView	= ::llc::str(constDynamicText);
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_MUTABLE_POD_TYPE, false == (::std::is_same_v<decltype(mutableDynamicView), ::llc::vs>)
		, "Mutable POD result type mismatch. size:%u."
		, mutableDynamicView.size()
		);
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_CONST_POD_TYPE, false == (::std::is_same_v<decltype(constDynamicView), ::llc::vcst_t>)
		, "Const POD result type mismatch. size:%u."
		, constDynamicView.size()
		);
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_POD_RANGE
		, stringMismatch(mutableDynamicView, LLC_CXS("pod")) || mutableDynamicView.begin() != dynamicText.begin() || stringMismatch(constDynamicView, LLC_CXS("pod")) || constDynamicView.begin() != dynamicText.begin()
		, "POD range mismatch. mutable:%p/%u, const:%p/%u, expected begin:%p."
		, (cnst void*)mutableDynamicView.begin(), mutableDynamicView.size(), (cnst void*)constDynamicView.begin(), constDynamicView.size(), (cnst void*)dynamicText.begin()
		);
	mutableDynamicView[0] = 'P';
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_POD_MUTATION, dynamicText[0] != 'P'
		, "Mutable POD source was not updated. source[0]:%c, expected:P."
		, dynamicText[0]
		);

	cnst ::llc::asc_t	emptyDynamic;
	::llc::sc_t			emptyArray[] = "";
	auto					emptyDynamicText	= ::llc::str(emptyDynamic);
	auto					emptyArrayText		= ::llc::str(emptyArray);
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_EMPTY_STRING
		, emptyDynamicText.size() || 0 == emptyDynamicText.begin() || emptyDynamicText.begin()[0] || emptyArrayText.size() || 0 == emptyArrayText.begin() || emptyArrayText.begin()[0]
		, "Empty string mismatch. POD:%p/%u/'%c', array:%p/%u/'%c'."
		, (cnst void*)emptyDynamicText.begin(), emptyDynamicText.size(), emptyDynamicText.begin()[0], (cnst void*)emptyArrayText.begin(), emptyArrayText.size(), emptyArrayText.begin()[0]
		);

	auto trueText	= ::llc::str(true);
	auto falseText	= ::llc::str(false);
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_BOOL_TEXT, stringMismatch(trueText, LLC_CXS("true")) || stringMismatch(falseText, LLC_CXS("false"))
		, "Boolean text mismatch. true:'%.*s', false:'%.*s'."
		, (int)trueText.size(), trueText.begin(), (int)falseText.size(), falseText.begin()
		);
	if_fail_fe(testNumericStr(errors));
	rtrn 0;
}
