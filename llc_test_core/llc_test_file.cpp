#include "llc_test_core_suites.h"
#include "llc_file.h"
#include "llc_path.h"
#include "llc_string_compose.h"

GDEFINE_ENUM_TYPE(TEST_FILE_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(TEST_FILE_RESULT, MISSING_SOURCE   , 0, "File splitting reports a missing source file.");
GDEFINE_ENUM_VALUED(TEST_FILE_RESULT, ZERO_PART_SIZE  , 1, "File splitting rejects a zero-byte part size.");
GDEFINE_ENUM_VALUED(TEST_FILE_RESULT, SPLIT_PARTS     , 2, "File splitting writes every part with the expected byte range.");
GDEFINE_ENUM_VALUED(TEST_FILE_RESULT, WRITE_FAILURE   , 3, "File splitting reports a part write failure after retaining completed parts.");
GDEFINE_ENUM_VALUED(TEST_FILE_RESULT, JOIN            , 4, "File joining reconstructs every byte written by file splitting.");
GDEFINE_ENUM_VALUED(TEST_FILE_RESULT, TWO_DIGIT_SUFFIX, 5, "Split and join use the same zero-padded .00 and .01 suffixes.");

sttc ::llc::err_t fileTestPath(::llc::vcst_t outputFolder, ::llc::vcst_t fileName, ::llc::string & output) {
	rtrn ::llc::pathNameCompose(outputFolder, fileName, output);
}

sttc ::llc::err_t fileTestPartPath(::llc::vcst_t source, ::llc::u2_t part, ::llc::string & output) {
	output.clear();
	if_fail_fe(::llc::append_strings(output, source, '.'));
	if(part < 10)
		if_fail_fe(output.push_back('0'));
	if_fail_fe(::llc::append_strings(output, ::llc::str(part)));
	rtrn 0;
}

sttc bool fileTestEqual(::llc::vcu0_t left, ::llc::vcu0_t right) {
	rtrn left.size() == right.size() && (0 == left.size() || 0 == memcmp(left.begin(), right.begin(), left.size()));
}

sttc ::llc::err_t testFileMissingSource(::llc::ATestError & results, ::llc::vcst_t outputFolder) {
	::llc::string source;
	LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_MISSING_SOURCE, 0 != ::fileTestPath(outputFolder, "missing_source.bin", source));
	LLC_TEST_CHECK(results, TEST_FILE_RESULT_MISSING_SOURCE, 0 <= ::llc::fileSplit(source, 3));
	rtrn 0;
}

sttc ::llc::err_t testFileZeroPartSize(::llc::ATestError & results, ::llc::vcst_t outputFolder) {
	::llc::string source;
	LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_ZERO_PART_SIZE, 0 != ::fileTestPath(outputFolder, "zero_part.bin", source));
	LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_ZERO_PART_SIZE, 0 != ::llc::fileFromMemory(source, ::llc::vcst_t{"abc"}));
	LLC_TEST_CHECK(results, TEST_FILE_RESULT_ZERO_PART_SIZE, 0 <= ::llc::fileSplit(source, 0));
	rtrn 0;
}

sttc ::llc::err_t testFileSplitParts(::llc::ATestError & results, ::llc::vcst_t outputFolder) {
	sttc const ::llc::u0_t sourceBytes[] = {0, 1, 2, 3, 4, 5, 6};
	::llc::string source;
	LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_SPLIT_PARTS, 0 != ::fileTestPath(outputFolder, "split_parts.bin", source));
	LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_SPLIT_PARTS, 0 != ::llc::fileFromMemory(source, sourceBytes));
	LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_SPLIT_PARTS, 3 != ::llc::fileSplit(source, 3));
	for(::llc::u2_t iPart = 0, offset = 0; iPart < 3; ++iPart) {
		::llc::string partPath;
		::llc::au0_t partBytes;
		LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_SPLIT_PARTS, 0 != ::fileTestPartPath(source, iPart, partPath));
		LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_SPLIT_PARTS, 0 != ::llc::fileToMemory(partPath, partBytes));
		cnst ::llc::u2_t expectedSize = ::llc::min((::llc::u2_t)3, (::llc::u2_t)szof(sourceBytes) - offset);
		LLC_TEST_CHECK(results, TEST_FILE_RESULT_SPLIT_PARTS, false == ::fileTestEqual(partBytes, {sourceBytes + offset, expectedSize}));
		offset += expectedSize;
	}
	rtrn 0;
}

