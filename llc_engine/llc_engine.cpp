#include "llc_engine.h"
#include "llc_voxel.h"
#include "llc_ascii_color.h"
#include "llc_geometry_lh.h"
#include "llc_engine_shader.h"

::llc::error_t			llc::updateEntityTransforms			
	( uint32_t								iEntity
	, const ::llc::SEntity			& entity
	, const ::llc::SVirtualEntityManager	& managedEntities	
	, ::llc::SRigidBodyIntegrator			& integrator		
	, ::llc::SRenderNodeManager				& renderNodes
	)
{
	const ::llc::pau32			& children				= managedEntities.Children[iEntity];
	if(-1 != entity.RenderNode) {
		::llc::SRenderNodeConstants	& transforms			= renderNodes.Transforms[entity.RenderNode];
		::llc::m4f32				& worldTransform		= transforms.Model;
		if(-1 == entity.RigidBody)
			worldTransform			= ::llc::m4f32::GetIdentity();
		else {
			integrator.GetTransform(entity.RigidBody, worldTransform);
		}

		if(-1 != entity.Parent) {
			const ::llc::SEntity	& entityParent			= managedEntities.Entities[entity.Parent];
			if(-1 != entityParent.RenderNode)
				worldTransform			= worldTransform * renderNodes.Transforms[entityParent.RenderNode].Model;
			else if(-1 != entityParent.RigidBody) {
				worldTransform			= worldTransform * integrator.TransformsLocal[entityParent.RigidBody];
			}
		}

		transforms.ModelInverse				= worldTransform.GetInverse();
		transforms.ModelInverseTranspose	= transforms.ModelInverse.GetTranspose();
	}
	else if(-1 != entity.RigidBody) {
		::llc::m4f32							& worldTransform				= integrator.TransformsLocal[entity.RigidBody];
		integrator.GetTransform(entity.RigidBody, worldTransform);
		if(-1 != entity.Parent) {
			const ::llc::SEntity				& entityParent					= managedEntities.Entities[entity.Parent];
			if(-1 != entityParent.RenderNode)
				worldTransform						= worldTransform * renderNodes.Transforms[entityParent.RenderNode].Model;
			else if(-1 != entityParent.RigidBody) {
				worldTransform						= worldTransform * integrator.TransformsLocal[entityParent.RigidBody];
			}
		}
	}
	if(children) {
		for(uint32_t iChild = 0; iChild < children->size(); ++iChild) {
			const uint32_t								iChildEntity					= (*children)[iChild];
			::llc::updateEntityTransforms(iChildEntity, managedEntities.Entities[iChildEntity], managedEntities, integrator, renderNodes);
		}
	}

	return 0;
}

::llc::error_t			llc::SEngine::CreateLight(::llc::LIGHT_TYPE type) {
	int32_t							iEntity				= this->Entities.Create();
	::llc::SEntity			& entity			= Entities[iEntity];
	entity.RenderNode			= Scene->RenderNodes.Create();

	uint16_t						indexLight			= (uint16_t)-1;
	switch(type) {
	case ::llc::LIGHT_TYPE_Directional	: indexLight = (uint16_t)Scene->RenderNodes.LightsDirectional	.push_back({}); break;
	case ::llc::LIGHT_TYPE_Point		: indexLight = (uint16_t)Scene->RenderNodes.LightsPoint			.push_back({}); break;
	case ::llc::LIGHT_TYPE_Spot			: indexLight = (uint16_t)Scene->RenderNodes.LightsSpot			.push_back({}); break;
	default:
		error_printf("Invalid light type: %i", (int)type);
		return -1;
	}
	Scene->RenderNodes.Lights		[entity.RenderNode]->push_back({type, indexLight});
	Scene->RenderNodes.Transforms	[entity.RenderNode] = {};
	return iEntity;
}

