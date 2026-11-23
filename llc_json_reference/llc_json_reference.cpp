#include "llc_json_expression.h"
#include "llc_parse.h"

static constexpr char exampleJSON[] = R"({"name":"Ada","age":37,"active":true,"ratio":1.5,"items":[10,20]})";

LLC_USING_TYPEINT();
LLC_USING_VIEW();


static void printSection(const char* title) {
    printf("\n  %s\n  ------------------------------------------------------------\n", title);
}

static llc::err_t demoJSONSteps(const llc::vcst_t json) {
    llc::SJSONReader reader = {};
    for(reader.StateRead.IndexCurrentChar = 0; reader.StateRead.IndexCurrentChar < json.size(); ++reader.StateRead.IndexCurrentChar)
        if_fail_fe(llc::jsonParseStep(reader, json));
    if_true_fef(reader.StateRead.NestLevel, "If nesting level is not zero, it means there is still document to parse. In this case, the nesting level is:%i.",reader.StateRead.NestLevel);
    printf("  jsonParseStep       : %u tokens\n", reader.Token.size());
    return 0;
}

static llc::err_t demoJSONAccess(const llc::SJSONReader& reader) {
    llc::vcst_t name = {};
    llc::b8_t active = {};
    llc::u2_t age = {};
    if_fail_fe(llc::jsonObjectGetAsString(reader, 0, "name", name));
    if_fail_fe(llc::jsonObjectGetAsBoolean(reader, 0, "active", active));
    if_fail_fe(llc::jsonObjectGetAsInteger(reader, 0, "age", age));
    printf("  accessors           : name=%.*s  age=%u  active=%u\n", LLC_VIEW_ARGS(name), age, (unsigned)active);
    llc::err_t items = {};
    if_fail_fe(items = llc::jsonObjectValueGet(reader, 0, "items"));
    llc::err_t count = {};
    if_fail_fe(count = llc::jsonArraySize(reader, items));
    printf("  array size          : %i\n", count);
    llc::err_t second = {};
    if_fail_fe(second = llc::jsonArrayValueGet(reader, items, 1));
    printf("  array[1]            : %.*s\n", LLC_VIEW_ARGS(reader.View[second]));
    llc::string written = {};
    if_fail_fe(llc::jsonWrite(reader.Tree[0], reader.View, written));
    printf("  jsonWrite           : %.*s\n", LLC_VIEW_ARGS(written));
    return 0;
}

