#include "llc_view.h"
#include "llc_array_obj.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(VIEW_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, OK						, 0, "All view tests passed.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, INVALID_CONSTRUCTION		, 8, "view<> accepted null storage with a nonzero element count.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_FULL				, 9, "slice() did not reproduce the full source range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_REMAINDER			, 10, "slice() did not produce the expected range after an offset.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_COUNT				, 11, "slice() did not honor an explicit valid element count.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_END					, 12, "slice() at the exact end did not produce an empty one-past-end range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_SELF				, 13, "slice() could not advance and shorten its own view.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_INVALID_OFFSET		, 14, "slice() accepted an offset beyond the source range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_INVALID_COUNT		, 15, "slice() accepted a count beyond the remaining source range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_EMPTY				, 17, "Slicing a default empty view did not produce a default empty range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SUBSCRIPT_READ			, 18, "operator[] did not read the selected element.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SUBSCRIPT_WRITE			, 19, "Mutable operator[] did not update the selected element.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, INVALID_SUBSCRIPT			, 20, "operator[] accepted an index at or beyond the view size.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, EMPTY_SUBSCRIPT			, 21, "operator[] accepted an element access on a default empty view.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, EQUALITY_IDENTITY			, 22, "Equal views over the same range compared different.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, EQUALITY_CONTENT			, 23, "Views over independent equal contents compared different.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, EQUALITY_VALUE			, 24, "Views containing a different value compared equal.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, EQUALITY_SIZE				, 25, "Views with different element counts compared equal.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, EQUALITY_EMPTY			, 26, "Empty views with different boundary pointers compared different.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, INEQUALITY_SYMMETRY		, 27, "operator!= was not the inverse of operator==.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, BYTE_COUNT				, 28, "byte_count() did not report the storage occupied by the view elements.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, BIT_COUNT					, 29, "bit_count() did not report eight times the byte count.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, MUTABLE_CHAR_VIEW			, 30, "c() did not expose the complete mutable character representation.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, MUTABLE_BYTE_VIEW			, 31, "u8() did not expose the complete mutable byte representation.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, CONST_CHAR_VIEW			, 32, "cc() did not expose the complete const character representation.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, CONST_BYTE_VIEW			, 33, "Const u8() did not expose the complete const byte representation.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, CONST_BYTE_ALIAS			, 34, "cu8() did not match the const byte representation.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, MUTABLE_REPRESENTATION	, 35, "A write through a mutable representation view did not update the backing storage.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FILL_FULL					, 37, "fill() did not replace every element in the view.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FILL_RANGE				, 38, "fill() did not restrict replacement to the requested range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FILL_CLIPPED				, 39, "fill() did not clip its stop position to the view size.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FILL_EMPTY_RANGE			, 40, "fill() changed storage when given an empty range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FILL_EMPTY_VIEW			, 41, "fill() did not accept an empty view.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, REVERT_ODD				, 42, "revert() did not reverse an odd number of elements.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, REVERT_EVEN				, 43, "revert() did not reverse an even subrange without changing its guards.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, REVERT_EDGE				, 44, "revert() did not accept a single-element or empty view.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, REVERSE_ODD				, 45, "reverse() did not reverse an odd number of elements.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, REVERSE_EVEN				, 46, "reverse() did not reverse an even subrange without changing its guards.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, REVERSE_EDGE				, 47, "reverse() did not accept a single-element or empty view.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FOREACH_MUTABLE_FULL		, 48, "Mutable for_each() did not visit and update every element.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FOREACH_MUTABLE_OFFSET	, 49, "Mutable for_each() did not report and process the elements after its offset.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FOREACH_MUTABLE_RANGE		, 50, "Mutable for_each() did not honor its explicit range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FOREACH_MUTABLE_CLIPPED	, 51, "Mutable for_each() did not clip its stop position to the view size.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FOREACH_CONST				, 52, "Const for_each() did not visit the expected elements without mutation.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FOREACH_EMPTY				, 53, "for_each() invoked its callback or reported work for an empty range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, ENUMERATE_MUTABLE			, 54, "Mutable enumerate() did not provide the correct indices and elements.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, ENUMERATE_RANGE			, 55, "enumerate() did not honor its requested range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, ENUMERATE_CONST			, 56, "Const enumerate() did not provide the correct indices and elements.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, ENUMERATE_EMPTY			, 57, "enumerate() invoked its callback or reported work for an empty range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FIND_MUTABLE				, 60, "Mutable predicate find() did not return the first matching index.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FIND_CONST				, 61, "Const predicate find() did not return the first matching index.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FIND_VALUE				, 62, "Value find() did not return the first matching index.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FIND_OFFSET				, 63, "find() did not honor its starting offset.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FIND_NOT_FOUND			, 64, "find() did not return -1 when no element matched.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FIND_EMPTY				, 65, "find() invoked its predicate or found an element in an empty range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, MAXIMUM_OUTPUT			, 66, "max() did not return the first maximum and its transformed value.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, MINIMUM_OUTPUT			, 67, "min() did not return the first minimum and its transformed value.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, EXTREMA_OFFSET			, 68, "min()/max() did not honor the starting offset.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, EXTREMA_CONVENIENCE		, 69, "The min()/max() convenience overload did not return the correct index.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, EXTREMA_EMPTY			, 70, "min()/max() did not reject an empty search range without changing its output.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, EXTREMA_SINGLE			, 71, "min()/max() did not handle a single-element view.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SPLIT_VALUE				, 72, "split() did not remove a scalar delimiter and preserve both resulting ranges.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SPLIT_VALUE_MISSING		, 73, "split() did not preserve the complete input when a scalar delimiter was absent.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SPLIT_VALUE_BOUNDARY		, 74, "split() did not preserve empty boundary ranges around a scalar delimiter.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SPLIT_AT_VALUE			, 75, "splitAt() did not retain the scalar delimiter at the start of the right range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SPLIT_SEQUENCE			, 76, "split() did not remove a delimiter sequence and preserve both resulting ranges.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SPLIT_SEQUENCE_MISSING	, 77, "split() did not preserve the complete input when a delimiter sequence was absent.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SPLIT_AT_SEQUENCE		, 78, "splitAt() did not retain the delimiter sequence at the start of the right range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SPLIT_SEQUENCE_IN_PLACE	, 79, "The in-place sequence split did not preserve its left and right ranges.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SPLIT_COLLECTION			, 80, "The scalar split collector did not discard empty fields and delimiters.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SPLIT_COLLECTION_APPEND	, 81, "The scalar split collector did not append to the existing output.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SPLIT_COLLECTION_SEQUENCE	, 82, "The delimiter-sequence split collector did not preserve exact sequence semantics.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SPLIT_STRING_COLLECTION	, 83, "The string split collector did not discard empty fields and delimiters.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_CONST_OUTPUT		, 85, "A mutable view did not produce the requested const slice.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SPLIT_VALUE_CONST_OUTPUT	, 86, "split() did not project a mutable scalar-split source into const output views.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SPLIT_AT_VALUE_CONST_OUTPUT, 87, "splitAt() did not project a mutable scalar-split source into const output views.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SPLIT_SEQUENCE_CONST_OUTPUT, 88, "split() did not project a mutable sequence-split source into const output views.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SPLIT_AT_SEQUENCE_CONST_OUTPUT, 89, "splitAt() did not project a mutable sequence-split source into const output views.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FIND_SEQUENCE			, 90, "Sequence find() did not use element equality or honor its starting offset.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RFIND_VALUE				, 91, "Value rfind() did not return the last matching index or honor its offset.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RFIND_SEQUENCE			, 92, "Sequence rfind() did not use element equality or honor its offset.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_DEFAULT_VIEW	, 93, "Range mismatch in Default view.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_ARRAY_VIEW	, 94, "Range mismatch in Array view.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_COUNT_FIRST_VIEW	, 95, "Range mismatch in Count-first view.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_CLIPPED_COUNT_FIRST_VIEW	, 96, "Range mismatch in Clipped count-first view.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_ARRAY_FIRST_VIEW	, 97, "Range mismatch in Array-first view.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_POINTER_BACKED_VIEW	, 98, "Range mismatch in Pointer-backed view.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_EMPTY_VIEW_AT_DATA	, 99, "Range mismatch in Empty view at data.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_EMPTY_VIEW_AT_NULL	, 100, "Range mismatch in Empty view at null.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_CONST_CONVERSION	, 101, "Range mismatch in Const conversion.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_MUTABLE_FULL_SLICE	, 102, "Range mismatch in Mutable full slice.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_MUTABLE_REMAINDER_SLICE	, 103, "Range mismatch in Mutable remainder slice.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_MUTABLE_COUNTED_SLICE	, 104, "Range mismatch in Mutable counted slice.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_MUTABLE_END_SLICE	, 105, "Range mismatch in Mutable end slice.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_MUTABLE_TO_CONST_SLICE	, 106, "Range mismatch in Mutable-to-const slice.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_MUTABLE_SELF_SLICE	, 107, "Range mismatch in Mutable self-slice.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_MUTABLE_INVALID_OFFSET_OUTPUT	, 108, "Range mismatch in Mutable invalid-offset output.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_MUTABLE_INVALID_COUNT_OUTPUT	, 109, "Range mismatch in Mutable invalid-count output.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_MUTABLE_EMPTY_SLICE	, 110, "Range mismatch in Mutable empty slice.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_CONST_FULL_SLICE	, 111, "Range mismatch in Const full slice.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_CONST_REMAINDER_SLICE	, 112, "Range mismatch in Const remainder slice.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_CONST_COUNTED_SLICE	, 113, "Range mismatch in Const counted slice.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_CONST_END_SLICE	, 114, "Range mismatch in Const end slice.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_CONST_SELF_SLICE	, 115, "Range mismatch in Const self-slice.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_CONST_INVALID_OFFSET_OUTPUT	, 116, "Range mismatch in Const invalid-offset output.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_CONST_INVALID_COUNT_OUTPUT	, 117, "Range mismatch in Const invalid-count output.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_CONST_EMPTY_SLICE	, 118, "Range mismatch in Const empty slice.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_NULL_EMPTY_MUTABLE_CHARACTERS	, 119, "Range mismatch in Null-empty mutable characters.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_NULL_EMPTY_MUTABLE_BYTES	, 120, "Range mismatch in Null-empty mutable bytes.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_NULL_EMPTY_CONST_CHARACTERS	, 121, "Range mismatch in Null-empty const characters.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_NULL_EMPTY_CONST_BYTES	, 122, "Range mismatch in Null-empty const bytes.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_NULL_EMPTY_CONST_BYTE_ALIAS	, 123, "Range mismatch in Null-empty const byte alias.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_BOUNDARY_EMPTY_MUTABLE_CHARACTERS	, 124, "Range mismatch in Boundary-empty mutable characters.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_BOUNDARY_EMPTY_MUTABLE_BYTES	, 125, "Range mismatch in Boundary-empty mutable bytes.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_BOUNDARY_EMPTY_CONST_CHARACTERS	, 126, "Range mismatch in Boundary-empty const characters.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_BOUNDARY_EMPTY_CONST_BYTES	, 127, "Range mismatch in Boundary-empty const bytes.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_BOUNDARY_EMPTY_CONST_BYTE_ALIAS	, 128, "Range mismatch in Boundary-empty const byte alias.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FAILURE_MUTABLE_FOR_EACH	, 129, "Failure propagation in Mutable for_each.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FAILURE_CONST_FOR_EACH	, 130, "Failure propagation in Const for_each.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FAILURE_MUTABLE_RANGED_FOR_EACH	, 131, "Failure propagation in Mutable ranged for_each.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FAILURE_CONST_RANGED_FOR_EACH	, 132, "Failure propagation in Const ranged for_each.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FAILURE_MUTABLE_ENUMERATE	, 133, "Failure propagation in Mutable enumerate.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FAILURE_CONST_ENUMERATE	, 134, "Failure propagation in Const enumerate.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FAILURE_MUTABLE_RANGED_ENUMERATE	, 135, "Failure propagation in Mutable ranged enumerate.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FAILURE_CONST_RANGED_ENUMERATE	, 136, "Failure propagation in Const ranged enumerate.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_IN_PLACE_SCALAR_SPLIT_LEFT	, 137, "Range mismatch in In-place scalar split left.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_MISSING_IN_PLACE_SCALAR_SPLIT_LEFT	, 138, "Range mismatch in Missing in-place scalar split left.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_SCALAR_SPLIT_LEFT	, 139, "Range mismatch in Scalar split left.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_SCALAR_SPLIT_RIGHT	, 140, "Range mismatch in Scalar split right.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_LEADING_SCALAR_SPLIT_LEFT	, 141, "Range mismatch in Leading scalar split left.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_LEADING_SCALAR_SPLIT_RIGHT	, 142, "Range mismatch in Leading scalar split right.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_TRAILING_SCALAR_SPLIT_LEFT	, 143, "Range mismatch in Trailing scalar split left.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_TRAILING_SCALAR_SPLIT_RIGHT	, 144, "Range mismatch in Trailing scalar split right.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_MISSING_SCALAR_SPLIT_LEFT	, 145, "Range mismatch in Missing scalar split left.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_SCALAR_SPLIT_AT_LEFT	, 146, "Range mismatch in Scalar splitAt left.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_SCALAR_SPLIT_AT_RIGHT	, 147, "Range mismatch in Scalar splitAt right.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_CONST_OUTPUT_SCALAR_SPLIT_LEFT	, 148, "Range mismatch in Const-output scalar split left.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_CONST_OUTPUT_SCALAR_SPLIT_RIGHT	, 149, "Range mismatch in Const-output scalar split right.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_CONST_OUTPUT_SCALAR_SPLIT_AT_LEFT	, 150, "Range mismatch in Const-output scalar splitAt left.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_CONST_OUTPUT_SCALAR_SPLIT_AT_RIGHT	, 151, "Range mismatch in Const-output scalar splitAt right.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_CONST_SCALAR_SPLIT_LEFT	, 152, "Range mismatch in Const scalar split left.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_CONST_SCALAR_SPLIT_RIGHT	, 153, "Range mismatch in Const scalar split right.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_SEQUENCE_SPLIT_LEFT	, 154, "Range mismatch in Sequence split left.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_SEQUENCE_SPLIT_RIGHT	, 155, "Range mismatch in Sequence split right.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_MISSING_SEQUENCE_SPLIT_LEFT	, 156, "Range mismatch in Missing sequence split left.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_SEQUENCE_SPLIT_AT_LEFT	, 157, "Range mismatch in Sequence splitAt left.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_SEQUENCE_SPLIT_AT_RIGHT	, 158, "Range mismatch in Sequence splitAt right.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_CONST_OUTPUT_SEQUENCE_SPLIT_LEFT	, 159, "Range mismatch in Const-output sequence split left.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_CONST_OUTPUT_SEQUENCE_SPLIT_RIGHT	, 160, "Range mismatch in Const-output sequence split right.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_CONST_OUTPUT_SEQUENCE_SPLIT_AT_LEFT	, 161, "Range mismatch in Const-output sequence splitAt left.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_CONST_OUTPUT_SEQUENCE_SPLIT_AT_RIGHT	, 162, "Range mismatch in Const-output sequence splitAt right.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_IN_PLACE_SEQUENCE_SPLIT_LEFT	, 163, "Range mismatch in In-place sequence split left.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_IN_PLACE_SEQUENCE_SPLIT_RIGHT	, 164, "Range mismatch in In-place sequence split right.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_ORDINARY_COLLECTION_FIELD_0	, 165, "Range mismatch in Ordinary collection field 0.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_ORDINARY_COLLECTION_FIELD_1	, 166, "Range mismatch in Ordinary collection field 1.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_CONSECUTIVE_DELIMITER_FIELD_0	, 167, "Range mismatch in Consecutive-delimiter field 0.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_CONSECUTIVE_DELIMITER_FIELD_1	, 168, "Range mismatch in Consecutive-delimiter field 1.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_BOUNDARY_DELIMITER_FIELD_0	, 169, "Range mismatch in Boundary-delimiter field 0.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_DELIMITER_FREE_FIELD_0	, 170, "Range mismatch in Delimiter-free field 0.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_APPEND_COLLECTION_SEED	, 171, "Range mismatch in Append collection seed.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_APPEND_COLLECTION_FIELD_1	, 172, "Range mismatch in Append collection field 1.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_APPEND_COLLECTION_FIELD_2	, 173, "Range mismatch in Append collection field 2.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_DELIMITER_SEQUENCE_FIELD_0	, 174, "Range mismatch in Delimiter-sequence field 0.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, RANGE_DELIMITER_SEQUENCE_FIELD_1	, 175, "Range mismatch in Delimiter-sequence field 1.");

