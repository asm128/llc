#include "llc_array_obj.h"
#include "llc_string.h"
#include "llc_string_compose.h"
#include "llc_keyval.h"

#ifndef LLC_ARGS_H_23627
#define LLC_ARGS_H_23627

namespace llc
{
	stct SCommandLineArgs {
		vcst_t					ProgramName = {};
		aobj<keyval<vcst_t>>	Options		= {};
		aobj<vcst_t>			Positionals	= {};
	};

	err_t			argsParse			(SCommandLineArgs & output, view<vcst_t> argv);
	stin	err_t	argsOptionIndex		(const SCommandLineArgs & input, vcst_t key)					{ return input.Options.find([&key](const llc::kvvcst_t<llc::vcst_t> & option) { return option.Key == key; }); }
	err_t			argsOptionValue		(const SCommandLineArgs & input, vcst_t key, vcst_t & output);
}

#endif // LLC_ARGS_H_23627
