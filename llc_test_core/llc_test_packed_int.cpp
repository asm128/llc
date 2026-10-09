#include "llc_test_core.h"
#include "llc_view_serialize.h"
#include "llc_noise.h"

static_assert(1 == ::llc::uint_width_field_size<::llc::u0_t>(), "u8 width field");
static_assert(1 == ::llc::uint_width_field_size<::llc::u1_t>(), "u16 width field");
static_assert(2 == ::llc::uint_width_field_size<::llc::u2_t>(), "u32 width field");
static_assert(3 == ::llc::uint_width_field_size<::llc::u3_t>(), "u64 width field");

GDEFINE_ENUM_TYPE(PACKED_UINT_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(PACKED_UINT_TEST_RESULT, OK				, 0, "All packed-uint tests passed.");
GDEFINE_ENUM_VALUED(PACKED_UINT_TEST_RESULT, DEFAULT_VALUE		, 1, "A default packed integer did not represent zero in one byte.");
GDEFINE_ENUM_VALUED(PACKED_UINT_TEST_RESULT, TAIL_WIDTH		, 2, "The packed tail width did not match the value's width band.");
GDEFINE_ENUM_VALUED(PACKED_UINT_TEST_RESULT, VALUE_WIDTH		, 3, "The serialized width did not equal one byte plus the tail width.");
GDEFINE_ENUM_VALUED(PACKED_UINT_TEST_RESULT, MULTIPLIER		, 4, "The first-byte multiplier did not contain the value's high bits.");
GDEFINE_ENUM_VALUED(PACKED_UINT_TEST_RESULT, TAIL_BASE			, 5, "The tail helper did not contain the value's low bytes.");
GDEFINE_ENUM_VALUED(PACKED_UINT_TEST_RESULT, VALUE_ROUND_TRIP	, 6, "The packed integer did not reconstruct its source value.");
GDEFINE_ENUM_VALUED(PACKED_UINT_TEST_RESULT, BYTE_VIEW			, 7, "The serialization byte view reported the wrong address or width.");
GDEFINE_ENUM_VALUED(PACKED_UINT_TEST_RESULT, LOAD_RESULT		, 8, "loadUInt() did not report the packed byte width.");
GDEFINE_ENUM_VALUED(PACKED_UINT_TEST_RESULT, LOAD_VALUE			, 9, "loadUInt() did not reconstruct the packed source value.");
GDEFINE_ENUM_VALUED(PACKED_UINT_TEST_RESULT, LOAD_CONSUMPTION	, 10, "loadUInt() did not consume exactly one packed integer.");
GDEFINE_ENUM_VALUED(PACKED_UINT_TEST_RESULT, INVALID_VALUE		, 11, "packed_uint<> accepted a value outside its representable range.");

stxp ::llc::u3_t PACKED_RANDOM_SEED	= 0x5041434B45445549ULL;
stxp ::llc::u2_t PACKED_RANDOM_COUNT	= 256;

tplt<tpnm T>
sttc ::llc::err_t testDefault(ATestError & errors) {
	cnst ::llc::packed_uint<T> packed{};
	LLC_TEST_CHECK(errors, PACKED_UINT_TEST_RESULT_DEFAULT_VALUE, packed.TailWidth
		, "%u-bit default tail width:%u, expected:0.", bcof(T), ::llc::u2_t(packed.TailWidth));
	LLC_TEST_CHECK(errors, PACKED_UINT_TEST_RESULT_DEFAULT_VALUE, packed.Multiplier
		, "%u-bit default multiplier:%u, expected:0.", bcof(T), ::llc::u2_t(packed.Multiplier));
	LLC_TEST_CHECK(errors, PACKED_UINT_TEST_RESULT_DEFAULT_VALUE, packed.ValueWidth() != 1
		, "%u-bit default value width:%u, expected:1.", bcof(T), ::llc::u2_t(packed.ValueWidth()));
	LLC_TEST_CHECK(errors, PACKED_UINT_TEST_RESULT_DEFAULT_VALUE, packed.Value()
		, "%u-bit default value:%" LLC_FMT_U3 ", expected:0.", bcof(T), ::llc::u3_t(packed.Value()));
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testInvalidValue(ATestError & errors) {
#ifdef LLC_WINDOWS
	stxp ::llc::u0_t WIDTH_FIELD = ::llc::uint_width_field_size<T>();
	cnst T maximum = T(-1) >> WIDTH_FIELD;
	cnst T invalid = maximum + 1;
	LLC_TEST_CHECK(errors, PACKED_UINT_TEST_RESULT_INVALID_VALUE, !testThrows([&]() { cnst ::llc::packed_uint<T> packed{invalid}; (void)packed; })
		, "%u-bit packed_uint<> accepted value:%" LLC_FMT_U3 " above maximum:%" LLC_FMT_U3 "."
		, ::llc::u2_t(szof(T) * 8), ::llc::u3_t(invalid), ::llc::u3_t(maximum)
		);
#else
	(void)errors;
#endif
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testValue(ATestError & errors, ::llc::u3_c source, ::llc::u0_c expectedTailWidth) {
	cnst T value = (T)source;
	cnst ::llc::packed_uint<T> packed{value};
	::llc::u3_c expectedMultiplier = source >> (expectedTailWidth * 8);
	::llc::u3_c expectedTail = expectedTailWidth ? source & ((::llc::u3_t(1) << (expectedTailWidth * 8)) - 1) : source;
	::llc::u0_c expectedValueWidth = 1 + expectedTailWidth;
	::llc::u2_c typeBits = szof(T) * 8;

	LLC_TEST_CHECK(errors, PACKED_UINT_TEST_RESULT_TAIL_WIDTH, packed.TailWidth != expectedTailWidth
		, "%u-bit packed tail width:%u, expected:%u for value:%" LLC_FMT_U3 "."
		, typeBits, ::llc::u2_t(packed.TailWidth), ::llc::u2_t(expectedTailWidth), source);
	LLC_TEST_CHECK(errors, PACKED_UINT_TEST_RESULT_TAIL_WIDTH, ::llc::uint_tail_width(value) != expectedTailWidth
		, "%u-bit helper tail width:%u, expected:%u for value:%" LLC_FMT_U3 "."
		, typeBits, ::llc::u2_t(::llc::uint_tail_width(value)), ::llc::u2_t(expectedTailWidth), source);
	LLC_TEST_CHECK(errors, PACKED_UINT_TEST_RESULT_VALUE_WIDTH, packed.ValueWidth() != expectedValueWidth
		, "%u-bit packed value width:%u, expected:%u for value:%" LLC_FMT_U3 "."
		, typeBits, ::llc::u2_t(packed.ValueWidth()), ::llc::u2_t(expectedValueWidth), source);
	LLC_TEST_CHECK(errors, PACKED_UINT_TEST_RESULT_VALUE_WIDTH, ::llc::uint_value_width(value) != expectedValueWidth
		, "%u-bit helper value width:%u, expected:%u for value:%" LLC_FMT_U3 "."
		, typeBits, ::llc::u2_t(::llc::uint_value_width(value)), ::llc::u2_t(expectedValueWidth), source);
	LLC_TEST_CHECK(errors, PACKED_UINT_TEST_RESULT_MULTIPLIER, ::llc::u3_t(packed.Multiplier) != expectedMultiplier
		, "%u-bit packed multiplier:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 " for value:%" LLC_FMT_U3 "."
		, typeBits, ::llc::u3_t(packed.Multiplier), expectedMultiplier, source);
	LLC_TEST_CHECK(errors, PACKED_UINT_TEST_RESULT_MULTIPLIER, ::llc::u3_t(::llc::uint_tail_multiplier(value)) != expectedMultiplier
		, "%u-bit helper multiplier:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 " for value:%" LLC_FMT_U3 "."
		, typeBits, ::llc::u3_t(::llc::uint_tail_multiplier(value)), expectedMultiplier, source);
	LLC_TEST_CHECK(errors, PACKED_UINT_TEST_RESULT_TAIL_BASE, ::llc::u3_t(::llc::uint_tail_base(value)) != expectedTail
		, "%u-bit tail mismatch for value:%" LLC_FMT_U3 ". helper:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "."
		, typeBits, source, ::llc::u3_t(::llc::uint_tail_base(value)), expectedTail
		);
	LLC_TEST_CHECK(errors, PACKED_UINT_TEST_RESULT_VALUE_ROUND_TRIP, ::llc::u3_t(packed.Value()) != source
		, "%u-bit round-trip mismatch. source:%" LLC_FMT_U3 ", unpacked:%" LLC_FMT_U3 "."
		, typeBits, source, ::llc::u3_t(packed.Value())
		);

	cnst ::llc::vcu0_t bytes = packed.tplt cu8<::llc::vcu0_t>();
	LLC_TEST_REQUIRE(errors, PACKED_UINT_TEST_RESULT_BYTE_VIEW, (::llc::uP_t)bytes.begin() != (::llc::uP_t)&packed
		, "%u-bit byte view began at:%p, expected:%p for value:%" LLC_FMT_U3 "."
		, typeBits, bytes.begin(), &packed, source);
	LLC_TEST_REQUIRE(errors, PACKED_UINT_TEST_RESULT_BYTE_VIEW, bytes.size() > szof(packed)
		, "%u-bit byte view size:%u exceeds packed storage:%u for value:%" LLC_FMT_U3 "."
		, typeBits, bytes.size(), ::llc::u2_t(szof(packed)), source);
	LLC_TEST_CHECK(errors, PACKED_UINT_TEST_RESULT_BYTE_VIEW, bytes.size() != expectedValueWidth
		, "%u-bit byte-view mismatch for value:%" LLC_FMT_U3 ". begin:%p, expected begin:%p, size:%u, expected size:%u."
		, typeBits, source, bytes.begin(), &packed, bytes.size(), ::llc::u2_t(expectedValueWidth)
		);

	::llc::vcu0_t input = bytes;
	T loaded = {};
	cnst ::llc::err_t loadResult = ::llc::loadUInt(input, loaded);
	LLC_TEST_CHECK(errors, PACKED_UINT_TEST_RESULT_LOAD_RESULT, loadResult != expectedValueWidth
		, "%u-bit load result mismatch for value:%" LLC_FMT_U3 ". result:%i, expected:%u."
		, typeBits, source, loadResult, ::llc::u2_t(expectedValueWidth)
		);
	LLC_TEST_CHECK(errors, PACKED_UINT_TEST_RESULT_LOAD_VALUE, ::llc::u3_t(loaded) != source
		, "%u-bit loaded-value mismatch. source:%" LLC_FMT_U3 ", loaded:%" LLC_FMT_U3 "."
		, typeBits, source, ::llc::u3_t(loaded)
		);
	LLC_TEST_CHECK(errors, PACKED_UINT_TEST_RESULT_LOAD_CONSUMPTION, input.size()
		, "%u-bit load left bytes behind for value:%" LLC_FMT_U3 ". remaining:%u, expected:0."
		, typeBits, source, input.size()
		);
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testType(ATestError & errors) {
	if_fail_fe(testDefault<T>(errors));
	if_fail_fe(testInvalidValue<T>(errors));

	stxp ::llc::u0_t MULTIPLIER_BITS = 8 - ::llc::uint_width_field_size<T>();
	::llc::u3_t previousMaximum = 0;
	for(::llc::u0_t tailWidth = 0; tailWidth < szof(T); ++tailWidth) {
		::llc::u0_c valueBits = MULTIPLIER_BITS + tailWidth * 8;
		::llc::u3_c maximum = (::llc::u3_t(1) << valueBits) - 1;
		::llc::u3_c minimum = tailWidth ? previousMaximum + 1 : 0;
		::llc::u3_c sample = minimum + (maximum - minimum) / 3;
		if_fail_fe(testValue<T>(errors, minimum, tailWidth));
		if(sample != minimum && sample != maximum)
			if_fail_fe(testValue<T>(errors, sample, tailWidth));
		if(maximum != minimum)
			if_fail_fe(testValue<T>(errors, maximum, tailWidth));
		previousMaximum = maximum;
	}
	::llc::SPRNG random = {PACKED_RANDOM_SEED};
	stxp ::llc::u0_t VALUE_BITS = 8 - ::llc::uint_width_field_size<T>() + (szof(T) - 1) * 8;
	stxp ::llc::u3_t VALUE_MASK = (::llc::u3_t(1) << VALUE_BITS) - 1;
	for(::llc::u2_t iRandom = 0; iRandom < PACKED_RANDOM_COUNT; ++iRandom) {
		cnst T value = T(random.Next() & VALUE_MASK);
		cnst ::llc::u2_t failureCount = testErrorCount(errors);
		if_fail_fe(testValue<T>(errors, value, ::llc::uint_tail_width(value)));
		if(failureCount != testErrorCount(errors))
			error_printf("%u-bit randomized packed_uint<> case failed. seed:%" LLC_FMT_U3 ", iteration:%u, value:%" LLC_FMT_U3 "."
				, ::llc::u2_t(szof(T) * 8), PACKED_RANDOM_SEED, iRandom, ::llc::u3_t(value)
				);
	}
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testTypeLogged(ATestError & errors) {
	cnst ::llc::u2_t checkCount = testCheckCount(errors);
	cnst ::llc::u2_t failureCount = testErrorCount(errors);
	if_fail_fe(testType<T>(errors));
	cnst ::llc::u2_t typeFailures = testErrorCount(errors) - failureCount;
	cnst ::llc::u2_t typeChecks = testCheckCount(errors) - checkCount;
	if(typeFailures) error_printf("%2u-bit suite completed: %u/%u checks passed, %u failed.", ::llc::u2_t(szof(T) * 8), typeChecks - typeFailures, typeChecks, typeFailures);
	return 0;
}

::llc::err_t testPackedUInt(ATestError & errors) {
	cnst ::llc::u2_t failureCount = testErrorCount(errors);
	if_fail_fe(testTypeLogged<::llc::u0_t>(errors));
	if_fail_fe(testTypeLogged<::llc::u1_t>(errors));
	if_fail_fe(testTypeLogged<::llc::u2_t>(errors));
	if_fail_fe(testTypeLogged<::llc::u3_t>(errors));
	if(failureCount == testErrorCount(errors))
		always_printf("Element widths tested successfully:\n%u, %u, %u and %u bits."
			, ::llc::u2_t(szof(::llc::u0_t) * 8), ::llc::u2_t(szof(::llc::u1_t) * 8), ::llc::u2_t(szof(::llc::u2_t) * 8), ::llc::u2_t(szof(::llc::u3_t) * 8)
			);
	return 0;
}