// The expected addresses verify begin()/end() boundaries; they are never dereferenced.
tplt<tpnm T>
sttc ::llc::err_t viewRangeCheck(ATestError & errors, VIEW_TEST_RESULT result, ::llc::view<T> actual, ::llc::u2_t expectedCount, cnst void * expectedBegin, cnst void * expectedEnd) {
	LLC_TEST_CHECKF(errors, result, actual.size() != expectedCount , "Count:%u, expected:%u." , actual.size(), expectedCount );
	LLC_TEST_CHECKF(errors, result, actual.begin() != expectedBegin , "Begin:%p, expected:%p." , actual.begin(), expectedBegin );
	if(actual.size() == expectedCount && actual.begin() == expectedBegin) {
		LLC_TEST_CHECKF(errors, result, actual.end() != expectedEnd , "End:%p, expected:%p." , actual.end(), expectedEnd );
	}
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testRepresentation(ATestError & errors) {
	T data[5] = {};
	::llc::view<T> empty;
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_DEFAULT_VIEW, empty, 0, nullptr, nullptr));

	// The remaining pointer arithmetic in this function verifies the one-past boundary returned by view::end().
	::llc::view<T> full{data};
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_ARRAY_VIEW, full, ::llc::size(data), data, data + ::llc::size(data)));

	::llc::view<T> partialCountFirst{3U, data};
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_COUNT_FIRST_VIEW, partialCountFirst, 3, data, &data[3]));
	::llc::view<T> clippedCountFirst{::llc::u2_t(::llc::size(data) + 1), data};
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_CLIPPED_COUNT_FIRST_VIEW, clippedCountFirst, ::llc::size(data), data, data + ::llc::size(data)));

	::llc::view<T> partialArrayFirst{data, 3U};
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_ARRAY_FIRST_VIEW, partialArrayFirst, 3, data, &data[3]));

	::llc::view<T> pointerRange{&data[1], 3};
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_POINTER_BACKED_VIEW, pointerRange, 3, &data[1], &data[4]));

	::llc::view<T> emptyAtData{data, 0};
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_EMPTY_VIEW_AT_DATA, emptyAtData, 0, data, data));
	::llc::view<T> emptyAtNull{nullptr, 0};
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_EMPTY_VIEW_AT_NULL, emptyAtNull, 0, nullptr, nullptr));

	::llc::view<cnst T> readOnly = pointerRange;
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_CONST_CONVERSION, readOnly, pointerRange.size(), pointerRange.begin(), pointerRange.end()));
	return 0;
}

tplt<tpnm TSource, tpnm TOutput>
sttc ::llc::err_t sliceExpectedFailure(TSource & source, TOutput & output, ::llc::u2_c offset, ::llc::u2_c count = (::llc::u2_t)-1) {
	::llc::setupLogCallbacks(0, 0);
	cnst ::llc::err_t result = source.slice(output, offset, count);
	::llc::setupDefaultLogCallbacks();
	return result;
}

