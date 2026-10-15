#include "llc_engine_entity.h"
#include "llc_engine_scene.h"
#include "llc_rigid_body.h"
#include "llc_linear_map_pod.h"
#include "llc_geometry.h"
#include "llc_png.h"

#ifndef LLC_ENGINE_H_23627
#define LLC_ENGINE_H_23627

namespace llc 
{
	::llc::error_t					updateEntityTransforms			
		( uint32_t								iEntity
		, const ::llc::SEntity			& entity
		, const ::llc::SVirtualEntityManager	& managedEntities	
		, ::llc::SRigidBodyIntegrator			& integrator		
		, ::llc::SRenderNodeManager				& renderNodes
		);

	struct SEngine {
		::llc::pobj<::llc::SEngineScene>	Scene			;
		::llc::SVirtualEntityManager		Entities		;
		::llc::SRigidBodyIntegrator			Integrator		;

		::llc::lmpod<SParamsBox			, uint32_t>	ParamsBox			;
		::llc::lmpod<SParamsDisc		, uint32_t>	ParamsDisc			;
		::llc::lmpod<SParamsCircle		, uint32_t>	ParamsCircleSide	;
		::llc::lmpod<SParamsCylinderWall, uint32_t>	ParamsCylinderWall	;
		::llc::lmpod<SParamsCylinderWall, uint32_t>	ParamsCylinder		;
		::llc::lmpod<SParamsHelix		, uint32_t>	ParamsFigure0		;
		::llc::lmpod<SParamsTender		, uint32_t>	ParamsTender		;
		::llc::lmpod<SParamsGrid		, uint32_t>	ParamsGrid			;
		::llc::lmpod<SParamsHelix		, uint32_t>	ParamsHelixHalf		;
		::llc::lmpod<SParamsHelix		, uint32_t>	ParamsHelix			;
		::llc::lmpod<SParamsSphere		, uint32_t>	ParamsSphere		;
		::llc::lmpod<SParamsTube		, uint32_t>	ParamsTube			;
		::llc::lmpod<SParamsRingSide	, uint32_t>	ParamsRingSide		;
		::llc::lmpod<SParamsRingSide	, uint32_t>	ParamsRingFlat		;

		::llc::avcsc_t					ImageNames		= {};
		::llc::au2_t					Images			= {};

		::llc::SPNGData						PNGCache		= {};

		inline	::llc::error_t		GetRigidBody		(::llc::eid_t idEntity)					const	{ return Entities[idEntity].RigidBody; }
		inline	::llc::error_t		GetRenderNode		(::llc::eid_t idEntity)					const	{ return Entities[idEntity].RenderNode; }
		inline	::llc::error_t		GetParentIndex		(::llc::eid_t idEntity)					const	{ return Entities[idEntity].Parent; }

		inline	::llc::error_t		GetChildCount		(::llc::eid_t idEntity)					const	{ const ::llc::papod<::llc::eid_t> & children = Entities.Children[idEntity]; return children ? children->size() : 0; }
		inline	::llc::vceid		GetChildren			(::llc::eid_t idEntity)					const	{ const ::llc::papod<::llc::eid_t> & children = Entities.Children[idEntity]; return children ? ::llc::vceid{*children} : ::llc::vceid{}; }
		inline	::llc::veid			GetChildren			(::llc::eid_t idEntity)							{ ::llc::papod<::llc::eid_t> & children = Entities.Children[idEntity]; return children ? *children : ::llc::veid{}; }

