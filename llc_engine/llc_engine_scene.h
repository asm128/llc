#include "llc_engine_renderbuffer.h"
#include "llc_engine_rendermesh.h"
#include "llc_engine_rendernode.h"
#include "llc_engine_rendersurface.h"

#include "llc_font.h"
#include "llc_raster_lh.h"
#include "llc_grid_color.h"
#include "llc_apod_tri.h"
#include "llc_apod_tri2.h"
#include "llc_apod_tri3.h"

#include <functional>

#ifndef LLC_ENGINE_SCENE_H_23627
#define LLC_ENGINE_SCENE_H_23627

namespace llc 
{
	
#pragma pack(push, 1)
	struct SEngineSceneConstants {
		::llc::m4f2_t			View					= {}; 
		::llc::m4f2_t			Perspective				= {}; 
		::llc::m4f2_t			Screen					= {}; 
		::llc::m4f2_t			VP						= {}; 
		::llc::m4f2_t			VPS						= {}; 
		::llc::n3f2_t			CameraPosition			= {};
		f2_t					CameraAngle				= 0;
		::llc::n3f2_t			CameraFront				= {}; 
		f2_t					Time					= 0;
		::llc::n3f2_t			LightDirection			= {}; 
		f2_t					LightSpotPower			= 0;
		::llc::n3f2_t			LightPosition			= {}; 
		f2_t					PaddingB				= 0;
	};
	stxp size_t		SENGINESCENECONSTANTSSIZE = sizeof(SEngineSceneConstants);


	struct SRenderLight {
		::llc::SRenderColor		Color					= {};
		::llc::n3f2_t			Position				= {};
		::llc::f2_t				Range					= 0;
		::llc::n3f2_t			Direction				= {};
		::llc::f2_t				SpotPower				= 0;
		::llc::u2_t				Type					= ::llc::LIGHT_TYPE_Point;
		::llc::u2_t				Padding[3]				= {};
	};

	struct SPSIn {
		const ::llc::SRenderNodeConstants & NodeConstants;
		::llc::n3f2_t			WeightedPosition	;
		::llc::n3f2_t			WeightedNormal		;
		::llc::n2f2_t			WeightedUV			;
		::llc::SRenderMaterial	Material			;
		::llc::view<const ::llc::gc8bgra>	Surfaces		;
		::llc::view<const ::llc::SRenderLight> Lights		= {};
	};

	stxp size_t		SPSINSIZE = sizeof(SPSIn);
#pragma pack(pop)

	typedef ::llc::err_t	(TFuncPixelShader)
		( const ::llc::SEngineSceneConstants	& constants
		, const ::llc::SPSIn					& inPS
		, ::llc::bgra							& outputPixel
		);

	struct SVSCache {
		::llc::atrif2_t			TriangleWeights			= {};
		::llc::an2s1_t			SolidPixelCoords		= {};
		::llc::an3f2_t			WireframePixelCoords	= {};
	};

	struct SVSOutput {
		::llc::atri3f2_t			PositionsWorld			= {};
		::llc::atri3f2_t			PositionsScreen			= {};
		::llc::atri3f2_t			Normals					= {};
		::llc::atri3f2_t			NormalsScreen			= {};
		::llc::atri2f2_t			UVs						= {};
	};

	struct SEngineRenderCache {
		::llc::SVSOutput		VertexShaderOutput		= {};
		::llc::SVSCache			VertexShaderCache		= {};
		::llc::apod<::llc::SRenderLight> LightsWorld		= {};
		::llc::apod<::llc::SRenderLight> LightsNode		= {};
	};

	struct SSkin {
		::llc::SRenderMaterial	Material;
		::llc::au2_t				Textures;

		::llc::err_t			Save			(::llc::au0_t & output) const { 
			if_fail_fe(llc::savePOD(output, Material));
			if_fail_fe(llc::saveView(output, Textures));
			return 0;
		}
		::llc::err_t			Load			(::llc::vcu0_t & input) {
			if_fail_fe(llc::loadPOD(input, Material));
			if_fail_fe(llc::loadView(input, Textures));
			return 0;
		}
	};

	typedef	::llc::llc_linear_map_pobj<::llc::SSkin>						SSkinManager;
	typedef	::llc::llc_linear_map_pobj<::std::function<TFuncPixelShader>>	SShaderManager;

	struct SEngineGraphics {
		::llc::SRenderBufferManager	Buffers		= {};
		::llc::SSurfaceManager		Surfaces	= {};
		::llc::SMeshManager			Meshes		= {};
		::llc::SSkinManager			Skins		= {};

