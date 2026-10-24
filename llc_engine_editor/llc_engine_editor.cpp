#include "llc_app_impl.h"
#include "llc_engine.h"
#include "llc_framework.h"
#include "llc_gui_desktop.h"
#include "llc_gui_viewport.h"
#include "llc_json.h"
#include "llc_system_key.h"

#include <cstdio>
#include <mutex>

enum EDITOR_KIND : ::llc::u0_t { EDITOR_KIND_Scene = 0 };

stxp ::llc::vcst_t WORKBENCH_UI = R"({"File":{"New":{"PNG":"new_png"}}})";

stct SWorkbench {
	::llc::SDesktop			Desktop		= {};
	::llc::SJSONReader		UI			= {};
	::llc::aobj<::llc::vcst_t>	Actions		= {};
};

stct SEditor {
	EDITOR_KIND				Kind		= EDITOR_KIND_Scene;
	::llc::SWindow			Window		= {};
	::llc::SGUI				GUI			= {};
	::llc::SEngine			Engine		= {};
	::llc::cid_t				Controls[5]	= {};
	::llc::cid_t				StatusId	= ::llc::CID_INVALID;
	::llc::u2_t				Selected	= 0;
	bool					Focused		= false;
	bool					Captured	= false;
	::llc::sc_t				Title[48]	= {};
	::llc::u2_t				TitleLength	= 0;
	::llc::sc_t				Status[128]	= {};
};

stct SApplication {
	::llc::SFramework		Framework;
	SWorkbench				Workbench	= {};
	::llc::cid_t			NewScene	= ::llc::CID_INVALID;
	::llc::cid_t			Help		= ::llc::CID_INVALID;
	::llc::aobj<::llc::SViewport>	Viewports	= {};
	::llc::apobj<SEditor>	Editors		= {};
	::std::mutex			Lock;

	SApplication(::llc::SRuntimeValues & runtimeValues) : Framework(runtimeValues, {}) {}
};

sttc ::llc::err_t workbenchBuildMenu(SWorkbench & workbench, ::llc::SGUI & gui, cnst ::llc::SJSONNode & definition, ::llc::cid_t parentList = -1, ::llc::cid_t parentItem = -1) {
	if_true_fe(definition.Token->Type != ::llc::JSON_TYPE_OBJECT || (definition.Children.size() & 1));
	cnst ::llc::err_t iMenu = ::llc::desktopCreateControlList(gui, workbench.Desktop);
	if_fail_fe(iMenu);
	::llc::SControlList & menu = workbench.Desktop.Items.ControlLists[iMenu];
	for(::llc::u2_t iItem = 0; iItem < definition.Children.size(); iItem += 2) {
		cnst ::llc::SJSONNode & key = *definition.Children[iItem];
		cnst ::llc::SJSONNode & value = *definition.Children[iItem + 1];
		if_true_fe(key.Token->Type != ::llc::JSON_TYPE_STRING);
		if_true_fe(value.Token->Type != ::llc::JSON_TYPE_OBJECT && value.Token->Type != ::llc::JSON_TYPE_STRING);
		cnst ::llc::vcst_t label = workbench.UI.View[key.ObjectIndex];
		::llc::s3_t event = 0;
		if(value.Token->Type == ::llc::JSON_TYPE_STRING) {
			if_fail_fe(workbench.Actions.push_back(workbench.UI.View[value.ObjectIndex]));
			event = workbench.Actions.size();
		}
		if_fail_fe(::llc::controlListPush(gui, menu, label, event));
	}
	if(parentList < 0) {
		menu.Orientation = ::llc::DIRECTION_RIGHT;
		gui.Controls.Placement[menu.IdControl].Area.Offset = {20, 18};
		if_fail_fe(::llc::controlListArrange(gui, menu));
	}
	else
		if_fail_fe(::llc::desktopControlListSetParent(gui, workbench.Desktop, (::llc::cid_t)iMenu, parentList, parentItem));
	for(::llc::u2_t iItem = 0; iItem < definition.Children.size(); iItem += 2) {
		cnst ::llc::SJSONNode & value = *definition.Children[iItem + 1];
		if(value.Token->Type == ::llc::JSON_TYPE_OBJECT)
			if_fail_fe(::workbenchBuildMenu(workbench, gui, value, (::llc::cid_t)iMenu, (::llc::cid_t)(iItem / 2)));
	}
	rtrn 0;
}

