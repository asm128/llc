#include "llc_engine_shader.h"
#include "llc_noise.h"

::llc::rgbaf			llc::lightCalcSpecular	(::llc::n3f2_t gEyePosW, float specularPower, ::llc::rgbaf specularLight, ::llc::rgbaf specularMaterial, ::llc::n3f2_t posW, ::llc::n3f2_t normalW, ::llc::n3f2_t lightVecW) {
	const ::llc::n3f2_t			pointToEye				= (gEyePosW - posW).Normalize();
	const ::llc::n3f2_t			reflected				= normalW.Reflect(-lightVecW);
	const double				reflectedFactor			= ::llc::max(reflected.Dot(pointToEye), 0.0);
	if(0 == reflectedFactor) 
		return ::llc::BLACK;

	const double				factor					= pow(reflectedFactor, specularPower);
	::llc::rgbaf				result					= specularMaterial * specularLight * factor;
	result.a					= specularMaterial.a;
	return result;
}

::llc::rgbaf			llc::lightCalcDiffuse	(::llc::rgbaf diffuserMaterial, ::llc::n3f2_t normalW, ::llc::n3f2_t lightVecW) {
	double						lightFactor				= ::llc::max(0.0, normalW.Dot(lightVecW));
	return lightFactor ? diffuserMaterial * (float)lightFactor : ::llc::BLACK;
}

::llc::error_t			llc::psSolid
	( const ::llc::SEngineSceneConstants	& constants
	, const ::llc::SPSIn					& inPS
	, ::llc::bgra							& outputPixel
	) {
	const ::llc::n3f2_t			lightVecW				= (constants.LightPosition - inPS.WeightedPosition).Normalize();
	const ::llc::rgbaf			diffuse					= ::llc::lightCalcDiffuse(::llc::DARKGREEN, inPS.WeightedNormal, lightVecW);
	outputPixel				= ::llc::rgbaf(diffuse).Clamp();
	return 0;
}

::llc::error_t			llc::psHidden
	( const ::llc::SEngineSceneConstants	& /*constants*/
	, const ::llc::SPSIn					& /*inPS*/
	, ::llc::bgra							& /*outputPixel*/
	) {
	return 0;
}

::llc::error_t			llc::psGrid
	( const ::llc::SEngineSceneConstants	& constants
	, const ::llc::SPSIn					& inPS
	, ::llc::bgra							& outputPixel
	) { 
	const ::llc::n3f2_t			lightVecW				= (constants.LightPosition - inPS.WeightedPosition).Normalize();
	const ::llc::rgbaf			diffuse					= ::llc::lightCalcDiffuse(::llc::DARKGREEN, inPS.WeightedNormal, lightVecW);
	outputPixel				= ::llc::rgbaf(diffuse).Clamp();
	return 0; 
}

