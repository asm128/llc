#include "llc_debug.h"

#ifdef LLC_WINDOWS
#	include <cstdint>
#else
#	if defined(LLC_ATMEL)
#		include <stddef.h>
#		include <stdint.h>
#	else
#		include <cstddef>
#		include <cstdint>
#	endif
#endif

#ifdef LLC_ATMEL
//#	include "initializer_list.h"
#else
#	include <initializer_list>
#endif

#ifndef LLC_TYPEINT_H_23627
#define LLC_TYPEINT_H_23627

namespace llc
{
	tydf bool		b8_t, b8u_t;
	tydf char		sc_t, c0s_t;
	tydf int8_t		s0_t, i0s_t;
	tydf int16_t	s1_t, i1s_t;
	tydf int32_t	s2_t, i2s_t;
	tydf int64_t	s3_t, i3s_t;
	tydf unsd char	uc_t, c0u_t;
	tydf uint8_t	u0_t, i0u_t;
	tydf uint16_t	u1_t, i1u_t;
	tydf uint32_t	u2_t, i2u_t;
	tydf uint64_t	u3_t, i3u_t;
	tydf float		f2_t, f2s_t;
	tydf double		f3_t, f3s_t;
	tydf size_t		uP_t, iPu_t;
	tydf intptr_t	sP_t, iPs_t;
	tydf void*		uV_t, iVu_t;
#define	szof(type)																\
	( (sizeof(type) > 0xFFFFU)	? ::llc::u2_t(sizeof(type) & 0xFFFFFFFFU	)	\
	: (sizeof(type) > 0xFFU)	? ::llc::u1_t(sizeof(type) & 0xFFFFU		)	\
	: ::llc::u0_t(sizeof(type) & 0xFFU)											\
	)
	tdcs	b8_t	b8_c, b8u_c;
	tdcs	uc_t	uc_c, c0u_c;
	tdcs	sc_t	sc_c, c0s_c;
	tdcs	u0_t	u0_c, i0u_c;
	tdcs	u1_t	u1_c, i1u_c;
	tdcs	u2_t	u2_c, i2u_c;
	tdcs	u3_t	u3_c, i3u_c;
	tdcs	s0_t	s0_c, i0s_c;
	tdcs	s1_t	s1_c, i1s_c;
	tdcs	s2_t	s2_c, i2s_c;
	tdcs	s3_t	s3_c, i3s_c;
	tdcs	f2_t	f2_c, f2s_c;
	tdcs	f3_t	f3_c, f3s_c;
	tdcs	uP_t	uP_c, iPu_c;

#define tplN0u		tplt<::llc::u0_t N>
#define tplN1u		tplt<::llc::u1_t N>
#define tplN2u		tplt<::llc::u2_t N>
#define tplN3u		tplt<::llc::u3_t N>
#define tplNPu		tplt<::llc::uP_t N>
#define tpl_nu0		tplt<::llc::u0_t _nu0>
#define tpl_nu1		tplt<::llc::u1_t _nu1>
#define tpl_nu2		tplt<::llc::u2_t _nu2>
#define tpl_nu3		tplt<::llc::u3_t _nu3>
#define tpl_nuP		tplt<::llc::uP_t _nuP>
#define tplTIn		tplt<tpnm TIn>
#define tplTOut		tplt<tpnm TOut>
#define tplTN2		tplt<tpnm T		, ::llc::u2_t N>
#define tplTOutN2	tplt<tpnm TOut	, ::llc::u2_t N>
#define tplTInN2	tplt<tpnm TIn	, ::llc::u2_t N>
#define tpl_t_nu2	tplt<tpnm _t	, ::llc::u2_t _nu2>
#define tplN2ustct	tplN2u	stct
#define tplN2usttc	tplN2u	sttc
#define tplN2uinln	tplN2u	inln
#define tplN2ustin	tplN2u	stin
#define tplN2uusng	tplN2u	usng
#define tplN2undsx	tplN2u	ndsx
#define tplN2usinx	tplN2u	sinx
#define tplN2ustxp	tplN2u	stxp
#define tplN2unsix	tplN2u	nsix
#define tplTN2stct	tplTN2	stct
#define tplTN2sttc	tplTN2	sttc
#define tplTN2inln	tplTN2	inln
#define tplTN2usng	tplTN2	usng
#define tplTN2ndsx	tplTN2	ndsx
#define tplTN2sinx	tplTN2	sinx
#define tplTN2stxp	tplTN2	stxp
#define tplTN2nsix	tplTN2	nsix
#if defined(LLC_ANDROID) || defined(LLC_CLANG)
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wtautological-constant-out-of-range-compare"
#endif
	// -- Returns 0 on little-endian machines
	ndsi	int		test_endianness		()	nxpt	{ stxp u1_c test = 0xFF00U; rtrn (0xFFU == ((sc_c*)&test)[0]) ? 1 : 0; }
#if defined(LLC_ANDROID) || defined(LLC_CLANG)
#	pragma clang diagnostic pop
#endif
	stxp	b8_c	flse			= false;
	stxp	u0_c	BYTE_SIZE		= 8;
	tydf	intptr_t	refcount_t;
	tdcs	refcount_t	refcount_c;
	tdvl	refcount_t	refcount_vl;

