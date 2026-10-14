#include "llc_apod_n3.h"

#ifndef LLC_ATMEL		
#	include <cfloat>
#else
#	include <float.h>
#endif
#ifndef LLC_PARTICLE_H_23627
#define LLC_PARTICLE_H_23627

namespace llc
{
#pragma pack(push, 1)
	tplt<tpnm _tElement>
	struct SParticle2Forces {
		typedef	::llc::n2<_tElement>	TCoord;

		stacxpr	const _tElement		VelocityEpsilon				= 0.0001f;

		TCoord						AccumulatedForce			= {};
		TCoord						Acceleration				= {};	// A vector representing the speed in a given direction
		TCoord						Velocity					= {};	// A vector representing the speed in a given direction

		inlcxpr	bool				VelocityDepleted			()				const	noexcept	{ return (Velocity + Acceleration).LengthSquared() < (VelocityEpsilon * VelocityEpsilon); }
		// This basically does Acceleration += (Force * 1 / Mass) and Velocity += (Acceleration * Time).
		void						IntegrateAccumulatedForce	(const _tElement inverseMass, const _tElement damping, const double timeElapsed)			noexcept	{
			Acceleration				+= AccumulatedForce * inverseMass;				// Calculate linear acceleration from force inputs.
			AccumulatedForce			= {};											// Clear this out now that we've used it already.

			Velocity					+= Acceleration * timeElapsed;					// Update linear velocity from both acceleration and impulse.
			Velocity					*= ::pow(damping, timeElapsed);					// Impose drag.
		}
	};

	tplt<tpnm _tElement>
	struct SParticle2 {
		// The member variables are organized such that matches the order in which they are used.
		::llc::n2<_tElement>		Position					= {};
		SParticle2Forces<_tElement>	Forces						= {};
		_tElement					InverseMass					= 0;
		_tElement					Damping						= .99f;	// A vector representing the speed in a given direction

		inline	void				SetMass						(const double mass)																			noexcept	{ InverseMass = mass ? ((_tElement)(1.0 / mass)) : 0;		}
		inlcxpr	double				GetMass						()				const	noexcept	{ return (InverseMass == 0) ? DBL_MAX : 1.0 / InverseMass;	}
		inlcxpr	bool				HasFiniteMass				()				const	noexcept	{ return InverseMass >= 0.0f;								}
	};

	// This compact structure allows to define all the boolean states of the particle packed in a single byte.
	struct SParticle2State {
		bool					Unused						: 1;
		bool					Active						: 1;

		inlcxpr	bool			RequiresProcessing			()				const	noexcept	{ return (false == Unused) && Active; }
	};
#pragma pack(pop)
	// This basically does FinalPosition = InitialPosition + Velocity * Time.
	tplt<tpnm _tElement>
	stainli	void			particleIntegratePosition
		(	const ::llc::n2<_tElement>	& velocity
		,	const double				timeElapsed
		,	const double				timeElapsedHalfSquared
		,	::llc::n2<_tElement>		& position
		) {
		position				+= velocity * timeElapsed;
		position				+= velocity * timeElapsedHalfSquared;
	}

	tplt<tpnm _tElement>
	::llc::error_t			integrate					(const ::llc::view<::llc::SParticle2<_tElement>>& particles, ::llc::apod<::llc::SParticle2State>& particleStates, ::llc::view<::llc::SParticle2<_tElement>>& particlesNext, double timeElapsed, double timeElapsedHalfSquared)			{
		for(uint32_t iParticle = 0, particleCount = (uint32_t)particleStates.size(); iParticle < particleCount; ++iParticle)
			if(particleStates[iParticle].RequiresProcessing()) {
				::llc::SParticle2<_tElement>												& particleNext								= particlesNext[iParticle] = particles[iParticle];	// Copy the current particle state to the next
				::llc::particleIntegratePosition(particleNext.Forces.Velocity, timeElapsed, timeElapsedHalfSquared, particleNext.Position);
				particleNext.Forces.IntegrateAccumulatedForce(particleNext.InverseMass, particleNext.Damping, timeElapsed);
				if(particleNext.Forces.VelocityDepleted())
					particleStates[iParticle].Active										= false;
			}
		return 0;
	}

	tplt<tpnm _tElement>
	struct SParticle2Integrator {
		typedef	::llc::SParticle2	<_tElement>	TParticle;
		typedef	::llc::n2			<_tElement>	TCoord;

