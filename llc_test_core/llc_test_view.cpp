#include "llc_view.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(VIEW_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, OK					, 0, "All view tests passed.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, DEFAULT_STATE			, 1, "A default view was not empty with null boundaries.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, ARRAY_CONSTRUCTION		, 2, "An array-backed view did not preserve its storage and full element count.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, COUNT_FIRST_CONSTRUCTION	, 3, "A count-first array view did not clip its element count to the backing array.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, ARRAY_FIRST_CONSTRUCTION	, 4, "An array-first view did not preserve its explicitly checked element count.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, POINTER_CONSTRUCTION		, 5, "A pointer-backed view did not preserve its supplied range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, EMPTY_RANGE			, 6, "A zero-length view did not preserve equal begin and end boundaries.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, CONST_CONVERSION			, 7, "Converting a mutable view to a const view changed its range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, INVALID_CONSTRUCTION		, 8, "view<> accepted null storage with a nonzero element count.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_FULL				, 9, "slice() did not reproduce the full source range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_REMAINDER			, 10, "slice() did not produce the expected range after an offset.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_COUNT			, 11, "slice() did not honor an explicit valid element count.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_END				, 12, "slice() at the exact end did not produce an empty one-past-end range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_SELF				, 13, "slice() could not advance and shorten its own view.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_INVALID_OFFSET		, 14, "slice() accepted an offset beyond the source range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_INVALID_COUNT		, 15, "slice() accepted a count beyond the remaining source range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_FAILURE_STATE		, 16, "A failed slice() modified its output view.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SLICE_EMPTY			, 17, "Slicing a default empty view did not produce a default empty range.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SUBSCRIPT_READ			, 18, "operator[] did not read the selected element.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, SUBSCRIPT_WRITE			, 19, "Mutable operator[] did not update the selected element.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, INVALID_SUBSCRIPT			, 20, "operator[] accepted an index at or beyond the view size.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, EMPTY_SUBSCRIPT			, 21, "operator[] accepted an element access on a default empty view.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, EQUALITY_IDENTITY			, 22, "Equal views over the same range compared different.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, EQUALITY_CONTENT			, 23, "Views over independent equal contents compared different.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, EQUALITY_VALUE			, 24, "Views containing a different value compared equal.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, EQUALITY_SIZE			, 25, "Views with different element counts compared equal.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, EQUALITY_EMPTY			, 26, "Empty views with different boundary pointers compared different.");
GDEFINE_ENUM_VALUED(VIEW_TEST_RESULT, INEQUALITY_SYMMETRY		, 27, "operator!= was not the inverse of operator==.");

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
