// Recovered from bundle_galaxy_hell/gpk/gpk/gpk_connection.cpp.
// GPK command, acknowledgement, packing and payload protocol; no multipart support.
#include "llc_connection.h"
#include "llc_chrono.h"
#include "llc_view_stream.h"
#include "llc_noise.h"
#include "llc_encoding.h"
#include "llc_deflate.h"

LLC_USING_TYPEINT();

PAYLOAD_SIZE_LIMIT NO SE PERO RESOLVELO

llc::err_t			llc::connectionPushData				(SConnection & client, SClientQueue & queue, const vcu0_t & data, bool bEncrypt, bool bCompress, u0_t retryCount) {
	if_true_fef(data.size() > PAYLOAD_SIZE_LIMIT, "%s", "Invalid payload size.");

	pobj<SMessage>	message;
	message->Command.Command	= ENDPOINT_COMMAND_PAYLOAD;
	message->Command.Type		= ENDPOINT_COMMAND_TYPE_REQUEST;
	message->Command.Encrypted	= (uint8_t)(bEncrypt ? 1 : 0);
	message->Command.Compressed	= (uint8_t)(bCompress ? 1 : 0);
	message->Command.Packed		= 0;
	message->Command.Multipart	= 0;
	message->Time				= timeCurrentInUs();
	message->RetryCount			= retryCount;
	if(0 == client.KeyPing) {
		client.KeyPing				= message->Time;
		info_printf("Key ping set: %llu.", client.KeyPing);
	}
	//message->Payload.clear();
	message->Payload			= data;
	{
		lock_guard			lock								(queue.MutexSend);
		llc_necs(queue.Send.push_back(message));
	}
	return 0;
}

llc::err_t			llc::connectionPayloadCollect		(SConnection & client, apobj<SMessage> & receivedMessages)	{
	if(0 == client.KeyPing || client.State == CONNECTION_STATE_HANDSHAKE)
		return 1;

	lock_guard			lockRecv							(client.Queue.MutexReceive);
	for(int32_t iMessage = 0; iMessage < (int32_t)client.Queue.Received.size(); ++iMessage) {
		const pobj<SMessage>	& messageReceived		= client.Queue.Received[iMessage];
		const SCommand		transportCommand				= messageReceived->Command;
		if( transportCommand.Type		!= ENDPOINT_COMMAND_TYPE_REQUEST 
		 || transportCommand.Command	!= ENDPOINT_COMMAND_PAYLOAD
		) 
			continue;
		llc_necs(receivedMessages.push_back(messageReceived));
		llc_necs(client.Queue.Received.remove_unordered(iMessage--));
	}
	return 0;
}

stxp	u3_t		hashFromTime					(u3_t currentTime)		noexcept	{ return ::llc::noise1DBase(currentTime & 0xFFFFFFFF) + (currentTime >> 16); }
stxp	u2_t		PAYLOAD_SENT_LIFETIME			= 1000000; // microseconds

