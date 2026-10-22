#include "llc_grid_color.h"
#include "llc_img_color.h"
#include "llc_img_serialize.h"
#include "llc_font.h"
#include "llc_test_core.h"

#include <type_traits>
#include <utility>

GDEFINE_ENUM_TYPE(GRID_IMAGE_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, IMAGE_COPY	, 5, "img<> did not own an independent copy of its texels.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, IMAGE_RESIZE	, 6, "img<>::resize() lost dimensions, values or view ownership.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, IMAGE_SAVE_RESULT			,  7, "saveImage() reported failure for a valid image.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, IMAGE_SAVE_SIZE			,  8, "saveImage() wrote the wrong number of bytes.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, IMAGE_SAVE_PREFIX			,  9, "saveImage() modified bytes already present in the destination.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, IMAGE_LOAD_RESULT			, 10, "loadImage() reported failure for a valid image.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, IMAGE_LOAD_WIDTH			, 11, "loadImage() restored the wrong width.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, IMAGE_LOAD_HEIGHT			, 12, "loadImage() restored the wrong height.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, IMAGE_LOAD_COUNT			, 13, "loadImage() restored the wrong texel count.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, IMAGE_LOAD_VALUE			, 14, "loadImage() restored a different texel value.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, IMAGE_LOAD_CURSOR			, 15, "loadImage() did not leave the input at the following field.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, IMAGE_LOAD_OWNERSHIP		, 16, "loadImage() did not point its grid view into its owned texels.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, IMAGE_TRUNCATED_RESULT		, 17, "loadImage() accepted truncated input.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, IMAGE_TRUNCATED_CURSOR		, 18, "loadImage() moved the input cursor after rejecting truncated input.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, IMAGE_TRUNCATED_OUTPUT		, 19, "loadImage() modified its output after rejecting truncated input.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, IMAGE_METRIC_COUNT_RESULT	, 20, "loadImage() accepted metrics that disagree with the texel count.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, IMAGE_METRIC_COUNT_CURSOR	, 21, "loadImage() moved the input cursor after rejecting inconsistent metrics.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, IMAGE_METRIC_COUNT_OUTPUT	, 22, "loadImage() modified its output after rejecting inconsistent metrics.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, MONO_RESIZE_RESULT			, 23, "imgmono<>::resize() reported the wrong result.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, MONO_RESIZE_WIDTH			, 24, "imgmono<>::resize() stored the wrong width.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, MONO_RESIZE_HEIGHT			, 25, "imgmono<>::resize() stored the wrong height.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, MONO_RESIZE_BITS			, 26, "imgmono<>::resize() stored the wrong bit count.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, MONO_RESIZE_CELLS			, 27, "imgmono<>::resize() allocated the wrong cell count.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, MONO_RESIZE_BYTES			, 28, "imgmono<>::resize() allocated the wrong byte count.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, MONO_VIEW_OWNERSHIP		, 29, "imgmono<> view does not point into its owned cells.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, MONO_BIT_VALUE				, 30, "imgmono<> returned the wrong bit value.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, MONO_COPY_SIZE				, 31, "imgmono<> copy lost its dimensions or bit count.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, MONO_COPY_OWNERSHIP		, 32, "imgmono<> copy aliases another image's cells.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, MONO_COPY_VALUE			, 33, "imgmono<> copy lost or shared a bit value.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, FONT_DEFAULT_RESULT		, 34, "rasterFontDefaults() reported failure.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, FONT_DEFAULT_COUNT		, 35, "rasterFontDefaults() loaded the wrong font count.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, FONT_DEFAULT_POINTER		, 36, "rasterFontDefaults() left a null font entry.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, FONT_DEFAULT_CHAR_WIDTH	, 37, "A built-in font has no character width.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, FONT_DEFAULT_CHAR_HEIGHT	, 38, "A built-in font has no character height.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, FONT_DEFAULT_IMAGE_WIDTH	, 39, "A built-in font texture has the wrong width.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, FONT_DEFAULT_IMAGE_HEIGHT	, 40, "A built-in font texture has the wrong height.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, FONT_DEFAULT_BIT_COUNT	, 41, "A built-in font texture has the wrong bit count.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, FONT_DEFAULT_CELL_COUNT	, 42, "A built-in font texture has the wrong cell count.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, FONT_DEFAULT_OWNERSHIP	, 43, "A built-in font texture view does not point into its owned cells.");
GDEFINE_ENUM_VALUED(GRID_IMAGE_TEST_RESULT, FONT_DEFAULT_CONTENT		, 44, "A built-in font texture contains no set bits.");

stxp ::llc::u0_t IMAGE_SERIALIZE_PREFIX[] = {0x53U, 0xA7U, 0x19U};
stxp ::llc::u0_t IMAGE_SERIALIZE_SUFFIX[] = {0xE1U, 0x26U, 0xBCU, 0x48U};


sttc ::llc::err_t testImage(::llc::ATestError & errors) {
	::llc::u2_t cells[2][2] = {{1, 2}, {3, 4}};
	::llc::grid<::llc::u2_t> source = {cells};
	::llc::img<::llc::u2_t> image = {source};
	LLC_TEST_REQUIREF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, 4 != image.size(), "Copied image size:%u, expected:4.", image.size());
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, source.begin() == image.begin(), "%s", "Image aliases source storage.");
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, image.Texels.begin() != image.View.begin(), "%s", "Image view does not point into owned texels.");
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, 4 != image[1][1], "Copied cell (1,1):%u, expected:4.", image[1][1]);
	cnst ::llc::img<::llc::u2_t> & constantImage = image;
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, 3 != constantImage[1][0], "Const image cell (0,1):%u, expected:3.", constantImage[1][0]);
	image[0][0] = 8;
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, 1 != source[0][0], "Source cell changed to:%u.", source[0][0]);

	::llc::img<::llc::u2_t> copied = image;
	LLC_TEST_REQUIREF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, 4 != copied.size(), "Copy size:%u, expected:4.", copied.size());
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, copied.begin() == image.begin(), "%s", "Image copy aliases original storage.");
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, copied.View.begin() != copied.Texels.begin(), "%s", "Copied view does not point into copied texels.");
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, 2 != copied.metrics().y, "Copied height:%u, expected:2.", copied.metrics().y);
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, 8 != copied[0][0], "Copied first cell:%u, expected:8.", copied[0][0]);

	cnst ::llc::n2u2_t newSize = {3, 2};
	cnst ::llc::err_t resizeResult = image.resize(newSize, 12);
	LLC_TEST_REQUIREF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_RESIZE, ::llc::failed(resizeResult), "Resize failed:%i.", resizeResult);
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_RESIZE, 6 != resizeResult, "Resize result:%i, expected:6.", resizeResult);
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_RESIZE, 3 != image.metrics().x, "Resized width:%u, expected:3.", image.metrics().x);
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_RESIZE, 2 != image.metrics().y, "Resized height:%u, expected:2.", image.metrics().y);
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_RESIZE, image.Texels.begin() != image.View.begin(), "%s", "Resized view does not point into owned texels.");
	for(::llc::u2_t iCell = 0; iCell < image.size(); ++iCell) {
		LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_RESIZE, 12 != image.begin()[iCell] , "Cell:%u, value:%u, expected:12.", iCell, image.begin()[iCell]);
	}
	cnst ::llc::n2u2_t smallerSize = {2, 1};
	cnst ::llc::err_t smallerResult = image.resize(smallerSize);
	LLC_TEST_REQUIREF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_RESIZE, ::llc::failed(smallerResult), "Resize without fill failed:%i.", smallerResult);
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_RESIZE, 0 != smallerResult, "Resize without fill returned:%i, expected:0.", smallerResult);
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_RESIZE, 2 != image.size(), "Reduced image size:%u, expected:2.", image.size());
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_RESIZE, 2 != image.metrics().x, "Reduced image width:%u, expected:2.", image.metrics().x);
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_RESIZE, 1 != image.metrics().y, "Reduced image height:%u, expected:1.", image.metrics().y);
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_RESIZE, image.Texels.begin() != image.View.begin(), "%s", "Reduced view does not point into owned texels.");

	::llc::img<::llc::u2_t> assigned = {};
	assigned = source;
	LLC_TEST_REQUIREF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, 4 != assigned.size(), "Assigned image size:%u, expected:4.", assigned.size());
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, source.begin() == assigned.begin(), "%s", "Assigned image aliases source storage.");
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, assigned.Texels.begin() != assigned.View.begin(), "%s", "Assigned view does not point into owned texels.");
	assigned = image;
	LLC_TEST_REQUIREF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, 2 != assigned.size(), "Copy-assigned size:%u, expected:2.", assigned.size());
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, assigned.begin() == image.begin(), "%s", "Copy assignment aliases original storage.");
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, assigned.Texels.begin() != assigned.View.begin(), "%s", "Copy-assigned view does not point into owned texels.");
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, 1 != assigned.metrics().y, "Copy-assigned height:%u, expected:1.", assigned.metrics().y);
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_COPY, 12 != assigned[0][0], "Copy-assigned first cell:%u, expected:12.", assigned[0][0]);
	rtrn 0;
}

