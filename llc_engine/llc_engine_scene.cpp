#include "llc_engine_geometry.h"
#include "llc_engine_scene.h"
#include "llc_engine_shader.h"
#include "llc_noise.h"
#include "llc_ascii_color.h"
#include "llc_raster_lh.h"
#include "llc_view_n3.h"
#include "llc_view_n2.h"
#include "llc_view_tri.h"

LLC_USING_APOD();
LLC_USING_VIEW();
LLC_USING_TYPEINT();
LLC_USING_N2();
LLC_USING_N3();

::llc::err_t llc::SEngineScene::BuildLightList(::llc::apod<::llc::SRenderLight> & lights) const {
	lights.clear();
	for(::llc::u2_t iNode = 0; iNode < RenderNodes.size(); ++iNode) {
		if(!RenderNodes.Lights[iNode])
			continue;
		const ::llc::m4f2_t model = RenderNodes.BaseTransforms[iNode].Model * RenderNodes.Transforms[iNode].Model;
		for(const ::llc::SLight & reference : *RenderNodes.Lights[iNode]) {
			const auto append = [&](const auto & source) -> ::llc::err_t {
				::llc::SRenderLight light;
				light.Type = reference.Type;
				light.Color = source.Color;
				light.Position = model.GetTranslation();
				if constexpr(requires { source.Direction; })
					light.Direction = model.TransformDirection(source.Direction).Normalize();
				if constexpr(requires { source.Range; })
					light.Range = source.Range;
				if constexpr(requires { source.SpotPower; })
					light.SpotPower = source.SpotPower;
				return lights.push_back(light);
			};
			const auto appendGroup = [&](const auto & groups) -> ::llc::err_t {
				if_true_fef(reference.Index >= groups.size(), "Light group:%u, type:%u.", reference.Index, reference.Type);
				if(groups[reference.Index])
					for(const auto & light : *groups[reference.Index])
						if_fail_fe(append(light));
				return 0;
			};
			switch(reference.Type) {
			case ::llc::LIGHT_TYPE_Directional: if_fail_fe(appendGroup(RenderNodes.LightsDirectional)); break;
			case ::llc::LIGHT_TYPE_Point      : if_fail_fe(appendGroup(RenderNodes.LightsPoint)); break;
			case ::llc::LIGHT_TYPE_Spot       : if_fail_fe(appendGroup(RenderNodes.LightsSpot)); break;
			default: return ::llc::OS_INVALID_PARAMETER;
			}
		}
	}
	return 0;
}

::llc::err_t llc::lightFilter(const ::llc::n3f2_t & position, ::llc::view<const ::llc::SRenderLight> lights, ::llc::apod<::llc::SRenderLight> & selected) {
	selected.clear();
	for(const ::llc::SRenderLight & light : lights)
		if(light.Type == ::llc::LIGHT_TYPE_Directional || (light.Range > 0 && (light.Position - position).LengthSquared() < light.Range * light.Range))
			if_fail_fe(selected.push_back(light));
	return 0;
}

