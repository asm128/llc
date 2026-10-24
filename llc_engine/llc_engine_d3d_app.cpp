#include "llc_engine_d3d_app.h"
#include "llc_view_color.h"
#include "llc_rect_align.h"

static ::llc::err_t guiControlResources(ID3D11Device * device, ::llc::SD3DGUIResources<::llc::bgra> & gpu) {
	::llc::au0_t code;
	llc_necs(::llc::fileToMemory("vsGUIControl.cso", code));
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
	llc_necs(::llc::fileToMemory("psGUIControl.cso", code));
	llc_hrcall(device->CreatePixelShader(code.begin(), code.size(), nullptr, &gpu.ControlPixelShader));

	llc_necs(::llc::fileToMemory("vsGUITexture.cso", code));
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
	llc_necs(::llc::fileToMemory("psGUITexture.cso", code));
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
	llc_necs(pixels.resize(::llc::n2u2_t{font->Texture.Pitch, font->Texture.View.size() / font->Texture.Pitch}));
	for(::llc::u2_t i = 0; i < pixels.Texels.size(); ++i)
		pixels.Texels[i] = {255, 255, 255, (::llc::u0_t)(font->Texture.View[i] ? 255 : 0)};
	llc_necs(::llc::d3dCreateTextureDynamic(resources.GetD3DDevice(), next.Texture, next.View, pixels.View));
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
	llc_necs(guiControlInstance(gui, id, gpu.ControlInstances[id], textColor));
	SD3DGUIDrawCommand controlCommand = {};
	controlCommand.Index = id;
	llc_necs(commands.push_back(controlCommand));
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
			llc_necs(guiImageTexture(resources, gpu.ControlImages[id], image.Image, size));
			::llc::rect2f2_t uv = {{source.x / (float)size.x, source.y / (float)size.y}, {rect.Size.x / (float)size.x, rect.Size.y / (float)size.y}};
			if(image.ImageInvertY) { uv.Offset.y = (image.Image.metrics().y - source.y) / (float)size.y; uv.Size.y = -uv.Size.y; }
			const ::llc::u2_t first = gpu.ControlTextureInstances.size();
			llc_necs(gpu.ControlTextureInstances.push_back({rect, uv, ::llc::WHITE, 2}));
			llc_necs(guiTextureCommand(commands, gpu.ControlImages[id].View, first, 1, true));
		}
	}
	const auto & text = gui.Controls.Text[id];
	if(drawText && text.Text.size()) {
		const auto & font = gui.Fonts[::llc::in_range(text.FontSelected, (::llc::s1_t)0, (::llc::s1_t)gui.Fonts.size()) ? text.FontSelected : gui.SelectedFont];
		::llc::s2_t iFont = -1;
		llc_necs(iFont = guiFontTexture(resources, gpu, font));
		const ::llc::n2f2_t atlas = {(float)font->Texture.Pitch, (float)(font->Texture.View.size() / font->Texture.Pitch)};
		const ::llc::u2_t columns = font->Texture.Pitch / font->CharSize.x;
		::llc::n2s1_t position = area.Text.Offset;
		if(state.IsHovered() || state.IsPressed()) position += {1, 1};
		const ::llc::u2_t first = gpu.ControlTextureInstances.size();
		for(::llc::u2_t i = 0; i < text.Text.size(); ++i) {
			const ::llc::u0_t glyph = (::llc::u0_t)text.Text[i];
			const ::llc::rect2f2_t uv = {{(glyph % columns) * font->CharSize.x / atlas.x, (glyph / columns) * font->CharSize.y / atlas.y}, {font->CharSize.x / atlas.x, font->CharSize.y / atlas.y}};
			const ::llc::rect2s1_t rectangle = {position, {(::llc::s1_t)font->CharSize.x, (::llc::s1_t)font->CharSize.y}};
			llc_necs(gpu.ControlTextureInstances.push_back({rectangle, uv, textColor, 1}));
			position.x += font->CharSize.x;
		}
		llc_necs(guiTextureCommand(commands, gpu.ControlFonts[iFont].View, first, gpu.ControlTextureInstances.size() - first, false));
	}
	for(::llc::cid_t child : gui.Controls.Children[id])
		llc_necs(guiControlBuild(resources, gpu, gui, child, targetSize, commands));
	return 0;
}

