#include "llc_image.h"
#include "llc_color.h"
#include "llc_array_obj.h"
#include "llc_ptr_obj.h"

#ifndef LLC_IMG_COLOR_H_23627
#define LLC_IMG_COLOR_H_23627

nmsp llc
{
	tydf	img<::llc::bgr  >	img8bgr	;
	tydf	img<::llc::rgb  >	img8rgb	;
	tydf	img<::llc::bgra >	img8bgra;
	tydf	img<::llc::rgba >	img8rgba;
	tydf	img<::llc::rgbaf>	imgfrgba;

	tydf rt<::llc::bgra, u2_t>	rtbgra8d32;
	tydf rt<::llc::rgba, u2_t>	rtrgba8d32;
	tydf rt<::llc::bgra, u0_t>	rtbgra8s8;
	tydf rt<::llc::rgba, u0_t>	rtrgba8s8;

	tydf	pobj<::llc::img8bgr		>		pimg8bgr ;
	tydf	pobj<::llc::img8rgb		>		pimg8rgb ;
	tydf	pobj<::llc::img8bgra	>		pimg8bgra;
	tydf	pobj<::llc::img8rgba	>		pimg8rgba;
	tydf	pobj<::llc::imgfrgba	>		pimgfrgba;

	tydf pobj<::llc::rtbgra8d32		>	prtbgra8d32;
	tydf pobj<::llc::rtrgba8d32		>	prtrgba8d32;
	tydf pobj<::llc::rtbgra8s8		>	prtbgra8s8;
	tydf pobj<::llc::rtrgba8s8		>	prtrgba8s8;

	tydf	aobj<::llc::pimg8bgr		>	apimg8bgr ;
	tydf	aobj<::llc::pimg8rgb		>	apimg8rgb ;
	tydf	aobj<::llc::pimg8bgra		>	apimg8bgra;
	tydf	aobj<::llc::pimg8rgba		>	apimg8rgba;
	tydf	aobj<::llc::pimgfrgba		>	apimgfrgba;

	tydf aobj<::llc::prtbgra8d32	>	aprtbgra8d32;
	tydf aobj<::llc::prtrgba8d32	>	aprtrgba8d32;
	tydf aobj<::llc::prtbgra8s8		>	aprtbgra8s8;
	tydf aobj<::llc::prtrgba8s8		>	aprtrgba8s8;
} // nmsp

#endif // LLC_IMG_COLOR_H_23627
