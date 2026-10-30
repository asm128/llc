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

#include "llc_grid_serialize.h"
#include "llc_view_stream.h"

::llc::err_t				llc::roGATLoad					(SROGAT & output, vcu0_t input) {
	::llc::view_stream<::llc::u0_c>	inputStream					= {input.begin(), input.size()};
	::llc::u0_t				magic[4]					= {};
	if_fail_fe(inputStream.read_pod(magic));
	if_true_fef('G' != magic[0] || 'R' != magic[1] || 'A' != magic[2] || 'T' != magic[3]
		, "Invalid GAT signature: %02X %02X %02X %02X.", (::llc::u2_t)magic[0], (::llc::u2_t)magic[1], (::llc::u2_t)magic[2], (::llc::u2_t)magic[3]);
	if_fail_fe(inputStream.read_pod(output.Version));
	if_true_fef(1 != output.Version.Major || 2 != output.Version.Minor, "Unsupported GAT version: %u.%u.", (::llc::u2_t)output.Version.Major, (::llc::u2_t)output.Version.Minor);

	::llc::u2_c				gridOffset					= inputStream.CursorPosition;
	if_fail_fe(inputStream.read_pod(output.Dimensions));
	::llc::grid<cnst ::llc::SROGATTile>	tiles						= {};
	::llc::err_t				gridBytes					= 0;
	if_fail_fe(gridBytes = ::llc::gridPascal(tiles, ::llc::vcu0_t{input.begin() + gridOffset, input.size() - gridOffset}));
	if_true_fef((::llc::u2_t)gridBytes > 0x7FFFFFFFU - gridOffset, "GAT byte count exceeds the result range: %i + %" LLC_FMT_U2 ".", gridBytes, gridOffset);
	if_fail_fe(output.Tiles.resize(tiles.size()));
	if(tiles.size())
		memcpy(output.Tiles.begin(), tiles.begin(), tiles.byte_count());
	return (::llc::err_t)gridOffset + gridBytes;
}
