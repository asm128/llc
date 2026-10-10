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
		u0_t			Type				[4]	= {};
		u2_t			CRC						= {};
		::llc::au0_t	Data					= {};
	};


	struct SPNGIHDR {
		::llc::n2u2_t	Size					= {};
		s0_t			BitDepth				= 0;
		COLOR_TYPE		ColorType				= COLOR_TYPE_GRAYSCALE;
		s0_t			MethodCompression		= 0;
		s0_t			MethodFilter			= 0;
		s0_t			MethodInterlace			= 0;
	};

	GDEFINE_ENUM_TYPE(PNG_TAG, u0_t);
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
		::llc::aau0_t					Scanlines		;
		::llc::a8bgr					Palette			;
		::llc::n2u2_t					Adam7Sizes	[7]	= {};
		//::llc::SPNGFeature				Feature			= {};
		::llc::SPNGIHDR					Header			= {};
		::llc::asts2_t<PNG_TAG_COUNT>	Feature			;
	};
#pragma pack(pop)

	u2_t					update_crc	(const ::llc::vcu0_t & buf, u2_t crc)		;
	stainli	u2_t			get_crc		(const ::llc::vcu0_t & buf)					{ return update_crc(buf, 0xffffffffL) ^ 0xffffffffL; }

	::llc::err_t			pngDecode	(::llc::SPNGData & pngData, ::llc::g8bgra	out_View);
	::llc::err_t			pngDecode	(::llc::SPNGData & pngData, ::llc::gu1_t	out_View);
	::llc::err_t			pngDecode	(::llc::SPNGData & pngData, ::llc::gu0_t	out_View);
	::llc::err_t			pngDecode	(::llc::SPNGData & pngData, ::llc::au0_t	& out_Data);
	::llc::err_t			pngDecode	(::llc::SPNGData & pngData, ::llc::au0_t	& out_Data, ::llc::g8bgra & out_View);
	::llc::err_t			pngDecode	(::llc::SPNGData & pngData, ::llc::au0_t	& out_Data, ::llc::gu1_t   & out_View);
	::llc::err_t			pngDecode	(::llc::SPNGData & pngData, ::llc::au0_t	& out_Data, ::llc::gu0_t   & out_View);
	::llc::err_t			pngDecode	(::llc::SPNGData & pngData, ::llc::img8bgra & out_Image);
	::llc::err_t			pngDecode	(::llc::SPNGData & pngData, ::llc::imgu1_t	& out_Image);
	::llc::err_t			pngDecode	(::llc::SPNGData & pngData, ::llc::imgu0_t	& out_Image);
	
	::llc::err_t			pngFileLoad	(::llc::SPNGData & pngData, const ::llc::vcst_t	& filename	);
	::llc::err_t			pngFileLoad	(::llc::SPNGData & pngData, const ::llc::vcu0_t	& source	);
	::llc::err_t			pngFileLoad	(::llc::SPNGData & pngData, const ::llc::vcst_t	& filename	, ::llc::img8bgra & out_Texture)	;
	::llc::err_t			pngFileLoad	(::llc::SPNGData & pngData, const ::llc::vcu0_t	& source	, ::llc::img8bgra & out_Texture)	;
	::llc::err_t			pngFileLoad	(::llc::SPNGData & pngData, const ::llc::vcst_t	& filename	, ::llc::au0_t & out_Data, ::llc::g8bgra & out_View)	;
	::llc::err_t			pngFileLoad	(::llc::SPNGData & pngData, const ::llc::vcu0_t	& source	, ::llc::au0_t & out_Data, ::llc::g8bgra & out_View)	;
	::llc::err_t			pngFileLoad	(::llc::SPNGData & pngData, const ::llc::vcst_t	& filename	, ::llc::au0_t & out_Data)	;
	::llc::err_t			pngFileLoad	(::llc::SPNGData & pngData, const ::llc::vcu0_t	& source	, ::llc::au0_t & out_Data)	;

	::llc::err_t			pngFileWrite(const ::llc::gc8bgra & out_ImageView, ::llc::au0_t & out_Bytes);

	stainli	::llc::err_t	pngFileLoad	(const ::llc::vcst_t	& filename	, ::llc::img8bgra & out_Texture)					{ ::llc::SPNGData tempCache; return pngFileLoad(tempCache, filename	, out_Texture); }
	stainli	::llc::err_t	pngFileLoad	(const ::llc::vcu0_t	& source	, ::llc::img8bgra & out_Texture)					{ ::llc::SPNGData tempCache; return pngFileLoad(tempCache, source	, out_Texture); }
	stainli	::llc::err_t	pngFileLoad	(const ::llc::vcst_t	& filename	, ::llc::au0_t & out_Data, ::llc::g8bgra & out_View)	{ ::llc::SPNGData tempCache; return pngFileLoad(tempCache, filename	, out_Data, out_View); }
	stainli	::llc::err_t	pngFileLoad	(const ::llc::vcu0_t	& source	, ::llc::au0_t & out_Data, ::llc::g8bgra & out_View)	{ ::llc::SPNGData tempCache; return pngFileLoad(tempCache, source	, out_Data, out_View); }
	stainli	::llc::err_t	pngFileLoad	(const ::llc::vcst_t	& filename	, ::llc::au0_t & out_Data)							{ ::llc::SPNGData tempCache; return pngFileLoad(tempCache, filename	, out_Data); }
	stainli	::llc::err_t	pngFileLoad	(const ::llc::vcu0_t	& source	, ::llc::au0_t & out_Data)							{ ::llc::SPNGData tempCache; return pngFileLoad(tempCache, source	, out_Data); }
} // namespace

#endif // LLC_PNG_H_23627
