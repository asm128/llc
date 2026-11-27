#include "llc_connection.h"
#include "llc_apod_serialize.h"
#include "llc_chrono.h"
#include "llc_view_stream.h"
#include "llc_noise.h"
#include "llc_encoding.h"
#include "llc_deflate.h"

usns llc;

::llc::err_t			llc::connectionPushData				(::llc::SConnection & client, ::llc::SClientQueue & queue, vcu0_t data, b8_t bEncrypt, b8_t bCompress, u0_t retryCount) {
	if_true_fef(data.size() > ::llc::UDP_PAYLOAD_SIZE_LIMIT, "%s", "Invalid payload size.");

	::llc::pobj<::llc::SMessage>	message;
	message->Command.Command	= ::llc::ENDPOINT_COMMAND_PAYLOAD;
	message->Command.Type		= ::llc::ENDPOINT_COMMAND_TYPE_REQUEST;
	message->Command.Encrypted	= (u0_t)(bEncrypt ? 1 : 0);
	message->Command.Compressed	= (u0_t)(bCompress ? 1 : 0);
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
		lock_guard			lock								(queue.MutexSend);
		if_fail_fe(queue.Send.push_back(message));
	}
	return 0;
}

::llc::err_t			llc::connectionPayloadCollect		(::llc::SConnection & client, ::llc::apobj<::llc::SMessage> & receivedMessages)	{
	if(0 == client.KeyPing || client.State == ::llc::CONNECTION_STATE_HANDSHAKE)
		return 1;

	lock_guard			lockRecv							(client.Queue.MutexReceive);
	for(s2_t iMessage = 0; iMessage < (s2_t)client.Queue.Received.size(); ++iMessage) {
		cnst ::llc::pobj<::llc::SMessage>	& messageReceived		= client.Queue.Received[iMessage];
		cnst ::llc::SCommand				udpCommand				= messageReceived->Command;
		if( udpCommand.Type		!= ::llc::ENDPOINT_COMMAND_TYPE_REQUEST 
		 || udpCommand.Command	!= ::llc::ENDPOINT_COMMAND_PAYLOAD
		) 
			continue;
		if_fail_fe(receivedMessages.push_back(messageReceived));
		if_fail_fe(client.Queue.Received.remove_unordered(iMessage--));
	}
	return 0;
}

stxp	u3_t		hashFromTime						(u3_t currentTime)		nxpt	{ return ::llc::noise1DBase(currentTime & 0xFFFFFFFF) + (currentTime >> 16); }
stxp	u2_t		UDP_PAYLOAD_SENT_LIFETIME			= 1000000; // microseconds

