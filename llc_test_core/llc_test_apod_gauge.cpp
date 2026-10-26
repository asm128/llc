#include "llc_test_core_suites.h"
#include "llc_apod_serialize.h"

#if __has_include("llc_apod_gauge.h")
#	include "llc_apod_gauge.h"
#	define LLC_TEST_HAS_APOD_GAUGE 1
#else
#	define LLC_TEST_HAS_APOD_GAUGE 0
#endif

GDEFINE_ENUM_TYPE(TEST_APOD_GAUGE_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_APOD_GAUGE_RESULT, HEADER_AVAILABLE ,  0, "The canonical LLC POD-gauge array header is available to the test target.");
GDEFINE_ENUM_VALUED(TEST_APOD_GAUGE_RESULT, ALIAS            ,  1, "The generic gauge-array alias preserves its declared element type.");
GDEFINE_ENUM_VALUED(TEST_APOD_GAUGE_RESULT, EMPTY            ,  2, "A gauge array begins empty.");
GDEFINE_ENUM_VALUED(TEST_APOD_GAUGE_RESULT, VALUES           ,  3, "Gauge elements retain their complete object representation.");
GDEFINE_ENUM_VALUED(TEST_APOD_GAUGE_RESULT, COPY             ,  4, "Copying a gauge array duplicates storage and values.");
GDEFINE_ENUM_VALUED(TEST_APOD_GAUGE_RESULT, MOVE             ,  5, "Moving a gauge array transfers storage and clears the source.");
GDEFINE_ENUM_VALUED(TEST_APOD_GAUGE_RESULT, SELF_ASSIGNMENT  ,  6, "Self-assignment preserves gauge-array storage and values.");
GDEFINE_ENUM_VALUED(TEST_APOD_GAUGE_RESULT, RESERVE_REJECTION,  7, "Rejecting an impossible reserve preserves the gauge array.");
GDEFINE_ENUM_VALUED(TEST_APOD_GAUGE_RESULT, BOUNDARIES       ,  8, "Removing the first and final gauges preserves the middle gauge.");
GDEFINE_ENUM_VALUED(TEST_APOD_GAUGE_RESULT, VIEW_ALIASING    ,  9, "A gauge-array view aliases the array's live storage.");
GDEFINE_ENUM_VALUED(TEST_APOD_GAUGE_RESULT, SERIALIZATION    , 10, "A gauge array round-trips through the canonical view serializer.");

sttc ::llc::err_t testAPodGaugeHeaderAvailable(::llc::ATestError & results) {
	LLC_TEST_CHECK(results, TEST_APOD_GAUGE_RESULT_HEADER_AVAILABLE, 0 == LLC_TEST_HAS_APOD_GAUGE);
	rtrn 0;
}

#if LLC_TEST_HAS_APOD_GAUGE
sttc ::llc::gauge<::llc::u2_t> testAPodGaugePattern(::llc::u0_t seed) {
	::llc::gauge<::llc::u2_t> value = {};
	::llc::u0_t * bytes = (::llc::u0_t*)&value;
	for(::llc::u2_t i = 0; i < szof(value); ++i)
		bytes[i] = (::llc::u0_t)(seed + i);
	rtrn value;
}

sttc ::llc::err_t testAPodGaugeAlias(::llc::ATestError & results) {
	::llc::apod<::llc::gauge<::llc::u2_t>> * expected = (::llc::agauge<::llc::u2_t>*)0;
	LLC_TEST_CHECK(results, TEST_APOD_GAUGE_RESULT_ALIAS, 0 != expected);
	rtrn 0;
}

sttc ::llc::err_t testAPodGaugeEmpty(::llc::ATestError & results) {
	::llc::agauge<::llc::u2_t> values;
	LLC_TEST_CHECK(results, TEST_APOD_GAUGE_RESULT_EMPTY, values.size() || values.begin());
	rtrn 0;
}

sttc ::llc::err_t testAPodGaugeValues(::llc::ATestError & results) {
	cnst ::llc::gauge<::llc::u2_t> first = ::testAPodGaugePattern(1);
	cnst ::llc::gauge<::llc::u2_t> second = ::testAPodGaugePattern(17);
	::llc::agauge<::llc::u2_t> values;
	LLC_TEST_REQUIRE(results, TEST_APOD_GAUGE_RESULT_VALUES, 0 != values.push_back(first));
	LLC_TEST_REQUIRE(results, TEST_APOD_GAUGE_RESULT_VALUES, 1 != values.push_back(second));
	LLC_TEST_CHECK(results, TEST_APOD_GAUGE_RESULT_VALUES, values.size() != 2 || 0 != memcmp(&values[0], &first, szof(first)) || 0 != memcmp(&values[1], &second, szof(second)));
	rtrn 0;
}

sttc ::llc::err_t testAPodGaugeCopy(::llc::ATestError & results) {
	::llc::agauge<::llc::u2_t> source = {::testAPodGaugePattern(1), ::testAPodGaugePattern(17)};
	::llc::agauge<::llc::u2_t> copied = source;
	LLC_TEST_CHECK(results, TEST_APOD_GAUGE_RESULT_COPY, copied.size() != source.size() || copied.begin() == source.begin() || 0 != memcmp(copied.begin(), source.begin(), source.byte_count()));
	rtrn 0;
}