tplt <tpnm _tIndex>
static	::llc::err_t	transformTriangles					
	( ::llc::SVSOutput		& output
	, ::llc::view<const _tIndex>	indices			
	, ::llc::vcn3f2_t			positions	
	, ::llc::vcn3f2_t			normals		
	, ::llc::vcn2f2_t			uv			
	, const ::llc::m4f2_t	& projection		
	, const ::llc::m4f2_t	& worldTransform	
	, const ::llc::n3f2_t	& cameraFront
)	{ 
	::llc::vctri<_tIndex>				view_indices			= {(const ::llc::tri<_tIndex>*)indices.begin(), indices.size() / 3};
	const ::llc::m4f2_t			mWVPS					= worldTransform * projection;

	for(::llc::u2_t iTriangle = 0; iTriangle < view_indices.size(); ++iTriangle) {
		const ::llc::tri<_tIndex>	vertexIndices			= view_indices[iTriangle];
		::llc::tri3f2_t				transformedNormals		= {normals[vertexIndices.A], normals[vertexIndices.B], normals[vertexIndices.C]};
		::llc::transformDirection(transformedNormals, worldTransform);
		transformedNormals.A.Normalize();
		transformedNormals.B.Normalize();
		transformedNormals.C.Normalize();
		f3_t						directionFactorA		= transformedNormals.A.Dot(cameraFront);
		f3_t						directionFactorB		= transformedNormals.B.Dot(cameraFront);
		f3_t						directionFactorC		= transformedNormals.C.Dot(cameraFront);
		if(directionFactorA > .35 && directionFactorB > .35 && directionFactorC > .35)
			continue;

		output.Normals.push_back(transformedNormals);

		::llc::tri3f2_t				transformedPositions	= {positions[vertexIndices.A], positions[vertexIndices.B], positions[vertexIndices.C]};
		::llc::transform(transformedPositions, worldTransform);
		output.PositionsWorld.push_back(transformedPositions);
		
		::llc::transform(transformedPositions, projection);
		output.PositionsScreen.push_back(transformedPositions);
		
		::llc::tri2f2_t				transformedUVs			= {uv[vertexIndices.A], uv[vertexIndices.B], uv[vertexIndices.C]};
		if( transformedUVs.A.x > 1.0f
		 || transformedUVs.A.y > 1.0f
		 || transformedUVs.B.x > 1.0f
		 || transformedUVs.B.y > 1.0f
		 || transformedUVs.C.x > 1.0f
		 || transformedUVs.C.y > 1.0f
		) 
			_CrtDbgBreak();
		output.UVs.push_back(transformedUVs);
	}
	return 0; 
}

static	::llc::err_t	transformTriangles
	( ::llc::SEngineRenderCache				& renderCache
	, const ::llc::SEngineScene				& scene
	, const ::llc::SEngineSceneConstants	& constants
	, const ::llc::SRenderNodeConstants		& transforms
	, s2_t								iRenderNode
	) {
	const ::llc::SRenderNode	& renderNode			= scene.RenderNodes.RenderNodes[iRenderNode];
	const ::llc::SGeometryMesh	& mesh					= *scene.Graphics->Meshes[renderNode.Mesh];
	verbose_printf("Drawing node %i, mesh %i, slice %i, mesh name: %s", iRenderNode, renderNode.Mesh, renderNode.Slice, scene.Graphics->Meshes.Names[renderNode.Mesh].begin());


	renderCache.VertexShaderOutput	= {};

	if(mesh.GeometryBuffers.size()) {
		const ::llc::m4f2_t			& worldTransform		= transforms.Model;

		const ::llc::vcn3f2_t			positions				= (mesh.GeometryBuffers.size() > 1) ? ::llc::vcn3f2_t	{(const ::llc::n3f2_t*)scene.Graphics->Buffers[mesh.GeometryBuffers[1]]->Data.begin(), scene.Graphics->Buffers[mesh.GeometryBuffers[1]]->Data.size() / (::llc::u0_t)sizeof(const ::llc::n3f2_t)}	: ::llc::vcn3f2_t{};
		const ::llc::vcn3f2_t			normals					= (mesh.GeometryBuffers.size() > 2) ? ::llc::vcn3f2_t	{(const ::llc::n3f2_t*)scene.Graphics->Buffers[mesh.GeometryBuffers[2]]->Data.begin(), scene.Graphics->Buffers[mesh.GeometryBuffers[2]]->Data.size() / (::llc::u0_t)sizeof(const ::llc::n3f2_t)}	: ::llc::vcn3f2_t{};
		const ::llc::vcn2f2_t			uv						= (mesh.GeometryBuffers.size() > 3) ? ::llc::vcn2f2_t	{(const ::llc::n2f2_t*)scene.Graphics->Buffers[mesh.GeometryBuffers[3]]->Data.begin(), scene.Graphics->Buffers[mesh.GeometryBuffers[3]]->Data.size() / (::llc::u0_t)sizeof(const ::llc::n2f2_t)}	: ::llc::vcn2f2_t{};

		::llc::pobj<::llc::SRenderBuffer>	indexBuffer		= scene.Graphics->Buffers[mesh.GeometryBuffers[0]];
		if(indexBuffer->Desc.Format.TotalBytes() == 1) {
			const ::llc::vcu0_t			indices					= ::llc::vcu0_t{(const ::llc::u0_t*)indexBuffer->Data.begin(), scene.Graphics->Buffers[mesh.GeometryBuffers[0]]->Data.size() / (::llc::u0_t)sizeof(const ::llc::u0_t)};
			const ::llc::SGeometrySlice	slice					= (renderNode.Slice < mesh.GeometrySlices.size()) ? mesh.GeometrySlices[renderNode.Slice] : ::llc::SGeometrySlice{{0, indices.size() / 3}};
			::transformTriangles(renderCache.VertexShaderOutput, indices, positions, normals, uv, constants.VPS, worldTransform, constants.CameraFront);
		}
		else if(indexBuffer->Desc.Format.TotalBytes() == 2) {
			const ::llc::vcu1_t			indices					= ::llc::vcu1_t{(const ::llc::u1_t*)indexBuffer->Data.begin(), scene.Graphics->Buffers[mesh.GeometryBuffers[0]]->Data.size() / (::llc::u0_t)sizeof(const ::llc::u1_t)};
			const ::llc::SGeometrySlice	slice					= (renderNode.Slice < mesh.GeometrySlices.size()) ? mesh.GeometrySlices[renderNode.Slice] : ::llc::SGeometrySlice{{0, indices.size() / 3}};
			::transformTriangles(renderCache.VertexShaderOutput, indices, positions, normals, uv, constants.VPS, worldTransform, constants.CameraFront);
		}
		else if(indexBuffer->Desc.Format.TotalBytes() == 4) {
			const ::llc::vcu2_t			indices					= ::llc::vcu2_t{(const ::llc::u2_t*)indexBuffer->Data.begin(), scene.Graphics->Buffers[mesh.GeometryBuffers[0]]->Data.size() / (::llc::u0_t)sizeof(const ::llc::u2_t)};
			const ::llc::SGeometrySlice	slice					= (renderNode.Slice < mesh.GeometrySlices.size()) ? mesh.GeometrySlices[renderNode.Slice] : ::llc::SGeometrySlice{{0, indices.size() / 3}};
			::transformTriangles(renderCache.VertexShaderOutput, indices, positions, normals, uv, constants.VPS, worldTransform, constants.CameraFront);
		}
	}
	return 0;
}

