#include "llc_debug.h"
#if defined(LLC_WINDOWS) && defined(LLC_DEBUG_ENABLED)
#	include <crtdbg.h>
#endif

sttc	int	DebugBreakEnabled	= 0;

int	llc::debugBreakEnable	(int enabled)	nxpt { return DebugBreakEnabled = 0 != enabled; }
int	llc::debugBreakDisable	()				nxpt { return debugBreakEnable(false); }
int	llc::debugBreakEnabled	()				nxpt { return DebugBreakEnabled; }
int	llc::debugBreak			()				nxpt {
	if(debugBreakEnabled()) {
#if defined(LLC_WINDOWS) && defined(LLC_DEBUG_ENABLED)
		_CrtDbgBreak();
#endif
	}
	return 0;
}