static llc::err_t demoJSONNavigation(const llc::SJSONReader& reader) {
    llc::err_t keys = {};
    if_fail_fe(keys = llc::jsonObjectKeyCount(reader, 0));
    llc::aobj<::llc::vcst_t> names = {};
    if_fail_fe(llc::jsonObjectKeyList(reader, 0, names));
    printf("  object keys         : %i (%u listed)\n", keys, names.size());
    for(llc::u2_t i = 0; i < names.size(); ++i)
        printf("    key[%u]            : %.*s\n", i, LLC_VIEW_ARGS(names[i]));
    llc::as2_t keyIndices = {};
    if_fail_fe(llc::jsonObjectKeyList(reader, 0, keyIndices));
    llc::as2_t bothIndices = {};
    llc::aobj<vcst_t> bothNames = {};
    if_fail_fe(llc::jsonObjectKeyList(reader, 0, bothIndices, bothNames));
    printf("  key index overloads : %u, %u\n", keyIndices.size(), bothIndices.size());
    llc::as2_t nodeIndices = {};
    llc::aobj<vcst_t> nodeNames = {};
    if_fail_fe(llc::jsonObjectKeyList(*reader.Tree[0], nodeIndices));
    if_fail_fe(llc::jsonObjectKeyList(*reader.Tree[0], reader.View, nodeNames));
    llc::as2_t nodeBothIndices = {};
    llc::aobj<vcst_t> nodeBothNames = {};
    if_fail_fe(llc::jsonObjectKeyList(*reader.Tree[0], reader.View, nodeBothIndices, nodeBothNames));
    printf("  node key overloads  : %u, %u, %u\n", nodeIndices.size(), nodeNames.size(), nodeBothNames.size());
    llc::err_t nameIndex = {};
    if_fail_fe(nameIndex = llc::jsonObjectValueGet(reader, 0, "name"));
    llc::vcst_t name = {}, cName = {};
    if_fail_fe(llc::jsonObjectGetString(reader, nameIndex, name));
    if_fail_fe(llc::jsonObjectGetCString(reader, nameIndex, cName));
    printf("  node string         : %.*s (C string: %.*s)\n", LLC_VIEW_ARGS(name), LLC_VIEW_ARGS(cName));
    llc::err_t ageIndex = {}, ratioIndex = {}, activeIndex = {}, itemsIndex = {};
    if_fail_fe(ageIndex = llc::jsonObjectValueGet(reader, 0, "age"));
    if_fail_fe(ratioIndex = llc::jsonObjectValueGet(reader, 0, "ratio"));
    if_fail_fe(activeIndex = llc::jsonObjectValueGet(reader, 0, "active"));
    if_fail_fe(itemsIndex = llc::jsonObjectValueGet(reader, 0, "items"));
    if_fail_fe(llc::jsonObjectKeyCount(*reader.Tree[0]));
    if_fail_fe(llc::jsonArraySize(*reader.Tree[itemsIndex]));
    if_fail_fe(llc::jsonArrayValueGet(*reader.Tree[itemsIndex], 1));
    if_fail_fe(llc::jsonObjectValueGet(*reader.Tree[0], reader.View, "name"));
    llc::f3_t decimal = {};
    llc::b8_t truth = {};
    llc::s3_t integer = {};
    if_fail_fe(llc::jsonObjectGetDecimal(reader, ratioIndex, decimal));
    if_fail_fe(llc::jsonObjectGetInteger(reader, ageIndex, integer));
    if_fail_fe(llc::jsonObjectGetBoolean(reader, activeIndex, truth));
    printf("  scalar nodes        : decimal=%g  integer=%lld  boolean=%u\n", decimal, (long long)integer, (unsigned)truth);
    llc::aobj<llc::string> parts = {};
    if_fail_fe(llc::jsonArraySplit(*reader.Tree[itemsIndex], reader.View, 1, parts));
    printf("  jsonArraySplit      : %u parts\n", parts.size());
    for(llc::u2_t i = 0; i < parts.size(); ++i)
        printf("    part[%u]           : %.*s\n", i, LLC_VIEW_ARGS(parts[i]));
    printSection("JSON comparisons (library diagnostics follow)");
    fflush(stdout);
    if_true_fef(1 != llc::jsonCompareNumber	(*reader.Tree[ageIndex]     , *reader.Tree[ageIndex]    , reader.View)			            		, "%s", "JSON comparison mismatch.");
    if_true_fef(1 != llc::jsonCompareArray	(*reader.Tree[itemsIndex]   , *reader.Tree[itemsIndex]  , reader.View)			                	, "%s", "JSON comparison mismatch.");
    if_true_fef(1 != llc::jsonCompareObject	(*reader.Tree[0]            , *reader.Tree[0]           , reader.View)						    	, "%s", "JSON comparison mismatch.");
    if_true_fef(1 != llc::jsonCompareNumber	(*reader.Tree[ageIndex]	    , reader.View               , *reader.Tree[ageIndex], reader.View)		, "%s", "JSON comparison mismatch.");
    if_true_fef(1 != llc::jsonCompareArray	(*reader.Tree[itemsIndex]	, reader.View               , *reader.Tree[itemsIndex], reader.View)	, "%s", "JSON comparison mismatch.");
    if_true_fef(1 != llc::jsonCompareObject	(*reader.Tree[0]			, reader.View               , *reader.Tree[0], reader.View)		    	, "%s", "JSON comparison mismatch.");
    printf("  comparisons        : 6 passed\n");
    return 0;
}

