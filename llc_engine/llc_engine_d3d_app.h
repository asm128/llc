#include "Sample3DSceneRenderer.h"
#include "SampleFpsTextRenderer.h"
#include "llc_d3d_device_resources.h"

#include "llc_engine_d3d.h"

#include "llc_image.h"
#include "llc_file.h"
#include "llc_gui.h"

#ifndef LLC_ENGINE_D3D_APP_H_23627
#define LLC_ENGINE_D3D_APP_H_23627

namespace llc
{
	struct SD3DRenderTarget {
		::llc::pcom<ID3D11Texture2D>			Color, Depth, ReadColor, ReadDepth;
		::llc::pcom<ID3D11RenderTargetView>	Target;
		::llc::pcom<ID3D11DepthStencilView>	DepthView;
		::llc::err_t Begin	(::DX::D3DDeviceResources & resources, ::llc::gc8bgra colors);
		::llc::err_t Upload	(::DX::D3DDeviceResources & resources, ::llc::gc8bgra colors);
		::llc::err_t Read	(::DX::D3DDeviceResources & resources, ::llc::g8bgra colors, ::llc::gu2_t depth);
	};

	struct SD3DGUIVertex {
		::llc::n2f2_t Position, UV;
		::llc::bgra Color;
		::llc::u2_t Mode = 0;
		::llc::tri2f2_t Triangle = {};
	};
	struct SD3DGUIFont {
		::llc::pobj<::llc::SRasterFont> Font;
		::llc::pcom<ID3D11Texture2D> Texture;
		::llc::pcom<ID3D11ShaderResourceView> View;
	};
	tplt<tpnm _tColor>
	struct SD3DGUIResources {
		typedef	::llc::img<_tColor>				TRenderTarget;

		TRenderTarget							RenderTarget		= {};

		// We need to render the GUI separately to compose DirectX target from a dynamic texture.
		::llc::pcom<ID3D11BlendState>			BlendState			= {};
		::llc::pcom<ID3D11RasterizerState>		RasterizerState		= {};
		::llc::pcom<ID3D11SamplerState>			SamplerState		= {};
		::llc::pcom<ID3D11DepthStencilState>	DepthStencilState	= {};	// - Size-dependent
		//															= {}
		::llc::pcom<ID3D11Buffer>				IndexBuffer			= {};
		::llc::pcom<ID3D11Buffer>				VertexBuffer		= {};
		::llc::pcom<ID3D11Buffer>				ConstantBufferScene	= {};
		::llc::pcom<ID3D11Buffer>				ConstantBufferNode	= {};
		::llc::pcom<ID3D11InputLayout>			InputLayout			= {};
		::llc::pcom<ID3D11VertexShader>			VertexShader		= {};
		::llc::pcom<ID3D11PixelShader>			PixelShader			= {};
		::llc::pcom<ID3D11ShaderResourceView>	ShaderResourceView	= {};
		::llc::pcom<ID3D11Texture2D>			Texture2D			= {};	// - Size-dependent

		::llc::apod<::llc::SD3DGUIVertex> ControlVertices;
		::llc::aobj<::llc::SD3DGUIFont> ControlFonts;
		::llc::pcom<ID3D11Buffer> ControlVertexBuffer, ControlConstants;
		::llc::pcom<ID3D11InputLayout> ControlInputLayout;
		::llc::pcom<ID3D11VertexShader> ControlVertexShader;
		::llc::pcom<ID3D11PixelShader> ControlPixelShader;
		::llc::pcom<ID3D11RasterizerState> ControlRasterizerState;
		::llc::pcom<ID3D11BlendState> ControlImageBlendState;
		::llc::pcom<ID3D11Texture2D> ControlImageTexture;
		::llc::pcom<ID3D11ShaderResourceView> ControlImageView;

		void					ReleaseDeviceResources		() {
			BlendState				= {};
			RasterizerState			= {};
			SamplerState			= {};
			DepthStencilState		= {};

			IndexBuffer				= {};
			VertexBuffer			= {};
			ConstantBufferScene		= {};
			ConstantBufferNode		= {};
			InputLayout				= {};
			VertexShader			= {};
			PixelShader				= {};
			ShaderResourceView		= {};
			Texture2D				= {};
			ControlVertices.clear(); ControlFonts.clear();
			ControlVertexBuffer = {}; ControlConstants = {}; ControlInputLayout = {};
			ControlVertexShader = {}; ControlPixelShader = {}; ControlRasterizerState = {};
			ControlImageBlendState = {}; ControlImageTexture = {}; ControlImageView = {};
		}