::llc::error_t			llc::psGridRuler
	( const ::llc::SEngineSceneConstants	& constants
	, const ::llc::SPSIn					& inPS
	, ::llc::bgra							& outputPixel
	) { 
	::llc::n2f2_t				scaledUV				= {inPS.WeightedUV.x * inPS.NodeConstants.NodeSize.x, inPS.WeightedUV.y * inPS.NodeConstants.NodeSize.z};
	if( 0.0025f > (scaledUV - ::llc::n2f2_t{0, 0}).LengthSquared()
	 || 0.0025f > (scaledUV - ::llc::n2f2_t{inPS.NodeConstants.NodeSize.x, 0}).LengthSquared()
	 || 0.0025f > (scaledUV - ::llc::n2f2_t{0, inPS.NodeConstants.NodeSize.z}).LengthSquared()
	 || 0.0025f > (scaledUV - ::llc::n2f2_t{inPS.NodeConstants.NodeSize.x, inPS.NodeConstants.NodeSize.z}).LengthSquared()
	 || 0.0025f > (scaledUV - ::llc::n2f2_t{inPS.NodeConstants.NodeSize.x * .5f, inPS.NodeConstants.NodeSize.z}).LengthSquared()
	 || 0.0025f > (scaledUV - ::llc::n2f2_t{inPS.NodeConstants.NodeSize.x * .5f, 0}).LengthSquared()
	 || 0.0025f > (scaledUV - ::llc::n2f2_t{inPS.NodeConstants.NodeSize.x, inPS.NodeConstants.NodeSize.z * .5f}).LengthSquared()
	 || 0.0025f > (scaledUV - ::llc::n2f2_t{0, inPS.NodeConstants.NodeSize.z * .5f}).LengthSquared()
	) {
		return 0;
	}
	else {
		::llc::n3f2_t				lightVecW				= (constants.LightPosition - inPS.WeightedPosition).Normalize();
		double						diffuseFactor			= ::llc::max(lightVecW.Dot(inPS.WeightedNormal.Normalized()), 0.0);
		::llc::rgbaf				materialColor			= 
			 ( (scaledUV.x > inPS.NodeConstants.NodeSize.x * .5 - .005 && scaledUV.x < inPS.NodeConstants.NodeSize.x * .5 + .005) 
			|| (scaledUV.y > inPS.NodeConstants.NodeSize.z * .5 - .005 && scaledUV.y < inPS.NodeConstants.NodeSize.z * .5 + .005)
			)	? ::llc::rgbaf(::llc::WHITE.rgb(), inPS.Material.Color.Diffuse.a)
				: inPS.Material.Color.Diffuse
				;
		::llc::rgbaf				diffuse					= diffuseFactor ? ::llc::rgbaf(((materialColor * (double)diffuseFactor) * ::llc::max(.65, ::llc::noise2D(uint32_t(inPS.WeightedUV.x * 10000), uint32_t(inPS.WeightedUV.y * 10000), 10000))).rgb(), materialColor.a) : ::llc::BLACK;
		return outputPixel		= diffuse.Clamp();
	}
}

::llc::error_t			llc::psCylinderIn
	( const ::llc::SEngineSceneConstants	& constants
	, const ::llc::SPSIn					& inPS
	, ::llc::bgra							& outputPixel
	) { 
	const ::llc::n3f2_t			lightVecW					= (constants.LightPosition - inPS.WeightedPosition).Normalize();
	const ::llc::rgbaf			diffuse						= ::llc::lightCalcDiffuse(::llc::DARKGRAY, inPS.WeightedNormal, lightVecW);
	outputPixel				= ::llc::rgbaf(diffuse).Clamp();
	return 0; 
}

stacxpr	float			LIGHT_FACTOR_AMBIENT		= .025f;
stacxpr	float			LIGHT_FACTOR_SPECULAR_POWER	= 30.0f;
stacxpr	::llc::bgra8	PIXEL_BLACK_NUMBER			= ::llc::bgra{0, 0, 0, 255};

static	float			shaderRand				(const ::llc::n2f2_t & uv) {
	const double				value					= ::sin(uv.Dot({12.9898f, 78.233f})) * 43758.5453123;
	return (float)(value - ::floor(value));
}

static	::llc::rgbaf	shaderSample			(const ::llc::SPSIn & inPS, const ::llc::n2f2_t & uv) {
	const ::llc::n2u2_t		metrics					= inPS.Surface.metrics();
	if(0 == metrics.x || 0 == metrics.y)
		return {};
	const ::llc::n2f2_t		wrapped					= {uv.x - (float)::floor(uv.x), uv.y - (float)::floor(uv.y)};
	const ::llc::n2u2_t		coord					= {(uint32_t)(wrapped.x * metrics.x) % metrics.x, (uint32_t)(wrapped.y * metrics.y) % metrics.y};
	return inPS.Surface[coord.y][coord.x];
}