::llc::err_t			payloadQueueOptimize				(::llc::SConnection & client, ::llc::apobj<::llc::SMessage> & messageCacheSent, ::llc::apobj<::llc::SMessage> & payloadsToSend) {
	if(payloadsToSend.size() < 2)
		return 0;

	{
		::llc::apobj<::llc::SMessage>	optimizedPayloads		= {};
		::llc::apobj<::llc::SMessage>	payloadsToRemove		= {};
		::llc::au1_t						payloadSizes			= {};
		::llc::au0_t						serialized				= {};

		::llc::pobj<::llc::SMessage>		optimizedMsg			= {};
		optimizedMsg->Time				= ::llc::timeCurrentInUs();
		optimizedMsg->Command			= {};
		optimizedMsg->Command.Command	= ::llc::ENDPOINT_COMMAND_PAYLOAD;
		optimizedMsg->Command.Type		= ::llc::ENDPOINT_COMMAND_TYPE_REQUEST;
		optimizedMsg->Command.Packed	= 1;
		optimizedMsg->RetryCount		= 1;

		b8_t						keyPing								= false;
		for(u2_t iPayload = 0; iPayload < payloadsToSend.size(); ++iPayload) {
			::llc::pobj<::llc::SMessage>	currentPayloadMsg				= payloadsToSend[iPayload];
			cnst ::llc::vcu0_t				currentPayloadView				= currentPayloadMsg->Payload;
			if(0 == currentPayloadMsg.get_ref())
				continue;
			if((currentPayloadMsg->Command.Packed & 1) || currentPayloadMsg->Command.Type == ::llc::ENDPOINT_COMMAND_TYPE_RESPONSE || currentPayloadMsg->Command.Command != ::llc::ENDPOINT_COMMAND_PAYLOAD)
				continue;
			b8_c						sizeEnough						= (serialized.size() + sizeof(u1_t) + currentPayloadMsg->Payload.size()) < ::llc::UDP_PAYLOAD_SIZE_LIMIT;
			if(false == sizeEnough)
				continue;
			if(0 == currentPayloadView.size())
				continue;
			if(currentPayloadMsg->Time == client.KeyPing)
				keyPing						= true;
			if_fail_fe(payloadSizes.push_back((u1_t)currentPayloadView.size()));
			if_fail_fe(serialized.append(currentPayloadView));
			if_fail_fe(payloadsToRemove.push_back(currentPayloadMsg));
			if_fail_fe(messageCacheSent.push_back(currentPayloadMsg));
		}
		if_true_vif(0, 0 == payloadSizes.size(), "No packets to optimize. Total packets searched: %u.", payloadsToSend.size());
		info_printf("%u packets optimized.", payloadSizes.size());
		u2_c				sizeRequired						= serialized.size() + payloadSizes.byte_count() + sizeof(u1_t);	// payloads + sizes + packet count
		u1_t					packetCount							= (u1_t)payloadSizes.size();
		::llc::au0_t				& optimizedPayloadBuffer			= optimizedMsg->Payload;
		if_fail_fef(optimizedPayloadBuffer.resize(sizeRequired), "Out of memory? Size required: %u.", sizeRequired);
		optimizedPayloadBuffer.clear();
		if_fail_fe(savePOD(optimizedPayloadBuffer, packetCount));
		if_fail_fe(optimizedPayloadBuffer.append(payloadSizes.as<u0_c>()));
		if_fail_fe(optimizedPayloadBuffer.append(serialized));
		if_true_fef(sizeRequired != optimizedPayloadBuffer.size(), "Invalid sizes! Size required: %u. Actual size: %u.", sizeRequired, optimizedPayloadBuffer.size());
		optimizedMsg->Command.Compressed	= 1;
		optimizedMsg->Command.Encrypted		= 1;
		if_fail_fef(payloadsToSend.push_back(optimizedMsg), "Failed! Payload queue size: %u.", payloadsToSend.size());

		if(keyPing)
			client.KeyPing						= optimizedMsg->Time;

		for(s2_t iPayloadTR = 0; iPayloadTR < (s2_t)payloadsToRemove.size(); ++iPayloadTR)	// Done. Remove packed payloads from send queue.
			for(s2_t iPayload = 0; iPayload < (s2_t)payloadsToSend.size(); ++iPayload) {
				if(payloadsToRemove[iPayloadTR] == payloadsToSend[iPayload]) {
					payloadsToSend.remove_unordered(iPayload);
					--iPayload;
				}
			}
	}
	return 0;
}