tplt<tpnm T>
sttc ::llc::err_t testMutableSlice(ATestError & errors) {
	T data[5] = {};
	::llc::view<T> source{data};
	::llc::view<T> output;

	::llc::err_t result = source.slice(output, 0);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SLICE_FULL, result != 5 , "Mutable full slice result:%i, expected:5.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_MUTABLE_FULL_SLICE, output, 5, data, source.end()));
	result = source.slice(output, 2);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SLICE_REMAINDER, result != 3 , "Mutable remainder slice result:%i, expected:3.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_MUTABLE_REMAINDER_SLICE, output, 3, &data[2], source.end()));
	result = source.slice(output, 1, 2);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SLICE_COUNT, result != 2 , "Mutable counted slice result:%i, expected:2.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_MUTABLE_COUNTED_SLICE, output, 2, &data[1], &data[3]));
	result = source.slice(output, 5);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SLICE_END, result , "Mutable end slice result:%i, expected:0.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_MUTABLE_END_SLICE, output, 0, source.end(), source.end()));
	::llc::view<cnst T> constOutput;
	result = source.slice(constOutput, 1, 2);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SLICE_CONST_OUTPUT, result != 2 , "Mutable-to-const slice result:%i, expected:2.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_MUTABLE_TO_CONST_SLICE, constOutput, 2, &data[1], &data[3]));

	::llc::view<T> self{data};
	result = self.slice(self, 2, 2);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SLICE_SELF, result != 2 , "Mutable self-slice result:%i, expected:2.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_MUTABLE_SELF_SLICE, self, 2, &data[2], &data[4]));

	T preserved[2] = {};
	::llc::view<T> failedOutput{preserved};
	result = sliceExpectedFailure(source, failedOutput, 6);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SLICE_INVALID_OFFSET, 0 <= result , "mutable slice accepted offset:6 for size:5. result:%i." , result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_MUTABLE_INVALID_OFFSET_OUTPUT, failedOutput, ::llc::size(preserved), preserved, &preserved[2]));
	result = sliceExpectedFailure(source, failedOutput, 3, 3);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SLICE_INVALID_COUNT, 0 <= result , "mutable slice accepted count:3 with only 2 elements remaining. result:%i." , result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_MUTABLE_INVALID_COUNT_OUTPUT, failedOutput, ::llc::size(preserved), preserved, &preserved[2]));

	::llc::view<T> empty;
	output = source;
	result = empty.slice(output, 0);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SLICE_EMPTY, result , "Mutable empty slice result:%i, expected:0.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_MUTABLE_EMPTY_SLICE, output, 0, nullptr, nullptr));
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testConstSlice(ATestError & errors) {
	T data[5] = {};
	cnst ::llc::view<T> source{data};
	::llc::view<cnst T> output;

	::llc::err_t result = source.slice(output, 0);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SLICE_FULL, result != 5 , "Const full slice result:%i, expected:5.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_CONST_FULL_SLICE, output, 5, data, source.end()));
	result = source.slice(output, 2);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SLICE_REMAINDER, result != 3 , "Const remainder slice result:%i, expected:3.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_CONST_REMAINDER_SLICE, output, 3, &data[2], source.end()));
	result = source.slice(output, 1, 2);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SLICE_COUNT, result != 2 , "Const counted slice result:%i, expected:2.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_CONST_COUNTED_SLICE, output, 2, &data[1], &data[3]));
	result = source.slice(output, 5);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SLICE_END, result , "Const end slice result:%i, expected:0.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_CONST_END_SLICE, output, 0, source.end(), source.end()));

	::llc::view<cnst T> self{data};
	result = self.slice(self, 2, 2);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SLICE_SELF, result != 2 , "Const self-slice result:%i, expected:2.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_CONST_SELF_SLICE, self, 2, &data[2], &data[4]));

	T preserved[2] = {};
	::llc::view<cnst T> failedOutput{preserved};
	result = sliceExpectedFailure(source, failedOutput, 6);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SLICE_INVALID_OFFSET, 0 <= result , "const slice accepted offset:6 for size:5. result:%i." , result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_CONST_INVALID_OFFSET_OUTPUT, failedOutput, ::llc::size(preserved), preserved, &preserved[2]));
	result = sliceExpectedFailure(source, failedOutput, 3, 3);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SLICE_INVALID_COUNT, 0 <= result , "const slice accepted count:3 with only 2 elements remaining. result:%i." , result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_CONST_INVALID_COUNT_OUTPUT, failedOutput, ::llc::size(preserved), preserved, &preserved[2]));

	cnst ::llc::view<T> empty;
	output = source;
	result = empty.slice(output, 0);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SLICE_EMPTY, result , "Const empty slice result:%i, expected:0.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_CONST_EMPTY_SLICE, output, 0, nullptr, nullptr));
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testSubscript(ATestError & errors) {
	T data[4] = {T(1), T(2), T(3), T(4)};
	::llc::view<T> mutableView{data};
	cnst ::llc::view<T> & constView = mutableView;
	for(::llc::u2_t iElement = 0; iElement < ::llc::size(data); ++iElement) {
		LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SUBSCRIPT_READ, mutableView[iElement] != data[iElement] , "mutable subscript mismatch. index:%u, value:%" LLC_FMT_S3 ", expected:%" LLC_FMT_S3 "." , iElement, ::llc::s3_t(mutableView[iElement]), ::llc::s3_t(data[iElement]) );
		LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SUBSCRIPT_READ, constView[iElement] != data[iElement] , "const subscript mismatch. index:%u, value:%" LLC_FMT_S3 ", expected:%" LLC_FMT_S3 "." , iElement, ::llc::s3_t(constView[iElement]), ::llc::s3_t(data[iElement]) );
	}
	mutableView[2] = T(9);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SUBSCRIPT_WRITE, data[2] != T(9) , "Mutable subscript write left storage[2]:%" LLC_FMT_S3 ", expected:9." , ::llc::s3_t(data[2]) );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SUBSCRIPT_WRITE, mutableView[2] != T(9) , "Mutable subscript read after write:%" LLC_FMT_S3 ", expected:9." , ::llc::s3_t(mutableView[2]) );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SUBSCRIPT_WRITE, constView[2] != T(9) , "Const subscript read after mutable write:%" LLC_FMT_S3 ", expected:9." , ::llc::s3_t(constView[2]) );
#ifdef LLC_WINDOWS
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_INVALID_SUBSCRIPT, !testThrows([&]() { (void)mutableView[mutableView.size()]; }) , "mutable view accepted index:%u at size:%u." , mutableView.size(), mutableView.size() );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_INVALID_SUBSCRIPT, !testThrows([&]() { (void)constView[constView.size()]; }) , "const view accepted index:%u at size:%u." , constView.size(), constView.size() );

	::llc::view<T>		empty		; LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EMPTY_SUBSCRIPT, !testThrows([&]() { (void)empty		[0]; }));
	cnst ::llc::view<T> constEmpty	; LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EMPTY_SUBSCRIPT, !testThrows([&]() { (void)constEmpty [0]; }));
