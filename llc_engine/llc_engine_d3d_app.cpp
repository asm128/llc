#include "llc_engine_d3d_app.h"
#include "llc_view_color.h"
#include "llc_rect_align.h"

LLC_USING_APOD();
LLC_USING_VIEW();
LLC_USING_TYPEINT();
LLC_USING_N2();
LLC_USING_N3();

static ::llc::err_t guiControlResources(ID3D11Device * device, ::llc::SD3DGUIResources<::llc::bgra> & gpu) {
	::llc::au0_t code;
	if_fail_fe(::llc::fileToMemory("vsGUIControl.cso", code));
	llc_hrcall(device->CreateVertexShader(code.begin(), code.size(), nullptr, &gpu.ControlVertexShader));
	const D3D11_INPUT_ELEMENT_DESC controlLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R8G8_UINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R16G16B16A16_SINT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"TEXCOORD", 1, DXGI_FORMAT_R16G16B16A16_SINT, 1, 8, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"COLOR", 1, DXGI_FORMAT_R8G8B8A8_UNORM, 1, 20, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"COLOR", 2, DXGI_FORMAT_R8G8B8A8_UNORM, 1, 24, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"COLOR", 3, DXGI_FORMAT_R8G8B8A8_UNORM, 1, 28, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"COLOR", 4, DXGI_FORMAT_R8G8B8A8_UNORM, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"TEXCOORD", 2, DXGI_FORMAT_R32_UINT, 1, 36, D3D11_INPUT_PER_INSTANCE_DATA, 1},
	};
	static_assert(sizeof(::llc::n2u0_t) == 2);
	static_assert(sizeof(::llc::SD3DGUIControlInstance) == 40);
	llc_hrcall(device->CreateInputLayout(controlLayout, ARRAYSIZE(controlLayout), code.begin(), code.size(), &gpu.ControlInputLayout));
	code.clear();
	if_fail_fe(::llc::fileToMemory("psGUIControl.cso", code));
	llc_hrcall(device->CreatePixelShader(code.begin(), code.size(), nullptr, &gpu.ControlPixelShader));

	if_fail_fe(::llc::fileToMemory("vsGUITexture.cso", code));
	llc_hrcall(device->CreateVertexShader(code.begin(), code.size(), nullptr, &gpu.ControlTextureVertexShader));
	const D3D11_INPUT_ELEMENT_DESC textureLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R16G16B16A16_SINT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 8, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 1, 24, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"TEXCOORD", 2, DXGI_FORMAT_R32_UINT, 1, 28, D3D11_INPUT_PER_INSTANCE_DATA, 1},
	};
	static_assert(sizeof(::llc::SD3DGUITextureInstance) == 32);
	llc_hrcall(device->CreateInputLayout(textureLayout, ARRAYSIZE(textureLayout), code.begin(), code.size(), &gpu.ControlTextureInputLayout));
	code.clear();
	if_fail_fe(::llc::fileToMemory("psGUITexture.cso", code));
	llc_hrcall(device->CreatePixelShader(code.begin(), code.size(), nullptr, &gpu.ControlTexturePixelShader));

	D3D11_BUFFER_DESC constants = {16};
	constants.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	llc_hrcall(device->CreateBuffer(&constants, nullptr, &gpu.ControlConstants));
	const ::llc::n2u0_t controlVertices[] = {
		{0, 0}, {1, 0}, {2, 0}, {3, 0},
		{0, 1}, {1, 1}, {2, 1}, {3, 1},
		{0, 2}, {1, 2}, {2, 2}, {3, 2},
		{0, 3}, {1, 3}, {2, 3}, {3, 3},
	};
	const ::llc::u1_t controlIndices[] = {
		0, 5, 1, 0, 4, 5, 1, 6, 2, 1, 5, 6, 2, 6, 3, 3, 6, 7,
		4, 9, 5, 4, 8, 9, 5, 10, 6, 5, 9, 10, 6, 11, 7, 6, 10, 11,
		8, 12, 9, 9, 12, 13, 9, 14, 10, 9, 13, 14, 10, 15, 11, 10, 14, 15,
	};
	D3D11_BUFFER_DESC bufferDesc = {sizeof(controlVertices)};
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA bufferData = {controlVertices};
	llc_hrcall(device->CreateBuffer(&bufferDesc, &bufferData, &gpu.ControlVertexBuffer));
	bufferDesc.ByteWidth = sizeof(controlIndices);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferData.pSysMem = controlIndices;
	llc_hrcall(device->CreateBuffer(&bufferDesc, &bufferData, &gpu.ControlIndexBuffer));
	bufferDesc.ByteWidth = sizeof(::llc::SD3DGUIControlInstance);
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	llc_hrcall(device->CreateBuffer(&bufferDesc, nullptr, &gpu.ControlInstanceBuffer));

	const ::llc::n2f2_t textureVertices[] = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
	bufferDesc = {sizeof(textureVertices)};
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferData = {textureVertices};
	llc_hrcall(device->CreateBuffer(&bufferDesc, &bufferData, &gpu.ControlTextureVertexBuffer));

	D3D11_RASTERIZER_DESC raster = {};
	raster.FillMode = D3D11_FILL_SOLID;
	raster.CullMode = D3D11_CULL_NONE;
	llc_hrcall(device->CreateRasterizerState(&raster, &gpu.ControlRasterizerState));
	D3D11_BLEND_DESC blend = {};
	blend.RenderTarget[0].BlendEnable = true;
	blend.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blend.RenderTarget[0].BlendOp = blend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	llc_hrcall(device->CreateBlendState(&blend, &gpu.ControlImageBlendState));
	return 0;
}