		inline	const SBodyFlags&	GetBodyFlags		(::llc::eid_t idEntity)					const	{ return Integrator.GetBodyFlags (Entities[idEntity].RigidBody); }
		inline	const SBodyFrame&	GetBodyFrame		(::llc::eid_t idEntity)					const	{ return Integrator.GetBodyFrame (Entities[idEntity].RigidBody); }
		inline	const SBodyForces&	GetBodyForces		(::llc::eid_t idEntity)					const	{ return Integrator.GetBodyForces(Entities[idEntity].RigidBody); }
		inline	const SBodyMass&	GetBodyMass			(::llc::eid_t idEntity)					const	{ return Integrator.GetBodyMass  (Entities[idEntity].RigidBody); }
		inline	const SBodyCenter&	GetBodyCenter		(::llc::eid_t idEntity)					const	{ return Integrator.GetBodyCenter(Entities[idEntity].RigidBody); }
		inline	SBodyFlags&			GetBodyFlags		(::llc::eid_t idEntity)							{ return Integrator.GetBodyFlags (Entities[idEntity].RigidBody); }
		inline	SBodyFrame&			GetBodyFrame		(::llc::eid_t idEntity)							{ return Integrator.GetBodyFrame (Entities[idEntity].RigidBody); }
		inline	SBodyForces&		GetBodyForces		(::llc::eid_t idEntity)							{ return Integrator.GetBodyForces(Entities[idEntity].RigidBody); }
		inline	SBodyMass&			GetBodyMass			(::llc::eid_t idEntity)							{ return Integrator.GetBodyMass  (Entities[idEntity].RigidBody); }
		inline	SBodyCenter&		GetBodyCenter		(::llc::eid_t idEntity)							{ return Integrator.GetBodyCenter(Entities[idEntity].RigidBody); }

		inline	::llc::error_t		SetParent			(::llc::eid_t idEntity, ::llc::eid_t idParent)	{ return Entities.SetParent(idEntity, idParent); }

		::llc::error_t				Load				(::llc::vcu0_t & input)			{
			llc_necs(Scene		->Load(input));
			llc_necs(Entities	.Load(input));
			llc_necs(Integrator	.Load(input));
			return 0;
		}

		::llc::error_t				Save				(::llc::au0_t & output)	const	{
			if(Scene)
				llc_necs(Scene->Save(output));
			else 
				llc_necs(llc::SEngineScene{}.Save(output));

			llc_necs(Entities	.Save(output));
			llc_necs(Integrator	.Save(output));
			return 0;
		}

		::llc::error_t				Update				(double secondsLastFrame)		{
			Integrator.Integrate(secondsLastFrame);
			for(uint32_t iEntity = 0; iEntity < Entities.size(); ++iEntity) {
				::llc::SEntity			& entity			= Entities[iEntity];
				if(entity.Parent != -1)
					continue;

				e_if(failed(::llc::updateEntityTransforms(iEntity, entity, Entities, Integrator, Scene->RenderNodes)));
			}
			return 0;
		}

		// This function messes up with the Entities array so be careful of not having any references to its contents or it may end up trying to access invalid memory
		::llc::error_t				Clone				(uint32_t iEntitySource, bool cloneSkin, bool cloneSurfaces, bool cloneShaders) {
			const ::llc::SEntity		entitySource		= Entities[iEntitySource];
			int32_t							iEntityNew			= Entities.Create();
			::llc::SEntity			& entityNew			= Entities[iEntityNew];
			entityNew.RenderNode		= Scene->Clone(entitySource.RenderNode, cloneSkin, cloneSurfaces, cloneShaders);
			entityNew.RigidBody			= ((uint32_t)entitySource.RigidBody < Integrator.Flags.size()) ? Integrator.Clone(entitySource.RigidBody) : (uint32_t)-1;
			entityNew.Parent			= entitySource.Parent;

			const ::llc::pau2_t				childrenSource		= Entities.Children[iEntitySource];
			if(childrenSource && childrenSource->size()) {
				::llc::pau2_t					childrenNew			= Entities.Children[iEntityNew];
				for(uint32_t iChild = 0; iChild < childrenSource->size(); ++iChild) {
					uint32_t						entityChild			= Clone((*childrenSource)[iChild], cloneSkin, cloneSurfaces, cloneShaders);
					Entities[entityChild].Parent= iEntityNew;
					childrenNew->push_back(entityChild);
				}
				Entities.Children[iEntityNew]	= childrenNew;
			}
			return iEntityNew;
		}

