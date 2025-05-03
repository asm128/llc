#include "llc_encoding.h"
#include "llc_view_bit.h"
#include "llc_noise.h"
#include "llc_chrono.h"
#include "llc_parse.h"


#ifdef LLC_ATMEL
#	include <time.h>
#else
#	include <ctime>
#	include <random>
#endif

stxp const char HEX_DIGITS_UPPER	[]	= "0123456789ABCDEF";
stxp const char HEX_DIGITS_LOWER	[]	= "0123456789abcdef";

::llc::error_t			llc::saltDataSalt				(vcu0_c & binary, ::llc::au0_t & salted)				{
	llc_necs(salted.resize(binary.size() * 2));
	uint8_t						* pSalted						= salted.begin();
	const uint8_t				* pBinary						= binary.begin();
	for(uint32_t iBinary = 0; iBinary < binary.size(); ++iBinary) {
		pSalted[iBinary * 2]		= pBinary[iBinary];
		pSalted[iBinary * 2 + 1]	= (::llc::noise1DBase(::llc::timeCurrentInUs()) + ::llc::timeCurrentInUs()) & 0xFF;
	}
	return 0;
}

::llc::error_t			llc::saltDataUnsalt				(vcu0_c & salted, ::llc::au0_t & binary)				{
	llc_necs(binary.resize(salted.size() / 2));
	const uint8_t				* pSalted						= salted.begin();
	uint8_t						* pBinary						= binary.begin();
	for(uint32_t iBinary = 0; iBinary < binary.size(); ++iBinary)
		pBinary[iBinary]		= pSalted[iBinary * 2];
	return 0;
}

static	::llc::error_t	hexFromByte		(uint8_t byteValue, char * hexed)																{
	hexed[0] = HEX_DIGITS_UPPER[byteValue >> 4];
	hexed[1] = HEX_DIGITS_UPPER[byteValue & 0xF];
	return 0;
}

static	::llc::error_t	hexToByte		(const char* s, uint8_t & byte)															{
	char						temp [3]		= {s[0], s[1]};
	llc_necs(llc::parseIntegerHexadecimal(::llc::vcst_t{temp}, byte));
	return 0;
}

static	::llc::error_t	hexToByte		(const char* s, int8_t & byte)															{
	char						temp [3]		= {s[0], s[1]};
	llc_necs(llc::parseIntegerHexadecimal(::llc::vcst_t{temp}, byte));
	return 0;
}

::llc::error_t			llc::hexEncode					(vcu0_c & in_binary, ::llc::asc_t & out_hexed	)	{
	uint32_t					offset							= out_hexed.size();
	llc_necs(out_hexed.resize(offset + in_binary.size() * 2));
	char						* pHexed						= out_hexed.begin();
	const uint8_t				* pBinary						= in_binary.begin();
	for(uint32_t iByte = 0; iByte < in_binary.size(); ++iByte)
		hexFromByte(pBinary[iByte], &pHexed[offset + iByte * 2]);
	return 0;
}

::llc::error_t			llc::hexDecode					(::llc::vcsc_c & in_hexed	, ::llc::au0_t & out_binary)	{
	uint32_t					offset							= out_binary.size();
	uint32_t					binarySize						= in_hexed.size() >> 1;
	llc_necs(out_binary.resize(offset + binarySize));
	const char					* pHexed						= in_hexed.begin();
	uint8_t						* pBinary						= out_binary.begin();
	for(uint32_t iByte = 0; iByte < binarySize; ++iByte)
		hexToByte(&pHexed[iByte * 2], pBinary[offset + iByte]);
	return 0;
}

::llc::error_t			llc::hexDecode					(::llc::vcsc_c & in_hexed	, ::llc::as0_t & out_binary)	{
	uint32_t					offset							= out_binary.size();
	uint32_t					binarySize						= in_hexed.size() >> 1;
	llc_necs(out_binary.resize(offset + binarySize));
	const char					* pHexed						= in_hexed.begin();
	int8_t						* pBinary						= out_binary.begin();
	for(uint32_t iByte = 0; iByte < binarySize; ++iByte)
		hexToByte(&pHexed[iByte * 2], pBinary[offset + iByte]);
	return 0;
}

