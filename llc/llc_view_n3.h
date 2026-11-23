/// Copyright 2009-2023 - asm128
#include "llc_n3.h"
#include "llc_view.h"

#ifndef LLC_VIEW_N3_2973498273423617
#define LLC_VIEW_N3_2973498273423617

namespace llc
{
	tplT using	vn3		= ::llc::view<::llc::n3<T>>;
	tplT using	vcn3	= ::llc::view<cnst ::llc::n3<T>>;
	tplT using	v3		= ::llc::vn3<T>;
	tplT using	vc3		= ::llc::vcn3<T>;

	tydf	::llc::vn3<sc_t>	vn3sc_t;	tdcs	vn3sc_t	vn3sc_c;
	tydf	::llc::vn3<uc_t>	vn3uc_t;	tdcs	vn3uc_t	vn3uc_c;
	tydf	::llc::vn3<u0_t>	vn3u0_t;	tdcs	vn3u0_t	vn3u0_c;
	tydf	::llc::vn3<u1_t>	vn3u1_t;	tdcs	vn3u1_t	vn3u1_c;
	tydf	::llc::vn3<u2_t>	vn3u2_t;	tdcs	vn3u2_t	vn3u2_c;
	tydf	::llc::vn3<u3_t>	vn3u3_t;	tdcs	vn3u3_t	vn3u3_c;
	tydf	::llc::vn3<s0_t>	vn3s0_t;	tdcs	vn3s0_t	vn3s0_c;
	tydf	::llc::vn3<s1_t>	vn3s1_t;	tdcs	vn3s1_t	vn3s1_c;
	tydf	::llc::vn3<s2_t>	vn3s2_t;	tdcs	vn3s2_t	vn3s2_c;
	tydf	::llc::vn3<s3_t>	vn3s3_t;	tdcs	vn3s3_t	vn3s3_c;
	tydf	::llc::vn3<f2_t>	vn3f2_t;	tdcs	vn3f2_t	vn3f2_c;
	tydf	::llc::vn3<f3_t>	vn3f3_t;	tdcs	vn3f3_t	vn3f3_c;

	tydf	::llc::vcn3<sc_t>	vcn3sc_t;	tdcs	vcn3sc_t	vcn3sc_c;
	tydf	::llc::vcn3<uc_t>	vcn3uc_t;	tdcs	vcn3uc_t	vcn3uc_c;
	tydf	::llc::vcn3<u0_t>	vcn3u0_t;	tdcs	vcn3u0_t	vcn3u0_c;
	tydf	::llc::vcn3<u1_t>	vcn3u1_t;	tdcs	vcn3u1_t	vcn3u1_c;
	tydf	::llc::vcn3<u2_t>	vcn3u2_t;	tdcs	vcn3u2_t	vcn3u2_c;
	tydf	::llc::vcn3<u3_t>	vcn3u3_t;	tdcs	vcn3u3_t	vcn3u3_c;
	tydf	::llc::vcn3<s0_t>	vcn3s0_t;	tdcs	vcn3s0_t	vcn3s0_c;
	tydf	::llc::vcn3<s1_t>	vcn3s1_t;	tdcs	vcn3s1_t	vcn3s1_c;
	tydf	::llc::vcn3<s2_t>	vcn3s2_t;	tdcs	vcn3s2_t	vcn3s2_c;
	tydf	::llc::vcn3<s3_t>	vcn3s3_t;	tdcs	vcn3s3_t	vcn3s3_c;
	tydf	::llc::vcn3<f2_t>	vcn3f2_t;	tdcs	vcn3f2_t	vcn3f2_c;
	tydf	::llc::vcn3<f3_t>	vcn3f3_t;	tdcs	vcn3f3_t	vcn3f3_c;

	tydf	::llc::v3<sc_t>	v3sc_t;	tdcs	v3sc_t	v3sc_c;
	tydf	::llc::v3<uc_t>	v3uc_t;	tdcs	v3uc_t	v3uc_c;
	tydf	::llc::v3<u0_t>	v3u0_t;	tdcs	v3u0_t	v3u0_c;
	tydf	::llc::v3<u1_t>	v3u1_t;	tdcs	v3u1_t	v3u1_c;
	tydf	::llc::v3<u2_t>	v3u2_t;	tdcs	v3u2_t	v3u2_c;
	tydf	::llc::v3<u3_t>	v3u3_t;	tdcs	v3u3_t	v3u3_c;
	tydf	::llc::v3<s0_t>	v3s0_t;	tdcs	v3s0_t	v3s0_c;
	tydf	::llc::v3<s1_t>	v3s1_t;	tdcs	v3s1_t	v3s1_c;
	tydf	::llc::v3<s2_t>	v3s2_t;	tdcs	v3s2_t	v3s2_c;
	tydf	::llc::v3<s3_t>	v3s3_t;	tdcs	v3s3_t	v3s3_c;
	tydf	::llc::v3<f2_t>	v3f2_t;	tdcs	v3f2_t	v3f2_c;
	tydf	::llc::v3<f3_t>	v3f3_t;	tdcs	v3f3_t	v3f3_c;

	tydf	::llc::vc3<sc_t>	vc3sc_t;	tdcs	vc3sc_t	vc3sc_c;
	tydf	::llc::vc3<uc_t>	vc3uc_t;	tdcs	vc3uc_t	vc3uc_c;
	tydf	::llc::vc3<u0_t>	vc3u0_t;	tdcs	vc3u0_t	vc3u0_c;
	tydf	::llc::vc3<u1_t>	vc3u1_t;	tdcs	vc3u1_t	vc3u1_c;
	tydf	::llc::vc3<u2_t>	vc3u2_t;	tdcs	vc3u2_t	vc3u2_c;
	tydf	::llc::vc3<u3_t>	vc3u3_t;	tdcs	vc3u3_t	vc3u3_c;
	tydf	::llc::vc3<s0_t>	vc3s0_t;	tdcs	vc3s0_t	vc3s0_c;
	tydf	::llc::vc3<s1_t>	vc3s1_t;	tdcs	vc3s1_t	vc3s1_c;
	tydf	::llc::vc3<s2_t>	vc3s2_t;	tdcs	vc3s2_t	vc3s2_c;
	tydf	::llc::vc3<s3_t>	vc3s3_t;	tdcs	vc3s3_t	vc3s3_c;
	tydf	::llc::vc3<f2_t>	vc3f2_t;	tdcs	vc3f2_t	vc3f2_c;
	tydf	::llc::vc3<f3_t>	vc3f3_t;	tdcs	vc3f3_t	vc3f3_c;
} // namespace

#endif // LLC_VIEW_N3_2973498273423617
