#include "llc_array_static.h"

#include "llc_test_core.h"

#include <type_traits>

GDEFINE_ENUM_TYPE(ARRAY_STATIC_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, OK						, 0, "All array_static<> tests passed.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, EXTENT					, 1, "array_static<> did not preserve its compile-time extent.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, STORAGE_LAYOUT			, 2, "array_static<> storage did not match its declared element array.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, MEMBER_SIZE				, 3, "array_static<>::size() returned an incorrect element count.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, FREE_SIZE				, 4, "The array_static<> size() helper returned an incorrect element count.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, BYTE_COUNT				, 5, "array_static<> returned an incorrect byte count.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, BIT_COUNT				, 6, "array_static<> returned an incorrect bit count.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, BOUNDARIES				, 7, "array_static<> returned incorrect begin/end boundaries.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, MUTABLE_VIEW			, 8, "array_static<> did not expose its mutable element view.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, CONST_VIEW				, 9, "array_static<> did not expose its const element view.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, SUBSCRIPT_READ			, 10, "array_static<>::operator[] did not read the selected element.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, SUBSCRIPT_WRITE		, 11, "Mutable array_static<>::operator[] did not update its storage.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, INVALID_SUBSCRIPT		, 12, "array_static<>::operator[] accepted an index outside its extent.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, EQUALITY				, 13, "Equal array_static<> values compared different.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, INEQUALITY				, 14, "Different array_static<> values compared equal.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, REPRESENTATION_TYPE	, 15, "An array_static<> representation returned an incorrect view type.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, MUTABLE_BYTE_VIEW		, 16, "array_static<>::u8() did not expose its mutable byte range.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, CONST_BYTE_VIEW		, 17, "Const array_static<>::u8() did not expose its const byte range.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, CONST_BYTE_ALIAS		, 18, "array_static<>::cu8() did not match its const byte range.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, MUTABLE_CHAR_VIEW		, 19, "array_static<>::c() did not expose its mutable character range.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, CONST_CHAR_VIEW		, 20, "array_static<>::cc() did not expose its const character range.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, REPRESENTATION_WRITE	, 21, "A mutable representation view did not update array_static<> storage.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, SLICE_FULL				, 22, "array_static<>::slice() did not reproduce its full range.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, SLICE_REMAINDER		, 23, "array_static<>::slice() did not return the expected remainder.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, SLICE_COUNT			, 24, "array_static<>::slice() did not honor an explicit element count.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, SLICE_END				, 25, "array_static<>::slice() did not produce a valid one-past-end empty range.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, SLICE_SELF				, 26, "array_static<>::slice() could not replace an existing view of itself.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, CONST_SLICE			, 27, "Const array_static<>::slice() returned an incorrect range.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, SLICE_INVALID_OFFSET	, 28, "array_static<>::slice() accepted an offset beyond its extent.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, SLICE_INVALID_COUNT	, 29, "array_static<>::slice() accepted a count beyond its remaining extent.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, SLICE_FAILURE_STATE	, 30, "A failed array_static<>::slice() modified its output view.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, FIND_FIRST				, 31, "array_static<> find() did not return the first matching index.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, FIND_OFFSET			, 32, "array_static<> find() did not honor its starting offset.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, FIND_NOT_FOUND			, 33, "array_static<> find() did not return -1 when no element matched.");

tplt<tpnm TSource, tpnm TOutput>
sttc ::llc::err_t staticSliceExpectedFailure(TSource & source, TOutput & output, ::llc::u2_t offset, ::llc::u2_t count = (::llc::u2_t)-1) {
	::llc::setupLogCallbacks(0, 0);
	cnst ::llc::err_t result = source.slice(output, offset, count);
	::llc::setupDefaultLogCallbacks();
	rtrn result;
}

tplt<tpnm T>
sttc ::llc::err_t testStaticStructure(ATestError & errors) {
	::llc::array_static<T, 5> data = {T(1), T(2), T(3), T(4), T(5)};
	cnst ::llc::array_static<T, 5> & constData = data;
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_EXTENT, data.N != 5
		, "extent mismatch. N:%u, expected:5."
		, data.N
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_STORAGE_LAYOUT, szof(data) != szof(T) * 5U || data.Storage != data.begin()
		, "storage layout mismatch. object bytes:%u, expected:%u, storage:%p, begin:%p."
		, (::llc::u2_t)szof(data), (::llc::u2_t)(szof(T) * 5U), data.Storage, data.begin()
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_MEMBER_SIZE, data.size() != 5 || constData.size() != 5
		, "member size mismatch. mutable:%u, const:%u, expected:5."
		, data.size(), constData.size()
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_FREE_SIZE, ::llc::size(data) != 5
		, "free size mismatch. actual:%u, expected:5."
		, ::llc::size(data)
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_BYTE_COUNT, data.byte_count() != szof(T) * 5U || ::llc::byte_count(data) != szof(T) * 5U
		, "byte count mismatch. member:%u, free:%u, expected:%u."
		, data.byte_count(), ::llc::byte_count(data), (::llc::u2_t)(szof(T) * 5U)
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_BIT_COUNT, data.bit_count() != szof(T) * 5U * 8U
		, "bit count mismatch. actual:%u, expected:%u."
		, data.bit_count(), (::llc::u2_t)(szof(T) * 5U * 8U)
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_BOUNDARIES
		, data.begin() != data.Storage || data.end() != data.Storage + 5 || constData.begin() != data.Storage || constData.end() != data.Storage + 5
		, "boundaries mismatch. mutable:%p..%p, const:%p..%p, expected:%p..%p."
		, data.begin(), data.end(), constData.begin(), constData.end(), data.Storage, (data.Storage + 5)
		);

	::llc::view<T> mutableView = data;
	::llc::view<cnst T> constView = constData;
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_MUTABLE_VIEW, mutableView.begin() != data.begin() || mutableView.size() != data.size()
		, "mutable view mismatch. begin:%p, expected:%p, size:%u, expected:%u."
		, mutableView.begin(), data.begin(), mutableView.size(), data.size()
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_CONST_VIEW, constView.begin() != constData.begin() || constView.size() != constData.size()
		, "const view mismatch. begin:%p, expected:%p, size:%u, expected:%u."
		, constView.begin(), constData.begin(), constView.size(), constData.size()
		);
	mutableView[1] = T(7);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_MUTABLE_VIEW, data.Storage[1] != T(7)
		, "mutable view write mismatch. actual:%" LLC_FMT_S3 ", expected:7."
		, (::llc::s3_t)data.Storage[1]
		);
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testStaticSubscript(ATestError & errors) {
	::llc::array_static<T, 5> data = {T(1), T(2), T(3), T(4), T(5)};
	cnst ::llc::array_static<T, 5> & constData = data;
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SUBSCRIPT_READ, data[2] != T(3) || constData[3] != T(4)
		, "subscript read mismatch. mutable[2]:%" LLC_FMT_S3 ", const[3]:%" LLC_FMT_S3 "."
		, (::llc::s3_t)data[2], (::llc::s3_t)constData[3]
		);
	data[2] = T(9);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SUBSCRIPT_WRITE, data.Storage[2] != T(9)
		, "subscript write mismatch. storage[2]:%" LLC_FMT_S3 ", expected:9."
		, (::llc::s3_t)data.Storage[2]
		);
