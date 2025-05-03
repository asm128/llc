/// Copyright 2010-2025 - ogarnd
#ifndef LLC_PLATFORM_GLOBALS_H_23627
#define LLC_PLATFORM_GLOBALS_H_23627

#if(!defined(LLC_ESPIDF)) && (defined(ESP32) || defined(ESP_PLATFORM) || defined(ARDUINO_ARCH_ESP32))
#	define LLC_ESPIDF
#   define LLC_FREERTOS
#   define LLC_ESP32
#endif

#if (!defined(LLC_ARDUINO))	&&	(defined(ARDUINO) || defined(__ARDUINO__) || defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266))
#	define LLC_ARDUINO
#endif

#if (!defined(LLC_ATMEL))	&&	defined(__ATMEGA__)
#	define LLC_ATMEL
#elif (!defined(LLC_ESP32))		&&	(defined(ESP32)		||	defined(ARDUINO_ARCH_ESP32))
#	define LLC_ESP32
#elif (!defined(LLC_ESP8266))	&&	(defined(ESP8266)	||	defined(ARDUINO_ARCH_ESP8266))
#	define LLC_ESP8266
#elif (!defined(LLC_ANDROID))	&&	(defined(ANDROID)	||	defined(__ANDROID__))
#	define LLC_ANDROID
#elif (!defined(LLC_LINUX))		&&	defined(__linux__)
#	define LLC_LINUX
#elif (!defined(LLC_WINDOWS))	&&	(defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64))
#	define LLC_WINDOWS
#endif

#if (!defined(LLC_ST)) && !defined(LLC_CMSIS) && !defined(LLC_ESP32) && !defined(LLC_ATMEL) && !defined(LLC_ARDUINO) && !defined(LLC_WINDOWS) && !defined(LLC_LINUX) && !defined(LLC_ANDROID)
#	error "Unsupported platform."
#	define LLC_UNKNOWN
#endif

#if (defined(DEBUG) || defined(_DEBUG)) && (!defined(LLC_DISABLE_DEBUG)) && !defined(LLC_DEBUG_ENABLED)
#	define LLC_DEBUG_ENABLED
#	ifndef USE_DEBUG_BREAK_ON_ERROR_LOG
#		define USE_DEBUG_BREAK_ON_ERROR_LOG
#	endif // USE_DEBUG_BREAK_ON_ERROR_LOG
#endif	// _DEBUG

#if (!defined(LLC_MTSUPPORT)) && !defined(LLC_DISABLE_MTSUPPORT)
#	define LLC_MTSUPPORT
#endif


#ifdef __GNUC__
#	define UNUSED __attribute__((__unused__))
#elif defined(LLC_WINDOWS)
#	ifdef _DEBUG
#		define UNUSED
#	else
#		define UNUSED
#	endif
#else
#	define UNUSED
#endif

#if defined(LLC_WINDOWS)
//#	pragma warning(disable : 4592)		// We needed this because of a bug in the compiler causing our static const variables to throw a warning as if its constructor was defined as constexpr.
//#	pragma warning(disable : 6326)		// Bogus potential comparison of a constant with another constant for static tplt functionality.
#	pragma warning(disable : 4706)		// Enable assignment within conditional expression. We do this constantly inside our macros in a completely valid way.
#endif

#ifndef LLC_WINDOWS
#	define LLC_DYNAMIC_LIBRARY_EXTENSION "os"
#else
#	define LLC_DYNAMIC_LIBRARY_EXTENSION "dll"
#	if !defined(NOMINMAX)
#		define NOMINMAX
#	endif
#	if !defined(WIN32_LEAN_AND_MEAN)
#		define WIN32_LEAN_AND_MEAN
#	endif
#endif

#if defined LLC_ANDROID
#	define LLC_DISABLE_CPP_EXCEPTIONS
#endif

