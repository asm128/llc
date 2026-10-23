#include "llc_comm.h"

#include "llc_apod_serialize.h"
#include "llc_view_stream.h"

namespace
{
	tplt<tpnm _tInt>
	::llc::u2_t packedSize(_tInt value) {
		cnst ::llc::packed_uint<_tInt>	packed	= value;
		rtrn packed.ValueWidth();
	}

	tplt<tpnm _tInt>
	::llc::err_t streamWritePacked(::llc::view_stream<::llc::u0_t> & output, _tInt value) {
		cnst ::llc::packed_uint<_tInt>	packed	= value;
		rtrn output.write_pod(::llc::vcu0_t{(cnst ::llc::u0_t*)&packed, packed.ValueWidth()});
	}

	tplt<tpnm _tInt>
	::llc::err_t streamReadPacked(::llc::view_stream<::llc::u0_c> & input, _tInt & value) {
		if_zero_fe(input.remaining());
		cnst ::llc::packed_uint<_tInt>	& packed	= *(cnst ::llc::packed_uint<_tInt>*)&input.Data[input.CursorPosition];
		::llc::u0_c			valueWidth	= packed.ValueWidth();
		if_true_fef(valueWidth > input.remaining(), LLC_FMT_GT_U2, (::llc::u2_t)valueWidth, input.remaining());
		::llc::vcu0_t			packedBytes	= {&input.Data[input.CursorPosition], input.remaining()};
		::llc::err_t			bytesRead	= 0;
		if_fail_fe(bytesRead = ::llc::loadPacked(packedBytes, value));
		input.CursorPosition			+= bytesRead;
		rtrn bytesRead;
	}

	::llc::u2_t packetHeaderSize(cnst ::llc::SPacket & packet, ::llc::u2_t fragmentSize) {
		::llc::u2_t	result	=
			1
			+ ::packedSize(packet.Network)
			+ ::packedSize(packet.Source)
			+ ::packedSize(packet.Target)
			+ ::packedSize(packet.Message)
			+ ::packedSize(fragmentSize)
			;
		if(packet.Comm.Multipart) {
			result	+= ::packedSize(packet.Size);
			result	+= ::packedSize(packet.Offset);
		}
		rtrn result;
	}
}

::llc::u0_t llc::commEncode(cnst SCommComm & command) nxpt {
	rtrn (u0_t)
		( ((u0_t)command.Command	& 0x03U) << 0
		| ((u0_t)command.Type		& 0x01U) << 2
		| ((u0_t)command.Packed		& 0x01U) << 3
		| ((u0_t)command.Encrypted	& 0x01U) << 4
		| ((u0_t)command.Compressed	& 0x01U) << 5
		| ((u0_t)command.Multipart	& 0x01U) << 6
		| ((u0_t)command.Resend		& 0x01U) << 7
		);
}

::llc::SCommComm llc::commDecode(u0_t encoded) nxpt {
	SCommComm	command	= {};
	command.Command		= (COMM_COMMAND)((encoded >> 0) & 0x03U);
	command.Type		= (COMM_COMMAND_TYPE)((encoded >> 2) & 0x01U);
	command.Packed		= (encoded >> 3) & 0x01U;
	command.Encrypted	= (encoded >> 4) & 0x01U;
	command.Compressed	= (encoded >> 5) & 0x01U;
	command.Multipart	= (encoded >> 6) & 0x01U;
	command.Resend		= (encoded >> 7) & 0x01U;
	rtrn command;
}

::llc::err_t llc::commPayloadPack(view<cnst vcu0_t> payloads, au0_t & output) {
	output.clear();
	llc_necs(saveUInt(output, payloads.size()));
	for(u2_t iPayload = 0; iPayload < payloads.size(); ++iPayload)
		llc_necs(saveView(output, payloads[iPayload]));
	rtrn 0;
}

::llc::err_t llc::commPayloadLoad(vcu0_t input, SCommPayloadReader & reader) {
	reader		= {};
	reader.Input	= input;
	llc_necs(loadUInt(reader.Input, reader.Count));
	rtrn 0;
}

::llc::err_t llc::commPayloadNext(SCommPayloadReader & reader, vcu0_t & payload) {
	if_true_fef(reader.Index >= reader.Count, LLC_FMT_GE_U2, reader.Index, reader.Count);
	llc_necs(loadView(reader.Input, payload));
	++reader.Index;
	if_true_fef(reader.Index == reader.Count && reader.Input.size(), "Unexpected trailing payload bytes: %" LLC_FMT_U2 ".", reader.Input.size());
	rtrn 0;
}

