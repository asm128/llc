#include "llc_array_obj.h"

#include "llc_test_core.h"

#include <utility>

GDEFINE_ENUM_TYPE(ARRAY_OBJ_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(ARRAY_OBJ_TEST_RESULT, EMPLACE_INDEX	, 0, "array_obj<>::emplace_back() did not return the appended element index.");
GDEFINE_ENUM_VALUED(ARRAY_OBJ_TEST_RESULT, EMPLACE_VALUE	, 1, "array_obj<>::emplace_back() did not construct the requested value in place.");
GDEFINE_ENUM_VALUED(ARRAY_OBJ_TEST_RESULT, EMPLACE_DIRECT	, 2, "array_obj<>::emplace_back() copied a temporary instead of constructing from its arguments.");
GDEFINE_ENUM_VALUED(ARRAY_OBJ_TEST_RESULT, EMPLACE_GROW		, 3, "array_obj<>::emplace_back() did not preserve values while growing its allocation.");
GDEFINE_ENUM_VALUED(ARRAY_OBJ_TEST_RESULT, EMPLACE_LIFETIME	, 4, "array_obj<>::emplace_back() did not preserve object lifetime balance.");

GDEFINE_ENUM_VALUED(ARRAY_OBJ_TEST_RESULT, DEFAULT_STATE		, 5, "A default array_obj<> was not an empty null range.");
GDEFINE_ENUM_VALUED(ARRAY_OBJ_TEST_RESULT, CONSTRUCTION		, 6, "array_obj<> did not copy its source values during construction.");
GDEFINE_ENUM_VALUED(ARRAY_OBJ_TEST_RESULT, COPY_CONSTRUCTION	, 7, "array_obj<> copy construction did not produce independent equal storage.");
GDEFINE_ENUM_VALUED(ARRAY_OBJ_TEST_RESULT, MOVE_CONSTRUCTION	, 8, "array_obj<> move construction did not transfer its allocation and clear the source.");
GDEFINE_ENUM_VALUED(ARRAY_OBJ_TEST_RESULT, COPY_ASSIGNMENT	, 9, "array_obj<> copy assignment did not produce independent equal storage.");
GDEFINE_ENUM_VALUED(ARRAY_OBJ_TEST_RESULT, RESERVE			, 10, "array_obj<>::reserve() did not preserve its elements or stable allocation.");
GDEFINE_ENUM_VALUED(ARRAY_OBJ_TEST_RESULT, RESIZE			, 11, "array_obj<>::resize() did not preserve, construct or destroy the expected elements.");
GDEFINE_ENUM_VALUED(ARRAY_OBJ_TEST_RESULT, PUSH_BACK			, 12, "array_obj<>::push_back() did not append and return the inserted index.");
GDEFINE_ENUM_VALUED(ARRAY_OBJ_TEST_RESULT, APPEND			, 13, "array_obj<>::append() did not append its source range and return the first inserted index.");
GDEFINE_ENUM_VALUED(ARRAY_OBJ_TEST_RESULT, INSERT			, 14, "array_obj<>::insert() did not preserve order while inserting values or ranges.");
GDEFINE_ENUM_VALUED(ARRAY_OBJ_TEST_RESULT, POP_BACK			, 15, "array_obj<>::pop_back() did not remove or return the last element.");
GDEFINE_ENUM_VALUED(ARRAY_OBJ_TEST_RESULT, REMOVE			, 16, "array_obj<>::remove() did not erase an element while preserving order.");
GDEFINE_ENUM_VALUED(ARRAY_OBJ_TEST_RESULT, REMOVE_UNORDERED	, 17, "array_obj<>::remove_unordered() did not replace the removed element with the last one.");
GDEFINE_ENUM_VALUED(ARRAY_OBJ_TEST_RESULT, ERASE				, 18, "array_obj<>::erase() did not remove the addressed element.");
GDEFINE_ENUM_VALUED(ARRAY_OBJ_TEST_RESULT, CLEAR				, 19, "array_obj<>::clear() did not destroy its elements while retaining its allocation.");
GDEFINE_ENUM_VALUED(ARRAY_OBJ_TEST_RESULT, CLEAR_POINTER		, 20, "array_obj<>::clear_pointer() did not destroy its elements and release its allocation.");
GDEFINE_ENUM_VALUED(ARRAY_OBJ_TEST_RESULT, FIND_SEQUENCE		, 21, "array_obj<>::find() did not compare sequence elements by value.");
GDEFINE_ENUM_VALUED(ARRAY_OBJ_TEST_RESULT, RFIND_SEQUENCE		, 22, "array_obj<>::rfind() did not compare sequence elements by value.");

stct SArrayObjTestValue {
	sttc ::llc::u2_t	LiveCount;
	sttc ::llc::u2_t	ConstructorCount;
	sttc ::llc::u2_t	CopyCount;
	sttc ::llc::u2_t	DestructorCount;

	::llc::u2_t			Index	= {};
	::llc::u3_t			Value	= {};

	SArrayObjTestValue() { ++LiveCount; ++ConstructorCount; }
	SArrayObjTestValue(::llc::u2_t index) : SArrayObjTestValue(index, (::llc::u3_t)index * 10) {}
	SArrayObjTestValue(::llc::u2_t index, ::llc::u3_t value) : Index(index), Value(value) { ++LiveCount; ++ConstructorCount; }
	SArrayObjTestValue(cnst SArrayObjTestValue & other) : Index(other.Index), Value(other.Value) { ++LiveCount; ++CopyCount; }
	~SArrayObjTestValue() { --LiveCount; ++DestructorCount; }
	SArrayObjTestValue & operator=(cnst SArrayObjTestValue & other) { Index = other.Index; Value = other.Value; rtrn *this; }
};

::llc::u2_t SArrayObjTestValue::LiveCount			= {};
::llc::u2_t SArrayObjTestValue::ConstructorCount	= {};
::llc::u2_t SArrayObjTestValue::CopyCount			= {};
::llc::u2_t SArrayObjTestValue::DestructorCount		= {};

sttc ::llc::err_t arrayObjCheck(ATestError & errors, ARRAY_OBJ_TEST_RESULT result, cnst ::llc::aobj<SArrayObjTestValue> & actual, ::llc::view<cnst ::llc::u2_t> expected, ::llc::vcst_t operation) {
	LLC_TEST_CHECK(errors, result, actual.size() != expected.size()
		, "%.*s count:%u, expected:%u."
		, (int)operation.size(), operation.begin(), actual.size(), expected.size()
		);
	for(::llc::u2_t iValue = 0; iValue < actual.size() && iValue < expected.size(); ++iValue) {
		LLC_TEST_CHECK(errors, result, actual[iValue].Index != expected[iValue]
			, "%.*s element:%u index:%u, expected:%u."
			, (int)operation.size(), operation.begin(), iValue, actual[iValue].Index, expected[iValue]
			);
		LLC_TEST_CHECK(errors, result, actual[iValue].Value != (::llc::u3_t)expected[iValue] * 10
			, "%.*s element:%u value:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "."
			, (int)operation.size(), operation.begin(), iValue, actual[iValue].Value, (::llc::u3_t)expected[iValue] * 10
			);
	}
	rtrn 0;
}

sttc ::llc::err_t testArrayObjEmplace(ATestError & errors) {
	cnst ::llc::u2_t constructorsBefore	= SArrayObjTestValue::ConstructorCount;
	cnst ::llc::u2_t copiesBefore		= SArrayObjTestValue::CopyCount;
	cnst ::llc::u2_t liveBefore			= SArrayObjTestValue::LiveCount;
	{
		::llc::aobj<SArrayObjTestValue> values = {};
		if_fail_fe(values.reserve(16));
		cnst ::llc::err_t firstIndex	= values.emplace_back(::llc::u2_t{0}, ::llc::u3_t{0});
		cnst ::llc::err_t secondIndex	= values.emplace_back(::llc::u2_t{1}, ::llc::u3_t{10});
		LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_EMPLACE_INDEX, firstIndex
			, "First index:%i, expected:0."
			, firstIndex
			);
		LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_EMPLACE_INDEX, secondIndex != 1
			, "Second index:%i, expected:1."
			, secondIndex
			);
		LLC_TEST_REQUIRE(errors, ARRAY_OBJ_TEST_RESULT_EMPLACE_INDEX, values.size() != 2
			, "Count:%u, expected:2."
			, values.size()
			);
		LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_EMPLACE_DIRECT, SArrayObjTestValue::ConstructorCount - constructorsBefore != 2
			, "Direct constructions:%u, expected:2."
			, SArrayObjTestValue::ConstructorCount - constructorsBefore
			);
		LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_EMPLACE_DIRECT, SArrayObjTestValue::CopyCount != copiesBefore
			, "Copies:%u, expected:0."
			, SArrayObjTestValue::CopyCount - copiesBefore
			);
		LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_EMPLACE_DIRECT, SArrayObjTestValue::LiveCount - liveBefore != 2
			, "Live objects added:%u, expected:2."
			, SArrayObjTestValue::LiveCount - liveBefore
			);

		for(::llc::u2_t iValue = 2; iValue < 64; ++iValue) {
			cnst ::llc::err_t index = values.emplace_back(iValue, (::llc::u3_t)iValue * 10);
			LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_EMPLACE_INDEX, index != (::llc::err_t)iValue
				, "Value:%u returned index:%i, expected:%u."
				, iValue, index, iValue
				);
			LLC_TEST_REQUIRE(errors, ARRAY_OBJ_TEST_RESULT_EMPLACE_INDEX, values.size() != iValue + 1
				, "Value:%u left count:%u, expected:%u."
				, iValue, values.size(), iValue + 1
				);
		}
		for(::llc::u2_t iValue = 0; iValue < values.size(); ++iValue) {
			LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_EMPLACE_VALUE, values[iValue].Index != iValue
				, "Element:%u index:%u, expected:%u."
				, iValue, values[iValue].Index, iValue
				);
			LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_EMPLACE_VALUE, values[iValue].Value != (::llc::u3_t)iValue * 10
				, "Element:%u value:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "."
				, iValue, values[iValue].Value, (::llc::u3_t)iValue * 10
				);
		}
		LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_EMPLACE_GROW, SArrayObjTestValue::CopyCount == copiesBefore
			, "Growth made %u copies; expected at least one."
			, SArrayObjTestValue::CopyCount - copiesBefore
			);
		LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_EMPLACE_GROW, SArrayObjTestValue::LiveCount - liveBefore != values.size()
			, "Live objects added:%u, elements:%u."
			, SArrayObjTestValue::LiveCount - liveBefore, values.size()
			);
	}
	rtrn 0;
}

