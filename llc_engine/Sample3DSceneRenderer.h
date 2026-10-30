#include "llc_d3d_device_resources.h"
#include "llc_engine_scene.h"

#include "llc_file.h"

#ifndef SAMPLE3DSCENERENDERER_H_23627
#define SAMPLE3DSCENERENDERER_H_23627

namespace llc
{

	// This sample renderer instantiates a basic rendering pipeline.
	struct Sample3DSceneRenderer {
		::llc::pobj<DX::D3DDeviceResources>		DeviceResources;

		// Direct3D resources for cube geometry
		::llc::acom<ID3D11Buffer		>		VertexBuffer;
		::llc::acom<ID3D11Buffer		>		IndexBuffer;
		::llc::acom<ID3D11InputLayout	>		InputLayout;
		::llc::acom<ID3D11VertexShader	>		VertexShader;
		::llc::acom<ID3D11PixelShader	>		PixelShader;
		::llc::acom<ID3D11Buffer		>		ConstantBuffer;
		::llc::acom<ID3D11SamplerState	>		SamplerStates;
		::llc::acom<ID3D11ShaderResourceView>	ShaderResourceView;
		::llc::acom<ID3D11BlendState>			BlendState;
		::llc::acom<ID3D11RasterizerState>		RasterizerState;
		::llc::acom<ID3D11DepthStencilState>	DepthStencilState;
		UINT									BoundShaderResourceCount		= 0;
		::llc::au2_t							LightSlots					= {};
		::llc::pcom<ID3D11Buffer>				LightBuffer, LightConstants;
		::llc::pcom<ID3D11ShaderResourceView>	LightView;
		::llc::SEngineRenderCache				RenderCache;

		::llc::err_t							SetLights(::llc::view<const ::llc::SRenderLight> lights);
		::llc::err_t							CreateVertexShader(::llc::vcst_t name);
		
		// System resources for cube geometry.
		::llc::SEngineSceneConstants			ConstantBufferScene				= {};

		// Variables used with the rendering loo
		f3_t									TotalSecondsElapsed				= 0;
		
												~Sample3DSceneRenderer			() { ReleaseDeviceResources(); }

		::llc::err_t							Initialize		(const ::llc::pobj<DX::D3DDeviceResources> & deviceResources) {
			DeviceResources							= deviceResources;
			if_fail_fe(CreateDeviceResources(DeviceResources->GetD3DDevice()));
			if_fail_fe(CreateSizeDependentResources(DeviceResources->GetLogicalSize(), DeviceResources->GetOrientationTransform3D()));
			return 0;
		}

		void									ReleaseDeviceResources			() {
			VertexBuffer		.clear();
			IndexBuffer			.clear();
			InputLayout			.clear();
			VertexShader		.clear();
			PixelShader			.clear();
			ConstantBuffer		.clear();
			SamplerStates		.clear();
			ShaderResourceView	.clear();
			BlendState			.clear();
			RasterizerState		.clear();
			DepthStencilState	.clear();
			BoundShaderResourceCount = 0;
			LightSlots.clear();
			LightBuffer = {}; LightConstants = {}; LightView = {};
		}