::llc::err_t llc::commPacketSave(cnst SPacket & packet, vu0_t output, u2_t & sizeOutput) {
	sizeOutput	= 0;
	if_zero_fef(packet.Network, "%s", "A packet requires a network identifier.");
	if_true_fef(COMM_ENDPOINT_INVALID == packet.Source, "%s", "A packet requires a source endpoint.");
	if_true_fef(COMM_ENDPOINT_INVALID == packet.Target, "%s", "A packet requires a target endpoint.");
	if_true_fef(packet.Offset > packet.Size, LLC_FMT_GT_U2, packet.Offset, packet.Size);
	if_true_fef(packet.Payload.size() > packet.Size - packet.Offset, LLC_FMT_GT_U2, packet.Payload.size(), packet.Size - packet.Offset);
	if_true_fef(false == packet.Comm.Multipart && (packet.Offset || packet.Size != packet.Payload.size()), "%s", "A single-part packet must contain its complete payload.");

	view_stream<u0_t>	stream	= {output.begin(), output.size()};
	u0_c			encoded	= commEncode(packet.Comm);
	llc_necs(stream.write_pod(encoded));
	llc_necs(::streamWritePacked(stream, packet.Network));
	llc_necs(::streamWritePacked(stream, packet.Source));
	llc_necs(::streamWritePacked(stream, packet.Target));
	llc_necs(::streamWritePacked(stream, packet.Message));
	llc_necs(::streamWritePacked(stream, packet.Payload.size()));
	if(packet.Comm.Multipart) {
		llc_necs(::streamWritePacked(stream, packet.Size));
		llc_necs(::streamWritePacked(stream, packet.Offset));
	}
	llc_necs(stream.write_pod(packet.Payload));
	sizeOutput	= stream.CursorPosition;
	rtrn 0;
}

::llc::err_t llc::commPacketLoad(vcu0_t input, SPacket & packet) {
	packet	= {};
	view_stream<u0_c>	stream	= {input.begin(), input.size()};
	u0_t			encoded	= {};
	u2_t			fragmentSize	= {};
	llc_necs(stream.read_pod(encoded));
	packet.Comm	= commDecode(encoded);
	llc_necs(::streamReadPacked(stream, packet.Network));
	llc_necs(::streamReadPacked(stream, packet.Source));
	llc_necs(::streamReadPacked(stream, packet.Target));
	llc_necs(::streamReadPacked(stream, packet.Message));
	llc_necs(::streamReadPacked(stream, fragmentSize));
	if(packet.Comm.Multipart) {
		llc_necs(::streamReadPacked(stream, packet.Size));
		llc_necs(::streamReadPacked(stream, packet.Offset));
	}
	else {
		packet.Size	= fragmentSize;
		packet.Offset	= 0;
	}
	if_zero_fef(packet.Network, "%s", "A packet requires a network identifier.");
	if_true_fef(COMM_ENDPOINT_INVALID == packet.Source, "%s", "A packet requires a source endpoint.");
	if_true_fef(COMM_ENDPOINT_INVALID == packet.Target, "%s", "A packet requires a target endpoint.");
	if_true_fef(fragmentSize != stream.remaining(), "Invalid fragment size. Header: %" LLC_FMT_U2 ". Remaining: %" LLC_FMT_U2 ".", fragmentSize, stream.remaining());
	if_true_fef(packet.Offset > packet.Size, LLC_FMT_GT_U2, packet.Offset, packet.Size);
	if_true_fef(fragmentSize > packet.Size - packet.Offset, LLC_FMT_GT_U2, fragmentSize, packet.Size - packet.Offset);
	packet.Payload	= {fragmentSize ? &stream.Data[stream.CursorPosition] : 0, fragmentSize};
	rtrn 0;
}

::llc::err_t llc::commPacketizerBegin(SCommPacketizer & packetizer, cnst SPacket & packet) {
	packetizer			= {};
	packetizer.Packet		= packet;
	packetizer.Packet.Size		= packet.Payload.size();
	packetizer.Packet.Offset	= 0;
	packetizer.Packet.Comm.Multipart	= 0;
	rtrn 0;
}

::llc::err_t llc::commPacketNext(SCommPacketizer & packetizer, vu0_t output, u2_t & sizeOutput) {
	sizeOutput	= 0;
	if(packetizer.Complete)
		rtrn 0;
	if(false == packetizer.Started) {
		packetizer.Started	= true;
		u2_c sizeHeader		= ::packetHeaderSize(packetizer.Packet, packetizer.Packet.Size);
		packetizer.Packet.Comm.Multipart	= (sizeHeader + packetizer.Packet.Size > output.size());
	}

	SPacket	frame	= packetizer.Packet;
	frame.Offset	= packetizer.Offset;
	u2_c remaining	= packetizer.Packet.Size - packetizer.Offset;
	if(false == frame.Comm.Multipart) {
		frame.Payload		= packetizer.Packet.Payload;
		llc_necs(commPacketSave(frame, output, sizeOutput));
		packetizer.Offset	= packetizer.Packet.Size;
		packetizer.Complete	= true;
		rtrn 0;
	}

	u2_t fragmentSize	= ::llc::min(remaining, output.size());
	while(fragmentSize) {
		u2_c sizeHeader	= ::packetHeaderSize(frame, fragmentSize);
		if(sizeHeader + fragmentSize <= output.size())
			break;
		fragmentSize	= sizeHeader < output.size() ? ::llc::min(fragmentSize - 1, output.size() - sizeHeader) : 0;
	}
	if_zero_fef(fragmentSize, "%s", "The carrier frame is too small for the communication header.");
	llc_necs(packetizer.Packet.Payload.slice(frame.Payload, packetizer.Offset, fragmentSize));
	llc_necs(commPacketSave(frame, output, sizeOutput));
	packetizer.Offset	+= fragmentSize;
	packetizer.Complete	= (packetizer.Offset == packetizer.Packet.Size);
	rtrn 0;
}

