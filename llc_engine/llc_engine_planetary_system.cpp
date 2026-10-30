#include "llc_engine_planetary_system.h"
#include "llc_engine_orbit.h"

#include "llc_stdstring.h"
#include "llc_json_expression.h"
#include "llc_engine_shader.h"
#include "llc_gui_text.h"
#include "llc_string.h"
#include "llc_string_compose.h"

LLC_USING_TYPEINT();

stxp	f3_t			ASTRONOMICAL_UNIT_SCALE		= 1.0 / 149597870700;

::llc::err_t			llc::planetarySystemSetup(::llc::SEnginePlanetarySystem & planetarySystem, ::llc::SEngine & engine, const ::llc::SJSONReader & jsonData, ::llc::vcst_t imageFolder)	{
	if_fail_fef(llc::planetarySystemSetup(planetarySystem.PlanetarySystem, jsonData), "%.*s", (int)jsonData.View[0].size(), jsonData.View[0].begin());
	return ::llc::planetarySystemSetup(planetarySystem.PlanetarySystem, planetarySystem.EntityMap, engine, imageFolder);
}
::llc::err_t			llc::planetarySystemSetup(::llc::SEnginePlanetarySystem & planetarySystem, ::llc::SEngine & engine, ::llc::vcst_t jsonFilePath, ::llc::vcst_t imageFolder)				{
	if_fail_fef(llc::planetarySystemSetup(planetarySystem.PlanetarySystem, jsonFilePath), "%s", jsonFilePath.begin());						
	return ::llc::planetarySystemSetup(planetarySystem.PlanetarySystem, planetarySystem.EntityMap, engine, imageFolder);
}

::llc::err_t			llc::planetarySystemSetup(const ::llc::SPlanetarySystem & solarSystem, ::llc::SPlanetarySystemEntityMap & entityMap, ::llc::SEngine & engine, ::llc::vcst_t imageFolder) {
	stxp	uint8_t				MAX_COLORS				= 16;
	stxp	::llc::astatic<::llc::bgra, MAX_COLORS>	PLANET_COLORS	=
		{ ::llc::YELLOW
		, ::llc::ORANGE
		, ::llc::DARKRED * .5f
		, ::llc::GREEN
		, ::llc::WHITE
		, ::llc::RED
		, ::llc::PURPLE
		, ::llc::BLUE
		, ::llc::BLACK
		, ::llc::ORANGE
		, ::llc::BLUE
		, ::llc::RED
		, ::llc::PURPLE
		, ::llc::DARKRED * .5f
		, ::llc::GREEN
		, ::llc::YELLOW
		};
	if_fail_fe(llc::planetarySystemCreateEntities(solarSystem, entityMap, engine, imageFolder));
	if_fail_fe(llc::planetarySystemReset(solarSystem, entityMap, engine, PLANET_COLORS));
	return 0;
}

static	::llc::err_t	createOrbiterOrbit	(::llc::SEngine & engine, const ::llc::vcst_t shaderName = "psOrbit") {	// Gravity cemters
	::llc::SParamsRingSide		params					= {{}, 32};
	s2_t						iEntity;
	if_fail_fe(iEntity = engine.CreateRingFlat(params));
	if_fail_fe(engine.SetColorDiffuse	(iEntity, {1.0f, 1.0f, 1.0f, .5}));
	if_fail_fe(engine.SetShader		(iEntity, ::llc::psOrbit, shaderName));
	return iEntity;
}

static	::llc::err_t	createGravityCenter		(::llc::SEngine & engine, const ::llc::vcst_t shaderName = "psGravityCenter") {	// Gravity cemters
	::llc::SParamsSphere		params					= {{}, {8, 8}};
	s2_t						iEntity;
	if_fail_fe(iEntity = engine.CreateSphere(params));
	if_fail_fe(engine.SetColorDiffuse	(iEntity, {.0f, 0.0f, 1.0f, .5f}));
	if_fail_fe(engine.SetShader		(iEntity, ::llc::psGravityCenter, shaderName));
	return iEntity;
}

static	::llc::err_t	createOrbiterBody		(::llc::SEngine & engine, const ::llc::vcst_t shaderName = "psSun") {	// Gravity cemters
	::llc::SParamsSphere		params					= {{}, {32, 32}};
	s2_t						iEntity;
	if_fail_fe(iEntity = engine.CreateSphere(params));
	if_fail_fe(engine.SetColorDiffuse	(iEntity, {1.0f, 1.0f, 1.0f, 1.0f}));
	if_fail_fe(engine.SetShader		(iEntity, ::llc::psSphereSolid, shaderName));
	return iEntity;
}

