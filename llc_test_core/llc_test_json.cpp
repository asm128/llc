#include "llc_json.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(JSON_READER_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, OK					, 0, "All JSON reader tests passed.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, PARSE					, 1, "jsonParse() rejected a valid JSON document.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, ROOT_TYPE				, 6, "The semantic JSON root had an unexpected type.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, WRITE					, 7, "jsonWrite() failed to serialize a parsed document.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, ROUND_TRIP				, 8, "A parsed JSON document did not serialize to its expected canonical text.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, COUNTED_INPUT			, 9, "jsonParse() depended on a null terminator beyond its counted input.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, OPTION_PRESETS			, 14, "A JSON parse-option preset did not contain its documented flags.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, STORAGE_VIEW_COUNT		, 17, "The JSON token and view counts differ.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, STORAGE_TREE_COUNT		, 18, "The JSON token and tree counts differ.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, TOKEN_SPAN_ORDER		, 19, "A JSON token span ends before it begins.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, TOKEN_SPAN_BOUNDS		, 20, "A JSON token span exceeds the input.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, TOKEN_VIEW_SIZE		, 21, "A JSON token view has the wrong length.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, TOKEN_VIEW_BEGIN		, 22, "A JSON token view points outside its source span.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, TREE_NODE_PRESENT		, 23, "A JSON token has no tree node.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, TREE_NODE_TOKEN		, 24, "A JSON tree node references the wrong token.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, TREE_NODE_INDEX		, 25, "A JSON tree node has the wrong object index.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, TREE_NODE_PARENT		, 26, "A JSON tree node has the wrong parent.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, READER_DONE			, 27, "The JSON reader did not finish reading.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, READER_NEST_LEVEL		, 28, "The JSON reader retained a nonzero nesting level.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, READER_ELEMENT_INDEX	, 29, "The JSON reader retained a current element index.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, READER_CURRENT_ELEMENT	, 30, "The JSON reader retained a current element pointer.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, ROOT_PRESENT			, 31, "The JSON parse tree has no root node.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, ROOT_REFERENCE		, 32, "The JSON parse tree root reference is null.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, ROOT_VALUE_REFERENCE	, 33, "The semantic JSON root reference is null.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, INTEGER_ROOT_PRESENT	, 34, "The signed-integer parse tree has no root node.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, INTEGER_ROOT_REFERENCE, 35, "The signed-integer parse tree root reference is null.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, INTEGER_GET_RESULT		, 36, "jsonObjectGetInteger() rejected a parsed signed integer.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, INTEGER_GET_VALUE		, 37, "jsonObjectGetInteger() returned the wrong signed value.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, OPTION_NONZERO		, 38, "A JSON parse-option flag has no bit.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, OPTION_SINGLE_BIT		, 39, "A JSON parse-option flag occupies more than one bit.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, OPTION_DISTINCT_BIT	, 40, "A JSON parse-option flag overlaps an earlier flag.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, OPTION_COMBINED		, 41, "The combined JSON parse-option mask is incomplete.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, OPTION_NONE			, 42, "JSON_PARSE_OPTION_NONE is not zero.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, OPTION_OUTPUT_PARSE		, 43, "Parsing with no output-building flags failed.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, OPTION_OUTPUT_VIEW		, 44, "Parsing with no output-building flags produced the wrong view count.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, OPTION_OUTPUT_TREE		, 45, "Parsing with no output-building flags built a tree.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, OPTION_BUILD_TREE_CLEAR	, 46, "Clearing BUILD_TREE left the JSON parse option set.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, OPTION_FINAL_INPUT_SET	, 47, "Setting FINAL_INPUT did not persist in JSON parse options.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, RESET_ROOT_PRESENT		, 48, "The cached JSON tree has no root before reset.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, RESET_ROOT_REFERENCE	, 49, "The cached JSON root reference is null before reset.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, RESET_RESULT			, 50, "SJSONReader::Reset() failed.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, RESET_TOKENS			, 51, "SJSONReader::Reset() retained tokens.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, RESET_VIEWS			, 52, "SJSONReader::Reset() retained views.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, RESET_TREE_COUNT		, 53, "SJSONReader::Reset() changed the cached tree count.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, RESET_TREE_STORAGE		, 54, "SJSONReader::Reset() changed the cached tree storage.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, RESET_ROOT_STORAGE		, 55, "SJSONReader::Reset() changed the cached root reference.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, RESET_CHILD_STORAGE	, 56, "SJSONReader::Reset() changed the cached child storage.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, RESET_CHAR_INDEX		, 57, "SJSONReader::Reset() retained the character index.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, RESET_ELEMENT_INDEX	, 58, "SJSONReader::Reset() retained the element index.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, RESET_ELEMENT			, 59, "SJSONReader::Reset() retained the current element.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, RESET_NEST_LEVEL		, 60, "SJSONReader::Reset() retained the nesting level.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, RESET_CHARACTER		, 61, "SJSONReader::Reset() retained the current character.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, RESET_ESCAPING		, 62, "SJSONReader::Reset() retained the escaping flag.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, RESET_INSIDE_STRING	, 63, "SJSONReader::Reset() retained the inside-string flag.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, RESET_SEPARATOR		, 64, "SJSONReader::Reset() retained the expected-separator flag.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, RESET_DONE			, 65, "SJSONReader::Reset() retained the done-reading flag.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, REUSE_TREE_STORAGE		, 66, "Reparsing changed the cached JSON tree storage.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, REUSE_ROOT_STORAGE		, 67, "Reparsing changed the cached JSON root reference.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, REUSE_CHILD_STORAGE	, 68, "Reparsing changed the cached JSON child storage.");
GDEFINE_ENUM_VALUED(JSON_READER_TEST_RESULT, RESET_ROOT_REFERENCE_AFTER, 69, "The cached JSON root reference is null after reset.");