::llc::error_t			llc::SEngine::CreateCamera() {
	int32_t						iEntity						= this->Entities.Create();
	::llc::SEntity				& entity					= Entities[iEntity];
	entity.RenderNode		= Scene->RenderNodes.Create();
	entity.RigidBody		= this->Integrator.Create();

	::llc::SCamera				camera;
	camera.Front			= {1, 0, 0};
	camera.Up				= {0, 1, 0};
	camera.Right			= {0, 0, 1};
	camera.Angle			= .25 * ::llc::math_pi;

	Scene->RenderNodes.Cameras		[entity.RenderNode]->push_back(camera);
	Scene->RenderNodes.Transforms	[entity.RenderNode]		= {};

	int32_t						iSphere;
	llc_necs(iSphere = CreateSphere({}, "Camera reference"));
	llc_necs(Entities.SetParent(iSphere, iEntity));
	return iEntity;
}

::llc::error_t			llc::SEngine::CreateBox			(const ::llc::SParamsBox & params, ::llc::vcs entityName)	{
	return CreateEntityFromGeometry(entityName, params.HalfSizes, true, params, ParamsBox
		, [params](::llc::SGeometryBuffers & geometry) { 
			return ::llc::geometryBuildBox(geometry, params); 
		});
}

::llc::error_t			llc::SEngine::CreateCylinder		(const ::llc::SParamsCylinderWall & params, ::llc::vcs entityName)	{
	const float					radius							= ::llc::max(params.Radius.Min, params.Radius.Max);
	return CreateEntityFromGeometry(entityName, ::llc::n3f2_t{radius, params.Height * .5f, radius}, true, params, ParamsCylinder
		, [params](::llc::SGeometryBuffers & geometry) { 
			llc_necs(llc::geometryBuildCylinder(geometry, params));
			return 0;
		});
}

::llc::error_t			llc::SEngine::CreateSphere		(const ::llc::SParamsSphere & params, ::llc::vcs entityName)	{ 
	return CreateEntityFromGeometry(entityName, ::llc::n3f2_t{params.Radius, params.Radius, params.Radius}, true, params, ParamsSphere
		, [params](::llc::SGeometryBuffers & geometry) { 
			return ::llc::geometryBuildSphere(geometry, params); 
		});
}

::llc::error_t			llc::SEngine::CreateGrid		(const ::llc::SParamsGrid & params, ::llc::vcs entityName)	{ 
	return CreateEntityFromGeometry(entityName, ::llc::n3f2_t{params.Size.x * .5f, 0, params.Size.y * .5f}, true, params, ParamsGrid
		, [params](::llc::SGeometryBuffers & geometry) {
			llc_necs(llc::geometryBuildGrid(geometry, params));
			return 0;
		});
}

::llc::error_t			llc::SEngine::CreateHelixHalf	(const ::llc::SParamsHelix & params, ::llc::vcs entityName)	{ 
	const float					radius							= ::llc::max(params.Radius.Min, params.Radius.Max);
	return CreateEntityFromGeometry(entityName, ::llc::n3f2_t{radius, params.Length * .5f, radius}, true, params, ParamsHelix
		, [params](::llc::SGeometryBuffers & geometry) {
			llc_necs(llc::geometryBuildHelixHalf(geometry, params));
			return 0;
		});
}

::llc::error_t			llc::SEngine::CreateHelix		(const ::llc::SParamsHelix & params, ::llc::vcs entityName)	{ 
	const float					radius							= ::llc::max(params.Radius.Min, params.Radius.Max);
	return CreateEntityFromGeometry(entityName, ::llc::n3f2_t{radius, params.Length * .5f, radius}, true, params, ParamsHelix
		, [params](::llc::SGeometryBuffers & geometry) {
			llc_necs(llc::geometryBuildHelix(geometry, params));
			return 0;
		});
}

