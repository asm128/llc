#include "llc_view_bit.h"
#include "llc_enum.h"

static_assert(3 == ::llc::bit_offset_field_size<::llc::u0_t>(), "u8 offset width");
static_assert(4 == ::llc::bit_offset_field_size<::llc::u1_t>(), "u16 offset width");
static_assert(5 == ::llc::bit_offset_field_size<::llc::u2_t>(), "u32 offset width");
static_assert(6 == ::llc::bit_offset_field_size<::llc::u3_t>(), "u64 offset width");

GDEFINE_ENUM_TYPE(BIT_VIEW_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, OK						,  0, "All bit-view tests passed.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, PARTIAL_ITERATOR_POSITION	,  1, "A 10-bit iterator reported the wrong zero-based bit index or logical limit.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, PARTIAL_END_POSITION		,  2, "A 10-bit view produced the wrong logical end or one-past-storage position.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, PARTIAL_DECREMENT			,  3, "Decrementing the end of a 10-bit view did not select bit index 9.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, SIX_BIT_END_POSITION		,  4, "A Base64-style 6-bit view did not stop at bit offset 6 of its first element.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, EMPTY_DEFAULT_POSITION	,  5, "A default empty view did not produce equal iterators with null position pointers.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, EMPTY_DATA_POSITION		,  6, "A zero-length view with backing data did not normalize to equal null iterators.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, ITERATOR_EQUALITY			,  7, "Iterators at different bit positions compared equal because equality was not positional.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, PROXY_WRITE				,  8, "Assigning through a mutable bit proxy did not update the selected backing bit.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, CONST_ITERATION			,  9, "Const iteration did not visit exactly the logical bit count.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, WIDTH_ITERATOR_POSITION	, 10, "An iterator using a derived offset-field width reported the wrong index or limit.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, WIDTH_END_POSITION		, 11, "A partial view using a derived offset-field width produced the wrong end position.");
GDEFINE_ENUM_VALUED(BIT_VIEW_TEST_RESULT, WIDTH_DECREMENT			, 12, "Decrementing a partial end failed for one of the supported backing integer widths.");

tplt<tpnm T>
BIT_VIEW_TEST_RESULT testPartialWidth() {
	T					data[2]		= {};
	::llc::u2_c			bitCount	= szof(T) * 8 + 3;
	::llc::view_bit<T>	bits		{data, bitCount};
	auto				it			= bits.begin();
	auto				end			= bits.end();
	{
		::llc::u2_t count = 0;
		for(; it != end; ++it, ++count)
			if_true_vef(BIT_VIEW_TEST_RESULT_WIDTH_ITERATOR_POSITION, it.Index() != count || it.Limit() != bitCount
				, "%u-bit iterator position mismatch. index:%u, expected:%u, limit:%u, expected limit:%u."
				, ::llc::u2_t(szof(T) * 8), it.Index(), count, it.Limit(), bitCount
				);
		if_true_vef(BIT_VIEW_TEST_RESULT_WIDTH_END_POSITION, count != bitCount || end.Index() != bitCount || end.Element != data + 1 || end.Offset != 3 || end.End != data + 2
			, "%u-bit end position mismatch. count:%u, index:%u, element:%p, expected element:%p, offset:%u, storage end:%p, expected storage end:%p."
			, ::llc::u2_t(szof(T) * 8), count, end.Index(), (void*)end.Element, (void*)(data + 1), ::llc::u2_t(end.Offset), (void*)end.End, (void*)(data + 2)
			);
	}
	if_true_vef(BIT_VIEW_TEST_RESULT_WIDTH_DECREMENT, (--end).Index() != bitCount - 1
		, "%u-bit end decrement mismatch. index:%u, expected:%u."
		, ::llc::u2_t(szof(T) * 8), end.Index(), bitCount - 1
		);
	return BIT_VIEW_TEST_RESULT_OK;
}

tplt<tpnm _dataValues, size_t _dataSize>
BIT_VIEW_TEST_RESULT testPartial(_dataValues (&data)[_dataSize]) {
	stxp ::llc::u2_t ELEMENT_BITS			= szof(_dataValues) * 8;
	stxp ::llc::u2_t BIT_COUNT				= 10;
	static_assert(_dataSize * ELEMENT_BITS >= BIT_COUNT, "10-bit view exceeds its backing storage.");

	_dataValues		* expectedElement		= data + BIT_COUNT / ELEMENT_BITS;
	_dataValues		* expectedEnd			= data + (BIT_COUNT + ELEMENT_BITS - 1) / ELEMENT_BITS;
	::llc::u2_c		expectedOffset			= BIT_COUNT % ELEMENT_BITS;

	::llc::view_bit<_dataValues> partial	{data, BIT_COUNT};
	auto it = partial.begin();
	auto end = partial.end();
	{
		::llc::u2_t count = 0;
		for(; it != end; ++it, ++count)
			if_true_vef(BIT_VIEW_TEST_RESULT_PARTIAL_ITERATOR_POSITION, it.Index() != count || it.Limit() != BIT_COUNT
				, "%u-bit backing type, 10-bit iterator position mismatch. index:%u, expected:%u, limit:%u, expected limit:%u."
				, ELEMENT_BITS, it.Index(), count, it.Limit(), BIT_COUNT
				);
		if_true_vef(BIT_VIEW_TEST_RESULT_PARTIAL_END_POSITION, count != BIT_COUNT || end.Index() != BIT_COUNT || end.Element != expectedElement || end.Offset != expectedOffset || end.End != expectedEnd
			, "%u-bit backing type, 10-bit end position mismatch. count:%u, index:%u, element:%p, expected element:%p, offset:%u, expected offset:%u, storage end:%p, expected storage end:%p."
			, ELEMENT_BITS, count, end.Index(), (void*)end.Element, (void*)expectedElement, ::llc::u2_t(end.Offset), expectedOffset, (void*)end.End, (void*)expectedEnd
			);
	}
	if_true_vef(BIT_VIEW_TEST_RESULT_PARTIAL_DECREMENT, (--end).Index() != BIT_COUNT - 1
		, "%u-bit backing type, 10-bit end decrement mismatch. index:%u, expected:%u."
		, ELEMENT_BITS, end.Index(), BIT_COUNT - 1
		);
	return BIT_VIEW_TEST_RESULT_OK;
}

tplt<tpnm _dataValues, size_t _dataSize>
BIT_VIEW_TEST_RESULT testSixBits(_dataValues (&data)[_dataSize]) {
	stxp ::llc::u2_t ELEMENT_BITS = szof(_dataValues) * 8;
	::llc::view_bit<_dataValues> sixBits{data, 6};
	auto it = sixBits.begin();
	auto end = sixBits.end();
	::llc::u2_t count = 0;
	for(; it != end; ++it, ++count) {}
	if_true_vef(BIT_VIEW_TEST_RESULT_SIX_BIT_END_POSITION, count != 6 || end.Index() != 6 || end.Element != data || end.Offset != 6 || end.End != data + 1
		, "%u-bit backing type, 6-bit end position mismatch. count:%u, index:%u, element:%p, expected element:%p, offset:%u, storage end:%p, expected storage end:%p."
		, ELEMENT_BITS, count, end.Index(), (void*)end.Element, (void*)data, ::llc::u2_t(end.Offset), (void*)end.End, (void*)(data + 1)
		);
	return BIT_VIEW_TEST_RESULT_OK;
}

tplt<tpnm _dataValues, size_t _dataSize>
BIT_VIEW_TEST_RESULT testEmpty(_dataValues (&data)[_dataSize]) {
	{
		::llc::view_bit<_dataValues> empty;
		cnst auto begin = empty.begin();
		cnst auto end = empty.end();
		if_true_vef(BIT_VIEW_TEST_RESULT_EMPTY_DEFAULT_POSITION, begin != end || begin.Begin || end.End
			, "%u-bit backing type, default empty view is not represented by equal null iterators. begin element:%p, end element:%p, begin base:%p, storage end:%p."
			, ::llc::u2_t(szof(_dataValues) * 8), (cnst void*)begin.Element, (cnst void*)end.Element, (cnst void*)begin.Begin, (cnst void*)end.End
			);
	}
	{
		::llc::view_bit<_dataValues> empty{data, 0};
		cnst auto begin = empty.begin();
		cnst auto end = empty.end();
		if_true_vef(BIT_VIEW_TEST_RESULT_EMPTY_DATA_POSITION, begin != end || begin.Begin || end.End
			, "%u-bit backing type, data-backed empty view is not represented by equal null iterators. begin element:%p, end element:%p, begin base:%p, storage end:%p."
			, ::llc::u2_t(szof(_dataValues) * 8), (cnst void*)begin.Element, (cnst void*)end.Element, (cnst void*)begin.Begin, (cnst void*)end.End
			);
	}
	return BIT_VIEW_TEST_RESULT_OK;
}

tplt<tpnm T>
BIT_VIEW_TEST_RESULT testIteratorPosition() {
	T equalBits = 0;
	::llc::view_bit<T> positions{&equalBits, 2};
	auto first = positions.begin();
	auto second = first;
	++second;
	if_true_vef(BIT_VIEW_TEST_RESULT_ITERATOR_EQUALITY, first == second
		, "%u-bit backing type, iterators at different offsets compare equal. first index:%u, second index:%u."
		, ::llc::u2_t(szof(T) * 8), first.Index(), second.Index()
		);
	*first = true;
	if_true_vef(BIT_VIEW_TEST_RESULT_PROXY_WRITE, !positions[0]
		, "%u-bit backing type, bit proxy write did not update its backing element. element:%" LLC_FMT_U3 ", bit index:0."
		, ::llc::u2_t(szof(T) * 8), ::llc::u3_t(equalBits)
		);
	return BIT_VIEW_TEST_RESULT_OK;
}

tplt<tpnm _dataValues, size_t _dataSize>
BIT_VIEW_TEST_RESULT testConstIteration(_dataValues (&data)[_dataSize]) {
	cnst ::llc::view_bit<_dataValues> readOnly{data, 10};
	auto read = readOnly.begin();
	::llc::u2_t count = 0;
	for(; read != readOnly.end(); ++read, ++count)
		(void)(bool)*read;
	if_true_vef(BIT_VIEW_TEST_RESULT_CONST_ITERATION, count != 10
		, "%u-bit backing type, const iteration stopped at the wrong position. count:%u, expected:10."
		, ::llc::u2_t(szof(_dataValues) * 8), count
		);
	return BIT_VIEW_TEST_RESULT_OK;
}

tplt<tpnm T>
BIT_VIEW_TEST_RESULT testType() {
	T data[2] = {(T)0xA5U, (T)0x02U};
	BIT_VIEW_TEST_RESULT result = BIT_VIEW_TEST_RESULT_OK;
	if(BIT_VIEW_TEST_RESULT_OK != (result = testPartial			(data))) return result;
	if(BIT_VIEW_TEST_RESULT_OK != (result = testSixBits			(data))) return result;
	if(BIT_VIEW_TEST_RESULT_OK != (result = testEmpty			(data))) return result;
	if(BIT_VIEW_TEST_RESULT_OK != (result = testIteratorPosition<T>	())) return result;
	if(BIT_VIEW_TEST_RESULT_OK != (result = testConstIteration	(data))) return result;
	return testPartialWidth<T>();
}

int testViewBit() {
	BIT_VIEW_TEST_RESULT testResult = BIT_VIEW_TEST_RESULT_OK;
	if_true_vef(testResult, testResult = testType<::llc::u0_t>(), " 8-bit suite failed. %s: %s", ::llc::get_value_namep(testResult), ::llc::get_value_descp(testResult)) else always_printf(" 8-bit suite OK.");
	if_true_vef(testResult, testResult = testType<::llc::u1_t>(), "16-bit suite failed. %s: %s", ::llc::get_value_namep(testResult), ::llc::get_value_descp(testResult)) else always_printf("16-bit suite OK.");
	if_true_vef(testResult, testResult = testType<::llc::u2_t>(), "32-bit suite failed. %s: %s", ::llc::get_value_namep(testResult), ::llc::get_value_descp(testResult)) else always_printf("32-bit suite OK.");
	if_true_vef(testResult, testResult = testType<::llc::u3_t>(), "64-bit suite failed. %s: %s", ::llc::get_value_namep(testResult), ::llc::get_value_descp(testResult)) else always_printf("64-bit suite OK.");
	return BIT_VIEW_TEST_RESULT_OK;
}
