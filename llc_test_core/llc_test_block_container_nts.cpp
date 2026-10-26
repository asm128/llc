#include "llc_test_core_suites.h"
#include "llc_block_container_nts.h"

GDEFINE_ENUM_TYPE(TEST_BLOCK_CONTAINER_NTS_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_BLOCK_CONTAINER_NTS_RESULT, TERMINATOR , 0, "Stored text reserves and writes a trailing null outside its returned view.");
GDEFINE_ENUM_VALUED(TEST_BLOCK_CONTAINER_NTS_RESULT, PACK       , 1, "Null-terminated sequences pack while their reserved extents fit.");
GDEFINE_ENUM_VALUED(TEST_BLOCK_CONTAINER_NTS_RESULT, ROUNDTRIP  , 2, "Saved null-terminated block state loads without changing its serialization.");

::llc::err_t testBlockContainerNTS(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::block_container_nts<8> strings;
	::llc::vcst_t first, second, third;
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_NTS_RESULT_PACK, 0 != strings.push_sequence("one", 3, first));
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_NTS_RESULT_PACK, 0 != strings.push_sequence("go", 2, second));
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_NTS_RESULT_PACK, 1 != strings.push_sequence("four", 4, third));
	LLC_TEST_CHECK(results, TEST_BLOCK_CONTAINER_NTS_RESULT_TERMINATOR, first.size() != 3 || second.size() != 2 || third.size() != 4 || first.begin()[first.size()] || second.begin()[second.size()] || third.begin()[third.size()]);
	LLC_TEST_CHECK(results, TEST_BLOCK_CONTAINER_NTS_RESULT_PACK, first != "one" || second != "go" || third != "four");

	::llc::au0_t saved, savedAgain;
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_NTS_RESULT_ROUNDTRIP, 0 != strings.Save(saved));
	::llc::vcu0_t input = saved;
	::llc::block_container_nts<8> loaded;
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_NTS_RESULT_ROUNDTRIP, 0 != loaded.Load(input));
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_NTS_RESULT_ROUNDTRIP, 0 != loaded.Save(savedAgain));
	LLC_TEST_CHECK(results, TEST_BLOCK_CONTAINER_NTS_RESULT_ROUNDTRIP, input.size() || saved.size() != savedAgain.size() || 0 != memcmp(saved.begin(), savedAgain.begin(), saved.size()));
	rtrn 0;
}
