// Recovered from bundle_galaxy_hell/gpk/gpk/gpk_connection.cpp.
// GPK command, acknowledgement, packing and payload protocol; no multipart support.
#include "llc_connection.h"
#include "llc_chrono.h"
#include "llc_view_stream.h"
#include "llc_noise.h"
#include "llc_encoding.h"
#include "llc_deflate.h"

using ::llc::sc_t, ::llc::sc_c;

::llc::error_t			llc::connectionPushData				(::llc::SUDPConnection & client, ::llc::SUDPClientQueue & queue, const ::llc::vcu0_t & data, bool bEncrypt, bool bCompress, uint8_t retryCount) {
	if_true_fef(data.size() > ::llc::UDP_PAYLOAD_SIZE_LIMIT, "%s", "Invalid payload size.");

	::llc::pobj<::llc::SUDPMessage>	message;
	message->Command.Command	= ::llc::ENDPOINT_COMMAND_PAYLOAD;
	message->Command.Type		= ::llc::ENDPOINT_COMMAND_TYPE_REQUEST;
	message->Command.Encrypted	= (uint8_t)(bEncrypt ? 1 : 0);
	message->Command.Compressed	= (uint8_t)(bCompress ? 1 : 0);
	message->Command.Packed		= 0;
	message->Command.Multipart	= 0;
	message->Time				= ::llc::timeCurrentInUs();
	message->RetryCount			= retryCount;
	if(0 == client.KeyPing) {
		client.KeyPing				= message->Time;
		info_printf("Key ping set: %llu.", client.KeyPing);
	}
	//message->Payload.clear();
	message->Payload					= data;
	{
		::std::lock_guard			lock								(queue.MutexSend);
		llc_necs(queue.Send.push_back(message));
	}
	return 0;
}

::llc::error_t			llc::connectionPayloadCollect		(::llc::SUDPConnection & client, ::llc::apobj<::llc::SUDPMessage> & receivedMessages)	{
	if(0 == client.KeyPing || client.State == ::llc::UDP_CONNECTION_STATE_HANDSHAKE)
		return 1;

	::std::lock_guard			lockRecv							(client.Queue.MutexReceive);
	for(int32_t iMessage = 0; iMessage < (int32_t)client.Queue.Received.size(); ++iMessage) {
		const ::llc::pobj<::llc::SUDPMessage>	& messageReceived		= client.Queue.Received[iMessage];
		const ::llc::SUDPCommand				udpCommand				= messageReceived->Command;
		if( udpCommand.Type		!= ::llc::ENDPOINT_COMMAND_TYPE_REQUEST 
		 || udpCommand.Command	!= ::llc::ENDPOINT_COMMAND_PAYLOAD
		) 
			continue;
		llc_necs(receivedMessages.push_back(messageReceived));
		llc_necs(client.Queue.Received.remove_unordered(iMessage--));
	}
	return 0;
}

stxp	uint64_t		hashFromTime						(uint64_t currentTime)		noexcept	{ return ::llc::noise1DBase(currentTime & 0xFFFFFFFF) + (currentTime >> 16); }
stxp	uint32_t		UDP_PAYLOAD_SENT_LIFETIME			= 1000000; // microseconds

