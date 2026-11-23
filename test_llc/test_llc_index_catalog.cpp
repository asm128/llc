#include "test_llc_index_catalog.h"

#include "llc_file.h"
#include "llc_json_expression.h"
#include "test_llc_suites.h"

GDEFINE_ENUM_TYPE(INDEX_CATALOG_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(INDEX_CATALOG_TEST_RESULT, LOAD_RESULT		, 0, "The Obelisco catalog consumer could not load ads.json.");
GDEFINE_ENUM_VALUED(INDEX_CATALOG_TEST_RESULT, SOURCE_BYTES		, 1, "The retained catalog source does not match the reopened file.");
GDEFINE_ENUM_VALUED(INDEX_CATALOG_TEST_RESULT, ITEM_COUNT		, 2, "The shops category did not produce catalog items.");
GDEFINE_ENUM_VALUED(INDEX_CATALOG_TEST_RESULT, RESOLVE_RESULT	, 3, "The native resolver did not find the indexed phone fragment.");
GDEFINE_ENUM_VALUED(INDEX_CATALOG_TEST_RESULT, SOURCE_RESULT		, 4, "A resolved item could not be mapped back to its source JSON record.");
GDEFINE_ENUM_VALUED(INDEX_CATALOG_TEST_RESULT, SOURCE_RECORD		, 5, "The expected onsushi source record was not resolved.");
GDEFINE_ENUM_VALUED(INDEX_CATALOG_TEST_RESULT, INDEXED_VALUE		, 6, "The resolved record does not contain the indexed phone value.");
GDEFINE_ENUM_VALUED(INDEX_CATALOG_TEST_RESULT, EMPTY_RESULT		, 7, "A missing catalog term unexpectedly resolved records.");
GDEFINE_ENUM_VALUED(INDEX_CATALOG_TEST_RESULT, SCRIPT_RESULT		, 8, "outputSearchScript() rejected the catalog index.");
GDEFINE_ENUM_VALUED(INDEX_CATALOG_TEST_RESULT, SCRIPT_DATA		, 9, "outputSearchScript() omitted catalog index data.");

stxp ::llc::vcst_c INDEX_CATALOG_FILE = "D:/enterprise/programming_sciences/codebases/Callsite/old_web_resu/demo/obelisco/ads.json";

::llc::err_t SIndexCatalog::Load(::llc::vcst_t fileName, ::llc::AD_SHOP_CATEGORY category, ::llc::vcst_t language) {
	if_fail_fe(::llc::clear(Item, Word));
	if_fail_fe(::llc::jsonFileRead(Source, fileName));
	if_fail_fe(::llc::getItemViews(Source.Reader, category, language, Item));
	rtrn ::llc::getWordDict(WordData, Word, Item);
}

::llc::err_t SIndexCatalog::Resolve(::llc::vcst_t input, ::llc::au2_t & itemIndices) cnst {
	rtrn ::llc::wordDictResolve(Word, input, Item.size(), itemIndices);
}

::llc::err_t SIndexCatalog::SourceValue(::llc::u2_t itemIndex, ::llc::vcst_t expression, ::llc::vcst_t & output) cnst {
	if_true_fef(itemIndex >= Item.size(), "Invalid catalog item: %u. Item count: %u.", itemIndex, Item.size());
	cnst ::llc::err_t sourceNode = ::llc::jsonArrayValueGet(Source.Reader, 0, Item[itemIndex].ItemIndex);
	if_fail_fe(sourceNode);
	rtrn ::llc::jsonExpressionResolve(expression, Source.Reader, sourceNode, output);
}

::llc::err_t test_index_catalog(STestRunnerState &, ::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	SIndexCatalog catalog = {};
	cnst ::llc::err_t loadResult = catalog.Load(INDEX_CATALOG_FILE, ::llc::AD_SHOP_CATEGORY_shops, "en");
	LLC_TEST_REQUIREF(errors, INDEX_CATALOG_TEST_RESULT_LOAD_RESULT, ::llc::failed(loadResult), "Result:%i.", loadResult);
	::llc::string reopened = {};
	cnst ::llc::err_t reopenResult = ::llc::fileToMemory(INDEX_CATALOG_FILE, reopened);
	LLC_TEST_REQUIREF(errors, INDEX_CATALOG_TEST_RESULT_LOAD_RESULT, ::llc::failed(reopenResult), "Result:%i.", reopenResult);
	LLC_TEST_CHECKF(errors, INDEX_CATALOG_TEST_RESULT_SOURCE_BYTES, reopened != catalog.Source.Bytes, "Reopened:%u, retained:%u.", reopened.size(), catalog.Source.Bytes.size());
	LLC_TEST_REQUIREF(errors, INDEX_CATALOG_TEST_RESULT_ITEM_COUNT, 0 == catalog.Item.size(), "Items:%u.", catalog.Item.size());
	::llc::string script = {};
	cnst ::llc::err_t scriptResult = ::llc::outputSearchScript(script, catalog.Item);
	LLC_TEST_REQUIREF(errors, INDEX_CATALOG_TEST_RESULT_SCRIPT_RESULT, ::llc::failed(scriptResult), "Result:%i.", scriptResult);
	LLC_TEST_CHECK(errors, INDEX_CATALOG_TEST_RESULT_SCRIPT_DATA, 0 > script.find(::llc::vcst_t{"var names=["}));
	LLC_TEST_CHECK(errors, INDEX_CATALOG_TEST_RESULT_SCRIPT_DATA, 0 > script.find(::llc::vcst_t{"var indices=["}));
	LLC_TEST_CHECK(errors, INDEX_CATALOG_TEST_RESULT_SCRIPT_DATA, 0 > script.find(::llc::vcst_t{"var idList=["}));

	::llc::au2_t resolved = {};
	cnst ::llc::err_t resolveResult = catalog.Resolve("7518", resolved);
	LLC_TEST_REQUIREF(errors, INDEX_CATALOG_TEST_RESULT_RESOLVE_RESULT, 0 >= resolveResult || 0 == resolved.size(), "Result:%i, count:%u.", resolveResult, resolved.size());
	b8_t foundOnSushi = false;
	for(::llc::u2_t iResult = 0; iResult < resolved.size(); ++iResult) {
		cnst ::llc::u2_t itemIndex = resolved[iResult];
		::llc::vcst_t sourceName = {};
		cnst ::llc::err_t sourceResult = catalog.SourceValue(itemIndex, "name", sourceName);
		LLC_TEST_REQUIREF(errors, INDEX_CATALOG_TEST_RESULT_SOURCE_RESULT, ::llc::failed(sourceResult), "Item:%u, result:%i.", itemIndex, sourceResult);
		if(sourceName != "onsushi")
			continue;
		foundOnSushi = true;
		b8_t foundPhone = false;
		for(::llc::u2_t iPhone = 0; iPhone < catalog.Item[itemIndex].Phones.size(); ++iPhone)
			foundPhone |= catalog.Item[itemIndex].Phones[iPhone] == "7518-4019";
		LLC_TEST_CHECKF(errors, INDEX_CATALOG_TEST_RESULT_INDEXED_VALUE, false == foundPhone, "Item:%u, phones:%u.", itemIndex, catalog.Item[itemIndex].Phones.size());
	}
	LLC_TEST_CHECK(errors, INDEX_CATALOG_TEST_RESULT_SOURCE_RECORD, false == foundOnSushi);
	cnst ::llc::err_t missingResult = catalog.Resolve("definitely-not-in-obelisco", resolved);
	LLC_TEST_CHECKF(errors, INDEX_CATALOG_TEST_RESULT_EMPTY_RESULT, missingResult || resolved.size(), "Result:%i, count:%u.", missingResult, resolved.size());
	rtrn 0;
}