::llc::err_t			llc::planetarySystemCreateEntities(const ::llc::SPlanetarySystem & solarSystem, ::llc::SPlanetarySystemEntityMap & entityMap, ::llc::SEngine & engine, ::llc::vcst_t imageFolder) {
	::llc::string				fileName				= {};

	{	// Background sphere map
		::llc::SParamsSphere		params					= {{}, {32, 32}, true, 1, 2};
		if_fail_fe(entityMap.Background = engine.CreateSphere(params));
		if_fail_fe(engine.SetColorDiffuse(entityMap.Background, {1.0f, 1.0f, 1.0f, .999f}));
		if_fail_fe(engine.SetShader(entityMap.Background, ::llc::psBackgroundMilkyWay, "psBackgroundMilkyWay"));
		::llc::append_strings(fileName, "black", ".png");
		s2_t						iImage;
		if_fail_fe(iImage = engine.CreateImageFromFile(imageFolder, fileName));
		::llc::SRenderNode			& renderNode			= engine.Scene->RenderNodes[engine.GetRenderNode(entityMap.Background)];
		if_fail_fe(engine.Scene->Graphics->Skins[renderNode.Skin]->Textures.insert(0, iImage));
	}

	if_fail_fe(entityMap.GravityCenters.push_back(::createGravityCenter(engine)));
	for(u2_t iOrbiter = 1; iOrbiter < solarSystem.Body.size(); ++iOrbiter)
		if_fail_fe(entityMap.GravityCenters.push_back(engine.Clone(entityMap.GravityCenters[0], true, true, true)));

	if_fail_fe(engine.SetShader(entityMap.GravityCenters[0], ::llc::psSunfire, "psSunfire"));
	
	if_fail_fe(entityMap.Bodies.push_back(::createOrbiterBody(engine)));
	for(u2_t iOrbiter = 1; iOrbiter < solarSystem.Body.size(); ++iOrbiter)
		if_fail_fe(entityMap.Bodies.push_back(engine.Clone(entityMap.Bodies[0], true, true, true)));

	for(u2_t iOrbiter = 0; iOrbiter < solarSystem.Body.size(); ++iOrbiter) {
		fileName.clear();
		if(solarSystem.ImageColor[iOrbiter].size())
			::llc::append_strings(fileName, solarSystem.ImageColor[iOrbiter]);
		else
			::llc::append_strings(fileName, solarSystem.Body.Keys[iOrbiter], "_color.png");

		s2_t						iImage;
		if_fail_fe(iImage = engine.CreateImageFromFile(imageFolder, fileName));
		
		::llc::SRenderNode			& renderNode			= engine.Scene->RenderNodes[engine.GetRenderNode(entityMap.Bodies[iOrbiter])];
		::llc::SSkin				& skin					= *engine.Scene->Graphics->Skins[renderNode.Skin];
		if_fail_fe(skin.Textures.insert(0, iImage));
		if(solarSystem.ImageLight[iOrbiter].size()) {
			s2_t						iImageLight;
			if_fail_fe(iImageLight = engine.CreateImageFromFile(imageFolder, solarSystem.ImageLight[iOrbiter]));
			if(skin.Textures.size() > 1)
				skin.Textures[1]	= iImageLight;
			else
				if_fail_fe(skin.Textures.push_back(iImageLight));
		}
	}

	for(u2_t iOrbiter = 0; iOrbiter < solarSystem.Body.size(); ++iOrbiter) {
		if(solarSystem.ImageCloudColor[iOrbiter].size() && solarSystem.ImageCloudAlpha[iOrbiter].size()) {
			::llc::eid_t				iClouds;
			::llc::SParamsSphere		params					= {{}, {32, 32}};
			if_fail_fe(iClouds = engine.CreateSphere(params, "Clouds"));
			if_fail_fe(entityMap.Clouds.push_back(iClouds));
			if_fail_fe(engine.SetPhysicsActive(iClouds, true));
			if_fail_fe(engine.SetCollides(iClouds, false));
			if_fail_fe(engine.SetRotation(iClouds, {0, 1.0f, .5f}));
			if_fail_fe(engine.SetColorDiffuse(iClouds, {1, 1, 1, .999f}));
			if_fail_fe(engine.SetShader(iClouds, ::llc::psCloud, "psCloud"));
			s2_t						iImageColor, iImageAlpha;
			if_fail_fe(iImageColor = engine.CreateImageFromFile(imageFolder, solarSystem.ImageCloudColor[iOrbiter]));
			if_fail_fe(iImageAlpha = engine.CreateImageFromFile(imageFolder, solarSystem.ImageCloudAlpha[iOrbiter]));
			::llc::SRenderNode			& renderNode			= engine.Scene->RenderNodes[engine.GetRenderNode(iClouds)];
			::llc::SSkin				& skin					= *engine.Scene->Graphics->Skins[renderNode.Skin];
			skin.Textures[0]		= iImageColor;
			if(skin.Textures.size() > 1)
				skin.Textures[1]	= iImageAlpha;
			else
				if_fail_fe(skin.Textures.push_back(iImageAlpha));
		}
		else
			if_fail_fe(entityMap.Clouds.push_back(::llc::EID_INVALID));
	}

	if_fail_fe(entityMap.Orbits.push_back(::createOrbiterOrbit(engine)));
	for(u2_t iOrbiter = 1; iOrbiter < solarSystem.Body.size(); ++iOrbiter)
		if_fail_fe(entityMap.Orbits.push_back(engine.Clone(entityMap.Orbits[0], true, false, false)));

	//{	// Rings circles
	//	::llc::SParamsRing			params					= {{}, 32};
	//	if_fail_fe(entityMap.Rings.push_back(engine.CreateRingSide(params)));
	//	if_fail_fe(engine.SetColorDiffuse(entityMap.Rings[0], {.0f, 1.0f, 0.0f, .05f}));
	//	if_fail_fe(engine.SetShader(entityMap.Rings[0], ::llc::psGridRuler, "psOrbit"));
	//}
	//for(u2_t iOrbiter = 1; iOrbiter < solarSystem.Body.size(); ++iOrbiter)
	//	if_fail_fe(entityMap.Rings.push_back(engine.Clone(entityMap.Rings[0], true, false, false)));

	for(u2_t iOrbiter = 0; iOrbiter < solarSystem.Body.size(); ++iOrbiter) {
		if(-1 != solarSystem.Parent[iOrbiter])
			engine.Entities.SetParent(entityMap.Orbits[iOrbiter], entityMap.GravityCenters[solarSystem.Parent[iOrbiter]]);

		engine.Entities.SetParent(entityMap.GravityCenters[iOrbiter], entityMap.Orbits[iOrbiter]);
		engine.Entities.SetParent(entityMap.Bodies[iOrbiter], entityMap.GravityCenters[iOrbiter]);
		if(::llc::EID_INVALID != entityMap.Clouds[iOrbiter])
			engine.Entities.SetParent(entityMap.Clouds[iOrbiter], entityMap.GravityCenters[iOrbiter]);
		//engine.Entities.SetParent(entityMap.Rings[iOrbiter], entityMap.GravityCenters[iOrbiter]);
	}
	return 0;
}