static llc::err_t demoNumericHelpers() {
    const llc::vcst_t decimalText = "123rest";
    const llc::vcst_t hexText = "2a!";
    llc::u2_t decimal = {}, hex = {};
    llc::err_t digits = {};
    if_fail_fe(digits = llc::parseIntegerDecimal(decimalText, decimal));
    printf("  decimal             : %u from %i digits\n", decimal, digits);
    if_fail_fe(digits = llc::parseIntegerHexadecimal(hexText, hex));
    printf("  hexadecimal         : %u from %i digits\n", hex, digits);
    llc::u3_t binary = {};
    if_fail_fe(digits = llc::parseArbitraryBaseInteger(2, "01", "101", &binary));
    printf("  binary              : %" LLC_FMT_U3 "from %i digits\n", binary, digits);
     llc::vcst_c spaces = " \tname";
    llc::u2_t cursor = 0;
    if_fail_fe(llc::skipToNextCharacter(cursor, spaces));
    printf("  first nonspace      : offset=%u  space classified=%u\n", cursor, (unsigned)llc::isSpaceCharacter(' '));
    return 0;
}

static llc::err_t demoScalarOverloads(const llc::SJSONReader& reader) {
    llc::err_t age = {}, ratio = {}, active = {};
    if_fail_fe(age = llc::jsonObjectValueGet(reader, 0, "age"));
    if_fail_fe(ratio = llc::jsonObjectValueGet(reader, 0, "ratio"));
    if_fail_fe(active = llc::jsonObjectValueGet(reader, 0, "active"));
    llc::u0_t u0 = {}; llc::u1_t u1 = {}; llc::u2_t u2 = {}; llc::u3_t u3 = {};
    llc::s0_t s0 = {}; llc::s1_t s1 = {}; llc::s2_t s2 = {}; llc::s3_t s3 = {};
    if_fail_fe(llc::jsonObjectGetInteger(reader, age, u0));
    if_fail_fe(llc::jsonObjectGetInteger(reader, age, u1));
    if_fail_fe(llc::jsonObjectGetInteger(reader, age, u2));
    if_fail_fe(llc::jsonObjectGetInteger(reader, age, u3));
    if_fail_fe(llc::jsonObjectGetInteger(reader, age, s0));
    if_fail_fe(llc::jsonObjectGetInteger(reader, age, s1));
    if_fail_fe(llc::jsonObjectGetInteger(reader, age, s2));
    if_fail_fe(llc::jsonObjectGetInteger(reader, age, s3));
    if_fail_fe(llc::jsonObjectGetAsInteger(reader, 0, "age", u0));
    if_fail_fe(llc::jsonObjectGetAsInteger(reader, 0, "age", u1));
    if_fail_fe(llc::jsonObjectGetAsInteger(reader, 0, "age", u2));
    if_fail_fe(llc::jsonObjectGetAsInteger(reader, 0, "age", u3));
    if_fail_fe(llc::jsonObjectGetAsInteger(reader, 0, "age", s0));
    if_fail_fe(llc::jsonObjectGetAsInteger(reader, 0, "age", s1));
    if_fail_fe(llc::jsonObjectGetAsInteger(reader, 0, "age", s2));
    if_fail_fe(llc::jsonObjectGetAsInteger(reader, 0, "age", s3));
    printf("  integer widths      : %u %u %u %llu; %i %i %i %lld\n", (unsigned)u0, (unsigned)u1, u2, (unsigned long long)u3, (int)s0, (int)s1, s2, (long long)s3);
    llc::f2_t f2 = {}; llc::f3_t f3 = {}; llc::b8_t truth = {};
    if_fail_fe(llc::jsonObjectGetDecimal(reader, ratio, f2));
    if_fail_fe(llc::jsonObjectGetDecimal(reader, ratio, f3));
    if_fail_fe(llc::jsonObjectGetAsDecimal(reader, 0, "ratio", f2));
    if_fail_fe(llc::jsonObjectGetAsDecimal(reader, 0, "ratio", f3));
    printf("  decimal widths      : %g %g\n", (double)f2, (double)f3);
    if_fail_fe(llc::jsonObjectGetIntegerAsBoolean(reader, age, truth));
    printf("  integer as Boolean  : %u\n", (unsigned)truth);
    if_fail_fe(llc::jsonObjectGetIntegerAsDecimal(reader, age, f3));
    printf("  integer as decimal  : %g\n", (double)f3);
    if_fail_fe(llc::jsonObjectGetDecimalAsInteger(reader, ratio, s3));
    printf("  decimal as integer  : %lld\n", (long long)s3);
    if_fail_fe(llc::jsonObjectGetDecimalAsBoolean(reader, ratio, truth));
    printf("  decimal as Boolean  : %u\n", (unsigned)truth);
    if_fail_fe(llc::jsonObjectGetBooleanAsInteger(reader, active, s3));
    printf("  Boolean as integer  : %lld\n", (long long)s3);
    if_fail_fe(llc::jsonObjectGetBooleanAsDecimal(reader, active, f3));
    printf("  Boolean as decimal  : %g\n", (double)f3);
    printf("  scalar overloads    : eight integer widths, two decimal widths, six conversions\n");
    return 0;
}