sttc ::llc::err_t testAPodGaugeMove(::llc::ATestError & results) {
	::llc::agauge<::llc::u2_t> source = {::testAPodGaugePattern(1), ::testAPodGaugePattern(17)};
	::llc::gauge<::llc::u2_t> * sourceData = source.begin();
	::llc::agauge<::llc::u2_t> moved = (::llc::agauge<::llc::u2_t>&&)source;
	LLC_TEST_CHECK(results, TEST_APOD_GAUGE_RESULT_MOVE, source.size() || source.begin() || moved.size() != 2 || moved.begin() != sourceData);
	rtrn 0;
}

sttc ::llc::err_t testAPodGaugeSelfAssignment(::llc::ATestError & results) {
	cnst ::llc::gauge<::llc::u2_t> expected = ::testAPodGaugePattern(1);
	::llc::agauge<::llc::u2_t> values = {expected};
	::llc::gauge<::llc::u2_t> * originalData = values.begin();
	values = values;
	LLC_TEST_CHECK(results, TEST_APOD_GAUGE_RESULT_SELF_ASSIGNMENT, values.size() != 1 || values.begin() != originalData || 0 != memcmp(values.begin(), &expected, szof(expected)));
	rtrn 0;
}

sttc ::llc::err_t testAPodGaugeReserveRejection(::llc::ATestError & results) {
	cnst ::llc::gauge<::llc::u2_t> expected = ::testAPodGaugePattern(1);
	::llc::agauge<::llc::u2_t> values = {expected};
	::llc::gauge<::llc::u2_t> * originalData = values.begin();
	LLC_TEST_CHECK(results, TEST_APOD_GAUGE_RESULT_RESERVE_REJECTION, 0 <= values.reserve((::llc::u2_t)-1));
	LLC_TEST_CHECK(results, TEST_APOD_GAUGE_RESULT_RESERVE_REJECTION, values.size() != 1 || values.begin() != originalData || 0 != memcmp(values.begin(), &expected, szof(expected)));
	rtrn 0;
}

sttc ::llc::err_t testAPodGaugeBoundaries(::llc::ATestError & results) {
	cnst ::llc::gauge<::llc::u2_t> middle = ::testAPodGaugePattern(17);
	::llc::agauge<::llc::u2_t> values = {::testAPodGaugePattern(1), middle, ::testAPodGaugePattern(33)};
	LLC_TEST_REQUIRE(results, TEST_APOD_GAUGE_RESULT_BOUNDARIES, 2 != values.remove(0));
	LLC_TEST_REQUIRE(results, TEST_APOD_GAUGE_RESULT_BOUNDARIES, 1 != values.pop_back());
	LLC_TEST_CHECK(results, TEST_APOD_GAUGE_RESULT_BOUNDARIES, values.size() != 1 || 0 != memcmp(values.begin(), &middle, szof(middle)));
	rtrn 0;
}

sttc ::llc::err_t testAPodGaugeViewAliasing(::llc::ATestError & results) {
	::llc::agauge<::llc::u2_t> values = {::testAPodGaugePattern(1)};
	::llc::view<::llc::gauge<::llc::u2_t>> & view = values;
	((::llc::u0_t*)&view[0])[0] = 99;
	LLC_TEST_CHECK(results, TEST_APOD_GAUGE_RESULT_VIEW_ALIASING, view.begin() != values.begin() || ((::llc::u0_t*)&values[0])[0] != 99);
	rtrn 0;
}

sttc ::llc::err_t testAPodGaugeSerialization(::llc::ATestError & results) {
	::llc::agauge<::llc::u2_t> source = {::testAPodGaugePattern(1), ::testAPodGaugePattern(17)};
	::llc::au0_t encoded;
	cnst ::llc::view<cnst ::llc::gauge<::llc::u2_t>> sourceView = source;
	LLC_TEST_REQUIRE(results, TEST_APOD_GAUGE_RESULT_SERIALIZATION, 0 > ::llc::saveView(encoded, sourceView));
	::llc::vcu0_t input = encoded;
	::llc::agauge<::llc::u2_t> decoded;
	LLC_TEST_REQUIRE(results, TEST_APOD_GAUGE_RESULT_SERIALIZATION, 0 != ::llc::loadView(input, decoded));
	LLC_TEST_CHECK(results, TEST_APOD_GAUGE_RESULT_SERIALIZATION, input.size() || decoded.size() != source.size() || 0 != memcmp(decoded.begin(), source.begin(), source.byte_count()));
	rtrn 0;
}
#endif

::llc::err_t testAPodGauge(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = ::testAPodGaugeHeaderAvailable(results);
#if LLC_TEST_HAS_APOD_GAUGE
	result = ::llc::min(result, ::testAPodGaugeAlias(results));
	result = ::llc::min(result, ::testAPodGaugeEmpty(results));
	result = ::llc::min(result, ::testAPodGaugeValues(results));
	result = ::llc::min(result, ::testAPodGaugeCopy(results));
	result = ::llc::min(result, ::testAPodGaugeMove(results));
	result = ::llc::min(result, ::testAPodGaugeSelfAssignment(results));
	result = ::llc::min(result, ::testAPodGaugeReserveRejection(results));
	result = ::llc::min(result, ::testAPodGaugeBoundaries(results));
	result = ::llc::min(result, ::testAPodGaugeViewAliasing(results));
	result = ::llc::min(result, ::testAPodGaugeSerialization(results));
#endif
	rtrn result;
}
