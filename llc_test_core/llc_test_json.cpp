#include "llc_json.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(JSON_READER_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, OK					, 0, "All JSON reader tests passed.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, PARSE					, 1, "jsonParse() rejected a valid JSON document.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, STORAGE_COUNTS			, 2, "SJSONReader token, view and tree counts did not match.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, TOKEN_VIEW				, 3, "A JSON token view did not match its source span.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, TREE_NODE				, 4, "A JSON tree node did not reference its matching token or parent.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, READER_STATE			, 5, "SJSONReader did not finish a valid document in a closed state.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, ROOT_TYPE				, 6, "The semantic JSON root had an unexpected type.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, WRITE					, 7, "jsonWrite() failed to serialize a parsed document.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, ROUND_TRIP				, 8, "A parsed JSON document did not serialize to its expected canonical text.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, COUNTED_INPUT			, 9, "jsonParse() depended on a null terminator beyond its counted input.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, RESET					, 10, "SJSONReader::Reset() did not clear its input and parser state while retaining the tree cache.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, INTEGER_VALUE			, 11, "SJSONReader did not preserve a parsed signed integer value.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, TREE_REUSE				, 12, "SJSONReader did not reuse and correctly rebuild its cached tree storage.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, OPTION_LAYOUT			, 13, "JSON parse-option flags did not occupy one uniform integer or used overlapping masks.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, OPTION_PRESETS			, 14, "A JSON parse-option preset did not contain its documented flags.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, OPTION_MUTATION			, 15, "Generic LLC bit operations did not set, clear or test JSON parse-option flags correctly.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, OPTION_OUTPUTS			, 16, "JSON output-building flags did not control view or tree generation.");

static_assert(szof(::llc::JSON_PARSE_OPTION) == szof(::llc::u1_t), "JSON parse options must use one uniform 16-bit field.");
static_assert(szof(::llc::SJSONParseOptions) == szof(::llc::JSON_PARSE_OPTION), "bit_field<> must not add storage overhead.");

sttc bool jsonTextMismatch(::llc::vcsc_c & actual, ::llc::vcsc_c & expected) {
	rtrn actual.size() != expected.size() || (actual.size() && 0 != memcmp(actual.begin(), expected.begin(), actual.size()));
}

sttc ::llc::err_t testJSONStructure(ATestError & errors, cnst ::llc::SJSONReader & reader, ::llc::vcsc_c & input) {
	LLC_TEST_REQUIRE(errors, JSON_READER_TEST_RESULT_STORAGE_COUNTS
		, reader.Token.size() != reader.View.size() || reader.Token.size() != reader.Tree.size()
		, "Storage count mismatch. tokens:%u, views:%u, tree:%u."
		, reader.Token.size(), reader.View.size(), reader.Tree.size()
		);
	for(::llc::u2_t iToken = 0; iToken < reader.Token.size(); ++iToken) {
		cnst ::llc::SJSONToken & token = reader.Token[iToken];
		cnst ::llc::vcsc_t & tokenView = reader.View[iToken];
		cnst ::llc::pobj<::llc::SJSONNode> & node = reader.Tree[iToken];
		cnst bool nodeMissing = 0 == node.get_ref();
		cnst bool hasParent = token.ParentIndex >= 0 && (::llc::u2_t)token.ParentIndex < reader.Tree.size();
		cnst bool parentMismatch = false == nodeMissing && (hasParent ? node->Parent != reader.Tree[token.ParentIndex].operator->() : 0 != node->Parent);
		LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_TOKEN_VIEW
			, token.Span.Begin > token.Span.End || token.Span.End > input.size()
			|| tokenView.size() != token.Span.End - token.Span.Begin || tokenView.begin() != (token.Span.Begin < input.size() ? &input[token.Span.Begin] : input.end())
			, "Token %u view mismatch. type:%i, span:%u..%u, input:%u, view:%p/%u."
			, iToken, (::llc::s2_t)token.Type, token.Span.Begin, token.Span.End, input.size(), tokenView.begin(), tokenView.size()
			);
		LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_TREE_NODE
			, nodeMissing || node->Token != &reader.Token[iToken] || node->ObjectIndex != (::llc::json_id_t)iToken || parentMismatch
			, "Tree node %u mismatch. node:%p, token:%p/%p, index:%i, parent:%p/%p."
			, iToken, node.get_ref(), nodeMissing ? 0 : node->Token, &reader.Token[iToken]
			, nodeMissing ? -1 : node->ObjectIndex, nodeMissing ? 0 : node->Parent, hasParent ? reader.Tree[token.ParentIndex].operator->() : 0
			);
	}
	rtrn 0;
}