::llc::error_t			llc::SEngine::CreateFigure0		(const ::llc::SParamsHelix & params, ::llc::vcs entityName)	{ 
	const float					radius							= ::llc::max(params.Radius.Min, params.Radius.Max);
	return CreateEntityFromGeometry(entityName, ::llc::n3f2_t{radius, params.Length * .5f, radius}, true, params, ParamsHelix
		, [params](::llc::SGeometryBuffers & geometry) {
			llc_necs(llc::geometryBuildHelix(geometry, params));
			return 0;
		});
}

::llc::error_t			llc::SEngine::CreateCylinderWall(const ::llc::SParamsCylinderWall & params, ::llc::vcs entityName)	{ 
	const float					radius							= ::llc::max(params.Radius.Min, params.Radius.Max);
	return CreateEntityFromGeometry(entityName, ::llc::n3f2_t{radius, params.Height * .5f, radius}, true, params, ParamsCylinderWall
		, [params](::llc::SGeometryBuffers & geometry) { 
			llc_necs(llc::geometryBuildCylinderWall(geometry, params));
			return 0;
		});
}

::llc::error_t			llc::SEngine::CreateCircleSide		(const ::llc::SParamsCircle & params, ::llc::vcs entityName)	{ 
	const float					radius							= params.Radius;
	return CreateEntityFromGeometry(entityName, ::llc::n3f2_t{radius, 0, radius}, true, params, ParamsCircleSide
		, [params](::llc::SGeometryBuffers & geometry) { 
			llc_necs(llc::geometryBuildCircleSide(geometry, params));
			return 0;
		});
}

::llc::error_t			llc::SEngine::CreateDisc		(const ::llc::SParamsDisc & params, ::llc::vcs entityName)	{ 
	const float					radius							= params.Radius;
	return CreateEntityFromGeometry(entityName, ::llc::n3f2_t{radius, 0, radius}, true, params, ParamsDisc
		, [params](::llc::SGeometryBuffers & geometry) { 
			llc_necs(llc::geometryBuildDisc(geometry, params));
			return 0;
		});
}

::llc::error_t			llc::SEngine::CreateRingSide	(const ::llc::SParamsRingSide & params, ::llc::vcs entityName)	{ 
	const float					radius							= params.Radius.Max;
	return CreateEntityFromGeometry(entityName, ::llc::n3f2_t{radius, radius, radius}, true, params, ParamsRingSide
		, [params](::llc::SGeometryBuffers & geometry) { 
			llc_necs(llc::geometryBuildRingSide(geometry, params));
			return 0;
		});
}
::llc::error_t			llc::SEngine::CreateRingFlat	(const ::llc::SParamsRingSide & params, ::llc::vcs entityName)	{ 
	const float					radius							= params.Radius.Max;
	return CreateEntityFromGeometry(entityName, ::llc::n3f2_t{radius, radius, radius}, true, params, ParamsRingFlat
		, [params](::llc::SGeometryBuffers & geometry) { 
			llc_necs(llc::geometryBuildRingFlat(geometry, params));
			return 0;
		});
}

::llc::error_t			llc::SEngine::CreateTube		(const ::llc::SParamsTube & params, ::llc::vcs entityName)	{ 
	const float					radius							= ::llc::max(params.RadiusYMax.Max, params.RadiusYMin.Max);
	return CreateEntityFromGeometry(entityName, ::llc::n3f2_t{radius, radius, radius}, true, params, ParamsTube
		, [params](::llc::SGeometryBuffers & geometry) { 
			llc_necs(llc::geometryBuildRing(geometry, params));
			return 0;
		});
}

::llc::error_t			llc::SEngine::CreateImageFromFile(const ::llc::vcs & fileFolder, const ::llc::vcs & fileName) {
	sc_t						filePath	[2048]				= {};
	sprintf_s(filePath, "%s/%s", ::llc::toString(fileFolder).begin(), ::llc::toString(fileName).begin());
	int32_t						index							= Images.Keys.find(filePath);
	if(index != -1)
		return Images.Values[index];

	index					= Scene->Graphics->Surfaces.Create(::llc::label(filePath));
	::llc::pobj<::llc::SSurface>	& surface					= Scene->Graphics->Surfaces[index];
	llc_necs(llc::pngFileLoad(PNGCache, filePath, surface->Data));
	surface->Desc.Dimensions	= PNGCache.Header.Size.u1_t();
	surface->Desc.BitDepth		= PNGCache.Header.BitDepth;
	surface->Desc.ColorType		= PNGCache.Header.ColorType;
	return index; 
}

