#include "llc_img_color.h"
#include "llc_array_static.h"
#include "llc_color_type.h"
#include "llc_apod_color.h"
#include "llc_array.h"
#include "llc_grid_color.h"

#ifndef LLC_PNG_H_23627
#define LLC_PNG_H_23627

namespace llc
{
#pragma pack(push, 1)
	struct SPNGChunk {
		uint8_t			Type				[4]	= {};
		uint32_t		CRC						= {};
		::llc::au0_t	Data					= {};
	};


	struct SPNGIHDR {
		::llc::n2u2_t	Size					= {};
		int8_t			BitDepth				= 0;
		COLOR_TYPE		ColorType				= COLOR_TYPE_GRAYSCALE;
		int8_t			MethodCompression		= 0;
		int8_t			MethodFilter			= 0;
		int8_t			MethodInterlace			= 0;
	};

	GDEFINE_ENUM_TYPE(PNG_TAG, uint8_t);
	GDEFINE_ENUM_VALUE(PNG_TAG, tEXt ,  0);
	GDEFINE_ENUM_VALUE(PNG_TAG, zTXt ,  1);
	GDEFINE_ENUM_VALUE(PNG_TAG, bKGD ,  2);
	GDEFINE_ENUM_VALUE(PNG_TAG, cHRM ,  3);
	GDEFINE_ENUM_VALUE(PNG_TAG, dSIG ,  4);
	GDEFINE_ENUM_VALUE(PNG_TAG, eXIf ,  5);
	GDEFINE_ENUM_VALUE(PNG_TAG, gAMA ,  6);
	GDEFINE_ENUM_VALUE(PNG_TAG, hIST ,  7);
	GDEFINE_ENUM_VALUE(PNG_TAG, iCCP ,  8);
	GDEFINE_ENUM_VALUE(PNG_TAG, iTXt ,  9);
	GDEFINE_ENUM_VALUE(PNG_TAG, pHYs , 10);
	GDEFINE_ENUM_VALUE(PNG_TAG, sBIT , 11);
	GDEFINE_ENUM_VALUE(PNG_TAG, sPLT , 12);
	GDEFINE_ENUM_VALUE(PNG_TAG, sRGB , 13);
	GDEFINE_ENUM_VALUE(PNG_TAG, sTER , 14);
	GDEFINE_ENUM_VALUE(PNG_TAG, tIME , 15);
	GDEFINE_ENUM_VALUE(PNG_TAG, tRNS , 16);
	GDEFINE_ENUM_VALUE(PNG_TAG, fcTL , 17);
	GDEFINE_ENUM_VALUE(PNG_TAG, fdAT , 18);
	GDEFINE_ENUM_VALUE(PNG_TAG, acTL , 19);
	GDEFINE_ENUM_VALUE(PNG_TAG, COUNT, 20);
	
	struct SPNGData {
		sc_t							Signature	[8]	= {};
		::llc::aobj<SPNGChunk>			Chunks			;
		::llc::au0_t					Deflated		;
		::llc::au0_t					Inflated		;
		::llc::au0_t					Filters			;
		::llc::aau8						Scanlines		;
		::llc::a8bgr					Palette			;
		::llc::n2u2_t					Adam7Sizes	[7]	= {};
		//::llc::SPNGFeature				Feature			= {};
		::llc::SPNGIHDR					Header			= {};
		::llc::asti32<PNG_TAG_COUNT>	Feature			;
	};
#pragma pack(pop)

	uint32_t				update_crc	(const ::llc::vcu0_t & buf, uint32_t crc)		;
	stainli	uint32_t		get_crc		(const ::llc::vcu0_t & buf)					{ return update_crc(buf, 0xffffffffL) ^ 0xffffffffL; }

