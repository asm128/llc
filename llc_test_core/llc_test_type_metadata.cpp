#include "llc_member_registry.h"
#include "llc_pod_definition.h"
#include "llc_apod_serialize.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(TYPE_METADATA_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, OK                       ,  0, "All type-metadata tests passed.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_ELEMENT_SIZE        ,  1, "DATA_TYPE did not preserve the element bit count.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_ELEMENT_COUNT       ,  2, "DATA_TYPE did not preserve the element count.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_SIGN                ,  3, "DATA_TYPE did not preserve signedness.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_NORMALIZATION       ,  4, "DATA_TYPE did not preserve normalization.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_FLOAT               ,  5, "DATA_TYPE did not preserve floating-point classification.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_ENDIAN              ,  6, "DATA_TYPE did not preserve byte order.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_PADDING             ,  7, "DATA_TYPE did not preserve element-padding policy.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_ELEMENT_BYTES       ,  8, "SDataTypeID::ElementBytes() returned the wrong width.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_TOTAL_BYTES         ,  9, "SDataTypeID::TotalBytes() returned the wrong width.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_ROUND_TRIP          , 10, "DATA_TYPE did not survive decoding and encoding.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_STL                 , 11, "SDataTypeID::IsSTL() misclassified a descriptor.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_NON_UNIFORM         , 12, "SDataTypeID::IsNonUniform() misclassified a descriptor.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_UNIFORM             , 13, "SDataTypeID::IsUniform() misclassified a descriptor.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_INT64_NORM_256      , 14, "DATA_TYPE_INT64_NORM_256 has the wrong contract.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_SIZE_T              , 15, "DATA_TYPE_SIZE_T has the wrong contract.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_SCALAR_BOOL         , 16, "dataType() did not map bool.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_SCALAR_U8           , 17, "dataType() did not map an 8-bit unsigned value.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_SCALAR_U16          , 18, "dataType() did not map a 16-bit unsigned value.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_SCALAR_U32          , 19, "dataType() did not map a 32-bit unsigned value.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_SCALAR_U64          , 20, "dataType() did not map a 64-bit unsigned value.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_SCALAR_CHAR         , 21, "dataType() did not map char.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_SCALAR_I8           , 22, "dataType() did not map an 8-bit signed value.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_SCALAR_I16          , 23, "dataType() did not map a 16-bit signed value.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_SCALAR_I32          , 24, "dataType() did not map a 32-bit signed value.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_SCALAR_I64          , 25, "dataType() did not map a 64-bit signed value.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_SCALAR_F32          , 26, "dataType() did not map a 32-bit floating-point value.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, TYPE_SCALAR_F64          , 27, "dataType() did not map a 64-bit floating-point value.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, IDENTIFIER_NAMESPACE     , 28, "STypeIdentifier did not preserve its namespace.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, IDENTIFIER_NAME          , 29, "STypeIdentifier did not preserve its name.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, IDENTIFIER_TERMINATOR    , 30, "STypeIdentifier did not preserve terminated views.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, IDENTIFIER_EQUALITY      , 31, "STypeIdentifier equality ignored part of the identifier.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, REGISTRY_EMPTY           , 32, "An empty member_registry<> exposed a sentinel.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, REGISTRY_COUNT           , 33, "member_registry<> reported the wrong member count.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, REGISTRY_TYPES           , 34, "member_registry<> returned the wrong type identifiers.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, REGISTRY_NAMES           , 35, "member_registry<> returned the wrong member names.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, REGISTRY_DISPLAY_NAMES   , 36, "member_registry<> returned the wrong display names.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, REGISTRY_DESCRIPTIONS    , 37, "member_registry<> returned the wrong descriptions.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, REGISTRY_DATA_TYPES      , 38, "member_registry<> returned the wrong storage descriptors.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, REGISTRY_SIZES           , 39, "member_registry<> returned the wrong member sizes.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, REGISTRY_FREE_ACCESSOR   , 40, "get_member_registry<T>() returned a different registry.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, POD_PUSH_RESULT          , 41, "SPODDefinition rejected a valid member.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, POD_COUNTS               , 42, "SPODDefinition member arrays lost synchronization.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, POD_LABEL                , 43, "SPODDefinition did not preserve a member label.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, POD_TYPE                 , 44, "SPODDefinition did not preserve a storage descriptor.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, POD_OFFSET               , 45, "SPODDefinition recorded the wrong default-data offset.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, POD_DEFAULT_SIZE         , 46, "SPODDefinition stored the wrong default-data size.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, POD_DEFAULT_VALUE        , 47, "SPODDefinition changed a typed default value.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, POD_LABEL_OWNERSHIP      , 48, "SPODDefinition retained borrowed label storage.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, POD_DUPLICATE_REJECTION  , 49, "SPODDefinition accepted a duplicate member name.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, POD_DUPLICATE_STATE      , 50, "A rejected duplicate changed SPODDefinition state.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, POD_WIDTH_REJECTION      , 51, "SPODDefinition accepted a uniform default of the wrong width.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, POD_WIDTH_STATE          , 52, "A rejected default width changed SPODDefinition state.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, POD_PACKED_WIDTH         , 53, "SPODDefinition rejected a packed bit-field default.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, POD_PADDED_WIDTH         , 54, "SPODDefinition rejected a padded bit-field default.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, POD_NON_UNIFORM          , 55, "SPODDefinition rejected an explicitly sized non-uniform default.");
GDEFINE_ENUM_VALUED(TYPE_METADATA_TEST_RESULT, POD_LARGE_OFFSET         , 56, "SPODDefinition truncated an offset larger than 65535 bytes.");

