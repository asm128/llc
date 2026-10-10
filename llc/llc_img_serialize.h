#include "llc_image.h"
#include "llc_apod_serialize.h"

#ifndef LLC_IMG_SERIALIZE_H_23627
#define LLC_IMG_SERIALIZE_H_23627

namespace llc
{
	tplt<tpnm TPod>
	::llc::error_t				saveImage					(::llc::au0_t & output, const ::llc::grid<TPod> & input) {
		llc_necs(llc::savePOD(output, input.metrics()));
		llc_necs(llc::saveView(output, ::llc::view<const TPod>{input.begin(), input.size()}));
		return 0;
	}

	tplt<tpnm TPod>
	::llc::error_t				loadImage					(::llc::vcu0_t & input, ::llc::img<TPod> & output) {
		::llc::vcu0_t				inputToRead				= input;
		::llc::n2u2_t					metrics						= {};
		llc_necs(llc::loadPOD(inputToRead, metrics));
		::llc::view<cnst TPod>			texels						= {};
		::llc::err_t					bytesRead					= 0;
		llc_necs(bytesRead = llc::viewRead(texels, inputToRead));
		cnst ::llc::u3_t				texelCount					= (::llc::u3_t)metrics.x * metrics.y;
		if_true_fef(texelCount != texels.size(), "Image metrics: %u x %u. Texel count: %u.", metrics.x, metrics.y, texels.size());
		llc_necs(inputToRead.slice(inputToRead, bytesRead));
		llc_necs(output.Texels.resize(texels.size()));
		if(texels.size())
			memcpy(output.Texels.begin(), texels.begin(), texels.byte_count());
		output.View					= {output.Texels.begin(), metrics};
		input						= inputToRead;
		return 0;
	}

} // namespace

#endif // LLC_IMG_SERIALIZE_H_23627
