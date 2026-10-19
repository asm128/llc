#include "llc_gui.h"
#include "llc_virtual_keyboard.h"

#ifndef LLC_GUI_INPUTBOX_H_23627
#define LLC_GUI_INPUTBOX_H_23627

namespace llc
{
	::llc::err_t			virtualKeyboardSetup	(::llc::SGUI & gui, ::llc::SVirtualKeyboard & keyboard, ::llc::u0_t rowWidth, ::llc::n2u1_c & keySize, ::llc::vcu1_c & keys);
	::llc::err_t			virtualKeyboardSetup437	(::llc::SGUI & gui, ::llc::SVirtualKeyboard & keyboard);

	stct SInputBox {
		::llc::cid_t			IdRoot			= ::llc::CID_INVALID;
		::llc::cid_t			IdText			= ::llc::CID_INVALID;
		::llc::SVirtualKeyboard	VirtualKeyboard	= {};
		::llc::string			Text			= {};
		::llc::u2_t				MaxLength		= 16;
		bool					Editing			= false;

		inline	::llc::err_t	SetText			(::llc::SGUI & gui, ::llc::vcst_t text)		{ rtrn ::llc::controlTextSet(gui, IdText, (Text = text).cc()); }
		::llc::err_t			Edit			(::llc::SGUI & gui, bool editing) {
			Editing = editing;
			gui.Controls.SetHidden(IdRoot, false == Editing);
			rtrn one_if(Editing);
		}
#ifndef LLC_ATMEL
		// Returns INT_MAX if the (real or virtual) enter key was pressed.
		::llc::err_t			Update			(::llc::SGUI & gui, cnst ::llc::SVirtualKeyboard & virtualKeyboard, ::llc::view<cnst ::llc::pobj<::llc::SEventSystem>> frameEvents, ::llc::vcid processableControls);
#endif
	};

	::llc::err_t			inputBoxCreate	(::llc::SInputBox & inputBox, ::llc::SGUI & gui, ::llc::cid_t iParent = ::llc::CID_INVALID);
} // namespace llc

#endif // LLC_GUI_INPUTBOX_H_23627
