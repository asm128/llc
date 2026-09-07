// test_gpk_serialize.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include "llc_view_serialize.h"
#include "llc_apod_serialize.h"
#include "llc_array_static.h"

LLC_USING_TYPEINT();


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

int main() {
	::llc::setupLogCallbacks(test_base_log_print, test_base_log_write);
    always_printf("# Array Serialization Tests");
    //for(uint32_t iSet = 0; iSet < 4; ++iSet) {
        always_printf("## Running testView()"); if_fail_e(::testView<u0_t>(/*iSet*/)); 
        always_printf("## Running testView()"); if_fail_e(::testView<u1_t>(/*iSet*/)); 
        always_printf("## Running testView()"); if_fail_e(::testView<u2_t>(/*iSet*/)); 
        always_printf("## Running testView()"); if_fail_e(::testView<u3_t>(/*iSet*/)); 
        always_printf("## Running testView()"); if_fail_e(::testView<s0_t>(/*iSet*/)); 
        always_printf("## Running testView()"); if_fail_e(::testView<s1_t>(/*iSet*/)); 
        always_printf("## Running testView()"); if_fail_e(::testView<s2_t>(/*iSet*/)); 
        always_printf("## Running testView()"); if_fail_e(::testView<s3_t>(/*iSet*/)); 
        always_printf("## Running testView()"); if_fail_e(::testView<f2_t>(/*iSet*/)); 
        always_printf("## Running testView()"); if_fail_e(::testView<f3_t>(/*iSet*/)); 
    //}

    return 0;
}
