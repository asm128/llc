#include "llc_file.h"
#include "llc_path.h"
#include "llc_png.h"
#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(PNG_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(PNG_TEST_RESULT, FILE_LOAD			, 1, "pngFileLoad() failed to load a valid file.");
GDEFINE_ENUM_VALUED(PNG_TEST_RESULT, SOURCE_LOAD			, 2, "pngFileLoad() failed to load valid memory.");
GDEFINE_ENUM_VALUED(PNG_TEST_RESULT, INVALID_SOURCE		, 3, "pngFileLoad() accepted an invalid source.");
GDEFINE_ENUM_VALUED(PNG_TEST_RESULT, HEADER_WIDTH		, 4, "pngFileLoad() returned the wrong width.");
GDEFINE_ENUM_VALUED(PNG_TEST_RESULT, HEADER_HEIGHT		, 5, "pngFileLoad() returned the wrong height.");
GDEFINE_ENUM_VALUED(PNG_TEST_RESULT, HEADER_COLOR_TYPE	, 6, "pngFileLoad() returned the wrong color type.");
GDEFINE_ENUM_VALUED(PNG_TEST_RESULT, HEADER_BIT_DEPTH	, 7, "pngFileLoad() returned the wrong bit depth.");
GDEFINE_ENUM_VALUED(PNG_TEST_RESULT, HEADER_INTERLACE	, 8, "pngFileLoad() returned the wrong interlace method.");
GDEFINE_ENUM_VALUED(PNG_TEST_RESULT, IMAGE_SIZE			, 9, "pngFileLoad() returned the wrong texel count.");
GDEFINE_ENUM_VALUED(PNG_TEST_RESULT, INTERLACED_TEXEL	, 10, "Interlaced and non-interlaced PNGs decoded differently.");
GDEFINE_ENUM_VALUED(PNG_TEST_RESULT, SOURCE_TEXEL		, 11, "File and memory PNG loads decoded differently.");
GDEFINE_ENUM_VALUED(PNG_TEST_RESULT, INVALID_SIGNATURE	, 12, "pngFileLoad() accepted an invalid PNG signature.");

stct SPNGTestPair {
	::llc::vcst_t	Interlaced		= {};
	::llc::vcst_t	NonInterlaced	= {};
	::llc::COLOR_TYPE ColorType		= {};
	::llc::s0_t		BitDepth		= {};
};

stxp ::llc::vcst_t PNG_SUITE_DIRECTORY = LLC_CXS("../llc_data/pngsuite");
stxp ::llc::vcst_t PNG_INVALID_SIGNATURES[] =
	{ LLC_CXS("xcrn0g04.png")
	, LLC_CXS("xlfn0g04.png")
	, LLC_CXS("xs1n0g01.png")
	, LLC_CXS("xs2n0g01.png")
	, LLC_CXS("xs4n0g01.png")
	, LLC_CXS("xs7n0g01.png")
	};