namespace llc {
	struct STypeMetadataSample {
		LLCM(::llc, u0_t, Index , ::llc::DATA_TYPE_UINT8  , "Index display" , "Index description" );
		LLCM(::llc, f2_t, Weight, ::llc::DATA_TYPE_FLOAT32, "Weight display", "Weight description");
		LLCM_REGISTRY(LLCM_NAME(Index), LLCM_NAME(Weight));
	};
}

tplt<tpnm TCall>
sttc ::llc::err_t typeMetadataExpectedFailure(TCall call) {
	cnst ::llc::err_t result = call();
	rtrn result;
}

sttc ::llc::err_t testDataTypeMatrix(ATestError & errors) {
	cnst ::llc::u0_t bitCounts[] = {1, 5, 8, 9, 16, 32, 64, 128};
	cnst ::llc::u2_t elementCounts[] = {1, 2, 3, 8, 257, 0x100000};
	for(cnst ::llc::u0_t bitCount : bitCounts)
	for(cnst ::llc::u2_t elementCount : elementCounts)
	for(::llc::u0_t flags = 0; flags < 32; ++flags) {
		cnst bool isSigned = 0 != (flags & 0x01), isNormalized = 0 != (flags & 0x02), isFloat = 0 != (flags & 0x04), isBigEndian = 0 != (flags & 0x08), elementPad = 0 != (flags & 0x10);
		cnst ::llc::SDataTypeID source = {bitCount, elementCount, isFloat, isSigned, isNormalized, elementPad, isBigEndian};
		cnst ::llc::DATA_TYPE encoded = source;
		cnst ::llc::SDataTypeID decoded = encoded;
		cnst ::llc::u2_t expectedElementBytes = bitCount / 8 + one_if(bitCount % 8);
		cnst ::llc::u2_t expectedTotalBytes = elementPad ? expectedElementBytes * elementCount : bitCount * elementCount / 8 + one_if(bitCount * elementCount % 8);
		cnst bool expectedSTL = 1 == bitCount && isSigned && isNormalized && isFloat && !isBigEndian && !elementPad;
		cnst bool expectedNonUniform = 1 == bitCount && isSigned && !isNormalized && !isFloat && !isBigEndian && !elementPad;
		LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_ELEMENT_SIZE, bitCount != GTYPEID_ELEMENTSIZE(encoded), "bits:%u count:%u flags:0x%02X decoded:%u.", bitCount, elementCount, flags, GTYPEID_ELEMENTSIZE(encoded));
		LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_ELEMENT_COUNT, elementCount != GTYPEID_ELEMENTCOUNT(encoded), "bits:%u count:%u flags:0x%02X decoded:%u.", bitCount, elementCount, flags, GTYPEID_ELEMENTCOUNT(encoded));
		LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_SIGN, isSigned != (bool)GTYPEID_ISSIGNED(encoded), "bits:%u count:%u flags:0x%02X decoded:%u.", bitCount, elementCount, flags, (bool)GTYPEID_ISSIGNED(encoded));
		LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_NORMALIZATION, isNormalized != (bool)GTYPEID_ISNORMALIZED(encoded), "bits:%u count:%u flags:0x%02X decoded:%u.", bitCount, elementCount, flags, (bool)GTYPEID_ISNORMALIZED(encoded));
		LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_FLOAT, isFloat != (bool)GTYPEID_ISFLOAT(encoded), "bits:%u count:%u flags:0x%02X decoded:%u.", bitCount, elementCount, flags, (bool)GTYPEID_ISFLOAT(encoded));
		LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_ENDIAN, isBigEndian != (bool)GTYPEID_ISBIGENDIAN(encoded), "bits:%u count:%u flags:0x%02X decoded:%u.", bitCount, elementCount, flags, (bool)GTYPEID_ISBIGENDIAN(encoded));
		LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_PADDING, elementPad != (bool)GTYPEID_ELEMENTPAD(encoded), "bits:%u count:%u flags:0x%02X decoded:%u.", bitCount, elementCount, flags, (bool)GTYPEID_ELEMENTPAD(encoded));
		LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_ELEMENT_BYTES, expectedElementBytes != decoded.ElementBytes(), "bits:%u count:%u flags:0x%02X bytes:%u expected:%u.", bitCount, elementCount, flags, decoded.ElementBytes(), expectedElementBytes);
		LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_TOTAL_BYTES, expectedTotalBytes != decoded.TotalBytes(), "bits:%u count:%u flags:0x%02X bytes:%u expected:%u.", bitCount, elementCount, flags, decoded.TotalBytes(), expectedTotalBytes);
		LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_ROUND_TRIP, encoded != (::llc::DATA_TYPE)decoded, "bits:%u count:%u flags:0x%02X encoded:0x%08X returned:0x%08X.", bitCount, elementCount, flags, (::llc::u2_t)encoded, (::llc::u2_t)(::llc::DATA_TYPE)decoded);
		LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_STL, expectedSTL != decoded.IsSTL(), "bits:%u count:%u flags:0x%02X STL:%u expected:%u.", bitCount, elementCount, flags, decoded.IsSTL(), expectedSTL);
		LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_NON_UNIFORM, expectedNonUniform != decoded.IsNonUniform(), "bits:%u count:%u flags:0x%02X non-uniform:%u expected:%u.", bitCount, elementCount, flags, decoded.IsNonUniform(), expectedNonUniform);
		LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_UNIFORM, (!expectedSTL && !expectedNonUniform) != decoded.IsUniform(), "bits:%u count:%u flags:0x%02X uniform:%u expected:%u.", bitCount, elementCount, flags, decoded.IsUniform(), !expectedSTL && !expectedNonUniform);
	}
	rtrn 0;
}

