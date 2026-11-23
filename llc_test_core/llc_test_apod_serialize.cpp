#include "llc_test_core.h"
#include "llc_apod_serialize.h"
#include "llc_astatic_serialize.h"

GDEFINE_ENUM_TYPE(APOD_SERIALIZE_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(APOD_SERIALIZE_TEST_RESULT, PACKED_ROUNDTRIP, 1, "Packed integer save/load did not preserve the value and cursor.");
GDEFINE_ENUM_VALUED(APOD_SERIALIZE_TEST_RESULT, ARRAY_SETUP, 2, "Could not prepare the owned array.");
GDEFINE_ENUM_VALUED(APOD_SERIALIZE_TEST_RESULT, POD_SAVE, 3, "POD save overload reported the wrong byte count.");
GDEFINE_ENUM_VALUED(APOD_SERIALIZE_TEST_RESULT, VIEW_SAVE, 4, "Const view save overload differed from saveView().");
GDEFINE_ENUM_VALUED(APOD_SERIALIZE_TEST_RESULT, APOD_SAVE, 5, "Owned array save overload differed from saveView().");
GDEFINE_ENUM_VALUED(APOD_SERIALIZE_TEST_RESULT, NESTED_CONST_SAVE, 6, "Nested const view save reported the wrong bytes.");
GDEFINE_ENUM_VALUED(APOD_SERIALIZE_TEST_RESULT, NESTED_MUTABLE_SAVE, 7, "Nested mutable view save reported the wrong bytes.");
GDEFINE_ENUM_VALUED(APOD_SERIALIZE_TEST_RESULT, VARIADIC_SAVE, 8, "Variadic save changed field bytes or reported the wrong total.");
GDEFINE_ENUM_VALUED(APOD_SERIALIZE_TEST_RESULT, APOD_LOAD, 9, "Owned array load overload did not restore values and cursor.");
GDEFINE_ENUM_VALUED(APOD_SERIALIZE_TEST_RESULT, VARIADIC_LOAD, 10, "Variadic load did not restore mixed POD and owned array fields.");
GDEFINE_ENUM_VALUED(APOD_SERIALIZE_TEST_RESULT, VARIADIC_STOP, 11, "Variadic load continued after a failed owned array read.");
GDEFINE_ENUM_VALUED(APOD_SERIALIZE_TEST_RESULT, SIGNED_LOAD, 12, "Signed byte load did not preserve the value and cursor.");
GDEFINE_ENUM_VALUED(APOD_SERIALIZE_TEST_RESULT, STATIC_LOAD, 13, "Static array load did not preserve the values and cursor.");

::llc::err_t test_apod_serialize(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	stxp ::llc::u1_t POD = 0x1234;
	::llc::au0_t packedBytes = {};
	cnst ::llc::packedu32 packed = POD;
	cnst ::llc::err_t packedWritten = ::llc::savePacked(packedBytes, packed);
	::llc::vcu0_t packedInput{packedBytes.begin(), packedBytes.size()};
	::llc::u2_t packedValue = {};
	cnst ::llc::err_t packedRead = ::llc::loadPacked(packedInput, packedValue);
	LLC_TEST_CHECKF(errors, APOD_SERIALIZE_TEST_RESULT_PACKED_ROUNDTRIP, packedWritten != packedRead || packedValue != POD || packedInput.size() != 0, "Widths:%i/%i, value:%u, remaining:%u.", packedWritten, packedRead, packedValue, packedInput.size());
	::llc::au0_t signedBytes = {};
	LLC_TEST_REQUIREF(errors, APOD_SERIALIZE_TEST_RESULT_SIGNED_LOAD, ::llc::savePOD(signedBytes, POD) != szof(POD), "%s", "Could not serialize signed byte input.");
	::llc::vcs0_t signedInput{(cnst ::llc::s0_t*)signedBytes.begin(), signedBytes.size()};
	::llc::u1_t signedValue = {};
	cnst ::llc::err_t signedRead = ::llc::load(signedInput, signedValue);
	LLC_TEST_CHECKF(errors, APOD_SERIALIZE_TEST_RESULT_SIGNED_LOAD, signedRead != szof(POD) || signedValue != POD || signedInput.size() != 0, "Result:%i, value:%u, remaining:%u.", signedRead, signedValue, signedInput.size());
	::llc::au0_t staticBytes = {};
	::llc::u1_t staticValues[] = {POD, 0x5678};
	LLC_TEST_REQUIREF(errors, APOD_SERIALIZE_TEST_RESULT_STATIC_LOAD, ::llc::failed(::llc::saveView(staticBytes, ::llc::view<cnst ::llc::u1_t>{staticValues})), "%s", "Could not serialize static array input.");
	::llc::vcu0_t staticInput{staticBytes.begin(), staticBytes.size()};
	::llc::astu1_t<2> staticOutput = {};
	cnst ::llc::err_t staticRead = ::llc::load(staticInput, staticOutput);
	LLC_TEST_CHECKF(errors, APOD_SERIALIZE_TEST_RESULT_STATIC_LOAD, staticRead != 0 || staticOutput[0] != staticValues[0] || staticOutput[1] != staticValues[1] || staticInput.size() != 0, "Result:%i, remaining:%u.", staticRead, staticInput.size());

	::llc::apod<::llc::u1_t> values = {};
	LLC_TEST_REQUIREF(errors, APOD_SERIALIZE_TEST_RESULT_ARRAY_SETUP, ::llc::failed(values.push_back(0x5678)), "%s", "Could not append array value.");
	cnst ::llc::view<cnst ::llc::u1_t> constView{values.begin(), values.size()};
	::llc::au0_t expectedView = {};
	LLC_TEST_REQUIREF(errors, APOD_SERIALIZE_TEST_RESULT_ARRAY_SETUP, ::llc::failed(::llc::saveView(expectedView, constView)), "%s", "Could not serialize reference view.");

	::llc::au0_t podBytes = {};
	cnst ::llc::err_t podSaved = ::llc::save(podBytes, POD);
	LLC_TEST_CHECKF(errors, APOD_SERIALIZE_TEST_RESULT_POD_SAVE, podSaved != szof(POD) || podBytes.size() != szof(POD), "Saved:%i, size:%u.", podSaved, podBytes.size());
	::llc::au0_t constBytes = {};
	cnst ::llc::err_t constSaved = ::llc::save(constBytes, constView);
	LLC_TEST_CHECKF(errors, APOD_SERIALIZE_TEST_RESULT_VIEW_SAVE, constSaved != (::llc::err_t)expectedView.size() || constBytes != expectedView, "Saved:%i, size:%u, expected:%u.", constSaved, constBytes.size(), expectedView.size());
	::llc::au0_t arrayBytes = {};
	cnst ::llc::err_t arraySaved = ::llc::save(arrayBytes, values);
	LLC_TEST_CHECKF(errors, APOD_SERIALIZE_TEST_RESULT_APOD_SAVE, arraySaved != (::llc::err_t)expectedView.size() || arrayBytes != expectedView, "Saved:%i, size:%u, expected:%u.", arraySaved, arrayBytes.size(), expectedView.size());

	cnst ::llc::view<cnst ::llc::view<cnst ::llc::u1_t>> nestedConst{&constView, 1};
	::llc::au0_t nestedConstBytes = {};
	cnst ::llc::err_t nestedConstSaved = ::llc::save(nestedConstBytes, nestedConst);
	LLC_TEST_CHECKF(errors, APOD_SERIALIZE_TEST_RESULT_NESTED_CONST_SAVE, nestedConstSaved != (::llc::err_t)expectedView.size() || nestedConstBytes != expectedView, "Saved:%i, size:%u.", nestedConstSaved, nestedConstBytes.size());
	cnst ::llc::view<::llc::u1_t> mutableView{values.begin(), values.size()};
	cnst ::llc::view<cnst ::llc::view<::llc::u1_t>> nestedMutable{&mutableView, 1};
	::llc::au0_t nestedMutableBytes = {};
	cnst ::llc::err_t nestedMutableSaved = ::llc::save(nestedMutableBytes, nestedMutable);
	LLC_TEST_CHECKF(errors, APOD_SERIALIZE_TEST_RESULT_NESTED_MUTABLE_SAVE, nestedMutableSaved != (::llc::err_t)expectedView.size() || nestedMutableBytes != expectedView, "Saved:%i, size:%u.", nestedMutableSaved, nestedMutableBytes.size());

	::llc::au0_t expected = {};
	LLC_TEST_REQUIREF(errors, APOD_SERIALIZE_TEST_RESULT_ARRAY_SETUP, ::llc::failed(::llc::savePOD(expected, POD)) || ::llc::failed(::llc::saveView(expected, values)) || ::llc::failed(::llc::savePOD(expected, POD)), "%s", "Could not serialize reference fields.");
	::llc::au0_t actual = {};
	cnst ::llc::err_t totalSaved = ::llc::save(actual, POD, values, POD);
	LLC_TEST_CHECKF(errors, APOD_SERIALIZE_TEST_RESULT_VARIADIC_SAVE, totalSaved != (::llc::err_t)expected.size() || actual != expected, "Saved:%i, size:%u, expected:%u.", totalSaved, actual.size(), expected.size());
	::llc::vcu0_t arrayInput{arrayBytes.begin(), arrayBytes.size()};
	::llc::apod<::llc::u1_t> loadedArray = {};
	cnst ::llc::err_t arrayLoaded = ::llc::load(arrayInput, loadedArray);
	LLC_TEST_CHECKF(errors, APOD_SERIALIZE_TEST_RESULT_APOD_LOAD, arrayLoaded != 0 || loadedArray != values || arrayInput.size() != 0, "Result:%i, size:%u, remaining:%u.", arrayLoaded, loadedArray.size(), arrayInput.size());
	::llc::vcu0_t input{actual.begin(), actual.size()};
	::llc::u1_t first = {}, last = {};
	::llc::apod<::llc::u1_t> middle = {};
	cnst ::llc::err_t totalLoaded = ::llc::load(input, first, middle, last);
	LLC_TEST_CHECKF(errors, APOD_SERIALIZE_TEST_RESULT_VARIADIC_LOAD, totalLoaded != 0 || first != POD || last != POD || middle != values || input.size() != 0, "Result:%i, values:%u/%u, count:%u, remaining:%u.", totalLoaded, first, last, middle.size(), input.size());
	input = {actual.begin(), szof(POD)};
	first = 0; last = 77;
	middle.clear();
	cnst ::llc::err_t shortResult = ::llc::load(input, first, middle, last);
	LLC_TEST_CHECKF(errors, APOD_SERIALIZE_TEST_RESULT_VARIADIC_STOP, shortResult >= 0 || first != POD || middle.size() != 0 || last != 77 || input.size() != 0, "Result:%i, first:%u, count:%u, last:%u, remaining:%u.", shortResult, first, middle.size(), last, input.size());
	rtrn 0;
}
