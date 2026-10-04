#include "llc_view.h"
#include "llc_array_obj.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(VIEW_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, OK						, 0, "All view tests passed.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, DEFAULT_STATE				, 1, "A default view was not empty with null boundaries.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, ARRAY_CONSTRUCTION		, 2, "An array-backed view did not preserve its storage and full element count.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, COUNT_FIRST_CONSTRUCTION	, 3, "A count-first array view did not clip its element count to the backing array.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, ARRAY_FIRST_CONSTRUCTION	, 4, "An array-first view did not preserve its explicitly checked element count.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, POINTER_CONSTRUCTION		, 5, "A pointer-backed view did not preserve its supplied range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, EMPTY_RANGE				, 6, "A zero-length view did not preserve equal begin and end boundaries.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, CONST_CONVERSION			, 7, "Converting a mutable view to a const view changed its range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, INVALID_CONSTRUCTION		, 8, "view<> accepted null storage with a nonzero element count.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_FULL				, 9, "slice() did not reproduce the full source range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_REMAINDER			, 10, "slice() did not produce the expected range after an offset.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_COUNT				, 11, "slice() did not honor an explicit valid element count.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_END					, 12, "slice() at the exact end did not produce an empty one-past-end range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_SELF				, 13, "slice() could not advance and shorten its own view.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_INVALID_OFFSET		, 14, "slice() accepted an offset beyond the source range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_INVALID_COUNT		, 15, "slice() accepted a count beyond the remaining source range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_FAILURE_STATE		, 16, "A failed slice() modified its output view.");
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
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, EMPTY_REPRESENTATION		, 36, "A representation projection did not preserve an empty view boundary.");
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
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FOREACH_FAILURE			, 58, "for_each() did not stop and propagate a callback failure.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, ENUMERATE_FAILURE			, 59, "enumerate() did not stop and propagate a callback failure.");
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
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SPLIT_COLLECTION_SET		, 82, "The delimiter-set split collector did not discard empty fields and delimiters.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SPLIT_STRING_COLLECTION	, 83, "The string split collector did not discard empty fields and delimiters.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SPLIT_HETEROGENEOUS		, 84, "The split collector narrowed a comparable separator before testing equality.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_CONST_OUTPUT		, 85, "A mutable view did not produce the requested const slice.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SPLIT_VALUE_CONST_OUTPUT	, 86, "split() did not project a mutable scalar-split source into const output views.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SPLIT_AT_VALUE_CONST_OUTPUT, 87, "splitAt() did not project a mutable scalar-split source into const output views.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SPLIT_SEQUENCE_CONST_OUTPUT, 88, "split() did not project a mutable sequence-split source into const output views.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SPLIT_AT_SEQUENCE_CONST_OUTPUT, 89, "splitAt() did not project a mutable sequence-split source into const output views.");

