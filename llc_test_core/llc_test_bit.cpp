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
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, VALUE_WIDTH			, 18, "bitsof(value) returned the wrong storage width.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, BIT_ALL			, 19, "bit_all() missed a set bit.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, BIT_ANY_OTHER		, 20, "bit_any() found an unset bit.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, BIT_TEST			, 21, "bit_test() returned the wrong masked value.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, BIT_TRUE			, 22, "bit_true() returned the wrong masked value.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, BIT_TEST_OTHER		, 23, "bit_test() returned an unset bit.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, BIT_FALSE_SET		, 24, "bit_false() accepted a set bit.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, BIT_FALSE_OTHER		, 25, "bit_false() rejected an unset bit.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, MASKED_CLEAR			, 26, "bit_set_masked() did not clear the selected bits.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, MASKED_TEST_OTHER	, 27, "bit_test_masked() found bits outside the active mask.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, FIELD_VALUE			, 28, "bit_field<> did not preserve its stored value.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, FIELD_CONVERSION		, 29, "bit_field<> converted to the wrong value.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, FIELD_ALL			, 30, "bit_field<>::All() missed set flags.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, FIELD_ANY			, 31, "bit_field<>::Any() missed a set flag.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, FIELD_ANY_CLEAR		, 32, "bit_field<>::Any() found a cleared flag.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, CONSTEXPR_FIELD_ZERO	, 33, "The constexpr packed field 0 has the wrong value.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, CONSTEXPR_FIELD_ONE	, 34, "The constexpr packed field 1 has the wrong value.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, CONSTEXPR_FIELD_TWO	, 35, "The constexpr packed field 2 has the wrong value.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, FIELD_MASK_ONE		, 36, "Packed field 1 has the wrong local mask.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, FIELD_MASK_TWO		, 37, "Packed field 2 has the wrong local mask.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, PACKED_MASK_ZERO	, 38, "Packed field 0 has the wrong storage mask.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, PACKED_MASK_ONE		, 39, "Packed field 1 has the wrong storage mask.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, PACKED_MASK_TWO		, 40, "Packed field 2 has the wrong storage mask.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, PACKED_READ_ZERO	, 41, "Packed field 0 returned the wrong value.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, PACKED_READ_ONE		, 42, "Packed field 1 returned the wrong value.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, PACKED_READ_TWO		, 43, "Packed field 2 returned the wrong value.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, EXHAUSTIVE_READ_ZERO, 44, "Exhaustive packed field 0 returned the wrong value.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, EXHAUSTIVE_READ_ONE	, 45, "Exhaustive packed field 1 returned the wrong value.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, EXHAUSTIVE_READ_TWO	, 46, "Exhaustive packed field 2 returned the wrong value.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, CLIPPED_READ_ZERO	, 47, "Clipped packed field 0 returned the wrong value.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, CLIPPED_READ_ONE	, 48, "Clipped packed field 1 returned the wrong value.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, CLIPPED_READ_TWO	, 49, "Clipped packed field 2 returned the wrong value.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, FULL_FIELD_READ	, 50, "A full-width field returned the wrong value.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, FULL_FIELD_MASK	, 51, "A full-width field has the wrong mask.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, INVALID_DEPTH_THREE, 52, "reverse_bitfield() did not reject field width 3.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, INVALID_DEPTH_OVERFLOW, 53, "reverse_bitfield() did not reject a width beyond the type.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, ENUM_ALL			, 54, "Generated enum flags were not all set.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, ENUM_SECOND_CLEAR	, 55, "Generated enum flags included an unset bit.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, ENUM_MUTATION_ALL	, 56, "Generated enum flag mutation lost set bits.");
GDEFINE_ENUM_VALUED(BIT_FIELD_TEST_RESULT, ENUM_MUTATION_CLEAR, 57, "Generated enum flag mutation retained a cleared bit.");

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
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_TYPE_WIDTH, ::llc::bitsof<T>() != typeBits , "type width:%u, expected:%u.", ::llc::bitsof<T>(), typeBits);
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_VALUE_WIDTH, ::llc::bitsof(state) != typeBits , "value width:%u, expected:%u.", ::llc::bitsof(state), typeBits);
	for(::llc::u2_t iBit = 0; iBit < typeBits; ++iBit) {
		cnst T expected = T(1) << iBit;
		cnst T bit = ::llc::bit_make<T>((::llc::u0_t)iBit);
		LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_BIT_MAKE, bit != expected , "%u-bit mask mismatch at bit %u. actual:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "." , typeBits, iBit, (::llc::u3_t)bit, (::llc::u3_t)expected );
		state = ::llc::bit_set(state, bit);
		LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_SET_CLEAR, state != bit , "%u-bit set mismatch at bit %u. state:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "." , typeBits, iBit, (::llc::u3_t)state, (::llc::u3_t)bit );
		cnst T otherBits = T(~bit);
		LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_ANY_ALL, false == ::llc::bit_any(state, bit) , "%u-bit bit_any at bit:%u, state:%" LLC_FMT_U3 ".", typeBits, iBit, (::llc::u3_t)state);
		LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_BIT_ALL, false == ::llc::bit_all(state, bit) , "%u-bit bit_all at bit:%u, state:%" LLC_FMT_U3 ".", typeBits, iBit, (::llc::u3_t)state);
		LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_BIT_ANY_OTHER, ::llc::bit_any(state, otherBits) , "%u-bit bit_any outside bit:%u, state:%" LLC_FMT_U3 ".", typeBits, iBit, (::llc::u3_t)state);
		LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_BIT_TEST, ::llc::bit_test(state, bit) != state , "%u-bit bit_test at bit:%u, actual:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "." , typeBits, iBit, (::llc::u3_t)::llc::bit_test(state, bit), (::llc::u3_t)state);
		LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_BIT_TRUE, ::llc::bit_true(state, bit) != state , "%u-bit bit_true at bit:%u, actual:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "." , typeBits, iBit, (::llc::u3_t)::llc::bit_true(state, bit), (::llc::u3_t)state);
		LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_BIT_TEST_OTHER, ::llc::bit_test(state, otherBits) , "%u-bit bit_test outside bit:%u, actual:%" LLC_FMT_U3 "." , typeBits, iBit, (::llc::u3_t)::llc::bit_test(state, otherBits));
		LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_BIT_FALSE_SET, ::llc::bit_false(state, bit) , "%u-bit bit_false on set bit:%u.", typeBits, iBit);
		LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_BIT_FALSE_OTHER, false == ::llc::bit_false(state, otherBits) , "%u-bit bit_false outside bit:%u.", typeBits, iBit);
		state = ::llc::bit_clear(state, bit);
		LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_SET_CLEAR, state , "%u-bit clear mismatch at bit %u. state:%" LLC_FMT_U3 "." , typeBits, iBit, (::llc::u3_t)state );
	}

	cnst T mask = T(0x55U);
	cnst T requested = T(0x0FU);
	cnst T masked = T(mask & requested);
	state = ::llc::bit_set_masked(T(0), mask, requested, true);
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_MASKED_SET, state != masked , "%u-bit masked set:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "." , typeBits, (::llc::u3_t)state, (::llc::u3_t)masked);
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_MASKED_CLEAR, ::llc::bit_set_masked(state, mask, requested, false) , "%u-bit masked clear:%" LLC_FMT_U3 ", expected:0." , typeBits, (::llc::u3_t)::llc::bit_set_masked(state, mask, requested, false));
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_MASKED_TEST, false == ::llc::bit_test_masked(state, mask, requested) , "%u-bit masked test failed. state:%" LLC_FMT_U3 ", mask:%" LLC_FMT_U3 "." , typeBits, (::llc::u3_t)state, (::llc::u3_t)mask);
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_MASKED_TEST_OTHER, ::llc::bit_test_masked(state, T(~mask), requested) , "%u-bit masked test found outside bits. state:%" LLC_FMT_U3 ", mask:%" LLC_FMT_U3 "." , typeBits, (::llc::u3_t)state, (::llc::u3_t)T(~mask));

	::llc::bit_field<T> field{T(0x03U)};
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_FIELD_STORAGE, szof(field) != szof(T) , "%u-bit field bytes:%u, expected:%u." , typeBits, (::llc::u2_t)szof(field), (::llc::u2_t)szof(T));
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_FIELD_VALUE, field.Value != T(0x03U) , "%u-bit field value:%" LLC_FMT_U3 ", expected:3.", typeBits, (::llc::u3_t)field.Value);
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_FIELD_CONVERSION, T(field) != T(0x03U) , "%u-bit field conversion:%" LLC_FMT_U3 ", expected:3.", typeBits, (::llc::u3_t)T(field));
	field.Set(T(0x0CU)).Clear(T(0x02U)).Set(T(0x08U), false);
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_FIELD_OPERATIONS, field.Value != T(0x05U) , "%u-bit field value:%" LLC_FMT_U3 ", expected:5.", typeBits, (::llc::u3_t)field.Value);
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_FIELD_ALL, false == field.All(T(0x05U)) , "%u-bit field All(5) failed. value:%" LLC_FMT_U3 ".", typeBits, (::llc::u3_t)field.Value);
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_FIELD_ANY, false == field.Any(T(0x04U)) , "%u-bit field Any(4) failed. value:%" LLC_FMT_U3 ".", typeBits, (::llc::u3_t)field.Value);
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_FIELD_ANY_CLEAR, field.Any(T(0x08U)) , "%u-bit field Any(8) found a cleared bit. value:%" LLC_FMT_U3 ".", typeBits, (::llc::u3_t)field.Value);

	tydf ::llc::bit_field<T, 3, 4, 1> TPackedFields;
	static_assert(szof(TPackedFields) == szof(T), "Packed fields must not add storage overhead.");
	static_assert(TPackedFields::FIELD_COUNT == 3, "Packed field count mismatch.");
	static_assert(TPackedFields::FIELD_BITS == 8, "Packed field bit count mismatch.");
	static_assert(TPackedFields::tplt Offset<0>() == 0, "Packed field 0 offset mismatch.");
	static_assert(TPackedFields::tplt Offset<1>() == 3, "Packed field 1 offset mismatch.");
	static_assert(TPackedFields::tplt Offset<2>() == 7, "Packed field 2 offset mismatch.");
	static_assert(TPackedFields::tplt Width <0>() == 3, "Packed field 0 width mismatch.");
	static_assert(TPackedFields::tplt Width <1>() == 4, "Packed field 1 width mismatch.");
	static_assert(TPackedFields::tplt Width <2>() == 1, "Packed field 2 width mismatch.");
	stxp TPackedFields packedConstant = ::makePackedFields<T>();
	static_assert(packedConstant.Value == T(0xCDU), "Packed constexpr value mismatch.");
	static_assert(packedConstant.tplt Get<0>() == T(5), "Packed constexpr field 0 mismatch.");
	static_assert(packedConstant.tplt Get<1>() == T(9), "Packed constexpr field 1 mismatch.");
	static_assert(packedConstant.tplt Get<2>() == T(1), "Packed constexpr field 2 mismatch.");
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_PACKED_CONSTEXPR, packedConstant.Value != T(0xCDU) , "%u-bit constexpr packed value:%" LLC_FMT_U3 ", expected:205.", typeBits, (::llc::u3_t)packedConstant.Value);
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_CONSTEXPR_FIELD_ZERO, packedConstant.tplt Get<0>() != T(5) , "%u-bit constexpr field 0:%" LLC_FMT_U3 ", expected:5.", typeBits, (::llc::u3_t)packedConstant.tplt Get<0>());
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_CONSTEXPR_FIELD_ONE, packedConstant.tplt Get<1>() != T(9) , "%u-bit constexpr field 1:%" LLC_FMT_U3 ", expected:9.", typeBits, (::llc::u3_t)packedConstant.tplt Get<1>());
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_CONSTEXPR_FIELD_TWO, packedConstant.tplt Get<2>() != T(1) , "%u-bit constexpr field 2:%" LLC_FMT_U3 ", expected:1.", typeBits, (::llc::u3_t)packedConstant.tplt Get<2>());
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_PACKED_LAYOUT, TPackedFields::tplt FieldMask<0>() != T(0x07U) , "%u-bit field 0 local mask:%" LLC_FMT_U3 ".", typeBits, (::llc::u3_t)TPackedFields::tplt FieldMask<0>());
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_FIELD_MASK_ONE, TPackedFields::tplt FieldMask<1>() != T(0x0FU) , "%u-bit field 1 local mask:%" LLC_FMT_U3 ".", typeBits, (::llc::u3_t)TPackedFields::tplt FieldMask<1>());
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_FIELD_MASK_TWO, TPackedFields::tplt FieldMask<2>() != T(0x01U) , "%u-bit field 2 local mask:%" LLC_FMT_U3 ".", typeBits, (::llc::u3_t)TPackedFields::tplt FieldMask<2>());
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_PACKED_MASK_ZERO, TPackedFields::tplt Mask<0>() != T(0x07U) , "%u-bit field 0 storage mask:%" LLC_FMT_U3 ".", typeBits, (::llc::u3_t)TPackedFields::tplt Mask<0>());
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_PACKED_MASK_ONE, TPackedFields::tplt Mask<1>() != T(0x78U) , "%u-bit field 1 storage mask:%" LLC_FMT_U3 ".", typeBits, (::llc::u3_t)TPackedFields::tplt Mask<1>());
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_PACKED_MASK_TWO, TPackedFields::tplt Mask<2>() != T(0x80U) , "%u-bit field 2 storage mask:%" LLC_FMT_U3 ".", typeBits, (::llc::u3_t)TPackedFields::tplt Mask<2>());
	TPackedFields packed;
	packed.tplt Set<0>(T(5));
	packed.tplt Set<1>(T(9));
	packed.tplt Set<2>(T(1));
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_PACKED_GET_SET, packed.Value != T(0xCDU) , "%u-bit packed value:%" LLC_FMT_U3 ", expected:205.", typeBits, (::llc::u3_t)packed.Value);
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_PACKED_READ_ZERO, packed.tplt Get<0>() != T(5) , "%u-bit packed field 0:%" LLC_FMT_U3 ", expected:5.", typeBits, (::llc::u3_t)packed.tplt Get<0>());
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_PACKED_READ_ONE, packed.tplt Get<1>() != T(9) , "%u-bit packed field 1:%" LLC_FMT_U3 ", expected:9.", typeBits, (::llc::u3_t)packed.tplt Get<1>());
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_PACKED_READ_TWO, packed.tplt Get<2>() != T(1) , "%u-bit packed field 2:%" LLC_FMT_U3 ", expected:1.", typeBits, (::llc::u3_t)packed.tplt Get<2>());
	cnst T highBits = T(T(-1) ^ T(0xFFU));
	for(T value0 = 0; value0 < 8; ++value0) {
		for(T value1 = 0; value1 < 16; ++value1) {
			for(T value2 = 0; value2 < 2; ++value2) {
				packed.Value = highBits;
				packed.tplt Set<0>(value0);
				packed.tplt Set<1>(value1);
				packed.tplt Set<2>(value2);
				cnst T expected = T(highBits | value0 | T(value1 << 3) | T(value2 << 7));
				LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_PACKED_EXHAUSTIVE, packed.Value != expected , "%u-bit fields:%" LLC_FMT_U3 "/%" LLC_FMT_U3 "/%" LLC_FMT_U3 ", packed:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "." , typeBits, (::llc::u3_t)value0, (::llc::u3_t)value1, (::llc::u3_t)value2, (::llc::u3_t)packed.Value, (::llc::u3_t)expected);
				LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_EXHAUSTIVE_READ_ZERO, packed.tplt Get<0>() != value0 , "%u-bit fields:%" LLC_FMT_U3 "/%" LLC_FMT_U3 "/%" LLC_FMT_U3 ", read field 0:%" LLC_FMT_U3 "." , typeBits, (::llc::u3_t)value0, (::llc::u3_t)value1, (::llc::u3_t)value2, (::llc::u3_t)packed.tplt Get<0>());
				LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_EXHAUSTIVE_READ_ONE, packed.tplt Get<1>() != value1 , "%u-bit fields:%" LLC_FMT_U3 "/%" LLC_FMT_U3 "/%" LLC_FMT_U3 ", read field 1:%" LLC_FMT_U3 "." , typeBits, (::llc::u3_t)value0, (::llc::u3_t)value1, (::llc::u3_t)value2, (::llc::u3_t)packed.tplt Get<1>());
				LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_EXHAUSTIVE_READ_TWO, packed.tplt Get<2>() != value2 , "%u-bit fields:%" LLC_FMT_U3 "/%" LLC_FMT_U3 "/%" LLC_FMT_U3 ", read field 2:%" LLC_FMT_U3 "." , typeBits, (::llc::u3_t)value0, (::llc::u3_t)value1, (::llc::u3_t)value2, (::llc::u3_t)packed.tplt Get<2>());
			}
		}
	}
	packed.tplt Set<0>(T(-1));
	packed.tplt Set<1>(T(-1));
	packed.tplt Set<2>(T(-1));
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_PACKED_CLIP, packed.Value != T(-1) , "%u-bit clipped packed value:%" LLC_FMT_U3 ".", typeBits, (::llc::u3_t)packed.Value);
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_CLIPPED_READ_ZERO, packed.tplt Get<0>() != T(0x07U) , "%u-bit clipped field 0:%" LLC_FMT_U3 ".", typeBits, (::llc::u3_t)packed.tplt Get<0>());
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_CLIPPED_READ_ONE, packed.tplt Get<1>() != T(0x0FU) , "%u-bit clipped field 1:%" LLC_FMT_U3 ".", typeBits, (::llc::u3_t)packed.tplt Get<1>());
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_CLIPPED_READ_TWO, packed.tplt Get<2>() != T(0x01U) , "%u-bit clipped field 2:%" LLC_FMT_U3 ".", typeBits, (::llc::u3_t)packed.tplt Get<2>());
	packed.Value = T(-1);
	packed.tplt Set<1>(T(0));
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_PACKED_PRESERVE, packed.Value != T(T(-1) & T(~T(0x78U))) , "%u-bit neighbor preservation mismatch. value:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "." , typeBits, (::llc::u3_t)packed.Value, (::llc::u3_t)T(T(-1) & T(~T(0x78U))) );
	tydf ::llc::bit_field<T, (::llc::u0_t)::llc::bitsof<T>()> TFullField;
	TFullField fullField;
	fullField.tplt Set<0>(T(-1));
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_PACKED_GET_SET, fullField.Value != T(-1) , "%u-bit full-width stored value:%" LLC_FMT_U3 ".", typeBits, (::llc::u3_t)fullField.Value);
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_FULL_FIELD_READ, fullField.tplt Get<0>() != T(-1) , "%u-bit full-width read:%" LLC_FMT_U3 ".", typeBits, (::llc::u3_t)fullField.tplt Get<0>());
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_FULL_FIELD_MASK, TFullField::tplt Mask<0>() != T(-1) , "%u-bit full-width mask:%" LLC_FMT_U3 ".", typeBits, (::llc::u3_t)TFullField::tplt Mask<0>());

	cnst T sample = T(T(-1) / T(3)) ^ T(T(-1) >> 3);
	for(::llc::u2_t fieldWidth = 1; fieldWidth <= typeBits; fieldWidth *= 2) {
		cnst T expected = ::reverseFieldsReference(sample, fieldWidth);
		cnst T actual = ::llc::reverse_bitfield(sample, fieldWidth);
		LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_FIELD_REVERSE, actual != expected , "%u-bit reverse mismatch for %u-bit fields. source:%" LLC_FMT_U3 ", actual:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "." , typeBits, fieldWidth, (::llc::u3_t)sample, (::llc::u3_t)actual, (::llc::u3_t)expected );
	}
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_INVALID_DEPTH, ::llc::reverse_bitfield(sample, 0) != sample , "%u-bit reverse width 0 changed source:%" LLC_FMT_U3 ".", typeBits, (::llc::u3_t)sample);
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_INVALID_DEPTH_THREE, ::llc::reverse_bitfield(sample, 3) != sample , "%u-bit reverse width 3 changed source:%" LLC_FMT_U3 ".", typeBits, (::llc::u3_t)sample);
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_INVALID_DEPTH_OVERFLOW, ::llc::reverse_bitfield(sample, (::llc::s2_t)typeBits + 1) != sample , "%u-bit reverse width %u changed source:%" LLC_FMT_U3 ".", typeBits, typeBits + 1, (::llc::u3_t)sample);
	rtrn 0;
}

