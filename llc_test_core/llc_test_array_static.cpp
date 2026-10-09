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
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, STORAGE_BEGIN			, 34, "array_static<> storage and begin() differ.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, CONST_MEMBER_SIZE		, 35, "Const array_static<>::size() returned an incorrect element count.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, FREE_BYTE_COUNT		, 36, "The array_static<> byte_count() helper returned an incorrect byte count.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, END_BOUNDARY			, 37, "array_static<>::end() returned an incorrect boundary.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, CONST_BEGIN_BOUNDARY	, 38, "Const array_static<>::begin() returned an incorrect boundary.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, CONST_END_BOUNDARY		, 39, "Const array_static<>::end() returned an incorrect boundary.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, MUTABLE_VIEW_SIZE		, 40, "The mutable array_static<> view has the wrong size.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, CONST_VIEW_SIZE		, 41, "The const array_static<> view has the wrong size.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, CONST_SUBSCRIPT_READ	, 42, "Const array_static<>::operator[] read the wrong value.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, REFLEXIVE_EQUALITY	, 43, "An array_static<> compared unequal to itself.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, EQUAL_INEQUALITY		, 44, "Equal array_static<> values compared unequal with operator!=.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, DIFFERENT_INEQUALITY	, 45, "Different array_static<> values compared equal with operator!=.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, MUTABLE_CHAR_TYPE		, 46, "array_static<>::c() returned the wrong view type.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, CONST_BYTE_TYPE		, 47, "Const array_static<>::u8() returned the wrong view type.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, CONST_BYTE_ALIAS_TYPE	, 48, "array_static<>::cu8() returned the wrong view type.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, CONST_CHAR_TYPE		, 49, "array_static<>::cc() returned the wrong view type.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, MUTABLE_BYTE_SIZE		, 50, "array_static<>::u8() returned the wrong byte-view size.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, CONST_BYTE_SIZE		, 51, "Const array_static<>::u8() returned the wrong byte-view size.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, CONST_BYTE_ALIAS_SIZE	, 52, "array_static<>::cu8() returned the wrong byte-view size.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, MUTABLE_CHAR_SIZE		, 53, "array_static<>::c() returned the wrong character-view size.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, CONST_CHAR_SIZE		, 54, "array_static<>::cc() returned the wrong character-view size.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, CHAR_REPRESENTATION_WRITE, 55, "A mutable character view did not update array_static<> storage.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, SLICE_FULL_BEGIN		, 56, "The full slice starts at the wrong element.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, SLICE_FULL_END			, 57, "The full slice ends at the wrong element.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, SLICE_REMAINDER_BEGIN	, 58, "The remainder slice starts at the wrong element.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, SLICE_REMAINDER_END	, 59, "The remainder slice ends at the wrong element.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, SLICE_COUNT_BEGIN		, 60, "The counted slice starts at the wrong element.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, SLICE_COUNT_END		, 61, "The counted slice ends at the wrong element.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, SLICE_END_SIZE		, 62, "The one-past-end slice is not empty.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, SLICE_END_BEGIN		, 63, "The one-past-end slice has the wrong begin boundary.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, SLICE_END_END			, 64, "The one-past-end slice has the wrong end boundary.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, SLICE_SELF_BEGIN		, 65, "The self-replacing slice has the wrong begin boundary.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, SLICE_SELF_END		, 66, "The self-replacing slice has the wrong end boundary.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, CONST_SLICE_BEGIN		, 67, "The const slice has the wrong begin boundary.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, CONST_SLICE_END		, 68, "The const slice has the wrong end boundary.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, SLICE_FAILURE_SIZE	, 69, "A failed array_static<>::slice() changed its output size.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, FIND_THREE			, 70, "array_static<> find() returned the wrong index for value 3.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, FIND_OFFSET_THREE	, 71, "array_static<> find() returned the wrong index from offset 3.");
GDEFINE_ENUM_VALUED(ARRAY_STATIC_TEST_RESULT, FIND_END_OFFSET		, 72, "array_static<> find() accepted a search starting at its end.");

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
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_STORAGE_LAYOUT, szof(data) != szof(T) * 5U
		, "object bytes:%u, expected:%u.", (::llc::u2_t)szof(data), (::llc::u2_t)(szof(T) * 5U));
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_STORAGE_BEGIN, data.Storage != data.begin()
		, "storage:%p, begin:%p.", data.Storage, data.begin());
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_MEMBER_SIZE, data.size() != 5
		, "mutable size:%u, expected:5.", data.size());
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_CONST_MEMBER_SIZE, constData.size() != 5
		, "const size:%u, expected:5.", constData.size());
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_FREE_SIZE, ::llc::size(data) != 5
		, "free size mismatch. actual:%u, expected:5."
		, ::llc::size(data)
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_BYTE_COUNT, data.byte_count() != szof(T) * 5U
		, "member byte count:%u, expected:%u.", data.byte_count(), (::llc::u2_t)(szof(T) * 5U));
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_FREE_BYTE_COUNT, ::llc::byte_count(data) != szof(T) * 5U
		, "free byte count:%u, expected:%u.", ::llc::byte_count(data), (::llc::u2_t)(szof(T) * 5U));
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_BIT_COUNT, data.bit_count() != szof(T) * 5U * 8U
		, "bit count mismatch. actual:%u, expected:%u."
		, data.bit_count(), (::llc::u2_t)(szof(T) * 5U * 8U)
		);
	// The one-past expression is the expected boundary under test for array_static::end().
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_BOUNDARIES, data.begin() != data.Storage
		, "mutable begin:%p, expected:%p.", data.begin(), data.Storage);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_END_BOUNDARY, data.end() != data.Storage + 5
		, "mutable end:%p, expected:%p.", data.end(), data.Storage + 5);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_CONST_BEGIN_BOUNDARY, constData.begin() != data.Storage
		, "const begin:%p, expected:%p.", constData.begin(), data.Storage);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_CONST_END_BOUNDARY, constData.end() != data.Storage + 5
		, "const end:%p, expected:%p.", constData.end(), data.Storage + 5);

	::llc::view<T> mutableView = data;
	::llc::view<cnst T> constView = constData;
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_MUTABLE_VIEW, mutableView.begin() != data.begin()
		, "mutable view begin:%p, expected:%p.", mutableView.begin(), data.begin());
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_MUTABLE_VIEW_SIZE, mutableView.size() != data.size()
		, "mutable view size:%u, expected:%u.", mutableView.size(), data.size());
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_CONST_VIEW, constView.begin() != constData.begin()
		, "const view begin:%p, expected:%p.", constView.begin(), constData.begin());
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_CONST_VIEW_SIZE, constView.size() != constData.size()
		, "const view size:%u, expected:%u.", constView.size(), constData.size());
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
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SUBSCRIPT_READ, data[2] != T(3)
		, "mutable[2]:%" LLC_FMT_S3 ", expected:3.", (::llc::s3_t)data[2]);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_CONST_SUBSCRIPT_READ, constData[3] != T(4)
		, "const[3]:%" LLC_FMT_S3 ", expected:4.", (::llc::s3_t)constData[3]);
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
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_REFLEXIVE_EQUALITY, !(first == first), "array did not equal itself.");
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_EQUALITY, !(first == equal), "equal arrays compared unequal.");
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_EQUAL_INEQUALITY, first != equal, "equal arrays compared different.");
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_INEQUALITY, first == different, "different arrays compared equal.");
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_DIFFERENT_INEQUALITY, !(first != different), "different arrays compared equal with !=.");
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
		, false == (::std::is_same_v<decltype(mutableBytes), ::llc::view<::llc::u0_t>>)
		, "mutable byte view type mismatch."
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_MUTABLE_CHAR_TYPE
		, false == (::std::is_same_v<decltype(mutableChars), ::llc::view<::llc::sc_t>>)
		, "mutable character view type mismatch."
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_CONST_BYTE_TYPE
		, false == (::std::is_same_v<decltype(constBytes), ::llc::view<::llc::u0_c>>)
		, "const byte view type mismatch."
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_CONST_BYTE_ALIAS_TYPE
		, false == (::std::is_same_v<decltype(constByteAlias), ::llc::view<::llc::u0_c>>)
		, "const byte alias type mismatch."
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_CONST_CHAR_TYPE
		, false == (::std::is_same_v<decltype(constChars), ::llc::view<::llc::sc_c>>)
		, "const character view type mismatch."
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_MUTABLE_BYTE_VIEW, (::llc::uP_t)mutableBytes.begin() != (::llc::uP_t)data.begin()
		, "mutable byte begin:%p, expected:%p.", mutableBytes.begin(), data.begin());
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_MUTABLE_BYTE_SIZE, mutableBytes.size() != data.byte_count()
		, "mutable byte size:%u, expected:%u.", mutableBytes.size(), data.byte_count());
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_CONST_BYTE_VIEW, (::llc::uP_t)constBytes.begin() != (::llc::uP_t)constData.begin()
		, "const byte begin:%p, expected:%p.", constBytes.begin(), constData.begin());
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_CONST_BYTE_SIZE, constBytes.size() != constData.byte_count()
		, "const byte size:%u, expected:%u.", constBytes.size(), constData.byte_count());
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_CONST_BYTE_ALIAS, constByteAlias.begin() != constBytes.begin()
		, "const byte alias begin:%p, expected:%p.", constByteAlias.begin(), constBytes.begin());
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_CONST_BYTE_ALIAS_SIZE, constByteAlias.size() != constBytes.size()
		, "const byte alias size:%u, expected:%u.", constByteAlias.size(), constBytes.size());
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_MUTABLE_CHAR_VIEW, (::llc::uP_t)mutableChars.begin() != (::llc::uP_t)data.begin()
		, "mutable character begin:%p, expected:%p.", mutableChars.begin(), data.begin());
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_MUTABLE_CHAR_SIZE, mutableChars.size() != data.byte_count()
		, "mutable character size:%u, expected:%u.", mutableChars.size(), data.byte_count());
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_CONST_CHAR_VIEW, (::llc::uP_t)constChars.begin() != (::llc::uP_t)constData.begin()
		, "const character begin:%p, expected:%p.", constChars.begin(), constData.begin());
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_CONST_CHAR_SIZE, constChars.size() != constData.byte_count()
		, "const character size:%u, expected:%u.", constChars.size(), constData.byte_count());
	cnst ::llc::u0_t replacement = (::llc::u0_t)(mutableBytes[0] ^ 0x5AU);
	mutableBytes[0] = replacement;
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_REPRESENTATION_WRITE, constData.cu8()[0] != replacement
		, "byte representation write mismatch. storage byte:%u, expected:%u."
		, constData.cu8()[0], replacement
		);
	cnst ::llc::sc_t charReplacement = (::llc::sc_t)(mutableChars[1] ^ 0x35);
	mutableChars[1] = charReplacement;
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_CHAR_REPRESENTATION_WRITE, constData.cc()[1] != charReplacement
		, "character representation write mismatch. storage byte:%i, expected:%i."
		, constData.cc()[1], charReplacement
		);
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testStaticSlice(ATestError & errors) {
	::llc::array_static<T, 5> data = {T(1), T(2), T(3), T(4), T(5)};
	::llc::view<T> output;
	::llc::err_t result = data.slice(output, 0);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_FULL, result != 5, "full slice result:%i, expected:5.", result);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_FULL_BEGIN, output.begin() != data.begin()
		, "full slice begin:%p, expected:%p.", output.begin(), data.begin());
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_FULL_END, output.end() != data.end()
		, "full slice end:%p, expected:%p.", output.end(), data.end());
	result = data.slice(output, 2);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_REMAINDER, result != 3, "remainder slice result:%i, expected:3.", result);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_REMAINDER_BEGIN, output.begin() != &data[2]
		, "remainder slice begin:%p, expected:%p.", output.begin(), &data[2]);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_REMAINDER_END, output.end() != data.end()
		, "remainder slice end:%p, expected:%p.", output.end(), data.end());
	result = data.slice(output, 1, 2);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_COUNT, result != 2, "counted slice result:%i, expected:2.", result);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_COUNT_BEGIN, output.begin() != &data[1]
		, "counted slice begin:%p, expected:%p.", output.begin(), &data[1]);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_COUNT_END, output.end() != &data[3]
		, "counted slice end:%p, expected:%p.", output.end(), &data[3]);
	result = data.slice(output, 5);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_END, result, "end slice result:%i, expected:0.", result);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_END_SIZE, output.size(), "end slice size:%u, expected:0.", output.size());
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_END_BEGIN, output.begin() != data.end()
		, "end slice begin:%p, expected:%p.", output.begin(), data.end());
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_END_END, output.end() != data.end()
		, "end slice end:%p, expected:%p.", output.end(), data.end());
	::llc::view<T> self = data;
	result = data.slice(self, 2, 2);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_SELF, result != 2, "self slice result:%i, expected:2.", result);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_SELF_BEGIN, self.begin() != &data[2]
		, "self slice begin:%p, expected:%p.", self.begin(), &data[2]);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_SELF_END, self.end() != &data[4]
		, "self slice end:%p, expected:%p.", self.end(), &data[4]);

	cnst ::llc::array_static<T, 5> & constData = data;
	::llc::view<cnst T> constOutput;
	result = constData.slice(constOutput, 1, 3);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_CONST_SLICE, result != 3, "const slice result:%i, expected:3.", result);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_CONST_SLICE_BEGIN, constOutput.begin() != &constData[1]
		, "const slice begin:%p, expected:%p.", constOutput.begin(), &constData[1]);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_CONST_SLICE_END, constOutput.end() != &constData[4]
		, "const slice end:%p, expected:%p.", constOutput.end(), &constData[4]);

	T guard[] = {T(8), T(9)};
	output = {guard, 2};
	result = staticSliceExpectedFailure(data, output, 6);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_INVALID_OFFSET, false == ::llc::failed(result)
		, "slice accepted offset 6 for extent 5. result:%i."
		, result
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_FAILURE_STATE, output.begin() != guard
		, "failed offset slice begin:%p, expected:%p.", output.begin(), guard);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_FAILURE_SIZE, output.size() != 2
		, "failed offset slice size:%u, expected:2.", output.size());
	result = staticSliceExpectedFailure(data, output, 4, 2);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_INVALID_COUNT, false == ::llc::failed(result)
		, "slice accepted count 2 after offset 4. result:%i."
		, result
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_FAILURE_STATE, output.begin() != guard
		, "failed count slice begin:%p, expected:%p.", output.begin(), guard);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_FAILURE_SIZE, output.size() != 2
		, "failed count slice size:%u, expected:2.", output.size());
	constOutput = {guard, 2};
	result = staticSliceExpectedFailure(constData, constOutput, 6);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_INVALID_OFFSET, false == ::llc::failed(result)
		, "const slice accepted offset 6 for extent 5. result:%i."
		, result
		);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_FAILURE_STATE, constOutput.begin() != guard
		, "failed const slice begin:%p, expected:%p.", constOutput.begin(), guard);
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_SLICE_FAILURE_SIZE, constOutput.size() != 2
		, "failed const slice size:%u, expected:2.", constOutput.size());
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t testStaticFind(ATestError & errors) {
	cnst ::llc::array_static<T, 5> data = {T(2), T(3), T(2), T(4), T(2)};
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_FIND_FIRST, ::llc::find(T(2), data) != 0
		, "first value 2 index:%i, expected:0.", ::llc::find(T(2), data));
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_FIND_THREE, ::llc::find(T(3), data) != 1
		, "first value 3 index:%i, expected:1.", ::llc::find(T(3), data));
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_FIND_OFFSET, ::llc::find(T(2), data, 1) != 2
		, "value 2 from offset 1:%i, expected:2.", ::llc::find(T(2), data, 1));
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_FIND_OFFSET_THREE, ::llc::find(T(2), data, 3) != 4
		, "value 2 from offset 3:%i, expected:4.", ::llc::find(T(2), data, 3));
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_FIND_NOT_FOUND, ::llc::find(T(9), data) != -1
		, "absent value 9 index:%i, expected:-1.", ::llc::find(T(9), data));
	LLC_TEST_CHECK(errors, ARRAY_STATIC_TEST_RESULT_FIND_END_OFFSET, ::llc::find(T(2), data, 5) != -1
		, "value 2 from end offset:%i, expected:-1.", ::llc::find(T(2), data, 5));
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
