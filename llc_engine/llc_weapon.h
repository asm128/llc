#include "llc_gauge.h"

#include "llc_particle.h"
#include "llc_enum.h"
#include "llc_apod_serialize.h"

#ifndef LLC_WEAPON_H
#define LLC_WEAPON_H

namespace llc 
{

	typedef	::llc::aobj<::llc::an3char	>	aan3c	, aan3char		;
	typedef	::llc::aobj<::llc::an3uchar	>	aan3uc	, aan3uchar		;
	typedef	::llc::aobj<::llc::an3f32	>	aan3f32	;
	typedef	::llc::aobj<::llc::an3f64	>	aan3f64	;
	typedef	::llc::aobj<::llc::an3u8	>	aan3u8	;
	typedef	::llc::aobj<::llc::an3u16	>	aan3u16	;
	typedef	::llc::aobj<::llc::an3u32	>	aan3u32	;
	typedef	::llc::aobj<::llc::an3u64	>	aan3u64	;
	typedef	::llc::aobj<::llc::an3i8	>	aan3i8	;
	typedef	::llc::aobj<::llc::an3i16	>	aan3i16	;
	typedef	::llc::aobj<::llc::an3i32	>	aan3i32	;
	typedef	::llc::aobj<::llc::an3i64	>	aan3i64	;

	struct SShots	{
		::llc::aan3f32				DistanceToTargets	= {};
		::llc::af2_t				Lifetime			= {};
		::llc::af2_t				Brightness			= {};
		::llc::an3f32				PositionDraw		= {};
		::llc::an3f32				PositionPrev		= {};
		::llc::SParticles3			Particles			= {};

		int							Remove				(uint32_t iShot)			{
			Particles.Remove(iShot);
			DistanceToTargets	.remove_unordered(iShot);
			Lifetime			.remove_unordered(iShot);
			Brightness			.remove_unordered(iShot);
			PositionPrev		.remove_unordered(iShot);
			return PositionDraw	.remove_unordered(iShot);
		}

		int							SpawnForced			(const ::llc::n3f2_t & position, const ::llc::n3f2_t & direction, float speed, float brightness, float lifetime)	{
			PositionDraw.push_back(position);
			PositionPrev.push_back(position);
			Brightness	.push_back(brightness);
			Lifetime	.push_back(lifetime);
			DistanceToTargets[DistanceToTargets.push_back({})].push_back(direction);
			return Particles.Create(position, direction, speed);
		}

		int							SpawnForcedDirected	(double stabilityFactor, const ::llc::n3f2_t & position, const ::llc::n3f2_t & direction, float speed, float brightness, float lifetime)	{
			stacxpr	double					randUnit			= ::llc::math_2pi / RAND_MAX;
			::llc::n3f2_t					finalDirection		= {0, 1, 0};
			finalDirection.RotateX(rand() * randUnit);
			finalDirection.RotateY(rand() * randUnit);
			finalDirection.Normalize();
			return SpawnForced(position, ::llc::interpolate_linear(finalDirection, direction, stabilityFactor), speed, brightness, lifetime);
		}

		::llc::error_t				Update				(float secondsLastFrame)	{
			stacxpr	uint32_t				maxRange			= 200;
			stacxpr	uint32_t				maxRangeSquared		= maxRange * maxRange;
			memcpy(PositionPrev.begin(), Particles.Position.begin(), Particles.Position.size() * sizeof(::llc::n3f2_t));
			Particles.IntegrateSpeed(secondsLastFrame);
			for(uint32_t iShot = 0; iShot < Particles.Position.size(); ++iShot) {
				Lifetime[iShot] -= secondsLastFrame;
				if(0 >= Lifetime[iShot])
					Remove(iShot--);
				//if (Particles.Position[iShot].LengthSquared() > maxRangeSquared)
				//	Remove(iShot--);
			}
			return 0;
		}

		::llc::error_t				Save				(::llc::au0_t & output)	const	{ 
			llc_necs(Particles.Save(output));
			llc_necs(llc::saveView(output, PositionDraw	));
			llc_necs(llc::saveView(output, PositionPrev	));
			llc_necs(llc::saveView(output, Brightness		));
			llc_necs(llc::saveView(output, Lifetime		));
			for(uint32_t iParticle = 0; iParticle < Particles.Position.size(); ++iParticle) 
				llc_necs(llc::saveView(output, DistanceToTargets[iParticle]));
			
			return 0; 
		}

		::llc::error_t				Load				(::llc::vcu0_t & input)			{ 
			llc_necs(Particles.Load(input));
			llc_necs(llc::loadView(input, PositionPrev	));
			llc_necs(llc::loadView(input, PositionDraw	));
			llc_necs(llc::loadView(input, Brightness		));
			llc_necs(llc::loadView(input, Lifetime		));
			llc_necs(DistanceToTargets.resize(Particles.Position.size()));
			for(uint32_t iParticle = 0; iParticle < DistanceToTargets.size(); ++iParticle)
				llc_necs(llc::loadView(input, DistanceToTargets[iParticle]));

			return 0;
		}
	};

#pragma pack(push, 1)

	struct SWeaponTrigger {
		gaugemaxf32		Delay				= {};	// Trigger delay
		gaugemaxf32		Overheat			= {};	// Cooldown/Reload time in seconds
		float			OverheatPerShot		= {};	// 
		bool			CoolingDown			= {};	// Reloading

