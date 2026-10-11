#include "llc_system_event.h"

#include "llc_bit.h"

#define llc_gui_printf verbose_printf

#ifndef LLC_GUI_CONTROL_FLAG_H_23627
#define LLC_GUI_CONTROL_FLAG_H_23627

namespace llc
{
#pragma pack(push, 1)
	typedef	s1_t			cid_t;
	stacxpr	cid_t			CID_INVALID		= cid_t(-1);

	typedef	const cid_t		ccid_t;
	typedef	apod<cid_t>		acid;
	typedef	view<cid_t>		vcid;
	typedef	view<ccid_t>	vccid;

	GDEFINE_ENUM_TYPE(GUI_CONTROL_FLAG, u0_t)
	GDEFINE_ENUM_VALUE(GUI_CONTROL_FLAG, Hidden		, 0x01U);	// Prevents from drawing the control and its children
	GDEFINE_ENUM_VALUE(GUI_CONTROL_FLAG, Disabled	, 0x02U);	// Prevents the control from responding to user interaction
	GDEFINE_ENUM_VALUE(GUI_CONTROL_FLAG, Hovered	, 0x04U);	// Modifies behavior depending on whether the mouse pointer top is inside the control area.
	GDEFINE_ENUM_VALUE(GUI_CONTROL_FLAG, Pressed	, 0x08U);	// Modifies behavior depending on whether the control is being pressed by holding the mouse button, a key or a touch event.
	GDEFINE_ENUM_VALUE(GUI_CONTROL_FLAG, Focused	, 0x10U);	// Modified behavior depending on wether the control is selected for receiving user input.
	GDEFINE_ENUM_VALUE(GUI_CONTROL_FLAG, Selected	, 0x20U);	//
	GDEFINE_ENUM_VALUE(GUI_CONTROL_FLAG, Updated	, 0x40U);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_FLAG, Action		, 0x80U);
	GDEFINE_ENUM_VALUE(GUI_CONTROL_FLAG, Unused		, 0xFFU);
	stacxpr	GUI_CONTROL_FLAG	GUI_CONTROL_FLAG_All	= GUI_CONTROL_FLAG_Unused;

	inlcxpr	GUI_CONTROL_FLAG	isHidden		(const GUI_CONTROL_FLAG state, const GUI_CONTROL_FLAG mask)			noexcept	{ return bit_test_masked(state, mask, GUI_CONTROL_FLAG_Hidden	); }
	inlcxpr	GUI_CONTROL_FLAG	isDisabled		(const GUI_CONTROL_FLAG state, const GUI_CONTROL_FLAG mask)			noexcept	{ return bit_test_masked(state, mask, GUI_CONTROL_FLAG_Disabled	); }
	inlcxpr	GUI_CONTROL_FLAG	isHovered		(const GUI_CONTROL_FLAG state, const GUI_CONTROL_FLAG mask)			noexcept	{ return bit_test_masked(state, mask, GUI_CONTROL_FLAG_Hovered	); }
	inlcxpr	GUI_CONTROL_FLAG	isPressed		(const GUI_CONTROL_FLAG state, const GUI_CONTROL_FLAG mask)			noexcept	{ return bit_test_masked(state, mask, GUI_CONTROL_FLAG_Pressed	); }
	inlcxpr	GUI_CONTROL_FLAG	isFocused		(const GUI_CONTROL_FLAG state, const GUI_CONTROL_FLAG mask)			noexcept	{ return bit_test_masked(state, mask, GUI_CONTROL_FLAG_Focused	); }
	inlcxpr	GUI_CONTROL_FLAG	isSelected		(const GUI_CONTROL_FLAG state, const GUI_CONTROL_FLAG mask)			noexcept	{ return bit_test_masked(state, mask, GUI_CONTROL_FLAG_Selected	); }
	inlcxpr	GUI_CONTROL_FLAG	isUpdated		(const GUI_CONTROL_FLAG state, const GUI_CONTROL_FLAG mask)			noexcept	{ return bit_test_masked(state, mask, GUI_CONTROL_FLAG_Updated	); }
	inlcxpr	GUI_CONTROL_FLAG	isAction		(const GUI_CONTROL_FLAG state, const GUI_CONTROL_FLAG mask)			noexcept	{ return bit_test_masked(state, mask, GUI_CONTROL_FLAG_Action	); }
	inlcxpr	GUI_CONTROL_FLAG	isUnused		(const GUI_CONTROL_FLAG state)										noexcept	{ return bit_test(state, GUI_CONTROL_FLAG_Unused); }

	inlcxpr	GUI_CONTROL_FLAG	setFlag			(GUI_CONTROL_FLAG & state, const GUI_CONTROL_FLAG mask, bool value)	noexcept	{ return state = bit_set_masked(state, mask, GUI_CONTROL_FLAG_Hidden	, value); }
	inlcxpr	GUI_CONTROL_FLAG	setHidden		(GUI_CONTROL_FLAG & state, const GUI_CONTROL_FLAG mask, bool value)	noexcept	{ return state = bit_set_masked(state, mask, GUI_CONTROL_FLAG_Hidden	, value); }
	inlcxpr	GUI_CONTROL_FLAG	setDisabled		(GUI_CONTROL_FLAG & state, const GUI_CONTROL_FLAG mask, bool value)	noexcept	{ return state = bit_set_masked(state, mask, GUI_CONTROL_FLAG_Disabled	, value); }
	inlcxpr	GUI_CONTROL_FLAG	setHovered		(GUI_CONTROL_FLAG & state, const GUI_CONTROL_FLAG mask, bool value)	noexcept	{ return state = bit_set_masked(state, mask, GUI_CONTROL_FLAG_Hovered	, value); }
	inlcxpr	GUI_CONTROL_FLAG	setPressed		(GUI_CONTROL_FLAG & state, const GUI_CONTROL_FLAG mask, bool value)	noexcept	{
		return (value == false && bit_test_masked(state, mask, GUI_CONTROL_FLAG_Pressed) && bit_test_masked(state, mask, GUI_CONTROL_FLAG_Hovered))
			? state = bit_set_masked(bit_set_masked(state, mask, GUI_CONTROL_FLAG_Pressed, value), mask, GUI_CONTROL_FLAG_Action, true)
			: state = bit_set_masked(state, mask, GUI_CONTROL_FLAG_Pressed, value)
			;
	}
	inlcxpr	GUI_CONTROL_FLAG	setFocused		(GUI_CONTROL_FLAG & state, const GUI_CONTROL_FLAG mask, bool value)	noexcept	{ return state = bit_set_masked(state, mask, GUI_CONTROL_FLAG_Focused	, value); }
	inlcxpr	GUI_CONTROL_FLAG	setSelected		(GUI_CONTROL_FLAG & state, const GUI_CONTROL_FLAG mask, bool value)	noexcept	{ return state = bit_set_masked(state, mask, GUI_CONTROL_FLAG_Selected	, value); }
	inlcxpr	GUI_CONTROL_FLAG	setUpdated		(GUI_CONTROL_FLAG & state, const GUI_CONTROL_FLAG mask, bool value)	noexcept	{ return state = bit_set_masked(state, mask, GUI_CONTROL_FLAG_Updated	, value); }
	inlcxpr	GUI_CONTROL_FLAG	setAction		(GUI_CONTROL_FLAG & state, const GUI_CONTROL_FLAG mask, bool value)	noexcept	{ return state = bit_set_masked(state, mask, GUI_CONTROL_FLAG_Action	, value); }
	inlcxpr	GUI_CONTROL_FLAG	setUnused		(GUI_CONTROL_FLAG & state, bool value)								noexcept	{ return state = bit_set(state, GUI_CONTROL_FLAG_Unused, value); }


	GDEFINE_ENUM_TYPE (EVENT_GUI_CONTROL, u0_t)
	GDEFINE_ENUM_VALUE(EVENT_GUI_CONTROL, Create		, 0);
	GDEFINE_ENUM_VALUE(EVENT_GUI_CONTROL, Delete		, 1);
	GDEFINE_ENUM_VALUE(EVENT_GUI_CONTROL, StateChange	, 2);
	GDEFINE_ENUM_VALUE(EVENT_GUI_CONTROL, ParentChange	, 3);

	struct SControlEvent {
		cid_t				Id		= CID_INVALID;
	};

	struct SChangeControlState {
		cid_t				Id		= -1;
		GUI_CONTROL_FLAG	Clear	= {};
		GUI_CONTROL_FLAG	Set		= {};
	};

	stainli	err_t			eventEnqueueGuiControlStateChange(apobj<SEventSystem> & queue, cid_t iControl, GUI_CONTROL_FLAG oldState, GUI_CONTROL_FLAG newState) {
		SChangeControlState			stateChange				= {iControl};
		stateChange.Clear		= bit_clear(oldState, newState);
		stateChange.Set			= bit_clear(newState, oldState);
		llc_gui_printf("Control state change: %i", iControl);
		for(u2_t i = 0; i < 8 * sizeof(GUI_CONTROL_FLAG); ++i) {
			const GUI_CONTROL_FLAG bit = bit_make<GUI_CONTROL_FLAG>((u0_t)i);
			if(bit & stateChange.Set  ) llc_gui_printf("State set  : (0x%X) %s", (u2_t)bit, get_value_namep(bit));
			if(bit & stateChange.Clear) llc_gui_printf("State clear: (0x%X) %s", (u2_t)bit, get_value_namep(bit));
		}
		return eventEnqueueChild(queue, SYSTEM_EVENT_GUI, EVENT_GUI_CONTROL_StateChange, stateChange);
	}

	struct SChangeControlParent {
		cid_t				Id			= CID_INVALID;
		cid_t				ParentOld	= CID_INVALID;
		cid_t				ParentNew	= CID_INVALID;
	};

	stainli	err_t			eventEnqueueGuiControlCreate		(apobj<SEventSystem> & queue, cid_t iControl) {
		return eventEnqueueChild(queue, SYSTEM_EVENT_GUI, EVENT_GUI_CONTROL_Create, SControlEvent{iControl});
	}
	stainli	err_t			eventEnqueueGuiControlDelete		(apobj<SEventSystem> & queue, cid_t iControl) {
		return eventEnqueueChild(queue, SYSTEM_EVENT_GUI, EVENT_GUI_CONTROL_Delete, SControlEvent{iControl});
	}
	stainli	err_t			eventEnqueueGuiControlParentChange	(apobj<SEventSystem> & queue, cid_t iControl, cid_t parentOld, cid_t parentNew) {
		return eventEnqueueChild(queue, SYSTEM_EVENT_GUI, EVENT_GUI_CONTROL_ParentChange, SChangeControlParent{iControl, parentOld, parentNew});
	}

	tplt<tpnm _tState>
	using TFuncSetMaskedValue = function<_tState(void)>;

	tplt<tpnm _tState>
	_tState				setValue		(cid_t iControl, apobj<SEventSystem> & eventQueue, _tState & state, TFuncSetMaskedValue<_tState> funcSetValue) {
		const _tState			oldState		= state;
		state				= funcSetValue();
		if(oldState != state)
			es_if_failed(eventEnqueueGuiControlStateChange(eventQueue, iControl, oldState, state));
		return state;
	}

	struct SControlState {
		GUI_CONTROL_FLAG	Flag			= {};
		GUI_CONTROL_FLAG	Mask			= (GUI_CONTROL_FLAG)-1;

		bool				SetHovered		(cid_t iControl, apobj<SEventSystem> & eventQueue, bool value)	noexcept	{ return 0 != (Flag = setValue<GUI_CONTROL_FLAG>(iControl, eventQueue, Flag, [this, &value]() { return setHovered (Flag, Mask, value); })); }
		bool				SetPressed		(cid_t iControl, apobj<SEventSystem> & eventQueue, bool value)	noexcept	{ return 0 != (Flag = setValue<GUI_CONTROL_FLAG>(iControl, eventQueue, Flag, [this, &value]() { return setPressed (Flag, Mask, value); })); }
		bool				SetHidden		(cid_t iControl, apobj<SEventSystem> & eventQueue, bool value)	noexcept	{ return 0 != (Flag = setValue<GUI_CONTROL_FLAG>(iControl, eventQueue, Flag, [this, &value]() { return setHidden  (Flag, Mask, value); })); }
		bool				SetFocused		(cid_t iControl, apobj<SEventSystem> & eventQueue, bool value)	noexcept	{ return 0 != (Flag = setValue<GUI_CONTROL_FLAG>(iControl, eventQueue, Flag, [this, &value]() { return setFocused (Flag, Mask, value); })); }
		bool				SetSelected		(cid_t iControl, apobj<SEventSystem> & eventQueue, bool value)	noexcept	{ return 0 != (Flag = setValue<GUI_CONTROL_FLAG>(iControl, eventQueue, Flag, [this, &value]() { return setSelected(Flag, Mask, value); })); }
		bool				SetDisabled		(cid_t iControl, apobj<SEventSystem> & eventQueue, bool value)	noexcept	{ return 0 != (Flag = setValue<GUI_CONTROL_FLAG>(iControl, eventQueue, Flag, [this, &value]() { return setDisabled(Flag, Mask, value); })); }
		bool				SetUpdated		(cid_t iControl, apobj<SEventSystem> & eventQueue, bool value)	noexcept	{ return 0 != (Flag = setValue<GUI_CONTROL_FLAG>(iControl, eventQueue, Flag, [this, &value]() { return setUpdated (Flag, Mask, value); })); }
		bool				SetAction		(cid_t iControl, apobj<SEventSystem> & eventQueue, bool value)	noexcept	{ return 0 != (Flag = setValue<GUI_CONTROL_FLAG>(iControl, eventQueue, Flag, [this, &value]() { return setAction  (Flag, Mask, value); })); }
		bool				SetUnused		(cid_t iControl, apobj<SEventSystem> & eventQueue, bool value)	noexcept	{ return 0 != (Flag = setValue<GUI_CONTROL_FLAG>(iControl, eventQueue, Flag, [this, &value]() { return setUnused  (Flag, value);       })); }
		inlcxpr	bool		IsHovered		()	const	noexcept	{ return isHovered  (Flag, Mask); }
		inlcxpr	bool		IsPressed		()	const	noexcept	{ return isPressed  (Flag, Mask); }
		inlcxpr	bool		IsHidden		()	const	noexcept	{ return isHidden   (Flag, Mask); }
		inlcxpr	bool		IsFocused		()	const	noexcept	{ return isFocused  (Flag, Mask); }
		inlcxpr	bool		IsSelected		()	const	noexcept	{ return isSelected (Flag, Mask); }
		inlcxpr	bool		IsDisabled		()	const	noexcept	{ return isDisabled (Flag, Mask); }
		inlcxpr	bool		IsUpdated		()	const	noexcept	{ return isUpdated  (Flag, Mask); }
		inlcxpr	bool		IsAction		()	const	noexcept	{ return isAction   (Flag, Mask); }
		inlcxpr	bool		IsUnused		()	const	noexcept	{ return isUnused   (Flag); }

	};
	stacxpr	size_t SIZEOF_SCONTROL	= sizeof(SControlState);