::llc::error_t			payloadQueueOptimize				(::llc::SUDPConnection & client, ::llc::apobj<::llc::SUDPMessage> & messageCacheSent, ::llc::apobj<::llc::SUDPMessage> & payloadsToSend) {
	if(payloadsToSend.size() < 2)
		return 0;

	{
		::llc::apobj<::llc::SUDPMessage>	optimizedPayloads		= {};
		::llc::apobj<::llc::SUDPMessage>	payloadsToRemove		= {};
		::llc::au1_t						payloadSizes			= {};
		::llc::au0_t						serialized				= {};

		::llc::pobj<::llc::SUDPMessage>		optimizedMsg			= {};
		optimizedMsg->Time				= ::llc::timeCurrentInUs();
		optimizedMsg->Command			= {};
		optimizedMsg->Command.Command	= ::llc::ENDPOINT_COMMAND_PAYLOAD;
		optimizedMsg->Command.Type		= ::llc::ENDPOINT_COMMAND_TYPE_REQUEST;
		optimizedMsg->Command.Packed	= 1;
		optimizedMsg->RetryCount		= 1;

		bool						keyPing								= false;
		for(uint32_t iPayload = 0; iPayload < payloadsToSend.size(); ++iPayload) {
			::llc::pobj<::llc::SUDPMessage>	currentPayloadMsg				= payloadsToSend[iPayload];
			const ::llc::vcu0_t				currentPayloadView				= currentPayloadMsg->Payload;
			if(0 == currentPayloadMsg.get_ref())
				continue;
			if((currentPayloadMsg->Command.Packed & 1) || currentPayloadMsg->Command.Type == ::llc::ENDPOINT_COMMAND_TYPE_RESPONSE || currentPayloadMsg->Command.Command != ::llc::ENDPOINT_COMMAND_PAYLOAD)
				continue;
			const bool						sizeEnough						= (serialized.size() + sizeof(uint16_t) + currentPayloadMsg->Payload.size()) < ::llc::UDP_PAYLOAD_SIZE_LIMIT;
			if(false == sizeEnough)
				continue;
			if(0 == currentPayloadView.size())
				continue;
			if(currentPayloadMsg->Time == client.KeyPing)
				keyPing						= true;
			llc_necs(payloadSizes.push_back((uint16_t)currentPayloadView.size()));
			llc_necs(serialized.append(currentPayloadView));
			llc_necs(payloadsToRemove.push_back(currentPayloadMsg));
			llc_necs(messageCacheSent.push_back(currentPayloadMsg));
		}
		if_true_vif(0, 0 == payloadSizes.size(), "No packets to optimize. Total packets searched: %u.", payloadsToSend.size());
		info_printf("%u packets optimized.", payloadSizes.size());
		const uint32_t				sizeRequired						= serialized.size() + sizeof(uint16_t) * payloadSizes.size() + sizeof(uint16_t);	// payloads + sizes + packet count
		uint16_t					packetCount							= (uint16_t)payloadSizes.size();
		::llc::au0_t				& optimizedPayloadBuffer			= optimizedMsg->Payload;
		llc_necall(optimizedPayloadBuffer.resize(sizeRequired), "Out of memory? Size required: %u.", sizeRequired);
		optimizedPayloadBuffer.clear();
		optimizedPayloadBuffer.append((const uint8_t*)&packetCount, sizeof(uint16_t));
		optimizedPayloadBuffer.append((const uint8_t*)payloadSizes.begin(), sizeof(uint16_t) * payloadSizes.size());
		optimizedPayloadBuffer.append(serialized.begin(), serialized.size());
		if_true_fef(sizeRequired != optimizedPayloadBuffer.size(), "Invalid sizes! Size required: %u. Actual size: %u.", sizeRequired, optimizedPayloadBuffer.size());
		optimizedMsg->Command.Compressed	= 1;
		optimizedMsg->Command.Encrypted		= 1;
		llc_necall(payloadsToSend.push_back(optimizedMsg), "Failed! Payload queue size: %u.", payloadsToSend.size());

		if(keyPing)
			client.KeyPing						= optimizedMsg->Time;

		for(int32_t iPayloadTR = 0; iPayloadTR < (int32_t)payloadsToRemove.size(); ++iPayloadTR)	// Done. Remove packed payloads from send queue.
			for(int32_t iPayload = 0; iPayload < (int32_t)payloadsToSend.size(); ++iPayload) {
				if(payloadsToRemove[iPayloadTR] == payloadsToSend[iPayload]) {
					payloadsToSend.remove_unordered(iPayload);
					--iPayload;
				}
			}
	}
	return 0;
}

