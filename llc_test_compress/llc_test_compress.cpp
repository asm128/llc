// test_gpk_serialize.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//#include "llc_view_serialize.h"
#include "llc_apod_serialize.h"
#include "llc_array_static.h"
#include "llc_file.h"

LLC_USING_TYPEINT();

static llc::err_t testPackedU1 () {
    for(uint16_t iUnpacked = 0; iUnpacked < 0x7FFFU; ++iUnpacked) { 
        const llc::packedu16 packed = iUnpacked; 
        if_true_if(iUnpacked == 0x80 || 0x7F == iUnpacked, "Value: 0x%X|%u, packed.Value(): 0x%X|%u, packed.TailWidth: 0x%X|%u.", iUnpacked, iUnpacked, packed.Value(), packed.Value(), packed.TailWidth, packed.TailWidth);
        if_true_ef(iUnpacked != packed.Value(), "Value: %u, packed.Value(): %u.", iUnpacked, packed.Value()); 
        if_true_ef(iUnpacked > 0x7F && 1 > packed.TailWidth, "Value = %u, packed.TailWidth = %u.", iUnpacked, (uint32_t)packed.TailWidth);
    }
    return 0;
}
static llc::err_t testPackedU2 () {
    //for(uint16_t iUnpacked = 0; iUnpacked <             0x3FFFU  ; ++iUnpacked) { llc::packedu16 packed = iUnpacked; if_true_ef(iUnpacked != packed.Value(), "iUnpacked = %u, packed.Value() = %u.", iUnpacked, packed.Value()); if_true_e(iUnpacked > 0xFF packed.TailWidth();}
    //for(uint32_t iUnpacked = 0; iUnpacked < 0x3FFFFFFFFFFFFFFFULL; ++iUnpacked) { llc::packedu64 packed = iUnpacked; if_true_ef(iUnpacked != packed.Value(), "iUnpacked = %u, packed.Value() = %u.", iUnpacked, packed.Value()); if_true_e(iUnpacked > 0xFF packed.TailWidth();}
    for(uint32_t iUnpacked = 0; iUnpacked < 0x3FFFFFFFU; ++iUnpacked) { 
        const llc::packedu32 packed = iUnpacked; 
        if_true_if(iUnpacked == 0x40     || 0x3F     == iUnpacked, "Value: 0x%X|%u, packed.Value(): 0x%X|%u, packed.TailWidth: 0x%X|%u.", iUnpacked, iUnpacked, packed.Value(), packed.Value(), packed.TailWidth, packed.TailWidth);
        if_true_if(iUnpacked == 0x4000   || 0x3FFF   == iUnpacked, "Value: 0x%X|%u, packed.Value(): 0x%X|%u, packed.TailWidth: 0x%X|%u.", iUnpacked, iUnpacked, packed.Value(), packed.Value(), packed.TailWidth, packed.TailWidth);
        if_true_if(iUnpacked == 0x400000 || 0x3FFFFF == iUnpacked, "Value: 0x%X|%u, packed.Value(): 0x%X|%u, packed.TailWidth: 0x%X|%u.", iUnpacked, iUnpacked, packed.Value(), packed.Value(), packed.TailWidth, packed.TailWidth);
        if_true_ef(iUnpacked != packed.Value(), "Value: %u, packed.Value(): %u.", iUnpacked, packed.Value()); 
        if_true_ef(iUnpacked >  0x3F     && 1 > packed.TailWidth, "Value = %u, packed.TailWidth = %u.", iUnpacked, (uint32_t)packed.TailWidth);
        if_true_ef(iUnpacked >  0x3FFF   && 2 > packed.TailWidth, "Value = %u, packed.TailWidth = %u.", iUnpacked, (uint32_t)packed.TailWidth);
        if_true_ef(iUnpacked >  0x3FFFFF && 3 > packed.TailWidth, "Value = %u, packed.TailWidth = %u.", iUnpacked, (uint32_t)packed.TailWidth);
    }
    return 0;
}
static llc::err_t testPackedU3 () {
    //for(uint64_t iUnpacked = 0; iUnpacked < 0x2000000001; ++iUnpacked) { 
    //    const llc::packedu64 packed = iUnpacked; 
    //    if_true_if(iUnpacked == 0x20          || 0x1F           == iUnpacked, "Value: 0xll%X|%llu, packed.Value(): 0x%llX|%llu, packed.TailWidth: 0x%X|%u.", iUnpacked, iUnpacked, packed.Value(), packed.Value(), packed.TailWidth, packed.TailWidth);
    //    if_true_if(iUnpacked == 0x2000        || 0x1FFF         == iUnpacked, "Value: 0xll%X|%llu, packed.Value(): 0x%llX|%llu, packed.TailWidth: 0x%X|%u.", iUnpacked, iUnpacked, packed.Value(), packed.Value(), packed.TailWidth, packed.TailWidth);
    //    if_true_if(iUnpacked == 0x200000      || 0x1FFFFF       == iUnpacked, "Value: 0xll%X|%llu, packed.Value(): 0x%llX|%llu, packed.TailWidth: 0x%X|%u.", iUnpacked, iUnpacked, packed.Value(), packed.Value(), packed.TailWidth, packed.TailWidth);
    //    if_true_if(iUnpacked == 0x20000000    || 0x1FFFFFFF     == iUnpacked, "Value: 0xll%X|%llu, packed.Value(): 0x%llX|%llu, packed.TailWidth: 0x%X|%u.", iUnpacked, iUnpacked, packed.Value(), packed.Value(), packed.TailWidth, packed.TailWidth);
    //    if_true_if(iUnpacked == 0x2000000000  || 0x1FFFFFFFFF   == iUnpacked, "Value: 0xll%X|%llu, packed.Value(): 0x%llX|%llu, packed.TailWidth: 0x%X|%u.", iUnpacked, iUnpacked, packed.Value(), packed.Value(), packed.TailWidth, packed.TailWidth);
    //    if_true_ef(iUnpacked != packed.Value(), "Value: %u, packed.Value(): %u.", iUnpacked, packed.Value()); 
    //    if_true_ef(iUnpacked >         0x1F && 1 > packed.TailWidth, "Value = %llu, packed.TailWidth = %u.", iUnpacked, (uint32_t)packed.TailWidth);
    //    if_true_ef(iUnpacked >       0x1FFF && 2 > packed.TailWidth, "Value = %llu, packed.TailWidth = %u.", iUnpacked, (uint32_t)packed.TailWidth);
    //    if_true_ef(iUnpacked >     0x1FFFFF && 3 > packed.TailWidth, "Value = %llu, packed.TailWidth = %u.", iUnpacked, (uint32_t)packed.TailWidth);
    //    if_true_ef(iUnpacked >   0x1FFFFFFF && 4 > packed.TailWidth, "Value = %llu, packed.TailWidth = %u.", iUnpacked, (uint32_t)packed.TailWidth);
    //    if_true_ef(iUnpacked > 0x1FFFFFFFFF && 5 > packed.TailWidth, "Value = %llu, packed.TailWidth = %u.", iUnpacked, (uint32_t)packed.TailWidth);
    //}
    return 0;
}

