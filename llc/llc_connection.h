// Recovered from bundle_galaxy_hell/gpk/gpk/gpk_connection.h.
// GPK command, acknowledgement, packing and payload protocol; no multipart support.
#include "llc_array_ptr.h"
#include "llc_stdsocket.h"
#include "llc_enum.h"

#include <functional>

#if !defined(LLC_ATMEL)
#	include <mutex>
#endif

#ifndef LLC_CONNECTION_H_23627
#define LLC_CONNECTION_H_23627

namespace llc
{
#pragma pack(push, 1)
	GDEFINE_ENUM_TYPE (ENDPOINT_COMMAND, uint8_t)
	GDEFINE_ENUM_VALUE(ENDPOINT_COMMAND, NOOP		, 0);
	GDEFINE_ENUM_VALUE(ENDPOINT_COMMAND, CONNECT	, 1);	// - Payload 0: Request connect. - Payload 1: Confirm connect
	GDEFINE_ENUM_VALUE(ENDPOINT_COMMAND, PAYLOAD	, 2);	// - Payload 0: Single payload. - Payload 1: Payload combo.
	GDEFINE_ENUM_VALUE(ENDPOINT_COMMAND, DISCONNECT	, 3);

	GDEFINE_ENUM_TYPE (ENDPOINT_COMMAND_TYPE, uint8_t)
	GDEFINE_ENUM_VALUE(ENDPOINT_COMMAND_TYPE, REQUEST	, 0);
	GDEFINE_ENUM_VALUE(ENDPOINT_COMMAND_TYPE, RESPONSE	, 1);

	struct SUDPCommand {
		ENDPOINT_COMMAND					Command						: 2;
		ENDPOINT_COMMAND_TYPE				Type						: 1;
		uint8_t								Packed						: 1;
		uint8_t								Encrypted					: 1;
		uint8_t								Compressed					: 1;
		uint8_t								Multipart					: 1;
		uint8_t								Resend						: 1;
	};

	struct SUDPPayloadHeader {
		::llc::SUDPCommand					Command		;
		uint32_t							Size		;
		uint64_t							MessageId	;
	};
#pragma pack(pop)

	struct SUDPMessage {
		::llc::au0_t						Payload		;
		uint64_t							Time		;
		::llc::SUDPCommand					Command		;
		uint8_t								RetryCount	;
		::llc::au3_t						Hashes		;
	};

	struct SUDPClientQueue {
		::llc::apobj<::llc::SUDPMessage>	Received	;
		::llc::apobj<::llc::SUDPMessage>	Send		;
		::llc::apobj<::llc::SUDPMessage>	Sent		;
		::std::mutex						MutexSend	;
		::std::mutex						MutexReceive;
	};

	GDEFINE_ENUM_TYPE (UDP_CONNECTION_STATE, uint8_t)
	GDEFINE_ENUM_VALUE(UDP_CONNECTION_STATE, DISCONNECTED				, 0);
	GDEFINE_ENUM_VALUE(UDP_CONNECTION_STATE, HANDSHAKE					, 1);	// - Payload 0: Request connect. - Payload 1: Confirm connect
	GDEFINE_ENUM_VALUE(UDP_CONNECTION_STATE, IDLE						, 2);	// - Payload 0: Single payload. - Payload 1: Payload combo.

	struct SUDPConnection {
		::llc::auto_socket_close			Socket						;
		::llc::SUDPClientQueue				Queue						= {};
		::llc::SIPv4End						Address						= {};	// Local for servers, remote for clients
		uint64_t							LastPing					= 0;
		uint64_t							FirstPing					= 0;
		uint64_t							KeyPing						= 0;
		::llc::UDP_CONNECTION_STATE			State						= ::llc::UDP_CONNECTION_STATE_DISCONNECTED;
	};

	struct STCPConnection {
		::llc::auto_socket_close			Socket						;
		::llc::SUDPClientQueue				Queue						= {};
		::llc::SIPv4End						Address						= {};
		uint64_t							LastPing					= 0;
		uint64_t							FirstPing					= 0;
		uint64_t							KeyPing						= 0;
		::llc::UDP_CONNECTION_STATE			State						= ::llc::UDP_CONNECTION_STATE_DISCONNECTED;
	};

	stxp	const uint32_t				UDP_PAYLOAD_SIZE_LIMIT		= 60 * 1024;