sttc ::llc::err_t testJSONDocument
	(ATestError & errors, ::llc::vcsc_c & input, ::llc::vcsc_c & expected, ::llc::JSON_TYPE expectedType, JSON_READER_TEST_RESULT parseResult = JSON_READER_TEST_RESULT_PARSE) {
	::llc::SJSONReader reader;
	cnst ::llc::err_t result = ::llc::jsonParse(reader, input);
	LLC_TEST_REQUIRE(errors, parseResult, ::llc::failed(result)
		, "Valid JSON parse failed. result:%i, input:'%.*s'."
		, result, (int)input.size(), input.begin()
		);
	if_fail_fe(testJSONStructure(errors, reader, input));
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_READER_STATE
		, false == reader.StateRead.DoneReading || reader.StateRead.NestLevel || reader.StateRead.IndexCurrentElement != -1 || reader.StateRead.CurrentElement
		, "Reader final state mismatch. done:%u, level:%i, index:%i, current:%p."
		, (::llc::u2_t)reader.StateRead.DoneReading, reader.StateRead.NestLevel, reader.StateRead.IndexCurrentElement, reader.StateRead.CurrentElement
		);
	LLC_TEST_REQUIRE(errors, JSON_READER_TEST_RESULT_ROOT_TYPE, 0 == reader.Tree.size() || 0 == reader.Tree[0].get_ref()
		, "Missing JSON root. tree nodes:%u."
		, reader.Tree.size()
		);
	cnst ::llc::pobj<::llc::SJSONNode> & treeRoot = reader.Tree[0];
	cnst ::llc::pobj<::llc::SJSONNode> & root = treeRoot->Token->Type == ::llc::JSON_TYPE_VALUE && treeRoot->Children.size() ? treeRoot->Children[0] : treeRoot;
	LLC_TEST_REQUIRE(errors, JSON_READER_TEST_RESULT_ROOT_TYPE, 0 == root.get_ref() || root->Token->Type != expectedType
		, "Root type mismatch. actual:%i, expected:%i, tree nodes:%u."
		, root.get_ref() ? (::llc::s2_t)root->Token->Type : -1, (::llc::s2_t)expectedType, reader.Tree.size()
		);
	::llc::asc_t output;
	cnst ::llc::err_t writeResult = ::llc::jsonWrite(reader.Tree[0], reader.View, output);
	LLC_TEST_REQUIRE(errors, JSON_READER_TEST_RESULT_WRITE, ::llc::failed(writeResult)
		, "JSON write failed. result:%i, root type:%i."
		, writeResult, (::llc::s2_t)root->Token->Type
		);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_ROUND_TRIP, jsonTextMismatch(output, expected)
		, "Round-trip mismatch. input:'%.*s', actual:'%.*s', expected:'%.*s'."
		, (int)input.size(), input.begin(), (int)output.size(), output.begin(), (int)expected.size(), expected.begin()
		);
	rtrn 0;
}

sttc ::llc::err_t testJSONValidDocuments(ATestError & errors) {
	if_fail_fe(testJSONDocument(errors, LLC_CXS("true"), LLC_CXS("true"), ::llc::JSON_TYPE_BOOLEAN));
	if_fail_fe(testJSONDocument(errors, LLC_CXS("-42"), LLC_CXS("-42"), ::llc::JSON_TYPE_INTEGER));
	if_fail_fe(testJSONDocument(errors, LLC_CXS("12.5"), LLC_CXS("12.5"), ::llc::JSON_TYPE_DECIMAL));
	if_fail_fe(testJSONDocument(errors, LLC_CXS("\"text\""), LLC_CXS("\"text\""), ::llc::JSON_TYPE_STRING));
	if_fail_fe(testJSONDocument(errors, LLC_CXS("[]"), LLC_CXS("[]"), ::llc::JSON_TYPE_ARRAY));
	if_fail_fe(testJSONDocument(errors, LLC_CXS("{}"), LLC_CXS("{}"), ::llc::JSON_TYPE_OBJECT));
	if_fail_fe(testJSONDocument(errors, LLC_CXS("[1,true,null,\"x\"]"), LLC_CXS("[1,true,null,\"x\"]"), ::llc::JSON_TYPE_ARRAY));
	if_fail_fe(testJSONDocument(errors, LLC_CXS("{\"name\":\"llc\",\"count\":3,\"enabled\":true}"), LLC_CXS("{\"name\":\"llc\",\"count\":3,\"enabled\":true}"), ::llc::JSON_TYPE_OBJECT));
	if_fail_fe(testJSONDocument(errors, LLC_CXS(" \n { \"a\" : [ 1, 2 ] } \t"), LLC_CXS("{\"a\":[1,2]}"), ::llc::JSON_TYPE_OBJECT));

	cnst ::llc::sc_t countedInput[] = {'t', 'r', 'u', 'e'};
	rtrn testJSONDocument(errors, {countedInput}, LLC_CXS("true"), ::llc::JSON_TYPE_BOOLEAN, JSON_READER_TEST_RESULT_COUNTED_INPUT);
}

