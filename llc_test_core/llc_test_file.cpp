#include "llc_file.h"
#include "llc_string_compose.h"
#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(FILE_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(FILE_TEST_RESULT, SPLIT_COUNT, 1, "Splitting must return the number of numbered parts, including zero for an empty file.");
GDEFINE_ENUM_VALUED(FILE_TEST_RESULT, PART_BYTES, 2, "Every part must contain the exact source range without exceeding its requested size.");
GDEFINE_ENUM_VALUED(FILE_TEST_RESULT, JOIN, 3, "Joining numbered parts must reproduce the source bytes and part count.");
GDEFINE_ENUM_VALUED(FILE_TEST_RESULT, INVALID, 4, "Missing input and zero part size must fail before producing parts.");
GDEFINE_ENUM_VALUED(FILE_TEST_RESULT, PARTIAL, 5, "A failed split must report failure and retain completed parts for inspection.");

sttc ::llc::err_t testFileSplitCase(::llc::ATestError & errors, ::llc::vcst_t folder, ::llc::u2_t length, ::llc::u2_t partSize) {
	::llc::string name, path;
	if_fail_fe(::llc::append_strings(name, "split-", ::llc::str(length), '-', ::llc::str(partSize), ".bin"));
	if_fail_fe(::llc::pathNameCompose(folder, name, path));
	::llc::au0_t original;
	if_fail_fe(original.resize(length));
	for(::llc::u2_t i = 0; i < length; ++i)
		original[i] = (::llc::u0_t)(i * 37U);
	if_fail_fe(::llc::fileFromMemory(path, original));
	cnst ::llc::u2_t count = length / partSize + (length % partSize ? 1U : 0U);
	LLC_TEST_REQUIREF(errors, FILE_TEST_RESULT_SPLIT_COUNT, ::llc::fileSplit(path, partSize) != (::llc::err_t)count, "Length:%u, part size:%u.", length, partSize);
	for(::llc::u2_t part = 0; part < count; ++part) {
		::llc::string partPath;
		if_fail_fe(::llc::append_strings(partPath, path, '.', ::llc::str(part)));
		::llc::au0_t bytes;
		if_fail_fe(::llc::fileToMemory(partPath, bytes));
		::llc::vcu0_t expected;
		if_fail_fe(original.slice(expected, part * partSize, ::llc::min(partSize, length - part * partSize)));
		LLC_TEST_CHECKF(errors, FILE_TEST_RESULT_PART_BYTES, ::llc::vcu0_t{bytes} != expected, "Length:%u, part size:%u, part:%u.", length, partSize, part);
	}
	LLC_TEST_REQUIRE(errors, FILE_TEST_RESULT_JOIN, ::llc::fileJoin(path) != (::llc::err_t)count);
	LLC_TEST_CHECK(errors, FILE_TEST_RESULT_JOIN, ::llc::fileSize(path) != length);
	if(length) {
		::llc::au0_t restored;
		if_fail_fe(::llc::fileToMemory(path, restored));
		LLC_TEST_CHECK(errors, FILE_TEST_RESULT_JOIN, ::llc::vcu0_t{restored} != ::llc::vcu0_t{original});
	}
	for(::llc::u2_t part = 0; part < count; ++part) {
		::llc::string partPath;
		if_fail_fe(::llc::append_strings(partPath, path, '.', ::llc::str(part)));
		if_fail_fe(::llc::fileDelete(partPath));
	}
	return ::llc::fileDelete(path);
}

::llc::err_t testFile(::llc::ATestError & errors, ::llc::vcst_t outputFolder) {
	for(::llc::u2_c length : {0U, 1U, 7U, 8U, 9U, 257U})
		for(::llc::u2_c partSize : {1U, 8U, 64U})
			if_fail_fe(testFileSplitCase(errors, outputFolder, length, partSize));
	::llc::string source, first, blocked, missing;
	if_fail_fe(::llc::pathNameCompose(outputFolder, "partial.bin", source));
	if_fail_fe(::llc::pathNameCompose(outputFolder, "missing.bin", missing));
	if_fail_fe(::llc::append_strings(first, source, ".0"));
	if_fail_fe(::llc::append_strings(blocked, source, ".1"));
	cnst ::llc::u0_t values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	if_fail_fe(::llc::fileFromMemory(source, ::llc::vcu0_t{values}));
	LLC_TEST_CHECK(errors, FILE_TEST_RESULT_INVALID, ::llc::fileSplit(source, 0) >= 0);
	LLC_TEST_CHECK(errors, FILE_TEST_RESULT_INVALID, ::llc::fileSize(first) >= 0);
	LLC_TEST_CHECK(errors, FILE_TEST_RESULT_INVALID, ::llc::fileSplit(missing, 4) >= 0);
	if_fail_fe(::llc::pathCreate(blocked));
	LLC_TEST_REQUIRE(errors, FILE_TEST_RESULT_PARTIAL, ::llc::fileSplit(source, 4) >= 0);
	::llc::au0_t bytes;
	if_fail_fe(::llc::fileToMemory(first, bytes));
	LLC_TEST_CHECK(errors, FILE_TEST_RESULT_PARTIAL, ::llc::vcu0_t{bytes} != (::llc::vcu0_t{values, 4}));
	if_fail_fe(::llc::fileDelete(first));
	return ::llc::fileDelete(source);
}
