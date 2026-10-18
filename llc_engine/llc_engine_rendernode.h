#include "llc_engine_rendercolor.h"
#include "llc_array_ptr.h"
#include "llc_matrix.h"
#include "llc_camera.h"

#ifndef LLC_ENGINE_RENDERNODE_H_23627
#define LLC_ENGINE_RENDERNODE_H_23627

namespace llc 
{
#pragma pack(push, 1)
	// ------------------ Render Node
	struct SRenderNodeFlags {
		bool					NoAmbient			: 1;
		bool					NoDiffuse			: 1;
		bool					NoSpecular			: 1;
		bool					NoAlphaTest			: 1;
		bool					NoAlphaBlend		: 1;
		bool					NoDraw				: 1;
		//bool					NoTransformInherit	: 1;
	};
	struct SRenderNode {
		uint32_t				Mesh			= (u2_t)-1;
		uint32_t				Slice			= (u2_t)-1;
		uint32_t				Shader			= (u2_t)-1;
		uint32_t				Skin			= (u2_t)-1;
	};
	struct SRenderMaterial {
		::llc::SRenderColor		Color			= {};
		::llc::n3f2_t			Emission		= {};
		float					SpecularPower	= {};
	};
	stacxpr size_t		SRENDERMATERIALSIZE = sizeof(SRenderMaterial);

	struct SRenderNodeConstants {
		::llc::SRenderMaterial	Material				= {};
		::llc::m4f32			MVP						= ::llc::m4f32::GetIdentity();
		::llc::m4f32			Model					= ::llc::m4f32::GetIdentity();
		::llc::m4f32			ModelInverse			= ::llc::m4f32::GetIdentity();
		::llc::m4f32			ModelInverseTranspose	= ::llc::m4f32::GetIdentity();
		::llc::n3f2_t			NodeSize				= {}; 
		float					PaddingC				= 0;
	};
	stacxpr size_t		SRENDERNODECONSTANTSSIZE = sizeof(SRenderNodeConstants);


	GDEFINE_ENUM_TYPE(LIGHT_TYPE, uint8_t);
	GDEFINE_ENUM_VALUE(LIGHT_TYPE, Directional	, 0);
	GDEFINE_ENUM_VALUE(LIGHT_TYPE, Point		, 1);
	GDEFINE_ENUM_VALUE(LIGHT_TYPE, Spot			, 2);

	struct SLightSpot {
		::llc::n3f2_t			Direction;
		::llc::SRenderColor		Color;
		float					SpotPower;
		float					Range;
	};
	struct SLightPoint {
		::llc::SRenderColor		Color;
		float					Range;
	};
	struct SLightDirectional {
		::llc::n3f2_t			Direction;
		::llc::SRenderColor		Color;
	};
	struct SLight {
		::llc::LIGHT_TYPE		Type;
		uint16_t				Index;
	};
	struct SCamera : SCameraPoints, SCameraRange, SCameraVectors {};
#pragma pack(pop)

	struct SRenderNodeManager {
		::llc::apod<::llc::SRenderNode>				RenderNodes			= {};
		::llc::apod<::llc::SRenderNodeFlags>		Flags				= {};
		::llc::apod<::llc::SRenderNodeConstants>	Transforms			= {};
		::llc::apod<::llc::SRenderNodeConstants>	BaseTransforms		= {};
		//
		::llc::apobj<::llc::apod<::llc::SLight>>			Lights				= {};	// Map each light to either LightsDirectional, LightsPoint or LightsSpot
		::llc::apobj<::llc::apod<::llc::SCamera>>			Cameras				= {};	// Currently one camera type
		//
		::llc::apobj<::llc::apod<::llc::SLightDirectional>>	LightsDirectional	= {};
		::llc::apobj<::llc::apod<::llc::SLightPoint>>		LightsPoint			= {};
		::llc::apobj<::llc::apod<::llc::SLightSpot>>		LightsSpot			= {};

		const ::llc::SRenderNode&			operator[]		(uint32_t index)							const	{ return RenderNodes[index]; }
		SRenderNode&						operator[]		(uint32_t index)									{ return RenderNodes[index]; }

		uint32_t							size			()											const	{ return RenderNodes.size(); }

		::llc::error_t						Save			(::llc::apod<uint8_t> & output) const { 
			llc_necs(llc::saveView(output, RenderNodes	));
			llc_necs(llc::saveView(output, Flags			));
			llc_necs(llc::saveView(output, Transforms		));
			llc_necs(llc::saveView(output, BaseTransforms	));
			for(uint32_t iEntity = 0; iEntity < RenderNodes.size(); ++iEntity) {
				llc_necall(llc::saveView(output, Lights	[iEntity] ? ::llc::v1<const ::llc::SLight >{*Lights	[iEntity]} : ::llc::v1<const ::llc::SLight >{}), "iEntity: %i", iEntity);
				llc_necall(llc::saveView(output, Cameras	[iEntity] ? ::llc::v1<const ::llc::SCamera>{*Cameras[iEntity]} : ::llc::v1<const ::llc::SCamera>{}), "iEntity: %i", iEntity);
			}
			llc_necs(output.append({(const uint8_t*)&LightsDirectional.size(), 4}));
			for(uint32_t iEntity = 0; iEntity < LightsDirectional.size(); ++iEntity)
				llc_necall(llc::saveView(output, LightsDirectional[iEntity] ? ::llc::v1<const ::llc::SLightDirectional>{*LightsDirectional[iEntity]} : ::llc::v1<const ::llc::SLightDirectional>{}), "iEntity: %i", iEntity);

			llc_necs(output.append({(const uint8_t*)&LightsPoint.size(), 4}));
			for(uint32_t iEntity = 0; iEntity < LightsPoint.size(); ++iEntity)
				llc_necall(llc::saveView(output, LightsPoint[iEntity] ? ::llc::v1<const ::llc::SLightPoint>{*LightsPoint[iEntity]} : ::llc::v1<const ::llc::SLightPoint>{}), "iEntity: %i", iEntity);

			llc_necs(output.append({(const uint8_t*)&LightsSpot.size(), 4}));
			for(uint32_t iEntity = 0; iEntity < LightsSpot.size(); ++iEntity)
				llc_necall(llc::saveView(output, LightsSpot[iEntity] ? ::llc::v1<const ::llc::SLightSpot>{*LightsSpot[iEntity]} : ::llc::v1<const ::llc::SLightSpot>{}), "iEntity: %i", iEntity);

			info_printf("Saved %s, %i", "RenderNodes", RenderNodes.size());
			return 0;
		}