static	::llc::rgbaf	shaderPlanet			(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS) {
	::llc::n2f2_t			relativeToCenter			= inPS.WeightedUV - ::llc::n2f2_t{.5f, .5f};
	relativeToCenter.x		*= 2;
	::llc::rgbaf			materialColor				= ::shaderSample(inPS, inPS.WeightedUV);
	bool					shade						= false;
	float					ambientFactor				= ::LIGHT_FACTOR_AMBIENT;
	const ::llc::n3f2_t		lightVecW					= (constants.LightPosition - inPS.WeightedPosition).Normalize();
	const ::llc::n3f2_t		normal						= inPS.WeightedNormal.Normalized();
	const float				diffuseFactor				= (float)normal.Dot(lightVecW);
	if(relativeToCenter.LengthSquared() >= .030625f) {
		shade				= true;
		ambientFactor		= .35f;
	}
	else {
		const ::llc::rgbaf		surfaceColor				= ::shaderSample(inPS, {relativeToCenter.x * 2.f + .5f, relativeToCenter.y * 2.f + .5f});
		if(0 != surfaceColor.r + surfaceColor.g + surfaceColor.b) {
			shade			= 0 != ((uint32_t)(::shaderRand(inPS.WeightedUV * diffuseFactor) * 10) % 2);
			ambientFactor	= .35f;
		}
		else {
			materialColor	= ::llc::BLACK;
			shade			= true;
		}
	}
	if(shade) {
		if(diffuseFactor < 0) {
			ambientFactor	+= ::shaderRand(inPS.WeightedUV * diffuseFactor) * diffuseFactor * -.25f;
			materialColor	= (materialColor * ambientFactor).Clamp();
		}
		else {
			const ::llc::rgbaf	specular				= ::llc::lightCalcSpecular(constants.CameraPosition, ::LIGHT_FACTOR_SPECULAR_POWER, ::llc::WHITE, ::llc::WHITE, inPS.WeightedPosition, normal, lightVecW);
			const ::llc::rgbaf	diffuse					= materialColor * ::llc::max(0.0f, diffuseFactor);
			const ::llc::rgbaf	ambient					= materialColor * ambientFactor;
			materialColor	= ::llc::rgbaf(ambient + diffuse + specular).Clamp();
		}
	}
	materialColor.a			= inPS.Material.Color.Diffuse.a;
	return materialColor;
}

::llc::error_t			llc::psCylinder
	( const ::llc::SEngineSceneConstants	& constants
	, const ::llc::SPSIn					& inPS
	, ::llc::bgra							& outputPixel
	) { 
	::llc::rgbaf				materialcolor				= ::llc::BROWN + (::llc::ORANGE * .5f);
	const ::llc::n3f2_t			lightVecW					= (constants.LightPosition - inPS.WeightedPosition).Normalize();
	const ::llc::rgbaf			diffuse						= ::llc::lightCalcDiffuse(materialcolor, inPS.WeightedNormal, lightVecW);
	const ::llc::rgbaf			ambient						= materialcolor * ::LIGHT_FACTOR_AMBIENT;

	outputPixel				= ::llc::rgbaf(ambient + diffuse).Clamp();
	return 0; 
}