::llc::error_t			payloadQueueSend					(::llc::SUDPConnection & client, ::llc::apobj<::llc::SUDPMessage> & messageCacheSent, ::llc::apobj<::llc::SUDPMessage> & payloadsToSend) {
	::llc::au0_t				messageBytes;
	sockaddr_in					sa_remote							= {};							// Information about the client
	::llc::tcpipAddressToSockaddr(client.Address, sa_remote);
	for(uint32_t iPayload = 0; iPayload < payloadsToSend.size(); ++iPayload) {
		::llc::pobj<::llc::SUDPMessage>	pMessageToSend						= payloadsToSend[iPayload];
		if(0 == pMessageToSend.get_ref())
			continue;
		::llc::SUDPMessage				& messageToSend						= *pMessageToSend;
		::llc::SUDPPayloadHeader		payloadHeader						= {};
		payloadHeader.Command				= messageToSend.Command;
		payloadHeader.Size					= messageToSend.Payload.size();
		if(messageToSend.Command.Type == ::llc::ENDPOINT_COMMAND_TYPE_RESPONSE) {
			payloadHeader.MessageId				= messageToSend.Time;
			if_true_cef((int)sizeof(::llc::SUDPPayloadHeader) != ::sendto(client.Socket, (sc_c*)&payloadHeader, (int)sizeof(::llc::SUDPPayloadHeader), 0, (sockaddr*)&sa_remote, sizeof(sockaddr_in)), "%s", "Error sending datagram.");	// Send data back
			llc_necs(messageCacheSent.push_back(pMessageToSend));
			info_printf("Sent response to message id: %llx", payloadHeader.MessageId);
		}
		else {
			if_true_cef(messageToSend.Payload.size() > ::llc::UDP_PAYLOAD_SIZE_LIMIT, "Maximum allowed payload size is only %u bytes.", ::llc::UDP_PAYLOAD_SIZE_LIMIT);
			::llc::view_stream<uint8_t>				sendStream;
			::llc::au0_t					ardellEncoded;
			payloadHeader.MessageId				= ::hashFromTime(messageToSend.Time);
			messageToSend.Hashes.push_back(payloadHeader.MessageId);
			info_printf("Sending message with id: %llx.", payloadHeader.MessageId);
			::llc::au0_t					compressed;
			if(0 == payloadHeader.Command.Compressed) {
				if(0 == payloadHeader.Command.Encrypted)
					payloadHeader.Size				= messageToSend.Payload.size();
				else {
					::llc::ardellEncode(messageToSend.Payload, client.KeyPing & 0xFFFFFFFF, true, ardellEncoded);
					payloadHeader.Size				= ardellEncoded.size();
				}
			}
			else {
				::llc::arrayDeflate(messageToSend.Payload, compressed);
				if(0 == payloadHeader.Command.Encrypted)
					payloadHeader.Size				= compressed.size();
				else {
					::llc::ardellEncode(compressed, client.KeyPing & 0xFFFFFFFF, true, ardellEncoded);
					payloadHeader.Size				= ardellEncoded.size();
				}
			}

			llc_necs(messageBytes.resize((uint32_t)sizeof(::llc::SUDPPayloadHeader) + payloadHeader.Size));
			sendStream						= {messageBytes.begin(), messageBytes.size()};
			llc_necs(sendStream.write_pod(payloadHeader));

			if(0 != payloadHeader.Command.Encrypted) {
				info_printf("Original packet size: %u. Final size: %u.", messageToSend.Payload.size(), ardellEncoded.size());
				llc_necs(sendStream.write_pod(::llc::vcu0_t{ardellEncoded}));
			}
			else {
				if(0 == payloadHeader.Command.Compressed)
					llc_necs(sendStream.write_pod(::llc::vcu0_t{messageToSend.Payload}));
				else {
					info_printf("Original packet size: %u. Compressed size: %u.", messageToSend.Payload.size(), compressed.size());
					llc_necs(sendStream.write_pod(::llc::vcu0_t{compressed}));
				}
			}

			if_true_cef((int)sendStream.CursorPosition != ::sendto(client.Socket, (const sc_t*)sendStream.begin(), (int)sendStream.CursorPosition, 0, (sockaddr*)&sa_remote, sizeof(sockaddr_in)), "%s", "Error sending datagram.");	// Send data back
			verbose_printf("%u bytes sent.", sendStream.CursorPosition);
			{
				llc_necs(client.Queue.Sent.push_back(pMessageToSend));
			}
			llc_necs(messageCacheSent.push_back(pMessageToSend));
		}
	}
	return 0;
}