tplt<tpnm T>
sttc ::llc::err_t testRepresentation(ATestError & errors) {
	T data[5] = {};
	::llc::view<T> empty;
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_DEFAULT_STATE, empty.size() || empty.begin() || empty.end()
		, "default range mismatch. size:%u, begin:%p, end:%p."
		, empty.size(), empty.begin(), empty.end()
		);

	::llc::view<T> full{data};
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_ARRAY_CONSTRUCTION, full.size() != ::llc::size(data) || full.begin() != data || full.end() != data + ::llc::size(data)
		, "array range mismatch. size:%u, expected:%u, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, full.size(), ::llc::u2_t(::llc::size(data)), full.begin(), data, full.end(), (data + ::llc::size(data))
		);

	::llc::view<T> partialCountFirst{3U, data};
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_COUNT_FIRST_CONSTRUCTION, partialCountFirst.size() != 3 || partialCountFirst.begin() != data || partialCountFirst.end() != data + 3
		, "count-first range mismatch. size:%u, expected:3, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, partialCountFirst.size(), partialCountFirst.begin(), data, partialCountFirst.end(), (data + 3)
		);
	::llc::view<T> clippedCountFirst{::llc::u2_t(::llc::size(data) + 1), data};
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_COUNT_FIRST_CONSTRUCTION, clippedCountFirst.size() != ::llc::size(data) || clippedCountFirst.begin() != data || clippedCountFirst.end() != data + ::llc::size(data)
		, "clipped count-first range mismatch. size:%u, expected:%u, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, clippedCountFirst.size(), ::llc::u2_t(::llc::size(data)), clippedCountFirst.begin(), data, clippedCountFirst.end(), (data + ::llc::size(data))
		);

	::llc::view<T> partialArrayFirst{data, 3U};
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_ARRAY_FIRST_CONSTRUCTION, partialArrayFirst.size() != 3 || partialArrayFirst.begin() != data || partialArrayFirst.end() != data + 3
		, "array-first range mismatch. size:%u, expected:3, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, partialArrayFirst.size(), partialArrayFirst.begin(), data, partialArrayFirst.end(), (data + 3)
		);

	::llc::view<T> pointerRange{data + 1, 3};
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_POINTER_CONSTRUCTION, pointerRange.size() != 3 || pointerRange.begin() != data + 1 || pointerRange.end() != data + 4
		, "pointer range mismatch. size:%u, expected:3, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, pointerRange.size(), pointerRange.begin(), (data + 1), pointerRange.end(), (data + 4)
		);

	::llc::view<T> emptyAtData{data, 0};
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EMPTY_RANGE, emptyAtData.size() || emptyAtData.begin() != data || emptyAtData.end() != data
		, "zero-length range mismatch. size:%u, begin:%p, expected boundary:%p, end:%p."
		, emptyAtData.size(), emptyAtData.begin(), data, emptyAtData.end()
		);
	::llc::view<T> emptyAtNull{(T*)0, 0};
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EMPTY_RANGE, emptyAtNull.size() || emptyAtNull.begin() || emptyAtNull.end()
		, "null zero-length range mismatch. size:%u, begin:%p, end:%p."
		, emptyAtNull.size(), emptyAtNull.begin(), emptyAtNull.end()
		);

	::llc::view<cnst T> readOnly = pointerRange;
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_CONST_CONVERSION, readOnly.size() != pointerRange.size() || readOnly.begin() != pointerRange.begin() || readOnly.end() != pointerRange.end()
		, "const conversion mismatch. size:%u, expected:%u, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, readOnly.size(), pointerRange.size(), readOnly.begin(), pointerRange.begin(), readOnly.end(), pointerRange.end()
		);
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
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_FULL, result != 5 || output.size() != 5 || output.begin() != data || output.end() != data + 5
		, "mutable full slice mismatch. result:%i, size:%u, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, result, output.size(), output.begin(), data, output.end(), (data + 5)
		);
	result = source.slice(output, 2);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_REMAINDER, result != 3 || output.size() != 3 || output.begin() != data + 2 || output.end() != data + 5
		, "mutable remainder slice mismatch. result:%i, size:%u, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, result, output.size(), output.begin(), (data + 2), output.end(), (data + 5)
		);
	result = source.slice(output, 1, 2);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_COUNT, result != 2 || output.size() != 2 || output.begin() != data + 1 || output.end() != data + 3
		, "mutable counted slice mismatch. result:%i, size:%u, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, result, output.size(), output.begin(), (data + 1), output.end(), (data + 3)
		);
	result = source.slice(output, 5);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_END, result || output.size() || output.begin() != data + 5 || output.end() != data + 5
		, "mutable end slice mismatch. result:%i, size:%u, begin:%p, expected boundary:%p, end:%p."
		, result, output.size(), output.begin(), (data + 5), output.end()
		);
	::llc::view<cnst T> constOutput;
	result = source.slice(constOutput, 1, 2);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_CONST_OUTPUT, result != 2 || constOutput.size() != 2 || constOutput.begin() != data + 1 || constOutput.end() != data + 3
		, "mutable-to-const slice mismatch. result:%i, size:%u, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, result, constOutput.size(), constOutput.begin(), (data + 1), constOutput.end(), (data + 3)
		);

	::llc::view<T> self{data};
	result = self.slice(self, 2, 2);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_SELF, result != 2 || self.size() != 2 || self.begin() != data + 2 || self.end() != data + 4
		, "mutable self-slice mismatch. result:%i, size:%u, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, result, self.size(), self.begin(), (data + 2), self.end(), (data + 4)
		);

	T preserved[2] = {};
	::llc::view<T> failedOutput{preserved};
	result = sliceExpectedFailure(source, failedOutput, 6);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_INVALID_OFFSET, 0 <= result
		, "mutable slice accepted offset:6 for size:5. result:%i."
		, result
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_FAILURE_STATE, failedOutput.begin() != preserved || failedOutput.size() != ::llc::size(preserved)
		, "mutable invalid-offset slice changed output. begin:%p, expected:%p, size:%u, expected:%u."
		, failedOutput.begin(), preserved, failedOutput.size(), ::llc::u2_t(::llc::size(preserved))
		);
	result = sliceExpectedFailure(source, failedOutput, 3, 3);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_INVALID_COUNT, 0 <= result
		, "mutable slice accepted count:3 with only 2 elements remaining. result:%i."
		, result
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_FAILURE_STATE, failedOutput.begin() != preserved || failedOutput.size() != ::llc::size(preserved)
		, "mutable invalid-count slice changed output. begin:%p, expected:%p, size:%u, expected:%u."
		, failedOutput.begin(), preserved, failedOutput.size(), ::llc::u2_t(::llc::size(preserved))
		);

	::llc::view<T> empty;
	output = source;
	result = empty.slice(output, 0);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_EMPTY, result || output.size() || output.begin() || output.end()
		, "mutable empty slice mismatch. result:%i, size:%u, begin:%p, end:%p."
		, result, output.size(), output.begin(), output.end()
		);
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testConstSlice(ATestError & errors) {
	T data[5] = {};
	cnst ::llc::view<T> source{data};
	::llc::view<cnst T> output;

	::llc::err_t result = source.slice(output, 0);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_FULL, result != 5 || output.size() != 5 || output.begin() != data || output.end() != data + 5
		, "const full slice mismatch. result:%i, size:%u, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, result, output.size(), output.begin(), data, output.end(), (data + 5)
		);
	result = source.slice(output, 2);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_REMAINDER, result != 3 || output.size() != 3 || output.begin() != data + 2 || output.end() != data + 5
		, "const remainder slice mismatch. result:%i, size:%u, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, result, output.size(), output.begin(), (data + 2), output.end(), (data + 5)
		);
	result = source.slice(output, 1, 2);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_COUNT, result != 2 || output.size() != 2 || output.begin() != data + 1 || output.end() != data + 3
		, "const counted slice mismatch. result:%i, size:%u, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, result, output.size(), output.begin(), (data + 1), output.end(), (data + 3)
		);
	result = source.slice(output, 5);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_END, result || output.size() || output.begin() != data + 5 || output.end() != data + 5
		, "const end slice mismatch. result:%i, size:%u, begin:%p, expected boundary:%p, end:%p."
		, result, output.size(), output.begin(), (data + 5), output.end()
		);

	::llc::view<cnst T> self{data};
	result = self.slice(self, 2, 2);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_SELF, result != 2 || self.size() != 2 || self.begin() != data + 2 || self.end() != data + 4
		, "const self-slice mismatch. result:%i, size:%u, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, result, self.size(), self.begin(), (data + 2), self.end(), (data + 4)
		);

	T preserved[2] = {};
	::llc::view<cnst T> failedOutput{preserved};
	result = sliceExpectedFailure(source, failedOutput, 6);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_INVALID_OFFSET, 0 <= result
		, "const slice accepted offset:6 for size:5. result:%i."
		, result
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_FAILURE_STATE, failedOutput.begin() != preserved || failedOutput.size() != ::llc::size(preserved)
		, "const invalid-offset slice changed output. begin:%p, expected:%p, size:%u, expected:%u."
		, failedOutput.begin(), preserved, failedOutput.size(), ::llc::u2_t(::llc::size(preserved))
		);
	result = sliceExpectedFailure(source, failedOutput, 3, 3);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_INVALID_COUNT, 0 <= result
		, "const slice accepted count:3 with only 2 elements remaining. result:%i."
		, result
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_FAILURE_STATE, failedOutput.begin() != preserved || failedOutput.size() != ::llc::size(preserved)
		, "const invalid-count slice changed output. begin:%p, expected:%p, size:%u, expected:%u."
		, failedOutput.begin(), preserved, failedOutput.size(), ::llc::u2_t(::llc::size(preserved))
		);

	cnst ::llc::view<T> empty;
	output = source;
	result = empty.slice(output, 0);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_EMPTY, result || output.size() || output.begin() || output.end()
		, "const empty slice mismatch. result:%i, size:%u, begin:%p, end:%p."
		, result, output.size(), output.begin(), output.end()
		);
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testSubscript(ATestError & errors) {
	T data[4] = {T(1), T(2), T(3), T(4)};
	::llc::view<T> mutableView{data};
	cnst ::llc::view<T> & constView = mutableView;
	for(::llc::u2_t iElement = 0; iElement < ::llc::size(data); ++iElement) {
		LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SUBSCRIPT_READ, mutableView[iElement] != data[iElement]
			, "mutable subscript mismatch. index:%u, value:%" LLC_FMT_S3 ", expected:%" LLC_FMT_S3 "."
			, iElement, ::llc::s3_t(mutableView[iElement]), ::llc::s3_t(data[iElement])
			);
		LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SUBSCRIPT_READ, constView[iElement] != data[iElement]
			, "const subscript mismatch. index:%u, value:%" LLC_FMT_S3 ", expected:%" LLC_FMT_S3 "."
			, iElement, ::llc::s3_t(constView[iElement]), ::llc::s3_t(data[iElement])
			);
	}
	mutableView[2] = T(9);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SUBSCRIPT_WRITE, data[2] != T(9) || mutableView[2] != T(9) || constView[2] != T(9)
		, "mutable subscript write mismatch. storage:%" LLC_FMT_S3 ", mutable:%" LLC_FMT_S3 ", const:%" LLC_FMT_S3 "."
		, ::llc::s3_t(data[2]), ::llc::s3_t(mutableView[2]), ::llc::s3_t(constView[2])
		);
