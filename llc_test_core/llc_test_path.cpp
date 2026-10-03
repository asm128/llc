#include "llc_path.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(PATH_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, OK						, 0, "All path tests passed.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, LAST_SLASH_NONE			, 1, "findLastSlash() did not report that the path contains no separator.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, LAST_SLASH_POSITION		, 2, "findLastSlash() did not return the last mixed-separator position.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, LAST_SLASH_TRAILING		, 3, "findLastSlash() did not return a trailing separator position.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, COMPOSE_TEXT				, 4, "pathNameCompose() produced unexpected path text.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, COMPOSE_DUPLICATE_SEPARATOR, 5, "pathNameCompose() did not reject adjacent separators within an input component.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, COMPOSE_APPEND			, 6, "pathNameCompose() did not append to the caller output.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, COMPOSE_EMPTY				, 7, "pathNameCompose() changed an empty composition.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, COMPOSE_RETURN			, 8, "pathNameCompose() did not return the resulting output size.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, COMPOSE_TERMINATOR		, 9, "pathNameCompose() did not preserve the output terminator.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, COMPOSE_SEPARATOR		, 10, "pathNameCompose() did not normalize path separators.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, COMPOSE_BOUNDARY			, 11, "pathNameCompose() did not collapse separators at the component boundary.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, COMPOSE_PATH_ONLY		, 12, "pathNameCompose() changed a path composed without a filename.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, COMPOSE_ROOT				, 13, "pathNameCompose() did not preserve the path root.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, COMPOSE_FAILURE_PRESERVE	, 14, "pathNameCompose() changed caller output after rejecting invalid input.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_EMPTY			, 15, "pathNormalize() did not normalize an empty path.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_SEPARATOR		, 16, "pathNormalize() did not normalize path separators.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_DOT				, 17, "pathNormalize() did not remove current-directory segments.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_PARENT			, 18, "pathNormalize() did not resolve parent-directory segments.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_ROOT				, 19, "pathNormalize() did not preserve an absolute root.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_UNC				, 20, "pathNormalize() did not preserve a UNC root.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_DRIVE			, 21, "pathNormalize() did not preserve drive-path semantics.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_TRAILING		, 22, "pathNormalize() did not remove a non-root trailing separator.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_REPLACE			, 23, "pathNormalize() appended to output instead of replacing it.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_RETURN			, 24, "pathNormalize() did not return the normalized path size.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_TERMINATOR		, 25, "pathNormalize() did not preserve the output terminator.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_INVALID			, 26, "pathNormalize() did not reject invalid input.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, NORMALIZE_FAILURE_PRESERVE, 27, "pathNormalize() changed caller output after rejecting invalid input.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, ABSOLUTE_CURRENT			, 28, "pathAbsolute() did not resolve the current directory.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, ABSOLUTE_RELATIVE			, 29, "pathAbsolute() did not resolve and normalize a relative path.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, ABSOLUTE_NONEXISTENT		, 30, "pathAbsolute() required the target path to exist.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, ABSOLUTE_IDEMPOTENT		, 31, "pathAbsolute() changed an already absolute normalized path.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, ABSOLUTE_SEPARATOR		, 32, "pathAbsolute() did not honor the requested output separator.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, ABSOLUTE_RETURN			, 33, "pathAbsolute() did not return the resolved path size.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, ABSOLUTE_TERMINATOR		, 34, "pathAbsolute() did not preserve the output terminator.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, ABSOLUTE_INVALID			, 35, "pathAbsolute() did not reject invalid input.");
GDEFINE_ENUM_VALUED(PATH_TEST_RESULT, ABSOLUTE_FAILURE_PRESERVE	, 36, "pathAbsolute() changed caller output after rejecting invalid input.");

stct SPathSlashCase {
	::llc::vcsc_t		Path;
	::llc::err_t		Expected;
	PATH_TEST_RESULT	Result;
};

stct SPathComposeCase {
	::llc::vcsc_t		Prefix;
	::llc::vcsc_t		Path;
	::llc::vcsc_t		FileName;
	::llc::vcsc_t		Expected;
	PATH_TEST_RESULT	Result;
};

stct SPathNormalizeCase {
	::llc::vcsc_t		Path;
	::llc::vcsc_t		Expected;
	::llc::sc_t		Separator;
	PATH_TEST_RESULT	Result;
};