sttc ::llc::err_t testEnumFlags(ATestError & errors) {
	::llc::bit_field<BIT_FIELD_TEST_FLAG> flags;
	flags.Set(BIT_FIELD_TEST_FLAG_FIRST | BIT_FIELD_TEST_FLAG_THIRD);
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_ENUM_FLAGS, szof(flags) != szof(BIT_FIELD_TEST_FLAG) , "Generated flag bytes:%u, expected:%u." , (::llc::u2_t)szof(flags), (::llc::u2_t)szof(BIT_FIELD_TEST_FLAG));
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_ENUM_ALL, false == flags.All(BIT_FIELD_TEST_FLAG_FIRST | BIT_FIELD_TEST_FLAG_THIRD) , "Generated flag All(first|third) failed. value:0x%02X.", (::llc::u0_t)flags.Value);
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_ENUM_SECOND_CLEAR, flags.Any(BIT_FIELD_TEST_FLAG_SECOND) , "Generated flag Any(second) found unset bit. value:0x%02X.", (::llc::u0_t)flags.Value);
	flags.Set(BIT_FIELD_TEST_FLAG_SECOND).Clear(BIT_FIELD_TEST_FLAG_FIRST);
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_ENUM_MUTATION_ALL, false == flags.All(BIT_FIELD_TEST_FLAG_SECOND | BIT_FIELD_TEST_FLAG_THIRD) , "Generated flag All(second|third) failed. value:0x%02X.", (::llc::u0_t)flags.Value);
	LLC_TEST_CHECKF(errors, BIT_FIELD_TEST_RESULT_ENUM_MUTATION_CLEAR, flags.Any(BIT_FIELD_TEST_FLAG_FIRST) , "Generated flag Any(first) found cleared bit. value:0x%02X.", (::llc::u0_t)flags.Value);
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testBitTypeLogged(ATestError & errors) {
	cnst ::llc::u2_t checkCount = testCheckCount(errors);
	cnst ::llc::u2_t failureCount = testErrorCount(errors);
	if_fail_fe(testBitType<T>(errors));
	cnst ::llc::u2_t typeFailures = testErrorCount(errors) - failureCount;
	cnst ::llc::u2_t typeChecks = testCheckCount(errors) - checkCount;
	if(typeFailures) error_printf("%2u-bit suite completed: %u/%u checks passed, %u failed.", bcof(T), typeChecks - typeFailures, typeChecks, typeFailures);
	rtrn 0;
}

::llc::err_t testBitField(ATestError & errors) {
	cnst ::llc::u2_t failureCount = testErrorCount(errors);
	if_fail_fe(testBitTypeLogged<::llc::u0_t>(errors));
	if_fail_fe(testBitTypeLogged<::llc::u1_t>(errors));
	if_fail_fe(testBitTypeLogged<::llc::u2_t>(errors));
	if_fail_fe(testBitTypeLogged<::llc::u3_t>(errors));
	cnst bool typesSucceeded = failureCount == testErrorCount(errors);
	if_fail_fe(testEnumFlags(errors));
	if(typesSucceeded)
		always_printf("Element widths tested successfully:\n%u, %u, %u and %u bits."
			, ::llc::u2_t(szof(::llc::u0_t) * 8), ::llc::u2_t(szof(::llc::u1_t) * 8), ::llc::u2_t(szof(::llc::u2_t) * 8), ::llc::u2_t(szof(::llc::u3_t) * 8)
			);
	rtrn 0;
}
