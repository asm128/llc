#include "llc_engine_d3d_app.h"
#include "llc_view_color.h"

::llc::err_t llc::Sample3DSceneRenderer::CreateVertexShader(::llc::vcst_t name) {
	::llc::string filename;
	llc_necs(filename.append(name));
	llc_necs(filename.append(::llc::vcst_t{".cso"}));
	::llc::au0_t code;
	llc_necs(::llc::fileToMemory(filename, code));
	::llc::pcom<ID3D11VertexShader> shader;
	llc_hrcall(DeviceResources->GetD3DDevice()->CreateVertexShader(code.begin(), code.size(), nullptr, &shader));
	return VertexShader.push_back(shader);
}

::llc::err_t llc::SD3DRenderTarget::Upload(::DX::D3DDeviceResources & resources, ::llc::gc8bgra colors) {
	if_null_fe(Color.get());
	D3D11_TEXTURE2D_DESC desc = {};
	Color->GetDesc(&desc);
	if_true_fe(desc.Width != colors.metrics().x || desc.Height != colors.metrics().y);
	resources.GetD3DDeviceContext()->UpdateSubresource(Color, 0, nullptr, colors.begin(), (UINT)(colors.metrics().x * sizeof(::llc::bgra)), 0);
	return 0;
}

::llc::err_t llc::SD3DRenderTarget::Begin(::DX::D3DDeviceResources & resources, ::llc::gc8bgra colors) {
	if_true_fe(0 == colors.size());
	llc_necs(Begin(resources, colors.metrics(), {}));
	return Upload(resources, colors);
}

::llc::err_t llc::SD3DRenderTarget::Begin(::DX::D3DDeviceResources & resources, ::llc::n2u2_t size, ::llc::rgbaf clearColor) {
	if_true_fe(0 == size.x || 0 == size.y);
	auto device = resources.GetD3DDevice();
	auto context = resources.GetD3DDeviceContext();
	D3D11_TEXTURE2D_DESC desc = {};
	::llc::pcom<ID3D11Device> owner;
	if(Color) { Color->GetDesc(&desc); Color->GetDevice(&owner); }
	if(desc.Width != size.x || desc.Height != size.y || owner.get() != device) {
		::llc::SD3DRenderTarget next;
		desc = {};
		desc.Width = size.x; desc.Height = size.y;
		desc.ArraySize = desc.MipLevels = desc.SampleDesc.Count = 1;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		llc_hrcall(device->CreateTexture2D(&desc, nullptr, &next.Color));
		llc_hrcall(device->CreateRenderTargetView(next.Color, nullptr, &next.Target));
		llc_hrcall(device->CreateShaderResourceView(next.Color, nullptr, &next.ColorView));
		desc.Format = DXGI_FORMAT_R32_TYPELESS;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		llc_hrcall(device->CreateTexture2D(&desc, nullptr, &next.Depth));
		D3D11_DEPTH_STENCIL_VIEW_DESC depthView = {};
		depthView.Format = DXGI_FORMAT_D32_FLOAT;
		depthView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		llc_hrcall(device->CreateDepthStencilView(next.Depth, &depthView, &next.DepthView));
		desc.BindFlags = 0;
		desc.Usage = D3D11_USAGE_STAGING;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		llc_hrcall(device->CreateTexture2D(&desc, nullptr, &next.ReadDepth));
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		llc_hrcall(device->CreateTexture2D(&desc, nullptr, &next.ReadColor));
		*this = next;
	}
	context->OMSetRenderTargets(1, &Target, DepthView);
	const D3D11_VIEWPORT viewport = {0, 0, (float)size.x, (float)size.y, 0, 1};
	context->RSSetViewports(1, &viewport);
	const float rgba[] = {clearColor.r, clearColor.g, clearColor.b, clearColor.a};
	context->ClearRenderTargetView(Target, rgba);
	context->ClearDepthStencilView(DepthView, D3D11_CLEAR_DEPTH, 1, 0);
	return 0;
}