// These aliases make keyword width to be consistent and makes the code less verbose.
#define dnwn			do{}while(0)
#define tydf			typedef
#define cnst			const
#define tdcs			tydf cnst
#define vltl			volatile
#define tdvl			tydf vltl
#define csvl			cnst vltl
#define tdcv			tydf csvl
#define nxpt			noexcept
#define csnx			cnst nxpt
#define sttc			static
#define stcs			sttc cnst
#define stvl			sttc vltl
#define inln			inline
#define stin			sttc inln
#define cxpr			constexpr
#define stxp			sttc cxpr
#define inxp			inln cxpr
#define sinx			stin cxpr
#define ndsc			[[nodiscard]]
#define ndcs			ndsc cnst
#define ndst			ndsc sttc
#define ndin			ndsc inln
#define ndsi			ndsc stin
#define ndxp			ndsc cxpr
#define ndsx			ndsc stxp
#define ndix			ndsc inxp
#define nsix			ndsc sinx
#define rtrn			return
#define dflt			default
#define oper			operator
#define tplt			template
#define tpnm			typename
#define sgnd			signed
#define unsd			unsigned
#define usng			using
#define stct			struct
#define clss			class
#define pblc			public
#define prtc			protected
#define prvt			private
#define lt4(lt4type)	llc::lt4type
#define usl4(type)		usng lt4(type)

#define tdfT(type)		tydf	type	T

#define tpn(type)		tpnm	type
#define tnv(type)		tpnm...	type
#define tpnT			tpn(T)
#define tnv_tArgs  	    tnv(_tArgs)
#define tpl(type)		tplt<tpn(type)>
#define tplT			tpl(T)
#define tplT2			tpl(T2)
#define tpl_t			tpl(_t)
#define tpl_t2			tpl(_t2)
#define tpl_vtArgs		tplt<tnv_tArgs>
#define tplT_vtArgs		tplt<tpnT, tnv_tArgs>
#define tplTstct		tplT	stct
#define tplTsttc		tplT	sttc
#define tplTinln		tplT	inln
#define tplTstin		tplT	stin
#define tplTusng		tplT	usng
#define tplTndsx		tplT	ndsx
#define tplTsinx		tplT	sinx
#define tplTstxp		tplT	stxp
#define tplTnsix		tplT	nsix
#define tpl_tstct		tpl_t	stct
#define tpl_tsttc		tpl_t	sttc
#define tpl_tinln		tpl_t	inln
#define tpl_tusng		tpl_t	usng
#define tpl_tndsx		tpl_t	ndsx
#define tpl_tsinx		tpl_t	sinx
#define tpl_tstxp		tpl_t	stxp
#define tpl_tnsix		tpl_t	nsix 
#define tpl_tstin		tpl_t	stin

#define tplTInTOut		tplt<tpnm TIn, tpnm TOut>
#define tplTOutTIn		tplt<tpnm TOut, tpnm TIn>

#define cxpr_if(condition)		if cxpr(condition)
#define cxpr_if_not(condition)	if cxpr(not(condition))
#define if_not(condition)		if(not(condition))

#define LLC_STRINGIFY(x)	#x
#define LLC_TOSTRING(x)		LLC_STRINGIFY(x)

#define LLCREP0(param)	(param)
#define LLCREP1(param)	LLCREP0(param), (param)
#define LLCREP2(param)	LLCREP1(param), (param)
#define LLCREP3(param)	LLCREP2(param), (param)
#define LLCREP4(param)	LLCREP3(param), (param)
#define LLCREP5(param)	LLCREP4(param), (param)
#define LLCREP6(param)	LLCREP5(param), (param)
#define LLCREP7(param)	LLCREP6(param), (param)

