#include "llc_engine_planetary_system.h"
#include "llc_engine_orbit.h"

#include "llc_stdstring.h"
#include "llc_json_expression.h"
#include "llc_engine_shader.h"
#include "llc_gui_text.h"
#include "llc_string.h"
#include "llc_string_compose.h"

LLC_USING_TYPEINT();

stacxpr	double			ASTRONOMICAL_UNIT_SCALE		= 1.0 / 149597870700;

::llc::error_t			llc::planetarySystemSetup(::llc::SEnginePlanetarySystem & planetarySystem, ::llc::SEngine & engine, const ::llc::SJSONReader & jsonData)	{ 
	llc_necall(llc::planetarySystemSetup(planetarySystem.PlanetarySystem, jsonData), "%s", ::llc::toString(jsonData.View[0]).begin());	
	return ::llc::planetarySystemSetup(planetarySystem.PlanetarySystem, planetarySystem.EntityMap, engine); 
}
::llc::error_t			llc::planetarySystemSetup(::llc::SEnginePlanetarySystem & planetarySystem, ::llc::SEngine & engine, ::llc::vcsc_t jsonFilePath)				{ 
	llc_necall(llc::planetarySystemSetup(planetarySystem.PlanetarySystem, jsonFilePath), "%s", jsonFilePath.begin());						
	return ::llc::planetarySystemSetup(planetarySystem.PlanetarySystem, planetarySystem.EntityMap, engine); 
}

::llc::error_t			llc::planetarySystemSetup(const ::llc::SPlanetarySystem & solarSystem, ::llc::SPlanetarySystemEntityMap & entityMap, ::llc::SEngine & engine) {
	stacxpr	uint8_t				MAX_COLORS				= 16;
	stacxpr	::llc::astatic<::llc::bgra, MAX_COLORS>	PLANET_COLORS	=
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
	llc_necs(llc::planetarySystemCreateEntities(solarSystem, entityMap, engine));
	llc_necs(llc::planetarySystemReset(solarSystem, entityMap, engine, PLANET_COLORS));
	return 0;
}

static	::llc::error_t	createOrbiterOrbit	(::llc::SEngine & engine, const ::llc::vcst_t shaderName = ::llc::vcsc_t(5, "psOrbit")) {	// Gravity cemters
	::llc::SParamsRingSide		params					= {{}, 32};
	int32_t						iEntity;
	llc_necs(iEntity = engine.CreateRingFlat(params));
	llc_necs(engine.SetColorDiffuse	(iEntity, {1.0f, 1.0f, 1.0f, .5}));
	llc_necs(engine.SetShader		(iEntity, ::llc::psGridRuler, shaderName));
	return iEntity;
}

static	::llc::error_t	createGravityCenter		(::llc::SEngine & engine, const ::llc::vcst_t shaderName = ::llc::vcsc_t(15, "psGravityCenter")) {	// Gravity cemters
	::llc::SParamsSphere		params					= {{}, {8, 8}};
	int32_t						iEntity;
	llc_necs(iEntity = engine.CreateSphere(params));
	llc_necs(engine.SetColorDiffuse	(iEntity, {.0f, 0.0f, 1.0f, .5f}));
	llc_necs(engine.SetShader		(iEntity, ::llc::psSphereSolid, shaderName));
	return iEntity;
}

static	::llc::error_t	createOrbiterBody		(::llc::SEngine & engine, const ::llc::vcst_t shaderName = ::llc::vcsc_t(5, "psSun")) {	// Gravity cemters
	::llc::SParamsSphere		params					= {{}, {32, 32}};
	int32_t						iEntity;
	llc_necs(iEntity = engine.CreateSphere(params));
	llc_necs(engine.SetColorDiffuse	(iEntity, {1.0f, 1.0f, 1.0f, 1.0f}));
	llc_necs(engine.SetShader		(iEntity, ::llc::psSphereSolid, shaderName));
	return iEntity;
}