	tpl_tstct	rm_cnst				{ usng T = _t; };
	tpl_tstct	rm_cnst<cnst _t>	{ usng T = _t; };
	tpl_tstct	rm_vltl				{ usng T = _t; };
	tpl_tstct	rm_vltl<vltl _t>	{ usng T = _t; };
	tpl_tstct	rm_csvl				{ usng T = _t; };
	tpl_tstct	rm_csvl<vltl _t>	{ usng T = _t; };
	tpl_tstct	rm_csvl<cnst _t>	{ usng T = _t; };
	tpl_tstct	rm_csvl<csvl _t>	{ usng T = _t; };
	tpl_tusng	rm_cnst_t			= tpnm rm_cnst<_t>::T;
	tpl_tusng	rm_vltl_t			= tpnm rm_vltl<_t>::T;
	tpl_tusng	rm_csvl_t			= tpnm rm_csvl<_t>::T;
#define llc_rmcnst(type)	tpnm ::llc::rm_cnst_t<type>
#define llc_rmvltl(type)	tpnm ::llc::rm_vltl_t<type>
#define llc_rmcsvl(type)	tpnm ::llc::rm_csvl_t<type>

#define tydfTCnst(type)		tdcs		llc_rmcnst(type)	TCnst
#define tdfTTCnst(type)		tdfT(type);	tydfTCnst(type)

	tplTOutTIn	sinx	TOut	dim2		(TIn value)				nxpt	{ rtrn {value, value}; }
	tplTOutTIn	sinx	TOut	dim3		(TIn value)				nxpt	{ rtrn {value, value, value}; }
	tplT		nsix	u0_t	byte_at		(T value, u0_t index)	nxpt	{ rtrn u0_t((value & (T(0xFFU) << (8 * index))) >> (8 * index)); }
	tplT		nsix	T		byte_to		(u0_t byte, u0_t index)	nxpt	{ rtrn T(byte) << (u2_t(8U) * index); }
	tplT		ndsc	T&		global		()						nxpt	{ sttc T instance = {}; rtrn instance; }

