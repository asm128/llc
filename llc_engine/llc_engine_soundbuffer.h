#include "llc_datatype.h"
#include "llc_enum.h"
#include "llc_linear_map_pobj.h"

#ifndef LLC_ENGINE_SOUNDBUFFER_H_23_05_08
#define LLC_ENGINE_SOUNDBUFFER_H_23_05_08

namespace llc
{
	GDEFINE_ENUM_TYPE(SOUND_BUFFER_MODE, uint8_t);
	GDEFINE_ENUM_VALUE(SOUND_BUFFER_MODE, Loop				,   0);
	GDEFINE_ENUM_VALUE(SOUND_BUFFER_MODE, Discard			,   1);
	GDEFINE_ENUM_VALUE(SOUND_BUFFER_MODE, Bitfield			, 100);

#pragma pack(push, 1)
	struct SSoundBufferDescription {
		::llc::SDataTypeID							Format				= {};
		SOUND_BUFFER_MODE							Usage				= {};
	};
#pragma pack(pop)

	struct SSoundBuffer {
		SSoundBufferDescription						Desc			= {};
		::llc::au0_t									Data			= {};

		::llc::error_t								Save			(::llc::apod<uint8_t> & output)	const	{ 
			llc_necs(llc::savePOD	(output, Desc));
			llc_necs(llc::saveView(output, Data));
			return 0; 
		}

		::llc::error_t								Load			(::llc::vcu0_t & input)	{ 
			llc_necs(llc::loadPOD (input, Desc));
			llc_necs(llc::loadView(input, Data));
			return 0;
		}
	};

	typedef	::llc::llc_linear_map_pobj<::llc::SSoundBuffer>	SSoundBufferManager;

} // namespace

#endif // LLC_ENGINE_SOUNDBUFFER_H_23_05_08