sttc ::llc::err_t editorStatusUpdate(SEditor & editor) {
	::llc::n3f2_t position = {};
	if(editor.Selected < editor.Engine.Entities.size())
		if_fail_fe(editor.Engine.GetPosition(editor.Selected, position));
	cnst int length = ::snprintf(editor.Status, ::llc::size(editor.Status), "SCENE  |  %u objects  |  selected %u  |  position %.1f, %.1f, %.1f",
		(::llc::u2_t)editor.Engine.Entities.size(), editor.Selected + 1, position.x, position.y, position.z);
	if_true_fe(length < 0 || length >= (::llc::s2_t)::llc::size(editor.Status));
	if_fail_fe(::llc::controlTextSet(editor.GUI, editor.StatusId, {editor.Status, (::llc::u2_t)length}));
	editor.GUI.Controls.SetUpdated(editor.StatusId, false);
	rtrn 0;
}

sttc ::llc::err_t editorControlCreate(SEditor & editor, ::llc::u2_t index, ::llc::vcst_t text) {
	::llc::cid_t & id = editor.Controls[index];
	id = ::llc::controlCreate(editor.GUI);
	if_true_fe(id < 0);
	editor.GUI.Controls.Placement[id].Area = {{(::llc::s1_t)(10 + index * 100), 10}, {92, 32}};
	editor.GUI.Controls.Text[id].Text = text;
	editor.GUI.Controls.Text[id].FontSelected = 2;
	rtrn 0;
}

sttc ::llc::err_t editorInitialize(SEditor & editor, ::llc::u2_t iEditor) {
	if_null_fe(editor.Window.Input.create());
	if_null_fe(editor.Window.BackBuffer.create());
	editor.GUI.SelectedFont = 2;
	cnst int titleLength = ::snprintf(editor.Title, ::llc::size(editor.Title), "Scene %u", iEditor + 1);
	if_true_fe(titleLength < 0 || titleLength >= (::llc::s2_t)::llc::size(editor.Title));
	editor.TitleLength = (::llc::u2_t)titleLength;
	if_fail_fe(::editorControlCreate(editor, 0, "ADD BOX"));
	if_fail_fe(::editorControlCreate(editor, 1, "ADD SPHERE"));
	if_fail_fe(::editorControlCreate(editor, 2, "SELECT NEXT"));
	if_fail_fe(::editorControlCreate(editor, 3, "MOVE -X"));
	if_fail_fe(::editorControlCreate(editor, 4, "MOVE +X"));
	editor.StatusId = ::llc::controlCreate(editor.GUI);
	if_true_fe(editor.StatusId < 0);
	editor.GUI.Controls.Placement[editor.StatusId].Area = {{10, 48}, {505, 28}};
	editor.GUI.Controls.Text[editor.StatusId].FontSelected = 1;
	::llc::err_t created = iEditor & 1 ? editor.Engine.CreateSphere({}, "Sphere") : editor.Engine.CreateBox({}, "Box");
	if_fail_fe(created);
	if_fail_fe(editor.Engine.SetPosition((::llc::u2_t)created, {}));
	if_fail_fe(editor.Engine.Update(0));
	rtrn ::editorStatusUpdate(editor);
}

sttc ::llc::err_t workspaceActivate(SApplication & app, ::llc::u2_t index) {
	rtrn ::llc::viewportActivate(*app.Framework.GUI, app.Workbench.Desktop.IdControl,
		{app.Viewports.begin(), app.Viewports.size()}, (::llc::u0_t)index);
}

