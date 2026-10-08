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

sttc bool arrayObjMismatch(cnst ::llc::aobj<SArrayObjTestValue> & actual, ::llc::view<cnst ::llc::u2_t> expected) {
	if(actual.size() != expected.size())
		rtrn true;
	for(::llc::u2_t iValue = 0; iValue < actual.size(); ++iValue)
		if(actual[iValue].Index != expected[iValue] || actual[iValue].Value != (::llc::u3_t)expected[iValue] * 10)
			rtrn true;
	rtrn false;
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
		LLC_TEST_REQUIRE(errors, ARRAY_OBJ_TEST_RESULT_EMPLACE_INDEX, firstIndex || secondIndex != 1 || values.size() != 2
			, "First index:%i, second index:%i, count:%u."
			, firstIndex, secondIndex, values.size()
			);
		LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_EMPLACE_DIRECT
			, SArrayObjTestValue::ConstructorCount - constructorsBefore != 2 || SArrayObjTestValue::CopyCount != copiesBefore || SArrayObjTestValue::LiveCount - liveBefore != 2
			, "Direct constructions:%u, copies:%u, live objects:%u."
			, SArrayObjTestValue::ConstructorCount - constructorsBefore, SArrayObjTestValue::CopyCount - copiesBefore, SArrayObjTestValue::LiveCount - liveBefore
			);

		for(::llc::u2_t iValue = 2; iValue < 64; ++iValue) {
			cnst ::llc::err_t index = values.emplace_back(iValue, (::llc::u3_t)iValue * 10);
			LLC_TEST_REQUIRE(errors, ARRAY_OBJ_TEST_RESULT_EMPLACE_INDEX, index != (::llc::err_t)iValue || values.size() != iValue + 1
				, "Value:%u returned index:%i, count:%u."
				, iValue, index, values.size()
				);
		}
		for(::llc::u2_t iValue = 0; iValue < values.size(); ++iValue)
			LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_EMPLACE_VALUE, values[iValue].Index != iValue || values[iValue].Value != (::llc::u3_t)iValue * 10
				, "Value:%u produced index:%u, payload:%" LLC_FMT_U3 "."
				, iValue, values[iValue].Index, values[iValue].Value
				);
		LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_EMPLACE_GROW, SArrayObjTestValue::CopyCount == copiesBefore || SArrayObjTestValue::LiveCount - liveBefore != values.size()
			, "Copies:%u, live objects:%u, elements:%u."
			, SArrayObjTestValue::CopyCount - copiesBefore, SArrayObjTestValue::LiveCount - liveBefore, values.size()
			);
	}
	rtrn 0;
}

sttc ::llc::err_t testArrayObjConstruction(ATestError & errors) {
	::llc::aobj<SArrayObjTestValue> empty = {};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_DEFAULT_STATE, empty.size() || empty.begin() || empty.end()
		, "Default state size:%u, begin:%p, end:%p."
		, empty.size(), empty.begin(), empty.end()
		);

	SArrayObjTestValue source[] = {SArrayObjTestValue{1}, SArrayObjTestValue{2}, SArrayObjTestValue{3}};
	cnst ::llc::u2_t expected[] = {1, 2, 3};
	::llc::aobj<SArrayObjTestValue> fromArray{source};
	::llc::view<cnst SArrayObjTestValue> sourceView = {source};
	::llc::aobj<SArrayObjTestValue> fromView{sourceView};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_CONSTRUCTION, arrayObjMismatch(fromArray, {expected}) || arrayObjMismatch(fromView, {expected})
		, "Array/view construction sizes:%u/%u, expected:3/3."
		, fromArray.size(), fromView.size()
		);

	::llc::aobj<SArrayObjTestValue> copied{fromArray};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_COPY_CONSTRUCTION, arrayObjMismatch(copied, {expected}) || copied.begin() == fromArray.begin()
		, "Copy source:%p/%u, copy:%p/%u."
		, fromArray.begin(), fromArray.size(), copied.begin(), copied.size()
		);
	cnst ::llc::view<SArrayObjTestValue> movedStorage = copied;
	::llc::aobj<SArrayObjTestValue> moved{::std::move(copied)};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_MOVE_CONSTRUCTION
		, arrayObjMismatch(moved, {expected}) || moved.begin() != movedStorage.begin() || copied.size() || copied.begin()
		, "Moved:%p/%u, expected storage:%p, source:%p/%u."
		, moved.begin(), moved.size(), movedStorage.begin(), copied.begin(), copied.size()
		);

	::llc::aobj<SArrayObjTestValue> assigned = {SArrayObjTestValue{9}};
	assigned = fromArray;
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_COPY_ASSIGNMENT, arrayObjMismatch(assigned, {expected}) || assigned.begin() == fromArray.begin()
		, "Assignment source:%p/%u, target:%p/%u."
		, fromArray.begin(), fromArray.size(), assigned.begin(), assigned.size()
		);
	assigned = assigned;
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_COPY_ASSIGNMENT, arrayObjMismatch(assigned, {expected})
		, "Self-assignment changed %u elements."
		, assigned.size()
		);
	rtrn 0;
}

