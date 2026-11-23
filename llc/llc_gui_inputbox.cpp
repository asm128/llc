#include "llc_gui_inputbox.h"
#include "llc_gui_control_list.h"
#include "llc_label.h"

#ifndef LLC_WINDOWS
#	include "llc_string.h"
#endif

::llc::err_t			llc::virtualKeyboardSetup437(::llc::SGUI & gui, ::llc::SVirtualKeyboard & vk)	{
	::llc::au1_t					keys;
	for(uint16_t i = 1; i < 255; ++i) {
		keys.push_back(i);
	}
	keys.push_back('!');
	keys.push_back('?');
	llc_necs(llc::virtualKeyboardSetup(gui, vk, 32, ::llc::vcu1_t{keys}));
	gui.Controls.SetHidden(vk.IdRoot, true);
	return vk.IdRoot;
}

::llc::err_t			llc::virtualKeyboardSetup	(::llc::SGUI & gui, ::llc::SVirtualKeyboard & vk, uint8_t rowWidth, ::llc::vcu1_c & keys) {
	vk.Keys					= keys;

	vk.IdRoot				= ::llc::controlCreate(gui);
	const ::llc::n2u0_t			charSize					= gui.Fonts[gui.SelectedFont]->CharSize;
	const ::llc::n2s1_t			sizeKey						= (charSize + ::llc::n2u0_t{6, 6}) .s1();
	stxp	uint16_t			SIZE_BUTTON					= 88;
	const ::llc::n2s1_t			sizeKeypad					= {int16_t(sizeKey.x * rowWidth + 4 + SIZE_BUTTON), int16_t(sizeKey.y * (keys.size() / rowWidth) + 4)};

	{
		::llc::SControlPlacement	& controlRoot				= gui.Controls.Placement[vk.IdRoot];
		controlRoot.Area.Size	= sizeKeypad;
		controlRoot.Align		= ::llc::ALIGN_CENTER_BOTTOM;
	}
	for(uint32_t iKey = 0; iKey < vk.Keys.size(); ++iKey) {
		sc_t						text[8]		= {};
		sprintf_s(text, "%c", vk.Keys[iKey]);

		::llc::cid_t				idKey			= ::llc::controlCreate(gui);
		::llc::SControlPlacement	& controlKey	= gui.Controls.Placement[idKey];
		controlKey.Area.Size	= sizeKey;
		controlKey.Area.Offset.x= int16_t((iKey % rowWidth) * sizeKey.x);
		controlKey.Area.Offset.y= int16_t((iKey / rowWidth) * sizeKey.y);
		controlKey.Border		= {1, 1, 1, 1};
		//controlKey.ColorTheme			= ::llc::ASCII_COLOR_DARKGREY * 16 + 8;
		gui.Controls.Text[idKey].Align	= ::llc::ALIGN_CENTER;

		llc_necs(llc::controlSetParent(gui, idKey, vk.IdRoot));
		llc_necs(llc::controlTextSet(gui, idKey, ::llc::label(text)));
	}

	llc_necs(llc::guiCreateControlList<::llc::VK_SCANCODE>(gui, vk.IdRoot, SIZE_BUTTON, 0, ::llc::ALIGN_TOP_RIGHT, ::llc::ALIGN_CENTER, vk.IdKeys));
	return 0;
}

::llc::err_t			llc::inputBoxCreate		(::llc::SInputBox & inputBox, ::llc::SGUI & gui, ::llc::cid_t iParent)	{
	llc_necs(inputBox.IdRoot = ::llc::controlCreate(gui));
	gui.Controls.States		[inputBox.IdRoot].Mask		&= ~::llc::GUI_CONTROL_FLAG_Hovered;
	gui.Controls.Placement	[inputBox.IdRoot].Border	= {};
	gui.Controls.Placement	[inputBox.IdRoot].Margin	= {};
	//gui.Controls.Draw		[inputBox.IdRoot].NoBorder	= true;
	gui.Controls.Draw		[inputBox.IdRoot].NoClient	= true;

	llc_necs(inputBox.IdText = ::llc::controlCreate(gui));
	gui.Controls.Placement	[inputBox.IdText].Border	= {2, 2, 2, 2};
	gui.Controls.Placement	[inputBox.IdText].Margin	= {1, 1, 1, 1};
	gui.Controls.Placement	[inputBox.IdText].Area.Size	= {0, 22};
	gui.Controls.Text		[inputBox.IdText].Align		= ::llc::ALIGN_CENTER;

	llc_necs(llc::virtualKeyboardSetup437(gui, inputBox.VirtualKeyboard));
	gui.Controls.SetHidden(inputBox.VirtualKeyboard.IdRoot, false);
	gui.Controls.Placement	[inputBox.VirtualKeyboard.IdRoot].Align	= gui.Controls.Placement[inputBox.IdText].Align;
	gui.Controls.Placement	[inputBox.VirtualKeyboard.IdRoot].Area.Offset.y
		= gui.Controls.Placement	[inputBox.IdText].Area.Offset.y
		+ gui.Controls.Placement	[inputBox.IdText].Area.Size.y
		;
	gui.Controls.Placement[inputBox.IdText].Area.Size.x	= gui.Controls.Placement[inputBox.VirtualKeyboard.IdRoot].Area.Size.x;

	gui.Controls.Placement[inputBox.IdRoot].Area.Size.x	= ::llc::max(gui.Controls.Placement[inputBox.IdText].Area.Size.x, gui.Controls.Placement[inputBox.VirtualKeyboard.IdRoot].Area.Size.x);
	gui.Controls.Placement[inputBox.IdRoot].Area.Size.y	+= gui.Controls.Placement[inputBox.VirtualKeyboard.IdRoot].Area.Size.y;

	llc_necs(llc::controlSetParent(gui, inputBox.IdText					, inputBox.IdRoot));
	llc_necs(llc::controlSetParent(gui, inputBox.VirtualKeyboard.IdRoot	, inputBox.IdRoot));
	if(iParent >= 0)
		llc_necall(llc::controlSetParent(gui, inputBox.IdRoot, iParent), "iParent: %i", iParent);

	return inputBox.IdRoot;
}
