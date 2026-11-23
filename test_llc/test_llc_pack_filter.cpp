#include "llc_pack.h"
#include "test_llc_suites.h"

GDEFINE_ENUM_TYPE(PACK_FILTER_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(PACK_FILTER_TEST_RESULT, WRAPPER_RESULT	, 0, "A numbered pack-filter wrapper returned the wrong element count.");
GDEFINE_ENUM_VALUED(PACK_FILTER_TEST_RESULT, WRAPPER_OUTPUT	, 1, "A numbered pack-filter wrapper did not append converted elements.");
GDEFINE_ENUM_VALUED(PACK_FILTER_TEST_RESULT, DISPATCH_RESULT	, 2, "packFilter() returned the wrong element count for a valid filter.");
GDEFINE_ENUM_VALUED(PACK_FILTER_TEST_RESULT, DISPATCH_OUTPUT	, 3, "packFilter() did not append converted elements for a valid filter.");
GDEFINE_ENUM_VALUED(PACK_FILTER_TEST_RESULT, INVALID_RESULT	, 4, "packFilter() accepted an invalid filter value.");
GDEFINE_ENUM_VALUED(PACK_FILTER_TEST_RESULT, INVALID_OUTPUT	, 5, "packFilter() changed the output for an invalid filter value.");

using FPackFilter = ::llc::err_t (*)(::llc::apod<::llc::u1_t> &, ::llc::vcu0_t);

::llc::err_t test_pack_filter(STestRunnerState &, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	sttc cnst FPackFilter wrappers[] =
		{ &::llc::packFilter0<::llc::u0_t, ::llc::u1_t>
		, &::llc::packFilter1<::llc::u0_t, ::llc::u1_t>
		, &::llc::packFilter2<::llc::u0_t, ::llc::u1_t>
		, &::llc::packFilter3<::llc::u0_t, ::llc::u1_t>
		, &::llc::packFilter4<::llc::u0_t, ::llc::u1_t>
		};
	cnst ::llc::u0_t input[] = {1, 127, 255};

	for(::llc::u0_t iFilter = 0; iFilter < 5; ++iFilter) {
		::llc::apod<::llc::u1_t> wrapperOutput = {};
		if_fail_fe(wrapperOutput.push_back(0x1FF));
		cnst ::llc::err_t wrapperResult = wrappers[iFilter](wrapperOutput, input);
		LLC_TEST_CHECK(errors, PACK_FILTER_TEST_RESULT_WRAPPER_RESULT, wrapperResult != 3);
		LLC_TEST_CHECK(errors, PACK_FILTER_TEST_RESULT_WRAPPER_OUTPUT, wrapperOutput.size() != 4 || wrapperOutput[0] != 0x1FF || wrapperOutput[1] != 1 || wrapperOutput[2] != 127 || wrapperOutput[3] != 255);

		::llc::apod<::llc::u1_t> dispatchOutput = {};
		if_fail_fe(dispatchOutput.push_back(0x1FF));
		cnst ::llc::err_t dispatchResult = ::llc::packFilter(dispatchOutput, ::llc::vcu0_t{input}, (::llc::PACK_FILTER)iFilter);
		LLC_TEST_CHECK(errors, PACK_FILTER_TEST_RESULT_DISPATCH_RESULT, dispatchResult != 3);
		LLC_TEST_CHECK(errors, PACK_FILTER_TEST_RESULT_DISPATCH_OUTPUT, dispatchOutput.size() != 4 || dispatchOutput[0] != 0x1FF || dispatchOutput[1] != 1 || dispatchOutput[2] != 127 || dispatchOutput[3] != 255);
	}

	::llc::apod<::llc::u1_t> invalidOutput = {};
	if_fail_fe(invalidOutput.push_back(0x1FF));
	cnst ::llc::err_t invalidResult = ::llc::packFilter(invalidOutput, ::llc::vcu0_t{input}, (::llc::PACK_FILTER)0xFF);
	LLC_TEST_CHECK(errors, PACK_FILTER_TEST_RESULT_INVALID_RESULT, invalidResult >= 0);
	LLC_TEST_CHECK(errors, PACK_FILTER_TEST_RESULT_INVALID_OUTPUT, invalidOutput.size() != 1 || invalidOutput[0] != 0x1FF);
	return 0;
}
