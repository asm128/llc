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

		::llc::error_t			Save			(::llc::au0_t & output)	const	{ 
			llc_necs(llc::saveView(output, GravityCenters	));
			llc_necs(llc::saveView(output, Orbits			));
			llc_necs(llc::saveView(output, Bodies			));
			llc_necs(llc::saveView(output, Clouds			));
			llc_necs(llc::saveView(output, Cameras		));
			llc_necs(llc::savePOD (output, Camera			));
			llc_necs(llc::savePOD (output, Background		));
			return 0;
		}

		::llc::error_t			Load				(::llc::vcu0_t & input)	{ 
			llc_necs(llc::loadView(input, GravityCenters	));
			llc_necs(llc::loadView(input, Orbits			));
			llc_necs(llc::loadView(input, Bodies			));
			llc_necs(llc::loadView(input, Clouds			));
			llc_necs(llc::loadView(input, Cameras			));
			llc_necs(llc::loadPOD (input, Camera			));
			llc_necs(llc::loadPOD (input, Background		));
			return 0;
		}
	};

	struct SEnginePlanetarySystem {
		SPlanetarySystem			PlanetarySystem;
		SPlanetarySystemEntityMap	EntityMap;

		::llc::error_t				Save			(::llc::au0_t & output)	const	{ 
			llc_necs(PlanetarySystem.Save(output));
			llc_necs(EntityMap.Save(output));
			return 0;
		}

		::llc::error_t				Load			(::llc::vcu0_t & input)	{ 
			llc_necs(PlanetarySystem.Load(input));
			llc_necs(EntityMap.Load(input));
			return 0;
		}
	};

	::llc::error_t			planetarySystemReset			(const ::llc::SPlanetarySystem & solarSystem, const ::llc::SPlanetarySystemEntityMap & entityMap, ::llc::SEngine & engine, const ::llc::vc8bgra & colors);
	::llc::error_t			planetarySystemCreateEntities	(const ::llc::SPlanetarySystem & solarSystem, ::llc::SPlanetarySystemEntityMap & entityMap, ::llc::SEngine & engine, ::llc::vcs imageFolder, ::llc::vcs backgroundImage);
	::llc::error_t			planetarySystemSetup			(const ::llc::SPlanetarySystem & solarSystem, ::llc::SPlanetarySystemEntityMap & entityMap, ::llc::SEngine & engine, ::llc::vcs imageFolder, ::llc::vcs backgroundImage);
	::llc::error_t			planetarySystemSetup			(::llc::SEnginePlanetarySystem & planetarySystem, ::llc::SEngine & engine, const ::llc::SJSONReader & jsonData, ::llc::vcs imageFolder, ::llc::vcs backgroundImage);
	::llc::error_t			planetarySystemSetup			(::llc::SEnginePlanetarySystem & planetarySystem, ::llc::SEngine & engine, ::llc::vcs jsonFilePath, ::llc::vcs imageFolder, ::llc::vcs backgroundImage);
} // namespace

#endif // LLC_ENGINE_PLANETARY_SYSTEM_H_23701
