#include "llc_linear_map_pod.h"
#include "llc_linear_map_pobj.h"
#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(LINEAR_MAP_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, PUSH_INDEX			,  1, "linear_map_pod<>::push_back() returned the wrong index.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, PUSH_SIZE			,  2, "linear_map_pod<>::push_back() broke the key/value size invariant.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, PUSH_VALUE			,  3, "linear_map_pod<>::push_back() stored the wrong key or value.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, DUPLICATE_RESULT		,  4, "linear_map_pod<> accepted a duplicate key.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, DUPLICATE_PRESERVATION	,  5, "Rejected duplicate insertion modified linear_map_pod<>.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, CREATE_INDEX			,  6, "linear_map_pod<>::Create() returned the wrong index.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, CREATE_VALUE			,  7, "linear_map_pod<>::Create() did not value-initialize its value.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, CLONE_INDEX			,  8, "linear_map_pod<>::Clone() returned the wrong index.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, CLONE_VALUE			,  9, "linear_map_pod<>::Clone() did not copy its source value.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, CLONE_INVALID_RESULT	, 10, "linear_map_pod<>::Clone() accepted an invalid source index.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, DELETE_RESULT			, 11, "linear_map_pod<>::Delete() reported the wrong result.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, DELETE_SIZE			, 12, "linear_map_pod<>::Delete() broke the key/value size invariant.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, DELETE_VALUE			, 13, "linear_map_pod<>::Delete() removed or reordered only one side of the map.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, DELETE_MISSING_RESULT	, 14, "linear_map_pod<>::Delete() accepted a missing key.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, SAVE_RESULT			, 15, "linear_map_pod<>::Save() reported failure for a valid map.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, SAVE_PREFIX			, 16, "linear_map_pod<>::Save() modified preceding output bytes.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, LOAD_RESULT			, 17, "linear_map_pod<>::Load() reported failure for a valid map.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, LOAD_SIZE			, 18, "linear_map_pod<>::Load() restored mismatched key/value counts.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, LOAD_KEY				, 19, "linear_map_pod<>::Load() restored a different key.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, LOAD_VALUE			, 20, "linear_map_pod<>::Load() restored a different value.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, LOAD_CURSOR			, 21, "linear_map_pod<>::Load() did not leave the input at the following field.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, TRUNCATED_RESULT		, 22, "linear_map_pod<>::Load() accepted truncated input.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, TRUNCATED_CURSOR		, 23, "linear_map_pod<>::Load() moved the cursor after rejecting truncated input.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, TRUNCATED_OUTPUT		, 24, "linear_map_pod<>::Load() modified the map after rejecting truncated input.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, COUNT_MISMATCH_RESULT	, 25, "linear_map_pod<>::Load() accepted different key and value counts.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, COUNT_MISMATCH_CURSOR	, 26, "linear_map_pod<>::Load() moved the cursor after rejecting different counts.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, COUNT_MISMATCH_OUTPUT	, 27, "linear_map_pod<>::Load() modified the map after rejecting different counts.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, DUPLICATE_LOAD_RESULT	, 28, "linear_map_pod<>::Load() accepted duplicate serialized keys.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, DUPLICATE_LOAD_OUTPUT	, 29, "linear_map_pod<>::Load() modified the map after rejecting duplicate keys.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, INVALID_SAVE_RESULT		, 30, "linear_map_pod<>::Save() accepted mismatched key/value counts.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, INVALID_SAVE_OUTPUT		, 31, "linear_map_pod<>::Save() wrote bytes before rejecting an invalid map.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, POBJ_CREATE_INDEX		, 32, "llc_linear_map_pobj<>::Create() returned the wrong index.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, POBJ_CREATE_SIZE		, 33, "llc_linear_map_pobj<>::Create() broke the name/element size invariant.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, POBJ_CREATE_VALUE		, 34, "llc_linear_map_pobj<>::Create() did not provide writable lazy object ownership.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, POBJ_CLONE_INDEX		, 35, "llc_linear_map_pobj<>::Clone() returned the wrong index.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, POBJ_CLONE_NAME		, 36, "llc_linear_map_pobj<>::Clone() did not preserve the source name.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, POBJ_CLONE_VALUE		, 37, "llc_linear_map_pobj<>::Clone() did not make an independent value copy.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, POBJ_DELETE_RESULT		, 38, "llc_linear_map_pobj<>::Delete() reported the wrong result.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, POBJ_DELETE_PAIR		, 39, "llc_linear_map_pobj<>::Delete() reordered only one side of the map.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, POBJ_SAVE_RESULT		, 40, "llc_linear_map_pobj<>::Save() reported failure for valid objects.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, POBJ_LOAD_RESULT		, 41, "llc_linear_map_pobj<>::Load() reported failure for valid objects.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, POBJ_LOAD_SIZE		, 42, "llc_linear_map_pobj<>::Load() restored mismatched name/element counts.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, POBJ_LOAD_NAME		, 43, "llc_linear_map_pobj<>::Load() restored a different name.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, POBJ_LOAD_VALUE		, 44, "llc_linear_map_pobj<>::Load() restored a different object value.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, POBJ_LOAD_CURSOR		, 45, "llc_linear_map_pobj<>::Load() did not leave the input at the following field.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, POBJ_TRUNCATED_RESULT	, 46, "llc_linear_map_pobj<>::Load() accepted truncated input.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, POBJ_TRUNCATED_CURSOR	, 47, "llc_linear_map_pobj<>::Load() moved the cursor after rejecting truncated input.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, POBJ_TRUNCATED_OUTPUT	, 48, "llc_linear_map_pobj<>::Load() modified the map after rejecting truncated input.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, POBJ_NULL_SAVE_RESULT	, 49, "llc_linear_map_pobj<>::Save() accepted a null element.");
GDEFINE_ENUM_VALUED(LINEAR_MAP_TEST_RESULT, POBJ_NULL_SAVE_OUTPUT	, 50, "llc_linear_map_pobj<>::Save() wrote bytes before rejecting a null element.");