sttc ::llc::err_t testDataTypeConstants(ATestError & errors) {
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_INT64_NORM_256, 64 != GTYPEID_ELEMENTSIZE(::llc::DATA_TYPE_INT64_NORM_256), "Bits:%u, expected:64.", GTYPEID_ELEMENTSIZE(::llc::DATA_TYPE_INT64_NORM_256));
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_INT64_NORM_256, 256 != GTYPEID_ELEMENTCOUNT(::llc::DATA_TYPE_INT64_NORM_256), "Count:%u, expected:256.", GTYPEID_ELEMENTCOUNT(::llc::DATA_TYPE_INT64_NORM_256));
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_INT64_NORM_256, !GTYPEID_ISSIGNED(::llc::DATA_TYPE_INT64_NORM_256), "%s", "INT64_NORM_256 was encoded unsigned.");
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_INT64_NORM_256, !GTYPEID_ISNORMALIZED(::llc::DATA_TYPE_INT64_NORM_256), "%s", "INT64_NORM_256 was not encoded normalized.");
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_INT64_NORM_256, GTYPEID_ISFLOAT(::llc::DATA_TYPE_INT64_NORM_256), "%s", "INT64_NORM_256 was encoded floating-point.");
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_SIZE_T, sizeof(size_t) * 8 != GTYPEID_ELEMENTSIZE(::llc::DATA_TYPE_SIZE_T), "Bits:%u, expected:%u.", GTYPEID_ELEMENTSIZE(::llc::DATA_TYPE_SIZE_T), (::llc::u2_t)(sizeof(size_t) * 8));
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_SIZE_T, GTYPEID_ISSIGNED(::llc::DATA_TYPE_SIZE_T), "%s", "size_t was encoded signed.");
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_SIZE_T, 1 != GTYPEID_ELEMENTCOUNT(::llc::DATA_TYPE_SIZE_T), "Count:%u, expected:1.", GTYPEID_ELEMENTCOUNT(::llc::DATA_TYPE_SIZE_T));
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_SCALAR_BOOL, ::llc::DATA_TYPE_BOOL != ::llc::dataType(::llc::b8_t{}), "%s", "bool mapped incorrectly.");
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_SCALAR_U8, ::llc::DATA_TYPE_UINT8 != ::llc::dataType(::llc::u0_t{}), "%s", "u0_t mapped incorrectly.");
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_SCALAR_U8, ::llc::DATA_TYPE_UINT8 != ::llc::dataType(::llc::uc_t{}), "%s", "uc_t mapped incorrectly.");
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_SCALAR_U16, ::llc::DATA_TYPE_UINT16 != ::llc::dataType(::llc::u1_t{}), "%s", "u1_t mapped incorrectly.");
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_SCALAR_U32, ::llc::DATA_TYPE_UINT32 != ::llc::dataType(::llc::u2_t{}), "%s", "u2_t mapped incorrectly.");
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_SCALAR_U64, ::llc::DATA_TYPE_UINT64 != ::llc::dataType(::llc::u3_t{}), "%s", "u3_t mapped incorrectly.");
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_SCALAR_CHAR, ::llc::DATA_TYPE_INT8 != ::llc::dataType(::llc::sc_t{}), "%s", "sc_t mapped incorrectly.");
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_SCALAR_I8, ::llc::DATA_TYPE_INT8 != ::llc::dataType(::llc::s0_t{}), "%s", "s0_t mapped incorrectly.");
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_SCALAR_I16, ::llc::DATA_TYPE_INT16 != ::llc::dataType(::llc::s1_t{}), "%s", "s1_t mapped incorrectly.");
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_SCALAR_I32, ::llc::DATA_TYPE_INT32 != ::llc::dataType(::llc::s2_t{}), "%s", "s2_t mapped incorrectly.");
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_SCALAR_I64, ::llc::DATA_TYPE_INT64 != ::llc::dataType(::llc::s3_t{}), "%s", "s3_t mapped incorrectly.");
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_SCALAR_F32, ::llc::DATA_TYPE_FLOAT32 != ::llc::dataType(::llc::f2_t{}), "%s", "f2_t mapped incorrectly.");
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_TYPE_SCALAR_F64, ::llc::DATA_TYPE_FLOAT64 != ::llc::dataType(::llc::f3_t{}), "%s", "f3_t mapped incorrectly.");
	rtrn 0;
}