#endif
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testEquality(ATestError & errors) {
	T valuesA[4] = {T(1), T(2), T(3), T(4)};
	T valuesB[4] = {T(1), T(2), T(3), T(4)};
	T valuesDifferent[4] = {T(1), T(2), T(3), T(5)};
	::llc::view<T> viewA{valuesA};
	::llc::view<T> aliasA{valuesA};
	::llc::view<T> viewB{valuesB};
	::llc::view<T> viewDifferent{valuesDifferent};
	::llc::view<T> viewShort{3U, valuesA};

	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EQUALITY_IDENTITY, viewA != aliasA , "Same-range views differ under !=. left:%p, right:%p." , viewA.begin(), aliasA.begin() );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EQUALITY_IDENTITY, !(viewA == aliasA) , "Same-range views differ under ==. left:%p, right:%p." , viewA.begin(), aliasA.begin() );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EQUALITY_CONTENT, viewA != viewB , "Equal-content views differ under !=. left:%p, right:%p." , viewA.begin(), viewB.begin() );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EQUALITY_CONTENT, !(viewA == viewB) , "Equal-content views differ under ==. left:%p, right:%p." , viewA.begin(), viewB.begin() );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EQUALITY_VALUE, viewA == viewDifferent , "%s", "Different-content views compare equal under ==." );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EQUALITY_VALUE, !(viewA != viewDifferent) , "%s", "Different-content views compare equal under !=." );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EQUALITY_SIZE, viewA == viewShort , "Different-size views compare equal under ==. left:%u, right:%u." , viewA.size(), viewShort.size() );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EQUALITY_SIZE, !(viewA != viewShort) , "Different-size views compare equal under !=. left:%u, right:%u." , viewA.size(), viewShort.size() );
	::llc::view<T> emptyDefault;
	::llc::view<T> emptyAtData{0U, valuesA};
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EQUALITY_EMPTY, emptyDefault != emptyAtData , "Empty views differ under !=. left:%p, right:%p." , emptyDefault.begin(), emptyAtData.begin() );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EQUALITY_EMPTY, !(emptyDefault == emptyAtData) , "Empty views differ under ==. left:%p, right:%p." , emptyDefault.begin(), emptyAtData.begin() );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_INEQUALITY_SYMMETRY, (viewA == viewB) == (viewA != viewB) , "%s", "Equal-content views violate ==/!= symmetry." );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_INEQUALITY_SYMMETRY, (viewA == viewDifferent) == (viewA != viewDifferent) , "%s", "Different-content views violate ==/!= symmetry." );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_INEQUALITY_SYMMETRY, (viewA == viewShort) == (viewA != viewShort) , "%s", "Different-size views violate ==/!= symmetry." );
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testRepresentationViews(ATestError & errors) {
	T data[3] = {};
	::llc::view<T> mutableView{data};
	cnst ::llc::view<T> & constView = mutableView;
	::llc::u2_c expectedBytes = szof(data);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_BYTE_COUNT, mutableView.byte_count() != expectedBytes , "Mutable byte count:%u, expected:%u." , mutableView.byte_count(), expectedBytes );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_BYTE_COUNT, constView.byte_count() != expectedBytes , "Const byte count:%u, expected:%u." , constView.byte_count(), expectedBytes );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_BYTE_COUNT, ::llc::byte_count(mutableView) != expectedBytes , "Free byte_count():%u, expected:%u." , ::llc::byte_count(mutableView), expectedBytes );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_BIT_COUNT, mutableView.bit_count() != expectedBytes * 8ULL , "Mutable bit count:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "." , mutableView.bit_count(), ::llc::u3_t(expectedBytes * 8ULL) );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_BIT_COUNT, constView.bit_count() != expectedBytes * 8ULL , "Const bit count:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "." , constView.bit_count(), ::llc::u3_t(expectedBytes * 8ULL) );

	::llc::view<::llc::sc_t> chars = mutableView.c();
	::llc::view<::llc::u0_t> bytes = mutableView.u8();
	LLC_TEST_REQUIREF(errors, VIEW_TEST_RESULT_MUTABLE_CHAR_VIEW, (::llc::uP_t)chars.begin() != (::llc::uP_t)data , "Mutable character view begin:%p, expected:%p." , chars.begin(), data );
	LLC_TEST_REQUIREF(errors, VIEW_TEST_RESULT_MUTABLE_CHAR_VIEW, chars.size() != expectedBytes , "Mutable character view size:%u, expected:%u." , chars.size(), expectedBytes );
	LLC_TEST_REQUIREF(errors, VIEW_TEST_RESULT_MUTABLE_BYTE_VIEW, (::llc::uP_t)bytes.begin() != (::llc::uP_t)data , "Mutable byte view begin:%p, expected:%p." , bytes.begin(), data );
	LLC_TEST_REQUIREF(errors, VIEW_TEST_RESULT_MUTABLE_BYTE_VIEW, bytes.size() != expectedBytes , "Mutable byte view size:%u, expected:%u." , bytes.size(), expectedBytes );
	::llc::view<::llc::u0_c> constByteAlias = constView.cu8();
	LLC_TEST_REQUIREF(errors, VIEW_TEST_RESULT_CONST_BYTE_ALIAS, (::llc::uP_t)constByteAlias.begin() != (::llc::uP_t)data , "Const byte alias begin:%p, expected:%p." , constByteAlias.begin(), data );
	LLC_TEST_REQUIREF(errors, VIEW_TEST_RESULT_CONST_BYTE_ALIAS, constByteAlias.size() != expectedBytes , "Const byte alias size:%u, expected:%u." , constByteAlias.size(), expectedBytes );
	chars[0] = 0x2A;
	bytes[expectedBytes - 1] = 0x5A;
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_MUTABLE_REPRESENTATION, constByteAlias[0] != 0x2A , "Mutable character write left first byte:%u, expected:42." , constByteAlias[0] );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_MUTABLE_REPRESENTATION, constByteAlias[expectedBytes - 1] != 0x5A , "Mutable byte write left last byte:%u, expected:90." , constByteAlias[expectedBytes - 1] );

	::llc::view<::llc::sc_c> constChars = constView.cc();
	::llc::view<::llc::u0_c> constBytes = constView.u8();
	LLC_TEST_REQUIREF(errors, VIEW_TEST_RESULT_CONST_CHAR_VIEW, (::llc::uP_t)constChars.begin() != (::llc::uP_t)data , "Const character view begin:%p, expected:%p." , constChars.begin(), data );
	LLC_TEST_REQUIREF(errors, VIEW_TEST_RESULT_CONST_CHAR_VIEW, constChars.size() != expectedBytes , "Const character view size:%u, expected:%u." , constChars.size(), expectedBytes );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_CONST_CHAR_VIEW, (::llc::u0_t)constChars[0] != 0x2A , "Const character view first byte:%u, expected:42." , (::llc::u0_t)constChars[0] );
	LLC_TEST_REQUIREF(errors, VIEW_TEST_RESULT_CONST_BYTE_VIEW, (::llc::uP_t)constBytes.begin() != (::llc::uP_t)data , "Const byte view begin:%p, expected:%p." , constBytes.begin(), data );
	LLC_TEST_REQUIREF(errors, VIEW_TEST_RESULT_CONST_BYTE_VIEW, constBytes.size() != expectedBytes , "Const byte view size:%u, expected:%u." , constBytes.size(), expectedBytes );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_CONST_BYTE_VIEW, constBytes[expectedBytes - 1] != 0x5A , "Const byte view last byte:%u, expected:90." , constBytes[expectedBytes - 1] );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_CONST_BYTE_ALIAS, constByteAlias.begin() != constBytes.begin() , "Const byte alias begin:%p, const u8 begin:%p." , constByteAlias.begin(), constBytes.begin() );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_CONST_BYTE_ALIAS, constByteAlias.size() != constBytes.size() , "Const byte alias size:%u, const u8 size:%u." , constByteAlias.size(), constBytes.size() );

	::llc::view<T> nullEmpty;
	cnst ::llc::view<T> & constNullEmpty = nullEmpty;
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_NULL_EMPTY_MUTABLE_CHARACTERS, nullEmpty.c(), 0, nullptr, nullptr));
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_NULL_EMPTY_MUTABLE_BYTES, nullEmpty.u8(), 0, nullptr, nullptr));
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_NULL_EMPTY_CONST_CHARACTERS, constNullEmpty.cc(), 0, nullptr, nullptr));
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_NULL_EMPTY_CONST_BYTES, constNullEmpty.u8(), 0, nullptr, nullptr));
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_NULL_EMPTY_CONST_BYTE_ALIAS, constNullEmpty.cu8(), 0, nullptr, nullptr));
	::llc::view<T> boundaryEmpty{data, 0};
	cnst ::llc::view<T> & constBoundaryEmpty = boundaryEmpty;
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_BOUNDARY_EMPTY_MUTABLE_CHARACTERS, boundaryEmpty.c(), 0, data, data));
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_BOUNDARY_EMPTY_MUTABLE_BYTES, boundaryEmpty.u8(), 0, data, data));
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_BOUNDARY_EMPTY_CONST_CHARACTERS, constBoundaryEmpty.cc(), 0, data, data));
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_BOUNDARY_EMPTY_CONST_BYTES, constBoundaryEmpty.u8(), 0, data, data));
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_BOUNDARY_EMPTY_CONST_BYTE_ALIAS, constBoundaryEmpty.cu8(), 0, data, data));
	return 0;
}

