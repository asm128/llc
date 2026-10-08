#include "llc_label.h"
#include "llc_label_manager.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(LABEL_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(LABEL_TEST_RESULT, OK					, 0, "All label and block-container tests passed.");
GDEFINE_ENUM_VALUED(LABEL_TEST_RESULT, BLOCK_PLACEMENT		, 1, "block_container<> did not preserve first-fit block placement.");
GDEFINE_ENUM_VALUED(LABEL_TEST_RESULT, BLOCK_STORAGE		, 2, "block_container<> did not preserve stored sequences and stable views.");
GDEFINE_ENUM_VALUED(LABEL_TEST_RESULT, BLOCK_BOUNDS		, 3, "A block container accepted a sequence larger than its capacity.");
GDEFINE_ENUM_VALUED(LABEL_TEST_RESULT, BLOCK_SERIALIZATION	, 4, "A block container did not preserve its serialized state.");
GDEFINE_ENUM_VALUED(LABEL_TEST_RESULT, BLOCK_INITIALIZATION	, 5, "A block container exposed uninitialized cells through serialization.");
GDEFINE_ENUM_VALUED(LABEL_TEST_RESULT, NTS_TERMINATOR		, 6, "block_container_nts<> did not append a terminator outside the returned view.");
GDEFINE_ENUM_VALUED(LABEL_TEST_RESULT, LABEL_CONTENT		, 7, "label did not preserve its bounded string contents.");
GDEFINE_ENUM_VALUED(LABEL_TEST_RESULT, LABEL_INTERN			, 8, "Equal labels did not share their interned storage.");
GDEFINE_ENUM_VALUED(LABEL_TEST_RESULT, LABEL_COMPARE		, 9, "label equality did not follow string content.");
GDEFINE_ENUM_VALUED(LABEL_TEST_RESULT, LABEL_SERIALIZATION	, 10, "loadLabel() did not restore an interned label and consume only its bytes.");
GDEFINE_ENUM_VALUED(LABEL_TEST_RESULT, MANAGER_INTERN		, 11, "CLabelManager did not deduplicate equal strings.");
GDEFINE_ENUM_VALUED(LABEL_TEST_RESULT, MANAGER_SERIALIZATION, 12, "CLabelManager did not preserve labels across Save()/Load().");
GDEFINE_ENUM_VALUED(LABEL_TEST_RESULT, MANAGER_FAILURE_STATE	, 13, "A failed CLabelManager::Load() changed its input or prior state.");

