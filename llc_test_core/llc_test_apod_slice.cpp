#include "llc_test_core_suites.h"
#include "llc_apod_slice.h"
#include "llc_apod_serialize.h"

GDEFINE_ENUM_TYPE(TEST_APOD_SLICE_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_APOD_SLICE_RESULT, ALIASES          ,  0, "Every mutable slice array alias preserves its declared element type.");
GDEFINE_ENUM_VALUED(TEST_APOD_SLICE_RESULT, CONST_ALIASES    ,  1, "Every const slice array alias preserves the matching const array type.");
GDEFINE_ENUM_VALUED(TEST_APOD_SLICE_RESULT, EMPTY            ,  2, "A slice array begins empty.");
GDEFINE_ENUM_VALUED(TEST_APOD_SLICE_RESULT, INTEGER_VALUES   ,  3, "Integer slice elements retain begin and end.");
GDEFINE_ENUM_VALUED(TEST_APOD_SLICE_RESULT, FLOAT_VALUES     ,  4, "Floating-point slice elements retain begin and end.");
GDEFINE_ENUM_VALUED(TEST_APOD_SLICE_RESULT, COPY             ,  5, "Copying a slice array duplicates storage and values.");
GDEFINE_ENUM_VALUED(TEST_APOD_SLICE_RESULT, MOVE             ,  6, "Moving a slice array transfers storage and clears the source.");
GDEFINE_ENUM_VALUED(TEST_APOD_SLICE_RESULT, SELF_ASSIGNMENT  ,  7, "Self-assignment preserves slice-array storage and values.");
GDEFINE_ENUM_VALUED(TEST_APOD_SLICE_RESULT, RESERVE_REJECTION,  8, "Rejecting an impossible reserve preserves the existing slice array.");
GDEFINE_ENUM_VALUED(TEST_APOD_SLICE_RESULT, BOUNDARIES       ,  9, "Removing the first and final slices preserves the middle slice.");
GDEFINE_ENUM_VALUED(TEST_APOD_SLICE_RESULT, VIEW_ALIASING    , 10, "A slice view aliases the array's live storage.");
GDEFINE_ENUM_VALUED(TEST_APOD_SLICE_RESULT, SERIALIZATION    , 11, "A slice array round-trips through the canonical view serializer.");

sttc ::llc::err_t testAPodSliceAliases(::llc::ATestError & results) {
	::llc::apod<::llc::slice<::llc::sc_t>> * asc = (::llc::aslicesc_t*)0;
	::llc::apod<::llc::slice<::llc::uc_t>> * auc = (::llc::asliceuc_t*)0;
	::llc::apod<::llc::slice<::llc::u0_t>> * au0 = (::llc::asliceu0_t*)0;
	::llc::apod<::llc::slice<::llc::u1_t>> * au1 = (::llc::asliceu1_t*)0;
	::llc::apod<::llc::slice<::llc::u2_t>> * au2 = (::llc::asliceu2_t*)0;
	::llc::apod<::llc::slice<::llc::u3_t>> * au3 = (::llc::asliceu3_t*)0;
	::llc::apod<::llc::slice<::llc::s0_t>> * as0 = (::llc::aslices0_t*)0;
	::llc::apod<::llc::slice<::llc::s1_t>> * as1 = (::llc::aslices1_t*)0;
	::llc::apod<::llc::slice<::llc::s2_t>> * as2 = (::llc::aslices2_t*)0;
	::llc::apod<::llc::slice<::llc::s3_t>> * as3 = (::llc::aslices3_t*)0;
	::llc::apod<::llc::slice<::llc::f2_t>> * af2 = (::llc::aslicef2_t*)0;
	::llc::apod<::llc::slice<::llc::f3_t>> * af3 = (::llc::aslicef3_t*)0;
	LLC_TEST_CHECK(results, TEST_APOD_SLICE_RESULT_ALIASES, asc || auc || au0 || au1 || au2 || au3 || as0 || as1 || as2 || as3 || af2 || af3);
	rtrn 0;
}

