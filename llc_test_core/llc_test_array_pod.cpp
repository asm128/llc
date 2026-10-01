#include "llc_array_pod.h"

#include "llc_test_core.h"

#include <utility>

GDEFINE_ENUM_TYPE(ARRAY_POD_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, OK						, 0, "All array_pod<> tests passed.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, DEFAULT_STATE			, 1, "A default array_pod<> was not an empty null range.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, INITIALIZER_CONSTRUCTION	, 2, "array_pod<> did not copy its initializer list.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, ARRAY_CONSTRUCTION		, 3, "array_pod<> did not copy its source array.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, VIEW_CONSTRUCTION		, 4, "array_pod<> did not copy its source view.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, COPY_CONSTRUCTION		, 5, "array_pod<> copy construction did not produce independent equal storage.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, MOVE_CONSTRUCTION		, 6, "array_pod<> move construction did not transfer its allocation.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, MOVE_SOURCE				, 7, "A moved-from array_pod<> did not become an empty null range.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, CONST_VIEW				, 8, "array_pod<> did not expose its counted const view.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, COPY_ASSIGNMENT			, 9, "array_pod<> copy assignment did not produce independent equal storage.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, VIEW_ASSIGNMENT			, 10, "array_pod<> did not copy its assigned view.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, ARRAY_ASSIGNMENT			, 11, "array_pod<> did not copy its assigned array.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, SELF_ASSIGNMENT			, 12, "array_pod<> changed during self-assignment.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, RESERVE_GROW				, 13, "array_pod<>::reserve() did not provide the requested capacity.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, RESERVE_PRESERVE			, 14, "array_pod<>::reserve() did not preserve its elements.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, RESERVE_STABLE			, 15, "array_pod<>::reserve() reallocated for an already satisfied request.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, RESIZE_COUNT				, 16, "array_pod<>::resize() returned or stored an incorrect count.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, RESIZE_FILL				, 17, "array_pod<>::resize(count, value) did not initialize new elements.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, RESIZE_PRESERVE			, 18, "array_pod<>::resize() did not preserve retained elements.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, RESIZE_BITS				, 19, "array_pod<>::resize_bits() did not round to the required element count.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, PUSH_BACK					, 20, "array_pod<>::push_back() did not append and report the new element index.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, APPEND_POINTER			, 21, "array_pod<>::append(pointer, count) did not append its source range.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, APPEND_ARRAY				, 22, "array_pod<>::append(array) did not append its source range.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, APPEND_VIEW				, 23, "array_pod<>::append(view) did not append its source range.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, APPEND_EMPTY				, 24, "array_pod<>::append() changed state for an empty source range.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, CLEAR						, 25, "array_pod<>::clear() did not retain allocation as an empty range.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, CLEAR_POINTER			, 26, "array_pod<>::clear_pointer() did not release and reset its storage.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, TERMINATOR				, 27, "array_pod<> did not preserve its zero-value terminator.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, POP_BACK					, 28, "array_pod<>::pop_back() did not remove its final element.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, POP_BACK_VALUE			, 29, "array_pod<>::pop_back(value) did not return and remove its final element.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, INSERT_VALUE				, 30, "array_pod<>::insert(value) did not insert at the requested position.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, INSERT_VALUE_REALLOCATE	, 31, "array_pod<>::insert(value) failed while growing its allocation.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, INSERT_POINTER			, 32, "array_pod<>::insert(pointer, count) did not insert its source range.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, INSERT_ARRAY				, 33, "array_pod<>::insert(array) did not insert its source range.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, INSERT_VIEW				, 34, "array_pod<>::insert(view) did not insert its source range.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, INSERT_CHAIN_REALLOCATE	, 35, "array_pod<>::insert(pointer, count) failed while growing its allocation.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, REMOVE						, 36, "array_pod<>::remove() did not erase the requested element in order.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, REMOVE_UNORDERED			, 37, "array_pod<>::remove_unordered() did not replace the removed element with the last one.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, ERASE						, 38, "array_pod<>::erase() did not erase the addressed element.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, APPEND_CHAINS				, 39, "array_pod<>::append(view of views) did not append every source range and report the written count.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, APPEND_STRING_ARRAY		, 40, "array_pod<>::append_string(array) did not append text and report the written count.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, APPEND_STRING_VIEW		, 41, "array_pod<>::append_string(view) did not append text and report the written count.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, APPEND_STRING_ELEMENT		, 42, "array_pod<>::append_string(element) did not append one character and report one written element.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, APPEND_STRING_FUNCTION	, 43, "array_pod<>::append_string(function) did not invoke the formatter and forward its result.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, APPEND_STRING_EMPTY_FUNCTION, 44, "array_pod<>::append_string(function) changed output for an empty formatter.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, APPEND_STRINGS				, 45, "array_pod<>::append_strings() did not append every string and report the written count.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, POP_BACK_EMPTY				, 46, "An empty array_pod<>::pop_back() did not fail without modifying its output.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, INSERT_INVALID_INDEX		, 47, "array_pod<>::insert() accepted an index beyond its logical range or changed state on failure.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, REMOVE_INVALID_INDEX		, 48, "array_pod<>::remove() accepted an index beyond its logical range or changed state on failure.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, REMOVE_UNORDERED_INVALID	, 49, "array_pod<>::remove_unordered() accepted an index beyond its logical range or changed state on failure.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, ERASE_INVALID_POINTER		, 50, "array_pod<>::erase() accepted a pointer outside its logical range or changed state on failure.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, RESERVE_INVALID_COUNT		, 51, "array_pod<>::reserve() accepted a count beyond its supported range or changed state on failure.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, RESIZE_INVALID_COUNT		, 52, "array_pod<>::resize() accepted a count beyond its supported range or changed state on failure.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, APPEND_NULL_SOURCE			, 53, "array_pod<>::append() accepted a null non-empty source or changed state on failure.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, INSERT_NULL_SOURCE			, 54, "array_pod<>::insert() accepted a null non-empty source or changed state on failure.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, INSERT_EMPTY_SOURCE		, 55, "array_pod<>::insert() changed state for an empty source range.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, ALIAS_PUSH_BACK				, 56, "array_pod<>::push_back() did not preserve a source value from its own reallocated storage.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, ALIAS_RESIZE					, 57, "array_pod<>::resize(count, value) did not preserve a source value from its own reallocated storage.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, ALIAS_APPEND					, 58, "array_pod<>::append() did not preserve a source view of its own reallocated storage.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, ALIAS_INSERT_VALUE			, 59, "array_pod<>::insert(value) did not preserve a source value from its own shifted storage.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, ALIAS_INSERT_CHAIN			, 60, "array_pod<>::insert(chain) did not preserve a source view of its own shifted storage.");
GDEFINE_ENUM_VALUED(ARRAY_POD_TEST_RESULT, ALIAS_ASSIGNMENT			, 61, "array_pod<> assignment did not safely copy an overlapping subview of itself.");