stxp ::llc::u0_t LINEAR_MAP_PREFIX[] = {0x31U, 0x72U, 0xB4U};
stxp ::llc::u0_t LINEAR_MAP_SUFFIX[] = {0xE8U, 0x5DU, 0x0AU, 0xC3U};

using TLinearMap = ::llc::linear_map_pod<::llc::u2_t, ::llc::u3_t>;

struct SLinearMapObject {
	::llc::u2_t Value = {};

	::llc::err_t Save(::llc::au0_t & output) cnst { return ::llc::savePOD(output, Value); }
	::llc::err_t Load(::llc::vcu0_t & input) { return ::llc::loadPOD(input, Value); }
};

using TObjectLinearMap = ::llc::llc_linear_map_pobj<SLinearMapObject>;

sttc ::llc::err_t prepareMapSentinel(TLinearMap & map) {
	return map.push_back(99, 999);
}

sttc ::llc::err_t checkMapSentinel(::llc::ATestError & errors, LINEAR_MAP_TEST_RESULT result, cnst TLinearMap & map, ::llc::u2_c caseIndex) {
	LLC_TEST_CHECKF(errors, result, map.Keys.size() != 1, "Case:%u, key count:%u, expected:1.", caseIndex, map.Keys.size());
	LLC_TEST_CHECKF(errors, result, map.Values.size() != 1, "Case:%u, value count:%u, expected:1.", caseIndex, map.Values.size());
	if(map.Keys.size())
		LLC_TEST_CHECKF(errors, result, map.Keys[0] != 99, "Case:%u, key:%u, expected:99.", caseIndex, map.Keys[0]);
	if(map.Values.size())
		LLC_TEST_CHECKF(errors, result, map.Values[0] != 999, "Case:%u, value:%" LLC_FMT_U3 ", expected:999.", caseIndex, map.Values[0]);
	return 0;
}