sttc ::llc::err_t testArrayObjConstruction(ATestError & errors) {
	::llc::aobj<SArrayObjTestValue> empty = {};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_DEFAULT_STATE, empty.size()
		, "Default count:%u, expected:0."
		, empty.size()
		);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_DEFAULT_STATE, empty.begin()
		, "Default begin:%p, expected:null."
		, empty.begin()
		);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_DEFAULT_STATE, empty.end()
		, "Default end:%p, expected:null."
		, empty.end()
		);

	SArrayObjTestValue source[] = {SArrayObjTestValue{1}, SArrayObjTestValue{2}, SArrayObjTestValue{3}};
	cnst ::llc::u2_t expected[] = {1, 2, 3};
	::llc::aobj<SArrayObjTestValue> fromArray{source};
	::llc::view<cnst SArrayObjTestValue> sourceView = {source};
	::llc::aobj<SArrayObjTestValue> fromView{sourceView};
	if_fail_fe(arrayObjCheck(errors, ARRAY_OBJ_TEST_RESULT_CONSTRUCTION, fromArray, {expected}, LLC_CXS("Array construction")));
	if_fail_fe(arrayObjCheck(errors, ARRAY_OBJ_TEST_RESULT_CONSTRUCTION, fromView, {expected}, LLC_CXS("View construction")));

	::llc::aobj<SArrayObjTestValue> copied{fromArray};
	if_fail_fe(arrayObjCheck(errors, ARRAY_OBJ_TEST_RESULT_COPY_CONSTRUCTION, copied, {expected}, LLC_CXS("Copy construction")));
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_COPY_CONSTRUCTION, copied.begin() == fromArray.begin()
		, "Copy aliases source. source:%p, copy:%p."
		, fromArray.begin(), copied.begin()
		);
	cnst ::llc::view<SArrayObjTestValue> movedStorage = copied;
	::llc::aobj<SArrayObjTestValue> moved{::std::move(copied)};
	if_fail_fe(arrayObjCheck(errors, ARRAY_OBJ_TEST_RESULT_MOVE_CONSTRUCTION, moved, {expected}, LLC_CXS("Move construction")));
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_MOVE_CONSTRUCTION, moved.begin() != movedStorage.begin()
		, "Move did not transfer storage. moved:%p, source storage:%p."
		, moved.begin(), movedStorage.begin()
		);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_MOVE_CONSTRUCTION, copied.size()
		, "Move source count:%u, expected:0."
		, copied.size()
		);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_MOVE_CONSTRUCTION, copied.begin()
		, "Move source begin:%p, expected:null."
		, copied.begin()
		);

	::llc::aobj<SArrayObjTestValue> assigned = {SArrayObjTestValue{9}};
	assigned = fromArray;
	if_fail_fe(arrayObjCheck(errors, ARRAY_OBJ_TEST_RESULT_COPY_ASSIGNMENT, assigned, {expected}, LLC_CXS("Copy assignment")));
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_COPY_ASSIGNMENT, assigned.begin() == fromArray.begin()
		, "Assignment aliases source. source:%p, target:%p."
		, fromArray.begin(), assigned.begin()
		);
	assigned = assigned;
	if_fail_fe(arrayObjCheck(errors, ARRAY_OBJ_TEST_RESULT_COPY_ASSIGNMENT, assigned, {expected}, LLC_CXS("Self-assignment")));
	rtrn 0;
}

