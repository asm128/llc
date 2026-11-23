#include "llc_ctti_stct_json.h"
#include "llc_framework_module.h"
#include "llc_gui.h"
#include "llc_gui_control.h"
#include "llc_gui_control_json.h"
#include "llc_gui_control_json_expression.h"

#include "test_llc_suites.h"

usns llc;

GDEFINE_ENUM_TYPE(GUI_JSON_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(GUI_JSON_TEST_RESULT, CONTROL_CREATE	, 0, "The test could not create the source or destination control.");
GDEFINE_ENUM_VALUED(GUI_JSON_TEST_RESULT, TARGET_DEFAULT	, 1, "The destination control was not blank before loading.");
GDEFINE_ENUM_VALUED(GUI_JSON_TEST_RESULT, STATE_SAVE		, 2, "The SControlState table entry could not be saved to JSON.");
GDEFINE_ENUM_VALUED(GUI_JSON_TEST_RESULT, STATE_LOAD		, 3, "The SControlState JSON could not populate the destination table entry.");
GDEFINE_ENUM_VALUED(GUI_JSON_TEST_RESULT, STATE_VALUE		, 4, "The SControlState destination differs from the source.");
GDEFINE_ENUM_VALUED(GUI_JSON_TEST_RESULT, TEXT_SAVE		, 5, "The SControlText table entry could not be saved to JSON.");
GDEFINE_ENUM_VALUED(GUI_JSON_TEST_RESULT, TEXT_LOAD		, 6, "The SControlText JSON could not populate the destination table entry.");
GDEFINE_ENUM_VALUED(GUI_JSON_TEST_RESULT, TEXT_VALUE		, 7, "The SControlText destination differs from the source.");
GDEFINE_ENUM_VALUED(GUI_JSON_TEST_RESULT, INPUT_CURSOR	, 8, "The JSON input cursor was not fully consumed.");
GDEFINE_ENUM_VALUED(GUI_JSON_TEST_RESULT, TABLE_SAVE		, 9, "The control table could not be saved through its JSON layer.");
GDEFINE_ENUM_VALUED(GUI_JSON_TEST_RESULT, TABLE_PARSE	, 10, "The generated control-table JSON could not be parsed.");
GDEFINE_ENUM_VALUED(GUI_JSON_TEST_RESULT, TABLE_LOAD		, 11, "The control table could not be loaded through its JSON layer.");
GDEFINE_ENUM_VALUED(GUI_JSON_TEST_RESULT, BINARY_PARITY	, 12, "The JSON round trip does not match the established binary Save() contract.");
GDEFINE_ENUM_VALUED(GUI_JSON_TEST_RESULT, IMAGE_PRESERVE	, 13, "JSON loading changed separately owned control-image data.");
GDEFINE_ENUM_VALUED(GUI_JSON_TEST_RESULT, SCRIPT_RUN		, 14, "The control-table JSON script could not be executed.");
GDEFINE_ENUM_VALUED(GUI_JSON_TEST_RESULT, SCRIPT_LITERAL	, 15, "A literal JSON script did not update the target control.");
GDEFINE_ENUM_VALUED(GUI_JSON_TEST_RESULT, SCRIPT_EXPRESSION, 16, "A JSON expression did not copy the live control value.");
GDEFINE_ENUM_VALUED(GUI_JSON_TEST_RESULT, SCRIPT_STORED	, 17, "The script stored by control id did not execute.");
GDEFINE_ENUM_VALUED(GUI_JSON_TEST_RESULT, SCRIPT_ACTION	, 18, "Action-driven script execution did not update the target control.");
GDEFINE_ENUM_VALUED(GUI_JSON_TEST_RESULT, SCRIPT_EVENT	, 19, "The framework event binding did not enqueue the requested child event.");
GDEFINE_ENUM_VALUED(GUI_JSON_TEST_RESULT, GUI_SCRIPT_LOAD	, 20, "The GUI Exit control could not be constructed from JSON.");
GDEFINE_ENUM_VALUED(GUI_JSON_TEST_RESULT, GUI_SCRIPT_EVENT	, 21, "The GUI Runtime.Event script did not enqueue APPLICATION_STATE_EXIT.");

sttc ::llc::b8_t bytesEqual(::llc::vcu0_t left, ::llc::vcu0_t right) {
	if(left.size() != right.size())
		rtrn false;
	for(::llc::u2_t iByte = 0; iByte < left.size(); ++iByte)
		if(left[iByte] != right[iByte])
			rtrn false;
	rtrn true;
}

sttc ::llc::err_t testControlTableJSONBinaryParity(::llc::ATestError & errors) {
	::llc::SControlTable source = {};
	cnst ::llc::cid_t sourceRoot = ::llc::controlCreate(source, {});
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_CONTROL_CREATE, ::llc::failed(sourceRoot));
	cnst ::llc::cid_t sourceChild = ::llc::controlCreate(source, {});
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_CONTROL_CREATE, ::llc::failed(sourceChild));
	cnst ::llc::cid_t sourceUnused = ::llc::controlCreate(source, {});
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_CONTROL_CREATE, ::llc::failed(sourceUnused));
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_TABLE_LOAD, ::llc::failed(::llc::controlSetParent(source, sourceChild, sourceRoot)));
	source.Identifier[sourceRoot] = "root";
	source.Identifier[sourceChild] = "child";
	source.Identifier[sourceUnused] = "unused";
	source.SetUnused(sourceUnused, true);
	source.Text[sourceUnused].Text = "unused retained";
	source.States[sourceRoot].Flag = (::llc::GUI_CONTROL_FLAG)(::llc::GUI_CONTROL_FLAG_Hidden | ::llc::GUI_CONTROL_FLAG_Focused | ::llc::GUI_CONTROL_FLAG_Selected);
	source.Text[sourceRoot].Text = "root text";
	source.Text[sourceRoot].Align = ::llc::ALIGN_BOTTOM_RIGHT;
	source.Text[sourceRoot].Selection = {2, 7};
	source.Text[sourceRoot].FontSelected = 3;
	source.Script[sourceChild] = "{\"child\":{\"State\":{\"Hidden\":true}}}";
	source.TextState[sourceRoot].push_back({-1, ::llc::GUI_TEXT_Bold, ::llc::GUI_TEXT_FLAG_All});
	source.TextState[sourceRoot].push_back({0, (::llc::GUI_TEXT_FLAG)(::llc::GUI_TEXT_Selected | ::llc::GUI_TEXT_Italic), ::llc::GUI_TEXT_FLAG_All});
	source.Constraints[sourceRoot].AttachSizeToControl = {sourceChild, sourceChild};
	source.Constraints[sourceRoot].AttachSizeToText = {true, false};
	source.Constraints[sourceRoot].DockToControl = {sourceChild, sourceChild, sourceChild, sourceChild};
	source.Constraints[sourceRoot].SizeMinMax = {{7, 8}, {900, 700}};
	source.Placement[sourceRoot].Area = {{11, 12}, {320, 180}};
	source.Placement[sourceRoot].Margin = {13, 14, 15, 16};
	source.Placement[sourceRoot].Border = {17, 18, 19, 20};
	source.Placement[sourceRoot].Align = ::llc::ALIGN_BOTTOM_RIGHT;
	source.Placement[sourceRoot].RelativeBorderSize = ::llc::GUI_CONTROL_LIMIT_Right;
	source.Placement[sourceRoot].RelativeMarginSize = ::llc::GUI_CONTROL_LIMIT_Bottom;
	source.Placement[sourceRoot].RelativeMetrics = (::llc::GUI_CONTROL_METRICS_FLAG)(::llc::GUI_CONTROL_METRICS_FLAG_XSize | ::llc::GUI_CONTROL_METRICS_FLAG_YOffset);
	source.Draw[sourceRoot].ColorMode = ::llc::GUI_COLOR_MODE_Flat;
	source.Draw[sourceRoot].FrameOut = true;
	source.Draw[sourceRoot].NoBorder = true;
	source.Draw[sourceRoot].UseNewPalettes = true;
	source.Draw[sourceRoot].ColorTheme = 21;
	for(::llc::u2_t iPalette = 0; iPalette < source.Draw[sourceRoot].Palettes.size(); ++iPalette)
		source.Draw[sourceRoot].Palettes[iPalette] = (::llc::s1_t)(100 + iPalette);
	source.Area[sourceRoot].Total.Local = {{23, 24}, {25, 26}};
	source.Area[sourceRoot].Client.Global = {{27, 28}, {29, 30}};
	source.Area[sourceRoot].Text = {{31, 32}, {33, 34}};
	source.Area[sourceRoot].Rectangles[0] = {{35, 36}, {37, 38}};
	source.Area[sourceRoot].Triangles[0] = {{39, 40}, {41, 42}, {43, 44}};
	if_fail_fe(source.Images[sourceRoot].Temp.Color.resize(::llc::n2u2_t{2, 1}));
	source.Images[sourceRoot].Temp.Color.Texels[0] = {1, 2, 3, 4};
	source.Images[sourceRoot].Temp.Color.Texels[1] = {5, 6, 7, 8};
	source.Images[sourceRoot].Image = source.Images[sourceRoot].Temp.Color.View;
	if_fail_fe(source.EventQueue.resize(2));
	source.EventQueue[0]->Type = ::llc::SYSTEM_EVENT_GUI;
	if_fail_fe(source.EventQueue[0]->Data.push_back(1));
	if_fail_fe(source.EventQueue[0]->Data.push_back(2));
	if_fail_fe(source.EventQueue[0]->Data.push_back(3));
	if_fail_fe(source.EventQueue[0]->Data.push_back(4));

	::llc::SControlTable target = {};
	cnst ::llc::cid_t targetRoot = ::llc::controlCreate(target, {});
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_CONTROL_CREATE, ::llc::failed(targetRoot));
	cnst ::llc::cid_t targetChild = ::llc::controlCreate(target, {});
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_CONTROL_CREATE, ::llc::failed(targetChild));
	cnst ::llc::cid_t targetUnused = ::llc::controlCreate(target, {});
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_CONTROL_CREATE, ::llc::failed(targetUnused));
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_TABLE_LOAD, ::llc::failed(::llc::controlSetParent(target, targetChild, targetRoot)));
	target.Identifier[targetRoot] = "root";
	target.Identifier[targetChild] = "child";
	target.Identifier[targetUnused] = "unused";
	target.SetUnused(targetUnused, true);
	if_fail_fe(target.Images[targetRoot].Temp.Color.resize(::llc::n2u2_t{2, 1}));
	target.Images[targetRoot].Temp.Color.Texels[0] = {9, 10, 11, 12};
	target.Images[targetRoot].Temp.Color.Texels[1] = {13, 14, 15, 16};
	target.Images[targetRoot].Image = target.Images[targetRoot].Temp.Color.View;
	target.Images[targetRoot].ImageOffset = {45, 46};
	target.Images[targetRoot].ImageAlign = ::llc::ALIGN_BOTTOM_LEFT;
	target.Images[targetRoot].ImageInvertY = true;
	target.Images[targetRoot].Rotation = 2;
	cnst ::llc::n2u2_t targetImageMetrics = target.Images[targetRoot].Image.metrics();
	cnst ::llc::bgra targetImagePixel = target.Images[targetRoot].Image[0][0];
	cnst ::llc::n2s1_t targetImageOffset = target.Images[targetRoot].ImageOffset;
	cnst ::llc::u0_t targetImageAlign = target.Images[targetRoot].ImageAlign;
	cnst ::llc::u0_t targetImageInvertX = target.Images[targetRoot].ImageInvertX;
	cnst ::llc::u0_t targetImageInvertY = target.Images[targetRoot].ImageInvertY;
	cnst ::llc::u0_t targetImageRotation = target.Images[targetRoot].Rotation;

	::llc::string json = {};
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_TABLE_SAVE, ::llc::failed(::llc::controlTableJSONSave(source, json)));
	::llc::SJSONReader reader = {};
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_TABLE_PARSE, ::llc::failed(::llc::jsonParse(reader, json)));
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_TABLE_LOAD, ::llc::failed(::llc::controlTableJSONLoad(target, reader)));
	LLC_TEST_CHECK(errors, GUI_JSON_TEST_RESULT_IMAGE_PRESERVE
		, target.Images[targetRoot].Image.metrics() != targetImageMetrics
		|| target.Images[targetRoot].Image[0][0] != targetImagePixel
		|| target.Images[targetRoot].ImageOffset != targetImageOffset
		|| target.Images[targetRoot].ImageAlign != targetImageAlign
		|| target.Images[targetRoot].ImageInvertX != targetImageInvertX
		|| target.Images[targetRoot].ImageInvertY != targetImageInvertY
		|| target.Images[targetRoot].Rotation != targetImageRotation
		);

	::llc::au0_t sourceBinary = {}, targetBinary = {};
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_BINARY_PARITY, ::llc::failed(source.Save(sourceBinary)));
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_BINARY_PARITY, ::llc::failed(target.Save(targetBinary)));
	LLC_TEST_CHECKF(errors, GUI_JSON_TEST_RESULT_BINARY_PARITY
		, false == ::bytesEqual(sourceBinary, targetBinary)
		, "JSON:'%.*s'; source bytes:%u; target bytes:%u."
		, LLC_VIEW_ARGS(json), sourceBinary.size(), targetBinary.size()
		);
	rtrn 0;
}

