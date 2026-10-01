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