static	::llc::err_t	getNodeTransform
	( const ::llc::SRenderNodeManager		& nodeManager
	, ::llc::u2_t							iNode
	, const ::llc::SEngineSceneConstants	& constants
	, ::llc::SRenderNodeConstants			& nodeConstants
	) {
	nodeConstants						= nodeManager.Transforms[iNode];
	nodeConstants.Model					= nodeManager.BaseTransforms[iNode].Model * nodeConstants.Model;
	nodeConstants.ModelInverse			= nodeConstants.Model.GetInverse();
	nodeConstants.ModelInverseTranspose	= nodeConstants.ModelInverse.GetTranspose();
	nodeConstants.MVP						= nodeConstants.Model * constants.View * constants.Perspective;
	nodeConstants.NodeSize					= nodeManager.BaseTransforms[iNode].NodeSize;
	return 0;
}


static	::llc::err_t	drawBuffers
	( ::llc::g8bgra										& backBufferColors
	, ::llc::gu2_t										backBufferDepth
	, ::llc::SVSOutput									& outVS
	, ::llc::SVSCache									& cacheVS
	, const ::llc::SRenderMaterial						& material
	, ::llc::view<const ::llc::gc8bgra>				surfaces
	, bool												alphaBlend
	, const ::llc::SEngineSceneConstants				& constants
	, const ::llc::SRenderNodeConstants					& nodeConstants
	, ::llc::view<const ::llc::SRenderLight>			lights
	, const ::std::function<::llc::TFuncPixelShader>	& ps
	) {	// 
	::llc::atrif2_t				& triangleWeights			= cacheVS.TriangleWeights		;
	::llc::an2s1_t				& trianglePixelCoords		= cacheVS.SolidPixelCoords		;
	const ::llc::n2u1_t			offscreenMetrics			= backBufferColors.metrics().u1();
	const ::llc::n3f2_t			lightDirectionNormalized	= ::llc::n3f2_t{constants.LightDirection}.Normalize();
	::llc::SPSIn				inPS						= {nodeConstants};
	inPS.Surfaces			= surfaces;
	inPS.Material			= material;
	inPS.Lights				= lights;

	for(::llc::u2_t iTriangle = 0; iTriangle < outVS.PositionsScreen.size(); ++iTriangle) {
		const ::llc::tri3f2_t		& triPositions				= outVS.PositionsScreen	[iTriangle];
		if( triPositions.CulledZ({0, 0xFFFFFF})
		 || triPositions.CulledX({0, (f2_t)offscreenMetrics.x})
		 || triPositions.CulledY({0, (f2_t)offscreenMetrics.y})
		)
			continue;

		const ::llc::tri3f2_t		& triPositionsWorld			= outVS.PositionsWorld	[iTriangle];
		const ::llc::tri3f2_t		& triNormals				= outVS.Normals			[iTriangle];
		const ::llc::tri2f2_t		& triUVs					= outVS.UVs				[iTriangle];

		trianglePixelCoords.clear();
		triangleWeights.clear();
		if_fail_e(::llc::drawTriangle(offscreenMetrics.u2(), triPositions, trianglePixelCoords, triangleWeights, backBufferDepth));
		//const bool													stripped					= surface[0][0] == ::llc::bgra{llc::WHITE};
		for(::llc::u2_t iCoord = 0; iCoord < trianglePixelCoords.size(); ++iCoord) {
			const ::llc::trif2_t							& vertexWeights				= triangleWeights[iCoord];
			inPS.WeightedPosition								= triPositionsWorld.A * vertexWeights.A + triPositionsWorld.B * vertexWeights.B + triPositionsWorld.C * vertexWeights.C;
			inPS.WeightedNormal									= (triNormals.A * vertexWeights.A + triNormals.B * vertexWeights.B + triNormals.C * vertexWeights.C).Normalize();
			inPS.WeightedUV										= triUVs.A * vertexWeights.A + triUVs.B * vertexWeights.B + triUVs.C * vertexWeights.C;
			const ::llc::n2u1_t										coord						= trianglePixelCoords[iCoord].u1();
			::llc::bgra											outputPixel					= {};
			ps(constants, inPS, outputPixel);
			if(alphaBlend)
				::llc::pixelBlend(backBufferColors, coord.s1(), outputPixel);
			else
				backBufferColors[coord.y][coord.x]	= outputPixel;
		}
	}
	return 0;
}

