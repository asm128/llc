#include "llc_view.h"

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
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FOREACH_MUTABLE_RANGE		, 50, "Mutable for_each() did not honor its explicit or clipped range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FOREACH_CONST				, 51, "Const for_each() did not visit the expected elements without mutation.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FOREACH_EMPTY				, 52, "for_each() invoked its callback or reported work for an empty range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, ENUMERATE_MUTABLE			, 53, "Mutable enumerate() did not provide the correct indices and elements.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, ENUMERATE_RANGE			, 54, "enumerate() did not honor its requested range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, ENUMERATE_CONST			, 55, "Const enumerate() did not provide the correct indices and elements.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, ENUMERATE_EMPTY			, 56, "enumerate() invoked its callback or reported work for an empty range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, FOREACH_FAILURE			, 57, "for_each() did not stop and propagate a callback failure.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, ENUMERATE_FAILURE			, 58, "enumerate() did not stop and propagate a callback failure.");

tplt<tpnm T>
sttc ::llc::err_t testRepresentation(ATestError & errors) {
	T data[5] = {};
	::llc::view<T> empty;
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_DEFAULT_STATE, empty.size() || empty.begin() || empty.end()
		, "%s default range mismatch. size:%u, begin:%p, end:%p."
		, ::llc::get_type_namep<T>(), empty.size(), (void*)empty.begin(), (void*)empty.end()
		);

	::llc::view<T> full{data};
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_ARRAY_CONSTRUCTION, full.size() != ::llc::size(data) || full.begin() != data || full.end() != data + ::llc::size(data)
		, "%s array range mismatch. size:%u, expected:%u, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, ::llc::get_type_namep<T>(), full.size(), ::llc::u2_t(::llc::size(data)), (void*)full.begin(), (void*)data, (void*)full.end(), (void*)(data + ::llc::size(data))
		);

	::llc::view<T> partialCountFirst{3U, data};
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_COUNT_FIRST_CONSTRUCTION, partialCountFirst.size() != 3 || partialCountFirst.begin() != data || partialCountFirst.end() != data + 3
		, "%s count-first range mismatch. size:%u, expected:3, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, ::llc::get_type_namep<T>(), partialCountFirst.size(), (void*)partialCountFirst.begin(), (void*)data, (void*)partialCountFirst.end(), (void*)(data + 3)
		);
	::llc::view<T> clippedCountFirst{::llc::u2_t(::llc::size(data) + 1), data};
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_COUNT_FIRST_CONSTRUCTION, clippedCountFirst.size() != ::llc::size(data) || clippedCountFirst.begin() != data || clippedCountFirst.end() != data + ::llc::size(data)
		, "%s clipped count-first range mismatch. size:%u, expected:%u, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, ::llc::get_type_namep<T>(), clippedCountFirst.size(), ::llc::u2_t(::llc::size(data)), (void*)clippedCountFirst.begin(), (void*)data, (void*)clippedCountFirst.end(), (void*)(data + ::llc::size(data))
		);

	::llc::view<T> partialArrayFirst{data, 3U};
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_ARRAY_FIRST_CONSTRUCTION, partialArrayFirst.size() != 3 || partialArrayFirst.begin() != data || partialArrayFirst.end() != data + 3
		, "%s array-first range mismatch. size:%u, expected:3, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, ::llc::get_type_namep<T>(), partialArrayFirst.size(), (void*)partialArrayFirst.begin(), (void*)data, (void*)partialArrayFirst.end(), (void*)(data + 3)
		);

	::llc::view<T> pointerRange{data + 1, 3};
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_POINTER_CONSTRUCTION, pointerRange.size() != 3 || pointerRange.begin() != data + 1 || pointerRange.end() != data + 4
		, "%s pointer range mismatch. size:%u, expected:3, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, ::llc::get_type_namep<T>(), pointerRange.size(), (void*)pointerRange.begin(), (void*)(data + 1), (void*)pointerRange.end(), (void*)(data + 4)
		);

	::llc::view<T> emptyAtData{data, 0};
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EMPTY_RANGE, emptyAtData.size() || emptyAtData.begin() != data || emptyAtData.end() != data
		, "%s zero-length range mismatch. size:%u, begin:%p, expected boundary:%p, end:%p."
		, ::llc::get_type_namep<T>(), emptyAtData.size(), (void*)emptyAtData.begin(), (void*)data, (void*)emptyAtData.end()
		);
	::llc::view<T> emptyAtNull{(T*)0, 0};
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EMPTY_RANGE, emptyAtNull.size() || emptyAtNull.begin() || emptyAtNull.end()
		, "%s null zero-length range mismatch. size:%u, begin:%p, end:%p."
		, ::llc::get_type_namep<T>(), emptyAtNull.size(), (void*)emptyAtNull.begin(), (void*)emptyAtNull.end()
		);

	::llc::view<cnst T> readOnly = pointerRange;
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_CONST_CONVERSION, readOnly.size() != pointerRange.size() || readOnly.begin() != pointerRange.begin() || readOnly.end() != pointerRange.end()
		, "%s const conversion mismatch. size:%u, expected:%u, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, ::llc::get_type_namep<T>(), readOnly.size(), pointerRange.size(), (cnst void*)readOnly.begin(), (cnst void*)pointerRange.begin(), (cnst void*)readOnly.end(), (cnst void*)pointerRange.end()
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
		, "%s mutable full slice mismatch. result:%i, size:%u, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, ::llc::get_type_namep<T>(), result, output.size(), (void*)output.begin(), (void*)data, (void*)output.end(), (void*)(data + 5)
		);
	result = source.slice(output, 2);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_REMAINDER, result != 3 || output.size() != 3 || output.begin() != data + 2 || output.end() != data + 5
		, "%s mutable remainder slice mismatch. result:%i, size:%u, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, ::llc::get_type_namep<T>(), result, output.size(), (void*)output.begin(), (void*)(data + 2), (void*)output.end(), (void*)(data + 5)
		);
	result = source.slice(output, 1, 2);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_COUNT, result != 2 || output.size() != 2 || output.begin() != data + 1 || output.end() != data + 3
		, "%s mutable counted slice mismatch. result:%i, size:%u, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, ::llc::get_type_namep<T>(), result, output.size(), (void*)output.begin(), (void*)(data + 1), (void*)output.end(), (void*)(data + 3)
		);
	result = source.slice(output, 5);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_END, result || output.size() || output.begin() != data + 5 || output.end() != data + 5
		, "%s mutable end slice mismatch. result:%i, size:%u, begin:%p, expected boundary:%p, end:%p."
		, ::llc::get_type_namep<T>(), result, output.size(), (void*)output.begin(), (void*)(data + 5), (void*)output.end()
		);

	::llc::view<T> self{data};
	result = self.slice(self, 2, 2);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_SELF, result != 2 || self.size() != 2 || self.begin() != data + 2 || self.end() != data + 4
		, "%s mutable self-slice mismatch. result:%i, size:%u, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, ::llc::get_type_namep<T>(), result, self.size(), (void*)self.begin(), (void*)(data + 2), (void*)self.end(), (void*)(data + 4)
		);

	T preserved[2] = {};
	::llc::view<T> failedOutput{preserved};
	result = sliceExpectedFailure(source, failedOutput, 6);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_INVALID_OFFSET, 0 <= result
		, "%s mutable slice accepted offset:6 for size:5. result:%i."
		, ::llc::get_type_namep<T>(), result
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_FAILURE_STATE, failedOutput.begin() != preserved || failedOutput.size() != ::llc::size(preserved)
		, "%s mutable invalid-offset slice changed output. begin:%p, expected:%p, size:%u, expected:%u."
		, ::llc::get_type_namep<T>(), (void*)failedOutput.begin(), (void*)preserved, failedOutput.size(), ::llc::u2_t(::llc::size(preserved))
		);
	result = sliceExpectedFailure(source, failedOutput, 3, 3);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_INVALID_COUNT, 0 <= result
		, "%s mutable slice accepted count:3 with only 2 elements remaining. result:%i."
		, ::llc::get_type_namep<T>(), result
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_FAILURE_STATE, failedOutput.begin() != preserved || failedOutput.size() != ::llc::size(preserved)
		, "%s mutable invalid-count slice changed output. begin:%p, expected:%p, size:%u, expected:%u."
		, ::llc::get_type_namep<T>(), (void*)failedOutput.begin(), (void*)preserved, failedOutput.size(), ::llc::u2_t(::llc::size(preserved))
		);

	::llc::view<T> empty;
	output = source;
	result = empty.slice(output, 0);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_EMPTY, result || output.size() || output.begin() || output.end()
		, "%s mutable empty slice mismatch. result:%i, size:%u, begin:%p, end:%p."
		, ::llc::get_type_namep<T>(), result, output.size(), (void*)output.begin(), (void*)output.end()
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
		, "%s const full slice mismatch. result:%i, size:%u, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, ::llc::get_type_namep<T>(), result, output.size(), (cnst void*)output.begin(), (cnst void*)data, (cnst void*)output.end(), (cnst void*)(data + 5)
		);
	result = source.slice(output, 2);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_REMAINDER, result != 3 || output.size() != 3 || output.begin() != data + 2 || output.end() != data + 5
		, "%s const remainder slice mismatch. result:%i, size:%u, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, ::llc::get_type_namep<T>(), result, output.size(), (cnst void*)output.begin(), (cnst void*)(data + 2), (cnst void*)output.end(), (cnst void*)(data + 5)
		);
	result = source.slice(output, 1, 2);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_COUNT, result != 2 || output.size() != 2 || output.begin() != data + 1 || output.end() != data + 3
		, "%s const counted slice mismatch. result:%i, size:%u, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, ::llc::get_type_namep<T>(), result, output.size(), (cnst void*)output.begin(), (cnst void*)(data + 1), (cnst void*)output.end(), (cnst void*)(data + 3)
		);
	result = source.slice(output, 5);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_END, result || output.size() || output.begin() != data + 5 || output.end() != data + 5
		, "%s const end slice mismatch. result:%i, size:%u, begin:%p, expected boundary:%p, end:%p."
		, ::llc::get_type_namep<T>(), result, output.size(), (cnst void*)output.begin(), (cnst void*)(data + 5), (cnst void*)output.end()
		);

	::llc::view<cnst T> self{data};
	result = self.slice(self, 2, 2);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_SELF, result != 2 || self.size() != 2 || self.begin() != data + 2 || self.end() != data + 4
		, "%s const self-slice mismatch. result:%i, size:%u, begin:%p, expected begin:%p, end:%p, expected end:%p."
		, ::llc::get_type_namep<T>(), result, self.size(), (cnst void*)self.begin(), (cnst void*)(data + 2), (cnst void*)self.end(), (cnst void*)(data + 4)
		);

	T preserved[2] = {};
	::llc::view<cnst T> failedOutput{preserved};
	result = sliceExpectedFailure(source, failedOutput, 6);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_INVALID_OFFSET, 0 <= result
		, "%s const slice accepted offset:6 for size:5. result:%i."
		, ::llc::get_type_namep<T>(), result
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_FAILURE_STATE, failedOutput.begin() != preserved || failedOutput.size() != ::llc::size(preserved)
		, "%s const invalid-offset slice changed output. begin:%p, expected:%p, size:%u, expected:%u."
		, ::llc::get_type_namep<T>(), (cnst void*)failedOutput.begin(), (cnst void*)preserved, failedOutput.size(), ::llc::u2_t(::llc::size(preserved))
		);
	result = sliceExpectedFailure(source, failedOutput, 3, 3);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_INVALID_COUNT, 0 <= result
		, "%s const slice accepted count:3 with only 2 elements remaining. result:%i."
		, ::llc::get_type_namep<T>(), result
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_FAILURE_STATE, failedOutput.begin() != preserved || failedOutput.size() != ::llc::size(preserved)
		, "%s const invalid-count slice changed output. begin:%p, expected:%p, size:%u, expected:%u."
		, ::llc::get_type_namep<T>(), (cnst void*)failedOutput.begin(), (cnst void*)preserved, failedOutput.size(), ::llc::u2_t(::llc::size(preserved))
		);

	cnst ::llc::view<T> empty;
	output = source;
	result = empty.slice(output, 0);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SLICE_EMPTY, result || output.size() || output.begin() || output.end()
		, "%s const empty slice mismatch. result:%i, size:%u, begin:%p, end:%p."
		, ::llc::get_type_namep<T>(), result, output.size(), (cnst void*)output.begin(), (cnst void*)output.end()
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
			, "%s mutable subscript mismatch. index:%u, value:%" LLC_FMT_S3 ", expected:%" LLC_FMT_S3 "."
			, ::llc::get_type_namep<T>(), iElement, ::llc::s3_t(mutableView[iElement]), ::llc::s3_t(data[iElement])
			);
		LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SUBSCRIPT_READ, constView[iElement] != data[iElement]
			, "%s const subscript mismatch. index:%u, value:%" LLC_FMT_S3 ", expected:%" LLC_FMT_S3 "."
			, ::llc::get_type_namep<T>(), iElement, ::llc::s3_t(constView[iElement]), ::llc::s3_t(data[iElement])
			);
	}
	mutableView[2] = T(9);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_SUBSCRIPT_WRITE, data[2] != T(9) || mutableView[2] != T(9) || constView[2] != T(9)
		, "%s mutable subscript write mismatch. storage:%" LLC_FMT_S3 ", mutable:%" LLC_FMT_S3 ", const:%" LLC_FMT_S3 "."
		, ::llc::get_type_namep<T>(), ::llc::s3_t(data[2]), ::llc::s3_t(mutableView[2]), ::llc::s3_t(constView[2])
		);
