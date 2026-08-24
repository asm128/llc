// test_gpk_serialize.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include "llc_view_serialize.h"
#include "llc_apod_serialize.h"
#include "llc_array_static.h"

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
    stxp const _t   testInputs[4][6] = 
        { {0U, 1U, 2U, 3U, 4U, 5U}
        , {0U, 1U, 2U, 3U, 4U, 5U}
        , {0U, 1U, 2U, 3U, 4U, 5U}
        , {0U, 1U, 2U, 3U, 4U, 5U}
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

int testPackedCounters() {
    info_printf("# packed_uint<> Tests."); 
    info_printf("## Running testPackedU1()"); if_fail_e(testPackedU1()); 
    info_printf("## Running testPackedU2()"); if_fail_e(testPackedU2()); 
    info_printf("## Running testPackedU3()"); if_fail_e(testPackedU3()); 
    return 0;
}

int main() {
	::llc::setupLogCallbacks(test_base_log_print, test_base_log_write);
    info_printf("# Array Serialization Tests");
    //for(uint32_t iSet = 0; iSet < 4; ++iSet) {
        info_printf("## Running testView()"); if_fail_e(::testView<u0_t>(/*iSet*/)); 
        info_printf("## Running testView()"); if_fail_e(::testView<u1_t>(/*iSet*/)); 
        info_printf("## Running testView()"); if_fail_e(::testView<u2_t>(/*iSet*/)); 
        info_printf("## Running testView()"); if_fail_e(::testView<u3_t>(/*iSet*/)); 
        info_printf("## Running testView()"); if_fail_e(::testView<s0_t>(/*iSet*/)); 
        info_printf("## Running testView()"); if_fail_e(::testView<s1_t>(/*iSet*/)); 
        info_printf("## Running testView()"); if_fail_e(::testView<s2_t>(/*iSet*/)); 
        info_printf("## Running testView()"); if_fail_e(::testView<s3_t>(/*iSet*/)); 
        info_printf("## Running testView()"); if_fail_e(::testView<f2_t>(/*iSet*/)); 
        info_printf("## Running testView()"); if_fail_e(::testView<f3_t>(/*iSet*/)); 
    //}
    return 0;
}