::llc::err_t			llc::SEngineScene::BuildRenderList
	( const ::llc::n3f2_t		& cameraPosition
	, const ::llc::n3f2_t		& cameraFront
	, ::llc::SRenderNodeList	& renderList
	) const {
	renderList.Solid.clear();
	renderList.Alpha.clear();
	for(::llc::u2_t iNode = 0; iNode < RenderNodes.size(); ++iNode) {
		if(RenderNodes.Flags[iNode].NoDraw)
			continue;
		const ::llc::SRenderNode	& node					= RenderNodes[iNode];
		if(node.Mesh >= Graphics->Meshes.size() || node.Skin >= Graphics->Skins.size() || !Graphics->Skins[node.Skin])
			continue;
		const ::llc::m4f2_t		model					= RenderNodes.BaseTransforms[iNode].Model * RenderNodes.Transforms[iNode].Model;
		if((model.GetTranslation() - cameraPosition).Normalize().Dot(cameraFront) <= 0)
			continue;
		const ::llc::SSkin		& skin					= *Graphics->Skins[node.Skin];
		if_fail_fe((skin.Material.Color.Diffuse.a < 1.0f ? renderList.Alpha : renderList.Solid).push_back(iNode));
	}
	const auto nodeDepth = [this, &cameraFront](::llc::u2_t iNode) {
		const ::llc::m4f2_t model = RenderNodes.BaseTransforms[iNode].Model * RenderNodes.Transforms[iNode].Model;
		return cameraFront.Dot(model.GetTranslation());
	};
	//::std::sort(renderList.Solid.begin(), renderList.Solid.end(), [&nodeDepth](::llc::u2_t a, ::llc::u2_t b) { return nodeDepth(a) < nodeDepth(b); });
	//::std::sort(renderList.Alpha.begin(), renderList.Alpha.end(), [&nodeDepth](::llc::u2_t a, ::llc::u2_t b) { return nodeDepth(a) > nodeDepth(b); });
	return 0;
}