tpl_t
stxp    llc::err_t  testView    (/*uint32_t index*/) {
    stxp const _t   testInputs[6][20] = 
        { {0}
        , {0, 1}
        , {0, 1, 2}
        , {0, 1, 2, 3, 4, 5, 6, 7}
        , {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
        , {  0
          ,  1
          ,  2
          ,  3
          ,  4
          ,  5
          ,  6
          ,  7
          ,  8
          ,  9
          , 10
          , 11
          , 12
          , 13
          , 14
          , 15
          , 16
          , 17
          , 18
          , 19
          }
        };
    cnst u2_t       totalInputs         = llc::size(testInputs) / llc::size(testInputs[0]);
    for(u0_t iInput = 0; iInput < totalInputs; ++iInput) { 
        const llc::view<const _t> testInput   = testInputs[iInput]; 
        llc::au0_t          serialized; 
        llc::array_pod<_t>  testOutput;
        //for(uint32_t nSize = 0; nSize < 0xFFFFFFU; ++nSize) { 
            serialized.clear();
            if_fail_fef(llc::saveView(serialized, testInput), "serialized.size():0x%X, testInput.size():0x%X.", serialized.size(), testInput.size()); 
            always_printf("serialized.size() = %u, testInput.byte_count() = %u.", serialized.size(), testInput.byte_count());
            llc::vcsc_t         v           = serialized.cc(); 
            if_fail_fef(llc::loadView(v, testOutput), "", "");
            if_true_ef(testOutput.size() != testInput.size(), "testOutput.size() = %u, testInput.size() = %u", testOutput.size(), testInput.size());
            for(uint32_t iElement = 0; iElement < ::llc::min(testInput.size(), testOutput.size()); ++iElement) { 
                cnst auto valueIn    = testInput [iElement];
                cnst auto valueOut   = testOutput[iElement];
                if_true_fef(valueOut != valueIn, "iElement=%u, %u != %u", iElement, valueOut, valueIn);
            }
        //}
    }
    rtrn 0;
}

#if defined(LLC_WINDOWS)
static	::llc::error_t	test_base_log_write	(const char * text, uint32_t /*textLen*/) {	OutputDebugStringA(text); return (::llc::error_t)printf("%s", text); }
#elif defined(LLC_ANDROID)
static	::llc::error_t	test_base_log_write	(const char * text, uint32_t textLen) {	LOGI("%s", text); return (::llc::error_t)textLen; }
#elif defined(LLC_ARDUINO)
static	::llc::error_t	test_base_log_write	(const char * text, uint32_t textLen) {	return ::llc::error_t(Serial ? (::llc::error_t)Serial.write(text, textLen) : (::llc::error_t)textLen); }
#else
static	::llc::error_t	test_base_log_write	(const char * text, uint32_t textLen) {	(void)textLen; return (::llc::error_t)printf_s("%s", text, textLen); }
#endif

#if defined(LLC_WINDOWS)
static	::llc::error_t	test_base_log_print	(const char * text) {	OutputDebugStringA(text); return (::llc::error_t)printf("%s", text); }
#elif defined(LLC_ANDROID)z
static	::llc::error_t	test_base_log_print	(const char * text) {	LOGI("%s", text); return (::llc::error_t)strlen(text); }
#elif defined(LLC_ARDUINO)
static	::llc::error_t	test_base_log_print	(const char * text) {	return Serial ? (::llc::error_t)Serial.print(text) : (::llc::error_t)strlen(text); }
#else
static	::llc::error_t	test_base_log_print	(const char * text) {	return (::llc::error_t)printf("%s", text); }
#endif

static int testPackedCounters() {
    always_printf("# packed_uint<> Tests."); 
    always_printf("## Running testPackedU1()"); if_fail_e(testPackedU1()); 
    always_printf("## Running testPackedU2()"); if_fail_e(testPackedU2()); 
    always_printf("## Running testPackedU3()"); if_fail_e(testPackedU3()); 
    return 0;
}

int main() {
	::llc::setupLogCallbacks(test_base_log_print, test_base_log_write);
    always_printf("# Packed Counter Serialization Tests");
    if_fail_e(testPackedCounters());
    return 0;
}

//
//	::llc::error_t	test_packed_int_file	(const dstsrc<vcs> & paths, const function<error_t(const dstsrc<FILE*&>&)> & packFile)	{ 
//		llc::with(paths.dst, "wb", [](FILE*){ return 0; });
//		FILE			* fpDst		= {};
//		FILE			* fpSrc		= {};
//		llc_necs(::llc::fopen_s(&fpDst, paths.dst, "wb")); if_null_ve(-1, fpDst);
//		llc_necs(::llc::fopen_s(&fpSrc, paths.src, "rb")); if_null_ve(-1, fpSrc);
//		::llc::error_t	result; 
//		if_fail_ef(result = packFile({fpDst, fpSrc}), "dst:'%s', src:'%s'.", paths.dst.begin(), paths.src.begin());
//		fclose(fpSrc);
//		fclose(fpDst);
//		return result;
//	}
//	::llc::error_t	test_packed_int_png		(const dstsrc<vcs> & paths)		{ return test_packed_int_file(paths, [](const dstsrc<FILE*&>& files){ files.src, files.dst; return 0; }); }
//	::llc::error_t	test_packed_int_json	(const dstsrc<vcs> & paths)		{ return test_packed_int_file(paths, [](const dstsrc<FILE*&>& files){ files.src, files.dst; return 0; }); }
//} // namespace
//
//stxp	::llc::dstsrc<::llc::vcs>	TEST_PACKED_INT_PATHS	[32]	= 
//	{ {LLC_CXS("G:\\$extras"), LLC_CXS("C:\\$extras")}
//	};
//
////static	::llc::error_t	test_packed_int_files	()		{ return 0; }


