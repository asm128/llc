#include "llc_args.h"

llc::err_t			llc::argsOptionValue	(const SCommandLineArgs & input, vcst_t key, vcst_t & output)	{
	err_c 					optionIndex			= argsOptionIndex(input, key);
	if(0 <= optionIndex) { // success!
		output = input.Options[optionIndex].Val;
		return optionIndex;
	}
	llc::string				possibleKeys		= {};
	for(u2_t iKey = 0; iKey < input.Options.size(); ++iKey) {
		if_fail_fe(llc::append_strings(possibleKeys, iKey ? str(", ") : str(""), '"', input.Options[iKey].Key, '"'));
	}
	warning_printf("{%s} contains no [\"%.*s\"]", possibleKeys.begin(), key.size(), key.begin());
	return -1;
}

enum ARGS_STATE : llc::u0_t 
	{ ARGS_STATE_ARGUMENT
	, ARGS_STATE_OPTION_VALUE
	, ARGS_STATE_POSITIONAL
	};

sttc ::llc::err_t	argsOptionName			(::llc::vcst_c & argument, ::llc::kvvcst_t<::llc::vcst_t> & option, bool & hasValue) {
	::llc::b8_c				isDoubleDash		= '-' == argument[1];
	::llc::u0_c				prefixLen			= isDoubleDash ? 2U : 1U;
	::llc::u2_t				iChar				= prefixLen;
	for(; iChar < argument.size() && argument[iChar] != '='; ++iChar) 
		continue;
	option.Key			= {&argument[prefixLen], iChar - prefixLen};
	hasValue			= iChar < argument.size(); // If we found an '=' character, then the option has a value.
	if(hasValue) 
		option.Val			=  {&argument[iChar + 1], argument.size() - iChar - 1};
	rtrn option.Key.size();
}

::llc::err_t		llc::argsParse		(::llc::SCommandLineArgs & output, ::llc::view<vcst_t> argv) {
	ARGS_STATE				state				= ARGS_STATE_ARGUMENT;
	kvvcst_t<vcst_t>		option				= {};
	output.ProgramName = argv[0];

	for(u2_t iArg = 1; iArg < argv.size(); ++iArg) {
		vcst_t					argument			= {argv[iArg], (u2_t)-1};
		if(state == ARGS_STATE_POSITIONAL) {
			if_fail_fe(output.Positionals.push_back(argument));
			continue;
		}
		if(state == ARGS_STATE_OPTION_VALUE) {
			option.Val			= argument; // The value of the current option is the next argument
			if_fail_fe(output.Options.push_back(option));
			state				= ARGS_STATE_ARGUMENT;
			continue;
		}
		if(argument == vcsc_t{"--", 2}) {
			state				= ARGS_STATE_POSITIONAL;
			continue;
		}
		if(argument.size() > 1 && argument[0] == '-') {
			bool					hasValue			= false;
			llc_necs(argsOptionName(argument, option, hasValue));
			if(false == hasValue) 
				 state				= ARGS_STATE_OPTION_VALUE;
			else {
				if_fail_fe(output.Options.push_back(option));
				option				= {};
			}
			continue;
		}
		if_fail_fe(output.Positionals.push_back(argument));
	}
	if(state == ARGS_STATE_OPTION_VALUE)
		if_fail_fe(output.Options.push_back(option));
	rtrn output.Options.size() + output.Positionals.size();
}
