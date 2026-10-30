#include "llc_game_stage.h"
#include "llc_apod_serialize.h"

#ifndef LLC_GAME_H
#define LLC_GAME_H

namespace llc
{
	GDEFINE_ENUM_TYPE(PLAYER_MODE, u0_t);
	GDEFINE_ENUM_VALUE(PLAYER_MODE, AI			, 0);
	GDEFINE_ENUM_VALUE(PLAYER_MODE, Human		, 1);

	struct SPlayerInventory {
		//::llc::PLAYER_MODE	Mode				= (::llc::PLAYER_MODE)::llc::PLAYER_MODE_AI;
	};

	struct SPlayer {
		::llc::vcst_t		Name				= "Player 1";
		::llc::u2_t			Color				= 0xFFFF0000;
		::llc::PLAYER_MODE	Mode				= (::llc::PLAYER_MODE)::llc::PLAYER_MODE_AI;
	};

	GDEFINE_ENUM_TYPE(NETWORK_MODE, u0_t);
	GDEFINE_ENUM_VALUE(NETWORK_MODE, Offline, 0);
	GDEFINE_ENUM_VALUE(NETWORK_MODE, LAN	, 1);
	GDEFINE_ENUM_VALUE(NETWORK_MODE, Online	, 2);

#pragma pack(push, 1)
	struct SGameSetup {
		::llc::u2_t			OffsetStage			= 0;
		::llc::NETWORK_MODE	NetworkMode			= ::llc::NETWORK_MODE_Offline;
	};

	struct SGameState {
		::llc::u2_t			Stage				= 0;
		SStageTime			UserTime			= {};
		bool				Paused				= false;
		f3_t				CameraSwitchDelay	= 0;
	};
#pragma pack(pop)

	typedef ::llc::apod<SStageState>	astagestate;

	struct SGame {
		SGameSetup			Setup				= {};
		SGameState			State				= {};
		astagestate			Stages				= {};

		::llc::err_t		Save				(::llc::au0_t & output)	const	{ 
			if_fail_fe(llc::savePOD(output, Setup)); 
			if_fail_fe(llc::savePOD(output, State)); 
			if_fail_fe(llc::saveView(output, Stages));
			return 0;
		}

		::llc::err_t		Load				(::llc::vcu0_t & input)	{ 
			if_fail_fe(llc::loadPOD(input, Setup));
			if_fail_fe(llc::loadPOD(input, State)); 
			if_fail_fe(llc::loadView(input, Stages));
			return 0;
		}

	};
} // namespace

#endif // LLC_GAME_H
