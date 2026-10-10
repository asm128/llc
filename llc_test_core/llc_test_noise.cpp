#include "llc_test_core.h"
#include "llc_noise.h"

GDEFINE_ENUM_TYPE(PRNG_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(PRNG_TEST_RESULT, OK		, 0, "All pseudorandom-generator tests passed.");
GDEFINE_ENUM_VALUED(PRNG_TEST_RESULT, SEQUENCE	, 1, "SPRNG did not produce the SplitMix64 reference sequence.");
GDEFINE_ENUM_VALUED(PRNG_TEST_RESULT, STATE		, 2, "SPRNG did not track its position or last value.");
GDEFINE_ENUM_VALUED(PRNG_TEST_RESULT, RESET		, 3, "SPRNG reset did not restore its initial state and sequence.");
GDEFINE_ENUM_VALUED(PRNG_TEST_RESULT, SEED		, 4, "Different SPRNG seeds produced the same first value.");

::llc::err_t testSPRNG(ATestError & errors) {
	stxp ::llc::u3_t EXPECTED[] =
		{ 0xE220A8397B1DCDAFULL
		, 0x6E789E6AA1B965F4ULL
		, 0x06C45D188009454FULL
		, 0xF88BB8A8724C81ECULL
		};
	::llc::SPRNG random = {0};
	for(::llc::u2_t iValue = 0; iValue < ::llc::size(EXPECTED); ++iValue) {
		cnst ::llc::u3_t value = random.Next();
		LLC_TEST_CHECKF(errors, PRNG_TEST_RESULT_SEQUENCE, value != EXPECTED[iValue] , "SplitMix64 sequence mismatch. seed:%" LLC_FMT_U3 ", position:%u, value:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 "." , random.Seed, iValue + 1, value, EXPECTED[iValue] );
		LLC_TEST_CHECKF(errors, PRNG_TEST_RESULT_STATE, random.Position != iValue + 1 , "SPRNG position:%" LLC_FMT_U3 ", expected:%u.", random.Position, iValue + 1 );
		LLC_TEST_CHECKF(errors, PRNG_TEST_RESULT_STATE, random.Value != value , "SPRNG stored value:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 ".", random.Value, value );
	}
	random.Reset();
	LLC_TEST_CHECKF(errors, PRNG_TEST_RESULT_RESET, random.Seed , "SPRNG reset seed:%" LLC_FMT_U3 ", expected:0.", random.Seed );
	LLC_TEST_CHECKF(errors, PRNG_TEST_RESULT_RESET, random.Position , "SPRNG reset position:%" LLC_FMT_U3 ", expected:0.", random.Position );
	LLC_TEST_CHECKF(errors, PRNG_TEST_RESULT_RESET, random.Value , "SPRNG reset value:%" LLC_FMT_U3 ", expected:0.", random.Value );
	cnst ::llc::u3_t resetValue = random.Next();
	LLC_TEST_CHECKF(errors, PRNG_TEST_RESULT_RESET, resetValue != EXPECTED[0] , "SPRNG first value after reset:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 ".", resetValue, EXPECTED[0] );
	random.Reset(1);
	cnst ::llc::u3_t seededValue = random.Next();
	random.Reset(0);
	LLC_TEST_CHECKF(errors, PRNG_TEST_RESULT_SEED, seededValue == random.Next() , "SPRNG seed did not affect its first value. seed 1 value:%" LLC_FMT_U3 ", seed 0 value:%" LLC_FMT_U3 "." , seededValue, random.Value );
	return 0;
}
