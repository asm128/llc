#include "llc_engine_rendercolor.h"
#include "llc_array_ptr.h"
#include "llc_matrix3.h"
#include "llc_camera.h"

#ifndef LLC_ENGINE_RENDERNODE_H_23627
#define LLC_ENGINE_RENDERNODE_H_23627

namespace llc 
{
	GDEFINE_ENUM_TYPE(CULL_MODE, u0_t);
	GDEFINE_ENUM_VALUE(CULL_MODE, CCW	, 0);
	GDEFINE_ENUM_VALUE(CULL_MODE, CW	, 1);
	GDEFINE_ENUM_VALUE(CULL_MODE, NONE	, 2);

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
		::llc::u2_t				Mesh			= (u2_t)-1;
		::llc::u2_t				Slice			= (u2_t)-1;
		::llc::u2_t				Shader			= (u2_t)-1;
		::llc::u2_t				Skin			= (u2_t)-1;
	};
	struct SRenderMaterial {
		::llc::SRenderColor		Color			= {};
		::llc::n3f2_t			Emission		= {};
		f2_t					SpecularPower	= {};
	};
	stxp size_t		SRENDERMATERIALSIZE = sizeof(SRenderMaterial);

	struct SRenderNodeConstants {
		::llc::SRenderMaterial	Material				= {};
		::llc::m4f2_t			MVP						= ::llc::m4f2_t::GetIdentity();
		::llc::m4f2_t			Model					= ::llc::m4f2_t::GetIdentity();
		::llc::m4f2_t			ModelInverse			= ::llc::m4f2_t::GetIdentity();
		::llc::m4f2_t			ModelInverseTranspose	= ::llc::m4f2_t::GetIdentity();
		::llc::n3f2_t			NodeSize				= {}; 
		f2_t					PaddingC				= 0;
	};
	stxp size_t		SRENDERNODECONSTANTSSIZE = sizeof(SRenderNodeConstants);


	GDEFINE_ENUM_TYPE(LIGHT_TYPE, u0_t);
	GDEFINE_ENUM_VALUE(LIGHT_TYPE, Directional	, 0);
	GDEFINE_ENUM_VALUE(LIGHT_TYPE, Point		, 1);
	GDEFINE_ENUM_VALUE(LIGHT_TYPE, Spot			, 2);

	struct SLightSpot {
		::llc::n3f2_t			Direction;
		::llc::SRenderColor		Color;
		f2_t					SpotPower;
		f2_t					Range;
	};
	struct SLightPoint {
		::llc::SRenderColor		Color;
		f2_t					Range;
	};
	struct SLightDirectional {
		::llc::n3f2_t			Direction;
		::llc::SRenderColor		Color;
	};
	struct SLight {
		::llc::LIGHT_TYPE		Type;
		::llc::u1_t				Index;
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

		const ::llc::SRenderNode&			operator[]		(::llc::u2_t index)							const	{ return RenderNodes[index]; }
		SRenderNode&						operator[]		(::llc::u2_t index)									{ return RenderNodes[index]; }

		::llc::u2_t							size			()											const	{ return RenderNodes.size(); }

		::llc::err_t						Save			(::llc::apod<u0_t> & output) const { 
			if_fail_fe(llc::saveView(output, RenderNodes	));
			if_fail_fe(llc::saveView(output, Flags			));
			if_fail_fe(llc::saveView(output, Transforms		));
			if_fail_fe(llc::saveView(output, BaseTransforms	));
			for(::llc::u2_t iEntity = 0; iEntity < RenderNodes.size(); ++iEntity) {
				if_fail_fef(llc::saveView(output, Lights	[iEntity] ? ::llc::v1<const ::llc::SLight >{*Lights	[iEntity]} : ::llc::v1<const ::llc::SLight >{}), "iEntity: %i", iEntity);
				if_fail_fef(llc::saveView(output, Cameras	[iEntity] ? ::llc::v1<const ::llc::SCamera>{*Cameras[iEntity]} : ::llc::v1<const ::llc::SCamera>{}), "iEntity: %i", iEntity);
			}
			if_fail_fe(output.append({(const u0_t*)&LightsDirectional.size(), 4}));
			for(::llc::u2_t iEntity = 0; iEntity < LightsDirectional.size(); ++iEntity)
				if_fail_fef(llc::saveView(output, LightsDirectional[iEntity] ? ::llc::v1<const ::llc::SLightDirectional>{*LightsDirectional[iEntity]} : ::llc::v1<const ::llc::SLightDirectional>{}), "iEntity: %i", iEntity);

			if_fail_fe(output.append({(const u0_t*)&LightsPoint.size(), 4}));
			for(::llc::u2_t iEntity = 0; iEntity < LightsPoint.size(); ++iEntity)
				if_fail_fef(llc::saveView(output, LightsPoint[iEntity] ? ::llc::v1<const ::llc::SLightPoint>{*LightsPoint[iEntity]} : ::llc::v1<const ::llc::SLightPoint>{}), "iEntity: %i", iEntity);

			if_fail_fe(output.append({(const u0_t*)&LightsSpot.size(), 4}));
			for(::llc::u2_t iEntity = 0; iEntity < LightsSpot.size(); ++iEntity)
				if_fail_fef(llc::saveView(output, LightsSpot[iEntity] ? ::llc::v1<const ::llc::SLightSpot>{*LightsSpot[iEntity]} : ::llc::v1<const ::llc::SLightSpot>{}), "iEntity: %i", iEntity);

			info_printf("Saved %s, %i", "RenderNodes", RenderNodes.size());
			return 0;
		}

		::llc::err_t						Load			(::llc::vcu0_t & input) {
			if_fail_fe(llc::loadView(input, RenderNodes		));
			if_fail_fe(llc::loadView(input, Flags			));
			if_fail_fe(llc::loadView(input, Transforms		));
			if_fail_fe(llc::loadView(input, BaseTransforms	));
			if_fail_fef(Lights	.resize(RenderNodes.size()), "size: %i", RenderNodes.size());
			if_fail_fef(Cameras	.resize(RenderNodes.size()), "size: %i", RenderNodes.size());
			for(::llc::u2_t iEntity = 0; iEntity < RenderNodes.size(); ++iEntity) {
				if(!Lights	[iEntity]) 
					if_true_fef(0 == Lights	[iEntity].create(), "iEntity: %i", iEntity);
				if(!Cameras	[iEntity]) 
					if_true_fef(0 == Cameras	[iEntity].create(), "iEntity: %i", iEntity);
				if_fail_fef(llc::loadView(input, *Lights	[iEntity]), "iEntity: %i", iEntity);
				if_fail_fef(llc::loadView(input, *Cameras	[iEntity]), "iEntity: %i", iEntity);
			}

			if_fail_fe(LightsDirectional.resize(*(::llc::u2_t*)input.begin()));
			input								= {input.begin() + sizeof(::llc::u2_t), input.size() - 4};
			for(::llc::u2_t iEntity = 0; iEntity < LightsDirectional.size(); ++iEntity) {
				if(!LightsDirectional[iEntity])
					if_true_fef(0 == LightsDirectional[iEntity].create(), "iEntity: %i", iEntity);
				if_fail_fef(llc::loadView(input, *LightsDirectional[iEntity]), "iEntity: %i", iEntity);
			}

			if_fail_fe(LightsPoint.resize(*(::llc::u2_t*)input.begin()));
			input								= {input.begin() + sizeof(::llc::u2_t), input.size() - 4};
			for(::llc::u2_t iEntity = 0; iEntity < LightsPoint.size(); ++iEntity) {
				if(!LightsPoint[iEntity])
					if_true_fef(0 == LightsPoint[iEntity].create(), "iEntity: %i", iEntity);
				if_fail_fef(llc::loadView(input, *LightsPoint[iEntity]), "iEntity: %i", iEntity);
			}

			if_fail_fe(LightsSpot.resize(*(::llc::u2_t*)input.begin()));
			input								= {input.begin() + sizeof(::llc::u2_t), input.size() - 4};
			for(::llc::u2_t iEntity = 0; iEntity < LightsSpot.size(); ++iEntity) {
				if(!LightsSpot[iEntity])
					if_true_fef(0 == LightsSpot[iEntity].create(), "iEntity: %i", iEntity);
				if_fail_fef(llc::loadView(input, *LightsSpot[iEntity]), "iEntity: %i", iEntity);
			}
			return 0;
		}
		::llc::err_t						Create		()	{
			if_fail_fe(Transforms		.push_back({}));
			if_fail_fe(BaseTransforms	.push_back({}));
			if_fail_fe(Lights			.push_back({}));
			if_fail_fe(Cameras		.push_back({}));
			if_fail_fe(Flags			.push_back({}));
			return RenderNodes.push_back({(::llc::u2_t)-1});
		}

		::llc::err_t						Clone		(::llc::u2_t iNode)	{
			if_fail_fe(Transforms		.push_back(::llc::SRenderNodeConstants					{Transforms		[iNode]}));
			if_fail_fe(BaseTransforms	.push_back(::llc::SRenderNodeConstants					{BaseTransforms	[iNode]}));
			if_fail_fe(Lights			.push_back(::llc::pobj<::llc::apod<::llc::SLight	>>	{Lights			[iNode]}));
			if_fail_fe(Cameras		.push_back(::llc::pobj<::llc::apod<::llc::SCamera	>>	{Cameras		[iNode]}));
			if_fail_fe(Flags			.push_back(::llc::SRenderNodeFlags						{Flags			[iNode]}));
			return RenderNodes.push_back(::llc::SRenderNode{RenderNodes	[iNode]});
		}

		::llc::err_t						Delete		(::llc::u2_t indexNode)	{
			if_fail_fe(Transforms			.remove_unordered(indexNode));
			if_fail_fe(BaseTransforms		.remove_unordered(indexNode));
			if_fail_fe(Lights				.remove_unordered(indexNode));
			if_fail_fe(Cameras			.remove_unordered(indexNode));
			if_fail_fe(Flags				.remove_unordered(indexNode));
			return RenderNodes.remove_unordered(indexNode);
		}
	};
} // namespace

#endif // LLC_ENGINE_RENDERNODE_H_23627