static llc::err_t demoJSONParse(llc::SJSONReader& reader, const llc::vcst_t json) {
    if_fail_fe(llc::jsonParse(reader, json));
    printf("  jsonParse           : %u tokens, %u nodes\n", reader.Token.size(), reader.Tree.size());
    if_fail_fe(demoJSONAccess(reader));
    return 0;
}

static llc::err_t demoTreeRebuild(const llc::vcst_t jsonText) {
    llc::SJSONReader reader = {};
    if_fail_fe(llc::jsonParse(reader, jsonText, false, true));
    if_fail_fe(llc::jsonTreeRebuild(reader.Token, reader.Tree));
    printf("  jsonTreeRebuild     : %u nodes\n", reader.Tree.size());
    if_fail_fe(reader.Reset());
    printf("  Reset               : %u tokens, %u views\n", reader.Token.size(), reader.View.size());
    return 0;
}

static llc::err_t demoFieldMapping() {
    const llc::SJSONFieldBinding fields[] = {"name", llc::JSON_TYPE_STRING, "age", llc::JSON_TYPE_INTEGER};
    const llc::kv<llc::vcst_t> maps[] = {{"age", "37"}, {"name", "Ada"}};
    llc::as2_t fieldIndices = {}, mapIndices = {};
    if_fail_fe(llc::jsonMapToFields(fieldIndices, fields, maps));
    if_fail_fe(llc::jsonFieldsToMap(mapIndices, fields, maps));
    printf("  field maps          : %u forward, %u reverse\n", fieldIndices.size(), mapIndices.size());
    for(llc::u2_t i = 0; i < fieldIndices.size(); ++i)
        printf("    map[%u] -> field[%i]\n", i, (int)fieldIndices[i]);
    for(llc::u2_t i = 0; i < mapIndices.size(); ++i)
        printf("    field[%u] -> map[%i]\n", i, (int)mapIndices[i]);
    return 0;
}

static llc::err_t demoExpressionSteps(const llc::vcst_t expression) {
    llc::SExpressionReader reader = {};
    for(reader.StateRead.IndexCurrentChar = 0; reader.StateRead.IndexCurrentChar < expression.size(); ++reader.StateRead.IndexCurrentChar)
        if_fail_fe(llc::expressionReaderParseStep(reader, expression));
    if_true_fef(reader.StateRead.NestLevel, "Expression nesting did not close. Nest level: %u", reader.StateRead.NestLevel);
    printf("  expressionReaderParseStep : %u tokens\n", reader.Token.size());
    return 0;
}

static llc::err_t demoResolve(const llc::SJSONReader& json, const llc::vcst_t expression) {
    llc::SExpressionReader reader = {};
    if_fail_fe(llc::expressionReaderParse(reader, expression));
    llc::SJSONExpressionSolver solver = {};
    llc::vcst_t result = {};
    if_true_fef(-1 == llc::jsonExpressionResolve(reader, json, 0, solver, result), "Expression resolution failed. Expression: %.*s", LLC_VIEW_ARGS(expression));
    printf("  %-38.*s => %.*s\n", LLC_VIEW_ARGS(expression), LLC_VIEW_ARGS(result));
    return 0;
}