sttc ::llc::err_t testArrayObjReserveResize(ATestError & errors) {
	SArrayObjTestValue source[] = {SArrayObjTestValue{1}, SArrayObjTestValue{2}};
	::llc::aobj<SArrayObjTestValue> values{source};
	cnst ::llc::u2_t expectedInitial[] = {1, 2};
	cnst ::llc::err_t capacity = values.reserve(32);
	LLC_TEST_REQUIRE(errors, ARRAY_OBJ_TEST_RESULT_RESERVE, capacity < 32 || arrayObjMismatch(values, {expectedInitial})
		, "Reserve capacity:%i, size:%u, expected capacity at least 32 and size 2."
		, capacity, values.size()
		);
	cnst ::llc::view<SArrayObjTestValue> reservedStorage = values;
	cnst ::llc::err_t stableCapacity = values.reserve(16);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_RESERVE, stableCapacity != capacity || values.begin() != reservedStorage.begin()
		, "Stable reserve capacity:%i/%i, storage:%p/%p."
		, stableCapacity, capacity, values.begin(), reservedStorage.begin()
		);

	SArrayObjTestValue fill{9};
	::llc::err_t result = values.resize(4, fill);
	cnst ::llc::u2_t expectedFill[] = {1, 2, 9, 9};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_RESIZE, result != 4 || arrayObjMismatch(values, {expectedFill})
		, "Filled resize result:%i, size:%u, expected:4."
		, result, values.size()
		);
	result = values.resize(6, ::llc::u2_t{7}, ::llc::u3_t{70});
	cnst ::llc::u2_t expectedVariadic[] = {1, 2, 9, 9, 7, 7};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_RESIZE, result != 6 || arrayObjMismatch(values, {expectedVariadic})
		, "Variadic resize result:%i, size:%u, expected:6."
		, result, values.size()
		);
	result = values.resize(3);
	cnst ::llc::u2_t expectedShrink[] = {1, 2, 9};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_RESIZE, result != 3 || arrayObjMismatch(values, {expectedShrink})
		, "Shrink result:%i, size:%u, expected:3."
		, result, values.size()
		);
	result = values.resize(5);
	cnst ::llc::u2_t expectedDefault[] = {1, 2, 9, 0, 0};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_RESIZE, result != 5 || arrayObjMismatch(values, {expectedDefault})
		, "Default resize result:%i, size:%u, expected:5."
		, result, values.size()
		);
	rtrn 0;
}