sttc ::llc::err_t testTypeIdentifier(ATestError & errors) {
	cnst ::llc::STypeIdentifier type = {"llc", "u2_t"}, equal = {"llc", "u2_t"}, otherNamespace = {"std", "u2_t"}, otherName = {"llc", "u3_t"};
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_IDENTIFIER_NAMESPACE, type.NameSpace != "llc", "Namespace:'%.*s'.", LLC_VIEW_ARGS(type.NameSpace));
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_IDENTIFIER_NAME, type.Name != "u2_t", "Name:'%.*s'.", LLC_VIEW_ARGS(type.Name));
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_IDENTIFIER_TERMINATOR, type.NameSpace.begin()[type.NameSpace.size()], "Namespace terminator:%i.", type.NameSpace.begin()[type.NameSpace.size()]);
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_IDENTIFIER_TERMINATOR, type.Name.begin()[type.Name.size()], "Name terminator:%i.", type.Name.begin()[type.Name.size()]);
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_IDENTIFIER_EQUALITY, false == (type == equal), "%s", "Equal identifiers compared different.");
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_IDENTIFIER_EQUALITY, type == otherNamespace, "%s", "Different namespaces compared equal.");
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_IDENTIFIER_EQUALITY, type == otherName, "%s", "Different names compared equal.");
	rtrn 0;
}