static llc::err_t demoResolveOverloads(const llc::SJSONReader& json) {
    llc::vcst_c expression = "name";
    llc::SExpressionReader reader = {};
    if_fail_fe(llc::expressionReaderParse(reader, expression));
    llc::vcst_t result = {};
    if_fail_fe(llc::jsonExpressionResolve(reader, json, 0, result));
    if_fail_fe(llc::jsonExpressionResolve(reader, json, 0));
    if_fail_fe(llc::jsonExpressionResolve(expression, json, 0, result));
    if_fail_fe(llc::jsonExpressionResolve(expression, json, 0));
    llc::SEvaluationStepResult step = {};
    if_fail_fe(step.SetBoolCarry(true, result));
    printf("  resolve overloads   : name and Boolean carry %.*s\n", LLC_VIEW_ARGS(result));
    return 0;
}

static llc::err_t demoTypes(const llc::SJSONReader& json) {
    const llc::SJSONToken       & jsonToken    = json.Token[0];
    const llc::SJSONNode        & jsonNode     = *json[0];
    const llc::SJSONReaderState & jsonState    = json.StateRead;
    const llc::JSON_TYPE        jsonType       = jsonToken.Type;
    const llc::json_id_t        jsonId         = jsonNode.ObjectIndex;
    printf("  reader range        : %td nodes\n", json.end() - json.begin());
    llc::SJSONFile file = {};
    llc::TKeyValJSONFile keyedFile = {};
    (void)file; (void)keyedFile;
    printf("  JSON types          : id=%i  type=%i  cursor=%u\n", (int)jsonId, (int)jsonType, jsonState.IndexCurrentChar);
    llc::SExpressionReader expression = {};
    const llc::vcst_t expressionText = "name";
    if_fail_fe(llc::expressionReaderParse(expression, expressionText));
    const llc::SExpressionToken       & expressionToken = expression.Token[0];
    const llc::SExpressionNode        & expressionNode = *expression.Tree[0];
    const llc::SExpressionReaderState & expressionState = expression.StateRead;
    const llc::EXPRESSION_READER_TYPE expressionType = expressionToken.Type;
    printf("  expression types    : type=%i  children=%u  cursor=%u\n", (int)expressionType, expressionNode.Children.size(), expressionState.IndexCurrentChar);
    llc::SJSONExpressionSolver solver = {};
    llc::vcst_t result = {};
    if_true_fef(-1 == llc::jsonExpressionResolve(expression, json, 0, solver, result), "Expression resolution failed. Expression: %.*s", LLC_VIEW_ARGS(expressionText));
    if_zero_fef(solver.Results.size(), "No evaluation steps were recorded. Expression: %.*s", LLC_VIEW_ARGS(expressionText));
    const llc::SEvaluationStepResult& evaluation = solver.Results[0];
    printf("  evaluation step     : root=%u  result=%i\n", evaluation.IndexRootJSONNode, evaluation.IndexJSONResult);
    llc::SStripLiteralState stripState = {};
    llc::apod<llc::SStripLiteralType> strips = {};
    if_fail_fe(llc::stripLiteralParse(stripState, strips, "Hello, {name}!"));
    const llc::STRIP_LITERAL_TYPE stripType = strips[0].Type;
    printf("  strip types         : type=%i  spans=%u  cursor=%u\n", (int)stripType, strips.size(), stripState.IndexCurrentChar);
    return 0;
}

static llc::err_t demoLiteralSplit(const llc::vcst_t format) {
    llc::SStripLiteralState state = {};
    llc::apod<llc::SStripLiteralType> types = {};
    if_fail_fef(llc::stripLiteralParse(state, types, format), "%.+s", LLC_VIEW_ARGS(format));
    llc::aobj<llc::vcst_t> views = {};
    if_fail_fef(llc::stripLiteralGetViews(views, types, format), "%.+s", LLC_VIEW_ARGS(format));
    for(llc::u2_t i = 0; i < views.size(); ++i)
        printf("    span[%u] type=%i  : %.*s\n", i, (int)types[i].Type, LLC_VIEW_ARGS(views[i]));
    return 0;
}