sttc ::llc::err_t testArrayObjAppendInsert(ATestError & errors) {
	::llc::aobj<SArrayObjTestValue> values = {SArrayObjTestValue{1}, SArrayObjTestValue{2}};
	SArrayObjTestValue third{3};
	::llc::err_t result = values.push_back(third);
	cnst ::llc::u2_t expectedPush[] = {1, 2, 3};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_PUSH_BACK, result != 2 || arrayObjMismatch(values, {expectedPush})
		, "Push result:%i, size:%u, expected index 2 and size 3."
		, result, values.size()
		);

	SArrayObjTestValue arrayTail[] = {SArrayObjTestValue{4}, SArrayObjTestValue{5}};
	result = values.append(arrayTail);
	cnst ::llc::u2_t expectedArray[] = {1, 2, 3, 4, 5};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_APPEND, result != 3 || arrayObjMismatch(values, {expectedArray})
		, "Array append result:%i, size:%u, expected index 3 and size 5."
		, result, values.size()
		);
	SArrayObjTestValue viewTail[] = {SArrayObjTestValue{6}, SArrayObjTestValue{7}};
	result = values.append(::llc::view<cnst SArrayObjTestValue>{viewTail});
	cnst ::llc::u2_t expectedView[] = {1, 2, 3, 4, 5, 6, 7};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_APPEND, result != 5 || arrayObjMismatch(values, {expectedView})
		, "View append result:%i, size:%u, expected index 5 and size 7."
		, result, values.size()
		);
	result = values.append(::llc::view<cnst SArrayObjTestValue>{});
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_APPEND, result != 7 || arrayObjMismatch(values, {expectedView})
		, "Empty append result:%i, size:%u, expected 7/7."
		, result, values.size()
		);

	::llc::aobj<SArrayObjTestValue> inserted = {SArrayObjTestValue{2}, SArrayObjTestValue{5}};
	if_fail_fe(inserted.reserve(32));
	SArrayObjTestValue one{1};
	cnst ::llc::err_t insertFront = inserted.insert(0, one);
	SArrayObjTestValue middle[] = {SArrayObjTestValue{3}, SArrayObjTestValue{4}};
	cnst ::llc::err_t insertMiddle = inserted.insert(2, ::llc::view<cnst SArrayObjTestValue>{middle});
	SArrayObjTestValue six{6};
	cnst ::llc::err_t insertEnd = inserted.insert(inserted.size(), six);
	cnst ::llc::u2_t expectedInsert[] = {1, 2, 3, 4, 5, 6};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_INSERT, insertFront != 3 || insertMiddle != 5 || insertEnd != 6 || arrayObjMismatch(inserted, {expectedInsert})
		, "Insertion results:%i,%i,%i, size:%u; expected:3,5,6/6."
		, insertFront, insertMiddle, insertEnd, inserted.size()
		);

	::llc::aobj<SArrayObjTestValue> reallocated = {SArrayObjTestValue{1}, SArrayObjTestValue{3}};
	cnst ::llc::u2_t reallocatedCapacity = reallocated.reserve(reallocated.size());
	SArrayObjTestValue padding{8};
	if_fail_fe(reallocated.resize(reallocatedCapacity, padding));
	SArrayObjTestValue two{2};
	result = reallocated.insert(1, two);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_INSERT
		, result != (::llc::err_t)reallocatedCapacity + 1 || reallocated.size() != reallocatedCapacity + 1 || reallocated[0].Index != 1 || reallocated[1].Index != 2 || reallocated[2].Index != 3
		, "Reallocating insertion result:%i, size:%u/%u, first values:%u,%u,%u."
		, result, reallocated.size(), reallocatedCapacity + 1, reallocated[0].Index, reallocated[1].Index, reallocated[2].Index
		);
	rtrn 0;
}

