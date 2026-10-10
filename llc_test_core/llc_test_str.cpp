#include "llc_array_pod.h"
#include "llc_array_static.h"
#include "llc_stdstring.h"
#include "llc_string.h"

#include "llc_test_core.h"

#include <type_traits>

GDEFINE_ENUM_TYPE(STR_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, OK						, 0, "All str() adapter tests passed.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, MUTABLE_ARRAY_TYPE		, 1, "str() did not preserve mutable character-array access.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CONST_ARRAY_TYPE			, 2, "str() did not preserve const character-array access.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, ARRAY_RANGE				, 3, "str() did not expose the character-array text range.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, ARRAY_MUTATION			, 4, "A mutable character-array string view did not update its source.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, MUTABLE_VIEW_TYPE			, 5, "str() did not adapt a mutable character view to view_string.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CONST_VIEW_TYPE			, 6, "str() did not adapt a const character view to view_const_string.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, COUNTED_VIEW_RANGE		, 7, "str() did not preserve an explicitly counted character range.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, STRING_VIEW_TYPE			, 8, "str() did not preserve string-view mutability.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, STRING_VIEW_RANGE			, 9, "str() changed an existing string-view range.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, MUTABLE_STATIC_TYPE		, 10, "str() did not adapt mutable static character storage to view_string.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CONST_STATIC_TYPE		, 11, "str() did not adapt const static character storage to view_const_string.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, STATIC_RANGE				, 12, "str() did not expose the text stored in a static character array.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, MUTABLE_POD_TYPE			, 13, "str() did not adapt mutable POD character storage to view_string.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CONST_POD_TYPE			, 14, "str() did not adapt const POD character storage to view_const_string.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, POD_RANGE				, 15, "str() changed the counted POD character range.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, POD_MUTATION				, 16, "A mutable POD string view did not update its source.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, EMPTY_STRING				, 17, "str() did not produce a safe readable empty string.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, BOOL_TEXT					, 18, "str() did not expose the expected boolean text.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, NUMERIC_RESULT_TYPE		, 19, "Numeric str() returned an unexpected storage type.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, NUMERIC_TEXT				, 20, "Numeric str() produced unexpected text.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, NUMERIC_TERMINATION		, 21, "Numeric str() did not terminate its text within its static storage.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CHARACTER_UPPERCASE		, 22, "toupper(char&) did not mutate only lowercase ASCII characters.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CHARACTER_LOWERCASE		, 23, "tolower(char&) did not mutate only uppercase ASCII characters.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, VIEW_UPPERCASE			, 24, "toupper(view<char>) did not transform the complete counted range.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, VIEW_LOWERCASE			, 25, "tolower(view<char>) did not transform the complete counted range.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, EMPTY_CASE_VIEW			, 26, "Character-case transformation rejected an empty view.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, STRING_TO_UINT			, 27, "stoull() did not parse a counted decimal string.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CAMEL_CASE				, 28, "camelCase() produced unexpected text.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, NUMERIC_RANGE			, 29, "Numeric str() returned a view of the wrong storage.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, NUMERIC_CAPACITY		, 30, "Numeric str() filled its storage without room for a terminator.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, EMPTY_LOWER_CASE_VIEW	, 31, "tolower() rejected an empty view.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, STRING_TO_UINT_VALUE	, 32, "stoull() returned the wrong parsed value.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CAMEL_CASE_RESULT		, 33, "camelCase() failed on a valid input.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CONST_ARRAY_RANGE		, 34, "str() did not expose the const character-array range.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, MUTABLE_ARRAY_BEGIN	, 35, "str() changed the mutable array's starting address.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CONST_ARRAY_BEGIN		, 36, "str() changed the const array's starting address.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, COUNTED_VIEW_BEGIN	, 37, "str() changed a counted mutable view's starting address.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CONST_COUNTED_VIEW_SIZE, 38, "str() changed a counted const view's size.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CONST_COUNTED_VIEW_BEGIN, 39, "str() changed a counted const view's starting address.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, EMPTY_DYNAMIC_POINTER, 40, "str() returned a null pointer for empty POD storage.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, EMPTY_DYNAMIC_TERMINATOR, 41, "str() did not terminate empty POD storage.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, EMPTY_ARRAY_SIZE	, 42, "str() returned a nonempty view of an empty character array.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, EMPTY_ARRAY_POINTER	, 43, "str() returned a null pointer for an empty character array.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, EMPTY_ARRAY_TERMINATOR, 44, "str() did not terminate an empty character array.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, FALSE_TEXT			, 45, "str(false) returned unexpected text.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CONST_MUTABLE_STRING_TYPE, 46, "str() did not preserve const access to a mutable string view.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CONST_STRING_TYPE		, 47, "str() did not preserve const string-view access.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, MUTABLE_STRING_SIZE	, 48, "str() changed a mutable string-view size.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CONST_MUTABLE_STRING_BEGIN, 49, "str() changed a const mutable string-view start.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CONST_MUTABLE_STRING_SIZE, 50, "str() changed a const mutable string-view size.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CONST_STRING_BEGIN	, 51, "str() changed a const string-view start.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CONST_STRING_SIZE	, 52, "str() changed a const string-view size.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, MUTABLE_STATIC_BEGIN	, 53, "str() changed a mutable static array's starting address.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CONST_STATIC_TEXT	, 54, "str() changed const static array text.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CONST_STATIC_BEGIN	, 55, "str() changed a const static array's starting address.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, MUTABLE_POD_BEGIN	, 56, "str() changed mutable POD storage's starting address.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CONST_POD_TEXT		, 57, "str() changed const POD text.");
GDEFINE_ENUM_VALUED(STR_TEST_RESULT, CONST_POD_BEGIN		, 58, "str() changed const POD storage's starting address.");

tplt<tpnm TString>
sttc bool stringMismatch(cnst TString & value, ::llc::vcst_t expected) {
	rtrn value.size() != expected.size() || (value.size() && 0 != memcmp(value.begin(), expected.begin(), value.size()));
}

tplt<tpnm T, ::llc::u2_t N>
sttc ::llc::err_t testNumericStrValue(ATestError & errors, T value, ::llc::vcst_t expected) {
	auto				storage	= ::llc::str(value);
	auto				text	= ::llc::str(storage);
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_NUMERIC_RESULT_TYPE, false == (::std::is_same_v<decltype(storage), ::llc::astchar<N>>) , "result type mismatch. capacity:%u, expected:%u." , storage.size(), N );
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_NUMERIC_TEXT, stringMismatch(text, expected) , "actual:'%.*s'/%u, expected:'%.*s'/%u." , (int)text.size(), text.begin(), text.size(), (int)expected.size(), expected.begin(), expected.size());
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_NUMERIC_RANGE, text.begin() != storage.begin() , "numeric text begin:%p, expected:%p.", text.begin(), storage.begin());
	LLC_TEST_REQUIREF(errors, STR_TEST_RESULT_NUMERIC_CAPACITY, text.size() >= storage.size() , "numeric text size:%u, capacity:%u.", text.size(), storage.size());
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_NUMERIC_TERMINATION, storage.Storage[text.size()] , "numeric terminator:%i, expected:0.", storage.Storage[text.size()]);
	rtrn 0;
}