::llc::err_t			payloadQueueSend					(::llc::SConnection & client, ::llc::apobj<::llc::SMessage> & messageCacheSent, ::llc::apobj<::llc::SMessage> & payloadsToSend, cnst ::llc::FConnectionSend & send) {
	::llc::au0_t				messageBytes;
	for(u2_t iPayload = 0; iPayload < payloadsToSend.size(); ++iPayload) {
		::llc::pobj<::llc::SMessage>	pMessageToSend						= payloadsToSend[iPayload];
		if(0 == pMessageToSend.get_ref())
			continue;
		::llc::SMessage				& messageToSend						= *pMessageToSend;
		::llc::SPayloadHeader		payloadHeader						= {};
		payloadHeader.Command				= messageToSend.Command;
		payloadHeader.Size					= messageToSend.Payload.size();
		if(messageToSend.Command.Type == ::llc::ENDPOINT_COMMAND_TYPE_RESPONSE) {
			payloadHeader.MessageId				= messageToSend.Time;
			if_fail_fef(send({(cnst u0_t*)&payloadHeader, sizeof(::llc::SPayloadHeader)}), "%s", "Error sending datagram.");	// Send data back
			if_fail_fe(messageCacheSent.push_back(pMessageToSend));
			info_printf("Sent response to message id: %llx", payloadHeader.MessageId);
		}
		else {
			if_true_fef(messageToSend.Payload.size() > ::llc::UDP_PAYLOAD_SIZE_LIMIT, "Maximum allowed payload size is only %u bytes.", ::llc::UDP_PAYLOAD_SIZE_LIMIT);
			::llc::view_stream<u0_t>				sendStream;
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

			if_fail_fe(messageBytes.resize((u2_t)sizeof(::llc::SPayloadHeader) + payloadHeader.Size));
			sendStream						= {messageBytes.begin(), messageBytes.size()};
			if_fail_fe(sendStream.write_pod(payloadHeader));

			if(0 != payloadHeader.Command.Encrypted) {
				info_printf("Original packet size: %u. Final size: %u.", messageToSend.Payload.size(), ardellEncoded.size());
				if_fail_fe(sendStream.write_pod(::llc::vcu0_t{ardellEncoded.begin(), ardellEncoded.size()}));
			}
			else {
				if(0 == payloadHeader.Command.Compressed) {
					if_fail_fe(sendStream.write_pod(::llc::vcu0_t{messageToSend.Payload.begin(), messageToSend.Payload.size()}));
				}
				else {
					info_printf("Original packet size: %u. Compressed size: %u.", messageToSend.Payload.size(), compressed.size());
					if_fail_fe(sendStream.write_pod(::llc::vcu0_t{compressed.begin(), compressed.size()}));
				}
			}

			if_fail_fef(send({sendStream.begin(), sendStream.CursorPosition}), "%s", "Error sending datagram.");	// Send data back
			verbose_printf("%u bytes sent.", sendStream.CursorPosition);
			{
				if_fail_fe(client.Queue.Sent.push_back(pMessageToSend));
			}
			if_fail_fe(messageCacheSent.push_back(pMessageToSend));
		}
	}
	return 0;
}

