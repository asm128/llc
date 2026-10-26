#include "llc_test_core_suites.h"
#include "llc_grid_copy.h"
#include "llc_color.h"

namespace {
	struct SGridCopyProbe {
		sttc bool ThrowOnAssign;
		::llc::u2_t Value = {};

		SGridCopyProbe & operator=(cnst SGridCopyProbe & other) {
			if(ThrowOnAssign)
				throw 0;
			Value = other.Value;
			rtrn *this;
		}
		bool operator==(cnst SGridCopyProbe & other) cnst { rtrn Value == other.Value; }
		bool operator!=(cnst SGridCopyProbe & other) cnst { rtrn Value != other.Value; }
	};

	bool SGridCopyProbe::ThrowOnAssign = false;
}

GDEFINE_ENUM_TYPE(TEST_GRID_COPY_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_GRID_COPY_RESULT, OVERLOAD_COPY  , 0, "The destination-offset/source-rectangle copy overload instantiates and copies its selected cell.");
GDEFINE_ENUM_VALUED(TEST_GRID_COPY_RESULT, OVERLOAD_BLEND , 1, "The destination-offset/source-rectangle blend overload instantiates and blends its selected cell.");
GDEFINE_ENUM_VALUED(TEST_GRID_COPY_RESULT, VERTICAL_COPY  , 2, "A vertically clipped copy returns without touching storage outside the logical destination grid.");
GDEFINE_ENUM_VALUED(TEST_GRID_COPY_RESULT, VERTICAL_ALPHA , 3, "A vertically clipped alpha copy returns without touching storage outside the logical destination grid.");

sttc ::llc::err_t testGridCopyOverloadCopy(::llc::ATestError & results) {
	::llc::u2_t destinationStorage[1][1] = {};
	::llc::u2_t sourceStorage[1][1] = {{7}};
	::llc::grid<::llc::u2_t> destination = destinationStorage;
	::llc::grid<::llc::u2_t> source = sourceStorage;
	cnst ::llc::rect2s2_t sourceRectangle = {{}, {1, 1}};
	LLC_TEST_REQUIRE(results, TEST_GRID_COPY_RESULT_OVERLOAD_COPY, 1 != ::llc::grid_copy(destination, source, ::llc::n2s2_t{}, sourceRectangle));
	LLC_TEST_CHECK(results, TEST_GRID_COPY_RESULT_OVERLOAD_COPY, destinationStorage[0][0] != 7);
	rtrn 0;
}

sttc ::llc::err_t testGridCopyOverloadBlend(::llc::ATestError & results) {
	::llc::bgra destinationStorage[1][1] = {};
	::llc::bgra sourceStorage[1][1] = {};
	destinationStorage[0][0] = {9, 8, 7, 255};
	sourceStorage[0][0] = {1, 2, 3, 255};
	::llc::grid<::llc::bgra> destination = destinationStorage;
	::llc::grid<::llc::bgra> source = sourceStorage;
	cnst ::llc::rect2s2_t sourceRectangle = {{}, {1, 1}};
	LLC_TEST_REQUIRE(results, TEST_GRID_COPY_RESULT_OVERLOAD_BLEND, 1 != ::llc::grid_copy_blend(destination, source, ::llc::n2s2_t{}, sourceRectangle));
	LLC_TEST_CHECK(results, TEST_GRID_COPY_RESULT_OVERLOAD_BLEND, destinationStorage[0][0] != sourceStorage[0][0]);
	rtrn 0;
}

sttc ::llc::err_t testGridCopyVerticalCopy(::llc::ATestError & results) {
	SGridCopyProbe destinationStorage[4] = {};
	SGridCopyProbe sourceStorage[1] = {};
	sourceStorage[0].Value = 1;
	::llc::grid<SGridCopyProbe> destination = {destinationStorage, 1, 1};
	::llc::grid<SGridCopyProbe> source = {sourceStorage, 1, 1};
	cnst ::llc::rect2s2_t sourceRectangle = {{}, {1, 1}};
	bool touchedOutside = false;
	::llc::err_t copyResult = 0;
	SGridCopyProbe::ThrowOnAssign = true;
	try { copyResult = ::llc::grid_copy(destination, source, ::llc::n2s2_t{0, 2}, sourceRectangle); }
	catch(...) { touchedOutside = true; }
	SGridCopyProbe::ThrowOnAssign = false;
	LLC_TEST_CHECK(results, TEST_GRID_COPY_RESULT_VERTICAL_COPY, touchedOutside || copyResult != 0);
	rtrn 0;
}

sttc ::llc::err_t testGridCopyVerticalAlpha(::llc::ATestError & results) {
	SGridCopyProbe destinationStorage[4] = {};
	SGridCopyProbe sourceStorage[1] = {};
	SGridCopyProbe comparand = {};
	sourceStorage[0].Value = 1;
	::llc::grid<SGridCopyProbe> destination = {destinationStorage, 1, 1};
	::llc::grid<SGridCopyProbe> source = {sourceStorage, 1, 1};
	cnst ::llc::rect2s2_t sourceRectangle = {{}, {1, 1}};
	bool touchedOutside = false;
	::llc::err_t copyResult = 0;
	SGridCopyProbe::ThrowOnAssign = true;
	try { copyResult = ::llc::grid_copy_alpha(destination, source, ::llc::n2s2_t{0, 2}, sourceRectangle, comparand); }
	catch(...) { touchedOutside = true; }
	SGridCopyProbe::ThrowOnAssign = false;
	LLC_TEST_CHECK(results, TEST_GRID_COPY_RESULT_VERTICAL_ALPHA, touchedOutside || copyResult != 0);
	rtrn 0;
}

::llc::err_t testGridCopy(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testGridCopyOverloadCopy(results));
	result = ::llc::min(result, ::testGridCopyOverloadBlend(results));
	result = ::llc::min(result, ::testGridCopyVerticalCopy(results));
	result = ::llc::min(result, ::testGridCopyVerticalAlpha(results));
	rtrn result;
}