#ifdef LLC_WINDOWS
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_INVALID_SUBSCRIPT, !testThrows([&]() { (void)mutableView[mutableView.size()]; })
		, "%s mutable view accepted index:%u at size:%u."
		, ::llc::get_type_namep<T>(), mutableView.size(), mutableView.size()
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_INVALID_SUBSCRIPT, !testThrows([&]() { (void)constView[constView.size()]; })
		, "%s const view accepted index:%u at size:%u."
		, ::llc::get_type_namep<T>(), constView.size(), constView.size()
		);
	::llc::view<T> empty;
	cnst ::llc::view<T> constEmpty;
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EMPTY_SUBSCRIPT, !testThrows([&]() { (void)empty[0]; })
		, "%s mutable default empty view accepted index:0."
		, ::llc::get_type_namep<T>()
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EMPTY_SUBSCRIPT, !testThrows([&]() { (void)constEmpty[0]; })
		, "%s const default empty view accepted index:0."
		, ::llc::get_type_namep<T>()
		);
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
		, "%s same-range views compared different. left:%p, right:%p, size:%u."
		, ::llc::get_type_namep<T>(), (void*)viewA.begin(), (void*)aliasA.begin(), viewA.size()
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EQUALITY_CONTENT, viewA != viewB || !(viewA == viewB)
		, "%s equal-content views compared different. left:%p, right:%p, size:%u."
		, ::llc::get_type_namep<T>(), (void*)viewA.begin(), (void*)viewB.begin(), viewA.size()
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EQUALITY_VALUE, viewA == viewDifferent || !(viewA != viewDifferent)
		, "%s different-content views compared equal. size:%u."
		, ::llc::get_type_namep<T>(), viewA.size()
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EQUALITY_SIZE, viewA == viewShort || !(viewA != viewShort)
		, "%s different-size views compared equal. left size:%u, right size:%u."
		, ::llc::get_type_namep<T>(), viewA.size(), viewShort.size()
		);
	::llc::view<T> emptyDefault;
	::llc::view<T> emptyAtData{0U, valuesA};
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EQUALITY_EMPTY, emptyDefault != emptyAtData || !(emptyDefault == emptyAtData)
		, "%s empty views compared different. left:%p, right:%p."
		, ::llc::get_type_namep<T>(), (void*)emptyDefault.begin(), (void*)emptyAtData.begin()
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_INEQUALITY_SYMMETRY
		, (viewA == viewB) == (viewA != viewB) || (viewA == viewDifferent) == (viewA != viewDifferent) || (viewA == viewShort) == (viewA != viewShort)
		, "%s equality and inequality operators disagreed."
		, ::llc::get_type_namep<T>()
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
		, "%s byte count mismatch. mutable:%u, const:%u, free:%u, expected:%u."
		, ::llc::get_type_namep<T>(), mutableView.byte_count(), constView.byte_count(), ::llc::byte_count(mutableView), expectedBytes
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_BIT_COUNT, mutableView.bit_count() != expectedBytes * 8ULL || constView.bit_count() != expectedBytes * 8ULL
		, "%s bit count mismatch. mutable:%" LLC_FMT_U3 ", const:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "."
		, ::llc::get_type_namep<T>(), mutableView.bit_count(), constView.bit_count(), ::llc::u3_t(expectedBytes * 8ULL)
		);

	::llc::view<::llc::sc_t> chars = mutableView.c();
	::llc::view<::llc::u0_t> bytes = mutableView.u8();
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_MUTABLE_CHAR_VIEW, chars.begin() != (::llc::sc_t*)data || chars.size() != expectedBytes
		, "%s mutable character view mismatch. begin:%p, expected:%p, size:%u, expected:%u."
		, ::llc::get_type_namep<T>(), (void*)chars.begin(), (void*)data, chars.size(), expectedBytes
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_MUTABLE_BYTE_VIEW, bytes.begin() != (::llc::u0_t*)data || bytes.size() != expectedBytes
		, "%s mutable byte view mismatch. begin:%p, expected:%p, size:%u, expected:%u."
		, ::llc::get_type_namep<T>(), (void*)bytes.begin(), (void*)data, bytes.size(), expectedBytes
		);
	chars[0] = 0x2A;
	bytes[expectedBytes - 1] = 0x5A;
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_MUTABLE_REPRESENTATION, ((::llc::u0_t*)data)[0] != 0x2A || ((::llc::u0_t*)data)[expectedBytes - 1] != 0x5A
		, "%s mutable representation write mismatch. first:%u, last:%u."
		, ::llc::get_type_namep<T>(), ((::llc::u0_t*)data)[0], ((::llc::u0_t*)data)[expectedBytes - 1]
		);

	::llc::view<::llc::sc_c> constChars = constView.cc();
	::llc::view<::llc::u0_c> constBytes = constView.u8();
	::llc::view<::llc::u0_c> constByteAlias = constView.cu8();
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_CONST_CHAR_VIEW, constChars.begin() != (::llc::sc_c*)data || constChars.size() != expectedBytes || (::llc::u0_t)constChars[0] != 0x2A
		, "%s const character view mismatch. begin:%p, expected:%p, size:%u, expected:%u, first:%u."
		, ::llc::get_type_namep<T>(), (cnst void*)constChars.begin(), (cnst void*)data, constChars.size(), expectedBytes, (::llc::u0_t)constChars[0]
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_CONST_BYTE_VIEW, constBytes.begin() != (::llc::u0_c*)data || constBytes.size() != expectedBytes || constBytes[expectedBytes - 1] != 0x5A
		, "%s const byte view mismatch. begin:%p, expected:%p, size:%u, expected:%u, last:%u."
		, ::llc::get_type_namep<T>(), (cnst void*)constBytes.begin(), (cnst void*)data, constBytes.size(), expectedBytes, constBytes[expectedBytes - 1]
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_CONST_BYTE_ALIAS, constByteAlias.begin() != constBytes.begin() || constByteAlias.size() != constBytes.size()
		, "%s const byte aliases differ. u8 begin:%p, cu8 begin:%p, u8 size:%u, cu8 size:%u."
		, ::llc::get_type_namep<T>(), (cnst void*)constBytes.begin(), (cnst void*)constByteAlias.begin(), constBytes.size(), constByteAlias.size()
		);

	::llc::view<T> nullEmpty;
	cnst ::llc::view<T> & constNullEmpty = nullEmpty;
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EMPTY_REPRESENTATION
		, nullEmpty.c().size() || nullEmpty.c().begin() || nullEmpty.u8().size() || nullEmpty.u8().begin()
		|| constNullEmpty.cc().size() || constNullEmpty.cc().begin() || constNullEmpty.u8().size() || constNullEmpty.u8().begin() || constNullEmpty.cu8().size() || constNullEmpty.cu8().begin()
		, "%s null-empty representation mismatch."
		, ::llc::get_type_namep<T>()
		);
	::llc::view<T> boundaryEmpty{(T*)data, 0};
	cnst ::llc::view<T> & constBoundaryEmpty = boundaryEmpty;
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_EMPTY_REPRESENTATION
		, boundaryEmpty.c().size() || boundaryEmpty.c().begin() != (::llc::sc_t*)data || boundaryEmpty.u8().size() || boundaryEmpty.u8().begin() != (::llc::u0_t*)data
		|| constBoundaryEmpty.cc().size() || constBoundaryEmpty.cc().begin() != (::llc::sc_c*)data || constBoundaryEmpty.u8().size() || constBoundaryEmpty.u8().begin() != (::llc::u0_c*)data || constBoundaryEmpty.cu8().size() || constBoundaryEmpty.cu8().begin() != (::llc::u0_c*)data
		, "%s boundary-empty representation mismatch. boundary:%p."
		, ::llc::get_type_namep<T>(), (void*)data
		);
	return 0;
}