sttc bool pathTextMismatch(::llc::vcsc_t actual, ::llc::vcsc_t expected) {
	rtrn actual.size() != expected.size() || (actual.size() && memcmp(actual.begin(), expected.begin(), actual.size()));
}

sttc ::llc::err_t testFindLastSlash(ATestError & errors) {
	cnst SPathSlashCase cases[] =
		{ {LLC_CXS("")						, -1, PATH_TEST_RESULT_LAST_SLASH_NONE}
		, {LLC_CXS("file.txt")				, -1, PATH_TEST_RESULT_LAST_SLASH_NONE}
		, {LLC_CXS("folder/file.txt")		,  6, PATH_TEST_RESULT_LAST_SLASH_POSITION}
		, {LLC_CXS("folder\\file.txt")		,  6, PATH_TEST_RESULT_LAST_SLASH_POSITION}
		, {LLC_CXS("root/child\\file.txt")	, 10, PATH_TEST_RESULT_LAST_SLASH_POSITION}
		, {LLC_CXS("/file.txt")				,  0, PATH_TEST_RESULT_LAST_SLASH_POSITION}
		, {LLC_CXS("folder/")				,  6, PATH_TEST_RESULT_LAST_SLASH_TRAILING}
		, {LLC_CXS("folder\\")				,  6, PATH_TEST_RESULT_LAST_SLASH_TRAILING}
		};
	for(cnst SPathSlashCase & testCase : cases) {
		cnst ::llc::err_t actual = ::llc::findLastSlash(testCase.Path);
		LLC_TEST_CHECK(errors, testCase.Result, actual != testCase.Expected
			, "Path:'%.*s' returned:%" LLC_FMT_S2 ", expected:%" LLC_FMT_S2 "."
			, (int)testCase.Path.size(), testCase.Path.begin(), actual, testCase.Expected
			);
	}
	rtrn 0;
}

sttc ::llc::err_t testPathNameCompose(ATestError & errors) {
	cnst SPathComposeCase cases[] =
		{ {LLC_CXS("")		, LLC_CXS("")				, LLC_CXS("")			, LLC_CXS("")					, PATH_TEST_RESULT_COMPOSE_EMPTY}
		, {LLC_CXS("")		, LLC_CXS("")				, LLC_CXS("file.txt")	, LLC_CXS("file.txt")			, PATH_TEST_RESULT_COMPOSE_TEXT}
		, {LLC_CXS("")		, LLC_CXS("folder")			, LLC_CXS("file.txt")	, LLC_CXS("folder/file.txt")		, PATH_TEST_RESULT_COMPOSE_TEXT}
		, {LLC_CXS("")		, LLC_CXS("folder/")		, LLC_CXS("file.txt")	, LLC_CXS("folder/file.txt")		, PATH_TEST_RESULT_COMPOSE_TEXT}
		, {LLC_CXS("")		, LLC_CXS("folder\\")		, LLC_CXS("file.txt")	, LLC_CXS("folder/file.txt")		, PATH_TEST_RESULT_COMPOSE_SEPARATOR}
		, {LLC_CXS("")		, LLC_CXS("folder/")		, LLC_CXS("/file.txt")	, LLC_CXS("folder/file.txt")		, PATH_TEST_RESULT_COMPOSE_BOUNDARY}
		, {LLC_CXS("")		, LLC_CXS("folder")			, LLC_CXS("\\file.txt")	, LLC_CXS("folder/file.txt")		, PATH_TEST_RESULT_COMPOSE_BOUNDARY}
		, {LLC_CXS("")		, LLC_CXS("folder")			, LLC_CXS("")			, LLC_CXS("folder")				, PATH_TEST_RESULT_COMPOSE_PATH_ONLY}
		, {LLC_CXS("")		, LLC_CXS("/")				, LLC_CXS("/file.txt")	, LLC_CXS("/file.txt")			, PATH_TEST_RESULT_COMPOSE_ROOT}
		, {LLC_CXS("")		, LLC_CXS("C:\\root")			, LLC_CXS("file.txt")	, LLC_CXS("C:/root/file.txt")		, PATH_TEST_RESULT_COMPOSE_SEPARATOR}
		, {LLC_CXS("")		, LLC_CXS("\\\\server\\share")	, LLC_CXS("file.txt")	, LLC_CXS("//server/share/file.txt")	, PATH_TEST_RESULT_COMPOSE_ROOT}
		, {LLC_CXS("prefix:")	, LLC_CXS("folder")			, LLC_CXS("file.txt")	, LLC_CXS("prefix:folder/file.txt"), PATH_TEST_RESULT_COMPOSE_APPEND}
		};
	for(cnst SPathComposeCase & testCase : cases) {
		::llc::asc_t		output		= testCase.Prefix;
		cnst ::llc::err_t result		= ::llc::pathNameCompose(testCase.Path, testCase.FileName, output);
		LLC_TEST_CHECK(errors, testCase.Result, pathTextMismatch(output, testCase.Expected)
			, "Prefix:'%.*s', path:'%.*s', file:'%.*s' produced:'%.*s'/%u, expected:'%.*s'/%u."
			, (int)testCase.Prefix.size(), testCase.Prefix.begin(), (int)testCase.Path.size(), testCase.Path.begin(), (int)testCase.FileName.size(), testCase.FileName.begin()
			, (int)output.size(), output.begin(), output.size(), (int)testCase.Expected.size(), testCase.Expected.begin(), testCase.Expected.size()
			);
		LLC_TEST_CHECK(errors, PATH_TEST_RESULT_COMPOSE_RETURN, result != (::llc::err_t)output.size()
			, "Prefix:'%.*s', path:'%.*s', file:'%.*s' returned:%" LLC_FMT_S2 ", output size:%u."
			, (int)testCase.Prefix.size(), testCase.Prefix.begin(), (int)testCase.Path.size(), testCase.Path.begin(), (int)testCase.FileName.size(), testCase.FileName.begin(), result, output.size()
			);
		LLC_TEST_CHECK(errors, PATH_TEST_RESULT_COMPOSE_TERMINATOR, output.size() && output.begin()[output.size()]
			, "Prefix:'%.*s', path:'%.*s', file:'%.*s' output size:%u, terminator:%i."
			, (int)testCase.Prefix.size(), testCase.Prefix.begin(), (int)testCase.Path.size(), testCase.Path.begin(), (int)testCase.FileName.size(), testCase.FileName.begin(), output.size(), output.size() ? output.begin()[output.size()] : 0
			);
	}
	rtrn 0;
}

