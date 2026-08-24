#define STDOUT

#include "llc_circuit.h"

LLC_USING_TYPEINT();

sttc int test_circuitResistorCalc() {
	u2_c milliVSource = 4500, milliVDrop = 3300, milliAmp = 30;
	u3_t milliOhm = 0;
	always_printf("milliVSource = %u, milliVDrop = %u", milliAmp, milliVSource, milliVDrop);
	always_printf("milliAmp = %u", milliAmp);
	if_fail_fe(llc::circuitResistorCalc(milliAmp, milliVSource, milliVDrop, milliOhm));
	always_printf("milliOhm = %u", milliOhm);
	rtrn 0;
}

sttc int test_ledResistorArray() {
	u2_c milliVSource = 5000, milliVDrop = 3300, milliAmpLed = 15;
	always_printf("milliVSource = %u, milliVDrop = %u, milliAmpLed = %u", milliVSource, milliVDrop, milliAmpLed);
	{
		u3_t milliOhm = 0;
		if_true_fef(0 <= llc::circuitResistorCalc(0, milliVSource, milliVDrop, milliOhm), "%s", "Should fail on 0A input");
		always_printf("milliAmp = %u, milliOhm = %llu", 0, milliOhm);
	}
	for(u2_t iLedCount = 1; iLedCount <= 8; ++iLedCount) {
		u2_c milliAmp = iLedCount * milliAmpLed;
		u3_t milliOhm = 0;
		if_fail_ce(llc::circuitResistorCalc(milliAmp, milliVSource, milliVDrop, milliOhm));
		always_printf("iLedCount = %u, milliAmp = %u, milliOhm = %u", iLedCount, milliAmp, milliOhm);
	}
	rtrn 0;
}

sttc int test_circuitParallelResistor() {
	::llc::u3_c milliOhmR1 = 123000, milliOhmR2 = 456000;
	::llc::u3_c milliOhmExpected = u3_c(1000.0 / ((1000.0 / milliOhmR1) + (1000.0 / milliOhmR2)));
	always_printf("milliOhmR1 = %llu, milliOhmR2 = %llu, milliOhmExpected = %llu", milliOhmR1, milliOhmR2, milliOhmExpected);
	::llc::u3_t milliOhmTotal = 0;
	if_fail_fe(::llc::circuitParallelResistor(milliOhmR1, milliOhmR2, milliOhmTotal));
	if_true_fef(milliOhmTotal != milliOhmExpected, "milliOhmTotal = %llu", milliOhmTotal);
	always_printf("milliOhmTotal = %llu", milliOhmTotal);
	rtrn 0;
}

#if defined(LLC_WINDOWS)
static	::llc::error_t	test_base_log_write	(const char * text, uint32_t /*textLen*/) {	OutputDebugStringA(text); return (::llc::error_t)printf("%s", text); }
#elif defined(LLC_ANDROID)
static	::llc::error_t	test_base_log_write	(const char * text, uint32_t textLen) {	LOGI("%s", text); return (::llc::error_t)textLen; }
#elif defined(LLC_ARDUINO)
static	::llc::error_t	test_base_log_write	(const char * text, uint32_t textLen) {	return ::llc::error_t(Serial ? (::llc::error_t)Serial.write(text, textLen) : (::llc::error_t)textLen); }
#else
static	::llc::error_t	test_base_log_write	(const char * text, uint32_t textLen) {	(void)textLen; return (::llc::error_t)printf_s("%s", text, textLen); }
#endif

#if defined(LLC_WINDOWS)
static	::llc::error_t	test_base_log_print	(const char * text) {	OutputDebugStringA(text); return (::llc::error_t)printf("%s", text); }
#elif defined(LLC_ANDROID)z
static	::llc::error_t	test_base_log_print	(const char * text) {	LOGI("%s", text); return (::llc::error_t)strlen(text); }
#elif defined(LLC_ARDUINO)
static	::llc::error_t	test_base_log_print	(const char * text) {	return Serial ? (::llc::error_t)Serial.print(text) : (::llc::error_t)strlen(text); }
#else
static	::llc::error_t	test_base_log_print	(const char * text) {	return (::llc::error_t)printf("%s", text); }
#endif

int main() {
	::llc::setupLogCallbacks(test_base_log_print, test_base_log_write);
	if_fail_e(test_circuitResistorCalc());
	if_fail_e(test_ledResistorArray());
	if_fail_e(test_circuitParallelResistor());
	return 0; 
}

// So I bought:
// | ARS	| Total amount	| Product
//  3657.00	-	1590 g		- Puré de tomate La Campagnola 530g
// 11996.00	-	   4 L		- Leche La Serenísima Protein 1L
//  5624.17	-	 632 g		- Cremoso x kg
//  9832.80	-	 544 g		- Cremón La Serenísima 
//  6975.00	-	 1.5 L		- Aceite de girasol Natura 1.5L
//  3072.08 -	 176 g		- Jamon Cocido Feteado (got the wrong one in the rush, missed the discount on Bocatti)
//  4624.00	-	 578 g		- Tortuga de cerdo
// 13023.12	-	 880 g		- Tapa de nalga Estancia
//  4432.00	-	 800 g		- Lentejón COTO 400g
//	2800.00	-	 440 g		- Aceitunas Nucete 220g
// -------------------
//
// 66036.17 - Subtotal 
// -9817.76	- Discounts
// -------------------
//
// 56218.41 - Final supermarket
// 
// Plus
// | ARS	| Total amount	| Product
//  5000	-	  30 u		- Egg
//			-	   4 u		- Banana Ecuador (around 880g total)
//			-	   3 u		- Cebolla de verdeo
//	3326	-				- Total veggie shop
// -------------------
//
//  8326	- Final veggie shop
// -------------------
//
//  8326.00	- Final veggie shop
// 56218.41 - Final supermarket
// -------------------
// 64607.41 - Total food expenses in ARS
//