sttc ::llc::err_t testControlTableJSONScript(::llc::ATestError & errors) {
	::llc::SControlTable controls = {};
	cnst ::llc::cid_t source = ::llc::controlCreate(controls, {});
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_CONTROL_CREATE, ::llc::failed(source));
	cnst ::llc::cid_t target = ::llc::controlCreate(controls, {});
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_CONTROL_CREATE, ::llc::failed(target));
	cnst ::llc::cid_t action = ::llc::controlCreate(controls, {});
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_CONTROL_CREATE, ::llc::failed(action));
	controls.Identifier[source] = "source";
	controls.Identifier[target] = "target";
	controls.Identifier[action] = "action";
	controls.Draw[source].ColorTheme = 27;

	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_SCRIPT_RUN
		, ::llc::failed(::llc::controlTableScriptRun(controls, R"({"target":{"Draw":{"ColorMode":2}}})"))
		);
	LLC_TEST_CHECK(errors, GUI_JSON_TEST_RESULT_SCRIPT_LITERAL, controls.Draw[target].ColorMode != ::llc::GUI_COLOR_MODE_Flat);

	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_SCRIPT_RUN
		, ::llc::failed(::llc::controlTableScriptRun(controls, R"({"target":{"Draw":{"ColorTheme":"{source.Draw.ColorTheme}"}}})"))
		);
	LLC_TEST_CHECKF(errors, GUI_JSON_TEST_RESULT_SCRIPT_EXPRESSION, controls.Draw[target].ColorTheme != controls.Draw[source].ColorTheme
		, "Source:%i, target:%i.", controls.Draw[source].ColorTheme, controls.Draw[target].ColorTheme
		);

	controls.Script[action] = R"({"target":{"Placement":{"Align":"BOTTOM_RIGHT"}}})";
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_SCRIPT_RUN, ::llc::failed(::llc::controlTableScriptRun(controls, action)));
	LLC_TEST_CHECK(errors, GUI_JSON_TEST_RESULT_SCRIPT_STORED, controls.Placement[target].Align != ::llc::ALIGN_BOTTOM_RIGHT);

	controls.Script[action] = R"({"target":{"State":{"Flag":1}}})";
	controls.SetAction(action, true);
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_SCRIPT_RUN, ::llc::failed(::llc::controlTableScriptUpdate(controls)));
	LLC_TEST_CHECK(errors, GUI_JSON_TEST_RESULT_SCRIPT_ACTION, false == controls.States[target].IsHidden());

	controls.EventQueue.clear();
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_SCRIPT_RUN
		, ::llc::failed(::llc::controlTableScriptRun(controls, R"({"framework":{"event":{"Type":5,"Data":[9,8,7]}}})"))
		);
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_SCRIPT_EVENT, 1 != controls.EventQueue.size());
	::llc::SEvent<::llc::u0_t> childEvent = {};
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_SCRIPT_EVENT, ::llc::failed(controls.EventQueue[0]->ExtractChild(childEvent)));
	LLC_TEST_CHECK(errors, GUI_JSON_TEST_RESULT_SCRIPT_EVENT
		, 5 != childEvent.Type || 3 != childEvent.Data.size() || 9 != childEvent.Data[0] || 8 != childEvent.Data[1] || 7 != childEvent.Data[2]
		);
	rtrn 0;
}

