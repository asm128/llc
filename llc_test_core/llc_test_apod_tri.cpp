#include "llc_test_core_suites.h"
#include "llc_apod_tri.h"
#include "llc_apod_serialize.h"

GDEFINE_ENUM_TYPE(TEST_APOD_TRI_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_APOD_TRI_RESULT, ALIASES          ,  0, "Every mutable triangle array alias preserves its declared element type.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI_RESULT, CONST_ALIASES    ,  1, "Every const triangle array alias preserves the matching const array type.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI_RESULT, EMPTY            ,  2, "A triangle array begins empty.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI_RESULT, INTEGER_VALUES   ,  3, "Integer triangles retain all three vertices.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI_RESULT, FLOAT_VALUES     ,  4, "Floating-point triangles retain all three vertices.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI_RESULT, COPY             ,  5, "Copying a triangle array duplicates storage and values.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI_RESULT, MOVE             ,  6, "Moving a triangle array transfers storage and clears the source.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI_RESULT, SELF_ASSIGNMENT  ,  7, "Self-assignment preserves triangle-array storage and values.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI_RESULT, RESERVE_REJECTION,  8, "Rejecting an impossible reserve preserves the existing triangle array.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI_RESULT, BOUNDARIES       ,  9, "Removing the first and final triangles preserves the middle triangle.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI_RESULT, VIEW_ALIASING    , 10, "A triangle view aliases the array's live storage.");
GDEFINE_ENUM_VALUED(TEST_APOD_TRI_RESULT, SERIALIZATION    , 11, "A triangle array round-trips through the canonical view serializer.");

sttc ::llc::err_t testAPodTriAliases(::llc::ATestError & results) {
	::llc::apod<::llc::tri<::llc::sc_t>> * asc = (::llc::atrisc_t*)0;
	::llc::apod<::llc::tri<::llc::uc_t>> * auc = (::llc::atriuc_t*)0;
	::llc::apod<::llc::tri<::llc::u0_t>> * au0 = (::llc::atriu0_t*)0;
	::llc::apod<::llc::tri<::llc::u1_t>> * au1 = (::llc::atriu1_t*)0;
	::llc::apod<::llc::tri<::llc::u2_t>> * au2 = (::llc::atriu2_t*)0;
	::llc::apod<::llc::tri<::llc::u3_t>> * au3 = (::llc::atriu3_t*)0;
	::llc::apod<::llc::tri<::llc::s0_t>> * as0 = (::llc::atris0_t*)0;
	::llc::apod<::llc::tri<::llc::s1_t>> * as1 = (::llc::atris1_t*)0;
	::llc::apod<::llc::tri<::llc::s2_t>> * as2 = (::llc::atris2_t*)0;
	::llc::apod<::llc::tri<::llc::s3_t>> * as3 = (::llc::atris3_t*)0;
	::llc::apod<::llc::tri<::llc::f2_t>> * af2 = (::llc::atrif2_t*)0;
	::llc::apod<::llc::tri<::llc::f3_t>> * af3 = (::llc::atrif3_t*)0;
	LLC_TEST_CHECK(results, TEST_APOD_TRI_RESULT_ALIASES, asc || auc || au0 || au1 || au2 || au3 || as0 || as1 || as2 || as3 || af2 || af3);
	rtrn 0;
}