static	::llc::err_t	initOrbit				(::llc::SEngine & engine, s2_t iOrbiter, s2_t iEntity, f3_t radius, f3_t distanceScale) {
	if_fail_fe(engine.SetPhysicsActive(iEntity, iOrbiter));
	if_fail_fe(engine.SetCollides	(iEntity, false));

	::llc::n3f3_t				scale					= {radius, radius, radius};
	if(iOrbiter)
		scale					*= distanceScale * 2;
	else
		scale					= {.01f, .01f, .01f};
	if_fail_fe(engine.SetMeshScale(iEntity, ::llc::n3f3_t{scale.x * 1.001, scale.y, scale.z * 1.001}.f2(), true));
	if_fail_fe(engine.SetHidden	(iEntity, false));
	return 0;
}

stxp	::llc::n3f3_t	SUN_SCALE				= {.00625f, .00625f, .00625f};

static	::llc::err_t	initGravityCenter		(::llc::SEngine & engine, s2_t iEntity, bool isStar, f3_t diameter, f3_t distanceScale, bool ) {
	if_fail_fe(engine.SetPhysicsActive(iEntity, true));
	if_fail_fe(engine.SetCollides		(iEntity, false));

	::llc::n3f3_t				scale					= {diameter, diameter, diameter};
	scale					= isStar ? SUN_SCALE : scale * distanceScale;
	scale					*= 2;
	if_fail_fe(engine.SetMeshScale(iEntity, scale.f2(), true));
	if_fail_fe(engine.SetHidden	(iEntity, false));
	return 0;
}

