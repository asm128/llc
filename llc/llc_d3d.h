#include "llc_grid_color.h"

#if defined(LLC_WINDOWS)

#include "llc_complus.h"

#include <d3d11_3.h>

#ifndef LLC_D3D_H_23627
#define LLC_D3D_H_23627

namespace llc
{
	::llc::error_t	d3dDeviceValidate		(::llc::pcom<::ID3D11Device3> & d3dDevice);
	::llc::error_t	d3dCreateTextureDynamic	(ID3D11Device* pDevice, ::llc::pcom<ID3D11Texture2D> & texture, ::llc::pcom<ID3D11ShaderResourceView> & shaderResourceView, ::llc::gc8bgra initData); 
} // namespace

#endif // LLC_D3D_H_23627

#endif // LLC_WINDOWS