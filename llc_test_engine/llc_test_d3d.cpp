#include "llc_test_core.h"
#include "llc_d3d.h"
#include "llc_d3d_device_resources.h"
#include "llc_windows.h"

#if defined(LLC_WINDOWS)

#include "llc_engine_d3d_app.h"
#include "llc_png.h"
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
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, GUI_PIXELS					, 25, "GPU control drawing must match the existing CPU geometry, palettes, fonts and images.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, GUI_REUSE					, 26, "Repeated GUI drawing must reuse its vertex buffer and font textures.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, GUI_UNCHANGED				, 27, "GPU drawing must leave GUI geometry and state untouched.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, CULL_PIXELS					, 28, "Culling must select winding independently of alpha blending.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, CULL_DEPTH					, 29, "Changing culling must preserve the selected depth-write behavior.");
GDEFINE_ENUM_VALUED(D3D_TEST_RESULT, CULL_INVALID				, 30, "An invalid culling mode must fail before changing the rasterizer state.");

sttc ::llc::err_t testD3DGUI(::llc::ATestError & errors, ::DX::D3DDeviceResources & resources, ::llc::vcst_t outputFolder) {
	::llc::SGUI gui, overlay;
	cnst ::llc::GUI_CONTROL_FLAG states[] = {{}, ::llc::GUI_CONTROL_FLAG_Hovered, ::llc::GUI_CONTROL_FLAG_Selected, ::llc::GUI_CONTROL_FLAG_Pressed, ::llc::GUI_CONTROL_FLAG_Disabled};
	cnst ::llc::vcst_t labels[] = {"Normal", "Hovered", "Selected", "Pressed", "Disabled"};
	for(::llc::u2_t i = 0; i < 20; ++i) {
		::llc::cid_t id;
		llc_necs(id = ::llc::controlCreate(gui));
		gui.Controls.Placement[id].Area = {{(::llc::s1_t)(12 + 200 * (i % 5)), (::llc::s1_t)(12 + 100 * (i / 5))}, {188, 90}};
		gui.Controls.Placement[id].Border = {3, 5, 7, 9};
		gui.Controls.Placement[id].Margin = {2, 4, 6, 8};
		gui.Controls.Text[id].Text = labels[i % 5];
		gui.Controls.Text[id].Align = ::llc::ALIGN_CENTER;
		gui.Controls.States[id].Flag = states[i % 5];
		gui.Controls.Draw[id].NoClient = i / 5 == 2;
		gui.Controls.Draw[id].NoBorder = i / 5 == 3;
	}
	::llc::cid_t child;
	llc_necs(child = ::llc::controlCreateChild(gui, 6));
	gui.Controls.Placement[child].Area = {{-20, 50}, {150, 35}};
	gui.Controls.Text[child].Text = "Child overlaps";
	::llc::cid_t clipped;
	llc_necs(clipped = ::llc::controlCreate(gui));
	gui.Controls.Placement[clipped].Area = {{-10, -5}, {90, 28}};
	gui.Controls.Placement[clipped].Margin = gui.Controls.Placement[clipped].Border = {};
	gui.Controls.Text[clipped].Text = "Clipped";
	gui.Controls.Text[clipped].Align = ::llc::ALIGN_TOP_LEFT;
	::llc::img8bgra image;
	llc_necs(image.resize(::llc::n2u2_t{80, 60}));
	for(::llc::u2_t y = 0; y < 60; ++y) for(::llc::u2_t x = 0; x < 80; ++x)
		image.View[y][x] = {(::llc::u0_t)(x * 3), (::llc::u0_t)(y * 4), 125, (::llc::u0_t)((x % 3) * 127)};
	for(::llc::u2_t i = 0; i < 3; ++i) {
		::llc::cid_t id;
		llc_necs(id = ::llc::controlCreate(overlay));
		overlay.Controls.Placement[id].Area = {{(::llc::s1_t)(90 + i * 160), 430}, {140, 70}};
		overlay.Controls.Text[id].Text = "Image";
		overlay.Controls.Images[id].Image = image.View;
		overlay.Controls.Images[id].ImageAlign = ::llc::ALIGN_CENTER;
		overlay.Controls.Images[id].ImageInvertY = i == 1;
		overlay.Controls.Images[id].ImageOffset = {i == 2 ? (::llc::s1_t)-5 : (::llc::s1_t)0, 3};
	}
	overlay.SelectedFont = 6;
	::llc::SD3DGUIResources<::llc::bgra> gpu;
	llc_necs(gpu.CreateDeviceResources(resources.GetD3DDevice()));
	::llc::SD3DRenderTarget target;
	::llc::rt<::llc::bgra, ::llc::u2_t> actual;
	::llc::img8bgra expected;
	llc_necs(actual.resize(::llc::n2u2_t{1024, 512}));
	llc_necs(expected.resize(actual.metrics()));
	cnst ::llc::vcst_t variants[] = {"flat-theme", "raised-theme", "flat-palette", "raised-palette"};
	for(::llc::u2_t variant = 0; variant < 4; ++variant) {
		gui.ColorModeDefault = variant % 2 ? ::llc::GUI_COLOR_MODE_3D : ::llc::GUI_COLOR_MODE_Flat;
		for(auto & draw : gui.Controls.Draw) draw.UseNewPalettes = variant / 2;
		llc_necs(::llc::guiUpdateMetrics(gui, expected.metrics().u1(), true));
		llc_necs(::llc::guiUpdateMetrics(overlay, expected.metrics().u1(), true));
		llc_necs(expected.View.fill({23, 47, 71, 255}));
		llc_necs(actual.Color.View.fill({23, 47, 71, 255}));
		llc_necs(::llc::guiDraw(gui, expected.View));
		llc_necs(::llc::guiDraw(overlay, expected.View));
		::llc::apod<::llc::SControlArea> areas; ::llc::apod<::llc::SControlState> flags;
		llc_necs(areas.append(gui.Controls.Area)); llc_necs(flags.append(gui.Controls.States));
		llc_necs(target.Begin(resources, actual.Color));
		llc_necs(::llc::d3dGUIDraw(resources, gpu, gui, expected.metrics().u1()));
		llc_necs(::llc::d3dGUIDraw(resources, gpu, overlay, expected.metrics().u1()));
		llc_necs(target.Read(resources, actual.Color, actual.DepthStencil));
		::llc::u2_t different = 0, first = 0;
		for(::llc::u2_t i = 0; i < expected.Texels.size(); ++i) {
			cnst auto a = actual.Color.Texels[i], b = expected.Texels[i];
			cnst int tolerance = i / 1024 >= 430 ? 1 : 0; // Only image blending permits UNORM rounding.
			if(::llc::abs((int)a.r - b.r) > tolerance || ::llc::abs((int)a.g - b.g) > tolerance || ::llc::abs((int)a.b - b.b) > tolerance || a.a != b.a) { if(!different) first = i; ++different; }
		}
		cnst auto a = actual.Color.Texels[first], b = expected.Texels[first];
		LLC_TEST_CHECKF(errors, D3D_TEST_RESULT_GUI_PIXELS, different != 0, "Variant:%u, differing:%u, first:{%u,%u}, GPU BGRA:%u/%u/%u/%u, CPU:%u/%u/%u/%u.", variant, different, first % 1024, first / 1024, a.b, a.g, a.r, a.a, b.b, b.g, b.r, b.a);
		LLC_TEST_CHECK(errors, D3D_TEST_RESULT_GUI_UNCHANGED, ::memcmp(areas.begin(), gui.Controls.Area.begin(), areas.byte_count()) || ::memcmp(flags.begin(), gui.Controls.States.begin(), flags.byte_count()));
		cnst auto buffer = gpu.ControlVertexBuffer.get(); cnst auto fonts = gpu.ControlFonts.size();
		llc_necs(::llc::d3dGUIDraw(resources, gpu, gui, expected.metrics().u1()));
		llc_necs(::llc::d3dGUIDraw(resources, gpu, overlay, expected.metrics().u1()));
		LLC_TEST_CHECK(errors, D3D_TEST_RESULT_GUI_REUSE, buffer != gpu.ControlVertexBuffer.get() || fonts != gpu.ControlFonts.size());
		::llc::string folder; ::llc::au0_t bytes;
		llc_necs(::llc::pathNameCompose(outputFolder, variants[variant], folder));
		llc_necs(::llc::pathCreate(folder));
		llc_necs(::llc::pngFileWrite(expected.View, bytes)); llc_necs(::llc::fileFromMemory(folder, "gui-cpu.png", bytes));
		bytes.clear();
		llc_necs(::llc::pngFileWrite(actual.Color.View, bytes)); llc_necs(::llc::fileFromMemory(folder, "gui-gpu.png", bytes));
	}
	// Include GPU completion, not just asynchronous command submission, in the comparison.
	llc_necs(target.Read(resources, actual.Color, actual.DepthStencil));
	::llc::pcom<ID3D11Query> finished;
	cnst D3D11_QUERY_DESC query = {D3D11_QUERY_EVENT, 0};
	llc_hrcall(resources.GetD3DDevice()->CreateQuery(&query, &finished));
	cnst ::llc::u2_t frames = 32;
	::llc::STimer timer;
	for(::llc::u2_t i = 0; i < frames; ++i) { llc_necs(::llc::guiDraw(gui, expected.View)); llc_necs(::llc::guiDraw(overlay, expected.View)); }
	timer.Frame(); cnst double cpu = timer.LastTimeMicroseconds / (double)frames;
	timer.Reset();
	for(::llc::u2_t i = 0; i < frames; ++i) { llc_necs(::llc::d3dGUIDraw(resources, gpu, gui, expected.metrics().u1())); llc_necs(::llc::d3dGUIDraw(resources, gpu, overlay, expected.metrics().u1())); }
	auto context = resources.GetD3DDeviceContext();
	context->End(finished); context->Flush();
	cnst auto deadline = ::GetTickCount64() + 10000;
	HRESULT status;
	while(S_FALSE == (status = context->GetData(finished, nullptr, 0, 0))) { if_true_fe(::GetTickCount64() > deadline); ::Sleep(0); }
	llc_hrcall(status);
	timer.Frame();
	always_printf("GUI comparison: 1024x512, two instances, 25 controls, %u frames; CPU %.1f us/frame, GPU with completion %.1f us/frame, ratio %.2fx. Excludes layout and presentation.", frames, cpu, timer.LastTimeMicroseconds / (double)frames, cpu * frames / timer.LastTimeMicroseconds);
	rtrn 0;
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
	llc_necs(lights.resize(1));
	lights[0].Color.Diffuse = ::llc::RED;
	lights[0].Range = 10;
	for(bool reversed : {false, true}) for(bool alpha : {false, true})
	for(::llc::CULL_MODE mode : {::llc::CULL_MODE_CCW, ::llc::CULL_MODE_CW, ::llc::CULL_MODE_NONE}) {
		scene.RenderNodes.Transforms[0].Model.Scale({reversed ? -1.0f : 1.0f, 1, 1}, true);
		::llc::SRenderMaterial material = {};
		material.Color.Diffuse.a = alpha ? .5f : 1.0f;
		for(auto & pixel : pixels.Color.Texels) pixel = ::llc::BLUE;
		llc_necs(target.Begin(*resources, pixels.Color));
		llc_necs(::llc::d3dDrawEngineNode(renderer, scene, 0, lights, &material, 0, mode));
		llc_necs(target.Read(*resources, pixels.Color, pixels.DepthStencil));
		const bool visible = mode == ::llc::CULL_MODE_NONE || mode == (reversed ? ::llc::CULL_MODE_CW : ::llc::CULL_MODE_CCW);
		LLC_TEST_CHECKF(errors, D3D_TEST_RESULT_CULL_PIXELS, (pixels.Color[12][8].r != 0) != visible, "Mode:%u, reversed:%u, alpha:%u.", (::llc::u2_t)mode, reversed, alpha);
		LLC_TEST_CHECKF(errors, D3D_TEST_RESULT_CULL_DEPTH, pixels.DepthStencil[12][8] != (visible && !alpha ? (::llc::u2_t)(0xFFFFFFFU * .5) : 0xFFFFFFFFU), "Mode:%u, reversed:%u, alpha:%u.", (::llc::u2_t)mode, reversed, alpha);
	}
	::llc::pcom<ID3D11RasterizerState> beforeCull, afterCull;
	context->RSGetState(&beforeCull);
	const ::llc::err_t invalidCull = renderer.Render(0, {0, 6}, {}, 0, false, {}, {}, 0, (::llc::CULL_MODE)255);
	context->RSGetState(&afterCull);
	LLC_TEST_CHECK(errors, D3D_TEST_RESULT_CULL_INVALID, invalidCull != ::llc::OS_INVALID_PARAMETER || beforeCull.get() != afterCull.get());
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
	llc_necs(testD3DGUI(errors, resources, outputFolder));
	rtrn 0;
}

#else

::llc::err_t testD3D(::llc::ATestError &) { rtrn 0; }

#endif