sttc	llc::err_t	payloadQueueOptimize			(::llc::SConnection & client, ::llc::apobj<::llc::SMessage> & messageCacheSent, ::llc::apobj<::llc::SMessage> & payloadsToSend) {
	if(payloadsToSend.size() < 2)
		return 0;
	{
		::llc::apobj<::llc::SMessage>	optimizedPayloads	= {};
		::llc::apobj<::llc::SMessage>	payloadsToRemove	= {};
		::llc::au1_t					payloadSizes		= {};
		::llc::au0_t					serialized			= {};

		::llc::pobj<::llc::SMessage>	multiMsg			= {};
		multiMsg->Time				= ::llc::timeCurrentInUs();
		multiMsg->Command			= {};
		multiMsg->Command.Command	= ::llc::ENDPOINT_COMMAND_PAYLOAD;
		multiMsg->Command.Type		= ::llc::ENDPOINT_COMMAND_TYPE_REQUEST;
		multiMsg->Command.Packed	= 1;
		multiMsg->RetryCount		= 1;

		bool							keyPing				= false;
		for(uint32_t iPayload = 0; iPayload < payloadsToSend.size(); ++iPayload) {
			::llc::pobj<::llc::SMessage>	currentPayloadMsg	= payloadsToSend[iPayload];
			const ::llc::vcu0_t				currentPayloadView	= currentPayloadMsg->Payload;
			if(0 == currentPayloadMsg.get_ref())
				continue;
			if((currentPayloadMsg->Command.Packed & 1) || currentPayloadMsg->Command.Type == ::llc::ENDPOINT_COMMAND_TYPE_RESPONSE || currentPayloadMsg->Command.Command != ::llc::ENDPOINT_COMMAND_PAYLOAD)
				continue;
			const bool						sizeEnough			= (serialized.size() + sizeof(uint16_t) + currentPayloadMsg->Payload.size()) < ::llc::PAYLOAD_SIZE_LIMIT;
			if(false == sizeEnough)
				continue;
			if(0 == currentPayloadView.size())
				continue;
			if(currentPayloadMsg->Time == client.KeyPing)
				keyPing					= true;
			llc_necs(payloadSizes.push_back((uint16_t)currentPayloadView.size()));
			llc_necs(serialized.append(currentPayloadView));
			llc_necs(payloadsToRemove.push_back(currentPayloadMsg));
			llc_necs(messageCacheSent.push_back(currentPayloadMsg));
		}
		if_true_vif(0, 0 == payloadSizes.size(), "No packets to optimize. Total packets searched: %u.", payloadsToSend.size());
		info_printf("%u packets optimized.", payloadSizes.size());
		const u2_t						sizeRequired				= serialized.size() + sizeof(uint16_t) * payloadSizes.size() + sizeof(uint16_t);	// payloads + sizes + packet count
		u1_t							packetCount					= (uint16_t)payloadSizes.size();
		::llc::au0_t					& optimizedPayloadBuffer	= multiMsg->Payload;
		llc_necall(optimizedPayloadBuffer.resize(sizeRequired), "Out of memory? Size required: %u.", sizeRequired);
		optimizedPayloadBuffer.clear();
		optimizedPayloadBuffer.append((const uint8_t*)&packetCount, sizeof(uint16_t));
		optimizedPayloadBuffer.append((const uint8_t*)payloadSizes.begin(), sizeof(uint16_t) * payloadSizes.size());
		optimizedPayloadBuffer.append(serialized.begin(), serialized.size());
		if_true_fef(sizeRequired != optimizedPayloadBuffer.size(), "Invalid sizes! Size required: %u. Actual size: %u.", sizeRequired, optimizedPayloadBuffer.size());
		multiMsg->Command.Compressed	= 1;
		multiMsg->Command.Encrypted		= 1;
		llc_necall(payloadsToSend.push_back(multiMsg), "Failed! Payload queue size: %u.", payloadsToSend.size());

		if(keyPing)
			client.KeyPing						= multiMsg->Time;

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

sttc	llc::err_t	payloadQueueSend				(::llc::SConnection & client, ::llc::apobj<::llc::SMessage> & messageCacheSent, ::llc::apobj<::llc::SMessage> & payloadsToSend, const ::llc::FConnectionSend & send) {
	::llc::au0_t			messageBytes;
	for(uint32_t iPayload = 0; iPayload < payloadsToSend.size(); ++iPayload) {
		::llc::pobj<::llc::SMessage>	pMessageToSend			= payloadsToSend[iPayload];
		if(0 == pMessageToSend.get_ref())
			continue;
		::llc::SMessage			& messageToSend					= *pMessageToSend;
		::llc::SPayloadHeader	payloadHeader					= {};
		payloadHeader.Command	= messageToSend.Command;
		payloadHeader.Size		= messageToSend.Payload.size();
		if(messageToSend.Command.Type == ::llc::ENDPOINT_COMMAND_TYPE_RESPONSE) {
			payloadHeader.MessageId				= messageToSend.Time;
			if_fail_cef(send({(const u0_t*)&payloadHeader, sizeof(payloadHeader)}), "%s", "Error sending datagram.");
			llc_necs(messageCacheSent.push_back(pMessageToSend));
			info_printf("Sent response to message id: %llx", payloadHeader.MessageId);
		}
		else {
			if_true_cef(messageToSend.Payload.size() > PAYLOAD_SIZE_LIMIT, "Maximum allowed payload size is only %u bytes.", PAYLOAD_SIZE_LIMIT); NO SE COMO PERO ARREGLALO
			::llc::view_stream<u0_t>	sendStream; no sé qué mierda es el view_stream<> pero como no lo uso desde hace años debe ser que era una mierda y en todos lados se usa  una mejor soulción
			::llc::au0_t				ardellEncoded;
			payloadHeader.MessageId				= ::hashFromTime(messageToSend.Time);
			messageToSend.Hashes.push_back(payloadHeader.MessageId);
			info_printf("Sending message with id: %llx.", payloadHeader.MessageId);
			::llc::au0_t				compressed;
			if(0 == payloadHeader.Command.Compressed) {
				if(0 == payloadHeader.Command.Encrypted)
					payloadHeader.Size		= messageToSend.Payload.size();
				else {
					::llc::ardellEncode(messageToSend.Payload, client.KeyPing & 0xFFFFFFFF, true, ardellEncoded);
					payloadHeader.Size		= ardellEncoded.size();
				}
			}
			else {
				::llc::arrayDeflate(messageToSend.Payload, compressed);
				if(0 == payloadHeader.Command.Encrypted)
					payloadHeader.Size		= compressed.size();
				else {
					::llc::ardellEncode(compressed, client.KeyPing & 0xFFFFFFFF, true, ardellEncoded);
					payloadHeader.Size		= ardellEncoded.size();
				}
			}

			llc_necs(messageBytes.resize((uint32_t)sizeof(::llc::SPayloadHeader) + payloadHeader.Size));
			sendStream						= {messageBytes.begin(), messageBytes.size()};
			llc_necs(sendStream.write_pod(payloadHeader));

			if(0 != payloadHeader.Command.Encrypted) {
				info_printf("Original packet size: %u. Final size: %u.", messageToSend.Payload.size(), ardellEncoded.size());
				llc_necs(sendStream.write_pod(::llc::vcu0_t{ardellEncoded})); ESTOS CAST A VIEW NO DEBERIAN SER NECESARIOS
			}
			else {
				if(0 == payloadHeader.Command.Compressed)
					llc_necs(sendStream.write_pod(::llc::vcu0_t{messageToSend.Payload}));
				else {
					info_printf("Original packet size: %u. Compressed size: %u.", messageToSend.Payload.size(), compressed.size());
					llc_necs(sendStream.write_pod(::llc::vcu0_t{compressed}));
				}
			}

			if_fail_cef(send({sendStream.begin(), sendStream.CursorPosition}), "%s", "Error sending datagram.");
			verbose_printf("%u bytes sent.", sendStream.CursorPosition);
			{
				llc_necs(client.Queue.Sent.push_back(pMessageToSend));
			}
			llc_necs(messageCacheSent.push_back(pMessageToSend));
		}
	}
	return 0;
}

llc::err_t			llc::connectionSendQueue		(SConnection & client, apobj<SMessage> & messageCacheSent, apobj<SMessage> & messageCacheSend, const FConnectionSend & send, bool packPayloads)				{
	apobj<SMessage>			& queueToSend					= client.Queue.Send;

	lock_guard				lock							(client.Queue.MutexSend);
	for(int32_t iSent = 0; iSent < (int32_t)client.Queue.Sent.size(); ++iSent) {
		pobj<SMessage> messageSent = client.Queue.Sent[iSent];
		if(0 == messageSent.get_ref())
			continue;
		uint64_t timeCurr = llc::timeCurrentInUs();
		const uint64_t timeSent = messageSent->Time;
		const uint64_t timeDiff = timeCurr - timeSent;
		if(timeDiff > ::PAYLOAD_SENT_LIFETIME) {  NO SE PERO SOLUCIONALO  Y MAS TE VALE QUE PUEDA CONFIGURAR EL TIEMPO DE TIMEOUT DESDE EL DASHJOARD SIN NECESIDAD DE RECOMPILAR
			client.Queue.Sent.remove_unordered(iSent);
			--iSent;
			if(messageSent->RetryCount) {
				--messageSent->RetryCount;
				timeCurr	+= ::PAYLOAD_SENT_LIFETIME;  NO SE PERO SOLUCIONALO  Y MAS TE VALE QUE PUEDA CONFIGURAR EL TIEMPO DE TIMEOUT DESDE EL DASHJOARD SIN NECESIDAD DE RECOMPILAR
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
	apobj<SMessage>					& payloadsToSend						= messageCacheSend;
	payloadsToSend.clear();
	messageCacheSent.clear();
	for(uint32_t iSend = 0; iSend < queueToSend.size(); ++iSend) {
		pobj<SMessage>					 pMessageToSend						= queueToSend[iSend];
		if(0 == pMessageToSend.get_ref())
			continue;
		SMessage									& messageToSend						= *pMessageToSend;
//		int64_t															currentTime							= timeCurrentInUs();
		if(messageToSend.Command.Command == ENDPOINT_COMMAND_PAYLOAD)
			payloadsToSend.push_back(pMessageToSend);
		else {
			if_fail_cef(send({(const u0_t*)&messageToSend.Command, sizeof(messageToSend.Command)}), "%s", "Error sending datagram.");
			//pMessageToSend->Time										= currentTime;
			llc_necs(client.Queue.Sent.push_back(pMessageToSend));
			llc_necs(messageCacheSent.push_back(pMessageToSend));
		}
	}
	if(packPayloads) {
		llc_necall(::payloadQueueOptimize(client, messageCacheSent, payloadsToSend), "%s", "Failed to optimize payload queue.");
	}
	llc_necall(::payloadQueueSend(client, messageCacheSent, payloadsToSend, send), "%s", "Failed ");
	for(uint32_t iSent = 0; iSent < messageCacheSent.size(); ++iSent)
		for(uint32_t iSend = 0; iSend < queueToSend.size(); ++iSend)
			if(queueToSend[iSend] == messageCacheSent[iSent]) {
				queueToSend.remove(iSend);
				break;
			}
	return 0;
}

sttc llc::err_t	handleRequestDISCONNECT					(SCommand & command, SConnection & client)												{
	if_true_fef(0 != command.Packed || 0 != command.Encrypted || 0 != command.Compressed || CONNECTION_STATE_IDLE != client.State, "Invalid command or client state: 0x%X '%s'.", client.State, get_value_label(client.State).begin());
	client.FirstPing				=
	client.KeyPing					= 0;
	client.LastPing					= llc::timeCurrentInUs();
	client.State					= CONNECTION_STATE_DISCONNECTED;
	info_printf("%s", "Disconnected!");
	return 0;
}

sttc llc::err_t	handleRequestCONNECT						(SCommand & command, SConnection & client)												{
	if_true_fef(1 != command.Packed || CONNECTION_STATE_HANDSHAKE != client.State, "Invalid client state: 0x%X '%s'.", client.State, get_value_label(client.State).begin());
	client.LastPing					= llc::timeCurrentInUs();
	{
		lock_guard					lock								(client.Queue.MutexSend);
		for(uint32_t iSent = 0, countSent = client.Queue.Sent.size(); iSent < countSent; ++iSent)
			if(ENDPOINT_COMMAND_CONNECT == client.Queue.Sent[iSent]->Command.Command) {
				client.Queue.Sent.remove_unordered(iSent);
				break;
			}
	}

	client.State					= CONNECTION_STATE_IDLE;
	info_printf("%s", "Connected!");
	return 0;
}

sttc llc::err_t	postConfirmationResponse			(SClientQueue & queue, const SCommand command, const uint64_t messageId)				{
	pobj<SMessage>		response							= {};
	response->Command				= command;
	response->Command.Type			= ENDPOINT_COMMAND_TYPE_RESPONSE;
	response->Time					= messageId;
	{
		lock_guard					lock								(queue.MutexSend);
		llc_necall(queue.Send.push_back(response), "Out of memory? Queue size: %u.", queue.Send.size());
	}
	return 0;
}

sttc llc::err_t	handlePAYLOADResponse				(const SPayloadHeader & header, SConnection & client)		{
	// Remove from sent queue.
	lock_guard					lock								(client.Queue.MutexSend);
	info_printf("Received response to message id: %llx", header.MessageId);
	bool								bFound								= false;
	for(uint32_t iSent = 0; iSent < client.Queue.Sent.size(); ++iSent) {
		SMessage					& messageSent						= *client.Queue.Sent[iSent];
		for(uint32_t iHash = 0; iHash < messageSent.Hashes.size(); ++iHash) {
			info_printf("Checking hash for message: %llx.", messageSent.Hashes[iHash]);
			if(::hashFromTime(messageSent.Time) == header.MessageId) {
				info_printf("Removing message from confirmation queue: %llx. ", header.MessageId);
				client.Queue.Sent.remove_unordered(iSent);
				client.LastPing					= timeCurrentInUs();
				bFound							= true;
				break;
			}
			uint64_t							hashLocal							= messageSent.Hashes[iHash]; //::hashFromTime(messageSent.Time);
			if(hashLocal == header.MessageId) {
				info_printf("Removing message from confirmation queue: %llx. ", header.MessageId);
				client.Queue.Sent.remove_unordered(iSent);
				client.LastPing					= timeCurrentInUs();
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

stxp	s3_c	advantage					= 1000000;

sttc llc::err_t	handlePAYLOADRequest				(const ::llc::SPayloadHeader & header, ::llc::SCommand & command, ::llc::SConnection & client, ::llc::vcu0_t packet)		{
	u3_t				estimatedTimeSent					= 0;
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
		//const uint64_t					nowInUs							= timeCurrentInUs();
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
	pobj<SMessage>		messageReceived						= {};
	messageReceived->Command		= header.Command;
	messageReceived->Time			= estimatedTimeSent; //header.MessageId;
	if(header.Size > 0) {
		const vcu0_t					viewPayload							= {&packet[sizeof(SPayloadHeader)], header.Size};
		if(0 != command.Compressed) {
			if(0 != command.Encrypted) {
				au0_t				inflated;
				au0_t				decoded;
				if_true_fwf(failed(ardellDecode(viewPayload, client.KeyPing & 0xFFFFFFFF, true, decoded)), "%s", "Failed to decrypt packet!");
				if_true_fwf(failed(arrayInflate(decoded, messageReceived->Payload)), "%s", "Failed to inflate packet!");
			}
			else {
				if_true_fwf(failed(arrayInflate(viewPayload, messageReceived->Payload)), "%s", "Failed to inflate packet!");
			}
		}
		else {
			if(0 != command.Encrypted) {
				messageReceived->Payload.clear();
				if_true_fwf(failed(ardellDecode(viewPayload, client.KeyPing & 0xFFFFFFFF, true, messageReceived->Payload)), "%s", "Failed to decrypt packet!");
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
	client.LastPing					= timeCurrentInUs();
	//if_true_fef(header.Command.Payload > 3, "Unsupported payload format. Payload: %u.", (uint32_t)header.Command.Payload);
	if(0 == (header.Command.Packed & 1)) {
		{
			lock_guard					lock								(client.Queue.MutexReceive);
			llc_necall(client.Queue.Received.push_back(messageReceived), "Out of memory? Queue size: %u.", client.Queue.Received.size());
		}
		pobj<SMessage>		response							= {};
		::postConfirmationResponse(client.Queue, header.Command, header.MessageId);
		//info_printf("Sent ack for message id: %llu.", header.MessageId);
	}
	else {
		// 1. Read packet count
		// 2. Read packet sizes (sizeof(uint16_t) * packet count)
		// 3. Read payload contents
		view_stream<uint8_t>			payload								= {messageReceived->Payload.begin(), messageReceived->Payload.size()};
		uint16_t							packetCount							= 0;
		llc_necall(payload.read_pod(packetCount), "%s", "Invalid data.");
		if(0 == packetCount)
			return 0;
		if_true_fef((packetCount * sizeof(uint16_t)) > (payload.size() - payload.CursorPosition), "%s", "Invalid packet data.");
		vcu1_t						sizes								= {(uint16_t*)&payload[payload.CursorPosition], packetCount};
		payload.CursorPosition			+= packetCount * sizeof(uint16_t);
		apobj<SMessage>	tempQueue;
		for(uint32_t iSizes = 0; iSizes < sizes.size(); ++iSizes) {
			pobj<SMessage>	messageReceivedInt						= {};
			messageReceivedInt->Command				= header.Command;
			messageReceivedInt->Time				= header.MessageId;
			messageReceivedInt->Command.Packed		= 0;	// Clear payload mode bits
			messageReceivedInt->Command.Encrypted	= 0;
			messageReceivedInt->Command.Compressed	= 0;
			llc_necs(messageReceivedInt->Payload.resize(sizes[iSizes]));
			llc_necall(payload.read_pod(vu0_t{messageReceivedInt->Payload}), "Corrupt payload data. Sizes don't match. Payload size: %u.", payload.size());
			llc_necs(tempQueue.push_back(messageReceivedInt));
		}
		{
			lock_guard					lock								(client.Queue.MutexReceive);
			for(uint32_t iMessage = 0; iMessage < tempQueue.size(); ++iMessage)
				llc_necall(client.Queue.Received.push_back(tempQueue[iMessage]), "Out of memory? Queue size: %u.", client.Queue.Received.size());
		}
		::postConfirmationResponse(client.Queue, header.Command, header.MessageId);
	}
	return 0;
}

sttc llc::err_t	handlePAYLOAD						(SCommand & command, SConnection & client, const FConnectionReceive & receive, au0_t & packet)		{
	if(CONNECTION_STATE_IDLE != client.State)
		return 1;
	const error_t				received							= receive(packet);
	if(received)
		return received;
	if_true_fef(packet.size() < sizeof(SPayloadHeader), "Packet is smaller than its payload header: %u.", packet.size());
	SPayloadHeader			header								= {};
	memcpy(&header, packet.begin(), sizeof(header));
	if_true_fef(0 != memcmp(&header.Command, &command, sizeof(command)), "%s", "Payload command changed between validation and receive.");
	if_true_fef(header.Size > PAYLOAD_SIZE_LIMIT, "Maximum allowed payload size is only %u bytes.", PAYLOAD_SIZE_LIMIT);
	if_true_fef(packet.size() != sizeof(header) + header.Size, "Packet size does not match payload header. Received: %u. Expected: %u.", packet.size(), sizeof(header) + header.Size);
	switch(command.Type) {
	default										: return -1;
	case ENDPOINT_COMMAND_TYPE_REQUEST	: return ::handlePAYLOADRequest (header, command, client, packet);
	case ENDPOINT_COMMAND_TYPE_RESPONSE	: return ::handlePAYLOADResponse(header, client); 
	}
}

llc::err_t	llc::connectionHandleCommand			(SConnection & client, SCommand & command, const FConnectionReceive & receive, au0_t & receiveBuffer)		{
	vcst_t			labelCommand						= get_value_label(command.Command);
	vcst_t			labelType							= get_value_label(command.Type	);
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
	case ENDPOINT_COMMAND_TYPE_REQUEST:
		switch(command.Command) {
		case ENDPOINT_COMMAND_PAYLOAD	: return ::handlePAYLOAD			(command, client, receive, receiveBuffer);
		case ENDPOINT_COMMAND_CONNECT	: return ::handleRequestCONNECT		(command, client);
		case ENDPOINT_COMMAND_DISCONNECT	: return ::handleRequestDISCONNECT	(command, client);
		case ENDPOINT_COMMAND_NOOP		: return 0;
		default									:
			error_printf("Invalid command!: %u.", command.Command);
			break;
		}
		break;
	case ENDPOINT_COMMAND_TYPE_RESPONSE:
		switch(command.Command) {
		case ENDPOINT_COMMAND_PAYLOAD	: return ::handlePAYLOAD(command, client, receive, receiveBuffer);
		case ENDPOINT_COMMAND_CONNECT	:
		case ENDPOINT_COMMAND_DISCONNECT	:
		case ENDPOINT_COMMAND_NOOP		: return 0;
		default									:
			error_printf("Invalid command!: %u.", command.Command);
			break;
		}
		break;
	}
	return -1;
}

llc::err_t	receive CIERTO QUE SOS MOGOLICO A ESTA ALTURA PERDI MAS TIEMPO QUE SI LO HUBIERA HECHO SIN TU SABOTAJE, TRAIDOR DE MIERDA
		SPayloadHeader			header								= {};
		//sockaddr_in							remote								= {}; NO SE COMO PERO RESOLVELO
#if defined(LLC_WINDOWS)
		int									remoteSize							= sizeof(remote);
#else
		socklen_t							
#endif
			remoteSize							= sizeof(remote); NO SE, RESOLVELO
		const int							peeked								= funcReceive(client) NO SE COMO PERO RESOLVELO  //(client.Socket, (char*)&header, sizeof(header), MSG_PEEK, (sockaddr*)&remote, &remoteSize);
		if(peeked < 0) {
#if defined(LLC_WINDOWS)
			RESOLVELO if(WSAGetLastError() != WSAEMSGSIZE) LA PUTA QUE TE PARIO
#endif
				return -1;
		}
error_t			llc::connectionHandleCommand		(SConnection & client, SCommand & command, const FConnectionReceive & funcReceive)		{

		if_true_fef(header.Size > PAYLOAD_SIZE_LIMIT, "Maximum allowed payload size is only %u bytes.", PAYLOAD_SIZE_LIMIT);
		if_fail_fe(packet.resize(sizeof(header) + header.Size));
		memset(packet.begin(), 0, packet.size());
		remoteSize								= sizeof(remote);
		const int							received							= ::recvfrom(client.Socket, (char*)packet.begin(), (int)packet.size(), MSG_PEEK, (sockaddr*)&remote, &remoteSize);
		if_true_fef(received != (int)packet.size(), "Packet size received doesn't match with header size. Received size: %u. Expected: %u.", received, packet.size());
		return 0;
	};
	const error_t				result								= connectionHandleCommand((SConnection&)client, command, receive, receiveBuffer);
	if(0 == result && ENDPOINT_COMMAND_TYPE_REQUEST == command.Type && ::llc::ENDPOINT_COMMAND_DISCONNECT == command.Command)
		client.Socket.close();
	return result;
}