sttc ::llc::err_t testLinearMapOperations(::llc::ATestError & errors, TLinearMap & map) {
	cnst ::llc::err_t firstIndex = map.push_back(10, 100);
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_PUSH_INDEX, firstIndex != 0, "First index:%i, expected:0.", firstIndex);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_PUSH_SIZE, map.Keys.size() != map.Values.size(), "Key count:%u, value count:%u.", map.Keys.size(), map.Values.size());
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_PUSH_VALUE, map.Keys[0] != 10, "First key:%u, expected:10.", map.Keys[0]);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_PUSH_VALUE, map.Values[0] != 100, "First value:%" LLC_FMT_U3 ", expected:100.", map.Values[0]);

	cnst ::llc::u2_t originalKeyCount = map.Keys.size();
	cnst ::llc::u2_t originalValueCount = map.Values.size();
	cnst ::llc::err_t duplicateResult = map.push_back(10, 200);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_DUPLICATE_RESULT, 0 <= duplicateResult, "Duplicate result:%i.", duplicateResult);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_DUPLICATE_PRESERVATION, map.Keys.size() != originalKeyCount, "Key count:%u, expected:%u.", map.Keys.size(), originalKeyCount);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_DUPLICATE_PRESERVATION, map.Values.size() != originalValueCount, "Value count:%u, expected:%u.", map.Values.size(), originalValueCount);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_DUPLICATE_PRESERVATION, map.Values[0] != 100, "First value:%" LLC_FMT_U3 ", expected:100.", map.Values[0]);

	cnst ::llc::err_t createdIndex = map.Create(20);
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_CREATE_INDEX, createdIndex != 1, "Created index:%i, expected:1.", createdIndex);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_PUSH_SIZE, map.Keys.size() != map.Values.size(), "Key count:%u, value count:%u after Create().", map.Keys.size(), map.Values.size());
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_CREATE_VALUE, map.Values[createdIndex] != 0, "Created value:%" LLC_FMT_U3 ", expected:0.", map.Values[createdIndex]);
	map.Values[createdIndex] = 200;

	cnst ::llc::err_t clonedIndex = map.Clone(30, firstIndex);
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_CLONE_INDEX, clonedIndex != 2, "Cloned index:%i, expected:2.", clonedIndex);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_CLONE_VALUE, map.Values[clonedIndex] != map.Values[firstIndex], "Cloned value:%" LLC_FMT_U3 ", source:%" LLC_FMT_U3 ".", map.Values[clonedIndex], map.Values[firstIndex]);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_PUSH_SIZE, map.Keys.size() != map.Values.size(), "Key count:%u, value count:%u after Clone().", map.Keys.size(), map.Values.size());
	cnst ::llc::u2_t countBeforeInvalidClone = map.size();
	cnst ::llc::err_t invalidCloneResult = map.Clone(40, map.size());
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_CLONE_INVALID_RESULT, 0 <= invalidCloneResult, "Invalid clone result:%i.", invalidCloneResult);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_CLONE_INVALID_RESULT, map.size() != countBeforeInvalidClone, "Size after invalid clone:%u, expected:%u.", map.size(), countBeforeInvalidClone);

	cnst ::llc::err_t deleteResult = map.Delete(20);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_DELETE_RESULT, deleteResult != 2, "Delete result:%i, expected:2.", deleteResult);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_DELETE_SIZE, map.Keys.size() != map.Values.size(), "Key count:%u, value count:%u after Delete().", map.Keys.size(), map.Values.size());
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_DELETE_SIZE, map.size() != 2, "Map size:%u, expected:2.", map.size());
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_DELETE_VALUE, 0 <= map.Keys.find(20), "Deleted key 20 remains at index:%i.", map.Keys.find(20));
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_DELETE_VALUE, 0 > map.Keys.find(30), "%s", "Cloned key 30 disappeared during unordered deletion.");
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_DELETE_VALUE, map[30] != 100, "Cloned value after deletion:%" LLC_FMT_U3 ", expected:100.", map[30]);
	cnst ::llc::u2_t countBeforeMissingDelete = map.size();
	cnst ::llc::err_t missingDeleteResult = map.Delete(20);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_DELETE_MISSING_RESULT, 0 <= missingDeleteResult, "Missing-key delete result:%i.", missingDeleteResult);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_DELETE_MISSING_RESULT, map.size() != countBeforeMissingDelete, "Size after missing-key delete:%u, expected:%u.", map.size(), countBeforeMissingDelete);
	return 0;
}

