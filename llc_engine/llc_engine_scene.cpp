#include "llc_engine_geometry.h"
#include "llc_engine_scene.h"
#include "llc_engine_shader.h"
#include "llc_noise.h"
#include "llc_ascii_color.h"
#include "llc_raster_lh.h"
#include "llc_view_n3.h"
#include "llc_view_n2.h"
#include "llc_view_tri.h"

tplt <tpnm _tIndex>
static	::llc::error_t	transformTriangles					
	( ::llc::SVSOutput		& output
	, ::llc::view<const _tIndex>	indices			
	, ::llc::vc3f32			positions	
	, ::llc::vc3f32			normals		
	, ::llc::vc2f32			uv			
	, const ::llc::m4f32	& projection		
	, const ::llc::m4f32	& worldTransform	
	, const ::llc::n3f2_t	& cameraFront
)	{ 
	::llc::vctri<_tIndex>				view_indices			= {(const ::llc::tri<_tIndex>*)indices.begin(), indices.size() / 3};
	const ::llc::m4f32			mWVPS					= worldTransform * projection;

	for(uint32_t iTriangle = 0; iTriangle < view_indices.size(); ++iTriangle) {
		const ::llc::tri<_tIndex>	vertexIndices			= view_indices[iTriangle];
		::llc::tri3f32				transformedNormals		= {normals[vertexIndices.A], normals[vertexIndices.B], normals[vertexIndices.C]};
		::llc::transformDirection(transformedNormals, worldTransform);
		transformedNormals.A.Normalize();
		transformedNormals.B.Normalize();
		transformedNormals.C.Normalize();
		double						directionFactorA		= transformedNormals.A.Dot(cameraFront);
		double						directionFactorB		= transformedNormals.B.Dot(cameraFront);
		double						directionFactorC		= transformedNormals.C.Dot(cameraFront);
		if(directionFactorA > .35 && directionFactorB > .35 && directionFactorC > .35)
			continue;

		output.Normals.push_back(transformedNormals);

		::llc::tri3f32				transformedPositions	= {positions[vertexIndices.A], positions[vertexIndices.B], positions[vertexIndices.C]};
		::llc::transform(transformedPositions, worldTransform);
		output.PositionsWorld.push_back(transformedPositions);
		
		::llc::transform(transformedPositions, projection);
		output.PositionsScreen.push_back(transformedPositions);
		
		::llc::tri2f32				transformedUVs			= {uv[vertexIndices.A], uv[vertexIndices.B], uv[vertexIndices.C]};
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

static	::llc::error_t	transformTriangles
	( ::llc::SEngineRenderCache				& renderCache
	, const ::llc::SEngineScene				& scene
	, const ::llc::SEngineSceneConstants	& constants
	, int32_t								iRenderNode
	) {
	const ::llc::SRenderNode	& renderNode			= scene.RenderNodes.RenderNodes[iRenderNode];
	const ::llc::SGeometryMesh	& mesh					= *scene.Graphics->Meshes[renderNode.Mesh];
	verbose_printf("Drawing node %i, mesh %i, slice %i, mesh name: %s", iRenderNode, renderNode.Mesh, renderNode.Slice, scene.Graphics->Meshes.Names[renderNode.Mesh].begin());


	renderCache.VertexShaderOutput	= {};

	if(mesh.GeometryBuffers.size()) {
		::llc::SRenderNodeConstants	transforms				= scene.RenderNodes.Transforms[iRenderNode];
		const ::llc::m4f32			& worldTransform		= transforms.Model;

		const ::llc::vc3f32			positions				= (mesh.GeometryBuffers.size() > 1) ? ::llc::vc3f32	{(const ::llc::n3f2_t*)scene.Graphics->Buffers[mesh.GeometryBuffers[1]]->Data.begin(), scene.Graphics->Buffers[mesh.GeometryBuffers[1]]->Data.size() / (uint8_t)sizeof(const ::llc::n3f2_t)}	: ::llc::vc3f32{};
		const ::llc::vc3f32			normals					= (mesh.GeometryBuffers.size() > 2) ? ::llc::vc3f32	{(const ::llc::n3f2_t*)scene.Graphics->Buffers[mesh.GeometryBuffers[2]]->Data.begin(), scene.Graphics->Buffers[mesh.GeometryBuffers[2]]->Data.size() / (uint8_t)sizeof(const ::llc::n3f2_t)}	: ::llc::vc3f32{};
		const ::llc::vc2f32			uv						= (mesh.GeometryBuffers.size() > 3) ? ::llc::vc2f32	{(const ::llc::n2f2_t*)scene.Graphics->Buffers[mesh.GeometryBuffers[3]]->Data.begin(), scene.Graphics->Buffers[mesh.GeometryBuffers[3]]->Data.size() / (uint8_t)sizeof(const ::llc::n2f2_t)}	: ::llc::vc2f32{};

		transforms.Model					= scene.RenderNodes.BaseTransforms[iRenderNode].Model * transforms.Model;
		::llc::pobj<::llc::SRenderBuffer>	indexBuffer		= scene.Graphics->Buffers[mesh.GeometryBuffers[0]];
		if(indexBuffer->Desc.Format.TotalBytes() == 1) {
			const ::llc::vcu0_t			indices					= ::llc::vcu0_t{(const uint8_t*)indexBuffer->Data.begin(), scene.Graphics->Buffers[mesh.GeometryBuffers[0]]->Data.size() / (uint8_t)sizeof(const uint8_t)};
			const ::llc::SGeometrySlice	slice					= (renderNode.Slice < mesh.GeometrySlices.size()) ? mesh.GeometrySlices[renderNode.Slice] : ::llc::SGeometrySlice{{0, indices.size() / 3}};
			::transformTriangles(renderCache.VertexShaderOutput, indices, positions, normals, uv, constants.VPS, worldTransform, constants.CameraFront);
		}
		else if(indexBuffer->Desc.Format.TotalBytes() == 2) {
			const ::llc::vcu1_t			indices					= ::llc::vcu1_t{(const uint16_t*)indexBuffer->Data.begin(), scene.Graphics->Buffers[mesh.GeometryBuffers[0]]->Data.size() / (uint8_t)sizeof(const uint16_t)};
			const ::llc::SGeometrySlice	slice					= (renderNode.Slice < mesh.GeometrySlices.size()) ? mesh.GeometrySlices[renderNode.Slice] : ::llc::SGeometrySlice{{0, indices.size() / 3}};
			::transformTriangles(renderCache.VertexShaderOutput, indices, positions, normals, uv, constants.VPS, worldTransform, constants.CameraFront);
		}
		else if(indexBuffer->Desc.Format.TotalBytes() == 4) {
			const ::llc::vcu2_t			indices					= ::llc::vcu2_t{(const uint32_t*)indexBuffer->Data.begin(), scene.Graphics->Buffers[mesh.GeometryBuffers[0]]->Data.size() / (uint8_t)sizeof(const uint16_t)};
			const ::llc::SGeometrySlice	slice					= (renderNode.Slice < mesh.GeometrySlices.size()) ? mesh.GeometrySlices[renderNode.Slice] : ::llc::SGeometrySlice{{0, indices.size() / 3}};
			::transformTriangles(renderCache.VertexShaderOutput, indices, positions, normals, uv, constants.VPS, worldTransform, constants.CameraFront);
		}
	}
	return 0;
}


static	::llc::error_t	drawBuffers
	( ::llc::g8bgra										& backBufferColors
	, ::llc::gu32										backBufferDepth
	, ::llc::SVSOutput									& outVS
	, ::llc::SVSCache									& cacheVS
	, const ::llc::SRenderMaterial						& material
	, ::llc::gc8bgra									surface
	, const ::llc::SEngineSceneConstants				& constants
	, const ::llc::SRenderNodeConstants					& nodeConstants
	, const ::std::function<::llc::TFuncPixelShader>	& ps
	) {	// 
	::llc::atrif32				& triangleWeights			= cacheVS.TriangleWeights		;
	::llc::an2i16				& trianglePixelCoords		= cacheVS.SolidPixelCoords		;
	const ::llc::n2u1_t			offscreenMetrics			= backBufferColors.metrics().u1_t();
	const ::llc::n3f2_t			lightDirectionNormalized	= ::llc::n3f2_t{constants.LightDirection}.Normalize();
	::llc::SPSIn				inPS						= {nodeConstants};
	inPS.Surface			= surface;
	inPS.Material			= material;

	for(uint32_t iTriangle = 0; iTriangle < outVS.PositionsScreen.size(); ++iTriangle) {
		const ::llc::tri3f32		& triPositions				= outVS.PositionsScreen	[iTriangle];
		if( triPositions.CulledZ({0, 0xFFFFFF})
		 || triPositions.CulledX({0, (float)offscreenMetrics.x})
		 || triPositions.CulledY({0, (float)offscreenMetrics.y})
		)
			continue;

		const ::llc::tri3f32		& triPositionsWorld			= outVS.PositionsWorld	[iTriangle];
		const ::llc::tri3f32		& triNormals				= outVS.Normals			[iTriangle];
		const ::llc::tri2f32		& triUVs					= outVS.UVs				[iTriangle];

		trianglePixelCoords.clear();
		triangleWeights.clear();
		if_true_e_failed(::llc::drawTriangle(offscreenMetrics.u2_t(), triPositions, trianglePixelCoords, triangleWeights, backBufferDepth));
		//const bool													stripped					= surface[0][0] == ::llc::bgra{llc::WHITE};
		for(uint32_t iCoord = 0; iCoord < trianglePixelCoords.size(); ++iCoord) {
			const ::llc::trif32							& vertexWeights				= triangleWeights[iCoord];
			inPS.WeightedPosition								= triPositionsWorld.A * vertexWeights.A + triPositionsWorld.B * vertexWeights.B + triPositionsWorld.C * vertexWeights.C;
			inPS.WeightedNormal									= (triNormals.A * vertexWeights.A + triNormals.B * vertexWeights.B + triNormals.C * vertexWeights.C).Normalize();
			inPS.WeightedUV										= triUVs.A * vertexWeights.A + triUVs.B * vertexWeights.B + triUVs.C * vertexWeights.C;
			const ::llc::n2u1_t										coord						= trianglePixelCoords[iCoord].u1_t();
			ps(constants, inPS, backBufferColors[coord.y][coord.x]);
		}
	}
	return 0;
}

::llc::error_t			llc::drawScene									
	( ::llc::g8bgra				backBufferColors
	, ::llc::gu32				backBufferDepth
	, ::llc::SEngineRenderCache	& renderCache
	, const ::llc::SEngineScene	& scene
	, const ::llc::n3f2_t		& cameraPosition
	, const ::llc::n3f2_t		& cameraTarget
	, const ::llc::n3f2_t		& cameraUp
	, const ::llc::minmaxf2_t	& nearFar
) {	//
	const ::llc::n3f2_t			cameraFront			= (cameraTarget - cameraPosition).Normalize();
	const ::llc::n2u1_t			offscreenMetrics	= backBufferColors.metrics16();

	::llc::SEngineSceneConstants	constants		= {};
	constants.CameraPosition	= cameraPosition;
	constants.CameraFront		= cameraFront;

	constants.View.LookAt(cameraPosition, cameraTarget, cameraUp);
	constants.Perspective.FieldOfView(.25 * ::llc::math_pi, offscreenMetrics.x / (double)offscreenMetrics.y, nearFar.Min, nearFar.Max);
	constants.Screen.ViewportLH(offscreenMetrics);
	constants.VP			= constants.View * constants.Perspective;
	constants.VPS			= constants.VP * constants.Screen;

	static ::std::function<::llc::TFuncPixelShader>	defaultShader			= {::llc::psSolid};
	for(uint32_t iRenderNode = 0, countNodes = scene.RenderNodes.RenderNodes.size(); iRenderNode < countNodes; ++iRenderNode) {
		const ::llc::SRenderNodeFlags	& renderNodeFlags		= scene.RenderNodes.Flags[iRenderNode];
		if(renderNodeFlags.NoDraw)
			continue;

		const ::llc::SRenderNode		& renderNode		= scene.RenderNodes.RenderNodes[iRenderNode];
		if(renderNode.Mesh >= scene.Graphics->Meshes.size())
			continue;

		info_printf("Mesh name: %s", scene.Graphics->Meshes.Names[renderNode.Mesh].begin());
		
		::llc::SRenderNodeConstants		transforms			= scene.RenderNodes.Transforms[iRenderNode];
		transforms.NodeSize	= scene.RenderNodes.BaseTransforms[iRenderNode].NodeSize;
		const ::llc::m4f32				& worldTransform	= transforms.Model;
		if((worldTransform.GetTranslation() - constants.CameraPosition).Normalize().Dot(constants.CameraFront) <= 0)
			return 0;

		cif_true_ef(failed(::transformTriangles(renderCache, scene, constants, iRenderNode)), "iRenderNode %i", iRenderNode);

		const ::llc::SSkin				& skin				= *scene.Graphics->Skins.Elements[renderNode.Skin];
		const ::llc::SRenderMaterial	& material			= skin.Material;
		const uint32_t					tex					= skin.Textures[0];
		const ::llc::SSurface			& surface			= *scene.Graphics->Surfaces[tex];
		const ::std::function<::llc::TFuncPixelShader>	& fx
			= (renderNode.Shader >= scene.Graphics->Shaders.size()) ? defaultShader : *scene.Graphics->Shaders[renderNode.Shader];

		drawBuffers(backBufferColors, backBufferDepth, renderCache.VertexShaderOutput, renderCache.VertexShaderCache, material, {(const ::llc::bgra*)surface.Data.begin(), surface.Desc.Dimensions.u2_t()}, constants, transforms, fx);
	}

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

	for(uint32_t iVector = 0; iVector < 3; ++iVector) {
		wireframePixelCoords.clear();
		::llc::drawLine(offscreenMetrics, ::llc::line3f32{{}, xyz[iVector]}, constants.VPS, wireframePixelCoords, backBufferDepth);
		for(uint32_t iCoord = 0; iCoord < wireframePixelCoords.size(); ++iCoord) {
			::llc::n3u1_t									coord					= wireframePixelCoords[iCoord].u1_t();
			backBufferColors[coord.y][coord.x]	= colorXYZ[iVector];
		}
	}
	return 0;
}

::llc::error_t			llc::createBuffers	
	( const ::llc::SGeometryBuffers		& geometry
	, ::llc::pobj<::llc::SRenderBuffer>	& pIndicesVertex
	, ::llc::pobj<::llc::SRenderBuffer>	& pVertices
	, ::llc::pobj<::llc::SRenderBuffer>	& pNormals
	, ::llc::pobj<::llc::SRenderBuffer>	& pUV
	) {
	llc_necs(llc::createBuffers(geometry.Positions.size(), pIndicesVertex, pVertices, pNormals, pUV));

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
		::llc::vu16					viewIndices					= ::llc::vu16{(uint16_t*)pIndicesVertex->Data.begin(), geometry.PositionIndices.size()};
		viewIndices.enumerate([&geometry](uint32_t index, uint16_t & value){ value = (uint16_t)geometry.PositionIndices[index]; return 0; }, 0);
	}
	else {
		pIndicesVertex->Data.resize(geometry.PositionIndices.byte_count() >> 2);
		::llc::vu8					viewIndices					= ::llc::vu8{(uint8_t*)pIndicesVertex->Data.begin(), geometry.PositionIndices.size()};
		viewIndices.enumerate([&geometry](uint32_t index, uint8_t & value){ value = (uint8_t)geometry.PositionIndices[index]; return 0; }, 0);
	}

	return 0;
}

::llc::error_t			llc::createBuffers
	( ::llc::SRenderBufferManager	& bufferManager 
	, const ::llc::SGeometryBuffers	& geometry
	, uint32_t						& iIndicesVertex
	, uint32_t						& iVertices
	, uint32_t						& iNormals
	, uint32_t						& iUV
	) {
	::llc::pobj<::llc::SRenderBuffer>	pIndicesVertex, pVertices, pNormals, pUV;
	llc_necs(llc::createBuffers(geometry, pIndicesVertex, pVertices, pNormals, pUV));
	llc_necs(iIndicesVertex			= (uint32_t)bufferManager.push_back(pIndicesVertex));
	llc_necs(iVertices				= (uint32_t)bufferManager.push_back(pVertices));
	llc_necs(iNormals				= (uint32_t)bufferManager.push_back(pNormals));
	llc_necs(iUV					= (uint32_t)bufferManager.push_back(pUV));
	return 0;
}

::llc::error_t			llc::SEngineScene::CreateRenderNode
	( const ::llc::SGeometryBuffers		& geometry
	, const ::llc::vcsc_t				name 
	, bool								createSkin
	) {
	uint32_t					iIndicesVertex			= 0;
	uint32_t					iVertices				= 0;
	uint32_t					iNormals				= 0;
	uint32_t					iUV						= 0;
	::llc::createBuffers(Graphics->Buffers, geometry, iIndicesVertex, iVertices, iNormals, iUV);

	uint32_t					iMesh					= (uint32_t)Graphics->Meshes.Create();
	::llc::pobj<::llc::SGeometryMesh>	& mesh			= Graphics->Meshes[iMesh];
	Graphics->Meshes.Names[iMesh] = name;
	mesh->GeometryBuffers.append({iIndicesVertex, iVertices, iNormals, iUV});

	mesh->Desc.Mode			= ::llc::MESH_MODE_List;
	mesh->Desc.Type			= ::llc::GEOMETRY_TYPE_Triangle;
	mesh->Desc.NormalMode	= ::llc::NORMAL_MODE_Point;


	uint32_t					iRenderNode				= RenderNodes.Create();
	::llc::SRenderNode			& renderNode			= RenderNodes.RenderNodes[iRenderNode];
	renderNode.Mesh			= iMesh;
	if(createSkin) {
		uint32_t					iSkin					= (uint32_t)Graphics->Skins.Create();
		::llc::pobj<::llc::SSkin>	& skin					= Graphics->Skins.Elements[iSkin];
		skin->Material.Color.Ambient	= ::llc::bgra(::llc::ASCII_PALETTE[3]) * .1f;
		skin->Material.Color.Diffuse	= ::llc::bgra(::llc::ASCII_PALETTE[3]);
		skin->Material.Color.Specular	= ::llc::WHITE;
		skin->Material.SpecularPower	= 0.5f;

		uint32_t					iSurface				= (uint32_t)Graphics->Surfaces.Create();
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
		::llc::g8bgra				view					= {(::llc::bgra*)surface->Data.begin(), surface->Desc.Dimensions.u2_t()};
		::llc::rgba					color					= {::llc::ASCII_PALETTE[rand() % 16]};
		for(uint32_t y = surface->Desc.Dimensions.y / 3; y < surface->Desc.Dimensions.y / 3U * 2U; ++y)
		for(uint32_t x = 0; x < surface->Desc.Dimensions.x; ++x)
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
