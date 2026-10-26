#include "llc_test_core_suites.h"
#include "llc_apod_n2.h"
#include "llc_apod_serialize.h"

GDEFINE_ENUM_TYPE(TEST_APOD_N2_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_APOD_N2_RESULT, ALIASES          ,  0, "Every mutable n2 array alias preserves its declared element type.");
GDEFINE_ENUM_VALUED(TEST_APOD_N2_RESULT, CONST_ALIASES    ,  1, "Every const n2 array alias preserves the matching const array type.");
GDEFINE_ENUM_VALUED(TEST_APOD_N2_RESULT, EMPTY            ,  2, "An n2 array begins empty.");
GDEFINE_ENUM_VALUED(TEST_APOD_N2_RESULT, INTEGER_VALUES   ,  3, "Integer n2 elements retain both coordinates.");
GDEFINE_ENUM_VALUED(TEST_APOD_N2_RESULT, FLOAT_VALUES     ,  4, "Floating-point n2 elements retain both coordinates.");
GDEFINE_ENUM_VALUED(TEST_APOD_N2_RESULT, COPY             ,  5, "Copying an n2 array duplicates storage and values.");
GDEFINE_ENUM_VALUED(TEST_APOD_N2_RESULT, MOVE             ,  6, "Moving an n2 array transfers storage and clears the source.");
GDEFINE_ENUM_VALUED(TEST_APOD_N2_RESULT, SELF_ASSIGNMENT  ,  7, "Self-assignment preserves n2-array storage and values.");
GDEFINE_ENUM_VALUED(TEST_APOD_N2_RESULT, RESERVE_REJECTION,  8, "Rejecting an impossible reserve preserves the existing n2 array.");
GDEFINE_ENUM_VALUED(TEST_APOD_N2_RESULT, BOUNDARIES       ,  9, "Removing the first and final vectors preserves the middle vector.");
GDEFINE_ENUM_VALUED(TEST_APOD_N2_RESULT, VIEW_ALIASING    , 10, "An n2 view aliases the array's live storage.");
GDEFINE_ENUM_VALUED(TEST_APOD_N2_RESULT, SERIALIZATION    , 11, "An n2 array round-trips through the canonical view serializer.");

sttc ::llc::err_t testAPodN2Aliases(::llc::ATestError & results) {
	::llc::apod<::llc::n2<::llc::sc_t>> * asc = (::llc::an2sc_t*)0;
	::llc::apod<::llc::n2<::llc::uc_t>> * auc = (::llc::an2uc_t*)0;
	::llc::apod<::llc::n2<::llc::u0_t>> * au0 = (::llc::an2u0_t*)0;
	::llc::apod<::llc::n2<::llc::u1_t>> * au1 = (::llc::an2u1_t*)0;
	::llc::apod<::llc::n2<::llc::u2_t>> * au2 = (::llc::an2u2_t*)0;
	::llc::apod<::llc::n2<::llc::u3_t>> * au3 = (::llc::an2u3_t*)0;
	::llc::apod<::llc::n2<::llc::s0_t>> * as0 = (::llc::an2s0_t*)0;
	::llc::apod<::llc::n2<::llc::s1_t>> * as1 = (::llc::an2s1_t*)0;
	::llc::apod<::llc::n2<::llc::s2_t>> * as2 = (::llc::an2s2_t*)0;
	::llc::apod<::llc::n2<::llc::s3_t>> * as3 = (::llc::an2s3_t*)0;
	::llc::apod<::llc::n2<::llc::f2_t>> * af2 = (::llc::an2f2_t*)0;
	::llc::apod<::llc::n2<::llc::f3_t>> * af3 = (::llc::an2f3_t*)0;
	LLC_TEST_CHECK(results, TEST_APOD_N2_RESULT_ALIASES, asc || auc || au0 || au1 || au2 || au3 || as0 || as1 || as2 || as3 || af2 || af3);
	rtrn 0;
}