sttc ::llc::err_t testArrayObjReserveResize(ATestError & errors) {
	SArrayObjTestValue source[] = {SArrayObjTestValue{1}, SArrayObjTestValue{2}};
	::llc::aobj<SArrayObjTestValue> values{source};
	cnst ::llc::u2_t expectedInitial[] = {1, 2};
	cnst ::llc::err_t capacity = values.reserve(32);
	LLC_TEST_REQUIRE(errors, ARRAY_OBJ_TEST_RESULT_RESERVE, capacity < 32
		, "Reserve capacity:%i, expected at least:32."
		, capacity
		);
	if_fail_fe(arrayObjCheck(errors, ARRAY_OBJ_TEST_RESULT_RESERVE, values, {expectedInitial}, LLC_CXS("Reserve")));
	cnst ::llc::view<SArrayObjTestValue> reservedStorage = values;
	cnst ::llc::err_t stableCapacity = values.reserve(16);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_RESERVE, stableCapacity != capacity
		, "Stable reserve capacity:%i, expected:%i."
		, stableCapacity, capacity
		);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_RESERVE, values.begin() != reservedStorage.begin()
		, "Stable reserve moved storage. begin:%p, expected:%p."
		, values.begin(), reservedStorage.begin()
		);

	SArrayObjTestValue fill{9};
	::llc::err_t result = values.resize(4, fill);
	cnst ::llc::u2_t expectedFill[] = {1, 2, 9, 9};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_RESIZE, result != 4
		, "Filled resize result:%i, expected:4."
		, result
		);
	if_fail_fe(arrayObjCheck(errors, ARRAY_OBJ_TEST_RESULT_RESIZE, values, {expectedFill}, LLC_CXS("Filled resize")));
	result = values.resize(6, ::llc::u2_t{7}, ::llc::u3_t{70});
	cnst ::llc::u2_t expectedVariadic[] = {1, 2, 9, 9, 7, 7};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_RESIZE, result != 6
		, "Variadic resize result:%i, expected:6."
		, result
		);
	if_fail_fe(arrayObjCheck(errors, ARRAY_OBJ_TEST_RESULT_RESIZE, values, {expectedVariadic}, LLC_CXS("Variadic resize")));
	result = values.resize(3);
	cnst ::llc::u2_t expectedShrink[] = {1, 2, 9};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_RESIZE, result != 3
		, "Shrink result:%i, expected:3."
		, result
		);
	if_fail_fe(arrayObjCheck(errors, ARRAY_OBJ_TEST_RESULT_RESIZE, values, {expectedShrink}, LLC_CXS("Shrink resize")));
	result = values.resize(5);
	cnst ::llc::u2_t expectedDefault[] = {1, 2, 9, 0, 0};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_RESIZE, result != 5
		, "Default resize result:%i, expected:5."
		, result
		);
	if_fail_fe(arrayObjCheck(errors, ARRAY_OBJ_TEST_RESULT_RESIZE, values, {expectedDefault}, LLC_CXS("Default resize")));
	rtrn 0;
}