		::llc::err_t						Render							(u2_t iMesh, ::llc::rangeu2_t indexRange, ::llc::vcu2_t iTextures, u2_t iShader, bool alpha, const::llc::SRenderNodeConstants & constantBufferModel, ::llc::view<const ::llc::SRenderLight> lights = {}, ::llc::u2_t iVertexShader = 0, ::llc::CULL_MODE cullMode = ::llc::CULL_MODE_CCW) {
			if(cullMode > ::llc::CULL_MODE_NONE || (::llc::u2_t)cullMode >= RasterizerState.size())
				return ::llc::OS_INVALID_PARAMETER;
			if(iTextures.size() > D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT)
				return ::llc::OS_INVALID_PARAMETER;
			if(iShader >= PixelShader.size() || iVertexShader >= VertexShader.size() || iMesh >= IndexBuffer.size() || iMesh >= VertexBuffer.size())
				return ::llc::OS_INVALID_PARAMETER;
			const ::llc::u2_t lightSlot = iShader < LightSlots.size() ? LightSlots[iShader] : (::llc::u2_t)-1;
			if(lightSlot != (::llc::u2_t)-1) {
				if(lightSlot < iTextures.size() || lightSlot >= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT)
					return ::llc::OS_INVALID_PARAMETER;
				if_fail_fe(SetLights(lights));
			}
			auto										context							= DeviceResources->GetD3DDeviceContext();
			context->UpdateSubresource1(ConstantBuffer[0], 0, NULL, &constantBufferModel, 0, 0, 0);	// Prepare the constant buffer to send it to the graphics device.
			context->UpdateSubresource1(ConstantBuffer[1], 0, NULL, &ConstantBufferScene, 0, 0, 0);	// Prepare the constant buffer to send it to the graphics device.

			// Each vertex is one instance of the VertexPositionColor struct.
			UINT										stride							= sizeof(::llc::n3f2_t) * 2 + sizeof(::llc::n2f2_t);
			UINT										offset							= 0;

			::llc::pcom<ID3D11Buffer>					vb								= VertexBuffer[iMesh];
			::llc::pcom<ID3D11Buffer>					ib								= IndexBuffer [iMesh];
			context->IASetVertexBuffers		(0, 1, &vb, &stride, &offset);
			context->IASetIndexBuffer		(ib, DXGI_FORMAT_R16_UINT, 0);// Each index is one 16-bit unsigned integer (short).
			context->IASetPrimitiveTopology	(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			context->IASetInputLayout		(InputLayout[0]);

			context->VSSetShader			(VertexShader[iVertexShader], nullptr, 0);	// Attach our vertex shader.
			context->VSSetConstantBuffers1	(0, 1, &ConstantBuffer[0], nullptr, nullptr);	// Send the constant buffer to the graphics device.
			context->VSSetConstantBuffers1	(1, 1, &ConstantBuffer[1], nullptr, nullptr);

			context->PSSetConstantBuffers1	(0, 1, &ConstantBuffer[0], nullptr, nullptr);	// Send the constant buffer to the graphics device.
			context->PSSetConstantBuffers1	(1, 1, &ConstantBuffer[1], nullptr, nullptr);	// Send the constant buffer to the graphics device.
			context->PSSetShader			(PixelShader[iShader], nullptr, 0);	// Attach our pixel shader.

			D3D11_BUFFER_DESC							desc							= {};
			ib->GetDesc(&desc);

		    context->PSSetSamplers( 0, 1, &SamplerStates[0] );
			ID3D11ShaderResourceView					* resources[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = {};
			for(UINT iTexture = 0; iTexture < iTextures.size(); ++iTexture) {
				if(iTextures[iTexture] >= ShaderResourceView.size())
					return ::llc::OS_INVALID_PARAMETER;
				resources[iTexture]						= ShaderResourceView[iTextures[iTexture]].get();
			}
			UINT usedResourceCount = (UINT)iTextures.size();
			if(lightSlot != (::llc::u2_t)-1) {
				resources[lightSlot] = LightView.get();
				usedResourceCount = ::llc::max(usedResourceCount, lightSlot + 1);
				context->PSSetConstantBuffers(2, 1, &LightConstants);
			}
			const UINT								resourceCount					= ::llc::max(usedResourceCount, BoundShaderResourceCount);
			if(resourceCount)
				context->PSSetShaderResources(0, resourceCount, resources);
			BoundShaderResourceCount					= usedResourceCount;

			context->RSSetState(RasterizerState[cullMode]);
	
			context->OMSetBlendState(BlendState[alpha ? 1 : 0], 0, 0xFFFFFFFF);
			context->OMSetDepthStencilState(DepthStencilState[alpha ? 1 : 0], 0xFF);

			context->DrawIndexed(indexRange.Count, indexRange.Offset, 0);	// Draw the objects.
			return 0;
		}

		::llc::err_t							CreateSizeDependentResources	(const ::llc::n2f2_t & outputSize, const DirectX::XMFLOAT4X4 & orientation) {
			const f2_t									aspectRatio						= outputSize.x / outputSize.y;
			f2_t										fovAngleY						= 70.0f * DirectX::XM_PI / 180.0f;

			// This is a simple example of change that can be made when the app is in portrait or snapped view.
			if (aspectRatio < 1.0f)
				fovAngleY								*= 2.0f;

			// Note that the OrientationTransform3D matrix is post-multiplied here in order to correctly orient the scene to match the display orientation.
			// This post-multiplication step is required for any draw calls that are made to the swap chain render target. For draw calls to other targets, this transform should not be applied.
			// This sample makes use of a right-handed coordinate system using row-major matrices.
			DirectX::XMMATRIX							perspectiveMatrix				= DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, 0.01f, 100.0f);
			DirectX::XMMATRIX							orientationMatrix				= DirectX::XMLoadFloat4x4(&orientation);
			DirectX::XMStoreFloat4x4((DirectX::XMFLOAT4X4*)&ConstantBufferScene.Perspective, DirectX::XMMatrixTranspose(perspectiveMatrix * orientationMatrix));

			// Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
			const	DirectX::XMVECTORF32			eye								= { 0.0f, (f2_t)sin(TotalSecondsElapsed) * 5, 5.5f, 0.0f };
			stxp	DirectX::XMVECTORF32			at								= { 0.0f, -0.1f, 0.0f, 0.0f };
			stxp	DirectX::XMVECTORF32			up								= { 0.0f, 1.0f, 0.0f, 0.0f };
			DirectX::XMStoreFloat4x4((DirectX::XMFLOAT4X4*)&ConstantBufferScene.View, DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtLH(eye, at, up)));
			return 0;
		}

