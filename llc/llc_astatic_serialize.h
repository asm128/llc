#include "llc_array_static.h"

#include "llc_view_serialize.h"

#ifndef LLC_ASTATIC_SERIALIZE_H_23627
#define LLC_ASTATIC_SERIALIZE_H_23627

namespace llc
{
	tplt<tpnm _tPOD, uint32_t _nSize> 
	::llc::error_t			loadView					(::llc::vcu0_t & input, ::llc::astatic<_tPOD, _nSize> & output) { 
		::llc::view<const _tPOD>	readView					= {}; 
		uint32_t					bytesRead					= 0;
		llc_necs(bytesRead = ::llc::viewRead(readView, input)); 
		llc_necs(input.slice(input, bytesRead));
		memcpy(output.begin(), readView.begin(), ::llc::min(readView.byte_count(), ::llc::view<_tPOD>{output}.byte_count()));
		return 0;
	}
} // namespace

#endif // LLC_ASTATIC_SERIALIZE_H_23627
