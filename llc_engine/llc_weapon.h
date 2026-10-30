#include "llc_gauge.h"

#include "llc_particle.h"
#include "llc_enum.h"
#include "llc_apod_serialize.h"

#ifndef LLC_WEAPON_H
#define LLC_WEAPON_H

namespace llc 
{

	typedef	::llc::aobj<::llc::an3sc_t	>	aan3c	, aan3sc_t		;
	typedef	::llc::aobj<::llc::an3uc_t	>	aan3uc	, aan3uc_t		;
	typedef	::llc::aobj<::llc::an3f2_t	>	aan3f2_t	;
	typedef	::llc::aobj<::llc::an3f3_t	>	aan3f3_t	;
	typedef	::llc::aobj<::llc::an3u0_t	>	aan3u0_t	;
	typedef	::llc::aobj<::llc::an3u1_t	>	aan3u1_t	;
	typedef	::llc::aobj<::llc::an3u2_t	>	aan3u2_t	;
	typedef	::llc::aobj<::llc::an3u3_t	>	aan3u3_t	;
	typedef	::llc::aobj<::llc::an3s0_t	>	aan3s0_t	;
	typedef	::llc::aobj<::llc::an3s1_t	>	aan3s1_t	;
	typedef	::llc::aobj<::llc::an3s2_t	>	aan3s2_t	;
	typedef	::llc::aobj<::llc::an3s3_t	>	aan3s3_t	;

	struct SShots	{
		::llc::aan3f2_t				DistanceToTargets	= {};
		::llc::af2_t				Lifetime			= {};
		::llc::af2_t				Brightness			= {};
		::llc::an3f2_t				PositionDraw		= {};
		::llc::an3f2_t				PositionPrev		= {};
		::llc::SParticles3			Particles			= {};

		int							Remove				(::llc::u2_t iShot)			{
			Particles.Remove(iShot);
			DistanceToTargets	.remove_unordered(iShot);
			Lifetime			.remove_unordered(iShot);
			Brightness			.remove_unordered(iShot);
			PositionPrev		.remove_unordered(iShot);
			return PositionDraw	.remove_unordered(iShot);
		}

		int							SpawnForced			(const ::llc::n3f2_t & position, const ::llc::n3f2_t & direction, f2_t speed, f2_t brightness, f2_t lifetime)	{
			PositionDraw.push_back(position);
			PositionPrev.push_back(position);
			Brightness	.push_back(brightness);
			Lifetime	.push_back(lifetime);
			DistanceToTargets[DistanceToTargets.push_back({})].push_back(direction);
			return Particles.Create(position, direction, speed);
		}

		int							SpawnForcedDirected	(f3_t stabilityFactor, const ::llc::n3f2_t & position, const ::llc::n3f2_t & direction, f2_t speed, f2_t brightness, f2_t lifetime)	{
			stxp	f3_t					randUnit			= ::llc::math_2pi / RAND_MAX;
			::llc::n3f2_t					finalDirection		= {0, 1, 0};
			finalDirection.RotateX(rand() * randUnit);
			finalDirection.RotateY(rand() * randUnit);
			finalDirection.Normalize();
			return SpawnForced(position, ::llc::interpolate_linear(finalDirection, direction, stabilityFactor), speed, brightness, lifetime);
		}

		::llc::err_t				Update				(f2_t secondsLastFrame)	{
			stxp	::llc::u2_t				maxRange			= 200;
			stxp	::llc::u2_t				maxRangeSquared		= maxRange * maxRange;
			memcpy(PositionPrev.begin(), Particles.Position.begin(), Particles.Position.size() * sizeof(::llc::n3f2_t));
			Particles.IntegrateSpeed(secondsLastFrame);
			for(::llc::u2_t iShot = 0; iShot < Particles.Position.size(); ++iShot) {
				Lifetime[iShot] -= secondsLastFrame;
				if(0 >= Lifetime[iShot])
					Remove(iShot--);
				//if (Particles.Position[iShot].LengthSquared() > maxRangeSquared)
				//	Remove(iShot--);
			}
			return 0;
		}

