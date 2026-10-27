#include "llc_array_ptr.h"
#include "llc_enum.h"
#include "llc_functional.h"
#include "llc_mutex.h"

#ifndef LLC_CONNECTION_H_23627
#define LLC_CONNECTION_H_23627

namespace llc
{
#pragma pack(push, 1)
	GDEFINE_ENUM_TYPE (ENDPOINT_COMMAND, uint8_t)
	GDEFINE_ENUM_VALUE(ENDPOINT_COMMAND, NOOP		, 0);
	GDEFINE_ENUM_VALUE(ENDPOINT_COMMAND, CONNECT		, 1);
	GDEFINE_ENUM_VALUE(ENDPOINT_COMMAND, PAYLOAD		, 2);
	GDEFINE_ENUM_VALUE(ENDPOINT_COMMAND, DISCONNECT	, 3);

	GDEFINE_ENUM_TYPE (ENDPOINT_COMMAND_TYPE, uint8_t)
	GDEFINE_ENUM_VALUE(ENDPOINT_COMMAND_TYPE, REQUEST	, 0);
	GDEFINE_ENUM_VALUE(ENDPOINT_COMMAND_TYPE, RESPONSE	, 1);

	struct SCommand {
		ENDPOINT_COMMAND			Command						: 2;
		ENDPOINT_COMMAND_TYPE	Type						: 1;
		uint8_t					Packed						: 1;
		uint8_t					Encrypted					: 1;
		uint8_t					Compressed					: 1;
		uint8_t					Multipart					: 1;
		uint8_t					Resend						: 1;
	};

	struct SPayloadHeader {
		SCommand				Command		;
		uint32_t				Size		;
		uint64_t				MessageId	;
	};
#pragma pack(pop)

	struct SMessage {
		au0_t						Payload		;
		uint64_t				Time		;
		SCommand				Command		;
		uint8_t					RetryCount	;
		au3_t						Hashes		;
	};

	struct SClientQueue {
		apobj<SMessage>			Received	;
		apobj<SMessage>			Send		;
		apobj<SMessage>			Sent		;
		mutex						MutexSend	;
		mutex						MutexReceive;
	};

	GDEFINE_ENUM_TYPE (CONNECTION_STATE, uint8_t)
	GDEFINE_ENUM_VALUE(CONNECTION_STATE, DISCONNECTED	, 0);
	GDEFINE_ENUM_VALUE(CONNECTION_STATE, HANDSHAKE		, 1);
	GDEFINE_ENUM_VALUE(CONNECTION_STATE, IDLE				, 2);

	struct SConnection {
		SClientQueue				Queue		= {};
		uint64_t				LastPing	= 0;
		uint64_t				FirstPing	= 0;
		uint64_t				KeyPing		= 0;
		CONNECTION_STATE			State		= CONNECTION_STATE_DISCONNECTED;
	};

	typedef function<err_t(au0_t &)>			FConnectionReceive;
	typedef function<err_t(const vcu0_t &)>	FConnectionSend;

	stxp u2_c	UDP_PAYLOAD_SIZE_LIMIT						= 1024U * 128U;

	err_t	connectionPayloadCollect	(SConnection & client, apobj<SMessage> & receivedMessages);
	err_t	connectionSendQueue			(SConnection & client, apobj<SMessage> & messageCacheSent, apobj<SMessage> & messageCacheSend, const FConnectionSend & send);
	err_t	connectionHandleCommand		(SConnection & client, SCommand & command, const FConnectionReceive & receive, au0_t & receiveBuffer);
	err_t	connectionPushData			(SConnection & client, SClientQueue & queue, const vcu0_t & data, bool bEncrypted = true, bool bCompress = true, uint8_t retryCount = 0);

	stin err_t connectionPushData(SConnection & client, SClientQueue & queue, const vcs0_t & data, bool bEncrypted = true, bool bCompress = true, uint8_t retryCount = 0) {
		return connectionPushData(client, queue, {(const u0_t*)data.begin(), data.size()}, bEncrypted, bCompress, retryCount);
	}
	stin err_t connectionPushData(SConnection & client, SClientQueue & queue, vcst_c & data, bool bEncrypted = true, bool bCompress = true, uint8_t retryCount = 0) {
		return connectionPushData(client, queue, {(const u0_t*)data.begin(), data.size()}, bEncrypted, bCompress, retryCount);
	}

	tplt<tpnm _tWithLoad, tpnm _tState>
	err_t processClientQueueEvents(_tWithLoad & eventCache, uint32_t iClient, vpobj<SMessage> clientQueue, _tState & state, const function<err_t(uint32_t iClient, _tState &, const _tWithLoad &)> & funcProcessRequest) {
		for(uint32_t iMessage = 0; iMessage < clientQueue.size(); ++iMessage) {
			const SMessage & message = *clientQueue[iMessage];
			switch(message.Command.Command) {
			case ENDPOINT_COMMAND_PAYLOAD: {
				vcu0_t payloadInput = {(const uint8_t*)message.Payload.begin(), message.Payload.size()};
				llc_necs(eventCache.Load(payloadInput));
				switch(message.Command.Type) {
				case ENDPOINT_COMMAND_TYPE_REQUEST:
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
	stin err_t processClientQueueEvents(uint32_t iClient, vcpobj<SMessage> clientQueue, _tState & state, const function<err_t(uint32_t iClient, _tState &, const _tWithLoad &)> & funcProcessRequest) {
		_tWithLoad eventCache = {};
		return processClientQueueEvents(eventCache, iClient, clientQueue, state, funcProcessRequest);
	}

	tplt<tpnm _tEvent, tpnm _tState>
	err_t processEvents(view<apobj<SMessage>> queueReceived, _tState & state, const function<err_t(uint32_t iClient, _tState &, const _tEvent &)> & funcProcessRequest) {
		_tEvent eventReceived = {};
		for(uint32_t iClient = 0; iClient < queueReceived.size(); ++iClient) {
			const apobj<SMessage> & clientQueue = queueReceived[iClient];
			llc_necs(processClientQueueEvents(eventReceived, iClient, clientQueue, state, funcProcessRequest));
		}
		return 0;
	}

	typedef apobj<SMessage>		TPacketQueue;
	typedef apobj<au0_t>			TPacketQueueSend;
} // namespace

#endif // LLC_CONNECTION_H_23627
