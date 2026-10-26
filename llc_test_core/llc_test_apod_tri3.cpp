#include "llc_test_core_suites.h"
#include "llc_apod_tri3.h"
#include "llc_apod_serialize.h"

GDEFINE_ENUM_TYPE(TEST_APOD_TRI3_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_APOD_TRI3_RESULT, ALIASES          ,  0, "Every mutable tri3 array alias preserves its declared element type.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI3_RESULT, CONST_ALIASES    ,  1, "Every const tri3 array alias preserves the matching const array type.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI3_RESULT, EMPTY            ,  2, "A tri3 array begins empty.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI3_RESULT, INTEGER_VALUES   ,  3, "Integer tri3 elements retain all nine coordinates.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI3_RESULT, FLOAT_VALUES     ,  4, "Floating-point tri3 elements retain all nine coordinates.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI3_RESULT, COPY             ,  5, "Copying a tri3 array duplicates storage and values.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI3_RESULT, MOVE             ,  6, "Moving a tri3 array transfers storage and clears the source.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI3_RESULT, SELF_ASSIGNMENT  ,  7, "Self-assignment preserves tri3-array storage and values.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI3_RESULT, RESERVE_REJECTION,  8, "Rejecting an impossible reserve preserves the existing tri3 array.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI3_RESULT, BOUNDARIES       ,  9, "Removing the first and final tri3 values preserves the middle value.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI3_RESULT, VIEW_ALIASING    , 10, "A tri3 view aliases the array's live storage.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI3_RESULT, SERIALIZATION    , 11, "A tri3 array round-trips through the canonical view serializer.");

sttc ::llc::tri3<::llc::u2_t> testAPodTri3U2(::llc::u2_t base) {
	rtrn {{base + 0, base + 1, base + 2}, {base + 3, base + 4, base + 5}, {base + 6, base + 7, base + 8}};
}

sttc ::llc::tri3<::llc::f2_t> testAPodTri3F2(::llc::f2_t base) {
	rtrn {{base + 0.25f, base + 0.50f, base + 0.75f}, {base + 1.00f, base + 1.25f, base + 1.50f}, {base + 1.75f, base + 2.00f, base + 2.25f}};
}

sttc ::llc::err_t testAPodTri3Aliases(::llc::ATestError & results) {
	::llc::apod<::llc::tri3<::llc::sc_t>> * asc = (::llc::atri3sc_t*)0;
	::llc::apod<::llc::tri3<::llc::uc_t>> * auc = (::llc::atri3uc_t*)0;
	::llc::apod<::llc::tri3<::llc::u0_t>> * au0 = (::llc::atri3u0_t*)0;
	::llc::apod<::llc::tri3<::llc::u1_t>> * au1 = (::llc::atri3u1_t*)0;
	::llc::apod<::llc::tri3<::llc::u2_t>> * au2 = (::llc::atri3u2_t*)0;
	::llc::apod<::llc::tri3<::llc::u3_t>> * au3 = (::llc::atri3u3_t*)0;
	::llc::apod<::llc::tri3<::llc::s0_t>> * as0 = (::llc::atri3s0_t*)0;
	::llc::apod<::llc::tri3<::llc::s1_t>> * as1 = (::llc::atri3s1_t*)0;
	::llc::apod<::llc::tri3<::llc::s2_t>> * as2 = (::llc::atri3s2_t*)0;
	::llc::apod<::llc::tri3<::llc::s3_t>> * as3 = (::llc::atri3s3_t*)0;
	::llc::apod<::llc::tri3<::llc::f2_t>> * af2 = (::llc::atri3f2_t*)0;
	::llc::apod<::llc::tri3<::llc::f3_t>> * af3 = (::llc::atri3f3_t*)0;
	LLC_TEST_CHECK(results, TEST_APOD_TRI3_RESULT_ALIASES, asc || auc || au0 || au1 || au2 || au3 || as0 || as1 || as2 || as3 || af2 || af3);
	rtrn 0;
}

sttc ::llc::err_t testAPodTri3ConstAliases(::llc::ATestError & results) {
	cnst ::llc::atri3<::llc::sc_t> * asc = (::llc::atri3sc_c*)0;
	cnst ::llc::atri3<::llc::uc_t> * auc = (::llc::atri3uc_c*)0;
	cnst ::llc::atri3<::llc::u0_t> * au0 = (::llc::atri3u0_c*)0;
	cnst ::llc::atri3<::llc::u1_t> * au1 = (::llc::atri3u1_c*)0;
	cnst ::llc::atri3<::llc::u2_t> * au2 = (::llc::atri3u2_c*)0;
	cnst ::llc::atri3<::llc::u3_t> * au3 = (::llc::atri3u3_c*)0;
	cnst ::llc::atri3<::llc::s0_t> * as0 = (::llc::atri3s0_c*)0;
	cnst ::llc::atri3<::llc::s1_t> * as1 = (::llc::atri3s1_c*)0;
	cnst ::llc::atri3<::llc::s2_t> * as2 = (::llc::atri3s2_c*)0;
	cnst ::llc::atri3<::llc::s3_t> * as3 = (::llc::atri3s3_c*)0;
	cnst ::llc::atri3<::llc::f2_t> * af2 = (::llc::atri3f2_c*)0;
	cnst ::llc::atri3<::llc::f3_t> * af3 = (::llc::atri3f3_c*)0;
	LLC_TEST_CHECK(results, TEST_APOD_TRI3_RESULT_CONST_ALIASES, asc || auc || au0 || au1 || au2 || au3 || as0 || as1 || as2 || as3 || af2 || af3);
	rtrn 0;
}

sttc ::llc::err_t testAPodTri3Empty(::llc::ATestError & results) {
	::llc::atri3u2_t values;
	LLC_TEST_CHECK(results, TEST_APOD_TRI3_RESULT_EMPTY, values.size() || values.begin());
	rtrn 0;
}

sttc ::llc::err_t testAPodTri3IntegerValues(::llc::ATestError & results) {
	cnst ::llc::tri3<::llc::u2_t> first = ::testAPodTri3U2(1), second = ::testAPodTri3U2(20);
	::llc::atri3u2_t values;
	LLC_TEST_REQUIRE(results, TEST_APOD_TRI3_RESULT_INTEGER_VALUES, 0 != values.push_back(first));
	LLC_TEST_REQUIRE(results, TEST_APOD_TRI3_RESULT_INTEGER_VALUES, 1 != values.push_back(second));
	LLC_TEST_CHECK(results, TEST_APOD_TRI3_RESULT_INTEGER_VALUES, 0 != memcmp(&values[0], &first, szof(first)) || 0 != memcmp(&values[1], &second, szof(second)));
	rtrn 0;
}

sttc ::llc::err_t testAPodTri3FloatValues(::llc::ATestError & results) {
	cnst ::llc::tri3<::llc::f2_t> first = ::testAPodTri3F2(0), second = ::testAPodTri3F2(10);
	::llc::atri3f2_t values;
	LLC_TEST_REQUIRE(results, TEST_APOD_TRI3_RESULT_FLOAT_VALUES, 0 != values.push_back(first));
	LLC_TEST_REQUIRE(results, TEST_APOD_TRI3_RESULT_FLOAT_VALUES, 1 != values.push_back(second));
	LLC_TEST_CHECK(results, TEST_APOD_TRI3_RESULT_FLOAT_VALUES, 0 != memcmp(&values[0], &first, szof(first)) || 0 != memcmp(&values[1], &second, szof(second)));
	rtrn 0;
}

sttc ::llc::err_t testAPodTri3Copy(::llc::ATestError & results) {
	::llc::atri3u2_t source = {::testAPodTri3U2(1), ::testAPodTri3U2(20)};
	::llc::atri3u2_t copied = source;
	LLC_TEST_CHECK(results, TEST_APOD_TRI3_RESULT_COPY, copied.size() != source.size() || copied.begin() == source.begin() || 0 != memcmp(copied.begin(), source.begin(), source.byte_count()));
	rtrn 0;
}

sttc ::llc::err_t testAPodTri3Move(::llc::ATestError & results) {
	::llc::atri3u2_t source = {::testAPodTri3U2(1), ::testAPodTri3U2(20)};
	::llc::tri3<::llc::u2_t> * sourceData = source.begin();
	::llc::atri3u2_t moved = (::llc::atri3u2_t&&)source;
	LLC_TEST_CHECK(results, TEST_APOD_TRI3_RESULT_MOVE, source.size() || source.begin() || moved.size() != 2 || moved.begin() != sourceData);
	rtrn 0;
}

sttc ::llc::err_t testAPodTri3SelfAssignment(::llc::ATestError & results) {
	cnst ::llc::tri3<::llc::u2_t> expected = ::testAPodTri3U2(1);
	::llc::atri3u2_t values = {expected};
	::llc::tri3<::llc::u2_t> * originalData = values.begin();
	values = values;
	LLC_TEST_CHECK(results, TEST_APOD_TRI3_RESULT_SELF_ASSIGNMENT, values.size() != 1 || values.begin() != originalData || 0 != memcmp(values.begin(), &expected, szof(expected)));
	rtrn 0;
}

sttc ::llc::err_t testAPodTri3ReserveRejection(::llc::ATestError & results) {
	cnst ::llc::tri3<::llc::u2_t> expected = ::testAPodTri3U2(1);
	::llc::atri3u2_t values = {expected};
	::llc::tri3<::llc::u2_t> * originalData = values.begin();
	LLC_TEST_CHECK(results, TEST_APOD_TRI3_RESULT_RESERVE_REJECTION, 0 <= values.reserve((::llc::u2_t)-1));
	LLC_TEST_CHECK(results, TEST_APOD_TRI3_RESULT_RESERVE_REJECTION, values.size() != 1 || values.begin() != originalData || 0 != memcmp(values.begin(), &expected, szof(expected)));
	rtrn 0;
}

sttc ::llc::err_t testAPodTri3Boundaries(::llc::ATestError & results) {
	cnst ::llc::tri3<::llc::u2_t> middle = ::testAPodTri3U2(20);
	::llc::atri3u2_t values = {::testAPodTri3U2(1), middle, ::testAPodTri3U2(40)};
	LLC_TEST_REQUIRE(results, TEST_APOD_TRI3_RESULT_BOUNDARIES, 2 != values.remove(0));
	LLC_TEST_REQUIRE(results, TEST_APOD_TRI3_RESULT_BOUNDARIES, 1 != values.pop_back());
	LLC_TEST_CHECK(results, TEST_APOD_TRI3_RESULT_BOUNDARIES, values.size() != 1 || 0 != memcmp(values.begin(), &middle, szof(middle)));
	rtrn 0;
}

sttc ::llc::err_t testAPodTri3ViewAliasing(::llc::ATestError & results) {
	::llc::atri3u2_t values = {::testAPodTri3U2(1)};
	::llc::view<::llc::tri3<::llc::u2_t>> & view = values;
	view[0].C.z = 99;
	LLC_TEST_CHECK(results, TEST_APOD_TRI3_RESULT_VIEW_ALIASING, view.begin() != values.begin() || values[0].C.z != 99);
	rtrn 0;
}

sttc ::llc::err_t testAPodTri3Serialization(::llc::ATestError & results) {
	::llc::atri3u2_t source = {::testAPodTri3U2(1), ::testAPodTri3U2(20)};
	::llc::au0_t encoded;
	cnst ::llc::view<cnst ::llc::tri3<::llc::u2_t>> sourceView = source;
	LLC_TEST_REQUIRE(results, TEST_APOD_TRI3_RESULT_SERIALIZATION, 0 > ::llc::saveView(encoded, sourceView));
	::llc::vcu0_t input = encoded;
	::llc::atri3u2_t decoded;
	LLC_TEST_REQUIRE(results, TEST_APOD_TRI3_RESULT_SERIALIZATION, 0 != ::llc::loadView(input, decoded));
	LLC_TEST_CHECK(results, TEST_APOD_TRI3_RESULT_SERIALIZATION, input.size() || decoded.size() != source.size() || 0 != memcmp(decoded.begin(), source.begin(), source.byte_count()));
	rtrn 0;
}

::llc::err_t testAPodTri3(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testAPodTri3Aliases(results));
	result = ::llc::min(result, ::testAPodTri3ConstAliases(results));
	result = ::llc::min(result, ::testAPodTri3Empty(results));
	result = ::llc::min(result, ::testAPodTri3IntegerValues(results));
	result = ::llc::min(result, ::testAPodTri3FloatValues(results));
	result = ::llc::min(result, ::testAPodTri3Copy(results));
	result = ::llc::min(result, ::testAPodTri3Move(results));
	result = ::llc::min(result, ::testAPodTri3SelfAssignment(results));
	result = ::llc::min(result, ::testAPodTri3ReserveRejection(results));
	result = ::llc::min(result, ::testAPodTri3Boundaries(results));
	result = ::llc::min(result, ::testAPodTri3ViewAliasing(results));
	result = ::llc::min(result, ::testAPodTri3Serialization(results));
	rtrn result;
}