tplt<tpnm T, ::llc::u2_t N>
sttc ::llc::err_t testMutationValues(ATestError & errors, VIEW_TEST_RESULT result, cnst T (&actual)[N], cnst T (&expected)[N]) {
	for(::llc::u2_t iElement = 0; iElement < N; ++iElement)
		LLC_TEST_CHECKF(errors, result, actual[iElement] != expected[iElement] , "Mutation mismatch. index:%u, value:%" LLC_FMT_S3 ", expected:%" LLC_FMT_S3 "." , iElement, ::llc::s3_t(actual[iElement]), ::llc::s3_t(expected[iElement]) );
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testFill(ATestError & errors) {
	T full[6] = {T(0), T(1), T(2), T(3), T(4), T(5)};
	T fullExpected[6] = {T(9), T(9), T(9), T(9), T(9), T(9)};
	::llc::err_t result = ::llc::view<T>{full}.fill(T(9));
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FILL_FULL, result != (::llc::err_t)::llc::size(full) , "full fill returned:%i, expected:%u." , result, ::llc::u2_t(::llc::size(full)) );
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FILL_FULL, full, fullExpected));

	T ranged[6] = {T(0), T(1), T(2), T(3), T(4), T(5)};
	T rangedExpected[6] = {T(0), T(8), T(8), T(8), T(4), T(5)};
	result = ::llc::view<T>{ranged}.fill(T(8), 1, 4);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FILL_RANGE, result != 3 , "ranged fill returned:%i, expected:3." , result );
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FILL_RANGE, ranged, rangedExpected));

	T clipped[6] = {T(0), T(1), T(2), T(3), T(4), T(5)};
	T clippedExpected[6] = {T(0), T(1), T(2), T(7), T(7), T(7)};
	result = ::llc::view<T>{clipped}.fill(T(7), 3, 20);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FILL_CLIPPED, result != 3 , "clipped fill returned:%i, expected:3." , result );
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FILL_CLIPPED, clipped, clippedExpected));

	T unchanged[6] = {T(0), T(1), T(2), T(3), T(4), T(5)};
	T unchangedExpected[6] = {T(0), T(1), T(2), T(3), T(4), T(5)};
	result = ::llc::view<T>{unchanged}.fill(T(6), 4, 2);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FILL_EMPTY_RANGE, result , "empty-range fill returned:%i, expected:0." , result );
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FILL_EMPTY_RANGE, unchanged, unchangedExpected));

	::llc::view<T> nullEmpty;
	::llc::view<T> boundaryEmpty{unchanged, 0};
	cnst ::llc::err_t nullResult = nullEmpty.fill(T(1));
	cnst ::llc::err_t boundaryResult = boundaryEmpty.fill(T(1));
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FILL_EMPTY_VIEW, nullResult , "Null-empty fill result:%i, expected:0." , nullResult );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FILL_EMPTY_VIEW, boundaryResult , "Boundary-empty fill result:%i, expected:0." , boundaryResult );
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testReverse(ATestError & errors) {
	T odd[5] = {T(0), T(1), T(2), T(3), T(4)};
	T oddExpected[5] = {T(4), T(3), T(2), T(1), T(0)};
	::llc::err_t result = ::llc::view<T>{odd}.revert();
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_REVERT_ODD, result , "odd revert returned:%i." , result );
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_REVERT_ODD, odd, oddExpected));

	T even[6] = {T(9), T(1), T(2), T(3), T(4), T(8)};
	T evenExpected[6] = {T(9), T(4), T(3), T(2), T(1), T(8)};
	result = ::llc::view<T>{&even[1], 4}.revert();
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_REVERT_EVEN, result , "even subrange revert returned:%i." , result );
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_REVERT_EVEN, even, evenExpected));

	T one[1] = {T(5)};
	T oneExpected[1] = {T(5)};
	::llc::view<T> nullEmpty;
	::llc::view<T> boundaryEmpty{one, 0U};
	cnst ::llc::err_t oneRevertResult = ::llc::view<T>{one}.revert();
	cnst ::llc::err_t nullRevertResult = nullEmpty.revert();
	cnst ::llc::err_t boundaryRevertResult = boundaryEmpty.revert();
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_REVERT_EDGE, oneRevertResult , "Single-element revert result:%i, expected:0." , oneRevertResult );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_REVERT_EDGE, nullRevertResult , "Null-empty revert result:%i, expected:0." , nullRevertResult );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_REVERT_EDGE, boundaryRevertResult , "Boundary-empty revert result:%i, expected:0." , boundaryRevertResult );
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_REVERT_EDGE, one, oneExpected));

	T freeOdd[5] = {T(0), T(1), T(2), T(3), T(4)};
	result = ::llc::reverse(::llc::view<T>{freeOdd});
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_REVERSE_ODD, result , "odd reverse returned:%i." , result );
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_REVERSE_ODD, freeOdd, oddExpected));

	T freeEven[6] = {T(9), T(1), T(2), T(3), T(4), T(8)};
	result = ::llc::reverse(::llc::view<T>{&freeEven[1], 4});
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_REVERSE_EVEN, result , "even subrange reverse returned:%i." , result );
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_REVERSE_EVEN, freeEven, evenExpected));

	cnst ::llc::err_t oneReverseResult = ::llc::reverse(::llc::view<T>{one});
	cnst ::llc::err_t nullReverseResult = ::llc::reverse(nullEmpty);
	cnst ::llc::err_t boundaryReverseResult = ::llc::reverse(boundaryEmpty);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_REVERSE_EDGE, oneReverseResult , "Single-element reverse result:%i, expected:0." , oneReverseResult );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_REVERSE_EDGE, nullReverseResult , "Null-empty reverse result:%i, expected:0." , nullReverseResult );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_REVERSE_EDGE, boundaryReverseResult , "Boundary-empty reverse result:%i, expected:0." , boundaryReverseResult );
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_REVERSE_EDGE, one, oneExpected));
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testForEach(ATestError & errors) {
	T full[5] = {T(0), T(1), T(2), T(3), T(4)};
	T fullExpected[5] = {T(10), T(11), T(12), T(13), T(14)};
	::llc::u2_t visited = 0;
	::llc::TFuncForEach<T> addTen = [&visited](T & value) { ++visited; value += T(10); return 0; };
	::llc::err_t result = ::llc::view<T>{full}.for_each(addTen);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_FULL, result != 5 , "Mutable full for_each result:%i, expected:5.", result );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_FULL, visited != 5 , "Mutable full for_each visited:%u, expected:5.", visited );
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_FULL, full, fullExpected));

	T offset[5] = {T(0), T(1), T(2), T(3), T(4)};
	T offsetExpected[5] = {T(0), T(1), T(12), T(13), T(14)};
	visited = 0;
	result = ::llc::view<T>{offset}.for_each(addTen, 2);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_OFFSET, result != 3 , "Mutable offset for_each result:%i, expected:3.", result );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_OFFSET, visited != 3 , "Mutable offset for_each visited:%u, expected:3.", visited );
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_OFFSET, offset, offsetExpected));

	T ranged[5] = {T(0), T(1), T(2), T(3), T(4)};
	T rangedExpected[5] = {T(0), T(11), T(12), T(13), T(4)};
	visited = 0;
	result = ::llc::view<T>{ranged}.for_each(addTen, 1, 4);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_RANGE, result != 3 , "Mutable ranged for_each result:%i, expected:3.", result );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_RANGE, visited != 3 , "Mutable ranged for_each visited:%u, expected:3.", visited );
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_RANGE, ranged, rangedExpected));

	T clipped[5] = {T(0), T(1), T(2), T(3), T(4)};
	T clippedExpected[5] = {T(0), T(1), T(2), T(13), T(14)};
	visited = 0;
	result = ::llc::view<T>{clipped}.for_each(addTen, 3, 20);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_CLIPPED, result != 2 , "Mutable clipped for_each result:%i, expected:2.", result );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_CLIPPED, visited != 2 , "Mutable clipped for_each visited:%u, expected:2.", visited );
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_CLIPPED, clipped, clippedExpected));

	T constData[5] = {T(0), T(1), T(2), T(3), T(4)};
	cnst ::llc::view<T> constView{constData};
	::llc::s3_t sum = 0;
	visited = 0;
	::llc::TFuncForEachConst<T> read = [&visited, &sum](cnst T & value) { ++visited; sum += value; return 0; };
	result = constView.for_each(read, 2);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FOREACH_CONST, result != 3 , "Const offset for_each result:%i, expected:3.", result );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FOREACH_CONST, visited != 3 , "Const offset for_each visited:%u, expected:3.", visited );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FOREACH_CONST, sum != 9 , "Const offset for_each sum:%" LLC_FMT_S3 ", expected:9.", sum );

	T unchanged[5] = {T(0), T(1), T(2), T(3), T(4)};
	T unchangedExpected[5] = {T(0), T(1), T(2), T(3), T(4)};
	visited = 0;
	result = ::llc::view<T>{unchanged}.for_each(addTen, 4, 2);
	::llc::view<T> nullEmpty;
	cnst ::llc::err_t emptyResult = nullEmpty.for_each(addTen);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FOREACH_EMPTY, result , "Reversed-range for_each result:%i, expected:0.", result );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FOREACH_EMPTY, emptyResult , "Null-empty for_each result:%i, expected:0.", emptyResult );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FOREACH_EMPTY, visited , "Empty for_each visited:%u elements, expected:0.", visited );
	return testMutationValues(errors, VIEW_TEST_RESULT_FOREACH_EMPTY, unchanged, unchangedExpected);
}

tplt<tpnm T>
sttc ::llc::err_t testEnumerate(ATestError & errors) {
	T full[5] = {};
	T fullExpected[5] = {T(10), T(11), T(12), T(13), T(14)};
	::llc::u2_t visited = 0, indices = 0;
	::llc::TFuncEnumerate<T> writeIndex = [&visited, &indices](::llc::u2_t & index, T & value) { ++visited; indices |= 1U << index; value = T(10 + index); return 0; };
	::llc::err_t result = ::llc::view<T>{full}.enumerate(writeIndex);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_ENUMERATE_MUTABLE, result != 5 , "Mutable full enumerate result:%i, expected:5.", result );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_ENUMERATE_MUTABLE, visited != 5 , "Mutable full enumerate visited:%u, expected:5.", visited );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_ENUMERATE_MUTABLE, indices != 0x1F , "Mutable full enumerate indices:0x%X, expected:0x1F.", indices );
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_ENUMERATE_MUTABLE, full, fullExpected));

	T ranged[5] = {};
	T rangedExpected[5] = {T(0), T(11), T(12), T(13), T(0)};
	visited = indices = 0;
	result = ::llc::view<T>{ranged}.enumerate(writeIndex, 1, 4);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_ENUMERATE_RANGE, result != 3 , "Mutable ranged enumerate result:%i, expected:3.", result );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_ENUMERATE_RANGE, visited != 3 , "Mutable ranged enumerate visited:%u, expected:3.", visited );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_ENUMERATE_RANGE, indices != 0x0E , "Mutable ranged enumerate indices:0x%X, expected:0x0E.", indices );
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_ENUMERATE_RANGE, ranged, rangedExpected));

	T constData[5] = {T(0), T(1), T(2), T(3), T(4)};
	cnst ::llc::view<T> constView{constData};
	::llc::s3_t sum = 0;
	visited = indices = 0;
	::llc::TFuncEnumerateConst<T> readIndex = [&visited, &indices, &sum](::llc::u2_t & index, cnst T & value) { ++visited; indices |= 1U << index; sum += value; return 0; };
	result = constView.enumerate(readIndex, 2);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_ENUMERATE_CONST, result != 3 , "Const offset enumerate result:%i, expected:3.", result );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_ENUMERATE_CONST, visited != 3 , "Const offset enumerate visited:%u, expected:3.", visited );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_ENUMERATE_CONST, indices != 0x1C , "Const offset enumerate indices:0x%X, expected:0x1C.", indices );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_ENUMERATE_CONST, sum != 9 , "Const offset enumerate sum:%" LLC_FMT_S3 ", expected:9.", sum );

	visited = indices = 0;
	result = ::llc::view<T>{ranged}.enumerate(writeIndex, 4, 2);
	::llc::view<T> nullEmpty;
	cnst ::llc::err_t emptyResult = nullEmpty.enumerate(writeIndex);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_ENUMERATE_EMPTY, result , "Reversed-range enumerate result:%i, expected:0.", result );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_ENUMERATE_EMPTY, emptyResult , "Null-empty enumerate result:%i, expected:0.", emptyResult );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_ENUMERATE_EMPTY, visited , "Empty enumerate visited:%u elements, expected:0.", visited );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_ENUMERATE_EMPTY, indices , "Empty enumerate reported indices:0x%X, expected:0.", indices );
	return 0;
}

