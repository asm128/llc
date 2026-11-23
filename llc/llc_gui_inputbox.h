#include "llc_gui.h"
#include "llc_virtual_keyboard.h"
#include "llc_system_key.h"
#include "llc_event_input.h"

#include <limits.h>

#ifndef LLC_GUI_INPUTBOX_H_23627
#define LLC_GUI_INPUTBOX_H_23627

namespace llc
{
	::llc::err_t			virtualKeyboardSetup	(::llc::SGUI & gui, ::llc::SVirtualKeyboard & vk, uint8_t rowWidth, const ::llc::vcu1_t & keys);
	::llc::err_t			virtualKeyboardSetup437	(::llc::SGUI & gui, ::llc::SVirtualKeyboard & vk);

	struct SInputBox {
		cid_t					IdRoot				= {};
		cid_t					IdText				= {};
		::llc::SVirtualKeyboard	VirtualKeyboard		= {};
		::llc::asc_t			Text				= {};
		uint32_t				MaxLength			= 16;
		bool					Editing				= false;


		inline	::llc::err_t	SetText				(::llc::SGUI & gui, ::llc::vcst_t text) {
			Text = text;
			return ::llc::controlTextSet(gui, IdText, {Text.begin(), Text.size()});
		}
		::llc::err_t			Edit				(::llc::SGUI & gui, bool editing) {
			Editing					= editing;
			gui.Controls.SetHidden(IdRoot, false == Editing);
			return one_if(Editing);
		}
#ifndef LLC_ATMEL		
		// Returns INT_MAX if the (real or virtual) enter key was pressed.
		::llc::err_t			Update				(::llc::SGUI & gui, const ::llc::SVirtualKeyboard & virtualKeyboard, ::llc::view<const ::llc::pobj<::llc::SEventSystem>> frameEvents, ::llc::vcid processableControls) { 
			if(false == Editing)
				return 0;

			cid_t						handledControl;
			::llc::apod<::llc::SVirtualKeyboardEvent>	vkEvents;
			llc_necs(handledControl = (cid_t)::llc::guiProcessControls(gui, processableControls, [&](::llc::cid_t iControl) {
				if(::llc::virtualKeyboardHandleEvent(virtualKeyboard, iControl, vkEvents))
					return ::llc::err_t(handledControl = iControl);

				return 0;
			}));

			::llc::apobj<::llc::SEventSystem>	sysEvents			= frameEvents;
			for(uint32_t iEvent = 0; iEvent < vkEvents.size(); ++iEvent) {
				switch(vkEvents[iEvent].Type) {
				case ::llc::VK_EVENT_RELEASE:
					if(Text.size() < MaxLength) {
						Text.push_back((sc_t)vkEvents[iEvent].ScanCode);
						::llc::controlTextSet(gui, IdText, {Text.begin(), Text.size()});
					}
					break;
				case ::llc::VK_EVENT_EDIT:
						 if(vkEvents[iEvent].ScanCode == llc::VK_SCANCODE_Backspace	) { if_fail_e(::llc::eventEnqueueKeyboardDown(sysEvents, llc::SYSTEM_KEY_Backspace)); } 
					else if(vkEvents[iEvent].ScanCode == llc::VK_SCANCODE_Enter		) { if_fail_e(::llc::eventEnqueueKeyboardDown(sysEvents, llc::SYSTEM_KEY_Enter	)); } 
					else if(vkEvents[iEvent].ScanCode == llc::VK_SCANCODE_Escape	) { if_fail_e(::llc::eventEnqueueKeyboardDown(sysEvents, llc::SYSTEM_KEY_Escape	)); } 
					else if(vkEvents[iEvent].ScanCode == llc::VK_SCANCODE_Clear		) { if_fail_e(::llc::eventEnqueueKeyboardDown(sysEvents, llc::SYSTEM_KEY_Clear	)); } 
				}
			}
			;
			bool						enterKeyPressed		= false;
			sysEvents.for_each([this, &gui, &enterKeyPressed](const ::llc::pobj<::llc::SEventSystem> & ev) mutable -> ::llc::s2_t { 
				switch(ev->Type) { 
				case ::llc::SYSTEM_EVENT_Text: 
					::llc::eventExtractAndHandle<::llc::EVENT_TEXT>(*ev, [this, &gui](auto cev) {
						if(Text.size() < MaxLength)
							if(cev.Data[0] >= 0x20 && cev.Data[0] <= 0x7F) {
								Text.push_back(cev.Data[0]);
								::llc::controlTextSet(gui, IdText, {Text.begin(), Text.size()});
							}
						return 0;
					});
					break; 
				case ::llc::SYSTEM_EVENT_Keyboard: 
					::llc::eventExtractAndHandle<::llc::EVENT_KEYBOARD>(*ev, [this, &gui, &enterKeyPressed](auto kev) mutable {
						if(kev.Type == ::llc::EVENT_KEYBOARD_Down) {
							switch(kev.Data[0]) {
							case ::llc::SYSTEM_KEY_Escape		: Edit(gui, false);			break;
							case ::llc::SYSTEM_KEY_Enter		: enterKeyPressed = true;	break; 
							case ::llc::SYSTEM_KEY_Backspace	: 
								if(Text.size()) {
									llc_necs(Text.pop_back());
									llc_necs(llc::controlTextSet(gui, IdText, {Text.begin(), Text.size()})); 
								}
								break;
							case ::llc::SYSTEM_KEY_Clear		: 
								Text.clear();				
								llc_necs(llc::controlTextSet(gui, IdText, {Text.begin(), Text.size()})); 
								break;
							}
						}
						return 0; 
					});
					break; 
				}
				return 0;
			});
			return enterKeyPressed ? INT_MAX : handledControl ? handledControl : 0;
		}
#endif // LLC_ATMEL
	};

	::llc::err_t			inputBoxCreate		(::llc::SInputBox & inputBox, ::llc::SGUI & gui, ::llc::cid_t iParent = -1);
} // namespace

#endif // LLC_GUI_INPUTBOX_H_23627