::llc::error_t			llc::connectionSendQueue			(::llc::SUDPConnection & client, ::llc::apobj<::llc::SUDPMessage> & messageCacheSent, ::llc::apobj<::llc::SUDPMessage> & messageCacheSend, bool packPayloads)				{
	::llc::apobj<::llc::SUDPMessage>	& queueToSend						= client.Queue.Send;
	sockaddr_in							sa_remote							= {};							// Information about the client
	::llc::tcpipAddressToSockaddr(client.Address, sa_remote);

	::std::lock_guard					lock								(client.Queue.MutexSend);
	for(int32_t iSent = 0; iSent < (int32_t)client.Queue.Sent.size(); ++iSent) {
		::llc::pobj<::llc::SUDPMessage> messageSent = client.Queue.Sent[iSent];
		if(0 == messageSent.get_ref())
			continue;
		uint64_t timeCurr = llc::timeCurrentInUs();
		const uint64_t timeSent = messageSent->Time;
		const uint64_t timeDiff = timeCurr - timeSent;
		if(timeDiff > ::UDP_PAYLOAD_SENT_LIFETIME) {
			client.Queue.Sent.remove_unordered(iSent);
			--iSent;
			if(messageSent->RetryCount) {
				--messageSent->RetryCount;
				timeCurr	+= ::UDP_PAYLOAD_SENT_LIFETIME;
				info_printf("A message sent didn't receive a confirmation on time: %llx (%llx). Setting current time: %llx, Retries left: %u.", messageSent->Time, ::hashFromTime(messageSent->Time), timeCurr, (uint32_t)messageSent->RetryCount);
				if(messageSent->Time == client.KeyPing)
					messageSent->Time	= client.KeyPing	= timeCurr;
				else
					messageSent->Time	= timeCurr;
				queueToSend.push_back(messageSent);
			}
			else {
				info_printf("A message sent didn't receive a confirmation on time: %llx.", messageSent->Time);
			}
		}
	}

	// Send non-payload commands and
	::llc::apobj<::llc::SUDPMessage>					& payloadsToSend						= messageCacheSend;
	payloadsToSend.clear();
	messageCacheSent.clear();
	for(uint32_t iSend = 0; iSend < queueToSend.size(); ++iSend) {
		::llc::pobj<::llc::SUDPMessage>					 pMessageToSend						= queueToSend[iSend];
		if(0 == pMessageToSend.get_ref())
			continue;
		::llc::SUDPMessage									& messageToSend						= *pMessageToSend;
//		int64_t															currentTime							= ::llc::timeCurrentInUs();
		if(messageToSend.Command.Command == ::llc::ENDPOINT_COMMAND_PAYLOAD)
			payloadsToSend.push_back(pMessageToSend);
		else {
			if_true_cef((int)sizeof(::llc::SUDPCommand) != ::sendto(client.Socket, (const sc_t*)&messageToSend.Command, (int)sizeof(::llc::SUDPCommand), 0, (sockaddr*)&sa_remote, sizeof(sockaddr_in)), "%s", "Error sending datagram.");	// Send data back
			//pMessageToSend->Time										= currentTime;
			llc_necs(client.Queue.Sent.push_back(pMessageToSend));
			llc_necs(messageCacheSent.push_back(pMessageToSend));
		}
	}
	if(packPayloads) {
		llc_necall(::payloadQueueOptimize(client, messageCacheSent, payloadsToSend), "%s", "Failed to optimize payload queue.");
	}
	llc_necall(::payloadQueueSend(client, messageCacheSent, payloadsToSend), "%s", "Failed ");
	for(uint32_t iSent = 0; iSent < messageCacheSent.size(); ++iSent)
		for(uint32_t iSend = 0; iSend < queueToSend.size(); ++iSend)
			if(queueToSend[iSend] == messageCacheSent[iSent]) {
				queueToSend.remove(iSend);
				break;
			}
	return 0;
}

