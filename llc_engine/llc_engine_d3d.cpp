#include "llc_engine_d3d.h"

#if defined(LLC_WINDOWS)

#include "llc_file.h"
#include "llc_windows.h"

#include <algorithm>

static ::llc::error_t loadShaderFile(::llc::vcst_c shaderName, ::llc::as0_t & bytes) {
	::llc::astchar<512> fileName = {};
	llc_necs(::llc::sprintf_s(fileName.Storage, "%s.cso", shaderName.begin()));
	return ::llc::fileToMemory(::llc::vcst_t{fileName.Storage}, bytes);
}

static ::llc::error_t createMeshResources(ID3D11Device * device, const ::llc::SMeshManager & meshes, const ::llc::SRenderBufferManager & buffers, ::llc::aobj<::llc::SD3DMeshResources> & output) {
	llc_necs(output.resize(meshes.size()));
	for(uint32_t iMesh = 0; iMesh < meshes.size(); ++iMesh) {
		const ::llc::pobj<::llc::SGeometryMesh> & mesh = meshes[iMesh];
		if(!mesh)
			continue;
		::llc::pobj<::llc::SRenderBuffer> indices = {};
		::llc::apobj<::llc::SRenderBuffer> vertices = {};
		uint32_t vertexSize = 0;
		for(uint32_t iBuffer : mesh->GeometryBuffers) {
			const ::llc::pobj<::llc::SRenderBuffer> & buffer = buffers[iBuffer];
			if(!buffer)
				continue;
			if(buffer->Desc.Usage == ::llc::BUFFER_USAGE_Index)
				indices = buffer;
			else {
				llc_necs(vertices.push_back(buffer));
				vertexSize += buffer->Desc.Format.TotalBytes();
			}
		}
		if(!indices || !vertices.size() || !vertexSize)
			continue;

		const uint32_t vertexCount = vertices[0]->Data.size() / vertices[0]->Desc.Format.TotalBytes();
		::llc::au0_t packed = {};
		llc_necs(packed.reserve(vertexCount * vertexSize));
		for(uint32_t iValue = 0; iValue < vertexCount; ++iValue)
			for(const ::llc::pobj<::llc::SRenderBuffer> & buffer : vertices) {
				const uint32_t valueSize = buffer->Desc.Format.TotalBytes();
				llc_necs(packed.append({&buffer->Data[iValue * valueSize], valueSize}));
			}

		D3D11_SUBRESOURCE_DATA vertexData = {packed.begin()};
		D3D11_BUFFER_DESC vertexDesc = {};
		vertexDesc.ByteWidth = packed.size();
		vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		llc_hrcall(device->CreateBuffer(&vertexDesc, &vertexData, output[iMesh].VertexBuffer.put()));

		const uint32_t indexSize = indices->Desc.Format.TotalBytes();
		D3D11_SUBRESOURCE_DATA indexData = {indices->Data.begin()};
		D3D11_BUFFER_DESC indexDesc = {};
		indexDesc.ByteWidth = indices->Data.size();
		indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		::llc::au1_t promoted = {};
		if(indexSize == 1) {
			llc_necs(promoted.resize(indices->Data.size()));
			for(uint32_t i = 0; i < promoted.size(); ++i)
				promoted[i] = indices->Data[i];
			indexData.pSysMem = promoted.begin();
			indexDesc.ByteWidth = promoted.byte_count();
			output[iMesh].IndexFormat = DXGI_FORMAT_R16_UINT;
		}
		else
			output[iMesh].IndexFormat = indexSize == 4 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
		output[iMesh].IndexCount = indexDesc.ByteWidth / (output[iMesh].IndexFormat == DXGI_FORMAT_R32_UINT ? 4U : 2U);
		llc_hrcall(device->CreateBuffer(&indexDesc, &indexData, output[iMesh].IndexBuffer.put()));
	}
	return 0;
}

static ::llc::error_t createTextures(ID3D11Device * device, const ::llc::SSurfaceManager & surfaces, ::llc::acom<ID3D11ShaderResourceView> & output) {
	llc_necs(output.resize(surfaces.size()));
	for(uint32_t iSurface = 0; iSurface < surfaces.size(); ++iSurface) {
		const ::llc::pobj<::llc::SSurface> & surface = surfaces[iSurface];
		if(!surface || !surface->Data.size() || !surface->Desc.Dimensions.x || !surface->Desc.Dimensions.y)
			continue;
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = surface->Desc.Dimensions.x;
		desc.Height = surface->Desc.Dimensions.y;
		desc.MipLevels = desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		D3D11_SUBRESOURCE_DATA data = {surface->Data.begin(), desc.Width * 4U};
		::llc::pcom<ID3D11Texture2D> texture = {};
		llc_hrcall(device->CreateTexture2D(&desc, &data, texture.put()));
		llc_hrcall(device->CreateShaderResourceView(texture, 0, output[iSurface].put()));
	}
	return 0;
}