::llc::error_t			llc::psSphereAxis				
	( const ::llc::SEngineSceneConstants	& constants
	, const ::llc::SPSIn					& inPS
	, ::llc::bgra							& outputPixel
	) { 
	::llc::n2f2_t				relativeToCenter			= ::llc::n2f2_t{inPS.WeightedUV.x, inPS.WeightedUV.y} - ::llc::n2f2_t{.5f, .5f};
	relativeToCenter.x		*= 2;

	const ::llc::bgra			surfacecolor				
		= ((::llc::n2f2_t{ 0.0f, 0.0f} - relativeToCenter).LengthSquared() < .0025f) ? ::llc::RED 
		: ((::llc::n2f2_t{ 1.0f, 0.0f} - relativeToCenter).LengthSquared() < .0025f) ? ::llc::RED 
		: ((::llc::n2f2_t{-1.0f, 0.0f} - relativeToCenter).LengthSquared() < .0025f) ? ::llc::RED 
		: ((::llc::n2f2_t{ 0.5f, 0.0f} - relativeToCenter).LengthSquared() < .0025f) ? ::llc::RED 
		: ((::llc::n2f2_t{-0.5f, 0.0f} - relativeToCenter).LengthSquared() < .0025f) ? ::llc::RED 
		: (( 0.5f - relativeToCenter.y) <  .05f) ? ::llc::RED 
		: ((-0.5f - relativeToCenter.y) > -.05f) ? ::llc::RED 
		: ::llc::WHITE;
	::llc::rgbaf				materialcolor				= surfacecolor;		
	const ::llc::n3f2_t			lightVecW					= (constants.LightPosition - inPS.WeightedPosition).Normalize();
	const ::llc::rgbaf			specular					= ::llc::lightCalcSpecular(constants.CameraPosition, ::LIGHT_FACTOR_SPECULAR_POWER, llc::WHITE, ::llc::WHITE, inPS.WeightedPosition, inPS.WeightedNormal, lightVecW);
	const ::llc::rgbaf			diffuse						= ::llc::lightCalcDiffuse(materialcolor, inPS.WeightedNormal, lightVecW);
	const ::llc::rgbaf			ambient						= materialcolor * ::LIGHT_FACTOR_AMBIENT;

	outputPixel				= (0 == surfacecolor.g) ? ::llc::RED : ::llc::rgbaf(ambient + diffuse + specular).Clamp();
	return 0; 
}

::llc::error_t			llc::psSphereSolid				
	( const ::llc::SEngineSceneConstants	& constants
	, const ::llc::SPSIn					& inPS
	, ::llc::bgra							& outputPixel
	) { 
	const ::llc::n2u2_t			& surfaceSize				= inPS.Surface.metrics();
	::llc::n2f2_t				relativeToCenter			= ::llc::n2f2_t{inPS.WeightedUV.x, inPS.WeightedUV.y} - ::llc::n2f2_t{.5f, .5f};
	relativeToCenter.x		*= 2;

	::llc::rgbaf				materialColor;
	bool						shade						= false;
	float						ambientFactor				= ::LIGHT_FACTOR_AMBIENT;
	if((::llc::n2f2_t{0.0f, 0.0f} - relativeToCenter).LengthSquared() >= .0225f) {
		materialColor			= inPS.Material.Color.Diffuse;
		shade					= true;
	}
	else {
		const ::llc::n2u2_t			fetchCoord					= 
			{ (uint32_t)(relativeToCenter.x * 2.f * surfaceSize.x + surfaceSize.x / 2)
			, (uint32_t)(relativeToCenter.y * 4.f * surfaceSize.y + surfaceSize.y / 2)
			};
		const ::llc::bgra			surfacecolor				= inPS.Surface
			[fetchCoord.y % surfaceSize.y]
			[fetchCoord.x % surfaceSize.x]
			;
		if(surfacecolor != PIXEL_BLACK_NUMBER) {
			materialColor			= ::llc::WHITE;
			shade					= rand() % 2;
			ambientFactor			= .65f;
		}
		else {
			materialColor			= llc::BLACK;
			shade					= true;
		}
	}

	if(shade) {
		const ::llc::n3f2_t			lightVecW					= (constants.LightPosition - inPS.WeightedPosition).Normalize();
		double						diffuseFactor				= inPS.WeightedNormal.Dot(lightVecW);
		if(diffuseFactor < 0) {
			ambientFactor			+= (rand() % 256) / 255.0f * float(diffuseFactor) * -.25f;
			materialColor			= (materialColor * ambientFactor).Clamp();
		}
		else {
			const ::llc::rgbaf			specular					= ::llc::lightCalcSpecular(constants.CameraPosition, ::LIGHT_FACTOR_SPECULAR_POWER, llc::WHITE, ::llc::WHITE, inPS.WeightedPosition, inPS.WeightedNormal, lightVecW);
			const ::llc::rgbaf			diffuse						= materialColor * ::llc::max(0.0, diffuseFactor);
			const ::llc::rgbaf			ambient						= materialColor * ambientFactor;
			materialColor			= ::llc::rgbaf(ambient + diffuse + specular).Clamp();
		}
	}
	outputPixel				= materialColor;
	return 0; 
}

