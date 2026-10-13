#include "llc_gui.h"
#include "llc_minmax.h"

#ifndef LLC_GUI_SLIDER_H_23627
#define LLC_GUI_SLIDER_H_23627

namespace llc
{
	tydf function<err_t(astchar<64> &, s3_t, minmaxs3_c &)> FSliderTextFormat;

	stct SSlider {
		cid_t			IdRoot		= CID_INVALID;
		cid_t			IdThumb		= CID_INVALID;
		minmaxs3_t		ValueLimits	= {0, 100};
		s3_t			ValueCurrent	= 0;
		s3_t			ValueStep	= 1;
		s3_t			ValuePage	= 10;
		bool			Vertical	= false;
		astchar<64>		ValueText	= {};
		FSliderTextFormat	TextFormat	= {};
	};

	stct SSliderCreate {
		cid_t		Parent		= CID_INVALID;
		rect2s1_t	Area		= {{}, {240, 28}};
		minmaxs3_t	ValueLimits	= {0, 100};
		s3_t		Value		= 0;
		bool		Vertical	= false;
	};

	err_t	sliderCreate	(SSlider & slider, SGUI & gui, cnst SSliderCreate & setup = {});
	err_t	sliderSetValue	(SSlider & slider, SGUI & gui, s3_t value);
	err_t	sliderUpdate	(SSlider & slider, SGUI & gui, cnst SInput & input);
} // namespace llc

#endif // LLC_GUI_SLIDER_H_23627
