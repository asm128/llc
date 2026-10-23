#include "llc_test_core.h"
#include "llc_d3d.h"
#include "llc_d3d_device_resources.h"
#include "llc_windows.h"

#if defined(LLC_WINDOWS)

#include "llc_engine_d3d_app.h"
#include "llc_engine_gui_d3d.h"
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
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, LIGHT_SHADER				, 18, "The renderer must discover the light resource slot from the compiled shader.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, LIGHT_PIXELS				, 19, "The GPU must shade each pixel using all selected lights, including after the list grows or empties.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, SCENE_DEPTH				, 20, "Readback must preserve scene depth in the software rasterizer's units.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, TARGET_RESIZE				, 21, "The offscreen target must recreate color and depth resources at the requested size.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, VERTEX_SCENE				, 22, "The vertex shader must receive the scene constants at b1.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, OVERLAY_PIXELS				, 23, "The small overlay must preserve opaque, transparent and 20-percent label pixels.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, OVERLAY_DEPTH				, 24, "The overlay must leave ship depth unchanged for later geometry.");

GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, GUI_MESH_PIXELS, 25, "GUI mesh pixels must match CPU geometry, palettes, images and bitmap text.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, GUI_MESH_DEPTH, 26, "GUI geometry must preserve scene depth.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, GUI_MESH_REUSE, 27, "Drawing unchanged GUI geometry must reuse its GPU buffers.");