sttc ::llc::err_t viewFailureCheck(ATestError & errors, VIEW_TEST_RESULT result, ::llc::err_t callResult, ::llc::u2_t visited, ::llc::u2_t expectedVisits) {
	LLC_TEST_CHECKF(errors, result, !::llc::failed(callResult) , "Result:%i, expected failure." , callResult );
	LLC_TEST_CHECKF(errors, result, visited != expectedVisits , "Visited:%u, expected:%u." , visited, expectedVisits );
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testTraversalFailures(ATestError & errors) {
	T data[5] = {T(0), T(1), T(2), T(3), T(4)};
	::llc::view<T> mutableView{data};
	cnst ::llc::view<T> & constView = mutableView;
	::llc::u2_t mutableForEachVisits = 0, constForEachVisits = 0, mutableRangeVisits = 0, constRangeVisits = 0;
	::llc::u2_t mutableEnumerateVisits = 0, constEnumerateVisits = 0, mutableEnumerateRangeVisits = 0, constEnumerateRangeVisits = 0;
	::llc::TFuncForEach<T> mutableForEach = [&mutableForEachVisits](T &) { return (++mutableForEachVisits == 3) ? -1 : 0; };
	::llc::TFuncForEachConst<T> constForEach = [&constForEachVisits](cnst T &) { return (++constForEachVisits == 3) ? -1 : 0; };
	::llc::TFuncForEach<T> mutableRange = [&mutableRangeVisits](T &) { return (++mutableRangeVisits == 2) ? -1 : 0; };
	::llc::TFuncForEachConst<T> constRange = [&constRangeVisits](cnst T &) { return (++constRangeVisits == 2) ? -1 : 0; };
	::llc::TFuncEnumerate<T> mutableEnumerate = [&mutableEnumerateVisits](::llc::u2_t &, T &) { return (++mutableEnumerateVisits == 3) ? -1 : 0; };
	::llc::TFuncEnumerateConst<T> constEnumerate = [&constEnumerateVisits](::llc::u2_t &, cnst T &) { return (++constEnumerateVisits == 3) ? -1 : 0; };
	::llc::TFuncEnumerate<T> mutableEnumerateRange = [&mutableEnumerateRangeVisits](::llc::u2_t &, T &) { return (++mutableEnumerateRangeVisits == 2) ? -1 : 0; };
	::llc::TFuncEnumerateConst<T> constEnumerateRange = [&constEnumerateRangeVisits](::llc::u2_t &, cnst T &) { return (++constEnumerateRangeVisits == 2) ? -1 : 0; };

	::llc::setupLogCallbacks(0, 0);
	cnst ::llc::err_t mutableForEachResult = mutableView.for_each(mutableForEach);
	cnst ::llc::err_t constForEachResult = constView.for_each(constForEach);
	cnst ::llc::err_t mutableRangeResult = mutableView.for_each(mutableRange, 1, 5);
	cnst ::llc::err_t constRangeResult = constView.for_each(constRange, 1, 5);
	cnst ::llc::err_t mutableEnumerateResult = mutableView.enumerate(mutableEnumerate);
	cnst ::llc::err_t constEnumerateResult = constView.enumerate(constEnumerate);
	cnst ::llc::err_t mutableEnumerateRangeResult = mutableView.enumerate(mutableEnumerateRange, 1, 5);
	cnst ::llc::err_t constEnumerateRangeResult = constView.enumerate(constEnumerateRange, 1, 5);
	::llc::setupDefaultLogCallbacks();

	if_fail_fe(viewFailureCheck(errors, VIEW_TEST_RESULT_FAILURE_MUTABLE_FOR_EACH, mutableForEachResult, mutableForEachVisits, 3));
	if_fail_fe(viewFailureCheck(errors, VIEW_TEST_RESULT_FAILURE_CONST_FOR_EACH, constForEachResult, constForEachVisits, 3));
	if_fail_fe(viewFailureCheck(errors, VIEW_TEST_RESULT_FAILURE_MUTABLE_RANGED_FOR_EACH, mutableRangeResult, mutableRangeVisits, 2));
	if_fail_fe(viewFailureCheck(errors, VIEW_TEST_RESULT_FAILURE_CONST_RANGED_FOR_EACH, constRangeResult, constRangeVisits, 2));
	if_fail_fe(viewFailureCheck(errors, VIEW_TEST_RESULT_FAILURE_MUTABLE_ENUMERATE, mutableEnumerateResult, mutableEnumerateVisits, 3));
	if_fail_fe(viewFailureCheck(errors, VIEW_TEST_RESULT_FAILURE_CONST_ENUMERATE, constEnumerateResult, constEnumerateVisits, 3));
	if_fail_fe(viewFailureCheck(errors, VIEW_TEST_RESULT_FAILURE_MUTABLE_RANGED_ENUMERATE, mutableEnumerateRangeResult, mutableEnumerateRangeVisits, 2));
	if_fail_fe(viewFailureCheck(errors, VIEW_TEST_RESULT_FAILURE_CONST_RANGED_ENUMERATE, constEnumerateRangeResult, constEnumerateRangeVisits, 2));
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testFind(ATestError & errors) {
	T mutableData[5] = {T(0), T(1), T(2), T(3), T(2)};
	T mutableExpected[5] = {T(10), T(11), T(12), T(3), T(2)};
	::llc::u2_t visited = 0;
	::llc::FBool<T&> mutablePredicate = [&visited](T & value) { ++visited; cnst bool match = value == T(2); value += T(10); return match; };
	::llc::err_t result = ::llc::view<T>{mutableData}.find(mutablePredicate);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FIND_MUTABLE, result != 2 , "Mutable predicate find result:%i, expected:2.", result );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FIND_MUTABLE, visited != 3 , "Mutable predicate find visited:%u, expected:3.", visited );
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FIND_MUTABLE, mutableData, mutableExpected));

	T constData[5] = {T(0), T(1), T(2), T(3), T(2)};
	cnst ::llc::view<T> constView{constData};
	visited = 0;
	::llc::FBool<cnst T&> constPredicate = [&visited](cnst T & value) { ++visited; return value == T(2); };
	result = constView.find(constPredicate);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FIND_CONST, result != 2 , "Const predicate find result:%i, expected:2.", result );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FIND_CONST, visited != 3 , "Const predicate find visited:%u, expected:3.", visited );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FIND_VALUE, constView.find(T(2)) != 2 , "value find did not return index:2. result:%i." , constView.find(T(2)) );

	visited = 0;
	result = constView.find(constPredicate, 3);
	cnst ::llc::err_t valueOffsetResult = constView.find(T(2), 3);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FIND_OFFSET, result != 4 , "Offset predicate find result:%i, expected:4.", result );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FIND_OFFSET, visited != 2 , "Offset predicate find visited:%u, expected:2.", visited );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FIND_OFFSET, valueOffsetResult != 4 , "Offset value find result:%i, expected:4.", valueOffsetResult );

	visited = 0;
	::llc::FBool<cnst T&> missingPredicate = [&visited](cnst T & value) { ++visited; return value == T(9); };
	result = constView.find(missingPredicate);
	cnst ::llc::err_t missingValueResult = constView.find(T(9));
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FIND_NOT_FOUND, result != -1 , "Missing predicate find result:%i, expected:-1.", result );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FIND_NOT_FOUND, visited != constView.size() , "Missing predicate find visited:%u, expected:%u.", visited, constView.size() );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FIND_NOT_FOUND, missingValueResult != -1 , "Missing value find result:%i, expected:-1.", missingValueResult );

	visited = 0;
	::llc::view<T> empty;
	cnst ::llc::view<T> & constEmpty = empty;
	cnst ::llc::err_t emptyPredicateResult = constEmpty.find(missingPredicate);
	cnst ::llc::err_t emptyValueResult = constEmpty.find(T(0));
	cnst ::llc::err_t pastEndResult = constView.find(missingPredicate, constView.size());
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FIND_EMPTY, emptyPredicateResult != -1 , "Empty predicate find result:%i, expected:-1.", emptyPredicateResult );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FIND_EMPTY, emptyValueResult != -1 , "Empty value find result:%i, expected:-1.", emptyValueResult );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FIND_EMPTY, pastEndResult != -1 , "Past-end predicate find result:%i, expected:-1.", pastEndResult );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FIND_EMPTY, visited , "Empty/past-end find visited:%u elements, expected:0.", visited );

	cnst ::llc::err_t lastValue = constView.rfind(T(2));
	cnst ::llc::err_t offsetValue = constView.rfind(T(2), 1);
	cnst ::llc::err_t missingValue = constView.rfind(T(9));
	cnst ::llc::err_t emptyValue = constEmpty.rfind(T(0));
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_RFIND_VALUE, lastValue != 4 , "Value rfind result:%i, expected:4.", lastValue );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_RFIND_VALUE, offsetValue != 2 , "Offset value rfind result:%i, expected:2.", offsetValue );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_RFIND_VALUE, missingValue != -1 , "Missing value rfind result:%i, expected:-1.", missingValue );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_RFIND_VALUE, emptyValue != -1 , "Empty value rfind result:%i, expected:-1.", emptyValue );

	T sequenceTargetData[6] = {T(0), T(1), T(2), T(3), T(2), T(3)};
	T sequenceData[2] = {T(2), T(3)};
	cnst ::llc::view<T> sequenceTarget{sequenceTargetData};
	cnst ::llc::view<T> sequence{sequenceData};
	cnst ::llc::err_t firstSequence = sequenceTarget.find(sequence);
	cnst ::llc::err_t offsetSequence = sequenceTarget.find(sequence, 3);
	cnst ::llc::err_t emptySequence = sequenceTarget.find(constEmpty);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FIND_SEQUENCE, firstSequence != 2 , "Sequence find result:%i, expected:2.", firstSequence );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FIND_SEQUENCE, offsetSequence != 4 , "Offset sequence find result:%i, expected:4.", offsetSequence );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_FIND_SEQUENCE, emptySequence , "Empty sequence find result:%i, expected:0.", emptySequence );
	cnst ::llc::err_t lastSequence = sequenceTarget.rfind(sequence);
	cnst ::llc::err_t reverseOffsetSequence = sequenceTarget.rfind(sequence, 1);
	cnst ::llc::err_t reverseEmptySequence = sequenceTarget.rfind(constEmpty);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_RFIND_SEQUENCE, lastSequence != 4 , "Sequence rfind result:%i, expected:4.", lastSequence );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_RFIND_SEQUENCE, reverseOffsetSequence != 2 , "Offset sequence rfind result:%i, expected:2.", reverseOffsetSequence );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_RFIND_SEQUENCE, reverseEmptySequence != 6 , "Empty sequence rfind result:%i, expected:6.", reverseEmptySequence );
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testExtrema(ATestError & errors) {
	T data[6] = {T(3), T(7), T(1), T(7), T(2), T(6)};
	cnst ::llc::view<T> values{data};
	::llc::FTransform<::llc::s3_t, cnst T&> transform = [](cnst T & value) { return (::llc::s3_t)value; };
	::llc::s3_t maximum = 999, minimum = -999;
	::llc::err_t iMaximum = values.max(maximum, transform);
	::llc::err_t iMinimum = values.min(minimum, transform);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_MAXIMUM_OUTPUT, iMaximum != 1 , "Maximum index:%i, expected:1.", iMaximum );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_MAXIMUM_OUTPUT, maximum != 7 , "Maximum value:%" LLC_FMT_S3 ", expected:7.", maximum );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_MINIMUM_OUTPUT, iMinimum != 2 , "Minimum index:%i, expected:2.", iMinimum );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_MINIMUM_OUTPUT, minimum != 1 , "Minimum value:%" LLC_FMT_S3 ", expected:1.", minimum );

	maximum = 999;
	minimum = -999;
	iMaximum = values.max(maximum, transform, 3);
	iMinimum = values.min(minimum, transform, 3);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EXTREMA_OFFSET, iMaximum != 3 , "Offset maximum index:%i, expected:3.", iMaximum );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EXTREMA_OFFSET, maximum != 7 , "Offset maximum value:%" LLC_FMT_S3 ", expected:7.", maximum );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EXTREMA_OFFSET, iMinimum != 4 , "Offset minimum index:%i, expected:4.", iMinimum );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EXTREMA_OFFSET, minimum != 2 , "Offset minimum value:%" LLC_FMT_S3 ", expected:2.", minimum );

	iMaximum = values.max(transform);
	iMinimum = values.min(transform);
	cnst ::llc::err_t offsetMaximum = values.max(transform, 3);
	cnst ::llc::err_t offsetMinimum = values.min(transform, 3);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EXTREMA_CONVENIENCE, iMaximum != 1 , "Convenience maximum index:%i, expected:1.", iMaximum );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EXTREMA_CONVENIENCE, iMinimum != 2 , "Convenience minimum index:%i, expected:2.", iMinimum );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EXTREMA_CONVENIENCE, offsetMaximum != 3 , "Convenience offset maximum index:%i, expected:3.", offsetMaximum );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EXTREMA_CONVENIENCE, offsetMinimum != 4 , "Convenience offset minimum index:%i, expected:4.", offsetMinimum );

	T singleData[1] = {T(5)};
	cnst ::llc::view<T> single{singleData};
	maximum = 999;
	minimum = -999;
	iMaximum = single.max(maximum, transform);
	iMinimum = single.min(minimum, transform);
	cnst ::llc::err_t singleMaximum = single.max(transform);
	cnst ::llc::err_t singleMinimum = single.min(transform);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EXTREMA_SINGLE, iMaximum , "Single maximum index:%i, expected:0.", iMaximum );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EXTREMA_SINGLE, maximum != 5 , "Single maximum value:%" LLC_FMT_S3 ", expected:5.", maximum );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EXTREMA_SINGLE, iMinimum , "Single minimum index:%i, expected:0.", iMinimum );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EXTREMA_SINGLE, minimum != 5 , "Single minimum value:%" LLC_FMT_S3 ", expected:5.", minimum );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EXTREMA_SINGLE, singleMaximum , "Single convenience maximum index:%i, expected:0.", singleMaximum );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EXTREMA_SINGLE, singleMinimum , "Single convenience minimum index:%i, expected:0.", singleMinimum );

	cnst ::llc::view<T> empty;
	maximum = 123;
	minimum = 456;
	::llc::u2_t transformed = 0;
	::llc::FTransform<::llc::s3_t, cnst T&> countTransform = [&transformed](cnst T & value) { ++transformed; return (::llc::s3_t)value; };
	::llc::setupLogCallbacks(0, 0);
	iMaximum = empty.max(maximum, countTransform);
	iMinimum = empty.min(minimum, countTransform);
	cnst ::llc::err_t convenienceMaximum = empty.max(countTransform);
	cnst ::llc::err_t convenienceMinimum = empty.min(countTransform);
	::llc::setupDefaultLogCallbacks();
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EXTREMA_EMPTY, iMaximum != -1 , "Empty maximum index:%i, expected:-1.", iMaximum );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EXTREMA_EMPTY, iMinimum != -1 , "Empty minimum index:%i, expected:-1.", iMinimum );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EXTREMA_EMPTY, maximum != 123 , "Empty maximum changed output:%" LLC_FMT_S3 ", expected:123.", maximum );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EXTREMA_EMPTY, minimum != 456 , "Empty minimum changed output:%" LLC_FMT_S3 ", expected:456.", minimum );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EXTREMA_EMPTY, transformed , "Empty extrema called transform:%u times, expected:0.", transformed );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EXTREMA_EMPTY, convenienceMaximum != -1 , "Empty convenience maximum index:%i, expected:-1.", convenienceMaximum );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_EXTREMA_EMPTY, convenienceMinimum != -1 , "Empty convenience minimum index:%i, expected:-1.", convenienceMinimum );
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testSplit(ATestError & errors) {
	T values[5] = {T(1), T(2), T(3), T(2), T(4)};
	::llc::view<T> source{values};
	::llc::view<T> left = source, right;
	::llc::err_t result = ::llc::split(T(2), left);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_VALUE, result != 1 , "In-place scalar split result:%i, expected:1.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_IN_PLACE_SCALAR_SPLIT_LEFT, left, 1, values, &values[1]));

	left = source;
	result = ::llc::split(T(9), left);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_VALUE_MISSING, result != 5 , "Missing in-place scalar split result:%i, expected:5.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_MISSING_IN_PLACE_SCALAR_SPLIT_LEFT, left, 5, values, source.end()));

	result = ::llc::split(T(2), source, left, right);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_VALUE, result != 1 , "Scalar split result:%i, expected:1.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_SCALAR_SPLIT_LEFT, left, 1, values, &values[1]));
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_SCALAR_SPLIT_RIGHT, right, 3, &values[2], source.end()));

	result = ::llc::split(T(1), source, left, right);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_VALUE_BOUNDARY, result , "Leading scalar split result:%i, expected:0.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_LEADING_SCALAR_SPLIT_LEFT, left, 0, values, values));
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_LEADING_SCALAR_SPLIT_RIGHT, right, 4, &values[1], source.end()));
	result = ::llc::split(T(4), source, left, right);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_VALUE_BOUNDARY, result != 4 , "Trailing scalar split result:%i, expected:4.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_TRAILING_SCALAR_SPLIT_LEFT, left, 4, values, &values[4]));
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_TRAILING_SCALAR_SPLIT_RIGHT, right, 0, source.end(), source.end()));

	result = ::llc::split(T(9), source, left, right);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_VALUE_MISSING, result != -1 , "Missing scalar split result:%i, expected:-1.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_MISSING_SCALAR_SPLIT_LEFT, left, 5, values, source.end()));
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_VALUE_MISSING, right.size() , "Missing scalar split right count:%u, expected:0.", right.size() );

	result = ::llc::splitAt(T(2), source, left, right);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_AT_VALUE, result != 1 , "Scalar splitAt result:%i, expected:1.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_SCALAR_SPLIT_AT_LEFT, left, 1, values, &values[1]));
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_SCALAR_SPLIT_AT_RIGHT, right, 4, &values[1], source.end()));

	::llc::view<cnst T> constLeft, constRight;
	result = ::llc::split(T(2), source, constLeft, constRight);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_VALUE_CONST_OUTPUT, result != 1 , "Const-output scalar split result:%i, expected:1.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_CONST_OUTPUT_SCALAR_SPLIT_LEFT, constLeft, 1, values, &values[1]));
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_CONST_OUTPUT_SCALAR_SPLIT_RIGHT, constRight, 3, &values[2], source.end()));
	result = ::llc::splitAt(T(2), source, constLeft, constRight);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_AT_VALUE_CONST_OUTPUT, result != 1 , "Const-output scalar splitAt result:%i, expected:1.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_CONST_OUTPUT_SCALAR_SPLIT_AT_LEFT, constLeft, 1, values, &values[1]));
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_CONST_OUTPUT_SCALAR_SPLIT_AT_RIGHT, constRight, 4, &values[1], source.end()));

	cnst ::llc::view<cnst T> constSource{values};
	result = ::llc::split(T(2), constSource, constLeft, constRight);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_VALUE, result != 1 , "Const scalar split result:%i, expected:1.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_CONST_SCALAR_SPLIT_LEFT, constLeft, 1, values, &values[1]));
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_CONST_SCALAR_SPLIT_RIGHT, constRight, 3, &values[2], source.end()));

	T sequenceValues[2] = {T(2), T(3)};
	cnst ::llc::view<T> sequence{sequenceValues};
	result = ::llc::split(sequence, source, left, right);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_SEQUENCE, result != 1 , "Sequence split result:%i, expected:1.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_SEQUENCE_SPLIT_LEFT, left, 1, values, &values[1]));
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_SEQUENCE_SPLIT_RIGHT, right, 2, &values[3], source.end()));

	T missingValues[2] = {T(8), T(9)};
	cnst ::llc::view<T> missingSequence{missingValues};
	result = ::llc::split(missingSequence, source, left, right);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_SEQUENCE_MISSING, result != -1 , "Missing sequence split result:%i, expected:-1.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_MISSING_SEQUENCE_SPLIT_LEFT, left, 5, values, source.end()));
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_SEQUENCE_MISSING, right.size() , "Missing sequence split right count:%u, expected:0.", right.size() );

	result = ::llc::splitAt(sequence, source, left, right);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_AT_SEQUENCE, result != 1 , "Sequence splitAt result:%i, expected:1.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_SEQUENCE_SPLIT_AT_LEFT, left, 1, values, &values[1]));
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_SEQUENCE_SPLIT_AT_RIGHT, right, 4, &values[1], source.end()));
	result = ::llc::split(sequence, source, constLeft, constRight);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_SEQUENCE_CONST_OUTPUT, result != 1 , "Const-output sequence split result:%i, expected:1.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_CONST_OUTPUT_SEQUENCE_SPLIT_LEFT, constLeft, 1, values, &values[1]));
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_CONST_OUTPUT_SEQUENCE_SPLIT_RIGHT, constRight, 2, &values[3], source.end()));
	result = ::llc::splitAt(sequence, source, constLeft, constRight);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_AT_SEQUENCE_CONST_OUTPUT, result != 1 , "Const-output sequence splitAt result:%i, expected:1.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_CONST_OUTPUT_SEQUENCE_SPLIT_AT_LEFT, constLeft, 1, values, &values[1]));
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_CONST_OUTPUT_SEQUENCE_SPLIT_AT_RIGHT, constRight, 4, &values[1], source.end()));

	left = source;
	result = ::llc::split(sequence, left, right);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_SEQUENCE_IN_PLACE, result != 1 , "In-place sequence split result:%i, expected:1.", result );
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_IN_PLACE_SEQUENCE_SPLIT_LEFT, left, 1, values, &values[1]));
	if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_IN_PLACE_SEQUENCE_SPLIT_RIGHT, right, 2, &values[3], source.end()));
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testSplitCollections(ATestError & errors) {
	{
		T values[3] = {T(1), T(0), T(2)};
		::llc::aobj<::llc::view<cnst T>> output;
		cnst ::llc::err_t result = ::llc::split(::llc::view<cnst T>{values}, T(0), output);
		LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_COLLECTION, result != 2 , "Ordinary collection result:%i, expected:2.", result );
		LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_COLLECTION, output.size() != 2 , "Ordinary collection count:%u, expected:2.", output.size() );
		if(1 <= output.size()) {
			if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_ORDINARY_COLLECTION_FIELD_0, output[0], 1, values, &values[1]));
		}
		if(2 <= output.size()) {
			if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_ORDINARY_COLLECTION_FIELD_1, output[1], 1, &values[2], &values[3]));
		}
	}
	{
		T values[4] = {T(1), T(0), T(0), T(2)};
		::llc::aobj<::llc::view<cnst T>> output;
		cnst ::llc::err_t result = ::llc::split(::llc::view<cnst T>{values}, T(0), output);
		LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_COLLECTION, result != 2 , "Consecutive-delimiter collection result:%i, expected:2.", result );
		LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_COLLECTION, output.size() != 2 , "Consecutive-delimiter collection count:%u, expected:2.", output.size() );
		if(1 <= output.size()) {
			if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_CONSECUTIVE_DELIMITER_FIELD_0, output[0], 1, values, &values[1]));
		}
		if(2 <= output.size()) {
			if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_CONSECUTIVE_DELIMITER_FIELD_1, output[1], 1, &values[3], &values[4]));
		}
	}
	{
		T values[3] = {T(0), T(1), T(0)};
		::llc::aobj<::llc::view<cnst T>> output;
		cnst ::llc::err_t result = ::llc::split(::llc::view<cnst T>{values}, T(0), output);
		LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_COLLECTION, result != 1 , "Boundary-delimiter collection result:%i, expected:1.", result );
		LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_COLLECTION, output.size() != 1 , "Boundary-delimiter collection count:%u, expected:1.", output.size() );
		if(1 <= output.size()) {
			if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_BOUNDARY_DELIMITER_FIELD_0, output[0], 1, &values[1], &values[2]));
		}
	}
	{
		T values[2] = {T(0), T(0)};
		::llc::aobj<::llc::view<cnst T>> output;
		cnst ::llc::err_t result = ::llc::split(::llc::view<cnst T>{values}, T(0), output);
		LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_COLLECTION, result , "Delimiter-only collection result:%i, expected:0.", result );
		LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_COLLECTION, output.size() , "Delimiter-only collection count:%u, expected:0.", output.size() );
	}
	{
		T values[1] = {T(1)};
		::llc::aobj<::llc::view<cnst T>> output;
		cnst ::llc::err_t result = ::llc::split(::llc::view<cnst T>{values}, T(0), output);
		LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_COLLECTION, result != 1 , "Delimiter-free collection result:%i, expected:1.", result );
		LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_COLLECTION, output.size() != 1 , "Delimiter-free collection count:%u, expected:1.", output.size() );
		if(1 <= output.size()) {
			if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_DELIMITER_FREE_FIELD_0, output[0], 1, values, &values[1]));
		}
	}
	{
		::llc::aobj<::llc::view<cnst T>> output;
		cnst ::llc::err_t result = ::llc::split(::llc::view<cnst T>{}, T(0), output);
		LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_COLLECTION, result , "Empty collection result:%i, expected:0.", result );
		LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_COLLECTION, output.size() , "Empty collection count:%u, expected:0.", output.size() );
	}
	{
		T seed[1] = {T(9)};
		T values[4] = {T(1), T(0), T(0), T(2)};
		::llc::aobj<::llc::view<cnst T>> output;
		if_fail_fe(output.push_back({seed}));
		cnst ::llc::err_t result = ::llc::split(::llc::view<cnst T>{values}, T(0), output);
		LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_COLLECTION_APPEND, result != 3 , "Append collection result:%i, expected:3.", result );
		LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_COLLECTION_APPEND, output.size() != 3 , "Append collection count:%u, expected:3.", output.size() );
		if(1 <= output.size()) {
			if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_APPEND_COLLECTION_SEED, output[0], 1, seed, &seed[1]));
		}
		if(2 <= output.size()) {
			if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_APPEND_COLLECTION_FIELD_1, output[1], 1, values, &values[1]));
		}
		if(3 <= output.size()) {
			if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_APPEND_COLLECTION_FIELD_2, output[2], 1, &values[3], &values[4]));
		}
	}
	{
		T values[11] = {T(0), T(3), T(1), T(0), T(2), T(3), T(0), T(3), T(4), T(0), T(3)};
		T separators[2] = {T(0), T(3)};
		::llc::aobj<::llc::view<cnst T>> output;
		cnst ::llc::err_t result = ::llc::split(::llc::view<cnst T>{values}, ::llc::view<cnst T>{separators}, output);
		LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_COLLECTION_SEQUENCE, result != 2 , "Delimiter-sequence collection result:%i, expected:2.", result );
		LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_COLLECTION_SEQUENCE, output.size() != 2 , "Delimiter-sequence collection count:%u, expected:2.", output.size() );
		if(1 <= output.size()) {
			if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_DELIMITER_SEQUENCE_FIELD_0, output[0], 4, &values[2], &values[6]));
		}
		if(2 <= output.size()) {
			if_fail_fe(viewRangeCheck(errors, VIEW_TEST_RESULT_RANGE_DELIMITER_SEQUENCE_FIELD_1, output[1], 1, &values[8], &values[9]));
		}
	}
	rtrn 0;
}

