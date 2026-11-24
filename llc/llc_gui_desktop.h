#include "llc_gui_control_list.h"
#include "llc_gui_viewport.h"

#ifndef LLC_GUI_DESKTOP_H_23627
#define LLC_GUI_DESKTOP_H_23627

namespace llc
{

	tplt <tpnm _tElement>
	struct SRecyclableElementContainer {
		::llc::view_bit<uint64_t>	Unused;
		::llc::aobj<_tElement>		Elements;
		::llc::au3_t				UnusedData;

		inxp	const _tElement&	operator[]					(::llc::u2_t index)			const				{ return Elements[index]; }
		inxp	_tElement&			operator[]					(::llc::u2_t index)								{ return Elements[index]; }

		inxp	::llc::u2_t			size						()							const	noexcept	{ return Elements.size(); }
		::llc::err_t				push_back					(const _tElement & toPush)						{
			const ::llc::cid_t				elemIndex					= (::llc::cid_t)Elements.push_back(toPush);
			if_true_fef(failed(elemIndex), "%s", "Failed to add element to container! Out of memory?");
			stxp	::llc::u2_t				boolContainerElementSize	= sizeof(uint64_t) * ::llc::BYTE_SIZE;
			if_fail_fe(UnusedData.resize((elemIndex + 1) / boolContainerElementSize + 1));
			Unused						= {UnusedData.begin(), (::llc::u2_t)(elemIndex + 1)};
			Unused[elemIndex]			= false;
			return elemIndex;
		}
	};

#define LLC_DESKTOP_RECYCLABLE(elementType_token)	::llc::SRecyclableElementContainer<::llc::S##elementType_token>		elementType_token##s

	struct SDesktopItems {
		LLC_DESKTOP_RECYCLABLE(Viewport);
		LLC_DESKTOP_RECYCLABLE(PaletteGrid);
		LLC_DESKTOP_RECYCLABLE(ControlList);
	};

	struct SDesktop {
		::llc::cid_t				IdControl					= -1;
		::llc::SDesktopItems		Items						= {};
		::llc::aobj<::llc::acid>	Children					= {}; // Keep track of control list hierarchy.
	};

	::llc::err_t				desktopCreatePaletteGrid	(::llc::SGUI & gui, ::llc::SDesktop & desktop);
	::llc::err_t				desktopCreateControlList	(::llc::SGUI & gui, ::llc::SDesktop & desktop);
	::llc::err_t				desktopCreateViewport		(::llc::SGUI & gui, ::llc::SDesktop & desktop);
	::llc::err_t				desktopDeletePaletteGrid	(::llc::SGUI & gui, ::llc::SDesktop & desktop, ::llc::cid_t iElement);
	::llc::err_t				desktopDeleteControlList	(::llc::SGUI & gui, ::llc::SDesktop & desktop, ::llc::cid_t iElement);
	::llc::err_t				desktopDeleteViewport		(::llc::SGUI & gui, ::llc::SDesktop & desktop, ::llc::cid_t iElement);
																							    
	::llc::err_t				desktopControlListSetParent	(::llc::SGUI & gui, ::llc::SDesktop & desktop, ::llc::cid_t iControlList, ::llc::cid_t iParentControlList, ::llc::cid_t iParentListItem);
	int64_t						desktopUpdate				(::llc::SGUI & gui, ::llc::SDesktop & desktop, ::llc::SInput & input);

} // namespace

#endif // LLC_GUI_DESKTOP_H_23627