sttc ::llc::err_t testJSONReset(ATestError & errors) {
	::llc::SJSONReader reader;
	if_fail_fe(::llc::jsonParse(reader, LLC_CXS("{\"values\":[1,2,3],\"flag\":true}")));
	::llc::u2_c treeSize = reader.Tree.size();
	cnst ::llc::view<const ::llc::pobj<::llc::SJSONNode>> treeStorage = {reader.Tree.begin(), reader.Tree.size()};
	cnst ::llc::view<const ::llc::gref<::llc::SJSONNode>> rootReference = {reader.Tree[0].get_ref(), 1};
	cnst ::llc::view<const ::llc::pobj<::llc::SJSONNode>> childStorage = {reader.Tree[0]->Children.begin(), reader.Tree[0]->Children.size()};
	cnst ::llc::err_t result = reader.Reset();
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_RESET
		, result || reader.Token.size() || reader.View.size() || reader.Tree.size() != treeSize
		|| reader.Tree.begin() != treeStorage.begin() || reader.Tree[0].get_ref() != rootReference.begin() || reader.Tree[0]->Children.begin() != childStorage.begin()
		|| reader.StateRead.IndexCurrentChar || reader.StateRead.IndexCurrentElement != -1 || reader.StateRead.CurrentElement
		|| reader.StateRead.NestLevel || reader.StateRead.CharCurrent || reader.StateRead.Escaping || reader.StateRead.InsideString
		|| reader.StateRead.ExpectingSeparator || reader.StateRead.DoneReading
		, "Reset mismatch. result:%i, tokens:%u, views:%u, tree:%u/%u, storage:%p/%p, root:%p/%p, children:%p/%p, position:%u, element:%i, level:%i."
		, result, reader.Token.size(), reader.View.size(), reader.Tree.size(), treeSize
		, reader.Tree.begin(), treeStorage.begin(), reader.Tree[0].get_ref(), rootReference.begin()
		, reader.Tree[0]->Children.begin(), childStorage.begin()
		, reader.StateRead.IndexCurrentChar, reader.StateRead.IndexCurrentElement, reader.StateRead.NestLevel
		);

	::llc::vcsc_c & input = LLC_CXS("{\"value\":-2}");
	if_fail_fe(::llc::jsonParse(reader, input));
	if_fail_fe(testJSONStructure(errors, reader, input));
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_TREE_REUSE
		, reader.Tree.begin() != treeStorage.begin() || reader.Tree[0].get_ref() != rootReference.begin() || reader.Tree[0]->Children.begin() != childStorage.begin()
		, "Tree storage changed after rebuild. tree:%p/%p, root:%p/%p, children:%p/%p, nodes:%u."
		, reader.Tree.begin(), treeStorage.begin(), reader.Tree[0].get_ref(), rootReference.begin()
		, reader.Tree[0]->Children.begin(), childStorage.begin(), reader.Tree.size()
		);
	rtrn 0;
}

sttc ::llc::err_t testJSONSignedInteger(ATestError & errors) {
	::llc::SJSONReader reader;
	if_fail_fe(::llc::jsonParse(reader, LLC_CXS("-42")));
	LLC_TEST_REQUIRE(errors, JSON_READER_TEST_RESULT_INTEGER_VALUE, 0 == reader.Tree.size() || 0 == reader.Tree[0].get_ref()
		, "Missing signed-integer root. tree nodes:%u."
		, reader.Tree.size()
		);
	cnst ::llc::pobj<::llc::SJSONNode> & treeRoot = reader.Tree[0];
	cnst ::llc::pobj<::llc::SJSONNode> & root = treeRoot->Token->Type == ::llc::JSON_TYPE_VALUE && treeRoot->Children.size() ? treeRoot->Children[0] : treeRoot;
	::llc::s3_t value = {};
	cnst ::llc::err_t result = root.get_ref() ? ::llc::jsonObjectGetInteger(reader, root->ObjectIndex, value) : -1;
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_INTEGER_VALUE, ::llc::failed(result) || value != -42
		, "Signed integer mismatch. result:%i, value:%" LLC_FMT_S3 ", expected:-42."
		, result, value
		);
	rtrn 0;
}

