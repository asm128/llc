#include "llc_array.h"
#include "llc_keyval.h"

#ifndef LLC_ARGS_H_23627
#define LLC_ARGS_H_23627

namespace llc
{
	enum ARGS_STATE : u0_t {
		ARGS_STATE_ARGUMENT,
		ARGS_STATE_OPTION_VALUE,
		ARGS_STATE_POSITIONAL,
	};

	stct SCommandLineArgs {
		apod<kvvcst_t<vcst_t>>	Options		= {};
		aobj<vcst_t>			Positionals	= {};
	};

	err_t	argsParse			(view<sc_c *> argv, SCommandLineArgs & output);
	err_t	argsValueFromKey	(vcst_t key, vcst_t & output);
}

#endif // LLC_ARGS_H_23627