sttc ::llc::err_t testAPodN2ConstAliases(::llc::ATestError & results) {
	cnst ::llc::an2<::llc::sc_t> * asc = (::llc::an2sc_c*)0;
	cnst ::llc::an2<::llc::uc_t> * auc = (::llc::an2uc_c*)0;
	cnst ::llc::an2<::llc::u0_t> * au0 = (::llc::an2u0_c*)0;
	cnst ::llc::an2<::llc::u1_t> * au1 = (::llc::an2u1_c*)0;
	cnst ::llc::an2<::llc::u2_t> * au2 = (::llc::an2u2_c*)0;
	cnst ::llc::an2<::llc::u3_t> * au3 = (::llc::an2u3_c*)0;
	cnst ::llc::an2<::llc::s0_t> * as0 = (::llc::an2s0_c*)0;
	cnst ::llc::an2<::llc::s1_t> * as1 = (::llc::an2s1_c*)0;
	cnst ::llc::an2<::llc::s2_t> * as2 = (::llc::an2s2_c*)0;
	cnst ::llc::an2<::llc::s3_t> * as3 = (::llc::an2s3_c*)0;
	cnst ::llc::an2<::llc::f2_t> * af2 = (::llc::an2f2_c*)0;
	cnst ::llc::an2<::llc::f3_t> * af3 = (::llc::an2f3_c*)0;
	LLC_TEST_CHECK(results, TEST_APOD_N2_RESULT_CONST_ALIASES, asc || auc || au0 || au1 || au2 || au3 || as0 || as1 || as2 || as3 || af2 || af3);
	rtrn 0;
}

sttc ::llc::err_t testAPodN2Empty(::llc::ATestError & results) {
	::llc::an2u2_t values;
	LLC_TEST_CHECK(results, TEST_APOD_N2_RESULT_EMPTY, values.size() || values.begin());
	rtrn 0;
}

sttc ::llc::err_t testAPodN2IntegerValues(::llc::ATestError & results) {
	::llc::an2u2_t values;
	LLC_TEST_REQUIRE(results, TEST_APOD_N2_RESULT_INTEGER_VALUES, 0 != values.push_back({1, 3}));
	LLC_TEST_REQUIRE(results, TEST_APOD_N2_RESULT_INTEGER_VALUES, 1 != values.push_back({10, 20}));
	LLC_TEST_CHECK(results, TEST_APOD_N2_RESULT_INTEGER_VALUES, values[0].x != 1 || values[0].y != 3 || values[1].x != 10 || values[1].y != 20);
	rtrn 0;
}

sttc ::llc::err_t testAPodN2FloatValues(::llc::ATestError & results) {
	::llc::an2f2_t values;
	LLC_TEST_REQUIRE(results, TEST_APOD_N2_RESULT_FLOAT_VALUES, 0 != values.push_back({0.25f, 0.75f}));
	LLC_TEST_REQUIRE(results, TEST_APOD_N2_RESULT_FLOAT_VALUES, 1 != values.push_back({-1.5f, 2.5f}));
	LLC_TEST_CHECK(results, TEST_APOD_N2_RESULT_FLOAT_VALUES, values[0].x != 0.25f || values[0].y != 0.75f || values[1].x != -1.5f || values[1].y != 2.5f);
	rtrn 0;
}

sttc ::llc::err_t testAPodN2Copy(::llc::ATestError & results) {
	::llc::an2u2_t source = {{1, 3}, {10, 20}};
	::llc::an2u2_t copied = source;
	LLC_TEST_CHECK(results, TEST_APOD_N2_RESULT_COPY, copied.size() != source.size() || copied.begin() == source.begin() || 0 != memcmp(copied.begin(), source.begin(), source.byte_count()));
	rtrn 0;
}

sttc ::llc::err_t testAPodN2Move(::llc::ATestError & results) {
	::llc::an2u2_t source = {{1, 3}, {10, 20}};
	::llc::n2<::llc::u2_t> * sourceData = source.begin();
	::llc::an2u2_t moved = (::llc::an2u2_t&&)source;
	LLC_TEST_CHECK(results, TEST_APOD_N2_RESULT_MOVE, source.size() || source.begin() || moved.size() != 2 || moved.begin() != sourceData);
	rtrn 0;
}

