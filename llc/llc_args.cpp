#include "llc_args.h"

llc::err_t			llc::viewsFromEnvp(llc::aobj<vcst_t> & outputViews, char * envp[]) {
	if_null_fe(envp);
	u2_t iVar = 0;
	for (; envp[iVar]; ++iVar) {
		if_fail_fe(outputViews.push_back({envp[iVar], (u2_t)-1}));
	}
	rtrn iVar;
}

llc::err_t			llc::viewsFromArgv(llc::aobj<vcst_t> & outputViews, u2_t argc, char * argv[]) {
	if_zero_vi(0, argc);
	if_null_fe(argv);
	u2_t iArg = 0;
	for(; iArg < argc; ++iArg) {
		if_fail_fe(outputViews.push_back({argv[iArg], (u2_t)-1}));
	}
	rtrn iArg;
}
llc::err_t			llc::argsOptionValue	(const SCommandLineArgs & input, vcst_t key, vcst_t & output)	{
	output							= {};
	err_c					optionIndex			= argsOptionIndex(input, key);
	if_fail_fwf(optionIndex, "Option not found:'%.*s'.", (int)key.size(), key.begin());
	if_true_vef(-2, 0 <= argsOptionIndex(input, key, (u2_t)optionIndex + 1), "Option has multiple values:'%.*s'.", (int)key.size(), key.begin());
	output							= input.Options[optionIndex].Val;
	rtrn optionIndex;
}

llc::err_t			llc::argsOptionValues	(const SCommandLineArgs & input, vcst_t key, aobj<vcst_t> & output)	{
	u2_c					outputStart			= output.size();
	for(const kvvcst_t<vcst_t> & option : input.Options)
		if(option.Key == key)
			if_fail_fe(output.push_back(option.Val));
	rtrn output.size() - outputStart;
}

sttc ::llc::err_t	argsOptionName			(::llc::vcst_c & argument, ::llc::kvvcst_t<::llc::vcst_t> & option) {
	if_zero_fw(argument.size());
	::llc::b8_c				isDoubleDash		= argument.size() > 1 && '-' == argument[1];
	::llc::u0_c				prefixLen			= isDoubleDash ? 2U : 1U;
	::llc::u2_t				iChar				= prefixLen;
	for(; iChar < argument.size() && argument[iChar] != '='; ++iChar) 
		continue;

	if_fail_fe(argument.slice(option.Key, prefixLen, iChar - prefixLen));
	if_zero_fef(option.Key.size(), "Option has no name:'%.*s'.", (int)argument.size(), argument.begin());

	llc::b8_c				hasValue			= iChar < argument.size(); // If we found an '=' character, then the option has a value.
	if(hasValue)
		if_fail_fe(argument.slice(option.Val, iChar + 1));
	rtrn hasValue;
}


::llc::err_t		llc::argsParse			(SCommandLineArgs & output, int argc, char ** argv, char ** envp) {
	llc::aobj<llc::vcst_t> arguments, envvars;											
	if_fail_fe(viewsFromArgv(arguments, argc, argv));
	if(envp)
		if_fail_fe(viewsFromEnvp(envvars, envp));
	return argsParse(output, arguments, envvars);
}

::llc::err_t		llc::argsParse		(::llc::SCommandLineArgs & output, ::llc::view<cnst vcst_t> argv, ::llc::view<cnst vcst_t> envp) {
	output							= {};
	output.Environment				= envp;
	if_zero_vw(0, argv.size()); // Exit early if no argv: nothing to do

	output.ProgramName				= argv[0];

	kvvcst_t<vcst_t>		option				= {};
	bool					hasOptionValues		= false;
	bool					positionalOnly		= false;
	for(u2_t iArg = 1; iArg < argv.size(); ++iArg) {
		vcst_t					argument			= argv[iArg];
		if(positionalOnly) {
			if_fail_fe(output.Positionals.push_back(argument));
			continue;
		}
		if(argument == LLC_CXS("--")) {
			if(option.Key.size() && false == hasOptionValues) {
				if_fail_fe(output.Options.push_back(option));
			}
			option				= {};
			positionalOnly		= true;
			continue;
		}
		if(1 >= argument.size() || '-' != argument[0]) {
			if(0 == option.Key.size()) {
				if_fail_fe(output.Positionals.push_back(argument));
			}
			else {
				if_fail_fe(output.Options.emplace_back(option.Key, argument));
				hasOptionValues	= true;
			}
			continue;
		}
		if(option.Key.size() && false == hasOptionValues) {
			if_fail_fe(output.Options.push_back(option));
		}
		option				= {};
		hasOptionValues		= false;
		err_t					hasValue			= {};
		if_fail_fe(hasValue = argsOptionName(argument, option));
		if(hasValue) {
			if_fail_fe(output.Options.push_back(option));
			option				= {};
		}
	}
	if(option.Key.size() && false == hasOptionValues)
		if_fail_fe(output.Options.push_back(option));
	rtrn output.Options.size() + output.Positionals.size();
}
