#include "llc_engine_scene.h"
#include "llc_complus.h"

#if defined(LLC_WINDOWS)

#include <d3d11.h>
#include <dxgi.h>

#ifndef LLC_ENGINE_D3D_H_23627
#define LLC_ENGINE_D3D_H_23627

namespace llc {
	struct SD3DMeshResources {
		::llc::pcom<ID3D11Buffer>	VertexBuffer	= {};
		::llc::pcom<ID3D11Buffer>	IndexBuffer	= {};
		DXGI_FORMAT					IndexFormat	= DXGI_FORMAT_R16_UINT;
		uint32_t					IndexCount	= 0;
	};

	struct SD3DApplication {
		HWND							WindowHandle		= {};
		::llc::n2u1_t				WindowSize		= {};
		::llc::pcom<ID3D11Device>		Device			= {};
		::llc::pcom<ID3D11DeviceContext>	Context			= {};
		::llc::pcom<IDXGISwapChain>		SwapChain		= {};
		::llc::pcom<ID3D11RenderTargetView>	RenderTarget		= {};
		::llc::pcom<ID3D11Texture2D>		DepthTexture		= {};
		::llc::pcom<ID3D11DepthStencilView>	DepthStencil		= {};

		::llc::aobj<::llc::SD3DMeshResources>	Meshes			= {};
		::llc::acom<ID3D11ShaderResourceView>	Textures		= {};
		::llc::acom<ID3D11PixelShader>		PixelShaders		= {};
		::llc::pcom<ID3D11VertexShader>		VertexShader		= {};
		::llc::pcom<ID3D11InputLayout>		InputLayout		= {};
		::llc::pcom<ID3D11Buffer>			ConstantNode		= {};
		::llc::pcom<ID3D11Buffer>			ConstantScene		= {};
		::llc::pcom<ID3D11SamplerState>		SamplerLinear		= {};
		::llc::pcom<ID3D11BlendState>		BlendOpaque		= {};
		::llc::pcom<ID3D11BlendState>		BlendAlpha		= {};
		::llc::pcom<ID3D11RasterizerState>	Rasterizer		= {};
		::llc::pcom<ID3D11DepthStencilState>	DepthOpaque		= {};
		::llc::pcom<ID3D11DepthStencilState>	DepthAlpha		= {};

		::llc::error_t	Initialize	(HWND windowHandle, ::llc::n2u1_t windowSize, const ::llc::SEngineGraphics & graphics);
		::llc::error_t	Resize		(::llc::n2u1_t windowSize);
		::llc::error_t	Refresh		(const ::llc::SEngineGraphics & graphics);
		::llc::error_t	Draw		(const ::llc::SEngineScene & scene, const ::llc::n3f2_t & cameraPosition, const ::llc::n3f2_t & cameraTarget, const ::llc::n3f2_t & cameraUp, const ::llc::minmaxf2_t & nearFar, ::llc::f3_t time);
		void			Shutdown	();
	};
}

#endif // LLC_ENGINE_D3D_H_23627
#endif // LLC_WINDOWS