		::llc::error_t						Load			(::llc::vcu0_t & input) {
			llc_necs(llc::loadView(input, RenderNodes		));
			llc_necs(llc::loadView(input, Flags			));
			llc_necs(llc::loadView(input, Transforms		));
			llc_necs(llc::loadView(input, BaseTransforms	));
			llc_necall(Lights	.resize(RenderNodes.size()), "size: %i", RenderNodes.size());
			llc_necall(Cameras	.resize(RenderNodes.size()), "size: %i", RenderNodes.size());
			for(uint32_t iEntity = 0; iEntity < RenderNodes.size(); ++iEntity) {
				if(!Lights	[iEntity]) 
					if_true_fef(0 == Lights	[iEntity].create(), "iEntity: %i", iEntity);
				if(!Cameras	[iEntity]) 
					if_true_fef(0 == Cameras	[iEntity].create(), "iEntity: %i", iEntity);
				llc_necall(llc::loadView(input, *Lights	[iEntity]), "iEntity: %i", iEntity);
				llc_necall(llc::loadView(input, *Cameras	[iEntity]), "iEntity: %i", iEntity);
			}

			llc_necs(LightsDirectional.resize(*(uint32_t*)input.begin()));
			input								= {input.begin() + sizeof(uint32_t), input.size() - 4};
			for(uint32_t iEntity = 0; iEntity < LightsDirectional.size(); ++iEntity) {
				if(!LightsDirectional[iEntity])
					if_true_fef(0 == LightsDirectional[iEntity].create(), "iEntity: %i", iEntity);
				llc_necall(llc::loadView(input, *LightsDirectional[iEntity]), "iEntity: %i", iEntity);
			}

			llc_necs(LightsPoint.resize(*(uint32_t*)input.begin()));
			input								= {input.begin() + sizeof(uint32_t), input.size() - 4};
			for(uint32_t iEntity = 0; iEntity < LightsPoint.size(); ++iEntity) {
				if(!LightsPoint[iEntity])
					if_true_fef(0 == LightsPoint[iEntity].create(), "iEntity: %i", iEntity);
				llc_necall(llc::loadView(input, *LightsPoint[iEntity]), "iEntity: %i", iEntity);
			}

			llc_necs(LightsSpot.resize(*(uint32_t*)input.begin()));
			input								= {input.begin() + sizeof(uint32_t), input.size() - 4};
			for(uint32_t iEntity = 0; iEntity < LightsSpot.size(); ++iEntity) {
				if(!LightsSpot[iEntity])
					if_true_fef(0 == LightsSpot[iEntity].create(), "iEntity: %i", iEntity);
				llc_necall(llc::loadView(input, *LightsSpot[iEntity]), "iEntity: %i", iEntity);
			}
			return 0;
		}
		::llc::error_t						Create		()	{
			llc_necs(Transforms		.push_back({}));
			llc_necs(BaseTransforms	.push_back({}));
			llc_necs(Lights			.push_back({}));
			llc_necs(Cameras		.push_back({}));
			llc_necs(Flags			.push_back({}));
			return RenderNodes.push_back({(uint32_t)-1});
		}

		::llc::error_t						Clone		(uint32_t iNode)	{
			llc_necs(Transforms		.push_back(::llc::SRenderNodeConstants					{Transforms		[iNode]}));
			llc_necs(BaseTransforms	.push_back(::llc::SRenderNodeConstants					{BaseTransforms	[iNode]}));
			llc_necs(Lights			.push_back(::llc::pobj<::llc::apod<::llc::SLight	>>	{Lights			[iNode]}));
			llc_necs(Cameras		.push_back(::llc::pobj<::llc::apod<::llc::SCamera	>>	{Cameras		[iNode]}));
			llc_necs(Flags			.push_back(::llc::SRenderNodeFlags						{Flags			[iNode]}));
			return RenderNodes.push_back(::llc::SRenderNode{RenderNodes	[iNode]});
		}

		::llc::error_t						Delete		(uint32_t indexNode)	{
			llc_necs(Transforms			.remove_unordered(indexNode));
			llc_necs(BaseTransforms		.remove_unordered(indexNode));
			llc_necs(Lights				.remove_unordered(indexNode));
			llc_necs(Cameras			.remove_unordered(indexNode));
			llc_necs(Flags				.remove_unordered(indexNode));
			return RenderNodes.remove_unordered(indexNode);
		}
	};
} // namespace

#endif // LLC_ENGINE_RENDERNODE_H_23627