sttc ::llc::err_t testLinearMapSerialization(::llc::ATestError & errors, cnst TLinearMap & source) {
	::llc::au0_t serialized = {};
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_SAVE_RESULT, 0 > serialized.append(LINEAR_MAP_PREFIX), "%s", "Could not append map prefix.");
	cnst ::llc::u2_t mapOffset = serialized.size();
	cnst ::llc::err_t saveResult = source.Save(serialized);
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_SAVE_RESULT, ::llc::failed(saveResult), "Save result:%i.", saveResult);
	cnst ::llc::u2_t mapSize = serialized.size() - mapOffset;
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_SAVE_RESULT, 0 == mapSize, "%s", "Save wrote no map bytes.");
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_SAVE_PREFIX, 0 != memcmp(serialized.begin(), LINEAR_MAP_PREFIX, ::llc::size(LINEAR_MAP_PREFIX)), "%s", "Save changed its preceding field.");
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_SAVE_RESULT, 0 > serialized.append(LINEAR_MAP_SUFFIX), "%s", "Could not append map suffix.");

	::llc::vcu0_t input = {&serialized[mapOffset], mapSize + ::llc::size(LINEAR_MAP_SUFFIX)};
	TLinearMap loaded = {};
	cnst ::llc::err_t loadResult = loaded.Load(input);
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_LOAD_RESULT, ::llc::failed(loadResult), "Load result:%i.", loadResult);
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_LOAD_SIZE, loaded.Keys.size() != loaded.Values.size(), "Key count:%u, value count:%u.", loaded.Keys.size(), loaded.Values.size());
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_LOAD_SIZE, loaded.size() != source.size(), "Loaded size:%u, expected:%u.", loaded.size(), source.size());
	for(::llc::u2_t iPair = 0; iPair < source.size(); ++iPair) {
		LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_LOAD_KEY, loaded.Keys[iPair] != source.Keys[iPair], "Pair:%u, key:%u, expected:%u.", iPair, loaded.Keys[iPair], source.Keys[iPair]);
		LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_LOAD_VALUE, loaded.Values[iPair] != source.Values[iPair], "Pair:%u, value:%" LLC_FMT_U3 ", expected:%" LLC_FMT_U3 ".", iPair, loaded.Values[iPair], source.Values[iPair]);
	}
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_LOAD_CURSOR, input.size() != ::llc::size(LINEAR_MAP_SUFFIX), "Remaining bytes:%u, expected:%u.", input.size(), ::llc::u2_t(::llc::size(LINEAR_MAP_SUFFIX)));
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_LOAD_CURSOR, input.begin() != &serialized[mapOffset + mapSize], "Remaining field begin:%p, expected:%p.", input.begin(), &serialized[mapOffset + mapSize]);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_LOAD_CURSOR, 0 != memcmp(input.begin(), LINEAR_MAP_SUFFIX, ::llc::size(LINEAR_MAP_SUFFIX)), "%s", "Remaining field contents differ.");

	for(::llc::u2_t byteCount = 0; byteCount < mapSize; ++byteCount) {
		::llc::vcu0_t truncated = {&serialized[mapOffset], byteCount};
		cnst ::llc::vcu0_t originalInput = truncated;
		TLinearMap preserved = {};
		LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_TRUNCATED_OUTPUT, 0 > prepareMapSentinel(preserved), "Could not prepare sentinel map at byte:%u.", byteCount);
		cnst ::llc::err_t truncatedResult = preserved.Load(truncated);
		LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_TRUNCATED_RESULT, 0 <= truncatedResult, "Load accepted %u/%u bytes. result:%i.", byteCount, mapSize, truncatedResult);
		LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_TRUNCATED_CURSOR, truncated.begin() != originalInput.begin(), "Byte:%u, cursor begin:%p, expected:%p.", byteCount, truncated.begin(), originalInput.begin());
		LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_TRUNCATED_CURSOR, truncated.size() != originalInput.size(), "Byte:%u, cursor size:%u, expected:%u.", byteCount, truncated.size(), originalInput.size());
		if_fail_fe(checkMapSentinel(errors, LINEAR_MAP_TEST_RESULT_TRUNCATED_OUTPUT, preserved, byteCount));
	}
	return 0;
}