static	::llc::err_t	initBody				(::llc::SEngine & engine, s2_t iEntity, bool isStar, ::llc::PLANET_TYPE subtype, f3_t diameter, f3_t distanceScale, ::llc::vcst_t shaderOverride) {
	if_fail_fe(engine.SetPhysicsActive(iEntity, false == isStar));
	if_fail_fe(engine.SetCollides		(iEntity, false));

	::llc::n3f3_t				scale					= {diameter, diameter, diameter};
	scale					= isStar ? SUN_SCALE : scale * distanceScale;
	if_fail_fe(engine.SetMeshScale(iEntity, scale.f2(), true));
	::llc::TFuncPixelShader		* ps					= ::llc::psTerrestrial;
	::llc::vcst_t				psName					= "psTerrestrial";
	if(isStar) {
		ps						= ::llc::psSun;
		psName					= "psSun";
	}
	else switch(subtype) {
	case ::llc::PLANET_TYPE_Dwarf		: ps = ::llc::psDwarf		; psName = "psDwarf"		; break;
	case ::llc::PLANET_TYPE_GasGiant	: ps = ::llc::psGasGiant	; psName = "psGasGiant"	; break;
	case ::llc::PLANET_TYPE_IceGiant	: ps = ::llc::psIceGiant	; psName = "psIceGiant"	; break;
	case ::llc::PLANET_TYPE_MassiveSolid: ps = ::llc::psMassiveSolid; psName = "psMassiveSolid"; break;
	default: break;
	}
	if(shaderOverride.size()) {
		if(shaderOverride == "psSun")				{ ps = ::llc::psSun; psName = "psSun"; }
		else if(shaderOverride == "psTerrestrial")	{ ps = ::llc::psTerrestrial; psName = "psTerrestrial"; }
		else if(shaderOverride == "psEarth")			{ ps = ::llc::psEarth; psName = "psEarth"; }
		else if(shaderOverride == "psGasGiant")		{ ps = ::llc::psGasGiant; psName = "psGasGiant"; }
		else if(shaderOverride == "psIceGiant")		{ ps = ::llc::psIceGiant; psName = "psIceGiant"; }
		else if(shaderOverride == "psDwarf")			{ ps = ::llc::psDwarf; psName = "psDwarf"; }
		else if(shaderOverride == "psMassiveSolid")	{ ps = ::llc::psMassiveSolid; psName = "psMassiveSolid"; }
		else
			return ::llc::OS_INVALID_PARAMETER;
	}
	if_fail_fe(engine.SetShader	(iEntity, *ps, psName));
	if_fail_fe(engine.SetHidden	(iEntity, false));
	return 0;
}

static	::llc::err_t	initCloud				(::llc::SEngine & engine, s2_t iEntity, bool isStar, f3_t diameter, f3_t distanceScale) {
	::llc::n3f3_t				scale					= {diameter, diameter, diameter};
	scale					= isStar ? SUN_SCALE : scale * distanceScale;
	scale					*= 1.01;
	if_fail_fe(engine.SetMeshScale(iEntity, scale.f2(), true));
	if_fail_fe(engine.SetHidden(iEntity, false));
	return 0;
}

static	::llc::err_t	textureNumber			(::llc::g8bgra view, u2_t number, const ::llc::SRasterFont & font) { 
	sc_t						strNumber[4]			= {};
	sprintf_s(strNumber, "%i", number);
	const ::llc::rect2s1_t		targetRect				=
		{ {int16_t(view.metrics().x / 2 - (font.CharSize.x * strlen(strNumber)) / 2), int16_t(view.metrics().y / 2 - font.CharSize.y / 2)}
		, font.CharSize.s1()
		};
	::llc::apod<::llc::n2u1_t>	coords;
	::llc::textLineRaster(view.metrics(), font.CharSize, targetRect, font.Texture, strNumber, coords);
	for(u2_t iCoord = 0; iCoord < coords.size(); ++iCoord) {
		const ::llc::n2u1_t			coord						= coords[iCoord];
		view[coord.y][coord.x]	= ::llc::BLACK;
	}
	return 0; 
}