sttc ::llc::err_t testAPodTriConstAliases(::llc::ATestError & results) {
	cnst ::llc::atri<::llc::sc_t> * asc = (::llc::atrisc_c*)0;
	cnst ::llc::atri<::llc::uc_t> * auc = (::llc::atriuc_c*)0;
	cnst ::llc::atri<::llc::u0_t> * au0 = (::llc::atriu0_c*)0;
	cnst ::llc::atri<::llc::u1_t> * au1 = (::llc::atriu1_c*)0;
	cnst ::llc::atri<::llc::u2_t> * au2 = (::llc::atriu2_c*)0;
	cnst ::llc::atri<::llc::u3_t> * au3 = (::llc::atriu3_c*)0;
	cnst ::llc::atri<::llc::s0_t> * as0 = (::llc::atris0_c*)0;
	cnst ::llc::atri<::llc::s1_t> * as1 = (::llc::atris1_c*)0;
	cnst ::llc::atri<::llc::s2_t> * as2 = (::llc::atris2_c*)0;
	cnst ::llc::atri<::llc::s3_t> * as3 = (::llc::atris3_c*)0;
	cnst ::llc::atri<::llc::f2_t> * af2 = (::llc::atrif2_c*)0;
	cnst ::llc::atri<::llc::f3_t> * af3 = (::llc::atrif3_c*)0;
	LLC_TEST_CHECK(results, TEST_APOD_TRI_RESULT_CONST_ALIASES, asc || auc || au0 || au1 || au2 || au3 || as0 || as1 || as2 || as3 || af2 || af3);
	rtrn 0;
}

sttc ::llc::err_t testAPodTriEmpty(::llc::ATestError & results) {
	::llc::atriu2_t values;
	LLC_TEST_CHECK(results, TEST_APOD_TRI_RESULT_EMPTY, values.size() || values.begin());
	rtrn 0;
}

sttc ::llc::err_t testAPodTriIntegerValues(::llc::ATestError & results) {
	::llc::atriu2_t values;
	LLC_TEST_REQUIRE(results, TEST_APOD_TRI_RESULT_INTEGER_VALUES, 0 != values.push_back({1, 2, 3}));
	LLC_TEST_REQUIRE(results, TEST_APOD_TRI_RESULT_INTEGER_VALUES, 1 != values.push_back({10, 20, 30}));
	LLC_TEST_CHECK(results, TEST_APOD_TRI_RESULT_INTEGER_VALUES, values[0].A != 1 || values[0].B != 2 || values[0].C != 3 || values[1].A != 10 || values[1].B != 20 || values[1].C != 30);
	rtrn 0;
}

sttc ::llc::err_t testAPodTriFloatValues(::llc::ATestError & results) {
	::llc::atrif2_t values;
	LLC_TEST_REQUIRE(results, TEST_APOD_TRI_RESULT_FLOAT_VALUES, 0 != values.push_back({0.25f, 0.50f, 0.75f}));
	LLC_TEST_REQUIRE(results, TEST_APOD_TRI_RESULT_FLOAT_VALUES, 1 != values.push_back({-1.5f, 2.5f, 3.5f}));
	LLC_TEST_CHECK(results, TEST_APOD_TRI_RESULT_FLOAT_VALUES, values[0].A != 0.25f || values[0].B != 0.50f || values[0].C != 0.75f || values[1].A != -1.5f || values[1].B != 2.5f || values[1].C != 3.5f);
	rtrn 0;
}

sttc ::llc::err_t testAPodTriCopy(::llc::ATestError & results) {
	::llc::atriu2_t source = {{1, 2, 3}, {10, 20, 30}};
	::llc::atriu2_t copied = source;
	LLC_TEST_CHECK(results, TEST_APOD_TRI_RESULT_COPY, copied.size() != source.size() || copied.begin() == source.begin() || 0 != memcmp(copied.begin(), source.begin(), source.byte_count()));
	rtrn 0;
}

sttc ::llc::err_t testAPodTriMove(::llc::ATestError & results) {
	::llc::atriu2_t source = {{1, 2, 3}, {10, 20, 30}};
	::llc::tri<::llc::u2_t> * sourceData = source.begin();
	::llc::atriu2_t moved = (::llc::atriu2_t&&)source;
	LLC_TEST_CHECK(results, TEST_APOD_TRI_RESULT_MOVE, source.size() || source.begin() || moved.size() != 2 || moved.begin() != sourceData);
	rtrn 0;
}

sttc ::llc::err_t testAPodTriSelfAssignment(::llc::ATestError & results) {
	::llc::atriu2_t values = {{1, 2, 3}, {10, 20, 30}};
	::llc::tri<::llc::u2_t> * originalData = values.begin();
	values = values;
	LLC_TEST_CHECK(results, TEST_APOD_TRI_RESULT_SELF_ASSIGNMENT, values.size() != 2 || values.begin() != originalData || values[0].A != 1 || values[0].C != 3 || values[1].B != 20);
	rtrn 0;
}

