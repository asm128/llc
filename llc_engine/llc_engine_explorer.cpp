#include "llc_engine_explorer.h"
#include "llc_gui_text.h"
#include "llc_gui_control_list.h"

static	::llc::cid_t	menuInit		(::llc::SGUI & gui, ::llc::acid & idButtons) {
	::llc::cid_t				idMenu			= ::llc::controlCreate(gui);
	::llc::SControlPlacement	& controlMenu	= gui.Controls.Placement[idMenu];
	controlMenu.Area		= {0, 0, 0, 20};
	controlMenu.Area.Size.y	= 20;
	controlMenu.Margin		= {};
	controlMenu.Border		= {};

	::llc::SControlConstraints	& controlMenuState	= gui.Controls.Constraints[idMenu];
	controlMenuState.AttachSizeToControl.x	= (::llc::cid_t)0;
	controlMenuState.AttachSizeToControl.y	= -1;

	::llc::SControlPlacement	controlItem		= {};
	controlItem.Margin		= {4, 2, 4, 2};
	controlItem.Border		= {1, 1, 1, 1};
	llc_necs(llc::guiCreateControlList<::llc::EDITOR_APP_MENU>(gui, ::llc::DIRECTION_RIGHT, idMenu, controlItem, ::llc::ALIGN_CENTER_LEFT, idButtons));
	return idMenu;
}

::llc::error_t			llc::editorCreate	(::llc::SEditor & editor) {
	if(!editor.UI.GUI)
		editor.UI.GUI.create();

	::llc::SGUI					& gui				= *editor.UI.GUI;
	llc_necs(llc::inputBoxCreate(editor.UI.InputBox, gui));

	gui.Controls.Placement[editor.UI.InputBox.IdRoot].Align			= ::llc::ALIGN_CENTER_BOTTOM;

	llc_necs(editor.Dialogs.resize(::llc::get_value_count<::llc::EDITOR_APP_DIALOG>()));
	editor.Dialogs[::llc::EDITOR_APP_DIALOG_Menu] = ::menuInit(gui, editor.Menu);
	return 0;
}

::llc::error_t			llc::editorUpdate	(::llc::SEditor & editor, ::llc::SInput & input, ::llc::vpobj<::llc::SEventSystem> queuedEvents) {
	::llc::acid					controlsToProcess	= {};

	::llc::SGUI					& gui				= *editor.UI.GUI;
	::llc::guiProcessInput(gui, input, queuedEvents);

	::llc::guiGetProcessableControls(gui, controlsToProcess);
	if(int32_t result = editor.UI.InputBox.Update(gui, editor.UI.InputBox.VirtualKeyboard, queuedEvents, controlsToProcess)) {
		if(result == INT_MAX) { // enter key pressed
			::llc::vcsc_t					trimmed				= {};
			llc_necs(editor.UI.InputBox.Text.slice(trimmed, 0, ::llc::min(editor.UI.InputBox.Text.size(), 128U)));
			llc_necs(llc::trim(trimmed));
			if(trimmed.size()) {
				::llc::pobj<::llc::asc_t>	textInput;
				textInput.create();
				*textInput	= ::llc::label(trimmed);
				editor.UI.InputHistory.push_back(textInput);
			}
			editor.UI.InputBox.Edit(gui, false);
		}
	}
	else {
		::llc::guiProcessControls(gui, controlsToProcess, [&](::llc::cid_t iControl) {
			//uint32_t					offsetControl				= editor.Dialogs[::llc::EDITOR_APP_DIALOG_Menu];
			editor.UI.InputBox.SetText(gui, gui.Controls.Text[iControl].Text);
			editor.UI.InputBox.Edit(gui, true);
			return 0;
		});
	}
	return 0;
}