sttc ::llc::err_t testNumericStr(ATestError & errors) {
	if_fail_fe((testNumericStrValue<::llc::u0_t,   5>(errors,   0, LLC_CXS("0"))));
	if_fail_fe((testNumericStrValue<::llc::u0_t,   5>(errors,   9, LLC_CXS("9"))));
	if_fail_fe((testNumericStrValue<::llc::u0_t,   5>(errors,  10, LLC_CXS("10"))));
	if_fail_fe((testNumericStrValue<::llc::u0_t,   5>(errors,  99, LLC_CXS("99"))));
	if_fail_fe((testNumericStrValue<::llc::u0_t,   5>(errors, 100, LLC_CXS("100"))));
	if_fail_fe((testNumericStrValue<::llc::u0_t,   5>(errors, 255, LLC_CXS("255"))));

	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors,    0, LLC_CXS("0"))));
	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors,    1, LLC_CXS("1"))));
	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors,   -1, LLC_CXS("-1"))));
	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors,    9, LLC_CXS("9"))));
	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors,   -9, LLC_CXS("-9"))));
	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors,   10, LLC_CXS("10"))));
	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors,  -10, LLC_CXS("-10"))));
	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors,   99, LLC_CXS("99"))));
	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors,  -99, LLC_CXS("-99"))));
	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors,  100, LLC_CXS("100"))));
	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors, -100, LLC_CXS("-100"))));
	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors,  127, LLC_CXS("127"))));
	if_fail_fe((testNumericStrValue<::llc::s0_t,   5>(errors, -128, LLC_CXS("-128"))));

	if_fail_fe((testNumericStrValue<::llc::u1_t,   7>(errors, ::llc::u1_t(    0), LLC_CXS("0"))));
	if_fail_fe((testNumericStrValue<::llc::u1_t,   7>(errors, ::llc::u1_t(   10), LLC_CXS("10"))));
	if_fail_fe((testNumericStrValue<::llc::u1_t,   7>(errors, ::llc::u1_t(65535), LLC_CXS("65535"))));
	if_fail_fe((testNumericStrValue<::llc::s1_t,   8>(errors, ::llc::s1_t(     0), LLC_CXS("0"))));
	if_fail_fe((testNumericStrValue<::llc::s1_t,   8>(errors, ::llc::s1_t(    -1), LLC_CXS("-1"))));
	if_fail_fe((testNumericStrValue<::llc::s1_t,   8>(errors, ::llc::s1_t( 32767), LLC_CXS("32767"))));
	if_fail_fe((testNumericStrValue<::llc::s1_t,   8>(errors, ::llc::s1_t(-32768), LLC_CXS("-32768"))));

	if_fail_fe((testNumericStrValue<::llc::u2_t,  12>(errors, ::llc::u2_t(         0U), LLC_CXS("0"))));
	if_fail_fe((testNumericStrValue<::llc::u2_t,  12>(errors, ::llc::u2_t(        10U), LLC_CXS("10"))));
	if_fail_fe((testNumericStrValue<::llc::u2_t,  12>(errors, ::llc::u2_t(4294967295U), LLC_CXS("4294967295"))));
	if_fail_fe((testNumericStrValue<::llc::s2_t,  13>(errors, ::llc::s2_t(          0), LLC_CXS("0"))));
	if_fail_fe((testNumericStrValue<::llc::s2_t,  13>(errors, ::llc::s2_t(         -1), LLC_CXS("-1"))));
	if_fail_fe((testNumericStrValue<::llc::s2_t,  13>(errors, ::llc::s2_t( 2147483647), LLC_CXS("2147483647"))));
	if_fail_fe((testNumericStrValue<::llc::s2_t,  13>(errors, ::llc::s2_t(-2147483647 - 1), LLC_CXS("-2147483648"))));

	if_fail_fe((testNumericStrValue<::llc::u3_t,  22>(errors, ::llc::u3_t(                   0ULL), LLC_CXS("0"))));
	if_fail_fe((testNumericStrValue<::llc::u3_t,  22>(errors, ::llc::u3_t(                  10ULL), LLC_CXS("10"))));
	if_fail_fe((testNumericStrValue<::llc::u3_t,  22>(errors, ::llc::u3_t(18446744073709551615ULL), LLC_CXS("18446744073709551615"))));
	if_fail_fe((testNumericStrValue<::llc::s3_t,  22>(errors, ::llc::s3_t(                   0LL), LLC_CXS("0"))));
	if_fail_fe((testNumericStrValue<::llc::s3_t,  22>(errors, ::llc::s3_t(                  -1LL), LLC_CXS("-1"))));
	if_fail_fe((testNumericStrValue<::llc::s3_t,  22>(errors, ::llc::s3_t( 9223372036854775807LL), LLC_CXS("9223372036854775807"))));
	if_fail_fe((testNumericStrValue<::llc::s3_t,  22>(errors, ::llc::s3_t(-9223372036854775807LL - 1), LLC_CXS("-9223372036854775808"))));

	if_fail_fe((testNumericStrValue<::llc::f2_t,  64>(errors, ::llc::f2_t(       0.0), LLC_CXS("0.000000"))));
	if_fail_fe((testNumericStrValue<::llc::f2_t,  64>(errors, ::llc::f2_t(      1.25), LLC_CXS("1.250000"))));
	if_fail_fe((testNumericStrValue<::llc::f2_t,  64>(errors, ::llc::f2_t(      -2.5), LLC_CXS("-2.500000"))));
	if_fail_fe((testNumericStrValue<::llc::f2_t,  64>(errors, ::llc::f2_t(  123456.5), LLC_CXS("123456.500000"))));
	if_fail_fe((testNumericStrValue<::llc::f3_t, 384>(errors, ::llc::f3_t(         0.0), LLC_CXS("0.000000"))));
	if_fail_fe((testNumericStrValue<::llc::f3_t, 384>(errors, ::llc::f3_t(       1.125), LLC_CXS("1.125000"))));
	if_fail_fe((testNumericStrValue<::llc::f3_t, 384>(errors, ::llc::f3_t(       -2.25), LLC_CXS("-2.250000"))));
	if_fail_fe((testNumericStrValue<::llc::f3_t, 384>(errors, ::llc::f3_t(123456789.125), LLC_CXS("123456789.125000"))));
	rtrn 0;
}