#ifdef LLC_WINDOWS
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_INVALID_SUBSCRIPT, !testThrows([&]() { (void)mutableView[mutableView.size()]; })
		, "mutable view accepted index:%u at size:%u."
		, mutableView.size(), mutableView.size()
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_INVALID_SUBSCRIPT, !testThrows([&]() { (void)constView[constView.size()]; })
		, "const view accepted index:%u at size:%u."
		, constView.size(), constView.size()
		);

	::llc::view<T>		empty		; LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EMPTY_SUBSCRIPT, !testThrows([&]() { (void)empty		[0]; }), "mutable default empty view accepted index:0.");
	cnst ::llc::view<T> constEmpty	; LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EMPTY_SUBSCRIPT, !testThrows([&]() { (void)constEmpty [0]; }), "const default empty view accepted index:0.");
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

	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EQUALITY_IDENTITY, viewA != aliasA || !(viewA == aliasA)
		, "same-range views compared different. left:%p, right:%p, size:%u."
		, viewA.begin(), aliasA.begin(), viewA.size()
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EQUALITY_CONTENT, viewA != viewB || !(viewA == viewB)
		, "equal-content views compared different. left:%p, right:%p, size:%u."
		, viewA.begin(), viewB.begin(), viewA.size()
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EQUALITY_VALUE, viewA == viewDifferent || !(viewA != viewDifferent)
		, "different-content views compared equal. size:%u."
		, viewA.size()
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EQUALITY_SIZE, viewA == viewShort || !(viewA != viewShort)
		, "different-size views compared equal. left size:%u, right size:%u."
		, viewA.size(), viewShort.size()
		);
	::llc::view<T> emptyDefault;
	::llc::view<T> emptyAtData{0U, valuesA};
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EQUALITY_EMPTY, emptyDefault != emptyAtData || !(emptyDefault == emptyAtData)
		, "empty views compared different. left:%p, right:%p."
		, emptyDefault.begin(), emptyAtData.begin()
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_INEQUALITY_SYMMETRY
		, (viewA == viewB) == (viewA != viewB) || (viewA == viewDifferent) == (viewA != viewDifferent) || (viewA == viewShort) == (viewA != viewShort)
		, "equality and inequality operators disagreed."
		);
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testRepresentationViews(ATestError & errors) {
	T data[3] = {};
	::llc::view<T> mutableView{data};
	cnst ::llc::view<T> & constView = mutableView;
	::llc::u2_c expectedBytes = szof(data);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_BYTE_COUNT, mutableView.byte_count() != expectedBytes || constView.byte_count() != expectedBytes || ::llc::byte_count(mutableView) != expectedBytes
		, "byte count mismatch. mutable:%u, const:%u, free:%u, expected:%u."
		, mutableView.byte_count(), constView.byte_count(), ::llc::byte_count(mutableView), expectedBytes
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_BIT_COUNT, mutableView.bit_count() != expectedBytes * 8ULL || constView.bit_count() != expectedBytes * 8ULL
		, "bit count mismatch. mutable:%" LLC_FMT_U3 ", const:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "."
		, mutableView.bit_count(), constView.bit_count(), ::llc::u3_t(expectedBytes * 8ULL)
		);

	::llc::view<::llc::sc_t> chars = mutableView.c();
	::llc::view<::llc::u0_t> bytes = mutableView.u8();
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_MUTABLE_CHAR_VIEW, chars.begin() != (::llc::sc_t*)data || chars.size() != expectedBytes
		, "mutable character view mismatch. begin:%p, expected:%p, size:%u, expected:%u."
		, chars.begin(), data, chars.size(), expectedBytes
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_MUTABLE_BYTE_VIEW, bytes.begin() != (::llc::u0_t*)data || bytes.size() != expectedBytes
		, "mutable byte view mismatch. begin:%p, expected:%p, size:%u, expected:%u."
		, bytes.begin(), data, bytes.size(), expectedBytes
		);
	chars[0] = 0x2A;
	bytes[expectedBytes - 1] = 0x5A;
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_MUTABLE_REPRESENTATION, ((::llc::u0_t*)data)[0] != 0x2A || ((::llc::u0_t*)data)[expectedBytes - 1] != 0x5A
		, "mutable representation write mismatch. first:%u, last:%u."
		, ((::llc::u0_t*)data)[0], ((::llc::u0_t*)data)[expectedBytes - 1]
		);

	::llc::view<::llc::sc_c> constChars = constView.cc();
	::llc::view<::llc::u0_c> constBytes = constView.u8();
	::llc::view<::llc::u0_c> constByteAlias = constView.cu8();
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_CONST_CHAR_VIEW, constChars.begin() != (::llc::sc_c*)data || constChars.size() != expectedBytes || (::llc::u0_t)constChars[0] != 0x2A
		, "const character view mismatch. begin:%p, expected:%p, size:%u, expected:%u, first:%u."
		, constChars.begin(), data, constChars.size(), expectedBytes, (::llc::u0_t)constChars[0]
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_CONST_BYTE_VIEW, constBytes.begin() != (::llc::u0_c*)data || constBytes.size() != expectedBytes || constBytes[expectedBytes - 1] != 0x5A
		, "const byte view mismatch. begin:%p, expected:%p, size:%u, expected:%u, last:%u."
		, constBytes.begin(), data, constBytes.size(), expectedBytes, constBytes[expectedBytes - 1]
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_CONST_BYTE_ALIAS, constByteAlias.begin() != constBytes.begin() || constByteAlias.size() != constBytes.size()
		, "const byte aliases differ. u8 begin:%p, cu8 begin:%p, u8 size:%u, cu8 size:%u."
		, constBytes.begin(), constByteAlias.begin(), constBytes.size(), constByteAlias.size()
		);

	::llc::view<T> nullEmpty;
	cnst ::llc::view<T> & constNullEmpty = nullEmpty;
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EMPTY_REPRESENTATION
		, nullEmpty.c().size() || nullEmpty.c().begin() || nullEmpty.u8().size() || nullEmpty.u8().begin()
		|| constNullEmpty.cc().size() || constNullEmpty.cc().begin() || constNullEmpty.u8().size() || constNullEmpty.u8().begin() || constNullEmpty.cu8().size() || constNullEmpty.cu8().begin()
		, "null-empty representation mismatch."
		);
	::llc::view<T> boundaryEmpty{(T*)data, 0};
	cnst ::llc::view<T> & constBoundaryEmpty = boundaryEmpty;
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EMPTY_REPRESENTATION
		, boundaryEmpty.c().size() || boundaryEmpty.c().begin() != (::llc::sc_t*)data || boundaryEmpty.u8().size() || boundaryEmpty.u8().begin() != (::llc::u0_t*)data
		|| constBoundaryEmpty.cc().size() || constBoundaryEmpty.cc().begin() != (::llc::sc_c*)data || constBoundaryEmpty.u8().size() || constBoundaryEmpty.u8().begin() != (::llc::u0_c*)data || constBoundaryEmpty.cu8().size() || constBoundaryEmpty.cu8().begin() != (::llc::u0_c*)data
		, "boundary-empty representation mismatch. boundary:%p."
		, data
		);
	return 0;
}