		inline	::llc::error_t		SetNodeSize			(uint32_t iEntity, const ::llc::n3f2_t & size)					{ Scene->RenderNodes.BaseTransforms[GetRenderNode(iEntity)].NodeSize = size; return 0; }
		inline	::llc::error_t		SetMeshScale		(uint32_t iEntity, const ::llc::n3f2_t & scale, bool halfSizes)	{ Scene->RenderNodes.BaseTransforms[GetRenderNode(iEntity)].Model.Scale(scale, true); 
			if(halfSizes) {
				llc_necs(SetHalfSizes(iEntity, scale * .5f));
				llc_necs(SetNodeSize(iEntity, scale));
			}
			return 0; 
		}
		inline	::llc::error_t		SetMeshPosition		(uint32_t iEntity, const ::llc::n3f2_t & position)				{ Scene->RenderNodes.BaseTransforms[GetRenderNode(iEntity)].Model.SetTranslation(position, false); return 0; }
		::llc::error_t				SetColorDiffuse		(uint32_t iEntity, const ::llc::rgbaf & diffuse)				{ Scene->Graphics->Skins[Scene->RenderNodes[GetRenderNode(iEntity)].Skin]->Material.Color.Diffuse = diffuse; return 0; }
		::llc::error_t				SetShader			(uint32_t iEntity, const ::std::function<::llc::TFuncPixelShader> & shader, ::llc::vcs name) {
			const uint32_t					iShader				= Scene->RenderNodes[GetRenderNode(iEntity)].Shader;
			Scene->Graphics->Shaders[iShader].create(shader);
			Scene->Graphics->Shaders.Names[iShader] = name;
			return 0;
		}
		inline	::llc::error_t		IsPhysicsActive		(uint32_t iEntity)										const	{ return Integrator.Active	(Entities[iEntity].RigidBody) ? 1 : 0; }
		inline	float				GetMass				(uint32_t iEntity)										const	{ return Integrator.GetMass	(Entities[iEntity].RigidBody);	}

		inline	::llc::error_t		InvalidateTransform	(uint32_t iEntity)												{ Integrator.Flags[GetRigidBody(iEntity)].UpdatedTransform = false;		return 0; }

