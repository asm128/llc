#include "llc_array_pod.h"
#include "llc_enum.h"

#ifndef LLC_COMM_H_23627
#define LLC_COMM_H_23627

namespace llc
{
	GDEFINE_ENUM_TYPE (COMM_COMMAND, u0_t);
	GDEFINE_ENUM_VALUE(COMM_COMMAND, Noop		, 0);
	GDEFINE_ENUM_VALUE(COMM_COMMAND, Connect	, 1);
	GDEFINE_ENUM_VALUE(COMM_COMMAND, Payload	, 2);
	GDEFINE_ENUM_VALUE(COMM_COMMAND, Disconnect	, 3);

	GDEFINE_ENUM_TYPE (COMM_COMMAND_TYPE, u0_t);
	GDEFINE_ENUM_VALUE(COMM_COMMAND_TYPE, Request	, 0);
	GDEFINE_ENUM_VALUE(COMM_COMMAND_TYPE, Response	, 1);

#pragma pack(push, 1)
	stct SCommComm {
		COMM_COMMAND		Command		: 2;
		COMM_COMMAND_TYPE	Type		: 1;
		u0_t			Packed		: 1;
		u0_t			Encrypted	: 1;
		u0_t			Compressed	: 1;
		u0_t			Multipart	: 1;
		u0_t			Resend		: 1;
	};
#pragma pack(pop)
	static_assert(1 == szof(SCommComm));

	usng comm_network_id_t	= u3_t;
	usng comm_endpoint_id_t	= u1_t;
	usng comm_message_id_t	= u2_t;

	stxp comm_endpoint_id_t	COMM_ENDPOINT_COORDINATOR	= 0;
	stxp comm_endpoint_id_t	COMM_ENDPOINT_INVALID		= (comm_endpoint_id_t)-1;

	stct SCommAddress {
		comm_network_id_t	Network		= {};
		comm_endpoint_id_t	Endpoint	= COMM_ENDPOINT_INVALID;
	};

	stct SPacket {
		SCommComm		Comm		= {};
		comm_network_id_t	Network		= {};
		comm_endpoint_id_t	Source		= COMM_ENDPOINT_INVALID;
		comm_endpoint_id_t	Target		= COMM_ENDPOINT_INVALID;
		comm_message_id_t	Message		= {};
		u2_t			Size		= {};
		u2_t			Offset		= {};
		vcu0_t			Payload		= {};
	};

	stct SCommPacketizer {
		SPacket	Packet		= {};
		u2_t	Offset		= {};
		bool	Started		= {};
		bool	Complete	= {};
	};

	stct SCommPayloadReader {
		vcu0_t	Input	= {};
		u2_t	Count	= {};
		u2_t	Index	= {};
	};

	stct SCommReassembly {
		comm_network_id_t	Network		= {};
		comm_endpoint_id_t	Source		= COMM_ENDPOINT_INVALID;
		comm_message_id_t	Message		= {};
		u2_t			Size		= {};
		u2_t			Received	= {};
		vu0_t			Storage		= {};
		bool			Active		= {};
	};

	stct SCommEndpoint {
		SCommAddress		Address		= {};
		u1_t			FrameSize	= {};
		comm_message_id_t	MessageNext	= 1;
	};

	stct SCommCoordinator {
		comm_network_id_t	Network		= {};
		u1_t			FrameSize	= {};
		comm_endpoint_id_t	EndpointNext	= 1;
	};

	u0_t	commEncode			(cnst SCommComm & command)		nxpt;
	SCommComm	commDecode			(u0_t encoded)				nxpt;

	err_t	commPayloadPack		(view<cnst vcu0_t> payloads, au0_t & output);
	err_t	commPayloadLoad		(vcu0_t input, SCommPayloadReader & reader);
	err_t	commPayloadNext		(SCommPayloadReader & reader, vcu0_t & payload);

	err_t	commPacketSave		(cnst SPacket & packet, vu0_t output, u2_t & sizeOutput);
	err_t	commPacketLoad		(vcu0_t input, SPacket & packet);
	err_t	commPacketizerBegin	(SCommPacketizer & packetizer, cnst SPacket & packet);
	err_t	commPacketNext		(SCommPacketizer & packetizer, vu0_t output, u2_t & sizeOutput);

	err_t	commReassemblyReset	(SCommReassembly & assembly, vu0_t storage);
	err_t	commPacketReceive	(SCommReassembly & assembly, cnst SPacket & packet, vcu0_t & message, bool & complete);

	err_t	commCoordinatorInit	(SCommCoordinator & coordinator, comm_network_id_t network, u1_t frameSize);
	err_t	commCoordinatorJoin	(SCommCoordinator & coordinator, u1_t frameSize, SCommEndpoint & endpoint);
	err_t	commMessageNext		(SCommEndpoint & endpoint, comm_message_id_t & message);
}

#endif // LLC_COMM_H_23627
