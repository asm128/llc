#include "llc_img_serialize.h"
#include "llc_array_ptr.h"
#include "llc_apod_n2.h"
#include "llc_apod_n3.h"

#include "llc_noise.h"
#include "llc_enum.h"

#ifndef LLC_TILE_MAP
#define LLC_TILE_MAP

namespace llc
{
	typedef	uint64_t				tileid_t;
	typedef	::llc::img<tileid_t>	tilegrid;

	GDEFINE_ENUM_TYPE (TILE_FLAG, uint8_t);
	GDEFINE_ENUM_VALUE(TILE_FLAG, Walk, 1);
	GDEFINE_ENUM_VALUE(TILE_FLAG, Shot, 2);

	struct STileBlock {
		::llc::apod<TILE_FLAG>	Flags;
		::llc::tilegrid			Tiles;

		::llc::error_t			Save				(::llc::au0_t & output)	const	{ 
			if_fail_fe(llc::saveView (output, Flags)); 
			if_fail_fe(llc::saveImage(output, Tiles.View)); 
			return 0;
		}

		::llc::error_t			Load				(::llc::vcu0_t & input)	{ 
			if_fail_fe(llc::loadView (input, Flags )); 
			if_fail_fe(llc::loadImage(input, Tiles)); 
			return 0;
		}
	};

#pragma pack(push, 1)
	struct STileMapSetup {
		uint64_t				Seed				= ::llc::noise1DBase(::llc::timeCurrentInUs());
		::llc::n2u0_t				BlockSize			= {255, 255};
		::llc::n2u1_t			WorldSize			= {255, 255};	// in blocks
	};
#pragma pack(pop)

	struct STileMap {
		::llc::STileMapSetup		Config;	// Map constants
		::llc::an3u32				Coords;	// Block coords
		::llc::apod<STimestamps>	Stamps;	// 
		::llc::apobj<STileBlock>	Blocks;	// 
	
		::llc::error_t			Save				(::llc::au0_t & output)	const	{ 
			if_fail_fe(llc::savePOD (output, Config)); 
			if_fail_fe(llc::saveView(output, Coords)); 
			if_fail_fe(llc::saveView(output, Stamps)); 
			if_fail_fe(llc::savePOD (output, Blocks.size())); 
			for(uint32_t iMap = 0; iMap < Blocks.size(); ++iMap) {
				if_fail_fe(Blocks[iMap] ? Blocks[iMap]->Save(output) : STileBlock{}.Save(output));
			}
			return 0; 
		}
		::llc::error_t			Load				(::llc::vcu0_t & input) { 
			if_fail_fe(llc::loadPOD (input, Config)); 
			if_fail_fe(llc::loadView(input, Coords)); 
			if_fail_fe(llc::loadView(input, Stamps)); 
			uint32_t					blockCount			= {};
			if_fail_fe(llc::loadPOD (input, blockCount)); 
			if_fail_fe(Blocks.reserve(blockCount));
			for(uint32_t iMap = 0; iMap < blockCount; ++iMap) {
				::llc::pobj<STileBlock>		block;
				if_fail_fe(block->Load(input));
				if(0 == block->Tiles.size())
					block.clear();

				if_fail_fe(Blocks.push_back(block));
			}
			return 0; 
		}
	};

} // namespace ssg

#endif // LLC_TILE_MAP