::llc::error_t			llc::planetarySystemCreateEntities(const ::llc::SPlanetarySystem & solarSystem, ::llc::SPlanetarySystemEntityMap & entityMap, ::llc::SEngine & engine) {
	::llc::SPNGData				pngCache				= {};
	stxp vcst_t					fileFolder				= LLC_CXS("../llc_data/images");
	::llc::string				fileName				= {};
	int32_t						iBackground				= 0;

	{	// Background sphere map
		::llc::SParamsSphere		params					= {{}, {32, 32}, true, 1, 2};
		llc_necs(iBackground = engine.CreateSphere(params));
		llc_necs(engine.SetColorDiffuse(iBackground, {1.0f, 1.0f, 1.0f, 1.0f}));
		llc_necs(engine.SetShader(iBackground, ::llc::psSphereAxis, "psBackgroundMilkyWay"));
		::llc::append_strings(fileName, "black", ".png");
		int32_t						iImage;
		llc_necs(iImage = engine.CreateImageFromFile(fileFolder, fileName));
		::llc::SRenderNode			& renderNode			= engine.Scene->RenderNodes[engine.GetRenderNode(iBackground)];
		llc_necs(engine.Scene->Graphics->Skins[renderNode.Skin]->Textures.insert(0, iImage));
	}

	llc_necs(entityMap.GravityCenters.push_back(::createGravityCenter(engine)));
	for(uint32_t iOrbiter = 1; iOrbiter < solarSystem.Body.size(); ++iOrbiter)
		llc_necs(entityMap.GravityCenters.push_back(engine.Clone(entityMap.GravityCenters[0], true, true, true)));

	llc_necs(engine.SetShader(entityMap.GravityCenters[0], ::llc::psSphereAxis, "psSunfire"));
	
	llc_necs(entityMap.Bodies.push_back(::createOrbiterBody(engine)));
	for(uint32_t iOrbiter = 1; iOrbiter < solarSystem.Body.size(); ++iOrbiter)
		llc_necs(entityMap.Bodies.push_back(engine.Clone(entityMap.Bodies[0], true, true, true)));

	for(uint32_t iOrbiter = 0; iOrbiter < solarSystem.Body.size(); ++iOrbiter) {
		fileName.clear();
		::llc::append_strings(fileName, solarSystem.Body.Keys[iOrbiter], "_color.png");

		int32_t						iImage;
		llc_necs(iImage = engine.CreateImageFromFile(fileFolder, fileName));
		
		::llc::SRenderNode			& renderNode			= engine.Scene->RenderNodes[engine.GetRenderNode(entityMap.Bodies[iOrbiter])];
		llc_necs(engine.Scene->Graphics->Skins[renderNode.Skin]->Textures.insert(0, iImage));
	}

	llc_necs(entityMap.Orbits.push_back(::createOrbiterOrbit(engine)));
	for(uint32_t iOrbiter = 1; iOrbiter < solarSystem.Body.size(); ++iOrbiter)
		llc_necs(entityMap.Orbits.push_back(engine.Clone(entityMap.Orbits[0], true, false, false)));

	//{	// Rings circles
	//	::llc::SParamsRing			params					= {{}, 32};
	//	llc_necs(entityMap.Rings.push_back(engine.CreateRingSide(params)));
	//	llc_necs(engine.SetColorDiffuse(entityMap.Rings[0], {.0f, 1.0f, 0.0f, .05f}));
	//	llc_necs(engine.SetShader(entityMap.Rings[0], ::llc::psGridRuler, "psOrbit"));
	//}
	//for(uint32_t iOrbiter = 1; iOrbiter < solarSystem.Body.size(); ++iOrbiter)
	//	llc_necs(entityMap.Rings.push_back(engine.Clone(entityMap.Rings[0], true, false, false)));

	for(uint32_t iOrbiter = 0; iOrbiter < solarSystem.Body.size(); ++iOrbiter) {
		if(-1 != solarSystem.Parent[iOrbiter])
			engine.Entities.SetParent(entityMap.Orbits[iOrbiter], entityMap.GravityCenters[solarSystem.Parent[iOrbiter]]);

		engine.Entities.SetParent(entityMap.GravityCenters[iOrbiter], entityMap.Orbits[iOrbiter]);
		engine.Entities.SetParent(entityMap.Bodies[iOrbiter], entityMap.GravityCenters[iOrbiter]);
		//engine.Entities.SetParent(entityMap.Rings[iOrbiter], entityMap.GravityCenters[iOrbiter]);
	}
	return iBackground;
}

