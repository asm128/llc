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
		float					CameraAngle				= 0;
		::llc::n3f2_t			CameraFront				= {}; 
		float					Time					= 0;
		::llc::n3f2_t			LightDirection			= {}; 
		float					LightSpotPower			= 0;
		::llc::n3f2_t			LightPosition			= {}; 
		float					PaddingB				= 0;
	};
	stacxpr size_t		SENGINESCENECONSTANTSSIZE = sizeof(SEngineSceneConstants);


	struct SPSIn {
		const ::llc::SRenderNodeConstants & NodeConstants;
		::llc::n3f2_t			WeightedPosition	;
		::llc::n3f2_t			WeightedNormal		;
		::llc::n2f2_t			WeightedUV			;
		::llc::SRenderMaterial	Material			;
		::llc::gc8bgra			Surface				; 
	};

	stacxpr size_t		SPSINSIZE = sizeof(SPSIn);
#pragma pack(pop)

	typedef ::llc::error_t	(TFuncPixelShader)
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
	};

	struct SSkin {
		::llc::SRenderMaterial	Material;
		::llc::au2_t				Textures;

		::llc::error_t			Save			(::llc::au0_t & output) const { 
			llc_necs(llc::savePOD(output, Material));
			llc_necs(llc::saveView(output, Textures));
			return 0;
		}
		::llc::error_t			Load			(::llc::vcu0_t & input) {
			llc_necs(llc::loadPOD(input, Material));
			llc_necs(llc::loadView(input, Textures));
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

		::llc::error_t				Save		(::llc::au0_t & output)		const	{
			llc_necs(Buffers	.Save(output));
			llc_necs(Surfaces	.Save(output));
			llc_necs(Meshes		.Save(output));
			llc_necs(Skins		.Save(output));
			//llc_necs(Shaders	.Save(output));
			//llc_necs(Fonts	.Save(output));
			return 0;
		}
		::llc::error_t				Load		(::llc::vcu0_t & input) {
			llc_necs(Buffers	.Load(input));
			llc_necs(Surfaces	.Load(input));
			llc_necs(Meshes		.Load(input));
			llc_necs(Skins		.Load(input));
			//llc_necs(Shaders	.Load(input));
			//llc_necs(Fonts	.Load(input));
			return 0;
		}
	};

	struct SEngineScene {
		::llc::pobj<SEngineGraphics>	Graphics				= {};
		::llc::SRenderNodeManager		RenderNodes				= {};
		::llc::SEngineRenderCache		RenderCache				= {};

		::llc::error_t					CreateRenderNode		(const ::llc::SGeometryBuffers & geometry, ::llc::vcst_c name, bool createSkin);

		::llc::error_t					Load					(::llc::vcu0_t & input) {
			llc_necs(Graphics		->Load(input));
			llc_necs(RenderNodes	 .Load(input));
			//llc_necs(RenderCache	 .Load(input));
			return 0;
		}

		::llc::error_t					Save					(::llc::au0_t & output)	const	{
			if(Graphics) 
				llc_necs(Graphics->Save(output));
			else {
				SEngineGraphics{}.Save(output);
			}
			llc_necs(RenderNodes.Save(output));
			//llc_necs(RenderCache.Save(output));
			return 0;
		}

		::llc::error_t					Clone					(::llc::u2_t iRenderNode, bool cloneSkin, bool cloneSurfaces, bool cloneShaders) {
			const ::llc::u2_t						iNewNode				= RenderNodes.Clone(iRenderNode);
			const ::llc::u2_t						idShaderSource			= RenderNodes[iRenderNode].Shader;
			if(cloneShaders && idShaderSource < Graphics->Shaders.size())
				llc_necs(RenderNodes[iNewNode].Shader = Graphics->Shaders.Clone(idShaderSource));

			const ::llc::u2_t						idSkinSource			= RenderNodes[iNewNode].Skin;
			if(cloneSkin && idSkinSource < Graphics->Skins.size()) {
				::llc::u2_t							idSkin;
				llc_necs(idSkin = Graphics->Skins.Clone(idSkinSource));
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

	::llc::error_t							drawScene
		( ::llc::g8bgra				backBufferColors
		, ::llc::gu2_t				backBufferDepth
		, ::llc::SEngineRenderCache	& renderCache
		, const ::llc::SEngineScene	& scene
		, const ::llc::n3f2_t		& cameraPosition
		, const ::llc::n3f2_t		& cameraTarget
		, const ::llc::n3f2_t		& cameraUp
		, const ::llc::minmaxf2_t	& nearFar
		);

} // namespace

#endif // LLC_ENGINE_SCENE_H