		::llc::err_t				Save				(::llc::au0_t & output)	const	{ 
			if_fail_fe(Particles.Save(output));
			if_fail_fe(llc::saveView(output, PositionDraw	));
			if_fail_fe(llc::saveView(output, PositionPrev	));
			if_fail_fe(llc::saveView(output, Brightness		));
			if_fail_fe(llc::saveView(output, Lifetime		));
			for(::llc::u2_t iParticle = 0; iParticle < Particles.Position.size(); ++iParticle) 
				if_fail_fe(llc::saveView(output, DistanceToTargets[iParticle]));
			
			return 0; 
		}

		::llc::err_t				Load				(::llc::vcu0_t & input)			{ 
			if_fail_fe(Particles.Load(input));
			if_fail_fe(llc::loadView(input, PositionPrev	));
			if_fail_fe(llc::loadView(input, PositionDraw	));
			if_fail_fe(llc::loadView(input, Brightness		));
			if_fail_fe(llc::loadView(input, Lifetime		));
			if_fail_fe(DistanceToTargets.resize(Particles.Position.size()));
			for(::llc::u2_t iParticle = 0; iParticle < DistanceToTargets.size(); ++iParticle)
				if_fail_fe(llc::loadView(input, DistanceToTargets[iParticle]));

			return 0;
		}
	};

#pragma pack(push, 1)

	struct SWeaponTrigger {
		gaugemaxf2_t		Delay				= {};	// Trigger delay
		gaugemaxf2_t		Overheat			= {};	// Cooldown/Reload time in seconds
		f2_t			OverheatPerShot		= {};	// 
		bool			CoolingDown			= {};	// Reloading

		inxp	f2_t	Busy				()	const			noexcept	{ 
			return CoolingDown					? ::llc::max(Delay.Limit - Delay.Value, Overheat.Value)
				: (Delay.Value < Delay.Limit)	? Delay.Limit - Delay.Value
				: 0
				; 
		}
		f2_t			Pull				()					noexcept	{ 
			const f2_t			busy				= Busy();
			if(busy)
				return busy;

			Delay.Value		= 0;
			Overheat.Value	+= OverheatPerShot;
			if(Overheat.Value >= Overheat.Limit)
				CoolingDown		= true;
			return 0;
		}
		f2_t			Update				(f3_t seconds)	noexcept		{
			Delay.Value		+= (f2_t)seconds;
			if(0 >= Overheat.Value && CoolingDown)
				CoolingDown		= Overheat.Value	= 0;
			else
				Overheat.Value	-= (f2_t)seconds;

			return Busy();
		}

	};

	GDEFINE_FLAG_TYPE(WEAPON_LOAD, ::llc::u2_t);
	// these are the ammo classes that I can think of right now
	GDEFINE_FLAG_VALUE(WEAPON_LOAD, None		,	0);	
	GDEFINE_FLAG_VALUE(WEAPON_LOAD, Arrow		,	0x1UL);	
	GDEFINE_FLAG_VALUE(WEAPON_LOAD, Bullet		,	0x2UL);	
	GDEFINE_FLAG_VALUE(WEAPON_LOAD, Grenade		,	0x4UL);	
	GDEFINE_FLAG_VALUE(WEAPON_LOAD, Rocket		,	0x8UL);	
	//
	GDEFINE_FLAG_VALUE(WEAPON_LOAD, Ray			,	0x10UL);	
	GDEFINE_FLAG_VALUE(WEAPON_LOAD, Dart		,	0x20UL);	
	GDEFINE_FLAG_VALUE(WEAPON_LOAD, Flame		,	0x40UL);	
	GDEFINE_FLAG_VALUE(WEAPON_LOAD, Fluid		,	0x80UL);	
	//
	GDEFINE_FLAG_VALUE(WEAPON_LOAD, Shot		,	0x100UL);	
	GDEFINE_FLAG_VALUE(WEAPON_LOAD, Knife		,	0x200UL);	
	GDEFINE_FLAG_VALUE(WEAPON_LOAD, Rock		,	0x400UL);	
	GDEFINE_FLAG_VALUE(WEAPON_LOAD, Shrapnel	,	0x800UL);	
	//
	GDEFINE_FLAG_VALUE(WEAPON_LOAD, Boomerang	,	0x1000UL);	
	GDEFINE_FLAG_VALUE(WEAPON_LOAD, Lightning	,	0x2000UL);	
	GDEFINE_FLAG_VALUE(WEAPON_LOAD, Hook		,	0x4000UL);	
	GDEFINE_FLAG_VALUE(WEAPON_LOAD, Wave		,	0x8000UL);	
	//
	GDEFINE_ENUM_VALUE(WEAPON_LOAD, Missile		,	0x10000UL);
	GDEFINE_FLAG_VALUE(WEAPON_LOAD, Blast		,	0x20000UL);
	GDEFINE_ENUM_VALUE(WEAPON_LOAD, Cannonball	,	0x40000UL);
	GDEFINE_FLAG_VALUE(WEAPON_LOAD, Flare		,	0x80000UL);