sttc ::llc::err_t testPathNameComposeInvalid(ATestError & errors) {
	cnst SPathComposeCase cases[] =
		{ {LLC_CXS("preserved:")	, LLC_CXS("root//folder")	, LLC_CXS("file.txt")		, LLC_CXS("preserved:")	, PATH_TEST_RESULT_COMPOSE_DUPLICATE_SEPARATOR}
		, {LLC_CXS("preserved:")	, LLC_CXS("root\\\\folder")	, LLC_CXS("file.txt")		, LLC_CXS("preserved:")	, PATH_TEST_RESULT_COMPOSE_DUPLICATE_SEPARATOR}
		, {LLC_CXS("preserved:")	, LLC_CXS("root\\/folder")	, LLC_CXS("file.txt")		, LLC_CXS("preserved:")	, PATH_TEST_RESULT_COMPOSE_DUPLICATE_SEPARATOR}
		, {LLC_CXS("preserved:")	, LLC_CXS("root")			, LLC_CXS("child//file")	, LLC_CXS("preserved:")	, PATH_TEST_RESULT_COMPOSE_DUPLICATE_SEPARATOR}
		, {LLC_CXS("preserved:")	, LLC_CXS("///server/share")	, LLC_CXS("file.txt")		, LLC_CXS("preserved:")	, PATH_TEST_RESULT_COMPOSE_DUPLICATE_SEPARATOR}
		};
	for(cnst SPathComposeCase & testCase : cases) {
		::llc::asc_t		output		= testCase.Prefix;
		cnst ::llc::err_t result		= ::llc::pathNameCompose(testCase.Path, testCase.FileName, output);
		LLC_TEST_CHECK(errors, testCase.Result, false == ::llc::failed(result)
			, "Path:'%.*s', file:'%.*s' returned:%" LLC_FMT_S2 ", expected failure."
			, (int)testCase.Path.size(), testCase.Path.begin(), (int)testCase.FileName.size(), testCase.FileName.begin(), result
			);
		LLC_TEST_CHECK(errors, PATH_TEST_RESULT_COMPOSE_FAILURE_PRESERVE, pathTextMismatch(output, testCase.Expected)
			, "Path:'%.*s', file:'%.*s' changed output to:'%.*s'/%u, expected:'%.*s'/%u."
			, (int)testCase.Path.size(), testCase.Path.begin(), (int)testCase.FileName.size(), testCase.FileName.begin()
			, (int)output.size(), output.begin(), output.size(), (int)testCase.Expected.size(), testCase.Expected.begin(), testCase.Expected.size()
			);
	}
	rtrn 0;
}