sttc ::llc::err_t testMonochromeImage(::llc::ATestError & errors) {
	::llc::imgmonou3_t image = {};
	cnst ::llc::err_t resizeResult = image.resize(9, 9);
	LLC_TEST_REQUIREF(errors, GRID_IMAGE_TEST_RESULT_MONO_RESIZE_RESULT, ::llc::failed(resizeResult), "Resize result:%i.", resizeResult);
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_MONO_RESIZE_WIDTH, 9 != image.metrics().x, "Width:%u, expected:9.", image.metrics().x);
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_MONO_RESIZE_HEIGHT, 9 != image.metrics().y, "Height:%u, expected:9.", image.metrics().y);
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_MONO_RESIZE_BITS, 81 != image.size(), "Bit count:%u, expected:81.", image.size());
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_MONO_RESIZE_CELLS, 2 != image.Texels.size(), "Cell count:%u, expected:2.", image.Texels.size());
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_MONO_RESIZE_BYTES, 16 != image.byte_count(), "Byte count:%u, expected:16.", image.byte_count());
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_MONO_VIEW_OWNERSHIP, image.View.begin().Begin != image.Texels.begin(), "View begin:%p, owned begin:%p.", image.View.begin().Begin, image.Texels.begin());
	image.View[0] = true;
	image.View[63] = true;
	image.View[64] = true;
	image.View[80] = true;
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_MONO_BIT_VALUE, false == image.View[0]);
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_MONO_BIT_VALUE, false == image.View[63]);
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_MONO_BIT_VALUE, false == image.View[64]);
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_MONO_BIT_VALUE, false == image.View[80]);
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_MONO_BIT_VALUE, true == image.View[1]);

	::llc::imgmonou3_t copied = image;
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_MONO_COPY_SIZE, copied.metrics() != image.metrics(), "Copied metrics:%u x %u, expected:%u x %u.", copied.metrics().x, copied.metrics().y, image.metrics().x, image.metrics().y);
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_MONO_COPY_SIZE, copied.size() != image.size(), "Copied bit count:%u, expected:%u.", copied.size(), image.size());
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_MONO_COPY_OWNERSHIP, copied.Texels.begin() == image.Texels.begin(), "Copied cells alias source at:%p.", copied.Texels.begin());
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_MONO_COPY_OWNERSHIP, copied.View.begin().Begin != copied.Texels.begin(), "Copied view begin:%p, owned begin:%p.", copied.View.begin().Begin, copied.Texels.begin());
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_MONO_COPY_VALUE, false == copied.View[80]);
	copied.View[80] = false;
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_MONO_COPY_VALUE, false == image.View[80]);

	::llc::imgmonou3_t assigned = {};
	assigned = image;
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_MONO_COPY_SIZE, assigned.metrics() != image.metrics(), "Assigned metrics:%u x %u, expected:%u x %u.", assigned.metrics().x, assigned.metrics().y, image.metrics().x, image.metrics().y);
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_MONO_COPY_SIZE, assigned.size() != image.size(), "Assigned bit count:%u, expected:%u.", assigned.size(), image.size());
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_MONO_COPY_OWNERSHIP, assigned.Texels.begin() == image.Texels.begin(), "Assigned cells alias source at:%p.", assigned.Texels.begin());
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_MONO_COPY_OWNERSHIP, assigned.View.begin().Begin != assigned.Texels.begin(), "Assigned view begin:%p, owned begin:%p.", assigned.View.begin().Begin, assigned.Texels.begin());
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_MONO_COPY_VALUE, false == assigned.View[64]);
	assigned.View[64] = false;
	LLC_TEST_CHECK(errors, GRID_IMAGE_TEST_RESULT_MONO_COPY_VALUE, false == image.View[64]);

	LLC_TEST_REQUIREF(errors, GRID_IMAGE_TEST_RESULT_MONO_RESIZE_RESULT, 0 > image.resize(8, 8), "%s", "Exact-cell resize failed.");
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_MONO_RESIZE_CELLS, 1 != image.Texels.size(), "Exact-cell allocation:%u, expected:1.", image.Texels.size());
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_MONO_RESIZE_BYTES, 8 != image.byte_count(), "Exact-cell byte count:%u, expected:8.", image.byte_count());
	rtrn 0;
}

