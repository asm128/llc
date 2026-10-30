#include "llc_planetary_system.h"

::llc::err_t			llc::planetarySystemSetup	(::llc::SPlanetarySystem & planetarySystem, ::llc::vcst_t jsonFilePath) { 
	::llc::SJSONFile			jsonFile					= {};
	if_fail_fe(llc::jsonFileRead(jsonFile, jsonFilePath));
	return ::llc::planetarySystemSetup(planetarySystem, jsonFile.Reader);
}

::llc::err_t			llc::planetarySystemSetup	(::llc::SPlanetarySystem & solarSystem, const ::llc::SJSONReader & jsonData) {
	::llc::aobj<vcst_t>				stellarBodyNames			= {};
	::llc::as2_t					stellarBodyIndices;
	if_fail_fe(llc::jsonObjectKeyList(jsonData, 0, stellarBodyIndices, stellarBodyNames));
	const ::llc::view<vcst_c>		jsonView					= jsonData.View;
	::llc::aobj<vcst_t>				bodyParentNames;

	s2_t							iFurthest					= 0;
	f2_t							fFurthest					= 0;

	for(u2_t iPlanet = 0; iPlanet < stellarBodyIndices.size(); ++iPlanet) {
		const s2_t				jsonIndexStellarBody		= stellarBodyIndices[iPlanet];
		s2_t						indexProperty;
		const auto					loadStringProperty			= [&jsonData, &jsonView, jsonIndexStellarBody](::llc::aobj<::llc::vcst_t> & values, ::llc::vcst_t propertyName) -> ::llc::err_t {
			s2_t					indexValue;
			if_failed(indexValue = ::llc::jsonObjectValueGet(jsonData, jsonIndexStellarBody + 2, propertyName)) {
				return values.push_back({});
			}
			return values.push_back(::llc::label(jsonView[indexValue]));
		};

		if_failed(indexProperty = ::llc::jsonObjectValueGet(jsonData, jsonIndexStellarBody + 2, "parent")) {
			if_fail_fe(bodyParentNames.push_back({}));
		}
		else
			if_fail_fe(bodyParentNames.push_back(::llc::label(jsonView[indexProperty])));
		
		// Load planet properties.
		if_fail_cw(indexProperty = ::llc::jsonObjectValueGet(jsonData, jsonIndexStellarBody + 2, "type")); // ?? no type? shouldn't happen

		const ::llc::vcst_t			strType						= jsonData.View[indexProperty]; 
		const ::llc::CELESTIAL_BODY	bodyType					= ::llc::bodyTypeFromString(strType);
		::llc::SCelestialBody		bodyProperties				= {};
		if_fail_cw(::llc::loadCelestialBody(bodyProperties, bodyType, jsonData, jsonIndexStellarBody + 2));
		if(fFurthest < bodyProperties.Orbit.Radius) {
			fFurthest				= bodyProperties.Orbit.Radius;
			iFurthest				= iPlanet;
		}
		if_fail_fe(solarSystem.Type.push_back(bodyType));
		if_fail_fe(solarSystem.Body.push_back(::llc::label(stellarBodyNames[iPlanet]), bodyProperties));
		if_fail_fe(loadStringProperty(solarSystem.ImageColor		, "image_color"));
		if_fail_fe(loadStringProperty(solarSystem.ImageLight		, "image_light"));
		if_fail_fe(loadStringProperty(solarSystem.ImageCloudColor	, "image_cloud_color"));
		if_fail_fe(loadStringProperty(solarSystem.ImageCloudAlpha	, "image_cloud_alpha"));
		if_fail_fe(loadStringProperty(solarSystem.Shader			, "shader"));
	}

	for(u2_t iPlanet = 0; iPlanet < solarSystem.Body.Keys.size(); ++iPlanet)
		if_fail_fe(solarSystem.Parent.push_back(solarSystem.Body.Keys.find(bodyParentNames[iPlanet])));

	return iFurthest;
}
