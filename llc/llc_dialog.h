#include "llc_array_ptr.h"
#include "llc_gui.h"

#ifndef LLC_DIALOG_H_23627
#define LLC_DIALOG_H_23627

namespace llc
{
#pragma pack(push, 1)
	enum DIALOG_CONTROL : int8_t
		{ DIALOG_CONTROL_Label			= 0
		, DIALOG_CONTROL_Button
		, DIALOG_CONTROL_ListBox
		, DIALOG_CONTROL_Slider
		, DIALOG_CONTROL_ScrollBar
		, DIALOG_CONTROL_EditBox
		, DIALOG_CONTROL_VirtualKeyboard
		, DIALOG_CONTROL_COUNT
		};

	struct SDialog;
	struct IDialogControl {
		SDialog					* Dialog			= 0;
		cid_t					IdGUIControl		= (cid_t)-1;
		cid_t					IdParent			= (cid_t)-1;
		uint64_t				DeleteControl		: 1;

		constexpr				IDialogControl		()													: DeleteControl(0) {}
		virtual					~IDialogControl		();

		virtual	::llc::err_t	Resize				(const ::llc::n2<uint16_t> & newSize)				{ (void)newSize; return 0; }
		virtual	::llc::err_t	Update				(::llc::view<const ::llc::pobj<::llc::SEventSystem>> eventsIn)	= 0;
	};
	//tplt<tpnm _TDialogControl>
	//::llc::err_t				dialogCreate										(::llc::SGUI & gui, ::llc::pnco<_TDialogControl>& createdControl)	{
	//	::llc::llc_ref<_TDialogControl>								* newRef							= 0;
	//	if_true_fef(0 == ::llc::ref_create(&newRef));
	//	createdControl.set_ref((::llc::llc_ref<::llc::IDialogControl>*)newRef);
	//	createdControl->Dialog									= this;
	//	createdControl->IdGUIControl							= ::llc::controlCreate(gui);
	//	return 0;
	//}
#pragma pack(pop)

	struct SDialogColors {
		::llc::apod<::llc::asts1_t<::llc::GUI_CONTROL_PALETTE_COUNT>>	PerControlType	= {};
		::llc::asts1_t<::llc::GUI_CONTROL_PALETTE_COUNT>					Control			= {};
		::llc::asts1_t<::llc::GUI_CONTROL_PALETTE_COUNT>					Button			= {};
		::llc::asts1_t<::llc::GUI_CONTROL_PALETTE_COUNT>					CheckBox		= {};
		::llc::asts1_t<::llc::GUI_CONTROL_PALETTE_COUNT>					Viewport		= {};
		::llc::asts1_t<::llc::GUI_CONTROL_PALETTE_COUNT>					ViewportTitle	= {};
	};

	struct SDialog {
		::llc::pobj<::llc::SDialogColors>	Colors					= {};

		cid_t					FocusedCurrent		= (cid_t)-1;
		cid_t					FocusedPrevious		= (cid_t)-1;
		cid_t					SelectedCurrent		= (cid_t)-1;
		cid_t					SelectedPrevious	= (cid_t)-1;
		::llc::n2u1_t			LastSize			= {};
		cid_t					Root				= (cid_t)-1;
		::llc::as2_t			Buttons				= {};

		::llc::GUI_CONTROL_FLAG	DefaultControlMask	= (::llc::GUI_CONTROL_FLAG)-1;
		::llc::SControlDraw		DefaultControlDraw	= {};

		::llc::pobj<::llc::SInput>					Input			= {};
		::llc::pobj<::llc::SGUI>					GUI				= {};
		::llc::aobj<::llc::pnco<IDialogControl>>	Controls		= {};
		::llc::img<::llc::bgra>						ImageCrossBGRA	= {};
		::llc::imgmonou3_t							ImageCross		= {};

								SDialog			(const ::llc::pobj<::llc::SGUI> & gui);
								SDialog			(::llc::pobj<::llc::SGUI> & gui);
								SDialog			();

		inline	::llc::err_t	Focused			()	const	noexcept	{ return FocusedCurrent; }

		::llc::err_t			Focus			(cid_t iControl)		{ 
			if_true_fef(Controls.size() <= (uint32_t)(iControl) && iControl != -1, "Invalid control index: %i.", iControl); 
			FocusedPrevious	= FocusedCurrent; 
			FocusedCurrent	= iControl; 
			return 0; 
		}

		tplt<tpnm _TDialogControl>
		::llc::cid_t			Create			(::llc::pnco<_TDialogControl> & createdControl)	{
			if(0 == GUI.get_ref())
				GUI.create();
			::llc::SGUI				& gui				= *GUI;
			cid_t					index				= (cid_t)-1;
			for(uint32_t iControl = 0; iControl < Controls.size(); ++iControl)	// Look for unused slot
				if(0 == Controls[iControl].get_ref()) {
					index				= (cid_t)iControl;
					break;
				}

			if(-1 == index)
				llc_necs(index = (::llc::cid_t)Controls.push_back({}));

			//::llc::dialogCreate(createdControl);
			//Controls[index]	= createdControl;
			::llc::gref<_TDialogControl>	* newRef	= 0;
			if_true_ve(-1, 0 == ::llc::ref_create(&newRef));
			createdControl.set_ref(newRef);
			Controls[index].set_ref((::llc::gref<::llc::IDialogControl>*)::llc::ref_acquire(newRef));
			Controls[index]->Dialog	= this;
			const uint32_t			idGUIControl		= Controls[index]->IdGUIControl = ::llc::controlCreate(*GUI);
			gui.Controls.Draw[idGUIControl].UseNewPalettes	= true;
			::llc::memcpy_s(gui.Controls.Draw[idGUIControl].Palettes.Storage, Colors->Control.Storage);
			//llc_necall(llc::controlSetParent(gui, idGUIControl, Root), "%s", "Invalid root id!");
			return index;
		}

		tplt<tpnm _TDialogControl>
		::llc::cid_t			Get				(cid_t index, ::llc::pnco<_TDialogControl> & control)	{
			::llc::gref<::llc::IDialogControl>	* sourceRef	= (::llc::gref<::llc::IDialogControl>*)Controls[index].get_ref();
			if(0 == sourceRef || 0 == dynamic_cast<_TDialogControl*>(sourceRef->Instance))
				return -1;
			control.set_ref((::llc::gref<_TDialogControl>*)::llc::ref_acquire(sourceRef));
			return index;
		}

		::llc::err_t			Update			()		{
			if(0 == GUI.get_ref())
				GUI.create();

			::llc::SGUI					& gui			= *GUI;
			for(uint32_t iControl = 0; iControl < Controls.size(); ++iControl) {
				if(0 == Controls[iControl].get_ref() || ::llc::controlInvalid(gui, Controls[iControl]->IdGUIControl)) {
					Controls.remove_unordered(iControl);
					--iControl;
					continue;
				}
			}
			if(LastSize != gui.Controls.TargetMetrics) {
				for(uint32_t iControl = 0; iControl < Controls.size(); ++iControl)
					Controls[iControl]->Resize(gui.Controls.TargetMetrics);
				LastSize				= gui.Controls.TargetMetrics;
			}
			//if(0 != Input)
			//	llc_necall(llc::guiProcessInput(gui, *Input), "%s", "Unknown reason.");
			for(uint32_t iControl = 0; iControl < Controls.size(); ++iControl)
				if(-1 == Controls[iControl]->IdParent)
					Controls[iControl]->Update(gui.Controls.EventQueue);

			return 0;
		}
	};
}

#endif // LLC_DIALOG_H_23627
