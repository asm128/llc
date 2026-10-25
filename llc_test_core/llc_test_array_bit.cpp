#include "llc_test_core.h"
#include "llc_array_bit.h"

GDEFINE_ENUM_TYPE(ARRAY_BIT_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(ARRAY_BIT_TEST_RESULT, OK					,  0, "All array_bit<> tests passed.");
GDEFINE_ENUM_VALUED(ARRAY_BIT_TEST_RESULT, DEFAULT_SIZE			,  1, "A default array_bit<> reported a nonzero logical size.");
GDEFINE_ENUM_VALUED(ARRAY_BIT_TEST_RESULT, DEFAULT_STORAGE		,  2, "A default array_bit<> owned unexpected storage.");
GDEFINE_ENUM_VALUED(ARRAY_BIT_TEST_RESULT, DEFAULT_VIEW			,  3, "A default array_bit<> exposed a nonempty bit view.");
GDEFINE_ENUM_VALUED(ARRAY_BIT_TEST_RESULT, RESIZE_RESULT			,  4, "array_bit<>::resize() reported failure for a supported bit count.");
GDEFINE_ENUM_VALUED(ARRAY_BIT_TEST_RESULT, RESIZE_SIZE			,  5, "array_bit<>::resize() stored the wrong logical bit count.");
GDEFINE_ENUM_VALUED(ARRAY_BIT_TEST_RESULT, RESIZE_STORAGE		,  6, "array_bit<>::resize() allocated the wrong number of backing elements.");
GDEFINE_ENUM_VALUED(ARRAY_BIT_TEST_RESULT, RESIZE_BEGIN			,  7, "array_bit<>::resize() did not bind its view to its owned storage.");
GDEFINE_ENUM_VALUED(ARRAY_BIT_TEST_RESULT, RESIZE_END			,  8, "array_bit<>::resize() produced the wrong logical or physical end.");
GDEFINE_ENUM_VALUED(ARRAY_BIT_TEST_RESULT, BIT_WRITE				,  9, "Writing through array_bit<>::View did not update the selected owned bit.");
GDEFINE_ENUM_VALUED(ARRAY_BIT_TEST_RESULT, VIEW_SIZE				, 10, "Constructing or assigning array_bit<> from a view lost its logical bit count.");
GDEFINE_ENUM_VALUED(ARRAY_BIT_TEST_RESULT, VIEW_STORAGE			, 11, "Constructing or assigning array_bit<> from a view copied the wrong backing element count.");
GDEFINE_ENUM_VALUED(ARRAY_BIT_TEST_RESULT, VIEW_VALUE				, 12, "Constructing or assigning array_bit<> from a view copied the wrong backing value.");
GDEFINE_ENUM_VALUED(ARRAY_BIT_TEST_RESULT, VIEW_BINDING			, 13, "An array_bit<> built from a view did not bind to its owned copy.");
GDEFINE_ENUM_VALUED(ARRAY_BIT_TEST_RESULT, VIEW_INDEPENDENCE		, 14, "An array_bit<> built from a view still depended on the source storage.");
GDEFINE_ENUM_VALUED(ARRAY_BIT_TEST_RESULT, COPY_SIZE				, 15, "Copying array_bit<> lost its logical bit count.");
GDEFINE_ENUM_VALUED(ARRAY_BIT_TEST_RESULT, COPY_STORAGE			, 16, "Copying array_bit<> shared or lost its backing storage.");
GDEFINE_ENUM_VALUED(ARRAY_BIT_TEST_RESULT, COPY_VALUE				, 17, "Copying array_bit<> changed a logical bit value.");
GDEFINE_ENUM_VALUED(ARRAY_BIT_TEST_RESULT, COPY_BINDING			, 18, "A copied array_bit<> view remained bound to another owner.");
GDEFINE_ENUM_VALUED(ARRAY_BIT_TEST_RESULT, COPY_INDEPENDENCE		, 19, "Mutating a copied array_bit<> changed its source owner.");
GDEFINE_ENUM_VALUED(ARRAY_BIT_TEST_RESULT, SELF_VIEW_ASSIGNMENT	, 20, "Assigning array_bit<> from its own view corrupted its storage or binding.");
GDEFINE_ENUM_VALUED(ARRAY_BIT_TEST_RESULT, RESIZE_PRESERVATION	, 21, "Shrinking array_bit<> did not preserve every retained logical bit.");
GDEFINE_ENUM_VALUED(ARRAY_BIT_TEST_RESULT, EMPTY_ASSIGNMENT		, 22, "Assigning an empty bit view did not clear array_bit<>.");

tplt<tpnm T>
sttc ::llc::err_t testArrayBitResize(::llc::ATestError & errors) {
	stxp ::llc::u2_t ELEMENT_BITS = ::llc::view_bit<T>::ELEMENT_BITS;
	stxp ::llc::u2_t BIT_COUNTS[] = {0, 1, ELEMENT_BITS - 1, ELEMENT_BITS, ELEMENT_BITS + 1, ELEMENT_BITS * 2, ELEMENT_BITS * 2 + 3};
	::llc::array_bit<T> bits;
	LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_DEFAULT_SIZE, bits.size(), "%u-bit backing type, default logical size:%u, expected:0.", ELEMENT_BITS, bits.size());
	LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_DEFAULT_STORAGE, bits.Data.size(), "%u-bit backing type, default storage count:%u, expected:0.", ELEMENT_BITS, bits.Data.size());
	LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_DEFAULT_VIEW, bits.View.begin() != bits.View.end(), "%u-bit backing type, default view is not empty.", ELEMENT_BITS);
	for(::llc::u2_t iCount = 0; iCount < ::llc::size(BIT_COUNTS); ++iCount) {
		cnst ::llc::u2_t bitCount = BIT_COUNTS[iCount];
		cnst ::llc::u2_t elementCount = round_up(bitCount, ELEMENT_BITS);
		cnst ::llc::err_t resizeResult = bits.resize(bitCount);
		LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_RESIZE_RESULT, ::llc::failed(resizeResult), "%u-bit backing type, resize bit count:%u, result:%i.", ELEMENT_BITS, bitCount, resizeResult);
		LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_RESIZE_SIZE, bits.size() != bitCount, "%u-bit backing type, logical size:%u, expected:%u.", ELEMENT_BITS, bits.size(), bitCount);
		LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_RESIZE_STORAGE, bits.Data.size() != elementCount, "%u-bit backing type, bit count:%u, storage count:%u, expected:%u.", ELEMENT_BITS, bitCount, bits.Data.size(), elementCount);
		LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_RESIZE_BEGIN, bits.View.begin().Begin != (bitCount ? bits.Data.begin() : 0), "%u-bit backing type, bit count:%u, view begin:%p, expected:%p.", ELEMENT_BITS, bitCount, bits.View.begin().Begin, bitCount ? bits.Data.begin() : 0);
		LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_RESIZE_END, bits.View.end().Index() != bitCount, "%u-bit backing type, logical end:%u, expected:%u.", ELEMENT_BITS, bits.View.end().Index(), bitCount);
		LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_RESIZE_END, bits.View.end().End != (bitCount ? bits.Data.end() : 0), "%u-bit backing type, bit count:%u, physical end:%p, expected:%p.", ELEMENT_BITS, bitCount, bits.View.end().End, bitCount ? bits.Data.end() : 0);
		for(::llc::u2_t iBit = 0; iBit < bitCount; ++iBit) {
			cnst bool expected = 0 != (iBit % 3);
			bits.View[iBit] = expected;
			LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_BIT_WRITE, bool(bits.View[iBit]) != expected, "%u-bit backing type, bit count:%u, bit:%u, value:%u, expected:%u.", ELEMENT_BITS, bitCount, iBit, ::llc::u2_t(bool(bits.View[iBit])), ::llc::u2_t(expected));
		}
	}
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testArrayBitViewCopy(::llc::ATestError & errors) {
	stxp ::llc::u2_t ELEMENT_BITS = ::llc::view_bit<T>::ELEMENT_BITS;
	stxp ::llc::u2_t BIT_COUNT = ELEMENT_BITS * 2 + 3;
	T source[3] = {T(~T(0)), T(0x5A), T(5)};
	cnst ::llc::view_bit<T> sourceView{source, BIT_COUNT};
	::llc::array_bit<T> fromView = {sourceView};
	LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_VIEW_SIZE, fromView.size() != BIT_COUNT, "%u-bit backing type, constructed logical size:%u, expected:%u.", ELEMENT_BITS, fromView.size(), BIT_COUNT);
	LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_VIEW_STORAGE, fromView.Data.size() != ::llc::size(source), "%u-bit backing type, constructed storage count:%u, expected:%u.", ELEMENT_BITS, fromView.Data.size(), ::llc::size(source));
	LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_VIEW_BINDING, fromView.View.begin().Begin != fromView.Data.begin(), "%u-bit backing type, constructed view begin:%p, owned begin:%p.", ELEMENT_BITS, fromView.View.begin().Begin, fromView.Data.begin());
	for(::llc::u2_t iElement = 0; iElement < ::llc::size(source); ++iElement) {
		LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_VIEW_VALUE, fromView.Data[iElement] != source[iElement], "%u-bit backing type, constructed element:%u, value:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 ".", ELEMENT_BITS, iElement, ::llc::u3_t(fromView.Data[iElement]), ::llc::u3_t(source[iElement]));
	}
	cnst T ownedFirst = fromView.Data[0];
	source[0] ^= T(1);
	LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_VIEW_INDEPENDENCE, fromView.Data[0] != ownedFirst, "%u-bit backing type, owned first element changed from:%" LLC_FMT_U3 " to:%" LLC_FMT_U3 ".", ELEMENT_BITS, ::llc::u3_t(ownedFirst), ::llc::u3_t(fromView.Data[0]));

	::llc::array_bit<T> copyConstructed = fromView;
	LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_COPY_SIZE, copyConstructed.size() != fromView.size(), "%u-bit backing type, copy-constructed logical size:%u, expected:%u.", ELEMENT_BITS, copyConstructed.size(), fromView.size());
	LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_COPY_STORAGE, copyConstructed.Data.begin() == fromView.Data.begin(), "%u-bit backing type, copy-constructed storage:%p, source storage:%p.", ELEMENT_BITS, copyConstructed.Data.begin(), fromView.Data.begin());
	LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_COPY_BINDING, copyConstructed.View.begin().Begin != copyConstructed.Data.begin(), "%u-bit backing type, copy-constructed view begin:%p, owned begin:%p.", ELEMENT_BITS, copyConstructed.View.begin().Begin, copyConstructed.Data.begin());
	for(::llc::u2_t iBit = 0; iBit < BIT_COUNT; ++iBit) {
		LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_COPY_VALUE, bool(copyConstructed.View[iBit]) != bool(fromView.View[iBit]), "%u-bit backing type, copy-constructed bit:%u, value:%u, expected:%u.", ELEMENT_BITS, iBit, ::llc::u2_t(bool(copyConstructed.View[iBit])), ::llc::u2_t(bool(fromView.View[iBit])));
	}
	cnst bool sourceBit = fromView.View[0];
	copyConstructed.View[0] = !sourceBit;
	LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_COPY_INDEPENDENCE, bool(fromView.View[0]) != sourceBit, "%u-bit backing type, source bit changed after mutating copy construction. value:%u, expected:%u.", ELEMENT_BITS, ::llc::u2_t(bool(fromView.View[0])), ::llc::u2_t(sourceBit));

	::llc::array_bit<T> assignedView;
	assignedView = sourceView;
	LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_VIEW_SIZE, assignedView.size() != BIT_COUNT, "%u-bit backing type, view-assigned logical size:%u, expected:%u.", ELEMENT_BITS, assignedView.size(), BIT_COUNT);
	LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_VIEW_STORAGE, assignedView.Data.size() != ::llc::size(source), "%u-bit backing type, view-assigned storage count:%u, expected:%u.", ELEMENT_BITS, assignedView.Data.size(), ::llc::size(source));
	LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_VIEW_BINDING, assignedView.View.begin().Begin != assignedView.Data.begin(), "%u-bit backing type, view-assigned view begin:%p, owned begin:%p.", ELEMENT_BITS, assignedView.View.begin().Begin, assignedView.Data.begin());
	for(::llc::u2_t iElement = 0; iElement < ::llc::size(source); ++iElement) {
		LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_VIEW_VALUE, assignedView.Data[iElement] != source[iElement], "%u-bit backing type, view-assigned element:%u, value:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 ".", ELEMENT_BITS, iElement, ::llc::u3_t(assignedView.Data[iElement]), ::llc::u3_t(source[iElement]));
	}

	::llc::array_bit<T> copyAssigned;
	copyAssigned = assignedView;
	LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_COPY_SIZE, copyAssigned.size() != assignedView.size(), "%u-bit backing type, copy-assigned logical size:%u, expected:%u.", ELEMENT_BITS, copyAssigned.size(), assignedView.size());
	LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_COPY_STORAGE, copyAssigned.Data.begin() == assignedView.Data.begin(), "%u-bit backing type, copy-assigned storage:%p, source storage:%p.", ELEMENT_BITS, copyAssigned.Data.begin(), assignedView.Data.begin());
	LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_COPY_BINDING, copyAssigned.View.begin().Begin != copyAssigned.Data.begin(), "%u-bit backing type, copy-assigned view begin:%p, owned begin:%p.", ELEMENT_BITS, copyAssigned.View.begin().Begin, copyAssigned.Data.begin());
	for(::llc::u2_t iBit = 0; iBit < BIT_COUNT; ++iBit) {
		LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_COPY_VALUE, bool(copyAssigned.View[iBit]) != bool(assignedView.View[iBit]), "%u-bit backing type, copy-assigned bit:%u, value:%u, expected:%u.", ELEMENT_BITS, iBit, ::llc::u2_t(bool(copyAssigned.View[iBit])), ::llc::u2_t(bool(assignedView.View[iBit])));
	}
	cnst bool assignedBit = assignedView.View[1];
	copyAssigned.View[1] = !assignedBit;
	LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_COPY_INDEPENDENCE, bool(assignedView.View[1]) != assignedBit, "%u-bit backing type, source bit changed after mutating copy assignment. value:%u, expected:%u.", ELEMENT_BITS, ::llc::u2_t(bool(assignedView.View[1])), ::llc::u2_t(assignedBit));

	cnst T selfFirst = assignedView.Data[0];
	assignedView = assignedView.View;
	LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_SELF_VIEW_ASSIGNMENT, assignedView.size() != BIT_COUNT, "%u-bit backing type, self-view-assigned logical size:%u, expected:%u.", ELEMENT_BITS, assignedView.size(), BIT_COUNT);
	LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_SELF_VIEW_ASSIGNMENT, assignedView.Data[0] != selfFirst, "%u-bit backing type, self-view-assigned first element:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 ".", ELEMENT_BITS, ::llc::u3_t(assignedView.Data[0]), ::llc::u3_t(selfFirst));
	LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_SELF_VIEW_ASSIGNMENT, assignedView.View.begin().Begin != assignedView.Data.begin(), "%u-bit backing type, self-view-assigned view begin:%p, owned begin:%p.", ELEMENT_BITS, assignedView.View.begin().Begin, assignedView.Data.begin());

	assignedView = ::llc::view_bit<T>{};
	LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_EMPTY_ASSIGNMENT, assignedView.size(), "%u-bit backing type, empty-assigned logical size:%u, expected:0.", ELEMENT_BITS, assignedView.size());
	LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_EMPTY_ASSIGNMENT, assignedView.Data.size(), "%u-bit backing type, empty-assigned storage count:%u, expected:0.", ELEMENT_BITS, assignedView.Data.size());
	LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_EMPTY_ASSIGNMENT, assignedView.View.begin() != assignedView.View.end(), "%u-bit backing type, empty-assigned view is not empty.", ELEMENT_BITS);
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testArrayBitPreservation(::llc::ATestError & errors) {
	stxp ::llc::u2_t ELEMENT_BITS = ::llc::view_bit<T>::ELEMENT_BITS;
	stxp ::llc::u2_t LARGE_COUNT = ELEMENT_BITS * 2 + 3;
	stxp ::llc::u2_t SMALL_COUNT = ELEMENT_BITS + 2;
	::llc::array_bit<T> bits;
	if_fail_fe(bits.resize(LARGE_COUNT));
	for(::llc::u2_t iBit = 0; iBit < LARGE_COUNT; ++iBit) {
		bits.View[iBit] = 0 == (iBit % 2);
	}
	if_fail_fe(bits.resize(SMALL_COUNT));
	for(::llc::u2_t iBit = 0; iBit < SMALL_COUNT; ++iBit) {
		LLC_TEST_CHECKF(errors, ARRAY_BIT_TEST_RESULT_RESIZE_PRESERVATION, bool(bits.View[iBit]) != (0 == (iBit % 2)), "%u-bit backing type, retained bit:%u, value:%u, expected:%u.", ELEMENT_BITS, iBit, ::llc::u2_t(bool(bits.View[iBit])), ::llc::u2_t(0 == (iBit % 2)));
	}
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t testArrayBitType(::llc::ATestError & errors) {
	if_fail_fe(testArrayBitResize<T>(errors));
	if_fail_fe(testArrayBitViewCopy<T>(errors));
	return testArrayBitPreservation<T>(errors);
}

::llc::err_t testArrayBit(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	static_assert(::std::is_same_v<::llc::abitu0_t, ::llc::array_bit<::llc::u0_t>>);
	static_assert(::std::is_same_v<::llc::abitu1_t, ::llc::array_bit<::llc::u1_t>>);
	static_assert(::std::is_same_v<::llc::abitu2_t, ::llc::array_bit<::llc::u2_t>>);
	static_assert(::std::is_same_v<::llc::abitu3_t, ::llc::array_bit<::llc::u3_t>>);
	return ::llc::testTypePack(::llc::unsigned_integer_types{}, [&](auto type) { return testArrayBitType<typename decltype(type)::type>(errors); });
}