sttc ::llc::err_t testAPodTriReserveRejection(::llc::ATestError & results) {
	::llc::atriu2_t values = {{1, 2, 3}};
	::llc::tri<::llc::u2_t> * originalData = values.begin();
	LLC_TEST_CHECK(results, TEST_APOD_TRI_RESULT_RESERVE_REJECTION, 0 <= values.reserve((::llc::u2_t)-1));
	LLC_TEST_CHECK(results, TEST_APOD_TRI_RESULT_RESERVE_REJECTION, values.size() != 1 || values.begin() != originalData || values[0].A != 1 || values[0].B != 2 || values[0].C != 3);
	rtrn 0;
}

sttc ::llc::err_t testAPodTriBoundaries(::llc::ATestError & results) {
	::llc::atriu2_t values = {{1, 2, 3}, {10, 20, 30}, {40, 50, 60}};
	LLC_TEST_REQUIRE(results, TEST_APOD_TRI_RESULT_BOUNDARIES, 2 != values.remove(0));
	LLC_TEST_CHECK(results, TEST_APOD_TRI_RESULT_BOUNDARIES, values[0].A != 10 || values[1].C != 60);
	LLC_TEST_REQUIRE(results, TEST_APOD_TRI_RESULT_BOUNDARIES, 1 != values.pop_back());
	LLC_TEST_CHECK(results, TEST_APOD_TRI_RESULT_BOUNDARIES, values.size() != 1 || values[0].A != 10 || values[0].B != 20 || values[0].C != 30);
	rtrn 0;
}

sttc ::llc::err_t testAPodTriViewAliasing(::llc::ATestError & results) {
	::llc::atriu2_t values = {{1, 2, 3}};
	::llc::view<::llc::tri<::llc::u2_t>> & view = values;
	view[0].C = 7;
	LLC_TEST_CHECK(results, TEST_APOD_TRI_RESULT_VIEW_ALIASING, view.begin() != values.begin() || values[0].A != 1 || values[0].B != 2 || values[0].C != 7);
	rtrn 0;
}

sttc ::llc::err_t testAPodTriSerialization(::llc::ATestError & results) {
	::llc::atriu2_t source = {{1, 2, 3}, {10, 20, 30}};
	::llc::au0_t encoded;
	cnst ::llc::view<cnst ::llc::tri<::llc::u2_t>> sourceView = source;
	LLC_TEST_REQUIRE(results, TEST_APOD_TRI_RESULT_SERIALIZATION, 0 > ::llc::saveView(encoded, sourceView));
	::llc::vcu0_t input = encoded;
	::llc::atriu2_t decoded;
	LLC_TEST_REQUIRE(results, TEST_APOD_TRI_RESULT_SERIALIZATION, 0 != ::llc::loadView(input, decoded));
	LLC_TEST_CHECK(results, TEST_APOD_TRI_RESULT_SERIALIZATION, input.size() || decoded.size() != source.size() || 0 != memcmp(decoded.begin(), source.begin(), source.byte_count()));
	rtrn 0;
}

::llc::err_t testAPodTri(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testAPodTriAliases(results));
	result = ::llc::min(result, ::testAPodTriConstAliases(results));
	result = ::llc::min(result, ::testAPodTriEmpty(results));
	result = ::llc::min(result, ::testAPodTriIntegerValues(results));
	result = ::llc::min(result, ::testAPodTriFloatValues(results));
	result = ::llc::min(result, ::testAPodTriCopy(results));
	result = ::llc::min(result, ::testAPodTriMove(results));
	result = ::llc::min(result, ::testAPodTriSelfAssignment(results));
	result = ::llc::min(result, ::testAPodTriReserveRejection(results));
	result = ::llc::min(result, ::testAPodTriBoundaries(results));
	result = ::llc::min(result, ::testAPodTriViewAliasing(results));
	result = ::llc::min(result, ::testAPodTriSerialization(results));
	rtrn result;
}
