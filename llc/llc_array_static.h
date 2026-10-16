#include "llc_cpow.h"
#include "llc_view.h"

#ifndef LLC_ARRAY_STATIC_H_23627
#define LLC_ARRAY_STATIC_H_23627

namespace llc
{
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
		inln	view<u0_t>	u8				()						nxpt	{ rtrn view<T		>(Storage, N).u8	(); }
		inxp	view<u0_c>	u8				()						csnx	{ rtrn view<TCnst	>(Storage, N).u8	(); }
		inxp	view<u0_c>	cu8				()						csnx	{ rtrn view<TCnst	>(Storage, N).cu8	(); }
		inln	view<sc_t>	c				()						nxpt	{ rtrn view<T		>(Storage, N).c	(); }
		inxp	view<sc_c>	cc				()						csnx	{ rtrn view<TCnst	>(Storage, N).cc	(); }
		inxp	u2_t		size			()						csnx	{ rtrn N; }
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
	tplTN2usng	asttc		= array_static<T, N>;
	tplTN2usng	astatic		= array_static<T, N>;

	tplN2uusng	astvcs		= astatic<vcst_t	, N>;	tplN2uusng astaticvcs	= astvcs	<N>;
	tplN2uusng	astvs		= astatic<vs		, N>;	tplN2uusng astaticvs	= astvs	<N>;

	tplN2uusng	astb8_t		= astatic<b8_t, N>; tplN2uusng	astb8_c	= const astb8_t<N>; 
	tplN2uusng	astuc_t		= astatic<uc_t, N>; tplN2uusng	astuc_c	= const astuc_t<N>; tplN2uusng astuc 	= astatic<uc_t, N>; tplN2uusng astaticuc 	= astuc 	<N>; tplN2uusng astuchar	= astatic<uc_t, N>;
	tplN2uusng	astsc_t		= astatic<sc_t, N>; tplN2uusng	astsc_c	= const astsc_t<N>; tplN2uusng astc		= astatic<sc_t, N>; tplN2uusng astaticc		= astc	<N>; tplN2uusng astchar		= astatic<sc_t, N>;
	tplN2uusng	astu0_t		= astatic<u0_t, N>; tplN2uusng	astu0_c	= const astu0_t<N>; tplN2uusng astu0_t= astatic<u0_t, N>; tplN2uusng astaticu8 	= astu0_t<N>;
	tplN2uusng	astu1_t		= astatic<u1_t, N>; tplN2uusng	astu1_c	= const astu1_t<N>; tplN2uusng astu1_t= astatic<u1_t, N>; tplN2uusng astaticu16	= astu1_t<N>;
	tplN2uusng	astu2_t		= astatic<u2_t, N>; tplN2uusng	astu2_c	= const astu2_t<N>; tplN2uusng astu2_t= astatic<u2_t, N>; tplN2uusng astaticu32	= astu2_t<N>;
	tplN2uusng	astu3_t		= astatic<u3_t, N>; tplN2uusng	astu3_c	= const astu3_t<N>; tplN2uusng astu3_t= astatic<u3_t, N>; tplN2uusng astaticu64	= astu3_t<N>;
	tplN2uusng	asts0_t		= astatic<s0_t, N>; tplN2uusng	asts0_c	= const asts0_t<N>; tplN2uusng asts0_t= astatic<s0_t, N>; tplN2uusng astatici8 	= asts0_t<N>;
	tplN2uusng	asts1_t		= astatic<s1_t, N>; tplN2uusng	asts1_c	= const asts1_t<N>; tplN2uusng asts1_t= astatic<s1_t, N>; tplN2uusng astatici16	= asts1_t<N>;
	tplN2uusng	asts2_t		= astatic<s2_t, N>; tplN2uusng	asts2_c	= const asts2_t<N>; tplN2uusng asts2_t= astatic<s2_t, N>; tplN2uusng astatici32	= asts2_t<N>;
	tplN2uusng	asts3_t		= astatic<s3_t, N>; tplN2uusng	asts3_c	= const asts3_t<N>; tplN2uusng asts3_t= astatic<s3_t, N>; tplN2uusng astatici64	= asts3_t<N>;
	tplN2uusng	astf2_t		= astatic<f2_t, N>; tplN2uusng	astf2_c	= const astf2_t<N>; tplN2uusng astf2_t= astatic<f2_t, N>; tplN2uusng astaticf32	= astf2_t<N>;
	tplN2uusng	astf3_t		= astatic<f3_t, N>; tplN2uusng	astf3_c	= const astf3_t<N>; tplN2uusng astf3_t= astatic<f3_t, N>; tplN2uusng astaticf64	= astf3_t<N>;

