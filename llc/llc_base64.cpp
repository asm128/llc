#include "llc_base64.h"
#include "llc_view_bit.h"

static	::llc::error_t	base64EncodeTriplet								(::llc::vcsc_c & base64Symbols, ::llc::vu8 inputTriplet, ::llc::vu8 out_base64) {
	for (uint32_t iSingleIn = 0; iSingleIn < 3; ++iSingleIn) { // reverse bits of each input byte
		::llc::view_bit<uint8_t>									inputBits														= {&inputTriplet[iSingleIn], 8};
		::llc::reverse_bits(inputBits);
	}
	const ::llc::view_bit<uint8_t>								inputBits														= {inputTriplet.begin(), 24};
	uint32_t					iBitIn															= 0;
	uint8_t														outputQuad[4]													= {};
	for (uint32_t iSingleOut = 0; iSingleOut < ::llc::size(outputQuad); ++iSingleOut) { // encode to four output bytes
		::llc::view_bit<uint8_t>									outputBits														= {&outputQuad[iSingleOut], 6};
		for (uint32_t iBitOut = 0; iBitOut < 6; ++iBitOut) // copy the relevant input bits to the output bytes
			outputBits[iBitOut]										= (bool)inputBits[iBitIn++];
		::llc::reverse_bits(outputBits);
	}
	for (uint32_t iSingleOut = 0; iSingleOut < ::llc::size(outputQuad); ++iSingleOut) { // encode to four output bytes
		if (outputQuad[iSingleOut] >= base64Symbols.size()) {
			error_printf("%s", "Out of range. This could happen if the algorithm or the input character table got broken.");
			out_base64[iSingleOut]									= (uint8_t)-1;
		}
		else
			out_base64[iSingleOut]									= base64Symbols[outputQuad[iSingleOut]];
	}
	return 0;
}
::llc::error_t			llc::base64Encode								(::llc::vcsc_c & base64Symbols, char base64PadSymbol, vcu0_c & inputBytes, ::llc::au0_t & out_base64)	{
	rni_if(0 == inputBytes.size(), "%s", "Empty input stream.");
	u2_c				packsNeeded														= inputBytes.size() / 3 + one_if(inputBytes.size() % 3);
	uint32_t					iOutput64														= out_base64.size(); //
	llc_necall(out_base64.resize(out_base64.size() + packsNeeded * 4), "Out of memory? out_base64.size() : %" LLC_FMT_U2 ". packsNeeded : %" LLC_FMT_U2 ".", iOutput64, packsNeeded);
	for(uint32_t iInputByte = 0, inputByteCount = inputBytes.size(); iInputByte < inputByteCount; iInputByte += 3) { // process each byte triplet and turn it into 4 bytes padded with 2 bit
		const bool													use1															= ((int32_t)iInputByte) < (((int32_t)inputByteCount) - 1);
		const bool													use2															= ((int32_t)iInputByte) < (((int32_t)inputByteCount) - 2);
		uint8_t														inputTriplet	[3]												=
			{		 (uint8_t)(inputBytes[iInputByte]		& 0xFF)
			, use1 ? (uint8_t)(inputBytes[iInputByte + 1]	& 0xFF): (uint8_t)0U
			, use2 ? (uint8_t)(inputBytes[iInputByte + 2]	& 0xFF): (uint8_t)0U
			};
		::base64EncodeTriplet(base64Symbols, inputTriplet, {&out_base64[iOutput64], 4});
		iOutput64												+= 4;
	}
	u2_c				modTriplet														= inputBytes.size() % 3;
	if(0 != modTriplet) { // pad with '='
		out_base64[out_base64.size() - 1]						= base64PadSymbol;
		if(1 == modTriplet) // pad with another '='
			out_base64[out_base64.size() - 2]						= base64PadSymbol;
	}
	llc_necs(out_base64.push_back(0)); // add a null byte at the end for safety.
	llc_necs(out_base64.resize(out_base64.size() - 1)); // restore the count to ignore the null byte just added.
	return 0;
}

