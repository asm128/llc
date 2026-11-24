#include "llc_array_static.h"

#include "llc_view_serialize.h"

#ifndef LLC_ASTATIC_SERIALIZE_H_23627
#define LLC_ASTATIC_SERIALIZE_H_23627

namespace llc
{
	tplt<tpnm _tPOD, ::llc::u2_t _nSize> 
	::llc::err_t			loadView					(::llc::vcu0_t & input, ::llc::astatic<_tPOD, _nSize> & output) { 
		::llc::view<const _tPOD>	readView					= {}; 
		::llc::u2_t					bytesRead					= 0;
		if_fail_fe(bytesRead = ::llc::viewRead(readView, input)); 
		if_fail_fe(input.slice(input, bytesRead));
		memcpy(output.begin(), readView.begin(), ::llc::min(readView.byte_count(), ::llc::view<_tPOD>{output}.byte_count()));
		return 0;
	}
} // namespace

#endif // LLC_ASTATIC_SERIALIZE_H_23627