static	::llc::error_t	handleRequestDISCONNECT					(::llc::SUDPCommand & command, ::llc::SUDPConnection & client)												{
	if_true_fef(0 != command.Packed || 0 != command.Encrypted || 0 != command.Compressed || ::llc::UDP_CONNECTION_STATE_IDLE != client.State, "Invalid command or client state: 0x%X '%s'.", client.State, ::llc::get_value_label(client.State).begin());
	client.FirstPing				=
	client.KeyPing					= 0;
	client.LastPing					= llc::timeCurrentInUs();
	client.State					= ::llc::UDP_CONNECTION_STATE_DISCONNECTED;
	client.Socket.close();
	info_printf("%s", "Disconnected!");
	return 0;
}

static	::llc::error_t	handleRequestCONNECT						(::llc::SUDPCommand & command, ::llc::SUDPConnection & client)												{
	if_true_fef(1 != command.Packed || ::llc::UDP_CONNECTION_STATE_HANDSHAKE != client.State, "Invalid client state: 0x%X '%s'.", client.State, ::llc::get_value_label(client.State).begin());
	client.LastPing					= llc::timeCurrentInUs();
	{
		::std::lock_guard					lock								(client.Queue.MutexSend);
		for(uint32_t iSent = 0, countSent = client.Queue.Sent.size(); iSent < countSent; ++iSent)
			if(::llc::ENDPOINT_COMMAND_CONNECT == client.Queue.Sent[iSent]->Command.Command) {
				client.Queue.Sent.remove_unordered(iSent);
				break;
			}
	}

	client.State					= ::llc::UDP_CONNECTION_STATE_IDLE;
	info_printf("%s", "Connected!");
	return 0;
}

#if defined(LLC_WINDOWS)
typedef int socklen_t;
#endif

static	::llc::error_t	postConfirmationResponse			(::llc::SUDPClientQueue & queue, const ::llc::SUDPCommand command, const uint64_t messageId)				{
	::llc::pobj<::llc::SUDPMessage>		response							= {};
	response->Command				= command;
	response->Command.Type			= ::llc::ENDPOINT_COMMAND_TYPE_RESPONSE;
	response->Time					= messageId;
	{
		::std::lock_guard					lock								(queue.MutexSend);
		llc_necall(queue.Send.push_back(response), "Out of memory? Queue size: %u.", queue.Send.size());
	}
	return 0;
}

static	::llc::error_t	handlePAYLOADResponse				(const ::llc::SUDPPayloadHeader & header, ::llc::SUDPConnection & client)		{
	// Remove from sent queue.
	::std::lock_guard					lock								(client.Queue.MutexSend);
	info_printf("Received response to message id: %llx", header.MessageId);
	bool								bFound								= false;
	for(uint32_t iSent = 0; iSent < client.Queue.Sent.size(); ++iSent) {
		::llc::SUDPMessage					& messageSent						= *client.Queue.Sent[iSent];
		for(uint32_t iHash = 0; iHash < messageSent.Hashes.size(); ++iHash) {
			info_printf("Checking hash for message: %llx.", messageSent.Hashes[iHash]);
			if(::hashFromTime(messageSent.Time) == header.MessageId) {
				info_printf("Removing message from confirmation queue: %llx. ", header.MessageId);
				client.Queue.Sent.remove_unordered(iSent);
				client.LastPing					= ::llc::timeCurrentInUs();
				bFound							= true;
				break;
			}
			uint64_t							hashLocal							= messageSent.Hashes[iHash]; //::hashFromTime(messageSent.Time);
			if(hashLocal == header.MessageId) {
				info_printf("Removing message from confirmation queue: %llx. ", header.MessageId);
				client.Queue.Sent.remove_unordered(iSent);
				client.LastPing					= ::llc::timeCurrentInUs();
				bFound							= true;
				break;
			}
		}
		if(bFound)
			break;
	}
	if(false == bFound)
		info_printf("Response confirmation for message was not found: %llx.", header.MessageId);

	return 0;
}