static ::llc::error_t createPixelShaders(ID3D11Device * device, const ::llc::SShaderManager & shaders, ::llc::acom<ID3D11PixelShader> & output) {
	llc_necs(output.resize(shaders.size()));
	for(uint32_t iShader = 0; iShader < shaders.size(); ++iShader) {
		::llc::as0_t bytes = {};
		llc_necall(::loadShaderFile(shaders.Names[iShader], bytes), "Shader: %s", shaders.Names[iShader].begin());
		llc_hrcall(device->CreatePixelShader(bytes.begin(), bytes.size(), 0, output[iShader].put()));
	}
	return 0;
}

static ::llc::error_t getNodeTransformD3D(const ::llc::SRenderNodeManager & nodes, uint32_t iNode, const ::llc::SEngineSceneConstants & scene, ::llc::SRenderNodeConstants & output) {
	output = nodes.Transforms[iNode];
	output.Model = nodes.BaseTransforms[iNode].Model * output.Model;
	output.ModelInverse = output.Model.GetInverse();
	output.ModelInverseTranspose = output.ModelInverse.GetTranspose();
	output.MVP = output.Model * scene.View * scene.Perspective;
	output.Model = output.Model.GetTranspose();
	output.ModelInverse = output.ModelInverse.GetTranspose();
	output.ModelInverseTranspose = output.ModelInverseTranspose.GetTranspose();
	output.MVP = output.MVP.GetTranspose();
	output.NodeSize = nodes.BaseTransforms[iNode].NodeSize;
	return 0;
}

::llc::error_t llc::SD3DApplication::Initialize(HWND windowHandle, ::llc::n2u1_t windowSize, const ::llc::SEngineGraphics & graphics) {
	Shutdown();
	WindowHandle = windowHandle;
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferDesc.Width = windowSize.x;
	swapDesc.BufferDesc.Height = windowSize.y;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.BufferCount = 2;
	swapDesc.OutputWindow = windowHandle;
	swapDesc.Windowed = TRUE;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	uint32_t flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	const D3D_FEATURE_LEVEL levels[] = {D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0};
	D3D_FEATURE_LEVEL level = {};
	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, flags, levels, ::llc::size(levels), D3D11_SDK_VERSION, &swapDesc, SwapChain.put(), Device.put(), &level, Context.put());
	if(FAILED(hr))
		hr = ::D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_WARP, 0, flags & ~D3D11_CREATE_DEVICE_DEBUG, levels, ::llc::size(levels), D3D11_SDK_VERSION, &swapDesc, SwapChain.put(), Device.put(), &level, Context.put());
	llc_rv_hrcall(-1, hr);
	llc_necs(Resize(windowSize));
	return Refresh(graphics);
}

::llc::error_t llc::SD3DApplication::Resize(::llc::n2u1_t windowSize) {
	if(!SwapChain || !windowSize.x || !windowSize.y)
		return 0;
	Context->OMSetRenderTargets(0, 0, 0);
	RenderTarget = {};
	DepthStencil = {};
	DepthTexture = {};
	if(WindowSize != windowSize)
		llc_hrcall(SwapChain->ResizeBuffers(0, windowSize.x, windowSize.y, DXGI_FORMAT_UNKNOWN, 0));
	WindowSize = windowSize;
	::llc::pcom<ID3D11Texture2D> backBuffer = {};
	llc_hrcall(SwapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.put())));
	llc_hrcall(Device->CreateRenderTargetView(backBuffer, 0, RenderTarget.put()));
	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width = windowSize.x;
	depthDesc.Height = windowSize.y;
	depthDesc.MipLevels = depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	llc_hrcall(Device->CreateTexture2D(&depthDesc, 0, DepthTexture.put()));
	llc_hrcall(Device->CreateDepthStencilView(DepthTexture, 0, DepthStencil.put()));
	return 0;
}