sttc ::llc::err_t workspaceCreateScene(SApplication & app) {
	cnst ::llc::u2_t index = app.Editors.size();
	if_true_fe(index >= 255);
	::llc::pobj<SEditor> editor;
	if_null_fe(editor.create());
	if_fail_fe(::editorInitialize(*editor, index));
	if_fail_fe(app.Editors.push_back(editor));
	::llc::SViewport viewport = {};
	::llc::SGUI & gui = *app.Framework.GUI;
	if_fail_fe(::llc::viewportInitialize(gui, viewport));
	if_fail_fe(::llc::controlSetParent(gui, viewport.IdControl, app.Workbench.Desktop.IdControl));
	::llc::SControlPlacement & root = gui.Controls.Placement[viewport.IdControl];
	root.Area = {{(::llc::s1_t)(40 + (index % 4) * 110), (::llc::s1_t)(90 + (index % 4) * 70)}, {620, 470}};
	root.Border = {2, 2, 2, 2};
	cnst ::llc::cid_t title = viewport.IdControls[::llc::VIEWPORT_CONTROL_TITLE];
	cnst ::llc::cid_t client = viewport.IdControls[::llc::VIEWPORT_CONTROL_TARGET];
	gui.Controls.Text[title].Text = {editor->Title, editor->TitleLength};
	gui.Controls.Placement[client].Area.Size = {612, 438};
	if_fail_fe(app.Viewports.push_back(viewport));
	if_fail_fe(::workspaceActivate(app, index));
	rtrn ::llc::controlMetricsInvalidate(gui, viewport.IdControl);
}

sttc ::llc::err_t workspaceSetup(SApplication & app) {
	::llc::SWindow & window = app.Framework.RootWindow;
	window.Size = {1280, 800};
	if_null_fe(window.BackBuffer.create());
	if_fail_fe(window.BackBuffer->resize(window.Size.u2()));
	if_fail_fe(::llc::mainWindowCreate(window, app.Framework.RuntimeValues.PlatformDetail, window.Input));
	::llc::SGUI & gui = *app.Framework.GUI;
	gui.SelectedFont = 2;
	app.Workbench.Desktop.IdControl = ::llc::controlCreate(gui);
	if_true_fe(app.Workbench.Desktop.IdControl < 0);
	gui.Controls.Draw[app.Workbench.Desktop.IdControl].NoBorder = true;
	gui.Controls.Constraints[app.Workbench.Desktop.IdControl].AttachSizeToControl = {app.Workbench.Desktop.IdControl, app.Workbench.Desktop.IdControl};
	if_fail_fe(::llc::jsonParse(app.Workbench.UI, WORKBENCH_UI, ::llc::JSON_PARSE_STRICT));
	if_fail_fe(::workbenchBuildMenu(app.Workbench, gui, *app.Workbench.UI.Tree[0]));
	app.NewScene = ::llc::controlCreateChild(gui, app.Workbench.Desktop.IdControl);
	if_true_fe(app.NewScene < 0);
	gui.Controls.Placement[app.NewScene].Area = {{115, 18}, {145, 40}};
	gui.Controls.Text[app.NewScene].Text = "NEW SCENE";
	gui.Controls.Text[app.NewScene].FontSelected = 2;
	app.Help = ::llc::controlCreateChild(gui, app.Workbench.Desktop.IdControl);
	if_true_fe(app.Help < 0);
	gui.Controls.Placement[app.Help].Area = {{280, 18}, {900, 40}};
	gui.Controls.Text[app.Help].Text = "LLC ENGINE EDITOR  |  Drag titles, resize corners, edit each scene independently";
	gui.Controls.Text[app.Help].FontSelected = 1;
	if_fail_fe(::workspaceCreateScene(app));
	if_fail_fe(::workspaceCreateScene(app));
	rtrn ::llc::guiUpdateMetrics(gui, window.Size, false);
}

