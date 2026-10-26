#include "llc_test_core_suites.h"
#include "llc_apod_range.h"
#include "llc_apod_serialize.h"

GDEFINE_ENUM_TYPE(TEST_APOD_RANGE_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_APOD_RANGE_RESULT, ALIASES          ,  0, "Every mutable range array alias preserves its declared element type.");
GDEFINE_ENUM_VALUED(TEST_APOD_RANGE_RESULT, CONST_ALIASES    ,  1, "Every const range array alias preserves the matching const array type.");
GDEFINE_ENUM_VALUED(TEST_APOD_RANGE_RESULT, EMPTY            ,  2, "A range array begins empty.");
GDEFINE_ENUM_VALUED(TEST_APOD_RANGE_RESULT, INTEGER_VALUES   ,  3, "Integer range elements retain offset and count.");
GDEFINE_ENUM_VALUED(TEST_APOD_RANGE_RESULT, FLOAT_VALUES     ,  4, "Floating-point range elements retain offset and count.");
GDEFINE_ENUM_VALUED(TEST_APOD_RANGE_RESULT, COPY             ,  5, "Copying a range array duplicates storage and values.");
GDEFINE_ENUM_VALUED(TEST_APOD_RANGE_RESULT, MOVE             ,  6, "Moving a range array transfers storage and clears the source.");
GDEFINE_ENUM_VALUED(TEST_APOD_RANGE_RESULT, SELF_ASSIGNMENT  ,  7, "Self-assignment preserves range-array storage and values.");
GDEFINE_ENUM_VALUED(TEST_APOD_RANGE_RESULT, RESERVE_REJECTION,  8, "Rejecting an impossible reserve preserves the existing range array.");
GDEFINE_ENUM_VALUED(TEST_APOD_RANGE_RESULT, BOUNDARIES       ,  9, "Removing the first and final ranges preserves the middle range.");
GDEFINE_ENUM_VALUED(TEST_APOD_RANGE_RESULT, VIEW_ALIASING    , 10, "A range view aliases the array's live storage.");
GDEFINE_ENUM_VALUED(TEST_APOD_RANGE_RESULT, SERIALIZATION    , 11, "A range array round-trips through the canonical view serializer.");

sttc ::llc::err_t testAPodRangeAliases(::llc::ATestError & results) {
	::llc::apod<::llc::range<::llc::sc_t>> * asc = (::llc::arangesc_t*)0;
	::llc::apod<::llc::range<::llc::uc_t>> * auc = (::llc::arangeuc_t*)0;
	::llc::apod<::llc::range<::llc::u0_t>> * au0 = (::llc::arangeu0_t*)0;
	::llc::apod<::llc::range<::llc::u1_t>> * au1 = (::llc::arangeu1_t*)0;
	::llc::apod<::llc::range<::llc::u2_t>> * au2 = (::llc::arangeu2_t*)0;
	::llc::apod<::llc::range<::llc::u3_t>> * au3 = (::llc::arangeu3_t*)0;
	::llc::apod<::llc::range<::llc::s0_t>> * as0 = (::llc::aranges0_t*)0;
	::llc::apod<::llc::range<::llc::s1_t>> * as1 = (::llc::aranges1_t*)0;
	::llc::apod<::llc::range<::llc::s2_t>> * as2 = (::llc::aranges2_t*)0;
	::llc::apod<::llc::range<::llc::s3_t>> * as3 = (::llc::aranges3_t*)0;
	::llc::apod<::llc::range<::llc::f2_t>> * af2 = (::llc::arangef2_t*)0;
	::llc::apod<::llc::range<::llc::f3_t>> * af3 = (::llc::arangef3_t*)0;
	LLC_TEST_CHECK(results, TEST_APOD_RANGE_RESULT_ALIASES, asc || auc || au0 || au1 || au2 || au3 || as0 || as1 || as2 || as3 || af2 || af3);
	rtrn 0;
}