	::llc::error_t			pngDecode	(::llc::SPNGData & pngData, ::llc::g8bgra	out_View);
	::llc::error_t			pngDecode	(::llc::SPNGData & pngData, ::llc::gu16		out_View);
	::llc::error_t			pngDecode	(::llc::SPNGData & pngData, ::llc::gu8		out_View);
	::llc::error_t			pngDecode	(::llc::SPNGData & pngData, ::llc::au0_t	& out_Data);
	::llc::error_t			pngDecode	(::llc::SPNGData & pngData, ::llc::au0_t	& out_Data, ::llc::g8bgra & out_View); 
	::llc::error_t			pngDecode	(::llc::SPNGData & pngData, ::llc::au0_t	& out_Data, ::llc::gu16   & out_View); 
	::llc::error_t			pngDecode	(::llc::SPNGData & pngData, ::llc::au0_t	& out_Data, ::llc::gu8    & out_View); 
	::llc::error_t			pngDecode	(::llc::SPNGData & pngData, ::llc::img8bgra & out_Image);
	::llc::error_t			pngDecode	(::llc::SPNGData & pngData, ::llc::imgu16	& out_Image);
	::llc::error_t			pngDecode	(::llc::SPNGData & pngData, ::llc::imgu8	& out_Image);
	
	::llc::error_t			pngFileLoad	(::llc::SPNGData & pngData, const ::llc::vcst_t	& filename	);
	::llc::error_t			pngFileLoad	(::llc::SPNGData & pngData, const ::llc::vcu0_t	& source	);
	::llc::error_t			pngFileLoad	(::llc::SPNGData & pngData, const ::llc::vcst_t	& filename	, ::llc::img8bgra & out_Texture)	;
	::llc::error_t			pngFileLoad	(::llc::SPNGData & pngData, const ::llc::vcu0_t	& source	, ::llc::img8bgra & out_Texture)	;
	::llc::error_t			pngFileLoad	(::llc::SPNGData & pngData, const ::llc::vcst_t	& filename	, ::llc::au0_t & out_Data, ::llc::g8bgra & out_View)	;
	::llc::error_t			pngFileLoad	(::llc::SPNGData & pngData, const ::llc::vcu0_t	& source	, ::llc::au0_t & out_Data, ::llc::g8bgra & out_View)	;
	::llc::error_t			pngFileLoad	(::llc::SPNGData & pngData, const ::llc::vcst_t	& filename	, ::llc::au0_t & out_Data)	;
	::llc::error_t			pngFileLoad	(::llc::SPNGData & pngData, const ::llc::vcu0_t	& source	, ::llc::au0_t & out_Data)	;

	::llc::error_t			pngFileWrite(const ::llc::gc8bgra & out_ImageView, ::llc::au0_t & out_Bytes);

	stainli	::llc::error_t	pngFileLoad	(const ::llc::vcst_t	& filename	, ::llc::img8bgra & out_Texture)					{ ::llc::SPNGData tempCache; return pngFileLoad(tempCache, filename	, out_Texture); }
	stainli	::llc::error_t	pngFileLoad	(const ::llc::vcu0_t	& source	, ::llc::img8bgra & out_Texture)					{ ::llc::SPNGData tempCache; return pngFileLoad(tempCache, source	, out_Texture); }
	stainli	::llc::error_t	pngFileLoad	(const ::llc::vcst_t	& filename	, ::llc::au0_t & out_Data, ::llc::g8bgra & out_View)	{ ::llc::SPNGData tempCache; return pngFileLoad(tempCache, filename	, out_Data, out_View); }
	stainli	::llc::error_t	pngFileLoad	(const ::llc::vcu0_t	& source	, ::llc::au0_t & out_Data, ::llc::g8bgra & out_View)	{ ::llc::SPNGData tempCache; return pngFileLoad(tempCache, source	, out_Data, out_View); }
	stainli	::llc::error_t	pngFileLoad	(const ::llc::vcst_t	& filename	, ::llc::au0_t & out_Data)							{ ::llc::SPNGData tempCache; return pngFileLoad(tempCache, filename	, out_Data); }
	stainli	::llc::error_t	pngFileLoad	(const ::llc::vcu0_t	& source	, ::llc::au0_t & out_Data)							{ ::llc::SPNGData tempCache; return pngFileLoad(tempCache, source	, out_Data); }
} // namespace

#endif // LLC_PNG_H_23627