		inlcxpr	float	Busy				()	const			noexcept	{ 
			return CoolingDown					? ::llc::max(Delay.Limit - Delay.Value, Overheat.Value)
				: (Delay.Value < Delay.Limit)	? Delay.Limit - Delay.Value
				: 0
				; 
		}
		float			Pull				()					noexcept	{ 
			const float			busy				= Busy();
			if(busy)
				return busy;

			Delay.Value		= 0;
			Overheat.Value	+= OverheatPerShot;
			if(Overheat.Value >= Overheat.Limit)
				CoolingDown		= true;
			return 0;
		}
		float			Update				(double seconds)	noexcept		{
			Delay.Value		+= (float)seconds;
			if(0 >= Overheat.Value && CoolingDown)
				CoolingDown		= Overheat.Value	= 0;
			else
				Overheat.Value	-= (float)seconds;

			return Busy();
		}

	};

	GDEFINE_FLAG_TYPE(WEAPON_LOAD, uint32_t);
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

	GDEFINE_ENUM_TYPE(WEAPON_ACTION, uint16_t);
	GDEFINE_ENUM_VALUE(WEAPON_ACTION, Shoot		, 0x0);
	GDEFINE_ENUM_VALUE(WEAPON_ACTION, Cooldown	, 0x1);
	GDEFINE_ENUM_VALUE(WEAPON_ACTION, Target	, 0x2);

	GDEFINE_ENUM_TYPE (DAMAGE_TYPE, uint16_t);
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

	GDEFINE_ENUM_TYPE(WEAPON_TYPE, uint8_t);
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
		uint16_t		ParticleCount		= 1;	//= 1;
		int32_t			Damage				= 1;	//= 1;
		float			Stability			= 1;	//= 1.0;
		float			Lifetime			= .5f;	//= 0;
		float			Speed				= {};	//= 150;
	};

	// One per orbiter
	struct SWeapon {
		WEAPON_TYPE		Type				= {};
		SWeaponTrigger	Trigger				= {};
		SWeaponShot		Shot				= {};

		int				Shoot				(::llc::SShots & shots, const ::llc::n3f2_t & position, const ::llc::n3f2_t & direction, float brightness)	{
			if(Trigger.Pull())
				return 0;

			if(0 == Shot.ParticleCount) 
				return shots.SpawnForcedDirected(Shot.Stability, position, direction, Shot.Speed, brightness, Shot.Lifetime);

			const int32_t		indexFirst			= shots.SpawnForcedDirected(Shot.Stability, position, direction, Shot.Speed, brightness, Shot.Lifetime);
			const uint32_t		stop				= uint32_t(Shot.ParticleCount - 1);
			for(uint32_t iDebris = 0; iDebris < stop; ++iDebris)
				shots.SpawnForcedDirected(Shot.Stability, position, direction, Shot.Speed, brightness, Shot.Lifetime);

			return indexFirst;
		}
	};

	struct SWeaponScore {
		int64_t		ImpactTime		= 0;
		int64_t		BulletsHit		= 0;
		int64_t		BulletsUsed		= 0;
		int64_t		BulletsLoaded	= 0;
		int64_t		ShotsUsed		= 0;
		int64_t		ShotsLoaded		= 0;
		int64_t		ReloadCount		= 0;
		int64_t		ReloadTime		= 0;
		int64_t		OverheatCount	= 0;
		int64_t		OverheatTime	= 0;
	};
#pragma pack(pop)

	struct SWeaponManager {
		::llc::apod<::llc::SWeapon>	Weapons			= {};	// 
		::llc::aobj<::llc::SShots >	Shots			= {};	// One per weapon
		::llc::apobj<::llc::an3f32>	WeaponTargets	= {};	// One per weapon

		int32_t						Clear					()		{ ::llc::clear(Weapons, Shots, WeaponTargets); return 0; }
		::llc::error_t				Save					(::llc::au0_t & output)	const	{ 
			llc_necs(llc::saveView(output, Weapons));
			info_printf("Saved %s, %i", "Weapons", Weapons.size());
			for(uint32_t iWeapon = 0; iWeapon < Weapons.size(); ++iWeapon) {
				if(WeaponTargets[iWeapon] && WeaponTargets[iWeapon]->size())
					llc_necall(llc::saveView(output, *WeaponTargets[iWeapon]), "iWeapon: %i", iWeapon);
				else
					llc_necall(llc::saveView(output, ::llc::vc3f32{}), "iWeapon: %i", iWeapon);
				llc_necall(Shots[iWeapon].Save(output), "iWeapon: %i", iWeapon);
			}
			return 0;
		}
		::llc::error_t				Load				(::llc::vcu0_t & input) { 
			Weapons.clear();
			llc_necs(llc::loadView(input, Weapons));
			Shots.resize(Weapons.size());
			llc_necs(WeaponTargets.reserve(Weapons.size()));
			for(uint32_t iWeapon = 0; iWeapon < Weapons.size(); ++iWeapon) {
				llc_necs(llc::loadView(input, WeaponTargets, iWeapon));
				llc_necall(Shots[iWeapon].Load(input), "iWeapon: %i", iWeapon);
			}
			return 0;
		}
	};
} // namespace

#endif // LLC_GALAXY_HELL_WEAPON_H