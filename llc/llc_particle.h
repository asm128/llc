#include "llc_apod_n3.h"

#ifndef LLC_ATMEL		
#	include <cfloat>
#else
#	include <f2_t.h>
#endif
#ifndef LLC_PARTICLE_H_23627
#define LLC_PARTICLE_H_23627

namespace llc
{
#pragma pack(push, 1)
	tplt<tpnm _tElement>
	struct SParticle2Forces {
		typedef	::llc::n2<_tElement>	TCoord;

		stxp	const _tElement		VelocityEpsilon				= 0.0001f;

		TCoord						AccumulatedForce			= {};
		TCoord						Acceleration				= {};	// A vector representing the speed in a given direction
		TCoord						Velocity					= {};	// A vector representing the speed in a given direction

		inxp	bool				VelocityDepleted			()				const	noexcept	{ return (Velocity + Acceleration).LengthSquared() < (VelocityEpsilon * VelocityEpsilon); }
		// This basically does Acceleration += (Force * 1 / Mass) and Velocity += (Acceleration * Time).
		void						IntegrateAccumulatedForce	(const _tElement inverseMass, const _tElement damping, const f3_t timeElapsed)			noexcept	{
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

		inline	void				SetMass						(const f3_t mass)																			noexcept	{ InverseMass = mass ? ((_tElement)(1.0 / mass)) : 0;		}
		inxp	f3_t				GetMass						()				const	noexcept	{ return (InverseMass == 0) ? DBL_MAX : 1.0 / InverseMass;	}
		inxp	bool				HasFiniteMass				()				const	noexcept	{ return InverseMass >= 0.0f;								}
	};

	// This compact structure allows to define all the boolean states of the particle packed in a single byte.
	struct SParticle2State {
		bool					Unused						: 1;
		bool					Active						: 1;

		inxp	bool			RequiresProcessing			()				const	noexcept	{ return (false == Unused) && Active; }
	};
#pragma pack(pop)
	// This basically does FinalPosition = InitialPosition + Velocity * Time.
	tplt<tpnm _tElement>
	stin	void			particleIntegratePosition
		(	const ::llc::n2<_tElement>	& velocity
		,	const f3_t				timeElapsed
		,	const f3_t				timeElapsedHalfSquared
		,	::llc::n2<_tElement>		& position
		) {
		position				+= velocity * timeElapsed;
		position				+= velocity * timeElapsedHalfSquared;
	}

	tplt<tpnm _tElement>
	::llc::err_t			integrate					(const ::llc::view<::llc::SParticle2<_tElement>>& particles, ::llc::apod<::llc::SParticle2State>& particleStates, ::llc::view<::llc::SParticle2<_tElement>>& particlesNext, f3_t timeElapsed, f3_t timeElapsedHalfSquared)			{
		for(::llc::u2_t iParticle = 0, particleCount = (::llc::u2_t)particleStates.size(); iParticle < particleCount; ++iParticle)
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
		inline	::llc::err_t					Integrate									(f3_t timeElapsed, f3_t timeElapsedHalfSquared)														{ return Integrate(ParticleNext, timeElapsed, timeElapsedHalfSquared);	}
		::llc::err_t							Integrate									(::llc::view<TParticle>& particleNext, f3_t timeElapsed, f3_t timeElapsedHalfSquared)			{ return ::llc::integrate(Particle, ParticleState, particleNext, timeElapsed, timeElapsedHalfSquared);		}
		// --------------------------------------------------------
		::llc::err_t							AddParticle					(const TParticle& particleData)																			{
			const ::llc::u2_t								particleCount								= (::llc::u2_t)ParticleState.size();
			stxp	const ::llc::SParticle2State		initialParticleState						= {false, true};

			for(::llc::u2_t iBody = 0; iBody < particleCount; ++iBody)	// Check if there is any unused particle that we can recycle.
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
		s2_t					IndexParticlePhysics						= -1;
	};

	tplt<tpnm _tParticleType, tpnm _tCoord>
	::llc::err_t			addParticle
		(	const _tParticleType										& particleType
		,	::llc::apod<::llc::SParticleBinding<_tParticleType>>	& particleInstances
		,	::llc::SParticle2Integrator<_tCoord>						& particleIntegrator
		,	const ::llc::SParticle2<_tCoord>							& particleDefinition
		)
	{
		::llc::SParticleBinding<_tParticleType>	newInstance									= {};
		newInstance.Binding		= particleType;
		if_fail_fe(newInstance.IndexParticlePhysics = particleIntegrator.AddParticle(particleDefinition));
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
	stin	::llc::err_t			addParticle					(const _tParticleType& particleType, ::llc::SParticleSystem<_tParticleType, _tCoord> & particleSystem,	const ::llc::SParticle2<_tCoord> & particleDefinition)	{
		return addParticle(particleType, particleSystem.Instances, particleSystem.Integrator, particleDefinition);
	}

	// simple particle force integrator
	struct SParticles3 {
		::llc::an3f2_t					OldPosition			= {};
		::llc::an3f2_t					Position			= {};
		::llc::an3f2_t					Direction			= {};
		::llc::af2_t					Speed				= {};

		::llc::err_t					IntegrateSpeed		(f3_t secondsLastFrame);

		::llc::err_t					Create				(n3f2_c & position, n3f2_c & direction, f2_t speed);
		::llc::err_t					Remove				(s2_t iParticle);

		::llc::err_t					Load				(::llc::vcu0_t & input);
		::llc::err_t					Save				(::llc::au0_t & output)	const;
	};
} // namespace

#endif // LLC_PARTICLE_H_23627