sttc ::llc::err_t testDefaultFonts(::llc::ATestError & errors) {
	::llc::SRasterFontManager manager = {};
	cnst ::llc::err_t defaultsResult = ::llc::rasterFontDefaults(manager);
	LLC_TEST_REQUIREF(errors, GRID_IMAGE_TEST_RESULT_FONT_DEFAULT_RESULT, ::llc::failed(defaultsResult), "Default font result:%i.", defaultsResult);
	LLC_TEST_REQUIREF(errors, GRID_IMAGE_TEST_RESULT_FONT_DEFAULT_COUNT, 13 != manager.Fonts.size(), "Font count:%u, expected:13.", manager.Fonts.size());
	for(::llc::u2_t iFont = 0; iFont < manager.Fonts.size(); ++iFont) {
		LLC_TEST_REQUIREF(errors, GRID_IMAGE_TEST_RESULT_FONT_DEFAULT_POINTER, 0 == manager.Fonts[iFont], "Null font index:%u.", iFont);
		cnst ::llc::SRasterFont & font = *manager.Fonts[iFont];
		LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_FONT_DEFAULT_CHAR_WIDTH, 0 == font.CharSize.x, "Font:%u, character width:%u.", iFont, font.CharSize.x);
		LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_FONT_DEFAULT_CHAR_HEIGHT, 0 == font.CharSize.y, "Font:%u, character height:%u.", iFont, font.CharSize.y);
		LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_FONT_DEFAULT_IMAGE_WIDTH, font.Texture.metrics().x != font.CharSize.x, "Font:%u, texture width:%u, expected:%u.", iFont, font.Texture.metrics().x, font.CharSize.x);
		LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_FONT_DEFAULT_IMAGE_HEIGHT, font.Texture.metrics().y != font.CharSize.y * 256U, "Font:%u, texture height:%u, expected:%u.", iFont, font.Texture.metrics().y, font.CharSize.y * 256U);
		LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_FONT_DEFAULT_BIT_COUNT, font.Texture.size() != ::llc::u2_t(font.CharSize.x) * font.CharSize.y * 256U, "Font:%u, bit count:%u, expected:%u.", iFont, font.Texture.size(), ::llc::u2_t(font.CharSize.x) * font.CharSize.y * 256U);
		LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_FONT_DEFAULT_CELL_COUNT, font.Texture.Texels.size() != font.Texture.size() / 64U + one_if(font.Texture.size() % 64U), "Font:%u, cell count:%u, expected:%u.", iFont, font.Texture.Texels.size(), font.Texture.size() / 64U + one_if(font.Texture.size() % 64U));
		LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_FONT_DEFAULT_OWNERSHIP, font.Texture.View.begin().Begin != font.Texture.Texels.begin(), "Font:%u, view begin:%p, owned begin:%p.", iFont, font.Texture.View.begin().Begin, font.Texture.Texels.begin());
		bool anyBit = false;
		for(::llc::u2_t iCell = 0; iCell < font.Texture.Texels.size(); ++iCell)
			anyBit |= 0 != font.Texture.Texels[iCell];
		LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_FONT_DEFAULT_CONTENT, false == anyBit, "Font:%u has no set bits.", iFont);
	}
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testImageSerialization(::llc::ATestError & errors) {
	T cells[2][3] = {};
	for(::llc::u2_t y = 0; y < ::llc::size(cells); ++y)
		for(::llc::u2_t x = 0; x < ::llc::size(cells[y]); ++x)
			cells[y][x] = T(y * ::llc::size(cells[y]) + x + 1);
	cnst ::llc::grid<T> source = {cells};
	::llc::au0_t serialized = {};
	LLC_TEST_REQUIREF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_SAVE_SIZE, 0 > serialized.append(IMAGE_SERIALIZE_PREFIX), "%u-bit image test could not append its preceding field.", bcof(T));
	cnst ::llc::u2_t imageOffset = serialized.size();
	cnst ::llc::err_t saveResult = ::llc::saveImage(serialized, source);
	LLC_TEST_REQUIREF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_SAVE_RESULT, ::llc::failed(saveResult), "%u-bit saveImage() result:%i.", bcof(T), saveResult);
	cnst ::llc::u2_t imageSize = szof(::llc::n2u2_t) + ::llc::packedu32{source.size()}.ValueWidth() + source.byte_count();
	LLC_TEST_REQUIREF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_SAVE_SIZE, serialized.size() != imageOffset + imageSize, "%u-bit serialized image size:%u, expected:%u.", bcof(T), serialized.size() - imageOffset, imageSize);
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_SAVE_PREFIX, 0 != memcmp(serialized.begin(), IMAGE_SERIALIZE_PREFIX, ::llc::size(IMAGE_SERIALIZE_PREFIX)), "%u-bit saveImage() changed its %u-byte preceding field.", bcof(T), ::llc::u2_t(::llc::size(IMAGE_SERIALIZE_PREFIX)));
	LLC_TEST_REQUIREF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_SAVE_SIZE, 0 > serialized.append(IMAGE_SERIALIZE_SUFFIX), "%u-bit image test could not append its following field.", bcof(T));

	::llc::vcu0_t input = {&serialized[imageOffset], imageSize + ::llc::size(IMAGE_SERIALIZE_SUFFIX)};
	::llc::img<T> loaded = {};
	cnst ::llc::err_t loadResult = ::llc::loadImage(input, loaded);
	LLC_TEST_REQUIREF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_LOAD_RESULT, ::llc::failed(loadResult), "%u-bit loadImage() result:%i.", bcof(T), loadResult);
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_LOAD_WIDTH, source.metrics().x != loaded.metrics().x, "%u-bit loaded width:%u, expected:%u.", bcof(T), loaded.metrics().x, source.metrics().x);
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_LOAD_HEIGHT, source.metrics().y != loaded.metrics().y, "%u-bit loaded height:%u, expected:%u.", bcof(T), loaded.metrics().y, source.metrics().y);
	LLC_TEST_REQUIREF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_LOAD_COUNT, source.size() != loaded.size(), "%u-bit loaded texel count:%u, expected:%u.", bcof(T), loaded.size(), source.size());
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_LOAD_OWNERSHIP, loaded.Texels.begin() != loaded.View.begin(), "%u-bit loaded grid begin:%p, owned texel begin:%p.", bcof(T), loaded.View.begin(), loaded.Texels.begin());
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_LOAD_OWNERSHIP, source.begin() == loaded.begin(), "%u-bit loaded image aliases serialized source texels at:%p.", bcof(T), loaded.begin());
	for(::llc::u2_t iCell = 0; iCell < source.size(); ++iCell)
		LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_LOAD_VALUE, source.begin()[iCell] != loaded.begin()[iCell], "%u-bit texel:%u, loaded:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 ".", bcof(T), iCell, ::llc::u3_t(loaded.begin()[iCell]), ::llc::u3_t(source.begin()[iCell]));
	LLC_TEST_REQUIREF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_LOAD_CURSOR, input.size() != ::llc::size(IMAGE_SERIALIZE_SUFFIX), "%u-bit remaining bytes:%u, expected:%u.", bcof(T), input.size(), ::llc::u2_t(::llc::size(IMAGE_SERIALIZE_SUFFIX)));
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_LOAD_CURSOR, input.begin() != &serialized[imageOffset + imageSize], "%u-bit remaining field begin:%p, expected:%p.", bcof(T), input.begin(), &serialized[imageOffset + imageSize]);
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_LOAD_CURSOR, 0 != memcmp(input.begin(), IMAGE_SERIALIZE_SUFFIX, ::llc::size(IMAGE_SERIALIZE_SUFFIX)), "%u-bit remaining field contents differ.", bcof(T));

	cnst ::llc::n2u2_t sentinelMetrics = {1, 1};
	cnst T sentinelValue = T(0x5AU);
	for(::llc::u2_t byteCount = 0; byteCount < imageSize; ++byteCount) {
		::llc::vcu0_t truncated = {&serialized[imageOffset], byteCount};
		cnst ::llc::vcu0_t originalInput = truncated;
		::llc::img<T> preserved = {};
		LLC_TEST_REQUIREF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_TRUNCATED_OUTPUT, 0 > preserved.resize(sentinelMetrics, sentinelValue), "%u-bit truncation sentinel image could not be prepared at byte:%u.", bcof(T), byteCount);
		cnst ::llc::vcu0_t originalTexels = preserved.Texels.cu8();
		cnst ::llc::err_t truncatedResult = ::llc::loadImage(truncated, preserved);
		LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_TRUNCATED_RESULT, 0 <= truncatedResult, "%u-bit loadImage() accepted %u/%u bytes. result:%i.", bcof(T), byteCount, imageSize, truncatedResult);
		LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_TRUNCATED_CURSOR, truncated.begin() != originalInput.begin(), "%u-bit truncated cursor begin:%p, expected:%p at byte:%u.", bcof(T), truncated.begin(), originalInput.begin(), byteCount);
		LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_TRUNCATED_CURSOR, truncated.size() != originalInput.size(), "%u-bit truncated cursor size:%u, expected:%u at byte:%u.", bcof(T), truncated.size(), originalInput.size(), byteCount);
		LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_TRUNCATED_OUTPUT, preserved.Texels.cu8().begin() != originalTexels.begin(), "%u-bit rejected load replaced output storage at byte:%u. begin:%p, expected:%p.", bcof(T), byteCount, preserved.Texels.begin(), originalTexels.begin());
		LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_TRUNCATED_OUTPUT, preserved.metrics() != sentinelMetrics, "%u-bit rejected load changed output metrics at byte:%u. metrics:%u x %u.", bcof(T), byteCount, preserved.metrics().x, preserved.metrics().y);
		LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_TRUNCATED_OUTPUT, preserved.size() != 1, "%u-bit rejected load changed output count at byte:%u. count:%u.", bcof(T), byteCount, preserved.size());
		if(preserved.size())
			LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_TRUNCATED_OUTPUT, preserved.begin()[0] != sentinelValue, "%u-bit rejected load changed output value at byte:%u. value:%" LLC_FMT_U3 ".", bcof(T), byteCount, ::llc::u3_t(preserved.begin()[0]));
	}

	::llc::au0_t malformed = serialized;
	cnst ::llc::n2u2_t inconsistentMetrics = {::llc::u2_t(source.metrics().x + 1), source.metrics().y};
	memcpy(&malformed[imageOffset], &inconsistentMetrics, szof(inconsistentMetrics));
	::llc::vcu0_t inconsistentInput = {&malformed[imageOffset], imageSize};
	cnst ::llc::vcu0_t originalInconsistentInput = inconsistentInput;
	::llc::img<T> preserved = {};
	LLC_TEST_REQUIREF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_METRIC_COUNT_OUTPUT, 0 > preserved.resize(sentinelMetrics, sentinelValue), "%u-bit inconsistent-metrics sentinel image could not be prepared.", bcof(T));
	cnst ::llc::vcu0_t originalTexels = preserved.Texels.cu8();
	cnst ::llc::err_t inconsistentResult = ::llc::loadImage(inconsistentInput, preserved);
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_METRIC_COUNT_RESULT, 0 <= inconsistentResult, "%u-bit loadImage() accepted metrics:%u x %u for %u texels. result:%i.", bcof(T), inconsistentMetrics.x, inconsistentMetrics.y, source.size(), inconsistentResult);
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_METRIC_COUNT_CURSOR, inconsistentInput.begin() != originalInconsistentInput.begin(), "%u-bit inconsistent-metrics cursor begin:%p, expected:%p.", bcof(T), inconsistentInput.begin(), originalInconsistentInput.begin());
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_METRIC_COUNT_CURSOR, inconsistentInput.size() != originalInconsistentInput.size(), "%u-bit inconsistent-metrics cursor size:%u, expected:%u.", bcof(T), inconsistentInput.size(), originalInconsistentInput.size());
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_METRIC_COUNT_OUTPUT, preserved.Texels.cu8().begin() != originalTexels.begin(), "%u-bit inconsistent-metrics load replaced output storage. begin:%p, expected:%p.", bcof(T), preserved.Texels.begin(), originalTexels.begin());
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_METRIC_COUNT_OUTPUT, preserved.metrics() != sentinelMetrics, "%u-bit inconsistent-metrics load changed output metrics to:%u x %u.", bcof(T), preserved.metrics().x, preserved.metrics().y);
	LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_METRIC_COUNT_OUTPUT, preserved.size() != 1, "%u-bit inconsistent-metrics load changed output count to:%u.", bcof(T), preserved.size());
	if(preserved.size())
		LLC_TEST_CHECKF(errors, GRID_IMAGE_TEST_RESULT_IMAGE_METRIC_COUNT_OUTPUT, preserved.begin()[0] != sentinelValue, "%u-bit inconsistent-metrics load changed output value to:%" LLC_FMT_U3 ".", bcof(T), ::llc::u3_t(preserved.begin()[0]));
	return 0;
}