	GDEFINE_FLAG_VALUE(WEAPON_LOAD, Singularity	,	0x100000UL);

	GDEFINE_FLAG_VALUE(WEAPON_LOAD, Temporized	,	0x10000000UL);
	GDEFINE_FLAG_VALUE(WEAPON_LOAD, Detonable	,	0x20000000UL);

	GDEFINE_ENUM_TYPE(WEAPON_ACTION, ::llc::u1_t);
	GDEFINE_ENUM_VALUE(WEAPON_ACTION, Shoot		, 0x0);
	GDEFINE_ENUM_VALUE(WEAPON_ACTION, Cooldown	, 0x1);
	GDEFINE_ENUM_VALUE(WEAPON_ACTION, Target	, 0x2);

	GDEFINE_ENUM_TYPE (DAMAGE_TYPE, ::llc::u1_t);
	GDEFINE_ENUM_VALUE(DAMAGE_TYPE, Pierce		, 0x0);
	GDEFINE_ENUM_VALUE(DAMAGE_TYPE, Impact		, 0x1);
	GDEFINE_ENUM_VALUE(DAMAGE_TYPE, Wave		, 0x2);
	GDEFINE_ENUM_VALUE(DAMAGE_TYPE, Burn		, 0x4);
	GDEFINE_ENUM_VALUE(DAMAGE_TYPE, Shock		, 0x8);
	GDEFINE_ENUM_VALUE(DAMAGE_TYPE, Magnetic	, 0x10);
	GDEFINE_ENUM_VALUE(DAMAGE_TYPE, Radiation	, 0x20);
	GDEFINE_ENUM_VALUE(DAMAGE_TYPE, Poison		, 0x40);
	GDEFINE_ENUM_VALUE(DAMAGE_TYPE, Plasma		, 0x80);
	GDEFINE_ENUM_VALUE(DAMAGE_TYPE, Light		, 0x100);
	GDEFINE_ENUM_VALUE(DAMAGE_TYPE, Sound		, 0x200);
	GDEFINE_ENUM_VALUE(DAMAGE_TYPE, EMP			, 0x400);
	GDEFINE_ENUM_VALUE(DAMAGE_TYPE, Gravity		, 0x800);

	GDEFINE_ENUM_TYPE(WEAPON_TYPE, u0_t);
	GDEFINE_ENUM_VALUE(WEAPON_TYPE, Gun			, 0);
	GDEFINE_ENUM_VALUE(WEAPON_TYPE, Cannon		, 1);
	GDEFINE_ENUM_VALUE(WEAPON_TYPE, Rocket		, 2);
	GDEFINE_ENUM_VALUE(WEAPON_TYPE, Missile		, 3);
	GDEFINE_ENUM_VALUE(WEAPON_TYPE, Torpedo		, 4);
	GDEFINE_ENUM_VALUE(WEAPON_TYPE, Shotgun		, 5);
	GDEFINE_ENUM_VALUE(WEAPON_TYPE, Battery		, 6);
	GDEFINE_ENUM_VALUE(WEAPON_TYPE, Mothership	, 7);
	GDEFINE_ENUM_VALUE(WEAPON_TYPE, Shield		, 8);