sttc ::llc::err_t testCharacterCase(ATestError & errors) {
	::llc::sc_t		lowerCharacter		= 'a';
	::llc::sc_t		upperCharacter		= 'Z';
	::llc::sc_t		digitCharacter		= '7';
	::llc::toupper(lowerCharacter);
	::llc::toupper(upperCharacter);
	::llc::toupper(digitCharacter);
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_CHARACTER_UPPERCASE, 'A' != lowerCharacter , "toupper('a'):%c, expected:A.", lowerCharacter);
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_CHARACTER_UPPERCASE, 'Z' != upperCharacter , "toupper('Z'):%c, expected:Z.", upperCharacter);
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_CHARACTER_UPPERCASE, '7' != digitCharacter , "toupper('7'):%c, expected:7.", digitCharacter);

	lowerCharacter		= 'a';
	upperCharacter		= 'Z';
	digitCharacter		= '7';
	::llc::tolower(lowerCharacter);
	::llc::tolower(upperCharacter);
	::llc::tolower(digitCharacter);
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_CHARACTER_LOWERCASE, 'a' != lowerCharacter , "tolower('a'):%c, expected:a.", lowerCharacter);
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_CHARACTER_LOWERCASE, 'z' != upperCharacter , "tolower('Z'):%c, expected:z.", upperCharacter);
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_CHARACTER_LOWERCASE, '7' != digitCharacter , "tolower('7'):%c, expected:7.", digitCharacter);

	::llc::sc_t		upperStorage[]		= {'a', 'Z', 0, 'm', '-', '7'};
	cnst ::llc::err_t upperResult		= ::llc::toupper({upperStorage});
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_VIEW_UPPERCASE, upperResult , "Uppercase view result:%i.", upperResult);
	::llc::sc_c expectedUpper[] = {'A', 'Z', 0, 'M', '-', '7'};
	for(::llc::u2_t iCharacter = 0; iCharacter < ::llc::size(expectedUpper); ++iCharacter) {
		LLC_TEST_CHECKF(errors, STR_TEST_RESULT_VIEW_UPPERCASE, upperStorage[iCharacter] != expectedUpper[iCharacter] , "Uppercase view[%u]:%i, expected:%i." , iCharacter, upperStorage[iCharacter], expectedUpper[iCharacter]);
	}

	::llc::sc_t		lowerStorage[]		= {'A', 'z', 0, 'M', '_', '7'};
	cnst ::llc::err_t lowerResult		= ::llc::tolower({lowerStorage});
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_VIEW_LOWERCASE, lowerResult , "Lowercase view result:%i.", lowerResult);
	::llc::sc_c expectedLower[] = {'a', 'z', 0, 'm', '_', '7'};
	for(::llc::u2_t iCharacter = 0; iCharacter < ::llc::size(expectedLower); ++iCharacter) {
		LLC_TEST_CHECKF(errors, STR_TEST_RESULT_VIEW_LOWERCASE, lowerStorage[iCharacter] != expectedLower[iCharacter] , "Lowercase view[%u]:%i, expected:%i." , iCharacter, lowerStorage[iCharacter], expectedLower[iCharacter]);
	}

	::llc::view<::llc::sc_t> emptyView = {};
	cnst ::llc::err_t emptyUpperResult = ::llc::toupper(emptyView);
	cnst ::llc::err_t emptyLowerResult = ::llc::tolower(emptyView);
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_EMPTY_CASE_VIEW, emptyUpperResult , "Empty uppercase view result:%i.", emptyUpperResult);
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_EMPTY_LOWER_CASE_VIEW, emptyLowerResult , "Empty lowercase view result:%i.", emptyLowerResult);
	rtrn 0;
}

