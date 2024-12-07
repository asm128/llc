#include "llc_cpow.h"
#include "llc_view.h"

#ifndef LLC_ARRAY_STATIC_H_23627
#define LLC_ARRAY_STATIC_H_23627

namespace llc
{
#pragma pack(push, 1)
	tpl_t_nu2 stct array_static	{
		stxp		u2_c				N 	= 	_nu2;
		tdfTTCnst(_t);
		tydf		array_static<T, N>	TAStatic;
		tydf		view<TCnst>			VCnst;
		tydf		view<T>				V;
		T			Storage	[N]			;

		inln	oper		view<T>			()								{ rtrn {Storage, N}; }
		inxp	oper		view<TCnst>		()						csnx	{ rtrn {Storage, N}; }

		inln	TCnst&		oper[]			(u2_t index)			cnst	{ gthrow_if(index >= N, LLC_FMT_GE_U2, index, N); rtrn Storage[index]; }
		inln	T&			oper[]			(u2_t index)					{ gthrow_if(index >= N, LLC_FMT_GE_U2, index, N); rtrn Storage[index]; }
		LLC_DEFAULT_OPERATOR(TAStatic
			, (this->size() != other.size()) ? false
			: (this->begin() == other.begin()) ? true
			: ::llc::equal(other.begin(), this->begin(), this->size())
			);
		inxp	u2_t		byte_count		()						csnx	{ rtrn N * szof(T);	}
		inxp	u2_t		bit_count		()						csnx	{ rtrn byte_count() * 8U;	}
		inln	view<u0_t>	u8				()						nxpt	{ rtrn view<T		>(Storage, byte_count()).u8		(); }	
		inxp	view<u0_c>	u8				()						csnx	{ rtrn view<T		>(Storage, byte_count()).u8		(); }	
		inxp	view<u0_c>	cu8				()						csnx	{ rtrn view<TCnst	>(Storage, byte_count()).cu8	(); }	
		inln	view<sc_t>	c				()						nxpt	{ rtrn view<T		>(Storage, byte_count()).c		(); }
		inxp	view<sc_c>	cc				()						csnx	{ rtrn view<TCnst	>(Storage, byte_count()).cc		(); }	
		inxp	u2_c&		size			()						csnx	{ rtrn N; }
		inln	T*			begin			()						nxpt	{ rtrn Storage;			}
		inln	T*			end				()						nxpt	{ rtrn Storage + N;	}
		inxp	TCnst*		begin			()						csnx	{ rtrn Storage;			}
		inxp	TCnst*		end				()						csnx	{ rtrn Storage + N;	}

		err_t				slice			(V & out, u2_t offset, u2_t count = (u2_t)-1)				{
			ree_if(offset > N, LLC_FMT_GT_U2, offset, N);
			u2_c					newSize			= N - offset;
			if(count != (u2_t)-1)
				ree_if(count > newSize, LLC_FMT_GT_U2, count, newSize);
			out						= {&Storage[offset], ::llc::min(newSize, count)};
			rtrn out.size();
		}
		err_t				slice			(VCnst & out, u2_t offset, u2_t count = (u2_t)-1)	cnst	{
			ree_if(offset > N, LLC_FMT_GT_U2, offset, N);
			u2_c					newSize			= N - offset;
			if(count != (u2_t)-1)
				ree_if(count > newSize, LLC_FMT_GT_U2, count, newSize);
			out						= {&Storage[offset], ::llc::min(newSize, count)};
			rtrn out.size();
		}
	};
#pragma pack(pop)
	tplTN2usng	asttc		= ::llc::array_static<T, N>;
	tplTN2usng	astatic		= ::llc::array_static<T, N>;

	tplN2uusng	astvcs		= ::llc::astatic<::llc::vcst_t	, N>;	tplN2uusng astaticvcs	= ::llc::astvcs	<N>;
	tplN2uusng	astvs		= ::llc::astatic<::llc::vs		, N>;	tplN2uusng astaticvs	= ::llc::astvs	<N>;