sttc ::llc::err_t testGUIJSONExitScript(::llc::ATestError & errors) {
	::llc::SGUI gui = {};
	::llc::vcst_t document = R"({"exit":{"Placement":{"Area":{"Offset":{"x":2,"y":2},"Size":{"x":52,"y":20}},"Align":"TOP_RIGHT"},"Text":{"Text":"EXIT","Align":"CENTER"},"Script":"{\"Runtime\":{\"Event\":{\"Type\":\"EXIT\"}}}"}})";
	::llc::SJSONReader reader = {};
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_GUI_SCRIPT_LOAD, ::llc::failed(::llc::jsonParse(reader, document)));
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_GUI_SCRIPT_LOAD, 1 != ::llc::controlTableJSONLoad(gui.Controls, ::llc::vs1_t{}, reader));
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_GUI_SCRIPT_LOAD, 1 != gui.Controls.States.size());
	LLC_TEST_CHECK(errors, GUI_JSON_TEST_RESULT_GUI_SCRIPT_LOAD, gui.Controls.Identifier[0] != "exit" || gui.Controls.Text[0].Text != "EXIT");

	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_GUI_SCRIPT_EVENT, ::llc::failed(::llc::guiScriptRun(gui, 0)));
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_GUI_SCRIPT_EVENT, 1 != gui.Controls.EventQueue.size());
	LLC_TEST_CHECK(errors, GUI_JSON_TEST_RESULT_GUI_SCRIPT_EVENT, gui.Controls.EventQueue[0]->Type != ::llc::SYSTEM_EVENT_Framework);
	::llc::SEvent<::llc::u0_t> childEvent = {};
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_GUI_SCRIPT_EVENT, ::llc::failed(gui.Controls.EventQueue[0]->ExtractChild(childEvent)));
	LLC_TEST_CHECK(errors, GUI_JSON_TEST_RESULT_GUI_SCRIPT_EVENT, childEvent.Type != ::llc::APPLICATION_STATE_EXIT || childEvent.Data.size());
	rtrn 0;
}