sttc ::llc::err_t editorUpdate(SEditor & editor, ::llc::SGUI & parent, ::llc::cid_t client, cnst ::llc::SInput & input) {
	cnst ::llc::rect2s1_t area = parent.Controls.Area[client].Client.Global;
	cnst bool visible = !::llc::controlHidden(parent, client) && area.Size.x > 0 && area.Size.y > 0;
	cnst bool hovered = visible && parent.Controls.IsHovered(client);
	if(input.ButtonDown(0))
		editor.Focused = editor.Captured = hovered;
	if(!visible)
		editor.Focused = editor.Captured = false;
	::llc::SInput & childInput = *editor.Window.Input;
	::llc::inputNextFrame(childInput);
	childInput.MouseCurrent = input.MouseCurrent;
	childInput.MouseCurrent.Position -= area.Offset;
	childInput.KeyboardCurrent = editor.Focused ? input.KeyboardCurrent : ::llc::SInputKeyboard{};
	if(!hovered && !editor.Captured) {
		childInput.MouseCurrent.Position = {-1, -1};
		childInput.MouseCurrent.ButtonState = {};
		childInput.MouseCurrent.Deltas = {};
	}
	if(!input.MouseCurrent.ButtonState[0])
		editor.Captured = false;
	if(!visible)
		rtrn 0;
	cnst ::llc::n2u2_t size = area.Size.u2();
	if(editor.Window.BackBuffer->Color.metrics() != size)
		if_fail_fe(editor.Window.BackBuffer->resize(size));
	editor.Window.Size = size.u1();
	parent.Controls.Images[client].Image = editor.Window.BackBuffer->Color.View;
	parent.Controls.Images[client].ImageAlign = ::llc::ALIGN_TOP_LEFT;
	if_fail_fe(::llc::guiProcessInput(editor.GUI, childInput, editor.Window.EventQueue));
	bool sceneChanged = false;
	bool selectionChanged = false;
	if(editor.GUI.Controls.IsAction(editor.Controls[0]) || editor.GUI.Controls.IsAction(editor.Controls[1])) {
		cnst bool sphere = editor.GUI.Controls.IsAction(editor.Controls[1]);
		cnst ::llc::err_t created = sphere ? editor.Engine.CreateSphere({}, "Sphere") : editor.Engine.CreateBox({}, "Box");
		if_fail_fe(created);
		editor.Selected = (::llc::u2_t)created;
		if_fail_fe(editor.Engine.SetPosition(editor.Selected, {((::llc::f2_t)editor.Selected) * .6f, 0, 0}));
		sceneChanged = selectionChanged = true;
	}
	if(editor.GUI.Controls.IsAction(editor.Controls[2]) && editor.Engine.Entities.size()) {
		editor.Selected = (editor.Selected + 1) % editor.Engine.Entities.size();
		selectionChanged = true;
	}
	if(editor.GUI.Controls.IsAction(editor.Controls[3]) || editor.GUI.Controls.IsAction(editor.Controls[4])) {
		::llc::n3f2_t position = {};
		if_fail_fe(editor.Engine.GetPosition(editor.Selected, position));
		position.x += editor.GUI.Controls.IsAction(editor.Controls[4]) ? .25f : -.25f;
		if_fail_fe(editor.Engine.SetPosition(editor.Selected, position));
		sceneChanged = true;
	}
	if(sceneChanged)
		if_fail_fe(editor.Engine.Update(0));
	if(sceneChanged || selectionChanged)
		if_fail_fe(::editorStatusUpdate(editor));
	rtrn ::llc::guiUpdateMetrics(editor.GUI, editor.Window.Size, false);
}