::llc::err_t			llc::drawScene									
	( ::llc::g8bgra				backBufferColors
	, ::llc::gu2_t				backBufferDepth
	, ::llc::SEngineRenderCache	& renderCache
	, const ::llc::SEngineScene	& scene
	, const ::llc::n3f2_t		& cameraPosition
	, const ::llc::n3f2_t		& cameraTarget
	, const ::llc::n3f2_t		& cameraUp
	, const ::llc::minmaxf2_t	& nearFar
	, const ::llc::n3f2_t		& lightPos
	, ::llc::f3_t				time
) {	//
	const ::llc::n3f2_t			cameraFront			= (cameraTarget - cameraPosition).Normalize();
	const ::llc::n2u1_t			offscreenMetrics	= backBufferColors.metrics16();

	::llc::SEngineSceneConstants	constants		= {};
	constants.CameraPosition	= cameraPosition;
	constants.CameraFront		= cameraFront;
	constants.LightPosition	= lightPos;
	constants.Time			= (::llc::f2_t)time;

	constants.View.LookAt(cameraPosition, cameraTarget, cameraUp);
	constants.Perspective.FieldOfView(.25 * ::llc::math_pi, offscreenMetrics.x / (f3_t)offscreenMetrics.y, nearFar.Min, nearFar.Max);
	constants.Screen.ViewportLH(offscreenMetrics);
	constants.VP			= constants.View * constants.Perspective;
	constants.VPS			= constants.VP * constants.Screen;

	static ::std::function<::llc::TFuncPixelShader>	defaultShader			= {::llc::psSolid};
	::llc::SRenderNodeList			renderList;
	if_fail_fe(scene.BuildRenderList(constants.CameraPosition, constants.CameraFront, renderList));
	if_fail_fe(scene.BuildLightList(renderCache.LightsWorld));
	const auto drawNode = [&](::llc::u2_t iRenderNode, bool alphaBlend) -> ::llc::err_t {
		const ::llc::SRenderNode		& renderNode		= scene.RenderNodes.RenderNodes[iRenderNode];
		const ::llc::SSkin				& skin				= *scene.Graphics->Skins.Elements[renderNode.Skin];
		::llc::SRenderNodeConstants		transforms			= {};
		if_fail_fe(::getNodeTransform(scene.RenderNodes, iRenderNode, constants, transforms));
		if_fail_fe(::llc::lightFilter(transforms.Model.GetTranslation(), renderCache.LightsWorld, renderCache.LightsNode));

		if_fail_fef(::transformTriangles(renderCache, scene, constants, transforms, iRenderNode), "iRenderNode %i", iRenderNode);

		const ::llc::SRenderMaterial	& material			= skin.Material;
		::llc::apod<::llc::gc8bgra>	surfaces;
		for(::llc::u2_t iTexture = 0; iTexture < skin.Textures.size(); ++iTexture) {
			const ::llc::u2_t				iSurface			= skin.Textures[iTexture];
			if(iSurface >= scene.Graphics->Surfaces.size())
				continue;
			const ::llc::SSurface			& surface			= *scene.Graphics->Surfaces[iSurface];
			if_fail_fe(surfaces.push_back({(const ::llc::bgra*)surface.Data.begin(), surface.Desc.Dimensions.u2()}));
		}
		const ::std::function<::llc::TFuncPixelShader>	& fx
			= (renderNode.Shader >= scene.Graphics->Shaders.size()) ? defaultShader : *scene.Graphics->Shaders[renderNode.Shader];

		return drawBuffers(backBufferColors, backBufferDepth, renderCache.VertexShaderOutput, renderCache.VertexShaderCache, material, surfaces, alphaBlend, constants, transforms, renderCache.LightsNode, fx);
	};
	for(::llc::u2_t iNode : renderList.Solid)
		if_fail_fe(drawNode(iNode, false));
	for(::llc::u2_t iNode : renderList.Alpha)
		if_fail_fe(drawNode(iNode, true));

	::llc::apod<::llc::n3f2_t>					& wireframePixelCoords	= renderCache.VertexShaderCache.WireframePixelCoords;

	// ---- Draw axis vectors at the origin (0, 0, 0)
	constexpr ::llc::n3f2_t						xyz	[3]					= 
		{ {1}
		, {0, 1}
		, {0, 0, 1}
		};
	constexpr ::llc::bgra						colorXYZ[3]				= 
		{ ::llc::RED
		, ::llc::GREEN
		, ::llc::BLUE
		};

	for(::llc::u2_t iVector = 0; iVector < 3; ++iVector) {
		wireframePixelCoords.clear();
		::llc::drawLine(offscreenMetrics, ::llc::line3f2_t{{}, xyz[iVector]}, constants.VPS, wireframePixelCoords, backBufferDepth);
		for(::llc::u2_t iCoord = 0; iCoord < wireframePixelCoords.size(); ++iCoord) {
			::llc::n3u1_t									coord					= wireframePixelCoords[iCoord].u1();
			backBufferColors[coord.y][coord.x]	= colorXYZ[iVector];
		}
	}
	return 0;
}

