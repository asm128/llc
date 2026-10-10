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
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_MANAGER_INTERN, ::llc::failed(iAlpha) , "View('alpha') failed:%i." , iAlpha );
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_MANAGER_INTERN, ::llc::failed(iBeta) , "View('beta') failed:%i." , iBeta );
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_MANAGER_INTERN, ::llc::failed(iRepeated) , "Repeated View('alpha') failed:%i." , iRepeated );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_MANAGER_INTERN, 1 != iAlpha , "Alpha index:%i, expected:1." , iAlpha );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_MANAGER_INTERN, 2 != iBeta , "Beta index:%i, expected:2." , iBeta );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_MANAGER_INTERN, iAlpha != iRepeated , "Repeated alpha index:%i, original:%i." , iRepeated, iAlpha );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_MANAGER_INTERN, alpha.begin() != repeated.begin() , "Repeated alpha storage:%p, original:%p." , repeated.begin(), alpha.begin() );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_MANAGER_INTERN, 3 != manager.Size() , "Manager size:%i, expected:3." , manager.Size() );
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_MANAGER_INTERN, !alpha.begin() , "%s", "Manager alpha has no storage." );
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_MANAGER_INTERN, !beta.begin() , "%s", "Manager beta has no storage." );
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_MANAGER_INTERN, alpha.size() != 5 , "Manager alpha size:%u, expected:5." , alpha.size() );
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_MANAGER_INTERN, beta.size() != 4 , "Manager beta size:%u, expected:4." , beta.size() );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_MANAGER_INTERN, alpha != LLC_CXS("alpha") , "Manager alpha:'%.*s', expected:'alpha'." , (int)alpha.size(), alpha.begin() );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_MANAGER_INTERN, beta != LLC_CXS("beta") , "Manager beta:'%.*s', expected:'beta'." , (int)beta.size(), beta.begin() );

	::llc::au0_t serialized = {};
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_MANAGER_SERIALIZATION, ::llc::failed(manager.Save(serialized)) , "%s", "manager Save() failed." );
	cnst ::llc::u2_t serializedSize = serialized.size();
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_MANAGER_SERIALIZATION, 0 == serializedSize , "%s", "manager Save() produced no bytes." );
	::llc::vcu0_t malformed = {serialized.begin(), serializedSize - 1};
	::llc::CLabelManager preserved = {};
	::llc::vcst_t kept = {};
	if_fail_fe(preserved.View("kept", 4, kept));
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_MANAGER_FAILURE_STATE, !kept.begin() , "%s", "Preserved manager did not return the initial label." );
	cnst ::llc::vcu0_t malformedBefore = malformed;
	cnst ::llc::err_t malformedResult = ::labelExpectedFailure([&]() { rtrn preserved.Load(malformed); });
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_MANAGER_FAILURE_STATE, false == ::llc::failed(malformedResult) , "Malformed manager Load() result:%i, expected failure." , malformedResult );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_MANAGER_FAILURE_STATE, malformed.begin() != malformedBefore.begin() , "Malformed Load() moved input begin:%p, expected:%p." , malformed.begin(), malformedBefore.begin() );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_MANAGER_FAILURE_STATE, malformed.size() != malformedBefore.size() , "Malformed Load() changed input size:%u, expected:%u." , malformed.size(), malformedBefore.size() );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_MANAGER_FAILURE_STATE, preserved.Size() != 2 , "Malformed Load() changed manager size:%i, expected:2." , preserved.Size() );
	if(2 <= preserved.Size()) {
		cnst ::llc::vcst_t preservedLabel = preserved.View(1);
		LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_MANAGER_FAILURE_STATE, !preservedLabel.begin() , "%s", "Malformed Load() removed preserved label storage." );
		LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_MANAGER_FAILURE_STATE, preservedLabel.size() != kept.size() , "Malformed Load() changed preserved label size:%u, expected:%u." , preservedLabel.size(), kept.size() );
		LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_MANAGER_FAILURE_STATE, preservedLabel != kept , "Malformed Load() changed preserved label:'%.*s', expected:'%.*s'." , (int)preservedLabel.size(), preservedLabel.begin(), (int)kept.size(), kept.begin() );
	}

	::llc::CLabelManager loaded = {};
	if_fail_fe(loaded.View("old", 3));
	if_fail_fe(serialized.push_back(0xA5));
	::llc::vcu0_t input = serialized;
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_MANAGER_SERIALIZATION, ::llc::failed(loaded.Load(input)) , "%s", "manager Load() failed." );
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_MANAGER_SERIALIZATION, loaded.Size() != 3 , "Loaded manager size:%i, expected:3." , loaded.Size() );
	::llc::vcst_t loadedAlpha = loaded.View(1), loadedBeta = loaded.View(2), loadedRepeated = {};
	cnst ::llc::err_t iLoadedRepeated = loaded.View("alpha", 5, loadedRepeated);
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_MANAGER_SERIALIZATION, !loadedAlpha.begin() , "%s", "Loaded alpha has no storage." );
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_MANAGER_SERIALIZATION, !loadedBeta.begin() , "%s", "Loaded beta has no storage." );
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_MANAGER_SERIALIZATION, loadedAlpha.size() != alpha.size() , "Loaded alpha size:%u, expected:%u." , loadedAlpha.size(), alpha.size() );
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_MANAGER_SERIALIZATION, loadedBeta.size() != beta.size() , "Loaded beta size:%u, expected:%u." , loadedBeta.size(), beta.size() );
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_MANAGER_SERIALIZATION, 1 != input.size() , "Loaded manager left %u input bytes, expected:1." , input.size() );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_MANAGER_SERIALIZATION, 0xA5 != input[0] , "Loaded manager changed following byte:%u, expected:165." , input[0] );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_MANAGER_SERIALIZATION, loadedAlpha != alpha , "Loaded alpha:'%.*s', expected:'%.*s'." , (int)loadedAlpha.size(), loadedAlpha.begin(), (int)alpha.size(), alpha.begin() );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_MANAGER_SERIALIZATION, loadedBeta != beta , "Loaded beta:'%.*s', expected:'%.*s'." , (int)loadedBeta.size(), loadedBeta.begin(), (int)beta.size(), beta.begin() );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_MANAGER_SERIALIZATION, 1 != iLoadedRepeated , "Loaded repeated alpha index:%i, expected:1." , iLoadedRepeated );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_MANAGER_SERIALIZATION, loadedAlpha.begin() != loadedRepeated.begin() , "Loaded repeated alpha storage:%p, first alpha:%p." , loadedRepeated.begin(), loadedAlpha.begin() );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_MANAGER_SERIALIZATION, 0 <= loaded.Index(LLC_CXS("old")) , "Loaded manager retained old label at index:%i." , loaded.Index(LLC_CXS("old")) );
	rtrn 0;
}