::llc::error_t			llc::ardellEncode				(as2_t & cache, vcu0_c & input, uint64_t key, bool salt, ::llc::au0_t & output)						{
	// Originally written by Gary Ardell as Visual Basic code. free from all copyright restrictions.
	char						saltValue		[4]				= {};
	if (salt)
		for (int32_t i = 0; i < 4; i++) {
			int32_t					t								= 100 * (1 + saltValue[i]) * rand() * (((int32_t)time(0)) + 1);
			saltValue[i]		= t % 256;
		}
	const int32_t				keyFinal[8]						=
	{ (int32_t)(11 + (key % 233))
	, (int32_t)( 7 + (key % 239))
	, (int32_t)( 5 + (key % 241))
	, (int32_t)( 3 + (key % 251))
	};
	int32_t						n								= salt ? input.size() + 4 : input.size();
	llc_necs(cache.resize(n));
	int32_t						* sn							= cache.begin();
	if(salt) {
		for(int32_t i = 0; i < 2; ++i)
			sn[i]					= saltValue[i];
		for(int32_t i = 0; i < n - 4; ++i)
			sn[2 + i]				= input[i];
		for(int32_t i = 0; i < 2; ++i)
			sn[2 + n + i]			= saltValue[2 + i];
	}
	else
		for(int32_t i = 0; i < n; ++i)
			sn[i]					= input[i];
	int32_t						i;
	for(i = 1		; i  < n; ++i) sn[i] = sn[i] ^ sn[i - 1] ^ ((keyFinal[0] * sn[i - 1]) % 256);
	for(i = n - 2	; i >= 0; --i) sn[i] = sn[i] ^ sn[i + 1] ^  (keyFinal[1] * sn[i + 1]) % 256 ;
	for(i = 2		; i  < n; ++i) sn[i] = sn[i] ^ sn[i - 2] ^  (keyFinal[2] * sn[i - 1]) % 256 ;
	for(i = n - 3	; i >= 0; --i) sn[i] = sn[i] ^ sn[i + 2] ^  (keyFinal[3] * sn[i + 1]) % 256 ;

	uint32_t					outputOffset					= output.size();
	llc_necs(output.resize(outputOffset + n));
	uint8_t					* outputFast					= output.begin();
	for( i = 0; i < n; ++i)
		outputFast[outputOffset + i]	= (char)sn[i];
	return 0;
}

::llc::error_t			llc::ardellDecode				(::llc::as2_t & cache, vcu0_c & input, uint64_t key, bool salt, ::llc::au0_t & output)		{
	// Originally written by Gary Ardell as Visual Basic code. free from all copyright restrictions.
	const int32_t				keyFinal[8]						=
		{ (int32_t)(11 + (key % 233))
		, (int32_t)( 7 + (key % 239))
		, (int32_t)( 5 + (key % 241))
		, (int32_t)( 3 + (key % 251))
		};
	int32_t						n								= (int32_t)input.size();

	llc_necs(cache.resize(n));
	int32_t						* sn							= cache.begin();
	int32_t						i;
	for(i = 0		; i  < n	; ++i)	sn[i]			= input[i];
	for(i = 0		; i  < n - 2; ++i)	sn[i]			= sn[i] ^ sn[i + 2] ^ (keyFinal[3] * sn[i + 1]) % 256;
	for(i = n - 1	; i >= 2	; --i)	sn[i]			= sn[i] ^ sn[i - 2] ^ (keyFinal[2] * sn[i - 1]) % 256;
	for(i = 0		; i  < n - 1; ++i)	sn[i]			= sn[i] ^ sn[i + 1] ^ (keyFinal[1] * sn[i + 1]) % 256;
	for(i = n - 1	; i >= 1	; --i)	sn[i]			= sn[i] ^ sn[i - 1] ^ (keyFinal[0] * sn[i - 1]) % 256;

	uint32_t					outputOffset					= output.size();
	u2_c				finalStringSize					= salt ? n - 4 : n;
	const ::llc::vci32			finalValues						= {salt ? &sn[2] : sn, finalStringSize};
	llc_necs(output.resize(outputOffset + finalStringSize));
	uint8_t						* outputFast					= output.begin();
	const int32_t				* finalValuesFast				= finalValues.begin();
	for( i = 0; i < (int32_t)finalStringSize; ++i)
		outputFast[outputOffset + i]	= (char)finalValuesFast[i];
	return 0;
}