	tplN2uusng	astvcuc 	= astatic<vcuc 	, N>;	tplN2uusng astaticvcuc 		= astvcuc <N>;
	tplN2uusng	astvcc		= astvcs<N>;			tplN2uusng astaticvcc		= astvcc<N>;
	tplN2uusng	astvcu0_t 	= astatic<vcu0_t, N>;	tplN2uusng astaticvcu8 		= astvcu0_t<N>;
	tplN2uusng	astvcu1_t	= astatic<vcu1_t, N>;	tplN2uusng astaticvcu16		= astvcu1_t<N>;
	tplN2uusng	astvcu2_t	= astatic<vcu2_t, N>;	tplN2uusng astaticvcu2_t	= astvcu2_t<N>;
	tplN2uusng	astvcu3_t	= astatic<vcu3_t, N>;	tplN2uusng astaticvcu64		= astvcu3_t<N>;
	tplN2uusng	astvcs0_t	= astatic<vcs0_t, N>;	tplN2uusng astaticvcs0_t	= astvcs0_t<N>;
	tplN2uusng	astvcs1_t	= astatic<vcs1_t, N>;	tplN2uusng astaticvcs1_t	= astvcs1_t<N>;
	tplN2uusng	astvcs2_t	= astatic<vcs2_t, N>;	tplN2uusng astaticvcs2_t	= astvcs2_t<N>;
	tplN2uusng	astvcs3_t	= astatic<vcs3_t, N>;	tplN2uusng astaticvcs3_t	= astvcs3_t<N>;
	tplN2uusng	astvcf2_t	= astatic<vcf2_t, N>;	tplN2uusng astaticvcf2_t	= astvcf2_t<N>;
	tplN2uusng	astvcf3_t	= astatic<vcf3_t, N>;	tplN2uusng astaticvcf3_t	= astvcf3_t<N>;
	
	tplTN2sinx	u2_t	size		(astatic<T, N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N);				}
	tplTN2sinx	u2_t	byte_count	(astatic<T, N> /*viewToTest*/)	nxpt	{ rtrn u2_t(sizeof(T) * N);	}

	// Is this really worth it?
	tplN2usinx	u2_t	size(astc	<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	//tplN2usinx	u2_t	size(astuc 	<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(astu0_t<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(astu1_t<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(astu2_t<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(astu3_t<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(asts1_t<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(asts2_t<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(asts3_t<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(astf2_t<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	tplN2usinx	u2_t	size(astf3_t<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }

	//tplN2usinx	u2_t	size(astvcc	 <N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	////tplN2usinx	u2_t	size(astvcuc <N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	//tplN2usinx	u2_t	size(astu0_t<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	//tplN2usinx	u2_t	size(astu1_t<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	//tplN2usinx	u2_t	size(astu2_t<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	//tplN2usinx	u2_t	size(astu3_t<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	//tplN2usinx	u2_t	size(asts1_t<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	//tplN2usinx	u2_t	size(asts2_t<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	//tplN2usinx	u2_t	size(asts3_t<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	//tplN2usinx	u2_t	size(astf2_t<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }
	//tplN2usinx	u2_t	size(astf3_t<N> /*viewToTest*/)	nxpt	{ rtrn u2_t(N); }

	tplTN2sinx	err_t	find		(cnst T & element, cnst astatic<T, N> & target, u2_t offset = 0)		{ rtrn find(element, view<cnst T>{target.begin(), target.size()}, offset); }
	tplN2ustin	vs		str			(astchar<N> & arg)		{ rtrn arg.Storage; }
	tplN2ustin	vcst_t	str			(cnst astchar<N> & arg)	{ rtrn arg.Storage; }
	stin astchar< 7>	str			(u1_t arg)	{ astchar< 7> dest = {}; sprintf_s(dest.Storage, "%u" , arg); rtrn dest; }
	stin astchar<12>	str			(u2_t arg)	{ astchar<12> dest = {}; sprintf_s(dest.Storage, "%" LLC_FMT_U2, arg); rtrn dest; }
	stin astchar<22>	str			(u3_t arg)	{ astchar<22> dest = {}; sprintf_s(dest.Storage, "%" LLC_FMT_U3, arg); rtrn dest; }
	stin astchar< 8>	str			(s1_t arg)	{ astchar< 8> dest = {}; sprintf_s(dest.Storage, "%i", arg); rtrn dest; }
	stin astchar<13>	str			(s2_t arg)	{ astchar<13> dest = {}; sprintf_s(dest.Storage, "%" LLC_FMT_S2, arg); rtrn dest; }
	stin astchar<22>	str			(s3_t arg)	{ astchar<22> dest = {}; sprintf_s(dest.Storage, "%" LLC_FMT_S3, arg); rtrn dest; }
	stin astchar<64>	str			(f2_t arg)	{ astchar<64> dest = {}; sprintf_s(dest.Storage, "%f", arg); rtrn dest; }
	stin astchar<384>	str			(f3_t arg)	{ astchar<384> dest = {}; sprintf_s(dest.Storage, "%f", arg); rtrn dest; }
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
