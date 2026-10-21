#include "llc_planetary_system.h"

::llc::error_t			llc::planetarySystemSetup	(::llc::SPlanetarySystem & planetarySystem, ::llc::vcst_t jsonFilePath) { 
	::llc::SJSONFile			jsonFile					= {};
	llc_necs(llc::jsonFileRead(jsonFile, jsonFilePath));
	return ::llc::planetarySystemSetup(planetarySystem, jsonFile.Reader);
}

::llc::error_t			llc::planetarySystemSetup	(::llc::SPlanetarySystem & solarSystem, const ::llc::SJSONReader & jsonData) {
	::llc::aobj<vcst_t>				stellarBodyNames			= {};
	::llc::as2_t					stellarBodyIndices;
	llc_necs(llc::jsonObjectKeyList(jsonData, 0, stellarBodyIndices, stellarBodyNames));
	const ::llc::view<vcst_c>		jsonView					= jsonData.View;
	::llc::aobj<vcst_t>				bodyParentNames;

	int32_t							iFurthest					= 0;
	float							fFurthest					= 0;

	for(uint32_t iPlanet = 0; iPlanet < stellarBodyIndices.size(); ++iPlanet) {
		const int32_t				jsonIndexStellarBody		= stellarBodyIndices[iPlanet];
		int32_t						indexProperty;
		const auto					loadStringProperty			= [&jsonData, &jsonView, jsonIndexStellarBody](::llc::aobj<::llc::vcst_t> & values, ::llc::vcst_t propertyName) -> ::llc::error_t {
			int32_t					indexValue;
			if_failed(indexValue = ::llc::jsonObjectValueGet(jsonData, jsonIndexStellarBody + 2, propertyName))
				return values.push_back({});
			return values.push_back(::llc::label(jsonView[indexValue]));
		};

		if_failed(indexProperty = ::llc::jsonObjectValueGet(jsonData, jsonIndexStellarBody + 2, "parent")) 
			llc_necs(bodyParentNames.push_back({}));
		else
			llc_necs(bodyParentNames.push_back(::llc::label(jsonView[indexProperty])));
		
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
		llc_necs(solarSystem.Type.push_back(bodyType));
		llc_necs(solarSystem.Body.push_back(::llc::label(stellarBodyNames[iPlanet]), bodyProperties));
		llc_necs(loadStringProperty(solarSystem.ImageColor		, "image_color"));
		llc_necs(loadStringProperty(solarSystem.ImageLight		, "image_light"));
		llc_necs(loadStringProperty(solarSystem.ImageCloudColor	, "image_cloud_color"));
		llc_necs(loadStringProperty(solarSystem.ImageCloudAlpha	, "image_cloud_alpha"));
		llc_necs(loadStringProperty(solarSystem.Shader			, "shader"));
	}

	for(uint32_t iPlanet = 0; iPlanet < solarSystem.Body.Keys.size(); ++iPlanet)
		llc_necs(solarSystem.Parent.push_back(solarSystem.Body.Keys.find(bodyParentNames[iPlanet])));

	return iFurthest;
}