	tplN2uusng	astb8_t		= ::llc::astatic<b8_t, N>; tplN2uusng	astb8_c	= const ::llc::astb8_t<N>; 
	tplN2uusng	astuc_t		= ::llc::astatic<uc_t, N>; tplN2uusng	astuc_c	= const ::llc::astuc_t<N>; tplN2uusng astuc 	= ::llc::astatic<uc_t, N>; tplN2uusng astaticuc 	= ::llc::astuc 	<N>; tplN2uusng astuchar	= ::llc::astatic<uc_t, N>;
	tplN2uusng	astsc_t		= ::llc::astatic<sc_t, N>; tplN2uusng	astsc_c	= const ::llc::astsc_t<N>; tplN2uusng astc		= ::llc::astatic<sc_t, N>; tplN2uusng astaticc		= ::llc::astc	<N>; tplN2uusng astchar		= ::llc::astatic<sc_t, N>;
	tplN2uusng	astu0_t		= ::llc::astatic<u0_t, N>; tplN2uusng	astu0_c	= const ::llc::astu0_t<N>; tplN2uusng astu8 	= ::llc::astatic<u0_t, N>; tplN2uusng astaticu8 	= ::llc::astu8 	<N>;
	tplN2uusng	astu1_t		= ::llc::astatic<u1_t, N>; tplN2uusng	astu1_c	= const ::llc::astu1_t<N>; tplN2uusng astu16	= ::llc::astatic<u1_t, N>; tplN2uusng astaticu16	= ::llc::astu16	<N>;
	tplN2uusng	astu2_t		= ::llc::astatic<u2_t, N>; tplN2uusng	astu2_c	= const ::llc::astu2_t<N>; tplN2uusng astu32	= ::llc::astatic<u2_t, N>; tplN2uusng astaticu32	= ::llc::astu32	<N>;
	tplN2uusng	astu3_t		= ::llc::astatic<u3_t, N>; tplN2uusng	astu3_c	= const ::llc::astu3_t<N>; tplN2uusng astu64	= ::llc::astatic<u3_t, N>; tplN2uusng astaticu64	= ::llc::astu64	<N>;
	tplN2uusng	asts0_t		= ::llc::astatic<s0_t, N>; tplN2uusng	asts0_c	= const ::llc::asts0_t<N>; tplN2uusng asti8 	= ::llc::astatic<s0_t, N>; tplN2uusng astatici8 	= ::llc::asti8 	<N>;
	tplN2uusng	asts1_t		= ::llc::astatic<s1_t, N>; tplN2uusng	asts1_c	= const ::llc::asts1_t<N>; tplN2uusng asti16	= ::llc::astatic<s1_t, N>; tplN2uusng astatici16	= ::llc::asti16	<N>;
	tplN2uusng	asts2_t		= ::llc::astatic<s2_t, N>; tplN2uusng	asts2_c	= const ::llc::asts2_t<N>; tplN2uusng asti32	= ::llc::astatic<s2_t, N>; tplN2uusng astatici32	= ::llc::asti32	<N>;
	tplN2uusng	asts3_t		= ::llc::astatic<s3_t, N>; tplN2uusng	asts3_c	= const ::llc::asts3_t<N>; tplN2uusng asti64	= ::llc::astatic<s3_t, N>; tplN2uusng astatici64	= ::llc::asti64	<N>;
	tplN2uusng	astf2_t		= ::llc::astatic<f2_t, N>; tplN2uusng	astf2_c	= const ::llc::astf2_t<N>; tplN2uusng astf32	= ::llc::astatic<f2_t, N>; tplN2uusng astaticf32	= ::llc::astf32	<N>;
	tplN2uusng	astf3_t		= ::llc::astatic<f3_t, N>; tplN2uusng	astf3_c	= const ::llc::astf3_t<N>; tplN2uusng astf64	= ::llc::astatic<f3_t, N>; tplN2uusng astaticf64	= ::llc::astf64	<N>;

	tplN2uusng	astvcuc 	= ::llc::astatic<::llc::vcuc 	, N>; tplN2uusng astaticvcuc 	= ::llc::astvcuc <N>;
	tplN2uusng	astvcc		= ::llc::astatic<::llc::vcsc_t	, N>; tplN2uusng astaticvcc		= ::llc::astvcc  <N>;
	tplN2uusng	astvcu8 	= ::llc::astatic<::llc::vcu0_t 	, N>; tplN2uusng astaticvcu8 	= ::llc::astvcu8 <N>;
	tplN2uusng	astvcu16	= ::llc::astatic<::llc::vcu16	, N>; tplN2uusng astaticvcu16	= ::llc::astvcu16<N>;
	tplN2uusng	astvcu32	= ::llc::astatic<::llc::vcu32	, N>; tplN2uusng astaticvcu32	= ::llc::astvcu32<N>;
	tplN2uusng	astvcu64	= ::llc::astatic<::llc::vcu64	, N>; tplN2uusng astaticvcu64	= ::llc::astvcu64<N>;
	tplN2uusng	astvci8 	= ::llc::astatic<::llc::vcs0_t 	, N>; tplN2uusng astaticvci8 	= ::llc::astvci8 <N>;
	tplN2uusng	astvci16	= ::llc::astatic<::llc::vci16	, N>; tplN2uusng astaticvci16	= ::llc::astvci16<N>;
	tplN2uusng	astvci32	= ::llc::astatic<::llc::vci32	, N>; tplN2uusng astaticvci32	= ::llc::astvci32<N>;
	tplN2uusng	astvci64	= ::llc::astatic<::llc::vci64	, N>; tplN2uusng astaticvci64	= ::llc::astvci64<N>;
	tplN2uusng	astvcf32	= ::llc::astatic<::llc::vcf32	, N>; tplN2uusng astaticvcf32	= ::llc::astvcf32<N>;
	tplN2uusng	astvcf64	= ::llc::astatic<::llc::vcf64	, N>; tplN2uusng astaticvcf64	= ::llc::astvcf64<N>;
	