::llc::error_t			llc::psSphereMeridian			
	( const ::llc::SEngineSceneConstants	& constants
	, const ::llc::SPSIn					& inPS
	, ::llc::bgra							& outputPixel
	) { 
	const ::llc::n2u2_t			& surfaceSize			= inPS.Surface.metrics();
	::llc::n2f2_t				relativeToCenter		= ::llc::n2f2_t{inPS.WeightedUV.x, inPS.WeightedUV.y} - ::llc::n2f2_t{.5f, .5f};
	relativeToCenter.x		*= 2;

	::llc::rgbaf				materialColor;
	bool						shade					= false;
	float						ambientFactor			= ::LIGHT_FACTOR_AMBIENT;
	if((::llc::n2f2_t{0.0f, 0.0f} - relativeToCenter).LengthSquared() >= .0225f) {
		materialColor							
			= (relativeToCenter.y >  .20f) ? ::llc::WHITE
			: (relativeToCenter.y < -.20f) ? ::llc::WHITE
			: inPS.Material.Color.Diffuse
			;
		shade				= true;
	}
	else {
		const ::llc::n2u2_t			fetchCoord				= 
			{ (uint32_t)(relativeToCenter.x * 2.f * surfaceSize.x + surfaceSize.x / 2)
			, (uint32_t)(relativeToCenter.y * 4.f * surfaceSize.y + surfaceSize.y / 2)
			};
		const ::llc::bgra			surfacecolor			= inPS.Surface
			[fetchCoord.y % surfaceSize.y]
			[fetchCoord.x % surfaceSize.x]
			;

		if(surfacecolor != PIXEL_BLACK_NUMBER) {
			materialColor		= ::llc::WHITE;
			shade				= rand() % 2;
			ambientFactor		= .65f;
		}
		else {
			materialColor		= ::llc::BLACK;
			shade				= true;
		} 
	}
	if(shade) {
		const ::llc::n3f2_t			lightVecW				= (constants.LightPosition - inPS.WeightedPosition).Normalize();
		double						diffuseFactor			= inPS.WeightedNormal.Dot(lightVecW);
		if(diffuseFactor < 0) {
			ambientFactor			+= (rand() % 256) * (1.0f / 255) * float(diffuseFactor) * -.25f;
			materialColor			= (materialColor * ambientFactor).Clamp();
		}
		else {
			const ::llc::rgbaf			specular				= ::llc::lightCalcSpecular(constants.CameraPosition, ::LIGHT_FACTOR_SPECULAR_POWER, llc::WHITE, ::llc::WHITE, inPS.WeightedPosition, inPS.WeightedNormal, lightVecW);
			const ::llc::rgbaf			diffuse					= materialColor * ::llc::max(0.0, diffuseFactor);
			const ::llc::rgbaf			ambient					= materialColor * ambientFactor;
			materialColor			= ::llc::rgbaf(ambient + diffuse + specular).Clamp();
		}
	}
	outputPixel				= materialColor;
	return 0; 
}