//
//	int32_t						iEntity					= CreateBox();
//	::llc::SEntity		& entity				= Entities[iEntity];
//	::llc::SRenderNode			& renderNode			= Scene->RenderNodes[entity.RenderNode];
//
//	uint32_t					iMesh					= renderNode.Mesh;
//	::llc::pobj<::llc::SGeometryMesh>	& mesh			= Scene->Graphics->Meshes[iMesh];
//	Scene->Graphics->Skins[renderNode.Skin]->Material.Color.Diffuse.a = .25f;
//	mesh->GeometrySlices.resize(6);	// one per face
//
//	uint32_t					offsetIndex				= 0;
//	for(uint32_t iFace = 0; iFace < 6; ++iFace) {
//		uint32_t					iSkin					= (uint32_t)Scene->Graphics->Skins.Create();
//		::llc::pobj<::llc::SSkin>	& skin					= Scene->Graphics->Skins.Elements[iSkin];
//
//		uint32_t					iSurface				= (uint32_t)Scene->Graphics->Surfaces.Create();
//		::llc::pobj<::llc::SSurface>& surface				= Scene->Graphics->Surfaces[iSurface];
//		surface->Desc.ColorType				= ::llc::COLOR_TYPE_BGRA;
//		surface->Desc.MethodCompression		= 0;
//		surface->Desc.MethodFilter			= 0;
//		surface->Desc.MethodInterlace		= 0;
//		surface->Desc.Dimensions			= {1, 1};
//		surface->Data.resize(1 * sizeof(::llc::bgra));
//		*(::llc::bgra*)&surface->Data[0]	= ::llc::rgba{::llc::VOXEL_PALETTE[iFace]};
//		skin->Textures.push_back(iSurface);
//
//		skin->Material.Color.Ambient		= ::llc::bgra(::llc::ASCII_PALETTE[1 + iFace]);
//		skin->Material.Color.Diffuse		= ::llc::bgra(::llc::ASCII_PALETTE[1 + iFace]);
//		skin->Material.Color.Specular		= ::llc::WHITE;
//		skin->Material.SpecularPower		= 0.5f;
//
//		skin->Material.Color.Ambient		*= .1f;
//
//		::llc::SGeometrySlice		& slice					= mesh->GeometrySlices[iFace];
//		slice.Slice				= {offsetIndex, 6};
//		offsetIndex				+= slice.Slice.Count;
//
//		int32_t						iFaceEntity				= Entities.Create();
//		uint32_t					iFaceRenderNode			= Scene->RenderNodes.Create();
//		::llc::SEntity		& faceEntity			= Entities[iFaceEntity];
//		::llc::SRenderNode			& faceRenderNode		= Scene->RenderNodes.RenderNodes[iFaceRenderNode];
//
//		faceRenderNode.Mesh		= iMesh;
//		faceRenderNode.Slice	= iFace;
//		faceRenderNode.Skin		= iSkin;
//		Scene->Graphics->Shaders[faceRenderNode.Shader = Scene->Graphics->Shaders.push_back({})].create(::llc::psSolid);
//		Scene->Graphics->Shaders.Names[faceRenderNode.Shader] = "psSolid";
//
//		//faceEntity.RigidBody				= Integrator.Create();
//		faceEntity.Parent					= iEntity;
//		faceEntity.RenderNode				= iFaceRenderNode;
//		this->Entities.Names	[iFaceEntity]	= ::llc::get_value_label((VOXEL_FACE)iFace);
//
//		this->Entities.Children[iEntity]->push_back(iFaceEntity);
//	}
//
//	return iEntity;
//}