::llc::error_t			llc::utf8FromCodePoint			(uint32_t codePoint, ::llc::asc_t & hexDigits) {
	u2_c				offset							= hexDigits.size();
	if (codePoint <= 0x7f) {
		hexDigits.resize(offset + 1);
		hexDigits[offset + 0]	= static_cast<char>(codePoint);
	} else {
		if (codePoint <= 0x7FF) {
			hexDigits.resize(offset + 2);
			hexDigits[offset + 1]	= static_cast<char>(0x80 | (0x3f & codePoint));
			hexDigits[offset + 0]	= static_cast<char>(0xC0 | (0x1f & (codePoint >> 6)));
		} else if (codePoint <= 0xFFFF) {
			hexDigits.resize(offset + 3);
			hexDigits[offset + 2]	= static_cast<char>(0x80 | (0x3f &  codePoint));
			hexDigits[offset + 1]	= static_cast<char>(0x80 | (0x3f & (codePoint >> 6)));
			hexDigits[offset + 0]	= static_cast<char>(0xE0 | (0x0f & (codePoint >> 12)));
		} else if (codePoint <= 0x10FFFF) {
			hexDigits.resize(offset + 4);
			hexDigits[offset + 3]	= static_cast<char>(0x80 | (0x3f &  codePoint));
			hexDigits[offset + 2]	= static_cast<char>(0x80 | (0x3f & (codePoint >> 6)));
			hexDigits[offset + 1]	= static_cast<char>(0x80 | (0x3f & (codePoint >> 12)));
			hexDigits[offset + 0]	= static_cast<char>(0xF0 | (0x07 & (codePoint >> 18)));
		}
	}
	return 0;
}

::llc::error_t			llc::digest				(vcu0_c & input, ::llc::au2_t & digest)		{
	uint32_t					x								= 0;
	::llc::au2_t					filtered						= {};
	for(uint32_t i = 0; i < input.size() - 8; ++i) {
		x	+= ::llc::noise1DBase32(input[i])
			+  ::llc::noise1DBase32(input[i + 1])
			+  ::llc::noise1DBase32(input[i + 2])
			+  ::llc::noise1DBase32(input[i + 3])
			+  ::llc::noise1DBase32(input[i + 4])
			+  ::llc::noise1DBase32(input[i + 5])
			+  ::llc::noise1DBase32(input[i + 6])
			+  ::llc::noise1DBase32(input[i + 7])
			;
		x												+= x ^ (x << 11);
		filtered.push_back(x);
	}
	for(uint32_t i = 0; i < filtered.size() - 8; ++i) {
		filtered[i]	^= ::llc::noise1DBase32(filtered[i])
					+  ::llc::noise1DBase32(filtered[i + 1])
					+  ::llc::noise1DBase32(filtered[i + 2])
					+  ::llc::noise1DBase32(filtered[i + 3])
					+  ::llc::noise1DBase32(filtered[i + 4])
					+  ::llc::noise1DBase32(filtered[i + 5])
					+  ::llc::noise1DBase32(filtered[i + 6])
					+  ::llc::noise1DBase32(filtered[i + 7])
					;
	}
	for(uint32_t i = 2; i < (filtered.size() - 32); i += 2) {
		for(uint32_t j = 0; j < 32; j++)
			filtered[j]										+= filtered[i + j];
	}
	digest.append({filtered.begin(), ::llc::min((uint32_t)32U, filtered.size())});
	return 0;
}

::llc::error_t			llc::digest						(vcu0_c & input, ::llc::asc_t & digest)		{
	u2_t						x								= 0;
	::llc::au2_t				filtered						= {};
	for(uint32_t i = 0; i < input.size() - 8; ++i) {
		x	+= ::llc::noise1DBase32(input[i])
			+  ::llc::noise1DBase32(input[i + 1])
			+  ::llc::noise1DBase32(input[i + 2])
			+  ::llc::noise1DBase32(input[i + 3])
			+  ::llc::noise1DBase32(input[i + 4])
			+  ::llc::noise1DBase32(input[i + 5])
			+  ::llc::noise1DBase32(input[i + 6])
			+  ::llc::noise1DBase32(input[i + 7])
			;
		x						+= x ^ (x << 11);
		filtered.push_back(x);
	}
	for(uint32_t i = 0; i < filtered.size() - 8; ++i) {
		filtered[i]	^= ::llc::noise1DBase32(filtered[i])
					+  ::llc::noise1DBase32(filtered[i + 1])
					+  ::llc::noise1DBase32(filtered[i + 2])
					+  ::llc::noise1DBase32(filtered[i + 3])
					+  ::llc::noise1DBase32(filtered[i + 4])
					+  ::llc::noise1DBase32(filtered[i + 5])
					+  ::llc::noise1DBase32(filtered[i + 6])
					+  ::llc::noise1DBase32(filtered[i + 7])
					;
	}
	for(uint32_t i = 2, count = (filtered.size() - 8); i < count; i += 2) {
		for(uint32_t j = 0; j < 8; j++)
			filtered[j]				+= filtered[i + j];
	}
	char						temp		[32]				= {};
	for(uint32_t i = 0; i < ::llc::min(filtered.size(), (uint32_t)8U); ++i) {
		snprintf(temp, ::llc::size(temp) - 2, "%" LLC_FMT_U2, filtered[i]);
		llc_necs(digest.append_string(temp));
	}
	return 0;
}