	tpl_tstct srcdst { tdfTTCnst(_t); T src, dst; }; 
	tpl_tstct dstsrc { tdfTTCnst(_t); T dst, src; };
#define LLC_USING_TYPEINT()				\
	using	::llc::b8_t, ::llc::b8_c, ::llc::b8u_t, ::llc::b8u_c	\
		,	::llc::uc_t, ::llc::uc_c, ::llc::c0u_t, ::llc::c0u_c	\
		,	::llc::sc_t, ::llc::sc_c, ::llc::c0s_t, ::llc::c0s_c	\
		,	::llc::u0_t, ::llc::u0_c, ::llc::i0u_t, ::llc::i0u_c	\
		,	::llc::u1_t, ::llc::u1_c, ::llc::i1u_t, ::llc::i1u_c	\
		,	::llc::u2_t, ::llc::u2_c, ::llc::i2u_t, ::llc::i2u_c	\
		,	::llc::u3_t, ::llc::u3_c, ::llc::i3u_t, ::llc::i3u_c	\
		,	::llc::s0_t, ::llc::s0_c, ::llc::i0s_t, ::llc::i0s_c	\
		,	::llc::s1_t, ::llc::s1_c, ::llc::i1s_t, ::llc::i1s_c	\
		,	::llc::s2_t, ::llc::s2_c, ::llc::i2s_t, ::llc::i2s_c	\
		,	::llc::s3_t, ::llc::s3_c, ::llc::i3s_t, ::llc::i3s_c	\
		,	::llc::f2_t, ::llc::f2_c, ::llc::f2s_t, ::llc::f2s_c	\
		,	::llc::f3_t, ::llc::f3_c, ::llc::f3s_t, ::llc::f3s_c	\
		,	::llc::uP_t, ::llc::uP_c

} // namespace

#if !defined(LLC_ANDROID) && !defined(LLC_ESP32) && !defined(LLC_ARDUINO)
#	define LLC_DEFAULT_OPERATOR_I(rightOperand, statement)	inln	bool	oper==(cnst rightOperand & other) csnx { rtrn statement; }
#	define LLC_DEFAULT_OPERATOR_C(rightOperand, statement)	cxpr	bool	oper==(cnst rightOperand & other) csnx { rtrn statement; }
#	define LLC_DEFAULT_OPERATOR_IC(rightOperand, statement)	inxp	bool	oper==(cnst rightOperand & other) csnx { rtrn statement; }
#	define LLC_DEFAULT_OPERATOR_D(rightOperand, statement)	bool			oper==(cnst rightOperand & other) csnx { rtrn statement; }
#else
#	define LLC_DEFAULT_OPERATOR_I(rightOperand, statement)	inln	bool	oper==(cnst rightOperand & other) csnx { rtrn statement; }	inln	bool	oper!=(cnst rightOperand & other) csnx { rtrn !oper==(other); }	// I had to add this because Clang coming with vs doesn't support C++20
#	define LLC_DEFAULT_OPERATOR_C(rightOperand, statement)	cxpr	bool	oper==(cnst rightOperand & other) csnx { rtrn statement; }	cxpr	bool	oper!=(cnst rightOperand & other) csnx { rtrn !oper==(other); }	// I had to add this because Clang coming with vs doesn't support C++20
#	define LLC_DEFAULT_OPERATOR_IC(rightOperand, statement)	inxp	bool	oper==(cnst rightOperand & other) csnx { rtrn statement; }	inxp	bool	oper!=(cnst rightOperand & other) csnx { rtrn !oper==(other); }	// I had to add this because Clang coming with vs doesn't support C++20
#	define LLC_DEFAULT_OPERATOR_D(rightOperand, statement)	bool			oper==(cnst rightOperand & other) csnx { rtrn statement; }			bool	oper!=(cnst rightOperand & other) csnx { rtrn !oper==(other); }	// I had to add this because Clang coming with vs doesn't support C++20
#endif // LLC_ANDROID

#define LLC_DEFAULT_OPERATOR LLC_DEFAULT_OPERATOR_IC

