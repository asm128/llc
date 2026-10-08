#include "llc_block_container_nts.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(BLOCK_CONTAINER_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(BLOCK_CONTAINER_TEST_RESULT, OK				, 0, "All block-container tests passed.");
GDEFINE_ENUM_VALUED(BLOCK_CONTAINER_TEST_RESULT, PLACEMENT			, 1, "block_container<> did not preserve first-fit block placement.");
GDEFINE_ENUM_VALUED(BLOCK_CONTAINER_TEST_RESULT, STORAGE			, 2, "block_container<> did not preserve stored sequences and stable views.");
GDEFINE_ENUM_VALUED(BLOCK_CONTAINER_TEST_RESULT, BOUNDS				, 3, "A block container accepted a sequence larger than its capacity.");
GDEFINE_ENUM_VALUED(BLOCK_CONTAINER_TEST_RESULT, SERIALIZATION		, 4, "A block container did not preserve its serialized state.");
GDEFINE_ENUM_VALUED(BLOCK_CONTAINER_TEST_RESULT, INITIALIZATION		, 5, "A block container exposed uninitialized cells through serialization.");
GDEFINE_ENUM_VALUED(BLOCK_CONTAINER_TEST_RESULT, NTS_TERMINATOR		, 6, "block_container_nts<> did not append a terminator outside the returned view.");

tplt<tpnm TCall>
sttc ::llc::err_t blockContainerExpectedFailure(TCall call) {
	::llc::setupLogCallbacks(0, 0);
	cnst ::llc::err_t result = call();
	::llc::setupDefaultLogCallbacks();
	rtrn result;
}

tplt<tpnm T>
sttc ::llc::err_t testBlockContainerType(ATestError & errors) {
	::llc::block_container<T, 4> blocks = {};
	cnst T firstData[]	= {T(1), T(2)};
	cnst T secondData[]	= {T(3)};
	cnst T thirdData[]	= {T(4), T(5)};
	::llc::view<cnst T> first = {}, second = {}, third = {};
	cnst ::llc::err_t firstBlock	= blocks.push_sequence(firstData, ::llc::size(firstData), first);
	cnst ::llc::err_t secondBlock	= blocks.push_sequence(secondData, ::llc::size(secondData), second);
	cnst ::llc::err_t thirdBlock	= blocks.push_sequence(thirdData, ::llc::size(thirdData), third);
	LLC_TEST_REQUIRE(errors, BLOCK_CONTAINER_TEST_RESULT_PLACEMENT, ::llc::failed(firstBlock) || ::llc::failed(secondBlock) || ::llc::failed(thirdBlock)
		, "push_sequence() failed. first:%i, second:%i, third:%i."
		, firstBlock, secondBlock, thirdBlock
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_PLACEMENT, firstBlock || secondBlock || 1 != thirdBlock
		, "unexpected block indices. first:%i, second:%i, third:%i."
		, firstBlock, secondBlock, thirdBlock
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_STORAGE
		, first != ::llc::view<cnst T>{firstData} || second != ::llc::view<cnst T>{secondData} || third != ::llc::view<cnst T>{thirdData}
		, "stored sequence mismatch. sizes:%u/%u/%u."
		, first.size(), second.size(), third.size()
		);

	::llc::view<T> reserved = {};
	cnst ::llc::err_t reservedBlock = blocks.reserve(1, reserved);
	LLC_TEST_REQUIRE(errors, BLOCK_CONTAINER_TEST_RESULT_PLACEMENT, ::llc::failed(reservedBlock)
		, "reserve() failed with result:%i."
		, reservedBlock
		);
	reserved[0] = T(6);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_PLACEMENT, reservedBlock || T(6) != reserved[0]
		, "reserve() returned block:%i."
		, reservedBlock
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_STORAGE, first != ::llc::view<cnst T>{firstData}
		, "a later reservation invalidated the first stored view. size:%u."
		, first.size()
		);

	cnst T oversized[] = {T(7), T(8), T(9), T(10), T(11)};
	::llc::view<cnst T> rejected = first;
	cnst ::llc::err_t oversizedResult = ::blockContainerExpectedFailure([&]() { rtrn blocks.push_sequence(oversized, ::llc::size(oversized), rejected); });
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_BOUNDS, false == ::llc::failed(oversizedResult) || rejected.begin() != first.begin() || rejected.size() != first.size()
		, "oversized push result:%i, output:%p/%u, expected:%p/%u."
		, oversizedResult, rejected.begin(), rejected.size(), first.begin(), first.size()
		);

	::llc::au0_t serialized = {}, restored = {};
	LLC_TEST_REQUIRE(errors, BLOCK_CONTAINER_TEST_RESULT_SERIALIZATION, ::llc::failed(blocks.Save(serialized))
		, "%s", "Save() failed."
		);
	::llc::vcu0_t serializedState = serialized;
	::llc::au2_t remaining = {};
	::llc::view<cnst T> firstStorage = {}, secondStorage = {};
	if_fail_fe(::llc::loadView(serializedState, remaining));
	if_fail_fe(::llc::loadView(serializedState, firstStorage));
	if_fail_fe(::llc::loadView(serializedState, secondStorage));
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_INITIALIZATION
		, remaining.size() != 2 || remaining[0] || remaining[1] != 2
		|| firstStorage.size() != 4 || secondStorage.size() != 4
		|| secondStorage[2] != T{} || secondStorage[3] != T{}
		, "serialized block state mismatch. remaining count:%u, block sizes:%u/%u."
		, remaining.size(), firstStorage.size(), secondStorage.size()
		);
	::llc::vcu0_t input = serialized;
	::llc::block_container<T, 4> loaded = {};
	LLC_TEST_REQUIRE(errors, BLOCK_CONTAINER_TEST_RESULT_SERIALIZATION, ::llc::failed(loaded.Load(input))
		, "%s", "Load() failed."
		);
	LLC_TEST_REQUIRE(errors, BLOCK_CONTAINER_TEST_RESULT_SERIALIZATION, ::llc::failed(loaded.Save(restored))
		, "%s", "Save() after Load() failed."
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_SERIALIZATION, input.size() || ::llc::vcu0_t{serialized} != ::llc::vcu0_t{restored}
		, "round-trip mismatch. remaining:%u, serialized:%u, restored:%u."
		, input.size(), serialized.size(), restored.size()
		);
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testBlockContainerTypeLogged(ATestError & errors) {
	cnst ::llc::u2_t checkCount = testCheckCount(errors);
	cnst ::llc::u2_t failureCount = testErrorCount(errors);
	if_fail_fe(::testBlockContainerType<T>(errors));
	cnst ::llc::u2_t typeFailures = testErrorCount(errors) - failureCount;
	cnst ::llc::u2_t typeChecks = testCheckCount(errors) - checkCount;
	if(typeFailures)
		error_printf("%s block-container tests completed: %u/%u checks passed, %u failed.", ::llc::get_type_namep<T>(), typeChecks - typeFailures, typeChecks, typeFailures);
	rtrn 0;
}

