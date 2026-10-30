#include "llc_planetary_system.h"
#include "llc_engine.h"
#include "llc_json.h"
#include "llc_view_color.h"

#ifndef LLC_ENGINE_PLANETARY_SYSTEM_H_23701
#define LLC_ENGINE_PLANETARY_SYSTEM_H_23701

namespace llc
{
	struct SPlanetarySystemEntityMap {
		::llc::aeid				GravityCenters	;
		::llc::aeid				Orbits			;
		::llc::aeid				Bodies			;
		::llc::aeid				Clouds			;
		::llc::aeid				Cameras			;
		::llc::eid_t			Camera			= ::llc::EID_INVALID;
		::llc::eid_t			Background		= ::llc::EID_INVALID;

		::llc::err_t			Save			(::llc::au0_t & output)	const	{ 
			if_fail_fe(llc::saveView(output, GravityCenters	));
			if_fail_fe(llc::saveView(output, Orbits			));
			if_fail_fe(llc::saveView(output, Bodies			));
			if_fail_fe(llc::saveView(output, Clouds			));
			if_fail_fe(llc::saveView(output, Cameras		));
			if_fail_fe(llc::savePOD (output, Camera			));
			if_fail_fe(llc::savePOD (output, Background		));
			return 0;
		}

		::llc::err_t			Load				(::llc::vcu0_t & input)	{ 
			if_fail_fe(llc::loadView(input, GravityCenters	));
			if_fail_fe(llc::loadView(input, Orbits			));
			if_fail_fe(llc::loadView(input, Bodies			));
			if_fail_fe(llc::loadView(input, Clouds			));
			if_fail_fe(llc::loadView(input, Cameras			));
			if_fail_fe(llc::loadPOD (input, Camera			));
			if_fail_fe(llc::loadPOD (input, Background		));
			return 0;
		}
	};

	struct SEnginePlanetarySystem {
		SPlanetarySystem			PlanetarySystem;
		SPlanetarySystemEntityMap	EntityMap;

		::llc::err_t				Save			(::llc::au0_t & output)	const	{ 
			if_fail_fe(PlanetarySystem.Save(output));
			if_fail_fe(EntityMap.Save(output));
			return 0;
		}

		::llc::err_t				Load			(::llc::vcu0_t & input)	{ 
			if_fail_fe(PlanetarySystem.Load(input));
			if_fail_fe(EntityMap.Load(input));
			return 0;
		}
	};

	::llc::err_t			planetarySystemReset			(const ::llc::SPlanetarySystem & solarSystem, const ::llc::SPlanetarySystemEntityMap & entityMap, ::llc::SEngine & engine, const ::llc::vc8bgra & colors);
	::llc::err_t			planetarySystemCreateEntities	(const ::llc::SPlanetarySystem & solarSystem, ::llc::SPlanetarySystemEntityMap & entityMap, ::llc::SEngine & engine, ::llc::vcst_t imageFolder);
	::llc::err_t			planetarySystemSetup			(const ::llc::SPlanetarySystem & solarSystem, ::llc::SPlanetarySystemEntityMap & entityMap, ::llc::SEngine & engine, ::llc::vcst_t imageFolder);
	::llc::err_t			planetarySystemSetup			(::llc::SEnginePlanetarySystem & planetarySystem, ::llc::SEngine & engine, const ::llc::SJSONReader & jsonData, ::llc::vcst_t imageFolder);
	::llc::err_t			planetarySystemSetup			(::llc::SEnginePlanetarySystem & planetarySystem, ::llc::SEngine & engine, ::llc::vcst_t jsonFilePath, ::llc::vcst_t imageFolder);
} // namespace

#endif // LLC_ENGINE_PLANETARY_SYSTEM_H_23701