tplt<tpnm TCall>
sttc ::llc::err_t podExpectedFailure(TCall call) {
	::llc::setupLogCallbacks(0, 0);
	cnst ::llc::err_t result = call();
	::llc::setupDefaultLogCallbacks();
	rtrn result;
}

tplt<tpnm T>
sttc bool podMismatch(cnst ::llc::apod<T> & actual, ::llc::view<cnst T> expected) {
	rtrn actual.size() != expected.size() || (actual.size() && 0 != memcmp(actual.begin(), expected.begin(), actual.byte_count()));
}

tplt<tpnm T, size_t N>
sttc bool podMismatch(cnst ::llc::apod<T> & actual, cnst T (&expected)[N]) {
	rtrn podMismatch(actual, ::llc::view<cnst T>{expected});
}

tplt<tpnm T>
sttc bool podTerminatorMismatch(cnst ::llc::apod<T> & value) {
	rtrn value.begin() && value.begin()[value.size()] != T{};
}

tplt<tpnm T>
sttc ::llc::err_t testPodConstruction(ATestError & errors) {
	::llc::apod<T> empty;
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_DEFAULT_STATE, empty.size() || empty.begin() || empty.end()
		, "%s default state mismatch. size:%u, begin:%p, end:%p."
		, ::llc::get_type_namep<T>(), empty.size(), (void*)empty.begin(), (void*)empty.end()
		);

	T expected[] = {T(1), T(2), T(3)};
	::llc::apod<T> initialized = {T(1), T(2), T(3)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_INITIALIZER_CONSTRUCTION, podMismatch(initialized, expected)
		, "%s initializer construction mismatch. size:%u, expected:3."
		, ::llc::get_type_namep<T>(), initialized.size()
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_TERMINATOR, podTerminatorMismatch(initialized)
		, "%s initializer terminator mismatch. size:%u, terminator:%" LLC_FMT_S3 "."
		, ::llc::get_type_namep<T>(), initialized.size(), (::llc::s3_t)initialized.begin()[initialized.size()]
		);

	::llc::apod<T> fromArray{expected};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_ARRAY_CONSTRUCTION, podMismatch(fromArray, expected) || fromArray.begin() == expected
		, "%s array construction mismatch. size:%u, source:%p, copy:%p."
		, ::llc::get_type_namep<T>(), fromArray.size(), (void*)expected, (void*)fromArray.begin()
		);
	::llc::view<cnst T> sourceView{expected};
	::llc::apod<T> fromView{sourceView};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_VIEW_CONSTRUCTION, podMismatch(fromView, expected) || fromView.begin() == expected
		, "%s view construction mismatch. size:%u, source:%p, copy:%p."
		, ::llc::get_type_namep<T>(), fromView.size(), (cnst void*)sourceView.begin(), (void*)fromView.begin()
		);

	::llc::apod<T> copied{initialized};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_COPY_CONSTRUCTION, podMismatch(copied, expected) || copied.begin() == initialized.begin()
		, "%s copy construction mismatch. source:%p/%u, copy:%p/%u."
		, ::llc::get_type_namep<T>(), (void*)initialized.begin(), initialized.size(), (void*)copied.begin(), copied.size()
		);
	copied[0] = T(9);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_COPY_CONSTRUCTION, initialized[0] != T(1)
		, "%s copied storage was not independent. source[0]:%" LLC_FMT_S3 ", expected:1."
		, ::llc::get_type_namep<T>(), (::llc::s3_t)initialized[0]
		);

	T * movedAddress = initialized.begin();
	::llc::apod<T> moved{::std::move(initialized)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_MOVE_CONSTRUCTION, moved.begin() != movedAddress || podMismatch(moved, expected)
		, "%s move construction mismatch. old:%p, moved:%p/%u."
		, ::llc::get_type_namep<T>(), (void*)movedAddress, (void*)moved.begin(), moved.size()
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_MOVE_SOURCE, initialized.size() || initialized.begin() || initialized.end()
		, "%s moved source mismatch. size:%u, begin:%p, end:%p."
		, ::llc::get_type_namep<T>(), initialized.size(), (void*)initialized.begin(), (void*)initialized.end()
		);

	cnst ::llc::apod<T> & constMoved = moved;
	::llc::view<cnst T> constView = constMoved;
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_CONST_VIEW, constView.begin() != moved.begin() || constView.size() != moved.size()
		, "%s const view mismatch. view:%p/%u, source:%p/%u."
		, ::llc::get_type_namep<T>(), (cnst void*)constView.begin(), constView.size(), (void*)moved.begin(), moved.size()
		);
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testPodAssignment(ATestError & errors) {
	T firstExpected[]	= {T(1), T(2), T(3)};
	T secondExpected[]	= {T(4), T(5)};
	::llc::apod<T> source{firstExpected};
	::llc::apod<T> target{T(9)};
	target = source;
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_COPY_ASSIGNMENT, podMismatch(target, firstExpected) || target.begin() == source.begin()
		, "%s copy assignment mismatch. source:%p/%u, target:%p/%u."
		, ::llc::get_type_namep<T>(), (void*)source.begin(), source.size(), (void*)target.begin(), target.size()
		);
	target[0] = T(8);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_COPY_ASSIGNMENT, source[0] != T(1)
		, "%s assigned storage was not independent. source[0]:%" LLC_FMT_S3 ", expected:1."
		, ::llc::get_type_namep<T>(), (::llc::s3_t)source[0]
		);

	::llc::view<cnst T> secondView{secondExpected};
	target = secondView;
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_VIEW_ASSIGNMENT, podMismatch(target, secondExpected)
		, "%s view assignment mismatch. size:%u, expected:2."
		, ::llc::get_type_namep<T>(), target.size()
		);
	target = firstExpected;
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_ARRAY_ASSIGNMENT, podMismatch(target, firstExpected)
		, "%s array assignment mismatch. size:%u, expected:3."
		, ::llc::get_type_namep<T>(), target.size()
		);
	target = target;
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_SELF_ASSIGNMENT, podMismatch(target, firstExpected)
		, "%s self-assignment changed content. size:%u, expected:3."
		, ::llc::get_type_namep<T>(), target.size()
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_TERMINATOR, podTerminatorMismatch(target)
		, "%s assignment terminator mismatch. size:%u, terminator:%" LLC_FMT_S3 "."
		, ::llc::get_type_namep<T>(), target.size(), (::llc::s3_t)target.begin()[target.size()]
		);
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testPodReserveResize(ATestError & errors) {
	T expected[] = {T(1), T(2), T(3)};
	::llc::apod<T> values{expected};
	cnst ::llc::err_t capacity = values.reserve(32);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_RESERVE_GROW, capacity < 32
		, "%s reserve capacity mismatch. actual:%i, requested:32."
		, ::llc::get_type_namep<T>(), capacity
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_RESERVE_PRESERVE, podMismatch(values, expected)
		, "%s reserve changed content. size:%u, expected:3."
		, ::llc::get_type_namep<T>(), values.size()
		);
	T * reservedAddress = values.begin();
	cnst ::llc::err_t stableCapacity = values.reserve(16);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_RESERVE_STABLE, stableCapacity != capacity || values.begin() != reservedAddress
		, "%s satisfied reserve changed allocation. capacity:%i/%i, address:%p/%p."
		, ::llc::get_type_namep<T>(), stableCapacity, capacity, (void*)values.begin(), (void*)reservedAddress
		);

	::llc::err_t result = values.resize(5, T(9));
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_RESIZE_COUNT, result != 5 || values.size() != 5
		, "%s filled resize count mismatch. result:%i, size:%u, expected:5."
		, ::llc::get_type_namep<T>(), result, values.size()
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_RESIZE_FILL, values[3] != T(9) || values[4] != T(9)
		, "%s resize fill mismatch. values[3]:%" LLC_FMT_S3 ", values[4]:%" LLC_FMT_S3 ", expected:9."
		, ::llc::get_type_namep<T>(), (::llc::s3_t)values[3], (::llc::s3_t)values[4]
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_RESIZE_PRESERVE, values[0] != T(1) || values[1] != T(2) || values[2] != T(3)
		, "%s resize growth changed retained values. actual:%" LLC_FMT_S3 ",%" LLC_FMT_S3 ",%" LLC_FMT_S3 "."
		, ::llc::get_type_namep<T>(), (::llc::s3_t)values[0], (::llc::s3_t)values[1], (::llc::s3_t)values[2]
		);
	result = values.resize(2);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_RESIZE_COUNT, result != 2 || values.size() != 2
		, "%s shrinking resize count mismatch. result:%i, size:%u, expected:2."
		, ::llc::get_type_namep<T>(), result, values.size()
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_RESIZE_PRESERVE, values[0] != T(1) || values[1] != T(2)
		, "%s shrinking resize changed retained values. actual:%" LLC_FMT_S3 ",%" LLC_FMT_S3 "."
		, ::llc::get_type_namep<T>(), (::llc::s3_t)values[0], (::llc::s3_t)values[1]
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_TERMINATOR, podTerminatorMismatch(values)
		, "%s resize terminator mismatch. size:%u, terminator:%" LLC_FMT_S3 "."
		, ::llc::get_type_namep<T>(), values.size(), (::llc::s3_t)values.begin()[values.size()]
		);

	::llc::apod<T> bits;
	stxp ::llc::u2_t elementBits = szof(T) * 8U;
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_RESIZE_BITS, bits.resize_bits(0) != 0 || bits.size()
		, "%s zero-bit resize mismatch. size:%u."
		, ::llc::get_type_namep<T>(), bits.size()
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_RESIZE_BITS, bits.resize_bits(1) != 1 || bits.size() != 1
		, "%s one-bit resize mismatch. size:%u, expected:1."
		, ::llc::get_type_namep<T>(), bits.size()
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_RESIZE_BITS, bits.resize_bits(elementBits) != 1 || bits.size() != 1
		, "%s exact-element bit resize mismatch. bits:%u, size:%u, expected:1."
		, ::llc::get_type_namep<T>(), elementBits, bits.size()
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_RESIZE_BITS, bits.resize_bits(elementBits + 1) != 2 || bits.size() != 2
		, "%s rounded bit resize mismatch. bits:%u, size:%u, expected:2."
		, ::llc::get_type_namep<T>(), elementBits + 1, bits.size()
		);
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testPodAppend(ATestError & errors) {
	::llc::apod<T> values = {T(1), T(2)};
	::llc::err_t result = values.push_back(T(3));
	T expectedPush[] = {T(1), T(2), T(3)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_PUSH_BACK, result != 2 || podMismatch(values, expectedPush)
		, "%s push_back mismatch. result:%i, size:%u, expected index:2/size:3."
		, ::llc::get_type_namep<T>(), result, values.size()
		);

	T pointerTail[] = {T(4), T(5)};
	result = values.append(pointerTail, 2);
	T expectedPointer[] = {T(1), T(2), T(3), T(4), T(5)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_APPEND_POINTER, result != 3 || podMismatch(values, expectedPointer)
		, "%s pointer append mismatch. result:%i, size:%u, expected index:3/size:5."
		, ::llc::get_type_namep<T>(), result, values.size()
		);

	T arrayTail[] = {T(6), T(7)};
	result = values.append(arrayTail);
	T expectedArray[] = {T(1), T(2), T(3), T(4), T(5), T(6), T(7)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_APPEND_ARRAY, result != 5 || podMismatch(values, expectedArray)
		, "%s array append mismatch. result:%i, size:%u, expected index:5/size:7."
		, ::llc::get_type_namep<T>(), result, values.size()
		);

	T viewTail[] = {T(8), T(9)};
	::llc::view<cnst T> tailView{viewTail};
	result = values.append(tailView);
	T expectedView[] = {T(1), T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_APPEND_VIEW, result != 7 || podMismatch(values, expectedView)
		, "%s view append mismatch. result:%i, size:%u, expected index:7/size:9."
		, ::llc::get_type_namep<T>(), result, values.size()
		);
	T * addressBeforeEmpty = values.begin();
	result = values.append((cnst T*)0, 0);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_APPEND_EMPTY, result != 9 || values.size() != 9 || values.begin() != addressBeforeEmpty
		, "%s empty append mismatch. result:%i, size:%u, address:%p/%p."
		, ::llc::get_type_namep<T>(), result, values.size(), (void*)values.begin(), (void*)addressBeforeEmpty
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_TERMINATOR, podTerminatorMismatch(values)
		, "%s append terminator mismatch. size:%u, terminator:%" LLC_FMT_S3 "."
		, ::llc::get_type_namep<T>(), values.size(), (::llc::s3_t)values.begin()[values.size()]
		);

	T firstChain[] = {T(1), T(2)};
	T secondChain[] = {T(3), T(4), T(5)};
	::llc::view<cnst T> chains[] = {{firstChain}, {}, {secondChain}};
	::llc::apod<T> chained = {T(9)};
	result = chained.append(::llc::view<cnst ::llc::view<cnst T>>{chains});
	T expectedChains[] = {T(9), T(1), T(2), T(3), T(4), T(5)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_APPEND_CHAINS, result != 5 || podMismatch(chained, expectedChains)
		, "%s chained append mismatch. result:%i, size:%u, expected written:5/size:6."
		, ::llc::get_type_namep<T>(), result, chained.size()
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_TERMINATOR, podTerminatorMismatch(chained)
		, "%s chained append terminator mismatch. size:%u, terminator:%" LLC_FMT_S3 "."
		, ::llc::get_type_namep<T>(), chained.size(), (::llc::s3_t)chained.begin()[chained.size()]
		);
	rtrn 0;
}

