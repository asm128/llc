#include "llc_runtime.h"
#include "llc_file.h"
#include "llc_json.h"
#include "llc_json_expression.h"

stxp const char input_json [] = R"jenjson(
[
        { "properties" : [{ "name" : "color", "type" : "string"}, { "name" : "age", "type" : "int"} ]
        , "selection"  : {"index" : 1 , "active" : true }
        , "people"     :
                [{ "name"     : "David"
                 , "property" :
                        { "color" : "red", "age" : 25 }
                },
                { "name"      : "Charles"
                 , "property" :
                        { "color" : "green", "age" : 32 }
                },
                { "name"      : "Charles"
                 , "property" :
                        { "color" : "blue", "age" : 95 }
                },
                ],
        },
]
)jenjson";
stxp const char input_expression [] = "{people['2'].name}'s {properties[selection.index].name}: {people['2'].property.(properties[selection.index].name)}";

static ::llc::err_t jenStringFormat(::llc::vcst_t format, const ::llc::SJSONReader & reader, ::llc::asc_t & output) {
	return ::llc::expressionStringFormat(format, [&](::llc::vcst_c & expression, ::llc::vcst_t & resolved) {
		::llc::SExpressionReader parsed = {};
		if_fail_fe(::llc::expressionReaderParse(parsed, expression));
		const ::llc::err_t result = ::llc::jsonExpressionResolve(parsed, reader, 0, resolved);
		return result == -1 ? -1 : 0;
	}, output);
}

sttc ::llc::err_t jen_entry_point(::llc::SRuntimeValues & runtimeValues);
LLC_OS_ENTRY_POINT(::jen_entry_point);

sttc ::llc::err_t jen_entry_point(::llc::SRuntimeValues & runtimeValues) {
	cnst ::llc::SCommandLineArgs & args = runtimeValues.EntryPointArgs;
	::llc::SJSONFile file;
	if(2 != args.Order.size()) {
		printf("usage: %.*s [-f] [json] [-f] [expression]\ninput_json:\n%s\ninput_expression:\n%s\n", LLC_VIEW_ARGS(args.ProgramName), input_json, input_expression);
		const ::llc::vcst_t demoJson = {input_json, sizeof(input_json) - 1};
		const ::llc::vcst_t demoExpression = {input_expression, sizeof(input_expression) - 1};
		if_fail_fe(llc::jsonParse(file.Reader, demoJson));
		::llc::err_t demoRoot = {};
		if_fail_fe(demoRoot = llc::jsonArrayValueGet(file.Reader, 0, 0));
		::llc::SJSONReader demoReader = {};
		if_fail_fe(llc::jsonParse(demoReader, file.Reader.View[demoRoot]));
		::llc::asc_t demoResult = {};
		if_fail_fe(::jenStringFormat(demoExpression, demoReader, demoResult));
		printf("result:\n%.*s\n", LLC_VIEW_ARGS(demoResult));
		return 0 == args.Order.size() ? 0 : -1;
	}
	::llc::vcst_t input[2] = {};
	bool fromFile[2] = {};
	::llc::u2_t iOption = 0, iPositional = 0;
	for(::llc::u2_t iInput = 0; iInput < 2; ++iInput) {
		if(1 == args.Order[iInput]) {
			fromFile[iInput] = 0 != args.OptionStarts[iOption];
			if(fromFile[iInput])
				if_true_fef((args.Options[iOption].Key != ::llc::vcst_t{"f", 1}) || !args.Options[iOption].Val.size(), "%s", "Expected -f followed by a filename.");
			input[iInput] = args.Options[iOption++].Val;
		}
		else
			input[iInput] = args.Positionals[iPositional++];
	}
	if(fromFile[0]) {
		if_fail_fe(llc::jsonFileRead(file, input[0]));
	}
	else {
		if_fail_fe(llc::jsonParse(file.Reader, input[0]));
	}
	::llc::string expressionBytes = {};
	if(fromFile[1]) {
		if_fail_fe(llc::fileToMemory(input[1], expressionBytes));
		if_fail_fe(expressionBytes.slice(input[1], 0));
	}
	::llc::asc_t output = {};
	if_fail_fe(::jenStringFormat(input[1], file.Reader, output));
	printf("%.*s\n", LLC_VIEW_ARGS(output));
	return 0;
}
