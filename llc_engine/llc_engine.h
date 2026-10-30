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
	::llc::err_t					updateEntityTransforms			
		( ::llc::u2_t								iEntity
		, const ::llc::SEntity			& entity
		, const ::llc::SVirtualEntityManager	& managedEntities	
		, ::llc::SRigidBodyIntegrator			& integrator		
		, ::llc::SRenderNodeManager				& renderNodes
		);

	struct SEngine {
		::llc::pobj<::llc::SEngineScene>	Scene			;
		::llc::SVirtualEntityManager		Entities		;
		::llc::SRigidBodyIntegrator			Integrator		;

		::llc::lmpod<SParamsBox			, ::llc::u2_t>	ParamsBox			;
		::llc::lmpod<SParamsDisc		, ::llc::u2_t>	ParamsDisc			;
		::llc::lmpod<SParamsCircle		, ::llc::u2_t>	ParamsCircleSide	;
		::llc::lmpod<SParamsCylinderWall, ::llc::u2_t>	ParamsCylinderWall	;
		::llc::lmpod<SParamsCylinderWall, ::llc::u2_t>	ParamsCylinder		;
		::llc::lmpod<SParamsHelix		, ::llc::u2_t>	ParamsFigure0		;
		::llc::lmpod<SParamsGrid		, ::llc::u2_t>	ParamsGrid			;
		::llc::lmpod<SParamsHelix		, ::llc::u2_t>	ParamsHelixHalf		;
		::llc::lmpod<SParamsHelix		, ::llc::u2_t>	ParamsHelix			;
		::llc::lmpod<SParamsSphere		, ::llc::u2_t>	ParamsSphere		;
		::llc::lmpod<SParamsTube		, ::llc::u2_t>	ParamsTube			;
		::llc::lmpod<SParamsRingSide	, ::llc::u2_t>	ParamsRingSide		;
		::llc::lmpod<SParamsRingSide	, ::llc::u2_t>	ParamsRingFlat		;

		::llc::lmpod<::llc::vcst_t, ::llc::u2_t>	Images			= {};

		::llc::SPNGData						PNGCache		= {};

		inline	::llc::err_t		GetRigidBody		(::llc::eid_t idEntity)					const	{ return Entities[idEntity].RigidBody; }
		inline	::llc::err_t		GetRenderNode		(::llc::eid_t idEntity)					const	{ return Entities[idEntity].RenderNode; }
		inline	::llc::err_t		GetParentIndex		(::llc::eid_t idEntity)					const	{ return Entities[idEntity].Parent; }

		inline	::llc::err_t		GetChildCount		(::llc::eid_t idEntity)					const	{ const ::llc::papod<::llc::eid_t> & children = Entities.Children[idEntity]; return children ? children->size() : 0; }
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

		inline	::llc::err_t		SetParent			(::llc::eid_t idEntity, ::llc::eid_t idParent)	{ return Entities.SetParent(idEntity, idParent); }

		::llc::err_t				Load				(::llc::vcu0_t & input)			{
			if_fail_fe(Scene		->Load(input));
			if_fail_fe(Entities	.Load(input));
			if_fail_fe(Integrator	.Load(input));
			return 0;
		}

		::llc::err_t				Save				(::llc::au0_t & output)	const	{
			if(Scene) {
				if_fail_fe(Scene->Save(output));
			}
			else 
				if_fail_fe(llc::SEngineScene{}.Save(output));

			if_fail_fe(Entities	.Save(output));
			if_fail_fe(Integrator	.Save(output));
			return 0;
		}

		::llc::err_t				Update				(f3_t secondsLastFrame)		{
			Integrator.Integrate(secondsLastFrame);
			for(::llc::u2_t iEntity = 0; iEntity < Entities.size(); ++iEntity) {
				::llc::SEntity			& entity			= Entities[iEntity];
				if(entity.Parent != -1)
					continue;

				if_fail_e(::llc::updateEntityTransforms(iEntity, entity, Entities, Integrator, Scene->RenderNodes));
			}
			return 0;
		}

		// This function messes up with the Entities array so be careful of not having any references to its contents or it may end up trying to access invalid memory
		::llc::err_t				Clone				(::llc::u2_t iEntitySource, bool cloneSkin, bool cloneSurfaces, bool cloneShaders) {
			const ::llc::SEntity		entitySource		= Entities[iEntitySource];
			s2_t							iEntityNew			= Entities.Create();
			::llc::SEntity			& entityNew			= Entities[iEntityNew];
			entityNew.RenderNode		= Scene->Clone(entitySource.RenderNode, cloneSkin, cloneSurfaces, cloneShaders);
			entityNew.RigidBody			= ((::llc::u2_t)entitySource.RigidBody < Integrator.Flags.size()) ? Integrator.Clone(entitySource.RigidBody) : (::llc::u2_t)-1;
			entityNew.Parent			= entitySource.Parent;

			const ::llc::pau2_t				childrenSource		= Entities.Children[iEntitySource];
			if(childrenSource && childrenSource->size()) {
				::llc::pau2_t					childrenNew			= Entities.Children[iEntityNew];
				for(::llc::u2_t iChild = 0; iChild < childrenSource->size(); ++iChild) {
					::llc::u2_t						entityChild			= Clone((*childrenSource)[iChild], cloneSkin, cloneSurfaces, cloneShaders);
					Entities[entityChild].Parent= iEntityNew;
					childrenNew->push_back(entityChild);
				}
				Entities.Children[iEntityNew]	= childrenNew;
			}
			return iEntityNew;
		}

		inline	::llc::err_t		SetNodeSize			(::llc::u2_t iEntity, const ::llc::n3f2_t & size)					{ Scene->RenderNodes.BaseTransforms[GetRenderNode(iEntity)].NodeSize = size; return 0; }
		inline	::llc::err_t		SetMeshScale		(::llc::u2_t iEntity, const ::llc::n3f2_t & scale, bool halfSizes)	{ Scene->RenderNodes.BaseTransforms[GetRenderNode(iEntity)].Model.Scale(scale, true); 
			if(halfSizes) {
				if_fail_fe(SetHalfSizes(iEntity, scale * .5f));
				if_fail_fe(SetNodeSize(iEntity, scale));
			}
			return 0; 
		}
		inline	::llc::err_t		SetMeshPosition		(::llc::u2_t iEntity, const ::llc::n3f2_t & position)				{ Scene->RenderNodes.BaseTransforms[GetRenderNode(iEntity)].Model.SetTranslation(position, false); return 0; }
		::llc::err_t				SetColorDiffuse		(::llc::u2_t iEntity, const ::llc::rgbaf & diffuse)				{ Scene->Graphics->Skins[Scene->RenderNodes[GetRenderNode(iEntity)].Skin]->Material.Color.Diffuse = diffuse; return 0; }
		::llc::err_t				SetShader			(::llc::u2_t iEntity, const ::std::function<::llc::TFuncPixelShader> & shader, ::llc::vcst_t name) {
			const ::llc::u2_t					iShader				= Scene->RenderNodes[GetRenderNode(iEntity)].Shader;
			Scene->Graphics->Shaders[iShader].create(shader);
			Scene->Graphics->Shaders.Names[iShader] = name;
			return 0;
		}
		inline	::llc::err_t		IsPhysicsActive		(::llc::u2_t iEntity)										const	{ return Integrator.Active	(Entities[iEntity].RigidBody) ? 1 : 0; }
		inline	f2_t				GetMass				(::llc::u2_t iEntity)										const	{ return Integrator.GetMass	(Entities[iEntity].RigidBody);	}

		inline	::llc::err_t		InvalidateTransform	(::llc::u2_t iEntity)												{ Integrator.Flags[GetRigidBody(iEntity)].UpdatedTransform = false;		return 0; }

		inline	::llc::err_t		GetPosition			(::llc::u2_t iEntity, ::llc::n3f2_t & position)				const	{ Integrator.GetPosition	(GetRigidBody(iEntity), position);			return 0; }
		inline	::llc::err_t		GetVelocity			(::llc::u2_t iEntity, ::llc::n3f2_t & velocity)				const	{ Integrator.GetVelocity	(GetRigidBody(iEntity), velocity);			return 0; }
		inline	::llc::err_t		GetAcceleration		(::llc::u2_t iEntity, ::llc::n3f2_t & acceleration)			const	{ Integrator.GetAcceleration(GetRigidBody(iEntity), acceleration);		return 0; }
		inline	::llc::err_t		GetRotation			(::llc::u2_t iEntity, ::llc::n3f2_t & velocity)				const	{ Integrator.GetRotation	(GetRigidBody(iEntity), velocity);			return 0; }
		inline	::llc::err_t		GetOrientation		(::llc::u2_t iEntity, ::llc::quatf2_t & orientation)		const	{ Integrator.GetOrientation	(GetRigidBody(iEntity), orientation);		return 0; }
		inline	::llc::err_t		SetPhysicsActive	(::llc::u2_t iEntity, bool active)									{ Integrator.SetActive		(GetRigidBody(iEntity), active);			return 0; }
		inline	::llc::err_t		SetCollides			(::llc::u2_t iEntity, bool collides)								{ Integrator.SetCollides	(GetRigidBody(iEntity), collides);			return 0; }
		inline	::llc::err_t		SetHalfSizes		(::llc::u2_t iEntity, const ::llc::n3f2_t & halfSizes)				{ Integrator.SetHalfSizes	(GetRigidBody(iEntity), halfSizes);			return 0; }
		inline	::llc::err_t		SetMass				(::llc::u2_t iEntity, f2_t mass)									{ Integrator.SetMass		(GetRigidBody(iEntity), mass);				return 0; }
		inline	::llc::err_t		SetMassInverse		(::llc::u2_t iEntity, f2_t inverseMass)							{ Integrator.SetMassInverse	(GetRigidBody(iEntity), inverseMass);		return 0; }
		inline	::llc::err_t		SetPosition			(::llc::u2_t iEntity, const ::llc::n3f2_t & position)				{ Integrator.SetPosition	(GetRigidBody(iEntity), position);			return 0; }
		inline	::llc::err_t		SetVelocity			(::llc::u2_t iEntity, const ::llc::n3f2_t & velocity)				{ Integrator.SetVelocity	(GetRigidBody(iEntity), velocity);			return 0; }
		inline	::llc::err_t		SetAcceleration		(::llc::u2_t iEntity, const ::llc::n3f2_t & acceleration)			{ Integrator.SetAcceleration(GetRigidBody(iEntity), acceleration);		return 0; }
		inline	::llc::err_t		SetRotation			(::llc::u2_t iEntity, const ::llc::n3f2_t & velocity)				{ Integrator.SetRotation	(GetRigidBody(iEntity), velocity);			return 0; }
		inline	::llc::err_t		SetOrientation		(::llc::u2_t iEntity, const ::llc::quatf2_t & orientation)			{ Integrator.SetOrientation	(GetRigidBody(iEntity), orientation);		return 0; }
		inline	::llc::err_t		SetDampingLinear	(::llc::u2_t iEntity, f2_t damping)								{ Integrator.Masses[GetRigidBody(iEntity)].LinearDamping	= damping;	return 0; }
		inline	::llc::err_t		SetDampingAngular	(::llc::u2_t iEntity, f2_t damping)								{ Integrator.Masses[GetRigidBody(iEntity)].AngularDamping	= damping;	return 0; }
		::llc::err_t				SetHidden			(::llc::u2_t iEntity, bool hidden)									{ Scene->RenderNodes.Flags[GetRenderNode(iEntity)].NoDraw	= hidden;	return 0; }	

		::llc::err_t				ToggleHidden		(::llc::u2_t iEntity) {
			::llc::SRenderNodeFlags			& flags				= Scene->RenderNodes.Flags[GetRenderNode(iEntity)];
			flags.NoDraw				= !flags.NoDraw;
			return 0;
		}

		::llc::err_t				CreateLight			(::llc::LIGHT_TYPE type);
		::llc::err_t				CreateCamera		();


		::llc::err_t				CreateBox			(const ::llc::SParamsBox			& params, ::llc::vcst_t entityName = "Box");
		::llc::err_t				CreateSphere		(const ::llc::SParamsSphere			& params, ::llc::vcst_t entityName = "Sphere");
		::llc::err_t				CreateCylinderWall	(const ::llc::SParamsCylinderWall	& params, ::llc::vcst_t entityName = "Cylinder Wall");
		::llc::err_t				CreateCylinder		(const ::llc::SParamsCylinderWall	& params, ::llc::vcst_t entityName = "Cylinder");
		::llc::err_t				CreateCircleSide	(const ::llc::SParamsCircle			& params, ::llc::vcst_t entityName = "Circle Side");
		::llc::err_t				CreateDisc			(const ::llc::SParamsDisc			& params, ::llc::vcst_t entityName = "Disc");
		::llc::err_t				CreateTube			(const ::llc::SParamsTube			& params, ::llc::vcst_t entityName = "Tube");
		::llc::err_t				CreateRingSide		(const ::llc::SParamsRingSide		& params, ::llc::vcst_t entityName = "Ring Side");
		::llc::err_t				CreateRingFlat		(const ::llc::SParamsRingSide		& params, ::llc::vcst_t entityName = "Ring flat");
		::llc::err_t				CreateGrid			(const ::llc::SParamsGrid			& params, ::llc::vcst_t entityName = "Grid");
		::llc::err_t				CreateHelixHalf		(const ::llc::SParamsHelix			& params, ::llc::vcst_t entityName = "Helix half");
		::llc::err_t				CreateHelix			(const ::llc::SParamsHelix			& params, ::llc::vcst_t entityName = "Helix");
		::llc::err_t				CreateFigure0		(const ::llc::SParamsHelix			& params, ::llc::vcst_t entityName = "Figure0");

		::llc::err_t				CreateImageFromFile	(const ::llc::vcst_t & fileFolder, const ::llc::vcst_t & fileName);

		typedef std::function<::llc::err_t(::llc::SGeometryBuffers&)> TGeometryFunc;

		tplt<tpnm _tParams>
		::llc::err_t				CreateEntityFromGeometry
			( const ::llc::vcst_t							name		
			, const ::llc::n3f2_t						halfSizes
			, bool										createSkin
			, const _tParams							& params
			, ::llc::linear_map_pod<_tParams, ::llc::u2_t>	& recycleRenderNodeMap
			, const TGeometryFunc						& funcGeometry
			) {
			s2_t							iEntity				= Entities.Create();
			Entities.Names[iEntity]		= name;

			::llc::SEntity					& entity			= Entities[iEntity];
			Integrator.BoundingVolumes[entity.RigidBody = Integrator.Create()].HalfSizes = halfSizes;

			s2_t							reuse				= recycleRenderNodeMap.Keys.find([params](const _tParams & value) { return params == value; }, 0);
			if(recycleRenderNodeMap.size() > (::llc::u2_t)reuse)
				entity.RenderNode		= Scene->Clone(recycleRenderNodeMap.Values[reuse], true, true, true);
			else {
				::llc::SGeometryBuffers			geometry;
				if_fail_fe(funcGeometry(geometry));
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
		f3_t					ForceTransferRatioA				= 0;
		f3_t					ForceTransferRatioB				= 0;

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
		f3_t					Time							= {};
		::llc::u2_t				EntityA							= {};
		::llc::u2_t				EntityB							= {};
		::llc::n3f2_t			CenterDistance					= {};
		f3_t					DistanceLength					= {};
		::llc::SContactResult	Result							= {};
	};
#pragma pack(pop)

	::llc::err_t			collisionDetect		(const ::llc::SEngine & engine, f3_t totalSeconds, ::llc::apod<::llc::SContact> & contactsDetected);


} // namespace

#endif // LLC_ENGINE_H_23627