		::llc::err_t							CreateDeviceResources			(ID3D11Device3 * d3dDevice)	{
			ReleaseDeviceResources();
			{	// Create a point sampler state for fonts 
				D3D11_SAMPLER_DESC							samDesc						= {};
				samDesc.Filter							= D3D11_FILTER_MIN_MAG_MIP_POINT;
				samDesc.AddressU						= D3D11_TEXTURE_ADDRESS_CLAMP;
				samDesc.AddressV						= D3D11_TEXTURE_ADDRESS_CLAMP;
				samDesc.AddressW						= D3D11_TEXTURE_ADDRESS_CLAMP;
				samDesc.MaxAnisotropy					= 1;
				samDesc.ComparisonFunc					= D3D11_COMPARISON_ALWAYS;
				samDesc.MaxLOD							= D3D11_FLOAT32_MAX;

				::llc::pcom<ID3D11SamplerState>				samplerState;
				llc_hrcall(d3dDevice->CreateSamplerState(&samDesc, &samplerState));
				SamplerStates.push_back(samplerState);
			}
			{	// Create a sampler state for diffuse map
				D3D11_SAMPLER_DESC							samDesc						= {};
				samDesc.Filter							= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				samDesc.AddressU						= D3D11_TEXTURE_ADDRESS_WRAP;
				samDesc.AddressV						= D3D11_TEXTURE_ADDRESS_WRAP;
				samDesc.AddressW						= D3D11_TEXTURE_ADDRESS_WRAP;
				samDesc.MaxAnisotropy					= 1;
				samDesc.ComparisonFunc					= D3D11_COMPARISON_ALWAYS;
				samDesc.MaxLOD							= D3D11_FLOAT32_MAX;

				::llc::pcom<ID3D11SamplerState>				samplerState;
				llc_hrcall(d3dDevice->CreateSamplerState(&samDesc, &samplerState));
				SamplerStates.push_back(samplerState);
			}
			{
				// Load shaders asynchronously.
				::llc::pcom<ID3D11InputLayout>				inputLayout;
				::llc::pcom<ID3D11VertexShader>				vertexShader;

				::llc::string								fileVS;
				if_fail_fe(llc::fileToMemory("SampleVertexShader.cso", fileVS));
				stxp	D3D11_INPUT_ELEMENT_DESC			vertexDesc []			=
					{	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 00, D3D11_INPUT_PER_VERTEX_DATA, 0 }
					,	{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
					,	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
					};
				// After the vertex shader file is loaded, create the shader and input layout.
				llc_hrcall(d3dDevice->CreateVertexShader(&fileVS[0], fileVS.size(), nullptr, &vertexShader));
				llc_hrcall(d3dDevice->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), &fileVS[0], fileVS.size(), &inputLayout)); 
				InputLayout		.push_back(inputLayout);
				VertexShader	.push_back(vertexShader);
			}
			{
				::llc::pcom<ID3D11Buffer>					constantBuffer;
				D3D11_BUFFER_DESC							constantBufferDesc		= {sizeof(::llc::SRenderNodeConstants)};
				constantBufferDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
				llc_hrcall(d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer));
				ConstantBuffer.push_back(constantBuffer);
			}
			{
				::llc::pcom<ID3D11Buffer>					constantBuffer;
				D3D11_BUFFER_DESC							constantBufferDesc		= {sizeof(::llc::SEngineSceneConstants)};
				constantBufferDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
				llc_hrcall(d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer));
				ConstantBuffer.push_back(constantBuffer);
			}
			{
				D3D11_BLEND_DESC				blendDesc		= {};
				blendDesc.RenderTarget[0].BlendEnable			= 0;
				blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
				::llc::pcom<ID3D11BlendState>	blendState;
				llc_hrcall(d3dDevice->CreateBlendState(&blendDesc, &blendState));
				BlendState.push_back(blendState);
			}
			{ // straight
				D3D11_BLEND_DESC				blendDesc	= {};
				blendDesc.RenderTarget[0].BlendEnable			= TRUE;
				blendDesc.RenderTarget[0].SrcBlend				=
				blendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_SRC_ALPHA;
				blendDesc.RenderTarget[0].DestBlend				=
				blendDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_INV_SRC_ALPHA;
				blendDesc.RenderTarget[0].BlendOp				=
				blendDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
				blendDesc.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
				::llc::pcom<ID3D11BlendState>	blendState;
				llc_hrcall(d3dDevice->CreateBlendState(&blendDesc, &blendState));
				BlendState.push_back(blendState);
			}
			{ // premultiplied
				D3D11_BLEND_DESC				blendDesc	= {};
				blendDesc.RenderTarget[0].BlendEnable			= TRUE;
				blendDesc.RenderTarget[0].SrcBlend				=
				blendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
				blendDesc.RenderTarget[0].DestBlend				=
				blendDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_INV_SRC_ALPHA;
				blendDesc.RenderTarget[0].BlendOp				=
				blendDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
				blendDesc.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
				::llc::pcom<ID3D11BlendState>	blendState;
				llc_hrcall(d3dDevice->CreateBlendState(&blendDesc, &blendState));
				BlendState.push_back(blendState);
			}
			{ // additiv
				D3D11_BLEND_DESC				blendDesc	= {};
				blendDesc.RenderTarget[0].BlendEnable			= TRUE;
				blendDesc.RenderTarget[0].SrcBlend				=
				blendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_SRC_ALPHA;
				blendDesc.RenderTarget[0].DestBlend				=
				blendDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_ONE;
				blendDesc.RenderTarget[0].BlendOp				=
				blendDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
				blendDesc.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
				::llc::pcom<ID3D11BlendState>	blendState;
				llc_hrcall(d3dDevice->CreateBlendState(&blendDesc, &blendState));
				BlendState.push_back(blendState);
			}
			for(::llc::CULL_MODE mode : {::llc::CULL_MODE_CCW, ::llc::CULL_MODE_CW, ::llc::CULL_MODE_NONE}) {
				D3D11_RASTERIZER_DESC			rsDesc							= {};
				rsDesc.FillMode				= D3D11_FILL_SOLID; // D3D11_FILL_WIREFRAME; // 
				rsDesc.CullMode				= mode == ::llc::CULL_MODE_NONE ? D3D11_CULL_NONE : (mode == ::llc::CULL_MODE_CCW ? D3D11_CULL_BACK : D3D11_CULL_FRONT);
				rsDesc.DepthClipEnable		= false;
				::llc::pcom<ID3D11RasterizerState>	rasterizerState;
				llc_hrcall(d3dDevice->CreateRasterizerState(&rsDesc, &rasterizerState));
				if_fail_fe(RasterizerState.push_back(rasterizerState));
			}
			{ // enabled
				D3D11_DEPTH_STENCIL_DESC		depthStencilDesc				= {};
				depthStencilDesc.DepthEnable		= TRUE;
				depthStencilDesc.DepthWriteMask		= D3D11_DEPTH_WRITE_MASK_ALL;
				depthStencilDesc.DepthFunc			= D3D11_COMPARISON_LESS;
				depthStencilDesc.StencilEnable		= 0;
				depthStencilDesc.StencilReadMask	= D3D11_DEFAULT_STENCIL_READ_MASK;
				depthStencilDesc.StencilWriteMask	= D3D11_DEFAULT_STENCIL_WRITE_MASK;

				depthStencilDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
				depthStencilDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_KEEP;
				depthStencilDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
				depthStencilDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
				::llc::pcom<ID3D11DepthStencilState>	depthStencilState;
				llc_hrcall(d3dDevice->CreateDepthStencilState(&depthStencilDesc, &depthStencilState));
				DepthStencilState.push_back(depthStencilState);
			}
			{ // alpha
				D3D11_DEPTH_STENCIL_DESC		depthStencilDesc				= {};
				depthStencilDesc.DepthEnable		= TRUE;
				depthStencilDesc.DepthWriteMask		= D3D11_DEPTH_WRITE_MASK_ZERO;
				depthStencilDesc.DepthFunc			= D3D11_COMPARISON_LESS;
				depthStencilDesc.StencilEnable		= 0;
				depthStencilDesc.StencilReadMask	= D3D11_DEFAULT_STENCIL_READ_MASK;
				depthStencilDesc.StencilWriteMask	= D3D11_DEFAULT_STENCIL_WRITE_MASK;

				depthStencilDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
				depthStencilDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_KEEP;
				depthStencilDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
				depthStencilDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
				::llc::pcom<ID3D11DepthStencilState>	depthStencilState;
				llc_hrcall(d3dDevice->CreateDepthStencilState(&depthStencilDesc, &depthStencilState));
				DepthStencilState.push_back(depthStencilState);
			}
			{ // disabled
				D3D11_DEPTH_STENCIL_DESC		depthStencilDesc				= {};
				depthStencilDesc.DepthEnable		= 0;
				depthStencilDesc.DepthWriteMask		= D3D11_DEPTH_WRITE_MASK_ZERO;
				depthStencilDesc.DepthFunc			= D3D11_COMPARISON_NEVER;
				depthStencilDesc.StencilEnable		= 0;
				depthStencilDesc.StencilReadMask	= D3D11_DEFAULT_STENCIL_READ_MASK;
				depthStencilDesc.StencilWriteMask	= D3D11_DEFAULT_STENCIL_WRITE_MASK;

				depthStencilDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
				depthStencilDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_KEEP;
				depthStencilDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
				depthStencilDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
				depthStencilDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
				depthStencilDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_KEEP;
				depthStencilDesc.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
				depthStencilDesc.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
				::llc::pcom<ID3D11DepthStencilState>	depthStencilState;
				llc_hrcall(d3dDevice->CreateDepthStencilState(&depthStencilDesc, &depthStencilState));
				DepthStencilState.push_back(depthStencilState);
			}
			return 0;
		}
	};
}

#endif // SAMPLE3DSCENERENDERER_H_293468238