tplt<tpnm T, ::llc::u2_t N>
sttc ::llc::err_t testMutationValues(ATestError & errors, VIEW_TEST_RESULT result, cnst T (&actual)[N], cnst T (&expected)[N]) {
	for(::llc::u2_t iElement = 0; iElement < N; ++iElement)
		LLC_TEST_CHECK(errors, result, actual[iElement] != expected[iElement]
			, "Mutation mismatch. index:%u, value:%" LLC_FMT_S3 ", expected:%" LLC_FMT_S3 "."
			, iElement, ::llc::s3_t(actual[iElement]), ::llc::s3_t(expected[iElement])
			);
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testFill(ATestError & errors) {
	T full[6] = {T(0), T(1), T(2), T(3), T(4), T(5)};
	T fullExpected[6] = {T(9), T(9), T(9), T(9), T(9), T(9)};
	::llc::err_t result = ::llc::view<T>{full}.fill(T(9));
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FILL_FULL, result != (::llc::err_t)::llc::size(full)
		, "full fill returned:%i, expected:%u."
		, result, ::llc::u2_t(::llc::size(full))
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FILL_FULL, full, fullExpected));

	T ranged[6] = {T(0), T(1), T(2), T(3), T(4), T(5)};
	T rangedExpected[6] = {T(0), T(8), T(8), T(8), T(4), T(5)};
	result = ::llc::view<T>{ranged}.fill(T(8), 1, 4);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FILL_RANGE, result != 3
		, "ranged fill returned:%i, expected:3."
		, result
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FILL_RANGE, ranged, rangedExpected));

	T clipped[6] = {T(0), T(1), T(2), T(3), T(4), T(5)};
	T clippedExpected[6] = {T(0), T(1), T(2), T(7), T(7), T(7)};
	result = ::llc::view<T>{clipped}.fill(T(7), 3, 20);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FILL_CLIPPED, result != 3
		, "clipped fill returned:%i, expected:3."
		, result
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FILL_CLIPPED, clipped, clippedExpected));

	T unchanged[6] = {T(0), T(1), T(2), T(3), T(4), T(5)};
	T unchangedExpected[6] = {T(0), T(1), T(2), T(3), T(4), T(5)};
	result = ::llc::view<T>{unchanged}.fill(T(6), 4, 2);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FILL_EMPTY_RANGE, result
		, "empty-range fill returned:%i, expected:0."
		, result
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FILL_EMPTY_RANGE, unchanged, unchangedExpected));

	::llc::view<T> nullEmpty;
	::llc::view<T> boundaryEmpty{(T*)unchanged, 0};
	cnst ::llc::err_t nullResult = nullEmpty.fill(T(1));
	cnst ::llc::err_t boundaryResult = boundaryEmpty.fill(T(1));
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FILL_EMPTY_VIEW, nullResult || boundaryResult
		, "empty fill returned a nonzero result. null:%i, boundary:%i."
		, nullResult, boundaryResult
		);
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testReverse(ATestError & errors) {
	T odd[5] = {T(0), T(1), T(2), T(3), T(4)};
	T oddExpected[5] = {T(4), T(3), T(2), T(1), T(0)};
	::llc::err_t result = ::llc::view<T>{odd}.revert();
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_REVERT_ODD, result
		, "odd revert returned:%i."
		, result
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_REVERT_ODD, odd, oddExpected));

	T even[6] = {T(9), T(1), T(2), T(3), T(4), T(8)};
	T evenExpected[6] = {T(9), T(4), T(3), T(2), T(1), T(8)};
	result = ::llc::view<T>{even + 1, 4}.revert();
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_REVERT_EVEN, result
		, "even subrange revert returned:%i."
		, result
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_REVERT_EVEN, even, evenExpected));

	T one[1] = {T(5)};
	T oneExpected[1] = {T(5)};
	::llc::view<T> nullEmpty;
	::llc::view<T> boundaryEmpty{one, 0U};
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_REVERT_EDGE, ::llc::view<T>{one}.revert() || nullEmpty.revert() || boundaryEmpty.revert()
		, "edge revert returned a failure."
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_REVERT_EDGE, one, oneExpected));

	T freeOdd[5] = {T(0), T(1), T(2), T(3), T(4)};
	result = ::llc::reverse(::llc::view<T>{freeOdd});
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_REVERSE_ODD, result
		, "odd reverse returned:%i."
		, result
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_REVERSE_ODD, freeOdd, oddExpected));

	T freeEven[6] = {T(9), T(1), T(2), T(3), T(4), T(8)};
	result = ::llc::reverse(::llc::view<T>{freeEven + 1, 4});
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_REVERSE_EVEN, result
		, "even subrange reverse returned:%i."
		, result
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_REVERSE_EVEN, freeEven, evenExpected));

	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_REVERSE_EDGE, ::llc::reverse(::llc::view<T>{one}) || ::llc::reverse(nullEmpty) || ::llc::reverse(boundaryEmpty)
		, "edge reverse returned a failure."
		);
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
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_FULL, result != 5 || visited != 5
		, "mutable full for_each mismatch. result:%i, visited:%u, expected:5."
		, result, visited
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_FULL, full, fullExpected));

	T offset[5] = {T(0), T(1), T(2), T(3), T(4)};
	T offsetExpected[5] = {T(0), T(1), T(12), T(13), T(14)};
	visited = 0;
	result = ::llc::view<T>{offset}.for_each(addTen, 2);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_OFFSET, result != 3 || visited != 3
		, "mutable offset for_each mismatch. result:%i, visited:%u, expected:3."
		, result, visited
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_OFFSET, offset, offsetExpected));

	T ranged[5] = {T(0), T(1), T(2), T(3), T(4)};
	T rangedExpected[5] = {T(0), T(11), T(12), T(13), T(4)};
	visited = 0;
	result = ::llc::view<T>{ranged}.for_each(addTen, 1, 4);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_RANGE, result != 3 || visited != 3
		, "mutable ranged for_each mismatch. result:%i, visited:%u, expected:3."
		, result, visited
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_RANGE, ranged, rangedExpected));

	T clipped[5] = {T(0), T(1), T(2), T(3), T(4)};
	T clippedExpected[5] = {T(0), T(1), T(2), T(13), T(14)};
	visited = 0;
	result = ::llc::view<T>{clipped}.for_each(addTen, 3, 20);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_CLIPPED, result != 2 || visited != 2
		, "mutable clipped for_each mismatch. result:%i, visited:%u, expected:2."
		, result, visited
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_CLIPPED, clipped, clippedExpected));

	T constData[5] = {T(0), T(1), T(2), T(3), T(4)};
	cnst ::llc::view<T> constView{constData};
	::llc::s3_t sum = 0;
	visited = 0;
	::llc::TFuncForEachConst<T> read = [&visited, &sum](cnst T & value) { ++visited; sum += value; return 0; };
	result = constView.for_each(read, 2);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FOREACH_CONST, result != 3 || visited != 3 || sum != 9
		, "const offset for_each mismatch. result:%i, visited:%u, sum:%" LLC_FMT_S3 ", expected result:3, visited:3, sum:9."
		, result, visited, sum
		);

	T unchanged[5] = {T(0), T(1), T(2), T(3), T(4)};
	T unchangedExpected[5] = {T(0), T(1), T(2), T(3), T(4)};
	visited = 0;
	result = ::llc::view<T>{unchanged}.for_each(addTen, 4, 2);
	::llc::view<T> nullEmpty;
	cnst ::llc::err_t emptyResult = nullEmpty.for_each(addTen);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FOREACH_EMPTY, result || emptyResult || visited
		, "empty for_each mismatch. reversed result:%i, empty result:%i, visited:%u."
		, result, emptyResult, visited
		);
	return testMutationValues(errors, VIEW_TEST_RESULT_FOREACH_EMPTY, unchanged, unchangedExpected);
}

