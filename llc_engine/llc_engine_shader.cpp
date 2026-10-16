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