sttc ::llc::err_t testAPodSliceConstAliases(::llc::ATestError & results) {
	cnst ::llc::aslice<::llc::sc_t> * asc = (::llc::aslicesc_c*)0;
	cnst ::llc::aslice<::llc::uc_t> * auc = (::llc::asliceuc_c*)0;
	cnst ::llc::aslice<::llc::u0_t> * au0 = (::llc::asliceu0_c*)0;
	cnst ::llc::aslice<::llc::u1_t> * au1 = (::llc::asliceu1_c*)0;
	cnst ::llc::aslice<::llc::u2_t> * au2 = (::llc::asliceu2_c*)0;
	cnst ::llc::aslice<::llc::u3_t> * au3 = (::llc::asliceu3_c*)0;
	cnst ::llc::aslice<::llc::s0_t> * as0 = (::llc::aslices0_c*)0;
	cnst ::llc::aslice<::llc::s1_t> * as1 = (::llc::aslices1_c*)0;
	cnst ::llc::aslice<::llc::s2_t> * as2 = (::llc::aslices2_c*)0;
	cnst ::llc::aslice<::llc::s3_t> * as3 = (::llc::aslices3_c*)0;
	cnst ::llc::aslice<::llc::f2_t> * af2 = (::llc::aslicef2_c*)0;
	cnst ::llc::aslice<::llc::f3_t> * af3 = (::llc::aslicef3_c*)0;
	LLC_TEST_CHECK(results, TEST_APOD_SLICE_RESULT_CONST_ALIASES, asc || auc || au0 || au1 || au2 || au3 || as0 || as1 || as2 || as3 || af2 || af3);
	rtrn 0;
}

sttc ::llc::err_t testAPodSliceEmpty(::llc::ATestError & results) {
	::llc::asliceu2_t values;
	LLC_TEST_CHECK(results, TEST_APOD_SLICE_RESULT_EMPTY, values.size() || values.begin());
	rtrn 0;
}

sttc ::llc::err_t testAPodSliceIntegerValues(::llc::ATestError & results) {
	::llc::asliceu2_t values;
	LLC_TEST_REQUIRE(results, TEST_APOD_SLICE_RESULT_INTEGER_VALUES, 0 != values.push_back({1, 3}));
	LLC_TEST_REQUIRE(results, TEST_APOD_SLICE_RESULT_INTEGER_VALUES, 1 != values.push_back({10, 20}));
	LLC_TEST_CHECK(results, TEST_APOD_SLICE_RESULT_INTEGER_VALUES, values[0].Begin != 1 || values[0].End != 3 || values[1].Begin != 10 || values[1].End != 20);
	rtrn 0;
}

sttc ::llc::err_t testAPodSliceFloatValues(::llc::ATestError & results) {
	::llc::aslicef2_t values;
	LLC_TEST_REQUIRE(results, TEST_APOD_SLICE_RESULT_FLOAT_VALUES, 0 != values.push_back({0.25f, 0.75f}));
	LLC_TEST_REQUIRE(results, TEST_APOD_SLICE_RESULT_FLOAT_VALUES, 1 != values.push_back({-1.5f, 2.5f}));
	LLC_TEST_CHECK(results, TEST_APOD_SLICE_RESULT_FLOAT_VALUES, values[0].Begin != 0.25f || values[0].End != 0.75f || values[1].Begin != -1.5f || values[1].End != 2.5f);
	rtrn 0;
}

sttc ::llc::err_t testAPodSliceCopy(::llc::ATestError & results) {
	::llc::asliceu2_t source = {{1, 3}, {10, 20}};
	::llc::asliceu2_t copied = source;
	LLC_TEST_CHECK(results, TEST_APOD_SLICE_RESULT_COPY, copied.size() != source.size() || copied.begin() == source.begin() || 0 != memcmp(copied.begin(), source.begin(), source.byte_count()));
	rtrn 0;
}

sttc ::llc::err_t testAPodSliceMove(::llc::ATestError & results) {
	::llc::asliceu2_t source = {{1, 3}, {10, 20}};
	::llc::slice<::llc::u2_t> * sourceData = source.begin();
	::llc::asliceu2_t moved = (::llc::asliceu2_t&&)source;
	LLC_TEST_CHECK(results, TEST_APOD_SLICE_RESULT_MOVE, source.size() || source.begin() || moved.size() != 2 || moved.begin() != sourceData);
	rtrn 0;
}