sttc ::llc::err_t testAPodRangeConstAliases(::llc::ATestError & results) {
	cnst ::llc::arange<::llc::sc_t> * asc = (::llc::arangesc_c*)0;
	cnst ::llc::arange<::llc::uc_t> * auc = (::llc::arangeuc_c*)0;
	cnst ::llc::arange<::llc::u0_t> * au0 = (::llc::arangeu0_c*)0;
	cnst ::llc::arange<::llc::u1_t> * au1 = (::llc::arangeu1_c*)0;
	cnst ::llc::arange<::llc::u2_t> * au2 = (::llc::arangeu2_c*)0;
	cnst ::llc::arange<::llc::u3_t> * au3 = (::llc::arangeu3_c*)0;
	cnst ::llc::arange<::llc::s0_t> * as0 = (::llc::aranges0_c*)0;
	cnst ::llc::arange<::llc::s1_t> * as1 = (::llc::aranges1_c*)0;
	cnst ::llc::arange<::llc::s2_t> * as2 = (::llc::aranges2_c*)0;
	cnst ::llc::arange<::llc::s3_t> * as3 = (::llc::aranges3_c*)0;
	cnst ::llc::arange<::llc::f2_t> * af2 = (::llc::arangef2_c*)0;
	cnst ::llc::arange<::llc::f3_t> * af3 = (::llc::arangef3_c*)0;
	LLC_TEST_CHECK(results, TEST_APOD_RANGE_RESULT_CONST_ALIASES, asc || auc || au0 || au1 || au2 || au3 || as0 || as1 || as2 || as3 || af2 || af3);
	rtrn 0;
}

sttc ::llc::err_t testAPodRangeEmpty(::llc::ATestError & results) {
	::llc::arangeu2_t values;
	LLC_TEST_CHECK(results, TEST_APOD_RANGE_RESULT_EMPTY, values.size() || values.begin());
	rtrn 0;
}

sttc ::llc::err_t testAPodRangeIntegerValues(::llc::ATestError & results) {
	::llc::arangeu2_t values;
	LLC_TEST_REQUIRE(results, TEST_APOD_RANGE_RESULT_INTEGER_VALUES, 0 != values.push_back({1, 3}));
	LLC_TEST_REQUIRE(results, TEST_APOD_RANGE_RESULT_INTEGER_VALUES, 1 != values.push_back({10, 20}));
	LLC_TEST_CHECK(results, TEST_APOD_RANGE_RESULT_INTEGER_VALUES, values[0].Offset != 1 || values[0].Count != 3 || values[1].Offset != 10 || values[1].Count != 20);
	rtrn 0;
}

sttc ::llc::err_t testAPodRangeFloatValues(::llc::ATestError & results) {
	::llc::arangef2_t values;
	LLC_TEST_REQUIRE(results, TEST_APOD_RANGE_RESULT_FLOAT_VALUES, 0 != values.push_back({0.25f, 0.75f}));
	LLC_TEST_REQUIRE(results, TEST_APOD_RANGE_RESULT_FLOAT_VALUES, 1 != values.push_back({-1.5f, 2.5f}));
	LLC_TEST_CHECK(results, TEST_APOD_RANGE_RESULT_FLOAT_VALUES, values[0].Offset != 0.25f || values[0].Count != 0.75f || values[1].Offset != -1.5f || values[1].Count != 2.5f);
	rtrn 0;
}

sttc ::llc::err_t testAPodRangeCopy(::llc::ATestError & results) {
	::llc::arangeu2_t source = {{1, 3}, {10, 20}};
	::llc::arangeu2_t copied = source;
	LLC_TEST_CHECK(results, TEST_APOD_RANGE_RESULT_COPY, copied.size() != source.size() || copied.begin() == source.begin() || 0 != memcmp(copied.begin(), source.begin(), source.byte_count()));
	rtrn 0;
}

sttc ::llc::err_t testAPodRangeMove(::llc::ATestError & results) {
	::llc::arangeu2_t source = {{1, 3}, {10, 20}};
	::llc::range<::llc::u2_t> * sourceData = source.begin();
	::llc::arangeu2_t moved = (::llc::arangeu2_t&&)source;
	LLC_TEST_CHECK(results, TEST_APOD_RANGE_RESULT_MOVE, source.size() || source.begin() || moved.size() != 2 || moved.begin() != sourceData);
	rtrn 0;
}