stxp SPNGTestPair PNG_TEST_PAIRS[] =
	{ {LLC_CXS("basi0g01.png"), LLC_CXS("basn0g01.png"), ::llc::COLOR_TYPE_GRAYSCALE		,  1}
	, {LLC_CXS("basi0g02.png"), LLC_CXS("basn0g02.png"), ::llc::COLOR_TYPE_GRAYSCALE		,  2}
	, {LLC_CXS("basi0g04.png"), LLC_CXS("basn0g04.png"), ::llc::COLOR_TYPE_GRAYSCALE		,  4}
	, {LLC_CXS("basi0g08.png"), LLC_CXS("basn0g08.png"), ::llc::COLOR_TYPE_GRAYSCALE		,  8}
	, {LLC_CXS("basi0g16.png"), LLC_CXS("basn0g16.png"), ::llc::COLOR_TYPE_GRAYSCALE		, 16}
	, {LLC_CXS("basi2c08.png"), LLC_CXS("basn2c08.png"), ::llc::COLOR_TYPE_RGB				,  8}
	, {LLC_CXS("basi2c16.png"), LLC_CXS("basn2c16.png"), ::llc::COLOR_TYPE_RGB				, 16}
	, {LLC_CXS("basi3p01.png"), LLC_CXS("basn3p01.png"), ::llc::COLOR_TYPE_PALETTE			,  1}
	, {LLC_CXS("basi3p02.png"), LLC_CXS("basn3p02.png"), ::llc::COLOR_TYPE_PALETTE			,  2}
	, {LLC_CXS("basi3p04.png"), LLC_CXS("basn3p04.png"), ::llc::COLOR_TYPE_PALETTE			,  4}
	, {LLC_CXS("basi3p08.png"), LLC_CXS("basn3p08.png"), ::llc::COLOR_TYPE_PALETTE			,  8}
	, {LLC_CXS("basi4a08.png"), LLC_CXS("basn4a08.png"), ::llc::COLOR_TYPE_GRAYSCALE_ALPHA	,  8}
	, {LLC_CXS("basi4a16.png"), LLC_CXS("basn4a16.png"), ::llc::COLOR_TYPE_GRAYSCALE_ALPHA	, 16}
	, {LLC_CXS("basi6a08.png"), LLC_CXS("basn6a08.png"), ::llc::COLOR_TYPE_RGBA				,  8}
	, {LLC_CXS("basi6a16.png"), LLC_CXS("basn6a16.png"), ::llc::COLOR_TYPE_RGBA				, 16}
	};

sttc ::llc::err_t testPNGInvalidSource(ATestError & errors) {
	::llc::SPNGData pngData = {};
	LLC_TEST_CHECKF(errors, PNG_TEST_RESULT_INVALID_SOURCE, 0 <= ::llc::pngFileLoad(pngData, ::llc::vcu0_t{}), "%s", "Empty source was accepted.");
	cnst ::llc::u0_t signature[] = {0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A};
	LLC_TEST_CHECKF(errors, PNG_TEST_RESULT_INVALID_SOURCE, 0 <= ::llc::pngFileLoad(pngData, ::llc::vcu0_t{signature}), "%s", "Signature-only source was accepted.");
	for(::llc::u2_t iFile = 0; iFile < ::llc::size(PNG_INVALID_SIGNATURES); ++iFile) {
		::llc::string path = {};
		if_fail_fe(::llc::pathNameCompose(PNG_SUITE_DIRECTORY, PNG_INVALID_SIGNATURES[iFile], path));
		cnst ::llc::vcst_t pathView = {path.begin(), path.size()};
		LLC_TEST_CHECKF(errors, PNG_TEST_RESULT_INVALID_SIGNATURE, 0 <= ::llc::pngFileLoad(pngData, pathView) , "File:'%.*s'." , (int)pathView.size(), pathView.begin() );
	}
	rtrn 0;
}