sttc ::llc::err_t testArrayObjAppendInsert(ATestError & errors) {
	::llc::aobj<SArrayObjTestValue> values = {SArrayObjTestValue{1}, SArrayObjTestValue{2}};
	SArrayObjTestValue third{3};
	::llc::err_t result = values.push_back(third);
	cnst ::llc::u2_t expectedPush[] = {1, 2, 3};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_PUSH_BACK, result != 2
		, "Push result:%i, expected index:2."
		, result
		);
	if_fail_fe(arrayObjCheck(errors, ARRAY_OBJ_TEST_RESULT_PUSH_BACK, values, {expectedPush}, LLC_CXS("Push back")));

	SArrayObjTestValue arrayTail[] = {SArrayObjTestValue{4}, SArrayObjTestValue{5}};
	result = values.append(arrayTail);
	cnst ::llc::u2_t expectedArray[] = {1, 2, 3, 4, 5};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_APPEND, result != 3
		, "Array append result:%i, expected index:3."
		, result
		);
	if_fail_fe(arrayObjCheck(errors, ARRAY_OBJ_TEST_RESULT_APPEND, values, {expectedArray}, LLC_CXS("Array append")));
	SArrayObjTestValue viewTail[] = {SArrayObjTestValue{6}, SArrayObjTestValue{7}};
	result = values.append(::llc::view<cnst SArrayObjTestValue>{viewTail});
	cnst ::llc::u2_t expectedView[] = {1, 2, 3, 4, 5, 6, 7};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_APPEND, result != 5
		, "View append result:%i, expected index:5."
		, result
		);
	if_fail_fe(arrayObjCheck(errors, ARRAY_OBJ_TEST_RESULT_APPEND, values, {expectedView}, LLC_CXS("View append")));
	result = values.append(::llc::view<cnst SArrayObjTestValue>{});
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_APPEND, result != 7
		, "Empty append result:%i, expected index:7."
		, result
		);
	if_fail_fe(arrayObjCheck(errors, ARRAY_OBJ_TEST_RESULT_APPEND, values, {expectedView}, LLC_CXS("Empty append")));

	::llc::aobj<SArrayObjTestValue> inserted = {SArrayObjTestValue{2}, SArrayObjTestValue{5}};
	if_fail_fe(inserted.reserve(32));
	SArrayObjTestValue one{1};
	cnst ::llc::err_t insertFront = inserted.insert(0, one);
	SArrayObjTestValue middle[] = {SArrayObjTestValue{3}, SArrayObjTestValue{4}};
	cnst ::llc::err_t insertMiddle = inserted.insert(2, ::llc::view<cnst SArrayObjTestValue>{middle});
	SArrayObjTestValue six{6};
	cnst ::llc::err_t insertEnd = inserted.insert(inserted.size(), six);
	cnst ::llc::u2_t expectedInsert[] = {1, 2, 3, 4, 5, 6};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_INSERT, insertFront != 3
		, "Front insertion result:%i, expected:3."
		, insertFront
		);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_INSERT, insertMiddle != 5
		, "Middle insertion result:%i, expected:5."
		, insertMiddle
		);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_INSERT, insertEnd != 6
		, "End insertion result:%i, expected:6."
		, insertEnd
		);
	if_fail_fe(arrayObjCheck(errors, ARRAY_OBJ_TEST_RESULT_INSERT, inserted, {expectedInsert}, LLC_CXS("Insert")));

	::llc::aobj<SArrayObjTestValue> reallocated = {SArrayObjTestValue{1}, SArrayObjTestValue{3}};
	cnst ::llc::err_t reallocatedCapacity = reallocated.reserve(reallocated.size());
	LLC_TEST_REQUIRE(errors, ARRAY_OBJ_TEST_RESULT_RESERVE, reallocatedCapacity < 2
		, "Reallocation setup capacity:%i, expected at least:2."
		, reallocatedCapacity
		);
	SArrayObjTestValue padding{8};
	cnst ::llc::err_t fillResult = reallocated.resize(reallocatedCapacity, padding);
	LLC_TEST_REQUIRE(errors, ARRAY_OBJ_TEST_RESULT_RESIZE, fillResult != reallocatedCapacity
		, "Reallocation setup resize result:%i, expected:%i."
		, fillResult, reallocatedCapacity
		);
	LLC_TEST_REQUIRE(errors, ARRAY_OBJ_TEST_RESULT_RESIZE, reallocated.size() != (::llc::u2_t)reallocatedCapacity
		, "Reallocation setup count:%u, expected:%i."
		, reallocated.size(), reallocatedCapacity
		);
	SArrayObjTestValue two{2};
	result = reallocated.insert(1, two);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_INSERT, result != reallocatedCapacity + 1
		, "Reallocating insertion result:%i, expected:%i."
		, result, reallocatedCapacity + 1
		);
	LLC_TEST_REQUIRE(errors, ARRAY_OBJ_TEST_RESULT_INSERT, reallocated.size() != (::llc::u2_t)reallocatedCapacity + 1
		, "Reallocating insertion count:%u, expected:%u."
		, reallocated.size(), (::llc::u2_t)reallocatedCapacity + 1
		);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_INSERT, reallocated[0].Index != 1
		, "Reallocating insertion first index:%u, expected:1."
		, reallocated[0].Index
		);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_INSERT, reallocated[1].Index != 2
		, "Reallocating insertion second index:%u, expected:2."
		, reallocated[1].Index
		);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_INSERT, reallocated[2].Index != 3
		, "Reallocating insertion third index:%u, expected:3."
		, reallocated[2].Index
		);
	rtrn 0;
}

