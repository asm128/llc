#include "llc_test_core_suites.h"
#include "llc_block_container.h"

GDEFINE_ENUM_TYPE(TEST_BLOCK_CONTAINER_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_BLOCK_CONTAINER_RESULT, PACK      , 0, "Sequences share a block while sufficient space remains.");
GDEFINE_ENUM_VALUED(TEST_BLOCK_CONTAINER_RESULT, GROW      , 1, "A sequence starts a new block when the current blocks cannot hold it.");
GDEFINE_ENUM_VALUED(TEST_BLOCK_CONTAINER_RESULT, ROUNDTRIP , 2, "Saved block-container state loads without changing its serialized representation.");
GDEFINE_ENUM_VALUED(TEST_BLOCK_CONTAINER_RESULT, CLEAR     , 3, "Clearing a block container resets subsequent allocation to the first block.");

::llc::err_t testBlockContainer(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::block_container<::llc::u0_t, 4> blocks;
	cnst ::llc::u0_t first[] = {1, 2};
	cnst ::llc::u0_t second[] = {3, 4};
	cnst ::llc::u0_t third[] = {5};
	::llc::vcu0_t storedFirst, storedSecond, storedThird;
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_RESULT_PACK, 0 != blocks.push_sequence(first, ::llc::size(first), storedFirst));
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_RESULT_PACK, 0 != blocks.push_sequence(second, ::llc::size(second), storedSecond));
	LLC_TEST_CHECK(results, TEST_BLOCK_CONTAINER_RESULT_PACK, storedFirst.size() != 2 || storedSecond.size() != 2 || storedFirst[0] != 1 || storedFirst[1] != 2 || storedSecond[0] != 3 || storedSecond[1] != 4);
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_RESULT_GROW, 1 != blocks.push_sequence(third, ::llc::size(third), storedThird));
	LLC_TEST_CHECK(results, TEST_BLOCK_CONTAINER_RESULT_GROW, storedThird.size() != 1 || storedThird[0] != 5);

	::llc::au0_t saved, savedAgain;
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_RESULT_ROUNDTRIP, 0 != blocks.Save(saved));
	::llc::vcu0_t input = saved;
	::llc::block_container<::llc::u0_t, 4> loaded;
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_RESULT_ROUNDTRIP, 0 != loaded.Load(input));
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_RESULT_ROUNDTRIP, 0 != loaded.Save(savedAgain));
	LLC_TEST_CHECK(results, TEST_BLOCK_CONTAINER_RESULT_ROUNDTRIP, input.size() || saved.size() != savedAgain.size() || 0 != memcmp(saved.begin(), savedAgain.begin(), saved.size()));

	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_RESULT_CLEAR, 0 != blocks.clear());
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_RESULT_CLEAR, 0 != blocks.push_sequence(third, ::llc::size(third), storedThird));
	rtrn 0;
}