static_assert(szof(::llc::JSON_PARSE_OPTION) == szof(::llc::u1_t), "JSON parse options must use one uniform 16-bit field.");
static_assert(szof(::llc::SJSONParseOptions) == szof(::llc::JSON_PARSE_OPTION), "bit_field<> must not add storage overhead.");

tydf ::llc::view<const ::llc::vcst_t> TJSONConstViews;
tydf ::llc::err_t (*TJSONObjectValueGet)(cnst ::llc::SJSONNode &, TJSONConstViews, ::llc::vcst_t);
tydf ::llc::err_t (*TJSONObjectKeyList)(cnst ::llc::SJSONNode &, TJSONConstViews, ::llc::aobj<::llc::vcst_t> &);
tydf ::llc::err_t (*TJSONObjectIndexedKeyList)(cnst ::llc::SJSONNode &, TJSONConstViews, ::llc::as2_t &, ::llc::aobj<::llc::vcst_t> &);
tydf ::llc::err_t (*TJSONCompare)(cnst ::llc::SJSONNode &, TJSONConstViews, cnst ::llc::SJSONNode &, TJSONConstViews);
tydf ::llc::err_t (*TJSONCompareSharedViews)(cnst ::llc::SJSONNode &, cnst ::llc::SJSONNode &, TJSONConstViews);
tydf ::llc::err_t (*TJSONWrite)(cnst ::llc::SJSONNode *, TJSONConstViews, ::llc::string &);
tydf ::llc::err_t (*TJSONArraySplit)(cnst ::llc::SJSONNode &, TJSONConstViews, ::llc::u2_t, ::llc::aobj<::llc::string> &);

static_assert(::llc::is_cnst<TJSONConstViews::T>::Value, "JSON view collections must expose const view descriptors.");
static_assert(::llc::is_cnst<::llc::vcst_t::T>::Value, "String views must expose const characters.");
static_assert(requires {
	static_cast<TJSONObjectValueGet>(&::llc::jsonObjectValueGet);
	static_cast<TJSONObjectKeyList>(&::llc::jsonObjectKeyList);
	static_cast<TJSONObjectIndexedKeyList>(&::llc::jsonObjectKeyList);
	}, "JSON object access must preserve const view collections.");