static ::llc::err_t guiInstancesUpload(::DX::D3DDeviceResources & resources, ::llc::SD3DGUIResources<::llc::bgra> & gpu) {
	ID3D11Device * device = resources.GetD3DDevice();
	ID3D11DeviceContext * context = resources.GetD3DDeviceContext();
	llc_necs(guiDynamicBuffer(device, gpu.ControlInstanceBuffer, gpu.ControlInstances.byte_count()));
	llc_necs(guiDynamicBuffer(device, gpu.ControlTextureInstanceBuffer, gpu.ControlTextureInstances.byte_count()));
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
	if(!gpu.ControlImageBlendState) llc_necs(guiControlResources(resources.GetD3DDevice(), gpu));
	llc_necs(gpu.ControlInstances.resize(gui.Controls.States.size()));
	llc_necs(gpu.ControlImages.resize(gui.Controls.States.size()));
	gpu.ControlTextureInstances.clear();
	::llc::apod<SD3DGUIDrawCommand> commands;
	for(::llc::u2_t i = 0; i < gui.Controls.States.size(); ++i)
		if(!::llc::controlInvalid(gui, (::llc::cid_t)i) && ::llc::controlInvalid(gui, gui.Controls.States[i].Parent))
			llc_necs(guiControlBuild(resources, gpu, gui, (::llc::cid_t)i, targetSize, commands));
	llc_necs(guiInstancesUpload(resources, gpu));
	auto context = resources.GetD3DDeviceContext();
	const float scale[] = {2.0f / targetSize.x, -2.0f / targetSize.y, -1, 1};
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
	llc_necs(::llc::d3dGUIDraw(resources, app.GUIStuff, gui, targetSize));
	rtrn resources.Present();
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
	const ::llc::au2_t						& solid							= renderList.Solid;
	const ::llc::au2_t						& alpha							= renderList.Alpha;
	for(uint32_t iSolid = 0; iSolid < solid.size(); ++iSolid) {
		uint32_t								iNode							= solid[iSolid];
		::llc::SRenderNodeConstants				nodeConstants;
		getNodeTransform(engineScene.RenderNodes, iNode, constants, nodeConstants);
		
		const ::llc::SRenderNode				& node							= engineScene.RenderNodes.RenderNodes[iNode];
		const ::llc::SGeometryMesh				& mesh							= *engineScene.Graphics->Meshes[node.Mesh];

		uint32_t				indexCount			= 0;
		if(mesh.GeometryBuffers.size() > 0) {
			auto					pindices			= engineScene.Graphics->Buffers[mesh.GeometryBuffers[0]];
			indexCount			= pindices->Data.size() / pindices->Desc.Format.TotalBytes();
		}

		const ::llc::SSkin						& skin							= *engineScene.Graphics->Skins.Elements[node.Skin];
		const ::llc::SGeometrySlice				slice							= (node.Slice < mesh.GeometrySlices.size()) ? mesh.GeometrySlices[node.Slice] : ::llc::SGeometrySlice{{0, indexCount / 3}};
		nodeConstants.Material				= skin.Material;

		verbose_printf("Drawing node %i, mesh %i, slice %i, mesh name: %s", iNode, node.Mesh, node.Slice, engineScene.Graphics->Meshes.Names[node.Mesh].begin());
		llc_necs(d3dScene.Render(node.Mesh, slice.Slice, skin.Textures, node.Shader, skin.Material.Color.Diffuse.a != 1.0f, nodeConstants));
	}
	for(uint32_t iAlpha  = 0; iAlpha < alpha.size(); ++iAlpha) {
		uint32_t								iNode							= alpha[iAlpha];
		::llc::SRenderNodeConstants				nodeConstants;
		getNodeTransform(engineScene.RenderNodes, iNode, constants, nodeConstants);
		
		const ::llc::SRenderNode				& node							= engineScene.RenderNodes.RenderNodes[iNode];
		const ::llc::SGeometryMesh				& mesh							= *engineScene.Graphics->Meshes[node.Mesh];
		uint32_t				indexCount			= 0;
		if(mesh.GeometryBuffers.size() > 0) {
			auto					pindices			= engineScene.Graphics->Buffers[mesh.GeometryBuffers[0]];
			indexCount			= pindices->Data.size() / pindices->Desc.Format.TotalBytes();
		}

		const ::llc::SSkin			& skin							= *engineScene.Graphics->Skins.Elements[node.Skin];
		const ::llc::SGeometrySlice	slice							= (node.Slice < mesh.GeometrySlices.size()) ? mesh.GeometrySlices[node.Slice] : ::llc::SGeometrySlice{{0, indexCount / 3}};
		nodeConstants.Material	= skin.Material;

		verbose_printf("Drawing node %i, mesh %i, slice %i, mesh name: %s", iNode, node.Mesh, node.Slice, engineScene.Graphics->Meshes.Names[node.Mesh].begin());
		llc_necs(d3dScene.Render(node.Mesh, slice.Slice, skin.Textures, node.Shader, skin.Material.Color.Diffuse.a != 1.0f, nodeConstants));
	}
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