#ifdef LLC_WINDOWS
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_INVALID_SUBSCRIPT, !testThrows([&]() { (void)data[5]; })
		, "mutable subscript accepted index 5 for extent 5."
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_INVALID_SUBSCRIPT, !testThrows([&]() { (void)constData[5]; })
		, "const subscript accepted index 5 for extent 5."
		);
#endif
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testStaticEquality(ATestError & errors) {
	::llc::array_static<T, 5> first		= {T(1), T(2), T(3), T(4), T(5)};
	::llc::array_static<T, 5> equal		= {T(1), T(2), T(3), T(4), T(5)};
	::llc::array_static<T, 5> different	= {T(1), T(2), T(3), T(4), T(6)};
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_EQUALITY, !(first == first) || !(first == equal) || first != equal
		, "equal static arrays compared different."
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_INEQUALITY, first == different || !(first != different)
		, "different static arrays compared equal."
		);
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testStaticRepresentations(ATestError & errors) {
	::llc::array_static<T, 5> data = {T(1), T(2), T(3), T(4), T(5)};
	cnst ::llc::array_static<T, 5> & constData = data;
	auto mutableBytes	= data.u8();
	auto mutableChars	= data.c();
	auto constBytes		= constData.u8();
	auto constByteAlias	= constData.cu8();
	auto constChars		= constData.cc();
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_REPRESENTATION_TYPE
		, false == (::std::is_same_v<decltype(mutableBytes), ::llc::view<::llc::u0_t>>) || false == (::std::is_same_v<decltype(mutableChars), ::llc::view<::llc::sc_t>>) || false == (::std::is_same_v<decltype(constBytes), ::llc::view<::llc::u0_c>>) || false == (::std::is_same_v<decltype(constByteAlias), ::llc::view<::llc::u0_c>>) || false == (::std::is_same_v<decltype(constChars), ::llc::view<::llc::sc_c>>)
		, "representation type mismatch. byte count:%u."
		, data.byte_count()
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_MUTABLE_BYTE_VIEW, mutableBytes.begin() != (::llc::u0_t*)data.begin() || mutableBytes.size() != data.byte_count()
		, "mutable byte range mismatch. begin:%p, expected:%p, size:%u, expected:%u."
		, mutableBytes.begin(), data.begin(), mutableBytes.size(), data.byte_count()
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_CONST_BYTE_VIEW, constBytes.begin() != (cnst ::llc::u0_t*)constData.begin() || constBytes.size() != constData.byte_count()
		, "const byte range mismatch. begin:%p, expected:%p, size:%u, expected:%u."
		, constBytes.begin(), constData.begin(), constBytes.size(), constData.byte_count()
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_CONST_BYTE_ALIAS, constByteAlias.begin() != constBytes.begin() || constByteAlias.size() != constBytes.size()
		, "const byte alias mismatch. alias:%p/%u, bytes:%p/%u."
		, constByteAlias.begin(), constByteAlias.size(), constBytes.begin(), constBytes.size()
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_MUTABLE_CHAR_VIEW, mutableChars.begin() != (::llc::sc_t*)data.begin() || mutableChars.size() != data.byte_count()
		, "mutable character range mismatch. begin:%p, expected:%p, size:%u, expected:%u."
		, mutableChars.begin(), data.begin(), mutableChars.size(), data.byte_count()
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_CONST_CHAR_VIEW, constChars.begin() != (cnst ::llc::sc_t*)constData.begin() || constChars.size() != constData.byte_count()
		, "const character range mismatch. begin:%p, expected:%p, size:%u, expected:%u."
		, constChars.begin(), constData.begin(), constChars.size(), constData.byte_count()
		);
	cnst ::llc::u0_t replacement = (::llc::u0_t)(mutableBytes[0] ^ 0x5AU);
	mutableBytes[0] = replacement;
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_REPRESENTATION_WRITE, *(::llc::u0_t*)data.Storage != replacement
		, "byte representation write mismatch. storage byte:%u, expected:%u."
		, *(::llc::u0_t*)data.Storage, replacement
		);
	cnst ::llc::sc_t charReplacement = (::llc::sc_t)(mutableChars[1] ^ 0x35);
	mutableChars[1] = charReplacement;
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_REPRESENTATION_WRITE, *((::llc::sc_t*)data.Storage + 1) != charReplacement
		, "character representation write mismatch. storage byte:%i, expected:%i."
		, *((::llc::sc_t*)data.Storage + 1), charReplacement
		);
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testStaticSlice(ATestError & errors) {
	::llc::array_static<T, 5> data = {T(1), T(2), T(3), T(4), T(5)};
	::llc::view<T> output;
	::llc::err_t result = data.slice(output, 0);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_FULL, result != 5 || output.begin() != data.begin() || output.end() != data.end()
		, "full slice mismatch. result:%i, range:%p..%p, expected:%p..%p."
		, result, output.begin(), output.end(), data.begin(), data.end()
		);
	result = data.slice(output, 2);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_REMAINDER, result != 3 || output.begin() != data.begin() + 2 || output.end() != data.end()
		, "remainder slice mismatch. result:%i, range:%p..%p, expected:%p..%p."
		, result, output.begin(), output.end(), (data.begin() + 2), data.end()
		);
	result = data.slice(output, 1, 2);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_COUNT, result != 2 || output.begin() != data.begin() + 1 || output.end() != data.begin() + 3
		, "counted slice mismatch. result:%i, range:%p..%p, expected:%p..%p."
		, result, output.begin(), output.end(), (data.begin() + 1), (data.begin() + 3)
		);
	result = data.slice(output, 5);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_END, result || output.size() || output.begin() != data.end() || output.end() != data.end()
		, "end slice mismatch. result:%i, range:%p..%p, expected:%p."
		, result, output.begin(), output.end(), data.end()
		);
	::llc::view<T> self = data;
	result = data.slice(self, 2, 2);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_SELF, result != 2 || self.begin() != data.begin() + 2 || self.end() != data.begin() + 4
		, "self slice mismatch. result:%i, range:%p..%p, expected:%p..%p."
		, result, self.begin(), self.end(), (data.begin() + 2), (data.begin() + 4)
		);

	cnst ::llc::array_static<T, 5> & constData = data;
	::llc::view<cnst T> constOutput;
	result = constData.slice(constOutput, 1, 3);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_CONST_SLICE, result != 3 || constOutput.begin() != constData.begin() + 1 || constOutput.end() != constData.begin() + 4
		, "const slice mismatch. result:%i, range:%p..%p, expected:%p..%p."
		, result, constOutput.begin(), constOutput.end(), (constData.begin() + 1), (constData.begin() + 4)
		);

	T guard[] = {T(8), T(9)};
	output = {guard, 2};
	result = staticSliceExpectedFailure(data, output, 6);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_INVALID_OFFSET, false == ::llc::failed(result)
		, "slice accepted offset 6 for extent 5. result:%i."
		, result
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_FAILURE_STATE, output.begin() != guard || output.size() != 2
		, "failed offset slice changed output. begin:%p, expected:%p, size:%u, expected:2."
		, output.begin(), guard, output.size()
		);
	result = staticSliceExpectedFailure(data, output, 4, 2);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_INVALID_COUNT, false == ::llc::failed(result)
		, "slice accepted count 2 after offset 4. result:%i."
		, result
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_FAILURE_STATE, output.begin() != guard || output.size() != 2
		, "failed count slice changed output. begin:%p, expected:%p, size:%u, expected:2."
		, output.begin(), guard, output.size()
		);
	constOutput = {guard, 2};
	result = staticSliceExpectedFailure(constData, constOutput, 6);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_INVALID_OFFSET, false == ::llc::failed(result)
		, "const slice accepted offset 6 for extent 5. result:%i."
		, result
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_FAILURE_STATE, constOutput.begin() != guard || constOutput.size() != 2
		, "failed const slice changed output. begin:%p, expected:%p, size:%u, expected:2."
		, constOutput.begin(), guard, constOutput.size()
		);
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testStaticFind(ATestError & errors) {
	cnst ::llc::array_static<T, 5> data = {T(2), T(3), T(2), T(4), T(2)};
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_FIND_FIRST, ::llc::find(T(2), data) != 0 || ::llc::find(T(3), data) != 1
		, "first find mismatch. value 2:%i, value 3:%i."
		, ::llc::find(T(2), data), ::llc::find(T(3), data)
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_FIND_OFFSET, ::llc::find(T(2), data, 1) != 2 || ::llc::find(T(2), data, 3) != 4
		, "offset find mismatch. offset 1:%i, offset 3:%i."
		, ::llc::find(T(2), data, 1), ::llc::find(T(2), data, 3)
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_FIND_NOT_FOUND, ::llc::find(T(9), data) != -1 || ::llc::find(T(2), data, 5) != -1
		, "missing find mismatch. absent:%i, end offset:%i."
		, ::llc::find(T(9), data), ::llc::find(T(2), data, 5)
		);
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testStaticType(ATestError & errors) {
	if_fail_fe(testStaticStructure<T>(errors));
	if_fail_fe(testStaticSubscript<T>(errors));
	if_fail_fe(testStaticEquality<T>(errors));
	if_fail_fe(testStaticRepresentations<T>(errors));
	if_fail_fe(testStaticSlice<T>(errors));
	rtrn testStaticFind<T>(errors);
}