sttc ::llc::err_t testAPodSliceSelfAssignment(::llc::ATestError & results) {
	::llc::asliceu2_t values = {{1, 3}, {10, 20}};
	::llc::slice<::llc::u2_t> * originalData = values.begin();
	values = values;
	LLC_TEST_CHECK(results, TEST_APOD_SLICE_RESULT_SELF_ASSIGNMENT, values.size() != 2 || values.begin() != originalData || values[0].Begin != 1 || values[1].End != 20);
	rtrn 0;
}

sttc ::llc::err_t testAPodSliceReserveRejection(::llc::ATestError & results) {
	::llc::asliceu2_t values = {{1, 3}};
	::llc::slice<::llc::u2_t> * originalData = values.begin();
	LLC_TEST_CHECK(results, TEST_APOD_SLICE_RESULT_RESERVE_REJECTION, 0 <= values.reserve((::llc::u2_t)-1));
	LLC_TEST_CHECK(results, TEST_APOD_SLICE_RESULT_RESERVE_REJECTION, values.size() != 1 || values.begin() != originalData || values[0].Begin != 1 || values[0].End != 3);
	rtrn 0;
}

sttc ::llc::err_t testAPodSliceBoundaries(::llc::ATestError & results) {
	::llc::asliceu2_t values = {{1, 3}, {10, 20}, {30, 40}};
	LLC_TEST_REQUIRE(results, TEST_APOD_SLICE_RESULT_BOUNDARIES, 2 != values.remove(0));
	LLC_TEST_CHECK(results, TEST_APOD_SLICE_RESULT_BOUNDARIES, values[0].Begin != 10 || values[1].End != 40);
	LLC_TEST_REQUIRE(results, TEST_APOD_SLICE_RESULT_BOUNDARIES, 1 != values.pop_back());
	LLC_TEST_CHECK(results, TEST_APOD_SLICE_RESULT_BOUNDARIES, values.size() != 1 || values[0].Begin != 10 || values[0].End != 20);
	rtrn 0;
}

sttc ::llc::err_t testAPodSliceViewAliasing(::llc::ATestError & results) {
	::llc::asliceu2_t values = {{1, 3}};
	::llc::view<::llc::slice<::llc::u2_t>> & view = values;
	view[0].End = 7;
	LLC_TEST_CHECK(results, TEST_APOD_SLICE_RESULT_VIEW_ALIASING, view.begin() != values.begin() || values[0].Begin != 1 || values[0].End != 7);
	rtrn 0;
}

sttc ::llc::err_t testAPodSliceSerialization(::llc::ATestError & results) {
	::llc::asliceu2_t source = {{1, 3}, {10, 20}};
	::llc::au0_t encoded;
	cnst ::llc::view<cnst ::llc::slice<::llc::u2_t>> sourceView = source;
	LLC_TEST_REQUIRE(results, TEST_APOD_SLICE_RESULT_SERIALIZATION, 0 > ::llc::saveView(encoded, sourceView));
	::llc::vcu0_t input = encoded;
	::llc::asliceu2_t decoded;
	LLC_TEST_REQUIRE(results, TEST_APOD_SLICE_RESULT_SERIALIZATION, 0 != ::llc::loadView(input, decoded));
	LLC_TEST_CHECK(results, TEST_APOD_SLICE_RESULT_SERIALIZATION, input.size() || decoded.size() != source.size() || 0 != memcmp(decoded.begin(), source.begin(), source.byte_count()));
	rtrn 0;
}

::llc::err_t testAPodSlice(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testAPodSliceAliases(results));
	result = ::llc::min(result, ::testAPodSliceConstAliases(results));
	result = ::llc::min(result, ::testAPodSliceEmpty(results));
	result = ::llc::min(result, ::testAPodSliceIntegerValues(results));
	result = ::llc::min(result, ::testAPodSliceFloatValues(results));
	result = ::llc::min(result, ::testAPodSliceCopy(results));
	result = ::llc::min(result, ::testAPodSliceMove(results));
	result = ::llc::min(result, ::testAPodSliceSelfAssignment(results));
	result = ::llc::min(result, ::testAPodSliceReserveRejection(results));
	result = ::llc::min(result, ::testAPodSliceBoundaries(results));
	result = ::llc::min(result, ::testAPodSliceViewAliasing(results));
	result = ::llc::min(result, ::testAPodSliceSerialization(results));
	rtrn result;
}