static	::llc::err_t	initSkin				(::llc::SEngine & engine, ::llc::rgbaf color, s2_t iOrbiter, s2_t iEntity) {
	const ::llc::SEntity	& entity				= engine.Entities[iEntity];
	const ::llc::SRenderNode	& renderNode			= engine.Scene->RenderNodes[entity.RenderNode];
	::llc::SSkin				& skin					= *engine.Scene->Graphics->Skins[renderNode.Skin];
	::llc::SSurface				& surface				= *engine.Scene->Graphics->Surfaces[skin.Textures[0]];
	::llc::SRenderMaterial		& material				= skin.Material;
	material.Color.Specular	= ::llc::GRAY;
	material.Color.Diffuse	= color;
  	material.Color.Ambient	= color * .1f;
	::llc::g8bgra				view					= {(::llc::bgra*)surface.Data.begin(), surface.Desc.Dimensions.u2()};
	const ::llc::SRasterFont	& font					= *engine.Scene->Graphics->Fonts.Fonts[8];
	textureNumber(view, iOrbiter, font);
	return 0;
}

::llc::err_t			llc::planetarySystemReset
	( const ::llc::SPlanetarySystem				& solarSystem	
	, const ::llc::SPlanetarySystemEntityMap	& entityMap
	, ::llc::SEngine							& engine		
	, const ::llc::vc8bgra						& colors
	) {
	f2_t						fFurthest				= 0;
	solarSystem.Body.Values.max<f2_t>(fFurthest, [](const ::llc::SCelestialBody & body){ return body.Orbit.Radius; });
	const f3_t				distanceScale			= 1.0 / fFurthest * 10;
	const f3_t				rotationUnit			= 23.9;

	for(u2_t iOrbiter = 0; iOrbiter < solarSystem.Body.size(); ++iOrbiter) {
		const ::llc::SCelestialBody	& body					= solarSystem.Body.Values[iOrbiter];
		const bool					isStar					= solarSystem.Type[iOrbiter] == ::llc::CELESTIAL_BODY_Star;
		const bool					isMoon					= solarSystem.Type[iOrbiter] == ::llc::CELESTIAL_BODY_Moon;
		stxp	f2_t				moonScale				= 25.f;
		{
			const u2_t				iEntity					= entityMap.Orbits[iOrbiter];
			if_fail_fe(llc::initOrbiterOrbit(body, engine.Integrator, engine.GetRigidBody(iEntity), 1));

			f3_t						orbitRadius				= body.Orbit.Radius;// + body.Diameter * .0005;
			if(isMoon)
				orbitRadius				*= moonScale;
			if_fail_fe(::initOrbit(engine, iOrbiter, iEntity, orbitRadius, distanceScale));
		}
		{
			const u2_t				iEntity					= entityMap.GravityCenters[iOrbiter];
			if_fail_fe(llc::initOrbiterCenter(body, engine.Integrator, engine.GetRigidBody(iEntity), distanceScale * (isMoon ? moonScale : 1)));
			if_fail_fe(::initGravityCenter(engine, iEntity, isStar, body.Diameter, distanceScale * .0001, body.Detail.Planet.RingSystem));
		}
		{
			const u2_t				iEntity					= entityMap.Bodies[iOrbiter];
			if_fail_fe(llc::initOrbiterBody(body, engine.Integrator, engine.GetRigidBody(iEntity), rotationUnit));
			if_fail_fe(::initBody(engine, iEntity, isStar, body.Detail.Planet.Subtype, body.Diameter, distanceScale * .0001 * (isMoon ? moonScale * .05 : 1), solarSystem.Shader[iOrbiter]));
		}
		if(::llc::EID_INVALID != entityMap.Clouds[iOrbiter]) {
			const u2_t				iEntity					= entityMap.Clouds[iOrbiter];
			if_fail_fe(llc::initOrbiterBody(body, engine.Integrator, engine.GetRigidBody(iEntity), rotationUnit));
			if_fail_fe(::initCloud(engine, iEntity, isStar, body.Diameter, distanceScale * .0001 * (isMoon ? moonScale * .05 : 1)));
		}
		::llc::rgbaf				color					= colors[iOrbiter];
		const ::llc::eid_t			entities[3]				= {entityMap.Orbits[iOrbiter], entityMap.GravityCenters[iOrbiter], entityMap.Bodies[iOrbiter]};
		for(u2_t i = 0; i < 3; ++i) {
			color.a					= i ? (i == 2) ? 1 : .25f : .125f;
			if_fail_fe(::initSkin(engine, color, iOrbiter, entities[i]));
		}
	}

	engine.Update(365);
	return 0;
}
