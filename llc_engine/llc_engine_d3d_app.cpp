#include "llc_engine_d3d_app.h"
#include "llc_view_color.h"
#include "llc_rect_align.h"

static ::llc::err_t guiControlResources(ID3D11Device * device, ::llc::SD3DGUIResources<::llc::bgra> & gpu) {
	::llc::au0_t code;
	llc_necs(::llc::fileToMemory("vsGUIControl.cso", code));
	llc_hrcall(device->CreateVertexShader(code.begin(), code.size(), nullptr, &gpu.ControlVertexShader));
	const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 1, DXGI_FORMAT_R32_UINT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 3, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 4, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	static_assert(sizeof(::llc::SD3DGUIVertex) == 48);
	llc_hrcall(device->CreateInputLayout(layout, ARRAYSIZE(layout), code.begin(), code.size(), &gpu.ControlInputLayout));
	code.clear();
	llc_necs(::llc::fileToMemory("psGUIControl.cso", code));
	llc_hrcall(device->CreatePixelShader(code.begin(), code.size(), nullptr, &gpu.ControlPixelShader));
	D3D11_BUFFER_DESC constants = {16}; constants.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	llc_hrcall(device->CreateBuffer(&constants, nullptr, &gpu.ControlConstants));
	D3D11_RASTERIZER_DESC raster = {}; raster.FillMode = D3D11_FILL_SOLID; raster.CullMode = D3D11_CULL_NONE;
	llc_hrcall(device->CreateRasterizerState(&raster, &gpu.ControlRasterizerState));
	D3D11_BLEND_DESC blend = {};
	blend.RenderTarget[0].BlendEnable = true;
	blend.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE; blend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO; blend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blend.RenderTarget[0].BlendOp = blend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	llc_hrcall(device->CreateBlendState(&blend, &gpu.ControlImageBlendState));
	return 0;
}

static ::llc::err_t guiQuad(::llc::SD3DGUIResources<::llc::bgra> & gpu, ::llc::rect2f2_t rect, ::llc::bgra color, ::llc::u2_t mode = 0, ::llc::rect2f2_t uv = {}, ::llc::tri2f2_t triangle = {}) {
	if(rect.Size.x <= 0 || rect.Size.y <= 0) return 0;
	const ::llc::n2f2_t positions[] = {rect.Offset, {rect.Offset.x + rect.Size.x, rect.Offset.y}, rect.Limit(), {rect.Offset.x, rect.Offset.y + rect.Size.y}};
	const ::llc::n2f2_t texcoords[] = {uv.Offset, {uv.Offset.x + uv.Size.x, uv.Offset.y}, uv.Limit(), {uv.Offset.x, uv.Offset.y + uv.Size.y}};
	for(::llc::u2_t index : {0U, 1U, 2U, 0U, 2U, 3U})
		llc_necs(gpu.ControlVertices.push_back({positions[index], texcoords[index], color, mode, triangle}));
	return 0;
}