::llc::error_t llc::SD3DApplication::Refresh(const ::llc::SEngineGraphics & graphics) {
	Meshes.clear();
	Textures.clear();
	PixelShaders.clear();
	llc_necs(::createMeshResources(Device, graphics.Meshes, graphics.Buffers, Meshes));
	llc_necs(::createTextures(Device, graphics.Surfaces, Textures));
	llc_necs(::createPixelShaders(Device, graphics.Shaders, PixelShaders));
	{
		::llc::as0_t bytes = {};
		llc_necs(::loadShaderFile("SampleVertexShader", bytes));
		llc_hrcall(Device->CreateVertexShader(bytes.begin(), bytes.size(), 0, VertexShader.put()));
		const D3D11_INPUT_ELEMENT_DESC desc[] =
			{{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
			,{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
			,{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}};
		llc_hrcall(Device->CreateInputLayout(desc, ::llc::size(desc), bytes.begin(), bytes.size(), InputLayout.put()));
	}
	D3D11_BUFFER_DESC constantDesc = {};
	constantDesc.ByteWidth = sizeof(::llc::SRenderNodeConstants);
	constantDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	llc_hrcall(Device->CreateBuffer(&constantDesc, 0, ConstantNode.put()));
	constantDesc.ByteWidth = sizeof(::llc::SEngineSceneConstants);
	llc_hrcall(Device->CreateBuffer(&constantDesc, 0, ConstantScene.put()));
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = samplerDesc.AddressV = samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	llc_hrcall(Device->CreateSamplerState(&samplerDesc, SamplerLinear.put()));
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	llc_hrcall(Device->CreateBlendState(&blendDesc, BlendOpaque.put()));
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	llc_hrcall(Device->CreateBlendState(&blendDesc, BlendAlpha.put()));
	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthClipEnable = TRUE;
	llc_hrcall(Device->CreateRasterizerState(&rasterDesc, Rasterizer.put()));
	D3D11_DEPTH_STENCIL_DESC depthStateDesc = {};
	depthStateDesc.DepthEnable = TRUE;
	depthStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	llc_hrcall(Device->CreateDepthStencilState(&depthStateDesc, DepthOpaque.put()));
	depthStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	llc_hrcall(Device->CreateDepthStencilState(&depthStateDesc, DepthAlpha.put()));
	return 0;
}

::llc::error_t llc::SD3DApplication::Draw(const ::llc::SEngineScene & scene, const ::llc::n3f2_t & cameraPosition, const ::llc::n3f2_t & cameraTarget, const ::llc::n3f2_t & cameraUp, const ::llc::minmaxf2_t & nearFar, ::llc::f3_t time) {
	if(!RenderTarget || !WindowSize.x || !WindowSize.y)
		return 0;
	const float clearColor[] = {2 / 255.0f, 3 / 255.0f, 8 / 255.0f, 1};
	ID3D11RenderTargetView * targets[] = {RenderTarget};
	Context->OMSetRenderTargets(1, targets, DepthStencil);
	Context->ClearRenderTargetView(RenderTarget, clearColor);
	Context->ClearDepthStencilView(DepthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	const D3D11_VIEWPORT viewport = {0, 0, (float)WindowSize.x, (float)WindowSize.y, 0, 1};
	Context->RSSetViewports(1, &viewport);
	Context->RSSetState(Rasterizer);

	::llc::SEngineSceneConstants constants = {};
	constants.CameraPosition = cameraPosition;
	constants.CameraFront = (cameraTarget - cameraPosition).Normalize();
	constants.Time = (float)time;
	constants.LightPosition = {};
	constants.LightDirection = {0, -1, 0};
	constants.View.LookAt(cameraPosition, cameraTarget, cameraUp);
	constants.Perspective.FieldOfView(.25 * ::llc::math_pi, WindowSize.x / (double)WindowSize.y, nearFar.Min, nearFar.Max);
	constants.Screen.ViewportLH(WindowSize);
	constants.VP = constants.View * constants.Perspective;
	constants.VPS = constants.VP * constants.Screen;
	Context->UpdateSubresource(ConstantScene, 0, 0, &constants, 0, 0);

	::llc::au2_t solid = {}, alpha = {};
	for(uint32_t iNode = 0; iNode < scene.RenderNodes.size(); ++iNode) {
		const ::llc::SRenderNode & node = scene.RenderNodes[iNode];
		if(scene.RenderNodes.Flags[iNode].NoDraw || node.Mesh >= Meshes.size() || node.Skin >= scene.Graphics->Skins.size() || !scene.Graphics->Skins[node.Skin])
			continue;
		const ::llc::SSkin & skin = *scene.Graphics->Skins[node.Skin];
		(skin.Material.Color.Diffuse.a < 1 ? alpha : solid).push_back(iNode);
	}
	const auto nodeDepth = [&scene, &constants](uint32_t iNode) {
		const ::llc::m4f2_t model = scene.RenderNodes.BaseTransforms[iNode].Model * scene.RenderNodes.Transforms[iNode].Model;
		return constants.CameraFront.Dot(model.GetTranslation());
	};
	::std::sort(solid.begin(), solid.end(), [&nodeDepth](uint32_t a, uint32_t b) { return nodeDepth(a) < nodeDepth(b); });
	::std::sort(alpha.begin(), alpha.end(), [&nodeDepth](uint32_t a, uint32_t b) { return nodeDepth(a) > nodeDepth(b); });
	const auto drawNode = [&](uint32_t iNode, bool useAlpha) -> ::llc::error_t {
		const ::llc::SRenderNode & node = scene.RenderNodes[iNode];
		if(node.Mesh >= Meshes.size() || !Meshes[node.Mesh].VertexBuffer || !Meshes[node.Mesh].IndexBuffer || node.Shader >= PixelShaders.size() || !PixelShaders[node.Shader])
			return 0;
		const ::llc::SSkin & skin = *scene.Graphics->Skins[node.Skin];
		::llc::SRenderNodeConstants nodeConstants = {};
		llc_necs(::getNodeTransformD3D(scene.RenderNodes, iNode, constants, nodeConstants));
		nodeConstants.Material = skin.Material;
		Context->UpdateSubresource(ConstantNode, 0, 0, &nodeConstants, 0, 0);
		const uint32_t stride = sizeof(::llc::n3f2_t) * 2 + sizeof(::llc::n2f2_t);
		const uint32_t offset = 0;
		ID3D11Buffer * vb = Meshes[node.Mesh].VertexBuffer;
		Context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
		Context->IASetIndexBuffer(Meshes[node.Mesh].IndexBuffer, Meshes[node.Mesh].IndexFormat, 0);
		Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Context->IASetInputLayout(InputLayout);
		Context->VSSetShader(VertexShader, 0, 0);
		ID3D11Buffer * nodeBuffer = ConstantNode;
		ID3D11Buffer * sceneBuffer = ConstantScene;
		Context->VSSetConstantBuffers(0, 1, &nodeBuffer);
		Context->PSSetConstantBuffers(0, 1, &nodeBuffer);
		Context->PSSetConstantBuffers(1, 1, &sceneBuffer);
		Context->PSSetShader(PixelShaders[node.Shader], 0, 0);
		ID3D11SamplerState * sampler = SamplerLinear;
		Context->PSSetSamplers(0, 1, &sampler);
		ID3D11ShaderResourceView * textureViews[2] = {};
		for(uint32_t iTexture = 0; iTexture < ::llc::min(2U, skin.Textures.size()); ++iTexture)
			if(skin.Textures[iTexture] < Textures.size())
				textureViews[iTexture] = Textures[skin.Textures[iTexture]];
		Context->PSSetShaderResources(0, 2, textureViews);
		Context->OMSetBlendState(useAlpha ? BlendAlpha.get() : BlendOpaque.get(), 0, 0xFFFFFFFF);
		Context->OMSetDepthStencilState(useAlpha ? DepthAlpha.get() : DepthOpaque.get(), 0);
		const ::llc::SGeometryMesh & mesh = *scene.Graphics->Meshes[node.Mesh];
		const ::llc::rangeu2_t slice = node.Slice < mesh.GeometrySlices.size() ? mesh.GeometrySlices[node.Slice].Slice : ::llc::rangeu2_t{0, Meshes[node.Mesh].IndexCount};
		Context->DrawIndexed(slice.Count, slice.Offset, 0);
		return 0;
	};
	for(uint32_t iNode : solid)
		llc_necs(drawNode(iNode, false));
	for(uint32_t iNode : alpha)
		llc_necs(drawNode(iNode, true));
	llc_hrcall(SwapChain->Present(1, 0));
	return 0;
}

void llc::SD3DApplication::Shutdown() {
	if(Context)
		Context->ClearState();
	DepthAlpha = {};
	DepthOpaque = {};
	Rasterizer = {};
	BlendAlpha = {};
	BlendOpaque = {};
	SamplerLinear = {};
	ConstantScene = {};
	ConstantNode = {};
	InputLayout = {};
	VertexShader = {};
	PixelShaders.clear();
	Textures.clear();
	Meshes.clear();
	DepthStencil = {};
	DepthTexture = {};
	RenderTarget = {};
	SwapChain = {};
	Context = {};
	Device = {};
	WindowHandle = {};
	WindowSize = {};
}

#endif // LLC_WINDOWS