static_assert(requires {
	static_cast<TJSONCompare>(&::llc::jsonCompareNumber);
	static_cast<TJSONCompare>(&::llc::jsonCompareArray);
	static_cast<TJSONCompare>(&::llc::jsonCompareObject);
	static_cast<TJSONCompareSharedViews>(&::llc::jsonCompareNumber);
	static_cast<TJSONCompareSharedViews>(&::llc::jsonCompareArray);
	static_cast<TJSONCompareSharedViews>(&::llc::jsonCompareObject);
	}, "JSON comparison must preserve const view collections.");
static_assert(requires {
	static_cast<TJSONWrite>(&::llc::jsonWrite);
	static_cast<TJSONArraySplit>(&::llc::jsonArraySplit);
	}, "JSON serialization must preserve const view collections.");

sttc bool jsonTextMismatch(::llc::vcst_t actual, ::llc::vcst_t expected) {
	rtrn actual.size() != expected.size() || (actual.size() && 0 != memcmp(actual.begin(), expected.begin(), actual.size()));
}

sttc ::llc::err_t testJSONStructure(ATestError & errors, cnst ::llc::SJSONReader & reader, ::llc::vcst_t input) {
	LLC_TEST_REQUIRE(errors, JSON_READER_TEST_RESULT_STORAGE_VIEW_COUNT, reader.Token.size() != reader.View.size()
		, "Tokens:%u, views:%u."
		, reader.Token.size(), reader.View.size()
		);
	LLC_TEST_REQUIRE(errors, JSON_READER_TEST_RESULT_STORAGE_TREE_COUNT, reader.Token.size() != reader.Tree.size()
		, "Tokens:%u, tree nodes:%u."
		, reader.Token.size(), reader.Tree.size()
		);
	for(::llc::u2_t iToken = 0; iToken < reader.Token.size(); ++iToken) {
		cnst ::llc::SJSONToken & token = reader.Token[iToken];
		cnst ::llc::vcst_t & tokenView = reader.View[iToken];
		cnst ::llc::pobj<::llc::SJSONNode> & node = reader.Tree[iToken];
		cnst bool hasParent = token.ParentIndex >= 0 && (::llc::u2_t)token.ParentIndex < reader.Tree.size();
		LLC_TEST_REQUIRE(errors, JSON_READER_TEST_RESULT_TOKEN_SPAN_ORDER, token.Span.Begin > token.Span.End
			, "Token:%u span:%u..%u."
			, iToken, token.Span.Begin, token.Span.End
			);
		LLC_TEST_REQUIRE(errors, JSON_READER_TEST_RESULT_TOKEN_SPAN_BOUNDS, token.Span.End > input.size()
			, "Token:%u end:%u, input size:%u."
			, iToken, token.Span.End, input.size()
			);
		LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_TOKEN_VIEW_SIZE, tokenView.size() != token.Span.End - token.Span.Begin
			, "Token:%u view size:%u, span size:%u."
			, iToken, tokenView.size(), token.Span.End - token.Span.Begin
			);
		LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_TOKEN_VIEW_BEGIN, tokenView.begin() != (token.Span.Begin < input.size() ? &input[token.Span.Begin] : input.end())
			, "Token:%u view begin:%p, expected:%p."
			, iToken, tokenView.begin(), token.Span.Begin < input.size() ? &input[token.Span.Begin] : input.end()
			);
		LLC_TEST_REQUIRE(errors, JSON_READER_TEST_RESULT_TREE_NODE_PRESENT, 0 == node.get_ref()
			, "Token:%u node:%p, expected non-null."
			, iToken, node.get_ref()
			);
		LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_TREE_NODE_TOKEN, node->Token != &reader.Token[iToken]
			, "Node:%u token:%p, expected:%p."
			, iToken, node->Token, &reader.Token[iToken]
			);
		LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_TREE_NODE_INDEX, node->ObjectIndex != (::llc::json_id_t)iToken
			, "Node:%u object index:%i."
			, iToken, node->ObjectIndex
			);
		LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_TREE_NODE_PARENT, node->Parent != (hasParent ? reader.Tree[token.ParentIndex].operator->() : 0)
			, "Node:%u parent:%p, expected:%p."
			, iToken, node->Parent, hasParent ? reader.Tree[token.ParentIndex].operator->() : 0
			);
	}
	rtrn 0;
}