::llc::err_t			llc::connectionSendQueue			(::llc::SConnection & client, ::llc::apobj<::llc::SMessage> & messageCacheSent, ::llc::apobj<::llc::SMessage> & messageCacheSend, cnst ::llc::FConnectionSend & send)				{
	::llc::apobj<::llc::SMessage>	& queueToSend						= client.Queue.Send;

	lock_guard					lock								(client.Queue.MutexSend);
	for(s2_t iSent = 0; iSent < (s2_t)client.Queue.Sent.size(); ++iSent) {
		::llc::pobj<::llc::SMessage> messageSent = client.Queue.Sent[iSent];
		if(0 == messageSent.get_ref())
			continue;
		u3_t timeCurr = llc::timeCurrentInUs();
		cnst u3_t timeSent = messageSent->Time;
		cnst u3_t timeDiff = timeCurr - timeSent;
		if(timeDiff > ::UDP_PAYLOAD_SENT_LIFETIME) {
			client.Queue.Sent.remove_unordered(iSent);
			--iSent;
			if(messageSent->RetryCount) {
				--messageSent->RetryCount;
				timeCurr	+= ::UDP_PAYLOAD_SENT_LIFETIME;
				info_printf("A message sent didn't receive a confirmation on time: %llx (%llx). Setting current time: %llx, Retries left: %u.", messageSent->Time, ::hashFromTime(messageSent->Time), timeCurr, (u2_t)messageSent->RetryCount);
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
	::llc::apobj<::llc::SMessage>					& payloadsToSend						= messageCacheSend;
	::llc::clear(payloadsToSend, messageCacheSent);
	for(u2_t iSend = 0; iSend < queueToSend.size(); ++iSend) {
		::llc::pobj<::llc::SMessage>					 pMessageToSend						= queueToSend[iSend];
		if(0 == pMessageToSend.get_ref())
			continue;
		::llc::SMessage									& messageToSend						= *pMessageToSend;
//		s3_t															currentTime							= ::llc::timeCurrentInUs();
		if(messageToSend.Command.Command == ::llc::ENDPOINT_COMMAND_PAYLOAD)
			payloadsToSend.push_back(pMessageToSend);
		else {
			if_fail_fef(send({(cnst u0_t*)&messageToSend.Command, sizeof(::llc::SCommand)}), "%s", "Error sending datagram.");	// Send data back
			//pMessageToSend->Time										= currentTime;
			if_fail_fe(client.Queue.Sent.push_back(pMessageToSend));
			if_fail_fe(messageCacheSent.push_back(pMessageToSend));
		}
	}
	if_fail_fef(::payloadQueueOptimize(client, messageCacheSent, payloadsToSend), "%s", "Failed to optimize payload queue.");
	if_fail_fef(::payloadQueueSend(client, messageCacheSent, payloadsToSend, send), "%s", "Failed ");
	for(u2_t iSent = 0; iSent < messageCacheSent.size(); ++iSent)
		for(u2_t iSend = 0; iSend < queueToSend.size(); ++iSend)
			if(queueToSend[iSend] == messageCacheSent[iSent]) {
				queueToSend.remove(iSend);
				break;
			}
	return 0;
}

sttc	::llc::err_t	handleRequestDISCONNECT					(::llc::SCommand & command, ::llc::SConnection & client)												{
	if_true_vef(-1, 0 != command.Packed || 0 != command.Encrypted || 0 != command.Compressed || ::llc::CONNECTION_STATE_IDLE != client.State, "Invalid command or client state: 0x%X '%s'.", client.State, ::llc::get_value_label(client.State).begin());
	client.FirstPing				=
	client.KeyPing					= 0;
	client.LastPing					= llc::timeCurrentInUs();
	client.State					= ::llc::CONNECTION_STATE_DISCONNECTED;
	info_printf("%s", "Disconnected!");
	return 0;
}

sttc	::llc::err_t	handleRequestCONNECT						(::llc::SCommand & command, ::llc::SConnection & client)												{
	if_true_vef(-1, 1 != command.Packed || ::llc::CONNECTION_STATE_HANDSHAKE != client.State, "Invalid client state: 0x%X '%s'.", client.State, ::llc::get_value_label(client.State).begin());
	client.LastPing					= llc::timeCurrentInUs();
	{
		lock_guard					lock								(client.Queue.MutexSend);
		for(u2_t iSent = 0, countSent = client.Queue.Sent.size(); iSent < countSent; ++iSent)
			if(::llc::ENDPOINT_COMMAND_CONNECT == client.Queue.Sent[iSent]->Command.Command) {
				client.Queue.Sent.remove_unordered(iSent);
				break;
			}
	}

	client.State					= ::llc::CONNECTION_STATE_IDLE;
	info_printf("%s", "Connected!");
	return 0;
}

#if defined(LLC_WINDOWS)
tydf int socklen_t;
#endif

sttc	::llc::err_t	postConfirmationResponse			(::llc::SClientQueue & queue, cnst ::llc::SCommand command, cnst u3_t messageId)				{
	::llc::pobj<::llc::SMessage>		response							= {};
	response->Command				= command;
	response->Command.Type			= ::llc::ENDPOINT_COMMAND_TYPE_RESPONSE;
	response->Time					= messageId;
	{
		lock_guard					lock								(queue.MutexSend);
		if_fail_fef(queue.Send.push_back(response), "Out of memory? Queue size: %u.", queue.Send.size());
	}
	return 0;
}

sttc	::llc::err_t	handlePAYLOADResponse				(cnst ::llc::SPayloadHeader & header, ::llc::SConnection & client)		{
	// Remove from sent queue.
	lock_guard					lock								(client.Queue.MutexSend);
	info_printf("Received response to message id: %llx", header.MessageId);
	b8_t								bFound								= false;
	for(u2_t iSent = 0; iSent < client.Queue.Sent.size(); ++iSent) {
		::llc::SMessage					& messageSent						= *client.Queue.Sent[iSent];
		for(u2_t iHash = 0; iHash < messageSent.Hashes.size(); ++iHash) {
			info_printf("Checking hash for message: %llx.", messageSent.Hashes[iHash]);
			if(::hashFromTime(messageSent.Time) == header.MessageId) {
				info_printf("Removing message from confirmation queue: %llx. ", header.MessageId);
				client.Queue.Sent.remove_unordered(iSent);
				client.LastPing					= ::llc::timeCurrentInUs();
				bFound							= true;
				break;
			}
			u3_t							hashLocal							= messageSent.Hashes[iHash]; //::hashFromTime(messageSent.Time);
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

stxp	s3_t			advantage							= 1000000;

sttc	::llc::err_t	handlePAYLOADRequest				(cnst ::llc::SPayloadHeader & header, ::llc::SCommand & command, ::llc::SConnection & client, ::llc::au0_t & receiveBuffer)		{
	u3_t							estimatedTimeSent					= 0;
	if(client.KeyPing == 0) {
		for(s3_t iTime = 0, countLapse = 60000000; iTime < countLapse; ++iTime)
			if(::hashFromTime(client.FirstPing - advantage + iTime) == header.MessageId) {
				client.KeyPing					= client.FirstPing - advantage + iTime;
				estimatedTimeSent				= client.FirstPing - advantage + iTime;
				info_printf("Key ping detected in %u attempts: %llu. First ping: %llu. Difference: %llu.", iTime, client.KeyPing, client.FirstPing, client.KeyPing - client.FirstPing);
				break;
			}
		if_true_vef(-1, 0 == client.KeyPing, "%s", "Failed to determine encryption key!");
	}
	else { // Why the fuck is this commented???
		//cnst u3_t					nowInUs							= ::llc::timeCurrentInUs();
		//cnst u3_t					startTime						= nowInUs - 3000000;
		//for(u2_t iTime = 0, countLapse = 1000000; iTime < countLapse; ++iTime) {
		//	cnst u3_t						timeSent						= startTime + iTime;
		//	if(::hashFromTime(timeSent) == header.MessageId) {
		//		info_printf("Payload timestamp found in %u attempts: Latency: %llu.", iTime, nowInUs - timeSent);
		//		estimatedTimeSent				= timeSent;
		//		break;
		//	}
		//}
	}
	::llc::pobj<::llc::SMessage>		messageReceived						= {};
	messageReceived->Command		= header.Command;
	messageReceived->Time			= estimatedTimeSent; //header.MessageId;
	if(header.Size > 0) {
		cnst ::llc::vcu0_t					viewPayload							= {&receiveBuffer[sizeof(::llc::SPayloadHeader)], header.Size};
		if(0 != command.Compressed) {
			if(0 != command.Encrypted) {
				::llc::au0_t				inflated;
				::llc::au0_t				decoded;
				if_true_vwf(-1, ::llc::failed(::llc::ardellDecode(viewPayload, client.KeyPing & 0xFFFFFFFF, true, decoded)), "%s", "Failed to decrypt packet!");
				if_true_vwf(-1, ::llc::failed(::llc::arrayInflate(decoded, messageReceived->Payload)), "%s", "Failed to inflate packet!");
			}
			else {
				if_true_vwf(-1, ::llc::failed(::llc::arrayInflate(viewPayload, messageReceived->Payload)), "%s", "Failed to inflate packet!");
			}
		}
		else {
			if(0 != command.Encrypted) {
				messageReceived->Payload.clear();
				if_true_vwf(-1, ::llc::failed(::llc::ardellDecode(viewPayload, client.KeyPing & 0xFFFFFFFF, true, messageReceived->Payload)), "%s", "Failed to decrypt packet!");
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
	//if_true_vef(-1, header.Command.Payload > 3, "Unsupported payload format. Payload: %u.", (u2_t)header.Command.Payload);
	if(0 == (header.Command.Packed & 1)) {
		{
			lock_guard					lock								(client.Queue.MutexReceive);
			if_fail_fef(client.Queue.Received.push_back(messageReceived), "Out of memory? Queue size: %u.", client.Queue.Received.size());
		}
		::llc::pobj<::llc::SMessage>		response							= {};
		::postConfirmationResponse(client.Queue, header.Command, header.MessageId);
		//info_printf("Sent ack for message id: %llu.", header.MessageId);
	}
	else {
		// 1. Read packet count
		// 2. Read packet sizes (sizeof(u1_t) * packet count)
		// 3. Read payload contents
		::llc::view_stream<u0_t>			payload								= {messageReceived->Payload.begin(), messageReceived->Payload.size()};
		u1_t							packetCount							= 0;
		if_fail_fef(payload.read_pod(packetCount), "%s", "Invalid data.");
		if(0 == packetCount)
			return 0;
		if_true_vef(-1, (packetCount * sizeof(u1_t)) > (payload.size() - payload.CursorPosition), "%s", "Invalid packet data.");
		::llc::vcu1_t						sizes								= {(u1_t*)&payload[payload.CursorPosition], packetCount};
		payload.CursorPosition			+= packetCount * sizeof(u1_t);
		::llc::apobj<::llc::SMessage>	tempQueue;
		for(u2_t iSizes = 0; iSizes < sizes.size(); ++iSizes) {
			::llc::pobj<::llc::SMessage>	messageReceivedInt						= {};
			messageReceivedInt->Command				= header.Command;
			messageReceivedInt->Time				= header.MessageId;
			messageReceivedInt->Command.Packed		= 0;	// Clear payload mode bits
			messageReceivedInt->Command.Encrypted	= 0;
			messageReceivedInt->Command.Compressed	= 0;
			if_fail_fe(messageReceivedInt->Payload.resize(sizes[iSizes]));
			if_fail_fef(payload.read_pod(::llc::vu0_t{messageReceivedInt->Payload.begin(), messageReceivedInt->Payload.size()}), "Corrupt payload data. Sizes don't match. Payload size: %u.", payload.size());
			if_fail_fe(tempQueue.push_back(messageReceivedInt));
		}
		{
			lock_guard					lock								(client.Queue.MutexReceive);
			if_fail_fef(client.Queue.Received.append(tempQueue), "Out of memory? Queue size: %u.", client.Queue.Received.size());
		}
		::postConfirmationResponse(client.Queue, header.Command, header.MessageId);
	}
	return 0;
}

sttc	::llc::err_t	handlePAYLOAD						(::llc::SCommand & command, ::llc::SConnection & client, cnst ::llc::FConnectionReceive & receive, ::llc::au0_t & receiveBuffer)		{
	if(::llc::CONNECTION_STATE_IDLE != client.State)
		return 1;
	cnst ::llc::err_t received = receive(receiveBuffer);
	if(received)
		return received;
	::llc::SPayloadHeader			header								= {};
	memcpy(&header, receiveBuffer.begin(), sizeof(::llc::SPayloadHeader));
	switch(command.Type) {
	default										: return -1;
	case ::llc::ENDPOINT_COMMAND_TYPE_REQUEST	: return ::handlePAYLOADRequest (header, command, client, receiveBuffer); 
	case ::llc::ENDPOINT_COMMAND_TYPE_RESPONSE	: return ::handlePAYLOADResponse(header, client); 
	}
}

::llc::err_t			llc::connectionHandleCommand		(::llc::SConnection & client, ::llc::SCommand & command, cnst ::llc::FConnectionReceive & receive, ::llc::au0_t & receiveBuffer)		{
	::llc::vcst_t							labelCommand						= ::llc::get_value_label(command.Command);
	::llc::vcst_t							labelType							= ::llc::get_value_label(command.Type	);
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
		case ::llc::ENDPOINT_COMMAND_PAYLOAD	: return ::handlePAYLOAD			(command, client, receive, receiveBuffer);
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
		case ::llc::ENDPOINT_COMMAND_PAYLOAD	: return ::handlePAYLOAD(command, client, receive, receiveBuffer);
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