sttc ::llc::err_t testPNGLoadPairs(ATestError & errors) {
	::llc::SPNGData pngData = {};
	for(::llc::u2_t iPair = 0; iPair < ::llc::size(PNG_TEST_PAIRS); ++iPair) {
		cnst SPNGTestPair & pair = PNG_TEST_PAIRS[iPair];
		::llc::string interlacedPath = {}, nonInterlacedPath = {};
		if_fail_fe(::llc::pathNameCompose(PNG_SUITE_DIRECTORY, pair.Interlaced, interlacedPath));
		if_fail_fe(::llc::pathNameCompose(PNG_SUITE_DIRECTORY, pair.NonInterlaced, nonInterlacedPath));
		cnst ::llc::vcst_t interlacedPathView = {interlacedPath.begin(), interlacedPath.size()};
		cnst ::llc::vcst_t nonInterlacedPathView = {nonInterlacedPath.begin(), nonInterlacedPath.size()};

		::llc::img8bgra interlacedImage = {}, nonInterlacedImage = {}, sourceImage = {};
		cnst ::llc::err_t interlacedResult = ::llc::pngFileLoad(pngData, interlacedPathView, interlacedImage);
		LLC_TEST_CHECKF(errors, PNG_TEST_RESULT_FILE_LOAD, ::llc::failed(interlacedResult) , "File:'%.*s', result:%i." , (int)interlacedPathView.size(), interlacedPathView.begin(), interlacedResult );
		if(::llc::failed(interlacedResult))
			continue;
		LLC_TEST_CHECKF(errors, PNG_TEST_RESULT_HEADER_WIDTH, 32 != pngData.Header.Size.x , "File:'%.*s', width:%u, expected:32." , (int)pair.Interlaced.size(), pair.Interlaced.begin(), pngData.Header.Size.x );
		LLC_TEST_CHECKF(errors, PNG_TEST_RESULT_HEADER_HEIGHT, 32 != pngData.Header.Size.y , "File:'%.*s', height:%u, expected:32." , (int)pair.Interlaced.size(), pair.Interlaced.begin(), pngData.Header.Size.y );
		LLC_TEST_CHECKF(errors, PNG_TEST_RESULT_HEADER_COLOR_TYPE, pair.ColorType != pngData.Header.ColorType , "File:'%.*s', color type:%u, expected:%u." , (int)pair.Interlaced.size(), pair.Interlaced.begin(), (unsigned)pngData.Header.ColorType, (unsigned)pair.ColorType );
		LLC_TEST_CHECKF(errors, PNG_TEST_RESULT_HEADER_BIT_DEPTH, pair.BitDepth != pngData.Header.BitDepth , "File:'%.*s', bit depth:%i, expected:%i." , (int)pair.Interlaced.size(), pair.Interlaced.begin(), pngData.Header.BitDepth, pair.BitDepth );
		LLC_TEST_CHECKF(errors, PNG_TEST_RESULT_HEADER_INTERLACE, 1 != pngData.Header.MethodInterlace , "File:'%.*s', interlace:%i, expected:1." , (int)pair.Interlaced.size(), pair.Interlaced.begin(), pngData.Header.MethodInterlace );

		cnst ::llc::err_t nonInterlacedResult = ::llc::pngFileLoad(pngData, nonInterlacedPathView, nonInterlacedImage);
		LLC_TEST_CHECKF(errors, PNG_TEST_RESULT_FILE_LOAD, ::llc::failed(nonInterlacedResult) , "File:'%.*s', result:%i." , (int)nonInterlacedPathView.size(), nonInterlacedPathView.begin(), nonInterlacedResult );
		if(::llc::failed(nonInterlacedResult))
			continue;
		LLC_TEST_CHECKF(errors, PNG_TEST_RESULT_HEADER_WIDTH, 32 != pngData.Header.Size.x , "File:'%.*s', width:%u, expected:32." , (int)pair.NonInterlaced.size(), pair.NonInterlaced.begin(), pngData.Header.Size.x );
		LLC_TEST_CHECKF(errors, PNG_TEST_RESULT_HEADER_HEIGHT, 32 != pngData.Header.Size.y , "File:'%.*s', height:%u, expected:32." , (int)pair.NonInterlaced.size(), pair.NonInterlaced.begin(), pngData.Header.Size.y );
		LLC_TEST_CHECKF(errors, PNG_TEST_RESULT_HEADER_COLOR_TYPE, pair.ColorType != pngData.Header.ColorType , "File:'%.*s', color type:%u, expected:%u." , (int)pair.NonInterlaced.size(), pair.NonInterlaced.begin(), (unsigned)pngData.Header.ColorType, (unsigned)pair.ColorType );
		LLC_TEST_CHECKF(errors, PNG_TEST_RESULT_HEADER_BIT_DEPTH, pair.BitDepth != pngData.Header.BitDepth , "File:'%.*s', bit depth:%i, expected:%i." , (int)pair.NonInterlaced.size(), pair.NonInterlaced.begin(), pngData.Header.BitDepth, pair.BitDepth );
		LLC_TEST_CHECKF(errors, PNG_TEST_RESULT_HEADER_INTERLACE, 0 != pngData.Header.MethodInterlace , "File:'%.*s', interlace:%i, expected:0." , (int)pair.NonInterlaced.size(), pair.NonInterlaced.begin(), pngData.Header.MethodInterlace );

		::llc::au0_t source = {};
		if_fail_fef(::llc::fileToMemory(interlacedPathView, source), "Failed to read:'%.*s'.", (int)interlacedPathView.size(), interlacedPathView.begin());
		cnst ::llc::err_t sourceResult = ::llc::pngFileLoad(pngData, source.cu8(), sourceImage);
		LLC_TEST_CHECKF(errors, PNG_TEST_RESULT_SOURCE_LOAD, ::llc::failed(sourceResult) , "File:'%.*s', result:%i." , (int)interlacedPathView.size(), interlacedPathView.begin(), sourceResult );
		if(::llc::failed(sourceResult))
			continue;

		LLC_TEST_CHECKF(errors, PNG_TEST_RESULT_IMAGE_SIZE, 32 * 32 != interlacedImage.size() , "File:'%.*s', texels:%u, expected:1024." , (int)pair.Interlaced.size(), pair.Interlaced.begin(), interlacedImage.size() );
		LLC_TEST_CHECKF(errors, PNG_TEST_RESULT_IMAGE_SIZE, interlacedImage.size() != nonInterlacedImage.size() , "Files:'%.*s'/'%.*s', texels:%u/%u." , (int)pair.Interlaced.size(), pair.Interlaced.begin() , (int)pair.NonInterlaced.size(), pair.NonInterlaced.begin() , interlacedImage.size(), nonInterlacedImage.size() );
		LLC_TEST_CHECKF(errors, PNG_TEST_RESULT_IMAGE_SIZE, interlacedImage.size() != sourceImage.size() , "File:'%.*s', file/source texels:%u/%u." , (int)pair.Interlaced.size(), pair.Interlaced.begin(), interlacedImage.size(), sourceImage.size() );
		if(32 * 32 != interlacedImage.size() || interlacedImage.size() != nonInterlacedImage.size() || interlacedImage.size() != sourceImage.size())
			continue;

		for(::llc::u2_t y = 0; y < interlacedImage.metrics().y; ++y) {
			for(::llc::u2_t x = 0; x < interlacedImage.metrics().x; ++x) {
				cnst ::llc::u2_t interlacedTexel = interlacedImage[y][x];
				cnst ::llc::u2_t nonInterlacedTexel = nonInterlacedImage[y][x];
				cnst ::llc::u2_t sourceTexel = sourceImage[y][x];
				LLC_TEST_CHECKF(errors, PNG_TEST_RESULT_INTERLACED_TEXEL, interlacedTexel != nonInterlacedTexel , "Files:'%.*s'/'%.*s', cell:{%u,%u}, interlaced:0x%08X, non-interlaced:0x%08X." , (int)pair.Interlaced.size(), pair.Interlaced.begin() , (int)pair.NonInterlaced.size(), pair.NonInterlaced.begin() , x, y, interlacedTexel, nonInterlacedTexel );
				LLC_TEST_CHECKF(errors, PNG_TEST_RESULT_SOURCE_TEXEL, interlacedTexel != sourceTexel , "File:'%.*s', cell:{%u,%u}, file:0x%08X, source:0x%08X." , (int)pair.Interlaced.size(), pair.Interlaced.begin() , x, y, interlacedTexel, sourceTexel );
			}
		}
	}
	rtrn 0;
}

::llc::err_t testPNG(ATestError & errors) {
	if_fail_fe(::testPNGInvalidSource(errors));
	rtrn ::testPNGLoadPairs(errors);
}