sttc ::llc::err_t testPathNormalize(ATestError & errors) {
	cnst SPathNormalizeCase cases[] =
		{ {LLC_CXS("")							, LLC_CXS("")						, '/', PATH_TEST_RESULT_NORMALIZE_EMPTY}
		, {LLC_CXS(".")							, LLC_CXS(".")						, '/', PATH_TEST_RESULT_NORMALIZE_DOT}
		, {LLC_CXS("./folder/./file")			, LLC_CXS("folder/file")			, '/', PATH_TEST_RESULT_NORMALIZE_DOT}
		, {LLC_CXS("folder\\child/file")			, LLC_CXS("folder/child/file")		, '/', PATH_TEST_RESULT_NORMALIZE_SEPARATOR}
		, {LLC_CXS("folder/child/")				, LLC_CXS("folder/child")			, '/', PATH_TEST_RESULT_NORMALIZE_TRAILING}
		, {LLC_CXS("folder/child/../file")		, LLC_CXS("folder/file")			, '/', PATH_TEST_RESULT_NORMALIZE_PARENT}
		, {LLC_CXS("folder/../../file")			, LLC_CXS("../file")				, '/', PATH_TEST_RESULT_NORMALIZE_PARENT}
		, {LLC_CXS("folder/..")					, LLC_CXS(".")						, '/', PATH_TEST_RESULT_NORMALIZE_PARENT}
		, {LLC_CXS("/folder/../")				, LLC_CXS("/")						, '/', PATH_TEST_RESULT_NORMALIZE_ROOT}
		, {LLC_CXS("C:\\folder\\..\\file")		, LLC_CXS("C:/file")				, '/', PATH_TEST_RESULT_NORMALIZE_DRIVE}
		, {LLC_CXS("C:folder\\..\\file")			, LLC_CXS("C:file")				, '/', PATH_TEST_RESULT_NORMALIZE_DRIVE}
		, {LLC_CXS("C:\\")						, LLC_CXS("C:/")					, '/', PATH_TEST_RESULT_NORMALIZE_ROOT}
		, {LLC_CXS("\\\\server\\share\\folder\\..\\file"), LLC_CXS("//server/share/file")	, '/', PATH_TEST_RESULT_NORMALIZE_UNC}
		, {LLC_CXS("\\\\server\\share\\")			, LLC_CXS("//server/share/")		, '/', PATH_TEST_RESULT_NORMALIZE_UNC}
		, {LLC_CXS("C:/folder/file")				, LLC_CXS("C:\\folder\\file")		, '\\', PATH_TEST_RESULT_NORMALIZE_SEPARATOR}
		};
	for(cnst SPathNormalizeCase & testCase : cases) {
		::llc::asc_t		output		= LLC_CXS("stale");
		cnst ::llc::err_t result		= ::llc::pathNormalize(testCase.Path, output, testCase.Separator);
		LLC_TEST_CHECK(errors, testCase.Result, pathTextMismatch(output, testCase.Expected)
			, "Path:'%.*s', separator:'%c' produced:'%.*s'/%u, expected:'%.*s'/%u."
			, (int)testCase.Path.size(), testCase.Path.begin(), testCase.Separator, (int)output.size(), output.begin(), output.size(), (int)testCase.Expected.size(), testCase.Expected.begin(), testCase.Expected.size()
			);
		LLC_TEST_CHECK(errors, PATH_TEST_RESULT_NORMALIZE_REPLACE, output.size() >= 5 && 0 == memcmp(output.begin(), "stale", 5)
			, "Path:'%.*s' retained stale output:'%.*s'/%u."
			, (int)testCase.Path.size(), testCase.Path.begin(), (int)output.size(), output.begin(), output.size()
			);
		LLC_TEST_CHECK(errors, PATH_TEST_RESULT_NORMALIZE_RETURN, result != (::llc::err_t)output.size()
			, "Path:'%.*s' returned:%" LLC_FMT_S2 ", output size:%u."
			, (int)testCase.Path.size(), testCase.Path.begin(), result, output.size()
			);
		LLC_TEST_CHECK(errors, PATH_TEST_RESULT_NORMALIZE_TERMINATOR, output.size() && output.begin()[output.size()]
			, "Path:'%.*s' output size:%u, terminator:%i."
			, (int)testCase.Path.size(), testCase.Path.begin(), output.size(), output.size() ? output.begin()[output.size()] : 0
			);
	}
	rtrn 0;
}

