#include "llc_connection.h"
#include "llc_chrono.h"
#include "llc_deflate.h"
#include "llc_encoding.h"
#include "llc_noise.h"

#include <cstring>

LLC_USING_TYPEINT();

static constexpr uint64_t hashFromTime(uint64_t currentTime) noexcept {
	return ::llc::noise1DBase(currentTime & 0xFFFFFFFF) + (currentTime >> 16);
}
static constexpr uint32_t UDP_PAYLOAD_SENT_LIFETIME = 1000000;

llc::err_t llc::connectionPushData(SConnection & client, SClientQueue & queue, const vcu0_t & data, bool bEncrypt, bool bCompress, uint8_t retryCount) {
	if_true_fef(data.size() > UDP_PAYLOAD_SIZE_LIMIT, "%s", "Invalid payload size.");

	pobj<SMessage> message;
	message->Command.Command	= ENDPOINT_COMMAND_PAYLOAD;
	message->Command.Type		= ENDPOINT_COMMAND_TYPE_REQUEST;
	message->Command.Encrypted	= (uint8_t)(bEncrypt ? 1 : 0);
	message->Command.Compressed	= (uint8_t)(bCompress ? 1 : 0);
	message->Command.Packed		= 0;
	message->Command.Multipart	= 0;
	message->Time				= timeCurrentInUs();
	message->RetryCount			= retryCount;
	if(0 == client.KeyPing) {
		client.KeyPing = message->Time;
		info_printf("Key ping set: %llu.", client.KeyPing);
	}
	message->Payload = data;
	{
		lock_guard lock(queue.MutexSend);
		llc_necs(queue.Send.push_back(message));
	}
	return 0;
}

llc::err_t llc::connectionPayloadCollect(SConnection & client, apobj<SMessage> & receivedMessages) {
	if(0 == client.KeyPing || client.State == CONNECTION_STATE_HANDSHAKE)
		return 1;

	lock_guard lockReceive(client.Queue.MutexReceive);
	for(int32_t iMessage = 0; iMessage < (int32_t)client.Queue.Received.size(); ++iMessage) {
		const pobj<SMessage> & messageReceived = client.Queue.Received[iMessage];
		const SCommand command = messageReceived->Command;
		if(command.Type != ENDPOINT_COMMAND_TYPE_REQUEST || command.Command != ENDPOINT_COMMAND_PAYLOAD)
			continue;
		llc_necs(receivedMessages.push_back(messageReceived));
		llc_necs(client.Queue.Received.remove_unordered(iMessage--));
	}
	return 0;
}

