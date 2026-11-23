#include "llc_particle.h"
#include "test_llc_suites.h"

GDEFINE_ENUM_TYPE(PARTICLE_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(PARTICLE_TEST_RESULT, ADD_INDEX		, 0, "SParticle2Integrator::AddParticle() returned the wrong new index.");
GDEFINE_ENUM_VALUED(PARTICLE_TEST_RESULT, ARRAY_SIZES		, 1, "SParticle2Integrator::AddParticle() desynchronized its parallel arrays.");
GDEFINE_ENUM_VALUED(PARTICLE_TEST_RESULT, PARTICLE_DATA	, 2, "SParticle2Integrator::AddParticle() did not initialize both particle buffers.");
GDEFINE_ENUM_VALUED(PARTICLE_TEST_RESULT, PARTICLE_STATE	, 3, "SParticle2Integrator::AddParticle() did not initialize the active state.");
GDEFINE_ENUM_VALUED(PARTICLE_TEST_RESULT, REUSE_INDEX		, 4, "SParticle2Integrator::AddParticle() did not recycle an unused slot.");
GDEFINE_ENUM_VALUED(PARTICLE_TEST_RESULT, REUSE_SIZE		, 5, "SParticle2Integrator::AddParticle() grew arrays while recycling a slot.");
GDEFINE_ENUM_VALUED(PARTICLE_TEST_RESULT, REUSE_DATA		, 6, "SParticle2Integrator::AddParticle() did not replace both recycled particle buffers.");
GDEFINE_ENUM_VALUED(PARTICLE_TEST_RESULT, CREATE_INDEX		, 7, "SParticles3::Create() returned the wrong index.");
GDEFINE_ENUM_VALUED(PARTICLE_TEST_RESULT, CREATE_SIZES		, 8, "SParticles3::Create() desynchronized its parallel arrays.");
GDEFINE_ENUM_VALUED(PARTICLE_TEST_RESULT, CREATE_DATA		, 9, "SParticles3::Create() did not initialize the new particle.");

::llc::err_t test_particle(STestRunnerState &, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	::llc::SParticle2Integrator<::llc::f2_t>	integrator		= {};
	::llc::SParticle2<::llc::f2_t>			firstParticle	= {};
	firstParticle.Position					= {1, 2};
	firstParticle.Forces.Velocity			= {3, 4};
	firstParticle.InverseMass				= .5f;
	firstParticle.Damping					= .9f;
	LLC_TEST_CHECK(errors, PARTICLE_TEST_RESULT_ADD_INDEX, integrator.AddParticle(firstParticle) != 0);
	LLC_TEST_CHECK(errors, PARTICLE_TEST_RESULT_ARRAY_SIZES, integrator.ParticleState.size() != 1 || integrator.Particle.size() != 1 || integrator.ParticleNext.size() != 1);
	LLC_TEST_CHECK(errors, PARTICLE_TEST_RESULT_PARTICLE_DATA
		, integrator.Particle[0].Position != firstParticle.Position
		|| integrator.Particle[0].Forces.Velocity != firstParticle.Forces.Velocity
		|| integrator.ParticleNext[0].Position != firstParticle.Position
		|| integrator.ParticleNext[0].Forces.Velocity != firstParticle.Forces.Velocity
		);
	LLC_TEST_CHECK(errors, PARTICLE_TEST_RESULT_PARTICLE_STATE, integrator.ParticleState[0].Unused || false == integrator.ParticleState[0].Active);

	::llc::SParticle2<::llc::f2_t>			secondParticle	= firstParticle;
	secondParticle.Position					= {5, 6};
	if_fail_fe(integrator.AddParticle(secondParticle));
	integrator.ParticleState[0].Unused		= true;
	::llc::SParticle2<::llc::f2_t>			recycledParticle	= firstParticle;
	recycledParticle.Position				= {7, 8};
	LLC_TEST_CHECK(errors, PARTICLE_TEST_RESULT_REUSE_INDEX, integrator.AddParticle(recycledParticle) != 0);
	LLC_TEST_CHECK(errors, PARTICLE_TEST_RESULT_REUSE_SIZE, integrator.ParticleState.size() != 2 || integrator.Particle.size() != 2 || integrator.ParticleNext.size() != 2);
	LLC_TEST_CHECK(errors, PARTICLE_TEST_RESULT_REUSE_DATA
		, integrator.Particle[0].Position != recycledParticle.Position
		|| integrator.ParticleNext[0].Position != recycledParticle.Position
		|| integrator.ParticleState[0].Unused
		|| false == integrator.ParticleState[0].Active
		);

	::llc::SParticles3	particles3	= {};
	cnst ::llc::n3f2_t	position0	= {1, 2, 3};
	cnst ::llc::n3f2_t	direction0	= {4, 5, 6};
	LLC_TEST_CHECK(errors, PARTICLE_TEST_RESULT_CREATE_INDEX, particles3.Create(position0, direction0, 7) != 0);
	cnst ::llc::n3f2_t	position1	= {8, 9, 10};
	cnst ::llc::n3f2_t	direction1	= {11, 12, 13};
	LLC_TEST_CHECK(errors, PARTICLE_TEST_RESULT_CREATE_INDEX, particles3.Create(position1, direction1, 14) != 1);
	LLC_TEST_CHECK(errors, PARTICLE_TEST_RESULT_CREATE_SIZES, particles3.Position.size() != 2 || particles3.Direction.size() != 2 || particles3.Speed.size() != 2);
	LLC_TEST_CHECK(errors, PARTICLE_TEST_RESULT_CREATE_DATA, particles3.Position[1] != position1 || particles3.Direction[1] != direction1 || particles3.Speed[1] != 14);
	return 0;
}