static	::llc::error_t	base64DecodeQuad												(::llc::vu8 inputQuad, ::llc::vu8 outputBytes)	{
	for(uint32_t iSingleIn = 0; iSingleIn < 4; ++iSingleIn) { // reverse bits before extracting
		::llc::view_bit<uint8_t>									inputBits														= {&inputQuad[iSingleIn], 6};
		::llc::reverse_bits(inputBits);
	}
	const ::llc::view_bit<uint8_t>								inputBits														= {inputQuad.begin(), inputQuad.size() * 8};
	uint8_t														outputTriplet		[3]											= {};
	uint32_t					iBitIn															= 0;
	for(uint32_t iSingleOut = 0; iSingleOut < 3; ++iSingleOut) {
		::llc::view_bit<uint8_t>									outputBits														= {&outputTriplet[iSingleOut], 8};
		for(uint32_t iBitOut = 0; iBitOut < 8; ++iBitOut) { // extract relevant bits from encoded bytes
			outputBits[iBitOut]										= inputBits[iBitIn + (iBitIn / 6) * 2];
			++iBitIn;
		}
		::llc::reverse_bits(outputBits);
		outputBytes[iSingleOut]								= outputTriplet[iSingleOut];
	}
	return 0;
}

::llc::error_t			llc::base64Decode								(::llc::vcsc_c & base64Symbols, char base64PadSymbol, vcu0_c & in_base64, ::llc::au0_t & outputBytes)	{
	rni_if(0 == in_base64.size(), "%s", "Empty base64 string.");
	int32_t														lengthInput														= in_base64.size();
	if(uint32_t mymod = in_base64.size() % 4) {
		ree_if((mymod != 1) || (in_base64.size() && in_base64[in_base64.size() - 1] != 0), "%s", "Invalid base64 string.");
		lengthInput												= in_base64.size() - 1;
	}
	// --- Generate symbol value remap table.
	uint8_t														base64SymbolRemap	[128]										= {};
	base64SymbolRemap[(uint32_t)base64PadSymbol]			= 0;
	for(uint32_t iSymbol = 0; iSymbol < 64; ++iSymbol)
		base64SymbolRemap[(uint32_t)base64Symbols[iSymbol]]		= (uint8_t)iSymbol;

	// --- Decode input symbols
	u2_c				packsNeeded														= lengthInput / 4;
	uint32_t					iOutputByte														= outputBytes.size();
	llc_necall(outputBytes.resize(outputBytes.size() + packsNeeded * 3), "Out of memory? outputBinary.size() : %" LLC_FMT_U2 ". packsNeeded : %" LLC_FMT_U2 ".", iOutputByte, packsNeeded); // Append result
	for(uint32_t iInput64 = 0, symbolCount = lengthInput; iInput64 < symbolCount; iInput64 += 4) {
		uint8_t														inputQuad	[4]													=
			{ base64SymbolRemap[((uint8_t)in_base64[iInput64 + 0]) % ::llc::size(base64SymbolRemap)]
			, base64SymbolRemap[((uint8_t)in_base64[iInput64 + 1]) % ::llc::size(base64SymbolRemap)]
			, base64SymbolRemap[((uint8_t)in_base64[iInput64 + 2]) % ::llc::size(base64SymbolRemap)]
			, base64SymbolRemap[((uint8_t)in_base64[iInput64 + 3]) % ::llc::size(base64SymbolRemap)]
			};
		::base64DecodeQuad(inputQuad, {&outputBytes[iOutputByte], 3});
		iOutputByte												+= 3;
	}
		 if(in_base64[in_base64.size() - 2] == base64PadSymbol) { outputBytes.resize(outputBytes.size() - 2); } // Remove leading nulls.
	else if(in_base64[in_base64.size() - 1] == base64PadSymbol) { outputBytes.resize(outputBytes.size() - 1); } // Remove leading nulls.
	return 0;
}
