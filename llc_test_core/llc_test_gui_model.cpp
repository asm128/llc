#include "llc_gui.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(GUI_MODEL_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(GUI_MODEL_TEST_RESULT, CREATE_RESULT			,  0, "The GUI model could not create a valid control.");
GDEFINE_ENUM_VALUED(GUI_MODEL_TEST_RESULT, CREATE_CHILD_RESULT	,  1, "The GUI model could not create a valid child control.");
GDEFINE_ENUM_VALUED(GUI_MODEL_TEST_RESULT, CHILD_PARENT			,  2, "The GUI model created a child under the wrong parent.");
GDEFINE_ENUM_VALUED(GUI_MODEL_TEST_RESULT, TEXT_RESULT			,  3, "The GUI model rejected valid control text.");
GDEFINE_ENUM_VALUED(GUI_MODEL_TEST_RESULT, TEXT_VALUE			,  4, "The GUI model stored the wrong control text.");
GDEFINE_ENUM_VALUED(GUI_MODEL_TEST_RESULT, TEXT_INVALIDATION	,  5, "Changing control text did not invalidate its metrics.");
GDEFINE_ENUM_VALUED(GUI_MODEL_TEST_RESULT, TEXT_REPEAT			,  6, "Repeating unchanged control text invalidated its metrics.");
GDEFINE_ENUM_VALUED(GUI_MODEL_TEST_RESULT, FONT_RESULT			,  7, "The GUI model rejected a valid font index.");
GDEFINE_ENUM_VALUED(GUI_MODEL_TEST_RESULT, FONT_VALUE			,  8, "The GUI model stored the wrong font index.");
GDEFINE_ENUM_VALUED(GUI_MODEL_TEST_RESULT, FONT_INVALIDATION	,  9, "Changing a control font did not invalidate its metrics.");
GDEFINE_ENUM_VALUED(GUI_MODEL_TEST_RESULT, HIDDEN_INHERITANCE	, 10, "A GUI child did not inherit its parent's hidden state.");
GDEFINE_ENUM_VALUED(GUI_MODEL_TEST_RESULT, DISABLED_INHERITANCE	, 11, "A GUI child did not inherit its parent's disabled state.");
GDEFINE_ENUM_VALUED(GUI_MODEL_TEST_RESULT, METRICS_INVALIDATE	, 12, "GUI metric invalidation did not reach the complete hierarchy.");
GDEFINE_ENUM_VALUED(GUI_MODEL_TEST_RESULT, PROCESSABLE_RESULT	, 13, "Collecting processable GUI controls reported failure.");
GDEFINE_ENUM_VALUED(GUI_MODEL_TEST_RESULT, PROCESSABLE_COUNT	, 14, "The GUI model returned the wrong processable-control count.");
GDEFINE_ENUM_VALUED(GUI_MODEL_TEST_RESULT, PROCESSABLE_ID		, 15, "The GUI model returned the wrong processable control.");
GDEFINE_ENUM_VALUED(GUI_MODEL_TEST_RESULT, DELETE_RESULT			, 16, "The GUI model could not delete a valid control.");
GDEFINE_ENUM_VALUED(GUI_MODEL_TEST_RESULT, SELECT_RESULT			, 17, "Selecting a valid GUI control reported failure.");
GDEFINE_ENUM_VALUED(GUI_MODEL_TEST_RESULT, SELECTED_VALUE		, 18, "The GUI model retained the wrong selected control.");
GDEFINE_ENUM_VALUED(GUI_MODEL_TEST_RESULT, UNUSED_STATE			, 19, "GUI selection modified an unused control slot.");
GDEFINE_ENUM_VALUED(GUI_MODEL_TEST_RESULT, PROCESS_RESULT		, 20, "GUI action processing returned the wrong callback result.");
GDEFINE_ENUM_VALUED(GUI_MODEL_TEST_RESULT, PROCESS_COUNT			, 21, "GUI action processing invoked its callback the wrong number of times.");
GDEFINE_ENUM_VALUED(GUI_MODEL_TEST_RESULT, PROCESS_ID			, 22, "GUI action processing invoked its callback for the wrong control.");