::llc::error_t			llc::psOrbit
	( const ::llc::SEngineSceneConstants & constants
	, const ::llc::SPSIn & inPS
	, ::llc::bgra & outputPixel
	) {
	stacxpr float			margin						= .00025f;
	const float				u							= inPS.WeightedUV.x;
	const bool				markCenter					= (u > 0 && u < margin) || ::abs(u - .25f) < margin || ::abs(u - .5f) < margin || ::abs(u - .75f) < margin || (u > 1 - margin && u < 1);
	const float				diffuseFactor				= ::llc::max(0.0f, (float)::sin(inPS.WeightedPosition.Length() * 1200 + constants.Time * 10));
	const ::llc::n2f2_t		scaledUV					= {u * inPS.NodeConstants.NodeSize.x, inPS.WeightedUV.y * inPS.NodeConstants.NodeSize.z};
	const float				nodeZ						= inPS.NodeConstants.NodeSize.z ? inPS.NodeConstants.NodeSize.z : 1;
	const float				randFactor					= ::llc::max(0.0f, (float)::sin((scaledUV - ::llc::n2f2_t{inPS.NodeConstants.NodeSize.x * .5f, nodeZ * .5f}).Length() - constants.Time * (.005f / nodeZ)) * (float)::llc::math_pi * 100);
	const ::llc::rgbaf		diffuse						= inPS.Material.Color.Diffuse;
	::llc::rgbaf			materialColor				= diffuse * 2.0f;
	if(markCenter)
		materialColor		= {materialColor.r + ::llc::max(0.0f, diffuse.b * diffuseFactor * 2) + ::llc::max(0.0f, diffuse.g * randFactor), materialColor.g + ::llc::max(0.0f, diffuse.r * diffuseFactor * 2) + ::llc::max(0.0f, diffuse.b * randFactor), materialColor.b + ::llc::max(0.0f, diffuse.g * diffuseFactor * 2) + ::llc::max(0.0f, diffuse.r * randFactor), ::llc::max(diffuse.a * diffuseFactor * 4, .25f)};
	else
		materialColor.a	= diffuse.a * diffuseFactor * (1.0f - ::abs(inPS.WeightedUV.y - .5f) * 2);
	materialColor			+= materialColor * randFactor;
	outputPixel				= materialColor.Clamp();
	return 0;
}

::llc::error_t			llc::psSunfire
	( const ::llc::SEngineSceneConstants & constants
	, const ::llc::SPSIn & inPS
	, ::llc::bgra & outputPixel
	) {
	const ::llc::n2f2_t		scaledUV					= {inPS.WeightedUV.x * inPS.NodeConstants.NodeSize.x, inPS.WeightedUV.y * inPS.NodeConstants.NodeSize.z};
	const float				diffuseFactor				= ::llc::max(0.0f, (float)::sin(::llc::n2f2_t{inPS.WeightedPosition.x * 10, inPS.WeightedPosition.z * 10}.Length() * 1200 + constants.Time * 20));
	const ::llc::n3f2_t		fromCenter					= inPS.WeightedPosition * 10 - inPS.NodeConstants.NodeSize * .5f;
	const float				nodeZ						= inPS.NodeConstants.NodeSize.z ? inPS.NodeConstants.NodeSize.z : 1;
	const float				randFactor					= ::llc::max(0.0f, (float)::sin((fromCenter.Length() * 1000 - constants.Time * (.005f / nodeZ)) * ::llc::math_pi * 1000));
	const ::llc::rgbaf		diffuse						= inPS.Material.Color.Diffuse;
	::llc::rgbaf			materialColor				= diffuse * 2.0f;
	materialColor			= {materialColor.r + ::llc::max(0.0f, diffuse.b * diffuseFactor * 2) + ::llc::max(0.0f, diffuse.g * randFactor), materialColor.g + ::llc::max(0.0f, diffuse.r * diffuseFactor * 2) + ::llc::max(0.0f, diffuse.b * randFactor), materialColor.b + ::llc::max(0.0f, diffuse.g * diffuseFactor * .1f), ::llc::max(diffuse.a * diffuseFactor * 2, .125f)};
	materialColor			+= materialColor * randFactor;
	outputPixel				= materialColor.Clamp();
	(void)scaledUV;
	return 0;
}

