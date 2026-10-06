#include "llc_array_obj.h"
#include "llc_keyval.h"

#ifndef LLC_ARGS_H_23627
#define LLC_ARGS_H_23627

namespace llc
{
	stct SCommandLineArgs {
		vcst_t					ProgramName = {};
		aobj<keyval<vcst_t>>	Options		= {};
		aobj<vcst_t>			Positionals	= {};
		aobj<vcst_t>			Environment	= {};
	};

	err_t			argsParse			(SCommandLineArgs & output, view<vcst_t> argv, view<vcst_t> envp = {});
	err_t			argsParse			(SCommandLineArgs & output, int argc, char ** argv, char ** envp = 0);
	err_t			argsOptionValue		(const SCommandLineArgs & input, vcst_t key, vcst_t & output);
	err_t			argsOptionValues	(const SCommandLineArgs & input, vcst_t key, aobj<vcst_t> & output);
	stin	err_t	argsOptionIndex		(const SCommandLineArgs & input, vcst_t key, u2_t offset = 0)	{ rtrn input.Options.find([&key](const kvvcst_t<vcst_t> & option) { rtrn option.Key == key; }, offset); }
	err_t			viewsFromEnvp		(aobj<vcst_t> & outputViews, char * envp[]);
	err_t			viewsFromArgv		(aobj<vcst_t> & outputViews, u2_t argc, char * argv[]);
}

#endif // LLC_ARGS_H_23627