		::llc::error_t			CreateSizeDependentResources(ID3D11Device3 * d3dDevice, ::llc::n2u1_t windowSize)	{
			llc_necs(RenderTarget.resize(windowSize.u2(), {0, 0, 0, 0}));
			llc_necs(llc::d3dCreateTextureDynamic(d3dDevice, Texture2D, ShaderResourceView, RenderTarget));
			return 0;
		}

		::llc::error_t			CreateDeviceResources		(ID3D11Device3 * d3dDevice)	{
			{		
				::llc::pcom<ID3D11Buffer>	constantBuffer;
				D3D11_BUFFER_DESC			constantBufferDesc			= {sizeof(::llc::SRenderNodeConstants)};
				constantBufferDesc.BindFlags= D3D11_BIND_CONSTANT_BUFFER;
				llc_hrcall(d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer));
				ConstantBufferNode		= constantBuffer;
			}
			{
				::llc::pcom<ID3D11Buffer>	constantBuffer;
				D3D11_BUFFER_DESC			constantBufferDesc			= {sizeof(::llc::SEngineSceneConstants)};
				constantBufferDesc.BindFlags= D3D11_BIND_CONSTANT_BUFFER;
				llc_hrcall(d3dDevice->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer));
				ConstantBufferScene		= constantBuffer;
			}
			{
				// Load shaders asynchronously.
				::llc::pcom<ID3D11InputLayout>	inputLayout	;
				::llc::pcom<ID3D11VertexShader>	vertexShader;

				::llc::vcs					shaderName					= "vsGUI";
				sc_t						shaderFileName	[1024]		= {};
				cnstxpr	D3D11_INPUT_ELEMENT_DESC	vertexDesc []		=
					{ { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 00, D3D11_INPUT_PER_VERTEX_DATA, 0 }
					, { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
					};
				sprintf_s(shaderFileName, "%s.cso", shaderName.begin());

				::llc::as0_t				fileVS;
				llc_necs(llc::fileToMemory(::llc::vcs{shaderFileName}, fileVS));
				// After the vertex shader file is loaded, create the shader and input layout.
				llc_hrcall(d3dDevice->CreateVertexShader(&fileVS[0], fileVS.size(), nullptr, &vertexShader));
				llc_hrcall(d3dDevice->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), &fileVS[0], fileVS.size(), &inputLayout)); 
				InputLayout				= inputLayout;
				VertexShader			= vertexShader;
			}
			{
				::llc::vcst_t					shaderName							= "psGUI";
				sc_t							shaderFileName	[1024]				= {};
				::llc::pcom<ID3D11PixelShader>	pixelShader;
				sprintf_s(shaderFileName, "%s.cso", shaderName.begin());
				::llc::as0_t					filePS;
				llc_necs(llc::fileToMemory(::llc::vcs{shaderFileName}, filePS));

				llc_hrcall(d3dDevice->CreatePixelShader(&filePS[0], filePS.size(), nullptr, &pixelShader));
				PixelShader					= pixelShader;
			}
			{
#pragma pack(push, 1)
				struct SPosUV  {
					::llc::n3f2_t		Position;
					::llc::n2f2_t		UV;
				};
#pragma pack(pop)
				constexpr SPosUV			vertices[4]				= {{{-1, 1}, {0, 0}},{{1, 1}, {1, 0}},{{-1, -1}, {0, 1}}, {{1, -1}, {1, 1}}};

				D3D11_SUBRESOURCE_DATA		vertexBufferData		= {vertices};
				D3D11_BUFFER_DESC			vertexBufferDesc		= {sizeof(SPosUV) * 4};
				vertexBufferDesc.BindFlags	= D3D11_BIND_VERTEX_BUFFER;
				::llc::pcom<ID3D11Buffer>	d3dBuffer;
				llc_hrcall(d3dDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &d3dBuffer));
				VertexBuffer				= d3dBuffer;
			}
			{
				constexpr uint16_t			indices[]							= {0, 1, 2, 1, 3, 2};
				D3D11_SUBRESOURCE_DATA		vertexBufferData					= {indices};
				D3D11_BUFFER_DESC			vertexBufferDesc					= {sizeof(uint16_t) * 6};
				vertexBufferDesc.BindFlags	= D3D11_BIND_INDEX_BUFFER;
				::llc::pcom<ID3D11Buffer>	d3dBuffer;
				llc_hrcall(d3dDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &d3dBuffer));
				IndexBuffer				= d3dBuffer;
			}
			// 3515504948
			{
				// Create a sampler state
				D3D11_SAMPLER_DESC			samDesc					= {};
				samDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_POINT;
				samDesc.AddressU		= D3D11_TEXTURE_ADDRESS_CLAMP;
				samDesc.AddressV		= D3D11_TEXTURE_ADDRESS_CLAMP;
				samDesc.AddressW		= D3D11_TEXTURE_ADDRESS_CLAMP;
				samDesc.MaxAnisotropy	= 1;
				samDesc.ComparisonFunc	= D3D11_COMPARISON_ALWAYS;
				samDesc.MaxLOD			= D3D11_FLOAT32_MAX;

				::llc::pcom<ID3D11SamplerState>	samplerState;
				llc_hrcall(d3dDevice->CreateSamplerState(&samDesc, &samplerState));
				SamplerState			= samplerState;
			}
			{ // straight
				D3D11_BLEND_DESC			blendDesc				= {};
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
				BlendState					= blendState;
			}
			{
				D3D11_RASTERIZER_DESC		rsDesc					= {};
				rsDesc.FillMode			= D3D11_FILL_SOLID;
				rsDesc.CullMode			= D3D11_CULL_BACK;
				rsDesc.DepthClipEnable	= false;
				::llc::pcom<ID3D11RasterizerState>	rasterizerState;
				llc_hrcall(d3dDevice->CreateRasterizerState(&rsDesc, &rasterizerState));
				RasterizerState			= rasterizerState;
			}
			//{
			//	D3D11_RASTERIZER_DESC		rsDesc					= {};
			//	rsDesc.FillMode			= D3D11_FILL_WIREFRAME;
			//	rsDesc.CullMode			= D3D11_CULL_BACK;
			//	rsDesc.DepthClipEnable	= false;
			//	::llc::pcom<ID3D11RasterizerState>	rasterizerState;
			//	llc_hrcall(d3dDevice->CreateRasterizerState(&rsDesc, &rasterizerState));
			//	RasterizerState			= rasterizerState;
			//}
			{ // disabled
				D3D11_DEPTH_STENCIL_DESC	depthStencilDesc				= {};
				depthStencilDesc.DepthEnable		= FALSE;
				depthStencilDesc.DepthWriteMask		= D3D11_DEPTH_WRITE_MASK_ALL;
				depthStencilDesc.DepthFunc			= D3D11_COMPARISON_LESS;
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
				DepthStencilState				= depthStencilState;
			}
			return 0;
		}

	};

	::llc::error_t			d3dGUIDraw				(::DX::D3DDeviceResources & d3dResources, ::llc::SD3DGUIResources<::llc::bgra> & guiStuff);
	::llc::err_t d3dGUIDraw(::DX::D3DDeviceResources & resources, ::llc::SD3DGUIResources<::llc::bgra> & gpu, const ::llc::SGUI & gui, ::llc::n2u1_t targetSize);

	struct SD3DApplication : DX::IDeviceNotify {
		::llc::pobj<::llc::SEngineGraphics>		EngineGraphics;

		::llc::pobj<::DX::D3DDeviceResources>	DeviceResources			;
		::llc::Sample3DSceneRenderer			Scene					;
		::llc::SampleFpsTextRenderer			Text					;
		::llc::SD3DGUIResources<::llc::bgra>	GUIStuff				;
	
		virtual void			OnDeviceLost			() { return ReleaseDeviceResources(); }
		virtual ::llc::error_t	OnDeviceRestored		() { return CreateDeviceResources(*EngineGraphics); }

		::llc::error_t			Shutdown				()	{
			DeviceResources->m_swapChain->SetFullscreenState(FALSE, 0);
			ReleaseDeviceResources();
			Scene			= {};
			Text			= {};
			GUIStuff		= {};
			DeviceResources	= {};
			return 0;
		}
		::llc::error_t			Initialize				(HWND hWnd, const ::llc::pobj<::llc::SEngineGraphics> & engineGraphics)	{
			EngineGraphics			= engineGraphics;

			llc_necs(DeviceResources->Initialize());
			llc_necs(DeviceResources->SetWindow(hWnd));
			DeviceResources->RegisterDeviceNotify(this);
			llc_necs(Scene.Initialize(DeviceResources));
			llc_necs(Text .Initialize(DeviceResources));
			return 0;
		}

		// Notifies renderers that device resources need to be released.
		void					ReleaseDeviceResources	()	{
			Scene		.ReleaseDeviceResources();
			Text		.ReleaseDeviceResources();
			GUIStuff	.ReleaseDeviceResources();
		}
		// Notifies renderers that device resources may now be recreated.

		::llc::error_t			CreateDeviceDependentEngineResources(ID3D11Device3 * d3dDevice,  const ::llc::SEngineGraphics & engineGraphics)	{
			llc_necs(llc::d3dCreateBuffersFromEngineMeshes		(d3dDevice, engineGraphics.Meshes	, engineGraphics.Buffers, Scene.IndexBuffer, Scene.VertexBuffer));
			llc_necs(llc::d3dCreateTexturesFromEngineSurfaces		(d3dDevice, engineGraphics.Surfaces	, Scene.ShaderResourceView));
			llc_necs(llc::d3dCreatePixelShadersFromEngineShaders	(d3dDevice, engineGraphics.Shaders	, Scene.PixelShader, &Scene.LightSlots));
			return 0;
		}
		::llc::error_t			CreateDeviceResources	(const ::llc::SEngineGraphics & engineGraphics)	{
			ID3D11Device3				* pd3dDevice			= DeviceResources->GetD3DDevice();
			llc_necs(Scene		.CreateDeviceResources(pd3dDevice));
			llc_necs(Text		.CreateDeviceResources(DeviceResources->GetD2DDeviceContext()));
			llc_necs(GUIStuff	.CreateDeviceResources(pd3dDevice));
			llc_necs(CreateDeviceDependentEngineResources(pd3dDevice, engineGraphics));
			return 0; 
		}

		::llc::error_t			CreateSizeDependentResources(::llc::n2u1_t windowSize)	{
			llc_necs(Scene		.CreateSizeDependentResources(DeviceResources->GetOutputSize(), DeviceResources->GetOrientationTransform3D()));
			llc_necs(GUIStuff	.CreateSizeDependentResources(DeviceResources->GetD3DDevice(), windowSize));
			return 0;
		}

		::llc::error_t			SetWindowSize			(::llc::n2u1_t windowSize)	{
			llc_necs(DeviceResources->SetLogicalSize(windowSize.f2()));
			llc_necs(CreateSizeDependentResources(windowSize));
			return 0;
		}
	};

	::llc::err_t			d3dDrawEngineNode		(::llc::Sample3DSceneRenderer & renderer, const ::llc::SEngineScene & scene, ::llc::u2_t node, ::llc::view<const ::llc::SRenderLight> lights, const ::llc::SRenderMaterial * material = nullptr, ::llc::u2_t vertexShader = 0, ::llc::CULL_MODE cullMode = ::llc::CULL_MODE_CCW);
	::llc::error_t			d3dDrawEngineScene		(::llc::Sample3DSceneRenderer & d3dScene, const ::llc::SEngineScene & engineScene, const ::llc::n2u1_t & targetMetrics, const ::llc::n3f2_t & lightPos, const ::llc::n3f2_t & cameraPosition, const llc::n3f2_t & cameraTarget, const llc::minmaxf2_t & nearFar, ::llc::f3_t time = 0);
	::llc::error_t			d3dAppDraw				(::llc::SD3DApplication & d3dApp, const ::llc::SEngineScene & engineScene, const ::llc::rgbaf & clearColor, const ::llc::n3f2_t & lightPos, const ::llc::n3f2_t & cameraPosition, const llc::n3f2_t & cameraTarget, const llc::minmaxf2_t & nearFar, ::llc::f3_t time = 0);
};

#endif // LLC_ENGINE_D3D_APP_H_28903749823