sttc ::llc::err_t testFileWriteFailure(::llc::ATestError & results, ::llc::vcst_t outputFolder) {
	sttc const ::llc::u0_t sourceBytes[] = {0, 1, 2, 3, 4, 5};
	::llc::string source, firstPart, blockedPart;
	LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_WRITE_FAILURE, 0 != ::fileTestPath(outputFolder, "write_failure.bin", source));
	LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_WRITE_FAILURE, 0 != ::llc::fileFromMemory(source, sourceBytes));
	LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_WRITE_FAILURE, 0 != ::fileTestPartPath(source, 0, firstPart));
	LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_WRITE_FAILURE, 0 != ::fileTestPartPath(source, 1, blockedPart));
	LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_WRITE_FAILURE, 0 != ::llc::pathCreate(blockedPart));
	LLC_TEST_CHECK(results, TEST_FILE_RESULT_WRITE_FAILURE, 0 <= ::llc::fileSplit(source, 3));
	LLC_TEST_CHECK(results, TEST_FILE_RESULT_WRITE_FAILURE, 3 != ::llc::fileSize(firstPart));
	rtrn 0;
}

sttc ::llc::err_t testFileJoin(::llc::ATestError & results, ::llc::vcst_t outputFolder) {
	sttc const ::llc::u0_t sourceBytes[] = {9, 8, 7, 6, 5, 4, 3};
	::llc::string source;
	LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_JOIN, 0 != ::fileTestPath(outputFolder, "join.bin", source));
	LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_JOIN, 0 != ::llc::fileFromMemory(source, sourceBytes));
	LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_JOIN, 3 != ::llc::fileSplit(source, 3));
	LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_JOIN, 3 != ::llc::fileJoin(source));
	::llc::au0_t joined;
	LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_JOIN, 0 != ::llc::fileToMemory(source, joined));
	LLC_TEST_CHECK(results, TEST_FILE_RESULT_JOIN, false == ::fileTestEqual(sourceBytes, joined));
	rtrn 0;
}

sttc ::llc::err_t testFileTwoDigitSuffix(::llc::ATestError & results, ::llc::vcst_t outputFolder) {
	sttc const ::llc::u0_t sourceBytes[] = {1, 2, 3, 4};
	::llc::string source, part00, part01, part0, part1;
	LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_TWO_DIGIT_SUFFIX, 0 != ::fileTestPath(outputFolder, "suffix.bin", source));
	LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_TWO_DIGIT_SUFFIX, 0 != ::llc::fileFromMemory(source, sourceBytes));
	LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_TWO_DIGIT_SUFFIX, 2 != ::llc::fileSplit(source, 2));
	LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_TWO_DIGIT_SUFFIX, 0 != ::fileTestPartPath(source, 0, part00));
	LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_TWO_DIGIT_SUFFIX, 0 != ::fileTestPartPath(source, 1, part01));
	LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_TWO_DIGIT_SUFFIX, 0 != ::llc::append_strings(part0, source, ".0"));
	LLC_TEST_REQUIRE(results, TEST_FILE_RESULT_TWO_DIGIT_SUFFIX, 0 != ::llc::append_strings(part1, source, ".1"));
	LLC_TEST_CHECK(results, TEST_FILE_RESULT_TWO_DIGIT_SUFFIX, 2 != ::llc::fileSize(part00) || 2 != ::llc::fileSize(part01) || 0 <= ::llc::fileSize(part0) || 0 <= ::llc::fileSize(part1));
	rtrn 0;
}

::llc::err_t testFile(::llc::ATestError & results, ::llc::vcst_t outputFolder) {
	::llc::err_t result = 0;
	result = ::llc::min(result, ::testFileMissingSource(results, outputFolder));
	result = ::llc::min(result, ::testFileZeroPartSize(results, outputFolder));
	result = ::llc::min(result, ::testFileSplitParts(results, outputFolder));
	result = ::llc::min(result, ::testFileWriteFailure(results, outputFolder));
	result = ::llc::min(result, ::testFileJoin(results, outputFolder));
	result = ::llc::min(result, ::testFileTwoDigitSuffix(results, outputFolder));
	rtrn result;
}