stxp	int64_t			advantage							= 1000000;

static	::llc::error_t	handlePAYLOADRequest				(const ::llc::SUDPPayloadHeader & header, ::llc::SUDPCommand & command, ::llc::SUDPConnection & client, ::llc::au0_t & receiveBuffer)		{
	sockaddr_in					sa_client							= {};						// Information about the client
	socklen_t					sa_length							= (socklen_t)sizeof(sockaddr_in);	// Length of client struct
	int							bytes_received						= {};
	llc_necs(receiveBuffer.resize(sizeof(::llc::SUDPPayloadHeader) + header.Size));
	memset(receiveBuffer.begin(), 0, receiveBuffer.size());
	if(::llc::failed(bytes_received = ::recvfrom(client.Socket, (sc_t*)receiveBuffer.begin(), (int)receiveBuffer.size(), MSG_PEEK, (sockaddr*)&sa_client, &sa_length))) {
#if defined(LLC_WINDOWS)
		if_true_fwf(WSAGetLastError() != WSAEMSGSIZE, "%s", "Could not receive payload data.")
		else
			warning_printf("%s", "Failed to receive all of the payload data.");
#else
		error_printf("%s", "Failed to receive all of the payload data.");
		return -1;
#endif
	}
	else
		if_true_vif(0, 0 == bytes_received, "The peer has performed an orderly shutdown: '%s'", "https://linux.die.net/man/3/recvfrom");

	if_true_fef(receiveBuffer.size() != (uint32_t)bytes_received, "Packet size received doesn't match with header size. Received size: %u. Expected: %u.", bytes_received, receiveBuffer.size());

	uint64_t							estimatedTimeSent					= 0;
	if(client.KeyPing == 0) {
		for(int64_t iTime = 0, countLapse = 60000000; iTime < countLapse; ++iTime)
			if(::hashFromTime(client.FirstPing - advantage + iTime) == header.MessageId) {
				client.KeyPing					= client.FirstPing - advantage + iTime;
				estimatedTimeSent				= client.FirstPing - advantage + iTime;
				info_printf("Key ping detected in %u attempts: %llu. First ping: %llu. Difference: %llu.", iTime, client.KeyPing, client.FirstPing, client.KeyPing - client.FirstPing);
				break;
			}
		if_true_fef(0 == client.KeyPing, "%s", "Failed to determine encryption key!");
	}
	else { // Why the fuck is this commented???
		//const uint64_t					nowInUs							= ::llc::timeCurrentInUs();
		//const uint64_t					startTime						= nowInUs - 3000000;
		//for(uint32_t iTime = 0, countLapse = 1000000; iTime < countLapse; ++iTime) {
		//	const uint64_t						timeSent						= startTime + iTime;
		//	if(::hashFromTime(timeSent) == header.MessageId) {
		//		info_printf("Payload timestamp found in %u attempts: Latency: %llu.", iTime, nowInUs - timeSent);
		//		estimatedTimeSent				= timeSent;
		//		break;
		//	}
		//}
	}
	::llc::pobj<::llc::SUDPMessage>		messageReceived						= {};
	messageReceived->Command		= header.Command;
	messageReceived->Time			= estimatedTimeSent; //header.MessageId;
	if(header.Size > 0) {
		const ::llc::vcu0_t					viewPayload							= {&receiveBuffer[sizeof(::llc::SUDPPayloadHeader)], header.Size};
		if(0 != command.Compressed) {
			if(0 != command.Encrypted) {
				::llc::au0_t				inflated;
				::llc::au0_t				decoded;
				if_true_fwf(::llc::failed(::llc::ardellDecode(viewPayload, client.KeyPing & 0xFFFFFFFF, true, decoded)), "%s", "Failed to decrypt packet!");
				if_true_fwf(::llc::failed(::llc::arrayInflate(decoded, messageReceived->Payload)), "%s", "Failed to inflate packet!");
			}
			else {
				if_true_fwf(::llc::failed(::llc::arrayInflate(viewPayload, messageReceived->Payload)), "%s", "Failed to inflate packet!");
			}
		}
		else {
			if(0 != command.Encrypted) {
				messageReceived->Payload.clear();
				if_true_fwf(::llc::failed(::llc::ardellDecode(viewPayload, client.KeyPing & 0xFFFFFFFF, true, messageReceived->Payload)), "%s", "Failed to decrypt packet!");
			}
			else {
				messageReceived->Payload.resize(header.Size);
				memcpy(messageReceived->Payload.begin(), viewPayload.begin(), header.Size);
				{
					messageReceived->Payload.resize(header.Size + 1, 0);
					messageReceived->Payload.resize(header.Size);
				}
			}
		}
	}
	info_printf("Packet received. Payload size received: %u. Payload size after unpacking/decrypting: %u.", header.Size, messageReceived->Payload.size());
	client.LastPing					= ::llc::timeCurrentInUs();
	//if_true_fef(header.Command.Payload > 3, "Unsupported payload format. Payload: %u.", (uint32_t)header.Command.Payload);
	if(0 == (header.Command.Packed & 1)) {
		{
			::std::lock_guard					lock								(client.Queue.MutexReceive);
			llc_necall(client.Queue.Received.push_back(messageReceived), "Out of memory? Queue size: %u.", client.Queue.Received.size());
		}
		::llc::pobj<::llc::SUDPMessage>		response							= {};
		::postConfirmationResponse(client.Queue, header.Command, header.MessageId);
		//info_printf("Sent ack for message id: %llu.", header.MessageId);
	}
	else {
		// 1. Read packet count
		// 2. Read packet sizes (sizeof(uint16_t) * packet count)
		// 3. Read payload contents
		::llc::view_stream<uint8_t>			payload								= {messageReceived->Payload.begin(), messageReceived->Payload.size()};
		uint16_t							packetCount							= 0;
		llc_necall(payload.read_pod(packetCount), "%s", "Invalid data.");
		if(0 == packetCount)
			return 0;
		if_true_fef((packetCount * sizeof(uint16_t)) > (payload.size() - payload.CursorPosition), "%s", "Invalid packet data.");
		::llc::vcu1_t						sizes								= {(uint16_t*)&payload[payload.CursorPosition], packetCount};
		payload.CursorPosition			+= packetCount * sizeof(uint16_t);
		::llc::apobj<::llc::SUDPMessage>	tempQueue;
		for(uint32_t iSizes = 0; iSizes < sizes.size(); ++iSizes) {
			::llc::pobj<::llc::SUDPMessage>	messageReceivedInt						= {};
			messageReceivedInt->Command				= header.Command;
			messageReceivedInt->Time				= header.MessageId;
			messageReceivedInt->Command.Packed		= 0;	// Clear payload mode bits
			messageReceivedInt->Command.Encrypted	= 0;
			messageReceivedInt->Command.Compressed	= 0;
			llc_necs(messageReceivedInt->Payload.resize(sizes[iSizes]));
			llc_necall(payload.read_pod(::llc::vu0_t{messageReceivedInt->Payload}), "Corrupt payload data. Sizes don't match. Payload size: %u.", payload.size());
			llc_necs(tempQueue.push_back(messageReceivedInt));
		}
		{
			::std::lock_guard					lock								(client.Queue.MutexReceive);
			for(uint32_t iMessage = 0; iMessage < tempQueue.size(); ++iMessage)
				llc_necall(client.Queue.Received.push_back(tempQueue[iMessage]), "Out of memory? Queue size: %u.", client.Queue.Received.size());
		}
		::postConfirmationResponse(client.Queue, header.Command, header.MessageId);
	}
	return 0;
}

