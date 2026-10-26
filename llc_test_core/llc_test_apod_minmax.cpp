#include "llc_test_core_suites.h"
#include "llc_apod_minmax.h"
#include "llc_apod_serialize.h"

GDEFINE_ENUM_TYPE(TEST_APOD_MINMAX_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_APOD_MINMAX_RESULT, ALIASES          ,  0, "Every mutable min/max array alias preserves its declared element type.");
GDEFINE_ENUM_VALUED(TEST_APOD_MINMAX_RESULT, CONST_ALIASES    ,  1, "Every const min/max array alias preserves the matching const array type.");
GDEFINE_ENUM_VALUED(TEST_APOD_MINMAX_RESULT, EMPTY            ,  2, "A min/max array begins empty.");
GDEFINE_ENUM_VALUED(TEST_APOD_MINMAX_RESULT, INTEGER_VALUES   ,  3, "Integer min/max elements retain both endpoints.");
GDEFINE_ENUM_VALUED(TEST_APOD_MINMAX_RESULT, FLOAT_VALUES     ,  4, "Floating-point min/max elements retain both endpoints.");
GDEFINE_ENUM_VALUED(TEST_APOD_MINMAX_RESULT, COPY             ,  5, "Copying a min/max array duplicates storage and values.");
GDEFINE_ENUM_VALUED(TEST_APOD_MINMAX_RESULT, MOVE             ,  6, "Moving a min/max array transfers storage and clears the source.");
GDEFINE_ENUM_VALUED(TEST_APOD_MINMAX_RESULT, SELF_ASSIGNMENT  ,  7, "Self-assignment preserves min/max storage and values.");
GDEFINE_ENUM_VALUED(TEST_APOD_MINMAX_RESULT, RESERVE_REJECTION,  8, "Rejecting an impossible reserve preserves the existing array.");
GDEFINE_ENUM_VALUED(TEST_APOD_MINMAX_RESULT, BOUNDARIES       ,  9, "Removing the first and final ranges preserves the middle range.");
GDEFINE_ENUM_VALUED(TEST_APOD_MINMAX_RESULT, VIEW_ALIASING    , 10, "A min/max view aliases the array's live storage.");
GDEFINE_ENUM_VALUED(TEST_APOD_MINMAX_RESULT, SERIALIZATION    , 11, "A min/max array round-trips through the canonical view serializer.");

sttc ::llc::err_t testAPodMinMaxAliases(::llc::ATestError & results) {
	::llc::apod<::llc::minmax<::llc::sc_t>> * asc = (::llc::aminmaxsc_t*)0;
	::llc::apod<::llc::minmax<::llc::uc_t>> * auc = (::llc::aminmaxuc_t*)0;
	::llc::apod<::llc::minmax<::llc::u0_t>> * au0 = (::llc::aminmaxu0_t*)0;
	::llc::apod<::llc::minmax<::llc::u1_t>> * au1 = (::llc::aminmaxu1_t*)0;
	::llc::apod<::llc::minmax<::llc::u2_t>> * au2 = (::llc::aminmaxu2_t*)0;
	::llc::apod<::llc::minmax<::llc::u3_t>> * au3 = (::llc::aminmaxu3_t*)0;
	::llc::apod<::llc::minmax<::llc::s0_t>> * as0 = (::llc::aminmaxs0_t*)0;
	::llc::apod<::llc::minmax<::llc::s1_t>> * as1 = (::llc::aminmaxs1_t*)0;
	::llc::apod<::llc::minmax<::llc::s2_t>> * as2 = (::llc::aminmaxs2_t*)0;
	::llc::apod<::llc::minmax<::llc::s3_t>> * as3 = (::llc::aminmaxs3_t*)0;
	::llc::apod<::llc::minmax<::llc::f2_t>> * af2 = (::llc::aminmaxf2_t*)0;
	::llc::apod<::llc::minmax<::llc::f3_t>> * af3 = (::llc::aminmaxf3_t*)0;
	LLC_TEST_CHECK(results, TEST_APOD_MINMAX_RESULT_ALIASES, asc || auc || au0 || au1 || au2 || au3 || as0 || as1 || as2 || as3 || af2 || af3);
	rtrn 0;
}