		inline	::llc::error_t		GetPosition			(uint32_t iEntity, ::llc::n3f2_t & position)				const	{ Integrator.GetPosition	(GetRigidBody(iEntity), position);			return 0; }
		inline	::llc::error_t		GetVelocity			(uint32_t iEntity, ::llc::n3f2_t & velocity)				const	{ Integrator.GetVelocity	(GetRigidBody(iEntity), velocity);			return 0; }
		inline	::llc::error_t		GetAcceleration		(uint32_t iEntity, ::llc::n3f2_t & acceleration)			const	{ Integrator.GetAcceleration(GetRigidBody(iEntity), acceleration);		return 0; }
		inline	::llc::error_t		GetRotation			(uint32_t iEntity, ::llc::n3f2_t & velocity)				const	{ Integrator.GetRotation	(GetRigidBody(iEntity), velocity);			return 0; }
		inline	::llc::error_t		GetOrientation		(uint32_t iEntity, ::llc::quatf2_t & orientation)		const	{ Integrator.GetOrientation	(GetRigidBody(iEntity), orientation);		return 0; }
		inline	::llc::error_t		SetPhysicsActive	(uint32_t iEntity, bool active)									{ Integrator.SetActive		(GetRigidBody(iEntity), active);			return 0; }
		inline	::llc::error_t		SetCollides			(uint32_t iEntity, bool collides)								{ Integrator.SetCollides	(GetRigidBody(iEntity), collides);			return 0; }
		inline	::llc::error_t		SetHalfSizes		(uint32_t iEntity, const ::llc::n3f2_t & halfSizes)				{ Integrator.SetHalfSizes	(GetRigidBody(iEntity), halfSizes);			return 0; }
		inline	::llc::error_t		SetMass				(uint32_t iEntity, float mass)									{ Integrator.SetMass		(GetRigidBody(iEntity), mass);				return 0; }
		inline	::llc::error_t		SetMassInverse		(uint32_t iEntity, float inverseMass)							{ Integrator.SetMassInverse	(GetRigidBody(iEntity), inverseMass);		return 0; }
		inline	::llc::error_t		SetPosition			(uint32_t iEntity, const ::llc::n3f2_t & position)				{ Integrator.SetPosition	(GetRigidBody(iEntity), position);			return 0; }
		inline	::llc::error_t		SetVelocity			(uint32_t iEntity, const ::llc::n3f2_t & velocity)				{ Integrator.SetVelocity	(GetRigidBody(iEntity), velocity);			return 0; }
		inline	::llc::error_t		SetAcceleration		(uint32_t iEntity, const ::llc::n3f2_t & acceleration)			{ Integrator.SetAcceleration(GetRigidBody(iEntity), acceleration);		return 0; }
		inline	::llc::error_t		SetRotation			(uint32_t iEntity, const ::llc::n3f2_t & velocity)				{ Integrator.SetRotation	(GetRigidBody(iEntity), velocity);			return 0; }
		inline	::llc::error_t		SetOrientation		(uint32_t iEntity, const ::llc::quatf2_t & orientation)			{ Integrator.SetOrientation	(GetRigidBody(iEntity), orientation);		return 0; }
		inline	::llc::error_t		SetDampingLinear	(uint32_t iEntity, float damping)								{ Integrator.Masses[GetRigidBody(iEntity)].LinearDamping	= damping;	return 0; }
		inline	::llc::error_t		SetDampingAngular	(uint32_t iEntity, float damping)								{ Integrator.Masses[GetRigidBody(iEntity)].AngularDamping	= damping;	return 0; }
		::llc::error_t				SetHidden			(uint32_t iEntity, bool hidden)									{ Scene->RenderNodes.Flags[GetRenderNode(iEntity)].NoDraw	= hidden;	return 0; }	

		::llc::error_t				ToggleHidden		(uint32_t iEntity) {
			::llc::SRenderNodeFlags			& flags				= Scene->RenderNodes.Flags[GetRenderNode(iEntity)];
			flags.NoDraw				= !flags.NoDraw;
			return 0;
		}

		::llc::error_t				CreateLight			(::llc::LIGHT_TYPE type);
		::llc::error_t				CreateCamera		();


		::llc::error_t				CreateBox			(const ::llc::SParamsBox			& params, ::llc::vcs entityName = ::llc::vcst_t{ 3, "Box"});
		::llc::error_t				CreateSphere		(const ::llc::SParamsSphere			& params, ::llc::vcs entityName = ::llc::vcst_t{ 6, "Sphere"});
		::llc::error_t				CreateCylinderWall	(const ::llc::SParamsCylinderWall	& params, ::llc::vcs entityName = ::llc::vcst_t{13, "Cylinder Wall"});
		::llc::error_t				CreateCylinder		(const ::llc::SParamsCylinderWall	& params, ::llc::vcs entityName = ::llc::vcst_t{ 8, "Cylinder"});		
		::llc::error_t				CreateCircleSide	(const ::llc::SParamsCircle			& params, ::llc::vcs entityName = ::llc::vcst_t{11, "Circle Side"});
		::llc::error_t				CreateDisc			(const ::llc::SParamsDisc			& params, ::llc::vcs entityName = ::llc::vcst_t{ 4, "Disc"});
		::llc::error_t				CreateTube			(const ::llc::SParamsTube			& params, ::llc::vcs entityName = ::llc::vcst_t{ 4, "Tube"});
		::llc::error_t				CreateRingSide		(const ::llc::SParamsRingSide		& params, ::llc::vcs entityName = ::llc::vcst_t{ 9, "Ring Side"});
		::llc::error_t				CreateRingFlat		(const ::llc::SParamsRingSide		& params, ::llc::vcs entityName = ::llc::vcst_t{ 9, "Ring flat"});
		::llc::error_t				CreateGrid			(const ::llc::SParamsGrid			& params, ::llc::vcs entityName = ::llc::vcst_t{ 4, "Grid"});
		::llc::error_t				CreateHelixHalf		(const ::llc::SParamsHelix			& params, ::llc::vcs entityName = ::llc::vcst_t{10, "Helix half"});
		::llc::error_t				CreateHelix			(const ::llc::SParamsHelix			& params, ::llc::vcs entityName = ::llc::vcst_t{ 5, "Helix"});
		::llc::error_t				CreateFigure0		(const ::llc::SParamsHelix			& params, ::llc::vcs entityName = ::llc::vcs{"Figure0"});
		::llc::error_t				CreateTender		(const ::llc::SParamsTender			& params, ::llc::vcs entityName = ::llc::vcs{"Tender"});