::llc::err_t			llc::createBuffers	
	( const ::llc::SGeometryBuffers		& geometry
	, ::llc::pobj<::llc::SRenderBuffer>	& pIndicesVertex
	, ::llc::pobj<::llc::SRenderBuffer>	& pVertices
	, ::llc::pobj<::llc::SRenderBuffer>	& pNormals
	, ::llc::pobj<::llc::SRenderBuffer>	& pUV
	) {
	if_fail_fe(llc::createBuffers(geometry.Positions.size(), pIndicesVertex, pVertices, pNormals, pUV));

	pVertices	->Data.resize(geometry.Positions	.byte_count());
	pNormals	->Data.resize(geometry.Normals		.byte_count());
	pUV			->Data.resize(geometry.TextureCoords.byte_count());
	memcpy(&pVertices	->Data[0], geometry.Positions		.begin(), pVertices	->Data.size());
	memcpy(&pNormals	->Data[0], geometry.Normals			.begin(), pNormals	->Data.size());
	memcpy(&pUV			->Data[0], geometry.TextureCoords	.begin(), pUV		->Data.size());
	if(geometry.Positions.size() > 0xFFFF) {
		pIndicesVertex->Data.resize(geometry.PositionIndices.byte_count());
		memcpy(pIndicesVertex->Data.begin(), geometry.PositionIndices.begin(), pIndicesVertex->Data.size());
	}
	else if(geometry.Positions.size() > 0xFF) {
		pIndicesVertex->Data.resize(geometry.PositionIndices.byte_count() >> 1);
		::llc::vu1_t					viewIndices					= ::llc::vu1_t{(::llc::u1_t*)pIndicesVertex->Data.begin(), geometry.PositionIndices.size()};
		viewIndices.enumerate([&geometry](::llc::u2_t index, ::llc::u1_t & value){ value = (::llc::u1_t)geometry.PositionIndices[index]; return 0; }, 0);
	}
	else {
		pIndicesVertex->Data.resize(geometry.PositionIndices.byte_count() >> 2);
		::llc::vu0_t					viewIndices					= ::llc::vu0_t{(u0_t*)pIndicesVertex->Data.begin(), geometry.PositionIndices.size()};
		viewIndices.enumerate([&geometry](::llc::u2_t index, u0_t & value){ value = (u0_t)geometry.PositionIndices[index]; return 0; }, 0);
	}

	return 0;
}

::llc::err_t			llc::createBuffers
	( ::llc::SRenderBufferManager	& bufferManager 
	, const ::llc::SGeometryBuffers	& geometry
	, ::llc::u2_t						& iIndicesVertex
	, ::llc::u2_t						& iVertices
	, ::llc::u2_t						& iNormals
	, ::llc::u2_t						& iUV
	) {
	::llc::pobj<::llc::SRenderBuffer>	pIndicesVertex, pVertices, pNormals, pUV;
	if_fail_fe(llc::createBuffers(geometry, pIndicesVertex, pVertices, pNormals, pUV));
	if_fail_fe(iIndicesVertex			= (::llc::u2_t)bufferManager.push_back(pIndicesVertex));
	if_fail_fe(iVertices				= (::llc::u2_t)bufferManager.push_back(pVertices));
	if_fail_fe(iNormals				= (::llc::u2_t)bufferManager.push_back(pNormals));
	if_fail_fe(iUV					= (::llc::u2_t)bufferManager.push_back(pUV));
	return 0;
}