	struct SWeaponShot {
		WEAPON_LOAD		Type				= {};	//= WEAPON_LOAD_Bullet;
		DAMAGE_TYPE		DamageType			= {};	//= WEAPON_DAMAGE_Pierce;
		::llc::u1_t		ParticleCount		= 1;	//= 1;
		s2_t			Damage				= 1;	//= 1;
		f2_t			Stability			= 1;	//= 1.0;
		f2_t			Lifetime			= .5f;	//= 0;
		f2_t			Speed				= {};	//= 150;
	};

	// One per orbiter
	struct SWeapon {
		WEAPON_TYPE		Type				= {};
		SWeaponTrigger	Trigger				= {};
		SWeaponShot		Shot				= {};

		int				Shoot				(::llc::SShots & shots, const ::llc::n3f2_t & position, const ::llc::n3f2_t & direction, f2_t brightness)	{
			if(Trigger.Pull())
				return 0;

			if(0 == Shot.ParticleCount) 
				return shots.SpawnForcedDirected(Shot.Stability, position, direction, Shot.Speed, brightness, Shot.Lifetime);

			const s2_t		indexFirst			= shots.SpawnForcedDirected(Shot.Stability, position, direction, Shot.Speed, brightness, Shot.Lifetime);
			const ::llc::u2_t		stop				= ::llc::u2_t(Shot.ParticleCount - 1);
			for(::llc::u2_t iDebris = 0; iDebris < stop; ++iDebris)
				shots.SpawnForcedDirected(Shot.Stability, position, direction, Shot.Speed, brightness, Shot.Lifetime);

			return indexFirst;
		}
	};

	struct SWeaponScore {
		s3_t		ImpactTime		= 0;
		s3_t		BulletsHit		= 0;
		s3_t		BulletsUsed		= 0;
		s3_t		BulletsLoaded	= 0;
		s3_t		ShotsUsed		= 0;
		s3_t		ShotsLoaded		= 0;
		s3_t		ReloadCount		= 0;
		s3_t		ReloadTime		= 0;
		s3_t		OverheatCount	= 0;
		s3_t		OverheatTime	= 0;
	};
#pragma pack(pop)

	struct SWeaponManager {
		::llc::apod<::llc::SWeapon>	Weapons			= {};	// 
		::llc::aobj<::llc::SShots >	Shots			= {};	// One per weapon
		::llc::apobj<::llc::an3f2_t>	WeaponTargets	= {};	// One per weapon

		s2_t						Clear					()		{ ::llc::clear(Weapons, Shots, WeaponTargets); return 0; }
		::llc::err_t				Save					(::llc::au0_t & output)	const	{ 
			if_fail_fe(llc::saveView(output, Weapons));
			info_printf("Saved %s, %i", "Weapons", Weapons.size());
			for(::llc::u2_t iWeapon = 0; iWeapon < Weapons.size(); ++iWeapon) {
				if(WeaponTargets[iWeapon] && WeaponTargets[iWeapon]->size())
					if_fail_fef(llc::saveView(output, *WeaponTargets[iWeapon]), "iWeapon: %i", iWeapon);
				else
					if_fail_fef(llc::saveView(output, ::llc::vcn3f2_t{}), "iWeapon: %i", iWeapon);
				if_fail_fef(Shots[iWeapon].Save(output), "iWeapon: %i", iWeapon);
			}
			return 0;
		}
		::llc::err_t				Load				(::llc::vcu0_t & input) { 
			Weapons.clear();
			if_fail_fe(llc::loadView(input, Weapons));
			Shots.resize(Weapons.size());
			if_fail_fe(WeaponTargets.reserve(Weapons.size()));
			for(::llc::u2_t iWeapon = 0; iWeapon < Weapons.size(); ++iWeapon) {
				if_fail_fe(llc::loadView(input, WeaponTargets, iWeapon));
				if_fail_fef(Shots[iWeapon].Load(input), "iWeapon: %i", iWeapon);
			}
			return 0;
		}
	};
} // namespace

#endif // LLC_GALAXY_HELL_WEAPON_H