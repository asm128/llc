#include "llc_image.h"
#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(GRID_IMAGE_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, RESIZE_RESULT	, 0, "img<>::resize() rejected valid metrics.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, METRICS		, 1, "img<> did not publish the requested metrics.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, INITIAL_VALUE	, 2, "img<>::resize() did not initialize every texel.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, COPY_STORAGE	, 3, "img<> copy construction did not create independent storage.");

::llc::err_t testGridImage(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	cnst ::llc::n2u2_t metrics = {3, 2};
	::llc::imgu0_t image;
	cnst ::llc::err_t result = image.resize(metrics, 7);
	LLC_TEST_REQUIRE(errors, GRID_IMAGE_TEST_RESULT_RESIZE_RESULT, ::llc::failed(result));
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_METRICS, image.metrics() != metrics || image.size() != 6);
	bool invalidValue = false;
	for(::llc::u2_t iTexel = 0; iTexel < image.size(); ++iTexel)
		invalidValue |= 7 != image.begin()[iTexel];
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_INITIAL_VALUE, invalidValue);
	::llc::imgu0_t copied = image;
	image.begin()[0] = 9;
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_COPY_STORAGE, copied.begin() == image.begin() || copied.begin()[0] != 7);
	rtrn 0;
}