sttc ::llc::err_t testMemberRegistry(ATestError & errors) {
	cnst ::llc::member_registry<> empty = {};
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_EMPTY, empty.get_member_count(), "Members:%u.", empty.get_member_count());
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_EMPTY, empty.get_types().size(), "Types:%u.", empty.get_types().size());
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_EMPTY, empty.get_names().size(), "Names:%u.", empty.get_names().size());
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_EMPTY, empty.get_display_names().size(), "Display names:%u.", empty.get_display_names().size());
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_EMPTY, empty.get_descriptions().size(), "Descriptions:%u.", empty.get_descriptions().size());
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_EMPTY, empty.get_data_type_ids().size(), "Data types:%u.", empty.get_data_type_ids().size());
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_EMPTY, empty.get_member_sizes().size(), "Sizes:%u.", empty.get_member_sizes().size());
	cnst auto & registry = ::llc::STypeMetadataSample::get_member_registry();
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_COUNT, 2 != registry.get_member_count(), "Members:%u, expected:2.", registry.get_member_count());
	LLC_TEST_REQUIRE(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_TYPES, 2 != registry.get_types().size());
	LLC_TEST_REQUIRE(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_NAMES, 2 != registry.get_names().size());
	LLC_TEST_REQUIRE(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_DISPLAY_NAMES, 2 != registry.get_display_names().size());
	LLC_TEST_REQUIRE(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_DESCRIPTIONS, 2 != registry.get_descriptions().size());
	LLC_TEST_REQUIRE(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_DATA_TYPES, 2 != registry.get_data_type_ids().size());
	LLC_TEST_REQUIRE(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_SIZES, 2 != registry.get_member_sizes().size());
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_TYPES, registry.get_types()[0].NameSpace != "::llc", "Namespace:'%.*s'.", LLC_VIEW_ARGS(registry.get_types()[0].NameSpace));
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_TYPES, registry.get_types()[0].Name != "u0_t", "First type:'%.*s'.", LLC_VIEW_ARGS(registry.get_types()[0].Name));
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_TYPES, registry.get_types()[1].Name != "f2_t", "Second type:'%.*s'.", LLC_VIEW_ARGS(registry.get_types()[1].Name));
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_NAMES, registry.get_names()[0] != ::llc::label{"Index"}, "First name:'%.*s'.", LLC_VIEW_ARGS(registry.get_names()[0]));
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_NAMES, registry.get_names()[1] != ::llc::label{"Weight"}, "Second name:'%.*s'.", LLC_VIEW_ARGS(registry.get_names()[1]));
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_DISPLAY_NAMES, registry.get_display_names()[0] != ::llc::label{"Index display"}, "First display:'%.*s'.", LLC_VIEW_ARGS(registry.get_display_names()[0]));
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_DISPLAY_NAMES, registry.get_display_names()[1] != ::llc::label{"Weight display"}, "Second display:'%.*s'.", LLC_VIEW_ARGS(registry.get_display_names()[1]));
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_DESCRIPTIONS, registry.get_descriptions()[0] != ::llc::label{"Index description"}, "First description:'%.*s'.", LLC_VIEW_ARGS(registry.get_descriptions()[0]));
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_DESCRIPTIONS, registry.get_descriptions()[1] != ::llc::label{"Weight description"}, "Second description:'%.*s'.", LLC_VIEW_ARGS(registry.get_descriptions()[1]));
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_DATA_TYPES, ::llc::DATA_TYPE_UINT8 != registry.get_data_type_ids()[0], "First:0x%08X.", (::llc::u2_t)registry.get_data_type_ids()[0]);
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_DATA_TYPES, ::llc::DATA_TYPE_FLOAT32 != registry.get_data_type_ids()[1], "Second:0x%08X.", (::llc::u2_t)registry.get_data_type_ids()[1]);
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_SIZES, sizeof(::llc::u0_t) != registry.get_member_sizes()[0], "First:%u.", registry.get_member_sizes()[0]);
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_SIZES, sizeof(::llc::f2_t) != registry.get_member_sizes()[1], "Second:%u.", registry.get_member_sizes()[1]);
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_REGISTRY_FREE_ACCESSOR, &registry != &::llc::get_member_registry<::llc::STypeMetadataSample>(), "%s", "Free accessor returned another instance.");
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testPODValue(ATestError & errors, ::llc::SPODDefinition & definition, ::llc::vcst_t labelName, cnst T value, ::llc::DATA_TYPE expectedType) {
	cnst ::llc::u2_t oldMemberCount = definition.Labels.size(), oldByteCount = definition.DefaultData.size();
	cnst ::llc::err_t pushResult = definition.push_any(labelName, value);
	LLC_TEST_REQUIREF(errors, TYPE_METADATA_TEST_RESULT_POD_PUSH_RESULT, ::llc::failed(pushResult), "Label:'%.*s' result:%i.", LLC_VIEW_ARGS(labelName), pushResult);
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_POD_PUSH_RESULT, (::llc::err_t)oldMemberCount != pushResult, "Label:'%.*s' result:%i expected:%u.", LLC_VIEW_ARGS(labelName), pushResult, oldMemberCount);
	LLC_TEST_REQUIRE(errors, TYPE_METADATA_TEST_RESULT_POD_COUNTS, oldMemberCount + 1 != definition.Labels.size());
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_POD_COUNTS, definition.Labels.size() != definition.Types.size(), "Labels:%u types:%u.", definition.Labels.size(), definition.Types.size());
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_POD_COUNTS, definition.Labels.size() != definition.Offsets.size(), "Labels:%u offsets:%u.", definition.Labels.size(), definition.Offsets.size());
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_POD_LABEL, false == (definition.Labels[oldMemberCount] == labelName), "Stored:'%.*s' expected:'%.*s'.", LLC_VIEW_ARGS(definition.Labels[oldMemberCount]), LLC_VIEW_ARGS(labelName));
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_POD_TYPE, expectedType != definition.Types[oldMemberCount], "Stored:0x%08X expected:0x%08X.", (::llc::u2_t)definition.Types[oldMemberCount], (::llc::u2_t)expectedType);
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_POD_OFFSET, oldByteCount != definition.Offsets[oldMemberCount], "Stored:%u expected:%u.", definition.Offsets[oldMemberCount], oldByteCount);
	LLC_TEST_REQUIRE(errors, TYPE_METADATA_TEST_RESULT_POD_DEFAULT_SIZE, oldByteCount + sizeof(T) != definition.DefaultData.size());
	::llc::au0_t expectedBytes = {};
	if_fail_fe(::llc::savePOD(expectedBytes, value));
	cnst ::llc::vcu0_t expectedView = expectedBytes, storedView = {&definition.DefaultData[oldByteCount], expectedBytes.size()};
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_POD_DEFAULT_VALUE, expectedView != storedView, "Label:'%.*s' bytes differ.", LLC_VIEW_ARGS(labelName));
	rtrn 0;
}