		::llc::apod<::llc::SParticle2State>		ParticleState								= {};
		::llc::apod<TParticle>					Particle									= {};
		::llc::apod<TParticle>					ParticleNext								= {};
		// -----------------------------------------------------	---
		inline	::llc::error_t					Integrate									(double timeElapsed, double timeElapsedHalfSquared)														{ return Integrate(ParticleNext, timeElapsed, timeElapsedHalfSquared);	}
		::llc::error_t							Integrate									(::llc::view<TParticle>& particleNext, double timeElapsed, double timeElapsedHalfSquared)			{ return ::llc::integrate(Particle, ParticleState, particleNext, timeElapsed, timeElapsedHalfSquared);		}
		// --------------------------------------------------------
		::llc::error_t							AddParticle					(const TParticle& particleData)																			{
			const uint32_t								particleCount								= (uint32_t)ParticleState.size();
			stacxpr	const ::llc::SParticle2State		initialParticleState						= {false, true};

			for(uint32_t iBody = 0; iBody < particleCount; ++iBody)	// Check if there is any unused particle that we can recycle.
				if( ParticleState	[iBody].Unused ) {
					ParticleState	[iBody]													= initialParticleState;
					Particle		[iBody]													=
					ParticleNext	[iBody]													= particleData;
					return iBody;
				}
#if defined(LLC_DISABLE_CPP_EXCEPTIONS)
			ParticleState		.push_back(initialParticleState);
			Particle			.push_back(particleData);
			ParticleNext		.push_back(particleData);
#else
			try {	// Later on we're going to add a way to avoid using ::std::vector which require these ugly try/catch blocks.
				ParticleState		.push_back(initialParticleState);
				Particle			.push_back(particleData);
				ParticleNext		.push_back(particleData);
			}
			catch(...) { // Restore the previous size to all the containers and return an error.
				ParticleState		.resize(particleCount);
				Particle			.resize(particleCount);
				ParticleNext		.resize(particleCount);
				return -1;
			}
#endif
			return particleCount;
		}
	};

	tplt<tpnm _tParticleType>
	struct SParticleBinding {
		typedef	_tParticleType	TParticleType;

		TParticleType			Binding										= {};
		int32_t					IndexParticlePhysics						= -1;
	};

	tplt<tpnm _tParticleType, tpnm _tCoord>
	::llc::error_t			addParticle
		(	const _tParticleType										& particleType
		,	::llc::apod<::llc::SParticleBinding<_tParticleType>>	& particleInstances
		,	::llc::SParticle2Integrator<_tCoord>						& particleIntegrator
		,	const ::llc::SParticle2<_tCoord>							& particleDefinition
		)
	{
		::llc::SParticleBinding<_tParticleType>	newInstance									= {};
		newInstance.Binding		= particleType;
		llc_necs(newInstance.IndexParticlePhysics = particleIntegrator.AddParticle(particleDefinition));
		return particleInstances.push_back(newInstance);
	}

	tplt<tpnm _tParticleType, tpnm _tCoord>
	struct SParticleSystem {
		typedef	::llc::SParticleBinding<_tParticleType>	TParticleInstance;
		typedef	::llc::SParticle2Integrator<_tCoord>	TIntegrator;

		::llc::apod<TParticleInstance>	Instances									= {};
		TIntegrator						Integrator									= {};
	};

	tplt<tpnm _tParticleType, tpnm _tCoord>
	stainli	::llc::error_t			addParticle					(const _tParticleType& particleType, ::llc::SParticleSystem<_tParticleType, _tCoord> & particleSystem,	const ::llc::SParticle2<_tCoord> & particleDefinition)	{
		return addParticle(particleType, particleSystem.Instances, particleSystem.Integrator, particleDefinition);
	}

	// simple particle force integrator
	struct SParticles3 {
		::llc::an3f2_t					OldPosition			= {};
		::llc::an3f2_t					Position			= {};
		::llc::an3f2_t					Direction			= {};
		::llc::af2_t					Speed				= {};

		::llc::error_t					IntegrateSpeed		(double secondsLastFrame);

		::llc::error_t					Create				(n3f2_c & position, n3f2_c & direction, float speed);
		::llc::error_t					Remove				(int32_t iParticle);

		::llc::error_t					Load				(::llc::vcu0_t & input);
		::llc::error_t					Save				(::llc::au0_t & output)	const;
	};
} // namespace

#endif // LLC_PARTICLE_H_23627
