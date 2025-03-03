#include "llc_event.h"

#ifndef LLC_SYSTEM_EVENT_H_23627
#define LLC_SYSTEM_EVENT_H_23627

namespace llc
{
	GDEFINE_ENUM_TYPE(SYSTEM_EVENT, uint8_t);
	GDEFINE_ENUM_VALUE(SYSTEM_EVENT, Runtime	, 0);
	GDEFINE_ENUM_VALUE(SYSTEM_EVENT, Screen		, 1);
	GDEFINE_ENUM_VALUE(SYSTEM_EVENT, Keyboard	, 2);
	GDEFINE_ENUM_VALUE(SYSTEM_EVENT, Mouse		, 3);
	GDEFINE_ENUM_VALUE(SYSTEM_EVENT, Touch		, 4);
	GDEFINE_ENUM_VALUE(SYSTEM_EVENT, Joypad		, 5);
	GDEFINE_ENUM_VALUE(SYSTEM_EVENT, Text		, 6);
	GDEFINE_ENUM_VALUE(SYSTEM_EVENT, GUI		, 7);
	GDEFINE_ENUM_VALUE(SYSTEM_EVENT, Audio		, 8);
	GDEFINE_ENUM_VALUE(SYSTEM_EVENT, Camera		, 9);
	GDEFINE_ENUM_VALUE(SYSTEM_EVENT, Raster		,10);
	GDEFINE_ENUM_VALUE(SYSTEM_EVENT, Device		,11);
	GDEFINE_ENUM_VALUE(SYSTEM_EVENT, Wire		,12);
	GDEFINE_ENUM_VALUE(SYSTEM_EVENT, Bus		,13);

	tydf ::llc::SEvent            <::llc::SYSTEM_EVENT>	SEventSystem;
	tydf ::llc::SEView            <::llc::SYSTEM_EVENT>	SEViewSystem;
	tydf ::llc::FEventHandler     <::llc::SYSTEM_EVENT>	FEventSystem;
	tydf ::llc::FEventHandlerConst<::llc::SYSTEM_EVENT>	FEventSystemConst;
	tydf ::llc::FEViewHandler     <::llc::SYSTEM_EVENT>	FEViewSystem;
	tydf ::llc::FEViewHandlerConst<::llc::SYSTEM_EVENT>	FEViewSystemConst;
	tydf ::llc::pobj <::llc::SEventSystem>				PEventSystem;
	tydf ::llc::apobj<::llc::SEventSystem>				TQueueSystemEvent;
	
	template<typename _t>
	stin	::llc::error_t	eventEnqueueChild	(::llc::apobj<::llc::SEventSystem> & queue, ::llc::RESULT eventType, _t & eventData)	{ 
		return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Device, eventType, eventData); 
	}

	//GDEFINE_ENUM_TYPE (EVENT_RUNTIME, uint8_t);
	//GDEFINE_ENUM_VALUE(EVENT_RUNTIME, Init			, 0);
	//GDEFINE_ENUM_VALUE(EVENT_RUNTIME, Exit			, 1);
	//GDEFINE_ENUM_VALUE(EVENT_RUNTIME, Process_queue	, 2);
	//GDEFINE_ENUM_VALUE(EVENT_RUNTIME, Process_event	, 3);

#pragma pack(push, 1)
	stct SDeviceResult {
		const ::llc::SYSTEM_EVENT	SystemEvent 	= {};
		const uint8_t				TailWidth		: 2;
		const uint8_t				Multiplier		: 6;
		::llc::RESULT				DeviceEvent		= {};

		cxpr						SDeviceResult	(::llc::RESULT deviceEvent = ::llc::RESULT_Error)
			: SystemEvent {::llc::SYSTEM_EVENT_Device}
			, TailWidth   {::llc::uint_tail_width<uint32_t>((uint32_t)szof(::llc::RESULT))}
			, Multiplier  {::llc::uint_tail_multiplier<uint32_t>((uint32_t)szof(::llc::RESULT))}
			, DeviceEvent {deviceEvent}
			{}
	};
	stct SDeviceCommand {
		const ::llc::SYSTEM_EVENT	SystemEvent 	= {};
		const uint8_t				TailWidth		: 2;
		const uint8_t				Multiplier		: 6;
		::llc::COMMAND				DeviceEvent		= {};

		cxpr						SDeviceCommand	(::llc::COMMAND deviceEvent = ::llc::COMMAND_Ok)
			: SystemEvent {::llc::SYSTEM_EVENT_Device}
			, TailWidth   {::llc::uint_tail_width<uint32_t>((uint32_t)szof(::llc::COMMAND))}
			, Multiplier  {::llc::uint_tail_multiplier<uint32_t>((uint32_t)szof(::llc::COMMAND))}
			, DeviceEvent {deviceEvent}
			{}
	};
#pragma pack(pop)

	//stin	::llc::error_t	eventEnqueueRuntimeInit			(::llc::TQueueSystemEvent & queue)	{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Runtime, ::llc::EVENT_RUNTIME_Init			, ::llc::EVENT_RUNTIME_Init			); }
	//stin	::llc::error_t	eventEnqueueRuntimeExit			(::llc::TQueueSystemEvent & queue)	{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Runtime, ::llc::EVENT_RUNTIME_Exit			, ::llc::EVENT_RUNTIME_Exit			); }
	//stin	::llc::error_t	eventEnqueueRuntimeProcessQueue	(::llc::TQueueSystemEvent & queue)	{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Runtime, ::llc::EVENT_RUNTIME_Process_queue, ::llc::EVENT_RUNTIME_Process_queue); }
	//stin	::llc::error_t	eventEnqueueRuntimeProcessEvent	(::llc::TQueueSystemEvent & queue)	{ return ::llc::eventEnqueueChild(queue, ::llc::SYSTEM_EVENT_Runtime, ::llc::EVENT_RUNTIME_Process_event, ::llc::EVENT_RUNTIME_Process_event); }
} // namespace

#endif // LLC_SYSTEM_EVENT_H_23627