sttc ::llc::err_t testBlockContainerNTS(ATestError & errors) {
	::llc::block_container_nts<5> strings = {};
	::llc::vcst_t first = {}, second = {};
	cnst ::llc::err_t firstBlock = strings.push_sequence("four", 4, first);
	cnst ::llc::err_t secondBlock = strings.push_sequence("x", 1, second);
	LLC_TEST_REQUIRE(errors, BLOCK_CONTAINER_TEST_RESULT_PLACEMENT, ::llc::failed(firstBlock) || ::llc::failed(secondBlock)
		, "NTS push failed. first:%i, second:%i."
		, firstBlock, secondBlock
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_PLACEMENT, firstBlock || 1 != secondBlock
		, "NTS block indices mismatch. first:%i, second:%i."
		, firstBlock, secondBlock
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_NTS_TERMINATOR, first != LLC_CXS("four") || second != LLC_CXS("x") || first.begin()[first.size()] || second.begin()[second.size()]
		, "NTS contents or terminators differ. sizes:%u/%u, terminators:%i/%i."
		, first.size(), second.size(), first.begin()[first.size()], second.begin()[second.size()]
		);

	::llc::vcst_t rejected = first;
	cnst ::llc::err_t oversizedResult = ::blockContainerExpectedFailure([&]() { rtrn strings.push_sequence("12345", 5, rejected); });
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_BOUNDS, false == ::llc::failed(oversizedResult) || rejected.begin() != first.begin() || rejected.size() != first.size()
		, "oversized NTS result:%i, output:%p/%u, expected:%p/%u."
		, oversizedResult, rejected.begin(), rejected.size(), first.begin(), first.size()
		);
	rtrn 0;
}

::llc::err_t testBlockContainer(ATestError & errors) {
	if_fail_fe(::testBlockContainerTypeLogged<::llc::u0_t>(errors));
	if_fail_fe(::testBlockContainerTypeLogged<::llc::u1_t>(errors));
	if_fail_fe(::testBlockContainerTypeLogged<::llc::u2_t>(errors));
	if_fail_fe(::testBlockContainerTypeLogged<::llc::u3_t>(errors));
	if_fail_fe(::testBlockContainerTypeLogged<::llc::s0_t>(errors));
	if_fail_fe(::testBlockContainerTypeLogged<::llc::s1_t>(errors));
	if_fail_fe(::testBlockContainerTypeLogged<::llc::s2_t>(errors));
	if_fail_fe(::testBlockContainerTypeLogged<::llc::s3_t>(errors));
	rtrn ::testBlockContainerNTS(errors);
}