sttc ::llc::err_t testJSONOptions(ATestError & errors) {
	cnst ::llc::JSON_PARSE_OPTION individual[] =
		{ ::llc::JSON_PARSE_OPTION_BUILD_TREE, ::llc::JSON_PARSE_OPTION_BUILD_VIEWS, ::llc::JSON_PARSE_OPTION_FINAL_INPUT
		, ::llc::JSON_PARSE_OPTION_ALLOW_LINE_COMMENTS, ::llc::JSON_PARSE_OPTION_ALLOW_TRAILING_COMMAS, ::llc::JSON_PARSE_OPTION_ALLOW_LEADING_PLUS
		, ::llc::JSON_PARSE_OPTION_ALLOW_LEADING_DECIMAL_POINT, ::llc::JSON_PARSE_OPTION_ALLOW_TRAILING_DECIMAL_POINT, ::llc::JSON_PARSE_OPTION_ALLOW_LEADING_ZEROES
		, ::llc::JSON_PARSE_OPTION_ALLOW_TRAILING_CONTENT, ::llc::JSON_PARSE_OPTION_ALLOW_UNESCAPED_CONTROLS, ::llc::JSON_PARSE_OPTION_CONTAINER_ROOT_ONLY
		};
	::llc::u1_t combined = {};
	for(::llc::u2_t iOption = 0; iOption < ::llc::size(individual); ++iOption) {
		cnst ::llc::u1_t value = individual[iOption];
		LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_OPTION_LAYOUT, 0 == value || value & (value - 1) || combined & value
			, "Option %u is not an independent bit. value:0x%04X, previous:0x%04X."
			, iOption, value, combined
			);
		combined |= value;
	}
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_OPTION_LAYOUT, combined != 0x0FFFU || ::llc::JSON_PARSE_OPTION_NONE
		, "Option field mismatch. combined:0x%04X, expected:0x0FFF, none:0x%04X."
		, combined, (::llc::u1_t)::llc::JSON_PARSE_OPTION_NONE
		);

	cnst ::llc::JSON_PARSE_OPTION strictExpected = ::llc::JSON_PARSE_OPTION_BUILD_TREE | ::llc::JSON_PARSE_OPTION_BUILD_VIEWS | ::llc::JSON_PARSE_OPTION_FINAL_INPUT;
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_OPTION_PRESETS, (::llc::JSON_PARSE_OPTION)::llc::JSON_PARSE_STRICT != strictExpected
		, "Strict preset mismatch. actual:0x%04X, expected:0x%04X."
		, (::llc::u1_t)::llc::JSON_PARSE_STRICT, (::llc::u1_t)strictExpected
		);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_OPTION_PRESETS, (::llc::JSON_PARSE_OPTION)::llc::JSON_PARSE_LEGACY_ROOT != (strictExpected | ::llc::JSON_PARSE_OPTION_CONTAINER_ROOT_ONLY)
		, "Legacy-root preset mismatch. actual:0x%04X."
		, (::llc::u1_t)::llc::JSON_PARSE_LEGACY_ROOT
		);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_OPTION_PRESETS, (::llc::u1_t)::llc::JSON_PARSE_LLC != (::llc::u1_t)(combined & ~::llc::JSON_PARSE_OPTION_CONTAINER_ROOT_ONLY)
		, "LLC preset mismatch. actual:0x%04X, expected:0x%04X."
		, (::llc::u1_t)::llc::JSON_PARSE_LLC, (::llc::u1_t)(combined & ~::llc::JSON_PARSE_OPTION_CONTAINER_ROOT_ONLY)
		);

	::llc::SJSONParseOptions options = ::llc::JSON_PARSE_OPTION_NONE;
	options.Set(::llc::JSON_PARSE_OPTION_BUILD_TREE).Set(::llc::JSON_PARSE_OPTION_FINAL_INPUT).Clear(::llc::JSON_PARSE_OPTION_BUILD_TREE);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_OPTION_MUTATION
		, options.Any(::llc::JSON_PARSE_OPTION_BUILD_TREE) || false == options.All(::llc::JSON_PARSE_OPTION_FINAL_INPUT)
		, "Option mutation mismatch. options:0x%04X."
		, (::llc::u1_t)options
		);

	::llc::SJSONReader reader;
	cnst ::llc::SJSONParseOptions noOutputs = ::llc::JSON_PARSE_OPTION_FINAL_INPUT;
	cnst ::llc::err_t parseResult = ::llc::jsonParse(reader, LLC_CXS("{}"), noOutputs);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_OPTION_OUTPUTS, ::llc::failed(parseResult) || reader.View.size() != 1 || reader.Tree.size()
		, "Output flags mismatch. result:%i, tokens:%u, views:%u, tree:%u."
		, parseResult, reader.Token.size(), reader.View.size(), reader.Tree.size()
		);
	rtrn 0;
}

::llc::err_t testJSONReader(ATestError & errors) {
	if_fail_fe(testJSONOptions(errors));
	if_fail_fe(testJSONValidDocuments(errors));
	if_fail_fe(testJSONSignedInteger(errors));
	rtrn testJSONReset(errors);
}
