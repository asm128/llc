#include "llc_circuit.h"

// R1 = 100kΩ (top)
// R2 = 100kΩ (bottom)
// Vout = Vin * R2 / (R1 + R2)
::llc::err_t llc::circuitVoltageDivider(::llc::u2_t milliVIn, ::llc::u3_t milliOhmR1, ::llc::u3_t milliOhmR2, ::llc::u2_t & milliVOut) {
	if_true_fe(0 == milliOhmR1 && 0 == milliOhmR2);
	milliVOut = ::llc::u2_t(milliVIn * milliOhmR2 / (milliOhmR1 + milliOhmR2));
	return (milliOhmR1 && milliOhmR2) ? 0 
		: milliOhmR1 ? 1 
		: 2
		; 
}
