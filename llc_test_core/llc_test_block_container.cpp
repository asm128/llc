#include "llc_test_core_suites.h"
#include "llc_block_container.h"

GDEFINE_ENUM_TYPE(TEST_BLOCK_CONTAINER_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_BLOCK_CONTAINER_RESULT, PACK      , 0, "Sequences share a block while sufficient space remains.");
GDEFINE_ENUM_VALUED(TEST_BLOCK_CONTAINER_RESULT, GROW      , 1, "A sequence starts a new block when the current blocks cannot hold it.");
GDEFINE_ENUM_VALUED(TEST_BLOCK_CONTAINER_RESULT, ROUNDTRIP , 2, "Saved block-container state loads without changing its serialized representation.");
GDEFINE_ENUM_VALUED(TEST_BLOCK_CONTAINER_RESULT, CLEAR     , 3, "Clearing a block container resets subsequent allocation to the first block.");

sttc ::llc::err_t testBlockContainerPack(::llc::ATestError & results) {
	::llc::block_container<::llc::u0_t, 4> blocks;
	cnst ::llc::u0_t first[] = {1, 2};
	cnst ::llc::u0_t second[] = {3, 4};
	::llc::vcu0_t storedFirst, storedSecond;
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_RESULT_PACK, 0 != blocks.push_sequence(first, ::llc::size(first), storedFirst));
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_RESULT_PACK, 0 != blocks.push_sequence(second, ::llc::size(second), storedSecond));
	LLC_TEST_CHECK(results, TEST_BLOCK_CONTAINER_RESULT_PACK, storedFirst.size() != 2 || storedSecond.size() != 2 || storedFirst[0] != 1 || storedFirst[1] != 2 || storedSecond[0] != 3 || storedSecond[1] != 4);
	rtrn 0;
}

sttc ::llc::err_t testBlockContainerGrow(::llc::ATestError & results) {
	::llc::block_container<::llc::u0_t, 4> blocks;
	cnst ::llc::u0_t full[] = {1, 2, 3, 4};
	cnst ::llc::u0_t next[] = {5};
	::llc::vcu0_t storedFull, storedNext;
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_RESULT_GROW, 0 != blocks.push_sequence(full, ::llc::size(full), storedFull));
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_RESULT_GROW, 1 != blocks.push_sequence(next, ::llc::size(next), storedNext));
	LLC_TEST_CHECK(results, TEST_BLOCK_CONTAINER_RESULT_GROW, storedNext.size() != 1 || storedNext[0] != 5);
	rtrn 0;
}

sttc ::llc::err_t testBlockContainerRoundtrip(::llc::ATestError & results) {
	::llc::block_container<::llc::u0_t, 4> blocks;
	cnst ::llc::u0_t first[] = {1, 2, 3, 4};
	cnst ::llc::u0_t second[] = {5};
	::llc::vcu0_t stored;
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_RESULT_ROUNDTRIP, 0 != blocks.push_sequence(first, ::llc::size(first), stored));
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_RESULT_ROUNDTRIP, 1 != blocks.push_sequence(second, ::llc::size(second), stored));
	::llc::au0_t saved, savedAgain;
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_RESULT_ROUNDTRIP, 0 != blocks.Save(saved));
	::llc::vcu0_t input = saved;
	::llc::block_container<::llc::u0_t, 4> loaded;
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_RESULT_ROUNDTRIP, 0 != loaded.Load(input));
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_RESULT_ROUNDTRIP, 0 != loaded.Save(savedAgain));
	LLC_TEST_CHECK(results, TEST_BLOCK_CONTAINER_RESULT_ROUNDTRIP, input.size() || saved.size() != savedAgain.size() || 0 != memcmp(saved.begin(), savedAgain.begin(), saved.size()));
	rtrn 0;
}

sttc ::llc::err_t testBlockContainerClear(::llc::ATestError & results) {
	::llc::block_container<::llc::u0_t, 4> blocks;
	cnst ::llc::u0_t first[] = {1, 2, 3, 4};
	cnst ::llc::u0_t second[] = {5};
	::llc::vcu0_t stored;
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_RESULT_CLEAR, 0 != blocks.push_sequence(first, ::llc::size(first), stored));
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_RESULT_CLEAR, 0 != blocks.clear());
	LLC_TEST_REQUIRE(results, TEST_BLOCK_CONTAINER_RESULT_CLEAR, 0 != blocks.push_sequence(second, ::llc::size(second), stored));
	LLC_TEST_CHECK(results, TEST_BLOCK_CONTAINER_RESULT_CLEAR, stored.size() != 1 || stored[0] != 5);
	rtrn 0;
}

::llc::err_t testBlockContainer(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testBlockContainerPack(results));
	result = ::llc::min(result, ::testBlockContainerGrow(results));
	result = ::llc::min(result, ::testBlockContainerRoundtrip(results));
	result = ::llc::min(result, ::testBlockContainerClear(results));
	rtrn result;
}
