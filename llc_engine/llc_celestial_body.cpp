#include "llc_celestial_body.h"
#include "llc_stdstring.h"

::llc::error_t			llc::printStar		(const ::llc::SDetailStar & body) {
	info_printf(
		" \nGm					: %u"
		" \nVolume				: %u"
		" \nVolumetricMeanRadius: %u"
		" \nMeanDensity			: %u"
		" \nSurfaceGravity		: %f"
		" \nEllipticity			: %f"
		" \nMomentOfInertia		: %f"
		" \nVisualMagnitude		: %f"
		" \nAbsoluteMagnitude	: %f"
		" \nLuminosity			: %f"
		" \nMassConversionRate	: %u"
		" \nMeanEnergyProduction: %f"
		" \nSurfaceEmission		: %f"
		" \nCentralPressure		: %f"
		" \nCentralTemperature	: %f"
		" \nCentralDensity		: %f"
		, body.Gm							
		, body.Volume						
		, body.VolumetricMeanRadius		
		, body.MeanDensity				
		, body.SurfaceGravity				
		, body.Ellipticity				
		, body.MomentOfInertia			
		, body.VisualMagnitude			
		, body.AbsoluteMagnitude			
		, body.Luminosity					
		, body.MassConversionRate			
		, body.MeanEnergyProduction		
		, body.SurfaceEmission			
		, body.CentralPressure			
		, body.CentralTemperature			
		, body.CentralDensity				
	);
	return 0;
}

::llc::error_t			llc::printPlanet		(const ::llc::SDetailPlanet & body) {
	info_printf(
		" \nGravity					: %f"
		" \nSurfacePressure			: %u"
		" \nDensity					: %u"
		" \nMeanTemperature			: %u"
		" \nNumberOfMoons			: %u"
		" \nGlobalMagneticField		: %s"
		" \nRadiusPolar				: %f"
		" \nRadiusEquatorial		: %f"
		" \nPerihelion				: %f"
		" \nAphelion				: %f"
		" \nRingSystem				: %s"
		" \nLengthOfDay				: %f"
		, body.Gravity					
		, body.SurfacePressure
		, body.Density
		, body.MeanTemperature
		, body.NumberOfMoons
		, ::llc::bool2char(body.GlobalMagneticField)
		, body.RadiusPolar
		, body.RadiusEquatorial
		, body.Perihelion
		, body.Aphelion
		, ::llc::bool2char(body.RingSystem)
		, body.LengthOfDay
		);
	return 0;
}

::llc::error_t			llc::printCelestialBody	(const ::llc::SCelestialBody & body, ::llc::CELESTIAL_BODY type) {
	info_printf(
		" \nMass				: %f"
		" \nDiameter			: %u"
		" \nOrbitRadius			: %f"
		" \nOrbitalPeriod		: %f"
		" \nOrbitalVelocity		: %f"
		" \nOrbitalInclination	: %f"
		" \nOrbitalEccentricity	: %f"
		" \nObliquityToOrbit	: %f"
		" \nRotationPeriod		: %f"
		" \nEscapeVelocity		: %f"
		, body.Mass					
		, body.Diameter				
		, body.Orbit.Radius		
		, body.Orbit.RotationPeriod			
		, body.OrbitalVelocity			
		, body.Orbit.AxialTilt
		, body.OrbitalEccentricity		
		, body.Body.AxialTilt		
		, body.Body.RotationPeriod			
		, body.EscapeVelocity			
		);
	switch(type) {
	default:
	case ::llc::CELESTIAL_BODY_Planet: ::llc::printPlanet(body.Detail.Planet); break;
	case ::llc::CELESTIAL_BODY_Star  : ::llc::printStar  (body.Detail.Star  ); break;
	}
	return 0;
}

::llc::CELESTIAL_BODY	llc::bodyTypeFromString		(::llc::vcst_t bodyType) { 
	if(2 > bodyType.size())
		return (::llc::CELESTIAL_BODY)-1;

	return ::llc::get_value_camelcased<::llc::CELESTIAL_BODY>(bodyType);
}

::llc::PLANET_TYPE		llc::bodySubtypeFromString	(::llc::vcst_t bodyType) {
	if(2 > bodyType.size())
		return (::llc::PLANET_TYPE)-1;

	return ::llc::get_value_camelcased<::llc::PLANET_TYPE>(bodyType);
}