sttc ::llc::err_t testStringFunctions(ATestError & errors) {
	::llc::sc_c		digits[]			= {'4', '2', '9', 'x'};
	::llc::u3_t		parsedValue		= {};
	cnst ::llc::err_t parsedCount		= ::llc::stoull({digits}, parsedValue);
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_STRING_TO_UINT, 3 != parsedCount , "Parsed count:%i, expected:3.", parsedCount);
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_STRING_TO_UINT_VALUE, 429 != parsedValue , "Parsed value:%" LLC_FMT_U3 ", expected:429.", parsedValue);

	stct SCamelCase {
		::llc::vcst_t	Input		= {};
		::llc::vcst_t	Expected	= {};
	};
	cnst SCamelCase cases[] =
		{ {LLC_CXS("")                         , LLC_CXS("")}
		, {LLC_CXS("snake_case")               , LLC_CXS("SnakeCase")}
		, {LLC_CXS("kebab-case")               , LLC_CXS("KebabCase")}
		, {LLC_CXS("-multiple__separators-")   , LLC_CXS("MultipleSeparators")}
		, {LLC_CXS("alreadyCase")              , LLC_CXS("AlreadyCase")}
		, {LLC_CXS("UPPER_CASE")               , LLC_CXS("UPPERCASE")}
		};
	for(::llc::u2_t iCase = 0; iCase < ::llc::size(cases); ++iCase) {
		::llc::string actual = {};
		cnst ::llc::err_t result = ::llc::camelCase(cases[iCase].Input, actual);
		LLC_TEST_CHECKF(errors, STR_TEST_RESULT_CAMEL_CASE_RESULT, ::llc::failed(result) , "Case:%u, result:%i, input:'%.*s'." , iCase, result, (int)cases[iCase].Input.size(), cases[iCase].Input.begin());
		LLC_TEST_CHECKF(errors, STR_TEST_RESULT_CAMEL_CASE, stringMismatch(actual, cases[iCase].Expected) , "Case:%u, input:'%.*s', actual:'%.*s', expected:'%.*s'." , iCase , (int)cases[iCase].Input.size(), cases[iCase].Input.begin() , (int)actual.size(), actual.begin() , (int)cases[iCase].Expected.size(), cases[iCase].Expected.begin() );
	}
	rtrn 0;
}

