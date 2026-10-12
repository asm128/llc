#include "llc_keyval.h"
#include "llc_array_obj.h"
#include "llc_string.h"

#ifndef LLC_PROCESS_H_23627
#define LLC_PROCESS_H_23627

namespace llc
{
	error_t			environmentBlockViews		(vcs0_t environmentBlock, aobj<kv<vcst_t>> & out_Views);
	error_t			environmentBlockFromEnviron	(string & environmentBlock);
} // namespace

#endif // LLC_PROCESS_H_23627