static llc::err_t payloadQueueOptimize(::llc::SConnection & client, ::llc::apobj<::llc::SMessage> & messageCacheSent, ::llc::apobj<::llc::SMessage> & payloadsToSend) {
	if(payloadsToSend.size() < 2)
		return 0;

	::llc::apobj<::llc::SMessage> optimizedPayloads = {};
	::llc::apobj<::llc::SMessage> payloadsToRemove = {};
	::llc::au1_t payloadSizes = {};
	::llc::au0_t serialized = {};

	::llc::pobj<::llc::SMessage> optimizedMessage = {};
	optimizedMessage->Time				= ::llc::timeCurrentInUs();
	optimizedMessage->Command			= {};
	optimizedMessage->Command.Command	= ::llc::ENDPOINT_COMMAND_PAYLOAD;
	optimizedMessage->Command.Type		= ::llc::ENDPOINT_COMMAND_TYPE_REQUEST;
	optimizedMessage->Command.Packed	= 1;
	optimizedMessage->RetryCount			= 1;

	bool keyPing = false;
	for(uint32_t iPayload = 0; iPayload < payloadsToSend.size(); ++iPayload) {
		::llc::pobj<::llc::SMessage> currentPayloadMessage = payloadsToSend[iPayload];
		const ::llc::vcu0_t currentPayload = currentPayloadMessage->Payload;
		if(0 == currentPayloadMessage.get_ref())
			continue;
		if((currentPayloadMessage->Command.Packed & 1) || currentPayloadMessage->Command.Type == ::llc::ENDPOINT_COMMAND_TYPE_RESPONSE || currentPayloadMessage->Command.Command != ::llc::ENDPOINT_COMMAND_PAYLOAD)
			continue;
		const bool sizeEnough = (serialized.size() + sizeof(uint16_t) + currentPayloadMessage->Payload.size()) < ::llc::UDP_PAYLOAD_SIZE_LIMIT;
		if(false == sizeEnough)
			continue;
		if(0 == currentPayload.size())
			continue;
		if(currentPayloadMessage->Time == client.KeyPing)
			keyPing = true;
		llc_necs(payloadSizes.push_back((uint16_t)currentPayload.size()));
		llc_necs(serialized.append(currentPayload));
		llc_necs(payloadsToRemove.push_back(currentPayloadMessage));
		llc_necs(messageCacheSent.push_back(currentPayloadMessage));
	}
	if(0 == payloadSizes.size()) {
		info_printf("No packets to optimize. Total packets searched: %u.", payloadsToSend.size());
		return 0;
	}
	info_printf("%u packets optimized.", payloadSizes.size());

	const uint32_t sizeRequired = serialized.size() + sizeof(uint16_t) * payloadSizes.size() + sizeof(uint16_t);
	uint16_t packetCount = (uint16_t)payloadSizes.size();
	::llc::au0_t & optimizedPayloadBuffer = optimizedMessage->Payload;
	llc_necall(optimizedPayloadBuffer.resize(sizeRequired), "Out of memory? Size required: %u.", sizeRequired);
	optimizedPayloadBuffer.clear();
	optimizedPayloadBuffer.append((const uint8_t*)&packetCount, sizeof(uint16_t));
	optimizedPayloadBuffer.append((const uint8_t*)payloadSizes.begin(), sizeof(uint16_t) * payloadSizes.size());
	optimizedPayloadBuffer.append(serialized.begin(), serialized.size());
	if_true_fef(sizeRequired != optimizedPayloadBuffer.size(), "Invalid sizes! Size required: %u. Actual size: %u.", sizeRequired, optimizedPayloadBuffer.size());
	optimizedMessage->Command.Compressed	= 1;
	optimizedMessage->Command.Encrypted	= 1;
	llc_necall(payloadsToSend.push_back(optimizedMessage), "Failed! Payload queue size: %u.", payloadsToSend.size());

	if(keyPing)
		client.KeyPing = optimizedMessage->Time;

	for(int32_t iRemove = 0; iRemove < (int32_t)payloadsToRemove.size(); ++iRemove)
		for(int32_t iPayload = 0; iPayload < (int32_t)payloadsToSend.size(); ++iPayload) {
			if(payloadsToRemove[iRemove] == payloadsToSend[iPayload]) {
				payloadsToSend.remove_unordered(iPayload);
				--iPayload;
			}
		}
	return 0;
}