		::llc::SShaderManager		Shaders		= {};
		::llc::SRasterFontManager	Fonts		= {};

		::llc::err_t				Save		(::llc::au0_t & output)		const	{
			if_fail_fe(Buffers	.Save(output));
			if_fail_fe(Surfaces	.Save(output));
			if_fail_fe(Meshes		.Save(output));
			if_fail_fe(Skins		.Save(output));
			//if_fail_fe(Shaders	.Save(output));
			//if_fail_fe(Fonts	.Save(output));
			return 0;
		}
		::llc::err_t				Load		(::llc::vcu0_t & input) {
			if_fail_fe(Buffers	.Load(input));
			if_fail_fe(Surfaces	.Load(input));
			if_fail_fe(Meshes		.Load(input));
			if_fail_fe(Skins		.Load(input));
			//if_fail_fe(Shaders	.Load(input));
			//if_fail_fe(Fonts	.Load(input));
			return 0;
		}
	};

	struct SRenderNodeList {
		::llc::au2_t					Solid					= {};
		::llc::au2_t					Alpha					= {};
	};

	struct SEngineScene {
		::llc::pobj<SEngineGraphics>	Graphics				= {};
		::llc::SRenderNodeManager		RenderNodes				= {};
		::llc::SEngineRenderCache		RenderCache				= {};

		::llc::err_t					BuildRenderList			(const ::llc::n3f2_t & cameraPosition, const ::llc::n3f2_t & cameraFront, ::llc::SRenderNodeList & renderList) const;
		::llc::err_t						BuildLightList			(::llc::apod<::llc::SRenderLight> & lights) const;
		::llc::err_t					CreateRenderNode		(const ::llc::SGeometryBuffers & geometry, ::llc::vcst_c name, bool createSkin);

		::llc::err_t					Load					(::llc::vcu0_t & input) {
			if_fail_fe(Graphics		->Load(input));
			if_fail_fe(RenderNodes	 .Load(input));
			//if_fail_fe(RenderCache	 .Load(input));
			return 0;
		}

		::llc::err_t					Save					(::llc::au0_t & output)	const	{
			if(Graphics) {
				if_fail_fe(Graphics->Save(output));
			}
			else {
				SEngineGraphics{}.Save(output);
			}
			if_fail_fe(RenderNodes.Save(output));
			//if_fail_fe(RenderCache.Save(output));
			return 0;
		}

		::llc::err_t					Clone					(::llc::u2_t iRenderNode, bool cloneSkin, bool cloneSurfaces, bool cloneShaders) {
			const ::llc::u2_t						iNewNode				= RenderNodes.Clone(iRenderNode);
			const ::llc::u2_t						idShaderSource			= RenderNodes[iRenderNode].Shader;
			if(cloneShaders && idShaderSource < Graphics->Shaders.size())
				if_fail_fe(RenderNodes[iNewNode].Shader = Graphics->Shaders.Clone(idShaderSource));

			const ::llc::u2_t						idSkinSource			= RenderNodes[iNewNode].Skin;
			if(cloneSkin && idSkinSource < Graphics->Skins.size()) {
				::llc::u2_t							idSkin;
				if_fail_fe(idSkin = Graphics->Skins.Clone(idSkinSource));
				RenderNodes[iNewNode].Skin		= idSkin;
				if(cloneSurfaces) {
					if(Graphics->Skins[idSkin]) {
						::llc::SSkin						& newSkin			= *Graphics->Skins[idSkin];
						for(::llc::u2_t iTexture = 0; iTexture < newSkin.Textures.size(); ++iTexture) {
							newSkin.Textures[iTexture]			= Graphics->Surfaces.Clone(newSkin.Textures[iTexture]);
						}
					}
				}
			}
			return iNewNode;
		}

	};

	::llc::err_t							lightFilter	(const ::llc::n3f2_t & position, ::llc::view<const ::llc::SRenderLight> lights, ::llc::apod<::llc::SRenderLight> & selected);

	::llc::err_t							drawScene
		( ::llc::g8bgra				backBufferColors
		, ::llc::gu2_t				backBufferDepth
		, ::llc::SEngineRenderCache	& renderCache
		, const ::llc::SEngineScene	& scene
		, const ::llc::n3f2_t		& cameraPosition
		, const ::llc::n3f2_t		& cameraTarget
		, const ::llc::n3f2_t		& cameraUp
		, const ::llc::minmaxf2_t	& nearFar
		, const ::llc::n3f2_t		& lightPos = {}
		, ::llc::f3_t				time = 0
		);

} // namespace

#endif // LLC_ENGINE_SCENE_H