sttc ::llc::err_t testPODDefinition(ATestError & errors) {
	::llc::SPODDefinition definition = {};
	if_fail_fe(::testPODValue(errors, definition, "bool", ::llc::b8_t{true}, ::llc::DATA_TYPE_BOOL));
	if_fail_fe(::testPODValue(errors, definition, "u8", ::llc::u0_t{0xA5}, ::llc::DATA_TYPE_UINT8));
	if_fail_fe(::testPODValue(errors, definition, "u16", ::llc::u1_t{0xA55A}, ::llc::DATA_TYPE_UINT16));
	if_fail_fe(::testPODValue(errors, definition, "u32", ::llc::u2_t{0xA55AA55A}, ::llc::DATA_TYPE_UINT32));
	if_fail_fe(::testPODValue(errors, definition, "u64", ::llc::u3_t{0xA55AA55AA55AA55A}, ::llc::DATA_TYPE_UINT64));
	if_fail_fe(::testPODValue(errors, definition, "char", ::llc::sc_t{'z'}, ::llc::DATA_TYPE_INT8));
	if_fail_fe(::testPODValue(errors, definition, "i8", ::llc::s0_t{-12}, ::llc::DATA_TYPE_INT8));
	if_fail_fe(::testPODValue(errors, definition, "i16", ::llc::s1_t{-1234}, ::llc::DATA_TYPE_INT16));
	if_fail_fe(::testPODValue(errors, definition, "i32", ::llc::s2_t{-1234567}, ::llc::DATA_TYPE_INT32));
	if_fail_fe(::testPODValue(errors, definition, "i64", ::llc::s3_t{-1234567890123LL}, ::llc::DATA_TYPE_INT64));
	if_fail_fe(::testPODValue(errors, definition, "f32", ::llc::f2_t{0.25f}, ::llc::DATA_TYPE_FLOAT32));
	if_fail_fe(::testPODValue(errors, definition, "f64", ::llc::f3_t{-0.5}, ::llc::DATA_TYPE_FLOAT64));

	::llc::sc_t mutableName[] = {'o', 'w', 'n', 'e', 'd', 0};
	cnst ::llc::vcst_t mutableNameView = {mutableName, 5};
	cnst ::llc::err_t ownedResult = definition.push_any(mutableNameView, ::llc::u0_t{7});
	LLC_TEST_REQUIREF(errors, TYPE_METADATA_TEST_RESULT_POD_PUSH_RESULT, ::llc::failed(ownedResult), "Owned-label result:%i.", ownedResult);
	mutableName[0] = 'x';
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_POD_LABEL_OWNERSHIP, false == (definition.Labels[ownedResult] == "owned"), "Stored:'%.*s'.", LLC_VIEW_ARGS(definition.Labels[ownedResult]));

	cnst ::llc::u2_t memberCountBeforeFailure = definition.Labels.size(), byteCountBeforeFailure = definition.DefaultData.size();
	cnst ::llc::err_t duplicateResult = ::typeMetadataExpectedFailure([&]() { rtrn definition.push_any("u8", ::llc::u0_t{9}); });
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_POD_DUPLICATE_REJECTION, false == ::llc::failed(duplicateResult), "Result:%i.", duplicateResult);
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_POD_DUPLICATE_STATE, memberCountBeforeFailure != definition.Labels.size(), "Labels:%u expected:%u.", definition.Labels.size(), memberCountBeforeFailure);
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_POD_DUPLICATE_STATE, memberCountBeforeFailure != definition.Types.size(), "Types:%u expected:%u.", definition.Types.size(), memberCountBeforeFailure);
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_POD_DUPLICATE_STATE, memberCountBeforeFailure != definition.Offsets.size(), "Offsets:%u expected:%u.", definition.Offsets.size(), memberCountBeforeFailure);
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_POD_DUPLICATE_STATE, byteCountBeforeFailure != definition.DefaultData.size(), "Bytes:%u expected:%u.", definition.DefaultData.size(), byteCountBeforeFailure);

	cnst ::llc::u0_t shortDefault[] = {0x11};
	cnst ::llc::vcu0_t shortDefaultView = shortDefault;
	cnst ::llc::err_t widthResult = ::typeMetadataExpectedFailure([&]() { rtrn definition.push_any(::llc::DATA_TYPE_UINT32, "bad width", shortDefaultView); });
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_POD_WIDTH_REJECTION, false == ::llc::failed(widthResult), "Result:%i.", widthResult);
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_POD_WIDTH_STATE, memberCountBeforeFailure != definition.Labels.size(), "Labels:%u expected:%u.", definition.Labels.size(), memberCountBeforeFailure);
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_POD_WIDTH_STATE, memberCountBeforeFailure != definition.Types.size(), "Types:%u expected:%u.", definition.Types.size(), memberCountBeforeFailure);
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_POD_WIDTH_STATE, memberCountBeforeFailure != definition.Offsets.size(), "Offsets:%u expected:%u.", definition.Offsets.size(), memberCountBeforeFailure);
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_POD_WIDTH_STATE, byteCountBeforeFailure != definition.DefaultData.size(), "Bytes:%u expected:%u.", definition.DefaultData.size(), byteCountBeforeFailure);

	cnst ::llc::u0_t packedDefault[] = {0xAA, 0x55};
	cnst ::llc::vcu0_t packedDefaultView = packedDefault;
	cnst ::llc::err_t packedResult = definition.push_any(::llc::DATA_TYPE_UINT5_3, "packed", packedDefaultView);
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_POD_PACKED_WIDTH, ::llc::failed(packedResult), "Result:%i.", packedResult);
	cnst ::llc::DATA_TYPE paddedType = ::llc::SDataTypeID{5, 3, false, false, false, true};
	cnst ::llc::u0_t paddedDefault[] = {0x01, 0x02, 0x03};
	cnst ::llc::vcu0_t paddedDefaultView = paddedDefault;
	cnst ::llc::err_t paddedResult = definition.push_any(paddedType, "padded", paddedDefaultView);
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_POD_PADDED_WIDTH, ::llc::failed(paddedResult), "Result:%i.", paddedResult);

	::llc::SPODDefinition largeDefinition = {};
	::llc::au0_t arbitraryDefault = {};
	if_fail_fe(arbitraryDefault.resize(70000, 0x5A));
	cnst ::llc::vcu0_t arbitraryDefaultView = arbitraryDefault;
	cnst ::llc::err_t customResult = largeDefinition.push_any(::llc::DATA_TYPE_CUSTOM, "custom", arbitraryDefaultView);
	LLC_TEST_REQUIREF(errors, TYPE_METADATA_TEST_RESULT_POD_NON_UNIFORM, ::llc::failed(customResult), "Result:%i.", customResult);
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_POD_NON_UNIFORM, 70000 != largeDefinition.DefaultData.size(), "Bytes:%u expected:70000.", largeDefinition.DefaultData.size());
	cnst ::llc::err_t followingResult = largeDefinition.push_any("following", ::llc::u0_t{3});
	LLC_TEST_REQUIREF(errors, TYPE_METADATA_TEST_RESULT_POD_LARGE_OFFSET, ::llc::failed(followingResult), "Result:%i.", followingResult);
	LLC_TEST_REQUIRE(errors, TYPE_METADATA_TEST_RESULT_POD_LARGE_OFFSET, 2 != largeDefinition.Offsets.size());
	LLC_TEST_CHECKF(errors, TYPE_METADATA_TEST_RESULT_POD_LARGE_OFFSET, 70000 != largeDefinition.Offsets[1], "Offset:%u expected:70000.", largeDefinition.Offsets[1]);
	rtrn 0;
}

::llc::err_t testTypeMetadata(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	if_fail_fe(::testDataTypeMatrix(errors));
	if_fail_fe(::testDataTypeConstants(errors));
	if_fail_fe(::testTypeIdentifier(errors));
	if_fail_fe(::testMemberRegistry(errors));
	rtrn ::testPODDefinition(errors);
}