static ::llc::err_t guiVerticesDraw(::DX::D3DDeviceResources & resources, ::llc::SD3DGUIResources<::llc::bgra> & gpu, ID3D11ShaderResourceView * texture = nullptr, bool blend = false) {
	if(0 == gpu.ControlVertices.size()) return 0;
	auto context = resources.GetD3DDeviceContext();
	D3D11_BUFFER_DESC desc = {};
	if(gpu.ControlVertexBuffer) gpu.ControlVertexBuffer->GetDesc(&desc);
	if(desc.ByteWidth < gpu.ControlVertices.byte_count()) {
		desc.ByteWidth = ::llc::max<::llc::u2_t>(4096, ::llc::max(desc.ByteWidth * 2, gpu.ControlVertices.byte_count()));
		desc.Usage = D3D11_USAGE_DYNAMIC; desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		::llc::pcom<ID3D11Buffer> buffer;
		llc_hrcall(resources.GetD3DDevice()->CreateBuffer(&desc, nullptr, &buffer));
		gpu.ControlVertexBuffer = buffer;
	}
	D3D11_MAPPED_SUBRESOURCE mapped = {};
	llc_hrcall(context->Map(gpu.ControlVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
	memcpy(mapped.pData, gpu.ControlVertices.begin(), gpu.ControlVertices.byte_count());
	context->Unmap(gpu.ControlVertexBuffer, 0);
	const UINT stride = sizeof(::llc::SD3DGUIVertex), offset = 0;
	context->IASetVertexBuffers(0, 1, &gpu.ControlVertexBuffer, &stride, &offset);
	context->PSSetShaderResources(0, 1, &texture);
	context->OMSetBlendState(blend ? gpu.ControlImageBlendState.get() : nullptr, nullptr, 0xFFFFFFFF);
	context->Draw(gpu.ControlVertices.size(), 0);
	gpu.ControlVertices.clear();
	return 0;
}

static ::llc::err_t guiFontTexture(::DX::D3DDeviceResources & resources, ::llc::SD3DGUIResources<::llc::bgra> & gpu, const ::llc::pobj<::llc::SRasterFont> & font) {
	for(::llc::u2_t iFont = 0; iFont < gpu.ControlFonts.size(); ++iFont)
		if(gpu.ControlFonts[iFont].Font == font) return iFont;
	::llc::SD3DGUIFont next; next.Font = font;
	if_true_fe(0 == font->Texture.Pitch || 0 == font->CharSize.x || 0 == font->CharSize.y);
	::llc::img8bgra pixels;
	llc_necs(pixels.resize(::llc::n2u2_t{font->Texture.Pitch, font->Texture.View.size() / font->Texture.Pitch}));
	for(::llc::u2_t i = 0; i < pixels.Texels.size(); ++i)
		pixels.Texels[i] = {255, 255, 255, (::llc::u0_t)(font->Texture.View[i] ? 255 : 0)};
	llc_necs(::llc::d3dCreateTextureDynamic(resources.GetD3DDevice(), next.Texture, next.View, pixels.View));
	return gpu.ControlFonts.push_back(next);
}

static ::llc::err_t guiImageTexture(::DX::D3DDeviceResources & resources, ::llc::SD3DGUIResources<::llc::bgra> & gpu, ::llc::gc8bgra image, ::llc::n2u2_t & textureSize) {
	D3D11_TEXTURE2D_DESC desc = {};
	if(gpu.ControlImageTexture) gpu.ControlImageTexture->GetDesc(&desc);
	if(desc.Width < image.metrics().x || desc.Height < image.metrics().y) {
		desc.Width = ::llc::max(desc.Width, image.metrics().x); desc.Height = ::llc::max(desc.Height, image.metrics().y);
		desc.MipLevels = desc.ArraySize = desc.SampleDesc.Count = 1;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE; desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		::llc::pcom<ID3D11Texture2D> texture; ::llc::pcom<ID3D11ShaderResourceView> view;
		llc_hrcall(resources.GetD3DDevice()->CreateTexture2D(&desc, nullptr, &texture));
		llc_hrcall(resources.GetD3DDevice()->CreateShaderResourceView(texture, nullptr, &view));
		gpu.ControlImageTexture = texture; gpu.ControlImageView = view;
	}
	textureSize = {desc.Width, desc.Height};
	D3D11_MAPPED_SUBRESOURCE mapped = {};
	auto context = resources.GetD3DDeviceContext();
	llc_hrcall(context->Map(gpu.ControlImageTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
	for(::llc::u2_t y = 0; y < image.metrics().y; ++y)
		memcpy((::llc::u0_t*)mapped.pData + y * mapped.RowPitch, image[y].begin(), image.metrics().x * sizeof(::llc::bgra));
	context->Unmap(gpu.ControlImageTexture, 0);
	return 0;
}

static ::llc::err_t guiControlDraw(::DX::D3DDeviceResources & resources, ::llc::SD3DGUIResources<::llc::bgra> & gpu, const ::llc::SGUI & gui, ::llc::cid_t id, ::llc::n2u1_t targetSize, ID3D11ShaderResourceView * & fontView) {
	if(::llc::controlHidden(gui, id)) return 0;
	const auto & state = gui.Controls.States[id];
	const auto & draw = gui.Controls.Draw[id];
	const auto & area = gui.Controls.Area[id];
	const auto palette = ::llc::controlDisabled(gui, id) ? ::llc::GUI_CONTROL_PALETTE_DISABLED : state.IsPressed() ? ::llc::GUI_CONTROL_PALETTE_PRESSED : state.IsSelected() ? ::llc::GUI_CONTROL_PALETTE_SELECTED : state.IsHovered() ? ::llc::GUI_CONTROL_PALETTE_HOVER : ::llc::GUI_CONTROL_PALETTE_NORMAL;
	::llc::bgra colors[::llc::UI_CONTROL_AREA_COUNT];
	if(draw.UseNewPalettes) memcpy(colors, gui.Colors->Palettes[draw.Palettes[palette]].begin(), sizeof(colors));
	else {
		const auto & indices = (*gui.Colors->ControlThemes)[draw.ColorTheme ? draw.ColorTheme - 1 : gui.ThemeDefault].ColorCombos[palette];
		for(::llc::u2_t i = 0; i < ::llc::UI_CONTROL_AREA_COUNT; ++i) colors[i] = (*gui.Colors->Palette)[indices[i]];
	}
	const ::llc::bgra textColor = colors[::llc::UI_CONTROL_AREA_TEXT_FACE];
	const auto mode = draw.ColorMode ? draw.ColorMode : gui.ColorModeDefault;
	if(mode == ::llc::GUI_COLOR_MODE_Default || mode == ::llc::GUI_COLOR_MODE_3D) {
		colors[::llc::UI_CONTROL_AREA_BORDER_LEFT] = colors[::llc::UI_CONTROL_AREA_BORDER_TOP] = colors[::llc::UI_CONTROL_AREA_CLIENT] * (state.IsPressed() ? .4 : state.IsHovered() ? .8 : 1.2);
		colors[::llc::UI_CONTROL_AREA_BORDER_RIGHT] = colors[::llc::UI_CONTROL_AREA_BORDER_BOTTOM] = colors[::llc::UI_CONTROL_AREA_CLIENT] * (state.IsPressed() ? 1.5 : state.IsHovered() ? 1.2 : .8);
	}
	for(::llc::u2_t i = 0; i < ::llc::GUI_CONTROL_AREA_OLD_COUNT; ++i)
		if(i ? !draw.NoBorder : !draw.NoClient) llc_necs(guiQuad(gpu, area.Rectangles[i].f2(), colors[i + 1]));
	if(!draw.NoBorder && ::llc::controlNCSpacing(gui.Controls.Placement[id]).Area()) {
		const ::llc::u2_t shades[] = {3, 2, 3, 4, 2, 5, 4, 5};
		for(::llc::u2_t i = 0; i < 8; ++i) {
			const auto triangle = area.Triangles[i].f2();
			const ::llc::n2f2_t first = {::llc::min(triangle.A.x, ::llc::min(triangle.B.x, triangle.C.x)), ::llc::min(triangle.A.y, ::llc::min(triangle.B.y, triangle.C.y))};
			const ::llc::n2f2_t last = {::llc::max(triangle.A.x, ::llc::max(triangle.B.x, triangle.C.x)), ::llc::max(triangle.A.y, ::llc::max(triangle.B.y, triangle.C.y))};
			llc_necs(guiQuad(gpu, {first, last - first}, colors[shades[i]], 3, {}, triangle));
		}
	}
	const auto & image = gui.Controls.Images[id];
	bool drawText = true;
	if(image.Image.size()) {
		llc_necs(guiVerticesDraw(resources, gpu, fontView));
		::llc::rect2s1_t rect = {{}, image.Image.metrics().s1()};
		::llc::realignRectangle(area.Client.Global.Size, rect, rect, (::llc::ALIGN)image.ImageAlign);
		rect.Offset += area.Client.Global.Offset;
		if(state.IsHovered()) rect.Offset += {1, 1};
		const auto origin = rect.Offset, limit = rect.Limit();
		rect.Offset.x = ::llc::max(rect.Offset.x, area.Client.Global.Offset.x); rect.Offset.y = ::llc::max(rect.Offset.y, area.Client.Global.Offset.y);
		rect.Size = {::llc::max<::llc::s1_t>(0, ::llc::min(limit.x, area.Client.Global.Limit().x) - rect.Offset.x), ::llc::max<::llc::s1_t>(0, ::llc::min(limit.y, area.Client.Global.Limit().y) - rect.Offset.y)};
		drawText = rect.Size.x && rect.Size.y;
		::llc::n2s2_t source = (image.ImageOffset + rect.Offset - origin).s2();
		for(::llc::u2_t axis = 0; axis < 2; ++axis) {
			auto & start = axis ? source.y : source.x; auto & size = axis ? rect.Size.y : rect.Size.x; auto & dst = axis ? rect.Offset.y : rect.Offset.x;
			// Preserve grid_copy_blend's source offset, clipped count and skipped cells.
			const ::llc::s2_t clipped = ::llc::min<::llc::s2_t>(0, dst), destination = ::llc::max<::llc::s2_t>(0, dst);
			const ::llc::s2_t stop = (axis ? image.Image.metrics().y : image.Image.metrics().x) + clipped;
			const ::llc::s2_t count = ::llc::min<::llc::s2_t>(size, ::llc::min(stop - start, (axis ? targetSize.y : targetSize.x) - destination));
			start -= clipped;
			const ::llc::s2_t skipped = -::llc::min(0, start);
			dst = (::llc::s1_t)(destination + skipped); size = (::llc::s1_t)::llc::max(0, count - 2 * skipped); start += skipped;
		}
		if(rect.Size.x > 0 && rect.Size.y > 0) {
			::llc::n2u2_t size;
			llc_necs(guiImageTexture(resources, gpu, image.Image, size));
			::llc::rect2f2_t uv = {{source.x / (float)size.x, source.y / (float)size.y}, {rect.Size.x / (float)size.x, rect.Size.y / (float)size.y}};
			if(image.ImageInvertY) { uv.Offset.y = (image.Image.metrics().y - source.y) / (float)size.y; uv.Size.y = -uv.Size.y; }
			llc_necs(guiQuad(gpu, rect.f2(), ::llc::WHITE, 2, uv));
			llc_necs(guiVerticesDraw(resources, gpu, gpu.ControlImageView, true));
		}
	}
	const auto & text = gui.Controls.Text[id];
	::llc::s2_t iFont = -1;
	if(drawText && text.Text.size()) {
		const auto & font = gui.Fonts[::llc::in_range(text.FontSelected, (::llc::s1_t)0, (::llc::s1_t)gui.Fonts.size()) ? text.FontSelected : gui.SelectedFont];
		llc_necs(iFont = guiFontTexture(resources, gpu, font));
		if(fontView != gpu.ControlFonts[iFont].View.get()) {
			llc_necs(guiVerticesDraw(resources, gpu, fontView));
			fontView = gpu.ControlFonts[iFont].View.get();
		}
		const ::llc::n2f2_t atlas = {(float)font->Texture.Pitch, (float)(font->Texture.View.size() / font->Texture.Pitch)};
		const ::llc::u2_t columns = font->Texture.Pitch / font->CharSize.x;
		::llc::n2f2_t position = area.Text.Offset.f2();
		if(state.IsHovered() || state.IsPressed()) position += {1, 1};
		for(::llc::u2_t i = 0; i < text.Text.size(); ++i) {
			const ::llc::u0_t glyph = (::llc::u0_t)text.Text[i];
			const ::llc::rect2f2_t uv = {{(glyph % columns) * font->CharSize.x / atlas.x, (glyph / columns) * font->CharSize.y / atlas.y}, {font->CharSize.x / atlas.x, font->CharSize.y / atlas.y}};
			llc_necs(guiQuad(gpu, {position, font->CharSize.f2()}, textColor, 1, uv));
			position.x += font->CharSize.x;
		}
	}
	for(::llc::cid_t child : gui.Controls.Children[id]) llc_necs(guiControlDraw(resources, gpu, gui, child, targetSize, fontView));
	return 0;
}

::llc::err_t llc::d3dGUIDraw(::DX::D3DDeviceResources & resources, ::llc::SD3DGUIResources<::llc::bgra> & gpu, const ::llc::SGUI & gui, ::llc::n2u1_t targetSize) {
	if(0 == targetSize.x || 0 == targetSize.y) return 0;
	if(!gpu.ControlImageBlendState) llc_necs(guiControlResources(resources.GetD3DDevice(), gpu));
	auto context = resources.GetD3DDeviceContext();
	const float scale[] = {2.0f / targetSize.x, -2.0f / targetSize.y, -1, 1};
	context->UpdateSubresource(gpu.ControlConstants, 0, nullptr, scale, 0, 0);
	context->VSSetConstantBuffers(0, 1, &gpu.ControlConstants);
	context->IASetInputLayout(gpu.ControlInputLayout);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->VSSetShader(gpu.ControlVertexShader, nullptr, 0);
	context->PSSetShader(gpu.ControlPixelShader, nullptr, 0);
	context->PSSetSamplers(0, 1, &gpu.SamplerState);
	context->RSSetState(gpu.ControlRasterizerState);
	context->OMSetDepthStencilState(gpu.DepthStencilState, 0);
	gpu.ControlVertices.clear();
	ID3D11ShaderResourceView * fontView = nullptr;
	for(::llc::u2_t i = 0; i < gui.Controls.States.size(); ++i)
		if(!::llc::controlInvalid(gui, (::llc::cid_t)i) && ::llc::controlInvalid(gui, gui.Controls.States[i].Parent))
			llc_necs(guiControlDraw(resources, gpu, gui, (::llc::cid_t)i, targetSize, fontView));
	return guiVerticesDraw(resources, gpu, fontView);
}

::llc::err_t llc::Sample3DSceneRenderer::CreateVertexShader(::llc::vcst_t name) {
	::llc::string filename;
	llc_necs(filename.append(name));
	llc_necs(filename.append(::llc::vcst_t{".cso"}));
	::llc::au0_t code;
	llc_necs(::llc::fileToMemory(filename, code));
	::llc::pcom<ID3D11VertexShader> shader;
	llc_hrcall(DeviceResources->GetD3DDevice()->CreateVertexShader(code.begin(), code.size(), nullptr, &shader));
	return VertexShader.push_back(shader);
}

::llc::err_t llc::SD3DRenderTarget::Upload(::DX::D3DDeviceResources & resources, ::llc::gc8bgra colors) {
	if_null_fe(Color.get());
	D3D11_TEXTURE2D_DESC desc = {};
	Color->GetDesc(&desc);
	if_true_fe(desc.Width != colors.metrics().x || desc.Height != colors.metrics().y);
	resources.GetD3DDeviceContext()->UpdateSubresource(Color, 0, nullptr, colors.begin(), (UINT)(colors.metrics().x * sizeof(::llc::bgra)), 0);
	return 0;
}

::llc::err_t llc::SD3DRenderTarget::Begin(::DX::D3DDeviceResources & resources, ::llc::gc8bgra colors) {
	if_true_fe(0 == colors.size());
	auto device = resources.GetD3DDevice();
	auto context = resources.GetD3DDeviceContext();
	D3D11_TEXTURE2D_DESC desc = {};
	::llc::pcom<ID3D11Device> owner;
	if(Color) { Color->GetDesc(&desc); Color->GetDevice(&owner); }
	if(desc.Width != colors.metrics().x || desc.Height != colors.metrics().y || owner.get() != device) {
		::llc::SD3DRenderTarget next;
		desc = {};
		desc.Width = colors.metrics().x; desc.Height = colors.metrics().y;
		desc.ArraySize = desc.MipLevels = desc.SampleDesc.Count = 1;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET;
		llc_hrcall(device->CreateTexture2D(&desc, nullptr, &next.Color));
		llc_hrcall(device->CreateRenderTargetView(next.Color, nullptr, &next.Target));
		desc.Format = DXGI_FORMAT_R32_TYPELESS;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		llc_hrcall(device->CreateTexture2D(&desc, nullptr, &next.Depth));
		D3D11_DEPTH_STENCIL_VIEW_DESC depthView = {};
		depthView.Format = DXGI_FORMAT_D32_FLOAT;
		depthView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		llc_hrcall(device->CreateDepthStencilView(next.Depth, &depthView, &next.DepthView));
		desc.BindFlags = 0;
		desc.Usage = D3D11_USAGE_STAGING;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		llc_hrcall(device->CreateTexture2D(&desc, nullptr, &next.ReadDepth));
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		llc_hrcall(device->CreateTexture2D(&desc, nullptr, &next.ReadColor));
		*this = next;
	}
	llc_necs(Upload(resources, colors));
	context->OMSetRenderTargets(1, &Target, DepthView);
	const D3D11_VIEWPORT viewport = {0, 0, (float)colors.metrics().x, (float)colors.metrics().y, 0, 1};
	context->RSSetViewports(1, &viewport);
	context->ClearDepthStencilView(DepthView, D3D11_CLEAR_DEPTH, 1, 0);
	return 0;
}

::llc::err_t llc::SD3DRenderTarget::Read(::DX::D3DDeviceResources & resources, ::llc::g8bgra colors, ::llc::gu2_t depth) {
	if_null_fe(Color.get());
	D3D11_TEXTURE2D_DESC desc = {};
	Color->GetDesc(&desc);
	if_true_fe(colors.metrics() != depth.metrics() || colors.metrics().x != desc.Width || colors.metrics().y != desc.Height);
	auto context = resources.GetD3DDeviceContext();
	context->CopyResource(ReadColor, Color);
	context->CopyResource(ReadDepth, Depth);
	D3D11_MAPPED_SUBRESOURCE mapped = {};
	llc_hrcall(context->Map(ReadColor, 0, D3D11_MAP_READ, 0, &mapped));
	for(::llc::u2_t y = 0; y < desc.Height; ++y)
		memcpy(colors[y].begin(), (const ::llc::u0_t*)mapped.pData + y * mapped.RowPitch, colors[y].byte_count());
	context->Unmap(ReadColor, 0);
	llc_hrcall(context->Map(ReadDepth, 0, D3D11_MAP_READ, 0, &mapped));
	for(::llc::u2_t y = 0; y < desc.Height; ++y) {
		const float * row = (const float*)((const ::llc::u0_t*)mapped.pData + y * mapped.RowPitch);
		for(::llc::u2_t x = 0; x < desc.Width; ++x)
			depth[y][x] = row[x] >= 1 ? 0xFFFFFFFFU : (::llc::u2_t)(0xFFFFFFFU * (double)row[x]);
	}
	context->Unmap(ReadDepth, 0);
	return 0;
}

::llc::err_t llc::Sample3DSceneRenderer::SetLights(::llc::view<const ::llc::SRenderLight> lights) {
	static_assert(sizeof(::llc::SRenderLight) == 96);
	auto device = DeviceResources->GetD3DDevice();
	auto context = DeviceResources->GetD3DDeviceContext();
	if_true_fe(lights.size() > 0xFFFFFFFFU / sizeof(::llc::SRenderLight));
	D3D11_BUFFER_DESC desc = {};
	if(LightBuffer)
		LightBuffer->GetDesc(&desc);
	const UINT required = (UINT)::llc::max(lights.size(), 1U) * (UINT)sizeof(::llc::SRenderLight);
	if(desc.ByteWidth < required) {
		desc = {};
		desc.ByteWidth = required;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.StructureByteStride = sizeof(::llc::SRenderLight);
		::llc::pcom<ID3D11Buffer> buffer;
		::llc::pcom<ID3D11ShaderResourceView> view;
		llc_hrcall(device->CreateBuffer(&desc, nullptr, &buffer));
		D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
		srv.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srv.Buffer.NumElements = desc.ByteWidth / desc.StructureByteStride;
		llc_hrcall(device->CreateShaderResourceView(buffer, &srv, &view));
		LightBuffer = buffer;
		LightView = view;
	}
	if(lights.size()) {
		D3D11_MAPPED_SUBRESOURCE mapped = {};
		llc_hrcall(context->Map(LightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
		memcpy(mapped.pData, lights.begin(), lights.byte_count());
		context->Unmap(LightBuffer, 0);
	}
	if(!LightConstants) {
		desc = {};
		desc.ByteWidth = 16;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		llc_hrcall(device->CreateBuffer(&desc, nullptr, &LightConstants));
	}
	const ::llc::u2_t counts[4] = {lights.size()};
	context->UpdateSubresource(LightConstants, 0, nullptr, counts, 0, 0);
	return 0;
}

::llc::error_t			llc::d3dGUIDraw		(::DX::D3DDeviceResources & d3dResources, ::llc::SD3DGUIResources<::llc::bgra> & guiStuff)	{
	if(!guiStuff.Texture2D) 
		return 1;
	
	D3D11_MAPPED_SUBRESOURCE	mappedTexture				= {};
	llc_hrcall(d3dResources.GetD3DDeviceContext()->Map(guiStuff.Texture2D.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedTexture));
	if(mappedTexture.pData) {
		for(uint32_t y = 0; y < guiStuff.RenderTarget.metrics().y; ++y) {
			::llc::vcbgra			scanLineSrc					= guiStuff.RenderTarget[y];
			::llc::vbgra			scanLineDst					= {(::llc::bgra*)(&((sc_t*)mappedTexture.pData)[y * mappedTexture.RowPitch]), scanLineSrc.size()};
			memcpy(scanLineDst.begin(), scanLineSrc.begin(), scanLineSrc.byte_count());
			//for(uint32_t x = 0; x < scanLineDst.size(); ++x)
			//	 scanLineDst.begin()[x].a	= scanLineDst.begin()[x].a;
		}
	}

	d3dResources.GetD3DDeviceContext()->Unmap(guiStuff.Texture2D.get(), 0);
	// Each vertex is one instance of the VertexPositionColor struct.
	UINT									stride						= sizeof(::llc::n3f2_t) + sizeof(::llc::n2f2_t);
	UINT									offset						= 0;

	ID3D11DeviceContext						* context					= d3dResources.GetD3DDeviceContext();
	::llc::pcom<ID3D11Buffer>				vb							= guiStuff.VertexBuffer;
	::llc::pcom<ID3D11Buffer>				ib							= guiStuff.IndexBuffer ;
	context->IASetVertexBuffers		(0, 1, &vb, &stride, &offset);
	context->IASetIndexBuffer		(ib, DXGI_FORMAT_R16_UINT, 0);// Each index is one 16-bit unsigned integer (short).
	context->IASetPrimitiveTopology	(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout		(guiStuff.InputLayout);
	context->VSSetShader			(guiStuff.VertexShader, nullptr, 0);	// Attach our vertex shader.
	context->PSSetShader			(guiStuff.PixelShader, nullptr, 0);	// Attach our pixel shader.
	context->RSSetState				(guiStuff.RasterizerState);
	context->PSSetSamplers			(0, 1, &guiStuff.SamplerState);
	context->PSSetShaderResources	(0, 1, &guiStuff.ShaderResourceView);
	context->OMSetBlendState		(guiStuff.BlendState, 0, 0xFFFFFFFF);
	context->OMSetDepthStencilState	(guiStuff.DepthStencilState, 0);
	//
	context->DrawIndexed(6, 0, 0);	// Draw the objects.
	return 0;
}

static	::llc::error_t	getNodeTransform		(const ::llc::SRenderNodeManager & nodeManager, uint32_t iNode, const ::llc::SEngineSceneConstants & constants, ::llc::SRenderNodeConstants	& nodeConstants) {
	nodeConstants			= nodeManager.Transforms[iNode];

	nodeConstants.Model					= nodeManager.BaseTransforms[iNode].Model * nodeConstants.Model;
	nodeConstants.ModelInverse			= nodeConstants.Model.GetInverse();
	nodeConstants.ModelInverseTranspose	= nodeConstants.ModelInverse.GetTranspose();
	nodeConstants.MVP					= nodeConstants.Model * constants.View * constants.Perspective;

	nodeConstants.Model					= nodeConstants.Model.GetTranspose();
	nodeConstants.ModelInverse			= nodeConstants.ModelInverse.GetTranspose();
	nodeConstants.ModelInverseTranspose	= nodeConstants.ModelInverseTranspose.GetTranspose();
	nodeConstants.MVP					= nodeConstants.MVP.GetTranspose();
	nodeConstants.NodeSize				= nodeManager.BaseTransforms[iNode].NodeSize; // Have to update this some other way.
	return 0;
}

::llc::err_t llc::d3dDrawEngineNode(::llc::Sample3DSceneRenderer & renderer, const ::llc::SEngineScene & scene, ::llc::u2_t iNode, ::llc::view<const ::llc::SRenderLight> lights, const ::llc::SRenderMaterial * material, ::llc::u2_t vertexShader, ::llc::CULL_MODE cullMode) {
	if_true_fe(iNode >= scene.RenderNodes.size() || !scene.Graphics);
	const ::llc::SRenderNode & node = scene.RenderNodes[iNode];
	if_true_fe(node.Mesh >= scene.Graphics->Meshes.size() || node.Skin >= scene.Graphics->Skins.size());
	const ::llc::SGeometryMesh & mesh = *scene.Graphics->Meshes[node.Mesh];
	const ::llc::SSkin & skin = *scene.Graphics->Skins[node.Skin];
	if_true_fe(0 == mesh.GeometryBuffers.size());
	const auto & indices = *scene.Graphics->Buffers[mesh.GeometryBuffers[0]];
	const ::llc::u2_t indexCount = indices.Data.size() / indices.Desc.Format.TotalBytes();
	const ::llc::rangeu2_t slice = node.Slice < mesh.GeometrySlices.size() ? mesh.GeometrySlices[node.Slice].Slice : ::llc::rangeu2_t{0, indexCount};
	if_true_fe(slice.Offset > indexCount || slice.Count > indexCount - slice.Offset);
	::llc::SRenderNodeConstants constants;
	llc_necs(::getNodeTransform(scene.RenderNodes, iNode, renderer.ConstantBufferScene, constants));
	constants.Material = material ? *material : skin.Material;
	const ::llc::m4f2_t model = scene.RenderNodes.BaseTransforms[iNode].Model * scene.RenderNodes.Transforms[iNode].Model;
	llc_necs(::llc::lightFilter(model.GetTranslation(), lights, renderer.RenderCache.LightsNode));
	return renderer.Render(node.Mesh, slice, skin.Textures, node.Shader, constants.Material.Color.Diffuse.a != 1.0f, constants, renderer.RenderCache.LightsNode, vertexShader, cullMode);
}

::llc::error_t			llc::d3dDrawEngineScene	(::llc::Sample3DSceneRenderer & d3dScene, const ::llc::SEngineScene & engineScene, const ::llc::n2u1_t & targetMetrics, const ::llc::n3f2_t & lightPos, const ::llc::n3f2_t & cameraPosition, const llc::n3f2_t & cameraTarget, const llc::minmaxf2_t & nearFar, ::llc::f3_t time)	{
	::llc::n3f2_t				cameraFront				= (cameraTarget - cameraPosition).Normalize();

	::llc::SEngineSceneConstants& constants				= d3dScene.ConstantBufferScene;
	constants.CameraPosition= cameraPosition;
	constants.CameraFront	= cameraFront;
	constants.LightPosition	= lightPos;
	constants.LightDirection= {0, -1, 0};
	constants.Time			= (::llc::f2_t)time;

	constants.View.LookAt(cameraPosition, cameraTarget, {0, 1, 0});
	constants.Perspective.FieldOfView(.25 * ::llc::math_pi, targetMetrics.x / (double)targetMetrics.y, nearFar.Min, nearFar.Max);
	constants.Screen.ViewportLH(targetMetrics);
	constants.VP			= constants.View * constants.Perspective;
	constants.VPS			= constants.VP * constants.Screen;

	::llc::SRenderNodeList						renderList;
	llc_necs(engineScene.BuildRenderList(cameraPosition, cameraFront, renderList));
	llc_necs(engineScene.BuildLightList(d3dScene.RenderCache.LightsWorld));
	for(::llc::u2_t node : renderList.Solid)
		llc_necs(::llc::d3dDrawEngineNode(d3dScene, engineScene, node, d3dScene.RenderCache.LightsWorld));
	for(::llc::u2_t node : renderList.Alpha)
		llc_necs(::llc::d3dDrawEngineNode(d3dScene, engineScene, node, d3dScene.RenderCache.LightsWorld));
	return 0;
}
::llc::error_t							llc::d3dAppDraw				(::llc::SD3DApplication & d3dApp, const ::llc::SEngineScene & engineScene, const ::llc::rgbaf & clearColor, const ::llc::n3f2_t & lightPos, const ::llc::n3f2_t & cameraPosition, const llc::n3f2_t & cameraTarget, const llc::minmaxf2_t & nearFar, ::llc::f3_t time) {
	{ // Set up render target for this frame
		ID3D11DeviceContext3						* context							= d3dApp.DeviceResources->GetD3DDeviceContext();
		const D3D11_VIEWPORT						viewport							= d3dApp.DeviceResources->GetScreenViewport();	// Reset the viewport to target the whole screen.
		context->RSSetViewports(1, &viewport);

		ID3D11RenderTargetView *					targets	[]							= { d3dApp.DeviceResources->GetBackBufferRenderTargetView(), };	// Reset render targets to the screen.
		context->OMSetRenderTargets(1, targets, d3dApp.DeviceResources->GetDepthStencilView());

		context->ClearRenderTargetView(d3dApp.DeviceResources->GetBackBufferRenderTargetView(), &clearColor.r);	// Clear the back buffer and depth stencil view.
		context->ClearDepthStencilView(d3dApp.DeviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);	
	}
	{ // Render 3d scene
		//(void)engineScene; (void)lightPos; (void)cameraPosition; (void)cameraTarget; (void)nearFar;
		llc_necs(llc::d3dDrawEngineScene(d3dApp.Scene, engineScene, d3dApp.DeviceResources->GetLogicalSize().u1(), lightPos, cameraPosition, cameraTarget, nearFar, time));
	}
	{ // Render GUI
		llc_necs(llc::d3dGUIDraw(*d3dApp.DeviceResources, d3dApp.GUIStuff)); 

		llc_necs(d3dApp.Text.Render());
	}
	llc_necs(d3dApp.DeviceResources->Present());
	return 0;
}
