#include "llc_gui.h"

#ifndef LLC_GUI_CHECKBOX_H_23627
#define LLC_GUI_CHECKBOX_H_23627

namespace llc
{
	stct SCheckBox {
		cid_t		IdControl	= CID_INVALID;
		bool		Checked		= false;
		u0_t		MarkLength	= 0;
		astchar<16>	Mark		= {};
	};

	stct SCheckBoxCreate {
		cid_t		Parent	= CID_INVALID;
		rect2s1_t	Area	= {{}, {24, 24}};
		vcst_t		Text	= LLC_CXS("X");
		bool		Checked	= false;
	};

	err_t	checkBoxCreate		(SCheckBox & checkBox, SGUI & gui, cnst SCheckBoxCreate & setup = {});
	err_t	checkBoxSetChecked	(SCheckBox & checkBox, SGUI & gui, bool checked);
	err_t	checkBoxUpdate		(SCheckBox & checkBox, SGUI & gui);
} // namespace llc

#endif // LLC_GUI_CHECKBOX_H_23627