sttc ::llc::err_t testLinearMapInvalidSerialization(::llc::ATestError & errors) {
	cnst ::llc::u2_t keys[] = {1, 2};
	cnst ::llc::u3_t oneValue[] = {10};
	::llc::au0_t mismatched = {};
	if_fail_fe(::llc::saveView(mismatched, ::llc::view<cnst ::llc::u2_t>{keys}));
	if_fail_fe(::llc::saveView(mismatched, ::llc::view<cnst ::llc::u3_t>{oneValue}));
	::llc::vcu0_t mismatchInput = mismatched;
	cnst ::llc::vcu0_t originalMismatchInput = mismatchInput;
	TLinearMap mismatchOutput = {};
	if_fail_fe(prepareMapSentinel(mismatchOutput));
	cnst ::llc::err_t mismatchResult = mismatchOutput.Load(mismatchInput);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_COUNT_MISMATCH_RESULT, 0 <= mismatchResult, "Load accepted %u keys and %u values. result:%i.", ::llc::u2_t(::llc::size(keys)), ::llc::u2_t(::llc::size(oneValue)), mismatchResult);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_COUNT_MISMATCH_CURSOR, mismatchInput.begin() != originalMismatchInput.begin(), "Cursor begin:%p, expected:%p.", mismatchInput.begin(), originalMismatchInput.begin());
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_COUNT_MISMATCH_CURSOR, mismatchInput.size() != originalMismatchInput.size(), "Cursor size:%u, expected:%u.", mismatchInput.size(), originalMismatchInput.size());
	if_fail_fe(checkMapSentinel(errors, LINEAR_MAP_TEST_RESULT_COUNT_MISMATCH_OUTPUT, mismatchOutput, 0));

	cnst ::llc::u2_t duplicateKeys[] = {7, 7};
	cnst ::llc::u3_t duplicateValues[] = {70, 71};
	::llc::au0_t duplicate = {};
	if_fail_fe(::llc::saveView(duplicate, ::llc::view<cnst ::llc::u2_t>{duplicateKeys}));
	if_fail_fe(::llc::saveView(duplicate, ::llc::view<cnst ::llc::u3_t>{duplicateValues}));
	::llc::vcu0_t duplicateInput = duplicate;
	TLinearMap duplicateOutput = {};
	if_fail_fe(prepareMapSentinel(duplicateOutput));
	cnst ::llc::err_t duplicateResult = duplicateOutput.Load(duplicateInput);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_DUPLICATE_LOAD_RESULT, 0 <= duplicateResult, "Load accepted duplicate key 7. result:%i.", duplicateResult);
	if_fail_fe(checkMapSentinel(errors, LINEAR_MAP_TEST_RESULT_DUPLICATE_LOAD_OUTPUT, duplicateOutput, 0));

	TLinearMap invalidSave = {};
	if_fail_fe(invalidSave.push_back(1, 10));
	invalidSave.Values.pop_back();
	::llc::au0_t output = LINEAR_MAP_PREFIX;
	cnst ::llc::err_t invalidSaveResult = invalidSave.Save(output);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_INVALID_SAVE_RESULT, 0 <= invalidSaveResult, "Save accepted %u keys and %u values. result:%i.", invalidSave.Keys.size(), invalidSave.Values.size(), invalidSaveResult);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_INVALID_SAVE_OUTPUT, output.size() != ::llc::size(LINEAR_MAP_PREFIX), "Output size:%u, expected:%u.", output.size(), ::llc::u2_t(::llc::size(LINEAR_MAP_PREFIX)));
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_INVALID_SAVE_OUTPUT, 0 != memcmp(output.begin(), LINEAR_MAP_PREFIX, ::llc::size(LINEAR_MAP_PREFIX)), "%s", "Rejected Save changed existing output bytes.");
	return 0;
}

sttc ::llc::err_t prepareObjectMapSentinel(TObjectLinearMap & map) {
	cnst ::llc::err_t index = map.Create("sentinel");
	llc_necs(index);
	map[index]->Value = 999;
	return index;
}

sttc ::llc::err_t checkObjectMapSentinel(::llc::ATestError & errors, cnst TObjectLinearMap & map, ::llc::u2_c byteCount) {
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_TRUNCATED_OUTPUT, map.Names.size() != 1, "Byte:%u, name count:%u, expected:1.", byteCount, map.Names.size());
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_TRUNCATED_OUTPUT, map.Elements.size() != 1, "Byte:%u, element count:%u, expected:1.", byteCount, map.Elements.size());
	if(map.Names.size())
		LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_TRUNCATED_OUTPUT, map.Names[0] != ::llc::vcst_t{"sentinel"}, "Byte:%u, name:%.*s, expected:sentinel.", byteCount, LLC_VIEW_ARGS(map.Names[0]));
	if(map.Elements.size()) {
		LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_TRUNCATED_OUTPUT, 0 == map.Elements[0], "Byte:%u, sentinel element became null.", byteCount);
		if(map.Elements[0])
			LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_TRUNCATED_OUTPUT, map.Elements[0]->Value != 999, "Byte:%u, value:%u, expected:999.", byteCount, map.Elements[0]->Value);
	}
	return 0;
}

