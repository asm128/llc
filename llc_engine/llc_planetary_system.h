#include "llc_celestial_body.h"
#include "llc_array_obj.h"
#include "llc_apod_serialize.h"
#include "llc_label.h"

#include "llc_json.h"

#ifndef LLC_PLANETARY_SYSTEM_H_23701
#define LLC_PLANETARY_SYSTEM_H_23701

namespace llc
{
	struct SCelestialBodyMap {
		::llc::avcsc_t			Keys	= {};
		::llc::apod<SCelestialBody>	Values	= {};

		uint32_t		size		() const								{ return Keys.size(); }
		::llc::error_t	push_back	(::llc::vcs key, const SCelestialBody & value)	{ llc_necs(Keys.push_back(::llc::label(key))); return Values.push_back(value); }
		::llc::error_t	Save		(::llc::au0_t & output) const;
		::llc::error_t	Load		(::llc::vcu0_t & input);
	};

	struct SPlanetarySystem {
		::llc::SCelestialBodyMap			Body;
		::llc::apod<::llc::CELESTIAL_BODY>	Type;
		::llc::as2_t						Parent;

		::llc::error_t				Save			(::llc::au0_t & output)	const	{ 
			llc_necs(Body.Save(output));
			llc_necs(llc::saveView(output, Type));
			llc_necs(llc::saveView(output, Parent));
			return 0;
		}

		::llc::error_t				Load			(::llc::vcu0_t & input)	{ 
			llc_necs(Body.Load(input));
			llc_necs(llc::loadView(input, Type));
			llc_necs(llc::loadView(input, Parent));
			return 0;
		}
	};

	::llc::error_t			planetarySystemSetup	(::llc::SPlanetarySystem & planetarySystem, const ::llc::SJSONReader & jsonData);
	::llc::error_t			planetarySystemSetup	(::llc::SPlanetarySystem & planetarySystem, ::llc::vcs jsonFilePath);
} // namespace

#endif // LLC_PLANETARY_SYSTEM_H_23701

// Mercury - 20� E of crater Hun Kal
// Venus - central peak of crater Ariadne
// Earth - no longer defined by a single point, but by hundreds of points around the world, taking into account tectonic motion
// Mars - 47.95137� E of Viking 1 lander
// The gas giants - multiple systems are used, based either on planetary magnetic fields or observed atmospheric features.
// Moon - the mean Earth point: the point on average facing the Earth
// Io - the mean Jupiter point
// Europa - 178� W of crater Cilix
// Ganymede - 128� E of crater Anat
// Callisto - 34� W of crater Saga
// Mimas - 162� E of crater Palomides
// Enceladus - 5� E of crater Salih
// Tethys - 61� W of crater Arete
// Dione - 63� E of crater Palinurus
// Rhea - 20� W of crater Tore
// Iapetus - 84� W of crater Almeric
// The IAU also defines meridians for some dwarf planets, asteroids, and comets.
