#include "llc_array_obj.h"
#include "llc_lifecycle.h"
#include "llc_range.h"
#include "llc_string.h"

#ifndef LLC_DEFLATE_H_23627
#define LLC_DEFLATE_H_23627

namespace llc
{
#if defined(LLC_ESP32) || defined(LLC_ARDUINO)
	stxp	::llc::u2_t		DEFLATE_DEFAULT_CHUNK_SIZE		= 1024 * 4;
	stxp	::llc::u2_t		INFLATE_DEFAULT_CHUNK_SIZE		= 1024 * 4;
#else
	stxp	::llc::u2_t		DEFLATE_DEFAULT_CHUNK_SIZE		= 1024 * 32;
	stxp	::llc::u2_t		INFLATE_DEFAULT_CHUNK_SIZE		= 1024 * 32;
#endif
	struct SDeflateSettings {
		u2_t				SearchDepth			= 256;
		u0_t				WindowBits			= 15;
		u0_t				HashBits				= 16;
		u0_t				CompressionLevel		= 3;
	};

	struct SZipStream {
		apod<u0_t>			Window, Previous, Head;
		vcu0_t				Stream;
		SDeflateSettings	Settings				= {};
		LIFECYCLE_STATE		Status					= LIFECYCLE_STATE_IDLE;
	};

	struct SDeflateMatch {
		rangeu3_t			Match					= {};
		rangeu3_t			PreviousMatch			= {};
	};

	err_t			deflateInit			(SZipStream & output, vcu0_t input, SDeflateSettings settings = {});
	err_t			inflateInit			(SZipStream & output, vcu0_t input);

	err_t			arrayDeflate			(vcu0_t inflated, au0_t & deflated, SDeflateSettings settings, u2_c chunkSize = DEFLATE_DEFAULT_CHUNK_SIZE);
	err_t			arrayDeflate			(vcu0_t inflated, au0_t & deflated, u2_c chunkSize = DEFLATE_DEFAULT_CHUNK_SIZE);
	err_t			arrayInflate			(vcu0_t deflated, au0_t & inflated, u2_c chunkSize = INFLATE_DEFAULT_CHUNK_SIZE);

	stin	err_t	arrayDeflate			(vcs0_t inflated, au0_t & deflated, u2_c chunkSize = DEFLATE_DEFAULT_CHUNK_SIZE)	{ rtrn arrayDeflate(*(vcu0_t*)&inflated, deflated, chunkSize); }
	stin	err_t	arrayInflate			(vcs0_t deflated, au0_t & inflated, u2_c chunkSize = INFLATE_DEFAULT_CHUNK_SIZE)	{ rtrn arrayInflate(*(vcu0_t*)&deflated, inflated, chunkSize); }
	stin	err_t	arrayDeflate			(vcu0_t inflated, as0_t & deflated, u2_c chunkSize = DEFLATE_DEFAULT_CHUNK_SIZE)	{ rtrn arrayDeflate(inflated, *(au0_t*)&deflated, chunkSize); }
	stin	err_t	arrayInflate			(vcu0_t deflated, as0_t & inflated, u2_c chunkSize = INFLATE_DEFAULT_CHUNK_SIZE)	{ rtrn arrayInflate(deflated, *(au0_t*)&inflated, chunkSize); }
	stin	err_t	arrayDeflate			(vcs0_t inflated, as0_t & deflated, u2_c chunkSize = DEFLATE_DEFAULT_CHUNK_SIZE)	{ rtrn arrayDeflate(*(vcu0_t*)&inflated, *(au0_t*)&deflated, chunkSize); }
	stin	err_t	arrayInflate			(vcs0_t deflated, as0_t & inflated, u2_c chunkSize = INFLATE_DEFAULT_CHUNK_SIZE)	{ rtrn arrayInflate(*(vcu0_t*)&deflated, *(au0_t*)&inflated, chunkSize); }

	err_t			crcGenerate				(vcu0_t bytes, u3_t & crc);
	err_t			crcVerifyAndRemove		(au0_t & bytes);
	err_t			crcGenerateAndAppend	(au0_t & bytes);
} // namespace

#endif // LLC_DEFLATE_H_23627