::llc::error_t			llc::psGravityCenter
	( const ::llc::SEngineSceneConstants & constants
	, const ::llc::SPSIn & inPS
	, ::llc::bgra & outputPixel
	) {
	stacxpr float			margin						= .00025f;
	const float				u							= inPS.WeightedUV.x;
	const bool				markCenter					= (u > 0 && u < margin) || ::abs(u - .25f) < margin || ::abs(u - .5f) < margin || ::abs(u - .75f) < margin || (u > 1 - margin && u < 1);
	float					diffuseFactor				= ::llc::max(0.0f, (float)::sin(::llc::n2f2_t{inPS.WeightedPosition.x, inPS.WeightedPosition.z}.Length() * 1200 + constants.Time * 10));
	diffuseFactor			*= ::llc::max(0.0f, (float)::sin(::llc::n2f2_t{inPS.WeightedPosition.x, inPS.WeightedPosition.y}.Length() * 1200 + constants.Time * 10));
	const float				nodeZ						= inPS.NodeConstants.NodeSize.z ? inPS.NodeConstants.NodeSize.z : 1;
	const float				randFactor					= ::llc::max(0.0f, (float)::sin(((inPS.WeightedPosition - inPS.NodeConstants.NodeSize * .5f).Length() - constants.Time * (.005f / nodeZ)) * ::llc::math_pi * 100));
	const ::llc::rgbaf		diffuse						= inPS.Material.Color.Diffuse;
	::llc::rgbaf			materialColor				= diffuse * 2.0f;
	if(markCenter)
		materialColor		= {materialColor.r + ::llc::max(0.0f, diffuse.b * diffuseFactor * 2) + ::llc::max(0.0f, diffuse.g * randFactor), materialColor.g + ::llc::max(0.0f, diffuse.r * diffuseFactor * 2) + ::llc::max(0.0f, diffuse.b * randFactor), materialColor.b + ::llc::max(0.0f, diffuse.g * diffuseFactor * 2) + ::llc::max(0.0f, diffuse.r * randFactor), ::llc::max(diffuse.a * diffuseFactor * 4, .25f)};
	else
		materialColor.a	= diffuse.a * diffuseFactor * (1.0f - ::abs(inPS.WeightedUV.y - .5f) * 2);
	materialColor			+= materialColor * randFactor;
	outputPixel				= materialColor.Clamp();
	return 0;
}

::llc::error_t			llc::psSun
	( const ::llc::SEngineSceneConstants & constants
	, const ::llc::SPSIn & inPS
	, ::llc::bgra & outputPixel
	) {
	::llc::n2f2_t			relativeToCenter			= inPS.WeightedUV - ::llc::n2f2_t{.5f, .5f};
	relativeToCenter.x		*= 2;
	const float				weight						= ::abs(::shaderRand(relativeToCenter) * .75f) + ::abs((float)::sin(constants.Time) * .25f);
	const float				colorFactor				= ::abs(::shaderRand(inPS.WeightedUV * (float)::sin(constants.Time * 2))) + .5f;
	const ::llc::rgbaf		sampleColor				= ::shaderSample(inPS, inPS.WeightedUV * weight);
	const ::llc::rgbaf		randColor					= {1, .5f * weight + .5f, .15f * weight, 1};
	::llc::rgbaf			colorBody					= {sampleColor.r * .25f + sampleColor.r * ::abs((float)::sin(constants.Time)) + randColor.r, sampleColor.g * .25f + sampleColor.g * .25f * ::abs((float)::sin(constants.Time)) + randColor.g, sampleColor.b * .25f + randColor.b, inPS.Material.Color.Diffuse.a};
	const float				randWeight					= weight * .25f;
	const float				markerRadius				= .05f;
	const bool				markY						= markerRadius >= .5f - relativeToCenter.y || markerRadius <= -.5f - relativeToCenter.y;
	const bool				markX						= relativeToCenter.LengthSquared() <= markerRadius * markerRadius || (relativeToCenter - ::llc::n2f2_t{1, 0}).LengthSquared() <= markerRadius * markerRadius || (relativeToCenter - ::llc::n2f2_t{-1, 0}).LengthSquared() <= markerRadius * markerRadius;
	const bool				markZ						= (relativeToCenter - ::llc::n2f2_t{.5f, 0}).LengthSquared() <= markerRadius * markerRadius || (relativeToCenter - ::llc::n2f2_t{-.5f, 0}).LengthSquared() <= markerRadius * markerRadius;
	::llc::rgbaf			surfaceColor				= markX ? ::llc::rgbaf{1, randWeight, randWeight, colorBody.a} : markY ? ::llc::rgbaf{randWeight, 1, randWeight, colorBody.a} : markZ ? ::llc::rgbaf{randWeight, randWeight, 1, colorBody.a} : colorBody;
	surfaceColor			*= colorFactor;
	surfaceColor.a			= inPS.Material.Color.Diffuse.a;
	outputPixel				= surfaceColor.Clamp();
	return 0;
}