tplt<tpnm TCall>
sttc ::llc::err_t labelExpectedFailure(TCall call) {
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
	LLC_TEST_REQUIRE(errors, LABEL_TEST_RESULT_BLOCK_PLACEMENT, ::llc::failed(firstBlock) || ::llc::failed(secondBlock) || ::llc::failed(thirdBlock)
		, "push_sequence() failed. first:%i, second:%i, third:%i."
		, firstBlock, secondBlock, thirdBlock
		);
	LLC_TEST_CHECK(errors, LABEL_TEST_RESULT_BLOCK_PLACEMENT, firstBlock || secondBlock || 1 != thirdBlock
		, "unexpected block indices. first:%i, second:%i, third:%i."
		, firstBlock, secondBlock, thirdBlock
		);
	LLC_TEST_CHECK(errors, LABEL_TEST_RESULT_BLOCK_STORAGE
		, first != ::llc::view<cnst T>{firstData} || second != ::llc::view<cnst T>{secondData} || third != ::llc::view<cnst T>{thirdData}
		, "stored sequence mismatch. sizes:%u/%u/%u."
		, first.size(), second.size(), third.size()
		);

	::llc::view<T> reserved = {};
	cnst ::llc::err_t reservedBlock = blocks.reserve(1, reserved);
	LLC_TEST_REQUIRE(errors, LABEL_TEST_RESULT_BLOCK_PLACEMENT, ::llc::failed(reservedBlock)
		, "reserve() failed with result:%i."
		, reservedBlock
		);
	reserved[0] = T(6);
	LLC_TEST_CHECK(errors, LABEL_TEST_RESULT_BLOCK_PLACEMENT, reservedBlock || T(6) != reserved[0]
		, "reserve() returned block:%i."
		, reservedBlock
		);
	LLC_TEST_CHECK(errors, LABEL_TEST_RESULT_BLOCK_STORAGE, first != ::llc::view<cnst T>{firstData}
		, "a later reservation invalidated the first stored view. size:%u."
		, first.size()
		);

	cnst T oversized[] = {T(7), T(8), T(9), T(10), T(11)};
	::llc::view<cnst T> rejected = first;
	cnst ::llc::err_t oversizedResult = ::labelExpectedFailure([&]() { rtrn blocks.push_sequence(oversized, ::llc::size(oversized), rejected); });
	LLC_TEST_CHECK(errors, LABEL_TEST_RESULT_BLOCK_BOUNDS, false == ::llc::failed(oversizedResult) || rejected.begin() != first.begin() || rejected.size() != first.size()
		, "oversized push result:%i, output:%p/%u, expected:%p/%u."
		, oversizedResult, rejected.begin(), rejected.size(), first.begin(), first.size()
		);

	::llc::au0_t serialized = {}, restored = {};
	LLC_TEST_REQUIRE(errors, LABEL_TEST_RESULT_BLOCK_SERIALIZATION, ::llc::failed(blocks.Save(serialized))
		, "%s", "Save() failed."
		);
	::llc::vcu0_t serializedState = serialized;
	::llc::au2_t remaining = {};
	::llc::view<cnst T> firstStorage = {}, secondStorage = {};
	if_fail_fe(::llc::loadView(serializedState, remaining));
	if_fail_fe(::llc::loadView(serializedState, firstStorage));
	if_fail_fe(::llc::loadView(serializedState, secondStorage));
	LLC_TEST_CHECK(errors, LABEL_TEST_RESULT_BLOCK_INITIALIZATION
		, remaining.size() != 2 || remaining[0] || remaining[1] != 2
		|| firstStorage.size() != 4 || secondStorage.size() != 4
		|| secondStorage[2] != T{} || secondStorage[3] != T{}
		, "serialized block state mismatch. remaining count:%u, block sizes:%u/%u."
		, remaining.size(), firstStorage.size(), secondStorage.size()
		);
	::llc::vcu0_t input = serialized;
	::llc::block_container<T, 4> loaded = {};
	LLC_TEST_REQUIRE(errors, LABEL_TEST_RESULT_BLOCK_SERIALIZATION, ::llc::failed(loaded.Load(input))
		, "%s", "Load() failed."
		);
	LLC_TEST_REQUIRE(errors, LABEL_TEST_RESULT_BLOCK_SERIALIZATION, ::llc::failed(loaded.Save(restored))
		, "%s", "Save() after Load() failed."
		);
	LLC_TEST_CHECK(errors, LABEL_TEST_RESULT_BLOCK_SERIALIZATION, input.size() || ::llc::vcu0_t{serialized} != ::llc::vcu0_t{restored}
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
	LLC_TEST_REQUIRE(errors, LABEL_TEST_RESULT_BLOCK_PLACEMENT, ::llc::failed(firstBlock) || ::llc::failed(secondBlock)
		, "NTS push failed. first:%i, second:%i."
		, firstBlock, secondBlock
		);
	LLC_TEST_CHECK(errors, LABEL_TEST_RESULT_BLOCK_PLACEMENT, firstBlock || 1 != secondBlock
		, "NTS block indices mismatch. first:%i, second:%i."
		, firstBlock, secondBlock
		);
	LLC_TEST_CHECK(errors, LABEL_TEST_RESULT_NTS_TERMINATOR, first != LLC_CXS("four") || second != LLC_CXS("x") || first.begin()[first.size()] || second.begin()[second.size()]
		, "NTS contents or terminators differ. sizes:%u/%u, terminators:%i/%i."
		, first.size(), second.size(), first.begin()[first.size()], second.begin()[second.size()]
		);

	::llc::vcst_t rejected = first;
	cnst ::llc::err_t oversizedResult = ::labelExpectedFailure([&]() { rtrn strings.push_sequence("12345", 5, rejected); });
	LLC_TEST_CHECK(errors, LABEL_TEST_RESULT_BLOCK_BOUNDS, false == ::llc::failed(oversizedResult) || rejected.begin() != first.begin() || rejected.size() != first.size()
		, "oversized NTS result:%i, output:%p/%u, expected:%p/%u."
		, oversizedResult, rejected.begin(), rejected.size(), first.begin(), first.size()
		);
	rtrn 0;
}

sttc ::llc::err_t testLabelManager(ATestError & errors) {
	::llc::CLabelManager manager = {};
	::llc::vcst_t alpha = {}, beta = {}, repeated = {};
	cnst ::llc::err_t iAlpha	= manager.View("alpha", 5, alpha);
	cnst ::llc::err_t iBeta		= manager.View("beta", 4, beta);
	cnst ::llc::err_t iRepeated	= manager.View("alpha", 5, repeated);
	LLC_TEST_REQUIRE(errors, LABEL_TEST_RESULT_MANAGER_INTERN, ::llc::failed(iAlpha) || ::llc::failed(iBeta) || ::llc::failed(iRepeated)
		, "View() failed. alpha:%i, beta:%i, repeated:%i."
		, iAlpha, iBeta, iRepeated
		);
	LLC_TEST_CHECK(errors, LABEL_TEST_RESULT_MANAGER_INTERN, 1 != iAlpha || 2 != iBeta || iAlpha != iRepeated || alpha.begin() != repeated.begin() || 3 != manager.Size()
		, "manager state mismatch. alpha:%i, beta:%i, repeated:%i, size:%i."
		, iAlpha, iBeta, iRepeated, manager.Size()
		);

	::llc::au0_t serialized = {};
	LLC_TEST_REQUIRE(errors, LABEL_TEST_RESULT_MANAGER_SERIALIZATION, ::llc::failed(manager.Save(serialized))
		, "%s", "manager Save() failed."
		);
	cnst ::llc::u2_t serializedSize = serialized.size();
	::llc::vcu0_t malformed = {serialized.begin(), serializedSize - 1};
	::llc::CLabelManager preserved = {};
	::llc::vcst_t kept = {};
	if_fail_fe(preserved.View("kept", 4, kept));
	cnst ::llc::vcu0_t malformedBefore = malformed;
	cnst ::llc::err_t malformedResult = ::labelExpectedFailure([&]() { rtrn preserved.Load(malformed); });
	LLC_TEST_CHECK(errors, LABEL_TEST_RESULT_MANAGER_FAILURE_STATE
		, false == ::llc::failed(malformedResult)
		|| malformed.begin() != malformedBefore.begin() || malformed.size() != malformedBefore.size()
		|| preserved.Size() != 2 || preserved.View(1) != kept
		, "failed load state mismatch. result:%i, input:%p/%u, expected:%p/%u, manager size:%i."
		, malformedResult, malformed.begin(), malformed.size(), malformedBefore.begin(), malformedBefore.size(), preserved.Size()
		);

	::llc::CLabelManager loaded = {};
	if_fail_fe(loaded.View("old", 3));
	if_fail_fe(serialized.push_back(0xA5));
	::llc::vcu0_t input = serialized;
	LLC_TEST_REQUIRE(errors, LABEL_TEST_RESULT_MANAGER_SERIALIZATION, ::llc::failed(loaded.Load(input))
		, "%s", "manager Load() failed."
		);
	::llc::vcst_t loadedAlpha = loaded.View(1), loadedBeta = loaded.View(2), loadedRepeated = {};
	cnst ::llc::err_t iLoadedRepeated = loaded.View("alpha", 5, loadedRepeated);
	LLC_TEST_CHECK(errors, LABEL_TEST_RESULT_MANAGER_SERIALIZATION
		, 1 != input.size() || 0xA5 != input[0] || 3 != loaded.Size()
		|| loadedAlpha != alpha || loadedBeta != beta || 1 != iLoadedRepeated
		|| loadedAlpha.begin() != loadedRepeated.begin() || 0 <= loaded.Index(LLC_CXS("old"))
		, "loaded manager mismatch. remaining:%u, guard:%u, size:%i, repeated:%i."
		, input.size(), input.size() ? input[0] : 0, loaded.Size(), iLoadedRepeated
		);
	rtrn 0;
}

sttc ::llc::err_t testLabels(ATestError & errors) {
	cnst ::llc::sc_t boundedText[] = {'a', 'l', 'p', 'h', 'a'};
	cnst ::llc::label alpha		= "alpha";
	cnst ::llc::label repeated	= LLC_CXS("alpha");
	cnst ::llc::label truncated	= {"alphabet", 5};
	cnst ::llc::label bounded	= {boundedText, ::llc::size(boundedText)};
	cnst ::llc::label beta		= "beta";
	LLC_TEST_CHECK(errors, LABEL_TEST_RESULT_LABEL_CONTENT, alpha.size() != 5 || false == (alpha == LLC_CXS("alpha")) || alpha.begin()[alpha.size()]
		, "alpha mismatch. size:%u, terminator:%i, text:'%.*s'."
		, alpha.size(), alpha.begin()[alpha.size()], (int)alpha.size(), alpha.begin()
		);
	LLC_TEST_CHECK(errors, LABEL_TEST_RESULT_LABEL_INTERN, alpha.begin() != repeated.begin() || alpha.begin() != truncated.begin() || alpha.begin() != bounded.begin()
		, "interned pointers differ. alpha:%p, repeated:%p, truncated:%p, bounded:%p."
		, alpha.begin(), repeated.begin(), truncated.begin(), bounded.begin()
		);
	LLC_TEST_CHECK(errors, LABEL_TEST_RESULT_LABEL_COMPARE, false == (alpha == repeated) || false == (alpha == truncated) || false == (alpha != beta)
		, "label comparison mismatch. alpha:'%.*s', beta:'%.*s'."
		, (int)alpha.size(), alpha.begin(), (int)beta.size(), beta.begin()
		);

	::llc::au0_t serialized = {};
	LLC_TEST_REQUIRE(errors, LABEL_TEST_RESULT_LABEL_SERIALIZATION, ::llc::failed(::llc::saveView(serialized, (::llc::vcst_t)alpha))
		, "%s", "saveView() failed."
		);
	cnst ::llc::u2_t serializedSize = serialized.size();
	if_fail_fe(serialized.push_back(0x5A));
	::llc::vcu0_t input = serialized;
	::llc::vcst_t loaded = {};
	cnst ::llc::err_t bytesRead = ::llc::loadLabel(input, loaded);
	LLC_TEST_CHECK(errors, LABEL_TEST_RESULT_LABEL_SERIALIZATION
		, (::llc::err_t)serializedSize != bytesRead || loaded != alpha || loaded.begin() != alpha.begin() || 1 != input.size() || 0x5A != input[0]
		, "loadLabel() mismatch. bytes:%i/%u, remaining:%u, guard:%u, interned:%i."
		, bytesRead, serializedSize, input.size(), input.size() ? input[0] : 0, loaded.begin() == alpha.begin()
		);
	rtrn 0;
}

::llc::err_t testLabel(ATestError & errors) {
	if_fail_fe(::testBlockContainerTypeLogged<::llc::u0_t>(errors));
	if_fail_fe(::testBlockContainerTypeLogged<::llc::u1_t>(errors));
	if_fail_fe(::testBlockContainerTypeLogged<::llc::u2_t>(errors));
	if_fail_fe(::testBlockContainerTypeLogged<::llc::u3_t>(errors));
	if_fail_fe(::testBlockContainerTypeLogged<::llc::s0_t>(errors));
	if_fail_fe(::testBlockContainerTypeLogged<::llc::s1_t>(errors));
	if_fail_fe(::testBlockContainerTypeLogged<::llc::s2_t>(errors));
	if_fail_fe(::testBlockContainerTypeLogged<::llc::s3_t>(errors));
	if_fail_fe(::testBlockContainerNTS(errors));
	if_fail_fe(::testLabelManager(errors));
	rtrn ::testLabels(errors);
}
