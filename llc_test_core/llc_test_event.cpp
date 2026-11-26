#include "llc_event.h"
#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(EVENT_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, ENQUEUE, 0, "eventEnqueueChild() did not enqueue the event.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, EXTRACT, 1, "The child event could not be extracted.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, DATA   , 2, "The child event contains the wrong type or data bytes.");
GDEFINE_ENUM_VALUED(EVENT_TEST_RESULT, ROUNDTRIP, 3, "The saved event could not be loaded intact.");

::llc::err_t test_event(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	cnst ::llc::vcst_t text = "asd";
	::llc::sc_t mutableText[] = "asd";
	cnst ::llc::vstr_t mutableView = mutableText;
	cnst ::llc::sc_t paddedText[] = {'x', 'a', 's', 'd', 'y'};
	cnst ::llc::vcst_t countedText{paddedText + 1, 3};
	cnst ::llc::sc_t embeddedZero[] = {'a', 0, 'd'};
	cnst ::llc::vcst_t countedZero{embeddedZero, 3};
	cnst ::llc::u1_t values[] = {0x1234, 0x5678};
	cnst ::llc::view<cnst ::llc::u1_t> typedView{values};
	cnst ::llc::u2_t pod = 0x12345678;
	auto check = [&errors](cnst auto & input, ::llc::vcu0_t expected, ::llc::vcst_t label) -> ::llc::err_t {
		::llc::TEventQueue<::llc::RESULT> queue;
		LLC_TEST_REQUIREF(errors, EVENT_TEST_RESULT_ENQUEUE, ::llc::failed(::llc::eventEnqueueChild(queue, ::llc::RESULT_Connected, ::llc::RESULT_Read, input)) || queue.size() != 1, "%.*s event count:%u.", LLC_VIEW_ARGS(label), queue.size());
		::llc::SEventView<::llc::RESULT> child;
		LLC_TEST_REQUIREF(errors, EVENT_TEST_RESULT_EXTRACT, ::llc::failed(queue[0]->ExtractChild(child)), "%.*s child extraction failed.", LLC_VIEW_ARGS(label));
		LLC_TEST_CHECKF(errors, EVENT_TEST_RESULT_DATA, queue[0]->Type != ::llc::RESULT_Connected || child.Type != ::llc::RESULT_Read || child.Data.size() != expected.size() || (expected.size() && memcmp(child.Data.begin(), expected.begin(), expected.size())), "%.*s child bytes:%u expected:%u.", LLC_VIEW_ARGS(label), child.Data.size(), expected.size());
		rtrn 0;
	};
	auto checkWrapped = [&errors](cnst auto & input, ::llc::vcu0_t expected, ::llc::vcst_t label) -> ::llc::err_t {
		::llc::SEvent<::llc::RESULT> parent{::llc::RESULT_Connected};
		LLC_TEST_REQUIREF(errors, EVENT_TEST_RESULT_ENQUEUE, ::llc::failed(::llc::eventWrapChild(parent, ::llc::RESULT_Read, input)), "%.*s wrap failed.", LLC_VIEW_ARGS(label));
		::llc::SEventView<::llc::RESULT> child;
		LLC_TEST_REQUIREF(errors, EVENT_TEST_RESULT_EXTRACT, ::llc::failed(parent.ExtractChild(child)), "%.*s wrapped child extraction failed.", LLC_VIEW_ARGS(label));
		LLC_TEST_CHECKF(errors, EVENT_TEST_RESULT_DATA, parent.Type != ::llc::RESULT_Connected || child.Type != ::llc::RESULT_Read || child.Data.size() != expected.size() || (expected.size() && memcmp(child.Data.begin(), expected.begin(), expected.size())), "%.*s wrapped child bytes:%u expected:%u.", LLC_VIEW_ARGS(label), child.Data.size(), expected.size());
		rtrn 0;
	};
	if_fail_fe(check(text, text.cu8(), "vcst_t"));
	if_fail_fe(check(mutableView, text.cu8(), "vstr_t"));
	if_fail_fe(check("asd", text.cu8(), "string literal"));
	if_fail_fe(check(countedText, countedText.cu8(), "counted vcst_t"));
	if_fail_fe(check(countedZero, countedZero.cu8(), "embedded zero vcst_t"));
	if_fail_fe(check(::llc::vcst_t{}, ::llc::vcu0_t{}, "empty vcst_t"));
	if_fail_fe(check(typedView, typedView.cu8(), "view<const u1_t>"));
	if_fail_fe(check(pod, {(cnst ::llc::u0_t*)&pod, szof(pod)}, "POD u2_t"));
	if_fail_fe(checkWrapped(text, text.cu8(), "wrapped vcst_t"));
	if_fail_fe(checkWrapped(mutableView, text.cu8(), "wrapped vstr_t"));
	if_fail_fe(checkWrapped("asd", text.cu8(), "wrapped literal"));
	if_fail_fe(checkWrapped(countedZero, countedZero.cu8(), "wrapped embedded zero"));

	::llc::sc_t source[] = "asd";
	::llc::TEventQueue<::llc::RESULT> queue;
	LLC_TEST_REQUIREF(errors, EVENT_TEST_RESULT_ENQUEUE, ::llc::failed(::llc::eventEnqueueChild(queue, ::llc::RESULT_Connected, ::llc::RESULT_Read, ::llc::vstr_t{source})) || queue.size() != 1, "Mutable source event count:%u.", queue.size());
	source[0] = 'x';
	::llc::SEventView<::llc::RESULT> copied;
	LLC_TEST_REQUIREF(errors, EVENT_TEST_RESULT_EXTRACT, ::llc::failed(queue[0]->ExtractChild(copied)), "Copied child extraction failed.");
	LLC_TEST_CHECKF(errors, EVENT_TEST_RESULT_DATA, copied.Data.size() != text.size() || 0 != memcmp(copied.Data.begin(), text.begin(), text.size()), "Copied child bytes:%u expected:%u.", copied.Data.size(), text.size());

	::llc::au0_t saved;
	LLC_TEST_REQUIREF(errors, EVENT_TEST_RESULT_ROUNDTRIP, ::llc::failed(queue[0]->Save(saved)), "Parent event save failed.");
	::llc::vcu0_t serialized = saved.cu8();
	::llc::SEvent<::llc::RESULT> loaded;
	LLC_TEST_REQUIREF(errors, EVENT_TEST_RESULT_ROUNDTRIP, ::llc::failed(loaded.Load(serialized)), "Parent event load failed.");
	::llc::SEventView<::llc::RESULT> restored;
	LLC_TEST_REQUIREF(errors, EVENT_TEST_RESULT_ROUNDTRIP, ::llc::failed(loaded.ExtractChild(restored)), "Restored child extraction failed.");
	LLC_TEST_CHECKF(errors, EVENT_TEST_RESULT_ROUNDTRIP, loaded.Type != ::llc::RESULT_Connected || restored.Type != ::llc::RESULT_Read || restored.Data.size() != text.size() || 0 != memcmp(restored.Data.begin(), text.begin(), text.size()) || serialized.size(), "Restored child bytes:%u expected:%u, trailing:%u.", restored.Data.size(), text.size(), serialized.size());
	rtrn 0;
}