sttc ::llc::err_t testControlStateJSONRoundTrip(::llc::ATestError & errors) {
	::llc::SControlTable controls = {};
	cnst ::llc::cid_t idSource = ::llc::controlCreate(controls, {});
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_CONTROL_CREATE, ::llc::failed(idSource));
	cnst ::llc::cid_t idTarget = ::llc::controlCreate(controls, {});
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_CONTROL_CREATE, ::llc::failed(idTarget));

	::llc::SControlState & source = controls.States[idSource];
	source.Parent = 42;
	source.Flag = (::llc::GUI_CONTROL_FLAG)(::llc::GUI_CONTROL_FLAG_Hidden | ::llc::GUI_CONTROL_FLAG_Focused | ::llc::GUI_CONTROL_FLAG_Selected);
	source.Mask = (::llc::GUI_CONTROL_FLAG)(::llc::GUI_CONTROL_FLAG_Hidden | ::llc::GUI_CONTROL_FLAG_Focused | ::llc::GUI_CONTROL_FLAG_Selected | ::llc::GUI_CONTROL_FLAG_Action);

	cnst ::llc::SControlState & blank = controls.States[idTarget];
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_TARGET_DEFAULT
		, blank.Parent == source.Parent || blank.Flag == source.Flag || blank.Mask == source.Mask
		);

	::llc::string json = {};
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_STATE_SAVE, ::llc::failed(::llc::saveCTTI(json, source)));
	::llc::vcst_t jsonInput = json;
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_STATE_LOAD, ::llc::failed(::llc::loadCTTI(jsonInput, controls.States[idTarget])));
	LLC_TEST_CHECK(errors, GUI_JSON_TEST_RESULT_INPUT_CURSOR, jsonInput.size());

	cnst ::llc::SControlState & target = controls.States[idTarget];
	LLC_TEST_CHECKF(errors, GUI_JSON_TEST_RESULT_STATE_VALUE
		, target.Parent != source.Parent || target.Flag != source.Flag || target.Mask != source.Mask
		, "JSON:'%.*s'; loaded:{Parent:%i,Flag:0x%X,Mask:0x%X}; expected:{Parent:%i,Flag:0x%X,Mask:0x%X}."
		, LLC_VIEW_ARGS(json)
		, target.Parent, (::llc::u2_t)target.Flag, (::llc::u2_t)target.Mask
		, source.Parent, (::llc::u2_t)source.Flag, (::llc::u2_t)source.Mask
		);
	rtrn 0;
}