sttc ::llc::err_t testArrayObjRemoveClear(ATestError & errors) {
	::llc::aobj<SArrayObjTestValue> values = {SArrayObjTestValue{1}, SArrayObjTestValue{2}, SArrayObjTestValue{3}, SArrayObjTestValue{4}, SArrayObjTestValue{5}};
	SArrayObjTestValue removed = {};
	::llc::err_t result = values.pop_back(removed);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_POP_BACK, result != 4
		, "Value pop result:%i, expected:4."
		, result
		);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_POP_BACK, removed.Index != 5
		, "Value pop removed index:%u, expected:5."
		, removed.Index
		);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_POP_BACK, removed.Value != 50
		, "Value pop removed value:%" LLC_FMT_U3 ", expected:50."
		, removed.Value
		);
	result = values.pop_back();
	cnst ::llc::u2_t expectedPop[] = {1, 2, 3};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_POP_BACK, result != 3
		, "Pop result:%i, expected:3."
		, result
		);
	if_fail_fe(arrayObjCheck(errors, ARRAY_OBJ_TEST_RESULT_POP_BACK, values, {expectedPop}, LLC_CXS("Pop back")));
	result = values.remove(1);
	cnst ::llc::u2_t expectedRemove[] = {1, 3};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_REMOVE, result != 2
		, "Ordered removal result:%i, expected:2."
		, result
		);
	if_fail_fe(arrayObjCheck(errors, ARRAY_OBJ_TEST_RESULT_REMOVE, values, {expectedRemove}, LLC_CXS("Ordered removal")));

	::llc::aobj<SArrayObjTestValue> unordered = {SArrayObjTestValue{1}, SArrayObjTestValue{2}, SArrayObjTestValue{3}, SArrayObjTestValue{4}};
	result = unordered.remove_unordered(1);
	cnst ::llc::u2_t expectedUnordered[] = {1, 4, 3};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_REMOVE_UNORDERED, result != 3
		, "Unordered removal result:%i, expected:3."
		, result
		);
	if_fail_fe(arrayObjCheck(errors, ARRAY_OBJ_TEST_RESULT_REMOVE_UNORDERED, unordered, {expectedUnordered}, LLC_CXS("Unordered removal")));

	::llc::aobj<SArrayObjTestValue> erased = {SArrayObjTestValue{1}, SArrayObjTestValue{2}, SArrayObjTestValue{3}};
	result = erased.erase(&erased[1]);
	cnst ::llc::u2_t expectedErase[] = {1, 3};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_ERASE, result != 2
		, "Erase result:%i, expected:2."
		, result
		);
	if_fail_fe(arrayObjCheck(errors, ARRAY_OBJ_TEST_RESULT_ERASE, erased, {expectedErase}, LLC_CXS("Erase")));

	::llc::aobj<SArrayObjTestValue> cleared = {SArrayObjTestValue{1}, SArrayObjTestValue{2}, SArrayObjTestValue{3}};
	cnst ::llc::view<SArrayObjTestValue> allocatedStorage = cleared;
	result = cleared.clear();
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_CLEAR, result
		, "Clear result:%i, expected:0."
		, result
		);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_CLEAR, cleared.size()
		, "Clear count:%u, expected:0."
		, cleared.size()
		);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_CLEAR, cleared.begin() != allocatedStorage.begin()
		, "Clear moved storage. begin:%p, expected:%p."
		, cleared.begin(), allocatedStorage.begin()
		);
	if_fail_fe(cleared.emplace_back(::llc::u2_t{9}));
	result = cleared.clear_pointer();
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_CLEAR_POINTER, result
		, "Clear-pointer result:%i, expected:0."
		, result
		);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_CLEAR_POINTER, cleared.size()
		, "Clear-pointer count:%u, expected:0."
		, cleared.size()
		);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_CLEAR_POINTER, cleared.begin()
		, "Clear-pointer begin:%p, expected:null."
		, cleared.begin()
		);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_CLEAR_POINTER, cleared.end()
		, "Clear-pointer end:%p, expected:null."
		, cleared.end()
		);
	rtrn 0;
}