sttc ::llc::err_t testAPodRangeSelfAssignment(::llc::ATestError & results) {
	::llc::arangeu2_t values = {{1, 3}, {10, 20}};
	::llc::range<::llc::u2_t> * originalData = values.begin();
	values = values;
	LLC_TEST_CHECK(results, TEST_APOD_RANGE_RESULT_SELF_ASSIGNMENT, values.size() != 2 || values.begin() != originalData || values[0].Offset != 1 || values[1].Count != 20);
	rtrn 0;
}

sttc ::llc::err_t testAPodRangeReserveRejection(::llc::ATestError & results) {
	::llc::arangeu2_t values = {{1, 3}};
	::llc::range<::llc::u2_t> * originalData = values.begin();
	LLC_TEST_CHECK(results, TEST_APOD_RANGE_RESULT_RESERVE_REJECTION, 0 <= values.reserve((::llc::u2_t)-1));
	LLC_TEST_CHECK(results, TEST_APOD_RANGE_RESULT_RESERVE_REJECTION, values.size() != 1 || values.begin() != originalData || values[0].Offset != 1 || values[0].Count != 3);
	rtrn 0;
}

sttc ::llc::err_t testAPodRangeBoundaries(::llc::ATestError & results) {
	::llc::arangeu2_t values = {{1, 3}, {10, 20}, {30, 40}};
	LLC_TEST_REQUIRE(results, TEST_APOD_RANGE_RESULT_BOUNDARIES, 2 != values.remove(0));
	LLC_TEST_CHECK(results, TEST_APOD_RANGE_RESULT_BOUNDARIES, values[0].Offset != 10 || values[1].Count != 40);
	LLC_TEST_REQUIRE(results, TEST_APOD_RANGE_RESULT_BOUNDARIES, 1 != values.pop_back());
	LLC_TEST_CHECK(results, TEST_APOD_RANGE_RESULT_BOUNDARIES, values.size() != 1 || values[0].Offset != 10 || values[0].Count != 20);
	rtrn 0;
}

sttc ::llc::err_t testAPodRangeViewAliasing(::llc::ATestError & results) {
	::llc::arangeu2_t values = {{1, 3}};
	::llc::view<::llc::range<::llc::u2_t>> & view = values;
	view[0].Count = 7;
	LLC_TEST_CHECK(results, TEST_APOD_RANGE_RESULT_VIEW_ALIASING, view.begin() != values.begin() || values[0].Offset != 1 || values[0].Count != 7);
	rtrn 0;
}

sttc ::llc::err_t testAPodRangeSerialization(::llc::ATestError & results) {
	::llc::arangeu2_t source = {{1, 3}, {10, 20}};
	::llc::au0_t encoded;
	cnst ::llc::view<cnst ::llc::range<::llc::u2_t>> sourceView = source;
	LLC_TEST_REQUIRE(results, TEST_APOD_RANGE_RESULT_SERIALIZATION, 0 > ::llc::saveView(encoded, sourceView));
	::llc::vcu0_t input = encoded;
	::llc::arangeu2_t decoded;
	LLC_TEST_REQUIRE(results, TEST_APOD_RANGE_RESULT_SERIALIZATION, 0 != ::llc::loadView(input, decoded));
	LLC_TEST_CHECK(results, TEST_APOD_RANGE_RESULT_SERIALIZATION, input.size() || decoded.size() != source.size() || 0 != memcmp(decoded.begin(), source.begin(), source.byte_count()));
	rtrn 0;
}

::llc::err_t testAPodRange(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testAPodRangeAliases(results));
	result = ::llc::min(result, ::testAPodRangeConstAliases(results));
	result = ::llc::min(result, ::testAPodRangeEmpty(results));
	result = ::llc::min(result, ::testAPodRangeIntegerValues(results));
	result = ::llc::min(result, ::testAPodRangeFloatValues(results));
	result = ::llc::min(result, ::testAPodRangeCopy(results));
	result = ::llc::min(result, ::testAPodRangeMove(results));
	result = ::llc::min(result, ::testAPodRangeSelfAssignment(results));
	result = ::llc::min(result, ::testAPodRangeReserveRejection(results));
	result = ::llc::min(result, ::testAPodRangeBoundaries(results));
	result = ::llc::min(result, ::testAPodRangeViewAliasing(results));
	result = ::llc::min(result, ::testAPodRangeSerialization(results));
	rtrn result;
}
