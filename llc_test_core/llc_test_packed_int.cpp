#include "llc_view_serialize.h"
#include "llc_enum.h"

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

tplt<tpnm T>
PACKED_UINT_TEST_RESULT testDefault() {
	cnst ::llc::packed_uint<T> packed{};
	if_true_vef(PACKED_UINT_TEST_RESULT_DEFAULT_VALUE, packed.TailWidth || packed.Multiplier || packed.ValueWidth() != 1 || packed.Value()
		, "%u-bit default mismatch. tail width:%u, multiplier:%u, value width:%u, value:%" LLC_FMT_U3 "."
		, ::llc::u2_t(szof(T) * 8), ::llc::u2_t(packed.TailWidth), ::llc::u2_t(packed.Multiplier), ::llc::u2_t(packed.ValueWidth()), ::llc::u3_t(packed.Value())
		);
	return PACKED_UINT_TEST_RESULT_OK;
}

tplt<tpnm T>
PACKED_UINT_TEST_RESULT testValue(::llc::u3_c source, ::llc::u0_c expectedTailWidth) {
	cnst T value = (T)source;
	cnst ::llc::packed_uint<T> packed{value};
	::llc::u3_c expectedMultiplier = source >> (expectedTailWidth * 8);
	::llc::u3_c expectedTail = expectedTailWidth ? source & ((::llc::u3_t(1) << (expectedTailWidth * 8)) - 1) : source;
	::llc::u0_c expectedValueWidth = 1 + expectedTailWidth;
	::llc::u2_c typeBits = szof(T) * 8;

	if_true_vef(PACKED_UINT_TEST_RESULT_TAIL_WIDTH, packed.TailWidth != expectedTailWidth || ::llc::uint_tail_width(value) != expectedTailWidth
		, "%u-bit tail-width mismatch for value:%" LLC_FMT_U3 ". packed:%u, helper:%u, expected:%u."
		, typeBits, source, ::llc::u2_t(packed.TailWidth), ::llc::u2_t(::llc::uint_tail_width(value)), ::llc::u2_t(expectedTailWidth)
		);
	if_true_vef(PACKED_UINT_TEST_RESULT_VALUE_WIDTH, packed.ValueWidth() != expectedValueWidth || ::llc::uint_value_width(value) != expectedValueWidth
		, "%u-bit value-width mismatch for value:%" LLC_FMT_U3 ". packed:%u, helper:%u, expected:%u."
		, typeBits, source, ::llc::u2_t(packed.ValueWidth()), ::llc::u2_t(::llc::uint_value_width(value)), ::llc::u2_t(expectedValueWidth)
		);
	if_true_vef(PACKED_UINT_TEST_RESULT_MULTIPLIER, ::llc::u3_t(packed.Multiplier) != expectedMultiplier || ::llc::u3_t(::llc::uint_tail_multiplier(value)) != expectedMultiplier
		, "%u-bit multiplier mismatch for value:%" LLC_FMT_U3 ". packed:%" LLC_FMT_U3 ", helper:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "."
		, typeBits, source, ::llc::u3_t(packed.Multiplier), ::llc::u3_t(::llc::uint_tail_multiplier(value)), expectedMultiplier
		);
	if_true_vef(PACKED_UINT_TEST_RESULT_TAIL_BASE, ::llc::u3_t(::llc::uint_tail_base(value)) != expectedTail
		, "%u-bit tail mismatch for value:%" LLC_FMT_U3 ". helper:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "."
		, typeBits, source, ::llc::u3_t(::llc::uint_tail_base(value)), expectedTail
		);
	if_true_vef(PACKED_UINT_TEST_RESULT_VALUE_ROUND_TRIP, ::llc::u3_t(packed.Value()) != source
		, "%u-bit round-trip mismatch. source:%" LLC_FMT_U3 ", unpacked:%" LLC_FMT_U3 "."
		, typeBits, source, ::llc::u3_t(packed.Value())
		);

	cnst ::llc::vcu0_t bytes = packed.tplt cu8<::llc::vcu0_t>();
	if_true_vef(PACKED_UINT_TEST_RESULT_BYTE_VIEW, bytes.begin() != (cnst ::llc::u0_t*)(cnst void*)&packed || bytes.size() != expectedValueWidth
		, "%u-bit byte-view mismatch for value:%" LLC_FMT_U3 ". begin:%p, expected begin:%p, size:%u, expected size:%u."
		, typeBits, source, (cnst void*)bytes.begin(), (cnst void*)&packed, bytes.size(), ::llc::u2_t(expectedValueWidth)
		);

	::llc::vcu0_t input = bytes;
	T loaded = {};
	cnst ::llc::err_t loadResult = ::llc::loadUInt(input, loaded);
	if_true_vef(PACKED_UINT_TEST_RESULT_LOAD_RESULT, loadResult != expectedValueWidth
		, "%u-bit load result mismatch for value:%" LLC_FMT_U3 ". result:%i, expected:%u."
		, typeBits, source, loadResult, ::llc::u2_t(expectedValueWidth)
		);
	if_true_vef(PACKED_UINT_TEST_RESULT_LOAD_VALUE, ::llc::u3_t(loaded) != source
		, "%u-bit loaded-value mismatch. source:%" LLC_FMT_U3 ", loaded:%" LLC_FMT_U3 "."
		, typeBits, source, ::llc::u3_t(loaded)
		);
	if_true_vef(PACKED_UINT_TEST_RESULT_LOAD_CONSUMPTION, input.size()
		, "%u-bit load left bytes behind for value:%" LLC_FMT_U3 ". remaining:%u, expected:0."
		, typeBits, source, input.size()
		);
	return PACKED_UINT_TEST_RESULT_OK;
}