sttc ::llc::err_t testLabels(ATestError & errors) {
	cnst ::llc::sc_t boundedText[] = {'a', 'l', 'p', 'h', 'a'};
	cnst ::llc::label alpha		= "alpha";
	cnst ::llc::label repeated	= LLC_CXS("alpha");
	cnst ::llc::label truncated	= {"alphabet", 5};
	cnst ::llc::label bounded	= {boundedText, ::llc::size(boundedText)};
	cnst ::llc::label beta		= "beta";
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_LABEL_CONTENT, alpha.size() != 5 , "Alpha label size:%u, expected:5." , alpha.size() );
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_LABEL_CONTENT, !alpha.begin() , "%s", "Alpha label has no storage." );
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_LABEL_CONTENT, beta.size() != 4 , "Beta label size:%u, expected:4." , beta.size() );
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_LABEL_CONTENT, !beta.begin() , "%s", "Beta label has no storage." );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_LABEL_CONTENT, false == (alpha == LLC_CXS("alpha")) , "Alpha label text:'%.*s', expected:'alpha'." , (int)alpha.size(), alpha.begin() );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_LABEL_CONTENT, alpha.begin()[alpha.size()] , "Alpha label terminator:%i, expected:0." , alpha.begin()[alpha.size()] );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_LABEL_INTERN, alpha.begin() != repeated.begin() , "Repeated alpha storage:%p, expected:%p." , repeated.begin(), alpha.begin() );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_LABEL_INTERN, alpha.begin() != truncated.begin() , "Truncated alpha storage:%p, expected:%p." , truncated.begin(), alpha.begin() );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_LABEL_INTERN, alpha.begin() != bounded.begin() , "Bounded alpha storage:%p, expected:%p." , bounded.begin(), alpha.begin() );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_LABEL_COMPARE, false == (alpha == repeated) , "%s", "Alpha differs from its repeated label." );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_LABEL_COMPARE, false == (alpha == truncated) , "%s", "Alpha differs from its truncated label." );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_LABEL_COMPARE, false == (alpha != beta) , "Alpha equals beta:'%.*s'." , (int)beta.size(), beta.begin() );

	::llc::au0_t serialized = {};
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_LABEL_SERIALIZATION, ::llc::failed(::llc::saveView(serialized, (::llc::vcst_t)alpha)) , "%s", "saveView() failed." );
	cnst ::llc::u2_t serializedSize = serialized.size();
	if_fail_fe(serialized.push_back(0x5A));
	::llc::vcu0_t input = serialized;
	::llc::vcst_t loaded = {};
	cnst ::llc::err_t bytesRead = ::llc::loadLabel(input, loaded);
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_LABEL_SERIALIZATION, (::llc::err_t)serializedSize != bytesRead , "loadLabel() consumed:%i, expected:%u bytes." , bytesRead, serializedSize );
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_LABEL_SERIALIZATION, !loaded.begin() , "%s", "loadLabel() returned no storage." );
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_LABEL_SERIALIZATION, loaded.size() != alpha.size() , "loadLabel() count:%u, expected:%u." , loaded.size(), alpha.size() );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_LABEL_SERIALIZATION, loaded != alpha , "loadLabel() text:'%.*s', expected:'%.*s'." , (int)loaded.size(), loaded.begin(), (int)alpha.size(), alpha.begin() );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_LABEL_SERIALIZATION, loaded.begin() != alpha.begin() , "loadLabel() storage:%p, interned alpha:%p." , loaded.begin(), alpha.begin() );
	LLC_TEST_REQUIREF(errors, LABEL_TEST_RESULT_LABEL_SERIALIZATION, 1 != input.size() , "loadLabel() remaining bytes:%u, expected:1." , input.size() );
	LLC_TEST_CHECKF(errors, LABEL_TEST_RESULT_LABEL_SERIALIZATION, 0x5A != input[0] , "loadLabel() changed following byte:%u, expected:90." , input[0] );
	rtrn 0;
}

::llc::err_t testLabel(ATestError & errors) {
	if_fail_fe(::testLabelManager(errors));
	rtrn ::testLabels(errors);
}