sttc ::llc::err_t testObjectLinearMap(::llc::ATestError & errors) {
	TObjectLinearMap source = {};
	cnst ::llc::err_t firstIndex = source.Create("first");
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_POBJ_CREATE_INDEX, firstIndex != 0, "First index:%i, expected:0.", firstIndex);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_CREATE_SIZE, source.Names.size() != source.Elements.size(), "Name count:%u, element count:%u.", source.Names.size(), source.Elements.size());
	source[firstIndex]->Value = 100;
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_CREATE_VALUE, 0 == source[firstIndex], "%s", "Created object remained null after non-const access.");
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_CREATE_VALUE, source[firstIndex]->Value != 100, "Created value:%u, expected:100.", source[firstIndex]->Value);

	cnst ::llc::err_t cloneIndex = source.Clone(firstIndex);
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_POBJ_CLONE_INDEX, cloneIndex != 1, "Clone index:%i, expected:1.", cloneIndex);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_CLONE_NAME, source.Names[cloneIndex] != source.Names[firstIndex], "Clone name:%.*s, source:%.*s.", LLC_VIEW_ARGS(source.Names[cloneIndex]), LLC_VIEW_ARGS(source.Names[firstIndex]));
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_POBJ_CLONE_VALUE, 0 == source[cloneIndex], "%s", "Clone is null.");
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_CLONE_VALUE, source[cloneIndex].operator->() == source[firstIndex].operator->(), "Clone address:%p, source:%p.", source[cloneIndex].operator->(), source[firstIndex].operator->());
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_CLONE_VALUE, source[cloneIndex]->Value != source[firstIndex]->Value, "Clone value:%u, source:%u.", source[cloneIndex]->Value, source[firstIndex]->Value);
	source[cloneIndex]->Value = 101;
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_CLONE_VALUE, source[firstIndex]->Value != 100, "Source value changed to:%u after clone mutation.", source[firstIndex]->Value);

	cnst ::llc::err_t secondIndex = source.Create("second");
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_POBJ_CREATE_INDEX, secondIndex != 2, "Second index:%i, expected:2.", secondIndex);
	source[secondIndex]->Value = 200;
	cnst ::llc::err_t deleteResult = source.Delete(firstIndex);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_DELETE_RESULT, deleteResult != 2, "Delete result:%i, expected:2.", deleteResult);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_DELETE_PAIR, source.Names.size() != source.Elements.size(), "Name count:%u, element count:%u.", source.Names.size(), source.Elements.size());
	cnst ::llc::err_t movedIndex = source.find("second");
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_POBJ_DELETE_PAIR, movedIndex != 0, "Moved second index:%i, expected:0.", movedIndex);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_DELETE_PAIR, source[movedIndex]->Value != 200, "Moved value:%u, expected:200.", source[movedIndex]->Value);

	::llc::au0_t serialized = {};
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_POBJ_SAVE_RESULT, 0 > serialized.append(LINEAR_MAP_PREFIX), "%s", "Could not append object-map prefix.");
	cnst ::llc::u2_t mapOffset = serialized.size();
	cnst ::llc::err_t saveResult = source.Save(serialized);
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_POBJ_SAVE_RESULT, ::llc::failed(saveResult), "Save result:%i.", saveResult);
	cnst ::llc::u2_t mapSize = serialized.size() - mapOffset;
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_POBJ_SAVE_RESULT, 0 == mapSize, "%s", "Save wrote no object-map bytes.");
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_POBJ_SAVE_RESULT, 0 > serialized.append(LINEAR_MAP_SUFFIX), "%s", "Could not append object-map suffix.");

	::llc::vcu0_t input = {&serialized[mapOffset], mapSize + ::llc::size(LINEAR_MAP_SUFFIX)};
	TObjectLinearMap loaded = {};
	cnst ::llc::err_t loadResult = loaded.Load(input);
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_POBJ_LOAD_RESULT, ::llc::failed(loadResult), "Load result:%i.", loadResult);
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_POBJ_LOAD_SIZE, loaded.Names.size() != loaded.Elements.size(), "Name count:%u, element count:%u.", loaded.Names.size(), loaded.Elements.size());
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_POBJ_LOAD_SIZE, loaded.size() != source.size(), "Loaded size:%u, expected:%u.", loaded.size(), source.size());
	for(::llc::u2_t iElement = 0; iElement < source.size(); ++iElement) {
		LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_LOAD_NAME, loaded.Names[iElement] != source.Names[iElement], "Element:%u, name:%.*s, expected:%.*s.", iElement, LLC_VIEW_ARGS(loaded.Names[iElement]), LLC_VIEW_ARGS(source.Names[iElement]));
		LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_POBJ_LOAD_VALUE, 0 == loaded.Elements[iElement], "Element:%u loaded as null.", iElement);
		LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_LOAD_VALUE, loaded.Elements[iElement]->Value != source.Elements[iElement]->Value, "Element:%u, value:%u, expected:%u.", iElement, loaded.Elements[iElement]->Value, source.Elements[iElement]->Value);
	}
	LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_POBJ_LOAD_CURSOR, input.size() != ::llc::size(LINEAR_MAP_SUFFIX), "Remaining bytes:%u, expected:%u.", input.size(), ::llc::u2_t(::llc::size(LINEAR_MAP_SUFFIX)));
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_LOAD_CURSOR, input.begin() != &serialized[mapOffset + mapSize], "Remaining begin:%p, expected:%p.", input.begin(), &serialized[mapOffset + mapSize]);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_LOAD_CURSOR, 0 != memcmp(input.begin(), LINEAR_MAP_SUFFIX, ::llc::size(LINEAR_MAP_SUFFIX)), "%s", "Remaining field contents differ.");

	for(::llc::u2_t byteCount = 0; byteCount < mapSize; ++byteCount) {
		::llc::vcu0_t truncated = {&serialized[mapOffset], byteCount};
		cnst ::llc::vcu0_t originalInput = truncated;
		TObjectLinearMap preserved = {};
		LLC_TEST_REQUIREF(errors, LINEAR_MAP_TEST_RESULT_POBJ_TRUNCATED_OUTPUT, 0 > prepareObjectMapSentinel(preserved), "Could not prepare object-map sentinel at byte:%u.", byteCount);
		cnst ::llc::err_t truncatedResult = preserved.Load(truncated);
		LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_TRUNCATED_RESULT, 0 <= truncatedResult, "Load accepted %u/%u bytes. result:%i.", byteCount, mapSize, truncatedResult);
		LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_TRUNCATED_CURSOR, truncated.begin() != originalInput.begin(), "Byte:%u, cursor begin:%p, expected:%p.", byteCount, truncated.begin(), originalInput.begin());
		LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_TRUNCATED_CURSOR, truncated.size() != originalInput.size(), "Byte:%u, cursor size:%u, expected:%u.", byteCount, truncated.size(), originalInput.size());
		if_fail_fe(checkObjectMapSentinel(errors, preserved, byteCount));
	}

	TObjectLinearMap nullMap = {};
	if_fail_fe(nullMap.Create("null"));
	::llc::au0_t output = LINEAR_MAP_PREFIX;
	cnst ::llc::err_t nullSaveResult = nullMap.Save(output);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_NULL_SAVE_RESULT, 0 <= nullSaveResult, "Save accepted a null element. result:%i.", nullSaveResult);
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_NULL_SAVE_OUTPUT, output.size() != ::llc::size(LINEAR_MAP_PREFIX), "Output size:%u, expected:%u.", output.size(), ::llc::u2_t(::llc::size(LINEAR_MAP_PREFIX)));
	LLC_TEST_CHECKF(errors, LINEAR_MAP_TEST_RESULT_POBJ_NULL_SAVE_OUTPUT, 0 != memcmp(output.begin(), LINEAR_MAP_PREFIX, ::llc::size(LINEAR_MAP_PREFIX)), "%s", "Rejected Save changed existing output bytes.");
	return 0;
}

::llc::err_t testLinearMap(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	TLinearMap map = {};
	if_fail_fe(testLinearMapOperations(errors, map));
	if_fail_fe(testLinearMapSerialization(errors, map));
	if_fail_fe(testLinearMapInvalidSerialization(errors));
	return testObjectLinearMap(errors);
}