struct SD3DGUIDrawCommand {
	ID3D11ShaderResourceView * Texture = nullptr;
	::llc::u2_t Index = 0;
	::llc::u2_t Count = 0;
	::llc::u2_t TextureDraw : 1;
	::llc::u2_t Blend : 1;
};

static ::llc::err_t guiDynamicBuffer(ID3D11Device * device, ::llc::pcom<ID3D11Buffer> & buffer, ::llc::u2_t requiredBytes) {
	if(0 == requiredBytes)
		return 0;
	D3D11_BUFFER_DESC desc = {};
	if(buffer)
		buffer->GetDesc(&desc);
	if(desc.ByteWidth >= requiredBytes)
		return 0;
	desc.ByteWidth = ::llc::max<::llc::u2_t>(4096U, ::llc::max(desc.ByteWidth * 2U, requiredBytes));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	::llc::pcom<ID3D11Buffer> resized;
	llc_hrcall(device->CreateBuffer(&desc, nullptr, &resized));
	buffer = resized;
	return 0;
}

static ::llc::err_t guiFontTexture(::DX::D3DDeviceResources & resources, ::llc::SD3DGUIResources<::llc::bgra> & gpu, const ::llc::pobj<::llc::SRasterFont> & font) {
	for(::llc::u2_t iFont = 0; iFont < gpu.ControlFonts.size(); ++iFont)
		if(gpu.ControlFonts[iFont].Font == font) return iFont;
	::llc::SD3DGUIFont next; next.Font = font;
	if_true_fe(0 == font->Texture.Pitch || 0 == font->CharSize.x || 0 == font->CharSize.y);
	::llc::img8bgra pixels;
	if_fail_fe(pixels.resize(::llc::n2u2_t{font->Texture.Pitch, font->Texture.View.size() / font->Texture.Pitch}));
	for(::llc::u2_t i = 0; i < pixels.Texels.size(); ++i)
		pixels.Texels[i] = {255, 255, 255, (::llc::u0_t)(font->Texture.View[i] ? 255 : 0)};
	if_fail_fe(::llc::d3dCreateTextureDynamic(resources.GetD3DDevice(), next.Texture, next.View, pixels.View));
	return gpu.ControlFonts.push_back(next);
}