static llc::err_t payloadQueueSend(::llc::SConnection & client, ::llc::apobj<::llc::SMessage> & messageCacheSent, ::llc::apobj<::llc::SMessage> & payloadsToSend, const ::llc::FConnectionSend & send) {
	::llc::au0_t frame;
	for(uint32_t iPayload = 0; iPayload < payloadsToSend.size(); ++iPayload) {
		::llc::pobj<::llc::SMessage> messageToSendPointer = payloadsToSend[iPayload];
		if(0 == messageToSendPointer.get_ref())
			continue;
		::llc::SMessage & messageToSend = *messageToSendPointer;
		::llc::SPayloadHeader header = {};
		header.Command = messageToSend.Command;
		header.Size = messageToSend.Payload.size();
		if(messageToSend.Command.Type == ::llc::ENDPOINT_COMMAND_TYPE_RESPONSE) {
			header.MessageId = messageToSend.Time;
			if_fail_cef(send({(const ::llc::u0_t*)&header, sizeof(header)}), "%s", "Error sending datagram.");
			llc_necs(messageCacheSent.push_back(messageToSendPointer));
			info_printf("Sent response to message id: %llx", header.MessageId);
		}
		else {
			if_true_cef(messageToSend.Payload.size() > ::llc::UDP_PAYLOAD_SIZE_LIMIT, "Maximum allowed payload size is only %u bytes.", ::llc::UDP_PAYLOAD_SIZE_LIMIT);
			::llc::au0_t encoded;
			header.MessageId = ::hashFromTime(messageToSend.Time);
			messageToSend.Hashes.push_back(header.MessageId);
			info_printf("Sending message with id: %llx.", header.MessageId);
			::llc::au0_t compressed;
			if(0 == header.Command.Compressed) {
				if(0 == header.Command.Encrypted)
					header.Size = messageToSend.Payload.size();
				else {
					::llc::ardellEncode(messageToSend.Payload, client.KeyPing & 0xFFFFFFFF, true, encoded);
					header.Size = encoded.size();
				}
			}
			else {
				::llc::arrayDeflate(messageToSend.Payload, compressed);
				if(0 == header.Command.Encrypted)
					header.Size = compressed.size();
				else {
					::llc::ardellEncode(compressed, client.KeyPing & 0xFFFFFFFF, true, encoded);
					header.Size = encoded.size();
				}
			}

			llc_necs(frame.resize((u2_t)sizeof(header) + header.Size));
			::memcpy(frame.begin(), &header, sizeof(header));
			if(0 != header.Command.Encrypted) {
				info_printf("Original packet size: %u. Final size: %u.", messageToSend.Payload.size(), encoded.size());
				if(encoded.size())
					::memcpy(frame.begin() + sizeof(header), encoded.begin(), encoded.size());
			}
			else {
				if(0 == header.Command.Compressed) {
					if(messageToSend.Payload.size())
						::memcpy(frame.begin() + sizeof(header), messageToSend.Payload.begin(), messageToSend.Payload.size());
				}
				else {
					info_printf("Original packet size: %u. Compressed size: %u.", messageToSend.Payload.size(), compressed.size());
					if(compressed.size())
						::memcpy(frame.begin() + sizeof(header), compressed.begin(), compressed.size());
				}
			}

			if_fail_cef(send({frame.begin(), frame.size()}), "%s", "Error sending datagram.");
			verbose_printf("%u bytes sent.", frame.size());
			llc_necs(client.Queue.Sent.push_back(messageToSendPointer));
			llc_necs(messageCacheSent.push_back(messageToSendPointer));
		}
	}
	return 0;
}

llc::err_t llc::connectionSendQueue(SConnection & client, apobj<SMessage> & messageCacheSent, apobj<SMessage> & messageCacheSend, const FConnectionSend & send) {
	apobj<SMessage> & queueToSend = client.Queue.Send;
	lock_guard lock(client.Queue.MutexSend);
	for(int32_t iSent = 0; iSent < (int32_t)client.Queue.Sent.size(); ++iSent) {
		pobj<SMessage> messageSent = client.Queue.Sent[iSent];
		if(0 == messageSent.get_ref())
			continue;
		uint64_t timeCurrent = timeCurrentInUs();
		const uint64_t timeSent = messageSent->Time;
		const uint64_t timeDifference = timeCurrent - timeSent;
		if(timeDifference > UDP_PAYLOAD_SENT_LIFETIME) {
			client.Queue.Sent.remove_unordered(iSent);
			--iSent;
			if(messageSent->RetryCount) {
				--messageSent->RetryCount;
				timeCurrent += UDP_PAYLOAD_SENT_LIFETIME;
				info_printf("A message sent didn't receive a confirmation on time: %llx (%llx). Setting current time: %llx, Retries left: %u.", messageSent->Time, ::hashFromTime(messageSent->Time), timeCurrent, (uint32_t)messageSent->RetryCount);
				if(messageSent->Time == client.KeyPing)
					messageSent->Time = client.KeyPing = timeCurrent;
				else
					messageSent->Time = timeCurrent;
				queueToSend.push_back(messageSent);
			}
			else {
				info_printf("A message sent didn't receive a confirmation on time: %llx.", messageSent->Time);
			}
		}
	}

	apobj<SMessage> & payloadsToSend = messageCacheSend;
	payloadsToSend.clear();
	messageCacheSent.clear();
	for(uint32_t iSend = 0; iSend < queueToSend.size(); ++iSend) {
		pobj<SMessage> pMessageToSend = queueToSend[iSend];
		if(0 == pMessageToSend.get_ref())
			continue;
		SMessage & messageToSend = *pMessageToSend;
		if(messageToSend.Command.Command == ENDPOINT_COMMAND_PAYLOAD)
			payloadsToSend.push_back(pMessageToSend);
		else {
			if_fail_cef(send({(const u0_t*)&messageToSend.Command, sizeof(messageToSend.Command)}), "%s", "Error sending datagram.");
			llc_necs(client.Queue.Sent.push_back(pMessageToSend));
			llc_necs(messageCacheSent.push_back(pMessageToSend));
		}
	}
	llc_necall(::payloadQueueOptimize(client, messageCacheSent, payloadsToSend), "%s", "Failed to optimize payload queue.");
	llc_necall(::payloadQueueSend(client, messageCacheSent, payloadsToSend, send), "%s", "Failed ");
	for(uint32_t iSent = 0; iSent < messageCacheSent.size(); ++iSent)
		for(uint32_t iSend = 0; iSend < queueToSend.size(); ++iSend)
			if(queueToSend[iSend] == messageCacheSent[iSent]) {
				queueToSend.remove(iSend);
				break;
			}
	return 0;
}