sttc bool podStringMismatch(cnst ::llc::asc_t & actual, ::llc::vcst_t expected) {
	rtrn podMismatch(actual, ::llc::view<cnst ::llc::sc_t>{expected.begin(), expected.size()});
}

sttc ::llc::err_t testPodStringAppend(ATestError & errors) {
	::llc::asc_t text = {'s', 'e', 'e', 'd'};
	::llc::err_t result = text.append_string("ab");
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_APPEND_STRING_ARRAY, result != 2 || podStringMismatch(text, "seedab")
		, "Array string append mismatch. result:%i, size:%u, expected written:2/text:'seedab'."
		, result, text.size()
		);

	result = text.append_string(::llc::vcst_t{"xyz"});
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_APPEND_STRING_VIEW, result != 3 || podStringMismatch(text, "seedabxyz")
		, "View string append mismatch. result:%i, size:%u, expected written:3/text:'seedabxyz'."
		, result, text.size()
		);

	result = text.append_string('!');
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_APPEND_STRING_ELEMENT, result != 1 || podStringMismatch(text, "seedabxyz!")
		, "Element string append mismatch. result:%i, size:%u, expected written:1/text:'seedabxyz!'."
		, result, text.size()
		);

	::llc::function<::llc::err_t(::llc::asc_t&)> formatter = [](::llc::asc_t & output) {
		if_fail_fe(output.append_string("ok"));
		rtrn 2;
	};
	result = text.append_string(formatter);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_APPEND_STRING_FUNCTION, result != 2 || podStringMismatch(text, "seedabxyz!ok")
		, "Function string append mismatch. result:%i, size:%u, expected result:2/text:'seedabxyz!ok'."
		, result, text.size()
		);

	::llc::function<::llc::err_t(::llc::asc_t&)> emptyFormatter;
	result = text.append_string(emptyFormatter);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_APPEND_STRING_EMPTY_FUNCTION, result || podStringMismatch(text, "seedabxyz!ok")
		, "Empty function string append mismatch. result:%i, size:%u."
		, result, text.size()
		);

	::llc::vcst_t strings[] = {"12", "", "345"};
	result = text.append_strings(::llc::view<cnst ::llc::vcst_t>{strings});
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_APPEND_STRINGS, result != 5 || podStringMismatch(text, "seedabxyz!ok12345")
		, "Multiple string append mismatch. result:%i, size:%u, expected written:5/text:'seedabxyz!ok12345'."
		, result, text.size()
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_TERMINATOR, podTerminatorMismatch(text)
		, "String append terminator mismatch. size:%u, terminator:%i."
		, text.size(), text.begin()[text.size()]
		);
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testPodPopBack(ATestError & errors) {
	::llc::apod<T> values = {T(1), T(2), T(3)};
	T removed = {};
	::llc::err_t result = values.pop_back(removed);
	T expectedValue[] = {T(1), T(2)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_POP_BACK_VALUE, result != 2 || removed != T(3) || podMismatch(values, expectedValue)
		, "%s pop_back(value) mismatch. result:%i, removed:%" LLC_FMT_S3 ", size:%u."
		, ::llc::get_type_namep<T>(), result, (::llc::s3_t)removed, values.size()
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_TERMINATOR, podTerminatorMismatch(values)
		, "%s pop_back(value) terminator mismatch. size:%u, terminator:%" LLC_FMT_S3 "."
		, ::llc::get_type_namep<T>(), values.size(), (::llc::s3_t)values.begin()[values.size()]
		);

	result = values.pop_back();
	T expected[] = {T(1)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_POP_BACK, result != 1 || podMismatch(values, expected)
		, "%s pop_back mismatch. result:%i, size:%u, first:%" LLC_FMT_S3 "."
		, ::llc::get_type_namep<T>(), result, values.size(), (::llc::s3_t)values[0]
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_TERMINATOR, podTerminatorMismatch(values)
		, "%s pop_back terminator mismatch. size:%u, terminator:%" LLC_FMT_S3 "."
		, ::llc::get_type_namep<T>(), values.size(), (::llc::s3_t)values.begin()[values.size()]
		);
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testPodInsert(ATestError & errors) {
	::llc::apod<T> values = {T(2), T(4)};
	values.reserve(16);
	T * reservedAddress = values.begin();
	::llc::err_t result = values.insert(0, T(1));
	T expectedFront[] = {T(1), T(2), T(4)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_INSERT_VALUE, result != 3 || values.begin() != reservedAddress || podMismatch(values, expectedFront)
		, "%s front value insertion mismatch. result:%i, size:%u, address:%p/%p."
		, ::llc::get_type_namep<T>(), result, values.size(), (void*)values.begin(), (void*)reservedAddress
		);
	result = values.insert(2, T(3));
	T expectedMiddle[] = {T(1), T(2), T(3), T(4)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_INSERT_VALUE, result != 4 || values.begin() != reservedAddress || podMismatch(values, expectedMiddle)
		, "%s middle value insertion mismatch. result:%i, size:%u."
		, ::llc::get_type_namep<T>(), result, values.size()
		);
	result = values.insert(values.size(), T(5));
	T expectedEnd[] = {T(1), T(2), T(3), T(4), T(5)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_INSERT_VALUE, result != 5 || values.begin() != reservedAddress || podMismatch(values, expectedEnd)
		, "%s end value insertion mismatch. result:%i, size:%u."
		, ::llc::get_type_namep<T>(), result, values.size()
		);

	::llc::apod<T> reallocated = {T(1), T(3)};
	cnst ::llc::u2_t capacity = reallocated.reserve(reallocated.size());
	if_fail_fe(reallocated.resize(capacity, T(8)));
	T * previousAddress = reallocated.begin();
	result = reallocated.insert(1, T(2));
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_INSERT_VALUE_REALLOCATE
		, result != (::llc::err_t)capacity + 1 || reallocated.size() != capacity + 1 || reallocated.begin() == previousAddress
		|| reallocated[0] != T(1) || reallocated[1] != T(2) || reallocated[2] != T(3)
		, "%s reallocating value insertion mismatch. result:%i, size:%u/%u, address:%p/%p."
		, ::llc::get_type_namep<T>(), result, reallocated.size(), capacity + 1, (void*)reallocated.begin(), (void*)previousAddress
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_TERMINATOR, podTerminatorMismatch(reallocated)
		, "%s reallocating value insertion terminator mismatch. size:%u, terminator:%" LLC_FMT_S3 "."
		, ::llc::get_type_namep<T>(), reallocated.size(), (::llc::s3_t)reallocated.begin()[reallocated.size()]
		);

	::llc::apod<T> chains = {T(4), T(8)};
	chains.reserve(32);
	T pointerValues[] = {T(1), T(2), T(3)};
	result = chains.insert(0, pointerValues, 3);
	T expectedPointer[] = {T(1), T(2), T(3), T(4), T(8)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_INSERT_POINTER, result != 5 || podMismatch(chains, expectedPointer)
		, "%s pointer insertion mismatch. result:%i, size:%u."
		, ::llc::get_type_namep<T>(), result, chains.size()
		);
	T arrayValues[] = {T(5), T(6)};
	result = chains.insert(4, arrayValues);
	T expectedArray[] = {T(1), T(2), T(3), T(4), T(5), T(6), T(8)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_INSERT_ARRAY, result != 7 || podMismatch(chains, expectedArray)
		, "%s array insertion mismatch. result:%i, size:%u."
		, ::llc::get_type_namep<T>(), result, chains.size()
		);
	T viewValues[] = {T(7)};
	result = chains.insert(chains.size() - 1, ::llc::view<cnst T>{viewValues});
	T expectedView[] = {T(1), T(2), T(3), T(4), T(5), T(6), T(7), T(8)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_INSERT_VIEW, result != 8 || podMismatch(chains, expectedView)
		, "%s view insertion mismatch. result:%i, size:%u."
		, ::llc::get_type_namep<T>(), result, chains.size()
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_TERMINATOR, podTerminatorMismatch(chains)
		, "%s chain insertion terminator mismatch. size:%u, terminator:%" LLC_FMT_S3 "."
		, ::llc::get_type_namep<T>(), chains.size(), (::llc::s3_t)chains.begin()[chains.size()]
		);

	::llc::apod<T> chainReallocated = {T(1), T(4)};
	cnst ::llc::u2_t chainCapacity = chainReallocated.reserve(chainReallocated.size());
	if_fail_fe(chainReallocated.resize(chainCapacity, T(8)));
	previousAddress = chainReallocated.begin();
	T inserted[] = {T(2), T(3)};
	result = chainReallocated.insert(1, inserted, 2);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_INSERT_CHAIN_REALLOCATE
		, result != (::llc::err_t)chainCapacity + 2 || chainReallocated.size() != chainCapacity + 2 || chainReallocated.begin() == previousAddress
		|| chainReallocated[0] != T(1) || chainReallocated[1] != T(2) || chainReallocated[2] != T(3) || chainReallocated[3] != T(4)
		, "%s reallocating chain insertion mismatch. result:%i, size:%u/%u, address:%p/%p."
		, ::llc::get_type_namep<T>(), result, chainReallocated.size(), chainCapacity + 2, (void*)chainReallocated.begin(), (void*)previousAddress
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_TERMINATOR, podTerminatorMismatch(chainReallocated)
		, "%s reallocating chain insertion terminator mismatch. size:%u, terminator:%" LLC_FMT_S3 "."
		, ::llc::get_type_namep<T>(), chainReallocated.size(), (::llc::s3_t)chainReallocated.begin()[chainReallocated.size()]
		);
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testPodRemove(ATestError & errors) {
	::llc::apod<T> ordered = {T(1), T(2), T(3), T(4), T(5)};
	::llc::err_t result = ordered.remove(0);
	T expectedFront[] = {T(2), T(3), T(4), T(5)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_REMOVE, result != 4 || podMismatch(ordered, expectedFront)
		, "%s ordered front removal mismatch. result:%i, size:%u."
		, ::llc::get_type_namep<T>(), result, ordered.size()
		);
	result = ordered.remove(1);
	T expectedMiddle[] = {T(2), T(4), T(5)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_REMOVE, result != 3 || podMismatch(ordered, expectedMiddle)
		, "%s ordered middle removal mismatch. result:%i, size:%u."
		, ::llc::get_type_namep<T>(), result, ordered.size()
		);
	result = ordered.remove(ordered.size() - 1);
	T expectedEnd[] = {T(2), T(4)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_REMOVE, result != 2 || podMismatch(ordered, expectedEnd)
		, "%s ordered end removal mismatch. result:%i, size:%u."
		, ::llc::get_type_namep<T>(), result, ordered.size()
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_TERMINATOR, podTerminatorMismatch(ordered)
		, "%s ordered removal terminator mismatch. size:%u, terminator:%" LLC_FMT_S3 "."
		, ::llc::get_type_namep<T>(), ordered.size(), (::llc::s3_t)ordered.begin()[ordered.size()]
		);

	::llc::apod<T> unordered = {T(1), T(2), T(3), T(4)};
	result = unordered.remove_unordered(1);
	T expectedUnordered[] = {T(1), T(4), T(3)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_REMOVE_UNORDERED, result != 3 || podMismatch(unordered, expectedUnordered)
		, "%s unordered middle removal mismatch. result:%i, size:%u."
		, ::llc::get_type_namep<T>(), result, unordered.size()
		);
	result = unordered.remove_unordered(unordered.size() - 1);
	T expectedUnorderedEnd[] = {T(1), T(4)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_REMOVE_UNORDERED, result != 2 || podMismatch(unordered, expectedUnorderedEnd)
		, "%s unordered end removal mismatch. result:%i, size:%u."
		, ::llc::get_type_namep<T>(), result, unordered.size()
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_TERMINATOR, podTerminatorMismatch(unordered)
		, "%s unordered removal terminator mismatch. size:%u, terminator:%" LLC_FMT_S3 "."
		, ::llc::get_type_namep<T>(), unordered.size(), (::llc::s3_t)unordered.begin()[unordered.size()]
		);

	::llc::apod<T> erased = {T(1), T(2), T(3), T(4)};
	result = erased.erase(&erased[1]);
	T expectedErased[] = {T(1), T(3), T(4)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_ERASE, result != 3 || podMismatch(erased, expectedErased)
		, "%s addressed removal mismatch. result:%i, size:%u."
		, ::llc::get_type_namep<T>(), result, erased.size()
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_TERMINATOR, podTerminatorMismatch(erased)
		, "%s addressed removal terminator mismatch. size:%u, terminator:%" LLC_FMT_S3 "."
		, ::llc::get_type_namep<T>(), erased.size(), (::llc::s3_t)erased.begin()[erased.size()]
		);
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testPodFailures(ATestError & errors) {
	::llc::apod<T> empty;
	T removed = T(7);
	::llc::err_t result = podExpectedFailure([&empty]() { rtrn empty.pop_back(); });
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_POP_BACK_EMPTY, false == ::llc::failed(result) || empty.size() || empty.begin()
		, "%s empty pop_back mismatch. result:%i, size:%u, begin:%p."
		, ::llc::get_type_namep<T>(), result, empty.size(), (void*)empty.begin()
		);
	result = podExpectedFailure([&empty, &removed]() { rtrn empty.pop_back(removed); });
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_POP_BACK_EMPTY, false == ::llc::failed(result) || removed != T(7) || empty.size() || empty.begin()
		, "%s empty pop_back(value) mismatch. result:%i, output:%" LLC_FMT_S3 ", size:%u."
		, ::llc::get_type_namep<T>(), result, (::llc::s3_t)removed, empty.size()
		);

	T expected[] = {T(1), T(2), T(3)};
	::llc::apod<T> values{expected};
	T * address = values.begin();
	result = podExpectedFailure([&values]() { rtrn values.insert(values.size() + 1, T(9)); });
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_INSERT_INVALID_INDEX, false == ::llc::failed(result) || values.begin() != address || podMismatch(values, expected)
		, "%s invalid insertion mismatch. result:%i, size:%u, address:%p/%p."
		, ::llc::get_type_namep<T>(), result, values.size(), (void*)values.begin(), (void*)address
		);
	result = podExpectedFailure([&values]() { rtrn values.remove(values.size()); });
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_REMOVE_INVALID_INDEX, false == ::llc::failed(result) || values.begin() != address || podMismatch(values, expected)
		, "%s invalid ordered removal mismatch. result:%i, size:%u."
		, ::llc::get_type_namep<T>(), result, values.size()
		);
	result = podExpectedFailure([&values]() { rtrn values.remove_unordered(values.size()); });
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_REMOVE_UNORDERED_INVALID, false == ::llc::failed(result) || values.begin() != address || podMismatch(values, expected)
		, "%s invalid unordered removal mismatch. result:%i, size:%u."
		, ::llc::get_type_namep<T>(), result, values.size()
		);
	result = podExpectedFailure([&values]() { rtrn values.erase(values.end()); });
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_ERASE_INVALID_POINTER, false == ::llc::failed(result) || values.begin() != address || podMismatch(values, expected)
		, "%s one-past erase mismatch. result:%i, size:%u."
		, ::llc::get_type_namep<T>(), result, values.size()
		);
	result = podExpectedFailure([&values]() { rtrn values.erase(0); });
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_ERASE_INVALID_POINTER, false == ::llc::failed(result) || values.begin() != address || podMismatch(values, expected)
		, "%s null erase mismatch. result:%i, size:%u."
		, ::llc::get_type_namep<T>(), result, values.size()
		);
	T unrelated = T(9);
	result = podExpectedFailure([&values, &unrelated]() { rtrn values.erase(&unrelated); });
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_ERASE_INVALID_POINTER, false == ::llc::failed(result) || values.begin() != address || podMismatch(values, expected)
		, "%s unrelated-pointer erase mismatch. result:%i, size:%u."
		, ::llc::get_type_namep<T>(), result, values.size()
		);
	if constexpr(szof(T) > 1) {
		cnst T * misaligned = (cnst T*)((cnst ::llc::u0_t*)values.begin() + 1);
		result = podExpectedFailure([&values, misaligned]() { rtrn values.erase(misaligned); });
	}
	else
		result = -1;
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_ERASE_INVALID_POINTER, false == ::llc::failed(result) || values.begin() != address || podMismatch(values, expected)
		, "%s misaligned-pointer erase mismatch. result:%i, size:%u, element bytes:%u."
		, ::llc::get_type_namep<T>(), result, values.size(), (unsigned)szof(T)
		);

	result = podExpectedFailure([&values]() { rtrn values.reserve(0x40000000U); });
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_RESERVE_INVALID_COUNT, false == ::llc::failed(result) || values.begin() != address || podMismatch(values, expected)
		, "%s oversized reserve mismatch. result:%i, size:%u, address:%p/%p."
		, ::llc::get_type_namep<T>(), result, values.size(), (void*)values.begin(), (void*)address
		);
	result = podExpectedFailure([&values]() { rtrn values.resize(0x40000000U); });
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_RESIZE_INVALID_COUNT, false == ::llc::failed(result) || values.begin() != address || podMismatch(values, expected)
		, "%s oversized resize mismatch. result:%i, size:%u, address:%p/%p."
		, ::llc::get_type_namep<T>(), result, values.size(), (void*)values.begin(), (void*)address
		);
	result = podExpectedFailure([&values]() { rtrn values.resize(0x40000000U, T(9)); });
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_RESIZE_INVALID_COUNT, false == ::llc::failed(result) || values.begin() != address || podMismatch(values, expected)
		, "%s oversized filled resize mismatch. result:%i, size:%u, address:%p/%p."
		, ::llc::get_type_namep<T>(), result, values.size(), (void*)values.begin(), (void*)address
		);

	result = podExpectedFailure([&values]() { rtrn values.append((cnst T*)0, 1); });
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_APPEND_NULL_SOURCE, false == ::llc::failed(result) || values.begin() != address || podMismatch(values, expected)
		, "%s null append mismatch. result:%i, size:%u."
		, ::llc::get_type_namep<T>(), result, values.size()
		);
	result = podExpectedFailure([&values]() { rtrn values.insert(1, (cnst T*)0, 1); });
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_INSERT_NULL_SOURCE, false == ::llc::failed(result) || values.begin() != address || podMismatch(values, expected)
		, "%s null insertion mismatch. result:%i, size:%u."
		, ::llc::get_type_namep<T>(), result, values.size()
		);
	result = values.insert(1, (cnst T*)0, 0);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_INSERT_EMPTY_SOURCE, result != 3 || values.begin() != address || podMismatch(values, expected)
		, "%s empty insertion mismatch. result:%i, size:%u, address:%p/%p."
		, ::llc::get_type_namep<T>(), result, values.size(), (void*)values.begin(), (void*)address
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_TERMINATOR, podTerminatorMismatch(values)
		, "%s failed-operation terminator mismatch. size:%u, terminator:%" LLC_FMT_S3 "."
		, ::llc::get_type_namep<T>(), values.size(), (::llc::s3_t)values.begin()[values.size()]
		);
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testPodAliasing(ATestError & errors) {
	::llc::apod<T> pushed = {T(1), T(2)};
	cnst ::llc::u2_t pushCapacity = pushed.reserve(pushed.size());
	if_fail_fe(pushed.resize(pushCapacity, T(8)));
	::llc::err_t result = pushed.push_back(pushed[1]);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_ALIAS_PUSH_BACK
		, result != (::llc::err_t)pushCapacity || pushed.size() != pushCapacity + 1 || pushed[pushed.size() - 1] != T(2)
		, "%s aliased push_back mismatch. result:%i, size:%u/%u, appended:%" LLC_FMT_S3 "."
		, ::llc::get_type_namep<T>(), result, pushed.size(), pushCapacity + 1, (::llc::s3_t)pushed[pushed.size() - 1]
		);

	::llc::apod<T> resized = {T(1), T(2)};
	cnst ::llc::u2_t resizeCapacity = resized.reserve(resized.size());
	if_fail_fe(resized.resize(resizeCapacity, T(8)));
	result = resized.resize(resizeCapacity + 2, resized[1]);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_ALIAS_RESIZE
		, result != (::llc::err_t)resizeCapacity + 2 || resized.size() != resizeCapacity + 2
		|| resized[resizeCapacity] != T(2) || resized[resizeCapacity + 1] != T(2)
		, "%s aliased filled resize mismatch. result:%i, size:%u/%u, appended:%" LLC_FMT_S3 ",%" LLC_FMT_S3 "."
		, ::llc::get_type_namep<T>(), result, resized.size(), resizeCapacity + 2
		, (::llc::s3_t)resized[resizeCapacity], (::llc::s3_t)resized[resizeCapacity + 1]
		);

	::llc::apod<T> appended = {T(1), T(2), T(3)};
	::llc::view<cnst T> appendedSource{appended.begin(), appended.size()};
	result = appended.append(appendedSource);
	T expectedAppend[] = {T(1), T(2), T(3), T(1), T(2), T(3)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_ALIAS_APPEND, result != 3 || podMismatch(appended, expectedAppend)
		, "%s aliased append mismatch. result:%i, size:%u, expected index:3/size:6."
		, ::llc::get_type_namep<T>(), result, appended.size()
		);

	::llc::apod<T> insertedValue = {T(1), T(2), T(3)};
	insertedValue.reserve(16);
	result = insertedValue.insert(0, insertedValue[1]);
	T expectedValue[] = {T(2), T(1), T(2), T(3)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_ALIAS_INSERT_VALUE, result != 4 || podMismatch(insertedValue, expectedValue)
		, "%s aliased value insertion mismatch. result:%i, size:%u."
		, ::llc::get_type_namep<T>(), result, insertedValue.size()
		);

	::llc::apod<T> insertedChain = {T(1), T(2), T(3), T(4)};
	insertedChain.reserve(16);
	::llc::view<cnst T> insertedSource{&insertedChain[1], 2};
	result = insertedChain.insert(0, insertedSource);
	T expectedChain[] = {T(2), T(3), T(1), T(2), T(3), T(4)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_ALIAS_INSERT_CHAIN, result != 6 || podMismatch(insertedChain, expectedChain)
		, "%s aliased chain insertion mismatch. result:%i, size:%u."
		, ::llc::get_type_namep<T>(), result, insertedChain.size()
		);

	::llc::apod<T> assigned = {T(1), T(2), T(3), T(4)};
	::llc::view<cnst T> assignedSource{&assigned[1], 3};
	assigned = assignedSource;
	T expectedAssignment[] = {T(2), T(3), T(4)};
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_ALIAS_ASSIGNMENT, podMismatch(assigned, expectedAssignment)
		, "%s aliased view assignment mismatch. size:%u, expected:3."
		, ::llc::get_type_namep<T>(), assigned.size()
		);

	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_TERMINATOR
		, podTerminatorMismatch(pushed) || podTerminatorMismatch(resized) || podTerminatorMismatch(appended)
		|| podTerminatorMismatch(insertedValue) || podTerminatorMismatch(insertedChain) || podTerminatorMismatch(assigned)
		, "%s aliased operation lost a zero-value terminator."
		, ::llc::get_type_namep<T>()
		);
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testPodClear(ATestError & errors) {
	::llc::apod<T> values = {T(1), T(2), T(3)};
	T * allocated = values.begin();
	cnst ::llc::err_t result = values.clear();
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_CLEAR, result || values.size() || values.begin() != allocated || values.end() != allocated
		, "%s clear mismatch. result:%i, range:%p..%p, retained:%p."
		, ::llc::get_type_namep<T>(), result, (void*)values.begin(), (void*)values.end(), (void*)allocated
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_TERMINATOR, podTerminatorMismatch(values)
		, "%s clear terminator mismatch. size:%u, terminator:%" LLC_FMT_S3 "."
		, ::llc::get_type_namep<T>(), values.size(), (::llc::s3_t)values.begin()[values.size()]
		);
	LLC_TEST_CHECK(errors, ARRAY_POD_TEST_RESULT_CLEAR_POINTER, values.clear_pointer() || values.size() || values.begin() || values.end()
		, "%s clear_pointer mismatch. size:%u, begin:%p, end:%p."
		, ::llc::get_type_namep<T>(), values.size(), (void*)values.begin(), (void*)values.end()
		);
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testPodType(ATestError & errors) {
	if_fail_fe(testPodConstruction<T>(errors));
	if_fail_fe(testPodAssignment<T>(errors));
	if_fail_fe(testPodReserveResize<T>(errors));
	if_fail_fe(testPodAppend<T>(errors));
	if_fail_fe(testPodPopBack<T>(errors));
	if_fail_fe(testPodInsert<T>(errors));
	if_fail_fe(testPodRemove<T>(errors));
	if_fail_fe(testPodFailures<T>(errors));
	if_fail_fe(testPodAliasing<T>(errors));
	rtrn testPodClear<T>(errors);
}

