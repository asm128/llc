#include "llc_d3d.h"

#if defined(LLC_WINDOWS)

#include <dxgi1_4.h>

::llc::error_t			llc::d3dDeviceValidate				(::llc::pcom<::ID3D11Device3> & d3dDevice)				{
	// The D3D Device is no longer valid if the default adapter changed since the device was created or if the device has been removed. 
	::llc::pcom<IDXGIDevice3>		dxgiDevice;
	::llc::pcom<IDXGIAdapter>		deviceAdapter;
	::llc::pcom<IDXGIFactory4>		deviceFactory;
	::llc::pcom<IDXGIAdapter1>		previousDefaultAdapter;
	::llc::pcom<IDXGIFactory4>		currentFactory;
	::llc::pcom<IDXGIAdapter1>		currentDefaultAdapter;
	DXGI_ADAPTER_DESC1				previousDesc				= {};	// First, get the information for the default adapter from when the device was created.
	DXGI_ADAPTER_DESC1				currentDesc					= {};	// Next, get the information for the current default adapter.

	d3dDevice.as(dxgiDevice);
	llc_hrcall(dxgiDevice->GetAdapter(&deviceAdapter));
	llc_hrcall(deviceAdapter->GetParent(IID_PPV_ARGS(&deviceFactory)));
	llc_hrcall(deviceFactory->EnumAdapters1(0, &previousDefaultAdapter));
	llc_hrcall(previousDefaultAdapter->GetDesc1(&previousDesc));

	llc_hrcall(CreateDXGIFactory1(IID_PPV_ARGS(&currentFactory)));
	llc_hrcall(currentFactory->EnumAdapters1(0, &currentDefaultAdapter));
	llc_hrcall(currentDefaultAdapter->GetDesc1(&currentDesc));

	// If the adapter LUIDs don't match, or if the device reports that it has been removed, a new D3D device must be created. Release references to resources related to the old device.
	if (0 == memcmp(&previousDesc.AdapterLuid, &currentDesc.AdapterLuid, sizeof(LUID)) && !FAILED(d3dDevice->GetDeviceRemovedReason())) 
		return 0;

	return 1;
}


::llc::error_t			llc::d3dCreateTextureDynamic		(ID3D11Device* pDevice, ::llc::pcom<ID3D11Texture2D> & texture, ::llc::pcom<ID3D11ShaderResourceView> & shaderResourceView, ::llc::gc8bgra initData) {
	D3D11_TEXTURE2D_DESC					desc				= {initData.metrics().x, initData.metrics().y, 1, 1, DXGI_FORMAT_B8G8R8A8_UNORM, {1, 0}, D3D11_USAGE_DYNAMIC, D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE};
	D3D11_SUBRESOURCE_DATA					d3dInitData			= {};
	d3dInitData.pSysMem					= initData.begin();
	d3dInitData.SysMemPitch				= initData.metrics().x * 4;

	::llc::pcom<ID3D11Texture2D>			pTex2D				= {};
	llc_hrcall(pDevice->CreateTexture2D(&desc, &d3dInitData, &pTex2D));
	if(!pTex2D) 
		return -1;

	D3D11_SHADER_RESOURCE_VIEW_DESC			srvDesc				= {};
	srvDesc.Format						= desc.Format;
	srvDesc.ViewDimension				= D3D_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels			= desc.MipLevels;
	//srvDesc.

	::llc::pcom<ID3D11ShaderResourceView>	pSRV;
	llc_hrcall(pDevice->CreateShaderResourceView(pTex2D, &srvDesc, &pSRV));

	texture									= pTex2D;
	shaderResourceView						= pSRV;
	return 0;
}

#endif