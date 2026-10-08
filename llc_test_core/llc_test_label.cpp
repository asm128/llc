#include "llc_label.h"
#include "llc_label_manager.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(LABEL_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(LABEL_TEST_RESULT, OK					, 0, "All label tests passed.");
GDEFINE_ENUM_VALUED(LABEL_TEST_RESULT, LABEL_CONTENT		, 1, "label did not preserve its bounded string contents.");
GDEFINE_ENUM_VALUED(LABEL_TEST_RESULT, LABEL_INTERN			, 2, "Equal labels did not share their interned storage.");
GDEFINE_ENUM_VALUED(LABEL_TEST_RESULT, LABEL_COMPARE		, 3, "label equality did not follow string content.");
GDEFINE_ENUM_VALUED(LABEL_TEST_RESULT, LABEL_SERIALIZATION	, 4, "loadLabel() did not restore an interned label and consume only its bytes.");
GDEFINE_ENUM_VALUED(LABEL_TEST_RESULT, MANAGER_INTERN		, 5, "CLabelManager did not deduplicate equal strings.");
GDEFINE_ENUM_VALUED(LABEL_TEST_RESULT, MANAGER_SERIALIZATION, 6, "CLabelManager did not preserve labels across Save()/Load().");
GDEFINE_ENUM_VALUED(LABEL_TEST_RESULT, MANAGER_FAILURE_STATE	, 7, "A failed CLabelManager::Load() changed its input or prior state.");

tplt<tpnm TCall>
sttc ::llc::err_t labelExpectedFailure(TCall call) {
	::llc::setupLogCallbacks(0, 0);
	cnst ::llc::err_t result = call();
	::llc::setupDefaultLogCallbacks();
	rtrn result;
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
	if_fail_fe(::testLabelManager(errors));
	rtrn ::testLabels(errors);
}