::llc::err_t			llc::SEngineScene::CreateRenderNode
	( const ::llc::SGeometryBuffers		& geometry
	, const ::llc::vcst_t				name 
	, bool								createSkin
	) {
	::llc::u2_t					iIndicesVertex			= 0;
	::llc::u2_t					iVertices				= 0;
	::llc::u2_t					iNormals				= 0;
	::llc::u2_t					iUV						= 0;
	::llc::createBuffers(Graphics->Buffers, geometry, iIndicesVertex, iVertices, iNormals, iUV);

	::llc::u2_t					iMesh					= (::llc::u2_t)Graphics->Meshes.Create();
	::llc::pobj<::llc::SGeometryMesh>	& mesh			= Graphics->Meshes[iMesh];
	Graphics->Meshes.Names[iMesh] = name;
	mesh->GeometryBuffers.append({iIndicesVertex, iVertices, iNormals, iUV});

	mesh->Desc.Mode			= ::llc::MESH_MODE_List;
	mesh->Desc.Type			= ::llc::GEOMETRY_TYPE_Triangle;
	mesh->Desc.NormalMode	= ::llc::NORMAL_MODE_Point;


	::llc::u2_t					iRenderNode				= RenderNodes.Create();
	::llc::SRenderNode			& renderNode			= RenderNodes.RenderNodes[iRenderNode];
	renderNode.Mesh			= iMesh;
	if(createSkin) {
		::llc::u2_t					iSkin					= (::llc::u2_t)Graphics->Skins.Create();
		::llc::pobj<::llc::SSkin>	& skin					= Graphics->Skins.Elements[iSkin];
		skin->Material.Color.Ambient	= ::llc::bgra(::llc::ASCII_PALETTE[3]) * .1f;
		skin->Material.Color.Diffuse	= ::llc::bgra(::llc::ASCII_PALETTE[3]);
		skin->Material.Color.Specular	= ::llc::WHITE;
		skin->Material.SpecularPower	= 0.5f;

		::llc::u2_t					iSurface				= (::llc::u2_t)Graphics->Surfaces.Create();
		skin->Textures.push_back(iSurface);

		skin->Material.Color.Ambient	*= .1f;

		::llc::pobj<::llc::SSurface>	& surface			= Graphics->Surfaces[iSurface];
		surface->Desc.ColorType	= ::llc::COLOR_TYPE_BGRA;
		surface->Desc.MethodCompression	= 0;
		surface->Desc.MethodFilter		= 0;
		surface->Desc.MethodInterlace	= 0;
		surface->Desc.Dimensions		= {32, 32};
		surface->Data.resize(surface->Desc.Dimensions.Area() * sizeof(::llc::bgra));
		memset(surface->Data.begin(), 0xFF, surface->Data.size());
		::llc::g8bgra				view					= {(::llc::bgra*)surface->Data.begin(), surface->Desc.Dimensions.u2()};
		::llc::rgba					color					= {::llc::ASCII_PALETTE[rand() % 16]};
		for(::llc::u2_t y = surface->Desc.Dimensions.y / 3; y < surface->Desc.Dimensions.y / 3U * 2U; ++y)
		for(::llc::u2_t x = 0; x < surface->Desc.Dimensions.x; ++x)
			view[y][x]				= color;


		mesh->GeometrySlices.resize(1);	// 

		::llc::SGeometrySlice		& slice					= mesh->GeometrySlices[0];
		slice.Slice				= {0, geometry.PositionIndices.size()};

		renderNode.Skin			= iSkin;
		renderNode.Slice		= 0;
		Graphics->Shaders[renderNode.Shader = Graphics->Shaders.push_back({})].create(::llc::psSolid);
		Graphics->Shaders.Names[renderNode.Shader] = "psSolid";
	}
	return iRenderNode;
}