sttc ::llc::err_t workspaceUpdate(SApplication & app, bool systemRequestedExit) {
	if(systemRequestedExit)
		rtrn ::llc::APPLICATION_STATE_EXIT;
	cnst ::llc::err_t frameworkResult = ::llc::updateFramework(app.Framework);
	if(frameworkResult)
		rtrn frameworkResult;
	::llc::SWindow & window = app.Framework.RootWindow;
	if(window.Closed || window.Input->KeyDown(::llc::SYSTEM_KEY_Escape))
		rtrn ::llc::APPLICATION_STATE_EXIT;
	::llc::SGUI & gui = *app.Framework.GUI;
	cnst ::llc::s3_t menuEvent = ::llc::desktopUpdate(gui, app.Workbench.Desktop, *window.Input);
	if(menuEvent) {
		if_true_fe(menuEvent < 0 || (::llc::u3_t)menuEvent > app.Workbench.Actions.size());
		if(app.Workbench.Actions[(::llc::u2_t)menuEvent - 1] == "new_png")
			if_fail_fe(::llc::controlTextSet(gui, app.Help, "NEW PNG selected | canvas creation is next"));
	}
	if(gui.Controls.IsAction(app.NewScene))
		if_fail_fe(::workspaceCreateScene(app));
	for(::llc::u2_t iEditor = 0; iEditor < app.Editors.size(); ++iEditor) {
		::llc::SViewport & viewport = app.Viewports[iEditor];
		cnst ::llc::cid_t title = viewport.IdControls[::llc::VIEWPORT_CONTROL_TITLE];
		cnst ::llc::cid_t client = viewport.IdControls[::llc::VIEWPORT_CONTROL_TARGET];
		if(::llc::viewportIsPressed(gui, viewport))
			if_fail_fe(::workspaceActivate(app, iEditor));
		if(gui.Controls.IsAction(viewport.IdControls[::llc::VIEWPORT_CONTROL_CLOSE]))
			gui.Controls.SetHidden(viewport.IdControl, true);
		if(gui.Controls.IsPressed(title) && (window.Input->MouseCurrent.Deltas.x || window.Input->MouseCurrent.Deltas.y)) {
			gui.Controls.Placement[viewport.IdControl].Area.Offset += window.Input->MouseCurrent.Deltas.xy().s1();
			if_fail_fe(::llc::controlMetricsInvalidate(gui, viewport.IdControl));
		}
		cnst ::llc::cid_t resize = viewport.IdControls[::llc::VIEWPORT_CONTROL_RESIZE_BOTTOM_RIGHT];
		if(gui.Controls.IsPressed(resize) && (window.Input->MouseCurrent.Deltas.x || window.Input->MouseCurrent.Deltas.y)) {
			::llc::n2s1_t delta = window.Input->MouseCurrent.Deltas.xy().s1();
			gui.Controls.Placement[viewport.IdControl].Area.Size += delta;
			gui.Controls.Placement[client].Area.Size += delta;
			if_fail_fe(::llc::controlMetricsInvalidate(gui, viewport.IdControl));
		}
	}
	if_fail_fe(::llc::guiUpdateMetrics(gui, window.Size, false));
	for(::llc::u2_t iEditor = 0; iEditor < app.Editors.size(); ++iEditor) {
		cnst ::llc::cid_t client = app.Viewports[iEditor].IdControls[::llc::VIEWPORT_CONTROL_TARGET];
		if_fail_fe(::editorUpdate(*app.Editors[iEditor], gui, client, *window.Input));
	}
	if(window.BackBuffer->Color.metrics() != window.Size.u2())
		if_fail_fe(window.BackBuffer->resize(window.Size.u2()));
	::llc::sleep(1);
	rtrn 0;
}

sttc ::llc::err_t workspaceDraw(SApplication & app) {
	::llc::SWindow & window = app.Framework.RootWindow;
	if(window.NoDraw || !window.Size.x || !window.Size.y)
		rtrn 0;
	::llc::SGUI & gui = *app.Framework.GUI;
	if_fail_fe(window.BackBuffer->Color.View.fill({20, 19, 25, 255}));
	for(::llc::u2_t iEditor = 0; iEditor < app.Editors.size(); ++iEditor) {
		cnst ::llc::cid_t client = app.Viewports[iEditor].IdControls[::llc::VIEWPORT_CONTROL_TARGET];
		if(::llc::controlHidden(gui, client))
			continue;
		SEditor & editor = *app.Editors[iEditor];
		::llc::SWindow::TOffscreen & target = *editor.Window.BackBuffer;
		if(!target.Color.metrics().x || !target.Color.metrics().y)
			continue;
		if_fail_fe(target.Color.View.fill({35, 27, 24, 255}));
		if_fail_fe(target.DepthStencil.View.fill((::llc::u2_t)-1));
		if_fail_fe(::llc::drawScene(target.Color.View, target.DepthStencil.View, editor.Engine.Scene->RenderCache,
			*editor.Engine.Scene, {2.8f, 2.2f, -5.0f}, {0, 0, 0}, {0, 1, 0}, {.1f, 100.0f}, {4, 6, -3}));
		if_fail_fe(::llc::guiDraw(editor.GUI, target.Color.View));
	}
	if_fail_fe(::llc::guiDraw(gui, window.BackBuffer->Color.View));
	window.Repaint = true;
	rtrn 0;
}

::llc::err_t setup(SApplication & app) { rtrn ::workspaceSetup(app); }
::llc::err_t cleanup(SApplication & app) { rtrn ::llc::mainWindowDestroy(app.Framework.RootWindow); }
::llc::err_t update(SApplication & app, bool systemRequestedExit) {
	::std::lock_guard lock(app.Lock);
	rtrn ::workspaceUpdate(app, systemRequestedExit);
}
::llc::err_t draw(SApplication & app) {
	::std::lock_guard lock(app.Lock);
	rtrn ::workspaceDraw(app);
}

LLC_DEFINE_APPLICATION_ENTRY_POINT(SApplication, "LLC engine editor")