::llc::err_t testGridImage(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	static_assert(::std::is_same_v<::llc::gsc_t, ::llc::grid<::llc::sc_t>>);
	static_assert(::std::is_same_v<::llc::guc_t, ::llc::grid<::llc::uc_t>>);
	static_assert(::std::is_same_v<::llc::gu0_t, ::llc::grid<::llc::u0_t>>);
	static_assert(::std::is_same_v<::llc::gu1_t, ::llc::grid<::llc::u1_t>>);
	static_assert(::std::is_same_v<::llc::gu2_t, ::llc::grid<::llc::u2_t>>);
	static_assert(::std::is_same_v<::llc::gu3_t, ::llc::grid<::llc::u3_t>>);
	static_assert(::std::is_same_v<::llc::gs0_t, ::llc::grid<::llc::s0_t>>);
	static_assert(::std::is_same_v<::llc::gs1_t, ::llc::grid<::llc::s1_t>>);
	static_assert(::std::is_same_v<::llc::gs2_t, ::llc::grid<::llc::s2_t>>);
	static_assert(::std::is_same_v<::llc::gs3_t, ::llc::grid<::llc::s3_t>>);
	static_assert(::std::is_same_v<::llc::gf2_t, ::llc::grid<::llc::f2_t>>);
	static_assert(::std::is_same_v<::llc::gf3_t, ::llc::grid<::llc::f3_t>>);
	static_assert(::std::is_same_v<::llc::g8bgra, ::llc::grid<::llc::bgra>>);
	static_assert(::std::is_same_v<::llc::gc8bgra, ::llc::grid<cnst ::llc::bgra>>);
	static_assert(::std::is_same_v<::llc::imgu0_t, ::llc::img<::llc::u0_t>>);
	static_assert(::std::is_same_v<::llc::imgu1_t, ::llc::img<::llc::u1_t>>);
	static_assert(::std::is_same_v<::llc::imgu2_t, ::llc::img<::llc::u2_t>>);
	static_assert(::std::is_same_v<::llc::imgu3_t, ::llc::img<::llc::u3_t>>);
	static_assert(::std::is_same_v<::llc::imgs0_t, ::llc::img<::llc::s0_t>>);
	static_assert(::std::is_same_v<::llc::imgs1_t, ::llc::img<::llc::s1_t>>);
	static_assert(::std::is_same_v<::llc::imgs2_t, ::llc::img<::llc::s2_t>>);
	static_assert(::std::is_same_v<::llc::imgs3_t, ::llc::img<::llc::s3_t>>);
	static_assert(::std::is_same_v<::llc::imgf2_t, ::llc::img<::llc::f2_t>>);
	static_assert(::std::is_same_v<::llc::imgf3_t, ::llc::img<::llc::f3_t>>);
	static_assert(::std::is_same_v<::llc::img8bgra, ::llc::img<::llc::bgra>>);
	static_assert(::std::is_same_v<decltype(::std::declval<cnst ::llc::g8bgra &>()[0]), ::llc::view<cnst ::llc::bgra>>);
	static_assert(::std::is_same_v<decltype(::std::declval<cnst ::llc::img8bgra &>()[0]), ::llc::view<cnst ::llc::bgra>>);
	if_fail_fe(::testImage(errors));
	if_fail_fe(::testMonochromeImage(errors));
	if_fail_fe(::testDefaultFonts(errors));
	if_fail_fe(::testImageSerialization<::llc::u0_t>(errors));
	if_fail_fe(::testImageSerialization<::llc::u1_t>(errors));
	if_fail_fe(::testImageSerialization<::llc::u2_t>(errors));
	if_fail_fe(::testImageSerialization<::llc::u3_t>(errors));
	if_fail_fe(::testImageSerialization<::llc::s0_t>(errors));
	if_fail_fe(::testImageSerialization<::llc::s1_t>(errors));
	if_fail_fe(::testImageSerialization<::llc::s2_t>(errors));
	rtrn ::testImageSerialization<::llc::s3_t>(errors);
}
