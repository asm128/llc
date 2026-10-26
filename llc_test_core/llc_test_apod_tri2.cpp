#include "llc_test_core_suites.h"
#include "llc_apod_tri2.h"
#include "llc_apod_serialize.h"

GDEFINE_ENUM_TYPE(TEST_APOD_TRI2_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_APOD_TRI2_RESULT, ALIASES          ,  0, "Every mutable tri2 array alias preserves its declared element type.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI2_RESULT, CONST_ALIASES    ,  1, "Every const tri2 array alias preserves the matching const array type.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI2_RESULT, EMPTY            ,  2, "A tri2 array begins empty.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI2_RESULT, INTEGER_VALUES   ,  3, "Integer tri2 elements retain all six coordinates.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI2_RESULT, FLOAT_VALUES     ,  4, "Floating-point tri2 elements retain all six coordinates.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI2_RESULT, COPY             ,  5, "Copying a tri2 array duplicates storage and values.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI2_RESULT, MOVE             ,  6, "Moving a tri2 array transfers storage and clears the source.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI2_RESULT, SELF_ASSIGNMENT  ,  7, "Self-assignment preserves tri2-array storage and values.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI2_RESULT, RESERVE_REJECTION,  8, "Rejecting an impossible reserve preserves the existing tri2 array.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI2_RESULT, BOUNDARIES       ,  9, "Removing the first and final tri2 values preserves the middle value.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI2_RESULT, VIEW_ALIASING    , 10, "A tri2 view aliases the array's live storage.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI2_RESULT, SERIALIZATION    , 11, "A tri2 array round-trips through the canonical view serializer.");

sttc ::llc::tri2u2_t testAPodTri2U2(::llc::u2_t base) {
	rtrn {{base + 0, base + 1}, {base + 2, base + 3}, {base + 4, base + 5}};
}

sttc ::llc::tri2f2_t testAPodTri2F2(::llc::f2_t base) {
	rtrn {{base + 0.25f, base + 0.50f}, {base + 0.75f, base + 1.00f}, {base + 1.25f, base + 1.50f}};
}

sttc ::llc::err_t testAPodTri2Aliases(::llc::ATestError & results) {
	::llc::apod<::llc::tri2<::llc::sc_t>> * asc = (::llc::atri2sc_t*)0;
	::llc::apod<::llc::tri2<::llc::uc_t>> * auc = (::llc::atri2uc_t*)0;
	::llc::apod<::llc::tri2<::llc::u0_t>> * au0 = (::llc::atri2u0_t*)0;
	::llc::apod<::llc::tri2<::llc::u1_t>> * au1 = (::llc::atri2u1_t*)0;
	::llc::apod<::llc::tri2<::llc::u2_t>> * au2 = (::llc::atri2u2_t*)0;
	::llc::apod<::llc::tri2<::llc::u3_t>> * au3 = (::llc::atri2u3_t*)0;
	::llc::apod<::llc::tri2<::llc::s0_t>> * as0 = (::llc::atri2s0_t*)0;
	::llc::apod<::llc::tri2<::llc::s1_t>> * as1 = (::llc::atri2s1_t*)0;
	::llc::apod<::llc::tri2<::llc::s2_t>> * as2 = (::llc::atri2s2_t*)0;
	::llc::apod<::llc::tri2<::llc::s3_t>> * as3 = (::llc::atri2s3_t*)0;
	::llc::apod<::llc::tri2<::llc::f2_t>> * af2 = (::llc::atri2f2_t*)0;
	::llc::apod<::llc::tri2<::llc::f3_t>> * af3 = (::llc::atri2f3_t*)0;
	LLC_TEST_CHECK(results, TEST_APOD_TRI2_RESULT_ALIASES, asc || auc || au0 || au1 || au2 || au3 || as0 || as1 || as2 || as3 || af2 || af3);
	rtrn 0;
}

sttc ::llc::err_t testAPodTri2ConstAliases(::llc::ATestError & results) {
	cnst ::llc::atri2<::llc::sc_t> * asc = (::llc::atri2sc_c*)0;
	cnst ::llc::atri2<::llc::uc_t> * auc = (::llc::atri2uc_c*)0;
	cnst ::llc::atri2<::llc::u0_t> * au0 = (::llc::atri2u0_c*)0;
	cnst ::llc::atri2<::llc::u1_t> * au1 = (::llc::atri2u1_c*)0;
	cnst ::llc::atri2<::llc::u2_t> * au2 = (::llc::atri2u2_c*)0;
	cnst ::llc::atri2<::llc::u3_t> * au3 = (::llc::atri2u3_c*)0;
	cnst ::llc::atri2<::llc::s0_t> * as0 = (::llc::atri2s0_c*)0;
	cnst ::llc::atri2<::llc::s1_t> * as1 = (::llc::atri2s1_c*)0;
	cnst ::llc::atri2<::llc::s2_t> * as2 = (::llc::atri2s2_c*)0;
	cnst ::llc::atri2<::llc::s3_t> * as3 = (::llc::atri2s3_c*)0;
	cnst ::llc::atri2<::llc::f2_t> * af2 = (::llc::atri2f2_c*)0;
	cnst ::llc::atri2<::llc::f3_t> * af3 = (::llc::atri2f3_c*)0;
	LLC_TEST_CHECK(results, TEST_APOD_TRI2_RESULT_CONST_ALIASES, asc || auc || au0 || au1 || au2 || au3 || as0 || as1 || as2 || as3 || af2 || af3);
	rtrn 0;
}

sttc ::llc::err_t testAPodTri2Empty(::llc::ATestError & results) {
	::llc::atri2u2_t values;
	LLC_TEST_CHECK(results, TEST_APOD_TRI2_RESULT_EMPTY, values.size() || values.begin());
	rtrn 0;
}

sttc ::llc::err_t testAPodTri2IntegerValues(::llc::ATestError & results) {
	cnst ::llc::tri2u2_t first = ::testAPodTri2U2(1), second = ::testAPodTri2U2(10);
	::llc::atri2u2_t values;
	LLC_TEST_REQUIRE(results, TEST_APOD_TRI2_RESULT_INTEGER_VALUES, 0 != values.push_back(first));
	LLC_TEST_REQUIRE(results, TEST_APOD_TRI2_RESULT_INTEGER_VALUES, 1 != values.push_back(second));
	LLC_TEST_CHECK(results, TEST_APOD_TRI2_RESULT_INTEGER_VALUES, 0 != memcmp(&values[0], &first, szof(first)) || 0 != memcmp(&values[1], &second, szof(second)));
	rtrn 0;
}

sttc ::llc::err_t testAPodTri2FloatValues(::llc::ATestError & results) {
	cnst ::llc::tri2f2_t first = ::testAPodTri2F2(0), second = ::testAPodTri2F2(10);
	::llc::atri2f2_t values;
	LLC_TEST_REQUIRE(results, TEST_APOD_TRI2_RESULT_FLOAT_VALUES, 0 != values.push_back(first));
	LLC_TEST_REQUIRE(results, TEST_APOD_TRI2_RESULT_FLOAT_VALUES, 1 != values.push_back(second));
	LLC_TEST_CHECK(results, TEST_APOD_TRI2_RESULT_FLOAT_VALUES, 0 != memcmp(&values[0], &first, szof(first)) || 0 != memcmp(&values[1], &second, szof(second)));
	rtrn 0;
}

sttc ::llc::err_t testAPodTri2Copy(::llc::ATestError & results) {
	::llc::atri2u2_t source = {::testAPodTri2U2(1), ::testAPodTri2U2(10)};
	::llc::atri2u2_t copied = source;
	LLC_TEST_CHECK(results, TEST_APOD_TRI2_RESULT_COPY, copied.size() != source.size() || copied.begin() == source.begin() || 0 != memcmp(copied.begin(), source.begin(), source.byte_count()));
	rtrn 0;
}

sttc ::llc::err_t testAPodTri2Move(::llc::ATestError & results) {
	::llc::atri2u2_t source = {::testAPodTri2U2(1), ::testAPodTri2U2(10)};
	::llc::tri2u2_t * sourceData = source.begin();
	::llc::atri2u2_t moved = (::llc::atri2u2_t&&)source;
	LLC_TEST_CHECK(results, TEST_APOD_TRI2_RESULT_MOVE, source.size() || source.begin() || moved.size() != 2 || moved.begin() != sourceData);
	rtrn 0;
}

sttc ::llc::err_t testAPodTri2SelfAssignment(::llc::ATestError & results) {
	cnst ::llc::tri2u2_t expected = ::testAPodTri2U2(1);
	::llc::atri2u2_t values = {expected};
	::llc::tri2u2_t * originalData = values.begin();
	values = values;
	LLC_TEST_CHECK(results, TEST_APOD_TRI2_RESULT_SELF_ASSIGNMENT, values.size() != 1 || values.begin() != originalData || 0 != memcmp(values.begin(), &expected, szof(expected)));
	rtrn 0;
}

sttc ::llc::err_t testAPodTri2ReserveRejection(::llc::ATestError & results) {
	cnst ::llc::tri2u2_t expected = ::testAPodTri2U2(1);
	::llc::atri2u2_t values = {expected};
	::llc::tri2u2_t * originalData = values.begin();
	LLC_TEST_CHECK(results, TEST_APOD_TRI2_RESULT_RESERVE_REJECTION, 0 <= values.reserve((::llc::u2_t)-1));
	LLC_TEST_CHECK(results, TEST_APOD_TRI2_RESULT_RESERVE_REJECTION, values.size() != 1 || values.begin() != originalData || 0 != memcmp(values.begin(), &expected, szof(expected)));
	rtrn 0;
}

sttc ::llc::err_t testAPodTri2Boundaries(::llc::ATestError & results) {
	cnst ::llc::tri2u2_t middle = ::testAPodTri2U2(10);
	::llc::atri2u2_t values = {::testAPodTri2U2(1), middle, ::testAPodTri2U2(20)};
	LLC_TEST_REQUIRE(results, TEST_APOD_TRI2_RESULT_BOUNDARIES, 2 != values.remove(0));
	LLC_TEST_REQUIRE(results, TEST_APOD_TRI2_RESULT_BOUNDARIES, 1 != values.pop_back());
	LLC_TEST_CHECK(results, TEST_APOD_TRI2_RESULT_BOUNDARIES, values.size() != 1 || 0 != memcmp(values.begin(), &middle, szof(middle)));
	rtrn 0;
}

sttc ::llc::err_t testAPodTri2ViewAliasing(::llc::ATestError & results) {
	::llc::atri2u2_t values = {::testAPodTri2U2(1)};
	::llc::view<::llc::tri2u2_t> & view = values;
	view[0].C.y = 99;
	LLC_TEST_CHECK(results, TEST_APOD_TRI2_RESULT_VIEW_ALIASING, view.begin() != values.begin() || values[0].C.y != 99);
	rtrn 0;
}

sttc ::llc::err_t testAPodTri2Serialization(::llc::ATestError & results) {
	::llc::atri2u2_t source = {::testAPodTri2U2(1), ::testAPodTri2U2(10)};
	::llc::au0_t encoded;
	cnst ::llc::view<cnst ::llc::tri2u2_t> sourceView = source;
	LLC_TEST_REQUIRE(results, TEST_APOD_TRI2_RESULT_SERIALIZATION, 0 > ::llc::saveView(encoded, sourceView));
	::llc::vcu0_t input = encoded;
	::llc::atri2u2_t decoded;
	LLC_TEST_REQUIRE(results, TEST_APOD_TRI2_RESULT_SERIALIZATION, 0 != ::llc::loadView(input, decoded));
	LLC_TEST_CHECK(results, TEST_APOD_TRI2_RESULT_SERIALIZATION, input.size() || decoded.size() != source.size() || 0 != memcmp(decoded.begin(), source.begin(), source.byte_count()));
	rtrn 0;
}

::llc::err_t testAPodTri2(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testAPodTri2Aliases(results));
	result = ::llc::min(result, ::testAPodTri2ConstAliases(results));
	result = ::llc::min(result, ::testAPodTri2Empty(results));
	result = ::llc::min(result, ::testAPodTri2IntegerValues(results));
	result = ::llc::min(result, ::testAPodTri2FloatValues(results));
	result = ::llc::min(result, ::testAPodTri2Copy(results));
	result = ::llc::min(result, ::testAPodTri2Move(results));
	result = ::llc::min(result, ::testAPodTri2SelfAssignment(results));
	result = ::llc::min(result, ::testAPodTri2ReserveRejection(results));
	result = ::llc::min(result, ::testAPodTri2Boundaries(results));
	result = ::llc::min(result, ::testAPodTri2ViewAliasing(results));
	result = ::llc::min(result, ::testAPodTri2Serialization(results));
	rtrn result;
}