tplt<tpnm T>
sttc ::llc::err_t testEnumerate(ATestError & errors) {
	T full[5] = {};
	T fullExpected[5] = {T(10), T(11), T(12), T(13), T(14)};
	::llc::u2_t visited = 0, indices = 0;
	::llc::TFuncEnumerate<T> writeIndex = [&visited, &indices](::llc::u2_t & index, T & value) { ++visited; indices |= 1U << index; value = T(10 + index); return 0; };
	::llc::err_t result = ::llc::view<T>{full}.enumerate(writeIndex);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_ENUMERATE_MUTABLE, result != 5 || visited != 5 || indices != 0x1F
		, "mutable full enumerate mismatch. result:%i, visited:%u, indices:0x%X."
		, result, visited, indices
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_ENUMERATE_MUTABLE, full, fullExpected));

	T ranged[5] = {};
	T rangedExpected[5] = {T(0), T(11), T(12), T(13), T(0)};
	visited = indices = 0;
	result = ::llc::view<T>{ranged}.enumerate(writeIndex, 1, 4);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_ENUMERATE_RANGE, result != 3 || visited != 3 || indices != 0x0E
		, "mutable ranged enumerate mismatch. result:%i, visited:%u, indices:0x%X, expected result:3, visited:3, indices:0x0E."
		, result, visited, indices
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_ENUMERATE_RANGE, ranged, rangedExpected));

	T constData[5] = {T(0), T(1), T(2), T(3), T(4)};
	cnst ::llc::view<T> constView{constData};
	::llc::s3_t sum = 0;
	visited = indices = 0;
	::llc::TFuncEnumerateConst<T> readIndex = [&visited, &indices, &sum](::llc::u2_t & index, cnst T & value) { ++visited; indices |= 1U << index; sum += value; return 0; };
	result = constView.enumerate(readIndex, 2);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_ENUMERATE_CONST, result != 3 || visited != 3 || indices != 0x1C || sum != 9
		, "const offset enumerate mismatch. result:%i, visited:%u, indices:0x%X, sum:%" LLC_FMT_S3 "."
		, result, visited, indices, sum
		);

	visited = indices = 0;
	result = ::llc::view<T>{ranged}.enumerate(writeIndex, 4, 2);
	::llc::view<T> nullEmpty;
	cnst ::llc::err_t emptyResult = nullEmpty.enumerate(writeIndex);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_ENUMERATE_EMPTY, result || emptyResult || visited || indices
		, "empty enumerate mismatch. reversed result:%i, empty result:%i, visited:%u, indices:0x%X."
		, result, emptyResult, visited, indices
		);
	return 0;
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

	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FOREACH_FAILURE, !::llc::failed(mutableForEachResult) || mutableForEachVisits != 3
		, "mutable for_each failure mismatch. result:%i, visited:%u, expected visited:3."
		, mutableForEachResult, mutableForEachVisits
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FOREACH_FAILURE, !::llc::failed(constForEachResult) || constForEachVisits != 3
		, "const for_each failure mismatch. result:%i, visited:%u, expected visited:3."
		, constForEachResult, constForEachVisits
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FOREACH_FAILURE, !::llc::failed(mutableRangeResult) || mutableRangeVisits != 2
		, "mutable ranged for_each failure mismatch. result:%i, visited:%u, expected visited:2."
		, mutableRangeResult, mutableRangeVisits
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FOREACH_FAILURE, !::llc::failed(constRangeResult) || constRangeVisits != 2
		, "const ranged for_each failure mismatch. result:%i, visited:%u, expected visited:2."
		, constRangeResult, constRangeVisits
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_ENUMERATE_FAILURE, !::llc::failed(mutableEnumerateResult) || mutableEnumerateVisits != 3
		, "mutable enumerate failure mismatch. result:%i, visited:%u, expected visited:3."
		, mutableEnumerateResult, mutableEnumerateVisits
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_ENUMERATE_FAILURE, !::llc::failed(constEnumerateResult) || constEnumerateVisits != 3
		, "const enumerate failure mismatch. result:%i, visited:%u, expected visited:3."
		, constEnumerateResult, constEnumerateVisits
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_ENUMERATE_FAILURE, !::llc::failed(mutableEnumerateRangeResult) || mutableEnumerateRangeVisits != 2
		, "mutable ranged enumerate failure mismatch. result:%i, visited:%u, expected visited:2."
		, mutableEnumerateRangeResult, mutableEnumerateRangeVisits
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_ENUMERATE_FAILURE, !::llc::failed(constEnumerateRangeResult) || constEnumerateRangeVisits != 2
		, "const ranged enumerate failure mismatch. result:%i, visited:%u, expected visited:2."
		, constEnumerateRangeResult, constEnumerateRangeVisits
		);
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testFind(ATestError & errors) {
	T mutableData[5] = {T(0), T(1), T(2), T(3), T(2)};
	T mutableExpected[5] = {T(10), T(11), T(12), T(3), T(2)};
	::llc::u2_t visited = 0;
	::llc::FBool<T&> mutablePredicate = [&visited](T & value) { ++visited; cnst bool match = value == T(2); value += T(10); return match; };
	::llc::err_t result = ::llc::view<T>{mutableData}.find(mutablePredicate);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FIND_MUTABLE, result != 2 || visited != 3
		, "mutable predicate find mismatch. result:%i, visited:%u, expected result:2, visited:3."
		, result, visited
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FIND_MUTABLE, mutableData, mutableExpected));

	T constData[5] = {T(0), T(1), T(2), T(3), T(2)};
	cnst ::llc::view<T> constView{constData};
	visited = 0;
	::llc::FBool<cnst T&> constPredicate = [&visited](cnst T & value) { ++visited; return value == T(2); };
	result = constView.find(constPredicate);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FIND_CONST, result != 2 || visited != 3
		, "const predicate find mismatch. result:%i, visited:%u, expected result:2, visited:3."
		, result, visited
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FIND_VALUE, constView.find(T(2)) != 2
		, "value find did not return index:2. result:%i."
		, constView.find(T(2))
		);

	visited = 0;
	result = constView.find(constPredicate, 3);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FIND_OFFSET, result != 4 || visited != 2 || constView.find(T(2), 3) != 4
		, "offset find mismatch. predicate result:%i, visited:%u, value result:%i, expected result:4, visited:2."
		, result, visited, constView.find(T(2), 3)
		);

	visited = 0;
	::llc::FBool<cnst T&> missingPredicate = [&visited](cnst T & value) { ++visited; return value == T(9); };
	result = constView.find(missingPredicate);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FIND_NOT_FOUND, result != -1 || visited != constView.size() || constView.find(T(9)) != -1
		, "missing find mismatch. predicate result:%i, visited:%u, value result:%i."
		, result, visited, constView.find(T(9))
		);

	visited = 0;
	::llc::view<T> empty;
	cnst ::llc::view<T> & constEmpty = empty;
	cnst ::llc::err_t emptyPredicateResult = constEmpty.find(missingPredicate);
	cnst ::llc::err_t emptyValueResult = constEmpty.find(T(0));
	cnst ::llc::err_t pastEndResult = constView.find(missingPredicate, constView.size());
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FIND_EMPTY, emptyPredicateResult != -1 || emptyValueResult != -1 || pastEndResult != -1 || visited
		, "empty find mismatch. predicate:%i, value:%i, past-end:%i, visited:%u."
		, emptyPredicateResult, emptyValueResult, pastEndResult, visited
		);
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
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_MAXIMUM_OUTPUT, iMaximum != 1 || maximum != 7
		, "maximum mismatch. index:%i, value:%" LLC_FMT_S3 ", expected index:1, value:7."
		, iMaximum, maximum
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_MINIMUM_OUTPUT, iMinimum != 2 || minimum != 1
		, "minimum mismatch. index:%i, value:%" LLC_FMT_S3 ", expected index:2, value:1."
		, iMinimum, minimum
		);

	maximum = 999;
	minimum = -999;
	iMaximum = values.max(maximum, transform, 3);
	iMinimum = values.min(minimum, transform, 3);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EXTREMA_OFFSET, iMaximum != 3 || maximum != 7 || iMinimum != 4 || minimum != 2
		, "offset extrema mismatch. max index:%i, max:%" LLC_FMT_S3 ", min index:%i, min:%" LLC_FMT_S3 "."
		, iMaximum, maximum, iMinimum, minimum
		);

	iMaximum = values.max(transform);
	iMinimum = values.min(transform);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EXTREMA_CONVENIENCE, iMaximum != 1 || iMinimum != 2 || values.max(transform, 3) != 3 || values.min(transform, 3) != 4
		, "convenience extrema mismatch. max:%i, min:%i, offset max:%i, offset min:%i."
		, iMaximum, iMinimum, values.max(transform, 3), values.min(transform, 3)
		);

	T singleData[1] = {T(5)};
	cnst ::llc::view<T> single{singleData};
	maximum = 999;
	minimum = -999;
	iMaximum = single.max(maximum, transform);
	iMinimum = single.min(minimum, transform);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EXTREMA_SINGLE, iMaximum || maximum != 5 || iMinimum || minimum != 5 || single.max(transform) || single.min(transform)
		, "single extrema mismatch. max index:%i, max:%" LLC_FMT_S3 ", min index:%i, min:%" LLC_FMT_S3 "."
		, iMaximum, maximum, iMinimum, minimum
		);

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
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EXTREMA_EMPTY, iMaximum != -1 || iMinimum != -1 || maximum != 123 || minimum != 456 || transformed || convenienceMaximum != -1 || convenienceMinimum != -1
		, "empty extrema mismatch. max index:%i, min index:%i, max:%" LLC_FMT_S3 ", min:%" LLC_FMT_S3 ", transformed:%u."
		, iMaximum, iMinimum, maximum, minimum, transformed
		);
	return 0;
}