static	::llc::error_t	initOrbit				(::llc::SEngine & engine, int32_t iOrbiter, int32_t iEntity, double radius, double distanceScale) {
	llc_necs(engine.SetPhysicsActive(iEntity, iOrbiter));
	llc_necs(engine.SetCollides	(iEntity, false));

	::llc::n3f3_t				scale					= {radius, radius, radius};
	if(iOrbiter)
		scale					*= distanceScale * 2;
	else
		scale					= {.01f, .01f, .01f};
	llc_necs(engine.SetMeshScale(iEntity, ::llc::n3f3_t{scale.x * 1.001, scale.y, scale.z * 1.001}.f2_t(), true));
	llc_necs(engine.SetHidden	(iEntity, false));
	return 0;
}

stacxpr	::llc::n3f3_t	SUN_SCALE				= {.00625f, .00625f, .00625f};

static	::llc::error_t	initGravityCenter		(::llc::SEngine & engine, int32_t iEntity, bool isStar, double diameter, double distanceScale, bool ) {
	llc_necs(engine.SetPhysicsActive(iEntity, true));
	llc_necs(engine.SetCollides		(iEntity, false));

	::llc::n3f3_t				scale					= {diameter, diameter, diameter};
	scale					= isStar ? SUN_SCALE : scale * distanceScale;
	scale					*= 2;
	llc_necs(engine.SetMeshScale(iEntity, scale.f2_t(), true));
	llc_necs(engine.SetHidden	(iEntity, false));
	return 0;
}

static	::llc::error_t	initBody				(::llc::SEngine & engine, int32_t iEntity, bool isStar, double diameter, double distanceScale, bool ringSystem) {
	llc_necs(engine.SetPhysicsActive(iEntity, false == isStar));
	llc_necs(engine.SetCollides		(iEntity, false));

	::llc::n3f3_t				scale					= {diameter, diameter, diameter};
	scale					= isStar ? SUN_SCALE : scale * distanceScale;
	llc_necs(engine.SetMeshScale(iEntity, scale.f2_t(), true));
	::llc::TFuncPixelShader		& ps					= isStar ? ::llc::psSphereAxis	: ringSystem ? ::llc::psSphereMeridian	: ::llc::psSphereSolid		;
	const ::llc::vcsc_t			psName					= isStar ? ::llc::vcs{"psSun"}	: ringSystem ? ::llc::vcs{"psGasGiant"}	: ::llc::vcs{"psTerrestrial"};
	llc_necs(engine.SetShader	(iEntity, ps, psName));
	llc_necs(engine.SetHidden	(iEntity, false));
	return 0;
}

static	::llc::error_t	textureNumber			(::llc::g8bgra view, uint32_t number, const ::llc::SRasterFont & font) { 
	sc_t						strNumber[4]			= {};
	sprintf_s(strNumber, "%i", number);
	const ::llc::rect2i16		targetRect				= 
		{ {int16_t(view.metrics().x / 2 - (font.CharSize.x * strlen(strNumber)) / 2), int16_t(view.metrics().y / 2 - font.CharSize.y / 2)}
		, font.CharSize.s1_t()
		};
	::llc::apod<::llc::n2u1_t>	coords;
	::llc::textLineRaster(view.metrics(), font.CharSize, targetRect, font.Texture, strNumber, coords);
	for(uint32_t iCoord = 0; iCoord < coords.size(); ++iCoord) {
		const ::llc::n2u1_t			coord						= coords[iCoord];
		view[coord.y][coord.x]	= ::llc::BLACK;
	}
	return 0; 
}

