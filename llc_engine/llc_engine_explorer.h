#include "llc_engine_explorer_app_menu.h"
#include "llc_engine_gui.h"
#include "llc_gui_inputbox.h"

#ifndef LLC_ENGINE_EXPLORER_H_23627
#define LLC_ENGINE_EXPLORER_H_23627

namespace llc
{
	struct SEditorRigidBody		{ ::llc::acid IdControl = {}; int32_t IdEditable = -1; };
	struct SEditorImage			{ ::llc::acid IdControl = {}; int32_t IdEditable = -1; };
	struct SEditorMesh			{ ::llc::acid IdControl = {}; int32_t IdEditable = -1; };
	struct SEditorScene			{ ::llc::acid IdControl = {}; int32_t IdEditable = -1; };
	struct SEditorBuffer		{ ::llc::acid IdControl = {}; int32_t IdEditable = -1; };
	struct SEditorSurface		{ ::llc::acid IdControl = {}; int32_t IdEditable = -1; };
	struct SEditorPalette		{ ::llc::acid IdControl = {}; int32_t IdEditable = -1; };
	struct SEditorControl		{ ::llc::acid IdControl = {}; int32_t IdEditable = -1; };
	struct SEditorControlText	{ ::llc::acid IdControl = {}; int32_t IdEditable = -1; };
	struct SEditorLayout		{ ::llc::acid IdControl = {}; int32_t IdEditable = -1; };
	struct SEditorSettings		{ ::llc::acid IdControl = {}; int32_t IdEditable = -1; };


	struct SEditorProject {
		::llc::SGUI					GUI;
		::llc::SEngine				Engine;
	};

	struct SEditorUI {
		::llc::pobj<::llc::SGUI>	GUI					= {};
		::llc::SInputBox			InputBox			= {};
		::llc::apachar				InputHistory;
	};

	struct SEditor {
		::llc::SEditorUI			UI;
		::llc::acid					Dialogs;
		::llc::acid					Menu;
	};

	//struct SEditor : public SEditorApplication {
	//	::llc::acid			Hover;
	//	::llc::acid			Selection;
	//	::llc::acid			Expand;

	//	SEditorRigidBody	EditorRigidBody		= {};

	//	SEditorImage		EditorImage			= {};
	//	SEditorMesh			EditorMesh			= {};
	//	SEditorScene		EditorScene			= {};
	//	SEditorBuffer		EditorBuffer		= {};
	//	SEditorSurface		EditorSurface		= {};

	//	SEditorPalette		EditorPalette		= {};
	//	SEditorLayout		EditorLayout		= {};
	//	SEditorControl		EditorControl		= {};
	//	SEditorControlText	EditorControlText	= {};
	//};

	::llc::error_t				editorCreate	(::llc::SEditor & editor);
	::llc::error_t				editorUpdate	(::llc::SEditor & editor, ::llc::SInput & input, ::llc::vpobj<::llc::SEventSystem> systemEvents);
	//::llc::error_t			editorCreate	(::llc::SGUI & gui, ::llc::SLayoutEditor & editor);
	//::llc::error_t			editorCreate	(::llc::SGUI & gui, ::llc::SPaletteEditor & editor);
} // namespace

#endif // LLC_ENGINE_EXPLORER_H_23627
