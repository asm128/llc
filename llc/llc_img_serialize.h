#include "llc_image.h"
#include "llc_apod_serialize.h"

#ifndef LLC_IMG_SERIALIZE_H_23627
#define LLC_IMG_SERIALIZE_H_23627

namespace llc
{
	tplt<tpnm TPod>
	error_t				saveImage					(au0_t & output, const grid<TPod> & input) {
		if_fail_fe(savePOD(output, input.metrics()));
		if_fail_fe(saveView(output, view<const TPod>{input.begin(), input.size()}));
		return 0;
	}

	tplt<tpnm TPod>
	error_t				loadImage					(vcu0_t & input, img<TPod> & output) {
		vcu0_t				inputToRead				= input;
		n2u2_t					metrics						= {};
		if_fail_fe(loadPOD(inputToRead, metrics));
		view<cnst TPod>			texels						= {};
		err_t					bytesRead					= 0;
		if_fail_fe(bytesRead = viewRead(texels, inputToRead));
		cnst u3_t				texelCount					= (u3_t)metrics.x * metrics.y;
		if_true_fef(texelCount != texels.size(), "Image metrics: %u x %u. Texel count: %u.", metrics.x, metrics.y, texels.size());
		if_fail_fe(inputToRead.slice(inputToRead, bytesRead));
		if_fail_fe(output.Texels.resize(texels.size()));
		if(texels.size())
			memcpy(output.Texels.begin(), texels.begin(), texels.byte_count());
		output.View					= {output.Texels.begin(), metrics};
		input						= inputToRead;
		return 0;
	}

} // namespace

#endif // LLC_IMG_SERIALIZE_H_23627
