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
	LLC_TEST_REQUIRE(errors, BLOCK_CONTAINER_TEST_RESULT_PLACEMENT, ::llc::failed(firstBlock)
		, "First push_sequence() failed:%i."
		, firstBlock
		);
	LLC_TEST_REQUIRE(errors, BLOCK_CONTAINER_TEST_RESULT_PLACEMENT, ::llc::failed(secondBlock)
		, "Second push_sequence() failed:%i."
		, secondBlock
		);
	LLC_TEST_REQUIRE(errors, BLOCK_CONTAINER_TEST_RESULT_PLACEMENT, ::llc::failed(thirdBlock)
		, "Third push_sequence() failed:%i."
		, thirdBlock
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_PLACEMENT, firstBlock
		, "First block index:%i, expected:0."
		, firstBlock
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_PLACEMENT, secondBlock
		, "Second block index:%i, expected:0."
		, secondBlock
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_PLACEMENT, 1 != thirdBlock
		, "Third block index:%i, expected:1."
		, thirdBlock
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_STORAGE, first != ::llc::view<cnst T>{firstData}
		, "First stored sequence differs. size:%u, expected:%u."
		, first.size(), ::llc::u2_t(::llc::size(firstData))
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_STORAGE, second != ::llc::view<cnst T>{secondData}
		, "Second stored sequence differs. size:%u, expected:%u."
		, second.size(), ::llc::u2_t(::llc::size(secondData))
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_STORAGE, third != ::llc::view<cnst T>{thirdData}
		, "Third stored sequence differs. size:%u, expected:%u."
		, third.size(), ::llc::u2_t(::llc::size(thirdData))
		);

	::llc::view<T> reserved = {};
	cnst ::llc::err_t reservedBlock = blocks.reserve(1, reserved);
	LLC_TEST_REQUIRE(errors, BLOCK_CONTAINER_TEST_RESULT_PLACEMENT, ::llc::failed(reservedBlock)
		, "reserve() failed with result:%i."
		, reservedBlock
		);
	LLC_TEST_REQUIRE(errors, BLOCK_CONTAINER_TEST_RESULT_PLACEMENT, reserved.size() != 1
		, "reserve() returned view size:%u, expected:1."
		, reserved.size()
		);
	reserved[0] = T(6);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_PLACEMENT, reservedBlock
		, "reserve() returned block:%i, expected:0."
		, reservedBlock
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_STORAGE, T(6) != reserved[0]
		, "Reserved value:%" LLC_FMT_S3 ", expected:6."
		, (::llc::s3_t)reserved[0]
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_STORAGE, first != ::llc::view<cnst T>{firstData}
		, "a later reservation invalidated the first stored view. size:%u."
		, first.size()
		);

	cnst T oversized[] = {T(7), T(8), T(9), T(10), T(11)};
	::llc::view<cnst T> rejected = first;
	cnst ::llc::err_t oversizedResult = ::blockContainerExpectedFailure([&]() { rtrn blocks.push_sequence(oversized, ::llc::size(oversized), rejected); });
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_BOUNDS, false == ::llc::failed(oversizedResult)
		, "Oversized push result:%i, expected failure."
		, oversizedResult
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_BOUNDS, rejected.begin() != first.begin()
		, "Oversized push changed output begin:%p, expected:%p."
		, rejected.begin(), first.begin()
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_BOUNDS, rejected.size() != first.size()
		, "Oversized push changed output size:%u, expected:%u."
		, rejected.size(), first.size()
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
	LLC_TEST_REQUIRE(errors, BLOCK_CONTAINER_TEST_RESULT_INITIALIZATION, remaining.size() != 2
		, "Serialized remaining count:%u, expected:2."
		, remaining.size()
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_INITIALIZATION, remaining[0]
		, "First block remaining:%u, expected:0."
		, remaining[0]
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_INITIALIZATION, remaining[1] != 2
		, "Second block remaining:%u, expected:2."
		, remaining[1]
		);
	LLC_TEST_REQUIRE(errors, BLOCK_CONTAINER_TEST_RESULT_INITIALIZATION, firstStorage.size() != 4
		, "First serialized block size:%u, expected:4."
		, firstStorage.size()
		);
	LLC_TEST_REQUIRE(errors, BLOCK_CONTAINER_TEST_RESULT_INITIALIZATION, secondStorage.size() != 4
		, "Second serialized block size:%u, expected:4."
		, secondStorage.size()
		);
	cnst T firstExpected[] = {T(1), T(2), T(3), T(6)};
	cnst T secondExpected[] = {T(4), T(5), T{}, T{}};
	for(::llc::u2_t iElement = 0; iElement < 4; ++iElement) {
		LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_INITIALIZATION, firstStorage[iElement] != firstExpected[iElement]
			, "First serialized block element:%u value:%" LLC_FMT_S3 ", expected:%" LLC_FMT_S3 "."
			, iElement, (::llc::s3_t)firstStorage[iElement], (::llc::s3_t)firstExpected[iElement]
			);
		LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_INITIALIZATION, secondStorage[iElement] != secondExpected[iElement]
			, "Second serialized block element:%u value:%" LLC_FMT_S3 ", expected:%" LLC_FMT_S3 "."
			, iElement, (::llc::s3_t)secondStorage[iElement], (::llc::s3_t)secondExpected[iElement]
			);
	}
		);
	::llc::vcu0_t input = serialized;
	::llc::block_container<T, 4> loaded = {};
	LLC_TEST_REQUIRE(errors, BLOCK_CONTAINER_TEST_RESULT_SERIALIZATION, ::llc::failed(loaded.Load(input))
		, "%s", "Load() failed."
		);
	LLC_TEST_REQUIRE(errors, BLOCK_CONTAINER_TEST_RESULT_SERIALIZATION, ::llc::failed(loaded.Save(restored))
		, "%s", "Save() after Load() failed."
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_SERIALIZATION, input.size()
		, "Round-trip left %u input bytes; expected:0."
		, input.size()
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_SERIALIZATION, ::llc::vcu0_t{serialized} != ::llc::vcu0_t{restored}
		, "Round-trip serialized size:%u, restored size:%u; byte content differs."
		, serialized.size(), restored.size()
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
	LLC_TEST_REQUIRE(errors, BLOCK_CONTAINER_TEST_RESULT_PLACEMENT, ::llc::failed(firstBlock)
		, "First NTS push failed:%i."
		, firstBlock
		);
	LLC_TEST_REQUIRE(errors, BLOCK_CONTAINER_TEST_RESULT_PLACEMENT, ::llc::failed(secondBlock)
		, "Second NTS push failed:%i."
		, secondBlock
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_PLACEMENT, firstBlock
		, "First NTS block index:%i, expected:0."
		, firstBlock
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_PLACEMENT, 1 != secondBlock
		, "Second NTS block index:%i, expected:1."
		, secondBlock
		);
	LLC_TEST_REQUIRE(errors, BLOCK_CONTAINER_TEST_RESULT_NTS_TERMINATOR, first.size() != 4
		, "First NTS view size:%u, expected:4."
		, first.size()
		);
	LLC_TEST_REQUIRE(errors, BLOCK_CONTAINER_TEST_RESULT_NTS_TERMINATOR, second.size() != 1
		, "Second NTS view size:%u, expected:1."
		, second.size()
		);
	LLC_TEST_REQUIRE(errors, BLOCK_CONTAINER_TEST_RESULT_NTS_TERMINATOR, !first.begin()
		, "%s", "First NTS view has no storage."
		);
	LLC_TEST_REQUIRE(errors, BLOCK_CONTAINER_TEST_RESULT_NTS_TERMINATOR, !second.begin()
		, "%s", "Second NTS view has no storage."
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_NTS_TERMINATOR, first != LLC_CXS("four")
		, "First NTS view:'%.*s', expected:'four'."
		, (int)first.size(), first.begin()
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_NTS_TERMINATOR, second != LLC_CXS("x")
		, "Second NTS view:'%.*s', expected:'x'."
		, (int)second.size(), second.begin()
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_NTS_TERMINATOR, first.begin()[first.size()]
		, "First NTS terminator:%i, expected:0."
		, first.begin()[first.size()]
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_NTS_TERMINATOR, second.begin()[second.size()]
		, "Second NTS terminator:%i, expected:0."
		, second.begin()[second.size()]
		);

	::llc::vcst_t rejected = first;
	cnst ::llc::err_t oversizedResult = ::blockContainerExpectedFailure([&]() { rtrn strings.push_sequence("12345", 5, rejected); });
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_BOUNDS, false == ::llc::failed(oversizedResult)
		, "Oversized NTS push result:%i, expected failure."
		, oversizedResult
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_BOUNDS, rejected.begin() != first.begin()
		, "Oversized NTS push changed output begin:%p, expected:%p."
		, rejected.begin(), first.begin()
		);
	LLC_TEST_CHECK(errors, BLOCK_CONTAINER_TEST_RESULT_BOUNDS, rejected.size() != first.size()
		, "Oversized NTS push changed output size:%u, expected:%u."
		, rejected.size(), first.size()
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
