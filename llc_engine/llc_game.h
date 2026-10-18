#include "llc_game_stage.h"
#include "llc_apod_serialize.h"

#ifndef LLC_GAME_H
#define LLC_GAME_H

namespace llc
{
	GDEFINE_ENUM_TYPE(PLAYER_MODE, uint8_t);
	GDEFINE_ENUM_VALUE(PLAYER_MODE, AI			, 0);
	GDEFINE_ENUM_VALUE(PLAYER_MODE, Human		, 1);

	struct SPlayerInventory {
		//::llc::PLAYER_MODE	Mode				= (::llc::PLAYER_MODE)::llc::PLAYER_MODE_AI;
	};

	struct SPlayer {
		::llc::vcsc_t		Name				= "Player 1";
		uint32_t			Color				= 0xFFFF0000;
		::llc::PLAYER_MODE	Mode				= (::llc::PLAYER_MODE)::llc::PLAYER_MODE_AI;
	};

	GDEFINE_ENUM_TYPE(NETWORK_MODE, uint8_t);
	GDEFINE_ENUM_VALUE(NETWORK_MODE, Offline, 0);
	GDEFINE_ENUM_VALUE(NETWORK_MODE, LAN	, 1);
	GDEFINE_ENUM_VALUE(NETWORK_MODE, Online	, 2);

#pragma pack(push, 1)
	struct SGameSetup {
		uint32_t			OffsetStage			= 0;
		::llc::NETWORK_MODE	NetworkMode			= ::llc::NETWORK_MODE_Offline;
	};

	struct SGameState {
		uint32_t			Stage				= 0;
		SStageTime			UserTime			= {};
		bool				Paused				= false;
		double				CameraSwitchDelay	= 0;
	};
#pragma pack(pop)

	typedef ::llc::apod<SStageState>	astagestate;

	struct SGame {
		SGameSetup			Setup				= {};
		SGameState			State				= {};
		astagestate			Stages				= {};

		::llc::error_t		Save				(::llc::au0_t & output)	const	{ 
			llc_necs(llc::savePOD(output, Setup)); 
			llc_necs(llc::savePOD(output, State)); 
			llc_necs(llc::saveView(output, Stages));
			return 0;
		}

		::llc::error_t		Load				(::llc::vcu0_t & input)	{ 
			llc_necs(llc::loadPOD(input, Setup));
			llc_necs(llc::loadPOD(input, State)); 
			llc_necs(llc::loadView(input, Stages));
			return 0;
		}

	};
} // namespace

#endif // LLC_GAME_H