sttc ::llc::err_t testPathNormalizeInvalid(ATestError & errors) {
	cnst SPathNormalizeCase cases[] =
		{ {LLC_CXS("root//folder")	, LLC_CXS("preserved")	, '/', PATH_TEST_RESULT_NORMALIZE_INVALID}
		, {LLC_CXS("///server/share")	, LLC_CXS("preserved")	, '/', PATH_TEST_RESULT_NORMALIZE_INVALID}
		, {LLC_CXS("//server")		, LLC_CXS("preserved")	, '/', PATH_TEST_RESULT_NORMALIZE_INVALID}
		, {LLC_CXS("folder/file")		, LLC_CXS("preserved")	, ':', PATH_TEST_RESULT_NORMALIZE_INVALID}
		};
	for(cnst SPathNormalizeCase & testCase : cases) {
		::llc::asc_t		output		= testCase.Expected;
		cnst ::llc::err_t result		= ::llc::pathNormalize(testCase.Path, output, testCase.Separator);
		LLC_TEST_CHECK(errors, testCase.Result, false == ::llc::failed(result)
			, "Path:'%.*s', separator:'%c' returned:%" LLC_FMT_S2 ", expected failure."
			, (int)testCase.Path.size(), testCase.Path.begin(), testCase.Separator, result
			);
		LLC_TEST_CHECK(errors, PATH_TEST_RESULT_NORMALIZE_FAILURE_PRESERVE, pathTextMismatch(output, testCase.Expected)
			, "Path:'%.*s', separator:'%c' changed output to:'%.*s'/%u."
			, (int)testCase.Path.size(), testCase.Path.begin(), testCase.Separator, (int)output.size(), output.begin(), output.size()
			);
	}
	rtrn 0;
}

sttc bool pathIsAbsolute(::llc::vcsc_t path) {
	rtrn path.size() && ('/' == path[0] || '\\' == path[0] || (path.size() > 2 && ':' == path[1] && ('/' == path[2] || '\\' == path[2])));
}