static	::llc::error_t	handlePAYLOAD						(::llc::SUDPCommand & command, ::llc::SUDPConnection & client, ::llc::au0_t & receiveBuffer)		{
	if(::llc::UDP_CONNECTION_STATE_IDLE != client.State)
		return 1;
	::llc::SUDPPayloadHeader			header								= {};
	sockaddr_in							sa_client							= {};						// Information about the client
	socklen_t							sa_length							= (socklen_t)sizeof(sockaddr_in);	// Length of client struct
	int									bytes_received						= {};
	if(::llc::failed(bytes_received = ::recvfrom(client.Socket.Handle, (sc_t*)&header, (int)sizeof(::llc::SUDPPayloadHeader), MSG_PEEK, (sockaddr*)&sa_client, &sa_length))) {
#if defined(LLC_WINDOWS)
		if_true_fwf(WSAGetLastError() != WSAEMSGSIZE, "%s", "Could not receive payload header.");
#else
#endif
	}
	switch(command.Type) {
	default										: return -1;
	case ::llc::ENDPOINT_COMMAND_TYPE_REQUEST	: return ::handlePAYLOADRequest (header, command, client, receiveBuffer); 
	case ::llc::ENDPOINT_COMMAND_TYPE_RESPONSE	: return ::handlePAYLOADResponse(header, client); 
	}
}