#ifndef	LLC_ATMEL
#	define LLC_FMT_F32	"f"
#	define LLC_FMT_F64	"f"
#	define LLC_FMT_I32	"i"
#	define LLC_FMT_U32	"u"
#	define LLC_FMT_I64	"lli"
#	define LLC_FMT_U64	"llu"
#else
#	define LLC_FMT_F32	"f"
#	define LLC_FMT_F64	"f"
#	define LLC_FMT_I32	"li"
#	define LLC_FMT_U32	"lu"
#	define LLC_FMT_I64	"lli"
#	define LLC_FMT_U64	"llu"
#endif
#define LLC_FMT_I8			"i"
#define LLC_FMT_U8			"u"
#define LLC_FMT_I16			"i"
#define LLC_FMT_U16			"u"
#define LLC_FMT_F32_GT_F32	"%"	LLC_FMT_F32	" > %"	LLC_FMT_F32
#define LLC_FMT_F64_GT_F64	"%"	LLC_FMT_F64	" > %"	LLC_FMT_F64
#define LLC_FMT_I32_GT_I32	"%"	LLC_FMT_I32	" > %"	LLC_FMT_I32
#define LLC_FMT_U32_GT_U32	"%"	LLC_FMT_U32	" > %"	LLC_FMT_U32
#define LLC_FMT_I64_GT_I64	"%"	LLC_FMT_I64	" > %"	LLC_FMT_I64
#define LLC_FMT_U64_GT_U64	"%"	LLC_FMT_U64	" > %"	LLC_FMT_U64
#define LLC_FMT_F32_LT_F32	"%"	LLC_FMT_F32	" < %"	LLC_FMT_F32
#define LLC_FMT_F64_LT_F64	"%"	LLC_FMT_F64	" < %"	LLC_FMT_F64
#define LLC_FMT_I32_LT_I32	"%"	LLC_FMT_I32	" < %"	LLC_FMT_I32
#define LLC_FMT_U32_LT_U32	"%"	LLC_FMT_U32	" < %"	LLC_FMT_U32
#define LLC_FMT_I64_LT_I64	"%"	LLC_FMT_I64	" < %"	LLC_FMT_I64
#define LLC_FMT_U64_LT_U64	"%"	LLC_FMT_U64	" < %"	LLC_FMT_U64
#define LLC_FMT_F32_EQ_F32	"%"	LLC_FMT_F32	" == %"	LLC_FMT_F32
#define LLC_FMT_F64_EQ_F64	"%"	LLC_FMT_F64	" == %"	LLC_FMT_F64
#define LLC_FMT_I32_EQ_I32	"%"	LLC_FMT_I32	" == %"	LLC_FMT_I32
#define LLC_FMT_U32_EQ_U32	"%"	LLC_FMT_U32	" == %"	LLC_FMT_U32
#define LLC_FMT_I64_EQ_I64	"%"	LLC_FMT_I64	" == %"	LLC_FMT_I64
#define LLC_FMT_U64_EQ_U64	"%"	LLC_FMT_U64	" == %"	LLC_FMT_U64
#define LLC_FMT_F32_GE_F32	"%"	LLC_FMT_F32	" >= %"	LLC_FMT_F32
#define LLC_FMT_F64_GE_F64	"%"	LLC_FMT_F64	" >= %"	LLC_FMT_F64
#define LLC_FMT_I32_GE_I32	"%"	LLC_FMT_I32	" >= %"	LLC_FMT_I32
#define LLC_FMT_U32_GE_U32	"%"	LLC_FMT_U32	" >= %"	LLC_FMT_U32
#define LLC_FMT_I64_GE_I64	"%"	LLC_FMT_I64	" >= %"	LLC_FMT_I64
#define LLC_FMT_U64_GE_U64	"%"	LLC_FMT_U64	" >= %"	LLC_FMT_U64
#define LLC_FMT_F32_LE_F32	"%"	LLC_FMT_F32	" <= %"	LLC_FMT_F32
#define LLC_FMT_F64_LE_F64	"%"	LLC_FMT_F64	" <= %"	LLC_FMT_F64
#define LLC_FMT_I32_LE_I32	"%"	LLC_FMT_I32	" <= %"	LLC_FMT_I32
#define LLC_FMT_U32_LE_U32	"%"	LLC_FMT_U32	" <= %"	LLC_FMT_U32
#define LLC_FMT_I64_LE_I64	"%"	LLC_FMT_I64	" <= %"	LLC_FMT_I64
#define LLC_FMT_U64_LE_U64	"%"	LLC_FMT_U64	" <= %"	LLC_FMT_U64
#define LLC_FMT_F32_NE_F32	"%"	LLC_FMT_F32	" != %"	LLC_FMT_F32
#define LLC_FMT_F64_NE_F64	"%"	LLC_FMT_F64	" != %"	LLC_FMT_F64
#define LLC_FMT_I32_NE_I32	"%"	LLC_FMT_I32	" != %"	LLC_FMT_I32
#define LLC_FMT_U32_NE_U32	"%"	LLC_FMT_U32	" != %"	LLC_FMT_U32
#define LLC_FMT_I64_NE_I64	"%"	LLC_FMT_I64	" != %"	LLC_FMT_I64
#define LLC_FMT_U64_NE_U64	"%"	LLC_FMT_U64	" != %"	LLC_FMT_U64