sttc ::llc::err_t testJSONDocument
	(ATestError & errors, ::llc::vcst_t input, ::llc::vcst_t expected, ::llc::JSON_TYPE expectedType, JSON_READER_TEST_RESULT parseResult = JSON_READER_TEST_RESULT_PARSE) {
	::llc::SJSONReader reader;
	cnst ::llc::err_t result = ::llc::jsonParse(reader, input);
	LLC_TEST_REQUIRE(errors, parseResult, ::llc::failed(result)
		, "Valid JSON parse failed. result:%i, input:'%.*s'."
		, result, (int)input.size(), input.begin()
		);
	cnst ::llc::u2_t structureFailures = testErrorCount(errors);
	if_fail_fe(testJSONStructure(errors, reader, input));
	if(structureFailures != testErrorCount(errors))
		rtrn 0;
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_READER_DONE, false == reader.StateRead.DoneReading
		, "Done reading:%u, expected:1."
		, (::llc::u2_t)reader.StateRead.DoneReading
		);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_READER_NEST_LEVEL, reader.StateRead.NestLevel
		, "Nest level:%i, expected:0."
		, reader.StateRead.NestLevel
		);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_READER_ELEMENT_INDEX, reader.StateRead.IndexCurrentElement != -1
		, "Current element index:%i, expected:-1."
		, reader.StateRead.IndexCurrentElement
		);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_READER_CURRENT_ELEMENT, reader.StateRead.CurrentElement
		, "Current element:%p, expected:null."
		, reader.StateRead.CurrentElement
		);
	LLC_TEST_REQUIRE(errors, JSON_READER_TEST_RESULT_ROOT_PRESENT, 0 == reader.Tree.size()
		, "Missing JSON root. tree nodes:%u."
		, reader.Tree.size()
		);
	LLC_TEST_REQUIRE(errors, JSON_READER_TEST_RESULT_ROOT_REFERENCE, 0 == reader.Tree[0].get_ref()
		, "Root reference:%p, expected non-null."
		, reader.Tree[0].get_ref()
		);
	cnst ::llc::pobj<::llc::SJSONNode> & treeRoot = reader.Tree[0];
	cnst ::llc::pobj<::llc::SJSONNode> & root = treeRoot->Token->Type == ::llc::JSON_TYPE_VALUE && treeRoot->Children.size() ? treeRoot->Children[0] : treeRoot;
	LLC_TEST_REQUIRE(errors, JSON_READER_TEST_RESULT_ROOT_VALUE_REFERENCE, 0 == root.get_ref()
		, "Semantic root reference:%p, expected non-null."
		, root.get_ref()
		);
	LLC_TEST_REQUIRE(errors, JSON_READER_TEST_RESULT_ROOT_TYPE, root->Token->Type != expectedType
		, "Root type mismatch. actual:%i, expected:%i, tree nodes:%u."
		, (::llc::s2_t)root->Token->Type, (::llc::s2_t)expectedType, reader.Tree.size()
		);
	::llc::string output;
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
	LLC_TEST_REQUIRE(errors, JSON_READER_TEST_RESULT_RESET_ROOT_PRESENT, 0 == reader.Tree.size()
		, "Cached tree nodes:%u, expected at least one."
		, reader.Tree.size()
		);
	LLC_TEST_REQUIRE(errors, JSON_READER_TEST_RESULT_RESET_ROOT_REFERENCE, 0 == reader.Tree[0].get_ref()
		, "Cached root reference:%p, expected non-null."
		, reader.Tree[0].get_ref()
		);
	::llc::u2_c treeSize = reader.Tree.size();
	cnst ::llc::view<const ::llc::pobj<::llc::SJSONNode>> treeStorage = {reader.Tree.begin(), reader.Tree.size()};
	cnst ::llc::view<const ::llc::gref<::llc::SJSONNode>> rootReference = {reader.Tree[0].get_ref(), 1};
	cnst ::llc::view<const ::llc::pobj<::llc::SJSONNode>> childStorage = {reader.Tree[0]->Children.begin(), reader.Tree[0]->Children.size()};
	cnst ::llc::err_t result = reader.Reset();
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_RESET_RESULT, ::llc::failed(result), "Reset result:%i.", result);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_RESET_TOKENS, reader.Token.size(), "Tokens after reset:%u.", reader.Token.size());
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_RESET_VIEWS, reader.View.size(), "Views after reset:%u.", reader.View.size());
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_RESET_TREE_COUNT, reader.Tree.size() != treeSize
		, "Tree nodes after reset:%u, expected:%u."
		, reader.Tree.size(), treeSize
		);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_RESET_TREE_STORAGE, reader.Tree.begin() != treeStorage.begin()
		, "Tree storage after reset:%p, expected:%p."
		, reader.Tree.begin(), treeStorage.begin()
		);
	if(reader.Tree.size()) {
		LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_RESET_ROOT_REFERENCE_AFTER, 0 == reader.Tree[0].get_ref()
			, "Root reference after reset:%p, expected non-null."
			, reader.Tree[0].get_ref()
			);
		LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_RESET_ROOT_STORAGE, reader.Tree[0].get_ref() != rootReference.begin()
			, "Root reference after reset:%p, expected:%p."
			, reader.Tree[0].get_ref(), rootReference.begin()
			);
		if(reader.Tree[0].get_ref()) {
			LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_RESET_CHILD_STORAGE, reader.Tree[0]->Children.begin() != childStorage.begin()
				, "Child storage after reset:%p, expected:%p."
				, reader.Tree[0]->Children.begin(), childStorage.begin()
				);
		}
	}
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_RESET_CHAR_INDEX, reader.StateRead.IndexCurrentChar
		, "Character index after reset:%u."
		, reader.StateRead.IndexCurrentChar
		);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_RESET_ELEMENT_INDEX, reader.StateRead.IndexCurrentElement != -1
		, "Element index after reset:%i."
		, reader.StateRead.IndexCurrentElement
		);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_RESET_ELEMENT, reader.StateRead.CurrentElement
		, "Current element after reset:%p."
		, reader.StateRead.CurrentElement
		);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_RESET_NEST_LEVEL, reader.StateRead.NestLevel
		, "Nest level after reset:%i."
		, reader.StateRead.NestLevel
		);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_RESET_CHARACTER, reader.StateRead.CharCurrent
		, "Current character after reset:%i."
		, reader.StateRead.CharCurrent
		);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_RESET_ESCAPING, reader.StateRead.Escaping
		, "Escaping after reset:%u."
		, (::llc::u2_t)reader.StateRead.Escaping
		);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_RESET_INSIDE_STRING, reader.StateRead.InsideString
		, "Inside string after reset:%u."
		, (::llc::u2_t)reader.StateRead.InsideString
		);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_RESET_SEPARATOR, reader.StateRead.ExpectingSeparator
		, "Expecting separator after reset:%u."
		, (::llc::u2_t)reader.StateRead.ExpectingSeparator
		);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_RESET_DONE, reader.StateRead.DoneReading
		, "Done reading after reset:%u."
		, (::llc::u2_t)reader.StateRead.DoneReading
		);

	::llc::vcst_t input = LLC_CXS("{\"value\":-2}");
	if_fail_fe(::llc::jsonParse(reader, input));
	cnst ::llc::u2_t structureFailures = testErrorCount(errors);
	if_fail_fe(testJSONStructure(errors, reader, input));
	if(structureFailures != testErrorCount(errors))
		rtrn 0;
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_REUSE_TREE_STORAGE, reader.Tree.begin() != treeStorage.begin()
		, "Tree storage after rebuild:%p, expected:%p."
		, reader.Tree.begin(), treeStorage.begin()
		);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_REUSE_ROOT_STORAGE, reader.Tree[0].get_ref() != rootReference.begin()
		, "Root reference after rebuild:%p, expected:%p."
		, reader.Tree[0].get_ref(), rootReference.begin()
		);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_REUSE_CHILD_STORAGE, reader.Tree[0]->Children.begin() != childStorage.begin()
		, "Child storage after rebuild:%p, expected:%p."
		, reader.Tree[0]->Children.begin(), childStorage.begin()
		);
	rtrn 0;
}