sttc ::llc::err_t testControlTextJSONRoundTrip(::llc::ATestError & errors) {
	::llc::SControlTable controls = {};
	cnst ::llc::cid_t idSource = ::llc::controlCreate(controls, {});
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_CONTROL_CREATE, ::llc::failed(idSource));
	cnst ::llc::cid_t idTarget = ::llc::controlCreate(controls, {});
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_CONTROL_CREATE, ::llc::failed(idTarget));

	::llc::SControlText & source = controls.Text[idSource];
	source.Text = "CTTI table round trip";
	source.Align = ::llc::ALIGN_BOTTOM_RIGHT;
	source.Selection = {3, 11};
	source.FontSelected = 7;

	cnst ::llc::SControlText & blank = controls.Text[idTarget];
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_TARGET_DEFAULT
		, blank.Text == source.Text || blank.Align == source.Align || blank.Selection.Min == source.Selection.Min || blank.Selection.Max == source.Selection.Max || blank.FontSelected == source.FontSelected
		);

	::llc::string json = {};
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_TEXT_SAVE, ::llc::failed(::llc::saveCTTI(json, source)));
	::llc::vcst_t jsonInput = json;
	LLC_TEST_REQUIRE(errors, GUI_JSON_TEST_RESULT_TEXT_LOAD, ::llc::failed(::llc::loadCTTI(jsonInput, controls.Text[idTarget])));
	LLC_TEST_CHECK(errors, GUI_JSON_TEST_RESULT_INPUT_CURSOR, jsonInput.size());

	cnst ::llc::SControlText & target = controls.Text[idTarget];
	LLC_TEST_CHECKF(errors, GUI_JSON_TEST_RESULT_TEXT_VALUE
		, target.Text != source.Text || target.Align != source.Align || target.Selection.Min != source.Selection.Min || target.Selection.Max != source.Selection.Max || target.FontSelected != source.FontSelected
		, "JSON:'%.*s'; loaded:{Text:'%.*s',Align:0x%X,Selection:{%u,%u},Font:%i}; expected:{Text:'%.*s',Align:0x%X,Selection:{%u,%u},Font:%i}."
		, LLC_VIEW_ARGS(json)
		, LLC_VIEW_ARGS(target.Text), (::llc::u2_t)target.Align, target.Selection.Min, target.Selection.Max, target.FontSelected
		, LLC_VIEW_ARGS(source.Text), (::llc::u2_t)source.Align, source.Selection.Min, source.Selection.Max, source.FontSelected
		);
	rtrn 0;
}

::llc::err_t test_gui_json(STestRunnerState &, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t) {
	if_fail_fe(::testControlStateJSONRoundTrip(errors));
	if_fail_fe(::testControlTextJSONRoundTrip(errors));
	if_fail_fe(::testControlTableJSONBinaryParity(errors));
	if_fail_fe(::testControlTableJSONScript(errors));
	if_fail_fe(::testGUIJSONExitScript(errors));
	rtrn 0;
}
