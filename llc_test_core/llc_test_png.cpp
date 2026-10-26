#include "llc_test_core_suites.h"
#include "llc_png.h"
#include "llc_deflate.h"
#include "llc_string_compose.h"

GDEFINE_ENUM_TYPE(TEST_PNG_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_PNG_RESULT, VALID_GRAYSCALE       , 0, "A minimal valid grayscale PNG fixture loads and yields its single scanline byte.");
GDEFINE_ENUM_VALUED(TEST_PNG_RESULT, SHORT_IHDR            , 1, "PNG loading rejects an IHDR chunk shorter than the required 13 bytes.");
GDEFINE_ENUM_VALUED(TEST_PNG_RESULT, TEXT_WITHOUT_SEPARATOR, 2, "PNG loading rejects a tEXt chunk without its bounded null separator.");
GDEFINE_ENUM_VALUED(TEST_PNG_RESULT, INVALID_FIRST_FILTER  , 3, "PNG loading reports an invalid first scanline filter without hanging.");
GDEFINE_ENUM_VALUED(TEST_PNG_RESULT, INVALID_COLOR_TYPE    , 4, "PNG loading preserves and reports the signed invalid-color-format result.");

sttc ::llc::err_t pngAppendU32BE(::llc::au0_t & output, ::llc::u2_t value) {
	if_fail_fe(output.push_back((::llc::u0_t)(value >> 24)));
	if_fail_fe(output.push_back((::llc::u0_t)(value >> 16)));
	if_fail_fe(output.push_back((::llc::u0_t)(value >> 8)));
	if_fail_fe(output.push_back((::llc::u0_t)value));
	rtrn 0;
}

sttc ::llc::err_t pngAppendChunk(::llc::au0_t & output, ::llc::vcst_t type, ::llc::vcu0_t data) {
	if_true_fe(type.size() != 4);
	if_fail_fe(::pngAppendU32BE(output, data.size()));
	cnst ::llc::u2_t crcOffset = output.size();
	for(::llc::u2_t i = 0; i < 4; ++i)
		if_fail_fe(output.push_back((::llc::u0_t)type[i]));
	if_fail_fe(output.append(data));
	cnst ::llc::u2_t crc = ::llc::get_crc({output.begin() + crcOffset, 4 + data.size()});
	if_fail_fe(::pngAppendU32BE(output, crc));
	rtrn 0;
}

sttc ::llc::err_t pngFixture(::llc::au0_t & output, ::llc::u0_t filter, ::llc::u0_t colorType, bool shortHeader, bool unterminatedText) {
	sttc const ::llc::u0_t signature[] = {0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A};
	sttc const ::llc::u0_t header[] = {0, 0, 0, 1, 0, 0, 0, 1, 8, 0, 0, 0, 0};
	output.clear();
	if_fail_fe(output.append(signature));
	::llc::u0_t headerData[szof(header)] = {};
	memcpy(headerData, header, szof(header));
	headerData[9] = colorType;
	if_fail_fe(::pngAppendChunk(output, "IHDR", {headerData, shortHeader ? 12U : (::llc::u2_t)szof(headerData)}));
	if(unterminatedText) {
		sttc const ::llc::u0_t text[] = {'K', 'e', 'y'};
		if_fail_fe(::pngAppendChunk(output, "tEXt", text));
	}
	cnst ::llc::u0_t scanline[] = {filter, 42};
	::llc::au0_t compressed;
	if_fail_fe(::llc::arrayDeflate(scanline, compressed));
	if_fail_fe(::pngAppendChunk(output, "IDAT", compressed));
	if_fail_fe(::pngAppendChunk(output, "IEND", {}));
	rtrn 0;
}