tplt<tpnm T>
PACKED_UINT_TEST_RESULT testType() {
	PACKED_UINT_TEST_RESULT result = testDefault<T>();
	if(result) return result;

	stxp ::llc::u0_t MULTIPLIER_BITS = 8 - ::llc::uint_width_field_size<T>();
	::llc::u3_t previousMaximum = 0;
	for(::llc::u0_t tailWidth = 0; tailWidth < szof(T); ++tailWidth) {
		::llc::u0_c valueBits = MULTIPLIER_BITS + tailWidth * 8;
		::llc::u3_c maximum = (::llc::u3_t(1) << valueBits) - 1;
		::llc::u3_c minimum = tailWidth ? previousMaximum + 1 : 0;
		::llc::u3_c sample = minimum + (maximum - minimum) / 3;
		if(PACKED_UINT_TEST_RESULT_OK != (result = testValue<T>(minimum, tailWidth))) return result;
		if(sample != minimum && sample != maximum && PACKED_UINT_TEST_RESULT_OK != (result = testValue<T>(sample, tailWidth))) return result;
		if(maximum != minimum && PACKED_UINT_TEST_RESULT_OK != (result = testValue<T>(maximum, tailWidth))) return result;
		previousMaximum = maximum;
	}
	return PACKED_UINT_TEST_RESULT_OK;
}

int testPackedUInt() {
	PACKED_UINT_TEST_RESULT testResult = PACKED_UINT_TEST_RESULT_OK;
	if_true_vef(testResult, testResult = testType<::llc::u0_t>(), " 8-bit suite failed. %s: %s", ::llc::get_value_namep(testResult), ::llc::get_value_descp(testResult)) else always_printf(" 8-bit suite OK.");
	if_true_vef(testResult, testResult = testType<::llc::u1_t>(), "16-bit suite failed. %s: %s", ::llc::get_value_namep(testResult), ::llc::get_value_descp(testResult)) else always_printf("16-bit suite OK.");
	if_true_vef(testResult, testResult = testType<::llc::u2_t>(), "32-bit suite failed. %s: %s", ::llc::get_value_namep(testResult), ::llc::get_value_descp(testResult)) else always_printf("32-bit suite OK.");
	if_true_vef(testResult, testResult = testType<::llc::u3_t>(), "64-bit suite failed. %s: %s", ::llc::get_value_namep(testResult), ::llc::get_value_descp(testResult)) else always_printf("64-bit suite OK.");
	return PACKED_UINT_TEST_RESULT_OK;
}