	tplTN2sinx	u2_t	size		(::llc::astatic<T, N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N);				}
	tplTN2sinx	u2_t	byte_count	(::llc::astatic<T, N> viewToTest)		nxpt	{ rtrn u2_t(sizeof(T) * N);	}

	// Is this really worth it?
	tplN2usinx	u2_t	size(astc	<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	//tplN2usinx	u2_t	size(astuc 	<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(astu8 	<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(astu16	<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(astu32	<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(astu64	<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(asti8 	<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(asti16	<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(asti32	<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(asti64	<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(astf32	<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(astf64	<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }

	tplN2usinx	u2_t	size(astvcc	 <N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	//tplN2usinx	u2_t	size(astvcuc <N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(astvcu8 <N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(astvcu16<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(astvcu32<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(astvcu64<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(astvci8 <N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(astvci16<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(astvci32<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(astvci64<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(astvcf32<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(astvcf64<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }

	tplTN2sinx	err_t	find		(cnst T & element, cnst astatic<cnst T, N> & target, u2_t offset = 0)	{ rtrn ::llc::find(element, ::llc::view<cnst T>{target}, offset); }
	stin astchar< 7>	str			(u1_t arg)	{ astchar< 7> dest = {}; sprintf_s(dest.Storage, "%u" , arg); rtrn dest; }
	stin astchar<12>	str			(u2_t arg)	{ astchar<12> dest = {}; sprintf_s(dest.Storage, "%" LLC_FMT_U2, arg); rtrn dest; }
	stin astchar<22>	str			(u3_t arg)	{ astchar<22> dest = {}; sprintf_s(dest.Storage, "%" LLC_FMT_U3, arg); rtrn dest; }
	stin astchar< 7>	str			(s1_t arg)	{ astchar< 7> dest = {}; sprintf_s(dest.Storage, "%i", arg); rtrn dest; }
	stin astchar<12>	str			(s2_t arg)	{ astchar<12> dest = {}; sprintf_s(dest.Storage, "%" LLC_FMT_S2, arg); rtrn dest; }
	stin astchar<22>	str			(s3_t arg)	{ astchar<22> dest = {}; sprintf_s(dest.Storage, "%" LLC_FMT_U3, arg); rtrn dest; }
	stin astchar<12>	str			(f2_t arg)	{ astchar<12> dest = {}; sprintf_s(dest.Storage, "%f", arg); rtrn dest; }
	stin astchar<22>	str			(f3_t arg)	{ astchar<22> dest = {}; sprintf_s(dest.Storage, "%f", arg); rtrn dest; }
	stxp astchar<5 >	str			(u0_t arg) {
		rtrn	(arg >= 100) ?	astchar<5>{digit<2>(arg), digit<1>(arg), digit<0>(arg), 0}
			:	(arg >=  10) ?	astchar<5>{digit<1>(arg), digit<0>(arg), 0}
			:					astchar<5>{digit<0>(arg), 0}
			;
	}
	stxp astchar<5 >	str			(s0_t arg) {
		rtrn	(arg >=    0) ?	str(u0_t(arg))
			:	(arg <= -100) ?	astchar<5>{'-', digit<2>((int16_t)-arg), digit<1>((int16_t)-arg), digit<0>((int16_t)-arg)}
			:	(arg <=  -10) ?	astchar<5>{'-', digit<1>((int16_t)-arg), digit<0>((int16_t)-arg)}
			:					astchar<5>{'-', digit<0>((int16_t)-arg)}
			;
	}
} // namespace

#define LLC_USING_ASTATIC()					\
	usng	::llc::astb8_t, ::llc::astb8_c	\
		,	::llc::astuc_t, ::llc::astuc_c	\
		,	::llc::astsc_t, ::llc::astsc_c	\
		,	::llc::astu0_t, ::llc::astu0_c	\
		,	::llc::astu1_t, ::llc::astu1_c	\
		,	::llc::astu2_t, ::llc::astu2_c	\
		,	::llc::astu3_t, ::llc::astu3_c	\
		,	::llc::asts0_t, ::llc::asts0_c	\
		,	::llc::asts1_t, ::llc::asts1_c	\
		,	::llc::asts2_t, ::llc::asts2_c	\
		,	::llc::asts3_t, ::llc::asts3_c	\
		,	::llc::astf2_t, ::llc::astf2_c	\
		,	::llc::astf3_t, ::llc::astf3_c

#endif // LLC_ARRAY_STATIC_H_23627
