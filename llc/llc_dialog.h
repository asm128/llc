#include "llc_gui.h"

#ifndef LLC_DIALOG_H_23627
#define LLC_DIALOG_H_23627

namespace llc
{
#pragma pack(push, 1)
	struct SControlReference {
		cid_t	Id			= CID_INVALID;
		u2_t	Generation	= 0;

		inlcxpr bool	operator==		(cnst SControlReference & other) cnst nxpt { rtrn Id == other.Id && Generation == other.Generation; }
		inlcxpr bool	operator!=		(cnst SControlReference & other) cnst nxpt { rtrn false == operator==(other); }
	};

	struct SDialog {
		SControlReference	Root				= {};
		SControlReference	FocusedCurrent		= {};
		SControlReference	FocusedPrevious	= {};
		SControlReference	SelectedCurrent	= {};
		SControlReference	SelectedPrevious	= {};
	};

	struct SDialogCreate {
		cid_t	Parent	= CID_INVALID;
		n2s1_t	Size	= {};
		bool			Fill	= true;
	};
#pragma pack(pop)

	SControlReference	controlReference		(cnst SGUI & gui, cid_t iControl) nxpt;
	bool				controlReferenceValid	(cnst SGUI & gui, cnst SControlReference & reference) nxpt;
	bool				dialogControlBelongs	(cnst SDialog & dialog, cnst SGUI & gui, cid_t iControl) nxpt;

	err_t			dialogCreate			(SDialog & dialog, SGUI & gui, cnst SDialogCreate & setup = {});
	err_t			dialogDelete			(SDialog & dialog, SGUI & gui);
	cid_t			dialogControlCreate	(SDialog & dialog, SGUI & gui, cid_t iParent = CID_INVALID);
	err_t			dialogFocus			(SDialog & dialog, SGUI & gui, cid_t iControl);
	err_t			dialogSelect			(SDialog & dialog, SGUI & gui, cid_t iControl);
	err_t			dialogUpdate			(SDialog & dialog, cnst SGUI & gui, view<cnst pobj<SEventSystem>> events);
	err_t			dialogUpdate			(SDialog & dialog, cnst SGUI & gui);
} // namespace llc

#endif // LLC_DIALOG_H_23627
