#include "llc_test_core.h"
#include "llc_d3d.h"
#include "llc_d3d_device_resources.h"
#include "llc_windows.h"

#if defined(LLC_WINDOWS)

#include <dxgi1_4.h>
#include <cstring>

GDEFINE_ENUM_TYPE(D3D_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, DEVICE_VALIDATE			,  0, "d3dDeviceValidate() disagreed with the device and current-default adapter state.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, TEXTURE_RESULT				,  1, "d3dCreateTextureDynamic() rejected a valid 2x2 BGRA grid.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, TEXTURE_POINTER			,  2, "d3dCreateTextureDynamic() did not return a texture.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, VIEW_POINTER				,  3, "d3dCreateTextureDynamic() did not return a shader-resource view.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, TEXTURE_METRICS			,  4, "The dynamic texture has the wrong dimensions.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, TEXTURE_FORMAT				,  5, "The dynamic texture has the wrong format.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, TEXTURE_USAGE				,  6, "The texture does not have dynamic shader-resource and CPU-write usage.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, VIEW_FORMAT				,  7, "The shader-resource view has the wrong format or dimension.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, PIXEL_ROW_0				,  8, "The first uploaded BGRA row differs from the input.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, PIXEL_ROW_1				,  9, "The second uploaded BGRA row differs from the input.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, EMPTY_REJECTED				, 10, "An empty source grid was accepted as a texture.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, FAILURE_TEXTURE_PRESERVED	, 11, "A rejected texture creation replaced the existing texture.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, FAILURE_VIEW_PRESERVED		, 12, "A rejected texture creation replaced the existing shader-resource view.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, DEVICE_INIT				, 13, "The consumer could not initialize its D3D/D2D device resources.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, WINDOW_CREATE				, 14, "The consumer test window could not be created.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, WINDOW_RESOURCES			, 15, "The consumer could not create its window-sized resources.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, HARDWARE_SELECTED			, 16, "The consumer fell back to WARP although a hardware D3D device is available.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, DEVICE_PRESERVED			, 17, "Validation rebuilt a healthy consumer device.");

::llc::err_t testD3D(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	struct SWindowGuard { HWND Handle = {}; ~SWindowGuard() { if(Handle) ::DestroyWindow(Handle); } } window;
	struct SComGuard { HRESULT Result = ::CoInitializeEx(nullptr, COINIT_MULTITHREADED); ~SComGuard() { if(SUCCEEDED(Result)) ::CoUninitialize(); } } com;
	llc_hrcall(com.Result);
	window.Handle = ::CreateWindowExW(0, L"STATIC", L"LLC D3D consumer test", WS_OVERLAPPEDWINDOW, 0, 0, 128, 96, nullptr, nullptr, ::GetModuleHandleW(nullptr), nullptr);
	LLC_TEST_REQUIRE(errors, D3D_TEST_RESULT_WINDOW_CREATE, nullptr == window.Handle);
	::DX::D3DDeviceResources			resources;
	LLC_TEST_REQUIRE(errors, D3D_TEST_RESULT_DEVICE_INIT, ::llc::failed(resources.Initialize()));
	LLC_TEST_REQUIRE(errors, D3D_TEST_RESULT_WINDOW_RESOURCES, ::llc::failed(resources.SetWindow(window.Handle)) || nullptr == resources.GetSwapChain() || nullptr == resources.GetBackBufferRenderTargetView() || nullptr == resources.GetDepthStencilView());
	::llc::pcom<ID3D11Device3>		device3;
	device3 = resources.GetD3DDevice();
	::llc::pcom<ID3D11Device>			device;
	if_fail_fe(device3.as(device));
	ID3D11DeviceContext3			* context = resources.GetD3DDeviceContext();
	::llc::pcom<ID3D11Device>			hardwareProbe;
	cnst bool hardwareAvailable = SUCCEEDED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, nullptr, 0, D3D11_SDK_VERSION, &hardwareProbe, nullptr, nullptr));

	::llc::pcom<IDXGIDevice3>		dxgiDevice;
	::llc::pcom<IDXGIAdapter>		adapter;
	::llc::pcom<IDXGIFactory4>		previousFactory;
	::llc::pcom<IDXGIAdapter1>		previousDefaultAdapter;
	::llc::pcom<IDXGIFactory4>		factory;
	::llc::pcom<IDXGIAdapter1>		defaultAdapter;
	DXGI_ADAPTER_DESC1			previousDesc	= {};
	DXGI_ADAPTER_DESC1			defaultDesc	= {};
	if_fail_fe(device3.as(dxgiDevice));
	llc_hrcall(dxgiDevice->GetAdapter(&adapter));
	::llc::pcom<IDXGIAdapter1>		deviceAdapter1;
	if_fail_fe(adapter.as(deviceAdapter1));
	DXGI_ADAPTER_DESC1			deviceDesc = {};
	llc_hrcall(deviceAdapter1->GetDesc1(&deviceDesc));
	LLC_TEST_CHECK(errors, D3D_TEST_RESULT_HARDWARE_SELECTED, hardwareAvailable && (deviceDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE));
	llc_hrcall(adapter->GetParent(IID_PPV_ARGS(&previousFactory)));
	llc_hrcall(previousFactory->EnumAdapters1(0, &previousDefaultAdapter));
	llc_hrcall(previousDefaultAdapter->GetDesc1(&previousDesc));
	llc_hrcall(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));
	llc_hrcall(factory->EnumAdapters1(0, &defaultAdapter));
	llc_hrcall(defaultAdapter->GetDesc1(&defaultDesc));
	cnst bool sameAdapter = 0 == ::memcmp(&previousDesc.AdapterLuid, &defaultDesc.AdapterLuid, szof(LUID));
	cnst ::llc::err_t expectedValidation = sameAdapter && SUCCEEDED(device3->GetDeviceRemovedReason()) ? 0 : 1;
	ID3D11Device3* healthyDevice = resources.GetD3DDevice();
	cnst ::llc::err_t validation = resources.ValidateDevice();
	LLC_TEST_CHECKF(errors, D3D_TEST_RESULT_DEVICE_VALIDATE, validation != expectedValidation, "Result:%i, expected:%i, same adapter:%u.", validation, expectedValidation, sameAdapter);
	LLC_TEST_CHECK(errors, D3D_TEST_RESULT_DEVICE_PRESERVED, expectedValidation == 0 && resources.GetD3DDevice() != healthyDevice);

	cnst ::llc::bgra pixels[4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
	::llc::pcom<ID3D11Texture2D>		texture;
	::llc::pcom<ID3D11ShaderResourceView>	view;
	cnst ::llc::err_t result = ::llc::d3dCreateTextureDynamic(device.get(), texture, view, {pixels, 2, 2});
	LLC_TEST_REQUIRE(errors, D3D_TEST_RESULT_TEXTURE_RESULT, ::llc::failed(result));
	LLC_TEST_REQUIRE(errors, D3D_TEST_RESULT_TEXTURE_POINTER, nullptr == texture.get());
	LLC_TEST_REQUIRE(errors, D3D_TEST_RESULT_VIEW_POINTER, nullptr == view.get());
	D3D11_TEXTURE2D_DESC textureDesc = {};
	texture->GetDesc(&textureDesc);
	LLC_TEST_CHECKF(errors, D3D_TEST_RESULT_TEXTURE_METRICS, textureDesc.Width != 2 || textureDesc.Height != 2 || textureDesc.MipLevels != 1 || textureDesc.ArraySize != 1, "Size:%u x %u, mip levels:%u, array size:%u.", textureDesc.Width, textureDesc.Height, textureDesc.MipLevels, textureDesc.ArraySize);
	LLC_TEST_CHECKF(errors, D3D_TEST_RESULT_TEXTURE_FORMAT, textureDesc.Format != DXGI_FORMAT_B8G8R8A8_UNORM, "Format:%u.", textureDesc.Format);
	LLC_TEST_CHECKF(errors, D3D_TEST_RESULT_TEXTURE_USAGE, textureDesc.Usage != D3D11_USAGE_DYNAMIC || textureDesc.BindFlags != D3D11_BIND_SHADER_RESOURCE || textureDesc.CPUAccessFlags != D3D11_CPU_ACCESS_WRITE, "Usage:%u, bind:%u, CPU access:%u.", textureDesc.Usage, textureDesc.BindFlags, textureDesc.CPUAccessFlags);
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
	view->GetDesc(&viewDesc);
	LLC_TEST_CHECKF(errors, D3D_TEST_RESULT_VIEW_FORMAT, viewDesc.Format != DXGI_FORMAT_B8G8R8A8_UNORM || viewDesc.ViewDimension != D3D_SRV_DIMENSION_TEXTURE2D || viewDesc.Texture2D.MipLevels != 1, "Format:%u, dimension:%u, mip levels:%u.", viewDesc.Format, viewDesc.ViewDimension, viewDesc.Texture2D.MipLevels);

	D3D11_TEXTURE2D_DESC stagingDesc = textureDesc;
	stagingDesc.Usage = D3D11_USAGE_STAGING;
	stagingDesc.BindFlags = 0;
	stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	::llc::pcom<ID3D11Texture2D> staging;
	llc_hrcall(device->CreateTexture2D(&stagingDesc, nullptr, &staging));
	context->CopyResource(staging.get(), texture.get());
	D3D11_MAPPED_SUBRESOURCE mapped = {};
	llc_hrcall(context->Map(staging.get(), 0, D3D11_MAP_READ, 0, &mapped));
	cnst bool row0Differs = 0 != ::memcmp(mapped.pData, pixels, 2 * szof(::llc::bgra));
	cnst bool row1Differs = 0 != ::memcmp((cnst ::llc::u0_t*)mapped.pData + mapped.RowPitch, pixels + 2, 2 * szof(::llc::bgra));
	context->Unmap(staging.get(), 0);
	LLC_TEST_CHECK(errors, D3D_TEST_RESULT_PIXEL_ROW_0, row0Differs);
	LLC_TEST_CHECK(errors, D3D_TEST_RESULT_PIXEL_ROW_1, row1Differs);

	ID3D11Texture2D* previousTexture = texture.get();
	ID3D11ShaderResourceView* previousView = view.get();
	cnst ::llc::err_t emptyResult = ::llc::d3dCreateTextureDynamic(device.get(), texture, view, {});
	LLC_TEST_CHECKF(errors, D3D_TEST_RESULT_EMPTY_REJECTED, false == ::llc::failed(emptyResult), "Result:%i.", emptyResult);
	LLC_TEST_CHECKF(errors, D3D_TEST_RESULT_FAILURE_TEXTURE_PRESERVED, texture.get() != previousTexture, "Texture:%p, expected:%p.", texture.get(), previousTexture);
	LLC_TEST_CHECKF(errors, D3D_TEST_RESULT_FAILURE_VIEW_PRESERVED, view.get() != previousView, "View:%p, expected:%p.", view.get(), previousView);
	rtrn 0;
}

#else

::llc::err_t testD3D(::llc::ATestError &) { rtrn 0; }

#endif