sttc ::llc::err_t pngWorkerRun(::llc::vcst_t testName) {
#if defined(LLC_WINDOWS)
	char executable[MAX_PATH] = {};
	cnst DWORD executableLength = ::GetModuleFileNameA({}, executable, MAX_PATH);
	if_true_fe(0 == executableLength || executableLength >= MAX_PATH);
	cnst ::llc::vcst_t executableName = {(::llc::sc_c*)executable, (::llc::u2_t)executableLength};
	::llc::string commandLine;
	if_fail_fe(::llc::append_strings(commandLine, '"', executableName, "\" --png-worker=", testName));
	STARTUPINFOA startupInfo = {};
	startupInfo.cb = szof(startupInfo);
	PROCESS_INFORMATION processInfo = {};
	if_zero_fe(::CreateProcessA(executable, commandLine.begin(), {}, {}, FALSE, CREATE_NO_WINDOW, {}, {}, &startupInfo, &processInfo));
	cnst DWORD waitResult = ::WaitForSingleObject(processInfo.hProcess, 2000);
	if(WAIT_TIMEOUT == waitResult) {
		::TerminateProcess(processInfo.hProcess, 2);
		::WaitForSingleObject(processInfo.hProcess, INFINITE);
		::CloseHandle(processInfo.hThread);
		::CloseHandle(processInfo.hProcess);
		rtrn -2;
	}
	if(WAIT_OBJECT_0 != waitResult) {
		::TerminateProcess(processInfo.hProcess, 3);
		::WaitForSingleObject(processInfo.hProcess, INFINITE);
		::CloseHandle(processInfo.hThread);
		::CloseHandle(processInfo.hProcess);
		rtrn -1;
	}
	DWORD exitCode = {};
	cnst BOOL gotExitCode = ::GetExitCodeProcess(processInfo.hProcess, &exitCode);
	::CloseHandle(processInfo.hThread);
	::CloseHandle(processInfo.hProcess);
	if_zero_fe(gotExitCode);
	rtrn exitCode ? -1 : 0;
#else
	(void)testName;
	rtrn -1;
#endif
}

sttc ::llc::err_t testPNGValidGrayscale(::llc::ATestError & results) {
	::llc::au0_t fixture;
	LLC_TEST_REQUIRE(results, TEST_PNG_RESULT_VALID_GRAYSCALE, 0 != ::pngFixture(fixture, 0, 0, false, false));
	::llc::SPNGData png;
	LLC_TEST_REQUIRE(results, TEST_PNG_RESULT_VALID_GRAYSCALE, 0 != ::llc::pngFileLoad(png, fixture));
	cnst ::llc::n2u2_t expectedSize = {1, 1};
	LLC_TEST_CHECK(results, TEST_PNG_RESULT_VALID_GRAYSCALE, png.Header.Size != expectedSize || png.Scanlines.size() != 1 || png.Scanlines[0].size() != 1 || png.Scanlines[0][0] != 42);
	rtrn 0;
}

sttc ::llc::err_t testPNGShortIHDR(::llc::ATestError & results) {
	LLC_TEST_CHECK(results, TEST_PNG_RESULT_SHORT_IHDR, 0 != ::pngWorkerRun("short_ihdr"));
	rtrn 0;
}

sttc ::llc::err_t testPNGTextWithoutSeparator(::llc::ATestError & results) {
	LLC_TEST_CHECK(results, TEST_PNG_RESULT_TEXT_WITHOUT_SEPARATOR, 0 != ::pngWorkerRun("text_without_separator"));
	rtrn 0;
}

sttc ::llc::err_t testPNGInvalidFirstFilter(::llc::ATestError & results) {
	LLC_TEST_CHECK(results, TEST_PNG_RESULT_INVALID_FIRST_FILTER, 0 != ::pngWorkerRun("invalid_first_filter"));
	rtrn 0;
}

sttc ::llc::err_t testPNGInvalidColorType(::llc::ATestError & results) {
	LLC_TEST_CHECK(results, TEST_PNG_RESULT_INVALID_COLOR_TYPE, 0 != ::pngWorkerRun("invalid_color_type"));
	rtrn 0;
}

::llc::err_t testPNGWorker(::llc::vcst_t testName) {
	::llc::au0_t fixture;
	if(testName == "short_ihdr") {
		if_fail_fe(::pngFixture(fixture, 0, 0, true, false));
	}
	else if(testName == "text_without_separator") {
		if_fail_fe(::pngFixture(fixture, 0, 0, false, true));
	}
	else if(testName == "invalid_first_filter") {
		if_fail_fe(::pngFixture(fixture, 5, 0, false, false));
	}
	else if(testName == "invalid_color_type") {
		if_fail_fe(::pngFixture(fixture, 0, 1, false, false));
	}
	else
		rtrn -1;
	::llc::SPNGData png;
	rtrn ::llc::failed(::llc::pngFileLoad(png, fixture)) ? 0 : -1;
}

::llc::err_t testPNG(::llc::ATestError & results, ::llc::vcst_t) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testPNGValidGrayscale(results));
	result = ::llc::min(result, ::testPNGShortIHDR(results));
	result = ::llc::min(result, ::testPNGTextWithoutSeparator(results));
	result = ::llc::min(result, ::testPNGInvalidFirstFilter(results));
	result = ::llc::min(result, ::testPNGInvalidColorType(results));
	rtrn result;
}
