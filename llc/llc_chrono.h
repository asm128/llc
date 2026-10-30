#include "llc_typeint.h"
#include "llc_parse.h"
#include "llc_array_pod.h"

#ifndef LLC_CHRONO_H_23627
#define LLC_CHRONO_H_23627

namespace llc
{
#if defined(LLC_ANDROID) || defined(LLC_LINUX)
	long long				timeCurrent			();
	long long				timeCurrentInMs		();
	long long				timeCurrentInUs		();
#else
	s3_t					timeCurrent			();
	s3_t					timeCurrentInMs		();
	s3_t					timeCurrentInUs		();
#endif
	// Local calendar bindings: yy, MM, dd, hh, mm, ss, ff (hundredths), all two digits.
	err_t					timeStringFormat	(vcst_t format, s3_t timeInMs, asc_t & output);
	err_t					timeStringFormat	(vcst_t format, view<cnst SStripLiteralType> tokens, s3_t timeInMs, asc_t & output);
#pragma pack(push, 1)
	stct STimestamps {	
		u3_t				Created				= 0;
		u3_t				Loaded				= 0;
		u3_t				Modified			= 0;
		u3_t				Saved				= 0;
	};

	stct SDaylight {
		u3_t				TimeOffset		= {};
		::llc::u2_t				OffsetMinutes	= 90;
		f2_t					ExtraRatio		= .5;
	};
#pragma pack(pop)

} // namespace

#endif // LLC_CHRONO_H_23627