tplt<tpnm T, ::llc::u2_t N>
sttc ::llc::err_t testMutationValues(ATestError & errors, VIEW_TEST_RESULT result, cnst T (&actual)[N], cnst T (&expected)[N], ::llc::sc_c * operation) {
	for(::llc::u2_t iElement = 0; iElement < N; ++iElement)
		LLC_TEST_CHECK(errors, result, actual[iElement] != expected[iElement]
			, "%s %s mismatch. index:%u, value:%" LLC_FMT_S3 ", expected:%" LLC_FMT_S3 "."
			, ::llc::get_type_namep<T>(), operation, iElement, ::llc::s3_t(actual[iElement]), ::llc::s3_t(expected[iElement])
			);
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testFill(ATestError & errors) {
	T full[6] = {T(0), T(1), T(2), T(3), T(4), T(5)};
	T fullExpected[6] = {T(9), T(9), T(9), T(9), T(9), T(9)};
	::llc::err_t result = ::llc::view<T>{full}.fill(T(9));
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FILL_FULL, result != (::llc::err_t)::llc::size(full)
		, "%s full fill returned:%i, expected:%u."
		, ::llc::get_type_namep<T>(), result, ::llc::u2_t(::llc::size(full))
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FILL_FULL, full, fullExpected, "full fill"));

	T ranged[6] = {T(0), T(1), T(2), T(3), T(4), T(5)};
	T rangedExpected[6] = {T(0), T(8), T(8), T(8), T(4), T(5)};
	result = ::llc::view<T>{ranged}.fill(T(8), 1, 4);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FILL_RANGE, result != 3
		, "%s ranged fill returned:%i, expected:3."
		, ::llc::get_type_namep<T>(), result
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FILL_RANGE, ranged, rangedExpected, "ranged fill"));

	T clipped[6] = {T(0), T(1), T(2), T(3), T(4), T(5)};
	T clippedExpected[6] = {T(0), T(1), T(2), T(7), T(7), T(7)};
	result = ::llc::view<T>{clipped}.fill(T(7), 3, 20);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FILL_CLIPPED, result != 3
		, "%s clipped fill returned:%i, expected:3."
		, ::llc::get_type_namep<T>(), result
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FILL_CLIPPED, clipped, clippedExpected, "clipped fill"));

	T unchanged[6] = {T(0), T(1), T(2), T(3), T(4), T(5)};
	T unchangedExpected[6] = {T(0), T(1), T(2), T(3), T(4), T(5)};
	result = ::llc::view<T>{unchanged}.fill(T(6), 4, 2);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FILL_EMPTY_RANGE, result
		, "%s empty-range fill returned:%i, expected:0."
		, ::llc::get_type_namep<T>(), result
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FILL_EMPTY_RANGE, unchanged, unchangedExpected, "empty-range fill"));

	::llc::view<T> nullEmpty;
	::llc::view<T> boundaryEmpty{(T*)unchanged, 0};
	cnst ::llc::err_t nullResult = nullEmpty.fill(T(1));
	cnst ::llc::err_t boundaryResult = boundaryEmpty.fill(T(1));
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FILL_EMPTY_VIEW, nullResult || boundaryResult
		, "%s empty fill returned a nonzero result. null:%i, boundary:%i."
		, ::llc::get_type_namep<T>(), nullResult, boundaryResult
		);
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testReverse(ATestError & errors) {
	T odd[5] = {T(0), T(1), T(2), T(3), T(4)};
	T oddExpected[5] = {T(4), T(3), T(2), T(1), T(0)};
	::llc::err_t result = ::llc::view<T>{odd}.revert();
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_REVERT_ODD, result
		, "%s odd revert returned:%i."
		, ::llc::get_type_namep<T>(), result
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_REVERT_ODD, odd, oddExpected, "odd revert"));

	T even[6] = {T(9), T(1), T(2), T(3), T(4), T(8)};
	T evenExpected[6] = {T(9), T(4), T(3), T(2), T(1), T(8)};
	result = ::llc::view<T>{even + 1, 4}.revert();
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_REVERT_EVEN, result
		, "%s even subrange revert returned:%i."
		, ::llc::get_type_namep<T>(), result
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_REVERT_EVEN, even, evenExpected, "even subrange revert"));

	T one[1] = {T(5)};
	T oneExpected[1] = {T(5)};
	::llc::view<T> nullEmpty;
	::llc::view<T> boundaryEmpty{one, 0U};
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_REVERT_EDGE, ::llc::view<T>{one}.revert() || nullEmpty.revert() || boundaryEmpty.revert()
		, "%s edge revert returned a failure."
		, ::llc::get_type_namep<T>()
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_REVERT_EDGE, one, oneExpected, "single-element revert"));

	T freeOdd[5] = {T(0), T(1), T(2), T(3), T(4)};
	result = ::llc::reverse(::llc::view<T>{freeOdd});
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_REVERSE_ODD, result
		, "%s odd reverse returned:%i."
		, ::llc::get_type_namep<T>(), result
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_REVERSE_ODD, freeOdd, oddExpected, "odd reverse"));

	T freeEven[6] = {T(9), T(1), T(2), T(3), T(4), T(8)};
	result = ::llc::reverse(::llc::view<T>{freeEven + 1, 4});
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_REVERSE_EVEN, result
		, "%s even subrange reverse returned:%i."
		, ::llc::get_type_namep<T>(), result
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_REVERSE_EVEN, freeEven, evenExpected, "even subrange reverse"));

	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_REVERSE_EDGE, ::llc::reverse(::llc::view<T>{one}) || ::llc::reverse(nullEmpty) || ::llc::reverse(boundaryEmpty)
		, "%s edge reverse returned a failure."
		, ::llc::get_type_namep<T>()
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_REVERSE_EDGE, one, oneExpected, "single-element reverse"));
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
		, "%s mutable full for_each mismatch. result:%i, visited:%u, expected:5."
		, ::llc::get_type_namep<T>(), result, visited
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_FULL, full, fullExpected, "mutable full for_each"));

	T offset[5] = {T(0), T(1), T(2), T(3), T(4)};
	T offsetExpected[5] = {T(0), T(1), T(12), T(13), T(14)};
	visited = 0;
	result = ::llc::view<T>{offset}.for_each(addTen, 2);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_OFFSET, result != 3 || visited != 3
		, "%s mutable offset for_each mismatch. result:%i, visited:%u, expected:3."
		, ::llc::get_type_namep<T>(), result, visited
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_OFFSET, offset, offsetExpected, "mutable offset for_each"));

	T ranged[5] = {T(0), T(1), T(2), T(3), T(4)};
	T rangedExpected[5] = {T(0), T(11), T(12), T(13), T(4)};
	visited = 0;
	result = ::llc::view<T>{ranged}.for_each(addTen, 1, 4);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_RANGE, result != 3 || visited != 3
		, "%s mutable ranged for_each mismatch. result:%i, visited:%u, expected:3."
		, ::llc::get_type_namep<T>(), result, visited
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_RANGE, ranged, rangedExpected, "mutable ranged for_each"));

	T clipped[5] = {T(0), T(1), T(2), T(3), T(4)};
	T clippedExpected[5] = {T(0), T(1), T(2), T(13), T(14)};
	visited = 0;
	result = ::llc::view<T>{clipped}.for_each(addTen, 3, 20);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_RANGE, result != 2 || visited != 2
		, "%s mutable clipped for_each mismatch. result:%i, visited:%u, expected:2."
		, ::llc::get_type_namep<T>(), result, visited
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_FOREACH_MUTABLE_RANGE, clipped, clippedExpected, "mutable clipped for_each"));

	T constData[5] = {T(0), T(1), T(2), T(3), T(4)};
	cnst ::llc::view<T> constView{constData};
	::llc::s3_t sum = 0;
	visited = 0;
	::llc::TFuncForEachConst<T> read = [&visited, &sum](cnst T & value) { ++visited; sum += value; return 0; };
	result = constView.for_each(read, 2);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FOREACH_CONST, result != 3 || visited != 3 || sum != 9
		, "%s const offset for_each mismatch. result:%i, visited:%u, sum:%" LLC_FMT_S3 ", expected result:3, visited:3, sum:9."
		, ::llc::get_type_namep<T>(), result, visited, sum
		);

	T unchanged[5] = {T(0), T(1), T(2), T(3), T(4)};
	T unchangedExpected[5] = {T(0), T(1), T(2), T(3), T(4)};
	visited = 0;
	result = ::llc::view<T>{unchanged}.for_each(addTen, 4, 2);
	::llc::view<T> nullEmpty;
	cnst ::llc::err_t emptyResult = nullEmpty.for_each(addTen);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FOREACH_EMPTY, result || emptyResult || visited
		, "%s empty for_each mismatch. reversed result:%i, empty result:%i, visited:%u."
		, ::llc::get_type_namep<T>(), result, emptyResult, visited
		);
	return testMutationValues(errors, VIEW_TEST_RESULT_FOREACH_EMPTY, unchanged, unchangedExpected, "empty-range for_each");
}

