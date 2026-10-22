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

#include "llc_ro_defines.h"

#include "llc_array.h"
#include "llc_n2.h"
#include "llc_n3.h"

#ifndef LLC_RO_MAP_H
#define LLC_RO_MAP_H

namespace llc {
	// Heights are ordered west-to-east, south-to-north, as stored by GAT 1.2.
	struct SROGATTile {
		f2_t				Height	[4]				= {};
		u0_t				Type					= {};
		u0_t				Unknown	[3]				= {};
	};
	static_assert(20 == szof(SROGATTile));

	struct SROGAT {
		SROVersion			Version					= {};
		n2u2_t				Dimensions				= {};
		apod<SROGATTile>	Tiles					= {};
	};

	// Loaders decode progressively into output. A failure leaves the decoded prefix available
	// for inspection, and subsequent calls reuse the capacities owned by output.
	err_t					roGATLoad					(SROGAT & output, vcu0_t input);

	struct SROGNDLightmap {
		u0_t				Brightness	[8][8]			= {};
		n3u0_t				ColorRGB	[8][8]			= {};
	};
	static_assert(256 == szof(SROGNDLightmap));

	struct SROGNDSkin {
		f2_t				U			[4]				= {0, 1, 0, 1};
		f2_t				V			[4]				= {0, 0, 1, 1};
		s1_t				TextureIndex				= -1;
		s1_t				LightmapIndex				= -1;
		u2_t				Color					= 0xFFFFFFFFU;
	};
	static_assert(40 == szof(SROGNDSkin));

	struct SROGNDTile {
		f2_t				Height		[4]				= {};
		s2_t				SkinIndexTop				= -1;
		s2_t				SkinIndexRight				= -1;
		s2_t				SkinIndexFront				= -1;
		s1_t				Flags					= 0;
	};

	struct SROGND {
		SROVersion			Version					= {};
		n2u2_t				Dimensions				= {};
		f2_t				TileScale				= {};
		u2_t				TextureNameSize				= {};
		aobj<asc_t>			TextureNames				= {};
		n2u2_t				LightmapDimensions			= {};
		u2_t				LightmapTiles				= {};
		apod<SROGNDLightmap>Lightmaps					= {};
		apod<SROGNDSkin>	Skins					= {};
		apod<SROGNDTile>	Tiles					= {};
	};

	err_t					roGNDLoad					(SROGND & output, vcu0_t input);
}

#endif // LLC_RO_MAP_H
