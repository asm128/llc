#include "llc_args.h"
#include "llc_enum.h"

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

namespace llc 
{
	GDEFINE_ENUM_TYPE(ARGS_STATE, llc::u0_t);
	GDEFINE_ENUM_VALUE(ARGS_STATE, ARGUMENT		, 0);
	GDEFINE_ENUM_VALUE(ARGS_STATE, OPTION_VALUE	, 1);
	GDEFINE_ENUM_VALUE(ARGS_STATE, POSITIONAL	, 2);
} // namespace

sttc ::llc::err_t	argsOptionName			(::llc::vcst_c & argument, ::llc::kvvcst_t<::llc::vcst_t> & option) {
	::llc::b8_c				isDoubleDash		= '-' == argument[1];
	::llc::u0_c				prefixLen			= isDoubleDash ? 2U : 1U;
	::llc::u2_t				iChar				= prefixLen;
	for(; iChar < argument.size() && argument[iChar] != '='; ++iChar) 
		continue;

	option.Key			= {&argument[prefixLen], iChar - prefixLen};

	llc::b8_c				hasValue			= iChar < argument.size(); // If we found an '=' character, then the option has a value.
	if(not hasValue) 
		return 0;

	option.Val			=  {&argument[iChar + 1], argument.size() - iChar - 1};
	rtrn option.Val.size();
}

::llc::err_t		llc::argsParse		(::llc::SCommandLineArgs & output, ::llc::view<vcst_t> argv) {
	ARGS_STATE				state				= ARGS_STATE_ARGUMENT;
	kvvcst_t<vcst_t>		option				= {};
	output.ProgramName = argv[0];

	for(u2_t iArg = 1; iArg < argv.size(); ++iArg) {
		vcst_t					argument			= {argv[iArg], (u2_t)-1};
		if_zero_cwf(argument.size(), "iArg:(%" LLC_FMT_U2  ")", iArg);
		if(state != ARGS_STATE_ARGUMENT) {
				 if(state == ARGS_STATE_POSITIONAL)		{ if_fail_fe(output.Positionals.push_back(argument)); }
			else if(state != ARGS_STATE_OPTION_VALUE)	{ warning_printf("Unrecognized state! 0x%X(%s)", (u2_t)state, llc::get_value_namep(state)); }
			else { // state == ARGS_STATE_OPTION_VALUE, obviously
				option.Val			= argument; // The value of the current option is the next argument
				if_fail_fe(output.Options.push_back(option));
				state				= ARGS_STATE_ARGUMENT;
			}
			continue;
		}
		if(argument[0] != '-') {
			if_fail_fe(output.Positionals.push_back(argument));
			continue;
		}
		if(argument == vcsc_t{"--", 2}) {
			state				= ARGS_STATE_POSITIONAL;
			continue;
		}
		err_t				hasValue;
		if_fail_fe(hasValue = argsOptionName(argument, option));
		if(not hasValue) 
			state				= ARGS_STATE_OPTION_VALUE;
		else {
			if_fail_fe(output.Options.push_back(option));
			option				= {};
		}
	}
	if(state == ARGS_STATE_OPTION_VALUE)
		if_fail_fe(output.Options.push_back(option));
	rtrn output.Options.size() + output.Positionals.size();
}