tplt<tpnm T>
sttc ::llc::err_t testEnumerate(ATestError & errors) {
	T full[5] = {};
	T fullExpected[5] = {T(10), T(11), T(12), T(13), T(14)};
	::llc::u2_t visited = 0, indices = 0;
	::llc::TFuncEnumerate<T> writeIndex = [&visited, &indices](::llc::u2_t & index, T & value) { ++visited; indices |= 1U << index; value = T(10 + index); return 0; };
	::llc::err_t result = ::llc::view<T>{full}.enumerate(writeIndex);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_ENUMERATE_MUTABLE, result != 5 || visited != 5 || indices != 0x1F
		, "%s mutable full enumerate mismatch. result:%i, visited:%u, indices:0x%X."
		, ::llc::get_type_namep<T>(), result, visited, indices
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_ENUMERATE_MUTABLE, full, fullExpected, "mutable full enumerate"));

	T ranged[5] = {};
	T rangedExpected[5] = {T(0), T(11), T(12), T(13), T(0)};
	visited = indices = 0;
	result = ::llc::view<T>{ranged}.enumerate(writeIndex, 1, 4);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_ENUMERATE_RANGE, result != 3 || visited != 3 || indices != 0x0E
		, "%s mutable ranged enumerate mismatch. result:%i, visited:%u, indices:0x%X, expected result:3, visited:3, indices:0x0E."
		, ::llc::get_type_namep<T>(), result, visited, indices
		);
	if_fail_fe(testMutationValues(errors, VIEW_TEST_RESULT_ENUMERATE_RANGE, ranged, rangedExpected, "mutable ranged enumerate"));

	T constData[5] = {T(0), T(1), T(2), T(3), T(4)};
	cnst ::llc::view<T> constView{constData};
	::llc::s3_t sum = 0;
	visited = indices = 0;
	::llc::TFuncEnumerateConst<T> readIndex = [&visited, &indices, &sum](::llc::u2_t & index, cnst T & value) { ++visited; indices |= 1U << index; sum += value; return 0; };
	result = constView.enumerate(readIndex, 2);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_ENUMERATE_CONST, result != 3 || visited != 3 || indices != 0x1C || sum != 9
		, "%s const offset enumerate mismatch. result:%i, visited:%u, indices:0x%X, sum:%" LLC_FMT_S3 "."
		, ::llc::get_type_namep<T>(), result, visited, indices, sum
		);

	visited = indices = 0;
	result = ::llc::view<T>{ranged}.enumerate(writeIndex, 4, 2);
	::llc::view<T> nullEmpty;
	cnst ::llc::err_t emptyResult = nullEmpty.enumerate(writeIndex);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_ENUMERATE_EMPTY, result || emptyResult || visited || indices
		, "%s empty enumerate mismatch. reversed result:%i, empty result:%i, visited:%u, indices:0x%X."
		, ::llc::get_type_namep<T>(), result, emptyResult, visited, indices
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
		, "%s mutable for_each failure mismatch. result:%i, visited:%u, expected visited:3."
		, ::llc::get_type_namep<T>(), mutableForEachResult, mutableForEachVisits
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FOREACH_FAILURE, !::llc::failed(constForEachResult) || constForEachVisits != 3
		, "%s const for_each failure mismatch. result:%i, visited:%u, expected visited:3."
		, ::llc::get_type_namep<T>(), constForEachResult, constForEachVisits
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FOREACH_FAILURE, !::llc::failed(mutableRangeResult) || mutableRangeVisits != 2
		, "%s mutable ranged for_each failure mismatch. result:%i, visited:%u, expected visited:2."
		, ::llc::get_type_namep<T>(), mutableRangeResult, mutableRangeVisits
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_FOREACH_FAILURE, !::llc::failed(constRangeResult) || constRangeVisits != 2
		, "%s const ranged for_each failure mismatch. result:%i, visited:%u, expected visited:2."
		, ::llc::get_type_namep<T>(), constRangeResult, constRangeVisits
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_ENUMERATE_FAILURE, !::llc::failed(mutableEnumerateResult) || mutableEnumerateVisits != 3
		, "%s mutable enumerate failure mismatch. result:%i, visited:%u, expected visited:3."
		, ::llc::get_type_namep<T>(), mutableEnumerateResult, mutableEnumerateVisits
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_ENUMERATE_FAILURE, !::llc::failed(constEnumerateResult) || constEnumerateVisits != 3
		, "%s const enumerate failure mismatch. result:%i, visited:%u, expected visited:3."
		, ::llc::get_type_namep<T>(), constEnumerateResult, constEnumerateVisits
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_ENUMERATE_FAILURE, !::llc::failed(mutableEnumerateRangeResult) || mutableEnumerateRangeVisits != 2
		, "%s mutable ranged enumerate failure mismatch. result:%i, visited:%u, expected visited:2."
		, ::llc::get_type_namep<T>(), mutableEnumerateRangeResult, mutableEnumerateRangeVisits
		);
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_ENUMERATE_FAILURE, !::llc::failed(constEnumerateRangeResult) || constEnumerateRangeVisits != 2
		, "%s const ranged enumerate failure mismatch. result:%i, visited:%u, expected visited:2."
		, ::llc::get_type_namep<T>(), constEnumerateRangeResult, constEnumerateRangeVisits
		);
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testInvalidConstruction(ATestError & errors) {
#ifdef LLC_WINDOWS
	LLC_TEST_CHECK(errors, VIEW_TEST_RESULT_INVALID_CONSTRUCTION, !testThrows([&]() { cnst ::llc::view<T> invalid{(T*)0, 1}; (void)invalid; })
		, "%s view accepted null storage with one element."
		, ::llc::get_type_namep<T>()
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
	else always_printf("%s suite OK: %u checks passed.", ::llc::get_type_namep<T>(), typeChecks);
	return 0;
}

::llc::err_t testView(ATestError & errors) {
	if_fail_fe(testTypeLogged<::llc::u0_t>(errors));
	if_fail_fe(testTypeLogged<::llc::u1_t>(errors));
	if_fail_fe(testTypeLogged<::llc::u2_t>(errors));
	if_fail_fe(testTypeLogged<::llc::u3_t>(errors));
	if_fail_fe(testTypeLogged<::llc::s0_t>(errors));
	if_fail_fe(testTypeLogged<::llc::s1_t>(errors));
	if_fail_fe(testTypeLogged<::llc::s2_t>(errors));
	if_fail_fe(testTypeLogged<::llc::s3_t>(errors));
	return 0;
}
