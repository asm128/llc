#include "llc_color.h"
#include "llc_engine_scene.h"
#include "llc_raster_lh.h"

#ifndef LLC_ENGINE_SHADER_FUNC_H_23627
#define LLC_ENGINE_SHADER_FUNC_H_23627

namespace llc
{
	::llc::rgbaf			lightCalcSpecular	(::llc::n3f2_t gEyePosW, float specularPower, ::llc::rgbaf specularLight, ::llc::rgbaf specularMaterial, ::llc::n3f2_t posW, ::llc::n3f2_t normalW, ::llc::n3f2_t lightVecW);
	::llc::rgbaf			lightCalcDiffuse	(::llc::rgbaf diffuseMaterial, ::llc::n3f2_t normalW, ::llc::n3f2_t lightVecW);

	::llc::error_t			psHidden			(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel);
	::llc::error_t			psSolid				(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel);
	::llc::error_t			psTerrestrial		(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel);

	tplt<tpnm _tCoord>
	static	::llc::error_t	drawLine			(const ::llc::n2u1_t & targetMetrics, const ::llc::line3<_tCoord> & line, const ::llc::m4f2_t & mWVP, ::llc::apod<::llc::n3f2_t>& out_Points, ::llc::gu2_t depth) {
		::llc::n3<_tCoord>			vA					= mWVP.Transform(line.A); 
		::llc::n3<_tCoord>			vB					= mWVP.Transform(line.B);
		return ::llc::drawLine(targetMetrics, ::llc::line3<_tCoord>{vA, vB}, out_Points, depth);
	}

	::llc::error_t			psGrid				(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel);
	::llc::error_t			psGridRuler			(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel);
	::llc::error_t			psCylinderIn		(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel);
	::llc::error_t			psCylinder			(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel);
	::llc::error_t			psSphereAxis		(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel);
	::llc::error_t			psSphereSolid		(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel);
	::llc::error_t			psSphereMeridian	(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel);
	::llc::error_t			psOrbit				(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel);
	::llc::error_t			psGravityCenter		(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel);
	::llc::error_t			psSunfire			(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel);
	::llc::error_t			psSun				(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel);
	::llc::error_t			psEarth				(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel);
	::llc::error_t			psGasGiant			(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel);
	::llc::error_t			psIceGiant			(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel);
	::llc::error_t			psDwarf				(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel);
	::llc::error_t			psMassiveSolid		(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel);
	::llc::error_t			psCloud				(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel);
	::llc::error_t			psClouds			(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel);
	::llc::error_t			psBackgroundMilkyWay(const ::llc::SEngineSceneConstants & constants, const ::llc::SPSIn & inPS, ::llc::bgra & outputPixel);

}  // namespace


#endif // LLC_ENGINE_SHADER_FUNC_H_23627
