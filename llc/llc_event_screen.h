#include "llc_system_event.h"
#include "llc_n2.h"

#ifndef LLC_EVENT_SCREEN_H_23627
#define LLC_EVENT_SCREEN_H_23627

namespace llc
{
	GDEFINE_ENUM_TYPE (EVENT_SCREEN, u0_t);
	GDEFINE_ENUM_VALUE(EVENT_SCREEN, Create			,  0);
	GDEFINE_ENUM_VALUE(EVENT_SCREEN, Destroy		,  1);
	GDEFINE_ENUM_VALUE(EVENT_SCREEN, Close			,  2);
	GDEFINE_ENUM_VALUE(EVENT_SCREEN, Move			,  3);
	GDEFINE_ENUM_VALUE(EVENT_SCREEN, Activate		,  4);
	GDEFINE_ENUM_VALUE(EVENT_SCREEN, Deactivate		,  5);
	GDEFINE_ENUM_VALUE(EVENT_SCREEN, Resize			,  6);
	GDEFINE_ENUM_VALUE(EVENT_SCREEN, Redraw			,  7);
	GDEFINE_ENUM_VALUE(EVENT_SCREEN, Show			,  8);
	GDEFINE_ENUM_VALUE(EVENT_SCREEN, Hide			,  9);
	GDEFINE_ENUM_VALUE(EVENT_SCREEN, Constraints	, 10);

	tydf ::llc::SEvent            <EVENT_SCREEN>	SScreenEvent;
	tydf ::llc::SEventView        <EVENT_SCREEN>	SScreenEventView;
	tydf ::llc::FEventHandler     <EVENT_SCREEN>	FScreenEvent;
	tydf ::llc::FEventHandlerConst<EVENT_SCREEN>	FScreenEventConst;

	stct SEventArgsScreenCreate {
		::llc::n2u1_t		Size;
#ifdef LLC_WINDOWS
		CREATESTRUCT		PlatformDetail;
#endif
	};

	stin	::llc::err_t	eventEnqueueScreenDestroy		(::llc::apobj<::llc::SEventSystem> & queue)						{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Screen, ::llc::EVENT_SCREEN_Destroy, {}); }
	stin	::llc::err_t	eventEnqueueScreenClose			(::llc::apobj<::llc::SEventSystem> & queue)						{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Screen, ::llc::EVENT_SCREEN_Close  , {}); }
	stin	::llc::err_t	eventEnqueueScreenCreate		(::llc::apobj<::llc::SEventSystem> & queue, const SEventArgsScreenCreate & key)	{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Screen, ::llc::EVENT_SCREEN_Create, key); }
	stin	::llc::err_t	eventEnqueueScreenActivate		(::llc::apobj<::llc::SEventSystem> & queue, u0_t key)		{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Screen, ::llc::EVENT_SCREEN_Activate	, key); }
	stin	::llc::err_t	eventEnqueueScreenDeactivate	(::llc::apobj<::llc::SEventSystem> & queue, u0_t key)		{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Screen, ::llc::EVENT_SCREEN_Deactivate	, key); }
	stin	::llc::err_t	eventEnqueueScreenRedraw		(::llc::apobj<::llc::SEventSystem> & queue, u0_t key)		{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Screen, ::llc::EVENT_SCREEN_Redraw		, key); }
	stin	::llc::err_t	eventEnqueueScreenShow			(::llc::apobj<::llc::SEventSystem> & queue, u0_t key)		{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Screen, ::llc::EVENT_SCREEN_Show		, key); }
	stin	::llc::err_t	eventEnqueueScreenHide			(::llc::apobj<::llc::SEventSystem> & queue, u0_t key)		{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Screen, ::llc::EVENT_SCREEN_Hide		, key); }
	stin	::llc::err_t	eventEnqueueScreenConstraints	(::llc::apobj<::llc::SEventSystem> & queue, u0_t key)		{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Screen, ::llc::EVENT_SCREEN_Constraints, key); }
	stin	::llc::err_t	eventEnqueueScreenMove			(::llc::apobj<::llc::SEventSystem> & queue, ::llc::n2u1_t coord)	{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Screen, ::llc::EVENT_SCREEN_Move		, coord); }
	stin	::llc::err_t	eventEnqueueScreenResize		(::llc::apobj<::llc::SEventSystem> & queue, ::llc::n2u1_t size)	{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Screen, ::llc::EVENT_SCREEN_Resize		, size); }
} // namespace 

#endif // LLC_EVENT_SCREEN_H_23627