#pragma pack(pop)

} // namespace

#endif // LLC_GUI_CONTROL_FLAG_H_23627


	//GDEFINE_ENUM_TYPE(GUI_CONTROL_FLAG_EX, uint8_t)
	//GDEFINE_ENUM_VALUE(GUI_CONTROL_FLAG_EX, Open		, 0x01U);
	//GDEFINE_ENUM_VALUE(GUI_CONTROL_FLAG_EX, Locked		, 0x02U);
	//GDEFINE_ENUM_VALUE(GUI_CONTROL_FLAG_EX, Expanded	, 0x04U);
	//GDEFINE_ENUM_VALUE(GUI_CONTROL_FLAG_EX, Minimized	, 0x08U);	// Prevents from drawing the control and its children, draws the icon form of the control if required
	//GDEFINE_ENUM_VALUE(GUI_CONTROL_FLAG_EX, Maximized	, 0x10U);
	//GDEFINE_ENUM_VALUE(GUI_CONTROL_FLAG_EX, FullScreen	, 0x20U);
	//GDEFINE_ENUM_VALUE(GUI_CONTROL_FLAG_EX, Active		, 0x40U);
	//inlcxpr	bool	isOpen				(const GUI_CONTROL_FLAG_EX state, const GUI_CONTROL_FLAG_EX mask)			noexcept	{ return bit_test_masked(state, mask, GUI_CONTROL_FLAG_EX_Open			); }
	//inlcxpr	bool	isLocked			(const GUI_CONTROL_FLAG_EX state, const GUI_CONTROL_FLAG_EX mask)			noexcept	{ return bit_test_masked(state, mask, GUI_CONTROL_FLAG_EX_Locked		); }
	//inlcxpr	bool	isExpanded			(const GUI_CONTROL_FLAG_EX state, const GUI_CONTROL_FLAG_EX mask)			noexcept	{ return bit_test_masked(state, mask, GUI_CONTROL_FLAG_EX_Expanded		); }
	//inlcxpr	bool	isMinimized			(const GUI_CONTROL_FLAG_EX state, const GUI_CONTROL_FLAG_EX mask)			noexcept	{ return bit_test_masked(state, mask, GUI_CONTROL_FLAG_EX_Minimized	); }
	//inlcxpr	bool	isMaximized			(const GUI_CONTROL_FLAG_EX state, const GUI_CONTROL_FLAG_EX mask)			noexcept	{ return bit_test_masked(state, mask, GUI_CONTROL_FLAG_EX_Maximized	); }
	//inlcxpr	bool	isFullScreen		(const GUI_CONTROL_FLAG_EX state, const GUI_CONTROL_FLAG_EX mask)			noexcept	{ return bit_test_masked(state, mask, GUI_CONTROL_FLAG_EX_FullScreen	); }
	//inlcxpr	bool	isActive			(const GUI_CONTROL_FLAG_EX state, const GUI_CONTROL_FLAG_EX mask)			noexcept	{ return bit_test_masked(state, mask, GUI_CONTROL_FLAG_EX_Active		); }

	//inlcxpr	GUI_CONTROL_FLAG_EX	setOpen				(GUI_CONTROL_FLAG_EX & state, const GUI_CONTROL_FLAG_EX mask, bool value)	noexcept	{ return state = bit_set_masked(state, mask, GUI_CONTROL_FLAG_EX_Open			, value); }
	//inlcxpr	GUI_CONTROL_FLAG_EX	setLocked			(GUI_CONTROL_FLAG_EX & state, const GUI_CONTROL_FLAG_EX mask, bool value)	noexcept	{ return state = bit_set_masked(state, mask, GUI_CONTROL_FLAG_EX_Locked		, value); }
	//inlcxpr	GUI_CONTROL_FLAG_EX	setExpanded			(GUI_CONTROL_FLAG_EX & state, const GUI_CONTROL_FLAG_EX mask, bool value)	noexcept	{ return state = bit_set_masked(state, mask, GUI_CONTROL_FLAG_EX_Expanded		, value); }
	//inlcxpr	GUI_CONTROL_FLAG_EX	setMinimized		(GUI_CONTROL_FLAG_EX & state, const GUI_CONTROL_FLAG_EX mask, bool value)	noexcept	{ return state = bit_set_masked(state, mask, GUI_CONTROL_FLAG_EX_Minimized		, value); }
	//inlcxpr	GUI_CONTROL_FLAG_EX	setMaximized		(GUI_CONTROL_FLAG_EX & state, const GUI_CONTROL_FLAG_EX mask, bool value)	noexcept	{ return state = bit_set_masked(state, mask, GUI_CONTROL_FLAG_EX_Maximized		, value); }
	//inlcxpr	GUI_CONTROL_FLAG_EX	setFullScreen		(GUI_CONTROL_FLAG_EX & state, const GUI_CONTROL_FLAG_EX mask, bool value)	noexcept	{ return state = bit_set_masked(state, mask, GUI_CONTROL_FLAG_EX_FullScreen	, value); }
	//inlcxpr	GUI_CONTROL_FLAG_EX	setActive			(GUI_CONTROL_FLAG_EX & state, const GUI_CONTROL_FLAG_EX mask, bool value)	noexcept	{ return state = bit_set_masked(state, mask, GUI_CONTROL_FLAG_EX_Active		, value); }