tplt<tpnm TView, tpnm TPointer>
sttc bool splitRangeIs(cnst TView & range, TPointer rangeBegin, ::llc::u2_c count) {
	rtrn range.size() == count && range.begin() == rangeBegin && range.end() == rangeBegin + count;
}

tplt<tpnm T>
sttc ::llc::err_t testSplit(ATestError & errors) {
	T values[5] = {T(1), T(2), T(3), T(2), T(4)};
	::llc::view<T> source{values};
	::llc::view<T> left = source, right;
	::llc::err_t result = ::llc::split(T(2), left);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_VALUE, result != 1 || !splitRangeIs(left, values, 1)
		, "in-place scalar split mismatch. result:%i, left size:%u, expected result/size:1."
		, result, left.size()
		);

	left = source;
	result = ::llc::split(T(9), left);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_VALUE_MISSING, result != 5 || !splitRangeIs(left, values, 5)
		, "missing in-place scalar split mismatch. result:%i, left size:%u, expected result/size:5."
		, result, left.size()
		);

	result = ::llc::split(T(2), source, left, right);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_VALUE, result != 1 || !splitRangeIs(left, values, 1) || !splitRangeIs(right, values + 2, 3)
		, "scalar split mismatch. result:%i, left size:%u, right size:%u, expected result/left/right:1/1/3."
		, result, left.size(), right.size()
		);

	result = ::llc::split(T(1), source, left, right);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_VALUE_BOUNDARY, result || !splitRangeIs(left, values, 0) || !splitRangeIs(right, values + 1, 4)
		, "leading scalar split mismatch. result:%i, left size:%u, right size:%u, expected result/left/right:0/0/4."
		, result, left.size(), right.size()
		);
	result = ::llc::split(T(4), source, left, right);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_VALUE_BOUNDARY, result != 4 || !splitRangeIs(left, values, 4) || !splitRangeIs(right, values + 5, 0)
		, "trailing scalar split mismatch. result:%i, left size:%u, right size:%u, expected result/left/right:4/4/0."
		, result, left.size(), right.size()
		);

	result = ::llc::split(T(9), source, left, right);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_VALUE_MISSING, result != -1 || !splitRangeIs(left, values, 5) || right.size()
		, "missing scalar split mismatch. result:%i, left size:%u, right size:%u, expected result/left/right:-1/5/0."
		, result, left.size(), right.size()
		);

	result = ::llc::splitAt(T(2), source, left, right);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_AT_VALUE, result != 1 || !splitRangeIs(left, values, 1) || !splitRangeIs(right, values + 1, 4)
		, "scalar splitAt mismatch. result:%i, left size:%u, right size:%u, expected result/left/right:1/1/4."
		, result, left.size(), right.size()
		);

	::llc::view<cnst T> constLeft, constRight;
	result = ::llc::split(T(2), source, constLeft, constRight);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_VALUE_CONST_OUTPUT, result != 1 || !splitRangeIs(constLeft, values, 1) || !splitRangeIs(constRight, values + 2, 3)
		, "const-output scalar split mismatch. result:%i, left size:%u, right size:%u, expected result/left/right:1/1/3."
		, result, constLeft.size(), constRight.size()
		);
	result = ::llc::splitAt(T(2), source, constLeft, constRight);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_AT_VALUE_CONST_OUTPUT, result != 1 || !splitRangeIs(constLeft, values, 1) || !splitRangeIs(constRight, values + 1, 4)
		, "const-output scalar splitAt mismatch. result:%i, left size:%u, right size:%u, expected result/left/right:1/1/4."
		, result, constLeft.size(), constRight.size()
		);

	cnst ::llc::view<cnst T> constSource{values};
	result = ::llc::split(T(2), constSource, constLeft, constRight);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_VALUE, result != 1 || !splitRangeIs(constLeft, values, 1) || !splitRangeIs(constRight, values + 2, 3)
		, "const scalar split mismatch. result:%i, left size:%u, right size:%u, expected result/left/right:1/1/3."
		, result, constLeft.size(), constRight.size()
		);

	T sequenceValues[2] = {T(2), T(3)};
	cnst ::llc::view<T> sequence{sequenceValues};
	result = ::llc::split(sequence, source, left, right);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_SEQUENCE, result != 1 || !splitRangeIs(left, values, 1) || !splitRangeIs(right, values + 3, 2)
		, "sequence split mismatch. result:%i, left size:%u, right size:%u, expected result/left/right:1/1/2."
		, result, left.size(), right.size()
		);

	T missingValues[2] = {T(8), T(9)};
	cnst ::llc::view<T> missingSequence{missingValues};
	result = ::llc::split(missingSequence, source, left, right);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_SEQUENCE_MISSING, result != -1 || !splitRangeIs(left, values, 5) || right.size()
		, "missing sequence split mismatch. result:%i, left size:%u, right size:%u, expected result/left/right:-1/5/0."
		, result, left.size(), right.size()
		);

	result = ::llc::splitAt(sequence, source, left, right);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_AT_SEQUENCE, result != 1 || !splitRangeIs(left, values, 1) || !splitRangeIs(right, values + 1, 4)
		, "sequence splitAt mismatch. result:%i, left size:%u, right size:%u, expected result/left/right:1/1/4."
		, result, left.size(), right.size()
		);
	result = ::llc::split(sequence, source, constLeft, constRight);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_SEQUENCE_CONST_OUTPUT, result != 1 || !splitRangeIs(constLeft, values, 1) || !splitRangeIs(constRight, values + 3, 2)
		, "const-output sequence split mismatch. result:%i, left size:%u, right size:%u, expected result/left/right:1/1/2."
		, result, constLeft.size(), constRight.size()
		);
	result = ::llc::splitAt(sequence, source, constLeft, constRight);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_AT_SEQUENCE_CONST_OUTPUT, result != 1 || !splitRangeIs(constLeft, values, 1) || !splitRangeIs(constRight, values + 1, 4)
		, "const-output sequence splitAt mismatch. result:%i, left size:%u, right size:%u, expected result/left/right:1/1/4."
		, result, constLeft.size(), constRight.size()
		);

	left = source;
	result = ::llc::split(sequence, left, right);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_SEQUENCE_IN_PLACE, result != 1 || !splitRangeIs(left, values, 1) || !splitRangeIs(right, values + 3, 2)
		, "in-place sequence split mismatch. result:%i, left size:%u, right size:%u, expected result/left/right:1/1/2."
		, result, left.size(), right.size()
		);
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testSplitCollections(ATestError & errors) {
	{
		T values[3] = {T(1), T(0), T(2)};
		::llc::aobj<::llc::view<cnst T>> output;
		cnst ::llc::err_t result = ::llc::split(::llc::view<cnst T>{values}, T(0), output);
		LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_COLLECTION, result != 2 || output.size() != 2 || !splitRangeIs(output[0], values, 1) || !splitRangeIs(output[1], values + 2, 1)
			, "ordinary collection mismatch. result:%i, output size:%u, expected:2."
			, result, output.size()
			);
	}
	{
		T values[4] = {T(1), T(0), T(0), T(2)};
		::llc::aobj<::llc::view<cnst T>> output;
		cnst ::llc::err_t result = ::llc::split(::llc::view<cnst T>{values}, T(0), output);
		LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_COLLECTION, result != 2 || output.size() != 2 || !splitRangeIs(output[0], values, 1) || !splitRangeIs(output[1], values + 3, 1)
			, "consecutive delimiter collection mismatch. result:%i, output size:%u, expected:2."
			, result, output.size()
			);
	}
	{
		T values[3] = {T(0), T(1), T(0)};
		::llc::aobj<::llc::view<cnst T>> output;
		cnst ::llc::err_t result = ::llc::split(::llc::view<cnst T>{values}, T(0), output);
		LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_COLLECTION, result != 1 || output.size() != 1 || !splitRangeIs(output[0], values + 1, 1)
			, "boundary delimiter collection mismatch. result:%i, output size:%u, expected:1."
			, result, output.size()
			);
	}
	{
		T values[2] = {T(0), T(0)};
		::llc::aobj<::llc::view<cnst T>> output;
		cnst ::llc::err_t result = ::llc::split(::llc::view<cnst T>{values}, T(0), output);
		LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_COLLECTION, result || output.size()
			, "delimiter-only collection mismatch. result:%i, output size:%u, expected:0."
			, result, output.size()
			);
	}
	{
		T values[1] = {T(1)};
		::llc::aobj<::llc::view<cnst T>> output;
		cnst ::llc::err_t result = ::llc::split(::llc::view<cnst T>{values}, T(0), output);
		LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_COLLECTION, result != 1 || output.size() != 1 || !splitRangeIs(output[0], values, 1)
			, "delimiter-free collection mismatch. result:%i, output size:%u, expected:1."
			, result, output.size()
			);
	}
	{
		::llc::aobj<::llc::view<cnst T>> output;
		cnst ::llc::err_t result = ::llc::split(::llc::view<cnst T>{}, T(0), output);
		LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_COLLECTION, result || output.size()
			, "empty collection mismatch. result:%i, output size:%u, expected:0."
			, result, output.size()
			);
	}
	{
		T seed[1] = {T(9)};
		T values[4] = {T(1), T(0), T(0), T(2)};
		::llc::aobj<::llc::view<cnst T>> output;
		if_fail_fe(output.push_back({seed}));
		cnst ::llc::err_t result = ::llc::split(::llc::view<cnst T>{values}, T(0), output);
		LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_COLLECTION_APPEND, result != 3 || output.size() != 3 || !splitRangeIs(output[0], seed, 1) || !splitRangeIs(output[1], values, 1) || !splitRangeIs(output[2], values + 3, 1)
			, "append collection mismatch. result:%i, output size:%u, expected:3."
			, result, output.size()
			);
	}
	{
		T values[6] = {T(0), T(1), T(3), T(0), T(2), T(3)};
		T separators[2] = {T(0), T(3)};
		::llc::aobj<::llc::view<cnst T>> output;
		cnst ::llc::err_t result = ::llc::split(::llc::view<cnst T>{values}, ::llc::view<cnst T>{separators}, output);
		LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_COLLECTION_SET, result != 2 || output.size() != 2 || !splitRangeIs(output[0], values + 1, 1) || !splitRangeIs(output[1], values + 4, 1)
			, "delimiter-set collection mismatch. result:%i, output size:%u, expected:2."
			, result, output.size()
			);
	}
	rtrn 0;
}