	::llc::error_t						connectionPayloadCollect	(::llc::SUDPConnection & client, ::llc::apobj<::llc::SUDPMessage> & receivedMessages);
	::llc::error_t						connectionSendQueue			(::llc::SUDPConnection & client, ::llc::apobj<::llc::SUDPMessage>& messageCacheSent, ::llc::apobj<::llc::SUDPMessage>& messageCacheSend, bool packPayloads = true);
	::llc::error_t						connectionHandleCommand		(::llc::SUDPConnection & client, ::llc::SUDPCommand & command, ::llc::au0_t & receiveBuffer);
	::llc::error_t						connectionPushData			(::llc::SUDPConnection & client, ::llc::SUDPClientQueue & queue, const ::llc::vcu0_t & data, bool bEncrypted = true, bool bCompress = true, uint8_t retryCount = 0);
	stin	::llc::error_t				connectionPushData			(::llc::SUDPConnection & client, ::llc::SUDPClientQueue & queue, const ::llc::vcs0_t & data, bool bEncrypted = true, bool bCompress = true, uint8_t retryCount = 0) {
		return ::llc::connectionPushData(client, queue, *(const ::llc::vcu0_t*)&data, bEncrypted, bCompress, retryCount);
	}
	stin	::llc::error_t				connectionPushData			(::llc::SUDPConnection & client, ::llc::SUDPClientQueue & queue, ::llc::vcst_c & data, bool bEncrypted = true, bool bCompress = true, uint8_t retryCount = 0) {
		return ::llc::connectionPushData(client, queue, *(const ::llc::vcu0_t*)&data, bEncrypted, bCompress, retryCount);
	}

	tplt<tpnm _tWithLoad, tpnm _tState> 
	::llc::error_t						processUDPClientQueueEvents	
		( _tWithLoad						& eventCache
		, uint32_t							iClient
		, ::llc::vpobj<::llc::SUDPMessage>	clientQueue
		, _tState							& state
		, const ::std::function<::llc::error_t (uint32_t iClient, _tState &, const _tWithLoad &)>	& funcProcessRequest
		) {
		for(uint32_t iMessage = 0; iMessage < clientQueue.size(); ++iMessage) {
			const ::llc::SUDPMessage				& message					= *clientQueue[iMessage];
			switch(message.Command.Command) {
			case ::llc::ENDPOINT_COMMAND_PAYLOAD: {
				::llc::vcu0_t							payloadInput				= {(const uint8_t*)message.Payload.begin(), message.Payload.size()};
				llc_necs(eventCache.Load(payloadInput));
				switch(message.Command.Type) {
				case ::llc::ENDPOINT_COMMAND_TYPE_REQUEST	: 
					llc_necs(funcProcessRequest(iClient, state, eventCache)); 
					break;
				}
				break;
			}
			}
		}
		return 0;
	}

	tplt<tpnm _tWithLoad, tpnm _tState> 
	stin	::llc::error_t				processUDPClientQueueEvents	
		( uint32_t							iClient
		, ::llc::vcpobj<::llc::SUDPMessage>	clientQueue
		, _tState							& state
		, const ::std::function<::llc::error_t (uint32_t iClient, _tState &, const _tWithLoad &)>	& funcProcessRequest
		) {
		_tWithLoad								eventCache					= {};
		return ::llc::processUDPClientQueueEvents(eventCache, iClient, clientQueue, state, funcProcessRequest);
	}

	tplt<tpnm _tEvent, tpnm _tState> 
	::llc::error_t						processUDPEvents
		( ::llc::view<::llc::apobj<::llc::SUDPMessage>>	queueReceived
		, _tState										& state
		, const ::std::function<::llc::error_t (uint32_t iClient, _tState &, const _tEvent &)>	& funcProcessRequest
		) {
		_tEvent									eventReceived				= {};
		for(uint32_t iClient = 0; iClient < queueReceived.size(); ++iClient) {
			 const ::llc::apobj<::llc::SUDPMessage>	& clientQueue				= queueReceived[iClient];
			 llc_necs(llc::processUDPClientQueueEvents(eventReceived, iClient, clientQueue, state, funcProcessRequest));
		}
		return 0;
	}

	typedef	::llc::apobj<::llc::SUDPMessage>	TUDPQueue;
	typedef	::llc::apobj<::llc::au0_t>			TQueueSend;
}

#endif // LLC_CONNECTION_H_23627
