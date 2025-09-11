#include "llc_log.h"

#ifndef LLC_CIRCUIT_H
#define LLC_CIRCUIT_H

namespace llc 
{
	::llc::err_t circuitVoltageDivider	(::llc::u2_t milliVIn, ::llc::u3_t milliOhmR1, ::llc::u3_t milliOhmR2, ::llc::u2_t & milliVOut);
	//::llc::err_t circuitVoltageDrop		(::llc::u2_t milliAmp, ::llc::u3_t milliOhmR1, ::llc::u2_t & milliVDrop);
} // namespace 

#endif // LLC_CIRCUIT_H