#define LLC_FMT_EQ_F32	LLC_FMT_F32_EQ_F32
#define LLC_FMT_EQ_F64	LLC_FMT_F64_EQ_F64
#define LLC_FMT_EQ_I32	LLC_FMT_I32_EQ_I32
#define LLC_FMT_EQ_U32	LLC_FMT_U32_EQ_U32
#define LLC_FMT_EQ_I64	LLC_FMT_I64_EQ_I64
#define LLC_FMT_EQ_U64	LLC_FMT_U64_EQ_U64
#define LLC_FMT_NE_F32	LLC_FMT_F32_NE_F32
#define LLC_FMT_NE_F64	LLC_FMT_F64_NE_F64
#define LLC_FMT_NE_I32	LLC_FMT_I32_NE_I32
#define LLC_FMT_NE_U32	LLC_FMT_U32_NE_U32
#define LLC_FMT_NE_I64	LLC_FMT_I64_NE_I64
#define LLC_FMT_NE_U64	LLC_FMT_U64_NE_U64
#define LLC_FMT_GT_F32	LLC_FMT_F32_GT_F32
#define LLC_FMT_GT_F64	LLC_FMT_F64_GT_F64
#define LLC_FMT_GT_I32	LLC_FMT_I32_GT_I32
#define LLC_FMT_GT_U32	LLC_FMT_U32_GT_U32
#define LLC_FMT_GT_I64	LLC_FMT_I64_GT_I64
#define LLC_FMT_GT_U64	LLC_FMT_U64_GT_U64
#define LLC_FMT_GE_F32	LLC_FMT_F32_GE_F32
#define LLC_FMT_GE_F64	LLC_FMT_F64_GE_F64
#define LLC_FMT_GE_I32	LLC_FMT_I32_GE_I32
#define LLC_FMT_GE_U32	LLC_FMT_U32_GE_U32
#define LLC_FMT_GE_I64	LLC_FMT_I64_GE_I64
#define LLC_FMT_GE_U64	LLC_FMT_U64_GE_U64
#define LLC_FMT_LE_F32	LLC_FMT_F32_LE_F32
#define LLC_FMT_LE_F64	LLC_FMT_F64_LE_F64
#define LLC_FMT_LE_I32	LLC_FMT_I32_LE_I32
#define LLC_FMT_LE_U32	LLC_FMT_U32_LE_U32
#define LLC_FMT_LE_I64	LLC_FMT_I64_LE_I64
#define LLC_FMT_LE_U64	LLC_FMT_U64_LE_U64
#define LLC_FMT_LT_F32	LLC_FMT_F32_LT_F32
#define LLC_FMT_LT_F64	LLC_FMT_F64_LT_F64
#define LLC_FMT_LT_I32	LLC_FMT_I32_LT_I32
#define LLC_FMT_LT_U32	LLC_FMT_U32_LT_U32
#define LLC_FMT_LT_I64	LLC_FMT_I64_LT_I64
#define LLC_FMT_LT_U64	LLC_FMT_U64_LT_U64

// Legacy -- Compat
#define cnstnxpt	csnx
#define cnstvltl	csvl
#define unsgnd		unsd
#define operatr		oper
#define cnstxpr		cxpr
#define inlcxpr		inxp
#define stacxpr		stxp
#define stainli		stin
#define stincxp		sinx
#define prtctd		prtc
#define privte		prvt
#define nodscrd		ndsc
#define ndsttic		ndst
#define ndinlne		ndin
#define ndcnxpr		ndxp
#define ndstain		ndsi
#define ndstcxp		ndsx
#define ndincxp		ndix
#define ndstinx		nsix
#define tplTstruct	tplTstct
#define tplTndstcxp	tplTndsx
#define tplTstincxp	tplTsinx
#define tplTstacxpr	tplTstxp
#define tplTndstinx	tplTnsix

#endif	//	LLC_PLATFORM_GLOBALS_H_23627