::llc::error_t			llc::connectionHandleCommand		(::llc::SUDPConnection & client, ::llc::SUDPCommand & command, ::llc::au0_t & receiveBuffer)		{
	::llc::vcst_t				labelCommand						= ::llc::get_value_label(command.Command);
	::llc::vcst_t				labelType							= ::llc::get_value_label(command.Type	);
	(void)labelCommand	;
	(void)labelType		;
	info_printf("Command: %s. Type: %s."
		"\nCompressed   : %i."
		"\nEncrypted    : %i."
		"\nMultipart    : %i."
		"\nPacked       : %i."
		, labelCommand.begin(), labelType.begin()
		, command.Compressed ? 1 : 0
		, command.Encrypted	 ? 1 : 0
		, command.Multipart	 ? 1 : 0
		, command.Packed	 ? 1 : 0
		);
	switch(command.Type) {
	case ::llc::ENDPOINT_COMMAND_TYPE_REQUEST:
		switch(command.Command) {
		case ::llc::ENDPOINT_COMMAND_PAYLOAD	: return ::handlePAYLOAD			(command, client, receiveBuffer);
		case ::llc::ENDPOINT_COMMAND_CONNECT	: return ::handleRequestCONNECT		(command, client);
		case ::llc::ENDPOINT_COMMAND_DISCONNECT	: return ::handleRequestDISCONNECT	(command, client);
		case ::llc::ENDPOINT_COMMAND_NOOP		: return 0;
		default									:
			error_printf("Invalid command!: %u.", command.Command);
			break;
		}
		break;
	case ::llc::ENDPOINT_COMMAND_TYPE_RESPONSE:
		switch(command.Command) {
		case ::llc::ENDPOINT_COMMAND_PAYLOAD	: return ::handlePAYLOAD(command, client, receiveBuffer);
		case ::llc::ENDPOINT_COMMAND_CONNECT	:
		case ::llc::ENDPOINT_COMMAND_DISCONNECT	:
		case ::llc::ENDPOINT_COMMAND_NOOP		: return 0;
		default									:
			error_printf("Invalid command!: %u.", command.Command);
			break;
		}
		break;
	}
	return -1;
}
