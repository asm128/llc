#include "llc_gui.h"

#ifndef LLC_GUI_CONTROL_LIST_H_23627
#define LLC_GUI_CONTROL_LIST_H_23627

namespace llc
{
	::llc::cid_t			guiCreateControlList		(::llc::SGUI & gui, ::llc::DIRECTION direction, ::llc::view<::llc::vcst_c> buttonText, ::llc::cid_t iParent, const ::llc::SControlPlacement & args, ::llc::ALIGN textAlign, ::llc::vcid out_ids);
	stainli	::llc::cid_t	guiCreateControlList		(::llc::SGUI & gui, ::llc::DIRECTION direction, ::llc::view<::llc::vcst_c> buttonText, ::llc::cid_t iParent, const ::llc::SControlPlacement & args, ::llc::ALIGN textAlign, ::llc::acid & out_ids)									{ 
		const ::llc::u2_t offset = out_ids.size();
		llc_necs(out_ids.resize(offset + buttonText.size()));
		return ::llc::guiCreateControlList(gui, direction, buttonText, iParent, args, textAlign, vcid{out_ids.begin() + offset, buttonText.size()}); 
	}
	tplt<tpnm _tUIEnum>
	stainli	::llc::cid_t	guiCreateControlList		(::llc::SGUI & gui, ::llc::DIRECTION direction, ::llc::cid_t iParent, const ::llc::SControlPlacement & args, ::llc::ALIGN textAlign, ::llc::acid & out_ids) {
		return ::llc::guiCreateControlList(gui, direction, ::llc::get_value_labels<_tUIEnum>(), iParent, args, textAlign, out_ids);
	}

	::llc::cid_t			guiCreateControlList		(::llc::SGUI & gui, ::llc::view<::llc::vcst_c> buttonText, ::llc::cid_t iParent, const ::llc::n2u1_t & buttonSize, const ::llc::n2s1_t & offset, ::llc::ALIGN controlAlign, ::llc::ALIGN textAlign, ::llc::vcid out_ids);
	tplt<tpnm _tUIEnum>
	stainli	::llc::cid_t	guiCreateControlList		(::llc::SGUI & gui, ::llc::cid_t iParent, const ::llc::n2u1_t & buttonSize, const ::llc::n2s1_t & offset, ::llc::ALIGN controlAlign, ::llc::ALIGN textAlign, ::llc::vcid out_ids) {
		return ::llc::guiCreateControlList(gui, ::llc::get_value_labels<_tUIEnum>(), iParent, buttonSize, offset, controlAlign, textAlign, out_ids);
	}
	stainli	::llc::cid_t	guiCreateControlList		(::llc::SGUI & gui, ::llc::view<::llc::vcst_c> buttonText, ::llc::cid_t iParent, const ::llc::n2u1_t & buttonSize, const ::llc::n2s1_t & offset, ::llc::ALIGN controlAlign, ::llc::ALIGN textAlign, ::llc::acid & out_ids)	{ 
		const ::llc::u2_t offsetId = out_ids.size();
		out_ids.resize(offsetId + buttonText.size()); 
		return ::llc::guiCreateControlList(gui, buttonText, iParent, buttonSize, offset, controlAlign, textAlign, vcid{out_ids.begin() + offsetId, buttonText.size()}); 
	}

	tplt<tpnm _tUIEnum>
	stainli	::llc::cid_t	guiCreateControlList		(::llc::SGUI & gui, ::llc::cid_t iParent, ::llc::u1_t buttonWidth, int16_t yOffset, ::llc::ALIGN controlAlign, ::llc::ALIGN textAlign, ::llc::acid & out_ids) {
		return ::llc::guiCreateControlList(gui, ::llc::get_value_labels<_tUIEnum>(), iParent, {buttonWidth, 20}, {0, yOffset}, controlAlign, textAlign, out_ids);
	}
	tplt<tpnm _tUIEnum>
	stainli	::llc::cid_t	guiCreateControlList		(::llc::SGUI & gui, ::llc::cid_t iParent, ::llc::u1_t buttonWidth, const ::llc::n2s1_t & offset, ::llc::ALIGN controlAlign, ::llc::ALIGN textAlign, ::llc::acid & out_ids) {
		return ::llc::guiCreateControlList(gui, ::llc::get_value_labels<_tUIEnum>(), iParent, {buttonWidth, 20}, offset, controlAlign, textAlign, out_ids);
	}
	tplt<tpnm _tUIEnum>
	stainli	::llc::cid_t	guiCreateControlList		(::llc::SGUI & gui, ::llc::cid_t iParent, const ::llc::n2u1_t & buttonSize, const ::llc::n2s1_t & offset, ::llc::ALIGN controlAlign, ::llc::ALIGN textAlign, ::llc::acid & out_ids) {
		return ::llc::guiCreateControlList(gui, ::llc::get_value_labels<_tUIEnum>(), iParent, buttonSize, offset, controlAlign, textAlign, out_ids);
	} 

	::llc::cid_t			createScreenLayout			(::llc::SGUI & gui, ::llc::cid_t parent = -1);

	tplt<tpnm _tEnum>
	::llc::error_t			setupControlListWithRoot	(::llc::SGUI & gui, ::llc::acid & outIds, ::llc::cid_t parent, const ::llc::n2u1_t & buttonSize, ::llc::ALIGN controlAlign, ::llc::ALIGN textAlign = ::llc::ALIGN_CENTER)		{
		::llc::cid_t				root						= outIds[outIds.push_back(::llc::createScreenLayout(gui, parent))];
		::llc::view<vcst_c>			labels						= {::llc::get_value_labels<_tEnum>()};
		llc_necs(labels.slice(labels, 1));

		return ::llc::guiCreateControlList(gui, labels, root, buttonSize, {}, controlAlign, textAlign, outIds);
	}

	struct SControlList {
		cid_t					IdControl				= (cid_t)-1;
		::llc::DIRECTION		Orientation				= ::llc::DIRECTION_RIGHT;
		cid_t					IndexParentList			= (cid_t)-1;
		cid_t					IndexParentItem			= (cid_t)-1;
		::llc::acid				IdControls				= {};
		cid_t					IdSelected				= (cid_t)-1;
		::llc::acid				IdMultiselect			= {};
		::llc::as3_t			Events					= {};
	};

	::llc::error_t			controlListInitialize	(::llc::SGUI & gui, ::llc::SControlList & menu);
	::llc::error_t			controlListArrange		(::llc::SGUI & gui, ::llc::SControlList & menu);
	::llc::error_t			controlListPush			(::llc::SGUI & gui, ::llc::SControlList & menu, ::llc::vcst_c & text, int64_t eventCode = -1);

	struct SPaletteGrid {
		cid_t					IdControl				= -1;
		::llc::acid				IdControls				= {};
		::llc::g8bgra			Colors					= {};
	};

	struct SEditBox {
		cid_t					IdControl				= -1;
		::llc::acid				IdControls				= {};
		::llc::g8bgra			Colors					= {};
	};


	::llc::error_t			paletteGridInitialize	(::llc::SGUI & gui, ::llc::SPaletteGrid & palette);
	::llc::error_t			paletteGridColorsSet	(::llc::SGUI & gui, ::llc::SPaletteGrid & palette, const ::llc::g8bgra & colors);
}

#endif // LLC_GUI_CONTROL_LIST_H_23627
