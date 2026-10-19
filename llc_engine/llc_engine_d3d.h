#include "llc_engine_rendermesh.h"
#include "llc_engine_rendersurface.h"
#include "llc_engine_renderbuffer.h"
#include "llc_engine_scene.h"

#if defined(LLC_WINDOWS)

#include "llc_complus.h"

#include <d3d11_3.h>

#ifndef LLC_ENGINE_D3D_H_23627
#define LLC_ENGINE_D3D_H_23627

namespace llc
{

	::llc::error_t		d3dCreateBuffersFromEngineMeshes		(ID3D11Device* pDevice, const ::llc::SMeshManager & engineMeshes, const ::llc::SRenderBufferManager & engineBuffers, ::llc::array_com<ID3D11Buffer> & indexBuffers, ::llc::array_com<ID3D11Buffer> & vertexBuffers);
	::llc::error_t		d3dCreateTexturesFromEngineSurfaces		(ID3D11Device* pDevice, const ::llc::SSurfaceManager & engineSurfaces, ::llc::array_com<ID3D11ShaderResourceView> & shaderResourceViews);
	::llc::error_t		d3dCreatePixelShadersFromEngineShaders	(ID3D11Device* pDevice, const ::llc::SShaderManager & engineShaders, ::llc::array_com<ID3D11PixelShader> & pixelShaders);
} // namespace

#endif // LLC_ENGINE_D3D_H_23627

#endif // LLC_WINDOWS