static llc::err_t demoLiteralSteps(const llc::vcst_t format) {
    llc::SStripLiteralState state = {};
    llc::apod<llc::SStripLiteralType> spans = {};
    for(state.IndexCurrentChar = 0; state.IndexCurrentChar < format.size(); ++state.IndexCurrentChar) {
        state.CharCurrent = format[state.IndexCurrentChar];
        if_fail_fe(llc::stripLiteralParseStep(state, spans, format));
    }
    printf("  stripLiteralParseStep : %u spans\n", spans.size());
    return 0;
}

static llc::err_t demoFormat(const llc::SJSONReader& json, const llc::vcst_t format) {
    llc::asc_t result = {};
    if_fail_fe(llc::jsonStringFormat(format, json, 0, result));
    printf("  jsonStringFormat    : %.*s\n", LLC_VIEW_ARGS(result));
    return 0;
}

static llc::err_t demoExpressionExamples() {
    const llc::vcst_t jsonText = R"({"name":"Ada","alive":true,"married":false,"selection":{"active":true,"index":1},"children":["Rene",null,"Jamie"],"parents":[{}, {"property":{"color":"green"}}],"properties":[{"name":"age"},{"name":"color"}]})";
    llc::SJSONReader json = {};
    if_fail_fe(llc::jsonParse(json, jsonText));
    if_fail_fe(demoResolve(json, "children[('2')]"));
    if_fail_fe(demoResolve(json, "'Ada'"));
    if_fail_fe(demoResolve(json, "selection.active ? 'yes' : 'no'"));
    if_fail_fe(demoResolve(json, "alive && !married"));
    if_fail_fe(demoResolve(json, "alive || married"));
    if_fail_fe(demoResolve(json, "!married"));
    if_fail_fe(demoResolve(json, "selection.index == selection.index"));
    if_fail_fe(demoResolve(json, "parents['1'].property.(properties[selection.index].name)"));
    return 0;
}

int main(int argc, char** argv) {
    const llc::vcst_t jsonText = exampleJSON;
    printf("LLC JSON reference\n==================\n  Input: %.*s\n", LLC_VIEW_ARGS(jsonText));
    printSection("JSON parsing and access");
    if_fail_fe(demoJSONSteps(jsonText));
    llc::SJSONReader json = {};
    if_fail_fe(demoJSONParse(json, jsonText));
    printSection("Tree and object navigation");
    if_fail_fe(demoTreeRebuild(jsonText));
    if_fail_fe(demoJSONNavigation(json));
    printSection("Scalar conversions");
    if_fail_fe(demoScalarOverloads(json));
    printSection("Field mapping and numeric parsing");
    if_fail_fe(demoFieldMapping());
    if_fail_fe(demoNumericHelpers());
    printSection("Expression parsing and resolution");
    if_fail_fe(demoExpressionSteps("name"));
    if_fail_fe(demoResolve(json, "name"));
    if_fail_fe(demoResolve(json, "age"));
    if_fail_fe(demoResolve(json, "active"));
    if_fail_fe(demoResolveOverloads(json));
    printSection("Reader and token types");
    if_fail_fe(demoTypes(json));
    printSection("Literal splitting and formatting");
    if_fail_fe(demoLiteralSplit("Hello, {name}!"));
    if_fail_fe(demoLiteralSteps("Hello, {name}!"));
    if_fail_fe(demoFormat(json, "Hello, {name}!"));
    if_fail_fe(demoFormat(json, "Literal \\{name}; value {name}."));
    printSection("Expression examples");
    if_fail_fe(demoExpressionExamples());
    if(argc > 1) {
        printSection("JSON file input");
        llc::SJSONFile file = {};
        if_fail_fe(llc::jsonFileRead(file, llc::vcst_t{argv[1], (llc::u2_t)-1}));
        printf("  jsonFileRead        : %u tokens\n", file.Reader.Token.size());
    }
    printf("\n  All examples completed.\n\n");
    return 0;
}
