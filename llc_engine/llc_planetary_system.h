#include "llc_celestial_body.h"
#include "llc_linear_map_pod.h"

#include "llc_json.h"

#ifndef LLC_PLANETARY_SYSTEM_H_23701
#define LLC_PLANETARY_SYSTEM_H_23701

namespace llc
{
	typedef	::llc::lmpod<::llc::vcst_t, ::llc::SCelestialBody>	SCelestialBodyMap;

	struct SPlanetarySystem {
		::llc::SCelestialBodyMap			Body;
		::llc::apod<::llc::CELESTIAL_BODY>	Type;
		::llc::as2_t						Parent;
		::llc::aobj<::llc::vcst_t>			ImageColor;
		::llc::aobj<::llc::vcst_t>			ImageLight;
		::llc::aobj<::llc::vcst_t>			ImageCloudColor;
		::llc::aobj<::llc::vcst_t>			ImageCloudAlpha;
		::llc::aobj<::llc::vcst_t>			Shader;

		::llc::err_t				Save			(::llc::au0_t & output)	const	{ 
			if_fail_fe(Body.Save(output));
			if_fail_fe(llc::saveView(output, Type));
			if_fail_fe(llc::saveView(output, Parent));
			for(u2_t iBody = 0; iBody < Body.size(); ++iBody) {
				if_fail_fe(llc::saveView(output, ImageColor[iBody]));
				if_fail_fe(llc::saveView(output, ImageLight[iBody]));
				if_fail_fe(llc::saveView(output, ImageCloudColor[iBody]));
				if_fail_fe(llc::saveView(output, ImageCloudAlpha[iBody]));
				if_fail_fe(llc::saveView(output, Shader[iBody]));
			}
			return 0;
		}

		::llc::err_t				Load			(::llc::vcu0_t & input)	{ 
			if_fail_fe(Body.Load(input));
			if_fail_fe(llc::loadView(input, Type));
			if_fail_fe(llc::loadView(input, Parent));
			for(u2_t iBody = 0; iBody < Body.size(); ++iBody) {
				::llc::vcst_t imageColor = {}, imageLight = {}, imageCloudColor = {}, imageCloudAlpha = {}, shader = {};
				if_fail_fe(llc::loadLabel(input, imageColor));
				if_fail_fe(llc::loadLabel(input, imageLight));
				if_fail_fe(llc::loadLabel(input, imageCloudColor));
				if_fail_fe(llc::loadLabel(input, imageCloudAlpha));
				if_fail_fe(llc::loadLabel(input, shader));
				if_fail_fe(ImageColor.push_back(imageColor));
				if_fail_fe(ImageLight.push_back(imageLight));
				if_fail_fe(ImageCloudColor.push_back(imageCloudColor));
				if_fail_fe(ImageCloudAlpha.push_back(imageCloudAlpha));
				if_fail_fe(Shader.push_back(shader));
			}
			return 0;
		}
	};

	::llc::err_t			planetarySystemSetup	(::llc::SPlanetarySystem & planetarySystem, const ::llc::SJSONReader & jsonData);
	::llc::err_t			planetarySystemSetup	(::llc::SPlanetarySystem & planetarySystem, ::llc::vcst_t jsonFilePath);
} // namespace

#endif // LLC_PLANETARY_SYSTEM_H_23701

// Mercury - 20° E of crater Hun Kal
// Venus - central peak of crater Ariadne
// Earth - no longer defined by a single point, but by hundreds of points around the world, taking into account tectonic motion
// Mars - 47.95137° E of Viking 1 lander
// The gas giants - multiple systems are used, based either on planetary magnetic fields or observed atmospheric features.
// Moon - the mean Earth point: the point on average facing the Earth
// Io - the mean Jupiter point
// Europa - 178° W of crater Cilix
// Ganymede - 128° E of crater Anat
// Callisto - 34° W of crater Saga
// Mimas - 162° E of crater Palomides
// Enceladus - 5° E of crater Salih
// Tethys - 61° W of crater Arete
// Dione - 63° E of crater Palinurus
// Rhea - 20° W of crater Tore
// Iapetus - 84° W of crater Almeric
// The IAU also defines meridians for some dwarf planets, asteroids, and comets.