sttc ::llc::err_t testStringSplitCollection(ATestError & errors) {
	::llc::aobj<::llc::vcst_t> output;
	cnst ::llc::err_t result = ::llc::split(LLC_CXS(",a,,b,"), ',', output);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_STRING_COLLECTION, result != 2 || output.size() != 2 || output[0] != LLC_CXS("a") || output[1] != LLC_CXS("b")
		, "string collection mismatch. result:%i, output size:%u, expected:2."
		, result, output.size()
		);

	::llc::aobj<::llc::vcst_t> heterogeneousOutput;
	cnst ::llc::err_t heterogeneousResult = ::llc::split(LLC_CXS("a,b"), ::llc::u1_t(300), heterogeneousOutput);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SPLIT_HETEROGENEOUS, heterogeneousResult != 1 || heterogeneousOutput.size() != 1 || heterogeneousOutput[0] != LLC_CXS("a,b")
		, "heterogeneous separator mismatch. result:%i, output size:%u, expected:1."
		, heterogeneousResult, heterogeneousOutput.size()
		);
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testInvalidConstruction(ATestError & errors) {
#ifdef LLC_WINDOWS
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_INVALID_CONSTRUCTION, !testThrows([&]() { cnst ::llc::view<T> invalid{(T*)0, 1}; (void)invalid; })
		, "view accepted null storage with one element."
		);
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