static ::llc::err_t testGUIMesh(::llc::ATestError & errors, ::llc::pobj<::DX::D3DDeviceResources> & resources) {
	::llc::SGUI gui;
	::llc::cid_t parent, child;
	llc_necs(parent = ::llc::controlCreate(gui));
	llc_necs(child = ::llc::controlCreateChild(gui, parent));
	gui.Controls.Placement[parent].Area = {{3, 3}, {45, 32}};
	gui.Controls.Placement[parent].Border = {2, 3, 4, 2};
	gui.Controls.Placement[parent].Margin = {1, 1, 1, 1};
	gui.Controls.Placement[child].Area = {{9, 8}, {24, 16}};
	gui.Controls.Text[parent].Text = "Parent";
	gui.Controls.Text[child].Text = "AB";
	::llc::img8bgra image;
	llc_necs(image.resize(::llc::n2u2_t{4, 4}));
	for(::llc::u2_t i = 0; i < 16; ++i) image.Texels[i] = {(::llc::u0_t)(i * 10), 120, 200, (::llc::u0_t)(i % 3 == 0 ? 0 : i % 3 == 1 ? 128 : 255)};
	gui.Controls.Images[child].Image = image;
	::llc::SGUIMesh mesh;
	::llc::SD3DGUIMeshResources renderer;
	::llc::SD3DRenderTarget target;
	::llc::rt<::llc::bgra, ::llc::u2_t> gpu;
	::llc::img8bgra cpu;
	for(::llc::u2_t pass = 0; pass < 8; ++pass) {
		const ::llc::n2u2_t size = pass < 6 ? ::llc::n2u2_t{64, 48} : ::llc::n2u2_t{47, 35};
		llc_necs(cpu.resize(size, {13, 27, 39, 255})); cpu.View.fill({13, 27, 39, 255});
		llc_necs(gpu.resize(size, {13, 27, 39, 255}, 0xFFFFFFFFU)); gpu.Color.View.fill({13, 27, 39, 255});
		gui.Controls.States[parent].Flag = pass == 1 ? ::llc::GUI_CONTROL_FLAG_Hovered : pass == 2 ? ::llc::GUI_CONTROL_FLAG_Pressed : pass == 3 ? ::llc::GUI_CONTROL_FLAG_Disabled : ::llc::GUI_CONTROL_FLAG{};
		gui.Controls.Draw[parent].NoBorder = pass == 4;
		gui.Controls.Draw[parent].NoClient = pass == 5;
		gui.Controls.Images[child].ImageInvertY = pass % 2;
		if(pass == 7) gui.Controls.Placement[parent].Area.Offset = {-4, -5};
		llc_necs(::llc::guiUpdateMetrics(gui, size.u1(), true));
		llc_necs(::llc::guiDraw(gui, cpu.View));
		llc_necs(mesh.Clear());
		llc_necs(::llc::guiBuildMesh(gui, size, mesh));
		llc_necs(target.Begin(*resources, gpu.Color));
		llc_necs(::llc::d3dGUIDraw(*resources, renderer, mesh, size));
		ID3D11Buffer * vertices = renderer.Pipeline.VertexBuffer.get(), * indices = renderer.Pipeline.IndexBuffer.get();
		llc_necs(target.Read(*resources, gpu.Color, gpu.DepthStencil));
		::llc::u2_t different = 0, first = 0;
		for(::llc::u2_t i = 0; i < cpu.Texels.size(); ++i) {
			const auto a = cpu.Texels[i], b = gpu.Color.Texels[i];
			if(::llc::abs((int)a.r - b.r) > 1 || ::llc::abs((int)a.g - b.g) > 1 || ::llc::abs((int)a.b - b.b) > 1 || ::llc::abs((int)a.a - b.a) > 1) { if(0 == different) first = i; ++different; }
		}
		const auto a = cpu.Texels[first], b = gpu.Color.Texels[first];
		LLC_TEST_CHECKF(errors, D3D_TEST_RESULT_GUI_MESH_PIXELS, different != 0, "Case:%u, differing:%u, first:{%u,%u}, CPU:%u/%u/%u/%u, GPU:%u/%u/%u/%u.", pass, different, first % size.x, first / size.x, a.r, a.g, a.b, a.a, b.r, b.g, b.b, b.a);
		bool depthChanged = false;
		for(auto depth : gpu.DepthStencil.Texels) depthChanged |= depth != 0xFFFFFFFFU;
		LLC_TEST_CHECK(errors, D3D_TEST_RESULT_GUI_MESH_DEPTH, depthChanged);
		llc_necs(target.Begin(*resources, gpu.Color));
		llc_necs(::llc::d3dGUIDraw(*resources, renderer, mesh, size));
		LLC_TEST_CHECK(errors, D3D_TEST_RESULT_GUI_MESH_REUSE, vertices != renderer.Pipeline.VertexBuffer.get() || indices != renderer.Pipeline.IndexBuffer.get());
	}
	return 0;
}
sttc ::llc::err_t testD3DLights(::llc::ATestError & errors, ::llc::pobj<::DX::D3DDeviceResources> & resources) {
	::llc::SGeometryBuffers geometry;
	llc_necs(geometry.Positions.append({{-1, 1, .5f}, {1, 1, .5f}, {-1, -1, .5f}, {1, -1, .5f}}));
	llc_necs(geometry.Normals.resize(4, {0, 0, -1}));
	llc_necs(geometry.TextureCoords.resize(4));
	llc_necs(geometry.PositionIndices.append({0, 1, 2, 1, 3, 2}));
	::llc::SEngineScene scene;
	llc_necs(scene.CreateRenderNode(geometry, "Lighting probe", false));
	llc_necs(scene.Graphics->Skins.Create());
	if_null_fe(scene.Graphics->Skins[0].create());
	llc_necs(scene.Graphics->Shaders.Create());
	scene.Graphics->Shaders.Names[0] = "psTestLights";
	scene.RenderNodes[0].Skin = scene.RenderNodes[0].Shader = 0;
	::llc::Sample3DSceneRenderer renderer;
	llc_necs(renderer.Initialize(resources));
	llc_necs(::llc::d3dCreateBuffersFromEngineMeshes(resources->GetD3DDevice(), scene.Graphics->Meshes, scene.Graphics->Buffers, renderer.IndexBuffer, renderer.VertexBuffer));
	llc_necs(::llc::d3dCreatePixelShadersFromEngineShaders(resources->GetD3DDevice(), scene.Graphics->Shaders, renderer.PixelShader, &renderer.LightSlots));
	LLC_TEST_REQUIRE(errors, D3D_TEST_RESULT_LIGHT_SHADER, renderer.LightSlots.size() != 1 || renderer.LightSlots[0] != 2);
	renderer.ConstantBufferScene.View.Identity();
	renderer.ConstantBufferScene.Perspective.Identity();
	::llc::SD3DRenderTarget target;
	::llc::rt<::llc::bgra, ::llc::u2_t> pixels;
	llc_necs(pixels.resize(::llc::n2u2_t{32, 24}));
	::llc::apod<::llc::SRenderLight> lights;
	const ::llc::u2_t counts[] = {0, 1, 2, 257, 0};
	for(::llc::u2_t count : counts) {
		::llc::SRenderLight light;
		light.Color.Diffuse = {1.0f / 1024, 0, 0, 1};
		light.Range = 1;
		llc_necs(lights.resize(count, light));
		if(count > 256) lights[256].Color.Diffuse.r = .5f;
		llc_necs(target.Begin(*resources, pixels.Color));
		llc_necs(::llc::d3dDrawEngineNode(renderer, scene, 0, lights));
		::llc::pcom<ID3D11Buffer> vertexScene;
		resources->GetD3DDeviceContext()->VSGetConstantBuffers(1, 1, &vertexScene);
		LLC_TEST_CHECK(errors, D3D_TEST_RESULT_VERTEX_SCENE, vertexScene.get() != renderer.ConstantBuffer[1].get());
		llc_necs(target.Read(*resources, pixels.Color, pixels.DepthStencil));
		for(::llc::u2_t x : {8U, 16U}) {
			const float expected = (count * (1.0f / 1024) + (count > 256 ? .5f - 1.0f / 1024 : 0)) * (1 - (x + .5f) / 32) * 255;
			const ::llc::bgra actual = pixels.Color[12][x];
			LLC_TEST_CHECKF(errors, D3D_TEST_RESULT_LIGHT_PIXELS, actual.r < expected - 1 || actual.r > expected + 1 || actual.g || actual.b, "Lights:%u, pixel:%u, expected red:%f, actual:%u/%u/%u.", count, x, expected, actual.r, actual.g, actual.b);
			LLC_TEST_CHECK(errors, D3D_TEST_RESULT_SCENE_DEPTH, pixels.DepthStencil[12][x] != (::llc::u2_t)(0xFFFFFFFU * .5));
		}
	}
	::llc::SD3DGUIResources<::llc::bgra> label;
	llc_necs(label.CreateDeviceResources(resources->GetD3DDevice()));
	llc_necs(label.CreateSizeDependentResources(resources->GetD3DDevice(), {2, 2}));
	label.RenderTarget[0][0] = {0, 200, 0, 255}; label.RenderTarget[0][1] = {255, 255, 255, 0};
	label.RenderTarget[1][0] = {0, 200, 0, 51};  label.RenderTarget[1][1] = {0, 0, 200, 255};
	auto context = resources->GetD3DDeviceContext();
	const D3D11_VIEWPORT labelViewport = {8, 12, 2, 2, 0, 1}, fullViewport = {0, 0, 32, 24, 0, 1};
	context->RSSetViewports(1, &labelViewport);
	llc_necs(::llc::d3dGUIDraw(*resources, label));
	context->RSSetViewports(1, &fullViewport);
	llc_necs(target.Read(*resources, pixels.Color, pixels.DepthStencil));
	LLC_TEST_CHECK(errors, D3D_TEST_RESULT_OVERLAY_PIXELS, pixels.Color[12][8].g != 200 || pixels.Color[12][9].r || pixels.Color[12][9].g || pixels.Color[12][9].b || pixels.Color[13][8].g != 40 || pixels.Color[13][9].r != 200);
	LLC_TEST_CHECK(errors, D3D_TEST_RESULT_OVERLAY_DEPTH, pixels.DepthStencil[12][8] != (::llc::u2_t)(0xFFFFFFFU * .5) || pixels.DepthStencil[13][9] != pixels.DepthStencil[12][8]);
	for(float z : {.25f, -.25f}) {
		scene.RenderNodes.Transforms[0].Model.SetTranslation({0, 0, z}, false);
		llc_necs(::llc::d3dDrawEngineNode(renderer, scene, 0, lights));
		llc_necs(target.Read(*resources, pixels.Color, pixels.DepthStencil));
		LLC_TEST_CHECK(errors, D3D_TEST_RESULT_OVERLAY_DEPTH, pixels.Color[12][8].g != (z > 0 ? 200 : 0) || pixels.DepthStencil[12][8] != (::llc::u2_t)(0xFFFFFFFU * (z > 0 ? .5 : .25)));
	}
	llc_necs(pixels.resize(::llc::n2u2_t{19, 11}, ::llc::BLUE, 0));
	llc_necs(target.Begin(*resources, pixels.Color));
	llc_necs(target.Read(*resources, pixels.Color, pixels.DepthStencil));
	LLC_TEST_CHECK(errors, D3D_TEST_RESULT_TARGET_RESIZE, pixels.Color[10][18] != ::llc::bgra(::llc::BLUE) || pixels.DepthStencil[10][18] != 0xFFFFFFFFU);
	return 0;
}

::llc::err_t testD3D(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	struct SWindowGuard { HWND Handle = {}; ~SWindowGuard() { if(Handle) ::DestroyWindow(Handle); } } window;
	struct SComGuard { HRESULT Result = ::CoInitializeEx(nullptr, COINIT_MULTITHREADED); ~SComGuard() { if(SUCCEEDED(Result)) ::CoUninitialize(); } } com;
	llc_hrcall(com.Result);
	window.Handle = ::CreateWindowExW(0, L"STATIC", L"LLC D3D consumer test", WS_OVERLAPPEDWINDOW, 0, 0, 128, 96, nullptr, nullptr, ::GetModuleHandleW(nullptr), nullptr);
	LLC_TEST_REQUIRE(errors, D3D_TEST_RESULT_WINDOW_CREATE, nullptr == window.Handle);
	::llc::pobj<::DX::D3DDeviceResources> resourceOwner;
	if_null_fe(resourceOwner.create());
	::DX::D3DDeviceResources & resources = *resourceOwner;
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
	llc_necs(testD3DLights(errors, resourceOwner));
	llc_necs(testGUIMesh(errors, resourceOwner));
	rtrn 0;
}

#else

::llc::err_t testD3D(::llc::ATestError &) { rtrn 0; }

#endif