sttc ::llc::err_t testArrayObjFind(ATestError & errors) {
	::llc::sc_t targetA0[] = {'a'};
	::llc::sc_t targetB0[] = {'b'};
	::llc::sc_t targetA1[] = {'a'};
	::llc::sc_t targetB1[] = {'b'};
	cnst ::llc::vcst_t targetData[] = {{targetA0}, {targetB0}, {targetA1}, {targetB1}};
	cnst ::llc::aobj<::llc::vcst_t> target{targetData};

	::llc::sc_t sequenceA[] = {'a'};
	::llc::sc_t sequenceB[] = {'b'};
	cnst ::llc::vcst_t sequenceData[] = {{sequenceA}, {sequenceB}};
	cnst ::llc::view<cnst ::llc::vcst_t> sequence{sequenceData};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_FIND_SEQUENCE, target.find(sequence)
		, "Semantic sequence find result:%i, expected:0."
		, target.find(sequence)
		);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_FIND_SEQUENCE, target.find(sequence, 1) != 2
		, "Semantic sequence find after offset 1:%i, expected:2."
		, target.find(sequence, 1)
		);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_RFIND_SEQUENCE, target.rfind(sequence) != 2
		, "Semantic sequence rfind result:%i, expected:2."
		, target.rfind(sequence)
		);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_RFIND_SEQUENCE, target.rfind(sequence, 1)
		, "Semantic sequence rfind through offset 1:%i, expected:0."
		, target.rfind(sequence, 1)
		);
	rtrn 0;
}

::llc::err_t testArrayObj(ATestError & errors) {
	SArrayObjTestValue::LiveCount			= {};
	SArrayObjTestValue::ConstructorCount	= {};
	SArrayObjTestValue::CopyCount			= {};
	SArrayObjTestValue::DestructorCount		= {};
	if_fail_fe(::testArrayObjEmplace(errors));
	if_fail_fe(::testArrayObjConstruction(errors));
	if_fail_fe(::testArrayObjReserveResize(errors));
	if_fail_fe(::testArrayObjAppendInsert(errors));
	if_fail_fe(::testArrayObjRemoveClear(errors));
	if_fail_fe(::testArrayObjFind(errors));
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_EMPLACE_LIFETIME, SArrayObjTestValue::LiveCount
		, "Live objects after suite:%u, expected:0."
		, SArrayObjTestValue::LiveCount
		);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_EMPLACE_LIFETIME, SArrayObjTestValue::DestructorCount != SArrayObjTestValue::ConstructorCount + SArrayObjTestValue::CopyCount
		, "Destructors:%u, constructors:%u, copies:%u."
		, SArrayObjTestValue::DestructorCount, SArrayObjTestValue::ConstructorCount, SArrayObjTestValue::CopyCount
		);
	rtrn 0;
}