::llc::err_t llc::SD3DRenderTarget::Read(::DX::D3DDeviceResources & resources, ::llc::g8bgra colors, ::llc::gu2_t depth) {
	if_null_fe(Color.get());
	D3D11_TEXTURE2D_DESC desc = {};
	Color->GetDesc(&desc);
	if_true_fe(colors.metrics() != depth.metrics() || colors.metrics().x != desc.Width || colors.metrics().y != desc.Height);
	auto context = resources.GetD3DDeviceContext();
	context->CopyResource(ReadColor, Color);
	context->CopyResource(ReadDepth, Depth);
	D3D11_MAPPED_SUBRESOURCE mapped = {};
	llc_hrcall(context->Map(ReadColor, 0, D3D11_MAP_READ, 0, &mapped));
	for(::llc::u2_t y = 0; y < desc.Height; ++y)
		memcpy(colors[y].begin(), (const ::llc::u0_t*)mapped.pData + y * mapped.RowPitch, colors[y].byte_count());
	context->Unmap(ReadColor, 0);
	llc_hrcall(context->Map(ReadDepth, 0, D3D11_MAP_READ, 0, &mapped));
	for(::llc::u2_t y = 0; y < desc.Height; ++y) {
		const float * row = (const float*)((const ::llc::u0_t*)mapped.pData + y * mapped.RowPitch);
		for(::llc::u2_t x = 0; x < desc.Width; ++x)
			depth[y][x] = row[x] >= 1 ? 0xFFFFFFFFU : (::llc::u2_t)(0xFFFFFFFU * (double)row[x]);
	}
	context->Unmap(ReadDepth, 0);
	return 0;
}

