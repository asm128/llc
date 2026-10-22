// Copyright (c) 2011 Pablo Ariel Zorrilla Cepeda
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// http://www.opensource.org/licenses/mit-license.php

#include "llc_ro_map.h"

#include "llc_view_stream.h"

::llc::err_t				llc::roGNDLoad					(SROGND & output, vcu0_t input) {
	::llc::view_stream<::llc::u0_c>	inputStream					= {input.begin(), input.size()};
	::llc::u0_t				magic[4]					= {};
	llc_necs(inputStream.read_pod(magic));
	if_true_fef('G' != magic[0] || 'R' != magic[1] || 'G' != magic[2] || 'N' != magic[3]
		, "Invalid GND signature: %02X %02X %02X %02X.", (::llc::u2_t)magic[0], (::llc::u2_t)magic[1], (::llc::u2_t)magic[2], (::llc::u2_t)magic[3]);
	llc_necs(inputStream.read_pod(output.Version));
	if_true_fef(1 != output.Version.Major || (5 != output.Version.Minor && 7 != output.Version.Minor)
		, "Unsupported GND version: %u.%u.", (::llc::u2_t)output.Version.Major, (::llc::u2_t)output.Version.Minor);
	llc_necs(inputStream.read_pod(output.Dimensions));
	llc_necs(inputStream.read_pod(output.TileScale));

	::llc::u2_t				textureCount					= 0;
	llc_necs(inputStream.read_pod(textureCount));
	llc_necs(inputStream.read_pod(output.TextureNameSize));
	if_true_fef(textureCount && 0 == output.TextureNameSize, "GND declares %" LLC_FMT_U2 " textures with zero-length names.", textureCount);
	if_true_fef(textureCount && output.TextureNameSize > inputStream.remaining() / textureCount
		, "GND texture names exceed the remaining input: %" LLC_FMT_U2 " x %" LLC_FMT_U2 " > %" LLC_FMT_U2 ".", textureCount, output.TextureNameSize, inputStream.remaining());
	llc_necs(output.TextureNames.resize(textureCount));
	for(::llc::u2_t iTexture = 0; iTexture < textureCount; ++iTexture) {
		llc_necs(output.TextureNames[iTexture].resize(output.TextureNameSize));
		llc_necs(inputStream.read_pod(::llc::view<::llc::sc_t>{output.TextureNames[iTexture].begin(), output.TextureNames[iTexture].size()}));
	}

	::llc::u2_t				lightmapCount					= 0;
	llc_necs(inputStream.read_pod(lightmapCount));
	llc_necs(inputStream.read_pod(output.LightmapDimensions));
	llc_necs(inputStream.read_pod(output.LightmapTiles));
	if_true_fef((lightmapCount && output.LightmapDimensions != ::llc::n2u2_t{8, 8})
		, "Unsupported GND lightmap dimensions: %" LLC_FMT_U2 " x %" LLC_FMT_U2 ".", output.LightmapDimensions.x, output.LightmapDimensions.y);
	if_true_fef(lightmapCount > inputStream.remaining() / szof(::llc::SROGNDLightmap)
		, "GND lightmap payload exceeds the remaining input: %" LLC_FMT_U2 " > %" LLC_FMT_U2 ".", lightmapCount, inputStream.remaining() / szof(::llc::SROGNDLightmap));
	llc_necs(output.Lightmaps.resize(lightmapCount));
	llc_necs(inputStream.read_pod(::llc::view<::llc::SROGNDLightmap>{output.Lightmaps.begin(), output.Lightmaps.size()}));

	::llc::u2_t				skinCount						= 0;
	llc_necs(inputStream.read_pod(skinCount));
	if_true_fef(skinCount > inputStream.remaining() / szof(::llc::SROGNDSkin)
		, "GND skin payload exceeds the remaining input: %" LLC_FMT_U2 " > %" LLC_FMT_U2 ".", skinCount, inputStream.remaining() / szof(::llc::SROGNDSkin));
	llc_necs(output.Skins.resize(skinCount));
	llc_necs(inputStream.read_pod(::llc::view<::llc::SROGNDSkin>{output.Skins.begin(), output.Skins.size()}));

	::llc::u3_c				tileCount64					= (::llc::u3_t)output.Dimensions.x * output.Dimensions.y;
	if_true_fef(tileCount64 > 0x3FFFFFFFU, "GND tile count exceeds LLC array capacity: %" LLC_FMT_U3 ".", tileCount64);
	::llc::u2_c				tileCount						= (::llc::u2_t)tileCount64;
	::llc::u2_c				tileByteCount					= 5 == output.Version.Minor ? 24U : 28U;
	if_true_fef(tileCount > inputStream.remaining() / tileByteCount
		, "GND tile payload exceeds the remaining input: %" LLC_FMT_U2 " > %" LLC_FMT_U2 ".", tileCount, inputStream.remaining() / tileByteCount);
	llc_necs(output.Tiles.resize(tileCount));
	for(::llc::u2_t iTile = 0; iTile < tileCount; ++iTile) {
		::llc::SROGNDTile		& tile						= output.Tiles[iTile];
		tile							= {};
		llc_necs(inputStream.read_pod(tile.Height));
		if(5 == output.Version.Minor) {
			::llc::s1_t			indicesAndFlags[4]			= {};
			llc_necs(inputStream.read_pod(indicesAndFlags));
			tile.SkinIndexTop				= indicesAndFlags[0];
			tile.SkinIndexRight				= indicesAndFlags[1];
			tile.SkinIndexFront				= indicesAndFlags[2];
			tile.Flags						= indicesAndFlags[3];
		}
		else {
			::llc::s2_t			indices[3]					= {};
			llc_necs(inputStream.read_pod(indices));
			tile.SkinIndexTop				= indices[0];
			tile.SkinIndexRight				= indices[1];
			tile.SkinIndexFront				= indices[2];
		}
	}

	if_true_fef(inputStream.CursorPosition > 0x7FFFFFFFU, "GND byte count exceeds the result range: %" LLC_FMT_U2 ".", inputStream.CursorPosition);
	return (::llc::err_t)inputStream.CursorPosition;
}