static ::llc::err_t guiImageTexture(::DX::D3DDeviceResources & resources, ::llc::SD3DGUIImage & cached, ::llc::gc8bgra image, ::llc::n2u2_t & textureSize) {
	D3D11_TEXTURE2D_DESC desc = {};
	if(cached.Texture) cached.Texture->GetDesc(&desc);
	if(desc.Width < image.metrics().x || desc.Height < image.metrics().y) {
		desc.Width = ::llc::max(desc.Width, image.metrics().x); desc.Height = ::llc::max(desc.Height, image.metrics().y);
		desc.MipLevels = desc.ArraySize = desc.SampleDesc.Count = 1;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE; desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		::llc::pcom<ID3D11Texture2D> texture; ::llc::pcom<ID3D11ShaderResourceView> view;
		llc_hrcall(resources.GetD3DDevice()->CreateTexture2D(&desc, nullptr, &texture));
		llc_hrcall(resources.GetD3DDevice()->CreateShaderResourceView(texture, nullptr, &view));
		cached.Texture = texture; cached.View = view;
	}
	textureSize = {desc.Width, desc.Height};
	D3D11_MAPPED_SUBRESOURCE mapped = {};
	auto context = resources.GetD3DDeviceContext();
	llc_hrcall(context->Map(cached.Texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
	for(::llc::u2_t y = 0; y < image.metrics().y; ++y)
		memcpy((::llc::u0_t*)mapped.pData + y * mapped.RowPitch, image[y].begin(), image.metrics().x * sizeof(::llc::bgra));
	context->Unmap(cached.Texture, 0);
	return 0;
}

static ::llc::err_t guiControlInstance(const ::llc::SGUI & gui, ::llc::cid_t id, ::llc::SD3DGUIControlInstance & instance, ::llc::bgra & textColor) {
	const auto & state = gui.Controls.States[id];
	const auto & draw = gui.Controls.Draw[id];
	const auto & area = gui.Controls.Area[id];
	const auto palette = ::llc::controlDisabled(gui, id) ? ::llc::GUI_CONTROL_PALETTE_DISABLED : state.IsPressed() ? ::llc::GUI_CONTROL_PALETTE_PRESSED : state.IsSelected() ? ::llc::GUI_CONTROL_PALETTE_SELECTED : state.IsHovered() ? ::llc::GUI_CONTROL_PALETTE_HOVER : ::llc::GUI_CONTROL_PALETTE_NORMAL;
	::llc::bgra colors[::llc::UI_CONTROL_AREA_COUNT] = {};
	if(draw.UseNewPalettes)
		memcpy(colors, gui.Colors->Palettes[draw.Palettes[palette]].begin(), sizeof(colors));
	else {
		const auto & indices = (*gui.Colors->ControlThemes)[draw.ColorTheme ? draw.ColorTheme - 1 : gui.ThemeDefault].ColorCombos[palette];
		for(::llc::u2_t i = 0; i < ::llc::UI_CONTROL_AREA_COUNT; ++i) colors[i] = (*gui.Colors->Palette)[indices[i]];
	}
	textColor = colors[::llc::UI_CONTROL_AREA_TEXT_FACE];
	const auto mode = draw.ColorMode ? draw.ColorMode : gui.ColorModeDefault;
	if(mode == ::llc::GUI_COLOR_MODE_Default || mode == ::llc::GUI_COLOR_MODE_3D) {
		colors[::llc::UI_CONTROL_AREA_BORDER_LEFT] = colors[::llc::UI_CONTROL_AREA_BORDER_TOP] = colors[::llc::UI_CONTROL_AREA_CLIENT] * (state.IsPressed() ? .4 : state.IsHovered() ? .8 : 1.2);
		colors[::llc::UI_CONTROL_AREA_BORDER_RIGHT] = colors[::llc::UI_CONTROL_AREA_BORDER_BOTTOM] = colors[::llc::UI_CONTROL_AREA_CLIENT] * (state.IsPressed() ? 1.5 : state.IsHovered() ? 1.2 : .8);
	}
	const auto & left = area.Rectangles[::llc::GUI_CONTROL_AREA_OLD_BORDER_LEFT];
	const auto & top = area.Rectangles[::llc::GUI_CONTROL_AREA_OLD_BORDER_TOP];
	const auto & right = area.Rectangles[::llc::GUI_CONTROL_AREA_OLD_BORDER_RIGHT];
	const auto & bottom = area.Rectangles[::llc::GUI_CONTROL_AREA_OLD_BORDER_BOTTOM];
	instance.X[0] = left.Offset.x; instance.X[1] = left.Limit().x; instance.X[2] = right.Offset.x; instance.X[3] = right.Limit().x;
	instance.Y[0] = top.Offset.y; instance.Y[1] = top.Limit().y; instance.Y[2] = bottom.Offset.y; instance.Y[3] = bottom.Limit().y;
	instance.Colors[::llc::GUI_CONTROL_AREA_OLD_CLIENT] = colors[::llc::UI_CONTROL_AREA_CLIENT];
	instance.Colors[::llc::GUI_CONTROL_AREA_OLD_BORDER_LEFT] = colors[::llc::UI_CONTROL_AREA_BORDER_LEFT];
	instance.Colors[::llc::GUI_CONTROL_AREA_OLD_BORDER_TOP] = colors[::llc::UI_CONTROL_AREA_BORDER_TOP];
	instance.Colors[::llc::GUI_CONTROL_AREA_OLD_BORDER_RIGHT] = colors[::llc::UI_CONTROL_AREA_BORDER_RIGHT];
	instance.Colors[::llc::GUI_CONTROL_AREA_OLD_BORDER_BOTTOM] = colors[::llc::UI_CONTROL_AREA_BORDER_BOTTOM];
	instance.Flags = (draw.NoClient ? 0U : 1U) | (draw.NoBorder ? 0U : 2U);
	return 0;
}

static ::llc::err_t guiTextureCommand(::llc::apod<SD3DGUIDrawCommand> & commands, ID3D11ShaderResourceView * texture, ::llc::u2_t first, ::llc::u2_t count, bool blend) {
	if(0 == count)
		return 0;
	SD3DGUIDrawCommand command = {texture, first, count};
	command.TextureDraw = 1;
	command.Blend = blend;
	return commands.push_back(command);
}

static ::llc::err_t guiControlBuild(::DX::D3DDeviceResources & resources, ::llc::SD3DGUIResources<::llc::bgra> & gpu, const ::llc::SGUI & gui, ::llc::cid_t id, ::llc::n2u1_t targetSize, ::llc::apod<SD3DGUIDrawCommand> & commands) {
	if(::llc::controlHidden(gui, id))
		return 0;
	::llc::bgra textColor;
	if_fail_fe(guiControlInstance(gui, id, gpu.ControlInstances[id], textColor));
	SD3DGUIDrawCommand controlCommand = {};
	controlCommand.Index = id;
	if_fail_fe(commands.push_back(controlCommand));
	const auto & state = gui.Controls.States[id];
	const auto & area = gui.Controls.Area[id];
	const auto & image = gui.Controls.Images[id];
	bool drawText = true;
	if(image.Image.size()) {
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
			if_fail_fe(guiImageTexture(resources, gpu.ControlImages[id], image.Image, size));
			::llc::rect2f2_t uv = {{source.x / (f2_t)size.x, source.y / (f2_t)size.y}, {rect.Size.x / (f2_t)size.x, rect.Size.y / (f2_t)size.y}};
			if(image.ImageInvertY) { uv.Offset.y = (image.Image.metrics().y - source.y) / (f2_t)size.y; uv.Size.y = -uv.Size.y; }
			const ::llc::u2_t first = gpu.ControlTextureInstances.size();
			if_fail_fe(gpu.ControlTextureInstances.push_back({rect, uv, ::llc::WHITE, 2}));
			if_fail_fe(guiTextureCommand(commands, gpu.ControlImages[id].View, first, 1, true));
		}
	}
	const auto & text = gui.Controls.Text[id];
	if(drawText && text.Text.size()) {
		const auto & font = gui.Fonts[::llc::in_range(text.FontSelected, (::llc::s1_t)0, (::llc::s1_t)gui.Fonts.size()) ? text.FontSelected : gui.SelectedFont];
		::llc::s2_t iFont = -1;
		if_fail_fe(iFont = guiFontTexture(resources, gpu, font));
		const ::llc::n2f2_t atlas = {(f2_t)font->Texture.Pitch, (f2_t)(font->Texture.View.size() / font->Texture.Pitch)};
		const ::llc::u2_t columns = font->Texture.Pitch / font->CharSize.x;
		::llc::n2s1_t position = area.Text.Offset;
		if(state.IsHovered() || state.IsPressed()) position += {1, 1};
		const ::llc::u2_t first = gpu.ControlTextureInstances.size();
		for(::llc::u2_t i = 0; i < text.Text.size(); ++i) {
			const ::llc::u0_t glyph = (::llc::u0_t)text.Text[i];
			const ::llc::rect2f2_t uv = {{(glyph % columns) * font->CharSize.x / atlas.x, (glyph / columns) * font->CharSize.y / atlas.y}, {font->CharSize.x / atlas.x, font->CharSize.y / atlas.y}};
			const ::llc::rect2s1_t rectangle = {position, {(::llc::s1_t)font->CharSize.x, (::llc::s1_t)font->CharSize.y}};
			if_fail_fe(gpu.ControlTextureInstances.push_back({rectangle, uv, textColor, 1}));
			position.x += font->CharSize.x;
		}
		if_fail_fe(guiTextureCommand(commands, gpu.ControlFonts[iFont].View, first, gpu.ControlTextureInstances.size() - first, false));
	}
	for(::llc::cid_t child : gui.Controls.Children[id])
		if_fail_fe(guiControlBuild(resources, gpu, gui, child, targetSize, commands));
	return 0;
}

static ::llc::err_t guiInstancesUpload(::DX::D3DDeviceResources & resources, ::llc::SD3DGUIResources<::llc::bgra> & gpu) {
	ID3D11Device * device = resources.GetD3DDevice();
	ID3D11DeviceContext * context = resources.GetD3DDeviceContext();
	if_fail_fe(guiDynamicBuffer(device, gpu.ControlInstanceBuffer, gpu.ControlInstances.byte_count()));
	if_fail_fe(guiDynamicBuffer(device, gpu.ControlTextureInstanceBuffer, gpu.ControlTextureInstances.byte_count()));
	D3D11_MAPPED_SUBRESOURCE mapped = {};
	if(gpu.ControlInstances.size()) {
		llc_hrcall(context->Map(gpu.ControlInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
		memcpy(mapped.pData, gpu.ControlInstances.begin(), gpu.ControlInstances.byte_count());
		context->Unmap(gpu.ControlInstanceBuffer, 0);
	}
	if(gpu.ControlTextureInstances.size()) {
		llc_hrcall(context->Map(gpu.ControlTextureInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
		memcpy(mapped.pData, gpu.ControlTextureInstances.begin(), gpu.ControlTextureInstances.byte_count());
		context->Unmap(gpu.ControlTextureInstanceBuffer, 0);
	}
	return 0;
}

static void guiControlCommandDraw(ID3D11DeviceContext * context, ::llc::SD3DGUIResources<::llc::bgra> & gpu, ::llc::u2_t control) {
	ID3D11Buffer * buffers[] = {gpu.ControlVertexBuffer, gpu.ControlInstanceBuffer};
	const UINT strides[] = {sizeof(::llc::n2u0_t), sizeof(::llc::SD3DGUIControlInstance)};
	const UINT offsets[] = {0, 0};
	context->IASetVertexBuffers(0, 2, buffers, strides, offsets);
	context->IASetIndexBuffer(gpu.ControlIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	context->IASetInputLayout(gpu.ControlInputLayout);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->VSSetShader(gpu.ControlVertexShader, nullptr, 0);
	context->PSSetShader(gpu.ControlPixelShader, nullptr, 0);
	context->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
	context->DrawIndexedInstanced(54, 1, 0, 0, control);
}

static void guiTextureCommandDraw(ID3D11DeviceContext * context, ::llc::SD3DGUIResources<::llc::bgra> & gpu, const SD3DGUIDrawCommand & command) {
	ID3D11Buffer * buffers[] = {gpu.ControlTextureVertexBuffer, gpu.ControlTextureInstanceBuffer};
	const UINT strides[] = {sizeof(::llc::n2f2_t), sizeof(::llc::SD3DGUITextureInstance)};
	const UINT offsets[] = {0, 0};
	context->IASetVertexBuffers(0, 2, buffers, strides, offsets);
	context->IASetInputLayout(gpu.ControlTextureInputLayout);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	context->VSSetShader(gpu.ControlTextureVertexShader, nullptr, 0);
	context->PSSetShader(gpu.ControlTexturePixelShader, nullptr, 0);
	context->PSSetShaderResources(0, 1, &command.Texture);
	context->OMSetBlendState(command.Blend ? gpu.ControlImageBlendState.get() : nullptr, nullptr, 0xFFFFFFFF);
	context->DrawInstanced(4, command.Count, 0, command.Index);
}

::llc::err_t llc::d3dGUIDraw(::DX::D3DDeviceResources & resources, ::llc::SD3DGUIResources<::llc::bgra> & gpu, const ::llc::SGUI & gui, ::llc::n2u1_t targetSize) {
	if(0 == targetSize.x || 0 == targetSize.y) return 0;
	if(!gpu.ControlImageBlendState) if_fail_fe(guiControlResources(resources.GetD3DDevice(), gpu));
	if_fail_fe(gpu.ControlInstances.resize(gui.Controls.States.size()));
	if_fail_fe(gpu.ControlImages.resize(gui.Controls.States.size()));
	gpu.ControlTextureInstances.clear();
	::llc::apod<SD3DGUIDrawCommand> commands;
	for(::llc::u2_t i = 0; i < gui.Controls.States.size(); ++i)
		if(!::llc::controlInvalid(gui, (::llc::cid_t)i) && ::llc::controlInvalid(gui, gui.Controls.States[i].Parent))
			if_fail_fe(guiControlBuild(resources, gpu, gui, (::llc::cid_t)i, targetSize, commands));
	if_fail_fe(guiInstancesUpload(resources, gpu));
	auto context = resources.GetD3DDeviceContext();
	const f2_t scale[] = {2.0f / targetSize.x, -2.0f / targetSize.y, -1, 1};
	context->UpdateSubresource(gpu.ControlConstants, 0, nullptr, scale, 0, 0);
	context->VSSetConstantBuffers(0, 1, &gpu.ControlConstants);
	context->PSSetSamplers(0, 1, &gpu.SamplerState);
	context->RSSetState(gpu.ControlRasterizerState);
	context->OMSetDepthStencilState(gpu.DepthStencilState, 0);
	for(const SD3DGUIDrawCommand & command : commands)
		if(command.TextureDraw) guiTextureCommandDraw(context, gpu, command);
		else guiControlCommandDraw(context, gpu, command.Index);
	return 0;
}

::llc::err_t llc::d3dGUIFrameDraw(::llc::SD3DApplication & app, const ::llc::SGUI & gui, ::llc::n2u1_t targetSize, const ::llc::rgbaf & clearColor) {
	::DX::D3DDeviceResources & resources = *app.DeviceResources;
	ID3D11DeviceContext3 * context = resources.GetD3DDeviceContext();
	const D3D11_VIEWPORT viewport = resources.GetScreenViewport();
	context->RSSetViewports(1, &viewport);
	ID3D11RenderTargetView * targets[] = {resources.GetBackBufferRenderTargetView()};
	context->OMSetRenderTargets(1, targets, resources.GetDepthStencilView());
	context->ClearRenderTargetView(targets[0], &clearColor.r);
	context->ClearDepthStencilView(resources.GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	if_fail_fe(::llc::d3dGUIDraw(resources, app.GUIStuff, gui, targetSize));
	rtrn resources.Present();
}


::llc::err_t llc::Sample3DSceneRenderer::CreateVertexShader(::llc::vcst_t name) {
	::llc::string filename;
	if_fail_fe(filename.append(name));
	if_fail_fe(filename.append(::llc::vcst_t{".cso"}));
	::llc::au0_t code;
	if_fail_fe(::llc::fileToMemory(filename, code));
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
	if_fail_fe(Upload(resources, colors));
	context->OMSetRenderTargets(1, &Target, DepthView);
	const D3D11_VIEWPORT viewport = {0, 0, (f2_t)colors.metrics().x, (f2_t)colors.metrics().y, 0, 1};
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
		const f2_t * row = (const f2_t*)((const ::llc::u0_t*)mapped.pData + y * mapped.RowPitch);
		for(::llc::u2_t x = 0; x < desc.Width; ++x)
			depth[y][x] = row[x] >= 1 ? 0xFFFFFFFFU : (::llc::u2_t)(0xFFFFFFFU * (f3_t)row[x]);
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

::llc::err_t			llc::d3dGUIDraw		(::DX::D3DDeviceResources & d3dResources, ::llc::SD3DGUIResources<::llc::bgra> & guiStuff)	{
	if(!guiStuff.Texture2D) 
		return 1;
	
	D3D11_MAPPED_SUBRESOURCE	mappedTexture				= {};
	llc_hrcall(d3dResources.GetD3DDeviceContext()->Map(guiStuff.Texture2D.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedTexture));
	if(mappedTexture.pData) {
		for(u2_t y = 0; y < guiStuff.RenderTarget.metrics().y; ++y) {
			::llc::vcbgra			scanLineSrc					= guiStuff.RenderTarget[y];
			::llc::vbgra			scanLineDst					= {(::llc::bgra*)(&((sc_t*)mappedTexture.pData)[y * mappedTexture.RowPitch]), scanLineSrc.size()};
			memcpy(scanLineDst.begin(), scanLineSrc.begin(), scanLineSrc.byte_count());
			//for(u2_t x = 0; x < scanLineDst.size(); ++x)
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

static	::llc::err_t	getNodeTransform		(const ::llc::SRenderNodeManager & nodeManager, u2_t iNode, const ::llc::SEngineSceneConstants & constants, ::llc::SRenderNodeConstants	& nodeConstants) {
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
	if_fail_fe(::getNodeTransform(scene.RenderNodes, iNode, renderer.ConstantBufferScene, constants));
	constants.Material = material ? *material : skin.Material;
	const ::llc::m4f2_t model = scene.RenderNodes.BaseTransforms[iNode].Model * scene.RenderNodes.Transforms[iNode].Model;
	if_fail_fe(::llc::lightFilter(model.GetTranslation(), lights, renderer.RenderCache.LightsNode));
	return renderer.Render(node.Mesh, slice, skin.Textures, node.Shader, constants.Material.Color.Diffuse.a != 1.0f, constants, renderer.RenderCache.LightsNode, vertexShader, cullMode);
}

::llc::err_t			llc::d3dDrawEngineScene	(::llc::Sample3DSceneRenderer & d3dScene, const ::llc::SEngineScene & engineScene, const ::llc::n2u1_t & targetMetrics, const ::llc::n3f2_t & lightPos, const ::llc::n3f2_t & cameraPosition, const llc::n3f2_t & cameraTarget, const llc::minmaxf2_t & nearFar, ::llc::f3_t time)	{
	::llc::n3f2_t				cameraFront				= (cameraTarget - cameraPosition).Normalize();

	::llc::SEngineSceneConstants& constants				= d3dScene.ConstantBufferScene;
	constants.CameraPosition= cameraPosition;
	constants.CameraFront	= cameraFront;
	constants.LightPosition	= lightPos;
	constants.LightDirection= {0, -1, 0};
	constants.Time			= (::llc::f2_t)time;

	constants.View.LookAt(cameraPosition, cameraTarget, {0, 1, 0});
	constants.Perspective.FieldOfView(.25 * ::llc::math_pi, targetMetrics.x / (f3_t)targetMetrics.y, nearFar.Min, nearFar.Max);
	constants.Screen.ViewportLH(targetMetrics);
	constants.VP			= constants.View * constants.Perspective;
	constants.VPS			= constants.VP * constants.Screen;

	::llc::SRenderNodeList						renderList;
	if_fail_fe(engineScene.BuildRenderList(cameraPosition, cameraFront, renderList));
	if_fail_fe(engineScene.BuildLightList(d3dScene.RenderCache.LightsWorld));
	for(::llc::u2_t node : renderList.Solid)
		if_fail_fe(::llc::d3dDrawEngineNode(d3dScene, engineScene, node, d3dScene.RenderCache.LightsWorld));
	for(::llc::u2_t node : renderList.Alpha)
		if_fail_fe(::llc::d3dDrawEngineNode(d3dScene, engineScene, node, d3dScene.RenderCache.LightsWorld));
	return 0;
}
::llc::err_t							llc::d3dAppDraw				(::llc::SD3DApplication & d3dApp, const ::llc::SEngineScene & engineScene, const ::llc::rgbaf & clearColor, const ::llc::n3f2_t & lightPos, const ::llc::n3f2_t & cameraPosition, const llc::n3f2_t & cameraTarget, const llc::minmaxf2_t & nearFar, ::llc::f3_t time) {
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
		if_fail_fe(llc::d3dDrawEngineScene(d3dApp.Scene, engineScene, d3dApp.DeviceResources->GetLogicalSize().u1(), lightPos, cameraPosition, cameraTarget, nearFar, time));
	}
	{ // Render GUI
		if_fail_fe(llc::d3dGUIDraw(*d3dApp.DeviceResources, d3dApp.GUIStuff)); 

		if_fail_fe(d3dApp.Text.Render());
	}
	if_fail_fe(d3dApp.DeviceResources->Present());
	return 0;
}