static llc::err_t handleRequestDisconnect(::llc::SCommand & command, ::llc::SConnection & client) {
	if_true_fef(0 != command.Packed || 0 != command.Encrypted || 0 != command.Compressed || ::llc::CONNECTION_STATE_IDLE != client.State, "Invalid command or client state: 0x%X '%s'.", client.State, ::llc::get_value_label(client.State).begin());
	client.FirstPing = client.KeyPing = 0;
	client.LastPing = ::llc::timeCurrentInUs();
	client.State = ::llc::CONNECTION_STATE_DISCONNECTED;
	info_printf("%s", "Disconnected!");
	return 0;
}

static llc::err_t handleRequestConnect(::llc::SCommand & command, ::llc::SConnection & client) {
	if_true_fef(1 != command.Packed || ::llc::CONNECTION_STATE_HANDSHAKE != client.State, "Invalid client state: 0x%X '%s'.", client.State, ::llc::get_value_label(client.State).begin());
	client.LastPing = ::llc::timeCurrentInUs();
	{
		::llc::lock_guard lock(client.Queue.MutexSend);
		for(uint32_t iSent = 0, countSent = client.Queue.Sent.size(); iSent < countSent; ++iSent)
			if(::llc::ENDPOINT_COMMAND_CONNECT == client.Queue.Sent[iSent]->Command.Command) {
				client.Queue.Sent.remove_unordered(iSent);
				break;
			}
	}
	client.State = ::llc::CONNECTION_STATE_IDLE;
	info_printf("%s", "Connected!");
	return 0;
}

static llc::err_t postConfirmationResponse(::llc::SClientQueue & queue, const ::llc::SCommand command, const uint64_t messageId) {
	::llc::pobj<::llc::SMessage> response = {};
	response->Command = command;
	response->Command.Type = ::llc::ENDPOINT_COMMAND_TYPE_RESPONSE;
	response->Time = messageId;
	{
		::llc::lock_guard lock(queue.MutexSend);
		llc_necall(queue.Send.push_back(response), "Out of memory? Queue size: %u.", queue.Send.size());
	}
	return 0;
}

static llc::err_t handlePayloadResponse(const ::llc::SPayloadHeader & header, ::llc::SConnection & client) {
	::llc::lock_guard lock(client.Queue.MutexSend);
	info_printf("Received response to message id: %llx", header.MessageId);
	bool found = false;
	for(uint32_t iSent = 0; iSent < client.Queue.Sent.size(); ++iSent) {
		::llc::SMessage & messageSent = *client.Queue.Sent[iSent];
		for(uint32_t iHash = 0; iHash < messageSent.Hashes.size(); ++iHash) {
			info_printf("Checking hash for message: %llx.", messageSent.Hashes[iHash]);
			if(::hashFromTime(messageSent.Time) == header.MessageId) {
				info_printf("Removing message from confirmation queue: %llx. ", header.MessageId);
				client.Queue.Sent.remove_unordered(iSent);
				client.LastPing = ::llc::timeCurrentInUs();
				found = true;
				break;
			}
			uint64_t hashLocal = messageSent.Hashes[iHash];
			if(hashLocal == header.MessageId) {
				info_printf("Removing message from confirmation queue: %llx. ", header.MessageId);
				client.Queue.Sent.remove_unordered(iSent);
				client.LastPing = ::llc::timeCurrentInUs();
				found = true;
				break;
			}
		}
		if(found)
			break;
	}
	if(false == found)
		info_printf("Response confirmation for message was not found: %llx.", header.MessageId);
	return 0;
}