::llc::error_t llc::psTerrestrial(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel) { outputPixel = ::shaderPlanet(constants, inPS); return 0; }
::llc::error_t llc::psGasGiant  (const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel) { outputPixel = ::shaderPlanet(constants, inPS); return 0; }
::llc::error_t llc::psIceGiant  (const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel) { outputPixel = ::shaderPlanet(constants, inPS); return 0; }
::llc::error_t llc::psDwarf     (const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel) { outputPixel = ::shaderPlanet(constants, inPS); return 0; }
::llc::error_t llc::psMassiveSolid(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel) { outputPixel = ::shaderPlanet(constants, inPS); return 0; }

::llc::error_t			llc::psBackgroundMilkyWay
	( const ::llc::SEngineSceneConstants & constants
	, const ::llc::SPSIn & inPS
	, ::llc::bgra & outputPixel
	) {
	const ::llc::rgbaf		materialColor				= ::shaderSample(inPS, inPS.WeightedUV);
	const ::llc::rgbaf		cubicColor					= materialColor * materialColor * materialColor * materialColor;
	const float				randWeight					= .5f + .5f * ::shaderRand(inPS.WeightedUV);
	const float				randWaveR					= .5f + .5f * ::shaderRand(inPS.WeightedUV * (float)::sin(constants.Time));
	const float				randWaveG					= .5f + .5f * ::shaderRand(inPS.WeightedUV * (float)::sin(constants.Time) * 2);
	const float				randWaveB					= .5f + .5f * ::shaderRand(inPS.WeightedUV * (float)::sin(constants.Time) * 3);
	const float				colorFactor				= ::abs(randWaveR - randWeight);
	if(colorFactor > .2f)
		return 0;
	const float				nodeZ						= inPS.NodeConstants.NodeSize.z ? inPS.NodeConstants.NodeSize.z : 1;
	const ::llc::n2f2_t		scaledUV					= {inPS.WeightedUV.x * inPS.NodeConstants.NodeSize.x, inPS.WeightedUV.y * nodeZ};
	const float				randFactor					= .25f + ::llc::max(0.0f, (float)::sin(((scaledUV - ::llc::n2f2_t{inPS.NodeConstants.NodeSize.x * .25f, nodeZ * .5f}).Length() - constants.Time * (.005f / nodeZ)) * ::llc::math_pi * 100));
	const ::llc::rgbaf		randWaveColor				= {::llc::max(0.0f, randWaveR * .95f), ::llc::max(0.0f, randWaveG * .50f), ::llc::max(0.0f, randWaveB * .125f), 1};
	::llc::rgbaf			finalColor					= cubicColor * cubicColor * randWaveColor * randFactor * 4.0f + cubicColor * colorFactor;
	finalColor.a			= inPS.Material.Color.Diffuse.a;
	outputPixel				= finalColor.Clamp();
	return 0;
}