sttc ::llc::err_t testStringSplitCollection(ATestError & errors) {
	::llc::aobj<::llc::view<cnst ::llc::sc_t>> output;
	cnst ::llc::err_t result = ::llc::split(LLC_CXS(",a,,b,"), ',', output);
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_STRING_COLLECTION, result != 2 , "String collection result:%i, expected:2.", result );
	LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_STRING_COLLECTION, output.size() != 2 , "String collection count:%u, expected:2.", output.size() );
	if(1 <= output.size()) {
		LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_STRING_COLLECTION, output[0] != LLC_CXS("a") , "String collection field 0:'%.*s', expected:'a'." , (int)output[0].size(), output[0].begin() );
	}
	if(2 <= output.size()) {
		LLC_TEST_CHECKF(errors, VIEW_TEST_RESULT_SPLIT_STRING_COLLECTION, output[1] != LLC_CXS("b") , "String collection field 1:'%.*s', expected:'b'." , (int)output[1].size(), output[1].begin() );
	}

	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testInvalidConstruction(ATestError & errors) {
#ifdef LLC_WINDOWS
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_INVALID_CONSTRUCTION, !testThrows([&]() { cnst ::llc::view<T> invalid{nullptr, 1}; (void)invalid; }));
#else
	(void)errors;
#endif
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testType(ATestError & errors) {
	if_fail_fe(testRepresentation<T>(errors));
	if_fail_fe(testMutableSlice<T>(errors));
	if_fail_fe(testConstSlice<T>(errors));
	if_fail_fe(testSubscript<T>(errors));
	if_fail_fe(testEquality<T>(errors));
	if_fail_fe(testRepresentationViews<T>(errors));
	if_fail_fe(testFill<T>(errors));
	if_fail_fe(testReverse<T>(errors));
	if_fail_fe(testForEach<T>(errors));
	if_fail_fe(testEnumerate<T>(errors));
	if_fail_fe(testTraversalFailures<T>(errors));
	if_fail_fe(testFind<T>(errors));
	if_fail_fe(testSplit<T>(errors));
	if_fail_fe(testSplitCollections<T>(errors));
	if_fail_fe(testExtrema<T>(errors));
	return testInvalidConstruction<T>(errors);
}

