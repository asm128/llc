#include "llc_axis.h"
#include "llc_wifi.h"

#include "llc_test_core.h"

enum ENUM_ALIAS_SAMPLE : ::llc::u0_t {};

GDEFINE_ENUM_TYPE(ENUM_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(ENUM_TEST_RESULT, PRIMARY_REGISTER, 0, "The primary enum name could not be registered.");
GDEFINE_ENUM_VALUED(ENUM_TEST_RESULT, ALIAS_REGISTER	, 1, "The alias could not be registered for an existing value.");
GDEFINE_ENUM_VALUED(ENUM_TEST_RESULT, PRIMARY_LOOKUP	, 2, "The primary enum name did not resolve to its value.");
GDEFINE_ENUM_VALUED(ENUM_TEST_RESULT, ALIAS_LOOKUP	, 3, "The alias did not resolve to the primary value.");
GDEFINE_ENUM_VALUED(ENUM_TEST_RESULT, ALIAS_INDEX	, 4, "The alias was not retained as a separate named entry.");
GDEFINE_ENUM_VALUED(ENUM_TEST_RESULT, ALIAS_COUNT	, 5, "The registry did not retain both names for one value.");
GDEFINE_ENUM_VALUED(ENUM_TEST_RESULT, CANONICAL_NAME	, 6, "Value-to-name lookup did not retain the first registered name.");
GDEFINE_ENUM_VALUED(ENUM_TEST_RESULT, REPEAT_RESULT	, 7, "Repeating an identical enum name was not rejected.");
GDEFINE_ENUM_VALUED(ENUM_TEST_RESULT, REPEAT_COUNT	, 8, "Repeating an identical enum name added another entry.");
GDEFINE_ENUM_VALUED(ENUM_TEST_RESULT, CONFLICT_RESULT	, 9, "Registering an existing name with a different value was not rejected.");
GDEFINE_ENUM_VALUED(ENUM_TEST_RESULT, CONFLICT_COUNT	, 10, "A conflicting enum name added another entry.");
GDEFINE_ENUM_VALUED(ENUM_TEST_RESULT, CONFLICT_VALUE	, 11, "A conflicting enum name changed the original value.");
GDEFINE_ENUM_VALUED(ENUM_TEST_RESULT, AUTO_INDEX	, 12, "An automatic enum value did not follow the alias entry.");
GDEFINE_ENUM_VALUED(ENUM_TEST_RESULT, AUTO_VALUE	, 13, "An alias changed the next automatically assigned enum value.");
GDEFINE_ENUM_VALUED(ENUM_TEST_RESULT, ALIGN_X_ALIAS	, 14, "ALIGN_XCENTER did not resolve to ALIGN_HCENTER.");
GDEFINE_ENUM_VALUED(ENUM_TEST_RESULT, ALIGN_Y_ALIAS	, 15, "ALIGN_YCENTER did not resolve to ALIGN_VCENTER.");
GDEFINE_ENUM_VALUED(ENUM_TEST_RESULT, ALIGN_Z_ALIAS	, 16, "ALIGN_ZCENTER did not resolve to ALIGN_DCENTER.");
GDEFINE_ENUM_VALUED(ENUM_TEST_RESULT, WIFI_AUTH_ALIAS	, 17, "WIFI_AUTH_WPA2_ENTERPRISE did not resolve to WIFI_AUTH_ENTERPRISE.");

::llc::err_t testEnum(ATestError & errors) {
	cnst ENUM_ALIAS_SAMPLE sampleValue = (ENUM_ALIAS_SAMPLE)7;
	::llc::enum_definition<ENUM_ALIAS_SAMPLE> definition = {};
	definition.Name = LLC_CXS("ENUM_ALIAS_SAMPLE");
	cnst ::llc::err_t primaryIndex = definition.add_value(sampleValue, LLC_CXS("PRIMARY"), LLC_CXS("PRIMARY"), LLC_CXS("PRIMARY"));
	LLC_TEST_REQUIREF(errors, ENUM_TEST_RESULT_PRIMARY_REGISTER, primaryIndex != 0, "PRIMARY index:%i, expected:0.", primaryIndex);
	cnst ::llc::err_t aliasIndex = definition.add_value(sampleValue, LLC_CXS("ALIAS"), LLC_CXS("ALIAS"), LLC_CXS("Alternative name."));
	LLC_TEST_REQUIREF(errors, ENUM_TEST_RESULT_ALIAS_REGISTER, aliasIndex != 1, "ALIAS index:%i, expected:1.", aliasIndex);
	LLC_TEST_CHECK(errors, ENUM_TEST_RESULT_PRIMARY_LOOKUP , definition.get_value(LLC_CXS("PRIMARY")) != sampleValue);
	LLC_TEST_CHECK(errors, ENUM_TEST_RESULT_ALIAS_LOOKUP , definition.get_value(LLC_CXS("ALIAS")) != sampleValue);
	LLC_TEST_CHECK(errors, ENUM_TEST_RESULT_ALIAS_INDEX , definition.get_value_index(LLC_CXS("ALIAS")) != 1);
	LLC_TEST_CHECKF(errors, ENUM_TEST_RESULT_ALIAS_COUNT , definition.Values.size() != 2, "Entry count:%u, expected:2.", definition.Values.size() );
	LLC_TEST_CHECK(errors, ENUM_TEST_RESULT_CANONICAL_NAME , definition.get_value_label(sampleValue) != LLC_CXS("PRIMARY"));

	cnst ::llc::u2_t entryCount = definition.Values.size();
	cnst ::llc::err_t repeatResult = definition.add_value(sampleValue, LLC_CXS("ALIAS"), LLC_CXS("ALIAS"), LLC_CXS("Alternative name."));
	LLC_TEST_CHECKF(errors, ENUM_TEST_RESULT_REPEAT_RESULT, false == ::llc::failed(repeatResult), "Repeat result:%i.", repeatResult);
	LLC_TEST_CHECKF(errors, ENUM_TEST_RESULT_REPEAT_COUNT , entryCount != definition.Values.size() , "Entry count:%u, expected:%u.", definition.Values.size(), entryCount );
	cnst ::llc::err_t conflictResult = definition.add_value((ENUM_ALIAS_SAMPLE)8, LLC_CXS("ALIAS"), LLC_CXS("ALIAS"), LLC_CXS("Conflicting value."));
	LLC_TEST_CHECKF(errors, ENUM_TEST_RESULT_CONFLICT_RESULT, false == ::llc::failed(conflictResult), "Conflict result:%i.", conflictResult);
	LLC_TEST_CHECKF(errors, ENUM_TEST_RESULT_CONFLICT_COUNT , entryCount != definition.Values.size() , "Entry count:%u, expected:%u.", definition.Values.size(), entryCount );
	LLC_TEST_CHECK(errors, ENUM_TEST_RESULT_CONFLICT_VALUE , definition.get_value(LLC_CXS("ALIAS")) != sampleValue);
	cnst ::llc::err_t automaticResult = definition.add_value_auto(LLC_CXS("NEXT"), LLC_CXS("NEXT"), LLC_CXS("NEXT"));
	LLC_TEST_CHECKF(errors, ENUM_TEST_RESULT_AUTO_INDEX , automaticResult != 2 , "NEXT index:%i, expected:2.", automaticResult );
	LLC_TEST_CHECK(errors, ENUM_TEST_RESULT_AUTO_VALUE , definition.get_value(LLC_CXS("NEXT")) != (ENUM_ALIAS_SAMPLE)1);

	LLC_TEST_CHECK(errors, ENUM_TEST_RESULT_ALIGN_X_ALIAS , ::llc::get_enum<::llc::ALIGN>().get_value(LLC_CXS("XCENTER")) != ::llc::ALIGN_HCENTER);
	LLC_TEST_CHECK(errors, ENUM_TEST_RESULT_ALIGN_Y_ALIAS , ::llc::get_enum<::llc::ALIGN>().get_value(LLC_CXS("YCENTER")) != ::llc::ALIGN_VCENTER);
	LLC_TEST_CHECK(errors, ENUM_TEST_RESULT_ALIGN_Z_ALIAS , ::llc::get_enum<::llc::ALIGN>().get_value(LLC_CXS("ZCENTER")) != ::llc::ALIGN_DCENTER);
	LLC_TEST_CHECK(errors, ENUM_TEST_RESULT_WIFI_AUTH_ALIAS , ::llc::get_enum<::llc::WIFI_AUTH>().get_value(LLC_CXS("WPA2_ENTERPRISE")) != ::llc::WIFI_AUTH_ENTERPRISE);
	rtrn 0;
}