::llc::err_t testStr(ATestError & errors) {
	::llc::sc_t		mutableArray[]		= "alpha";
	::llc::sc_c		constArray[]		= "beta";
	auto				mutableArrayText	= ::llc::str(mutableArray);
	auto				constArrayText		= ::llc::str(constArray);
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_MUTABLE_ARRAY_TYPE, false == (::std::is_same_v<decltype(mutableArrayText), ::llc::vs>) , "Mutable array result type mismatch. size:%u." , mutableArrayText.size() );
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_CONST_ARRAY_TYPE, false == (::std::is_same_v<decltype(constArrayText), ::llc::vcst_t>) , "Const array result type mismatch. size:%u." , constArrayText.size() );
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_ARRAY_RANGE, stringMismatch(mutableArrayText, LLC_CXS("alpha")) , "Mutable array text:'%.*s', expected:'alpha'.", (int)mutableArrayText.size(), mutableArrayText.begin());
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_MUTABLE_ARRAY_BEGIN, mutableArrayText.begin() != mutableArray , "Mutable array begin:%p, expected:%p.", mutableArrayText.begin(), mutableArray);
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_CONST_ARRAY_RANGE, stringMismatch(constArrayText, LLC_CXS("beta")) , "Const array text:'%.*s', expected:'beta'.", (int)constArrayText.size(), constArrayText.begin());
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_CONST_ARRAY_BEGIN, constArrayText.begin() != constArray , "Const array begin:%p, expected:%p.", constArrayText.begin(), constArray);
	mutableArrayText[0] = 'A';
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_ARRAY_MUTATION, mutableArray[0] != 'A' , "Mutable array source was not updated. source[0]:%c, expected:A." , mutableArray[0] );

	::llc::sc_t		countedStorage[]		= {'c', 'o', 0, 'u', 'n', 't'};
	::llc::vsc_t	countedView				= {countedStorage, ::llc::size(countedStorage)};
	::llc::vcsc_t	countedConstView		= {countedStorage, ::llc::size(countedStorage)};
	auto			countedText				= ::llc::str(countedView);
	auto			countedConstText		= ::llc::str(countedConstView);
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_MUTABLE_VIEW_TYPE, false == (::std::is_same_v<decltype(countedText), ::llc::vs>), "Mutable view result type mismatch. size:%u.", countedText.size());
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_CONST_VIEW_TYPE, false == (::std::is_same_v<decltype(countedConstText), ::llc::vcst_t>), "Const view result type mismatch. size:%u.", countedConstText.size());
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_COUNTED_VIEW_RANGE, countedText.size() != ::llc::size(countedStorage)			, "Counted mutable size:%u, expected:%u.", countedText.size(), (::llc::u2_t)::llc::size(countedStorage));
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_COUNTED_VIEW_BEGIN, countedText.begin() != countedStorage						, "Counted mutable begin:%p, expected:%p.", countedText.begin(), countedStorage);
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_CONST_COUNTED_VIEW_SIZE, countedConstText.size() != ::llc::size(countedStorage)	, "Counted const size:%u, expected:%u.", countedConstText.size(), (::llc::u2_t)::llc::size(countedStorage));
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_CONST_COUNTED_VIEW_BEGIN, countedConstText.begin() != countedStorage				, "Counted const begin:%p, expected:%p.", countedConstText.begin(), countedStorage);
	countedText[3] = 'U';
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_ARRAY_MUTATION, countedStorage[3] != 'U', "Counted mutable source was not updated. source[3]:%c, expected:U.", countedStorage[3]);

	::llc::vs		mutableString			= mutableArray;
	cnst ::llc::vs	& constMutableString	= mutableString;
	::llc::vcst_t	constString				= constArray;
	auto			mutableStringText		= ::llc::str(mutableString);
	auto			constMutableText		= ::llc::str(constMutableString);
	auto			constStringText			= ::llc::str(constString);
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_STRING_VIEW_TYPE				, false == (::std::is_same_v<decltype(mutableStringText	), ::llc::vs	>));
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_CONST_MUTABLE_STRING_TYPE	, false == (::std::is_same_v<decltype(constMutableText	), ::llc::vcst_t>));
	LLC_TEST_CHECK(errors, STR_TEST_RESULT_CONST_STRING_TYPE			, false == (::std::is_same_v<decltype(constStringText	), ::llc::vcst_t>));
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_STRING_VIEW_RANGE			, mutableStringText .begin() != mutableString.begin	(), "Mutable string begin:%p, expected:%p."			, mutableStringText.begin(), mutableString.begin());
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_MUTABLE_STRING_SIZE			, mutableStringText .size () != mutableString.size	(), "Mutable string size:%u, expected:%u."			, mutableStringText.size (), mutableString.size ());
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_CONST_MUTABLE_STRING_BEGIN	, constMutableText  .begin() != mutableString.begin	(), "Const mutable string begin:%p, expected:%p."	, constMutableText .begin(), mutableString.begin());
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_CONST_MUTABLE_STRING_SIZE	, constMutableText  .size () != mutableString.size	(), "Const mutable string size:%u, expected:%u."	, constMutableText .size (), mutableString.size ());
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_CONST_STRING_BEGIN			, constStringText   .begin() != constString  .begin	(), "Const string begin:%p, expected:%p."			, constStringText  .begin(), constString  .begin());
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_CONST_STRING_SIZE			, constStringText   .size () != constString  .size	(), "Const string size:%u, expected:%u."			, constStringText  .size (), constString  .size ());

	::llc::astchar<8>	staticText		= {'s', 't', 'a', 't', 'i', 'c', 0};
	cnst ::llc::astchar<8> & constStaticText	= staticText;
	auto				mutableStaticView	= ::llc::str(staticText);
	auto				constStaticView		= ::llc::str(constStaticText);
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_MUTABLE_STATIC_TYPE, false == (::std::is_same_v<decltype(mutableStaticView), ::llc::vs>) , "Mutable static result type mismatch. size:%u." , mutableStaticView.size() );
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_CONST_STATIC_TYPE, false == (::std::is_same_v<decltype(constStaticView), ::llc::vcst_t>) , "Const static result type mismatch. size:%u." , constStaticView.size() );
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_STATIC_RANGE, stringMismatch(mutableStaticView, LLC_CXS("static")) , "Mutable static text:'%.*s'.", (int)mutableStaticView.size(), mutableStaticView.begin());
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_MUTABLE_STATIC_BEGIN, mutableStaticView.begin() != staticText.begin() , "Mutable static begin:%p, expected:%p.", mutableStaticView.begin(), staticText.begin());
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_CONST_STATIC_TEXT, stringMismatch(constStaticView, LLC_CXS("static")) , "Const static text:'%.*s'.", (int)constStaticView.size(), constStaticView.begin());
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_CONST_STATIC_BEGIN, constStaticView.begin() != staticText.begin() , "Const static begin:%p, expected:%p.", constStaticView.begin(), staticText.begin());

	::llc::asc_t	dynamicText		= {'p', 'o', 'd'};
	cnst ::llc::asc_t & constDynamicText	= dynamicText;
	auto				mutableDynamicView	= ::llc::str(dynamicText);
	auto				constDynamicView	= ::llc::str(constDynamicText);
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_MUTABLE_POD_TYPE, false == (::std::is_same_v<decltype(mutableDynamicView), ::llc::vs>) , "Mutable POD result type mismatch. size:%u." , mutableDynamicView.size() );
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_CONST_POD_TYPE, false == (::std::is_same_v<decltype(constDynamicView), ::llc::vcst_t>) , "Const POD result type mismatch. size:%u." , constDynamicView.size() );
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_POD_RANGE, stringMismatch(mutableDynamicView, LLC_CXS("pod")) , "Mutable POD text:'%.*s'.", (int)mutableDynamicView.size(), mutableDynamicView.begin());
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_MUTABLE_POD_BEGIN, mutableDynamicView.begin() != dynamicText.begin() , "Mutable POD begin:%p, expected:%p.", mutableDynamicView.begin(), dynamicText.begin());
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_CONST_POD_TEXT, stringMismatch(constDynamicView, LLC_CXS("pod")) , "Const POD text:'%.*s'.", (int)constDynamicView.size(), constDynamicView.begin());
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_CONST_POD_BEGIN, constDynamicView.begin() != dynamicText.begin() , "Const POD begin:%p, expected:%p.", constDynamicView.begin(), dynamicText.begin());
	mutableDynamicView[0] = 'P';
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_POD_MUTATION, dynamicText[0] != 'P' , "Mutable POD source was not updated. source[0]:%c, expected:P." , dynamicText[0] );

	cnst ::llc::asc_t	emptyDynamic;
	::llc::sc_t			emptyArray[] = "";
	auto					emptyDynamicText	= ::llc::str(emptyDynamic);
	auto					emptyArrayText		= ::llc::str(emptyArray);
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_EMPTY_STRING, emptyDynamicText.size() , "Empty POD string size:%u.", emptyDynamicText.size());
	LLC_TEST_REQUIREF(errors, STR_TEST_RESULT_EMPTY_DYNAMIC_POINTER, 0 == emptyDynamicText.begin() , "Empty POD string begin:%p.", emptyDynamicText.begin());
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_EMPTY_DYNAMIC_TERMINATOR, emptyDynamicText.begin()[0] , "Empty POD string terminator:%i.", emptyDynamicText.begin()[0]);
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_EMPTY_ARRAY_SIZE, emptyArrayText.size() , "Empty array string size:%u.", emptyArrayText.size());
	LLC_TEST_REQUIREF(errors, STR_TEST_RESULT_EMPTY_ARRAY_POINTER, 0 == emptyArrayText.begin() , "Empty array string begin:%p.", emptyArrayText.begin());
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_EMPTY_ARRAY_TERMINATOR, emptyArrayText.begin()[0] , "Empty array string terminator:%i.", emptyArrayText.begin()[0]);

	auto trueText	= ::llc::str(true);
	auto falseText	= ::llc::str(false);
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_BOOL_TEXT, stringMismatch(trueText, LLC_CXS("true")) , "True text:'%.*s'.", (int)trueText.size(), trueText.begin());
	LLC_TEST_CHECKF(errors, STR_TEST_RESULT_FALSE_TEXT, stringMismatch(falseText, LLC_CXS("false")) , "False text:'%.*s'.", (int)falseText.size(), falseText.begin());
	if_fail_fe(testNumericStr(errors));
	if_fail_fe(testCharacterCase(errors));
	if_fail_fe(testStringFunctions(errors));
	rtrn 0;
}