tplt<tpnm T>
sttc ::llc::err_t testStaticTypeLogged(ATestError & errors) {
	cnst ::llc::u2_t checkCount = testCheckCount(errors);
	cnst ::llc::u2_t failureCount = testErrorCount(errors);
	if_fail_fe(testStaticType<T>(errors));
	cnst ::llc::u2_t typeFailures = testErrorCount(errors) - failureCount;
	cnst ::llc::u2_t typeChecks = testCheckCount(errors) - checkCount;
	if(typeFailures) error_printf("%s suite completed: %u/%u checks passed, %u failed.", ::llc::get_type_namep<T>(), typeChecks - typeFailures, typeChecks, typeFailures);
	rtrn 0;
}

::llc::err_t testArrayStatic(ATestError & errors) {
	cnst ::llc::u2_t failureCount = testErrorCount(errors);
	if_fail_fe(testStaticTypeLogged<::llc::u0_t>(errors));
	if_fail_fe(testStaticTypeLogged<::llc::u1_t>(errors));
	if_fail_fe(testStaticTypeLogged<::llc::u2_t>(errors));
	if_fail_fe(testStaticTypeLogged<::llc::u3_t>(errors));
	if_fail_fe(testStaticTypeLogged<::llc::s0_t>(errors));
	if_fail_fe(testStaticTypeLogged<::llc::s1_t>(errors));
	if_fail_fe(testStaticTypeLogged<::llc::s2_t>(errors));
	if_fail_fe(testStaticTypeLogged<::llc::s3_t>(errors));
	if(failureCount == testErrorCount(errors))
		always_printf("Types tested successfully:\n%s, %s, %s, %s, %s, %s, %s, %s."
			, ::llc::get_type_namep<::llc::u0_t>(), ::llc::get_type_namep<::llc::u1_t>(), ::llc::get_type_namep<::llc::u2_t>(), ::llc::get_type_namep<::llc::u3_t>()
			, ::llc::get_type_namep<::llc::s0_t>(), ::llc::get_type_namep<::llc::s1_t>(), ::llc::get_type_namep<::llc::s2_t>(), ::llc::get_type_namep<::llc::s3_t>()
			);
	rtrn 0;
}
