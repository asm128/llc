#include "llc_test_core.h"
#include "llc_view.h"

GDEFINE_ENUM_TYPE(KEYVAL_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(KEYVAL_TEST_RESULT, FIRST_MATCH			, 0, "find() did not return the first matching key/value index and value.");
GDEFINE_ENUM_VALUED(KEYVAL_TEST_RESULT, SECOND_MATCH			, 1, "find() did not return a later matching key/value index and value.");
GDEFINE_ENUM_VALUED(KEYVAL_TEST_RESULT, MISSING_KEY			, 2, "find() did not reject a missing key and reset its output.");
GDEFINE_ENUM_VALUED(KEYVAL_TEST_RESULT, EMPTY_VIEW			, 3, "find() accepted a key in an empty key/value view.");
GDEFINE_ENUM_VALUED(KEYVAL_TEST_RESULT, SHALLOW_CONST_VIEW	, 4, "A const key/value record incorrectly made a mutable value view deep-const.");
GDEFINE_ENUM_VALUED(KEYVAL_TEST_RESULT, DEEP_CONST_VIEW		, 5, "find() did not preserve the element constness of a const value view.");
GDEFINE_ENUM_VALUED(KEYVAL_TEST_RESULT, LARGE_VIEW			, 6, "find() did not preserve an index beyond the eight-bit range.");

stct SKeyValTestKey {
	::llc::u2_t	Value	= {};
	LLC_DEFAULT_OPERATOR(SKeyValTestKey, Value == other.Value);
};

stct SKeyValTestValue {
	::llc::u2_t	Left	= {};
	::llc::u3_t	Right	= {};
	LLC_DEFAULT_OPERATOR(SKeyValTestValue, Left == other.Left && Right == other.Right);
};

tplt<tpnm TKey, tpnm TVal>
sttc ::llc::err_t testKeyValCombination
	(ATestError & errors, cnst TKey & firstKey, cnst TKey & secondKey, cnst TKey & missingKey, cnst TVal & firstValue, cnst TVal & secondValue) {
	cnst ::llc::keyval<TKey, TVal> values[] = {{firstKey, firstValue}, {secondKey, secondValue}, {firstKey, secondValue}};
	cnst ::llc::view<cnst ::llc::keyval<TKey, TVal>> keyvals = {values};
	llc_rmcnst(TVal) output = {};
	cnst ::llc::err_t first = ::llc::find(firstKey, keyvals, output);
	LLC_TEST_CHECK(errors, KEYVAL_TEST_RESULT_FIRST_MATCH, first
		, "First lookup index:%i, expected:0. Key bytes:%u, value bytes:%u."
		, first, (::llc::u2_t)sizeof(TKey), (::llc::u2_t)sizeof(TVal)
		);
	LLC_TEST_CHECK(errors, KEYVAL_TEST_RESULT_FIRST_MATCH, false == (output == firstValue)
		, "First lookup returned the wrong value. Key bytes:%u, value bytes:%u."
		, (::llc::u2_t)sizeof(TKey), (::llc::u2_t)sizeof(TVal)
		);
	cnst ::llc::err_t second = ::llc::find(secondKey, keyvals, output);
	LLC_TEST_CHECK(errors, KEYVAL_TEST_RESULT_SECOND_MATCH, second != 1
		, "Second lookup index:%i, expected:1. Key bytes:%u, value bytes:%u."
		, second, (::llc::u2_t)sizeof(TKey), (::llc::u2_t)sizeof(TVal)
		);
	LLC_TEST_CHECK(errors, KEYVAL_TEST_RESULT_SECOND_MATCH, false == (output == secondValue)
		, "Second lookup returned the wrong value. Key bytes:%u, value bytes:%u."
		, (::llc::u2_t)sizeof(TKey), (::llc::u2_t)sizeof(TVal)
		);
	cnst ::llc::err_t missing = ::llc::find(missingKey, keyvals, output);
	LLC_TEST_CHECK(errors, KEYVAL_TEST_RESULT_MISSING_KEY, missing != -1
		, "Missing lookup index:%i, expected:-1. Key bytes:%u, value bytes:%u."
		, missing, (::llc::u2_t)sizeof(TKey), (::llc::u2_t)sizeof(TVal)
		);
	LLC_TEST_CHECK(errors, KEYVAL_TEST_RESULT_MISSING_KEY, false == (output == TVal{})
		, "Missing lookup did not reset its value. Key bytes:%u, value bytes:%u."
		, (::llc::u2_t)sizeof(TKey), (::llc::u2_t)sizeof(TVal)
		);
	rtrn 0;
}

sttc ::llc::err_t testKeyValTypes(ATestError & errors) {
	if_fail_fe(::testKeyValCombination(errors, ::llc::u0_t{1}, ::llc::u0_t{2}, ::llc::u0_t{3}, ::llc::u3_t{11}, ::llc::u3_t{22}));
	if_fail_fe(::testKeyValCombination(errors, ::llc::s3_t{-7}, ::llc::s3_t{9}, ::llc::s3_t{0}, LLC_CXS("negative"), LLC_CXS("positive")));
	if_fail_fe(::testKeyValCombination(errors, LLC_CXS("alpha"), LLC_CXS("beta"), LLC_CXS("missing")
		, SKeyValTestValue{1, 11}, SKeyValTestValue{2, 22}
		));
	if_fail_fe(::testKeyValCombination(errors, SKeyValTestKey{0x1234}, SKeyValTestKey{0x5678}, SKeyValTestKey{0x9ABC}, ::llc::u0_t{1}, ::llc::u0_t{2}));
	rtrn 0;
}

sttc ::llc::err_t testKeyValConstTypes(ATestError & errors) {
	if_fail_fe((::testKeyValCombination<     ::llc::u2_t,      ::llc::u3_t>(errors, 1, 2, 3, 11, 22)));
	if_fail_fe((::testKeyValCombination<cnst ::llc::u2_t,      ::llc::u3_t>(errors, 1, 2, 3, 11, 22)));
	if_fail_fe((::testKeyValCombination<     ::llc::u2_t, cnst ::llc::u3_t>(errors, 1, 2, 3, 11, 22)));
	if_fail_fe((::testKeyValCombination<cnst ::llc::u2_t, cnst ::llc::u3_t>(errors, 1, 2, 3, 11, 22)));
	rtrn 0;
}

sttc ::llc::err_t testKeyValViewConstness(ATestError & errors) {
	cnst ::llc::u1_t keyData[] = {1, 2, 3};
	::llc::u1_t mutableData[] = {4, 5, 6};
	tydf ::llc::keyval<cnst ::llc::view<cnst ::llc::u1_t>, cnst ::llc::view<::llc::u1_t>> TMutableViewKeyVal;
	cnst TMutableViewKeyVal mutableValue[] = {{{keyData}, {mutableData}}};
	::llc::view<::llc::u1_t> mutableOutput = {};
	cnst ::llc::err_t mutableFound = ::llc::find(::llc::view<cnst ::llc::u1_t>{keyData}, ::llc::view<cnst TMutableViewKeyVal>{mutableValue}, mutableOutput);
	if(0 <= mutableFound && 1 < mutableOutput.size())
		mutableOutput[1] = 0x55;
	LLC_TEST_CHECK(errors, KEYVAL_TEST_RESULT_SHALLOW_CONST_VIEW, mutableFound
		, "Shallow-const lookup index:%i, expected:0.", mutableFound
		);
	LLC_TEST_CHECK(errors, KEYVAL_TEST_RESULT_SHALLOW_CONST_VIEW, mutableOutput.begin() != mutableData
		, "Shallow-const output:%p, expected:%p.", mutableOutput.begin(), mutableData
		);
	LLC_TEST_CHECK(errors, KEYVAL_TEST_RESULT_SHALLOW_CONST_VIEW, mutableOutput.size() != ::llc::size(mutableData)
		, "Shallow-const count:%u, expected:%u.", mutableOutput.size(), ::llc::u2_t(::llc::size(mutableData))
		);
	LLC_TEST_CHECK(errors, KEYVAL_TEST_RESULT_SHALLOW_CONST_VIEW, mutableData[1] != 0x55
		, "Shallow-const source element 1:%u, expected:85.", mutableData[1]
		);

	cnst ::llc::u1_t constData[] = {7, 8, 9};
	tydf ::llc::keyval<cnst ::llc::view<cnst ::llc::u1_t>, ::llc::view<cnst ::llc::u1_t>> TConstViewKeyVal;
	cnst TConstViewKeyVal constValue[] = {{{keyData}, {constData}}};
	::llc::view<cnst ::llc::u1_t> constOutput = {};
	cnst ::llc::err_t constFound = ::llc::find(::llc::view<cnst ::llc::u1_t>{keyData}, ::llc::view<cnst TConstViewKeyVal>{constValue}, constOutput);
	LLC_TEST_CHECK(errors, KEYVAL_TEST_RESULT_DEEP_CONST_VIEW, constFound
		, "Deep-const lookup index:%i, expected:0.", constFound
		);
	LLC_TEST_CHECK(errors, KEYVAL_TEST_RESULT_DEEP_CONST_VIEW, constOutput.begin() != constData
		, "Deep-const output:%p, expected:%p.", constOutput.begin(), constData
		);
	LLC_TEST_CHECK(errors, KEYVAL_TEST_RESULT_DEEP_CONST_VIEW, constOutput.size() != ::llc::size(constData)
		, "Deep-const count:%u, expected:%u.", constOutput.size(), ::llc::u2_t(::llc::size(constData))
		);
	if(2 < constOutput.size()) {
		LLC_TEST_CHECK(errors, KEYVAL_TEST_RESULT_DEEP_CONST_VIEW, constOutput[2] != 9
			, "Deep-const element 2:%u, expected:9.", constOutput[2]
			);
	}
	rtrn 0;
}

sttc ::llc::err_t testKeyValBoundaries(ATestError & errors) {
	cnst ::llc::view<cnst ::llc::keyval<::llc::u2_t, ::llc::u3_t>> empty = {};
	::llc::u3_t output = 1;
	cnst ::llc::err_t emptyResult = ::llc::find(::llc::u2_t{0}, empty, output);
	LLC_TEST_CHECK(errors, KEYVAL_TEST_RESULT_EMPTY_VIEW, emptyResult != -1
		, "Empty lookup index:%i, expected:-1.", emptyResult
		);
	LLC_TEST_CHECK(errors, KEYVAL_TEST_RESULT_EMPTY_VIEW, output
		, "Empty lookup output:%" LLC_FMT_U3 ", expected:0.", output
		);
	::llc::keyval<::llc::u2_t, ::llc::u3_t> values[513] = {};
	for(::llc::u2_t iValue = 0; iValue < ::llc::size(values); ++iValue)
		values[iValue] = {iValue, (::llc::u3_t)iValue * iValue};
	cnst ::llc::err_t found = ::llc::find(::llc::u2_t{512}, ::llc::view<cnst ::llc::keyval<::llc::u2_t, ::llc::u3_t>>{values}, output);
	LLC_TEST_CHECK(errors, KEYVAL_TEST_RESULT_LARGE_VIEW, found != 512
		, "Large lookup index:%i, expected:512.", found
		);
	LLC_TEST_CHECK(errors, KEYVAL_TEST_RESULT_LARGE_VIEW, output != 512ULL * 512
		, "Large lookup output:%" LLC_FMT_U3 ", expected:262144.", output
		);
	rtrn 0;
}

::llc::err_t testKeyVal(ATestError & errors) {
	if_fail_fe(::testKeyValTypes(errors));
	if_fail_fe(::testKeyValConstTypes(errors));
	if_fail_fe(::testKeyValViewConstness(errors));
	rtrn ::testKeyValBoundaries(errors);
}