tplt<tpnm T>
sttc ::llc::err_t testPodTypeLogged(ATestError & errors) {
	cnst ::llc::u2_t checkCount = testCheckCount(errors);
	cnst ::llc::u2_t failureCount = testErrorCount(errors);
	if_fail_fe(testPodType<T>(errors));
	cnst ::llc::u2_t typeFailures = testErrorCount(errors) - failureCount;
	cnst ::llc::u2_t typeChecks = testCheckCount(errors) - checkCount;
	if(typeFailures) error_printf("%s suite completed: %u/%u checks passed, %u failed.", ::llc::get_type_namep<T>(), typeChecks - typeFailures, typeChecks, typeFailures);
	else always_printf("%s suite OK: %u checks passed.", ::llc::get_type_namep<T>(), typeChecks);
	rtrn 0;
}

::llc::err_t testArrayPod(ATestError & errors) {
	if_fail_fe(testPodTypeLogged<::llc::u0_t>(errors));
	if_fail_fe(testPodTypeLogged<::llc::u1_t>(errors));
	if_fail_fe(testPodTypeLogged<::llc::u2_t>(errors));
	if_fail_fe(testPodTypeLogged<::llc::u3_t>(errors));
	if_fail_fe(testPodTypeLogged<::llc::s0_t>(errors));
	if_fail_fe(testPodTypeLogged<::llc::s1_t>(errors));
	if_fail_fe(testPodTypeLogged<::llc::s2_t>(errors));
	if_fail_fe(testPodTypeLogged<::llc::s3_t>(errors));
	rtrn testPodStringAppend(errors);
}
