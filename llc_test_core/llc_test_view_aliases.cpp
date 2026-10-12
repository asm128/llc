#include "llc_aobj_pobj.h"
#include "llc_view_n2.h"
#include "llc_view_n3.h"
#include "llc_view_ppod.h"
#include "llc_view_tri.h"
#include "llc_view_tri2.h"
#include "llc_view_tri3.h"

#include "llc_test_core.h"

#include <type_traits>

GDEFINE_ENUM_TYPE(VIEW_ALIAS_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(VIEW_ALIAS_TEST_RESULT, MUTABLE_ELEMENT		, 0, "A mutable view alias names the wrong element type.");
GDEFINE_ENUM_VALUED(VIEW_ALIAS_TEST_RESULT, CONST_ELEMENT		, 1, "A const-element view alias did not preserve element constness.");
GDEFINE_ENUM_VALUED(VIEW_ALIAS_TEST_RESULT, MUTABLE_TOP_CONST	, 2, "A mutable-element view's top-level const alias names the wrong type.");
GDEFINE_ENUM_VALUED(VIEW_ALIAS_TEST_RESULT, CONST_TOP_CONST		, 3, "A const-element view's top-level const alias names the wrong type.");
GDEFINE_ENUM_VALUED(VIEW_ALIAS_TEST_RESULT, SIGNED_CHAR_IDENTITY	, 4, "The signed byte type does not share the platform's signed character representation.");
GDEFINE_ENUM_VALUED(VIEW_ALIAS_TEST_RESULT, SIGNED_BYTE_VIEW		, 5, "A signed byte view alias names the wrong raw view type.");
GDEFINE_ENUM_VALUED(VIEW_ALIAS_TEST_RESULT, TEXT_VIEW_IDENTITY	, 6, "A semantic text view collapsed into its raw signed byte representation.");

#define LLC_TEST_VIEW_ALIAS(errors, mutableAlias, mutableConstAlias, constAlias, constConstAlias, elementType) do { \
	LLC_TEST_CHECKF(errors, VIEW_ALIAS_TEST_RESULT_MUTABLE_ELEMENT	, (false == ::std::is_same_v<::llc::mutableAlias, ::llc::view<elementType>>)			, "Alias:%s.", #mutableAlias); \
	LLC_TEST_CHECKF(errors, VIEW_ALIAS_TEST_RESULT_CONST_ELEMENT		, (false == ::std::is_same_v<::llc::constAlias, ::llc::view<cnst elementType>>)			, "Alias:%s.", #constAlias); \
	LLC_TEST_CHECKF(errors, VIEW_ALIAS_TEST_RESULT_MUTABLE_TOP_CONST	, (false == ::std::is_same_v<::llc::mutableConstAlias, cnst ::llc::mutableAlias>)	, "Alias:%s.", #mutableConstAlias); \
	LLC_TEST_CHECKF(errors, VIEW_ALIAS_TEST_RESULT_CONST_TOP_CONST	, (false == ::std::is_same_v<::llc::constConstAlias, cnst ::llc::constAlias>)		, "Alias:%s.", #constConstAlias); \
} while(0)

::llc::err_t testViewAliases(ATestError & errors) {
	LLC_TEST_CHECK(errors, VIEW_ALIAS_TEST_RESULT_SIGNED_CHAR_IDENTITY, (false == ::std::is_same_v<::llc::s0_t, ::llc::sc_t>));
	LLC_TEST_CHECK(errors, VIEW_ALIAS_TEST_RESULT_SIGNED_BYTE_VIEW, (false == ::std::is_same_v<::llc::vs0_t, ::llc::view<::llc::s0_t>>));
	LLC_TEST_CHECK(errors, VIEW_ALIAS_TEST_RESULT_SIGNED_BYTE_VIEW, (false == ::std::is_same_v<::llc::vcs0_t, ::llc::view<cnst ::llc::s0_t>>));
	LLC_TEST_CHECK(errors, VIEW_ALIAS_TEST_RESULT_TEXT_VIEW_IDENTITY, (::std::is_same_v<::llc::vstr_t, ::llc::vs0_t>));
	LLC_TEST_CHECK(errors, VIEW_ALIAS_TEST_RESULT_TEXT_VIEW_IDENTITY, (::std::is_same_v<::llc::vcst_t, ::llc::vcs0_t>));
	LLC_TEST_VIEW_ALIAS(errors, vn2sc_t	, vn2sc_c	, vcn2sc_t	, vcn2sc_c	, ::llc::n2<::llc::sc_t>);
	LLC_TEST_VIEW_ALIAS(errors, vn2uc_t	, vn2uc_c	, vcn2uc_t	, vcn2uc_c	, ::llc::n2<::llc::uc_t>);
	LLC_TEST_VIEW_ALIAS(errors, vn2u0_t	, vn2u0_c	, vcn2u0_t	, vcn2u0_c	, ::llc::n2<::llc::u0_t>);
	LLC_TEST_VIEW_ALIAS(errors, vn2u1_t	, vn2u1_c	, vcn2u1_t	, vcn2u1_c	, ::llc::n2<::llc::u1_t>);
	LLC_TEST_VIEW_ALIAS(errors, vn2u2_t	, vn2u2_c	, vcn2u2_t	, vcn2u2_c	, ::llc::n2<::llc::u2_t>);
	LLC_TEST_VIEW_ALIAS(errors, vn2u3_t	, vn2u3_c	, vcn2u3_t	, vcn2u3_c	, ::llc::n2<::llc::u3_t>);
	LLC_TEST_VIEW_ALIAS(errors, vn2s0_t	, vn2s0_c	, vcn2s0_t	, vcn2s0_c	, ::llc::n2<::llc::s0_t>);
	LLC_TEST_VIEW_ALIAS(errors, vn2s1_t	, vn2s1_c	, vcn2s1_t	, vcn2s1_c	, ::llc::n2<::llc::s1_t>);
	LLC_TEST_VIEW_ALIAS(errors, vn2s2_t	, vn2s2_c	, vcn2s2_t	, vcn2s2_c	, ::llc::n2<::llc::s2_t>);
	LLC_TEST_VIEW_ALIAS(errors, vn2s3_t	, vn2s3_c	, vcn2s3_t	, vcn2s3_c	, ::llc::n2<::llc::s3_t>);
	LLC_TEST_VIEW_ALIAS(errors, vn2f2_t	, vn2f2_c	, vcn2f2_t	, vcn2f2_c	, ::llc::n2<::llc::f2_t>);
	LLC_TEST_VIEW_ALIAS(errors, vn2f3_t	, vn2f3_c	, vcn2f3_t	, vcn2f3_c	, ::llc::n2<::llc::f3_t>);

	LLC_TEST_VIEW_ALIAS(errors, vn3sc_t	, vn3sc_c	, vcn3sc_t	, vcn3sc_c	, ::llc::n3<::llc::sc_t>);
	LLC_TEST_VIEW_ALIAS(errors, vn3uc_t	, vn3uc_c	, vcn3uc_t	, vcn3uc_c	, ::llc::n3<::llc::uc_t>);
	LLC_TEST_VIEW_ALIAS(errors, vn3u0_t	, vn3u0_c	, vcn3u0_t	, vcn3u0_c	, ::llc::n3<::llc::u0_t>);
	LLC_TEST_VIEW_ALIAS(errors, vn3u1_t	, vn3u1_c	, vcn3u1_t	, vcn3u1_c	, ::llc::n3<::llc::u1_t>);
	LLC_TEST_VIEW_ALIAS(errors, vn3u2_t	, vn3u2_c	, vcn3u2_t	, vcn3u2_c	, ::llc::n3<::llc::u2_t>);
	LLC_TEST_VIEW_ALIAS(errors, vn3u3_t	, vn3u3_c	, vcn3u3_t	, vcn3u3_c	, ::llc::n3<::llc::u3_t>);
	LLC_TEST_VIEW_ALIAS(errors, vn3s0_t	, vn3s0_c	, vcn3s0_t	, vcn3s0_c	, ::llc::n3<::llc::s0_t>);
	LLC_TEST_VIEW_ALIAS(errors, vn3s1_t	, vn3s1_c	, vcn3s1_t	, vcn3s1_c	, ::llc::n3<::llc::s1_t>);
	LLC_TEST_VIEW_ALIAS(errors, vn3s2_t	, vn3s2_c	, vcn3s2_t	, vcn3s2_c	, ::llc::n3<::llc::s2_t>);
	LLC_TEST_VIEW_ALIAS(errors, vn3s3_t	, vn3s3_c	, vcn3s3_t	, vcn3s3_c	, ::llc::n3<::llc::s3_t>);
	LLC_TEST_VIEW_ALIAS(errors, vn3f2_t	, vn3f2_c	, vcn3f2_t	, vcn3f2_c	, ::llc::n3<::llc::f2_t>);
	LLC_TEST_VIEW_ALIAS(errors, vn3f3_t	, vn3f3_c	, vcn3f3_t	, vcn3f3_c	, ::llc::n3<::llc::f3_t>);

	LLC_TEST_VIEW_ALIAS(errors, vtrisc_t	, vtrisc_c	, vctrisc_t	, vctrisc_c	, ::llc::tri<::llc::sc_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtriuc_t	, vtriuc_c	, vctriuc_t	, vctriuc_c	, ::llc::tri<::llc::uc_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtriu0_t	, vtriu0_c	, vctriu0_t	, vctriu0_c	, ::llc::tri<::llc::u0_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtriu1_t	, vtriu1_c	, vctriu1_t	, vctriu1_c	, ::llc::tri<::llc::u1_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtriu2_t	, vtriu2_c	, vctriu2_t	, vctriu2_c	, ::llc::tri<::llc::u2_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtriu3_t	, vtriu3_c	, vctriu3_t	, vctriu3_c	, ::llc::tri<::llc::u3_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtris0_t	, vtris0_c	, vctris0_t	, vctris0_c	, ::llc::tri<::llc::s0_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtris1_t	, vtris1_c	, vctris1_t	, vctris1_c	, ::llc::tri<::llc::s1_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtris2_t	, vtris2_c	, vctris2_t	, vctris2_c	, ::llc::tri<::llc::s2_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtris3_t	, vtris3_c	, vctris3_t	, vctris3_c	, ::llc::tri<::llc::s3_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtrif2_t	, vtrif2_c	, vctrif2_t	, vctrif2_c	, ::llc::tri<::llc::f2_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtrif3_t	, vtrif3_c	, vctrif3_t	, vctrif3_c	, ::llc::tri<::llc::f3_t>);

	LLC_TEST_VIEW_ALIAS(errors, vtri2sc_t	, vtri2sc_c	, vctri2sc_t	, vctri2sc_c	, ::llc::tri2<::llc::sc_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtri2uc_t	, vtri2uc_c	, vctri2uc_t	, vctri2uc_c	, ::llc::tri2<::llc::uc_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtri2u0_t	, vtri2u0_c	, vctri2u0_t	, vctri2u0_c	, ::llc::tri2<::llc::u0_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtri2u1_t	, vtri2u1_c	, vctri2u1_t	, vctri2u1_c	, ::llc::tri2<::llc::u1_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtri2u2_t	, vtri2u2_c	, vctri2u2_t	, vctri2u2_c	, ::llc::tri2<::llc::u2_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtri2u3_t	, vtri2u3_c	, vctri2u3_t	, vctri2u3_c	, ::llc::tri2<::llc::u3_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtri2s0_t	, vtri2s0_c	, vctri2s0_t	, vctri2s0_c	, ::llc::tri2<::llc::s0_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtri2s1_t	, vtri2s1_c	, vctri2s1_t	, vctri2s1_c	, ::llc::tri2<::llc::s1_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtri2s2_t	, vtri2s2_c	, vctri2s2_t	, vctri2s2_c	, ::llc::tri2<::llc::s2_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtri2s3_t	, vtri2s3_c	, vctri2s3_t	, vctri2s3_c	, ::llc::tri2<::llc::s3_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtri2f2_t	, vtri2f2_c	, vctri2f2_t	, vctri2f2_c	, ::llc::tri2<::llc::f2_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtri2f3_t	, vtri2f3_c	, vctri2f3_t	, vctri2f3_c	, ::llc::tri2<::llc::f3_t>);

	LLC_TEST_VIEW_ALIAS(errors, vtri3sc_t	, vtri3sc_c	, vctri3sc_t	, vctri3sc_c	, ::llc::tri3<::llc::sc_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtri3uc_t	, vtri3uc_c	, vctri3uc_t	, vctri3uc_c	, ::llc::tri3<::llc::uc_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtri3u0_t	, vtri3u0_c	, vctri3u0_t	, vctri3u0_c	, ::llc::tri3<::llc::u0_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtri3u1_t	, vtri3u1_c	, vctri3u1_t	, vctri3u1_c	, ::llc::tri3<::llc::u1_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtri3u2_t	, vtri3u2_c	, vctri3u2_t	, vctri3u2_c	, ::llc::tri3<::llc::u2_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtri3u3_t	, vtri3u3_c	, vctri3u3_t	, vctri3u3_c	, ::llc::tri3<::llc::u3_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtri3s0_t	, vtri3s0_c	, vctri3s0_t	, vctri3s0_c	, ::llc::tri3<::llc::s0_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtri3s1_t	, vtri3s1_c	, vctri3s1_t	, vctri3s1_c	, ::llc::tri3<::llc::s1_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtri3s2_t	, vtri3s2_c	, vctri3s2_t	, vctri3s2_c	, ::llc::tri3<::llc::s2_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtri3s3_t	, vtri3s3_c	, vctri3s3_t	, vctri3s3_c	, ::llc::tri3<::llc::s3_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtri3f2_t	, vtri3f2_c	, vctri3f2_t	, vctri3f2_c	, ::llc::tri3<::llc::f2_t>);
	LLC_TEST_VIEW_ALIAS(errors, vtri3f3_t	, vtri3f3_c	, vctri3f3_t	, vctri3f3_c	, ::llc::tri3<::llc::f3_t>);

	LLC_TEST_CHECKF(errors, VIEW_ALIAS_TEST_RESULT_MUTABLE_ELEMENT, (false == ::std::is_same_v<::llc::vpobj<::llc::s2_t>, ::llc::view<::llc::pobj<::llc::s2_t>>>), "Alias:%s.", "vpobj<s2_t>");
	LLC_TEST_CHECKF(errors, VIEW_ALIAS_TEST_RESULT_CONST_ELEMENT, (false == ::std::is_same_v<::llc::vcpobj<::llc::s2_t>, ::llc::view<cnst ::llc::pobj<::llc::s2_t>>>), "Alias:%s.", "vcpobj<s2_t>");
	LLC_TEST_CHECKF(errors, VIEW_ALIAS_TEST_RESULT_MUTABLE_ELEMENT, (false == ::std::is_same_v<::llc::vppod<::llc::s2_t>, ::llc::view<::llc::ppod<::llc::s2_t>>>), "Alias:%s.", "vppod<s2_t>");
	LLC_TEST_CHECKF(errors, VIEW_ALIAS_TEST_RESULT_CONST_ELEMENT, (false == ::std::is_same_v<::llc::vcppod<::llc::s2_t>, ::llc::view<cnst ::llc::ppod<::llc::s2_t>>>), "Alias:%s.", "vcppod<s2_t>");
	rtrn 0;
}

#undef LLC_TEST_VIEW_ALIAS