#ifdef LLC_ATMEL
#	define LLC_FMT_F2		"f"
#	define LLC_FMT_F3		"f"
#	define LLC_FMT_S2		"li"
#	define LLC_FMT_U2		"lu"
#	define LLC_FMT_S3		"lli"
#	define LLC_FMT_U3		"llu"
#elif defined(LLC_WINDOWS) || (defined(LLC_ARDUINO) && (defined(LLC_ESP8266) || defined(LLC_ESP32)))
#	define LLC_FMT_F2		"f"
#	define LLC_FMT_F3		"f"
#	define LLC_FMT_S2		"i"
#	define LLC_FMT_U2		"u"
#	define LLC_FMT_S3		"lli"
#	define LLC_FMT_U3		"llu"
#else
#	define LLC_FMT_F2		"f"
#	define LLC_FMT_F3		"f"
#	define LLC_FMT_S2		"li"
#	define LLC_FMT_U2		"lu"
#	define LLC_FMT_S3		"lli"
#	define LLC_FMT_U3		"llu"
#endif
#define LLC_FMT_SC		"c"
#define LLC_FMT_S0		"i"
#define LLC_FMT_U0		"u"
#define LLC_FMT_S1		"i"
#define LLC_FMT_U1		"u"
//
#define LLC_FMT_GT_F2	"%" LLC_FMT_F2 " > %" LLC_FMT_F2
#define LLC_FMT_GT_F3	"%" LLC_FMT_F3 " > %" LLC_FMT_F3
#define LLC_FMT_GT_S0	"%" LLC_FMT_S0 " > %" LLC_FMT_S0
#define LLC_FMT_GT_S1	"%" LLC_FMT_S1 " > %" LLC_FMT_S1
#define LLC_FMT_GT_S2	"%" LLC_FMT_S2 " > %" LLC_FMT_S2
#define LLC_FMT_GT_S3	"%" LLC_FMT_S3 " > %" LLC_FMT_S3
#define LLC_FMT_GT_U0	"%" LLC_FMT_U0 " > %" LLC_FMT_U0
#define LLC_FMT_GT_U1	"%" LLC_FMT_U1 " > %" LLC_FMT_U1
#define LLC_FMT_GT_U2	"%" LLC_FMT_U2 " > %" LLC_FMT_U2
#define LLC_FMT_GT_U3	"%" LLC_FMT_U3 " > %" LLC_FMT_U3
//
#define LLC_FMT_LT_F2	"%" LLC_FMT_F2 " < %" LLC_FMT_F2
#define LLC_FMT_LT_F3	"%" LLC_FMT_F3 " < %" LLC_FMT_F3
#define LLC_FMT_LT_S2	"%" LLC_FMT_S2 " < %" LLC_FMT_S2
#define LLC_FMT_LT_U2	"%" LLC_FMT_U2 " < %" LLC_FMT_U2
#define LLC_FMT_LT_S3	"%" LLC_FMT_S3 " < %" LLC_FMT_S3
#define LLC_FMT_LT_U3	"%" LLC_FMT_U3 " < %" LLC_FMT_U3
//
#define LLC_FMT_EQ_F2	"%" LLC_FMT_F2 " == %" LLC_FMT_F2
#define LLC_FMT_EQ_F3	"%" LLC_FMT_F3 " == %" LLC_FMT_F3
#define LLC_FMT_EQ_S2	"%" LLC_FMT_S2 " == %" LLC_FMT_S2
#define LLC_FMT_EQ_U2	"%" LLC_FMT_U2 " == %" LLC_FMT_U2
#define LLC_FMT_EQ_S3	"%" LLC_FMT_S3 " == %" LLC_FMT_S3
#define LLC_FMT_EQ_U3	"%" LLC_FMT_U3 " == %" LLC_FMT_U3
//
#define LLC_FMT_GE_F2	"%" LLC_FMT_F2 " >= %" LLC_FMT_F2
#define LLC_FMT_GE_F3	"%" LLC_FMT_F3 " >= %" LLC_FMT_F3
#define LLC_FMT_GE_S2	"%" LLC_FMT_S2 " >= %" LLC_FMT_S2
#define LLC_FMT_GE_U2	"%" LLC_FMT_U2 " >= %" LLC_FMT_U2
#define LLC_FMT_GE_S3	"%" LLC_FMT_S3 " >= %" LLC_FMT_S3
#define LLC_FMT_GE_U3	"%" LLC_FMT_U3 " >= %" LLC_FMT_U3
//
#define LLC_FMT_LE_F2	"%" LLC_FMT_F2 " <= %" LLC_FMT_F2
#define LLC_FMT_LE_F3	"%" LLC_FMT_F3 " <= %" LLC_FMT_F3
#define LLC_FMT_LE_S2	"%" LLC_FMT_S2 " <= %" LLC_FMT_S2
#define LLC_FMT_LE_U2	"%" LLC_FMT_U2 " <= %" LLC_FMT_U2
#define LLC_FMT_LE_S3	"%" LLC_FMT_S3 " <= %" LLC_FMT_S3
#define LLC_FMT_LE_U3	"%" LLC_FMT_U3 " <= %" LLC_FMT_U3
//
#define LLC_FMT_NE_F2	"%" LLC_FMT_F2 " != %" LLC_FMT_F2
#define LLC_FMT_NE_F3	"%" LLC_FMT_F3 " != %" LLC_FMT_F3
#define LLC_FMT_NE_S2	"%" LLC_FMT_S2 " != %" LLC_FMT_S2
#define LLC_FMT_NE_U2	"%" LLC_FMT_U2 " != %" LLC_FMT_U2
#define LLC_FMT_NE_S3	"%" LLC_FMT_S3 " != %" LLC_FMT_S3
#define LLC_FMT_NE_U3	"%" LLC_FMT_U3 " != %" LLC_FMT_U3