sttc ::llc::err_t testAPodN2SelfAssignment(::llc::ATestError & results) {
	::llc::an2u2_t values = {{1, 3}, {10, 20}};
	::llc::n2<::llc::u2_t> * originalData = values.begin();
	values = values;
	LLC_TEST_CHECK(results, TEST_APOD_N2_RESULT_SELF_ASSIGNMENT, values.size() != 2 || values.begin() != originalData || values[0].x != 1 || values[1].y != 20);
	rtrn 0;
}

sttc ::llc::err_t testAPodN2ReserveRejection(::llc::ATestError & results) {
	::llc::an2u2_t values = {{1, 3}};
	::llc::n2<::llc::u2_t> * originalData = values.begin();
	LLC_TEST_CHECK(results, TEST_APOD_N2_RESULT_RESERVE_REJECTION, 0 <= values.reserve((::llc::u2_t)-1));
	LLC_TEST_CHECK(results, TEST_APOD_N2_RESULT_RESERVE_REJECTION, values.size() != 1 || values.begin() != originalData || values[0].x != 1 || values[0].y != 3);
	rtrn 0;
}

sttc ::llc::err_t testAPodN2Boundaries(::llc::ATestError & results) {
	::llc::an2u2_t values = {{1, 3}, {10, 20}, {30, 40}};
	LLC_TEST_REQUIRE(results, TEST_APOD_N2_RESULT_BOUNDARIES, 2 != values.remove(0));
	LLC_TEST_CHECK(results, TEST_APOD_N2_RESULT_BOUNDARIES, values[0].x != 10 || values[1].y != 40);
	LLC_TEST_REQUIRE(results, TEST_APOD_N2_RESULT_BOUNDARIES, 1 != values.pop_back());
	LLC_TEST_CHECK(results, TEST_APOD_N2_RESULT_BOUNDARIES, values.size() != 1 || values[0].x != 10 || values[0].y != 20);
	rtrn 0;
}

sttc ::llc::err_t testAPodN2ViewAliasing(::llc::ATestError & results) {
	::llc::an2u2_t values = {{1, 3}};
	::llc::view<::llc::n2<::llc::u2_t>> & view = values;
	view[0].y = 7;
	LLC_TEST_CHECK(results, TEST_APOD_N2_RESULT_VIEW_ALIASING, view.begin() != values.begin() || values[0].x != 1 || values[0].y != 7);
	rtrn 0;
}

sttc ::llc::err_t testAPodN2Serialization(::llc::ATestError & results) {
	::llc::an2u2_t source = {{1, 3}, {10, 20}};
	::llc::au0_t encoded;
	cnst ::llc::view<cnst ::llc::n2<::llc::u2_t>> sourceView = source;
	LLC_TEST_REQUIRE(results, TEST_APOD_N2_RESULT_SERIALIZATION, 0 > ::llc::saveView(encoded, sourceView));
	::llc::vcu0_t input = encoded;
	::llc::an2u2_t decoded;
	LLC_TEST_REQUIRE(results, TEST_APOD_N2_RESULT_SERIALIZATION, 0 != ::llc::loadView(input, decoded));
	LLC_TEST_CHECK(results, TEST_APOD_N2_RESULT_SERIALIZATION, input.size() || decoded.size() != source.size() || 0 != memcmp(decoded.begin(), source.begin(), source.byte_count()));
	rtrn 0;
}

::llc::err_t testAPodN2(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testAPodN2Aliases(results));
	result = ::llc::min(result, ::testAPodN2ConstAliases(results));
	result = ::llc::min(result, ::testAPodN2Empty(results));
	result = ::llc::min(result, ::testAPodN2IntegerValues(results));
	result = ::llc::min(result, ::testAPodN2FloatValues(results));
	result = ::llc::min(result, ::testAPodN2Copy(results));
	result = ::llc::min(result, ::testAPodN2Move(results));
	result = ::llc::min(result, ::testAPodN2SelfAssignment(results));
	result = ::llc::min(result, ::testAPodN2ReserveRejection(results));
	result = ::llc::min(result, ::testAPodN2Boundaries(results));
	result = ::llc::min(result, ::testAPodN2ViewAliasing(results));
	result = ::llc::min(result, ::testAPodN2Serialization(results));
	rtrn result;
}