		::llc::error_t				CreateImageFromFile	(const ::llc::vcst_t & fileFolder, const ::llc::vcst_t & fileName);

		typedef std::function<::llc::error_t(::llc::SGeometryBuffers&)> TGeometryFunc;

		tplt<tpnm _tParams>
		::llc::error_t				CreateEntityFromGeometry
			( const ::llc::vcst_t							name		
			, const ::llc::n3f2_t						halfSizes
			, bool										createSkin
			, const _tParams							& params
			, ::llc::linear_map_pod<_tParams, uint32_t>	& recycleRenderNodeMap
			, const TGeometryFunc						& funcGeometry
			) {
			int32_t							iEntity				= Entities.Create();
			Entities.Names[iEntity]		= name;

			::llc::SEntity					& entity			= Entities[iEntity];
			Integrator.BoundingVolumes[entity.RigidBody = Integrator.Create()].HalfSizes = halfSizes;

			int32_t							reuse				= recycleRenderNodeMap.Keys.find([params](const _tParams & value) { return params == value; }, 0);
			if(recycleRenderNodeMap.size() > (uint32_t)reuse)
				entity.RenderNode		= Scene->Clone(recycleRenderNodeMap.Values[reuse], true, true, true);
			else {
				::llc::SGeometryBuffers			geometry;
				llc_necs(funcGeometry(geometry));
				entity.RenderNode			= Scene->CreateRenderNode(geometry, name, createSkin);
				recycleRenderNodeMap.push_back(params, entity.RenderNode);
			}
			return iEntity;
		}
	};

#pragma pack(push, 1)
	struct SContactResult {
		::llc::n3f2_t			ContactPosition					= {};
		::llc::n3f2_t			DistanceDirection				= {};
		double					ForceTransferRatioA				= 0;
		double					ForceTransferRatioB				= 0;

		::llc::n3f2_t			InitialVelocityA				= {};
		::llc::n3f2_t			InitialVelocityB				= {};
		::llc::n3f2_t			FinalVelocityA					= {};
		::llc::n3f2_t			FinalVelocityB					= {};

		::llc::n3f2_t			InitialRotationA				= {};
		::llc::n3f2_t			InitialRotationB				= {};
		::llc::n3f2_t			FinalRotationA					= {};
		::llc::n3f2_t			FinalRotationB					= {};
	};

	struct SContact {
		double					Time							= {};
		uint32_t				EntityA							= {};
		uint32_t				EntityB							= {};
		::llc::n3f2_t			CenterDistance					= {};
		double					DistanceLength					= {};
		::llc::SContactResult	Result							= {};
	};
#pragma pack(pop)

	::llc::error_t			collisionDetect		(const ::llc::SEngine & engine, double totalSeconds, ::llc::apod<::llc::SContact> & contactsDetected);


} // namespace

#endif // LLC_ENGINE_H_23627