static constexpr int64_t CONNECTION_KEY_ADVANTAGE = 1000000;

static llc::err_t handlePayloadRequest(const ::llc::SPayloadHeader & header, ::llc::SCommand & command, ::llc::SConnection & client, const ::llc::vcu0_t & packet) {
	uint64_t estimatedTimeSent = 0;
	if(client.KeyPing == 0) {
		for(int64_t iTime = 0, countLapse = 60000000; iTime < countLapse; ++iTime)
			if(::hashFromTime(client.FirstPing - CONNECTION_KEY_ADVANTAGE + iTime) == header.MessageId) {
				client.KeyPing = client.FirstPing - CONNECTION_KEY_ADVANTAGE + iTime;
				estimatedTimeSent = client.FirstPing - CONNECTION_KEY_ADVANTAGE + iTime;
				info_printf("Key ping detected in %u attempts: %llu. First ping: %llu. Difference: %llu.", iTime, client.KeyPing, client.FirstPing, client.KeyPing - client.FirstPing);
				break;
			}
		if_true_fef(0 == client.KeyPing, "%s", "Failed to determine encryption key!");
	}
	else {
	}

	::llc::pobj<::llc::SMessage> messageReceived = {};
	messageReceived->Command = header.Command;
	messageReceived->Time = estimatedTimeSent;
	if(header.Size > 0) {
		const ::llc::vcu0_t payload = {&packet[sizeof(::llc::SPayloadHeader)], header.Size};
		if(0 != command.Compressed) {
			if(0 != command.Encrypted) {
				::llc::au0_t inflated;
				::llc::au0_t decoded;
				if_fail_fef(::llc::ardellDecode(payload, client.KeyPing & 0xFFFFFFFF, true, decoded), "%s", "Failed to decrypt packet!");
				if_fail_fef(::llc::arrayInflate(decoded, messageReceived->Payload), "%s", "Failed to inflate packet!");
			}
			else {
				if_fail_fef(::llc::arrayInflate(payload, messageReceived->Payload), "%s", "Failed to inflate packet!");
			}
		}
		else {
			if(0 != command.Encrypted) {
				messageReceived->Payload.clear();
				if_fail_fef(::llc::ardellDecode(payload, client.KeyPing & 0xFFFFFFFF, true, messageReceived->Payload), "%s", "Failed to decrypt packet!");
			}
			else {
				messageReceived->Payload.resize(header.Size);
				::memcpy(messageReceived->Payload.begin(), payload.begin(), header.Size);
				messageReceived->Payload.resize(header.Size + 1, 0);
				messageReceived->Payload.resize(header.Size);
			}
		}
	}
	info_printf("Packet received. Payload size received: %u. Payload size after unpacking/decrypting: %u.", header.Size, messageReceived->Payload.size());
	client.LastPing = ::llc::timeCurrentInUs();
	if(0 == (header.Command.Packed & 1)) {
		{
			::llc::lock_guard lock(client.Queue.MutexReceive);
			llc_necall(client.Queue.Received.push_back(messageReceived), "Out of memory? Queue size: %u.", client.Queue.Received.size());
		}
		::llc::pobj<::llc::SMessage> response = {};
		::postConfirmationResponse(client.Queue, header.Command, header.MessageId);
	}
	else {
		u2_t cursor = 0;
		if_true_fef(messageReceived->Payload.size() < sizeof(uint16_t), "%s", "Invalid data.");
		uint16_t packetCount = 0;
		::memcpy(&packetCount, messageReceived->Payload.begin(), sizeof(packetCount));
		cursor += sizeof(packetCount);
		if(0 == packetCount)
			return 0;
		if_true_fef((packetCount * sizeof(uint16_t)) > (messageReceived->Payload.size() - cursor), "%s", "Invalid packet data.");
		const u2_t sizesOffset = cursor;
		cursor += packetCount * sizeof(uint16_t);
		::llc::apobj<::llc::SMessage> tempQueue;
		for(uint32_t iSizes = 0; iSizes < packetCount; ++iSizes) {
			uint16_t packetSize = 0;
			::memcpy(&packetSize, messageReceived->Payload.begin() + sizesOffset + iSizes * sizeof(packetSize), sizeof(packetSize));
			if_true_fef(packetSize > messageReceived->Payload.size() - cursor, "Corrupt payload data. Sizes don't match. Payload size: %u.", messageReceived->Payload.size());
			::llc::pobj<::llc::SMessage> messageReceivedInt = {};
			messageReceivedInt->Command = header.Command;
			messageReceivedInt->Time = header.MessageId;
			messageReceivedInt->Command.Packed = 0;
			messageReceivedInt->Command.Encrypted = 0;
			messageReceivedInt->Command.Compressed = 0;
			llc_necs(messageReceivedInt->Payload.resize(packetSize));
			if(packetSize)
				::memcpy(messageReceivedInt->Payload.begin(), messageReceived->Payload.begin() + cursor, packetSize);
			cursor += packetSize;
			llc_necs(tempQueue.push_back(messageReceivedInt));
		}
		{
			::llc::lock_guard lock(client.Queue.MutexReceive);
			for(uint32_t iMessage = 0; iMessage < tempQueue.size(); ++iMessage)
				llc_necall(client.Queue.Received.push_back(tempQueue[iMessage]), "Out of memory? Queue size: %u.", client.Queue.Received.size());
		}
		::postConfirmationResponse(client.Queue, header.Command, header.MessageId);
	}
	return 0;
}