::llc::err_t llc::Sample3DSceneRenderer::SetLights(::llc::view<const ::llc::SRenderLight> lights) {
	static_assert(sizeof(::llc::SRenderLight) == 96);
	auto device = DeviceResources->GetD3DDevice();
	auto context = DeviceResources->GetD3DDeviceContext();
	if_true_fe(lights.size() > 0xFFFFFFFFU / sizeof(::llc::SRenderLight));
	D3D11_BUFFER_DESC desc = {};
	if(LightBuffer)
		LightBuffer->GetDesc(&desc);
	const UINT required = (UINT)::llc::max(lights.size(), 1U) * (UINT)sizeof(::llc::SRenderLight);
	if(desc.ByteWidth < required) {
		desc = {};
		desc.ByteWidth = required;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.StructureByteStride = sizeof(::llc::SRenderLight);
		::llc::pcom<ID3D11Buffer> buffer;
		::llc::pcom<ID3D11ShaderResourceView> view;
		llc_hrcall(device->CreateBuffer(&desc, nullptr, &buffer));
		D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
		srv.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srv.Buffer.NumElements = desc.ByteWidth / desc.StructureByteStride;
		llc_hrcall(device->CreateShaderResourceView(buffer, &srv, &view));
		LightBuffer = buffer;
		LightView = view;
	}
	if(lights.size()) {
		D3D11_MAPPED_SUBRESOURCE mapped = {};
		llc_hrcall(context->Map(LightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
		memcpy(mapped.pData, lights.begin(), lights.byte_count());
		context->Unmap(LightBuffer, 0);
	}
	if(!LightConstants) {
		desc = {};
		desc.ByteWidth = 16;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		llc_hrcall(device->CreateBuffer(&desc, nullptr, &LightConstants));
	}
	const ::llc::u2_t counts[4] = {lights.size()};
	context->UpdateSubresource(LightConstants, 0, nullptr, counts, 0, 0);
	return 0;
}

::llc::error_t			llc::d3dGUIDraw		(::DX::D3DDeviceResources & d3dResources, ::llc::SD3DGUIResources<::llc::bgra> & guiStuff)	{
	if(!guiStuff.Texture2D) 
		return 1;
	
	D3D11_MAPPED_SUBRESOURCE	mappedTexture				= {};
	llc_hrcall(d3dResources.GetD3DDeviceContext()->Map(guiStuff.Texture2D.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedTexture));
	if(mappedTexture.pData) {
		for(uint32_t y = 0; y < guiStuff.RenderTarget.metrics().y; ++y) {
			::llc::vcbgra			scanLineSrc					= guiStuff.RenderTarget[y];
			::llc::vbgra			scanLineDst					= {(::llc::bgra*)(&((sc_t*)mappedTexture.pData)[y * mappedTexture.RowPitch]), scanLineSrc.size()};
			memcpy(scanLineDst.begin(), scanLineSrc.begin(), scanLineSrc.byte_count());
			//for(uint32_t x = 0; x < scanLineDst.size(); ++x)
			//	 scanLineDst.begin()[x].a	= scanLineDst.begin()[x].a;
		}
	}

	d3dResources.GetD3DDeviceContext()->Unmap(guiStuff.Texture2D.get(), 0);
	// Each vertex is one instance of the VertexPositionColor struct.
	UINT									stride						= sizeof(::llc::n3f2_t) + sizeof(::llc::n2f2_t);
	UINT									offset						= 0;

	ID3D11DeviceContext						* context					= d3dResources.GetD3DDeviceContext();
	::llc::pcom<ID3D11Buffer>				vb							= guiStuff.VertexBuffer;
	::llc::pcom<ID3D11Buffer>				ib							= guiStuff.IndexBuffer ;
	context->IASetVertexBuffers		(0, 1, &vb, &stride, &offset);
	context->IASetIndexBuffer		(ib, DXGI_FORMAT_R16_UINT, 0);// Each index is one 16-bit unsigned integer (short).
	context->IASetPrimitiveTopology	(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout		(guiStuff.InputLayout);
	context->VSSetShader			(guiStuff.VertexShader, nullptr, 0);	// Attach our vertex shader.
	context->PSSetShader			(guiStuff.PixelShader, nullptr, 0);	// Attach our pixel shader.
	context->RSSetState				(guiStuff.RasterizerState);
	context->PSSetSamplers			(0, 1, &guiStuff.SamplerState);
	context->PSSetShaderResources	(0, 1, &guiStuff.ShaderResourceView);
	context->OMSetBlendState		(guiStuff.BlendState, 0, 0xFFFFFFFF);
	context->OMSetDepthStencilState	(guiStuff.DepthStencilState, 0);
	//
	context->DrawIndexed(6, 0, 0);	// Draw the objects.
	return 0;
}

static	::llc::error_t	getNodeTransform		(const ::llc::SRenderNodeManager & nodeManager, uint32_t iNode, const ::llc::SEngineSceneConstants & constants, ::llc::SRenderNodeConstants	& nodeConstants) {
	nodeConstants			= nodeManager.Transforms[iNode];

	nodeConstants.Model					= nodeManager.BaseTransforms[iNode].Model * nodeConstants.Model;
	nodeConstants.ModelInverse			= nodeConstants.Model.GetInverse();
	nodeConstants.ModelInverseTranspose	= nodeConstants.ModelInverse.GetTranspose();
	nodeConstants.MVP					= nodeConstants.Model * constants.View * constants.Perspective;

	nodeConstants.Model					= nodeConstants.Model.GetTranspose();
	nodeConstants.ModelInverse			= nodeConstants.ModelInverse.GetTranspose();
	nodeConstants.ModelInverseTranspose	= nodeConstants.ModelInverseTranspose.GetTranspose();
	nodeConstants.MVP					= nodeConstants.MVP.GetTranspose();
	nodeConstants.NodeSize				= nodeManager.BaseTransforms[iNode].NodeSize; // Have to update this some other way.
	return 0;
}

::llc::err_t llc::d3dDrawEngineNode(::llc::Sample3DSceneRenderer & renderer, const ::llc::SEngineScene & scene, ::llc::u2_t iNode, ::llc::view<const ::llc::SRenderLight> lights, const ::llc::SRenderMaterial * material, ::llc::u2_t vertexShader) {
	if_true_fe(iNode >= scene.RenderNodes.size() || !scene.Graphics);
	const ::llc::SRenderNode & node = scene.RenderNodes[iNode];
	if_true_fe(node.Mesh >= scene.Graphics->Meshes.size() || node.Skin >= scene.Graphics->Skins.size());
	const ::llc::SGeometryMesh & mesh = *scene.Graphics->Meshes[node.Mesh];
	const ::llc::SSkin & skin = *scene.Graphics->Skins[node.Skin];
	if_true_fe(0 == mesh.GeometryBuffers.size());
	const auto & indices = *scene.Graphics->Buffers[mesh.GeometryBuffers[0]];
	const ::llc::u2_t indexCount = indices.Data.size() / indices.Desc.Format.TotalBytes();
	const ::llc::rangeu2_t slice = node.Slice < mesh.GeometrySlices.size() ? mesh.GeometrySlices[node.Slice].Slice : ::llc::rangeu2_t{0, indexCount};
	if_true_fe(slice.Offset > indexCount || slice.Count > indexCount - slice.Offset);
	::llc::SRenderNodeConstants constants;
	llc_necs(::getNodeTransform(scene.RenderNodes, iNode, renderer.ConstantBufferScene, constants));
	constants.Material = material ? *material : skin.Material;
	const ::llc::m4f2_t model = scene.RenderNodes.BaseTransforms[iNode].Model * scene.RenderNodes.Transforms[iNode].Model;
	llc_necs(::llc::lightFilter(model.GetTranslation(), lights, renderer.RenderCache.LightsNode));
	return renderer.Render(node.Mesh, slice, skin.Textures, node.Shader, constants.Material.Color.Diffuse.a != 1.0f, constants, renderer.RenderCache.LightsNode, vertexShader);
}

::llc::error_t			llc::d3dDrawEngineScene	(::llc::Sample3DSceneRenderer & d3dScene, const ::llc::SEngineScene & engineScene, const ::llc::n2u1_t & targetMetrics, const ::llc::n3f2_t & lightPos, const ::llc::n3f2_t & cameraPosition, const llc::n3f2_t & cameraTarget, const llc::minmaxf2_t & nearFar, ::llc::f3_t time)	{
	::llc::n3f2_t				cameraFront				= (cameraTarget - cameraPosition).Normalize();

	::llc::SEngineSceneConstants& constants				= d3dScene.ConstantBufferScene;
	constants.CameraPosition= cameraPosition;
	constants.CameraFront	= cameraFront;
	constants.LightPosition	= lightPos;
	constants.LightDirection= {0, -1, 0};
	constants.Time			= (::llc::f2_t)time;

	constants.View.LookAt(cameraPosition, cameraTarget, {0, 1, 0});
	constants.Perspective.FieldOfView(.25 * ::llc::math_pi, targetMetrics.x / (double)targetMetrics.y, nearFar.Min, nearFar.Max);
	constants.Screen.ViewportLH(targetMetrics);
	constants.VP			= constants.View * constants.Perspective;
	constants.VPS			= constants.VP * constants.Screen;

	::llc::SRenderNodeList						renderList;
	llc_necs(engineScene.BuildRenderList(cameraPosition, cameraFront, renderList));
	llc_necs(engineScene.BuildLightList(d3dScene.RenderCache.LightsWorld));
	for(::llc::u2_t node : renderList.Solid)
		llc_necs(::llc::d3dDrawEngineNode(d3dScene, engineScene, node, d3dScene.RenderCache.LightsWorld));
	for(::llc::u2_t node : renderList.Alpha)
		llc_necs(::llc::d3dDrawEngineNode(d3dScene, engineScene, node, d3dScene.RenderCache.LightsWorld));
	return 0;
}
::llc::error_t							llc::d3dAppDraw				(::llc::SD3DApplication & d3dApp, const ::llc::SEngineScene & engineScene, const ::llc::rgbaf & clearColor, const ::llc::n3f2_t & lightPos, const ::llc::n3f2_t & cameraPosition, const llc::n3f2_t & cameraTarget, const llc::minmaxf2_t & nearFar, ::llc::f3_t time) {
	{ // Set up render target for this frame
		ID3D11DeviceContext3						* context							= d3dApp.DeviceResources->GetD3DDeviceContext();
		const D3D11_VIEWPORT						viewport							= d3dApp.DeviceResources->GetScreenViewport();	// Reset the viewport to target the whole screen.
		context->RSSetViewports(1, &viewport);

		ID3D11RenderTargetView *					targets	[]							= { d3dApp.DeviceResources->GetBackBufferRenderTargetView(), };	// Reset render targets to the screen.
		context->OMSetRenderTargets(1, targets, d3dApp.DeviceResources->GetDepthStencilView());

		context->ClearRenderTargetView(d3dApp.DeviceResources->GetBackBufferRenderTargetView(), &clearColor.r);	// Clear the back buffer and depth stencil view.
		context->ClearDepthStencilView(d3dApp.DeviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);	
	}
	{ // Render 3d scene
		//(void)engineScene; (void)lightPos; (void)cameraPosition; (void)cameraTarget; (void)nearFar;
		llc_necs(llc::d3dDrawEngineScene(d3dApp.Scene, engineScene, d3dApp.DeviceResources->GetLogicalSize().u1(), lightPos, cameraPosition, cameraTarget, nearFar, time));
	}
	{ // Render GUI
		llc_necs(llc::d3dGUIDraw(*d3dApp.DeviceResources, d3dApp.GUIStuff)); 

		llc_necs(d3dApp.Text.Render());
	}
	llc_necs(d3dApp.DeviceResources->Present());
	return 0;
}
