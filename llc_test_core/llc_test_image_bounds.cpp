#include "llc_image.h"
#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(IMAGE_BOUNDS_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(IMAGE_BOUNDS_TEST_RESULT, VALID, 0, "Valid image resize changed its result or ownership contract.");

template<typename T>
sttc ::llc::err_t testImageBoundsTyped(::llc::ATestError & results) {
	cnst T replacementValue = T(9);
	::llc::img<T> image;
	cnst ::llc::n2u2_t validSize = {2, 3}, emptySize = {0, 3};
	LLC_TEST_REQUIRE(results, IMAGE_BOUNDS_TEST_RESULT_VALID, image.resize(validSize) != 0);
	LLC_TEST_REQUIRE(results, IMAGE_BOUNDS_TEST_RESULT_VALID, image.resize(validSize, replacementValue) != 6);
	for(cnst T value : image.Texels)
		LLC_TEST_CHECK(results, IMAGE_BOUNDS_TEST_RESULT_VALID, value != replacementValue);
	LLC_TEST_REQUIRE(results, IMAGE_BOUNDS_TEST_RESULT_VALID, image.resize(emptySize) != 0 || image.size() || image.metrics() != emptySize);
	rtrn 0;
}

::llc::err_t testImageBounds(::llc::ATestError & results, ::llc::vcst_t) {
	rtrn ::llc::testTypePack(::llc::integer_bounds_types{}, [&](auto type) { rtrn testImageBoundsTyped<typename decltype(type)::type>(results); });
}
