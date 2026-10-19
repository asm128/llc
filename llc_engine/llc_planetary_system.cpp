#include "llc_planetary_system.h"

::llc::error_t			llc::planetarySystemSetup	(::llc::SPlanetarySystem & planetarySystem, ::llc::vcst_t jsonFilePath) { 
	::llc::SJSONFile			jsonFile					= {};
	llc_necs(llc::jsonFileRead(jsonFile, jsonFilePath));
	return ::llc::planetarySystemSetup(planetarySystem, jsonFile.Reader);
}

::llc::error_t			llc::planetarySystemSetup	(::llc::SPlanetarySystem & solarSystem, const ::llc::SJSONReader & jsonData) {
	::llc::avcsc_t				stellarBodyNames			= {};
	::llc::as2_t					stellarBodyIndices;
	llc_necs(llc::jsonObjectKeyList(jsonData, 0, stellarBodyIndices, stellarBodyNames));
	const ::llc::view<vcsc_c>	jsonView					= jsonData.View;
	::llc::avcsc_t					bodyParentNames;

	int32_t						iFurthest					= 0;
	float						fFurthest					= 0;

	for(uint32_t iPlanet = 0; iPlanet < stellarBodyIndices.size(); ++iPlanet) {
		const int32_t				jsonIndexStellarBody		= stellarBodyIndices[iPlanet];
		int32_t						indexProperty;

		if_failed(indexProperty = ::llc::jsonObjectValueGet(jsonData, jsonIndexStellarBody + 2, "parent")) 
			llc_necs(bodyParentNames.push_back({}));
		else
			llc_necs(bodyParentNames.push_back(::llc::label(jsonView[indexProperty])));
		
		// Load planet properties.
		cws_if_failed(indexProperty = ::llc::jsonObjectValueGet(jsonData, jsonIndexStellarBody + 2, "type")); // ?? no type? shouldn't happen

		const ::llc::vcst_t			strType						= jsonData.View[indexProperty]; 
		const ::llc::CELESTIAL_BODY	bodyType					= ::llc::bodyTypeFromString(strType);
		::llc::SCelestialBody		bodyProperties				= {};
		cws_if_failed(::llc::loadCelestialBody(bodyProperties, bodyType, jsonData, jsonIndexStellarBody + 2));
		if(fFurthest < bodyProperties.Orbit.Radius) {
			fFurthest				= bodyProperties.Orbit.Radius;
			iFurthest				= iPlanet;
		}
		llc_necs(solarSystem.Type.push_back(bodyType));
		llc_necs(solarSystem.Body.push_back(::llc::label(stellarBodyNames[iPlanet]), bodyProperties));
	}

	for(uint32_t iPlanet = 0; iPlanet < solarSystem.Body.Keys.size(); ++iPlanet)
		llc_necs(solarSystem.Parent.push_back(solarSystem.Body.Keys.find(bodyParentNames[iPlanet])));

	return iFurthest;
}