sttc ::llc::err_t testAPodMinMaxConstAliases(::llc::ATestError & results) {
	cnst ::llc::aminmax<::llc::sc_t> * asc = (::llc::aminmaxsc_c*)0;
	cnst ::llc::aminmax<::llc::uc_t> * auc = (::llc::aminmaxuc_c*)0;
	cnst ::llc::aminmax<::llc::u0_t> * au0 = (::llc::aminmaxu0_c*)0;
	cnst ::llc::aminmax<::llc::u1_t> * au1 = (::llc::aminmaxu1_c*)0;
	cnst ::llc::aminmax<::llc::u2_t> * au2 = (::llc::aminmaxu2_c*)0;
	cnst ::llc::aminmax<::llc::u3_t> * au3 = (::llc::aminmaxu3_c*)0;
	cnst ::llc::aminmax<::llc::s0_t> * as0 = (::llc::aminmaxs0_c*)0;
	cnst ::llc::aminmax<::llc::s1_t> * as1 = (::llc::aminmaxs1_c*)0;
	cnst ::llc::aminmax<::llc::s2_t> * as2 = (::llc::aminmaxs2_c*)0;
	cnst ::llc::aminmax<::llc::s3_t> * as3 = (::llc::aminmaxs3_c*)0;
	cnst ::llc::aminmax<::llc::f2_t> * af2 = (::llc::aminmaxf2_c*)0;
	cnst ::llc::aminmax<::llc::f3_t> * af3 = (::llc::aminmaxf3_c*)0;
	LLC_TEST_CHECK(results, TEST_APOD_MINMAX_RESULT_CONST_ALIASES, asc || auc || au0 || au1 || au2 || au3 || as0 || as1 || as2 || as3 || af2 || af3);
	rtrn 0;
}

sttc ::llc::err_t testAPodMinMaxEmpty(::llc::ATestError & results) {
	::llc::aminmaxu2_t values;
	LLC_TEST_CHECK(results, TEST_APOD_MINMAX_RESULT_EMPTY, values.size() || values.begin());
	rtrn 0;
}

sttc ::llc::err_t testAPodMinMaxIntegerValues(::llc::ATestError & results) {
	::llc::aminmaxu2_t values;
	LLC_TEST_REQUIRE(results, TEST_APOD_MINMAX_RESULT_INTEGER_VALUES, 0 != values.push_back({1, 3}));
	LLC_TEST_REQUIRE(results, TEST_APOD_MINMAX_RESULT_INTEGER_VALUES, 1 != values.push_back({10, 20}));
	LLC_TEST_CHECK(results, TEST_APOD_MINMAX_RESULT_INTEGER_VALUES, values[0].Min != 1 || values[0].Max != 3 || values[1].Min != 10 || values[1].Max != 20);
	rtrn 0;
}

sttc ::llc::err_t testAPodMinMaxFloatValues(::llc::ATestError & results) {
	::llc::aminmaxf2_t values;
	LLC_TEST_REQUIRE(results, TEST_APOD_MINMAX_RESULT_FLOAT_VALUES, 0 != values.push_back({0.25f, 0.75f}));
	LLC_TEST_REQUIRE(results, TEST_APOD_MINMAX_RESULT_FLOAT_VALUES, 1 != values.push_back({-1.5f, 2.5f}));
	LLC_TEST_CHECK(results, TEST_APOD_MINMAX_RESULT_FLOAT_VALUES, values[0].Min != 0.25f || values[0].Max != 0.75f || values[1].Min != -1.5f || values[1].Max != 2.5f);
	rtrn 0;
}

sttc ::llc::err_t testAPodMinMaxCopy(::llc::ATestError & results) {
	::llc::aminmaxu2_t source = {{1, 3}, {10, 20}};
	::llc::aminmaxu2_t copied = source;
	LLC_TEST_CHECK(results, TEST_APOD_MINMAX_RESULT_COPY, copied.size() != source.size() || copied.begin() == source.begin() || 0 != memcmp(copied.begin(), source.begin(), source.byte_count()));
	rtrn 0;
}

sttc ::llc::err_t testAPodMinMaxMove(::llc::ATestError & results) {
	::llc::aminmaxu2_t source = {{1, 3}, {10, 20}};
	::llc::minmax<::llc::u2_t> * sourceData = source.begin();
	::llc::aminmaxu2_t moved = (::llc::aminmaxu2_t&&)source;
	LLC_TEST_CHECK(results, TEST_APOD_MINMAX_RESULT_MOVE, source.size() || source.begin() || moved.size() != 2 || moved.begin() != sourceData);
	rtrn 0;
}