namespace llc
{
		enum OPCODE_SET : u0_t
		{ OPCODE_SET_UNKNOWN
		, OPCODE_SET_ARM32
		, OPCODE_SET_ARM64
		, OPCODE_SET_AVR
		, OPCODE_SET_PIC16F
		, OPCODE_SET_PIC18F
		, OPCODE_SET_STM32
		, OPCODE_SET_X86
		, OPCODE_SET_X86_64
		, OPCODE_SET_XTENSA_LX6_SINGLE
		, OPCODE_SET_XTENSA_LX6_DUAL
		, OPCODE_SET_XTENSA_LX7
		, OPCODE_SET_CUSTOM		 = 0x40U
		};
	enum OPCODE_EXT : u0_t
		{ OPCODE_EXT_NONE
		, OPCODE_EXT_AVX
		, OPCODE_EXT_MMX
		, OPCODE_EXT_SSE
		, OPCODE_EXT_SSE2
		, OPCODE_EXT_SSE3
		, OPCODE_EXT_SSE4
		, OPCODE_EXT_CUSTOM		 = 0x40U
		};
	enum DEVICE_TYPE : u0_t
		{ DEVICE_TYPE_UNKNOWN
		, DEVICE_TYPE_ANDROID
		, DEVICE_TYPE_IPHONE
		, DEVICE_TYPE_PC
		, DEVICE_TYPE_RASPBERRY_PI_3
		, DEVICE_TYPE_RASPBERRY_PI_4
		, DEVICE_TYPE_MAC
		, DEVICE_TYPE_CUSTOM		= 0x40U
		};
	enum OS_FAMILY : u0_t
		{ OS_FAMILY_UNKNOWN
		, OS_FAMILY_ANDROID
		, OS_FAMILY_ARDUINO
		, OS_FAMILY_FREERTOS
		, OS_FAMILY_IOS
		, OS_FAMILY_LINUX
		, OS_FAMILY_MACOS
		, OS_FAMILY_NO_OS
		, OS_FAMILY_PI
		, OS_FAMILY_RTOS
		, OS_FAMILY_WINDOWS
		, OS_FAMILY_CUSTOM		  = 0x40U
		};
#define GDEFINE_ENUM_NAMEP(TEnum)	\
	nsix	sc_c*		get_enum_namep	(cnst TEnum&)	nxpt	{ rtrn #TEnum; } \
	ndsc	sc_c*		get_value_namep (TEnum value)	nxpt;
	GDEFINE_ENUM_NAMEP(DEVICE_TYPE  );
	GDEFINE_ENUM_NAMEP(OPCODE_SET   );
	GDEFINE_ENUM_NAMEP(OPCODE_EXT   );
	GDEFINE_ENUM_NAMEP(OS_FAMILY	);
#define llc_enum_value_log(printf_fn, enumVal)	printf_fn("'%s':(0x%X)(%" LLC_FMT_S2 ")(%c)'%s'", get_enum_namep LLCREP3(enumVal) ? char(enumVal) : '?', get_value_namep(enumVal))
} // namespace




#endif // LLC_TYPEINT_H_23627
