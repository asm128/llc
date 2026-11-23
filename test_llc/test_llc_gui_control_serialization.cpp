#include "llc_gui.h"
#include "llc_apod_serialize.h"
#include "test_llc_gui_control_corpus.h"
#include "test_llc_suites.h"
#include "llc_gui_text_flag.h"

static_assert(sizeof(::llc::SControlState) == 6, "The shared wire format requires the packed SControlState layout.");
static_assert(sizeof(::llc::STextState) == 6, "The shared wire format requires the STextState layout.");
static_assert(sizeof(::llc::SControlConstraints) == 22, "The shared wire format requires the packed SControlConstraints layout.");
static_assert(sizeof(::llc::SControlPlacement) == 28, "The shared wire format requires the packed SControlPlacement layout.");
static_assert(sizeof(::llc::SControlArea) == 176, "The shared wire format requires the packed SControlArea layout.");
static_assert(sizeof(::llc::SControlDraw) == 29, "The shared wire format requires the packed SControlDraw layout.");

GDEFINE_ENUM_TYPE(GUI_CONTROL_SERIALIZATION_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(GUI_CONTROL_SERIALIZATION_TEST_RESULT, SAVE				, 0, "SControlTable::Save() rejected a valid table.");
GDEFINE_ENUM_VALUED(GUI_CONTROL_SERIALIZATION_TEST_RESULT, PACKED_COUNT		, 1, "The 64-control count did not use the expected packed-u2_t boundary representation.");
GDEFINE_ENUM_VALUED(GUI_CONTROL_SERIALIZATION_TEST_RESULT, LOAD				, 2, "SControlTable::Load() rejected its own serialized table.");
GDEFINE_ENUM_VALUED(GUI_CONTROL_SERIALIZATION_TEST_RESULT, CONSUMPTION		, 3, "SControlTable::Load() did not consume exactly one table.");
GDEFINE_ENUM_VALUED(GUI_CONTROL_SERIALIZATION_TEST_RESULT, PARALLEL_ARRAYS	, 4, "The loaded control-table arrays do not share the serialized slot count.");
GDEFINE_ENUM_VALUED(GUI_CONTROL_SERIALIZATION_TEST_RESULT, STATE				, 5, "Control state did not round-trip or computed state remained marked current.");
GDEFINE_ENUM_VALUED(GUI_CONTROL_SERIALIZATION_TEST_RESULT, TEXT_STATE		, 6, "Per-character text state did not round-trip.");
GDEFINE_ENUM_VALUED(GUI_CONTROL_SERIALIZATION_TEST_RESULT, STRINGS			, 7, "Identifier, script or text did not round-trip.");
GDEFINE_ENUM_VALUED(GUI_CONTROL_SERIALIZATION_TEST_RESULT, COMPONENTS		, 8, "Constraints, placement, drawing or text metadata did not round-trip.");
GDEFINE_ENUM_VALUED(GUI_CONTROL_SERIALIZATION_TEST_RESULT, IMAGE				, 9, "Control image data entered the table stream or its dimensions were not represented.");
GDEFINE_ENUM_VALUED(GUI_CONTROL_SERIALIZATION_TEST_RESULT, HIERARCHY			, 10, "Control hierarchy or child order did not round-trip.");
GDEFINE_ENUM_VALUED(GUI_CONTROL_SERIALIZATION_TEST_RESULT, TABLE_MEMBERS		, 11, "Area, event queue or exact control flags did not round-trip.");
GDEFINE_ENUM_VALUED(GUI_CONTROL_SERIALIZATION_TEST_RESULT, TRUNCATED_REJECT	, 12, "The loader accepted truncated input.");
GDEFINE_ENUM_VALUED(GUI_CONTROL_SERIALIZATION_TEST_RESULT, PACKED_BOUNDARIES	, 13, "A packed-u2_t boundary value did not round-trip with the expected byte width.");
GDEFINE_ENUM_VALUED(GUI_CONTROL_SERIALIZATION_TEST_RESULT, PACKED_TRUNCATED	, 14, "loadPacked() accepted a count with a missing tail byte.");
GDEFINE_ENUM_VALUED(GUI_CONTROL_SERIALIZATION_TEST_RESULT, CORPUS_SETUP		, 15, "The complete GUI corpus could not be constructed.");
GDEFINE_ENUM_VALUED(GUI_CONTROL_SERIALIZATION_TEST_RESULT, CORPUS_STRUCTURE	, 16, "The complete GUI corpus did not preserve its authored structure.");
GDEFINE_ENUM_VALUED(GUI_CONTROL_SERIALIZATION_TEST_RESULT, CORPUS_STRINGS		, 17, "A loaded corpus text view does not borrow the retained serialized bytes.");
GDEFINE_ENUM_VALUED(GUI_CONTROL_SERIALIZATION_TEST_RESULT, CORPUS_IMAGES		, 18, "A separately loaded corpus image did not survive table loading.");
GDEFINE_ENUM_VALUED(GUI_CONTROL_SERIALIZATION_TEST_RESULT, CORPUS_CONSUMPTION	, 19, "A corpus round trip left unread bytes or changed its byte stream.");

sttc ::llc::err_t testPackedCounts(::llc::ATestError & errors) {
	cnst ::llc::u2_t values[] = {0, 0x3F, 0x40, 0x3FFF, 0x4000, 0x3FFFFF, 0x400000, 0x3FFFFFFF};
	cnst ::llc::u0_t widths[] = {1, 1, 2, 2, 3, 3, 4, 4};
	for(::llc::u2_t iValue = 0; iValue < ::llc::size(values); ++iValue) {
		::llc::au0_t encoded = {};
		cnst ::llc::err_t saveResult = ::llc::savePacked(encoded, values[iValue]);
		LLC_TEST_CHECKF(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_PACKED_BOUNDARIES
			, ::llc::failed(saveResult) || encoded.size() != widths[iValue]
			, "Value: %u, save result: %i, encoded width: %u, expected width: %u."
			, values[iValue], saveResult, encoded.size(), widths[iValue]
			);
		if(::llc::failed(saveResult) || encoded.size() != widths[iValue])
			continue;
		::llc::vcu0_t input = encoded;
		::llc::u2_t decoded = {};
		cnst ::llc::err_t loadResult = ::llc::loadPacked(input, decoded);
		LLC_TEST_CHECKF(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_PACKED_BOUNDARIES
			, ::llc::failed(loadResult) || decoded != values[iValue] || input.size()
			, "Value: %u, load result: %i, decoded: %u, remaining bytes: %u."
			, values[iValue], loadResult, decoded, input.size()
			);
		if(widths[iValue] > 1) {
			::llc::vcu0_t truncated = {encoded.begin(), encoded.size() - 1};
			decoded = {};
			LLC_TEST_CHECKF(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_PACKED_TRUNCATED
				, false == ::llc::failed(::llc::loadPacked(truncated, decoded))
				, "Value: %u, encoded width: %u."
				, values[iValue], encoded.size()
				);
		}
	}
	return 0;
}

sttc ::llc::err_t controlTableCreate64(::llc::SControlTable & table) {
	for(::llc::u2_t iControl = 0; iControl < 64; ++iControl)
		if_fail_fef(::llc::controlCreate(table, {}) , "iControl: %u", iControl);
	return 0;
}

sttc ::llc::err_t controlTablePopulate(::llc::SControlTable & table) {
	if_fail_fe(::controlTableCreate64(table));
	if_fail_fe(::llc::controlSetParent(table, 1, 0));
	if_fail_fe(::llc::controlSetParent(table, 2, 0));
	table.SetUnused(10, true);

	table.States[0].Flag = (::llc::GUI_CONTROL_FLAG)(::llc::GUI_CONTROL_FLAG_Selected | ::llc::GUI_CONTROL_FLAG_Hovered | ::llc::GUI_CONTROL_FLAG_Updated);
	table.States[0].Mask = (::llc::GUI_CONTROL_FLAG)(::llc::GUI_CONTROL_FLAG_Hidden | ::llc::GUI_CONTROL_FLAG_Disabled | ::llc::GUI_CONTROL_FLAG_Hovered | ::llc::GUI_CONTROL_FLAG_Selected | ::llc::GUI_CONTROL_FLAG_Updated);
	table.TextState[0].push_back({-1, ::llc::GUI_TEXT_Bold, ::llc::GUI_TEXT_FLAG_All});
	table.TextState[0].push_back({0, (::llc::GUI_TEXT_FLAG)(::llc::GUI_TEXT_Selected | ::llc::GUI_TEXT_Italic), ::llc::GUI_TEXT_FLAG_All});
	table.Identifier[0] = ::llc::vcst_t{"root_control"};
	table.Script[0] = "{\"root_control\":{\"Text\":{\"Text\":\"restored\"}}}";

	::llc::SControlConstraints & constraints = table.Constraints[0];
	constraints.AttachSizeToControl = {1, 2};
	constraints.AttachSizeToText = {true, false};
	constraints.DockToControl = {3, 4, 5, 6};
	constraints.SizeMinMax = {{7, 8}, {900, 700}};

	::llc::SControlPlacement & placement = table.Placement[0];
	placement.Area = {{11, 12}, {320, 180}};
	placement.Margin = {13, 14, 15, 16};
	placement.Border = {17, 18, 19, 20};
	placement.Align = ::llc::ALIGN_BOTTOM_RIGHT;
	placement.RelativeBorderSize = ::llc::GUI_CONTROL_LIMIT_Right;
	placement.RelativeMarginSize = ::llc::GUI_CONTROL_LIMIT_Bottom;
	placement.RelativeMetrics = (::llc::GUI_CONTROL_METRICS_FLAG)(::llc::GUI_CONTROL_METRICS_FLAG_XSize | ::llc::GUI_CONTROL_METRICS_FLAG_YOffset);
	table.Area[0].Total.Local = {{23, 24}, {25, 26}};
	table.Area[0].Client.Global = {{27, 28}, {29, 30}};
	table.Area[0].Text = {{31, 32}, {33, 34}};
	table.Area[0].Rectangles[0] = {{35, 36}, {37, 38}};
	table.Area[0].Triangles[0] = {{39, 40}, {41, 42}, {43, 44}};

	::llc::SControlDraw & draw = table.Draw[0];
	draw.ColorMode = ::llc::GUI_COLOR_MODE_Flat;
	draw.FrameOut = true;
	draw.NoBorder = true;
	draw.NoClient = false;
	draw.UseNewPalettes = true;
	draw.ColorTheme = 21;
	for(::llc::u2_t iPalette = 0; iPalette < draw.Palettes.size(); ++iPalette)
		draw.Palettes[iPalette] = (::llc::s1_t)(100 + iPalette);

	::llc::SControlText & text = table.Text[0];
	text.Text = ::llc::vcst_t{"Serialized GUI text"};
	text.Align = ::llc::ALIGN_CENTER_LEFT;
	text.Selection = {3, 9};
	text.FontSelected = 4;

	::llc::SControlImage & image = table.Images[0];
	image.ImageOffset = {5, 6};
	image.ImageAlign = ::llc::ALIGN_TOP_RIGHT;
	image.ImageInvertX = true;
	image.ImageInvertY = false;
	image.Rotation = 3;
	cnst ::llc::n2u2_t imageMetrics = {2, 1};
	if_fail_fe(image.Temp.Color.resize(imageMetrics));
	image.Temp.Color.Texels[0] = ::llc::bgra{1, 2, 3, 4};
	image.Temp.Color.Texels[1] = ::llc::bgra{5, 6, 7, 8};
	image.Image = image.Temp.Color.View;
	if_fail_fe(table.EventQueue.resize(table.EventQueue.size() + 1));
	return 0;
}

sttc bool viewBorrowsBytes(::llc::vcst_t text, ::llc::vcu0_t bytes) {
	if(0 == text.size())
		return true;
	cnst ::llc::u0_t * textBegin = (cnst ::llc::u0_t*)text.begin();
	cnst ::llc::u0_t * textEnd = (cnst ::llc::u0_t*)text.end();
	return textBegin >= bytes.begin() && textEnd <= bytes.end();
}

struct SControlCorpus {
	::llc::SGUI												GUI				= {};
	::llc::cid_t												Root			= ::llc::CID_INVALID;
	::dry::SScreen<::dry::SCREEN_CONTROL_HUD_COUNT>					Hud				= {};
	::dry::SScreen<::dry::SCREEN_CONTROL_MENU_COUNT>					Menu			= {};
	::dry::SScreen<::dry::SCREEN_CONTROL_MODAL_COUNT>					Modal			= {};
	::dry::SScreen<::dry::SCREEN_CONTROL_SPOOL_INFO_COUNT>			SpoolInfo		= {};
	::dry::SScreen<::dry::SCREEN_CONTROL_LOAD_FILAMENT_COUNT>		LoadFilament	= {};
	::dry::SScreen<::dry::SCREEN_CONTROL_INFO_COUNT>					Info			= {};
	::dry::SScreen<::dry::SCREEN_CONTROL_CONFIG_COUNT>				Config			= {};
};

sttc ::llc::err_t setupCorpus(SControlCorpus & corpus) {
	if_fail_fe(corpus.Root = ::llc::controlCreate(corpus.GUI));
	corpus.GUI.Controls.Identifier[corpus.Root] = ::llc::vcst_t{"corpus_root"};
	cnst ::llc::vs1_t defaultColors = corpus.GUI.Colors->DefaultColors;
	if_fail_fe(::dry::setupScreenFromControlParams(corpus.GUI.Controls, corpus.Root, corpus.Hud         , ::dry::control_list_hud          , defaultColors, false));
	if_fail_fe(::dry::setupScreenFromControlParams(corpus.GUI.Controls, corpus.Root, corpus.Menu        , ::dry::control_list_menu         , defaultColors, false));
	if_fail_fe(::dry::setupScreenFromControlParams(corpus.GUI.Controls, corpus.Root, corpus.Modal       , ::dry::control_list_modal        , defaultColors, false));
	if_fail_fe(::dry::setupScreenFromControlParams(corpus.GUI.Controls, corpus.Root, corpus.SpoolInfo   , ::dry::control_list_spool_info   , defaultColors, false));
	if_fail_fe(::dry::setupScreenFromControlParams(corpus.GUI.Controls, corpus.Root, corpus.LoadFilament, ::dry::control_list_load_filament, defaultColors, false));
	if_fail_fe(::dry::setupScreenFromControlParams(corpus.GUI.Controls, corpus.Root, corpus.Info        , ::dry::control_list_info         , defaultColors, false));
	if_fail_fe(::dry::setupScreenFromControlParams(corpus.GUI.Controls, corpus.Root, corpus.Config      , ::dry::control_list_config       , defaultColors, false));
	if_fail_fe(::dry::setupPalettes(*corpus.GUI.Colors));
	return 0;
}

sttc bool bytesEqual(::llc::vcu0_t left, ::llc::vcu0_t right) {
	if(left.size() != right.size())
		return false;
	for(::llc::u2_t iByte = 0; iByte < left.size(); ++iByte)
		if(left[iByte] != right[iByte])
			return false;
	return true;
}

sttc ::llc::err_t testCorpusRoundTrip(::llc::ATestError & errors) {
	SControlCorpus source = {};
	LLC_TEST_REQUIRE(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_CORPUS_SETUP, ::llc::failed(::setupCorpus(source)));
	cnst ::llc::cid_t sourceDoor = source.Hud.ControlIds[::dry::SCREEN_CONTROL_HUD_DOOR];
	::llc::img8bgra & sourceDoorStorage = source.Hud.ControlIcons[::dry::SCREEN_CONTROL_HUD_DOOR].Off;
	LLC_TEST_REQUIRE(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_CORPUS_IMAGES, ::llc::failed(sourceDoorStorage.resize(::llc::n2u2_t{1, 1})));
	sourceDoorStorage[0][0] = ::llc::bgra{1, 2, 3, 4};
	source.GUI.Controls.Images[sourceDoor].Image = sourceDoorStorage.View;
	cnst ::llc::u2_t expectedControlCount
		= 1
		+ ::llc::size(::dry::control_list_hud)
		+ ::llc::size(::dry::control_list_menu)
		+ ::llc::size(::dry::control_list_modal)
		+ ::llc::size(::dry::control_list_spool_info)
		+ ::llc::size(::dry::control_list_load_filament)
		+ ::llc::size(::dry::control_list_info)
		+ ::llc::size(::dry::control_list_config)
		;
	cnst ::llc::cid_t sourceSpoolDialog = source.SpoolInfo.ControlIds[::dry::SCREEN_CONTROL_SPOOL_INFO_DIALOG];
	cnst ::llc::cid_t sourceSpoolTitle = source.SpoolInfo.ControlIds[::dry::SCREEN_CONTROL_SPOOL_INFO_TITLE];
	LLC_TEST_CHECKF(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_CORPUS_STRUCTURE
		, source.GUI.Controls.States.size() != expectedControlCount
		|| source.GUI.Controls.Children[source.Root].size() != ::dry::SCREEN_COUNT - 1
		|| source.GUI.Controls.States[sourceSpoolDialog].Parent != source.Root
		|| source.GUI.Controls.States[sourceSpoolTitle].Parent != sourceSpoolDialog
		|| source.GUI.Controls.Text[sourceSpoolTitle].Text != ::llc::vcst_t{"Spool Scanned!"}
		, "Controls: %u; expected: %u; root children: %u."
		, source.GUI.Controls.States.size(), expectedControlCount, source.GUI.Controls.Children[source.Root].size()
		);

	::llc::au0_t serialized = {};
	LLC_TEST_REQUIRE(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_SAVE, ::llc::failed(source.GUI.Controls.Save(serialized)));
	SControlCorpus loaded = {};
	LLC_TEST_REQUIRE(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_CORPUS_SETUP, ::llc::failed(::setupCorpus(loaded)));
	cnst ::llc::cid_t loadedDoor = loaded.Hud.ControlIds[::dry::SCREEN_CONTROL_HUD_DOOR];
	::llc::img8bgra & loadedDoorStorage = loaded.Hud.ControlIcons[::dry::SCREEN_CONTROL_HUD_DOOR].Off;
	LLC_TEST_REQUIRE(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_CORPUS_IMAGES, ::llc::failed(loadedDoorStorage.resize(::llc::n2u2_t{1, 1})));
	::llc::SControlImage & separatelyLoadedImage = loaded.GUI.Controls.Images[loadedDoor];
	cnst ::llc::bgra separatelyLoadedMarker = {17, 34, 51, 68};
	loadedDoorStorage[0][0] = separatelyLoadedMarker;
	separatelyLoadedImage.Image = loadedDoorStorage.View;
	cnst ::llc::n2u2_t separatelyLoadedMetrics = separatelyLoadedImage.Image.metrics();
	separatelyLoadedImage.ImageAlign = ::llc::ALIGN_BOTTOM_RIGHT;

	::llc::vcu0_t input = serialized;
	LLC_TEST_REQUIRE(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_LOAD, ::llc::failed(loaded.GUI.Controls.Load(input)));
	LLC_TEST_CHECKF(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_CORPUS_CONSUMPTION, input.size(), "Corpus unread bytes: %u.", input.size());
	LLC_TEST_CHECK(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_CORPUS_IMAGES
		, loaded.GUI.Controls.Images[loadedDoor].Image.metrics() != separatelyLoadedMetrics
		|| loaded.GUI.Controls.Images[loadedDoor].Image[0][0] != separatelyLoadedMarker
		|| loaded.GUI.Controls.Images[loadedDoor].ImageAlign != ::llc::ALIGN_BOTTOM_RIGHT
		);
	::llc::au0_t serializedAgain = {};
	LLC_TEST_REQUIRE(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_SAVE, ::llc::failed(loaded.GUI.Controls.Save(serializedAgain)));
	LLC_TEST_CHECKF(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_CORPUS_CONSUMPTION
		, false == ::bytesEqual(serialized, serializedAgain)
		, "Original bytes: %u; second save bytes: %u.", serialized.size(), serializedAgain.size()
		);
	cnst ::llc::vcu0_t serializedBytes = serialized;
	for(::llc::u2_t iControl = 0; iControl < loaded.GUI.Controls.Text.size(); ++iControl) {
		LLC_TEST_CHECKF(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_CORPUS_STRINGS
			, false == ::viewBorrowsBytes(loaded.GUI.Controls.Text[iControl].Text, serializedBytes)
			, "Corpus control: %u; text length: %u.", iControl, loaded.GUI.Controls.Text[iControl].Text.size()
			);
		LLC_TEST_CHECKF(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_CORPUS_STRINGS
			, false == ::viewBorrowsBytes(loaded.GUI.Controls.Identifier[iControl], serializedBytes)
			, "Corpus control: %u; identifier length: %u.", iControl, loaded.GUI.Controls.Identifier[iControl].size()
			);
	}
	return 0;
}

::llc::err_t test_gui_control_serialization(STestRunnerState &, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	if_fail_fe(::testPackedCounts(errors));
	if_fail_fe(::testCorpusRoundTrip(errors));
	::llc::SGUI sourceGUI = {};
	::llc::SControlTable & source = sourceGUI.Controls;
	if_fail_fe(::controlTablePopulate(source));
	::llc::au0_t serialized = {};
	LLC_TEST_REQUIRE(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_SAVE, ::llc::failed(sourceGUI.Controls.Save(serialized)));
	LLC_TEST_REQUIRE(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_PACKED_COUNT, serialized.size() < 2);
	LLC_TEST_CHECKF(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_PACKED_COUNT, serialized[0] != 1 || serialized[1] != 64, "Packed count bytes: 0x%02X 0x%02X; expected: 0x01 0x40.", serialized[0], serialized[1]);
	::llc::au0_t serializedWithoutImage = {};
	source.Images[0].Image = {};
	LLC_TEST_REQUIRE(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_SAVE, ::llc::failed(sourceGUI.Controls.Save(serializedWithoutImage)));
	source.Images[0].Image = source.Images[0].Temp.Color.View;
	::llc::u2_t imageMetricByteDifferences = 0;
	for(::llc::u2_t iByte = 0, byteCount = ::llc::min(serialized.size(), serializedWithoutImage.size()); iByte < byteCount; ++iByte)
		imageMetricByteDifferences += serialized[iByte] != serializedWithoutImage[iByte];
	LLC_TEST_CHECKF(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_IMAGE
		, serialized.size() != serializedWithoutImage.size() || imageMetricByteDifferences != 2
		, "Serialized bytes with image: %u; without image: %u; differing W/H bytes: %u."
		, serialized.size(), serializedWithoutImage.size(), imageMetricByteDifferences
		);

	::llc::SGUI restoredGUI = {};
	::llc::SControlTable & restored = restoredGUI.Controls;
	if_fail_fe(::controlTableCreate64(restored));
	cnst ::llc::n2u2_t separatelyLoadedImageMetrics = {1, 1};
	cnst ::llc::n2s1_t separatelyLoadedImageOffset = {45, 46};
	cnst ::llc::bgra separatelyLoadedPixel = {9, 10, 11, 12};
	if_fail_fe(restored.Images[0].Temp.Color.resize(separatelyLoadedImageMetrics));
	restored.Images[0].Temp.Color.Texels[0] = separatelyLoadedPixel;
	restored.Images[0].Image = restored.Images[0].Temp.Color.View;
	restored.Images[0].ImageOffset = separatelyLoadedImageOffset;
	restored.Images[0].ImageAlign = ::llc::ALIGN_BOTTOM_LEFT;
	restored.Images[0].ImageInvertY = true;
	restored.Images[0].Rotation = 2;
	::llc::vcu0_t input = serialized;
	LLC_TEST_REQUIRE(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_LOAD, ::llc::failed(restoredGUI.Controls.Load(input)));
	LLC_TEST_CHECKF(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_CONSUMPTION, input.size(), "Remaining bytes: %u.", input.size());
	LLC_TEST_REQUIRE(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_PARALLEL_ARRAYS, restored.States.size() != 64);
	LLC_TEST_CHECK(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_PARALLEL_ARRAYS
		, restored.TextState.size() != 64 || restored.Identifier.size() != 64 || restored.Script.size() != 64
		|| restored.Constraints.size() != 64 || restored.Placement.size() != 64 || restored.Area.size() != 64
		|| restored.Draw.size() != 64 || restored.Text.size() != 64 || restored.Images.size() != 64 || restored.Children.size() != 64
		);
	LLC_TEST_CHECK(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_STATE
		, restored.States[0].Parent != source.States[0].Parent
		|| restored.States[0].Mask != source.States[0].Mask
		|| false == restored.States[0].IsSelected() || false == restored.States[0].IsHovered() || false == restored.States[0].IsUpdated()
		|| false == restored.States[10].IsUnused()
		);
	LLC_TEST_CHECK(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_TEXT_STATE
		, restored.TextState[0].size() != 2
		|| restored.TextState[0][0].Parent != -1 || false == restored.TextState[0][0].IsBold()
		|| restored.TextState[0][1].Parent != 0 || false == restored.TextState[0][1].IsSelected() || false == restored.TextState[0][1].IsItalic()
		);
	LLC_TEST_CHECK(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_STRINGS
		, restored.Identifier[0] != source.Identifier[0] || restored.Script[0] != source.Script[0] || restored.Text[0].Text != source.Text[0].Text
		|| (cnst ::llc::u0_t*)restored.Identifier[0].begin() < serialized.begin() || (cnst ::llc::u0_t*)restored.Identifier[0].end() > serialized.end()
		|| (cnst ::llc::u0_t*)restored.Text[0].Text.begin() < serialized.begin() || (cnst ::llc::u0_t*)restored.Text[0].Text.end() > serialized.end()
		);
	LLC_TEST_CHECK(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_COMPONENTS
		, restored.Constraints[0].AttachSizeToControl != source.Constraints[0].AttachSizeToControl
		|| restored.Constraints[0].DockToControl != source.Constraints[0].DockToControl
		|| restored.Placement[0].Area != source.Placement[0].Area || restored.Placement[0].Align != source.Placement[0].Align
		|| restored.Draw[0].ColorTheme != source.Draw[0].ColorTheme || restored.Draw[0].Palettes != source.Draw[0].Palettes
		|| restored.Text[0].Selection != source.Text[0].Selection || restored.Text[0].FontSelected != source.Text[0].FontSelected
		);
	cnst ::llc::n2u2_t sourceImageMetrics = source.Views[0].metrics();
	cnst ::llc::n2u2_t expectedImageMetrics = {2, 1};
	LLC_TEST_CHECKF(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_IMAGE
		, sourceImageMetrics != expectedImageMetrics || restored.Images[0].Image.metrics() != separatelyLoadedImageMetrics
		|| restored.Views[0][0][0] != separatelyLoadedPixel
		|| restored.Images[0].ImageOffset != separatelyLoadedImageOffset || restored.Images[0].ImageAlign != ::llc::ALIGN_BOTTOM_LEFT
		|| restored.Images[0].ImageInvertX || false == restored.Images[0].ImageInvertY || restored.Images[0].Rotation != 2
		, "Source metrics: %u x %u; restored texel count: %u; restored offset: %i, %i; align: %u; invert: %u, %u; rotation: %u."
		, sourceImageMetrics.x, sourceImageMetrics.y, restored.Images[0].Image.size()
		, restored.Images[0].ImageOffset.x, restored.Images[0].ImageOffset.y, restored.Images[0].ImageAlign
		, (::llc::u2_t)restored.Images[0].ImageInvertX, (::llc::u2_t)restored.Images[0].ImageInvertY, (::llc::u2_t)restored.Images[0].Rotation
		);
	LLC_TEST_CHECK(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_HIERARCHY
		, restored.States[1].Parent != 0 || restored.States[2].Parent != 0
		|| restored.Children[0].size() != 2 || restored.Children[0][0] != 1 || restored.Children[0][1] != 2
		);
	LLC_TEST_CHECK(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_TABLE_MEMBERS
		, restored.Area[0].Total.Local != source.Area[0].Total.Local
		|| restored.Area[0].Client.Global != source.Area[0].Client.Global
		|| restored.Area[0].Text != source.Area[0].Text
		|| restored.Area[0].Rectangles[0] != source.Area[0].Rectangles[0]
		|| restored.Area[0].Triangles[0] != source.Area[0].Triangles[0]
		|| restored.EventQueue.size() != source.EventQueue.size()
		|| restored.EventQueue.size() && (restored.EventQueue[0]->Type != source.EventQueue[0]->Type || restored.EventQueue[0]->Data != source.EventQueue[0]->Data)
		|| restored.EventQueue.size() < 2 || 0 == (cnst ::llc::SEventSystem*)restored.EventQueue[1]
		|| restored.EventQueue[1]->Type != ::llc::SYSTEM_EVENT_Runtime || restored.EventQueue[1]->Data.size()
		);

	::llc::SGUI truncatedGUI = {};
	::llc::vcu0_t truncatedInput = {serialized.begin(), serialized.size() - 1};
	LLC_TEST_CHECK(errors, GUI_CONTROL_SERIALIZATION_TEST_RESULT_TRUNCATED_REJECT
		, ::llc::succeeded(truncatedGUI.Controls.Load(truncatedInput))
		);
	return 0;
}
