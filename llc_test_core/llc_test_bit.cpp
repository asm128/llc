#include "llc_test_core.h"
#include "llc_bit.h"

GDEFINE_ENUM_TYPE(BIT_FIELD_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, OK				, 0, "All bit-field tests passed.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, TYPE_WIDTH		, 1, "bitsof() did not report the storage width in bits.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, BIT_MAKE			, 2, "bit_make() produced the wrong single-bit mask.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, SET_CLEAR			, 3, "bit_set() or bit_clear() produced the wrong state.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, ANY_ALL			, 4, "bit_any() or bit_all() reported the wrong mask relationship.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, MASKED_SET		, 5, "bit_set_masked() modified bits outside the active mask.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, MASKED_TEST		, 6, "bit_test_masked() reported the wrong masked state.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, FIELD_STORAGE		, 7, "bit_field<> added storage or failed to preserve its value.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, FIELD_OPERATIONS	, 8, "bit_field<> failed to set, clear, or test flags.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, FIELD_REVERSE		, 9, "reverse_bitfield() produced the wrong uniform-field ordering.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, INVALID_DEPTH		, 10, "reverse_bitfield() did not safely reject a non-uniform field width.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, ENUM_FLAGS			, 11, "bit_field<> failed with a generated LLC flag type.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, PACKED_LAYOUT		, 12, "bit_field<> derived the wrong width, offset, or mask for an arbitrary field.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, PACKED_GET_SET		, 13, "bit_field<> failed to read or write an arbitrary-width field.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, PACKED_CLIP			, 14, "bit_field<> did not clip an assigned value to its field width.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, PACKED_PRESERVE		, 15, "bit_field<> modified neighboring or unused storage bits.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, PACKED_CONSTEXPR		, 16, "bit_field<> field mapping could not be evaluated as a constant expression.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, PACKED_EXHAUSTIVE		, 17, "bit_field<> failed an exhaustive 3/4/1-bit field combination.");

GDEFINE_FLAG_TYPE(BIT_FIELD_TEST_FLAG, ::llc::u0_t);
GDEFINE_FLAG_VALUE(BIT_FIELD_TEST_FLAG, FIRST	, 0x01U);
GDEFINE_FLAG_VALUE(BIT_FIELD_TEST_FLAG, SECOND	, 0x02U);
GDEFINE_FLAG_VALUE(BIT_FIELD_TEST_FLAG, THIRD	, 0x04U);

tplt<tpnm T>
sttc T reverseFieldsReference(T input, ::llc::u2_t fieldWidth) {
	cnst ::llc::u2_t typeBits = szof(T) * ::llc::BYTE_SIZE;
	cnst ::llc::u2_t fieldCount = typeBits / fieldWidth;
	cnst T fieldMask = T(-1) >> (typeBits - fieldWidth);
	T result = {};
	for(::llc::u2_t iField = 0; iField < fieldCount; ++iField)
		result |= T((input >> (iField * fieldWidth)) & fieldMask) << ((fieldCount - 1 - iField) * fieldWidth);
	rtrn result;
}

tplt<tpnm T>
stxp ::llc::bit_field<T, 3, 4, 1> makePackedFields() {
	::llc::bit_field<T, 3, 4, 1> fields;
	fields.tplt Set<0>(T(5));
	fields.tplt Set<1>(T(9));
	fields.tplt Set<2>(T(1));
	rtrn fields;
}

tplt<tpnm T>
sttc ::llc::err_t testBitType(ATestError & errors) {
	cnst ::llc::u2_t typeBits = szof(T) * ::llc::BYTE_SIZE;
	T state = {};
	LLC_TEST_CHECK(errors, BIT_FIELD_TEST_RESULT_TYPE_WIDTH, ::llc::bitsof<T>() != typeBits || ::llc::bitsof(state) != typeBits
		, "%u-bit storage reported %u/%u bits."
		, typeBits, ::llc::bitsof<T>(), ::llc::bitsof(state)
		);
	for(::llc::u2_t iBit = 0; iBit < typeBits; ++iBit) {
		cnst T expected = T(1) << iBit;
		cnst T bit = ::llc::bit_make<T>((::llc::u0_t)iBit);
		LLC_TEST_CHECK(errors, BIT_FIELD_TEST_RESULT_BIT_MAKE, bit != expected
			, "%u-bit mask mismatch at bit %u. actual:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "."
			, typeBits, iBit, (::llc::u3_t)bit, (::llc::u3_t)expected
			);
		state = ::llc::bit_set(state, bit);
		LLC_TEST_CHECK(errors, BIT_FIELD_TEST_RESULT_SET_CLEAR, state != bit
			, "%u-bit set mismatch at bit %u. state:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "."
			, typeBits, iBit, (::llc::u3_t)state, (::llc::u3_t)bit
			);
		cnst T otherBits = T(~bit);
		LLC_TEST_CHECK(errors, BIT_FIELD_TEST_RESULT_ANY_ALL
			, false == ::llc::bit_any(state, bit) || false == ::llc::bit_all(state, bit) || ::llc::bit_any(state, otherBits)
			|| ::llc::bit_test(state, bit) != state || ::llc::bit_true(state, bit) != state || ::llc::bit_test(state, otherBits)
			|| ::llc::bit_false(state, bit) || false == ::llc::bit_false(state, otherBits)
			, "%u-bit test mismatch at bit %u. state:%" LLC_FMT_U3 ", bit:%" LLC_FMT_U3 "."
			, typeBits, iBit, (::llc::u3_t)state, (::llc::u3_t)bit
			);
		state = ::llc::bit_clear(state, bit);
		LLC_TEST_CHECK(errors, BIT_FIELD_TEST_RESULT_SET_CLEAR, state
			, "%u-bit clear mismatch at bit %u. state:%" LLC_FMT_U3 "."
			, typeBits, iBit, (::llc::u3_t)state
			);
	}

	cnst T mask = T(0x55U);
	cnst T requested = T(0x0FU);
	cnst T masked = T(mask & requested);
	state = ::llc::bit_set_masked(T(0), mask, requested, true);
	LLC_TEST_CHECK(errors, BIT_FIELD_TEST_RESULT_MASKED_SET, state != masked || ::llc::bit_set_masked(state, mask, requested, false)
		, "%u-bit masked set mismatch. state:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "."
		, typeBits, (::llc::u3_t)state, (::llc::u3_t)masked
		);
	LLC_TEST_CHECK(errors, BIT_FIELD_TEST_RESULT_MASKED_TEST, false == ::llc::bit_test_masked(state, mask, requested) || ::llc::bit_test_masked(state, T(~mask), requested)
		, "%u-bit masked test mismatch. state:%" LLC_FMT_U3 ", mask:%" LLC_FMT_U3 "."
		, typeBits, (::llc::u3_t)state, (::llc::u3_t)mask
		);

	::llc::bit_field<T> field{T(0x03U)};
	LLC_TEST_CHECK(errors, BIT_FIELD_TEST_RESULT_FIELD_STORAGE, szof(field) != szof(T) || field.Value != T(0x03U) || T(field) != T(0x03U)
		, "%u-bit field storage mismatch. size:%u/%u, value:%" LLC_FMT_U3 "."
		, typeBits, (::llc::u2_t)szof(field), (::llc::u2_t)szof(T), (::llc::u3_t)field.Value
		);
	field.Set(T(0x0CU)).Clear(T(0x02U)).Set(T(0x08U), false);
	LLC_TEST_CHECK(errors, BIT_FIELD_TEST_RESULT_FIELD_OPERATIONS, field.Value != T(0x05U) || false == field.All(T(0x05U)) || false == field.Any(T(0x04U)) || field.Any(T(0x08U))
		, "%u-bit field operation mismatch. value:%" LLC_FMT_U3 "."
		, typeBits, (::llc::u3_t)field.Value
		);

	tydf ::llc::bit_field<T, 3, 4, 1> TPackedFields;
	static_assert(szof(TPackedFields) == szof(T), "Packed fields must not add storage overhead.");
	static_assert(TPackedFields::FIELD_COUNT == 3 && TPackedFields::FIELD_BITS == 8, "Packed field count or width mismatch.");
	static_assert(TPackedFields::tplt Offset<0>() == 0 && TPackedFields::tplt Offset<1>() == 3 && TPackedFields::tplt Offset<2>() == 7, "Packed field offset mismatch.");
	static_assert(TPackedFields::tplt Width <0>() == 3 && TPackedFields::tplt Width <1>() == 4 && TPackedFields::tplt Width <2>() == 1, "Packed field width mismatch.");
	stxp TPackedFields packedConstant = ::makePackedFields<T>();
	static_assert(packedConstant.Value == T(0xCDU), "Packed constexpr value mismatch.");
	static_assert(packedConstant.tplt Get<0>() == T(5) && packedConstant.tplt Get<1>() == T(9) && packedConstant.tplt Get<2>() == T(1), "Packed constexpr read mismatch.");
	LLC_TEST_CHECK(errors, BIT_FIELD_TEST_RESULT_PACKED_CONSTEXPR
		, packedConstant.Value != T(0xCDU) || packedConstant.tplt Get<0>() != T(5) || packedConstant.tplt Get<1>() != T(9) || packedConstant.tplt Get<2>() != T(1)
		, "%u-bit constexpr packing mismatch. value:%" LLC_FMT_U3 "."
		, typeBits, (::llc::u3_t)packedConstant.Value
		);
	LLC_TEST_CHECK(errors, BIT_FIELD_TEST_RESULT_PACKED_LAYOUT
		, TPackedFields::tplt FieldMask<0>() != T(0x07U) || TPackedFields::tplt FieldMask<1>() != T(0x0FU) || TPackedFields::tplt FieldMask<2>() != T(0x01U)
		|| TPackedFields::tplt Mask<0>() != T(0x07U) || TPackedFields::tplt Mask<1>() != T(0x78U) || TPackedFields::tplt Mask<2>() != T(0x80U)
		, "%u-bit packed layout mismatch. masks:%" LLC_FMT_U3 "/%" LLC_FMT_U3 "/%" LLC_FMT_U3 "."
		, typeBits, (::llc::u3_t)TPackedFields::tplt Mask<0>(), (::llc::u3_t)TPackedFields::tplt Mask<1>(), (::llc::u3_t)TPackedFields::tplt Mask<2>()
		);
	TPackedFields packed;
	packed.tplt Set<0>(T(5));
	packed.tplt Set<1>(T(9));
	packed.tplt Set<2>(T(1));
	LLC_TEST_CHECK(errors, BIT_FIELD_TEST_RESULT_PACKED_GET_SET
		, packed.Value != T(0xCDU) || packed.tplt Get<0>() != T(5) || packed.tplt Get<1>() != T(9) || packed.tplt Get<2>() != T(1)
		, "%u-bit packed read/write mismatch. value:%" LLC_FMT_U3 ", fields:%" LLC_FMT_U3 "/%" LLC_FMT_U3 "/%" LLC_FMT_U3 "."
		, typeBits, (::llc::u3_t)packed.Value, (::llc::u3_t)packed.tplt Get<0>(), (::llc::u3_t)packed.tplt Get<1>(), (::llc::u3_t)packed.tplt Get<2>()
		);
	cnst T highBits = T(T(-1) ^ T(0xFFU));
	for(T value0 = 0; value0 < 8; ++value0)
	for(T value1 = 0; value1 < 16; ++value1)
	for(T value2 = 0; value2 < 2; ++value2) {
		packed.Value = highBits;
		packed.tplt Set<0>(value0);
		packed.tplt Set<1>(value1);
		packed.tplt Set<2>(value2);
		cnst T expected = T(highBits | value0 | T(value1 << 3) | T(value2 << 7));
		LLC_TEST_CHECK(errors, BIT_FIELD_TEST_RESULT_PACKED_EXHAUSTIVE
			, packed.Value != expected || packed.tplt Get<0>() != value0 || packed.tplt Get<1>() != value1 || packed.tplt Get<2>() != value2
			, "%u-bit exhaustive packing mismatch. fields:%" LLC_FMT_U3 "/%" LLC_FMT_U3 "/%" LLC_FMT_U3 ", value:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "."
			, typeBits, (::llc::u3_t)value0, (::llc::u3_t)value1, (::llc::u3_t)value2, (::llc::u3_t)packed.Value, (::llc::u3_t)expected
			);
	}
	packed.tplt Set<0>(T(-1));
	packed.tplt Set<1>(T(-1));
	packed.tplt Set<2>(T(-1));
	LLC_TEST_CHECK(errors, BIT_FIELD_TEST_RESULT_PACKED_CLIP, packed.Value != T(-1) || packed.tplt Get<0>() != T(0x07U) || packed.tplt Get<1>() != T(0x0FU) || packed.tplt Get<2>() != T(0x01U)
		, "%u-bit packed clipping mismatch. value:%" LLC_FMT_U3 "."
		, typeBits, (::llc::u3_t)packed.Value
		);
	packed.Value = T(-1);
	packed.tplt Set<1>(T(0));
	LLC_TEST_CHECK(errors, BIT_FIELD_TEST_RESULT_PACKED_PRESERVE, packed.Value != T(T(-1) & T(~T(0x78U)))
		, "%u-bit neighbor preservation mismatch. value:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "."
		, typeBits, (::llc::u3_t)packed.Value, (::llc::u3_t)T(T(-1) & T(~T(0x78U)))
		);
	tydf ::llc::bit_field<T, (::llc::u0_t)::llc::bitsof<T>()> TFullField;
	TFullField fullField;
	fullField.tplt Set<0>(T(-1));
	LLC_TEST_CHECK(errors, BIT_FIELD_TEST_RESULT_PACKED_GET_SET, fullField.Value != T(-1) || fullField.tplt Get<0>() != T(-1) || TFullField::tplt Mask<0>() != T(-1)
		, "%u-bit full-width field mismatch. value:%" LLC_FMT_U3 ", mask:%" LLC_FMT_U3 "."
		, typeBits, (::llc::u3_t)fullField.Value, (::llc::u3_t)TFullField::tplt Mask<0>()
		);

	cnst T sample = T(T(-1) / T(3)) ^ T(T(-1) >> 3);
	for(::llc::u2_t fieldWidth = 1; fieldWidth <= typeBits; fieldWidth *= 2) {
		cnst T expected = ::reverseFieldsReference(sample, fieldWidth);
		cnst T actual = ::llc::reverse_bitfield(sample, fieldWidth);
		LLC_TEST_CHECK(errors, BIT_FIELD_TEST_RESULT_FIELD_REVERSE, actual != expected
			, "%u-bit reverse mismatch for %u-bit fields. source:%" LLC_FMT_U3 ", actual:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "."
			, typeBits, fieldWidth, (::llc::u3_t)sample, (::llc::u3_t)actual, (::llc::u3_t)expected
			);
	}
	LLC_TEST_CHECK(errors, BIT_FIELD_TEST_RESULT_INVALID_DEPTH
		, ::llc::reverse_bitfield(sample, 0) != sample || ::llc::reverse_bitfield(sample, 3) != sample || ::llc::reverse_bitfield(sample, (::llc::s2_t)typeBits + 1) != sample
		, "%u-bit reverse accepted an invalid uniform-field width. source:%" LLC_FMT_U3 "."
		, typeBits, (::llc::u3_t)sample
		);
	rtrn 0;
}

sttc ::llc::err_t testEnumFlags(ATestError & errors) {
	::llc::bit_field<BIT_FIELD_TEST_FLAG> flags;
	flags.Set(BIT_FIELD_TEST_FLAG_FIRST | BIT_FIELD_TEST_FLAG_THIRD);
	LLC_TEST_CHECK(errors, BIT_FIELD_TEST_RESULT_ENUM_FLAGS
		, szof(flags) != szof(BIT_FIELD_TEST_FLAG) || false == flags.All(BIT_FIELD_TEST_FLAG_FIRST | BIT_FIELD_TEST_FLAG_THIRD) || flags.Any(BIT_FIELD_TEST_FLAG_SECOND)
		, "Generated flag field mismatch. size:%u/%u, value:0x%02X."
		, (::llc::u2_t)szof(flags), (::llc::u2_t)szof(BIT_FIELD_TEST_FLAG), (::llc::u0_t)flags.Value
		);
	flags.Set(BIT_FIELD_TEST_FLAG_SECOND).Clear(BIT_FIELD_TEST_FLAG_FIRST);
	LLC_TEST_CHECK(errors, BIT_FIELD_TEST_RESULT_ENUM_FLAGS
		, false == flags.All(BIT_FIELD_TEST_FLAG_SECOND | BIT_FIELD_TEST_FLAG_THIRD) || flags.Any(BIT_FIELD_TEST_FLAG_FIRST)
		, "Generated flag mutation mismatch. value:0x%02X."
		, (::llc::u0_t)flags.Value
		);
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testBitTypeLogged(ATestError & errors) {
	cnst ::llc::u2_t checkCount = testCheckCount(errors);
	cnst ::llc::u2_t failureCount = testErrorCount(errors);
	if_fail_fe(testBitType<T>(errors));
	cnst ::llc::u2_t typeFailures = testErrorCount(errors) - failureCount;
	cnst ::llc::u2_t typeChecks = testCheckCount(errors) - checkCount;
	if(typeFailures) error_printf("%2u-bit suite completed: %u/%u checks passed, %u failed.", ::llc::u2_t(szof(T) * 8), typeChecks - typeFailures, typeChecks, typeFailures);
	else always_printf("%2u-bit suite OK: %u checks passed.", ::llc::u2_t(szof(T) * 8), typeChecks);
	rtrn 0;
}

::llc::err_t testBitField(ATestError & errors) {
	if_fail_fe(testBitTypeLogged<::llc::u0_t>(errors));
	if_fail_fe(testBitTypeLogged<::llc::u1_t>(errors));
	if_fail_fe(testBitTypeLogged<::llc::u2_t>(errors));
	if_fail_fe(testBitTypeLogged<::llc::u3_t>(errors));
	rtrn testEnumFlags(errors);
}