tplt<tpnm T>
sttc ::llc::err_t testTypeLogged(ATestError & errors) {
	cnst ::llc::u2_t checkCount = testCheckCount(errors);
	cnst ::llc::u2_t failureCount = testErrorCount(errors);
	if_fail_fe(testType<T>(errors));
	cnst ::llc::u2_t typeFailures = testErrorCount(errors) - failureCount;
	cnst ::llc::u2_t typeChecks = testCheckCount(errors) - checkCount;
	if(typeFailures) error_printf("%s suite completed: %u/%u checks passed, %u failed.", ::llc::get_type_namep<T>(), typeChecks - typeFailures, typeChecks, typeFailures);
	return 0;
}

::llc::err_t testView(ATestError & errors) {
	cnst ::llc::u2_t failureCount = testErrorCount(errors);
	if_fail_fe(testTypeLogged<::llc::u0_t>(errors));
	if_fail_fe(testTypeLogged<::llc::u1_t>(errors));
	if_fail_fe(testTypeLogged<::llc::u2_t>(errors));
	if_fail_fe(testTypeLogged<::llc::u3_t>(errors));
	if_fail_fe(testTypeLogged<::llc::s0_t>(errors));
	if_fail_fe(testTypeLogged<::llc::s1_t>(errors));
	if_fail_fe(testTypeLogged<::llc::s2_t>(errors));
	if_fail_fe(testTypeLogged<::llc::s3_t>(errors));
	if_fail_fe(testStringSplitCollection(errors));
	if(failureCount == testErrorCount(errors))
		always_printf("Types tested successfully:\n%s, %s, %s, %s, %s, %s, %s, %s."
			, ::llc::get_type_namep<::llc::u0_t>(), ::llc::get_type_namep<::llc::u1_t>(), ::llc::get_type_namep<::llc::u2_t>(), ::llc::get_type_namep<::llc::u3_t>()
			, ::llc::get_type_namep<::llc::s0_t>(), ::llc::get_type_namep<::llc::s1_t>(), ::llc::get_type_namep<::llc::s2_t>(), ::llc::get_type_namep<::llc::s3_t>()
			);
	return 0;
}