sttc ::llc::err_t testJSONSignedInteger(ATestError & errors) {
	::llc::SJSONReader reader;
	if_fail_fe(::llc::jsonParse(reader, LLC_CXS("-42")));
	LLC_TEST_REQUIRE(errors, JSON_READER_TEST_RESULT_INTEGER_ROOT_PRESENT, 0 == reader.Tree.size()
		, "Missing signed-integer root. tree nodes:%u."
		, reader.Tree.size()
		);
	LLC_TEST_REQUIRE(errors, JSON_READER_TEST_RESULT_INTEGER_ROOT_REFERENCE, 0 == reader.Tree[0].get_ref()
		, "Signed-integer root reference:%p, expected non-null."
		, reader.Tree[0].get_ref()
		);
	cnst ::llc::pobj<::llc::SJSONNode> & treeRoot = reader.Tree[0];
	cnst ::llc::pobj<::llc::SJSONNode> & root = treeRoot->Token->Type == ::llc::JSON_TYPE_VALUE && treeRoot->Children.size() ? treeRoot->Children[0] : treeRoot;
	LLC_TEST_REQUIRE(errors, JSON_READER_TEST_RESULT_ROOT_VALUE_REFERENCE, 0 == root.get_ref()
		, "Signed-integer semantic root reference:%p, expected non-null."
		, root.get_ref()
		);
	::llc::s3_t value = {};
	cnst ::llc::err_t result = ::llc::jsonObjectGetInteger(reader, root->ObjectIndex, value);
	LLC_TEST_REQUIRE(errors, JSON_READER_TEST_RESULT_INTEGER_GET_RESULT, ::llc::failed(result)
		, "Signed integer getter result:%i, expected success."
		, result
		);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_INTEGER_GET_VALUE, value != -42
		, "Signed integer value:%" LLC_FMT_S3 ", expected:-42."
		, value
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
		LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_OPTION_NONZERO, 0 == value
			, "Option:%u value:0x%04X, expected nonzero."
			, iOption, value
			);
		LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_OPTION_SINGLE_BIT, value & (value - 1)
			, "Option:%u value:0x%04X, expected one bit."
			, iOption, value
			);
		LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_OPTION_DISTINCT_BIT, combined & value
			, "Option:%u value:0x%04X overlaps previous:0x%04X."
			, iOption, value, combined
			);
		combined |= value;
	}
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_OPTION_COMBINED, combined != 0x0FFFU
		, "Combined option mask:0x%04X, expected:0x0FFF."
		, combined
		);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_OPTION_NONE, ::llc::JSON_PARSE_OPTION_NONE
		, "NONE option mask:0x%04X, expected:0."
		, (::llc::u1_t)::llc::JSON_PARSE_OPTION_NONE
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
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_OPTION_BUILD_TREE_CLEAR, options.Any(::llc::JSON_PARSE_OPTION_BUILD_TREE)
		, "BUILD_TREE remained set. options:0x%04X."
		, (::llc::u1_t)options
		);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_OPTION_FINAL_INPUT_SET, false == options.All(::llc::JSON_PARSE_OPTION_FINAL_INPUT)
		, "FINAL_INPUT was cleared. options:0x%04X."
		, (::llc::u1_t)options
		);

	::llc::SJSONReader reader;
	cnst ::llc::SJSONParseOptions noOutputs = ::llc::JSON_PARSE_OPTION_FINAL_INPUT;
	cnst ::llc::err_t parseResult = ::llc::jsonParse(reader, LLC_CXS("{}"), noOutputs);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_OPTION_OUTPUT_PARSE, ::llc::failed(parseResult)
		, "No-output parse result:%i."
		, parseResult
		);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_OPTION_OUTPUT_VIEW, reader.View.size() != 1
		, "No-output view count:%u, expected:1."
		, reader.View.size()
		);
	LLC_TEST_CHECK(errors, JSON_READER_TEST_RESULT_OPTION_OUTPUT_TREE, reader.Tree.size()
		, "No-output tree count:%u, expected:0."
		, reader.Tree.size()
		);
	rtrn 0;
}

::llc::err_t testJSONReader(ATestError & errors) {
	if_fail_fe(testJSONOptions(errors));
	if_fail_fe(testJSONValidDocuments(errors));
	if_fail_fe(testJSONSignedInteger(errors));
	rtrn testJSONReset(errors);
}
