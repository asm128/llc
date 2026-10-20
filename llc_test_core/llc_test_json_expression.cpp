#include "llc_test_core.h"
#include "llc_json_expression.h"
#include "llc_file.h"

GDEFINE_ENUM_TYPE(JSON_EXPRESSION_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, OK                       ,  0, "All JSON-expression tests passed.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, PARSE                    ,  1, "jsonParse() rejected the JSON-expression fixture.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, DIRECT_RESULT            ,  2, "Direct path resolution failed.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, DIRECT_INDEX             ,  3, "Direct path resolution did not return a JSON node index.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, DIRECT_VALUE             ,  4, "Direct path resolution returned the wrong value.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, NESTED_RESULT            ,  5, "Resolution from a nested JSON node failed.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, NESTED_VALUE             ,  6, "Resolution from a nested JSON node returned the wrong value.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, ARRAY_RESULT             ,  7, "Array indexing failed.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, ARRAY_VALUE              ,  8, "Array indexing returned the wrong value.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, STRING_RESULT            ,  9, "String indexing failed.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, STRING_VALUE             , 10, "String indexing returned the wrong character.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, DYNAMIC_RESULT           , 11, "Dynamic-key resolution failed.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, DYNAMIC_VALUE            , 12, "Dynamic-key resolution returned the wrong value.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, EQUAL_RESULT             , 13, "Equality evaluation failed.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, EQUAL_VALUE              , 14, "Equality evaluation returned the wrong boolean.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, NOT_RESULT               , 15, "Unary-not evaluation failed.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, NOT_VALUE                , 16, "Unary-not evaluation returned the wrong boolean.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, DECIMAL_ZERO_RESULT      , 17, "Decimal-zero boolean evaluation failed.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, DECIMAL_ZERO_VALUE       , 18, "Negative decimal zero evaluated as true.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, AND_RESULT               , 19, "Boolean-and evaluation failed.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, AND_VALUE                , 20, "Boolean-and evaluation returned the wrong boolean.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, OR_RESULT                , 21, "Boolean-or evaluation failed.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, OR_VALUE                 , 22, "Boolean-or evaluation returned the wrong boolean.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, TERNARY_RESULT           , 23, "Conditional evaluation failed.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, TERNARY_VALUE            , 24, "Conditional evaluation returned the wrong branch.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, FORMAT_RESULT            , 25, "JSON string formatting failed.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, FORMAT_VALUE             , 26, "JSON string formatting returned the wrong text.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, FORMAT_ADJACENT_RESULT   , 27, "Formatting leading and adjacent tokens failed.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, FORMAT_ADJACENT_VALUE    , 28, "Leading or adjacent format tokens were misaligned.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, MISSING_RESULT           , 29, "A missing key was accepted.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, MISSING_OUTPUT           , 30, "A failed lookup changed the caller output.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, ROOT_RANGE_RESULT        , 31, "An out-of-range root node was accepted.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, ROOT_RANGE_OUTPUT        , 32, "An out-of-range root lookup changed the caller output.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, ARRAY_RANGE_RESULT       , 33, "An out-of-range array index was accepted.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, ARRAY_RANGE_OUTPUT       , 34, "An out-of-range array lookup changed the caller output.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, STRING_RANGE_RESULT      , 35, "An out-of-range string index was accepted.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, STRING_RANGE_OUTPUT      , 36, "An out-of-range string lookup changed the caller output.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, SOLVER_RESULT            , 37, "A failed solver lookup was accepted.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, SOLVER_OUTPUT            , 38, "A failed solver lookup changed the caller output.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, SOLVER_STATE             , 39, "A failed solver lookup retained partial evaluation steps.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, FORMAT_FAILURE_RESULT    , 40, "Formatting accepted an unresolved token.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_TEST_RESULT, FORMAT_FAILURE_OUTPUT    , 41, "Failed formatting changed the caller output.");

GDEFINE_ENUM_TYPE(JSON_EXPRESSION_HISTORICAL_TEST_RESULT, ::llc::u1_t);
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_HISTORICAL_TEST_RESULT, PARSE        ,   0, "jsonParse() rejected the historical JSON-expression fixture.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_HISTORICAL_TEST_RESULT, FORMAT_RESULT, 359, "Historical JSON string formatting failed.");
GDEFINE_ENUM_VALUED(JSON_EXPRESSION_HISTORICAL_TEST_RESULT, FORMAT_VALUE , 360, "Historical JSON string formatting returned the wrong text.");

#define LLC_JSON_EXPRESSION_HISTORICAL_CASE(caseName, caseIndex, expected, expression) \
	GDEFINE_ENUM_VALUED(JSON_EXPRESSION_HISTORICAL_TEST_RESULT, caseName##_RESULT, 1 + caseIndex * 2, "Historical JSON-expression " #caseName " did not resolve."); \
	GDEFINE_ENUM_VALUED(JSON_EXPRESSION_HISTORICAL_TEST_RESULT, caseName##_VALUE , 2 + caseIndex * 2, "Historical JSON-expression " #caseName " returned the wrong value.");
#define LLC_JSON_EXPRESSION_HISTORICAL_CASES() \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_000, 0, "true", "weight			== bpm") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_001, 1, "true", "height			== hindsight") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_002, 2, "true", "hindsight		== height") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_003, 3, "true", "bpm				== weight") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_004, 4, "false", "height			== weight") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_005, 5, "false", "hindsight		== bpm") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_006, 6, "false", "weight			== height") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_007, 7, "false", "bpm				== hindsight") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_008, 8, "false", "'Not'			== children[('0')]") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_009, 9, "false", "'Found'			== children[('1')]") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_010, 10, "false", "'Here'			== children[('2')]") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_011, 11, "false", "'Perro'			== 'Perre'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_012, 12, "true", "'null'			== 'false'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_013, 13, "true", "'null'			== ''") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_014, 14, "true", "'false'			== 'null'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_015, 15, "true", "'false'			== ''") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_016, 16, "true", "	'false'			== 'null'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_017, 17, "true", "	''				== 'null'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_018, 18, "true", "	'null'			== 'false'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_019, 19, "true", "	''				== 'false'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_020, 20, "true", "'Rene'			== children['0']") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_021, 21, "true", "'null'			== children['1']") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_022, 22, "true", "'false'			== children['1']") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_023, 23, "true", "children[('1')]	== children['1']") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_024, 24, "false", "children[('2')]	== children[('1')]") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_025, 25, "true", "'Jamie'			== children[('2')]") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_026, 26, "false", "alive			== married") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_027, 27, "false", "!alive			== !married") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_028, 28, "true", "(!alive)			==(married)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_029, 29, "true", "!alive			==married") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_030, 30, "false", "(!alive)			==(!married)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_031, 31, "true", "(alive )			==(!married)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_032, 32, "false", "!alive			==!married") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_033, 33, "true", "!alive			==married") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_034, 34, "false", "(alive )			=={ 'married'}") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_035, 35, "true", "(alive )			==!{ 'married'}") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_036, 36, "false", "'false'			== 'true'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_037, 37, "true", "'true'			== 'true'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_038, 38, "true", "'false'			== 'false'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_039, 39, "true", "'Perro'			== 'Perro'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_040, 40, "false", "(alive )			&&{ 'married'}") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_041, 41, "false", "(alive )			&&( married)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_042, 42, "true", "'false'			|| 'true'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_043, 43, "false", "(alive )			&&(married)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_044, 44, "false", "(alive)			&&(married)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_045, 45, "false", "(alive)			&&( married)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_046, 46, "false", "'false'			&& 'false'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_047, 47, "false", "'false'			|| 'false'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_048, 48, "true", "'true'			|| 'false'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_049, 49, "false", "!married			 ? !alive : 'unknown'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_050, 50, "unknown", "married			? !alive : 'unknown'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_051, 51, "false", "married			? 'unknown' : !alive") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_052, 52, "unknown", "!married			 ? 'unknown' : !alive") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_053, 53, "true", "(alive )			&&!(married)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_054, 54, "false", "!(alive )		&&!( married)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_055, 55, "false", "!(alive )		&&!(married)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_056, 56, "false", "!(alive)			&&!(married)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_057, 57, "false", "!(alive)			&&!( married)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_058, 58, "unknown", "married			&& alive && married ? !alive : 'unknown'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_059, 59, "true", "(alive && alive)	&& alive &&!( married)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_060, 60, "true", "(alive)			&&!(married)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_061, 61, "true", "(alive)			&&!( married)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_062, 62, "true", "(!married) &&!( married)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_063, 63, "false", "{'alive'}&&( married)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_064, 64, "false", "{'alive'}&&(married)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_065, 65, "false", "{'alive'}&&(married)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_066, 66, "false", "{'alive'} &&( married)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_067, 67, "false", "(alive )&&{'married' }") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_068, 68, "false", "(alive) &&{'married' }") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_069, 69, "false", "(alive) &&{ 'married'}") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_070, 70, "false", "alive && married") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_071, 71, "false", "alive &&married") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_072, 72, "false", "alive&&married") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_073, 73, "false", "alive&& married") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_074, 74, "it's alive!", "(!(!(alive))) ? \"it's alive!\" : 'dead!'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_075, 75, "true", "alive") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_076, 76, "it's alive!", " alive  ? \"it's alive!\" : 'dead!'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_077, 77, "true", "(alive)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_078, 78, "it's alive!", "(alive) ? \"it's alive!\" : 'dead!'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_079, 79, "false", "!(alive)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_080, 80, "false", "(!(alive))") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_081, 81, "true", "!(!(alive))") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_082, 82, "true", "(!(!(alive)))") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_083, 83, "Carlos", "name") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_084, 84, "1.60", "height") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_085, 85, "brown", "color") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_086, 86, "b", "color['0']") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_087, 87, "red", "race") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_088, 88, "160", "weight") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_089, 89, "Carlos", "(name)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_090, 90, "1.60", "(height)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_091, 91, "brown", "(color)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_092, 92, "red", "(race)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_093, 93, "160", "(weight)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_094, 94, "2", "selection.index") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_095, 95, "2", "(selection.('index'))") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_096, 96, "2", "((selection).index)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_097, 97, "Not married", " married  ? 'true' : 'Not married'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_098, 98, "Not married", "(married) ? 'true' : 'Not married'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_099, 99, "true", " alive  ? ('true') : ('false')") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_100, 100, "true", "!!(!!alive) ? ('true') : ('false')") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_101, 101, "false", " married  ? ('true') : ('false')") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_102, 102, "false", "(married) ? ('true') : ('false')") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_103, 103, "true", "(alive  ? (married ? 'false' : 'true') )") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_104, 104, "true", "(alive  ? married ? 'false' : 'true' : 'unknown')") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_105, 105, "alive", "(!alive || married ? 'dead' : 'alive')") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_106, 106, "true", " alive  ? married ? 'false' : 'true' : 'unknown'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_107, 107, "alive", "(alive && !married ? 'alive' : 'dead')") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_108, 108, "it's alive!", "!alive? 'dead!' : \"it's alive!\"") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_109, 109, "it's alive!", "!!(alive) ? \"it's alive!\" : 'dead!'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_110, 110, "it's alive!", "(!alive) ? 'dead!' : \"it's alive!\"") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_111, 111, "Rene", "children[('0')]") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_112, 112, "null", "children[('1')]") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_113, 113, "Jamie", "children[('2')]") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_114, 114, "Jamie", "children[selection.index]") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_115, 115, "Jamie", "children[(selection.('index'))]") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_116, 116, "Jamie", "children[({'selection'}.index)]") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_117, 117, "Jamie", "children[{'selection.index'}]") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_118, 118, "Alice", "parents['0'].name") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_119, 119, "Bob", "parents['1'].name") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_120, 120, "Alice", "'Alice'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_121, 121, "Alice", "((parents['0']).name)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_122, 122, "Bob", "((parents['1']).name)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_123, 123, "Alice", "{\"(parents['0']).name\"}") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_124, 124, "Bob", "{\"(parents['1']).name\"}") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_125, 125, "2", "(selection . ('index'))") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_126, 126, "2", "(selection . index)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_127, 127, "null", "(children[ ( ( '1') )] )") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_128, 128, "Jamie", "(children[((selection.index)) ])") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_129, 129, "is alive", "(alive ? married ? 'not alive' : 'is alive' : 'unknown')") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_130, 130, "false", "!alive  ? ('is_alive') : (!'is dead')") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_131, 131, "is alive", "(alive  ? (!alive ? 'is_dead' : 'is alive'))") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_132, 132, "red", "married  ? !alive ? 'is_dead' : 'is_alive' : race") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_133, 133, "unknown", "(!alive  ? (alive ? 'is_alive' : 'is_dead') : 'unknown')") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_134, 134, "true", "!'0'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_135, 135, "true", "!''") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_136, 136, "false", "!' '") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_137, 137, "true", "!'{}'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_138, 138, "true", "!'[]'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_139, 139, "true", "!'0'	&&	!'0'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_140, 140, "true", "!''	&&	!''") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_141, 141, "false", "!' '	&&	!' '") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_142, 142, "true", "!'{}'&&	!'{}'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_143, 143, "true", "!'[]'&&	!'[]'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_144, 144, "true", "!'0'	||	!'0'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_145, 145, "true", "!''	||	!''") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_146, 146, "false", "!' '	||	!' '") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_147, 147, "true", "!'{}'||	!'{}'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_148, 148, "true", "!'[]'||	!'[]'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_149, 149, "true", "'true'  || 'true'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_150, 150, "32", "parents['1'].property.{selection.default_property_name_path}") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_151, 151, "0", "!selection.active		? ('0') : selection.index") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_152, 152, "0", "!(selection.active)		? ('0') : selection.index") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_153, 153, "2", "(!(selection.active))	? (selection.index)	: '0'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_154, 154, "false", "selection.active") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_155, 155, "false", "(selection.active)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_156, 156, "2", "(selection.active)	? ('0')	: (selection.index)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_157, 157, "race", "!selection.active ? properties[selection.index].name : 'No selection.'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_158, 158, "No selection.", "selection.active ? 'is active' : 'No selection.'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_159, 159, "age", "properties[('0')].name") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_160, 160, "2", "(!selection.active) ? (selection.index) : ('1')") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_161, 161, "No selection.", "selection.active ? properties[selection.index].name : 'No selection.'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_162, 162, "race", "selection.active ? 'No selection.' : properties[selection.index].name") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_163, 163, "green", "parents['1'].property.(properties[selection.active ? selection.index : '1'].name)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_164, 164, "green", "parents['1'].property.(properties[selection.active ? (selection.index) : '1'].name)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_165, 165, "thin", "parents['1'].property.(properties[!selection.active ? (selection.index) : ('1')].name)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_166, 166, "green", "parents['1'].property.(properties[(selection.active) ? selection.index : '1'].name)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_167, 167, "16", "parents['0'].property.(properties[(selection.active) ? selection.index : '0'].name)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_168, 168, "2", "!selection.active	? selection.index : '1'") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_169, 169, "1", "(!selection.active)	? '1' : selection.index") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_170, 170, "green", "parents['1'].property.(properties[!selection.active		? '1'	: selection.index	].name)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_171, 171, "green", "parents['1'].property.(properties[!!!selection.active || married	? '1' : (selection.index)].name)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_172, 172, "green", "parents['1'].property.(properties[!!!selection.active || married	? '1' : (selection.index)].name)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_173, 173, "green", "parents['1'].property.(properties[!selection.active		? ('1')	: (selection.index) ].name)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_174, 174, "32", "parents['1'].property.{selection.default_property_name_path}") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_175, 175, "orange", "parents['0'].property.(properties[(!selection.active) ? '1'	: selection.index	].name)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_176, 176, "orange", "parents['0'].property.(properties[!(selection.active) ? '1'	: selection.index	].name)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_177, 177, "green", "parents['1'].property.(properties[!(!!selection.active) && (married || alive) ? '1' : (selection.index)].name)") \
	LLC_JSON_EXPRESSION_HISTORICAL_CASE(CASE_178, 178, "green", "parents['1'].property.(properties[!(!!selection.active) && (alive || married) ? '1' : (selection.index)].name)")
LLC_JSON_EXPRESSION_HISTORICAL_CASES()
#undef LLC_JSON_EXPRESSION_HISTORICAL_CASE

struct SJSONExpressionHistoricalCase {
	JSON_EXPRESSION_HISTORICAL_TEST_RESULT ResultCase = {};
	JSON_EXPRESSION_HISTORICAL_TEST_RESULT ValueCase = {};
	::llc::vcst_t Expression = {};
	::llc::vcst_t Expected = {};
};

#define LLC_JSON_EXPRESSION_HISTORICAL_CASE(caseName, caseIndex, expected, expression) {JSON_EXPRESSION_HISTORICAL_TEST_RESULT_##caseName##_RESULT, JSON_EXPRESSION_HISTORICAL_TEST_RESULT_##caseName##_VALUE, expression, expected},
stxp SJSONExpressionHistoricalCase JSON_EXPRESSION_HISTORICAL_CASES[] = {LLC_JSON_EXPRESSION_HISTORICAL_CASES()};
#undef LLC_JSON_EXPRESSION_HISTORICAL_CASE
#undef LLC_JSON_EXPRESSION_HISTORICAL_CASES

sttc void testJSONExpressionTrace(const ::llc::SJSONReader & reader, ::llc::u2_t root, ::llc::vcst_t expression, ::llc::vcst_t expected, ::llc::vcst_t actual, ::llc::err_t result, bool expectFailure = false, bool formatting = false) {
	cnst bool resultMatches = expectFailure ? result < 0 : formatting ? result >= 0 : result != -1;
	cnst char * type = expectFailure ? "caller output (must remain unchanged)" : formatting ? "formatted text" : result == -1 ? "resolution failed" : result == -5 ? "computed boolean" : "expression text (no JSON node)";
	if(!expectFailure && !formatting && result >= 0 && (::llc::u2_t)result < reader.Tree.size() && reader.Tree[result] && reader.Tree[result]->Token)
		type = ::llc::get_value_namep(reader.Tree[result]->Token->Type);
	always_printf("\n--- %s: %s ---\nRoot node: %u\nExpression/template (%u bytes):\n%.*s\nExpected return: %s\nActual return: %i\nActual type/origin: %s\nExpected output (%u bytes):\n%.*s\nActual output (%u bytes):\n%.*s\n--- end case ---\n"
		, formatting ? "FORMAT" : "RESOLVE", resultMatches && actual == expected ? "PASS" : "FAIL", root
		, expression.size(), LLC_VIEW_ARGS(expression), expectFailure ? "negative (failure)" : formatting ? "nonnegative (success)" : "anything except -1 (node index or computed-value sentinel)"
		, result, type, expected.size(), LLC_VIEW_ARGS(expected), actual.size(), LLC_VIEW_ARGS(actual));
	if(root < reader.View.size())
		always_printf("Root context:\n%.*s\n", LLC_VIEW_ARGS(reader.View[root]));
}

sttc ::llc::err_t testJSONExpressionResolve(ATestError & errors, const ::llc::SJSONReader & reader, ::llc::u2_t indexNodeJSON, ::llc::vcst_t expression, ::llc::vcst_t expected, JSON_EXPRESSION_TEST_RESULT resultCase, JSON_EXPRESSION_TEST_RESULT valueCase, ::llc::err_t & indexResult) {
	::llc::vcst_t output = "unchanged";
	indexResult = ::llc::jsonExpressionResolve(expression, reader, indexNodeJSON, output);
	::testJSONExpressionTrace(reader, indexNodeJSON, expression, expected, output, indexResult);
	LLC_TEST_REQUIREF(errors, resultCase, -1 == indexResult, "Expression:'%.*s', result:%i.", LLC_VIEW_ARGS(expression), indexResult);
	LLC_TEST_CHECKF(errors, valueCase, output != expected, "Expression:'%.*s', output:'%.*s', expected:'%.*s'.", LLC_VIEW_ARGS(expression), LLC_VIEW_ARGS(output), LLC_VIEW_ARGS(expected));
	rtrn 0;
}

sttc ::llc::err_t testJSONExpressionHistorical(ATestError & errors, ::llc::vcst_t outputFolder) {
	cnst ::llc::vcst_t document = "\n{\t \"name\"           : \"Carlos\""
		  "\n\t, \"height\"         : 1.60"
		  "\n\t, \"color\"          : \"brown\""
		  "\n\t, \"race\"           : \"red\""
		  "\n\t, \"weight\"         : 160"
		  "\n\t, \"bpm\"            : 160."
		  "\n\t, \"hindsight\"      : 1.600000000"
		  "\n\t, \"alive\"          : true"
		  "\n\t, \"married\"        : false"
		  "\n\t, \"children\"       : [\"Rene\", null, \"Jamie\"]"
		  "\n\t, \"selection\"      : { \"index\" : 2, \"name\" : \"children\", \"active\" : false , \"default_property_name_path\" : \"properties['0'].name\" }"
		  "\n\t, \"parents\"        : "
		  "\n\t    [{ \"name\"    : \"Alice\""
		  "\n\t    , \"property\" : { \"color\" : \"orange\", \"age\" : 16, \"race\" : \"fat\" }"
		  "\n\t    },"
		  "\n\t    { \"name\"     : \"Bob\" // this is another comment"
		  "\n\t    , \"property\" : { \"color\" : \"green\", \"age\" : 32, \"race\" : \"thin\" }"
		  "\n\t    }]"
		  "\n\t, \"properties\" : [{ \"name\" : \"age\", \"type\" : \"int\"}, { \"name\" : \"color\", \"type\" : \"string\"}, { \"name\" : \"race\", \"type\" : \"string\"} ]"
		  "\n}"
		;
	if_fail_fe(::llc::fileFromMemory(outputFolder, "historical_input.jsonc", document.cu8()));
	always_printf("FIXTURE: historical_input.jsonc (exact parser input, including comments)\n%.*s", LLC_VIEW_ARGS(document));
	::llc::SJSONReader reader = {};
	cnst ::llc::err_t parseResult = ::llc::jsonParse(reader, document);
	LLC_TEST_REQUIREF(errors, JSON_EXPRESSION_HISTORICAL_TEST_RESULT_PARSE, ::llc::failed(parseResult), "Result:%i, document:'%.*s'.", parseResult, LLC_VIEW_ARGS(document));

	for(::llc::u2_t iCase = 0; iCase < ::llc::size(JSON_EXPRESSION_HISTORICAL_CASES); ++iCase) {
		cnst SJSONExpressionHistoricalCase & expressionCase = JSON_EXPRESSION_HISTORICAL_CASES[iCase];
		::llc::vcst_t output = "unchanged";
		cnst ::llc::err_t result = ::llc::jsonExpressionResolve(expressionCase.Expression, reader, 0, output);
		always_printf("Historical case: %u (%s)", iCase, ::llc::get_value_namep(expressionCase.ValueCase));
		::testJSONExpressionTrace(reader, 0, expressionCase.Expression, expressionCase.Expected, output, result);
		LLC_TEST_CHECKF(errors, expressionCase.ResultCase, -1 == result, "Case:%u, expression:'%.*s', result:%i.", iCase, LLC_VIEW_ARGS(expressionCase.Expression), result);
		LLC_TEST_CHECKF(errors, expressionCase.ValueCase, output != expressionCase.Expected, "Case:%u, expression:'%.*s', output:'%.*s', expected:'%.*s'.", iCase, LLC_VIEW_ARGS(expressionCase.Expression), LLC_VIEW_ARGS(output), LLC_VIEW_ARGS(expressionCase.Expected));
	}

	cnst SJSONExpressionHistoricalCase & walkthrough = JSON_EXPRESSION_HISTORICAL_CASES[::llc::size(JSON_EXPRESSION_HISTORICAL_CASES) - 1];
	::llc::SExpressionReader expressionReader = {};
	if_fail_fe(::llc::expressionReaderParse(expressionReader, walkthrough.Expression));
	::llc::SJSONExpressionSolver solver = {};
	::llc::vcst_t solved = "unchanged";
	cnst ::llc::err_t solverResult = ::llc::jsonExpressionResolve(expressionReader, reader, 0, solver, solved);
	always_printf("%s", "SOLVER WALKTHROUGH: additional evaluation of the final historical case, with retained intermediate results.");
	::testJSONExpressionTrace(reader, 0, walkthrough.Expression, walkthrough.Expected, solved, solverResult);
	for(::llc::u2_t iStep = 0; iStep < solver.Results.size(); ++iStep) {
		cnst ::llc::SEvaluationStepResult & step = solver.Results[iStep];
		always_printf("Step %u | root:%u | result:%i | boolean carry:%u\nExpression:\n%.*s\nOutput (%u bytes):\n%.*s", iStep, step.IndexRootJSONNode, step.IndexJSONResult, (::llc::u2_t)step.LastValueCarry, LLC_VIEW_ARGS(step.Expression), step.Output.size(), LLC_VIEW_ARGS(step.Output));
	}
	LLC_TEST_CHECKF(errors, walkthrough.ResultCase, solverResult == -1, "Walkthrough result:%i.", solverResult);
	LLC_TEST_CHECKF(errors, walkthrough.ValueCase, solved != walkthrough.Expected, "Walkthrough output:'%.*s'.", LLC_VIEW_ARGS(solved));

	cnst ::llc::vcst_t format = "I want to replace this (but not \\{this}): \n{parents['1'].name}'s {properties[selection.active ? selection.index : '1'].name}: // this comment should appear here"
		  "\n{parents['1'].property.(properties[selection.active ? selection.index : '1'].name // this comment should not appear here"
		  "\n)}."
		;
	::llc::asc_t formatted = {};
	cnst ::llc::err_t formatResult = ::llc::jsonStringFormat(format, reader, 0, formatted);
	cnst ::llc::vcst_t expected = "I want to replace this (but not \\{this}): \nBob's color: // this comment should appear here\ngreen.";
	::testJSONExpressionTrace(reader, 0, format, expected, {formatted.begin(), formatted.size()}, formatResult, false, true);
	LLC_TEST_REQUIREF(errors, JSON_EXPRESSION_HISTORICAL_TEST_RESULT_FORMAT_RESULT, ::llc::failed(formatResult), "Result:%i, format:'%.*s'.", formatResult, LLC_VIEW_ARGS(format));
	cnst ::llc::vcst_t formattedView = {formatted.begin(), formatted.size()};
	LLC_TEST_CHECKF(errors, JSON_EXPRESSION_HISTORICAL_TEST_RESULT_FORMAT_VALUE, formattedView != expected, "Output:'%.*s', expected:'%.*s'.", LLC_VIEW_ARGS(formattedView), LLC_VIEW_ARGS(expected));
	rtrn 0;
}

::llc::err_t testJSONExpression(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	if_fail_fe(testJSONExpressionHistorical(errors, outputFolder));
	cnst ::llc::vcst_t document = "{\"application\":{\"blitter\":{\"database\":{\"name\":\"main\",\"enabled\":true,\"disabled\":false,\"negativeZero\":-0.0,\"path\":{\"database\":\"data\"},\"bind\":[\"id\",\"name\"]}}},\"selector\":\"selectorName\",\"selectorName\":\"name\"}";
	if_fail_fe(::llc::fileFromMemory(outputFolder, "focused_input.json", document.cu8()));
	always_printf("FIXTURE: focused_input.json (exact parser input)\n%.*s", LLC_VIEW_ARGS(document));
	::llc::SJSONReader reader = {};
	cnst ::llc::err_t parseResult = ::llc::jsonParse(reader, document);
	LLC_TEST_REQUIREF(errors, JSON_EXPRESSION_TEST_RESULT_PARSE, ::llc::failed(parseResult), "Result:%i, document:'%.*s'.", parseResult, LLC_VIEW_ARGS(document));

	::llc::err_t indexDatabase = -1;
	if_fail_fe(testJSONExpressionResolve(errors, reader, 0, "application.blitter.database", "{\"name\":\"main\",\"enabled\":true,\"disabled\":false,\"negativeZero\":-0.0,\"path\":{\"database\":\"data\"},\"bind\":[\"id\",\"name\"]}", JSON_EXPRESSION_TEST_RESULT_DIRECT_RESULT, JSON_EXPRESSION_TEST_RESULT_DIRECT_VALUE, indexDatabase));
	LLC_TEST_REQUIREF(errors, JSON_EXPRESSION_TEST_RESULT_DIRECT_INDEX, indexDatabase < 0, "Database index:%i.", indexDatabase);

	::llc::err_t result = -1;
	if_fail_fe(testJSONExpressionResolve(errors, reader, (::llc::u2_t)indexDatabase, "path.database", "data", JSON_EXPRESSION_TEST_RESULT_NESTED_RESULT, JSON_EXPRESSION_TEST_RESULT_NESTED_VALUE, result));
	if_fail_fe(testJSONExpressionResolve(errors, reader, 0, "application.blitter.database.bind['1']", "name", JSON_EXPRESSION_TEST_RESULT_ARRAY_RESULT, JSON_EXPRESSION_TEST_RESULT_ARRAY_VALUE, result));
	if_fail_fe(testJSONExpressionResolve(errors, reader, 0, "application.blitter.database.name['2']", "i", JSON_EXPRESSION_TEST_RESULT_STRING_RESULT, JSON_EXPRESSION_TEST_RESULT_STRING_VALUE, result));
	if_fail_fe(testJSONExpressionResolve(errors, reader, 0, "application.blitter.database.{selector}", "main", JSON_EXPRESSION_TEST_RESULT_DYNAMIC_RESULT, JSON_EXPRESSION_TEST_RESULT_DYNAMIC_VALUE, result));
	if_fail_fe(testJSONExpressionResolve(errors, reader, 0, "application.blitter.database.name == 'main'", "true", JSON_EXPRESSION_TEST_RESULT_EQUAL_RESULT, JSON_EXPRESSION_TEST_RESULT_EQUAL_VALUE, result));
	if_fail_fe(testJSONExpressionResolve(errors, reader, 0, "!application.blitter.database.disabled", "true", JSON_EXPRESSION_TEST_RESULT_NOT_RESULT, JSON_EXPRESSION_TEST_RESULT_NOT_VALUE, result));
	if_fail_fe(testJSONExpressionResolve(errors, reader, 0, "!application.blitter.database.negativeZero", "true", JSON_EXPRESSION_TEST_RESULT_DECIMAL_ZERO_RESULT, JSON_EXPRESSION_TEST_RESULT_DECIMAL_ZERO_VALUE, result));
	if_fail_fe(testJSONExpressionResolve(errors, reader, 0, "application.blitter.database.enabled && application.blitter.database.name", "true", JSON_EXPRESSION_TEST_RESULT_AND_RESULT, JSON_EXPRESSION_TEST_RESULT_AND_VALUE, result));
	if_fail_fe(testJSONExpressionResolve(errors, reader, 0, "application.blitter.database.disabled || application.blitter.database.name", "true", JSON_EXPRESSION_TEST_RESULT_OR_RESULT, JSON_EXPRESSION_TEST_RESULT_OR_VALUE, result));
	if_fail_fe(testJSONExpressionResolve(errors, reader, 0, "application.blitter.database.enabled ? 'yes' : 'no'", "yes", JSON_EXPRESSION_TEST_RESULT_TERNARY_RESULT, JSON_EXPRESSION_TEST_RESULT_TERNARY_VALUE, result));

	::llc::asc_t formatted = {};
	cnst ::llc::vcst_t format = "db:{application.blitter.database.name};first:{application.blitter.database.bind['0']}";
	cnst ::llc::err_t formatResult = ::llc::jsonStringFormat(format, reader, 0, formatted);
	::testJSONExpressionTrace(reader, 0, format, "db:main;first:id", {formatted.begin(), formatted.size()}, formatResult, false, true);
	LLC_TEST_REQUIREF(errors, JSON_EXPRESSION_TEST_RESULT_FORMAT_RESULT, ::llc::failed(formatResult), "Result:%i.", formatResult);
	cnst ::llc::vcst_t formattedView = {formatted.begin(), formatted.size()};
	LLC_TEST_CHECKF(errors, JSON_EXPRESSION_TEST_RESULT_FORMAT_VALUE, formattedView != "db:main;first:id", "Output:'%.*s'.", LLC_VIEW_ARGS(formattedView));

	::llc::asc_t adjacent = {};
	cnst ::llc::vcst_t adjacentFormat = "{application.blitter.database.name}{application.blitter.database.bind['1']}";
	cnst ::llc::err_t adjacentResult = ::llc::jsonStringFormat(adjacentFormat, reader, 0, adjacent);
	::testJSONExpressionTrace(reader, 0, adjacentFormat, "mainname", {adjacent.begin(), adjacent.size()}, adjacentResult, false, true);
	LLC_TEST_REQUIREF(errors, JSON_EXPRESSION_TEST_RESULT_FORMAT_ADJACENT_RESULT, ::llc::failed(adjacentResult), "Result:%i.", adjacentResult);
	cnst ::llc::vcst_t adjacentView = {adjacent.begin(), adjacent.size()};
	LLC_TEST_CHECKF(errors, JSON_EXPRESSION_TEST_RESULT_FORMAT_ADJACENT_VALUE, adjacentView != "mainname", "Output:'%.*s'.", LLC_VIEW_ARGS(adjacentView));

	::llc::vcst_t preserved = "preserved";
	cnst ::llc::err_t missingResult = ::llc::jsonExpressionResolve("application.blitter.missing.name", reader, 0, preserved);
	::testJSONExpressionTrace(reader, 0, "application.blitter.missing.name", "preserved", preserved, missingResult, true);
	LLC_TEST_CHECKF(errors, JSON_EXPRESSION_TEST_RESULT_MISSING_RESULT, 0 <= missingResult, "Result:%i.", missingResult);
	LLC_TEST_CHECKF(errors, JSON_EXPRESSION_TEST_RESULT_MISSING_OUTPUT, preserved != "preserved", "Output:'%.*s'.", LLC_VIEW_ARGS(preserved));

	preserved = "preserved";
	cnst ::llc::err_t rootRangeResult = ::llc::jsonExpressionResolve("application", reader, reader.Tree.size(), preserved);
	::testJSONExpressionTrace(reader, reader.Tree.size(), "application", "preserved", preserved, rootRangeResult, true);
	LLC_TEST_CHECKF(errors, JSON_EXPRESSION_TEST_RESULT_ROOT_RANGE_RESULT, 0 <= rootRangeResult, "Result:%i, root:%u, nodes:%u.", rootRangeResult, reader.Tree.size(), reader.Tree.size());
	LLC_TEST_CHECKF(errors, JSON_EXPRESSION_TEST_RESULT_ROOT_RANGE_OUTPUT, preserved != "preserved", "Output:'%.*s'.", LLC_VIEW_ARGS(preserved));

	preserved = "preserved";
	cnst ::llc::err_t arrayRangeResult = ::llc::jsonExpressionResolve("application.blitter.database.bind['9']", reader, 0, preserved);
	::testJSONExpressionTrace(reader, 0, "application.blitter.database.bind['9']", "preserved", preserved, arrayRangeResult, true);
	LLC_TEST_CHECKF(errors, JSON_EXPRESSION_TEST_RESULT_ARRAY_RANGE_RESULT, 0 <= arrayRangeResult, "Result:%i.", arrayRangeResult);
	LLC_TEST_CHECKF(errors, JSON_EXPRESSION_TEST_RESULT_ARRAY_RANGE_OUTPUT, preserved != "preserved", "Output:'%.*s'.", LLC_VIEW_ARGS(preserved));

	preserved = "preserved";
	cnst ::llc::err_t stringRangeResult = ::llc::jsonExpressionResolve("application.blitter.database.name['9']", reader, 0, preserved);
	::testJSONExpressionTrace(reader, 0, "application.blitter.database.name['9']", "preserved", preserved, stringRangeResult, true);
	LLC_TEST_CHECKF(errors, JSON_EXPRESSION_TEST_RESULT_STRING_RANGE_RESULT, 0 <= stringRangeResult, "Result:%i.", stringRangeResult);
	LLC_TEST_CHECKF(errors, JSON_EXPRESSION_TEST_RESULT_STRING_RANGE_OUTPUT, preserved != "preserved", "Output:'%.*s'.", LLC_VIEW_ARGS(preserved));

	::llc::SExpressionReader expressionReader = {};
	if_fail_fe(::llc::expressionReaderParse(expressionReader, "application.blitter.missing.name"));
	::llc::SJSONExpressionSolver solver = {};
	::llc::SEvaluationStepResult priorStep = {};
	priorStep.Output = "prior";
	if_fail_fe(solver.Results.push_back(priorStep));
	cnst ::llc::u2_t resultCountBefore = solver.Results.size();
	preserved = "preserved";
	cnst ::llc::err_t solverResult = ::llc::jsonExpressionResolve(expressionReader, reader, 0, solver, preserved);
	::testJSONExpressionTrace(reader, 0, "application.blitter.missing.name", "preserved", preserved, solverResult, true);
	always_printf("Solver rollback: expected %u retained steps; actual %u. %s.", resultCountBefore, solver.Results.size(), solver.Results.size() == resultCountBefore ? "PASS" : "FAIL");
	LLC_TEST_CHECKF(errors, JSON_EXPRESSION_TEST_RESULT_SOLVER_RESULT, 0 <= solverResult, "Result:%i.", solverResult);
	LLC_TEST_CHECKF(errors, JSON_EXPRESSION_TEST_RESULT_SOLVER_OUTPUT, preserved != "preserved", "Output:'%.*s'.", LLC_VIEW_ARGS(preserved));
	LLC_TEST_CHECKF(errors, JSON_EXPRESSION_TEST_RESULT_SOLVER_STATE, solver.Results.size() != resultCountBefore, "Steps:%u, expected:%u.", solver.Results.size(), resultCountBefore);

	::llc::asc_t failedFormat = {};
	if_fail_fe(failedFormat.append_string("preserved"));
	cnst ::llc::vcst_t failedTemplate = "prefix:{application.blitter.missing.name}";
	cnst ::llc::err_t failedFormatResult = ::llc::jsonStringFormat(failedTemplate, reader, 0, failedFormat);
	::testJSONExpressionTrace(reader, 0, failedTemplate, "preserved", {failedFormat.begin(), failedFormat.size()}, failedFormatResult, true, true);
	LLC_TEST_CHECKF(errors, JSON_EXPRESSION_TEST_RESULT_FORMAT_FAILURE_RESULT, 0 <= failedFormatResult, "Result:%i.", failedFormatResult);
	cnst ::llc::vcst_t failedFormatView = {failedFormat.begin(), failedFormat.size()};
	LLC_TEST_CHECKF(errors, JSON_EXPRESSION_TEST_RESULT_FORMAT_FAILURE_OUTPUT, failedFormatView != "preserved", "Output:'%.*s'.", LLC_VIEW_ARGS(failedFormatView));
	rtrn 0;
}
