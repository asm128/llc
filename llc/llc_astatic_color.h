#include "llc_array_static.h"
#include "llc_color.h"

#ifndef LLC_ASTATIC_COLOR_H_23627
#define LLC_ASTATIC_COLOR_H_23627

namespace llc
{
	tplt<size_t _pixelCount> using	astfrgba	= ::llc::astatic<::llc::frgba	, _pixelCount>;
	tplt<size_t _pixelCount> using	astu8bgra	= ::llc::astatic<::llc::u8bgra	, _pixelCount>;
	tplt<size_t _pixelCount> using	astu8bgr	= ::llc::astatic<::llc::u8bgr	, _pixelCount>;
	tplt<size_t _pixelCount> using	astu8rgba	= ::llc::astatic<::llc::u8rgba	, _pixelCount>;
	tplt<size_t _pixelCount> using	astu8rgb	= ::llc::astatic<::llc::u8rgb	, _pixelCount>;

} // namespace

#endif // LLC_ASTATIC_COLOR_H_23624