::llc::err_t testGUIModel(ATestError & errors) {
	::llc::SGUI gui = {};
	cnst ::llc::cid_t idRoot = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_MODEL_TEST_RESULT_CREATE_RESULT, idRoot < 0, "Root creation result:%i.", idRoot);
	cnst ::llc::cid_t idChild = ::llc::controlCreateChild(gui, idRoot);
	LLC_TEST_REQUIREF(errors, GUI_MODEL_TEST_RESULT_CREATE_CHILD_RESULT, idChild < 0, "Child creation result:%i.", idChild);
	LLC_TEST_CHECKF(errors, GUI_MODEL_TEST_RESULT_CHILD_PARENT, gui.Controls.Entities[idChild].Parent != idRoot, "Child parent:%i, expected:%i.", gui.Controls.Entities[idChild].Parent, idRoot);

	gui.Controls.SetUpdated(idChild, true);
	cnst ::llc::err_t textResult = ::llc::controlTextSet(gui, idChild, "Child label");
	LLC_TEST_REQUIREF(errors, GUI_MODEL_TEST_RESULT_TEXT_RESULT, ::llc::failed(textResult), "Text result:%i.", textResult);
	LLC_TEST_CHECK(errors, GUI_MODEL_TEST_RESULT_TEXT_VALUE, gui.Controls.Text[idChild].Text != LLC_CXS("Child label"));
	LLC_TEST_CHECK(errors, GUI_MODEL_TEST_RESULT_TEXT_INVALIDATION, gui.Controls.IsUpdated(idChild));
	gui.Controls.SetUpdated(idChild, true);
	LLC_TEST_REQUIREF(errors, GUI_MODEL_TEST_RESULT_TEXT_RESULT, 0 > ::llc::controlTextSet(gui, idChild, "Child label"), "%s", "Repeating control text failed.");
	LLC_TEST_CHECK(errors, GUI_MODEL_TEST_RESULT_TEXT_REPEAT, false == gui.Controls.IsUpdated(idChild));

	cnst ::llc::err_t fontResult = ::llc::controlFontSet(gui, idChild, 3);
	LLC_TEST_REQUIREF(errors, GUI_MODEL_TEST_RESULT_FONT_RESULT, ::llc::failed(fontResult), "Font result:%i.", fontResult);
	LLC_TEST_CHECKF(errors, GUI_MODEL_TEST_RESULT_FONT_VALUE, gui.Controls.Text[idChild].FontSelected != 3, "Font index:%i, expected:3.", gui.Controls.Text[idChild].FontSelected);
	LLC_TEST_CHECK(errors, GUI_MODEL_TEST_RESULT_FONT_INVALIDATION, gui.Controls.IsUpdated(idChild));

	gui.Controls.SetHidden(idRoot, true);
	LLC_TEST_CHECK(errors, GUI_MODEL_TEST_RESULT_HIDDEN_INHERITANCE, 0 == ::llc::controlHidden(gui, idChild));
	gui.Controls.SetHidden(idRoot, false);
	LLC_TEST_CHECK(errors, GUI_MODEL_TEST_RESULT_HIDDEN_INHERITANCE, 0 != ::llc::controlHidden(gui, idChild));
	gui.Controls.SetDisabled(idRoot, true);
	LLC_TEST_CHECK(errors, GUI_MODEL_TEST_RESULT_DISABLED_INHERITANCE, 0 == ::llc::controlDisabled(gui, idChild));
	gui.Controls.SetDisabled(idRoot, false);
	LLC_TEST_CHECK(errors, GUI_MODEL_TEST_RESULT_DISABLED_INHERITANCE, 0 != ::llc::controlDisabled(gui, idChild));

	gui.Controls.SetUpdated(idRoot, true);
	gui.Controls.SetUpdated(idChild, true);
	LLC_TEST_REQUIREF(errors, GUI_MODEL_TEST_RESULT_METRICS_INVALIDATE, 0 > ::llc::controlMetricsInvalidate(gui, idRoot), "%s", "Hierarchy invalidation failed.");
	LLC_TEST_CHECK(errors, GUI_MODEL_TEST_RESULT_METRICS_INVALIDATE, gui.Controls.IsUpdated(idRoot));
	LLC_TEST_CHECK(errors, GUI_MODEL_TEST_RESULT_METRICS_INVALIDATE, gui.Controls.IsUpdated(idChild));

	cnst ::llc::cid_t idOther = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_MODEL_TEST_RESULT_CREATE_RESULT, idOther < 0, "Independent creation result:%i.", idOther);
	gui.Controls.SetHidden(idRoot, true);
	::llc::acid processable = {};
	cnst ::llc::err_t processableResult = ::llc::guiGetProcessableControls(gui, processable);
	LLC_TEST_REQUIREF(errors, GUI_MODEL_TEST_RESULT_PROCESSABLE_RESULT, ::llc::failed(processableResult), "Processable result:%i.", processableResult);
	LLC_TEST_CHECKF(errors, GUI_MODEL_TEST_RESULT_PROCESSABLE_COUNT, processable.size() != 1, "Processable count:%u, expected:1.", processable.size());
	LLC_TEST_REQUIREF(errors, GUI_MODEL_TEST_RESULT_PROCESSABLE_ID, 0 == processable.size(), "%s", "Processable control missing.");
	LLC_TEST_CHECKF(errors, GUI_MODEL_TEST_RESULT_PROCESSABLE_ID, processable[0] != idOther, "Processable control:%i, expected:%i.", processable[0], idOther);
	gui.Controls.SetHidden(idRoot, false);

	LLC_TEST_REQUIREF(errors, GUI_MODEL_TEST_RESULT_DELETE_RESULT, 0 > ::llc::controlDelete(gui, idOther), "%s", "Independent control deletion failed.");
	cnst ::llc::err_t selectResult = ::llc::guiSelect(gui, idChild);
	LLC_TEST_REQUIREF(errors, GUI_MODEL_TEST_RESULT_SELECT_RESULT, ::llc::failed(selectResult), "Selection result:%i.", selectResult);
	LLC_TEST_CHECK(errors, GUI_MODEL_TEST_RESULT_SELECTED_VALUE, gui.Controls.IsSelected(idRoot));
	LLC_TEST_CHECK(errors, GUI_MODEL_TEST_RESULT_SELECTED_VALUE, false == gui.Controls.IsSelected(idChild));
	LLC_TEST_CHECK(errors, GUI_MODEL_TEST_RESULT_UNUSED_STATE, false == gui.Controls.IsUnused(idOther));

	cnst ::llc::cid_t idAction = ::llc::controlCreate(gui);
	LLC_TEST_REQUIREF(errors, GUI_MODEL_TEST_RESULT_CREATE_RESULT, idAction < 0, "Action control creation result:%i.", idAction);
	gui.Controls.SetAction(idAction, true);
	::llc::u2_t processCount = 0;
	::llc::cid_t processedId = ::llc::CID_INVALID;
	cnst ::llc::err_t expectedProcessResult = 17;
	cnst ::llc::err_t processResult = ::llc::guiProcessControls(gui, [&processCount, &processedId, expectedProcessResult](::llc::cid_t iControl) { ++processCount; processedId = iControl; rtrn expectedProcessResult; });
	LLC_TEST_CHECKF(errors, GUI_MODEL_TEST_RESULT_PROCESS_RESULT, processResult != expectedProcessResult, "Process result:%i, expected:%i.", processResult, expectedProcessResult);
	LLC_TEST_CHECKF(errors, GUI_MODEL_TEST_RESULT_PROCESS_COUNT, processCount != 1, "Callback count:%u, expected:1.", processCount);
	LLC_TEST_CHECKF(errors, GUI_MODEL_TEST_RESULT_PROCESS_ID, processedId != idAction, "Processed control:%i, expected:%i.", processedId, idAction);
	rtrn 0;
}