::llc::error_t			llc::loadPlanet		(::llc::SDetailPlanet & body, const ::llc::SJSONReader & jsonData, int32_t jsonIndexStellarBody) { 
	::llc::vcst_t					subtype;
	if_fail_w(::llc::jsonObjectGetAsString (jsonData, jsonIndexStellarBody, "subtype"				, subtype					));
	body.Subtype			= ::llc::bodySubtypeFromString(subtype);
	if_fail_w(::llc::jsonObjectGetAsInteger(jsonData, jsonIndexStellarBody, "density"				, body.Density				));
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "gravity"				, body.Gravity				));
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "perihelion"				, body.Perihelion			));
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "aphelion"				, body.Aphelion				));
	if_fail_w(::llc::jsonObjectGetAsInteger(jsonData, jsonIndexStellarBody, "mean_temperature"		, body.MeanTemperature		));
	if_fail_w(::llc::jsonObjectGetAsInteger(jsonData, jsonIndexStellarBody, "surface_pressure"		, body.SurfacePressure		));
	if_fail_w(::llc::jsonObjectGetAsInteger(jsonData, jsonIndexStellarBody, "number_of_moons"		, body.NumberOfMoons		));
	if_fail_w(::llc::jsonObjectGetAsBoolean(jsonData, jsonIndexStellarBody, "ring_system"			, body.RingSystem			));
	if_fail_w(::llc::jsonObjectGetAsBoolean(jsonData, jsonIndexStellarBody, "global_magnetic_field"	, body.GlobalMagneticField	));
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "length_of_day"			, body.LengthOfDay			));
	return 0; 
}

::llc::error_t			llc::loadStar		(::llc::SDetailStar & body, const ::llc::SJSONReader & jsonData, int32_t jsonIndexStellarBody) { 
	if_fail_w(::llc::jsonObjectGetAsInteger(jsonData, jsonIndexStellarBody, "gm"						, body.Gm					));
	if_fail_w(::llc::jsonObjectGetAsInteger(jsonData, jsonIndexStellarBody, "volume"					, body.Volume				));
	if_fail_w(::llc::jsonObjectGetAsInteger(jsonData, jsonIndexStellarBody, "volumetric_mean_radius"	, body.VolumetricMeanRadius	));
	if_fail_w(::llc::jsonObjectGetAsInteger(jsonData, jsonIndexStellarBody, "mean_density"			, body.MeanDensity			));
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "surface_gravity"		, body.SurfaceGravity		));
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "ellipticity"			, body.Ellipticity			));
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "moment_of_inertia"		, body.MomentOfInertia		));
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "visual_magnitude"		, body.VisualMagnitude		));
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "absolute_magnitude"		, body.AbsoluteMagnitude	));
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "luminosity"				, body.Luminosity			));
	if_fail_w(::llc::jsonObjectGetAsInteger(jsonData, jsonIndexStellarBody, "mass_conversion_rate"	, body.MassConversionRate	));
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "mean_energy_production"	, body.MeanEnergyProduction	));
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "surface_emission"		, body.SurfaceEmission		));
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "central_pressure"		, body.CentralPressure		));
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "central_temperature"	, body.CentralTemperature	));
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "central_density"		, body.CentralDensity		));
	return 0; 
}

::llc::error_t			llc::loadCelestialBody(::llc::SCelestialBody & body, ::llc::CELESTIAL_BODY type, const ::llc::SJSONReader & jsonData, int32_t jsonIndexStellarBody) { 
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "escape_velocity"		, body.EscapeVelocity		));
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "mass"					, body.Mass					));
	if_fail_w(::llc::jsonObjectGetAsInteger(jsonData, jsonIndexStellarBody, "diameter"				, body.Diameter				));
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "distance_from_sun"		, body.Orbit.Radius			));
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "escape_velocity"		, body.EscapeVelocity		));
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "rotation_period"		, body.Body.RotationPeriod	));
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "orbital_period"			, body.Orbit.RotationPeriod	));
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "orbital_velocity"		, body.OrbitalVelocity		));
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "orbital_inclination"	, body.Orbit.AxialTilt		));
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "orbital_eccentricity"	, body.OrbitalEccentricity	));
	if_fail_w(::llc::jsonObjectGetAsDecimal(jsonData, jsonIndexStellarBody, "obliquity_to_orbit"		, body.Body.AxialTilt		));
	body.Body.Radius		= body.Diameter * .5f;
	body.Body.TiltAxis		= ::llc::AXIS_X_POSITIVE;
	body.Orbit.TiltAxis		= ::llc::AXIS_Y_NEGATIVE;
	switch(type) {
	default:
	case ::llc::CELESTIAL_BODY_Planet: ::llc::loadPlanet(body.Detail.Planet, jsonData, jsonIndexStellarBody); break;
	case ::llc::CELESTIAL_BODY_Star  : ::llc::loadStar  (body.Detail.Star  , jsonData, jsonIndexStellarBody); break;
	}
	return 0; 
}