sttc ::llc::err_t testAPodMinMaxSelfAssignment(::llc::ATestError & results) {
	::llc::aminmaxu2_t values = {{1, 3}, {10, 20}};
	::llc::minmax<::llc::u2_t> * originalData = values.begin();
	values = values;
	LLC_TEST_CHECK(results, TEST_APOD_MINMAX_RESULT_SELF_ASSIGNMENT, values.size() != 2 || values.begin() != originalData || values[0].Min != 1 || values[1].Max != 20);
	rtrn 0;
}

sttc ::llc::err_t testAPodMinMaxReserveRejection(::llc::ATestError & results) {
	::llc::aminmaxu2_t values = {{1, 3}};
	::llc::minmax<::llc::u2_t> * originalData = values.begin();
	LLC_TEST_CHECK(results, TEST_APOD_MINMAX_RESULT_RESERVE_REJECTION, 0 <= values.reserve((::llc::u2_t)-1));
	LLC_TEST_CHECK(results, TEST_APOD_MINMAX_RESULT_RESERVE_REJECTION, values.size() != 1 || values.begin() != originalData || values[0].Min != 1 || values[0].Max != 3);
	rtrn 0;
}

sttc ::llc::err_t testAPodMinMaxBoundaries(::llc::ATestError & results) {
	::llc::aminmaxu2_t values = {{1, 3}, {10, 20}, {30, 40}};
	LLC_TEST_REQUIRE(results, TEST_APOD_MINMAX_RESULT_BOUNDARIES, 2 != values.remove(0));
	LLC_TEST_CHECK(results, TEST_APOD_MINMAX_RESULT_BOUNDARIES, values[0].Min != 10 || values[1].Max != 40);
	LLC_TEST_REQUIRE(results, TEST_APOD_MINMAX_RESULT_BOUNDARIES, 1 != values.pop_back());
	LLC_TEST_CHECK(results, TEST_APOD_MINMAX_RESULT_BOUNDARIES, values.size() != 1 || values[0].Min != 10 || values[0].Max != 20);
	rtrn 0;
}

sttc ::llc::err_t testAPodMinMaxViewAliasing(::llc::ATestError & results) {
	::llc::aminmaxu2_t values = {{1, 3}};
	::llc::view<::llc::minmax<::llc::u2_t>> & view = values;
	view[0].Max = 7;
	LLC_TEST_CHECK(results, TEST_APOD_MINMAX_RESULT_VIEW_ALIASING, view.begin() != values.begin() || values[0].Min != 1 || values[0].Max != 7);
	rtrn 0;
}

sttc ::llc::err_t testAPodMinMaxSerialization(::llc::ATestError & results) {
	::llc::aminmaxu2_t source = {{1, 3}, {10, 20}};
	::llc::au0_t encoded;
	cnst ::llc::view<cnst ::llc::minmax<::llc::u2_t>> sourceView = source;
	LLC_TEST_REQUIRE(results, TEST_APOD_MINMAX_RESULT_SERIALIZATION, 0 > ::llc::saveView(encoded, sourceView));
	::llc::vcu0_t input = encoded;
	::llc::aminmaxu2_t decoded;
	LLC_TEST_REQUIRE(results, TEST_APOD_MINMAX_RESULT_SERIALIZATION, 0 != ::llc::loadView(input, decoded));
	LLC_TEST_CHECK(results, TEST_APOD_MINMAX_RESULT_SERIALIZATION, input.size() || decoded.size() != source.size() || 0 != memcmp(decoded.begin(), source.begin(), source.byte_count()));
	rtrn 0;
}

::llc::err_t testAPodMinMax(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testAPodMinMaxAliases(results));
	result = ::llc::min(result, ::testAPodMinMaxConstAliases(results));
	result = ::llc::min(result, ::testAPodMinMaxEmpty(results));
	result = ::llc::min(result, ::testAPodMinMaxIntegerValues(results));
	result = ::llc::min(result, ::testAPodMinMaxFloatValues(results));
	result = ::llc::min(result, ::testAPodMinMaxCopy(results));
	result = ::llc::min(result, ::testAPodMinMaxMove(results));
	result = ::llc::min(result, ::testAPodMinMaxSelfAssignment(results));
	result = ::llc::min(result, ::testAPodMinMaxReserveRejection(results));
	result = ::llc::min(result, ::testAPodMinMaxBoundaries(results));
	result = ::llc::min(result, ::testAPodMinMaxViewAliasing(results));
	result = ::llc::min(result, ::testAPodMinMaxSerialization(results));
	rtrn result;
}