static llc::err_t handlePayload(::llc::SCommand & command, ::llc::SConnection & client, const ::llc::FConnectionReceive & receive, ::llc::au0_t & packet) {
	if(::llc::CONNECTION_STATE_IDLE != client.State)
		return 1;
	llc_necs(receive(packet));
	::llc::SPayloadHeader header = {};
	::memcpy(&header, packet.begin(), sizeof(header));
	switch(command.Type) {
	default								: return -1;
	case ::llc::ENDPOINT_COMMAND_TYPE_REQUEST	: return ::handlePayloadRequest(header, command, client, packet);
	case ::llc::ENDPOINT_COMMAND_TYPE_RESPONSE	: return ::handlePayloadResponse(header, client);
	}
}

llc::err_t llc::connectionHandleCommand(SConnection & client, SCommand & command, const FConnectionReceive & receive, au0_t & receiveBuffer) {
	const vcst_t labelCommand = get_value_label(command.Command);
	const vcst_t labelType = get_value_label(command.Type);
	info_printf("Command: %s. Type: %s."
		"\nCompressed   : %i."
		"\nEncrypted    : %i."
		"\nMultipart    : %i."
		"\nPacked       : %i."
		, labelCommand.begin(), labelType.begin()
		, command.Compressed ? 1 : 0
		, command.Encrypted ? 1 : 0
		, command.Multipart ? 1 : 0
		, command.Packed ? 1 : 0
		);
	switch(command.Type) {
	case ENDPOINT_COMMAND_TYPE_REQUEST:
		switch(command.Command) {
		case ENDPOINT_COMMAND_PAYLOAD		: return ::handlePayload(command, client, receive, receiveBuffer);
		case ENDPOINT_COMMAND_CONNECT		: return ::handleRequestConnect(command, client);
		case ENDPOINT_COMMAND_DISCONNECT	: return ::handleRequestDisconnect(command, client);
		case ENDPOINT_COMMAND_NOOP			: return 0;
		default								:
			error_printf("Invalid command!: %u.", command.Command);
			break;
		}
		break;
	case ENDPOINT_COMMAND_TYPE_RESPONSE:
		switch(command.Command) {
		case ENDPOINT_COMMAND_PAYLOAD		: return ::handlePayload(command, client, receive, receiveBuffer);
		case ENDPOINT_COMMAND_CONNECT		:
		case ENDPOINT_COMMAND_DISCONNECT	:
		case ENDPOINT_COMMAND_NOOP			: return 0;
		default								:
			error_printf("Invalid command!: %u.", command.Command);
			break;
		}
		break;
	}
	return -1;
}