sttc ::llc::err_t testArrayObjRemoveClear(ATestError & errors) {
	::llc::aobj<SArrayObjTestValue> values = {SArrayObjTestValue{1}, SArrayObjTestValue{2}, SArrayObjTestValue{3}, SArrayObjTestValue{4}, SArrayObjTestValue{5}};
	SArrayObjTestValue removed = {};
	::llc::err_t result = values.pop_back(removed);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_POP_BACK, result != 4 || removed.Index != 5
		, "Value pop result:%i, removed:%u, size:%u."
		, result, removed.Index, values.size()
		);
	result = values.pop_back();
	cnst ::llc::u2_t expectedPop[] = {1, 2, 3};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_POP_BACK, result != 3 || arrayObjMismatch(values, {expectedPop})
		, "Pop result:%i, size:%u, expected:3/3."
		, result, values.size()
		);
	result = values.remove(1);
	cnst ::llc::u2_t expectedRemove[] = {1, 3};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_REMOVE, result != 2 || arrayObjMismatch(values, {expectedRemove})
		, "Ordered removal result:%i, size:%u, expected:2/2."
		, result, values.size()
		);

	::llc::aobj<SArrayObjTestValue> unordered = {SArrayObjTestValue{1}, SArrayObjTestValue{2}, SArrayObjTestValue{3}, SArrayObjTestValue{4}};
	result = unordered.remove_unordered(1);
	cnst ::llc::u2_t expectedUnordered[] = {1, 4, 3};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_REMOVE_UNORDERED, result != 3 || arrayObjMismatch(unordered, {expectedUnordered})
		, "Unordered removal result:%i, size:%u, expected:3/3."
		, result, unordered.size()
		);

	::llc::aobj<SArrayObjTestValue> erased = {SArrayObjTestValue{1}, SArrayObjTestValue{2}, SArrayObjTestValue{3}};
	result = erased.erase(&erased[1]);
	cnst ::llc::u2_t expectedErase[] = {1, 3};
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_ERASE, result != 2 || arrayObjMismatch(erased, {expectedErase})
		, "Erase result:%i, size:%u, expected:2/2."
		, result, erased.size()
		);

	::llc::aobj<SArrayObjTestValue> cleared = {SArrayObjTestValue{1}, SArrayObjTestValue{2}, SArrayObjTestValue{3}};
	cnst ::llc::view<SArrayObjTestValue> allocatedStorage = cleared;
	result = cleared.clear();
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_CLEAR, result || cleared.size() || cleared.begin() != allocatedStorage.begin()
		, "Clear result:%i, size:%u, storage:%p/%p."
		, result, cleared.size(), cleared.begin(), allocatedStorage.begin()
		);
	if_fail_fe(cleared.emplace_back(::llc::u2_t{9}));
	result = cleared.clear_pointer();
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_CLEAR_POINTER, result || cleared.size() || cleared.begin() || cleared.end()
		, "Clear-pointer result:%i, size:%u, begin:%p, end:%p."
		, result, cleared.size(), cleared.begin(), cleared.end()
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
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_FIND_SEQUENCE, target.find(sequence) || target.find(sequence, 1) != 2
		, "semantic sequence find mismatch. first:%i, offset:%i; expected:0/2."
		, target.find(sequence), target.find(sequence, 1)
		);
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_RFIND_SEQUENCE, target.rfind(sequence) != 2 || target.rfind(sequence, 1)
		, "semantic sequence rfind mismatch. last:%i, offset:%i; expected:2/0."
		, target.rfind(sequence), target.rfind(sequence, 1)
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
	LLC_TEST_CHECK(errors, ARRAY_OBJ_TEST_RESULT_EMPLACE_LIFETIME
		, SArrayObjTestValue::LiveCount || SArrayObjTestValue::DestructorCount != SArrayObjTestValue::ConstructorCount + SArrayObjTestValue::CopyCount
		, "Live:%u, constructors:%u, copies:%u, destructors:%u."
		, SArrayObjTestValue::LiveCount, SArrayObjTestValue::ConstructorCount, SArrayObjTestValue::CopyCount, SArrayObjTestValue::DestructorCount
		);
	rtrn 0;
}