::llc::err_t llc::commReassemblyReset(SCommReassembly & assembly, vu0_t storage) {
	assembly		= {};
	assembly.Storage	= storage;
	rtrn 0;
}

::llc::err_t llc::commPacketReceive(SCommReassembly & assembly, cnst SPacket & packet, vcu0_t & message, bool & complete) {
	message		= {};
	complete	= false;
	if(false == packet.Comm.Multipart) {
		if_true_fef(packet.Offset || packet.Size != packet.Payload.size(), "%s", "Invalid single-part packet.");
		message		= packet.Payload;
		complete	= true;
		rtrn 0;
	}
	if_zero_fef(packet.Size, "%s", "A multipart packet requires a total size.");
	if(0 == packet.Offset) {
		if_true_fef(packet.Size > assembly.Storage.size(), LLC_FMT_GT_U2, packet.Size, assembly.Storage.size());
		assembly.Network	= packet.Network;
		assembly.Source	= packet.Source;
		assembly.Message	= packet.Message;
		assembly.Size		= packet.Size;
		assembly.Received	= 0;
		assembly.Active	= true;
	}
	if_true_fef(false == assembly.Active, "%s", "Received a continuation without an active message.");
	if_true_fef(assembly.Network != packet.Network || assembly.Source != packet.Source || assembly.Message != packet.Message || assembly.Size != packet.Size, "%s", "Multipart identity mismatch.");
	if_true_fef(packet.Offset != assembly.Received, "Out-of-order fragment. Expected: %" LLC_FMT_U2 ". Received: %" LLC_FMT_U2 ".", assembly.Received, packet.Offset);
	if_true_fef(packet.Payload.size() > assembly.Size - assembly.Received, LLC_FMT_GT_U2, packet.Payload.size(), assembly.Size - assembly.Received);
	if(packet.Payload.size())
		memcpy(&assembly.Storage[assembly.Received], packet.Payload.begin(), packet.Payload.size());
	assembly.Received	+= packet.Payload.size();
	if(assembly.Received == assembly.Size) {
		message		= {assembly.Storage.begin(), assembly.Size};
		complete	= true;
		assembly.Active	= false;
	}
	rtrn 0;
}

::llc::err_t llc::commCoordinatorInit(SCommCoordinator & coordinator, comm_network_id_t network, u1_t frameSize) {
	if_zero_fef(network, "%s", "A coordinator requires a network identifier.");
	if_zero_fef(frameSize, "%s", "A coordinator requires a carrier frame size.");
	coordinator.Network		= network;
	coordinator.FrameSize		= frameSize;
	coordinator.EndpointNext	= 1;
	rtrn 0;
}

::llc::err_t llc::commCoordinatorJoin(SCommCoordinator & coordinator, u1_t frameSize, SCommEndpoint & endpoint) {
	if_zero_fef(coordinator.Network, "%s", "The coordinator is not initialized.");
	if_zero_fef(frameSize, "%s", "An endpoint requires a carrier frame size.");
	if_true_fef(COMM_ENDPOINT_INVALID == coordinator.EndpointNext, "%s", "The coordinator has no endpoint identifiers left.");
	endpoint.Address.Network	= coordinator.Network;
	endpoint.Address.Endpoint	= coordinator.EndpointNext++;
	endpoint.FrameSize		= ::llc::min(frameSize, coordinator.FrameSize);
	endpoint.MessageNext		= 1;
	rtrn 0;
}

::llc::err_t llc::commMessageNext(SCommEndpoint & endpoint, comm_message_id_t & message) {
	if_zero_fef(endpoint.Address.Network, "%s", "The endpoint is not assigned to a network.");
	if_true_fef(COMM_ENDPOINT_INVALID == endpoint.Address.Endpoint, "%s", "The endpoint identifier is invalid.");
	if_zero_fef(endpoint.MessageNext, "%s", "The endpoint message identifier wrapped.");
	if_true_fef((comm_message_id_t)-1 == endpoint.MessageNext, "%s", "The endpoint has no message identifiers left.");
	message	= endpoint.MessageNext++;
	rtrn 0;
}
