#include "llc_typeint.h"

#ifndef LLC_CHRONO_H_23627
#define LLC_CHRONO_H_23627

namespace llc
{
#if defined(LLC_ANDROID) || defined(LLC_LINUX)
	long long				timeCurrent			();
	long long				timeCurrentInMs		();
	long long				timeCurrentInUs		();
#else
	int64_t					timeCurrent			();
	int64_t					timeCurrentInMs		();
	int64_t					timeCurrentInUs		();
#endif
#pragma pack(push, 1)
	stct STimestamps {	
		uint64_t				Created				= 0;
		uint64_t				Loaded				= 0;
		uint64_t				Modified			= 0;
		uint64_t				Saved				= 0;
	};

	stct SDaylight {
		uint64_t				TimeOffset		= {};
		uint32_t				OffsetMinutes	= 90;
		float					ExtraRatio		= .5;
	};
#pragma pack(pop)

} // namespace

#endif // LLC_CHRONO_H_23627