sttc ::llc::err_t testPathAbsolute(ATestError & errors) {
	::llc::asc_t		current;
	cnst ::llc::err_t currentResult	= ::llc::pathAbsolute(LLC_CXS("."), current);
	LLC_TEST_REQUIRE(errors, PATH_TEST_RESULT_ABSOLUTE_CURRENT, ::llc::failed(currentResult) || false == ::pathIsAbsolute(current)
		, "Current directory resolution returned:%" LLC_FMT_S2 ", path:'%.*s'/%u."
		, currentResult, (int)current.size(), current.begin(), current.size()
		);

	::llc::asc_t		expected;
	if_fail_fe(::llc::pathNameCompose(current, LLC_CXS("llc_test_path_nonexistent"), expected));
	::llc::asc_t		resolved;
	cnst ::llc::err_t resolvedResult = ::llc::pathAbsolute(LLC_CXS("./llc/../llc_test_path_nonexistent"), resolved);
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_ABSOLUTE_RELATIVE, pathTextMismatch(resolved, expected)
		, "Relative resolution produced:'%.*s'/%u, expected:'%.*s'/%u."
		, (int)resolved.size(), resolved.begin(), resolved.size(), (int)expected.size(), expected.begin(), expected.size()
		);
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_ABSOLUTE_NONEXISTENT, ::llc::failed(resolvedResult)
		, "Nonexistent target resolution returned:%" LLC_FMT_S2 ", path:'%.*s'/%u."
		, resolvedResult, (int)resolved.size(), resolved.begin(), resolved.size()
		);

	::llc::asc_t		idempotent;
	cnst ::llc::err_t idempotentResult = ::llc::pathAbsolute(current, idempotent);
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_ABSOLUTE_IDEMPOTENT, pathTextMismatch(idempotent, current)
		, "Absolute input:'%.*s'/%u produced:'%.*s'/%u."
		, (int)current.size(), current.begin(), current.size(), (int)idempotent.size(), idempotent.begin(), idempotent.size()
		);
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_ABSOLUTE_RETURN, currentResult != (::llc::err_t)current.size() || resolvedResult != (::llc::err_t)resolved.size() || idempotentResult != (::llc::err_t)idempotent.size()
		, "Return mismatch. current:%" LLC_FMT_S2 "/%u, resolved:%" LLC_FMT_S2 "/%u, idempotent:%" LLC_FMT_S2 "/%u."
		, currentResult, current.size(), resolvedResult, resolved.size(), idempotentResult, idempotent.size()
		);
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_ABSOLUTE_TERMINATOR, (current.size() && current.begin()[current.size()]) || (resolved.size() && resolved.begin()[resolved.size()]) || (idempotent.size() && idempotent.begin()[idempotent.size()])
		, "Terminator mismatch. current:%i, resolved:%i, idempotent:%i."
		, current.size() ? current.begin()[current.size()] : 0, resolved.size() ? resolved.begin()[resolved.size()] : 0, idempotent.size() ? idempotent.begin()[idempotent.size()] : 0
		);

	::llc::asc_t		backslash;
	if_fail_fe(::llc::pathAbsolute(LLC_CXS("."), backslash, '\\'));
	::llc::asc_t		backslashAsSlash;
	if_fail_fe(::llc::pathNormalize(backslash, backslashAsSlash));
	LLC_TEST_CHECK(errors, PATH_TEST_RESULT_ABSOLUTE_SEPARATOR, pathTextMismatch(backslashAsSlash, current)
		, "Backslash path:'%.*s' normalized to:'%.*s', expected:'%.*s'."
		, (int)backslash.size(), backslash.begin(), (int)backslashAsSlash.size(), backslashAsSlash.begin(), (int)current.size(), current.begin()
		);
	rtrn 0;
}

sttc ::llc::err_t testPathAbsoluteInvalid(ATestError & errors) {
	cnst SPathNormalizeCase cases[] =
		{ {LLC_CXS("")				, LLC_CXS("preserved")	, '/', PATH_TEST_RESULT_ABSOLUTE_INVALID}
		, {LLC_CXS("root//folder")	, LLC_CXS("preserved")	, '/', PATH_TEST_RESULT_ABSOLUTE_INVALID}
		, {LLC_CXS("folder/file")		, LLC_CXS("preserved")	, ':', PATH_TEST_RESULT_ABSOLUTE_INVALID}
		};
	for(cnst SPathNormalizeCase & testCase : cases) {
		::llc::asc_t		output		= testCase.Expected;
		cnst ::llc::err_t result		= ::llc::pathAbsolute(testCase.Path, output, testCase.Separator);
		LLC_TEST_CHECK(errors, testCase.Result, false == ::llc::failed(result)
			, "Path:'%.*s', separator:'%c' returned:%" LLC_FMT_S2 ", expected failure."
			, (int)testCase.Path.size(), testCase.Path.begin(), testCase.Separator, result
			);
		LLC_TEST_CHECK(errors, PATH_TEST_RESULT_ABSOLUTE_FAILURE_PRESERVE, pathTextMismatch(output, testCase.Expected)
			, "Path:'%.*s', separator:'%c' changed output to:'%.*s'/%u."
			, (int)testCase.Path.size(), testCase.Path.begin(), testCase.Separator, (int)output.size(), output.begin(), output.size()
			);
	}
	rtrn 0;
}

::llc::err_t testPath(ATestError & errors) {
	if_fail_fe(testFindLastSlash(errors));
	if_fail_fe(testPathNameCompose(errors));
	if_fail_fe(testPathNameComposeInvalid(errors));
	if_fail_fe(testPathNormalize(errors));
	if_fail_fe(testPathNormalizeInvalid(errors));
	if_fail_fe(testPathAbsolute(errors));
	if_fail_fe(testPathAbsoluteInvalid(errors));
	rtrn 0;
}
