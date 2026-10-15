#include "llc_planetary_system.h"

::llc::error_t			llc::SCelestialBodyMap::Save	(::llc::au0_t & output) const {
	llc_necs(::llc::saveView(output, Values));
	for(uint32_t iKey = 0; iKey < Keys.size(); ++iKey)
		llc_necs(::llc::saveView(output, Keys[iKey]));
	return 0;
}

::llc::error_t			llc::SCelestialBodyMap::Load	(::llc::vcu0_t & input) {
	::llc::vcu0_t			inputToRead		= input;
	::llc::apod<SCelestialBody>	loadedValues		= {};
	::llc::avcsc_t			loadedKeys		= {};
	llc_necs(::llc::loadView(inputToRead, loadedValues));
	for(uint32_t iKey = 0; iKey < loadedValues.size(); ++iKey) {
		::llc::vcst_t			key				= {};
		llc_necs(::llc::loadLabel(inputToRead, key));
		llc_necs(loadedKeys.push_back(key));
	}
	Keys = loadedKeys; Values = loadedValues; input = inputToRead;
	return 0;
}

::llc::error_t			llc::planetarySystemSetup	(::llc::SPlanetarySystem & planetarySystem, ::llc::vcs jsonFilePath) { 
	::llc::SJSONFile			jsonFile					= {};
	llc_necs(llc::jsonFileRead(jsonFile, jsonFilePath));
	return ::llc::planetarySystemSetup(planetarySystem, jsonFile.Reader);
}

::llc::error_t			llc::planetarySystemSetup	(::llc::SPlanetarySystem & solarSystem, const ::llc::SJSONReader & jsonData) {
	::llc::aobj<::llc::vcst_t>	stellarBodyNames			= {};
	::llc::as2_t					stellarBodyIndices;
	llc_necs(llc::jsonObjectKeyList(jsonData, 0, stellarBodyIndices, stellarBodyNames));
	const ::llc::view<::llc::vcst_c>	jsonView				= jsonData.View;
	::llc::avcsc_t					bodyParentNames;

	int32_t						iFurthest					= 0;
	float						fFurthest					= 0;

	for(uint32_t iPlanet = 0; iPlanet < stellarBodyIndices.size(); ++iPlanet) {
		const int32_t				jsonIndexStellarBody		= stellarBodyIndices[iPlanet];
		int32_t						indexProperty;
		const auto					loadImageProperty			= [&jsonData, &jsonView, jsonIndexStellarBody](::llc::avcsc_t & images, ::llc::vcst_t propertyName) -> ::llc::error_t {
			int32_t					indexImageProperty;
			if_failed(indexImageProperty = ::llc::jsonObjectValueGet(jsonData, jsonIndexStellarBody + 2, propertyName))
				return images.push_back({});
			return images.push_back(::llc::label(jsonView[indexImageProperty]));
		};

		if_failed(indexProperty = ::llc::jsonObjectValueGet(jsonData, jsonIndexStellarBody + 2, "parent")) 
			llc_necs(bodyParentNames.push_back({}));
		else
			llc_necs(bodyParentNames.push_back(::llc::label(jsonView[indexProperty])));
		
		// Load planet properties.
		cws_if_failed(indexProperty = ::llc::jsonObjectValueGet(jsonData, jsonIndexStellarBody + 2, "type")); // ?? no type? shouldn't happen

		const ::llc::vcs				strType						= jsonData.View[indexProperty]; 
		const ::llc::CELESTIAL_BODY	bodyType					= ::llc::bodyTypeFromString(strType);
		::llc::SCelestialBody		bodyProperties				= {};
		cws_if_failed(::llc::loadCelestialBody(bodyProperties, bodyType, jsonData, jsonIndexStellarBody + 2));
		if(fFurthest < bodyProperties.Orbit.Radius) {
			fFurthest				= bodyProperties.Orbit.Radius;
			iFurthest				= iPlanet;
		}
		llc_necs(solarSystem.Type.push_back(bodyType));
		llc_necs(solarSystem.Body.push_back(::llc::label(stellarBodyNames[iPlanet]), bodyProperties));
		llc_necs(loadImageProperty(solarSystem.ImageColor		, "image_color"));
		llc_necs(loadImageProperty(solarSystem.ImageLight		, "image_light"));
		llc_necs(loadImageProperty(solarSystem.ImageCloudColor	, "image_cloud_color"));
		llc_necs(loadImageProperty(solarSystem.ImageCloudAlpha	, "image_cloud_alpha"));
		llc_necs(loadImageProperty(solarSystem.Shader			, "shader"));
	}

	for(uint32_t iPlanet = 0; iPlanet < solarSystem.Body.Keys.size(); ++iPlanet)
		llc_necs(solarSystem.Parent.push_back(solarSystem.Body.Keys.find(bodyParentNames[iPlanet])));

	return iFurthest;
}
