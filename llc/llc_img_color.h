#include "llc_image.h"
#include "llc_color.h"
#include "llc_array_obj.h"
#include "llc_ptr_obj.h"

#ifndef LLC_IMG_COLOR_H_23627
#define LLC_IMG_COLOR_H_23627

namespace llc
{
	typedef	img<bgr  >	img8bgr	;
	typedef	img<rgb  >	img8rgb	;
	typedef	img<bgra >	img8bgra;
	typedef	img<rgba >	img8rgba;
	typedef	img<rgbaf>	imgfrgba;

	typedef rt<bgra, u2_t>	rtbgra8d32;
	typedef rt<rgba, u2_t>	rtrgba8d32;
	typedef rt<bgra, uint8_t>	rtbgra8s8;
	typedef rt<rgba, uint8_t>	rtrgba8s8;

	typedef	pobj<img8bgr		>		pimg8bgr ;
	typedef	pobj<img8rgb		>		pimg8rgb ;
	typedef	pobj<img8bgra	>		pimg8bgra;
	typedef	pobj<img8rgba	>		pimg8rgba;
	typedef	pobj<imgfrgba	>		pimgfrgba;

	typedef pobj<rtbgra8d32		>	prtbgra8d32;
	typedef pobj<rtrgba8d32		>	prtrgba8d32;
	typedef pobj<rtbgra8s8		>	prtbgra8s8;
	typedef pobj<rtrgba8s8		>	prtrgba8s8;

	typedef	aobj<pimg8bgr		>	apimg8bgr ;
	typedef	aobj<pimg8rgb		>	apimg8rgb ;
	typedef	aobj<pimg8bgra		>	apimg8bgra;
	typedef	aobj<pimg8rgba		>	apimg8rgba;
	typedef	aobj<pimgfrgba		>	apimgfrgba;

	typedef aobj<prtbgra8d32	>	aprtbgra8d32;
	typedef aobj<prtrgba8d32	>	aprtrgba8d32;
	typedef aobj<prtbgra8s8		>	aprtbgra8s8;
	typedef aobj<prtrgba8s8		>	aprtrgba8s8;
} // namespace

#endif // LLC_IMG_COLOR_H_23627