static	::llc::error_t	initSkin				(::llc::SEngine & engine, ::llc::rgbaf color, int32_t iOrbiter, int32_t iEntity) {
	const ::llc::SEntity	& entity				= engine.Entities[iEntity];
	const ::llc::SRenderNode	& renderNode			= engine.Scene->RenderNodes[entity.RenderNode];
	::llc::SSkin				& skin					= *engine.Scene->Graphics->Skins[renderNode.Skin];
	::llc::SSurface				& surface				= *engine.Scene->Graphics->Surfaces[skin.Textures[0]];
	::llc::SRenderMaterial		& material				= skin.Material;
	material.Color.Specular	= ::llc::GRAY;
	material.Color.Diffuse	= color;
  	material.Color.Ambient	= color * .1f;
	::llc::g8bgra				view					= {(::llc::bgra*)surface.Data.begin(), surface.Desc.Dimensions.u2_t()};
	const ::llc::SRasterFont	& font					= *engine.Scene->Graphics->Fonts.Fonts[8];
	textureNumber(view, iOrbiter, font);
	return 0;
}

::llc::error_t			llc::planetarySystemReset
	( const ::llc::SPlanetarySystem				& solarSystem	
	, const ::llc::SPlanetarySystemEntityMap	& entityMap
	, ::llc::SEngine							& engine		
	, const ::llc::vc8bgra						& colors
	) {
	float						fFurthest				= 0;
	solarSystem.Body.Values.max<float>(fFurthest, [](const ::llc::SCelestialBody & body){ return body.Orbit.Radius; });
	const double				distanceScale			= 1.0 / fFurthest * 10;
	const double				rotationUnit			= 23.9;

	for(uint32_t iOrbiter = 0; iOrbiter < solarSystem.Body.size(); ++iOrbiter) {
		const ::llc::SCelestialBody	& body					= solarSystem.Body.Values[iOrbiter];
		const bool					isStar					= solarSystem.Type[iOrbiter] == ::llc::CELESTIAL_BODY_Star;
		const bool					isMoon					= solarSystem.Type[iOrbiter] == ::llc::CELESTIAL_BODY_Moon;
		stacxpr	float				moonScale				= 25.f;
		{
			const uint32_t				iEntity					= entityMap.Orbits[iOrbiter];
			llc_necs(llc::initOrbiterOrbit(body, engine.Integrator, engine.GetRigidBody(iEntity), 1));

			double						orbitRadius				= body.Orbit.Radius;// + body.Diameter * .0005;
			if(isMoon)
				orbitRadius				*= moonScale;
			llc_necs(::initOrbit(engine, iOrbiter, iEntity, orbitRadius, distanceScale));
		}
		{
			const uint32_t				iEntity					= entityMap.GravityCenters[iOrbiter];
			llc_necs(llc::initOrbiterCenter(body, engine.Integrator, engine.GetRigidBody(iEntity), distanceScale * (isMoon ? moonScale : 1)));
			llc_necs(::initGravityCenter(engine, iEntity, isStar, body.Diameter, distanceScale * .0001, body.Detail.Planet.RingSystem));
		}
		{
			const uint32_t				iEntity					= entityMap.Bodies[iOrbiter];
			llc_necs(llc::initOrbiterBody(body, engine.Integrator, engine.GetRigidBody(iEntity), rotationUnit));
			llc_necs(::initBody(engine, iEntity, isStar, body.Diameter, distanceScale * .0001 * (isMoon ? moonScale * .05 : 1), body.Detail.Planet.RingSystem));
		}
		::llc::rgbaf				color					= colors[iOrbiter];
		const ::llc::eid_t			entities[3]				= {entityMap.Orbits[iOrbiter], entityMap.GravityCenters[iOrbiter], entityMap.Bodies[iOrbiter]};
		for(uint32_t i = 0; i < 3; ++i) {
			color.a					= i ? (i == 2) ? 1 : .25f : .125f;
			llc_necs(::initSkin(engine, color, iOrbiter, entities[i]));
		}
	}

	engine.Update(365);
	return 0;
}
