#include "llc_connection_ip.h"

#include <cstring>

LLC_USING_TYPEINT();

#if defined(LLC_WINDOWS)
typedef int socklen_t;
#endif

static llc::err_t connectionReceive(::llc::SSocketConnection & client, const ::llc::SCommand & command, ::llc::au0_t & packet) {
	::llc::SPayloadHeader header = {};
	sockaddr_in remote = {};
	socklen_t remoteSize = (socklen_t)sizeof(remote);
	const int headerBytes = ::recvfrom(client.Socket.Handle, (::llc::sc_t*)&header, (int)sizeof(header), MSG_PEEK, (sockaddr*)&remote, &remoteSize);
	if(headerBytes < 0) {
#if defined(LLC_WINDOWS)
		if(WSAGetLastError() != WSAEMSGSIZE) {
			error_printf("%s", "Could not receive payload header.");
			return -1;
		}
#endif
	}
	if(::llc::ENDPOINT_COMMAND_TYPE_RESPONSE == command.Type) {
		llc_necs(packet.resize(sizeof(header)));
		::memcpy(packet.begin(), &header, sizeof(header));
		return 0;
	}
	llc_necs(packet.resize((::llc::u2_t)sizeof(header) + header.Size));
	::memset(packet.begin(), 0, packet.size());
	remoteSize = (socklen_t)sizeof(remote);
	const int received = ::recvfrom(client.Socket.Handle, (::llc::sc_t*)packet.begin(), (int)packet.size(), MSG_PEEK, (sockaddr*)&remote, &remoteSize);
	if(received < 0) {
#if defined(LLC_WINDOWS)
		if(WSAGetLastError() != WSAEMSGSIZE) {
			error_printf("%s", "Could not receive payload data.");
			return -1;
		}
		warning_printf("%s", "Failed to receive all of the payload data.");
#else
		error_printf("%s", "Failed to receive all of the payload data.");
		return -1;
#endif
	}
	else if(0 == received) {
		info_printf("The peer has performed an orderly shutdown: '%s'", "https://linux.die.net/man/3/recvfrom");
		return 1;
	}
	if_true_fef(received != (int)packet.size(), "Packet size received doesn't match with header size. Received size: %u. Expected: %u.", received, packet.size());
	return 0;
}

llc::err_t llc::connectionSendQueue(SSocketConnection & client, apobj<SMessage> & messageCacheSent, apobj<SMessage> & messageCacheSend) {
	const FConnectionSend send = [&client](const vcu0_t & frame) -> err_t {
		sockaddr_in remote = {};
		::llc::tcpipAddressToSockaddr(client.Address, remote);
		const int sent = ::sendto(client.Socket.Handle, (const sc_t*)frame.begin(), (int)frame.size(), 0, (sockaddr*)&remote, sizeof(remote));
		return sent == (int)frame.size() ? 0 : -1;
	};
	return ::llc::connectionSendQueue((SConnection&)client, messageCacheSent, messageCacheSend, send);
}

llc::err_t llc::connectionHandleCommand(SSocketConnection & client, SCommand & command, au0_t & receiveBuffer) {
	const FConnectionReceive receive = [&client, &command](au0_t & packet) -> err_t {
		return ::connectionReceive(client, command, packet);
	};
	const err_t result = ::llc::connectionHandleCommand((SConnection&)client, command, receive, receiveBuffer);
	if(0 == result && ENDPOINT_COMMAND_TYPE_REQUEST == command.Type && ENDPOINT_COMMAND_DISCONNECT == command.Command)
		client.Socket.close();
	return result;
}
