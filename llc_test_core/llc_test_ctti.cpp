#include "llc_test_core.h"
#include "llc_ctti.h"

GDEFINE_ENUM_TYPE(CTTI_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(CTTI_TEST_RESULT, OK					,  0, "All CTTI parser tests passed.");
GDEFINE_ENUM_VALUED(CTTI_TEST_RESULT, COMPILE_TIME		,  1, "The CTTI parser could not produce its result at compile time.");
GDEFINE_ENUM_VALUED(CTTI_TEST_RESULT, STRUCT_NAME		,  2, "The CTTI parser did not preserve the structure name.");
GDEFINE_ENUM_VALUED(CTTI_TEST_RESULT, MEMBER_COUNT		,  3, "The CTTI parser produced the wrong member count.");
GDEFINE_ENUM_VALUED(CTTI_TEST_RESULT, MEMBER_ORDER		,  4, "The CTTI parser changed member declaration order.");
GDEFINE_ENUM_VALUED(CTTI_TEST_RESULT, MEMBER_TYPE		,  5, "The CTTI parser classified an LLC member type incorrectly.");
GDEFINE_ENUM_VALUED(CTTI_TEST_RESULT, QUALIFIED_TYPE		,  6, "The CTTI parser rejected a qualified LLC type name.");
GDEFINE_ENUM_VALUED(CTTI_TEST_RESULT, WHITESPACE			,  7, "The CTTI parser mishandled declaration whitespace.");
GDEFINE_ENUM_VALUED(CTTI_TEST_RESULT, EMPTY				,  8, "The CTTI parser rejected an empty structure.");
GDEFINE_ENUM_VALUED(CTTI_TEST_RESULT, INVALID_STRUCT_NAME	,  9, "The CTTI parser accepted an invalid structure name or reported the wrong position.");
GDEFINE_ENUM_VALUED(CTTI_TEST_RESULT, UNSUPPORTED_TYPE	, 10, "The CTTI parser accepted a type outside the initial LLC grammar.");
GDEFINE_ENUM_VALUED(CTTI_TEST_RESULT, INVALID_MEMBER_NAME	, 11, "The CTTI parser accepted an invalid member name or reported the wrong position.");
GDEFINE_ENUM_VALUED(CTTI_TEST_RESULT, MISSING_MEMBER		, 12, "The CTTI parser accepted a missing member after a comma.");
GDEFINE_ENUM_VALUED(CTTI_TEST_RESULT, MISSING_SEMICOLON	, 13, "The CTTI parser accepted a declaration without its terminating semicolon.");

stxp auto CTTI_COORD = ::llc::cttiParseStruct("coord", "u0_t x, y; s2_t z; f2_t weight;");
static_assert(CTTI_COORD.Success(), "The basic CTTI declaration must parse at compile time.");
static_assert(4 == CTTI_COORD.Count, "The basic CTTI declaration must contain four members.");
static_assert(::llc::CTTI_TYPE_U0 == CTTI_COORD.Members[0].Type && ::llc::CTTI_TYPE_U0 == CTTI_COORD.Members[1].Type, "Shared declarations must preserve their LLC type.");
static_assert(::llc::CTTI_TYPE_S2 == CTTI_COORD.Members[2].Type && ::llc::CTTI_TYPE_F2 == CTTI_COORD.Members[3].Type, "Scalar LLC types must be classified at compile time.");

stxp auto CTTI_ALL_TYPES = ::llc::cttiParseStruct("all_types"
	, "b8_t B; sc_t C; uc_t UC; u0_t U0; u1_t U1; u2_t U2; u3_t U3; s0_t S0; s1_t S1; s2_t S2; s3_t S3; f2_t F2; f3_t F3;"
	);
static_assert(CTTI_ALL_TYPES.Success() && 13 == CTTI_ALL_TYPES.Count, "Every initial llc_typeint.h spelling must parse at compile time.");

::llc::err_t testCTTIParser(ATestError & errors) {
	LLC_TEST_CHECK(errors, CTTI_TEST_RESULT_COMPILE_TIME, false == CTTI_COORD.Success()
		, "Compile-time parse failed. error:%u, state:%u, offset:%u."
		, (::llc::u2_t)CTTI_COORD.Error, (::llc::u2_t)CTTI_COORD.State, CTTI_COORD.ErrorOffset
		);
	LLC_TEST_CHECK(errors, CTTI_TEST_RESULT_STRUCT_NAME, false == ::llc::cttiSpanEquals(CTTI_COORD.StructName, {0, 5}, "coord")
		, "Structure name mismatch: '%s'."
		, CTTI_COORD.StructName
		);
	LLC_TEST_CHECK(errors, CTTI_TEST_RESULT_MEMBER_COUNT, 4 != CTTI_COORD.Count
		, "Member count mismatch. actual:%u, expected:4."
		, CTTI_COORD.Count
		);
	stxp ::llc::vcst_t expectedNames[] = {LLC_CXS("x"), LLC_CXS("y"), LLC_CXS("z"), LLC_CXS("weight")};
	for(::llc::u2_t iMember = 0; iMember < CTTI_COORD.Count; ++iMember) {
		cnst ::llc::SCTTISpan & nameSpan = CTTI_COORD.Members[iMember].Name;
		cnst ::llc::vcsc_t actualName = {&CTTI_COORD.MemberText[nameSpan.Offset], nameSpan.Count};
		LLC_TEST_CHECK(errors, CTTI_TEST_RESULT_MEMBER_ORDER, actualName != expectedNames[iMember]
			, "Member %u name mismatch. span:%u/%u, expected:'%.*s'."
			, iMember, nameSpan.Offset, nameSpan.Count, (int)expectedNames[iMember].size(), expectedNames[iMember].begin()
			);
	}

	stxp ::llc::CTTI_TYPE expectedTypes[] =
		{ ::llc::CTTI_TYPE_B8, ::llc::CTTI_TYPE_SC, ::llc::CTTI_TYPE_UC
		, ::llc::CTTI_TYPE_U0, ::llc::CTTI_TYPE_U1, ::llc::CTTI_TYPE_U2, ::llc::CTTI_TYPE_U3
		, ::llc::CTTI_TYPE_S0, ::llc::CTTI_TYPE_S1, ::llc::CTTI_TYPE_S2, ::llc::CTTI_TYPE_S3
		, ::llc::CTTI_TYPE_F2, ::llc::CTTI_TYPE_F3
		};
	LLC_TEST_CHECK(errors, CTTI_TEST_RESULT_MEMBER_COUNT, 13 != CTTI_ALL_TYPES.Count
		, "All-types member count mismatch. actual:%u, expected:13."
		, CTTI_ALL_TYPES.Count
		);
	for(::llc::u2_t iMember = 0; iMember < CTTI_ALL_TYPES.Count; ++iMember)
		LLC_TEST_CHECK(errors, CTTI_TEST_RESULT_MEMBER_TYPE, expectedTypes[iMember] != CTTI_ALL_TYPES.Members[iMember].Type
			, "Member %u type mismatch. actual:%u, expected:%u."
			, iMember, (::llc::u2_t)CTTI_ALL_TYPES.Members[iMember].Type, (::llc::u2_t)expectedTypes[iMember]
			);

	stxp auto qualified = ::llc::cttiParseStruct("qualified", "::llc::u0_t A; llc::s2_t B; ::llc::f3_t C;");
	LLC_TEST_CHECK(errors, CTTI_TEST_RESULT_QUALIFIED_TYPE
		, false == qualified.Success() || 3 != qualified.Count || ::llc::CTTI_TYPE_U0 != qualified.Members[0].Type || ::llc::CTTI_TYPE_S2 != qualified.Members[1].Type || ::llc::CTTI_TYPE_F3 != qualified.Members[2].Type
		, "Qualified parse mismatch. error:%u, count:%u, types:%u/%u/%u."
		, (::llc::u2_t)qualified.Error, qualified.Count, (::llc::u2_t)qualified.Members[0].Type, (::llc::u2_t)qualified.Members[1].Type, (::llc::u2_t)qualified.Members[2].Type
		);

	stxp auto whitespace = ::llc::cttiParseStruct("spaced", "\n\tu1_t\t First ,\nSecond ;\r\n s3_t Last ; \t");
	LLC_TEST_CHECK(errors, CTTI_TEST_RESULT_WHITESPACE
		, false == whitespace.Success() || 3 != whitespace.Count || false == ::llc::cttiSpanEquals(whitespace.MemberText, whitespace.Members[1].Name, "Second")
		, "Whitespace parse mismatch. error:%u, count:%u, second span:%u/%u."
		, (::llc::u2_t)whitespace.Error, whitespace.Count, whitespace.Members[1].Name.Offset, whitespace.Members[1].Name.Count
		);

	stxp auto empty = ::llc::cttiParseStruct("empty", " \t\r\n");
	LLC_TEST_CHECK(errors, CTTI_TEST_RESULT_EMPTY, false == empty.Success() || empty.Count
		, "Empty parse mismatch. error:%u, count:%u."
		, (::llc::u2_t)empty.Error, empty.Count
		);

	stxp auto invalidStruct = ::llc::cttiParseStruct("bad-name", "u0_t X;");
	LLC_TEST_CHECK(errors, CTTI_TEST_RESULT_INVALID_STRUCT_NAME
		, ::llc::CTTI_PARSE_ERROR_INVALID_STRUCT_NAME != invalidStruct.Error || ::llc::CTTI_PARSE_STATE_STRUCT_NAME != invalidStruct.State || 3 != invalidStruct.ErrorOffset
		, "Invalid structure diagnostic mismatch. error:%u, state:%u, offset:%u."
		, (::llc::u2_t)invalidStruct.Error, (::llc::u2_t)invalidStruct.State, invalidStruct.ErrorOffset
		);

	stxp auto unsupported = ::llc::cttiParseStruct("native", "int X;");
	LLC_TEST_CHECK(errors, CTTI_TEST_RESULT_UNSUPPORTED_TYPE
		, ::llc::CTTI_PARSE_ERROR_UNSUPPORTED_TYPE != unsupported.Error || ::llc::CTTI_PARSE_STATE_TYPE != unsupported.State || unsupported.ErrorOffset
		, "Unsupported type diagnostic mismatch. error:%u, state:%u, offset:%u."
		, (::llc::u2_t)unsupported.Error, (::llc::u2_t)unsupported.State, unsupported.ErrorOffset
		);

	stxp auto invalidMember = ::llc::cttiParseStruct("invalid_member", "u0_t 2x;");
	LLC_TEST_CHECK(errors, CTTI_TEST_RESULT_INVALID_MEMBER_NAME
		, ::llc::CTTI_PARSE_ERROR_INVALID_MEMBER_NAME != invalidMember.Error || ::llc::CTTI_PARSE_STATE_MEMBER_NAME != invalidMember.State || 5 != invalidMember.ErrorOffset
		, "Invalid member diagnostic mismatch. error:%u, state:%u, offset:%u."
		, (::llc::u2_t)invalidMember.Error, (::llc::u2_t)invalidMember.State, invalidMember.ErrorOffset
		);

	stxp auto missingMember = ::llc::cttiParseStruct("missing_member", "u0_t X, ;");
	LLC_TEST_CHECK(errors, CTTI_TEST_RESULT_MISSING_MEMBER
		, ::llc::CTTI_PARSE_ERROR_EXPECTED_MEMBER_NAME != missingMember.Error || ::llc::CTTI_PARSE_STATE_MEMBER_NAME != missingMember.State
		, "Missing member diagnostic mismatch. error:%u, state:%u, offset:%u."
		, (::llc::u2_t)missingMember.Error, (::llc::u2_t)missingMember.State, missingMember.ErrorOffset
		);

	stxp auto missingSemicolon = ::llc::cttiParseStruct("missing_semicolon", "u0_t X");
	LLC_TEST_CHECK(errors, CTTI_TEST_RESULT_MISSING_SEMICOLON
		, ::llc::CTTI_PARSE_ERROR_EXPECTED_SEMICOLON != missingSemicolon.Error || ::llc::CTTI_PARSE_STATE_MEMBER_NAME != missingSemicolon.State || 6 != missingSemicolon.ErrorOffset
		, "Missing semicolon diagnostic mismatch. error:%u, state:%u, offset:%u."
		, (::llc::u2_t)missingSemicolon.Error, (::llc::u2_t)missingSemicolon.State, missingSemicolon.ErrorOffset
		);
	rtrn 0;
}
